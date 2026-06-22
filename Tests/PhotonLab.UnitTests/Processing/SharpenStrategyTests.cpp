// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : SharpenStrategyTests.cpp                            */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Tests                                               */
/* Component   : Processing Strategy Tests                           */
/* Thread Safe : No (test suite)                                     */
/* Complexity  : O(n) per test case                                  */
/* API Status  : Stable                                              */
/*                                                                   */
/* Description : Google Test suite for validating SharpenStrategy    */
/* performance, including edge enhancement, value clamping, and      */
/* boundary robustness.                                              */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#include <gtest/gtest.h>
#include <vector>
#include "Processing/SharpenStrategy.hpp"

// ============================================================
// 1. SIGNAL INTEGRITY VALIDATION
// ============================================================

TEST(SharpenStrategyTests, UniformImageRemainsUnchanged)
{
    uint16_t pixels[]
    {
        1000, 1000, 1000,
        1000, 1000, 1000,
        1000, 1000, 1000
    };

    PhotonLab::SharpenStrategy strategy;

    strategy.Process(pixels, 3, 3);

    EXPECT_EQ(1000, pixels[4]);
}

// ============================================================
// 2. EDGE ENHANCEMENT VALIDATION
// ============================================================

TEST(SharpenStrategyTests, EnhancesBrightCenterPixel)
{
    uint16_t pixels[]
    {
        1000, 1000, 1000,
        1000, 5000, 1000,
        1000, 1000, 1000
    };

    PhotonLab::SharpenStrategy strategy;

    strategy.Process(pixels, 3, 3);

    // Sharpening increases the center value relative to neighbors
    EXPECT_GT(pixels[4], 5000);
}

TEST(SharpenStrategyTests, ClampsToMaximum16BitValue)
{
    uint16_t pixels[]
    {
        0, 0, 0,
        0, 65535, 0,
        0, 0, 0
    };

    PhotonLab::SharpenStrategy strategy;

    strategy.Process(pixels, 3, 3);

    EXPECT_EQ(65535, pixels[4]);
}

// ============================================================
// 3. BOUNDARY & ERROR HANDLING
// ============================================================

TEST(SharpenStrategyTests, HandlesSmallImageWithoutCrash)
{
    uint16_t pixels[] { 100 };

    PhotonLab::SharpenStrategy strategy;

    EXPECT_NO_THROW(strategy.Process(pixels, 1, 1));
}

// ============================================================
// 4. BENCHMARK
// ============================================================

TEST(SharpenStrategyTests, Benchmark1024x996Image)
{
    constexpr int width = 1024;
    constexpr int height = 996;
    constexpr int iterations = 50;

    std::vector<uint16_t> pixels(
        static_cast<size_t>(width) * height,
        1000);

    PhotonLab::SharpenStrategy strategy;

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
        << "Average sharpen time: "
        << (elapsed.count() /
            static_cast<double>(iterations))
        << " ms"
        << std::endl;
}
