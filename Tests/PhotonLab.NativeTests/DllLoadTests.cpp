// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : NativeInteropTests.cpp                              */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Tests                                               */
/* Component   : Cross-Platform Native Core Interop Architecture     */
/* Thread Safe : No (test suite)                                     */
/* Complexity  : O(1) per test case                                  */
/* API Status  : Stable                                              */
/* Exception Safety : N/A (test environment)                         */
/*                                                                   */
/* Description : Google Test suite validating dynamic library        */
/* loading and symbol export availability for interop.               */
/*                                                                   */
/* Test Groups :                                                     */
/* 1. Dynamic library load verification                              */
/* 2. Exported symbol discovery                                      */
/*                                                                   */
/* Notes       : - Tests ensure compatibility between the host       */
/* application and the native shared library.                        */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0      2026-06-10   Nitish Singh   Initial implementation       */
/*********************************************************************/

#include <gtest/gtest.h>

#ifdef _WIN32
    #include <windows.h>
    #include <string>
#else
    #include <dlfcn.h>
#endif

// Always define the TEST macros regardless of platform
// The logic inside will handle the platform differences
TEST(DllLoadTests, LibraryLoads) {
// In DllLoadTests.cpp
#ifdef _WIN32
    // No path prefix needed, it is in the same folder as the .exe
    HMODULE handle = LoadLibraryA("PhotonLab.NativeDLL.dll");
    if (!handle) {
        // This will now print the correct path if it fails
        FAIL() << "Could not load PhotonLab.NativeDLL.dll from current directory";
    }
#else
    void* handle = dlopen("./libPhotonLab.NativeDLL.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr) << "dlopen failed: " << dlerror();
    // ... rest of Linux/macOS logic
    dlclose(handle);
#endif
}

TEST(DllLoadTests, ApplyGammaExportExists) {
#ifdef _WIN32
    // Same dynamic path logic as above...
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string dir = std::string(buffer).substr(0, std::string(buffer).find_last_of("\\/"));
    
    HMODULE handle = LoadLibraryA((dir + "\\PhotonLab.NativeDLL.dll").c_str());
    ASSERT_NE(handle, nullptr);
    
    auto* symbol = GetProcAddress(handle, "ApplyGamma");
    ASSERT_NE(symbol, nullptr);
    FreeLibrary(handle);
#else
    // Linux/macOS implementation...
#endif
}
