/**************************************************************************************************
 * File         : NativeStructures.cs
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
 * Description  : Managed representations of unmanaged structures used by
 * PhotonLab.Native. Provides explicit memory layout to ensure
 * binary compatibility during P/Invoke marshaling.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial definition of interop structures.
 **************************************************************************************************/

using System.Runtime.InteropServices;

namespace PhotonLab.Desktop.Interop
{
    /// <summary>
    /// Managed equivalent of PhotonLab::StatisticsResult.
    /// <see cref="LayoutKind.Sequential"/> ensures the managed structure maps 
    /// directly to the memory layout expected by the native C++ library.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    internal struct NativeStatisticsResult
    {
        public ushort Min;

        public ushort Max;

        public double Mean;

        public double StandardDeviation;

        public double Median;

        /// <summary>
        /// Resets the statistical values to their default (zero) state.
        /// </summary>
        public void Reset()
        {
            Min = 0;
            Max = 0;
            Mean = 0.0;
            StandardDeviation = 0.0;
            Median = 0.0;
        }
    }
}
