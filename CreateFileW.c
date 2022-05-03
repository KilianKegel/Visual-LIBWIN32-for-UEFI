/*++

Copyright (c) 2021-2022, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    CreateFileW.c

Abstract:

    Win32 API CreateFileW() for UEFI

    Creates or opens a file. This implementation does not create an I/O device.

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <io.h>
#include "LibWin324UEFI.h"

static W4UFILE _w4uiobuf[64/*CDE_FILEV_MAX */];

/** CreateFileW()
Synopsis
    HANDLE CreateFileA(
      [in]           LPCWSTR               lpFileName,
      [in]           DWORD                 dwDesiredAccess,
      [in]           DWORD                 dwShareMode,
      [in, optional] LPSECURITY_ATTRIBUTES lpSecurityAttributes,
      [in]           DWORD                 dwCreationDisposition,
      [in]           DWORD                 dwFlagsAndAttributes,
      [in, optional] HANDLE                hTemplateFile
    );
    https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew#syntax
Description
    Creates or opens a file with a given wide string filename.
    This implementation does not create an I/O device.
Paramters
    https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew#parameters
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew#return-value
**/
extern HANDLE WINAPI _w4uCreateFileA(LPCSTR,DWORD,DWORD,LPSECURITY_ATTRIBUTES,DWORD,DWORD,HANDLE);

HANDLE WINAPI _w4uCreateFileW(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
) 
{
    size_t size = NULL != lpFileName ? sizeof(wchar_t) * ( (char)'\0' + wcslen(lpFileName)) : 0;
    char *pFileName = NULL != lpFileName ? malloc(size) : NULL;
    HANDLE hRet = 0;

    if (pFileName) do
    {
        wcstombs(pFileName, lpFileName, size);

        hRet = _w4uCreateFileA(
            pFileName, 
            dwDesiredAccess, 
            dwShareMode, 
            lpSecurityAttributes, 
            dwCreationDisposition, 
            dwFlagsAndAttributes, 
            hTemplateFile
        );

        free(pFileName);

    } while (0);

    return hRet;
}

void* __imp_CreateFileW = (void*) _w4uCreateFileW;
