// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : NativeImageProcessorIntegrationTests.cs             */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-11                                          */
/*                                                                   */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Integration Tests                                   */
/* Component   : Desktop Interop                                     */
/* Thread Safe : No (test suite)                                     */
/* Complexity  : O(n) per test case                                  */
/* API Status  : Stable                                              */
/* Exception Safety : N/A (test environment)                         */
/*                                                                   */
/* Description : xUnit integration tests validating the              */
/* Interop wrapper for native image processing and                   */
/* histogram operations.                                             */
/*                                                                   */
/* Test Groups :                                                     */
/* 1. Image Processing Operations                                    */
/* 2. Statistical Analysis                                           */
/*                                                                   */
/* Notes       : Validates both state modification and bin size      */
/* allocation across the Interop boundary.                           */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author         Description                 */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-11  Nitish Singh   Initial implementation      */
/*********************************************************************/

using System.Runtime.CompilerServices;
using PhotonLab.Desktop.Interop;
using Xunit;

namespace PhotonLab.DesktopIntegrationTests;

[Collection("PhotoLab Integration")]
public sealed class NativeImageProcessorIntegrationTests
{
    private readonly IntegrationTestSetup _fixture; private readonly NativeImageProcessor _processor;
    public NativeImageProcessorIntegrationTests(
        IntegrationTestSetup fixture)
    {
        _fixture = fixture; _processor = fixture.Processor;
    }

    // ============================================================
    // 1. IMAGE PROCESSING OPERATIONS
    // ============================================================

    [Fact]
    public async Task ApplyGamma_ShouldModifyPixels()
    {
        ushort[] pixels =
        {
            1000,
            2000,
            3000,
            4000
        };

        ushort[] original = pixels.ToArray();

        await _processor.ApplyGammaAsync(
            pixels,
            2,
            2,
            2.0);

        Assert.NotEqual(
            original,
            pixels);
    }

    // ============================================================
    // 2. STATISTICAL ANALYSIS
    // ============================================================

    [Fact]
    public async Task CalculateHistogram_ShouldReturn65536Bins()
    {
        ushort[] pixels =
        {
            0,
            100,
            200,
            65535
        };

        uint[] histogram = await _processor.CalculateHistogramAsync(pixels);

        Assert.Equal(
            65536,
            histogram.Length);
    }
}
