/*++

Copyright (c) 2021-2024, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    WriteFile.c

Abstract:

    Win32 API WriteFile() for UEFI

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "LibWin324UEFI.h"

extern DWORD _w4udwLastError;

static BOOL WINAPI _w4uWriteFile(
    _In_ HANDLE hFile,
    _In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer,
    _In_ DWORD nNumberOfBytesToWrite,
    _Out_opt_ LPDWORD lpNumberOfBytesWritten,
    _Inout_opt_ LPOVERLAPPED lpOverlapped
)
{
    W4UFILE* pw4uFile = hFile;
    size_t size;
    BOOL fRet = 0;

    if (WIN324UEFI_ID == pw4uFile->signature)
    {
        if ((GENERIC_WRITE | GENERIC_ALL) & pw4uFile->dwDesiredAccess)
        {
            size = fwrite(lpBuffer, 1, nNumberOfBytesToWrite, pw4uFile->pFile);

            if (NULL != lpNumberOfBytesWritten)
                *lpNumberOfBytesWritten = (uint32_t)size;

            fRet = 1;
        }
        else
        {
            _w4udwLastError = ERROR_ACCESS_DENIED;
        }
    }
    else
    {
        _w4udwLastError = ERROR_INVALID_HANDLE;
    }

    return fRet;
}

void* __imp_WriteFile = (void*)_w4uWriteFile;