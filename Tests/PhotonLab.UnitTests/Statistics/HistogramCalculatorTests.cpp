// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : HistogramCalculatorTests.cpp                        */
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
/* Description : Google Test suite validating Histogram              */
/* calculation for 16-bit intensity distributions.                   */
/*                                                                   */
/* Test Groups :                                                     */
/* 1. Frequency distribution accuracy                                */
/* 2. Edge case handling (empty input)                               */
/*                                                                   */
/* Notes       : - Validates histogram binning for 16-bit space      */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#include <gtest/gtest.h>
#include "Statistics/HistogramCalculator.hpp"

// ============================================================
// 1. FREQUENCY DISTRIBUTION ACCURACY
// ============================================================

TEST(HistogramCalculatorTests, CountsPixelFrequency)
{
    uint16_t pixels[]
    {
        0,
        1,
        1,
        2
    };

    auto histogram =
        PhotonLab::HistogramCalculator::
            Calculate(
                pixels,
                4);

    EXPECT_EQ(1u, histogram[0]);
    EXPECT_EQ(2u, histogram[1]);
    EXPECT_EQ(1u, histogram[2]);
}

// ============================================================
// 2. EDGE CASE HANDLING
// ============================================================

TEST(HistogramCalculatorTests, EmptyInputProducesEmptyHistogram)
{
    auto histogram =
        PhotonLab::HistogramCalculator::
            Calculate(
                nullptr,
                0);

    EXPECT_EQ(
        65536u,
        histogram.size());
}
