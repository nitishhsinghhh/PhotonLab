/**************************************************************************************************
 * File         : HistogramService.cs
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
 * Description  : Service layer for image intensity frequency distribution analysis.
 * Facades the native histogram calculation engine to provide managed consumers with 
 * statistical frequency arrays for image contrast visualization.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial implementation of histogram service facade.
 **************************************************************************************************/

using System.Threading;
using System.Threading.Tasks;
using PhotonLab.Core.Contracts;

namespace PhotonLab.Desktop.Services;

public sealed class HistogramService : IHistogramService
{
    private readonly IImageProcessor _processor;

    public HistogramService(IImageProcessor processor)
    {
        _processor = processor;
    }

    public async Task<uint[]> CalculateHistogramAsync(
        ushort[] pixels,
        CancellationToken cancellationToken = default)
    {
        cancellationToken.ThrowIfCancellationRequested();

        return await _processor.CalculateHistogramAsync(pixels);
    }
}
