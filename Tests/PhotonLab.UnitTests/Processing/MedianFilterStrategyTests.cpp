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
/* 1.1        2026-06-15 Nitish Singh   Added benchmark tests        */ 
/*********************************************************************/

#include <gtest/gtest.h>
#include <random>
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

// ============================================================
// 3. BOUNDARY & ERROR HANDLING
// ============================================================

TEST(MedianFilterStrategyTests, HandlesSmallImageWithoutCrash)
{
    uint16_t pixels[] { 100 };

    PhotonLab::MedianFilterStrategy strategy;

    EXPECT_NO_THROW(strategy.Process(pixels, 1, 1));
}

// ============================================================
// 4. NULL POINTER HANDLING
// ============================================================

TEST(MedianFilterStrategyTests, HandlesNullPointer)
{
    PhotonLab::MedianFilterStrategy strategy;

    EXPECT_NO_THROW(
        strategy.Process(nullptr, 100, 100));
}

//  ============================================================
// 5. ZERO DIMENSION HANDLING
//  ============================================================

TEST(MedianFilterStrategyTests, HandlesZeroWidth)
{
    uint16_t pixel = 100;

    PhotonLab::MedianFilterStrategy strategy;

    EXPECT_NO_THROW(
        strategy.Process(&pixel, 0, 1));
}

//  ============================================================
// 6. ZERO HEIGHT HANDLING
//  ============================================================

TEST(MedianFilterStrategyTests, HandlesZeroHeight)
{
    uint16_t pixel = 100;

    PhotonLab::MedianFilterStrategy strategy;

    EXPECT_NO_THROW(
        strategy.Process(&pixel, 1, 0));
}

// ============================================================
// 7. NEGATIVE DIMENSION HANDLING
// ============================================================

TEST(MedianFilterStrategyTests, HandlesNegativeDimensions)
{
    uint16_t pixel = 100;

    PhotonLab::MedianFilterStrategy strategy;

    EXPECT_NO_THROW(
        strategy.Process(&pixel, -5, 10));

    EXPECT_NO_THROW(
        strategy.Process(&pixel, 10, -5));
}

// ============================================================
// 8. BORDER PIXELS REMAIN UNCHANGED
// ============================================================

TEST(MedianFilterStrategyTests, BorderPixelsRemainUnchanged)
{
    uint16_t pixels[]
    {
         1,  2,  3,
         4,100,  6,
         7,  8,  9
    };

    PhotonLab::MedianFilterStrategy strategy;

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
// 9. REMOVES SALT NOISE
// ============================================================

TEST(MedianFilterStrategyTests, RemovesSaltNoise)
{
    uint16_t pixels[]
    {
        50,50,50,
        50,65535,50,
        50,50,50
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(50,pixels[4]);
}

// ============================================================
// 10. REMOVES PEPPER NOISE
// ============================================================

TEST(MedianFilterStrategyTests, RemovesPepperNoise)
{
    uint16_t pixels[]
    {
        500,500,500,
        500,0,500,
        500,500,500
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(500,pixels[4]);
}

//  ============================================================
// 11. HANDLES DUPLICATE VALUES
//  ============================================================

TEST(MedianFilterStrategyTests, HandlesDuplicateValues)
{
    uint16_t pixels[]
    {
        10,10,20,
        20,20,30,
        30,30,30
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(20,pixels[4]);
}

// ============================================================
// 12. HANDLES ALREADY SORTED WINDOW
// ============================================================

TEST(MedianFilterStrategyTests, AlreadySortedWindow)
{
    uint16_t pixels[]
    {
        1,2,3,
        4,5,6,
        7,8,9
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(5,pixels[4]);
}

// ============================================================
// 13. HANDLES REVERSE SORTED WINDOW
// ============================================================

TEST(MedianFilterStrategyTests, ReverseSortedWindow)
{
    uint16_t pixels[]
    {
        9,8,7,
        6,5,4,
        3,2,1
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(5,pixels[4]);
}

// ============================================================
// 14. HANDLES RANDOM VALUES
// ============================================================

TEST(MedianFilterStrategyTests, ComputesMedianCorrectly)
{
    uint16_t pixels[]
    {
        20,10,40,
        90,50,80,
        60,30,70
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(50,pixels[4]);
}

//  ============================================================
// 15. HANDLES MAXIMUM PIXEL VALUES
//  ============================================================

TEST(MedianFilterStrategyTests, HandlesMaximumPixelValues)
{
    uint16_t pixels[]
    {
        UINT16_MAX,UINT16_MAX,UINT16_MAX,
        UINT16_MAX,100,UINT16_MAX,
        UINT16_MAX,UINT16_MAX,UINT16_MAX
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(UINT16_MAX,pixels[4]);
}

//  ============================================================
// 16. HANDLES MINIMUM PIXEL VALUES
//  ============================================================

TEST(MedianFilterStrategyTests, HandlesMinimumPixelValues)
{
    uint16_t pixels[]
    {
        0,0,0,
        0,500,0,
        0,0,0
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(0,pixels[4]);
}

//  ============================================================
// 17. REMOVES MULTIPLE IMPULSE NOISE
//  ============================================================

TEST(MedianFilterStrategyTests, RemovesMultipleImpulseNoise)
{
    uint16_t pixels[]
    {
        100,5000,100,
        5000,100,5000,
        100,5000,100
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels,3,3);

    EXPECT_EQ(100,pixels[4]);
}

// ============================================================
// 18. HANDLES LARGER IMAGE WITH IMPULSE NOISE
// ============================================================

TEST(MedianFilterStrategyTests, FiltersCenterPixelInLargerImage)
{
    constexpr int width = 5;
    constexpr int height = 5;

    uint16_t pixels[]
    {
        10,10,10,10,10,
        10,10,10,10,10,
        10,10,9999,10,10,
        10,10,10,10,10,
        10,10,10,10,10
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels,width,height);

    EXPECT_EQ(10,pixels[12]);
}

// ============================================================
// 19. PROCESSING TWICE PRODUCES SAME RESULT
// ============================================================

TEST(MedianFilterStrategyTests, ProcessingTwiceProducesSameResult)
{
    uint16_t pixels[]
    {
        100,100,100,
        100,5000,100,
        100,100,100
    };

    PhotonLab::MedianFilterStrategy strategy;

    strategy.Process(pixels,3,3);

    uint16_t first = pixels[4];

    strategy.Process(pixels,3,3);

    EXPECT_EQ(first,pixels[4]);
}

// ============================================================
// 20. RANDOM LARGE IMAGE DOES NOT THROW
// ============================================================

TEST(MedianFilterStrategyTests, RandomLargeImageDoesNotThrow)
{
    constexpr int width = 512;
    constexpr int height = 512;

    std::vector<uint16_t> pixels(width * height);

    std::mt19937 generator(42);

    std::uniform_int_distribution<uint16_t> distribution(
        0,
        UINT16_MAX);

    for (auto& pixel : pixels)
    {
        pixel = distribution(generator);
    }

    PhotonLab::MedianFilterStrategy strategy;

    EXPECT_NO_THROW(
        strategy.Process(
            pixels.data(),
            width,
            height));
}

// ============================================================
// 21. MEDIAN VALUE SELECTED CORRECTLY
// ============================================================

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
// 22. BENCHMARK FOR LARGE IMAGE
// ============================================================

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

// ============================================================
// 23. PERFORMANCE BENCHMARK
// ============================================================

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
