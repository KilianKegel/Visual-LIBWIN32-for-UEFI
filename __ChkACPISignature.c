/*++

Copyright (c) 2021-2022, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    __ChkACPISignature.c

Abstract:

    Check if 4 Byte signature belongs to ACPI 6.2 table signatures

Author:

    Kilian Kegel

--*/
#include <stdio.h>
#include <string.h>

typedef struct _TABLEACPISIGNATURE {
    char Signature[4];
}TABLEACPISIGNATURE;

static TABLEACPISIGNATURE ACPISigs[] = {
    {'A', 'P', 'I', 'C'},
    {'B', 'E', 'R', 'T'},
    {'B', 'G', 'R', 'T'},
    {'C', 'P', 'E', 'P'},
    {'D', 'S', 'D', 'T'},
    {'E', 'C', 'D', 'T'},
    {'E', 'I', 'N', 'J'},
    {'E', 'R', 'S', 'T'},
    {'F', 'A', 'C', 'P'},
    {'F', 'A', 'C', 'S'},
    {'F', 'P', 'D', 'T'},
    {'G', 'T', 'D', 'T'},
    {'H', 'E', 'S', 'T'},
    {'H', 'M', 'A', 'T'},
    {'M', 'P', 'S', 'T'},
    {'M', 'S', 'C', 'T'},
    {'N', 'F', 'I', 'T'},
    {'P', 'D', 'T', 'T'},
    {'P', 'M', 'T', 'T'},
    {'P', 'P', 'T', 'T'},
    {'P', 'S', 'D', 'T'},
    {'R', 'A', 'S', 'F'},
    {'R', 'S', 'D', 'T'},
    {'S', 'B', 'S', 'T'},
    {'S', 'D', 'E', 'V'},
    {'S', 'L', 'I', 'T'},
    {'S', 'R', 'A', 'T'},
    {'S', 'S', 'D', 'T'},
    {'X', 'S', 'D', 'T'},
    {'B', 'O', 'O', 'T'},
    {'C', 'S', 'R', 'T'},
    {'D', 'B', 'G', '2'},
    {'D', 'B', 'G', 'P'},
    {'D', 'M', 'A', 'R'},
    {'D', 'P', 'P', 'T'},
    {'D', 'R', 'T', 'M'},
    {'E', 'T', 'D', 'T'},
    {'H', 'P', 'E', 'T'},
    {'i', 'B', 'F', 'T'},
    {'I', 'O', 'R', 'T'},
    {'I', 'V', 'R', 'S'},
    {'L', 'P', 'I', 'T'},
    {'M', 'C', 'F', 'G'},
    {'M', 'C', 'H', 'I'},
    {'M', 'S', 'D', 'M'},
    {'S', 'D', 'E', 'I'},
    {'S', 'L', 'I', 'C'},
    {'S', 'P', 'C', 'R'},
    {'S', 'P', 'M', 'I'},
    {'S', 'T', 'A', 'O'},
    {'T', 'C', 'P', 'A'},
    {'T', 'P', 'M', '2'},
    {'U', 'E', 'F', 'I'},
    {'W', 'A', 'E', 'T'},
    {'W', 'D', 'A', 'T'},
    {'W', 'D', 'R', 'T'},
    {'W', 'P', 'B', 'T'},
    {'W', 'S', 'M', 'T'},
    {'X', 'E', 'N', 'V'},
};

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

int __ChkACPISignature(char Sig[4])
{
    size_t i = 0;
    int found = 0;

    for (i = 0; i < sizeof(ACPISigs) / sizeof(ACPISigs[0]); i++)
    {
        if (0 == strncmp(Sig, ACPISigs[i].Signature, 4))
        {
            found = 1;
            break;
        }
    }
    return found;
}