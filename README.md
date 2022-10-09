# Visual-LIBWIN32-for-UEFI
![LOGO](visualWin32API4UefiWide.png)

	Copyright (c) 2021-2022, Kilian Kegel. All rights reserved.
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


## Revision history
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
    * [`Sleep()`]
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
