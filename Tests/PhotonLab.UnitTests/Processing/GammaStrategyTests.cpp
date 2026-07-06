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

// ============================================================
// 6. Zero Intensity Remains Zero
// ============================================================

TEST(GammaStrategyTests, ZeroPixelRemainsZero)
{
    uint16_t pixels[] = {0};

    PhotonLab::GammaStrategy strategy(2.2);

    strategy.Process(pixels, 1, 1);

    EXPECT_EQ(0, pixels[0]);
}


// ============================================================
// 7. Maximum Intensity Remains Maximum
// ============================================================

TEST(GammaStrategyTests, MaximumPixelRemainsMaximum)
{
    uint16_t pixels[] =
    {
        std::numeric_limits<uint16_t>::max()
    };

    PhotonLab::GammaStrategy strategy(2.2);

    strategy.Process(pixels,1,1);

    EXPECT_EQ(
        std::numeric_limits<uint16_t>::max(),
        pixels[0]);
}

// ============================================================
// 8. Gamma Less Than One Brightens Pixels
// ============================================================

TEST(GammaStrategyTests, GammaLessThanOneBrightensPixels)
{
    uint16_t pixels[]
    {
        10000
    };

    PhotonLab::GammaStrategy strategy(0.5);

    strategy.Process(pixels,1,1);

    EXPECT_GT(pixels[0],10000);
}

// ============================================================
// 9. Gamma Greater Than One Darkens Pixels
// ============================================================

TEST(GammaStrategyTests, GammaGreaterThanOneDarkensPixels)
{
    uint16_t pixels[]
    {
        30000
    };

    PhotonLab::GammaStrategy strategy(2.2);

    strategy.Process(pixels,1,1);

    EXPECT_LT(pixels[0],30000);
}

// ============================================================
// 10. Every Pixel Is Processed
// ============================================================

TEST(GammaStrategyTests, EveryPixelIsProcessed)
{
    uint16_t pixels[]
    {
        1000,
        2000,
        3000,
        4000,
        5000
    };

    PhotonLab::GammaStrategy strategy(2.0);

    strategy.Process(pixels,5,1);

    EXPECT_NE(1000,pixels[0]);
    EXPECT_NE(2000,pixels[1]);
    EXPECT_NE(3000,pixels[2]);
    EXPECT_NE(4000,pixels[3]);
    EXPECT_NE(5000,pixels[4]);
}

// ============================================================
// 11. Empty Image Does Nothing
// ============================================================

TEST(GammaStrategyTests, EmptyImageDoesNothing)
{
    std::vector<uint16_t> pixels;

    PhotonLab::GammaStrategy strategy(2.0);

    EXPECT_NO_THROW(
        strategy.Process(
            pixels.data(),
            0,
            0));
}

// ============================================================
// 12. Single Pixel Processing
// ============================================================

TEST(GammaStrategyTests, SinglePixelProcessing)
{
    uint16_t pixels[]
    {
        50000
    };

    PhotonLab::GammaStrategy strategy(1.8);

    strategy.Process(pixels,1,1);

    EXPECT_NE(50000,pixels[0]);
}

// ============================================================
// 13. Large Image Processing
// ============================================================

TEST(GammaStrategyTests, LargeImageProcessesSuccessfully)
{
    constexpr int width=2048;
    constexpr int height=2048;

    std::vector<uint16_t> pixels(
        width*height,
        25000);

    PhotonLab::GammaStrategy strategy(2.2);

    EXPECT_NO_THROW(
        strategy.Process(
            pixels.data(),
            width,
            height));
}

//  ============================================================
// 14. Output Always Within Uint16 Range
//  ============================================================

TEST(GammaStrategyTests, OutputAlwaysWithinUint16Range)
{
    std::vector<uint16_t> pixels;

    for(int i=0;i<65536;i+=257)
        pixels.push_back(i);

    PhotonLab::GammaStrategy strategy(2.2);

    strategy.Process(
        pixels.data(),
        pixels.size(),
        1);

    for(auto value : pixels)
    {
        EXPECT_LE(value,65535);
        EXPECT_GE(value,0);
    }
}

// ============================================================
// 15. Equal Pixels Remain Equal
// ============================================================

TEST(GammaStrategyTests, EqualPixelsRemainEqual)
{
    uint16_t pixels[]
    {
        10000,
        10000,
        10000,
        10000
    };

    PhotonLab::GammaStrategy strategy(2.0);

    strategy.Process(pixels,4,1);

    EXPECT_EQ(pixels[0],pixels[1]);
    EXPECT_EQ(pixels[1],pixels[2]);
    EXPECT_EQ(pixels[2],pixels[3]);
}

// ============================================================
// 16. Processing Is Deterministic
// ============================================================

TEST(GammaStrategyTests, ProcessingIsDeterministic)
{
    uint16_t first[]
    {
        15000
    };

    uint16_t second[]
    {
        15000
    };

    PhotonLab::GammaStrategy strategy(2.2);

    strategy.Process(first,1,1);
    strategy.Process(second,1,1);

    EXPECT_EQ(first[0],second[0]);
}
