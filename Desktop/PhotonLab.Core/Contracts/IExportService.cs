/**************************************************************************************************
 * File         : IExportService.cs
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
 * Description  : Core abstraction for image export and persistence services.
 * Defines the asynchronous contract for serializing image data to disk,
 * supporting high-performance file IO with cancellation support.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial definition of export service contract.
 **************************************************************************************************/

using System.Threading;
using System.Threading.Tasks;

namespace PhotonLab.Core.Contracts
{
    /// <summary>
    /// Defines the contract for services that persist image data to the filesystem.
    /// </summary>
    public interface IExportService
    {
        /// <summary>
        /// Asynchronously encodes and saves image pixel data to a PNG file.
        /// </summary>
        /// <param name="filePath">The target filesystem destination.</param>
        /// <param name="pixels">The raw 16-bit pixel buffer to export.</param>
        /// <param name="width">Image width.</param>
        /// <param name="height">Image height.</param>
        /// <param name="cancellationToken">A token to allow cancellation of the export/IO operation.</param>
        /// <returns>A task representing the asynchronous save operation.</returns>
        void SavePng(
        byte[] pixelBuffer,
        int width,
        int height,
        string path);

        void SaveJpeg(
            byte[] pixelBuffer,
            int width,
            int height,
            string path,
            int quality = 90);

        void SaveTiff(
            byte[] pixelBuffer,
            int width,
            int height,
            string path);
    }
}
