#MAIN_SOURCE=src/main.c
MAIN_SOURCE=src/main_tester.c

COMMON_SOURCES= \
			src/dirfile_writer.c \
			../common/src/data_gen.c \
			../common/src/data_output.c \
			../common/src/file_helper.c \
			../common/src/packet_parser.c \
			../common/src/packet_writer.c \
			../common/src/packet_data_type.c
#			src/socketserver.c \
#			src/packet_comms.c \

#BBB_SOURCES=src/tempsensor.c\

#HOST_SOURCES=src/dummydata.c

#WRAPPED_TEST_SOURCES:=test/sensor_test.c

#TEST_SOURCES=test/simple_test.c \
		   test/queue_test.c

#CLIENT_SOURCES=src/socketclient.c

MOCKED_FUNCTIONS=
#	test/bd_queue_test.c
