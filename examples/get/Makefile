# Makefile for building minimal C example

# CAVEAT: this makefile is messy. do not be alarmed.

#FIXME:  check IOTIVITY_HOME

# STAGE: debug or release

CFLAGS += -DTB_LOG -g
# -fPIC -Wall -Wextra

# if gcc add  -pthread?



CPPFLAGS += $(IOT_HEADERS)
LDFLAGS= $(SYSLDFLAGS) $(MYLDFLAGS)
LDLIBS= $(IOT_LIBS)
LIBS= -lm $(SYSLIBS) $(MYLIBS)

AR= ar rcu
MKDIR= mkdir -p
RANLIB= ranlib
RM= rm -f

SYSCFLAGS=
SYSLDFLAGS=
SYSLIBS=

MYCFLAGS=
MYLDFLAGS=${IOT_LIBPATH}
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
	-I${IOTIVITY_HOME}/resource/csdk/stack \
	-I${IOTIVITY_HOME}/resource/csdk/logger \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include/internal \
	-I${IOTIVITY_HOME}/resource/csdk/security \
	-I${IOTIVITY_HOME}/resource/csdk/connectivity/lib/libcoap-4.1.1 \
	-I${IOTIVITY_HOME}/extlibs # tinydtls debug.h

	# -I${IOTIVITY_HOME}/extlibs/boost/boost_1_58_0 \
	# -I${IOTIVITY_HOME}/build_common/android/compatibility \

	# -I${IOTIVITY_HOME}/resource/../tinycbor/tinycbor/src \
I#	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include/oxm \
#	# -I${IOTIVITY_HOME}/resource/c_common/oic_string/include \
#	# -I${IOTIVITY_HOME}/resource/c_common/oic_malloc/include \


IOT_LIBPATH=-L${INSTALL_SYSROOT}/lib \
	-L${BUILD_SYSROOT}/logger

# IOT_LIBS = $(IOT_LIBPATH)/liboctbstack.a \
# 	$(IOT_LIBPATH)/libc_common.a \
# 	$(IOT_LIBPATH)/libcoap.a \
# 	$(IOT_LIBPATH)/libocsrm.a \
# 	$(IOT_LIBPATH)/libroutingmanager.a \
# 	$(IOT_LIBPATH)/libconnectivity_abstraction.a \
# 	-loctbstack \
# 	-lpthread

IOT_LIBS = \
	-lchibity \
	-llogger \
	-lpthread

	# -lm \
	# -lc_common \
	# -lconnectivity_abstraction \
	# -lroutingmanager \
	# -lcoap \
# if SECURED == 1
	# -locsrm \

	# -loc_logger \
	# -loc \

# $(IOT_LIBPATH)/libconnectivity_abstraction.a \


all: get_server get_client

get_server:  get_server.o temperature.o utils.o

get_client:  get_client.o utils.o

clean:
	$(RM) get_server get_client *.o

.PHONY:  o a clean
