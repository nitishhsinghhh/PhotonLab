// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : IntegrationCollection.cs                             */
/* Author      : Nitish Singh                                         */
/* Created     : 2026-06-11                                           */
/* */
/* Copyright (c) 2026 Nitish Singh                                   */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information           */
/* */
/* Module      : Integration Tests                                    */
/* Component   : Test Lifecycle Infrastructure                        */
/* Thread Safe : No (test suite)                                      */
/* Complexity  : O(1) per lifecycle step                             */
/* API Status  : Stable                                               */
/* Exception Safety : N/A (test environment)                          */
/* */
/* Description : xUnit collection definition mapping the shared       */
/* IntegrationTestSetup fixture instance across test suites.           */
/* */
/* Test Groups :                                                     */
/* 1. Infrastructure Collection Mapping                               */
/* */
/* Notes       : Assures centralized sequential collection setup loops. */
/* */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date        Author         Description                 */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-11  Nitish Singh   Initial implementation      */
/*********************************************************************/

using Xunit;

namespace PhotonLab.DesktopIntegrationTests;

// ============================================================
// 1. INFRASTRUCTURE COLLECTION MAPPING
// ============================================================

[CollectionDefinition("PhotonLab Integration")]
public sealed class IntegrationCollection : ICollectionFixture<IntegrationTestSetup>
{
}
