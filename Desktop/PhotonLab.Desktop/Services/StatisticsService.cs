/**************************************************************************************************
 * File         : StatisticsService.cs
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
 * Description  : Implementation of the statistics calculation service.
 * Bridges the UI requests for image metrics to the native processing engine.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh     Initial implementation of the statistics service.
 **************************************************************************************************/

using System.Threading;
using System.Threading.Tasks;
using PhotonLab.Core.Contracts;
using PhotonLab.Core.DTOs;

namespace PhotonLab.Desktop.Services
{
    /// <summary>
    /// Provides services to calculate intensity metrics for image buffers.
    /// </summary>
    public sealed class StatisticsService : IStatisticsService
    {
        private readonly IImageProcessor _processor;

        /// <summary>
        /// Initializes a new instance of the <see cref="StatisticsService"/> class.
        /// </summary>
        /// <param name="processor">The native processing engine abstraction.</param>
        public StatisticsService(IImageProcessor processor)
        {
            _processor = processor;
        }

        /// <inheritdoc />
        public async Task<ImageStatistics> CalculateStatisticsAsync(ushort[] pixels, CancellationToken cancellationToken = default)
        {
            // The processor handles the async offloading via Task.Run
            return await _processor.CalculateStatisticsAsync(pixels);
        }
    }
}
