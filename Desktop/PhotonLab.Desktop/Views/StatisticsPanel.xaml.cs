/**************************************************************************************************
 * File         : StatisticsPanel.xaml.cs
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
 * Description  : UI panel for image statistical analysis.
 * Displays calculated metrics such as min, max, mean, and standard deviation,
 * providing users with quantitative insights into the intensity characteristics of the image.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-15   Nitish Singh     Initial implementation of statistics panel.
 **************************************************************************************************/

using System.Windows.Controls;

namespace PhotonLab.Desktop.Views.Panels;

public partial class StatisticsPanel : UserControl
{
    public StatisticsPanel()
    {
        InitializeComponent();
    }
}
