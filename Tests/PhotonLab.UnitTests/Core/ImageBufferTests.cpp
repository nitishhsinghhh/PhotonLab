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
