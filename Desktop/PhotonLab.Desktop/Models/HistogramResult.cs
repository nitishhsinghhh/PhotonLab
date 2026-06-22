/**************************************************************************************************
 * File         : HistogramResult.cs
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
 * Description  : Represents the intensity distribution data generated from image analysis.
 * Provides analytical helpers to extract statistical information like peak frequency counts.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial definition of the HistogramResult model.
 **************************************************************************************************/

using System.Collections.Generic;
using System.Linq;

namespace PhotonLab.Desktop.Models
{
    /// <summary>
    /// Encapsulates the frequency distribution of pixel intensities for a given image.
    /// Used primarily for contrast visualization and auto-leveling algorithms.
    /// </summary>
    public sealed class HistogramResult
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="HistogramResult"/> class.
        /// </summary>
        /// <param name="bins">The read-only collection of intensity frequencies.</param>
        public HistogramResult(IReadOnlyList<uint> bins)
        {
            Bins = bins;
        }

        /// <summary> Gets the frequency count for each intensity level. </summary>
        public IReadOnlyList<uint> Bins { get; }

        /// <summary>
        /// Gets the maximum frequency count (peak) found across all intensity bins.
        /// Useful for normalizing histogram rendering scale.
        /// </summary>
        public uint PeakCount =>
            Bins.Count == 0
                ? 0
                : Bins.Max();
    }
}
