/**************************************************************************************************
 * File         : CollectionDefinition.cs
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
 * Description  : Defines the shared test fixture collection for PhotonLab integration tests.
 * This ensures that the native interop environment is initialized once per test run.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh     Initial definition of the test collection fixture.
 **************************************************************************************************/

using Xunit;

namespace PhotonLab.DesktopIntegrationTests
{
    /// <summary>
    /// Defines the shared test fixture collection for PhotonLab integration tests.
    /// This prevents multiple initializations of the native interop environment, 
    /// significantly improving test suite execution time.
    /// </summary>
    [CollectionDefinition("PhotoLab Integration")]
    public class AlphaCollection : ICollectionFixture<IntegrationTestSetup>
    {
        // This class has no code, and is never created. Its purpose is simply
        // to be the place to apply [CollectionDefinition] and all the
        // ICollectionFixture<> interfaces.
    }
}
