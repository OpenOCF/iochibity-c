#!/usr/bin/env sh

# fixup dynamic links on darwin. maybe not needed if DYLD_LIBRARY_PATH correct?

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/oc_logger/liboc_logger.dylib \
    $IOTIVITY_LIBPATH/resource/oc_logger/liboc_logger.dylib \
    occlient

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/oc_logger/liboc_logger.dylib \
    $IOTIVITY_LIBPATH/resource/oc_logger/liboc_logger.dylib \
    occlientbasicops

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/oc_logger/liboc_logger.dylib \
    $IOTIVITY_LIBPATH/resource/oc_logger/liboc_logger.dylib \
    occlientslow

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/oc_logger/liboc_logger.dylib \
    $IOTIVITY_LIBPATH/resource/oc_logger/liboc_logger.dylib \
    ocserver

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/oc_logger/liboc_logger.dylib \
    $IOTIVITY_LIBPATH/resource/oc_logger/liboc_logger.dylib \
    ocserverbasicops

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/oc_logger/liboc_logger.dylib \
    $IOTIVITY_LIBPATH/resource/oc_logger/liboc_logger.dylib \
    ocserverslow

####
install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/csdk/liboctbstack.dylib \
    $IOTIVITY_LIBPATH/liboctbstack.dylib \
    occlient

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/csdk/liboctbstack.dylib \
    $IOTIVITY_LIBPATH/liboctbstack.dylib \
    occlientbasicops

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/csdk/liboctbstack.dylib \
    $IOTIVITY_LIBPATH/liboctbstack.dylib \
    occlientslow

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/csdk/liboctbstack.dylib \
    $IOTIVITY_LIBPATH/liboctbstack.dylib \
    ocserver

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/csdk/liboctbstack.dylib \
    $IOTIVITY_LIBPATH/liboctbstack.dylib \
    ocserverbasicops

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/csdk/liboctbstack.dylib \
    $IOTIVITY_LIBPATH/liboctbstack.dylib \
    ocserverslow

################################
install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/src/liboc.dylib \
    $IOTIVITY_LIBPATH/liboc.dylib \
    occlient

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/src/liboc.dylib \
    $IOTIVITY_LIBPATH/liboc.dylib \
    occlientbasicops

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/src/liboc.dylib \
    $IOTIVITY_LIBPATH/liboc.dylib \
    occlientslow

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/src/liboc.dylib \
    $IOTIVITY_LIBPATH/liboc.dylib \
    ocserver

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/src/liboc.dylib \
    $IOTIVITY_LIBPATH/liboc.dylib \
    ocserverbasicops

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/src/liboc.dylib \
    $IOTIVITY_LIBPATH/liboc.dylib \
    ocserverslow

install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/src/liboc.dylib \
    $IOTIVITY_LIBPATH/liboc.dylib \
    ocserverslow


################################################################
install_name_tool \
    -change \
    out/darwin/x86_64/${IOTIVITY_STAGE}/resource/oc_logger/liboc_logger.dylib \
    $IOTIVITY_LIBPATH/liboc_logger.dylib \
    $IOTIVITY_LIBPATH/liboc.dylib

