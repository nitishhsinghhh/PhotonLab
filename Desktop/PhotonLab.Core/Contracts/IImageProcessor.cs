/**************************************************************************************************
 * File         : IImageProcessor.cs
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
 * Description  : Core abstraction layer for high-performance 16-bit image processing operations.
 * Defines the contract for hardware-accelerated transformations and statistical 
 * analysis routines performed by the underlying native C++ engine.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial definition of image processing interface.
 **************************************************************************************************/

using System.Threading.Tasks;
using PhotonLab.Core.DTOs;

namespace PhotonLab.Core.Contracts
{
    /// <summary>
    /// Defines the contract for high-performance image processing operations,
    /// providing an abstraction over native compute implementations.
    /// </summary>
    public interface IImageProcessor
    {
        Task ApplyWindowLevelAsync(
            ushort[] pixels,
            int width,
            int height,
            int window,
            int level);

        Task ApplyGammaAsync(
            ushort[] pixels,
            int width,
            int height,
            double gamma);

        Task ApplyMedianAsync(
            ushort[] pixels,
            int width,
            int height);

        Task ApplySharpenAsync(
            ushort[] pixels,
            int width,
            int height);

        Task<uint[]> CalculateHistogramAsync(
            ushort[] pixels);

        Task<ImageStatistics> CalculateStatisticsAsync(
            ushort[] pixels);
    }
}
