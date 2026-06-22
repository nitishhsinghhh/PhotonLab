// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : GammaStrategyTests.cpp                              */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Tests                                               */
/* Component   : Processing Engine                                   */
/* Thread Safe : No (test suite)                                     */
/* Complexity  : O(n) per test case                                  */
/* API Status  : Stable                                              */
/* Exception Safety : N/A (test environment)                         */
/*                                                                   */
/* Description : Google Test suite validating Gamma Correction       */
/* logic, ensuring correct intensity scaling.                        */
/*                                                                   */
/* Test Groups :                                                     */
/* 1. Identity transform verification                                */
/* 2. Non-linear mapping verification                                */
/*                                                                   */
/* Notes       : - Validates intensity adjustment for 16-bit data    */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0      2026-06-10   Nitish Singh   Initial implementation       */
/*********************************************************************/

#include <gtest/gtest.h>
#include "Processing/GammaStrategy.hpp"

// ============================================================
// 1. IDENTITY TRANSFORM VERIFICATION
// ============================================================

TEST(GammaStrategyTests, GammaOneProducesNoChange)
{
    uint16_t pixels[]
    {
        1000,
        2000,
        3000
    };

    PhotonLab::GammaStrategy strategy(1.0);

    strategy.Process(pixels, 3, 1);

    EXPECT_EQ(1000, pixels[0]);
    EXPECT_EQ(2000, pixels[1]);
    EXPECT_EQ(3000, pixels[2]);
}

// ============================================================
// 2. NON-LINEAR MAPPING VERIFICATION
// ============================================================

TEST(GammaStrategyTests, GammaChangesPixels)
{
    uint16_t pixels[]
    {
        1000,
        2000,
        3000
    };

    PhotonLab::GammaStrategy strategy(2.0);

    strategy.Process(pixels, 3, 1);

    EXPECT_NE(1000, pixels[0]);
}

// ============================================================
// 4. BENCHMARK
// ============================================================

TEST(GammaStrategyTests, Benchmark1024x996Image)
{
    constexpr int width = 1024;
    constexpr int height = 996;
    constexpr int iterations = 20;

    std::vector<uint16_t> pixels(
        static_cast<size_t>(width) * height,
        30000);

    PhotonLab::GammaStrategy strategy(2.0);

    auto start =
        std::chrono::steady_clock::now();

    for (int i = 0; i < iterations; ++i)
    {
        strategy.Process(
            pixels.data(),
            width,
            height);
    }

    auto end =
        std::chrono::steady_clock::now();

    auto elapsed =
        std::chrono::duration_cast<
            std::chrono::milliseconds>(
                end - start);

    std::cout
        << "Average gamma time: "
        << (elapsed.count() /
            static_cast<double>(iterations))
        << " ms"
        << std::endl;
}

// ============================================================
// 5. Invariant Property Testing
// ============================================================

TEST(GammaStrategyTests, GammaAlwaysPreservesMonotonicity)
{
    // Property: If x1 > x2, then f(x1) >= f(x2)
    std::vector<uint16_t> pixels = {1000, 5000, 10000, 20000, 60000};
    PhotonLab::GammaStrategy strategy(2.2);
    
    strategy.Process(pixels.data(), 5, 1);
    
    for (size_t i = 0; i < pixels.size() - 1; ++i) {
        EXPECT_GE(pixels[i+1], pixels[i]) << "Monotonicity violated at index " << i;
    }
}