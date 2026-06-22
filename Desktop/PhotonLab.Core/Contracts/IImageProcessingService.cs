/**************************************************************************************************
 * File         : IImageProcessingService.cs
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
 * Description  : Orchestration contract for high-level image processing workflows.
 * Abstracts complex native engine calls into a simplified, asynchronous API
 * for UI-driven non-destructive image adjustments.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial definition of image processing orchestration contract.
 **************************************************************************************************/

using PhotonLab.Core.Contracts;
using PhotonLab.Core.DTOs;
using System.Diagnostics.Metrics;
using System.Threading;
using System.Threading.Tasks;

namespace PhotonLab.Core.Contracts
{
    /// <summary>
    /// Provides an orchestration layer for performing image transformations.
    /// This service manages the interaction between the user-facing API and the
    /// native processing engine, maintaining consistent state throughout operations.
    /// </summary>
    public interface IImageProcessingService
    {
        /// <summary>
        /// Applies window/level adjustments for contrast and brightness enhancement.
        /// </summary>
        Task ApplyWindowLevelAsync(
            int window,
            int level,
            CancellationToken cancellationToken = default);

        /// <summary>
        /// Applies non-linear gamma correction to the image intensities.
        /// </summary>
        Task ApplyGammaAsync(
            double gamma,
            CancellationToken cancellationToken = default);

        /// <summary>
        /// Applies a non-linear median filter for noise reduction.
        /// </summary>
        Task ApplyMedianAsync(
            CancellationToken cancellationToken = default);

        /// <summary>
        /// Applies a sharpening filter to enhance edge definition.
        /// </summary>
        Task ApplySharpenAsync(
            CancellationToken cancellationToken = default);

        Task<ImageStatistics> CalculateStatisticsAsync();

        Task<uint[]> CalculateHistogramAsync();

        /// <summary>
        /// Reverts all processing changes and restores the image to the original source state.
        /// </summary>
        Task ResetAsync();

        /// <summary>
        /// Retrieves the current pixel buffer state after all applied transformations.
        /// </summary>
        ushort[] GetCurrentPixels();
    }
}
