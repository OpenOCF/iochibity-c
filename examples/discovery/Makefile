# Makefile for building minimal C example

# CAVEAT: this makefile is messy. do not be alarmed.

#FIXME:  check IOTIVITY_HOME

# STAGE: debug or release

#CFLAGS = -g -std=c11 -m64 -fPIC -Wall -Wextra -DTB_LOG $(SYSCFLAGS) $(MYCFLAGS) $(CPATH)

# if gcc add  -pthread?

LDFLAGS=$(SYSLDFLAGS) $(MYLDFLAGS)
LIBS=-lm $(BUILD_SYSROOT) $(MYLIBS)


AR= ar rcu
MKDIR= mkdir -p
RANLIB= ranlib
RM= rm -f

IOT_LIBPATH=$(INSTALL_SYSROOT)/lib

SYSCFLAGS=
SYSLDFLAGS=-L${IOT_LIBPATH}

# -dylib_file /usr/local/lib/liboctbstack.dylib:${BUILD_SYSROOT}/liboctbstack.dylib
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

# if target_os == 'linux':
#    shlibsuffix = '.so'
#    jnilibsuffix = '.so'
# elif target_os == 'darwin':
#    shlibsuffix = '.dylib'
#    jnilibsuffix = '.jnilib'
# elif target_os == 'win32':
#    shlibsuffix = '.dll'
#    jnilibsuffix = '.dll'

# if target_os in ['darwin', 'ios']:
# 	jni_env.AppendUnique(CPPDEFINES = ['_DARWIN_C_SOURCE'])

IOT_HEADERS= -I. \
	-I./src \
	-I${IOTIVITY_HOME}/resource/csdk/cbor \
	-I${IOTIVITY_HOME}/resource/csdk/connectivity/api \
	-I${IOTIVITY_HOME}/resource/c_common \
	-I${IOTIVITY_HOME}/resource/csdk/stack \
	-I${IOTIVITY_HOME}/resource/csdk/logger \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include/internal \
	-I${IOTIVITY_HOME}/resource/csdk/security/include \
	-I${IOTIVITY_HOME}/resource/csdk/connectivity/lib/libcoap-4.1.1

	# -I${IOTIVITY_HOME}/resource/include \
	# -I${IOTIVITY_HOME}/extlibs/boost/boost_1_58_0 \
	# -I${IOTIVITY_HOME}/resource/../build_common/android/compatibility \


CPPFLAGS+=${IOT_HEADERS}

	# -I${IOTIVITY_HOME}/resource/../tinycbor/tinycbor/src \
#	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include/oxm \
#	# -I${IOTIVITY_HOME}/resource/c_common/oic_string/include \
#	# -I${IOTIVITY_HOME}/resource/c_common/oic_malloc/include \


# IOT_LIBS = $(IOT_LIBPATH)/liboctbstack.a \
# 	$(IOT_LIBPATH)/libc_common.a \
# 	$(IOT_LIBPATH)/libcoap.a \
# 	$(IOT_LIBPATH)/libocsrm.a \
# 	$(IOT_LIBPATH)/libroutingmanager.a \
# 	$(IOT_LIBPATH)/libconnectivity_abstraction.a \
# 	-loctbstack \
# 	-lpthread

LDLIBS += \
	-lm \
	-loctbstack \
	-lc_common \
	-lconnectivity_abstraction \
	-lroutingmanager \
	-lcoap \
	-lpthread

	# -locsrm \
#FIXME: add tinydtls if SECURED=1
	# -ltinydtls

	# -llogger \
	# -loc_logger \
	# -loc \


# $(IOT_LIBPATH)/libconnectivity_abstraction.a \

all: client server

server:  server.o button.o led.o

#	$(CC) -o server $(CFLAGS) -L. -L$(IOT_LIBPATH) button.o led.o server.o $(LDFLAGS)

#server.o:  server.c server.h

#	$(CC) $(CFLAGS) $(CPATH) -c server.c

client:  client.o

clean:
	$(RM) server client *.o

.PHONY:  o a clean
