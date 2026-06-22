/**************************************************************************************************
 * File         : IImageRenderer.cs
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
 * Description  : Core abstraction for image visualization services.
 * Defines the contract for transforming high-bit-depth pixel buffers into 
 * renderable UI bitmaps for WPF presentation.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial definition of image renderer contract.
 **************************************************************************************************/

namespace PhotonLab.Core.Contracts
{
    /// <summary>
    /// Defines the contract for services that convert raw 16-bit image data
    /// into displayable visual elements for the WPF rendering pipeline.
    /// </summary>
    public interface IImageRenderer
    {
        /// <summary>
        /// Renders raw 16-bit intensity pixel data into a platform-agnostic 8-bit display byte buffer.
        /// </summary>
        byte[] RenderToDisplayBuffer(ushort[] pixels, int width, int height);
    }
}
