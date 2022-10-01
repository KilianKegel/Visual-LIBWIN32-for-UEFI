/*++

Copyright (c) 2021-2022, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    GetTickCount64.c

Abstract:

    Win32 API GetTickCount64() for UEFI

    Retrieves the number of milliseconds that have elapsed since the system was started.

Author:

    Kilian Kegel

--*/
#include <stdint.h>
#include <time.h>
#include "LibWin324UEFI.h"

/** GetTickCount64()
Synopsis
    ULONGLONG GetTickCount64();
    https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-gettickcount64#syntax
Description
    Retrieves the number of milliseconds that have elapsed since the system was started.
    
    NOTE:   Actually the number of millioseconds since the process was started.

Paramters
    
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-gettickcount64#return-value
**/
uint64_t __cdecl/*EFIAPI*/ GetTickCount644UEFI(void)
{
    return (uint64_t)clock();
}

void* __imp_GetTickCount64 = (void*)GetTickCount644UEFI;
