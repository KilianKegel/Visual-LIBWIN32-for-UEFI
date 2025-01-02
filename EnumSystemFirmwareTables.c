/*++

Copyright (c) 2021-2025, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    EnumSystemFirmwareTables.c

Abstract:

    Win32 API EnumSystemFirmwareTables() for UEFI

Author:

    Kilian Kegel

--*/
#include <uefi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <Protocol\AcpiTable.h>
#include <Guid\Acpi.h>
#include <IndustryStandard/Acpi62.h>
#include <Protocol\AcpiSystemDescriptionTable.h>
#include "LibWin324UEFI.h"

extern EFI_SYSTEM_TABLE* _cdegST;
extern EFI_HANDLE _cdegImageHandle;

#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID))) //guiddef.h

/** EnumSystemFirmwareTables()
Synopsis
    uint32_t EnumSystemFirmwareTables(uint32_t FirmwareTableProviderSignature,void* pFirmwareTableEnumBuffer,uint32_t BufferSize);
    https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-enumsystemfirmwaretables#syntax
Description
    Enumerates all system firmware tables of the specified type.
    NOTE:   DSDT, XSDT is not returned by the function.
            If DataSize too small pFirmwareTableEnumBuffer is not touched
Paramters
    https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-enumsystemfirmwaretables#parameters
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-enumsystemfirmwaretables#return-value
**/
uint32_t EnumSystemFirmwareTables4UEFI(uint32_t FirmwareTableProviderSignature, void* pFirmwareTableEnumBuffer, uint32_t BufferSize)
{
    static EFI_GUID EfiAcpi20TableGuid = EFI_ACPI_20_TABLE_GUID;
    EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER* pRSD = NULL;
    EFI_CONFIGURATION_TABLE* pCfg = _cdegST->ConfigurationTable;
    EFI_ACPI_SDT_HEADER* pXSDT = NULL;
    int i, n, nRet = 0;

    do
    {
        if ((uint32_t)'ACPI' != FirmwareTableProviderSignature)
            break;                                                          // currently only support 'ACPI'

        for (i = 0; i < _cdegST->NumberOfTableEntries; i++)
        {
            int64_t qwSig;
            char* pStr8 = NULL;

            if (IsEqualGUID(&EfiAcpi20TableGuid, &_cdegST->ConfigurationTable[i].VendorGuid))
            {
                pRSD = pCfg[i].VendorTable;

                qwSig = pRSD->Signature;
                break;
            }
        }

        if (NULL != pRSD)
        {
            pXSDT = (void*)pRSD->XsdtAddress;
            EFI_ACPI_2_0_COMMON_HEADER* pTBLEnd = (void*)&(((char*)pXSDT)[pXSDT->Length]);
            EFI_ACPI_2_0_COMMON_HEADER** ppTBL = (void*)&(((char*)pXSDT)[sizeof(EFI_ACPI_SDT_HEADER)]);
            ptrdiff_t numtbl = pTBLEnd - (EFI_ACPI_2_0_COMMON_HEADER*)ppTBL;
            uint32_t* pSig32 = (void*)pFirmwareTableEnumBuffer;

            nRet = (int)numtbl * sizeof('FACP');

            if (BufferSize < numtbl * sizeof('FACP'))
                break;                                              // if buffersize too small, break

            if (NULL == pFirmwareTableEnumBuffer)
                break;                                              // if NULL buffer, break


            //printf("Signature   : %p -> %c%c%c%c\n", pXSDT, 0xFF & (pXSDT->Signature >> 0), 0xFF & (pXSDT->Signature >> 8), 0xFF & (pXSDT->Signature >> 16), 0xFF & (pXSDT->Signature >> 24));
            //printf("Length      : %d\n", pXSDT->Length);
            //printf("Revision    : %d\n", pXSDT->Revision);
            //printf("Checksum    : %02X\n", pXSDT->Checksum);

            //printf("numtbl %d\n", (int)numtbl);

            n = 0;
            while ((void*)pTBLEnd > (void*)ppTBL)
            {
                //printf("Signature %2d: %p -> %s\n", n, *ppTBL, strAcpiSig32((*ppTBL)->Signature, NULL));
                *pSig32++ = (*ppTBL)->Signature;
                ppTBL++;
            }
        }
    } while (0);

    return nRet;
}
UINT _w4uEnumSystemFirmwareTables(
    /*[in] */ DWORD FirmwareTableProviderSignature,
    /*[out]*/ PVOID pFirmwareTableEnumBuffer,
    /*[in] */ DWORD BufferSize
) {
    return EnumSystemFirmwareTables4UEFI(
        FirmwareTableProviderSignature,
        pFirmwareTableEnumBuffer,
        BufferSize
    );
}
void* __imp_EnumSystemFirmwareTables = (void*)_w4uEnumSystemFirmwareTables;