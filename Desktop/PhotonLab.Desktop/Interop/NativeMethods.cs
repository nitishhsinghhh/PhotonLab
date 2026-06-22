/**************************************************************************************************
 * File         : NativeMethods.cs
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
 * Description  : P/Invoke definitions for the native PhotonLab.Native C++ engine.
 * Maps managed .NET types to unmanaged memory buffers for hardware-accelerated
 * image processing and statistical analysis.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial P/Invoke mapping for native engine interop.
 **************************************************************************************************/

using System.Runtime.InteropServices;

namespace PhotonLab.Desktop.Interop
{
    /// <summary>
    /// Provides the bridge between the managed desktop application and the 
    /// unmanaged high-performance C++ image processing library.
    /// </summary>
    public static class NativeMethods
    {
        private const string DllName = "PhotonLab.NativeDLL";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int ApplyWindowLevel(
            ushort[] pixels,
            int width,
            int height,
            int window,
            int level);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int ApplyGamma(
            ushort[] pixels,
            int width,
            int height,
            double gamma);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int ApplyMedian(
            ushort[] pixels,
            int width,
            int height);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int ApplySharpen(
            ushort[] pixels,
            int width,
            int height);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int CalculateHistogram(
            ushort[] pixels,
            nuint count,
            uint[] histogram,
            nuint histogramSize);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int CalculateStatistics(
            ushort[] pixels,
            nuint count,
            out NativeStatisticsResult result);
    }
}
