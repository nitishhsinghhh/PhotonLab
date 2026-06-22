/**************************************************************************************************
 * File         : IntegrationTestSetup.cs
 *
 * Copyright    : (c) 2016–2026 Nitish Singh. All rights reserved.
 * License      : Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Description  : Shared test fixture for integration testing.
 * Provides a standardized environment, including the native processor and 
 * deterministic sample image data, for the integration test suite.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh     Initial setup of the integration test fixture.
 **************************************************************************************************/

using System;
using PhotonLab.Desktop.Interop;

namespace PhotonLab.DesktopIntegrationTests
{
    /// <summary>
    /// Provides shared state and dependencies for integration tests.
    /// This fixture is managed by the <see cref="AlphaCollection"/> to ensure 
    /// consistent state across the test lifecycle.
    /// </summary>
    public sealed class IntegrationTestSetup : IDisposable
    {
        public NativeImageProcessor Processor { get; } = new();

        /// <summary>
        /// A small sample buffer for basic transformation tests.
        /// </summary>
        public ushort[] SampleImage { get; } = new ushort[] { 100, 200, 300, 400 };
        
        /// <summary>
        /// A dedicated buffer for histogram validation.
        /// Expected distribution: Two 0s, Three 100s, One 500, One 1000, One 65535.
        /// </summary>
        public ushort[] HistogramImage { get; } = new ushort[] { 0, 0, 100, 100, 100, 500, 1000, 65535 };
        
        public int Width { get; } = 2;
        public int Height { get; } = 2;

        public IntegrationTestSetup()
        {
            // Initialization logic for native resources, if required.
        }

        public void Dispose()
        {
            // Cleanup logic for native resources, if required.
        }
    }
}
