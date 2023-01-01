/*++

Copyright (c) 2021-2023, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    CloseHandle.c

Abstract:

    Win32 API CloseHandle() for UEFI

Author:

    Kilian Kegel

--*/
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "LibWin324UEFI.h"

extern DWORD _w4udwLastError;

/** __ChkACPISignature()
Synopsis
    int __ChkACPISignature(char Sig[4]);
Description
    Retrieves the specified firmware table from the firmware table provider.
Paramters
    char Sig[4] :   pointer to signatur
Returns
    1   :   found
    0   :   not found
**/
BOOL WINAPI _w4uCloseHandle(_In_ HANDLE hFile)
{
    W4UFILE* pw4uFile = hFile;
    BOOL fRet = 0;
    //printf( __FILE__"(%d), "__FUNCTION__"(): " ">>>\n", __LINE__);

    if (WIN324UEFI_ID == pw4uFile->signature)
    {
        fclose(pw4uFile->pFile);
        pw4uFile->signature = 0ULL;
        fRet = 1;
    }
    else {
        _w4udwLastError = ERROR_INVALID_HANDLE;
    }
    //printf( __FILE__"(%d), "__FUNCTION__"(): " "<<<\n", __LINE__);

    return fRet;
}

void* __imp_CloseHandle = (void*)_w4uCloseHandle;