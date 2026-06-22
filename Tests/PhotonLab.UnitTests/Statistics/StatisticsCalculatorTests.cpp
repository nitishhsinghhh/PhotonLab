// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : StatisticsCalculatorTests.cpp                       */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Tests                                               */
/* Component   : Statistics Engine                                   */
/* Thread Safe : No (test suite)                                     */
/* Complexity  : O(n) per test case                                  */
/* API Status  : Stable                                              */
/* Exception Safety : N/A (test environment)                         */
/*                                                                   */
/* Description : Google Test suite validating statistical metrics    */
/* such as Min, Max, and Mean for image buffers.                     */
/*                                                                   */
/* Test Groups :                                                     */
/* 1. Range validation (Min/Max)                                     */
/* 2. Central tendency validation (Mean)                             */
/*                                                                   */
/* Notes       : - Validates statistical accuracy for 16-bit data    */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#include <gtest/gtest.h>
#include "Statistics/StatisticsCalculator.hpp"

// ============================================================
// 1. RANGE VALIDATION
// ============================================================

TEST(StatisticsCalculatorTests, CalculatesMinMax)
{
    uint16_t pixels[]
    {
        10,
        20,
        30
    };

    auto result =
        PhotonLab::StatisticsCalculator::
            Calculate(
                pixels,
                3);

    EXPECT_EQ(10, result.Min);
    EXPECT_EQ(30, result.Max);
}

// ============================================================
// 2. CENTRAL TENDENCY VALIDATION
// ============================================================

TEST(StatisticsCalculatorTests, CalculatesMean)
{
    uint16_t pixels[]
    {
        10,
        20,
        30
    };

    auto result =
        PhotonLab::StatisticsCalculator::
            Calculate(
                pixels,
                3);

    EXPECT_DOUBLE_EQ(
        20.0,
        result.Mean);
}
