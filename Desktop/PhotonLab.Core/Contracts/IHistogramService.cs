/**************************************************************************************************
 * File         : IHistogramService.cs
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
 * Description  : Core abstraction for image histogram generation services.
 * Defines the asynchronous contract for frequency analysis of pixel intensity 
 * distribution within raw 16-bit buffers.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial definition of histogram service contract.
 **************************************************************************************************/

using System.Threading;
using System.Threading.Tasks;

namespace PhotonLab.Core.Contracts
{
    /// <summary>
    /// Defines the contract for services that calculate intensity distribution histograms
    /// for high-bit-depth image data.
    /// </summary>
    public interface IHistogramService
    {
        /// <summary>
        /// Calculates the frequency distribution of pixel intensities within the provided buffer.
        /// </summary>
        /// <param name="pixels">The raw 16-bit pixel data to analyze.</param>
        /// <param name="cancellationToken">A token to allow cancellation of the analysis operation.</param>
        /// <returns>An array representing the frequency count of each intensity level.</returns>
        Task<uint[]> CalculateHistogramAsync(
            ushort[] pixels,
            CancellationToken cancellationToken = default);
    }
}
