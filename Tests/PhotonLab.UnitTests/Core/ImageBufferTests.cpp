// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ImageBufferTests.cpp                                */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Tests                                               */
/* Component   : Core Image Processing                               */
/* Thread Safe : No (test suite)                                     */
/* Complexity  : O(n) per test case                                  */
/* API Status  : Stable                                              */
/* Exception Safety : N/A (test environment)                         */
/*                                                                   */
/* Description : Google Test suite for validating ImageBuffer        */
/* lifecycle, including construction from data vectors and           */
/* raw pointer validation.                                           */
/*                                                                   */
/* Test Groups :                                                     */
/* 1. Default construction validation                                */
/* 2. Vector-based construction validation                           */
/* 3. Memory pointer integrity                                       */
/*                                                                   */
/* Notes       : - Ensures buffer dimensions and data access         */
/* integrity are preserved during construction.                      */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0      2026-06-10   Nitish Singh   Initial implementation       */
/*********************************************************************/

#include <gtest/gtest.h>
#include <vector>
#include <numeric>

#include "Core/ImageBuffer.hpp"
#include "Core/ImageBuffer.hpp"

// ============================================================
// 1. DEFAULT CONSTRUCTION VALIDATION
// ============================================================

TEST(ImageBufferTests, DefaultConstruction)
{
    PhotonLab::ImageBuffer buffer;

    EXPECT_EQ(0, buffer.Width());
    EXPECT_EQ(0, buffer.Height());
    EXPECT_EQ(0u, buffer.Size());
}

// ============================================================
// 2. VECTOR-BASED CONSTRUCTION VALIDATION
// ============================================================

TEST(ImageBufferTests, ConstructionWithPixels)
{
    std::vector<uint16_t> pixels
    {
        100,
        200,
        300,
        400
    };

    PhotonLab::ImageBuffer buffer(2, 2, pixels);

    EXPECT_EQ(2, buffer.Width());
    EXPECT_EQ(2, buffer.Height());
    EXPECT_EQ(4u, buffer.Size());
}

// ============================================================
// 3. MEMORY POINTER INTEGRITY
// ============================================================

TEST(ImageBufferTests, DataPointerIsValid)
{
    std::vector<uint16_t> pixels
    {
        10,
        20,
        30,
        40
    };

    PhotonLab::ImageBuffer buffer(2, 2, pixels);

    ASSERT_NE(nullptr, buffer.Data());

    EXPECT_EQ(10, buffer.Data()[0]);
    EXPECT_EQ(20, buffer.Data()[1]);
}

// ============================================================
// 4. EMPTY VECTOR CONSTRUCTION VALIDATION
// ============================================================

TEST(ImageBufferTests, ConstructionWithEmptyVector)
{
    std::vector<uint16_t> pixels;

    PhotonLab::ImageBuffer buffer(0, 0, pixels);

    EXPECT_EQ(0, buffer.Width());
    EXPECT_EQ(0, buffer.Height());
    EXPECT_EQ(0u, buffer.Size());

    EXPECT_EQ(buffer.Data(), pixels.data());
}

// ============================================================
// 5. SINGLE PIXEL IMAGE VALIDATION
// ============================================================

TEST(ImageBufferTests, SinglePixelImage)
{
    PhotonLab::ImageBuffer buffer(
        1,
        1,
        {65535}
    );

    EXPECT_EQ(1, buffer.Width());
    EXPECT_EQ(1, buffer.Height());
    EXPECT_EQ(1u, buffer.Size());

    ASSERT_NE(nullptr, buffer.Data());

    EXPECT_EQ(65535, buffer.Data()[0]);
}

// ============================================================
// 6. LARGE IMAGE CONSTRUCTION VALIDATION
// ============================================================

TEST(ImageBufferTests, LargeImageConstruction)
{
    constexpr int width = 1024;
    constexpr int height = 1024;

    std::vector<uint16_t> pixels(width * height, 42);

    PhotonLab::ImageBuffer buffer(width, height, pixels);

    EXPECT_EQ(width, buffer.Width());
    EXPECT_EQ(height, buffer.Height());
    EXPECT_EQ(width * height, buffer.Size());

    EXPECT_EQ(42, buffer.Data()[0]);
    EXPECT_EQ(42, buffer.Data()[500000]);
    EXPECT_EQ(42, buffer.Data()[buffer.Size() - 1]);
}

