// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : HistogramServiceIntegrationTests.cs                  */
/* Author      : Nitish Singh                                         */
/* Created     : 2026-06-11                                           */
/* */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information           */
/* */
/* Module      : Integration Tests                                    */
/* Component   : Statistics Engine                                    */
/* Thread Safe : No (test suite)                                      */
/* Complexity  : O(n) per test case                                  */
/* API Status  : Stable                                               */
/* Exception Safety : N/A (test environment)                          */
/* */
/* Description : xUnit integration tests validating histogram         */
/* generation distribution calculations from the desktop service      */
/* layer down through the native interop processor.                  */
/* */
/* Test Groups :                                                     */
/* 1. Frequency distribution accuracy                                 */
/* */
/* Notes       : Assures accurate bin count mapping across boundaries.*/
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author         Description                 */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-11  Nitish Singh   Initial implementation      */
/*********************************************************************/

using PhotonLab.Desktop.Interop;
using PhotonLab.Desktop.Services;
using Xunit;

namespace PhotonLab.DesktopIntegrationTests;

[Collection("PhotonLab Integration")]
public sealed class HistogramServiceIntegrationTests
{
    private readonly IntegrationTestSetup _fixture;

    public HistogramServiceIntegrationTests(
        IntegrationTestSetup fixture)
    {
        _fixture = fixture;
    }

    // ============================================================
    // 1. FREQUENCY DISTRIBUTION ACCURACY
    // ============================================================

    [Fact]
    public async Task Histogram_ShouldContainPixelCounts()
    {
        var processor =
            new NativeImageProcessor();

        var service =
            new HistogramService(
                processor);

        uint[] histogram =
    await service.CalculateHistogramAsync(
        _fixture.HistogramImage);

        Assert.Equal(
            (uint)2,
            histogram[0]);

        Assert.Equal(
            (uint)3,
            histogram[100]);

        Assert.Equal(
            (uint)1,
            histogram[500]);

        Assert.Equal(
            (uint)1,
            histogram[1000]);

        Assert.Equal(
            (uint)1,
            histogram[65535]);
    }

    [Fact]
    public async Task Histogram_ShouldReturn65536Bins()
    {
        var processor =
            new NativeImageProcessor();

        var service =
            new HistogramService(
                processor);

        uint[] histogram =
    await service.CalculateHistogramAsync(
        _fixture.HistogramImage);

        Assert.Equal(
            65536,
            histogram.Length);
    }
}
