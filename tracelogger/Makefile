CXX = g++
CXXFLAGS += -Wall -std=c++11 -g -O2

CXXFLAGS+= -D_DEBUG_RSLT_
CXXFLAGS+= -D_DEBUG_READ -I$(TOP)/include/disruptor

TOP = $(PWD)/..

LIBS += -lpthread 

SRCS = tracelogger.cpp $(TOP)/src/disruptor/shared_mem_manager.cpp $(TOP)/src/disruptor/ring_buffer_on_shmem.cpp
OBJECTS = tracelogger.o $(TOP)/src/disruptor/shared_mem_manager.o $(TOP)/src/disruptor/ring_buffer_on_shmem.o
TARGET = tracelogger.exe

$(TARGET) : $(OBJECTS) $(LINK) 
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) 

all: $(TARGET)

clean :
	rm -f $(OBJECTS) $(TARGET)