// ============================================================
// 7. VECTOR COPY SEMANTICS VALIDATION
// ============================================================

TEST(ImageBufferTests, ConstructorCopiesInputVector)
{
    std::vector<uint16_t> pixels
    {
        1,
        2,
        3,
        4
    };

    PhotonLab::ImageBuffer buffer(2, 2, pixels);

    pixels[0] = 999;

    EXPECT_EQ(1, buffer.Data()[0]);
}

// ============================================================
// 8. MUTABLE POINTER VALIDATION
// ============================================================

TEST(ImageBufferTests, MutablePointerAllowsModification)
{
    PhotonLab::ImageBuffer buffer(
        2,
        2,
        {1,2,3,4}
    );

    buffer.Data()[2] = 999;

    EXPECT_EQ(999, buffer.Data()[2]);
}

// ============================================================
// 9. CONST POINTER VALIDATION
// ============================================================

TEST(ImageBufferTests, ConstDataPointer)
{
    PhotonLab::ImageBuffer buffer(
        2,
        2,
        {5,6,7,8}
    );

    const PhotonLab::ImageBuffer& constBuffer = buffer;

    ASSERT_NE(nullptr, constBuffer.Data());

    EXPECT_EQ(5, constBuffer.Data()[0]);
    EXPECT_EQ(8, constBuffer.Data()[3]);
}

// ============================================================
// 10. DATA POINTER STABILITY VALIDATION
// ============================================================

TEST(ImageBufferTests, DataPointerRemainsStable)
{
    PhotonLab::ImageBuffer buffer(
        2,
        2,
        {1,2,3,4}
    );

    auto* ptr1 = buffer.Data();
    auto* ptr2 = buffer.Data();

    EXPECT_EQ(ptr1, ptr2);
}

// ============================================================
// 11. PIXEL ORDERING PRESERVATION VALIDATION
// ============================================================

TEST(ImageBufferTests, PixelOrderingPreserved)
{
    std::vector<uint16_t> pixels
    {
        10,
        20,
        30,
        40,
        50,
        60
    };

    PhotonLab::ImageBuffer buffer(3,2,pixels);

    for(size_t i=0;i<pixels.size();++i)
    {
        EXPECT_EQ(pixels[i], buffer.Data()[i]);
    }
}

// ============================================================
// 12. MAXIMUM UINT16 VALUE SUPPORT VALIDATION
// ============================================================

TEST(ImageBufferTests, SupportsMaximumUint16Value)
{
    PhotonLab::ImageBuffer buffer(
        1,
        2,
        {
            0,
            std::numeric_limits<uint16_t>::max()
        }
    );

    EXPECT_EQ(0, buffer.Data()[0]);
    EXPECT_EQ(
        std::numeric_limits<uint16_t>::max(),
        buffer.Data()[1]);
}

//  ============================================================
// 13. MOVE SEMANTICS VALIDATION
//  ============================================================

TEST(ImageBufferTests, ConstructionFromMovedVector)
{
    std::vector<uint16_t> pixels
    {
        1,
        2,
        3,
        4
    };

    auto originalPointer = pixels.data();

    PhotonLab::ImageBuffer buffer(
        2,
        2,
        std::move(pixels));

    EXPECT_EQ(originalPointer, buffer.Data());
}

//  ============================================================
// 14. BUFFER SIZE MATCHES EXPECTED IMAGE DIMENSIONS
//  ============================================================

TEST(ImageBufferTests, BufferSizeMatchesExpectedImageDimensions)
{
    constexpr int width = 5;
    constexpr int height = 7;

    PhotonLab::ImageBuffer buffer(
        width,
        height,
        std::vector<uint16_t>(width * height)
    );

    EXPECT_EQ(
        static_cast<size_t>(width * height),
        buffer.Size());
}

//  ============================================================
// 15. ITERATE ENTIRE BUFFER AND VALIDATE VALUES
//  ============================================================

