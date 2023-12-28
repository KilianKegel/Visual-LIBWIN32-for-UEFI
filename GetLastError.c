/*++

Copyright (c) 2021-2024, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    GetLastError.c

Abstract:

    Win32 API GetLastError() for UEFI

    Retrieves the last-error code value.

Author:

    Kilian Kegel

--*/
#include <windows.h>

DWORD _w4udwLastError;

/** GetLastError()
Synopsis
    DWORD GetLastError();
    https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
Description
    Retrieves the last-error code value.
Paramters
    https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror#parameters
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror#return-value
**/
static DWORD WINAPI _w4uUEFIGetLastError(VOID) {
    return _w4udwLastError;
}

void *__imp_GetLastError = (void*)_w4uUEFIGetLastError;
