//
// Created by user on 2022/4/14.
//
#include "com_toocol_ssh_common_jni_TerminatioJNI.h"
#include <windows.h>
#include <conio.h>

/*
 * Class:     com_toocol_ssh_common_jni_TerminatioJNI
 * Method:    getCh
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_toocol_ssh_common_jni_TerminatioJNI_getCh
  (JNIEnv *, jobject) {
    return getch();
}

/*
 * Class:     com_toocol_ssh_common_jni_TerminatioJNI
 * Method:    chooseFiles
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_toocol_ssh_common_jni_TerminatioJNI_chooseFiles
  (JNIEnv *, jobject) {
    return 0;
}

/*
 * Class:     com_toocol_ssh_common_jni_TerminatioJNI
 * Method:    getWindowWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_toocol_ssh_common_jni_TerminatioJNI_getWindowWidth
  (JNIEnv *, jobject) {
    HANDLE outputHandle = GetStdHandle (STD_OUTPUT_HANDLE);
    PCONSOLE_SCREEN_BUFFER_INFO info =  malloc (sizeof (CONSOLE_SCREEN_BUFFER_INFO));
    GetConsoleScreenBufferInfo (outputHandle, info);
    jint width = info->srWindow.Right - info->srWindow.Left+1;
    free(info);
    return width;
}

/*
 * Class:     com_toocol_ssh_common_jni_TerminatioJNI
 * Method:    getWindowHeight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_toocol_ssh_common_jni_TerminatioJNI_getWindowHeight
  (JNIEnv *, jobject) {
    HANDLE outputHandle = GetStdHandle (STD_OUTPUT_HANDLE);
    PCONSOLE_SCREEN_BUFFER_INFO info =  malloc (sizeof (CONSOLE_SCREEN_BUFFER_INFO));
    GetConsoleScreenBufferInfo (outputHandle, info);
    jint height = info->srWindow.Bottom - info->srWindow.Top+1;
    free(info);
    return height;
}