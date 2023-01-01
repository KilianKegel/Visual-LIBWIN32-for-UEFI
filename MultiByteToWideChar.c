/*++

Copyright (c) 2021-2023, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    MultiByteToWideChar.c

Abstract:

    Win32 API MultiByteToWideChar() for UEFI

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <stdint.h>

#define _In_
#define __out_data_source(src_sym)
#define _In_NLS_string_(size)
#define _Out_writes_to_opt_(size,count)

/** MultiByteToWideChar()
Synopsis
    https://docs.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar#syntax
Description
    Maps a character string to a UTF-16 (wide character) string.
Paramters
    https://docs.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar#parameters
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar#return-value
**/
int WINAPI _w4uUEFIMultiByteToWideChar(
    _In_ UINT CodePage,
    _In_ DWORD dwFlags,
    _In_NLS_string_(cbMultiByte) LPCCH lpMultiByteStr,
    _In_ int cbMultiByte,
    _Out_writes_to_opt_(cchWideChar, return) LPWSTR lpWideCharStr,
    _In_ int cchWideChar
) {
    int Err = ERROR_INVALID_PARAMETER;
    int c, nRet = 0;
    int i = 0;

    do{
        if (0 == cbMultiByte)
            break;

        if (NULL == lpMultiByteStr)
            break;

        if (NULL == lpWideCharStr)
            break;

        while (
            c = lpMultiByteStr[i],
            lpWideCharStr[i++] = (wchar_t)c,
            ((unsigned)i < (unsigned)cbMultiByte) && ('\0' != c)
            );
        //
        // fill remaining bytes if cbMultiByte != -1
        //
        if (i < cbMultiByte)                        
        {
            while (i < cbMultiByte)                     
                lpWideCharStr[i++] = (wchar_t)'\0';
        }

    }while(Err = ERROR_SUCCESS);

    if (ERROR_SUCCESS != Err)
        SetLastError(Err);

    return ERROR_SUCCESS == Err ? i : 0;
}

void* __imp_MultiByteToWideChar = (void*)_w4uUEFIMultiByteToWideChar;