TEST(ImageBufferTests, CanIterateEntireBuffer)
{
    std::vector<uint16_t> pixels(100);

    for(uint16_t i = 0; i < pixels.size(); ++i)
    {
        pixels[i] = i;
    }

    PhotonLab::ImageBuffer buffer(10,10,pixels);

    for(size_t i = 0; i < buffer.Size(); ++i)
    {
        EXPECT_EQ(i, buffer.Data()[i]);
    }
}

// ============================================================
// 16. MULTIPLE BUFFERS REMAIN INDEPENDENT
// ============================================================

TEST(ImageBufferTests, MultipleBuffersRemainIndependent)
{
    PhotonLab::ImageBuffer first(
        2,
        2,
        {1,2,3,4});

    PhotonLab::ImageBuffer second(
        2,
        2,
        {5,6,7,8});

    first.Data()[0] = 100;

    EXPECT_EQ(100, first.Data()[0]);
    EXPECT_EQ(5, second.Data()[0]);
}

// ============================================================
// 17. ZERO WIDTH BUFFER VALIDATION
// ============================================================

TEST(ImageBufferTests, ZeroWidthBuffer)
{
    PhotonLab::ImageBuffer buffer(
        0,
        5,
        {}
    );

    EXPECT_EQ(0, buffer.Width());
    EXPECT_EQ(5, buffer.Height());
    EXPECT_EQ(0u, buffer.Size());
}

// ============================================================
// 18. ZERO HEIGHT BUFFER VALIDATION
// ============================================================

TEST(ImageBufferTests, ZeroHeightBuffer)
{
    PhotonLab::ImageBuffer buffer(
        5,
        0,
        {}
    );

    EXPECT_EQ(5, buffer.Width());
    EXPECT_EQ(0, buffer.Height());
    EXPECT_EQ(0u, buffer.Size());
}

// ============================================================
// 19. MULTIPLE DATA CALLS RETURN SAME ADDRESS
// ============================================================

TEST(ImageBufferTests, MultipleDataCallsReturnSameAddress)
{
    PhotonLab::ImageBuffer buffer(
        2,
        2,
        {1,2,3,4}
    );

    EXPECT_EQ(buffer.Data(), buffer.Data());
    EXPECT_EQ(buffer.Data(), buffer.Data());
}

// ============================================================
// 20. DATA POINTER REFLECTS BUFFER CHANGES
// ============================================================

TEST(ImageBufferTests, DataPointerReflectsBufferChanges)
{
    PhotonLab::ImageBuffer buffer(
        2,
        2,
        {10,20,30,40}
    );

    uint16_t* ptr = buffer.Data();

    ptr[3] = 777;

    EXPECT_EQ(777, buffer.Data()[3]);
}

// ============================================================
// 21. DIMENSIONS REMAIN CONSTANT AFTER MODIFICATION
// ============================================================

TEST(ImageBufferTests, DimensionsRemainConstantAfterModification)
{
    PhotonLab::ImageBuffer buffer(
        2,
        3,
        {1,2,3,4,5,6}
    );

    buffer.Data()[0] = 999;

    EXPECT_EQ(2, buffer.Width());
    EXPECT_EQ(3, buffer.Height());
}

// ============================================================
// 22. BUFFER INITIALIZED WITH ZERO PIXELS
// ============================================================

TEST(ImageBufferTests, BufferInitializedWithZeroPixels)
{
    std::vector<uint16_t> pixels(256, 0);

    PhotonLab::ImageBuffer buffer(
        16,
        16,
        pixels
    );

    for (size_t i = 0; i < buffer.Size(); ++i)
    {
        EXPECT_EQ(0, buffer.Data()[i]);
    }
}

// ============================================================
// 23. BUFFER WITH ALTERNATING PIXEL VALUES
// ============================================================

TEST(ImageBufferTests, AlternatingPixelPatternPreserved)
{
    std::vector<uint16_t> pixels;

    for (int i = 0; i < 100; ++i)
    {
        pixels.push_back(i % 2 ? 65535 : 0);
    }

    PhotonLab::ImageBuffer buffer(
        10,
        10,
        pixels
    );

    for (size_t i = 0; i < buffer.Size(); ++i)
    {
        EXPECT_EQ(pixels[i], buffer.Data()[i]);
    }
}

// ============================================================
// 24. BUFFER WITH SEQUENTIAL PIXEL VALUES
// ============================================================

