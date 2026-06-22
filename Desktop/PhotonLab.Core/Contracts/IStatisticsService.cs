/**************************************************************************************************
 * File         : IStatisticsService.cs
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
 * Description  : Statistical analysis abstraction for image buffers.
 * Defines the contract for computing essential metrics (min, max, mean) 
 * on 16-bit grayscale pixel data.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh     Initial definition of statistics service contract.
 **************************************************************************************************/

using System.Threading;
using System.Threading.Tasks;
using PhotonLab.Core.DTOs;

namespace PhotonLab.Core.Contracts
{
    /// <summary>
    /// Defines the contract for services that perform statistical analysis on image buffers.
    /// </summary>
    public interface IStatisticsService
    {
        /// <summary>
        /// Asynchronously calculates intensity statistics for the provided pixel buffer.
        /// </summary>
        /// <param name="pixels">The raw 16-bit pixel data buffer.</param>
        /// <param name="cancellationToken">A token to allow cancellation of the analysis operation.</param>
        /// <returns>A <see cref="ImageStatistics"/> object containing calculated metrics.</returns>
        Task<ImageStatistics> CalculateStatisticsAsync(
            ushort[] pixels,
            CancellationToken cancellationToken = default);
    }
}
