/**************************************************************************************************
 * File         : IImageSessionService.cs
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
 * Description  : Core abstraction for image session state management.
 * Defines the contract for maintaining original vs. working pixel buffers,
 * enabling non-destructive editing and state rollback capabilities.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial definition of image session contract.
 **************************************************************************************************/

using PhotonLab.Core.DTOs;

namespace PhotonLab.Core.Contracts
{
    /// <summary>
    /// Defines the contract for managing the current image session, including 
    /// source-of-truth preservation and active working buffers.
    /// </summary>
    public interface IImageSessionService
    {
        /// <summary>
        /// Gets the immutable original pixels as loaded from the source file.
        /// </summary>
        ushort[] OriginalPixels { get; }

        /// <summary>
        /// Gets the mutable pixels currently subject to processing operations.
        /// </summary>
        ushort[] WorkingPixels { get; }

        /// <summary>
        /// Gets the width of the active image.
        /// </summary>
        int Width { get; }

        /// <summary>
        /// Gets the height of the active image.
        /// </summary>
        int Height { get; }

        /// <summary>
        /// Gets a value indicating whether an image is currently loaded in the session.
        /// </summary>
        bool HasImage { get; }

        /// <summary>
        /// Loads image data into the session and initializes the buffer state.
        /// </summary>
        /// <param name="pixels">The 16-bit pixel data array.</param>
        /// <param name="width">Image width.</param>
        /// <param name="height">Image height.</param>
        void Load(ushort[] pixels, int width, int height);

        /// <summary>
        /// Resets the working pixels to the original state.
        /// </summary>
        void Reset();
    }
}
