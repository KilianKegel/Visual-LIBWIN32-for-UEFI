/*++

Copyright (c) 2021-2022, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    LibWin324UEFI.h

Abstract:

    Win32 API for UEFI common definitions

Author:

    Kilian Kegel

--*/

#ifndef _WIN324UEFI_H_
#define _WIN324UEFI_H_

#define WIN324UEFI_ID 0x4946455534323357ULL

typedef struct tagW4UFILE
{
    uint64_t    signature;
    uint32_t    dwDesiredAccess;
    uint32_t    dwCreationDisposition;
    FILE*       pFile;

}W4UFILE;


#endif//_WIN324UEFI_H_
