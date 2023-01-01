/*++

Copyright (c) 2021-2023, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    QueryPerformanceFrequency.c

Abstract:

    Win32 API QueryPerformanceFrequency() for UEFI 

    Retrieves the frequency of the performance counter. The frequency of the performance 
    counter is fixed at system boot and is consistent across all processors. Therefore, 
    the frequency need only be queried upon application initialization, and the result can be cached.

Author:

    Kilian Kegel

--*/

#include <uefi.h>
#include <stdint.h>
#include "LibWin324UEFI.h"

#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID))) //guiddef.h

extern void _disable(void);
extern void _enable(void);

#pragma intrinsic (_disable, _enable)

#define TIMER 2

/** QueryPerformanceFrequency()
Synopsis
    BOOL QueryPerformanceFrequency(LARGE_INTEGER *lpFrequency);
    https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency#syntax
Description
    Retrieves the current value of the performance counter, which is a high resolution (<1us) time stamp that 
    can be used for time-interval measurements.

Paramters
    https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency#parameters
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency#return-value
**/

/** GetTscPerSec

    GetTscPerSec() returns the TimeStampCounter counts per second

    NTSC Color Subcarrier:  f = 3.579545MHz * 4 ->
                            f = 14.31818MHz / 12 -> 1.193181818...MHz
    PIT 8254 input clk:     f = 1.193181818MHz
                            f = 11931818181Hz / 59659 ->
                            f = 20Hz -> t = 1/f
                            t = 50ms
                            ========
                            50ms * 20 ->

                                  1s
                            ===============

    @param[in] VOID

    @retval number of CPU clock per second

**/
int32_t EFIAPI QueryPerformanceFrequency4UEFI(int64_t* lpFrequency)
{
    size_t eflags = __readeflags();         // save flaags
    unsigned long long qwTSCPerTick, qwTSCEnd, qwTSCStart, qwTSCDrift;
    unsigned char counterLoHi[2];
    unsigned short* pwCount = (unsigned short*)&counterLoHi[0];
    unsigned short wCountDrift;

    _disable();

    outp(0x61, 0);                          // stop counter
    outp(0x43, (TIMER << 6) + 0x34);        // program timer 2 for MODE 2
    outp(0x42, 0xFF);                       // write counter value low 65535
    outp(0x42, 0xFF);                       // write counter value high 65535
    outp(0x61, 1);                          // start counter

    qwTSCStart = __rdtsc();                 // get TSC start

    //
    // repeat counter latch command until 50ms
    //
    do                                                      //
    {                                                       //
        outp(0x43, (TIMER << 6) + 0x0);                     // counter latch timer 2
        counterLoHi[0] = (unsigned char)inp(0x40 + TIMER);  // get low byte
        counterLoHi[1] = (unsigned char)inp(0x40 + TIMER);  // get high byte
                                                            //
    } while (*pwCount > (65535 - 59659));                   // until 59659 ticks gone

    qwTSCEnd = __rdtsc();                                   // get TSC end ~50ms

    *pwCount = 65535 - *pwCount;                            // get true, not inverted, number of clock ticks...
                                                            // ... that really happened
    wCountDrift = *pwCount - 59659;                         // get the number of additional ticks gone through

    //
    // approximate the additional number of TSC
    //
    qwTSCPerTick = (qwTSCEnd - qwTSCStart) / *pwCount;      // get number of CPU TSC per 8254 ClkTick (1,19MHz)
    qwTSCDrift = wCountDrift * qwTSCPerTick;                // get TSC drift

    //printf("PITticks gone: %04d, PITticksDrift: %04d, TSCPerTick: %lld, TSCDrift: %5lld, TSC end/start diff: %lld, end - start - TSCDrift: %lld, TSC/Sec: %lld\n",
    //    *pwCount,                                         /* PITticks gone            */
    //    wCountDrift,                                      /* PITticksDrift            */
    //    qwTSCPerTick,                                     /* TSCPerTick               */
    //    qwTSCDrift,                                       /* TSCDrift                 */
    //    qwTSCEnd - qwTSCStart,                            /* TSC end/start diff       */
    //    qwTSCEnd - qwTSCDrift - qwTSCStart,               /* end - start - TSCDrift   */
    //    20 * (qwTSCEnd - qwTSCStart - qwTSCDrift)         /* TSC/Sec                  */
    //);

    if (0x200 & eflags)                                     // restore IF interrupt flag
        _enable();

    *lpFrequency = (qwTSCEnd - qwTSCStart - qwTSCDrift)/50; // subtract the drift from TSC difference, scale to 1/1000 second

    return 1;
}

BOOL WINAPI _w4uQueryPerformanceFrequency(
    /*_Out_*/ int64_t* lpFrequency
) {
    return QueryPerformanceFrequency4UEFI(lpFrequency);
}

void* __imp_QueryPerformanceFrequency = (void*)_w4uQueryPerformanceFrequency;

