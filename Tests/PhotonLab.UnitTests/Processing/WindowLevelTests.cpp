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

// ============================================================
// 1. PIXEL INSIDE WINDOW IS SCALED
// ============================================================

TEST(WindowLevelTests, PixelInsideWindowIsScaled)
{
    uint16_t pixels[] { 900 };

    PhotonLab::WindowLevelStrategy strategy(1000, 1000);

    strategy.Process(pixels, 1, 1);

    EXPECT_GT(pixels[0], 0);
    EXPECT_LT(pixels[0], 65535);
}

// ============================================================
// 2. PIXEL OUTSIDE WINDOW IS CLAMPED
// ============================================================

TEST(WindowLevelTests, LowerValuesAreClamped)
{
    uint16_t pixels[] { 100 };

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    strategy.Process(pixels, 1, 1);

    EXPECT_EQ(0, pixels[0]);
}

//  ============================================================
// 3. PIXEL ABOVE WINDOW IS CLAMPED
// ============================================================

TEST(WindowLevelTests, UpperValuesAreClamped)
{
    uint16_t pixels[] { 50000 };

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    strategy.Process(pixels, 1, 1);

    EXPECT_EQ(65535, pixels[0]);
}

// ============================================================
// 4. ENTIRE BUFFER IS PROCESSED
// ============================================================

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
// 5. Benchmark for 1024x996 image
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

// ============================================================
// 6. NULL POINTER HANDLING
// ============================================================ 

TEST(WindowLevelTests, HandlesNullPointer)
{
    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    EXPECT_NO_THROW(
        strategy.Process(nullptr,1,1));
}

//  ============================================================
// 7. ZERO DIMENSION HANDLING
//  ============================================================

TEST(WindowLevelTests, HandlesZeroWidth)
{
    uint16_t pixel = 1000;

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    EXPECT_NO_THROW(
        strategy.Process(&pixel,0,1));
}

//  ============================================================
// 8. ZERO HEIGHT HANDLING
//  ============================================================

TEST(WindowLevelTests, HandlesZeroHeight)
{
    uint16_t pixel = 1000;

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    EXPECT_NO_THROW(
        strategy.Process(&pixel,1,0));
}

//  ============================================================
// 9. NEGATIVE DIMENSIONS HANDLING
//  ============================================================

TEST(WindowLevelTests, HandlesNegativeDimensions)
{
    uint16_t pixel = 1000;

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    EXPECT_NO_THROW(
        strategy.Process(&pixel,-1,10));

    EXPECT_NO_THROW(
        strategy.Process(&pixel,10,-1));
}

// ============================================================
// 10. LOWER BOUNDARY CLAMPS TO ZERO
// ============================================================

TEST(WindowLevelTests, LowerBoundaryClampsToZero)
{
    uint16_t pixels[]
    {
        500
    };

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    strategy.Process(pixels,1,1);

    EXPECT_EQ(0,pixels[0]);
}

// ============================================================
// 11. UPPER BOUNDARY CLAMPS TO MAXIMUM
// ============================================================

TEST(WindowLevelTests, UpperBoundaryClampsToMaximum)
{
    uint16_t pixels[]
    {
        1500
    };

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    strategy.Process(pixels,1,1);

    EXPECT_EQ(65535,pixels[0]);
}

// ============================================================
// 12. LEVEL MAPS NEAR HALF INTENSITY
// ============================================================

TEST(WindowLevelTests, LevelMapsNearHalfIntensity)
{
    uint16_t pixels[]
    {
        1000
    };

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    strategy.Process(pixels,1,1);

    EXPECT_NEAR(
        32767,
        pixels[0],
        1);
}

// ============================================================
// 13. LOWER HALF INTENSITY SCALED
// ============================================================

TEST(WindowLevelTests, LowerHalfIntensityScaled)
{
    uint16_t pixels[]
    {
        750
    };

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    strategy.Process(pixels,1,1);

    EXPECT_LT(
        pixels[0],
        32767);

    EXPECT_GT(
        pixels[0],
        0);
}

// ============================================================
// 14. UPPER HALF INTENSITY SCALED
// ============================================================

TEST(WindowLevelTests, UpperHalfIntensityScaled)
{
    uint16_t pixels[]
    {
        1250
    };

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    strategy.Process(pixels,1,1);

    EXPECT_GT(
        pixels[0],
        32767);

    EXPECT_LT(
        pixels[0],
        65535);
}

//  ============================================================
// 15. UNIFORM IMAGE PRODUCES UNIFORM OUTPUT
//  ============================================================
TEST(WindowLevelTests, UniformImageProducesUniformOutput)
{
    std::vector<uint16_t> pixels(100,1000);

    PhotonLab::WindowLevelStrategy strategy(
        1000,
        1000);

    strategy.Process(
        pixels.data(),
        10,
        10);

    for(auto pixel : pixels)
    {
        EXPECT_EQ(
            pixels.front(),
            pixel);
    }
}

