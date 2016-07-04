#!/usr/bin/env sh

# fixup dynamic links on os x.

install_name_tool \
    -change \
    out/darwin/x86_64/release/resource/csdk/liboctbstack.dylib \
    $IOTIVITY_LIBPATH/liboctbstack.dylib \
    client

install_name_tool \
    -change \
    out/darwin/x86_64/release/resource/csdk/liboctbstack.dylib \
    $IOTIVITY_LIBPATH/liboctbstack.dylib \
    server

install_name_tool \
    -change \
    out/darwin/x86_64/release/resource/src/liboc.dylib \
    $IOTIVITY_LIBPATH/liboc.dylib \
    client

install_name_tool \
    -change \
    out/darwin/x86_64/release/resource/src/liboc.dylib \
    $IOTIVITY_LIBPATH/liboc.dylib \
    server


install_name_tool \
    -change \
    out/darwin/x86_64/release/resource/oc_logger/liboc_logger.dylib \
    $IOTIVITY_LIBPATH/liboc_logger.dylib \
    client

install_name_tool \
    -change \
    out/darwin/x86_64/release/resource/oc_logger/liboc_logger.dylib \
    $IOTIVITY_LIBPATH/liboc_logger.dylib \
    server
