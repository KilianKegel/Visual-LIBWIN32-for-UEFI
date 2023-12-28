/*++

Copyright (c) 2021-2024, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    IsBadWritePtr.c

Abstract:

    Win32 API IsBadWritePtr() for UEFI

    Verifies that the calling process has write access to the specified range of memory.

Author:

    Kilian Kegel

--*/
#include <windows.h>
/** IsBadWritePtr()
Synopsis
    BOOL IsBadWritePtr(const VOID *lp,UINT_PTR   ucb)
    https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-isbadwriteptr#syntax
Description
    Verifies that the calling process has write access to the specified range of memory.

Paramters
    https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-isbadwriteptr#parameters
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-isbadwriteptr#return-value
**/
int IsBadWritePtr4UEFI(const void* lp, UINT_PTR  ucb)
{
    return 0;
}

void* __imp_IsBadWritePtr = (void*)IsBadWritePtr4UEFI;
