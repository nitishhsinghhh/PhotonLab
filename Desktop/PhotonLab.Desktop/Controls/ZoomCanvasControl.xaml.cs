/**************************************************************************************************
 * File         : ZoomCanvasControl.xaml.cs
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
 * Description  : UI component providing interactive zooming functionality.
 * Implements mouse-wheel interaction to manipulate a ScaleTransform, enabling 
 * dynamic scaling of child elements within the canvas.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-15   Nitish Singh    Initial implementation of zoom canvas control.
 **************************************************************************************************/

using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace PhotonLab.Desktop.Controls;

public partial class ZoomCanvasControl : UserControl
{
    private double _zoom = 1.0;

    public ZoomCanvasControl()
    {
        InitializeComponent();

        MouseWheel += OnMouseWheel;
    }

    private void OnMouseWheel(object sender, MouseWheelEventArgs e)
    {
        if (e.Delta > 0)
            _zoom *= 1.1;
        else
            _zoom /= 1.1;

        ScaleTransform.ScaleX = _zoom;
        ScaleTransform.ScaleY = _zoom;
    }
}
