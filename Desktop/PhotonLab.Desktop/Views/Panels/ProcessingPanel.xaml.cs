/**************************************************************************************************
 * File         : ProcessingPanel.xaml.cs
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
 * Description  : UI panel for image processing operations.
 * Provides a control surface for triggering and configuring various image 
 * processing algorithms (filtering, adjustment, enhancement) on the active image.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-15   Nitish Singh     Initial implementation of processing panel.
 **************************************************************************************************/

using System.Windows.Controls;

namespace PhotonLab.Desktop.Views.Panels;

public partial class ProcessingPanel : UserControl
{
    public ProcessingPanel()
    {
        InitializeComponent();
    }
}
