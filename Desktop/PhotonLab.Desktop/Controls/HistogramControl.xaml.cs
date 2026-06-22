/**************************************************************************************************
 * File         : HistogramControl.xaml.cs
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
 * Description  : UI component for rendering image intensity histograms.
 * Provides a WPF-based visualization of frequency arrays, mapping intensity distribution
 * to a graphical bar representation on a canvas.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-13   Nitish Singh    Initial implementation of histogram UI control.
 **************************************************************************************************/

using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;

namespace PhotonLab.Desktop.Controls;

public partial class HistogramControl : UserControl
{
    public HistogramControl()
    {
        InitializeComponent();
    }

    public static readonly DependencyProperty HistogramProperty =
        DependencyProperty.Register(
            nameof(Histogram),
            typeof(uint[]),
            typeof(HistogramControl),
            new PropertyMetadata(null, OnHistogramChanged));

    public uint[] Histogram
    {
        get => (uint[])GetValue(HistogramProperty);
        set => SetValue(HistogramProperty, value);
    }

    private static void OnHistogramChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
        var control = (HistogramControl)d;
        control.DrawHistogram();
    }

    private void DrawHistogram()
    {
        HistogramCanvas.Children.Clear();

        if (Histogram == null || Histogram.Length == 0)
            return;

        double width = ActualWidth;
        double height = ActualHeight;

        if (width == 0 || height == 0)
            return;

        double max = 1;
        foreach (var v in Histogram)
            if (v > max) max = v;

        double barWidth = width / Histogram.Length;

        for (int i = 0; i < Histogram.Length; i++)
        {
            double normalized = Histogram[i] / max;
            double barHeight = normalized * height;

            var rect = new Rectangle
            {
                Width = barWidth,
                Height = barHeight,
                Fill = Brushes.White
            };

            Canvas.SetLeft(rect, i * barWidth);
            Canvas.SetTop(rect, height - barHeight);

            HistogramCanvas.Children.Add(rect);
        }
    }
}
