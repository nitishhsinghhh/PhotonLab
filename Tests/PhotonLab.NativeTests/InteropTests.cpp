// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : InteropTests.cpp                                    */
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
/* Complexity  : O(n) per test case                                  */
/* API Status  : Stable                                              */
/* Exception Safety : N/A (test environment)                         */
/*                                                                   */
/* Description : Google Test suite validating C-style exports from   */
/* the native shared library, ensuring stable FFI.                   */
/*                                                                   */
/* Test Groups :                                                     */
/* 1. Gamma correction export functionality                          */
/* 2. Statistics calculation export functionality                    */
/*                                                                   */
/* Notes       : - Validates that the C-ABI boundary correctly       */
/* handles buffer data and output structs.                           */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0      2026-06-10   Nitish Singh   Initial implementation       */
/*********************************************************************/

#include <gtest/gtest.h>
#include "Interop/NativeExports.hpp"

// ============================================================
// 1. GAMMA CORRECTION EXPORT FUNCTIONALITY
// ============================================================

TEST(InteropTests, GammaExportChangesPixels)
{
    uint16_t pixels[]
    {
        1000,
        2000,
        3000
    };

    int status =
        ApplyGamma(
            pixels,
            3,
            1,
            2.0);

    ASSERT_EQ(0, status);

    EXPECT_NE(1000, pixels[0]);
}

// ============================================================
// 2. STATISTICS CALCULATION EXPORT FUNCTIONALITY
// ============================================================

TEST(InteropTests, StatisticsInteropWorks)
{
    uint16_t pixels[]
    {
        10,
        20,
        30
    };

    PhotonLab::StatisticsResult result {};

    int status =
        CalculateStatistics(
            pixels,
            3,
            &result);

    ASSERT_EQ(0, status);

    EXPECT_EQ(10, result.Min);
    EXPECT_EQ(30, result.Max);
}
