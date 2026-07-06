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
#include <random>
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

// ============================================================
// 3. BOUNDARY & ERROR HANDLING
// ============================================================

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
// 4. BOUNDARY & ERROR HANDLING
// ============================================================

TEST(SharpenStrategyTests, HandlesSmallImageWithoutCrash)
{
    uint16_t pixels[] { 100 };

    PhotonLab::SharpenStrategy strategy;

    EXPECT_NO_THROW(strategy.Process(pixels, 1, 1));
}

// ============================================================
// 5. BENCHMARK
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

// ============================================================
// 6. NULL POINTER HANDLING
// ============================================================

TEST(SharpenStrategyTests, HandlesNullPointer)
{
    PhotonLab::SharpenStrategy strategy;

    EXPECT_NO_THROW(
        strategy.Process(nullptr, 10, 10));
}

// ============================================================
// 7. DIMENSION HANDLING: Zero Width
// ============================================================

TEST(SharpenStrategyTests, HandlesZeroWidth)
{
    uint16_t pixel = 100;

    PhotonLab::SharpenStrategy strategy;

    EXPECT_NO_THROW(
        strategy.Process(&pixel, 0, 1));
}

// ============================================================
// 8. DIMENSION HANDLING: Zero Height
// ============================================================

TEST(SharpenStrategyTests, HandlesZeroHeight)
{
    uint16_t pixel = 100;

    PhotonLab::SharpenStrategy strategy;

    EXPECT_NO_THROW(
        strategy.Process(&pixel, 1, 0));
}

// ============================================================
// 9. DIMENSION HANDLING: Negative Width and Height
// ============================================================

TEST(SharpenStrategyTests, HandlesNegativeDimensions)
{
    uint16_t pixel = 100;

    PhotonLab::SharpenStrategy strategy;

    EXPECT_NO_THROW(
        strategy.Process(&pixel, -5, 5));

    EXPECT_NO_THROW(
        strategy.Process(&pixel, 5, -5));
}

// ============================================================
// 10. BORDER PIXELS REMAIN UNCHANGED
// ============================================================

TEST(SharpenStrategyTests, BorderPixelsRemainUnchanged)
{
    uint16_t pixels[]
    {
         1, 2, 3,
         4, 5, 6,
         7, 8, 9
    };

    PhotonLab::SharpenStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(1,pixels[0]);
    EXPECT_EQ(2,pixels[1]);
    EXPECT_EQ(3,pixels[2]);
    EXPECT_EQ(4,pixels[3]);
    EXPECT_EQ(6,pixels[5]);
    EXPECT_EQ(7,pixels[6]);
    EXPECT_EQ(8,pixels[7]);
    EXPECT_EQ(9,pixels[8]);
}

// ============================================================
// 11. NEGATIVE RESULT CLAMPING
// ============================================================

TEST(SharpenStrategyTests, ClampsNegativeResultToZero)
{
    uint16_t pixels[]
    {
        5000,5000,5000,
        5000,1000,5000,
        5000,5000,5000
    };

    PhotonLab::SharpenStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(0,pixels[4]);
}

//  ============================================================
// 12. HANDLES MAXIMUM VALUES
//  ============================================================

TEST(SharpenStrategyTests, HandlesMaximumValues)
{
    uint16_t pixels[]
    {
        UINT16_MAX,UINT16_MAX,UINT16_MAX,
        UINT16_MAX,UINT16_MAX,UINT16_MAX,
        UINT16_MAX,UINT16_MAX,UINT16_MAX
    };

    PhotonLab::SharpenStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(UINT16_MAX,pixels[4]);
}

//  ============================================================
// 13. HANDLES MINIMUM VALUES
//  ============================================================

TEST(SharpenStrategyTests, HandlesMinimumValues)
{
    uint16_t pixels[]
    {
        0,0,0,
        0,0,0,
        0,0,0
    };

    PhotonLab::SharpenStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(0,pixels[4]);
}

//  ============================================================
// 14. COMPUTES KERNEL CORRECTLY
//  ============================================================

