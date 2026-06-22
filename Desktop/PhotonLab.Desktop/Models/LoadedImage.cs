/**************************************************************************************************
 * File         : LoadedImage.cs
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
 * Description  : Represents an immutable image snapshot loaded from disk, 
 * encapsulating its raw pixel data, spatial dimensions, and bit-depth metadata.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial definition of the LoadedImage domain model.
 **************************************************************************************************/

namespace PhotonLab.Desktop.Models
{
    /// <summary>
    /// Represents the immutable domain model for an image loaded within the application.
    /// Acts as a data container for raw sensor pixel data and structural metadata.
    /// </summary>
    public sealed class LoadedImage
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="LoadedImage"/> class.
        /// </summary>
        /// <param name="width">The width of the image in pixels.</param>
        /// <param name="height">The height of the image in pixels.</param>
        /// <param name="pixels">The 16-bit raw pixel array.</param>
        public LoadedImage(int width, int height, ushort[] pixels)
        {
            Width = width;
            Height = height;
            Pixels = pixels;
        }

        /// <summary> Gets the image width. </summary>
        public int Width { get; }

        /// <summary> Gets the image height. </summary>
        public int Height { get; }

        /// <summary> Gets the raw 16-bit pixel buffer. </summary>
        public ushort[] Pixels { get; }

        /// <summary> Calculates the total pixel count. </summary>
        public int PixelCount => Width * Height;

        /// <summary> Gets the bit depth of the loaded data (fixed at 16-bit for sensor compatibility). </summary>
        public int BitDepth => 16;
    }
}
