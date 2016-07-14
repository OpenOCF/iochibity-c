#!/usr/bin/env sh

# fixup dynamic links on darwin. maybe not needed if DYLD_LIBRARY_PATH correct?

install_name_tool \
    -change \
    out/darwin/x86_64/release/resource/oc_logger/liboc_logger.dylib \
    $IOTIVITY_LIBPATH/resource/oc_logger/liboc_logger.dylib \
    occlientbasicops

install_name_tool \
    -change \
    out/darwin/x86_64/release/resource/csdk/liboctbstack.dylib \
    $IOTIVITY_LIBPATH/liboctbstack.dylib \
    occlientbasicops

install_name_tool \
    -change \
    out/darwin/x86_64/release/resource/src/liboc.dylib \
    $IOTIVITY_LIBPATH/liboc.dylib \
    occlientbasicops


