# Makefile for building basic C example with libmraa

# CAVEAT: this makefile is messy. do not be alarmed.

#FIXME:  check IOTIVITY_HOME

#HOST=darwin
#ARCH=x86_64   # darwin

HOST=linux
# edison:
ARCH=x86

# STAGE: debug or release
STAGE=release


# clang:  -m64 -Werror=incompatible-pointer-types

CPPFLAGS = -g -fPIC -Wall -Werror=implicit-function-declaration -Wextra -DTB_LOG $(SYSCFLAGS) $(MYCFLAGS) $(CPATH)

CFLAGS   = -std=c11 $(CPPFLAGS)
CXXFLAGS   = -std=c++0x -stdlib=libstdc++ $(CPPFLAGS)
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

# if target_arch in ['x86']:
# 	env.AppendUnique(CCFLAGS = ['-m32'])
# 	env.AppendUnique(LINKFLAGS = ['-m32'])
# elif target_arch in ['x86_64']:
# 	env.AppendUnique(CCFLAGS = ['-m64'])
# 	env.AppendUnique(LINKFLAGS = ['-m64'])
# elif target_arch in ['arm'] or target_arch.find('v5') > 0:
# 	env.AppendUnique(CPPFLAGS = ['-march=armv5te'])
# elif target_arch.find('v7a-hard') > 0:
# 	env.AppendUnique(CPPFLAGS = ['-march=armv7-a'])
# 	env.AppendUnique(CPPFLAGS = ['-mfloat-abi=hard'])
# 	env.AppendUnique(CCFLAGS = ['-mfloat-abi=hard'])
# 	env.AppendUnique(LINKFLAGS = ['-mfloat-abi=hard'])
# elif target_arch.find('v7a') > 0:
# 	env.AppendUnique(CPPFLAGS = ['-march=armv7-a'])
# elif target_arch.find('arm64') >= 0:
# 	env.AppendUnique(CPPFLAGS = ['-march=armv8-a'])


IOT_HEADERS= -I. \
	-I./src \
	-I${IOTIVITY_HOME}/resource/csdk/cbor \
	-I${IOTIVITY_HOME}/resource/csdk/connectivity/api \
	-I${IOTIVITY_HOME}/resource/include \
	-I${IOTIVITY_HOME}/resource/c_common \
	-I${IOTIVITY_HOME}/resource/c_common/oic_malloc/include \
	-I${IOTIVITY_HOME}/resource/c_common/oic_string/include \
	-I${IOTIVITY_HOME}/resource/csdk/stack/include \
	-I${IOTIVITY_HOME}/resource/csdk/ocsocket/include \
	-I${IOTIVITY_HOME}/resource/csdk/logger/include \
	-I${IOTIVITY_HOME}/resource/../extlibs/boost/boost_1_58_0 \
	-I${IOTIVITY_HOME}/resource/../build_common/android/compatibility \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include/internal \
	-I${IOTIVITY_HOME}/resource/csdk/security/include \
	-I${IOTIVITY_HOME}/resource/csdk/connectivity/lib/libcoap-4.1.1 \
	-I/usr/include/mraa

#	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include/oxm \
#	# -I${IOTIVITY_HOME}/resource/c_common/oic_string/include \
#	# -I${IOTIVITY_HOME}/resource/c_common/oic_malloc/include \


IOT_LIBPATH=$(IOTIVITY_HOME)/out/$(HOST)/$(ARCH)/$(STAGE)

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
	-locsrm \
	-lc_common \
	-llogger \
	-loc \
	-lconnectivity_abstraction \
	-lroutingmanager \
	-lcoap \
	-lpthread \
	-lmraa \

server:  server.o button.o led.o temperature.o
	$(CC) -o server $(CFLAGS) -L. -L$(IOT_LIBPATH) button.o led.o temperature.o server.o $(LDFLAGS)

server.o:  server.c server.h
	$(CC) $(CFLAGS) $(CPATH) -c server.c

client:  client.o utils.o
	$(CC) -o client $(CFLAGS) -L. -L$(IOT_LIBPATH) utils.o client.o $(LDFLAGS)

client.o:  client.c client.h
	$(CC) $(OCCLFLAGS) $(CFLAGS) $(CPATH) -c client.c

utils.o: utils.c
	$(CC) $(OCCLFLAGS) $(CFLAGS) $(CPATH) -c utils.c


clean:
	$(RM) server client *.o

.PHONY:  o a clean
