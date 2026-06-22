// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ImageProcessingServiceTests.cs                       */
/* Author      : Nitish Singh                                         */
/* Created     : 2026-06-11                                           */
/* */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information           */
/* */
/* Module      : Integration Tests                                    */
/* Component   : Image Processing Pipeline                            */
/* Thread Safe : No (test suite)                                      */
/* Complexity  : O(n) per test case                                  */
/* API Status  : Stable                                               */
/* Exception Safety : N/A (test environment)                          */
/* */
/* Description : xUnit integration tests validating combined           */
/* operational workflows between image session buffers and native     */
/* interop adjustment algorithms.                                      */
/* */
/* Test Groups :                                                     */
/* 1. Buffer Pipeline Verification                                    */
/* */
/* Notes       : Validates downstream mutated state syncing over the   */
/* active session layout.                                             */
/* */
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
public sealed class ImageProcessingServiceTests
{
    private readonly IntegrationTestSetup _fixture;

    public ImageProcessingServiceTests(IntegrationTestSetup fixture)
    {
        _fixture = fixture;
    }

    // ============================================================
    // 1. BUFFER PIPELINE VERIFICATION
    // ============================================================

    [Fact]
    public async Task ApplyGamma_ShouldUpdateWorkingBuffer()
    {
        var processor = new NativeImageProcessor();
        var session = new ImageSessionService();

        session.Load(
            _fixture.SampleImage,
            _fixture.Width,
            _fixture.Height);

        var service = new ImageProcessingService(processor, session);

        ushort before = session.WorkingPixels[0];

        await service.ApplyGammaAsync(2.0);

        ushort after = session.WorkingPixels[0];

        Assert.NotEqual(before, after);
    }
}
