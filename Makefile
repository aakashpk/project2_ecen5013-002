
.PHONY: all build run clean test

include sources.mk

PLATFORM=HOST
TARGET=project2

CC=gcc
CSTD=gnu99

INCLUDE=-I./bbb/inc\
	-I./common/inc

CFLAGS=-std=$(CSTD)\
	-lpthread\
	-Wall\
	-lm\
	-g\
	-O0\
	-Wfatal-errors\
	-Werror

MOCKED_FUNCTIONS=read_reg_temp\
	read_reg_light_word

WRAP_FLAG=-Wl,--wrap=
WRAPPED_FUNCTIONS=$(addprefix $(WRAP_FLAG),$(MOCKED_FUNCTIONS))

TESTFLAGS=-lcmocka

#Type of connection local/remote for client executable
TYPE=LOCAL

#objest files for different scenarios
MAIN_OBJECT=$(MAIN_SOURCE:.c=.o)
COMMON_OBJECTS=$(COMMON_SOURCES:.c=.o)
BBB_OBJECTS=$(BBB_SOURCES:.c=.o)
HOST_OBJECTS=$(HOST_SOURCES:.c=.o)
CLIENT_OBJECTS=$(CLIENT_SOURCES:.c=.o)

TEST_TARGETS=$(TEST_SOURCES:.c=.test)
WRAPPED_TEST_TARGETS=$(WRAPPED_TEST_SOURCES:.c=.wrappedtest)

# Change compiler and files if cross compiling for BBB/BBG\
also removes the I2C sensor files\
when compiling for HOST
ifeq ($(PLATFORM),BBB)
CC=arm-linux-gnueabihf-gcc
OBJECTS= $(COMMON_OBJECTS) $(BBB_OBJECTS) $(MAIN_OBJECT)
CFLAGS+=-DBBB
else
OBJECTS=$(COMMON_OBJECTS) $(HOST_OBJECTS) $(MAIN_OBJECT)
endif


build: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(CFLAGS) $(INCLUDE)

#build and run , changes based on platform
run: build
ifeq ($(PLATFORM),BBB)
	scp $(TARGET) debian@192.168.7.2:/home/debian/proj2/$(TARGET)
else
	./$(TARGET) -f logfile.txt
endif

all: run

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE)

client:$(CLIENT_SOURCES)
	$(CC) -o $@ $(CLIENT_SOURCES) $(INCLUDE) -D$(TYPE)
ifeq ($(PLATFORM),BBB)
	scp $@ root@192.168.7.2:/home/proj1/$@
endif


#rule to run unit tests
test: $(TEST_TARGETS) $(WRAPPED_TEST_TARGETS)
	for x in $^; do echo --- running --- ./$$x; ./$$x; echo ---- done test group ----; done

# generates executable for each unit test file\
and runs them
%.wrappedtest:%.c $(BBB_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $< $(COMMON_OBJECTS) $(BBB_OBJECTS) -o $@ $(CFLAGS) $(INCLUDE) \
	$(TESTFLAGS) $(WRAPPED_FUNCTIONS)

#simple_test.test:sensor_test.c $(BBB_OBJECTS) $(COMMON_OBJECTS)

%.test:%.c $(BBB_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $< $(COMMON_OBJECTS) $(BBB_OBJECTS) -o $@ $(CFLAGS) $(INCLUDE) \
	$(TESTFLAGS)

clean:
	rm -f bbb/src/*.o bbb/test/*.o common/src/*.o
	rm -f $(TEST_TARGETS) $(TARGET) client
