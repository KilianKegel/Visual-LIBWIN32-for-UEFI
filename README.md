# Visual-LIBWIN32-for-UEFI
![LOGO](visualWin32API4UefiWide.png)

	Copyright (c) 2021-2025, Kilian Kegel. All rights reserved.
	SPDX-License-Identifier: GNU General Public License v3.0

Win32 API for UEFI Shell

## Goal
It provides Win32 API to enable porting of Windows projects to UEFI Shell.<br>
The function set is extended on demand only. It is not (yet) intended to provide 
the entire KERNEL32 API.

## Approach
Reimplement Win32 API based on Standard C Library ([Toro C Library](https://github.com/KilianKegel/toro-C-Library)) and UEFI API.

## HowTo
#### 1. install Visual Studio 2022 on a Windows PC<br>
https://github.com/KilianKegel/HowTo-setup-an-UEFI-Development-PC#howto-setup-an-uefi-development-pc

## Tool chain support
* **LLVM/CLANG:** NO
* **Microsoft v143, v142, v141:** YES

## Revision history
### 20240324
* add `RSMB`-support for [`GetSystemFirmwareTable()`](GetSystemFirmwareTable.c)
    * NOTE: The Windows-compatible implementation returns a pointer to [`RAWSMBIOSDATA`](https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemfirmwaretable#remarks).
      The true SMBIOS data is preceded by that structure.
```c
typedef struct _RAWSMBIOSDATA
{
    BYTE    Used20CallingMethod;
    BYTE    SMBIOSMajorVersion;
    BYTE    SMBIOSMinorVersion;
    BYTE    DmiRevision;
    DWORD   Length;
    BYTE    SMBIOSTableData[];
}RAWSMBIOSDATA;
```

### 20230114
* consolidate to **Toro C Library**
    - from now on use SystemTable and ImageHandle from **Toro C Library**
      `EFI_SYSTEM_TABLE* _cdegST`, `EFI_HANDLE _cdegImageHandle`
    - remove **extern** dependancy from `EFI_SYSTEM_TABLE* pEfiSystemTable`, `EFI_HANDLE hEfiImageHandle`
### 20221009
* add dedicated `Debug`/`Release` configurations
### 20221001
* simplify interaction of various **VISUAL-xyz** components, unitize directory structure 
  and import/export interface for those solutions
    * output folder for **Configuration Type: static Library** is commonly **$(SolutionPath)**\\***lib***
      to share one single *library folder* for all imported subprojects
    * adjust include path to be **$(ProjectDir)** relative
* add `WINAPI` interface for 
    * [`EnumSystemFirmwareTables()`](EnumSystemFirmwareTables.c)
    * [`GetSystemFirmwareTable()`](GetSystemFirmwareTable.c)
    * [`GetTickCount64()`](GetTickCount64.c)
    * [`IsBadReadPtr()`](IsBadReadPtr.c)
    * [`IsBadWritePtr()`](IsBadWritePtr.c)
    * [`QueryPerformanceCounter()`](QueryPerformanceCounter.c)
    * [`QueryPerformanceFrequency()`](QueryPerformanceFrequency.c)
    * [`Sleep()`](Sleep.c)
### 20220505
* suppress build warning *MSB8012* due to redirected library output path

### 20220503
* add functions:
   - [`CreateFileA()`](CreateFileA.c)
   - [`CreateFileW()`](CreateFileW.c)
   - [`CloseHandle()`](CloseHandle.c)
   - [`ReadFile()`](ReadFile.c)
   - [`WriteFile()`](WriteFile.c)
   - [`SetFilePointer()`](SetFilePointer.c)

### 20211101 alpha
* all projects build with minor compiler warnings
* all projects pass simple tests in the UEFI Shell
    * `AcpiDump.efi` dumps all ACPI tables
    * `AslCompiler.efi` processes [`badcode.asl`](https://github.com/RehabMan/Intel-iasl/blob/master/tests/misc/badcode.asl)
* further and comprehensive tests should be done by an ACPI expert
