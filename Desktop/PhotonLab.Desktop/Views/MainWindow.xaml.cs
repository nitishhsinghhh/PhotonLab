/**************************************************************************************************
 * File         : MainWindow.xaml.cs
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
 * Description  : Main application shell.
 * Coordinates the primary workspace, hosting the ViewModels and managing 
 * top-level interactions, including viewport synchronization and lifecycle management.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-15   Nitish Singh     Initial implementation of the main application shell.
 **************************************************************************************************/

using PhotonLab.Desktop.ViewModels;
using System.Windows;

namespace PhotonLab.Desktop.Views;

public partial class MainWindow : Window
{
    public MainWindow(MainViewModel viewModel)
    {
        InitializeComponent();

        // Dependency Injection of the MainViewModel
        DataContext = viewModel;
    }

    private void OnViewerSizeChanged(object sender, SizeChangedEventArgs e)
    {
        // Updates the ViewModel viewport dimensions to ensure consistency 
        // with the actual UI rendering area.
        if (DataContext is MainViewModel vm)
        {
            vm.ViewportWidth = e.NewSize.Width;
            vm.ViewportHeight = e.NewSize.Height;
        }
    }
}
