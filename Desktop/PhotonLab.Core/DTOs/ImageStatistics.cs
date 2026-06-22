/**************************************************************************************************
 * File         : ImageStatistics.cs
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
 * Description  : Data Transfer Object for image intensity statistics.
 * Encapsulates computed metrics (Min, Max, Mean) for 16-bit image buffers.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh     Initial definition of ImageStatistics DTO.
 **************************************************************************************************/

namespace PhotonLab.Core.DTOs
{
    /// <summary>
    /// Represents the statistical profile of an image buffer.
    /// </summary>
    public sealed class ImageStatistics
    {
        public ushort Min { get; set; }
        public ushort Max { get; set; }
        
        /// <summary>
        /// Gets or sets the arithmetic mean of pixel intensities in the buffer.
        /// </summary> 
        public double Mean { get; set; }

        public  double StandardDeviation { get; set; }

        public double Median { get; set; }

    }
}
