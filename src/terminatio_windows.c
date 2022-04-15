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
  (JNIEnv * env, jobject) {
    char open_file_names[MAX_PATH * 80];
    char sz_file_name[MAX_PATH * 80];
    char sz_path[MAX_PATH];
    char* p;
    int len;

    OPENFILENAME open;
    ZeroMemory(&open, sizeof(OPENFILENAME));
    open.lStructSize = sizeof(OPENFILENAME);
    open.lpstrFile = open_file_names;
    open.lpstrFile[0] = '\0';
    open.nMaxFile = sizeof(open_file_names);
    open.nFilterIndex = 1;
    open.lpstrFileTitle = NULL;
    open.nMaxFileTitle = 0;
    open.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT;
    if (GetOpenFileName(&open)) {
        lstrcpyn(sz_path, open_file_names, open.nFileOffset);
        sz_path[open.nFileOffset ] = '\0';
        len = lstrlen(sz_path);
        if(sz_path[len - 1] != '\\' )
        {
            // if selected multiple file, have to add '\\' in the end.
            lstrcat(sz_path, TEXT("\\"));
        }

        // pointer move to the first file
        p = open_file_names + open.nFileOffset;

        ZeroMemory(sz_file_name, sizeof(sz_file_name));
        while( *p )
        {
            lstrcat(sz_file_name, sz_path); // add path
            lstrcat(sz_file_name, p); // add file name
            lstrcat(sz_file_name, TEXT("\n")); // new line
            p += lstrlen(p) +1; // move to next file
        }
        return (*env) -> NewString(env, (const jchar *) sz_file_name, MAX_PATH * 80);
    }
    else {
        return 0;
    }
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