TEST(SharpenStrategyTests, ComputesKernelCorrectly)
{
    uint16_t pixels[]
    {
        10,10,10,
        10,100,10,
        10,10,10
    };

    PhotonLab::SharpenStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(460,pixels[4]);
}

// ============================================================
// 15. PROCESSES CENTER PIXEL IN LARGER IMAGE
// ============================================================

TEST(SharpenStrategyTests, ProcessesCenterPixelInLargerImage)
{
    constexpr int width = 5;
    constexpr int height = 5;

    uint16_t pixels[]
    {
        100,100,100,100,100,
        100,100,100,100,100,
        100,100,200,100,100,
        100,100,100,100,100,
        100,100,100,100,100
    };

    PhotonLab::SharpenStrategy strategy;

    strategy.Process(pixels,width,height);

    EXPECT_GT(pixels[12],200);
}

// ============================================================
// 16. PRESERVES FLAT REGIONS
// ============================================================

TEST(SharpenStrategyTests, PreservesFlatRegions)
{
    constexpr int width = 5;
    constexpr int height = 5;

    std::vector<uint16_t> pixels(
        width * height,
        500);

    PhotonLab::SharpenStrategy strategy;

    strategy.Process(
        pixels.data(),
        width,
        height);

    for (auto value : pixels)
    {
        EXPECT_EQ(500,value);
    }
}

// ============================================================
// 17. REPEATED PROCESSING DOES NOT CRASH
// ============================================================

TEST(SharpenStrategyTests, RepeatedProcessingDoesNotCrash)
{
    constexpr int width = 64;
    constexpr int height = 64;

    std::vector<uint16_t> pixels(
        width * height,
        1000);

    PhotonLab::SharpenStrategy strategy;

    EXPECT_NO_THROW(
    {
        for(int i = 0; i < 100; ++i)
        {
            strategy.Process(
                pixels.data(),
                width,
                height);
        }
    });
}

// ============================================================
// 18. RANDOM IMAGE DOES NOT THROW
// ============================================================

TEST(SharpenStrategyTests, RandomImageDoesNotThrow)
{
    constexpr int width = 256;
    constexpr int height = 256;

    std::vector<uint16_t> pixels(width * height);

    std::mt19937 generator(42);

    std::uniform_int_distribution<uint16_t> distribution(
        0,
        UINT16_MAX);

    for (auto& pixel : pixels)
    {
        pixel = distribution(generator);
    }

    PhotonLab::SharpenStrategy strategy;

    EXPECT_NO_THROW(
        strategy.Process(
            pixels.data(),
            width,
            height));
}

// ============================================================
// 19. PERFORMANCE BENCHMARK
// ============================================================

TEST(SharpenStrategyTests, Performance1024x996Image)
{
    constexpr int width = 1024;
    constexpr int height = 996;

    std::vector<uint16_t> pixels(
        width * height,
        1000);

    PhotonLab::SharpenStrategy strategy;

    auto start =
        std::chrono::steady_clock::now();

    strategy.Process(
        pixels.data(),
        width,
        height);

    auto end =
        std::chrono::steady_clock::now();

    auto elapsed =
        std::chrono::duration_cast<
            std::chrono::milliseconds>(
                end - start);

    EXPECT_LT(elapsed.count(),1000);
}

// ============================================================
// 20. ONLY INTERIOR PIXELS ARE MODIFIED
// ============================================================

TEST(SharpenStrategyTests, OnlyInteriorPixelsAreModified)
{
    constexpr int width = 5;
    constexpr int height = 5;

    uint16_t pixels[]
    {
        1,1,1,1,1,
        1,2,2,2,1,
        1,2,100,2,1,
        1,2,2,2,1,
        1,1,1,1,1
    };

    PhotonLab::SharpenStrategy strategy;

    strategy.Process(pixels,width,height);

    EXPECT_EQ(1,pixels[0]);
    EXPECT_EQ(1,pixels[4]);
    EXPECT_EQ(1,pixels[20]);
    EXPECT_EQ(1,pixels[24]);

    EXPECT_GT(pixels[12],100);
}
