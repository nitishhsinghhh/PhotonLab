/**************************************************************************************************
 * File         : ImageSessionService.cs
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
 * Description  : Manages the lifecycle and state of an image processing session.
 * Implements a primary source-of-truth copy and a mutable working buffer 
 * to enable non-destructive editing workflows.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial implementation for non-destructive session management.
 **************************************************************************************************/

using PhotonLab.Core.Contracts;
using PhotonLab.Core.DTOs;
using System;
using System.Linq;

namespace PhotonLab.Desktop.Services
{
    /// <summary>
    /// Provides session management for loaded image data.
    /// Maintains an immutable original source and a mutable working buffer
    /// to support features like undo, reset, and non-destructive filtering.
    /// </summary>
  
    public sealed class ImageSessionService : IImageSessionService
    {
        public bool HasImage => OriginalPixels.Length > 0;

        /// <summary>
        /// Gets the immutable reference copy of the source image pixels.
        /// </summary>
        public ushort[] OriginalPixels { get; private set; } = [];

        /// <summary>
        /// Gets the active buffer currently subjected to image processing transformations.
        /// </summary>
        public ushort[] WorkingPixels { get; private set; } = [];

        public int Width { get; private set; }
        public int Height { get; private set; }

        /// <summary>
        /// Initializes the session with image data.
        /// Performs deep copies to decouple the session state from the source stream.
        /// </summary>
        public void Load(ushort[] pixels, int width, int height)
        {
            Width = width;
            Height = height;

            // Perform deep copies to ensure original data remains preserved
            OriginalPixels = pixels.ToArray();
            WorkingPixels = pixels.ToArray();
        }

        /// <summary>
        /// Reverts the working buffer to the state of the original image.
        /// Useful for discarding pending edits or processing operations.
        /// </summary>
        public void Reset()
        {
            if (OriginalPixels.Length == 0) return;

            Array.Copy(
                OriginalPixels,
                WorkingPixels,
                OriginalPixels.Length);
        }
    }
}
