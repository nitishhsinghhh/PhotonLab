/**************************************************************************************************
 * File         : PixelInspectorControl.xaml.cs
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
 * Description  : UI component for precision pixel data inspection.
 * Allows users to probe specific coordinates within an image to retrieve raw
 * intensity values and metadata for technical analysis.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-13   Nitish Singh    Initial implementation of pixel inspector control.
 **************************************************************************************************/

using System.Windows.Controls;

namespace PhotonLab.Desktop.Controls;

public partial class PixelInspectorControl : UserControl
{
    public PixelInspectorControl()
    {
        InitializeComponent();
    }
}
