# Makefile for building minimal C example

# CAVEAT: this makefile is messy. do not be alarmed.

#FIXME:  check IOTIVITY_HOME

# STAGE: debug or release

CFLAGS = -g -std=c11 -m64 -fPIC -Wall -Wextra -DTB_LOG $(SYSCFLAGS) $(MYCFLAGS) $(CPATH)


# if gcc add  -pthread?

CPATH    = $(IOT_HEADERS)
LDFLAGS= $(SYSLDFLAGS) $(MYLDFLAGS) $(IOT_LIBS)
LIBS= -lm $(SYSLIBS) $(MYLIBS)

AR= ar rcu
MKDIR= mkdir -p
RANLIB= ranlib
RM= rm -f

SYSCFLAGS=
SYSLDFLAGS=
SYSLIBS=

MYCFLAGS=
MYLDFLAGS=
MYLIBS=
MYOBJS=

OCCLFLAGS=-DROUTING_EP   # ENDPOINT
#OCCLFLAGS=-DROUTING_GATEWAY

# -DIP_ADAPTER
#-DNO_EDR_ADAPTER -DNO_LE_ADAPTER
# -DHAVE_GDBUS -DNDEBUG
#

# -DWITH_BWT -DTB_LOG

# -D_DARWIN_C_SOURCE -DWITH_POSIX -D__darwin__

#-DHAVE_PTHREADS
 # -DHAVE_GETTIMEOFDAY

# ifeq ($(SECURED), 1)
#     IOTLIBS += "-ltinydtls -locprovision -locpmapi"
# endif

IOT_HEADERS= -I. \
	-I./src \
	-I${INSTALL_SYSROOT}/include \
	-I${IOTIVITY_HOME}/resource/csdk/cbor \
	-I${IOTIVITY_HOME}/resource/csdk/connectivity/api \
	-I${IOTIVITY_HOME}/resource/c_common \
	-I${IOTIVITY_HOME}/resource/c_common \
	-I${IOTIVITY_HOME}/resource/c_common \
	-I${IOTIVITY_HOME}/resource/csdk/stack \
	-I${IOTIVITY_HOME}/resource/csdk/logger \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include/internal \
	-I${IOTIVITY_HOME}/resource/csdk/security \
	-I${IOTIVITY_HOME}/resource/csdk/connectivity/lib/libcoap-4.1.1

	# -I${IOTIVITY_HOME}/extlibs/boost/boost_1_58_0 \
	# -I${IOTIVITY_HOME}/build_common/android/compatibility \

	# -I${IOTIVITY_HOME}/resource/../tinycbor/tinycbor/src \
I#	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include/oxm \
#	# -I${IOTIVITY_HOME}/resource/c_common/oic_string/include \
#	# -I${IOTIVITY_HOME}/resource/c_common/oic_malloc/include \


IOT_LIBPATH=${INSTALL_SYSROOT}/lib

# IOT_LIBS = $(IOT_LIBPATH)/liboctbstack.a \
# 	$(IOT_LIBPATH)/libc_common.a \
# 	$(IOT_LIBPATH)/libcoap.a \
# 	$(IOT_LIBPATH)/libocsrm.a \
# 	$(IOT_LIBPATH)/libroutingmanager.a \
# 	$(IOT_LIBPATH)/libconnectivity_abstraction.a \
# 	-loctbstack \
# 	-lpthread

IOT_LIBS = \
	-lm \
	-loctbstack \
	-lc_common \
	-llogger \
	-lconnectivity_abstraction \
	-lroutingmanager \
	-lcoap \
	-lpthread

# if SECURED == 1
	# -locsrm \

	# -loc_logger \
	# -loc \

# $(IOT_LIBPATH)/libconnectivity_abstraction.a \


all: server client

server:  server.o temperature.o utils.o
	$(CC) -o server $(CFLAGS) -L. -L$(IOT_LIBPATH) temperature.o utils.o server.o $(LDFLAGS)

server.o:  server.c server.h
	$(CC) $(CFLAGS) $(CPATH) -c server.c

client:  client.o utils.o
	$(CC) -o client $(CFLAGS) -L. -L$(IOT_LIBPATH) utils.o client.o $(LDFLAGS)

client.o:  client.c client.h
	$(CC) $(OCCLFLAGS) $(CFLAGS) $(CPATH) -c client.c

clean:
	$(RM) server client *.o

.PHONY:  o a clean
