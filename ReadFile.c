/*++

Copyright (c) 2021-2022, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    ReadFile.c

Abstract:

    Win32 API ReadFile() for UEFI

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "LibWin324UEFI.h"


extern DWORD _w4udwLastError;

/** ReadFile()
Synopsis
    BOOL ReadFile(
      [in]                HANDLE       hFile,
      [out]               LPVOID       lpBuffer,
      [in]                DWORD        nNumberOfBytesToRead,
      [out, optional]     LPDWORD      lpNumberOfBytesRead,
      [in, out, optional] LPOVERLAPPED lpOverlapped
    );
    https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile#syntax
Description
    Reads data from the specified file. 
    Reads occur at the position specified by the file pointer if supported by the device.
Paramters
    https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile#parameters
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile#return-value
**/
static BOOL WINAPI _w4uReadFile(
    _In_ HANDLE hFile,
    _Out_writes_bytes_to_opt_(nNumberOfBytesToRead, *lpNumberOfBytesRead) __out_data_source(FILE) LPVOID lpBuffer,
    _In_ DWORD nNumberOfBytesToRead,
    _Out_opt_ LPDWORD lpNumberOfBytesRead,
    _Inout_opt_ void* lpOverlappedIGNORED
) 
{
    W4UFILE* pw4uFile = hFile;
    size_t size;
    BOOL fRet = 0;

    if (WIN324UEFI_ID == pw4uFile->signature)
    {
        if ((GENERIC_READ | GENERIC_ALL) & pw4uFile->dwDesiredAccess)
        {
            size = fread(lpBuffer, 1, nNumberOfBytesToRead, pw4uFile->pFile);

            if (NULL != lpNumberOfBytesRead)
                *lpNumberOfBytesRead = (uint32_t)size;

            fRet = 1;
        }
        else
            _w4udwLastError = ERROR_ACCESS_DENIED;
    } else {
        _w4udwLastError = ERROR_INVALID_HANDLE;
    }

    return fRet;
}
void* __imp_ReadFile = (void*)_w4uReadFile;
