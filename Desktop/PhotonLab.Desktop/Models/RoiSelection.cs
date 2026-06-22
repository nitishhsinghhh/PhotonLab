/**************************************************************************************************
 * File         : RoiSelection.cs
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
 * Description  : Encapsulates a rectangular Region of Interest (ROI) selection
 * within the image coordinate space. Essential for targeted analysis or
 * partial-image processing workflows.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial definition of the ROI model.
 **************************************************************************************************/

namespace PhotonLab.Desktop.Models
{
    /// <summary>
    /// Represents a defined rectangular region of interest (ROI) within an image.
    /// Used to constrain analysis or processing to a specific subset of pixels.
    /// </summary>
    public sealed class RoiSelection
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="RoiSelection"/> class.
        /// </summary>
        /// <param name="x">The x-coordinate of the top-left corner.</param>
        /// <param name="y">The y-coordinate of the top-left corner.</param>
        /// <param name="width">The width of the rectangle.</param>
        /// <param name="height">The height of the rectangle.</param>
        public RoiSelection(int x, int y, int width, int height)
        {
            X = x;
            Y = y;
            Width = width;
            Height = height;
        }

        /// <summary> Gets the top-left X coordinate. </summary>
        public int X { get; }

        /// <summary> Gets the top-left Y coordinate. </summary>
        public int Y { get; }

        /// <summary> Gets the width of the ROI. </summary>
        public int Width { get; }

        /// <summary> Gets the height of the ROI. </summary>
        public int Height { get; }

        /// <summary> Gets the total area (in pixels) covered by the ROI. </summary>
        public int Area => Width * Height;

        /// <summary> Indicates if the selection has zero or negative dimensions. </summary>
        public bool IsEmpty => Width <= 0 || Height <= 0;
    }
}
