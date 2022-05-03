/*++

Copyright (c) 2021-2022, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    QueryPerformanceCounter.c

Abstract:

    Win32 API QueryPerformanceCounter() for UEFI 

    Retrieves the current value of the performance counter, which is a high resolution (<1us)
    time stamp that can be used for time-interval measurements.

Author:

    Kilian Kegel

--*/

#include <uefi.h>
#include <stdint.h>

#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID))) //guiddef.h

/** QueryPerformanceCounter()
Synopsis
    BOOL QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount);
    https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter#syntax
Description
    Retrieves the current value of the performance counter, which is a high resolution (<1us) time stamp that 
    can be used for time-interval measurements.

Paramters
    https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter#parameters
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter#return-value
**/
int32_t EFIAPI QueryPerformanceCounter4UEFI(int64_t* lpPerformanceCount)
{
    *lpPerformanceCount = __rdtsc() / 1000;

    return 1;
}