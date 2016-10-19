
CXX=g++

INC=-I$(IOTIVITY_HOME)/resource/include \
	-I$(IOTIVITY_HOME)/resource/c_common \
	-I$(IOTIVITY_HOME)/resource/csdk/stack/include \
	-I$(IOTIVITY_HOME)/resource/csdk/security/provisioning/include \
	-I$(IOTIVITY_HOME)/resource/csdk/security/provisioning/include/internal \
	-I$(IOTIVITY_HOME)/resource/c_common/ocrandom/include \
	-I$(IOTIVITY_HOME)/resource/csdk/logger/include \
	-I$(IOTIVITY_HOME)/resource/oc_logger/include

CXXFLAGS = -g -std=c++11 -DWITH_POSIX -D__linux__ -m32 -fPIC -Wall -Wextra -Wno-unused -Os -pthread $(INC)

LDFLAGS=-L$(IOTIVITY_LIBPATH)

LDLIBS= -lstdc++ -lcoap -lconnectivity_abstraction -loc_logger -loctbstack -loc -lpthread

all:	roomserver roomclient

clean:
	rm *.o roomserver roomclient
