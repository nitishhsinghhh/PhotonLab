/**************************************************************************************************
 * File         : ImageProcessingService.cs
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
 * Description  : Orchestration layer for image processing workflows.
 * Bridges the gap between the application's session state and the high-performance
 * native engine, providing a clean API for UI-driven transformations.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial implementation of the processing orchestration service.
 **************************************************************************************************/

using PhotonLab.Core.Contracts;
using PhotonLab.Core.DTOs;
using System.Threading;
using System.Threading.Tasks;

namespace PhotonLab.Desktop.Services
{
    /// <summary>
    /// Coordinates image processing operations. 
    /// Dispatches work to the native processor while managing the state within the session service.
    /// </summary>
    public sealed class ImageProcessingService : IImageProcessingService
    {
        private readonly IImageProcessor _nativeProcessor;
        private readonly IImageSessionService _session;

        public ImageProcessingService(IImageProcessor nativeProcessor, IImageSessionService session)
        {
            _nativeProcessor = nativeProcessor;
            _session = session;
        }

        /// <summary>
        /// Applies gamma correction to the current working buffer.
        /// </summary>
        public async Task ApplyGammaAsync(double gamma, CancellationToken cancellationToken = default)
        {
            cancellationToken.ThrowIfCancellationRequested();

            await _nativeProcessor.ApplyGammaAsync(
                _session.WorkingPixels,
                _session.Width,
                _session.Height,
                gamma);
        }

        /// <summary>
        /// Applies window/level (contrast/brightness) adjustments to the current working buffer.
        /// </summary>
        public async Task ApplyWindowLevelAsync(int window, int level, CancellationToken cancellationToken = default) 
        {
            cancellationToken.ThrowIfCancellationRequested();

            await _nativeProcessor.ApplyWindowLevelAsync(
            _session.WorkingPixels,
            _session.Width,
            _session.Height,
            window,
            level);
        }

        /// <summary>
        /// Applies a median noise-reduction filter to the current working buffer.
        /// </summary>
        public async Task ApplyMedianAsync(CancellationToken cancellationToken = default)
        {
            cancellationToken.ThrowIfCancellationRequested();

            await _nativeProcessor.ApplyMedianAsync(
                _session.WorkingPixels,
                _session.Width,
                _session.Height);
        }

        /// <summary>
        /// Applies a sharpening filter to the current working buffer.
        /// </summary>
        public async Task ApplySharpenAsync(CancellationToken cancellationToken = default)
        {
            cancellationToken.ThrowIfCancellationRequested();

            await _nativeProcessor.ApplySharpenAsync(
                _session.WorkingPixels,
                _session.Width,
                _session.Height);
        }

        /// <summary>
        /// Discards pending modifications and restores the working buffer to the original state.
        /// </summary>
        public Task ResetAsync()
        {
            _session.Reset();
            return Task.CompletedTask;
        }

        public async Task<uint[]> CalculateHistogramAsync()
        {
            return await _nativeProcessor.CalculateHistogramAsync(_session.WorkingPixels);
        }

        public async Task<ImageStatistics> CalculateStatisticsAsync()
        {
            if (_session.Width <= 0 || _session.Height <= 0 || _session.WorkingPixels == null || _session.WorkingPixels.Length == 0)
            {
                throw new InvalidOperationException(
                    "No image loaded.");
            }

            return await _nativeProcessor.CalculateStatisticsAsync(
                _session.WorkingPixels);
        }

        public ushort[] GetCurrentPixels()
        {
            return _session.WorkingPixels;
        }
    }
}
