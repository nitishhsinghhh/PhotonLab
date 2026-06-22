/**************************************************************************************************
 * File         : ImageRenderingService.cs
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
 * Description  : Rendering bridge for UI display.
 * Converts 16-bit grayscale intensity data into an 8-bit-per-channel 
 * display-ready buffer (BGRA32).
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh     Initial implementation of the display renderer.
 **************************************************************************************************/

using System;
using PhotonLab.Core.Contracts;

namespace PhotonLab.Desktop.Services
{
    /// <summary>
    /// Provides services to transform raw 16-bit image data into display-ready formats.
    /// </summary>
    public sealed class ImageRenderingService : IImageRenderer
    {
        /// <summary>
        /// Converts a 16-bit grayscale pixel buffer to a 32-bit BGRA display buffer.
        /// </summary>
        /// <param name="pixels">The raw 16-bit source pixels.</param>
        /// <param name="width">Image width.</param>
        /// <param name="height">Image height.</param>
        /// <returns>An array of bytes representing the BGRA32 display buffer.</returns>
        public byte[] RenderToDisplayBuffer(ushort[] pixels, int width, int height)
        {
            // BGRA32: 4 bytes per pixel (B, G, R, A)
            byte[] displayBuffer = new byte[width * height * 4];
            
            for (int i = 0; i < pixels.Length; i++)
            {
                // Simple downsampling: shift 16-bit to 8-bit range
                byte intensity = (byte)(pixels[i] >> 8);
                int targetIdx = i * 4;
                
                displayBuffer[targetIdx]     = intensity; // Blue
                displayBuffer[targetIdx + 1] = intensity; // Green
                displayBuffer[targetIdx + 2] = intensity; // Red
                displayBuffer[targetIdx + 3] = 255;       // Alpha (Opaque)
            }
            
            return displayBuffer;
        }
    }
}
