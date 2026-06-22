/**************************************************************************************************
 * File         : ImageViewerControl.xaml.cs
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
 * Description  : UI component for image display and manipulation.
 * Serves as the primary canvas for rendering image data, supporting interaction
 * and visual feedback for the PhotonLab desktop interface.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-13   Nitish Singh    Initial implementation of image viewer control.
 **************************************************************************************************/

using System.Windows.Controls;

namespace PhotonLab.Desktop.Controls;

public partial class ImageViewerControl : UserControl
{
    public ImageViewerControl()
    {
        InitializeComponent();
    }
}
