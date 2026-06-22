// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : WindowLevelStrategyTests.cpp                        */
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
/* Description : Google Test suite validating Window Leveling        */
/* logic, including clamping and buffer processing.                  */
/*                                                                   */
/* Test Groups :                                                     */
/* 1. Basic windowing logic                                          */
/* 2. Boundary clamping behavior                                     */
/* 3. Batch buffer processing                                        */
/*                                                                   */
/* Notes       : - Validates intensity windowing for 16-bit data     */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#include <gtest/gtest.h>

#include "Processing/WindowLevelStrategy.hpp"

TEST(WindowLevelTests, PixelInsideWindowIsScaled)
{
    uint16_t pixels[] { 900 };

    PhotonLab::WindowLevelStrategy strategy(1000, 1000);

    strategy.Process(pixels, 1, 1);

    EXPECT_GT(pixels[0], 0);
    EXPECT_LT(pixels[0], 65535);
}

TEST(WindowLevelTests, LowerValuesAreClamped)
{
    uint16_t pixels[] { 100 };

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    strategy.Process(pixels, 1, 1);

    EXPECT_EQ(0, pixels[0]);
}

TEST(WindowLevelTests, UpperValuesAreClamped)
{
    uint16_t pixels[] { 50000 };

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    strategy.Process(pixels, 1, 1);

    EXPECT_EQ(65535, pixels[0]);
}

TEST(WindowLevelTests, EntireBufferProcessed)
{
    uint16_t pixels[]
    {
        100,
        500,
        1000,
        1500,
        50000
    };

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    strategy.Process(pixels, 5, 1);

    EXPECT_EQ(0, pixels[0]);
    EXPECT_LT(pixels[1], 65535);
    EXPECT_GT(pixels[2], pixels[1]);
    EXPECT_LT(pixels[2], 65535);

    EXPECT_EQ(65535, pixels[3]);
    EXPECT_EQ(65535, pixels[4]);
}

// ============================================================
// 4. BENCHMARK
// ============================================================

TEST(WindowLevelTests, Benchmark1024x996Image)
{
    constexpr int width = 1024;
    constexpr int height = 996;
    constexpr int iterations = 100;

    std::vector<uint16_t> pixels(
        static_cast<size_t>(width) * height,
        30000);

    PhotonLab::WindowLevelStrategy strategy(
        20000,
        30000);

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
        << "Average execution time: "
        << (elapsed.count() /
            static_cast<double>(iterations))
        << " ms"
        << std::endl;
}
