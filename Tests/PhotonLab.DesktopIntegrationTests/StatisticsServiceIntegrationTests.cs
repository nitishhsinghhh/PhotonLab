// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : StatisticsServiceIntegrationTests.cs                 */
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
/* Description : xUnit integration tests validating combined           */
/* metrics calculations from the desktop business logic layer        */
/* down through the native interop processor.                         */
/* */
/* Test Groups :                                                     */
/* 1. Frequency distribution accuracy                                 */
/* */
/* Notes       : Assures underlying math integrity for image extrema. */
/* */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author         Description                 */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-11  Nitish Singh   Initial implementation      */
/*********************************************************************/

using PhotonLab.Desktop.Services;
using PhotonLab.Desktop.Interop;
using Xunit;

namespace PhotonLab.DesktopIntegrationTests;

[Collection("PhotonLab Integration")]
public sealed class StatisticsServiceIntegrationTests
{
    private readonly IntegrationTestSetup _fixture;

    public StatisticsServiceIntegrationTests(IntegrationTestSetup fixture)
    {
        _fixture = fixture;
    }

    // ============================================================
    // 1. FREQUENCY DISTRIBUTION ACCURACY
    // ============================================================

    [Fact]
    public async Task Statistics_ShouldReturnExpectedValues()
    {
        var processor = new NativeImageProcessor();
        var service = new StatisticsService(processor);

        var stats = await service.CalculateStatisticsAsync(_fixture.SampleImage);

        Assert.Equal(_fixture.SampleImage[0], stats.Min);
        Assert.Equal(_fixture.SampleImage[^1], stats.Max);
    }
}
