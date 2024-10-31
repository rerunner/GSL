
#include <iostream>
#include <thread>

#include "../include/disruptor/ring_buffer_on_shmem.hpp" 
#include "../include/disruptor/shared_mem_manager.hpp" 
#include "../include/disruptor/atomic_print.hpp"
#include "elapsed_time.hpp"

std::mutex AtomicPrint::lock_mutex_ ;

int LOOP_CNT ;

//Wait Strategy 
//SharedMemRingBuffer gSharedMemRingBuffer (YIELDING_WAIT); 
SharedMemRingBuffer gSharedMemRingBuffer (SLEEPING_WAIT); 
//SharedMemRingBuffer gSharedMemRingBuffer (BLOCKING_WAIT); 
        

///////////////////////////////////////////////////////////////////////////////
void TestFunc(int consumer_id)
{
    //1. register
    int64_t index_for_customer_use = -1;
    if(!gSharedMemRingBuffer.RegisterConsumer(consumer_id, & index_for_customer_use)) {
        return; //error
    }

    //2. run
    ElapsedTime elapsed;
    char    msg[2048];
    int64_t total_fetched = 0; 
    int64_t  my_index = index_for_customer_use ; 
    int64_t returned_index =-1;
    bool is_first=true;

    char  tmp_data[1024];
    for(int i=0; i < LOOP_CNT  ; i++) {
        if( total_fetched >= LOOP_CNT ) {
            break;
        }
        returned_index = gSharedMemRingBuffer.WaitFor(consumer_id, my_index);

        if(is_first) {
            is_first=false;
            elapsed.SetStartTime();
        }

        for(int64_t j = my_index; j <= returned_index; j++) {
            //batch job 
            size_t data_len =0;
            const char* data =  gSharedMemRingBuffer.GetData(j, & data_len );
            //const char*  SharedMemRingBuffer::GetData(int64_t nIndex, int* nOutLen)

            memset(tmp_data, 0x00, sizeof(tmp_data));
            strncpy(tmp_data, data, data_len);

            gSharedMemRingBuffer.CommitRead(consumer_id, j );

            std::cout << tmp_data;
            total_fetched++;

        } //for

        my_index = returned_index + 1; 
    }

    long long elapsed_micro= elapsed.SetEndTime(MICRO_SEC_RESOLUTION);
    snprintf(msg, sizeof(msg), "* consumer -> elapsed :%lld (micro sec) last data [%s] TPS %lld ", 
        elapsed_micro, tmp_data, (long long) (LOOP_CNT*1000000L)/elapsed_micro );
    {AtomicPrint atomicPrint(msg);}
}


///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    int consumer_id = 0; 
    LOOP_CNT = 10000;    //should be same as producer for TEST
    int MAX_RBUFFER_CAPACITY = 1024*8; //should be same as producer for TEST
    int MAX_RAW_MEM_BUFFER_SIZE = 1000000; //should be same as producer for TEST

    if(! gSharedMemRingBuffer.Init(123456,
                                   MAX_RBUFFER_CAPACITY, 
                                   923456,
                                   MAX_RAW_MEM_BUFFER_SIZE ) )
    { 
        std::cerr << "error" << '\n';
        return 1; 
    }

    TestFunc(consumer_id);

    return 0;
}

