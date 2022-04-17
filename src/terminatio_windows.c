//
// Created by user on 2022/4/14.
//
#include <com_toocol_ssh_common_jni_TerminatioJNI.h>
#include <windows.h>
#include <conio.h>
#include <shlobj.h>

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
        (JNIEnv *env, jobject) {
    TCHAR open_file_names[MAX_PATH * 80];
    TCHAR sz_file_name[MAX_PATH * 80];
    TCHAR sz_path[MAX_PATH];
    TCHAR *p;
    int len;

    OPENFILENAME open;
    ZeroMemory(&open, sizeof(OPENFILENAME));
    open.hwndOwner = GetForegroundWindow();
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
        sz_path[open.nFileOffset] = '\0';
        len = lstrlen(sz_path);
        if (sz_path[len - 1] != '\\') {
            // if selected multiple file, have to add '\\' in the end.
            lstrcat(sz_path, TEXT("\\"));
        }

        // pointer move to the first file
        p = open_file_names + open.nFileOffset;

        ZeroMemory(sz_file_name, sizeof(sz_file_name));
        while (*p) {
            lstrcat(sz_file_name, sz_path); // add path
            lstrcat(sz_file_name, p); // add file name
            lstrcat(sz_file_name, TEXT(",")); // add
            p += lstrlen(p) + 1; // move to next file
        }

        jstring jstring_file_names = (*env)->NewStringUTF(env, sz_file_name);
        ZeroMemory(&open, sizeof(OPENFILENAME));
        return jstring_file_names;
    } else {
        ZeroMemory(&open, sizeof(OPENFILENAME));
        return 0;
    }
}

/*
 * Class:     com_toocol_ssh_common_jni_TerminatioJNI
 * Method:    chooseDirectory
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_toocol_ssh_common_jni_TerminatioJNI_chooseDirectory
        (JNIEnv *env, jobject) {
    LPITEMIDLIST lp_dlist = NULL;
    TCHAR sz_path_name[MAX_PATH];

    BROWSEINFO bInfo;
    ZeroMemory(&bInfo, sizeof(BROWSEINFO));

    bInfo.hwndOwner = GetForegroundWindow();
    bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI |
                    BIF_UAHINT | BIF_NONEWFOLDERBUTTON;
    lp_dlist = SHBrowseForFolder(&bInfo);

    if (lp_dlist != NULL) {
        SHGetPathFromIDList(lp_dlist, sz_path_name);
        ZeroMemory(&bInfo, sizeof(BROWSEINFO));
        return (*env)->NewStringUTF(env, sz_path_name);
    } else {
        ZeroMemory(&bInfo, sizeof(BROWSEINFO));
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
    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    PCONSOLE_SCREEN_BUFFER_INFO info = malloc(sizeof(CONSOLE_SCREEN_BUFFER_INFO));
    GetConsoleScreenBufferInfo(outputHandle, info);
    jint width = info->srWindow.Right - info->srWindow.Left + 1;
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
    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    PCONSOLE_SCREEN_BUFFER_INFO info = malloc(sizeof(CONSOLE_SCREEN_BUFFER_INFO));
    GetConsoleScreenBufferInfo(outputHandle, info);
    jint height = info->srWindow.Bottom - info->srWindow.Top + 1;
    free(info);
    return height;
}

/*
 * Class:     com_toocol_ssh_common_jni_TerminatioJNI
 * Method:    getCursorPosition
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_toocol_ssh_common_jni_TerminatioJNI_getCursorPosition
        (JNIEnv *env, jobject) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    GetConsoleScreenBufferInfo(handle, &cbsi);
    COORD coord = cbsi.dwCursorPosition;

    TCHAR pos[1024];
    TCHAR tmp[512];
    ZeroMemory(pos, sizeof(TCHAR));
    ZeroMemory(tmp, sizeof(TCHAR));
    lstrcat(pos, ltoa(coord.X, tmp, 10));
    lstrcat(pos, ",");
    lstrcat(pos, ltoa(coord.Y, tmp, 10));
    return (*env)->NewStringUTF(env, pos);
}

/*
 * Class:     com_toocol_ssh_common_jni_TerminatioJNI
 * Method:    setCursorPosition
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_toocol_ssh_common_jni_TerminatioJNI_setCursorPosition
        (JNIEnv *, jobject, jint x, jint y) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {(short) x, (short) y};
    SetConsoleCursorPosition(handle, coord);
}

/*
 * Class:     com_toocol_ssh_common_jni_TerminatioJNI
 * Method:    cursorBackLine
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_toocol_ssh_common_jni_TerminatioJNI_cursorBackLine
        (JNIEnv *, jobject, jint lines) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    GetConsoleScreenBufferInfo(handle, &cbsi);
    COORD coord = cbsi.dwCursorPosition;
    coord.Y = (SHORT) (coord.Y - lines);

    SetConsoleCursorPosition(handle, coord);
}

/*
 * Class:     com_toocol_ssh_common_jni_TerminatioJNI
 * Method:    showCursor
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_toocol_ssh_common_jni_TerminatioJNI_showCursor
        (JNIEnv *, jobject) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cinfo;
    GetConsoleCursorInfo(handle, &cinfo);
    cinfo.bVisible = 1;
    SetConsoleCursorInfo(handle, &cinfo);
}

/*
 * Class:     com_toocol_ssh_common_jni_TerminatioJNI
 * Method:    hideCursor
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_toocol_ssh_common_jni_TerminatioJNI_hideCursor
        (JNIEnv *, jobject) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cinfo;
    GetConsoleCursorInfo(handle, &cinfo);
    cinfo.bVisible = 0;
    SetConsoleCursorInfo(handle, &cinfo);
}

/*
 * Class:     com_toocol_ssh_common_jni_TerminatioJNI
 * Method:    cursorLeft
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_toocol_ssh_common_jni_TerminatioJNI_cursorLeft
        (JNIEnv *, jobject) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    GetConsoleScreenBufferInfo(handle, &cbsi);
    COORD coord = cbsi.dwCursorPosition;
    coord.X = (SHORT) (coord.X - 1);

    SetConsoleCursorPosition(handle, coord);
}

/*
 * Class:     com_toocol_ssh_common_jni_TerminatioJNI
 * Method:    cursorRight
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_toocol_ssh_common_jni_TerminatioJNI_cursorRight
        (JNIEnv *, jobject) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    GetConsoleScreenBufferInfo(handle, &cbsi);
    COORD coord = cbsi.dwCursorPosition;
    coord.X = (SHORT) (coord.X + 1);

    SetConsoleCursorPosition(handle, coord);
}
