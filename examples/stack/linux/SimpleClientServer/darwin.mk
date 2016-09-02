CC=g++
CXX=g++

INC=-I$(IOTIVITY_HOME)/resource/include \
	-I$(IOTIVITY_HOME)/resource/c_common \
	-I$(IOTIVITY_HOME)/resource/c_common/oic_malloc/include \
	-I$(IOTIVITY_HOME)/resource/c_common/oic_string/include \
	-I$(IOTIVITY_HOME)/resource/c_common/oic_malloc/include \
	-I$(IOTIVITY_HOME)/resource/csdk/cbor \
	-I$(IOTIVITY_HOME)/resource/csdk/cjson \
	-I$(IOTIVITY_HOME)/resource/csdk/stack/include \
	-I$(IOTIVITY_HOME)/resource/csdk/security/provisioning/include \
	-I$(IOTIVITY_HOME)/resource/csdk/security/provisioning/include/internal \
	-I$(IOTIVITY_HOME)/resource/c_common/ocrandom/include \
	-I$(IOTIVITY_HOME)/resource/csdk/logger/include \
	-I$(IOTIVITY_HOME)/resource/oc_logger/include

CPPFLAGS= -g  -m64 -fPIC -DHAVE_PTHREAD_H -DHAVE_UNISTD_H -DWITH_POSIX -D__darwin__ -DTB_LOG

CXXFLAGS = -std=c++11 -Wall -Wextra -Wno-unused -Os -pthread $(INC)

LDFLAGS=-L$(IOTIVITY_LIBPATH)

# OSX must use either -lc++ or -lstdc++, -std=libstc++ not enough
LDLIBS=-lcoap -locsrm -lconnectivity_abstraction -loc_logger -llogger -loctbstack -loc -lpthread -lstdc++

ALL=occlient occlientbasicops occlientslow ocserver ocserverbasicops ocserverslow

all:	$(ALL)

$(ALL): common.o

occlientbasicops:  common.o

occlient:  common.o

ocserver:  common.o

ocserverbasicops:  common.o

clean:
	rm -f *.o $(ALL)
