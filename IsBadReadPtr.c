/*++

Copyright (c) 2021-2025, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    IsBadReadPtr.c

Abstract:

    Win32 API IsBadReadPtr() for UEFI

    Verifies that the calling process has read access to the specified range of memory.

Author:

    Kilian Kegel

--*/
#include <windows.h>

/** Sleep()
Synopsis
    BOOL IsBadReadPtr(const VOID *lp,UINT_PTR   ucb)
    https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-isbadreadptr#syntax
Description
    Verifies that the calling process has read access to the specified range of memory.

Paramters
    https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-isbadreadptr#parameters
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-isbadreadptr#return-value
**/
int IsBadReadPtr4UEFI(const void* lp, UINT_PTR   ucb)
{
    return 0;
}

void* __imp_IsBadReadPtr = (void*)IsBadReadPtr4UEFI;
