cmake_minimum_required(VERSION 3.21)
project(terminatio_native C)

set(CMAKE_C_STANDARD 99)

include_directories(src)

add_library(terminatio SHARED
        src/com_toocol_ssh_common_jni_TerminatioJNI.h
        src/jni.h
        src/jni_md.h
        src/terminatio_windows.c)
