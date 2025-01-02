/*++

Copyright (c) 2021-2025, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    CreateFileA.c

Abstract:

    Win32 API CreateFileA() for UEFI

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

/** CreateFileA()
Synopsis
    HANDLE CreateFileA(
      [in]           LPCSTR                lpFileName,
      [in]           DWORD                 dwDesiredAccess,
      [in]           DWORD                 dwShareMode,
      [in, optional] LPSECURITY_ATTRIBUTES lpSecurityAttributes,
      [in]           DWORD                 dwCreationDisposition,
      [in]           DWORD                 dwFlagsAndAttributes,
      [in, optional] HANDLE                hTemplateFile
    );
    https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea#syntax
Description
    Creates or opens a file with a given narrow string filename.
    This implementation does not create an I/O device.
Paramters
    https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea#parameters
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea#return-value
**/
HANDLE WINAPI _w4uCreateFileA(
    _In_ LPCSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
) {
    HANDLE hRet = (void*)-1;//assume error
    FILE *fp = INVALID_HANDLE_VALUE;
    int fFileExists = 0, fFileRW;
    int old_errno = errno;                                  // preserve original errno
    int fd;
    W4UFILE* pw4uFile = INVALID_HANDLE_VALUE;

    do {
        //
        // check invalid dwCreationDisposition
        //
        if (!( dwCreationDisposition == CREATE_NEW
            || dwCreationDisposition == CREATE_ALWAYS
            || dwCreationDisposition == OPEN_EXISTING
            || dwCreationDisposition == OPEN_ALWAYS)) {
            SetLastError(ERROR_INVALID_PARAMETER);
            break;
        }

        //
        // check existence of requested file, check write access
        //
        errno = 0;
        fp = fopen(lpFileName, "r+");

        fFileExists = (ENOENT != errno);
        fFileRW     = (EACCES != errno);

        if (NULL != fp)
            fclose(fp);

        //
        // invalid handle for error cases below
        //
        fp = INVALID_HANDLE_VALUE;                  // set INVALID_HANDLE_VALUE for error cases below

        //
        // check ERROR_FILE_NOT_FOUND
        //
        if (0 == fFileExists && OPEN_EXISTING == dwCreationDisposition) {
            SetLastError(ERROR_FILE_NOT_FOUND);
            break;
        }

        //
        // check ERROR_FILE_EXISTS
        //
        if (1 == fFileExists && CREATE_NEW == dwCreationDisposition) {
            SetLastError(ERROR_FILE_EXISTS);
            break;
        }

        //
        // check ERROR_ACCESS_DENIED
        //
        if (1 == fFileExists 
            && (
                    (CREATE_NEW == dwCreationDisposition)
                || (0 == fFileRW && GENERIC_WRITE & dwDesiredAccess)
                || ((GENERIC_ALL & dwDesiredAccess ) && ((CREATE_ALWAYS | OPEN_ALWAYS | OPEN_EXISTING) & dwCreationDisposition ))
                )
            )
        {
            SetLastError(ERROR_ACCESS_DENIED);
            break;
        }

        //
        // error codes for successful returns with ERROR_ALREADY_EXISTS set
        //
        if (1 == fFileExists 
            && (
                    (1 == fFileRW && (OPEN_ALWAYS == dwCreationDisposition || CREATE_ALWAYS == dwCreationDisposition))
                ||  (0 == fFileRW && (OPEN_ALWAYS == dwCreationDisposition))
                )
            ) 
        {
            SetLastError(ERROR_ALREADY_EXISTS);
        }

        //
        // error codes for successful returns with ERROR_ACCESS_DENIED set
        //
        if (    1 == fFileExists 
            &&  0 == fFileRW
            &&  CREATE_ALWAYS == dwCreationDisposition
            )
        {
            SetLastError(ERROR_ACCESS_DENIED);
        }


        //
        // open/create in write/readonly mode, truncate for CREATE_xxx and open for OPEN_xxx
        //
        //  NOTE: CREATE_NEW == 1, CREATE_ALWAYS == 2,  OPEN_EXISTING == 3, OPEN_ALWAYS == 4
        fp = fopen(lpFileName, dwCreationDisposition < OPEN_EXISTING ? "wb" : (1 == fFileRW ? ((1 == fFileExists) ? "rb+" : "wb+")/*"rb+"*/ : "rb"));   // open r/o files in r/o mode


        if (NULL != fp)
        {
            fd = _fileno(fp);

            pw4uFile = &_w4uiobuf[fd];
            pw4uFile->signature = WIN324UEFI_ID;
            pw4uFile->pFile = fp;
            pw4uFile->dwDesiredAccess = dwDesiredAccess;

            break;
        }

        fp = INVALID_HANDLE_VALUE;                      // set INVALID_HANDLE_VALUE return on error;

    } while (0);

    errno = old_errno;                                  // restore original errno

    return (HANDLE)pw4uFile;
}

void* __imp_CreateFileA = (void*) _w4uCreateFileA;