TEST(ImageBufferTests, SequentialPixelPatternPreserved)
{
    std::vector<uint16_t> pixels(512);

    std::iota(
        pixels.begin(),
        pixels.end(),
        0);

    PhotonLab::ImageBuffer buffer(
        32,
        16,
        pixels
    );

    for (size_t i = 0; i < buffer.Size(); ++i)
    {
        EXPECT_EQ(i, buffer.Data()[i]);
    }
}

// ============================================================
// 25. LARGE CONTIGUOUS BUFFER INTEGRITY
// ============================================================

TEST(ImageBufferTests, LargeContiguousBufferIntegrity)
{
    constexpr size_t pixelCount = 2'000'000;

    std::vector<uint16_t> pixels(pixelCount, 1234);

    PhotonLab::ImageBuffer buffer(
        2000,
        1000,
        pixels
    );

    EXPECT_EQ(pixelCount, buffer.Size());

    EXPECT_EQ(1234, buffer.Data()[0]);
    EXPECT_EQ(1234, buffer.Data()[999999]);
    EXPECT_EQ(1234, buffer.Data()[pixelCount - 1]);
}

//  ============================================================
// 26. MULTIPLE READS PRODUCE SAME RESULTS
//  ============================================================

TEST(ImageBufferTests, MultipleReadsProduceSameResults)
{
    PhotonLab::ImageBuffer buffer(
        2,
        2,
        {4,3,2,1}
    );

    EXPECT_EQ(4, buffer.Data()[0]);
    EXPECT_EQ(4, buffer.Data()[0]);
    EXPECT_EQ(4, buffer.Data()[0]);
}

// ============================================================
// 27. COPY CONSTRUCTION CREATES INDEPENDENT BUFFER
// ============================================================

TEST(ImageBufferTests, CopyConstructionCreatesIndependentBuffer)
{
    PhotonLab::ImageBuffer first(
        2,
        2,
        {1,2,3,4});

    PhotonLab::ImageBuffer second(first);

    second.Data()[0] = 999;

    EXPECT_EQ(1, first.Data()[0]);
    EXPECT_EQ(999, second.Data()[0]);
}

// ============================================================
// 28. COPY ASSIGNMENT CREATES INDEPENDENT BUFFER
// ============================================================

TEST(ImageBufferTests, CopyAssignmentCreatesIndependentBuffer)
{
    PhotonLab::ImageBuffer first(
        2,
        2,
        {1,2,3,4});

    PhotonLab::ImageBuffer second;

    second = first;

    second.Data()[1] = 777;

    EXPECT_EQ(2, first.Data()[1]);
    EXPECT_EQ(777, second.Data()[1]);
}

//  ============================================================
// 29. MOVE CONSTRUCTION TRANSFERS OWNERSHIP
//  ============================================================

TEST(ImageBufferTests, MoveConstructorTransfersOwnership)
{
    PhotonLab::ImageBuffer first(
        2,
        2,
        {1,2,3,4});

    auto* originalData = first.Data();

    PhotonLab::ImageBuffer second(std::move(first));

    EXPECT_EQ(originalData, second.Data());
}

//  ============================================================
// 30. MOVE ASSIGNMENT TRANSFERS OWNERSHIP
//  ============================================================

TEST(ImageBufferTests, MoveAssignmentTransfersOwnership)
{
    PhotonLab::ImageBuffer first(
        2,
        2,
        {10,20,30,40});

    auto* originalData = first.Data();

    PhotonLab::ImageBuffer second;

    second = std::move(first);

    EXPECT_EQ(originalData, second.Data());
}

//  ============================================================
// 31. PIXEL SUM REMAINS UNCHANGED AFTER BUFFER MODIFICATION
//  ============================================================

TEST(ImageBufferTests, PixelSumRemainsUnchanged)
{
    std::vector<uint16_t> pixels
    {
        10,
        20,
        30,
        40
    };

    PhotonLab::ImageBuffer buffer(
        2,
        2,
        pixels
    );

    uint32_t sum = 0;

    for (size_t i = 0; i < buffer.Size(); ++i)
    {
        sum += buffer.Data()[i];
    }

    EXPECT_EQ(100u, sum);
}
