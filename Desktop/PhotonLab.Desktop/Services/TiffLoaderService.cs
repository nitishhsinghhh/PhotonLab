/**************************************************************************************************
 * File         : TiffLoaderService.cs
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
 * Description  : Provides robust TIFF file ingestion services.
 * Implements scanline-by-scanline decompression to support memory-efficient
 * loading of 16-bit medical/scientific imaging formats.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial implementation for 16-bit TIFF stream parsing.
 **************************************************************************************************/

using System;
using System.Threading;
using System.Threading.Tasks;
using BitMiracle.LibTiff.Classic;
using PhotonLab.Core.Contracts;
using PhotonLab.Core.DTOs;

namespace PhotonLab.Desktop.Services;

public sealed class TiffLoaderService : ITiffLoader
{
    public Task<LoadedImage> LoadAsync(string filePath, CancellationToken cancellationToken = default)
    {
        if (string.IsNullOrWhiteSpace(filePath))
            throw new ArgumentException("File path cannot be empty.");

        using var image = Tiff.Open(filePath, "r");
        if (image == null)
            throw new Exception($"Failed to open TIFF file at: {filePath}");

        int width = image.GetField(TiffTag.IMAGEWIDTH)[0].ToInt();
        int height = image.GetField(TiffTag.IMAGELENGTH)[0].ToInt();

        ushort[] pixels = new ushort[width * height];
        byte[] scanline = new byte[image.ScanlineSize()];

        for (int row = 0; row < height; row++)
        {
            cancellationToken.ThrowIfCancellationRequested();

            image.ReadScanline(scanline, row);

            Buffer.BlockCopy(
                scanline,
                0,
                pixels,
                row * width * sizeof(ushort),
                width * sizeof(ushort));
        }

        return Task.FromResult(new LoadedImage
        {
            Width = width,
            Height = height,
            Pixels = pixels
        });
    }
}
