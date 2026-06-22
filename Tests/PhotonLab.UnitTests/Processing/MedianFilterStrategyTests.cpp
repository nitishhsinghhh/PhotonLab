// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : MedianFilterStrategyTests.cpp                       */
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
/* Description : Google Test suite for validating MedianFilter       */
/* Strategy performance, including impulse noise removal, uniform    */
/* region preservation, and edge-case boundary safety.               */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#include <gtest/gtest.h>
#include "Processing/MedianFilterStrategy.hpp"

// ============================================================
// 1. IMPULSE NOISE REMOVAL VALIDATION
// ============================================================

TEST(MedianFilterStrategyTests, RemovesSingleImpulseNoise)
{
    uint16_t pixels[]
    {
        100, 100, 100,
        100, 5000, 100,
        100, 100, 100
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels, 3, 3);

    EXPECT_EQ(100, pixels[4]);
}

// ============================================================
// 2. SIGNAL INTEGRITY VALIDATION
// ============================================================

TEST(MedianFilterStrategyTests, PreservesUniformImage)
{
    uint16_t pixels[]
    {
        1000, 1000, 1000,
        1000, 1000, 1000,
        1000, 1000, 1000
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels, 3, 3);

    EXPECT_EQ(1000, pixels[4]);
}

TEST(MedianFilterStrategyTests, MedianValueSelectedCorrectly)
{
    uint16_t pixels[]
    {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels, 3, 3);

    EXPECT_EQ(5, pixels[4]);
}

// ============================================================
// 3. BOUNDARY & ERROR HANDLING
// ============================================================

TEST(MedianFilterStrategyTests, HandlesSmallImageWithoutCrash)
{
    uint16_t pixels[] { 100 };

    PhotonLab::MedianFilterStrategy strategy;

    EXPECT_NO_THROW(strategy.Process(pixels, 1, 1));
}


TEST(MedianFilterStrategyTests, Performance1024x996Image)
{
    constexpr int width = 1024;
    constexpr int height = 996;

    std::vector<uint16_t> pixels(width * height, 1000);

    PhotonLab::MedianFilterStrategy strategy;

    auto start = std::chrono::steady_clock::now();

    strategy.Process(pixels.data(), width, height);

    auto end = std::chrono::steady_clock::now();

    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start);

    EXPECT_LT(elapsed.count(), 1000);
}

TEST(MedianFilterStrategyTests, Benchmark1024x996Image)
{
    constexpr int width = 1024;
    constexpr int height = 996;
    constexpr int iterations = 10;

    std::vector<uint16_t> pixels(width * height, 1000);

    PhotonLab::MedianFilterStrategy strategy;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < iterations; ++i)
    {
        strategy.Process(
            pixels.data(),
            width,
            height);
    }

    auto end = std::chrono::steady_clock::now();

    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start);

    std::cout
        << "Average execution time: "
        << (elapsed.count() / iterations)
        << " ms"
        << std::endl;
}
