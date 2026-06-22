/**************************************************************************************************
 * File         : ExportService.cs
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
 * Description  : Provides cross-platform image persistence services for the application.
 * Handles the serialization of raw RGBA pixel display buffers into standard PNG files.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 **************************************************************************************************/

using PhotonLab.Core.Contracts;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats.Jpeg;
using SixLabors.ImageSharp.Formats.Tiff;
using SixLabors.ImageSharp.PixelFormats;
using System;
using System.IO;

namespace PhotonLab.Desktop.Services
{
    /// <summary>
    /// Handles the export of rendered pixel arrays to PNG, JPEG,
    /// and TIFF image formats.
    /// </summary>
    public sealed class ExportService: IExportService
    {
        /// <summary>
        /// Saves a raw 8-bit RGBA display buffer to the specified file path in PNG format.
        /// </summary>
        /// <param name="rgbaBuffer">The processed 8-bit display buffer array (4 bytes per pixel).</param>
        /// <param name="width">Width of the image.</param>
        /// <param name="height">Height of the image.</param>
        /// <param name="path">The destination filesystem path.</param>
        public void SavePng(byte[] rgbaBuffer, int width, int height, string path)
        {
            if (rgbaBuffer == null)
                throw new ArgumentNullException(nameof(rgbaBuffer));

            if (string.IsNullOrWhiteSpace(path))
                throw new ArgumentException("Export path cannot be null or empty.");

            // Wrap the raw RGBA display byte array into an ImageSharp Image surface object
            using Image<Rgba32> image = Image.LoadPixelData<Rgba32>(rgbaBuffer, width, height);
            
            // Save directly out to the filesystem using cross-platform codecs
            image.SaveAsPng(path);
        }

        /// <summary>
        /// Saves a raw 8-bit RGBA display buffer to the specified file path in JPEG format.
        /// </summary>
        /// <param name="pixelBuffer">
        /// The processed 8-bit display buffer array (4 bytes per pixel).
        /// </param>
        /// <param name="width">Width of the image.</param>
        /// <param name="height">Height of the image.</param>
        /// <param name="path">The destination filesystem path.</param>
        /// <param name="quality">
        /// JPEG quality level between 1 and 100.
        /// </param>
        public void SaveJpeg(byte[] pixelBuffer, int width, int height, string path, int quality = 90)
        {
            if (pixelBuffer == null)
                throw new ArgumentNullException(nameof(pixelBuffer));

            if (string.IsNullOrWhiteSpace(path))
                throw new ArgumentException(
                    "Export path cannot be null or empty.");

            using Image<Rgba32> image =
                Image.LoadPixelData<Rgba32>(
                    pixelBuffer,
                    width,
                    height);

            image.SaveAsJpeg(
                path,
                new JpegEncoder
                {
                    Quality = quality
                });
        }

        /// <summary>
        /// Saves a raw 8-bit RGBA display buffer to the specified file path in TIFF format.
        /// </summary>
        /// <param name="pixelBuffer">
        /// The processed 8-bit display buffer array (4 bytes per pixel).
        /// </param>
        /// <param name="width">Width of the image.</param>
        /// <param name="height">Height of the image.</param>
        /// <param name="path">The destination filesystem path.</param>
        public void SaveTiff(byte[] pixelBuffer, int width, int height, string path)
        {
            if (pixelBuffer == null)
                throw new ArgumentNullException(nameof(pixelBuffer));

            if (string.IsNullOrWhiteSpace(path))
                throw new ArgumentException(
                    "Export path cannot be null or empty.");

            using Image<Rgba32> image =
                Image.LoadPixelData<Rgba32>(
                    pixelBuffer,
                    width,
                    height);

            image.SaveAsTiff(
                path,
                new TiffEncoder());
        }
    }
}
