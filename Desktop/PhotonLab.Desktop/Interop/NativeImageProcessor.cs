/**************************************************************************************************
 * File         : NativeImageProcessor.cs
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
 * Description  : Implementation of the native processing bridge.
 * Marshals managed C# calls into the high-performance C++ native engine via P/Invoke.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh     Initial implementation of the Native P/Invoke bridge.
 **************************************************************************************************/

using System;
using System.Threading.Tasks;
using PhotonLab.Core.Contracts;
using PhotonLab.Core.DTOs;

namespace PhotonLab.Desktop.Interop
{
    /// <summary>
    /// Implements the <see cref="IImageProcessor"/> contract by dispatching operations to the native C++ engine.
    /// </summary>
    public sealed class NativeImageProcessor : IImageProcessor
    {
        public async Task ApplyWindowLevelAsync(ushort[] pixels, int width, int height, int window, int level)
        {
            await Task.Run(() =>
            {
                int result =
                    NativeMethods.ApplyWindowLevel(
                        pixels,
                        width,
                        height,
                        window,
                        level);

                if (result != 0)
                {
                    throw new InvalidOperationException(
                        "Window/Level processing failed.");
                }
            });
        }

        public async Task ApplyGammaAsync(ushort[] pixels, int width, int height, double gamma)
        {
            await Task.Run(() => NativeMethods.ApplyGamma(pixels, width, height, gamma));
        }

        public async Task ApplyMedianAsync(ushort[] pixels, int width, int height)
        {
            await Task.Run(() => NativeMethods.ApplyMedian(pixels, width, height));
        }

        public async Task ApplySharpenAsync(ushort[] pixels, int width, int height)
        {
            await Task.Run(() => NativeMethods.ApplySharpen(pixels, width, height));
        }

        public async Task<uint[]> CalculateHistogramAsync(ushort[] pixels)
        {
            uint[] histogram = new uint[65536];
            await Task.Run(() => NativeMethods.CalculateHistogram(pixels, (nuint)pixels.Length, histogram, 65536));
            return histogram;
        }

        public async Task<ImageStatistics> CalculateStatisticsAsync(ushort[] pixels)
        {
            return await Task.Run(() =>
            {
                NativeMethods.CalculateStatistics(
                    pixels,
                    (nuint)pixels.Length,
                    out var native);

                return new ImageStatistics
                {
                    Min = native.Min,
                    Max = native.Max,
                    Mean = native.Mean,
                    Median = native.Median,
                    StandardDeviation = native.StandardDeviation
                };
            });
        }
    }
}
