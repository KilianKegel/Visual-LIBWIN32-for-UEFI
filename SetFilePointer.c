/*++

Copyright (c) 2021-2024, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    SetFilePointer.c

Abstract:

    Win32 API SetFilePointer() for UEFI

    Moves the file pointer of the specified file.

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "LibWin324UEFI.h"

extern DWORD _w4udwLastError;

/** SetFilePointer()
Synopsis
    DWORD SetFilePointer(
      [in]                HANDLE hFile,
      [in]                LONG   lDistanceToMove,
      [in, out, optional] PLONG  lpDistanceToMoveHigh,
      [in]                DWORD  dwMoveMethod
    );    
    https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setfilepointer#syntax
Description
    Moves the file pointer of the specified file.
Paramters
    https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setfilepointer#parameters
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setfilepointer#return-value
**/
static DWORD WINAPI _w4uUEFISetFilePointer(
    _In_ HANDLE hFile,
    _In_ LONG lDistanceToMove,
    _Inout_opt_ PLONG lpDistanceToMoveHigh,
    _In_ DWORD dwMoveMethod
)
{
    union /*tagWINDOWSSEEKPOINTER*/ {
        int64_t pos64;
        int32_t pos32;
        int32_t pos3264[2];
    }SeekPtr = {.pos64 = 0};

    W4UFILE* pw4uFile = hFile;
    BOOL fRet = 0;
    DWORD dwRet = INVALID_SET_FILE_POINTER;
    int n;
    fpos_t newpos;
    int old_errno = errno;                                  // preserve original errno
    errno = 0;                                              // clear errno

    do {
        if (WIN324UEFI_ID == pw4uFile->signature)
        {

            if (NULL != lpDistanceToMoveHigh) 
            {
                SeekPtr.pos3264[0] = (int32_t)lDistanceToMove;
                SeekPtr.pos3264[1] = (int32_t)*lpDistanceToMoveHigh;
            }
            else
                SeekPtr.pos64 = lDistanceToMove;

            n = fseek(pw4uFile->pFile, (long)SeekPtr.pos64, (int)dwMoveMethod);

            if (0 == n)
            {
                newpos = ftell(pw4uFile->pFile);
                dwRet = (DWORD)newpos;
            }
            else
                _w4udwLastError = ERROR_NEGATIVE_SEEK;

            ////printf(__FILE__"(%d): n == %X, SeekPtr.pos64 %lld, dwMoveMethod %d, errno %d\n",
            //    __LINE__,
            //    n,
            //    SeekPtr.pos64,
            //    dwMoveMethod,
            //    errno
            //);

        }
        else {
            _w4udwLastError = ERROR_INVALID_HANDLE;
        }
    } while (0);

    errno = old_errno;                                      // restore original errno

    return dwRet;
}

void *__imp_SetFilePointer = (void*)_w4uUEFISetFilePointer;