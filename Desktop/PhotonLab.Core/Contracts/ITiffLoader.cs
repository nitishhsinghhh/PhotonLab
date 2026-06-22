/**************************************************************************************************
 * File         : ITiffLoader.cs
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
 * Description  : Core abstraction for TIFF file ingestion.
 * Defines the asynchronous contract for loading high-bit-depth TIFF images
 * into the application domain model.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial definition of TIFF loader contract.
 **************************************************************************************************/

using System.Threading;
using System.Threading.Tasks;
using PhotonLab.Core.DTOs;

namespace PhotonLab.Core.Contracts
{
    /// <summary>
    /// Defines the contract for services that ingest TIFF image files.
    /// </summary>
    public interface ITiffLoader
    {
        /// <summary>
        /// Asynchronously loads a TIFF file from the filesystem.
        /// </summary>
        /// <param name="filePath">The absolute path to the target TIFF file.</param>
        /// <param name="cancellationToken">A token to allow cancellation of the IO-intensive load operation.</param>
        /// <returns>A <see cref="LoadedImage"/> containing the image metadata and raw pixel buffer.</returns>
        Task<LoadedImage> LoadAsync(
            string filePath,
            CancellationToken cancellationToken = default);
    }
}
