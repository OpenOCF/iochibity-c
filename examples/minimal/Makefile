# Makefile for building minimal C example

#FIXME:  check IOTIVITY_HOME

HOST=darwin
STAGE=release

# == CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT =======================

# Your platform. See PLATS for possible values.
PLAT= none

CPPFLAGS = -O2 -Wall -Wextra $(SYSCFLAGS) $(MYCFLAGS) $(CPATH)
CFLAGS   = -std=c11
CXXFLAGS = -std=c++11
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

# == END OF USER SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE =======

# IOTLIBPATH = [env.get('BUILD_DIR')])
# IOTRPATH = [env.get('BUILD_DIR')])

# IOTLIBS = -loc -loctbstack -loc_logger -lconnectivity_abstraction -lpthread

IOT_LIBPATH=$(IOTIVITY_HOME)/out/$(HOST)/x86_64/$(STAGE)

# IOT_LIBS = $(IOT_LIBPATH)/liboctbstack.a \
# 	$(IOT_LIBPATH)/libc_common.a \
# 	$(IOT_LIBPATH)/libcoap.a \
# 	$(IOT_LIBPATH)/libocsrm.a \
# 	$(IOT_LIBPATH)/libroutingmanager.a \
# 	$(IOT_LIBPATH)/libconnectivity_abstraction.a \
# 	-loctbstack \
# 	-lpthread

	# -loc \


	# $(IOT_LIBPATH)/liboc_logger.dylib \
	# $(IOT_LIBPATH)/liboc.dylib \

IOT_LIBS = \
	$(IOT_LIBPATH)/libconnectivity_abstraction.a \
	-loc \
	-loctbstack \
	-loc_logger \
	-lpthread

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

PLATS= aix bsd c89 freebsd generic linux darwin mingw posix solaris

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
	-I${IOTIVITY_HOME}/resource/csdk/connectivity/api \
	-I${IOTIVITY_HOME}/resource/include \
	-I${IOTIVITY_HOME}/resource/c_common \
	-I${IOTIVITY_HOME}/resource/c_common/oic_string/include \
	-I${IOTIVITY_HOME}/resource/c_common/oic_malloc/include \
	-I${IOTIVITY_HOME}/resource/csdk/stack/include \
	-I${IOTIVITY_HOME}/resource/csdk/ocsocket/include \
	-I${IOTIVITY_HOME}/resource/oc_logger/include \
	-I${IOTIVITY_HOME}/resource/csdk/logger/include \
	-I${IOTIVITY_HOME}/resource/../extlibs/boost/boost_1_58_0 \
	-I${IOTIVITY_HOME}/resource/../build_common/android/compatibility \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include/oxm \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include/internal \
	-I${IOTIVITY_HOME}/resource/csdk/security/include

# CORE_O= server.o

# # DEPS = server.h

# # $(ODIR)/%.o: %.c $(DEPS)
# # 	$(CC) -c -o $@ $< $(CFLAGS)

# # Targets start here.
# default: $(PLAT)

# BASE_O= $(CORE_O)

# all:	$(BASE_O)


# service: $(CORE_O)
# 	 g++ -o service
# 	-rpath $(IOT_LIBPATH) \
# 	-flat_namespace \
# 	$(IOT_LIBS) \
# 	-L$(IOT_LIBPATH) \
# 	-L. \
# 	$(CORE_O)

# the build target executable:
TARGET = server

all: $(TARGET)

server:  server.o button.o led.o
	$(CC) -o server $(CFLAGS) -L. -L$(IOT_LIBPATH) button.o led.o server.o $(LDFLAGS)

server.o:  server.c server.h
	$(CC) $(CFLAGS) $(CPATH) -c server.c

# $(TARGET): $(TARGET).c
# 	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET) *.o

# list targets that do not create files (but not all makes understand .PHONY)
.PHONY: all $(PLATS) default o a clean depend echo none

# (end of Makefile)
