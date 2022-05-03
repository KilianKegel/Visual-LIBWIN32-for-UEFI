/*++

Copyright (c) 2021-2022, Kilian Kegel. All rights reserved.<BR>

    SPDX-License-Identifier: GNU General Public License v3.0 only

Module Name:

    GetSystemFirmwareTable.c

Abstract:

    Win32 API GetSystemFirmwareTable() for UEFI 

Author:

    Kilian Kegel

--*/

#include <uefi.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <Guid\Acpi.h>
#include <Protocol\AcpiSystemDescriptionTable.h>

#define CDETRACE(msg) //printf(__FILE__"(%d) \\ " __FUNCTION__"(): ", __LINE__ ), printf msg

#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID))) //guiddef.h
//#define CDETRACE(msg) //printf(__FILE__"(%d) \\ " __FUNCTION__"(): ", __LINE__ ), printf msg
//
//  warning C4273: 'GetSystemFirmwareTable': inconsistent dll linkage
// 
#pragma warning(disable:4273)

//
// externs
//
extern EFI_SYSTEM_TABLE* pEfiSystemTable;
extern EFI_HANDLE hEfiImageHandle;

/** GetSystemFirmwareTable()
Synopsis
    UINT GetSystemFirmwareTable(DWORD FirmwareTableProviderSignature, DWORD FirmwareTableID, PVOID pFirmwareTableBuffer, DWORD BufferSize, ...);
    https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemfirmwaretable#syntax
Description
    Retrieves the specified firmware table from the firmware table provider.
    
    NOTE: This is an extended version that allows to pass the instance of SSDT as an additional parameter

Paramters
    https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemfirmwaretable#parameters
Returns
    https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemfirmwaretable#return-value
**/
uint32_t __cdecl/*EFIAPI*/ GetSystemFirmwareTable4UEFI(
    uint32_t FirmwareTableProviderSignature,
    uint32_t FirmwareTableID,
    void* pFirmwareTableBuffer,
    uint32_t BufferSize,
    ... // NOTE: extended for UEFI with 
        //      1. ACPI_PHYSICAL_ADDRESS *Address
        //      2. UINT32 Instance
)
{
    static EFI_GUID EfiAcpi20TableGuid = EFI_ACPI_20_TABLE_GUID;
    EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER* pRSD = NULL;
    EFI_CONFIGURATION_TABLE* pCfg = pEfiSystemTable->ConfigurationTable;
    uint32_t nRet = 0;
    int ssdtinstance = 0,i;
    va_list ap;
    va_start(ap, BufferSize);
    void* pTbl;
    uint32_t sizeTbl;
    bool foundTbl = false;
    //
    // NOTE: DSDT is _not_ located in the RSDT, but in the FACP == FADT
    //       Therefore patch FirmwareTableID to FACP == FADT and get DSDT from there
    //
    bool fDSDT = 'TDSD' == FirmwareTableID;
    uint64_t *pAddress = NULL;

    //CDETRACE(("--> \nFirmwareTableProviderSignature: %c%c%c%c, \nFirmwareTableID: %c%c%c%c\n", 
    //    0xFF & (FirmwareTableProviderSignature >> 24),
    //    0xFF & (FirmwareTableProviderSignature >> 16),
    //    0xFF & (FirmwareTableProviderSignature >> 8),
    //    0xFF & (FirmwareTableProviderSignature >> 0),
    //    0xFF & (FirmwareTableID >> 24),
    //    0xFF & (FirmwareTableID >> 16),
    //    0xFF & (FirmwareTableID >> 8),
    //    0xFF & (FirmwareTableID >> 0)
    //    ));
    
    do {

        if ('ACPI' != FirmwareTableProviderSignature)
            break;                                                          // currently only support 'ACPI'

        if (fDSDT)
            FirmwareTableID = 'PCAF';                                       // patch FirmwareTableID to FACP == FADT

        //
        // get variadic arg parameters
        //
        pAddress = va_arg(ap, void*);
        ssdtinstance = va_arg(ap, int);

        if ('TDSS' != FirmwareTableID){
            ssdtinstance = 0;

            //printf("SSDT instance %d\n", ssdtinstance);
        }
        else {
            //printf("%.4s\n", true == fDSDT ? "DSDT" : (char*)(&FirmwareTableID));
        }

        //CDETRACE(("--> pEfiSystemTable->NumberOfTableEntries %zd\n", pEfiSystemTable->NumberOfTableEntries));

        for (i = 0; i < pEfiSystemTable->NumberOfTableEntries; i++)
        {
            int64_t qwSig;
            char* pStr8 = NULL;

            if (IsEqualGUID(&EfiAcpi20TableGuid, &pEfiSystemTable->ConfigurationTable[i].VendorGuid))
            {
                pRSD = pCfg[i].VendorTable;
                qwSig = pRSD->Signature;
                break;
            }
        }

        //CDETRACE(("--> pRSD %p\n", pRSD));

        if (NULL != pRSD) do
        {
            EFI_ACPI_SDT_HEADER* pXSDT = (void*)pRSD->XsdtAddress;
            EFI_ACPI_2_0_COMMON_HEADER* pTBLEnd = (void*)&(((char*)pXSDT)[pXSDT->Length]);
            EFI_ACPI_2_0_COMMON_HEADER** ppTBL = (void*)&(((char*)pXSDT)[sizeof(EFI_ACPI_SDT_HEADER)]);
            ptrdiff_t numTbl = pTBLEnd - (EFI_ACPI_2_0_COMMON_HEADER*)ppTBL, idxTbl;

            if ('TDSX' == FirmwareTableID) {
                pTbl = pXSDT;
                sizeTbl = pXSDT->Length;
                foundTbl = true;                                        // mark found flag and...
                break;
            }

            for (idxTbl = 0; idxTbl < numTbl; idxTbl++, ppTBL++)        // walk through all tables in the XSDT
            {                                                           //
                if (FirmwareTableID == (*ppTBL)->Signature)             // if signatur match...
                {                                                       //
                    if ('TDSS' == FirmwareTableID)                      // if SSDT also consider ...
                    {                                                   //
                        if (0 != ssdtinstance) {                        // ... multiple table instances
                            if (ssdtinstance >= 0)                      // if positive...
                                ssdtinstance--;                         //  ... decrement instance counter
                            continue;                                   //
                        }                                               //
                    }                                                   //
                                                                        //
                    if (false == fDSDT)                                 // check DSDT request
                    {                                                   // 
                        pTbl = *ppTBL;                                  // ... save table address
                        sizeTbl = (*ppTBL)->Length;                     // save table length
                        foundTbl = true;                                // mark found flag and...
                        break;// for()                                  // ...break for()
                    }                                                   //
                    else                                                //
                    {                                                   // DSDT request
                        EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE* pFADT//
                            = (void*) * ppTBL;                          //
                        uint64_t tmp64 = pFADT->Dsdt;                   // typecast 32 -> 64 1/2
                        EFI_ACPI_2_0_COMMON_HEADER* pDSDT=(void*)tmp64; // typecast 32 -> 64 2/2
                        pTbl = pDSDT;                                   //
                        sizeTbl = pDSDT->Length;                        // save table length
                        foundTbl = true;                                // mark found flag and...
                        break;// for()                                  // ...break for()
                    }                                                   //
                }                                                       // 
            }

        } while (0);

        if (true == foundTbl)
        {
            nRet = sizeTbl;
            if (sizeTbl <= BufferSize)
                if (NULL != pFirmwareTableBuffer) {
                    memcpy(pFirmwareTableBuffer, pTbl, (size_t)sizeTbl);
                    *pAddress = (uint64_t)pTbl;
                }
        }

    } while (0);

    return nRet ;
}