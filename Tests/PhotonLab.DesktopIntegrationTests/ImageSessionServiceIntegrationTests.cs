// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ImageSessionServiceTests.cs                          */
/* Author      : Nitish Singh                                         */
/* Created     : 2026-06-11                                           */
/* */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information           */
/* */
/* Module      : Integration Tests                                    */
/* Component   : Image Session Management                             */
/* Thread Safe : No (test suite)                                      */
/* Complexity  : O(n) per test case                                  */
/* API Status  : Stable                                               */
/* Exception Safety : N/A (test environment)                          */
/* */
/* Description : xUnit unit/integration tests validating state        */
/* lifecycle management, backup caching, and restoration flow within  */
/* the ImageSessionService instance.                                  */
/* */
/* Test Groups :                                                     */
/* 1. Session State Lifecycle                                         */
/* */
/* Notes       : Assures mutation isolation and data restoration loops.*/
/* */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author         Description                 */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-11  Nitish Singh   Initial implementation      */
/*********************************************************************/

using PhotonLab.Desktop.Services;
using Xunit;

namespace PhotonLab.DesktopIntegrationTests;

[Collection("PhotonLab Integration")]
public sealed class ImageSessionServiceTests
{
    private readonly IntegrationTestSetup _fixture;

    public ImageSessionServiceTests(IntegrationTestSetup fixture)
    {
        _fixture = fixture;
    }

    // ============================================================
    // 1. SESSION STATE LIFECYCLE
    // ============================================================

    [Fact]
    public void Reset_ShouldRestoreOriginalPixels()
    {
        var session = new ImageSessionService();

        session.Load(
            _fixture.SampleImage,
            _fixture.Width,
            _fixture.Height);

        session.WorkingPixels[0] = 999;

        session.Reset();

        Assert.Equal(_fixture.SampleImage[0], session.WorkingPixels[0]);
    }
}
