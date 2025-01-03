/*++

Copyright (c) 2021-2025, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    SetLastError.c

Abstract:

    Win32 API SetLastError() for UEFI

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <stdint.h>

#define WINAPI      __stdcall
typedef uint32_t   DWORD;

#define _In_

extern DWORD _w4udwLastError;

static void WINAPI _w4uUEFISetLastError(_In_ DWORD dwErrCode)
{
    _w4udwLastError = dwErrCode;
}

void *__imp_SetLastError = (void*)_w4uUEFISetLastError;