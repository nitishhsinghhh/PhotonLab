/**************************************************************************************************
 * File         : MainViewModel.cs
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
 * Description  : Primary MVVM presentation model for the PhotonLab desktop
 * application.
 *
 * Acts as the central orchestration layer between the user interface,
 * application services, image processing engine, rendering subsystem,
 * export pipeline, and image session state.
 *
 * Responsibilities:
 *   - TIFF image loading and initialization
 *   - Session lifecycle management
 *   - Image rendering and display updates
 *   - Metadata presentation
 *   - Statistical analysis presentation
 *   - Histogram integration support
 *   - Window/Level adjustments
 *   - Gamma correction
 *   - Median filtering
 *   - Sharpening operations
 *   - Image export workflows
 *   - Zoom and viewport management
 *   - Command binding for UI interactions
 *   - MVVM property notification management
 *
 * The ViewModel maintains synchronization between the application's
 * presentation layer and the underlying native image processing engine,
 * exposing an asynchronous, user-friendly API for diagnostic image
 * visualization and manipulation workflows.
 *
 * Architectural Role:
 *   UI Layer
 *      ↓
 *   MainViewModel
 *      ↓
 *   Service Layer
 *      ↓
 *   Native Processing Engine
 *      ↓
 *   PhotonLab.NativeDLL
 *
 * Key Features:
 *   - Asynchronous image processing
 *   - Non-blocking UI interactions
 *   - Real-time image refresh
 *   - Managed/unmanaged interoperability
 *   - Session-based image editing
 *   - Diagnostic metadata visualization
 *   - Statistical image analysis
 *   - Multi-format export support
 *
 * Threading Model:
 *   - UI-bound properties execute on the WPF Dispatcher thread.
 *   - Image processing operations execute asynchronously.
 *   - Native engine calls are delegated through service abstractions.
 *
 * Dependencies:
 *   - ITiffLoader
 *   - IImageRenderer
 *   - IExportService
 *   - IImageProcessingService
 *   - IImageSessionService
 *   - RelayCommand
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author          Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh    Initial MVVM implementation.
 * 1.1        2026-06-12   Nitish Singh    Added export workflow support.
 * 1.2        2026-06-13   Nitish Singh    Added metadata presentation layer.
 * 1.3        2026-06-13   Nitish Singh    Added image statistics integration.
 * 1.4        2026-06-14   Nitish Singh    Added Window/Level processing support.
 * 1.5        2026-06-14   Nitish Singh    Added session-based image processing orchestration.
 **************************************************************************************************/

using Microsoft.Extensions.Logging;
using Microsoft.Win32;
using OpenTelemetry.Trace;
using PhotonLab.Core.Contracts;
using PhotonLab.Core.DTOs;
using PhotonLab.Desktop.Commands;
using PhotonLab.Desktop.Services;
using System.ComponentModel;
using System.Diagnostics;
using System.Diagnostics.Metrics;
using System.IO;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Xml.Linq;

namespace PhotonLab.Desktop.ViewModels;

public sealed class MainViewModel : INotifyPropertyChanged
{
    public event PropertyChangedEventHandler? PropertyChanged;

    private void OnPropertyChanged([CallerMemberName] string? propertyName = null)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }

    private static readonly ActivitySource ActivitySource =
        new("PhotonLab.MainViewModel");

public RelayCommand OpenImageCommand { get; }
    public RelayCommand ExportCommand { get; }
    public RelayCommand ExportPngCommand { get; }
    public RelayCommand ExportJpegCommand { get; }
    public RelayCommand ExportTiffCommand { get; }
    public RelayCommand ExitCommand { get; }

    public RelayCommand ResetZoomCommand { get; }
    public RelayCommand FitToScreenCommand { get; }

    public RelayCommand ApplyGammaCommand { get; }
    public RelayCommand ApplyMedianCommand { get; }
    public RelayCommand ApplySharpenCommand { get; }
    public RelayCommand ApplyWindowLevelCommand { get; }
    public RelayCommand ShowAboutDialogCommand { get; }
    public RelayCommand RefreshHistogramCommand { get; }

    private string _imageDimensions = "No Image";
    public string ImageDimensions
    {
        get => _imageDimensions;
        set
        {
            if (_imageDimensions != value)
            {
                _imageDimensions = value;
                OnPropertyChanged();
            }
        }
    }

    private string _bitDepth = "-";
    public string BitDepth
    {
        get => _bitDepth;
        set
        {
            if (_bitDepth != value)
            {
                _bitDepth = value;
                OnPropertyChanged();
            }
        }
    }

    private string _zoomPercentage = "100%";
    public string ZoomPercentage
    {
        get => _zoomPercentage;
        set
        {
            if (_zoomPercentage != value)
            {
                _zoomPercentage = value;
                OnPropertyChanged();
            }
        }
    }

    private string _cursorPosition = "(0,0)";
    public string CursorPosition
    {
        get => _cursorPosition;
        set
        {
            if (_cursorPosition != value)
            {
                _cursorPosition = value;
                OnPropertyChanged();
            }
        }
    }

    private string _pixelValue = "-";
    public string PixelValue
    {
        get => _pixelValue;
        set
        {
            if (_pixelValue != value)
            {
                _pixelValue = value;
                OnPropertyChanged();
            }
        }
    }

    private BitmapSource? _imageSource;

    public BitmapSource? ImageSource
    {
        get => _imageSource;
        set
        {
            _imageSource = value;
            OnPropertyChanged();
        }
    }


    /// <summary>
    /// Initializes a new instance of the MainViewModel and wires all
    /// application services, command handlers, and presentation state.
    /// </summary>
    /// 
    private readonly IImageRenderer _renderer;
    private readonly ITiffLoader _tiffLoader;
    private LoadedImage? _currentImage;
    private readonly IExportService _exportService;
    private readonly IImageProcessingService _processingService;
    private readonly IImageSessionService _session;
    private readonly ILogger<MainViewModel> _logger;

    public MainViewModel(
        ITiffLoader tiffLoader, 
        IImageRenderer renderer, 
        IExportService exportService, 
        IImageProcessingService processingService, 
        IImageSessionService session, 
        ILogger<MainViewModel> logger)
    {
        _tiffLoader = tiffLoader;
        _renderer = renderer;
        _exportService = exportService;
        _processingService = processingService;
        _session = session;
        _logger = logger;

        OpenImageCommand = new RelayCommand(OpenImage);
        ExportCommand = new RelayCommand(Export);
        ExportPngCommand = new RelayCommand(ExportPng);
        ExportJpegCommand = new RelayCommand(ExportJpeg);
        ExportTiffCommand = new RelayCommand(ExportTiff);

        ExitCommand = new RelayCommand(Exit);

        ResetZoomCommand = new RelayCommand(ResetZoom);
        FitToScreenCommand = new RelayCommand(FitToScreen);

        ApplyGammaCommand = new RelayCommand(ApplyGamma);
        ApplyMedianCommand = new RelayCommand(ApplyMedian);
        ApplySharpenCommand = new RelayCommand(ApplySharpen);
        ApplyWindowLevelCommand = new RelayCommand(ApplyWindowLevel);

        ShowAboutDialogCommand = new RelayCommand(ShowAboutDialog);

        RefreshHistogramCommand = new RelayCommand(
            async () => await ExecuteAsync("Updating Histogram", RefreshHistogram),
            () => _currentImage != null // Only allow if an image is loaded
        );
    }

    /// <summary>
    /// Creates a WPF bitmap source from a BGRA32 display buffer generated
    /// by the rendering subsystem.
    /// </summary>
    /// <param name="displayBuffer">
    /// Rendered BGRA32 pixel buffer.
    /// </param>
    /// <param name="width">
    /// Image width in pixels.
    /// </param>
    /// <param name="height">
    /// Image height in pixels.
    /// </param>
    /// <returns>
    /// BitmapSource suitable for WPF image presentation.
    /// </returns>
    /// 
    private BitmapSource CreateBitmapSource(
    byte[] displayBuffer,
    int width,
    int height)
    {
        return BitmapSource.Create(
            width,
            height,
            96,
            96,
            PixelFormats.Bgra32,
            null,
            displayBuffer,
            width * 4);
    }

    private string _fileName = "-";
    public string FileName
    {
        get => _fileName;
        set
        {
            _fileName = value;
            OnPropertyChanged();
        }
    }

    private string _fileSize = "-";
    public string FileSize
    {
        get => _fileSize;
        set
        {
            _fileSize = value;
            OnPropertyChanged();
        }
    }

    private string _pixelFormat = "-";
    public string PixelFormat
    {
        get => _pixelFormat;
        set
        {
            _pixelFormat = value;
            OnPropertyChanged();
        }
    }

    private int _width;
    public int Width
    {
        get => _width;
        set
        {
            _width = value;
            OnPropertyChanged();
        }
    }

    private int _height;
    public int Height
    {
        get => _height;
        set
        {
            _height = value;
            OnPropertyChanged();
        }
    }


    /// <summary>
    /// Opens a TIFF image from disk, initializes the processing session,
    /// generates the display buffer, updates image metadata, and refreshes
    /// statistical information displayed within the application.
    /// </summary>
    /// 
    private byte[]? _displayBuffer;

    private async void OpenImage()
    {
        using var activity = StartTrace("OpenImage");

        _logger.LogInformation("Opening image from dialog...");

        var dialog = new OpenFileDialog
        {
            Filter = "TIFF Files|*.tif;*.tiff"
        };

        if (dialog.ShowDialog() != true)
            return;

        if (_currentImage != null)
        {
            var result = MessageBox.Show(
                "A image is already loaded.\n\nLoading a new image will discard the current session, including:\n" +
                "- Applied filters\n- Window/Level changes\n- Histogram and statistics\n\nDo you want to continue?",
                "Confirm Load New Image",
                MessageBoxButton.YesNo,
                MessageBoxImage.Warning,
                MessageBoxResult.No);

            if (result != MessageBoxResult.Yes)
            {
                _logger.LogInformation("User cancelled image reload.");
                return;
            }
        }

        activity?.SetTag("file.name", dialog.FileName);

        var image =
            await _tiffLoader.LoadAsync(
                dialog.FileName);

        activity?.SetTag("image.width", image.Width);
        activity?.SetTag("image.height", image.Height);

        _currentImage = image;

        // Load into processing session
        _session.Load(
            image.Pixels,
            image.Width,
            image.Height);

        _displayBuffer =
         _renderer.RenderToDisplayBuffer(
             image.Pixels,
             image.Width,
             image.Height);

        ImageSource =
            CreateBitmapSource(
                _displayBuffer,
                image.Width,
                image.Height);

        ImageDimensions =
            $"{image.Width} × {image.Height}";

        var fileInfo = new FileInfo(dialog.FileName);

        Width = image.Width;

        Height = image.Height;

        FileName = fileInfo.Name;

        FileSize =
            $"{fileInfo.Length / 1024.0:F2} KB";

        PixelFormat =
            "Grayscale 16-bit";

        ImageDimensions =
            $"{image.Width} × {image.Height}";

        BitDepth =
            "16-bit";

        _logger.LogInformation(
            "Image loaded: {FileName}, {Width}x{Height}, Size: {FileSize}",
            FileName,
            Width,
            Height,
            FileSize);

        await RefreshStatistics();
    }

    /// <summary>
    /// Exports the currently displayed image using the specified file format.
    /// Supports PNG, JPEG, and TIFF output encodings.
    /// </summary>
    /// <param name="extension">
    /// File extension identifying the target image format.
    /// </param>
    /// 
    private void ExportPng()
    {
        ExportImage(".png");
    }

    private void ExportJpeg()
    {
        ExportImage(".jpg");
    }

    private void ExportTiff()
    {
        ExportImage(".tif");
    }
    private void ExportImage(string extension)
    {
        if (_currentImage == null || _displayBuffer == null)
        {
            MessageBox.Show(
                "No image is currently loaded. Please open an image before exporting.",
                "Export not available",
                MessageBoxButton.OK,
                MessageBoxImage.Warning);

            return;
        }

        _logger.LogInformation("Export started: {Format}", extension);

        var dialog = new SaveFileDialog
        {
            DefaultExt = extension,
            FileName = Path.GetFileNameWithoutExtension(FileName)
        };

        switch (extension)
        {
            case ".png":
                dialog.Filter = "PNG Image (*.png)|*.png";
                break;

            case ".jpg":
                dialog.Filter = "JPEG Image (*.jpg)|*.jpg";
                break;

            case ".tif":
                dialog.Filter = "TIFF Image (*.tif)|*.tif";
                break;
        }

        if (dialog.ShowDialog() != true)
            return;

        try
        {
            switch (extension)
            {
                case ".png":
                    _exportService.SavePng(
                        _displayBuffer,
                        _currentImage.Width,
                        _currentImage.Height,
                        dialog.FileName);
                    break;

                case ".jpg":
                    _exportService.SaveJpeg(
                        _displayBuffer,
                        _currentImage.Width,
                        _currentImage.Height,
                        dialog.FileName);
                    break;

                case ".tif":
                    _exportService.SaveTiff(
                        _displayBuffer,
                        _currentImage.Width,
                        _currentImage.Height,
                        dialog.FileName);
                    break;
            }

            _logger.LogInformation("Export successful: {Path}", dialog.FileName);

            MessageBox.Show(
                "Your image has been exported successfully.",
                "Export complete",
                MessageBoxButton.OK,
                MessageBoxImage.Information);
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Export failed");
            MessageBox.Show(
                "We couldn't complete the export. The operation was interrupted.",
                "Export failed",
                MessageBoxButton.OK,
                MessageBoxImage.Error);
        }
    }

    /// <summary>
    /// Executes the default image export workflow using PNG encoding.
    /// </summary>
    /// 
    private void Export()
    {
        if (_currentImage == null || _displayBuffer == null)
        {
            MessageBox.Show(
                "No image is currently loaded. Please open an image before exporting.",
                "Export not available",
                MessageBoxButton.OK,
                MessageBoxImage.Warning);

            return;
        }

        var dialog = new SaveFileDialog
        {
            Filter = "PNG Image (*.png)|*.png",
            DefaultExt = ".png",
            FileName = Path.GetFileNameWithoutExtension(FileName)
        };

        if (dialog.ShowDialog() != true)
            return;

        try
        {
            _exportService.SavePng(
                _displayBuffer,
                _currentImage.Width,
                _currentImage.Height,
                dialog.FileName);

            MessageBox.Show(
                "Your image has been exported successfully.",
                "Export complete",
                MessageBoxButton.OK,
                MessageBoxImage.Information);
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Error during Export");
            MessageBox.Show(
                "We couldn't complete the export. The operation was interrupted.",
                "Export failed",
                MessageBoxButton.OK,
                MessageBoxImage.Error);
        }
    }

    /// <summary>
    /// Terminates the PhotonLab application and shuts down the
    /// current WPF application instance.
    /// </summary>
    private void Exit()
    {
        const string taskName = "Application Exit";
        _logger.LogInformation("Starting {Operation}.", taskName);

        try
        {
            // If no image is loaded, exit immediately without confirmation.
            if (_currentImage == null)
            {
                _logger.LogInformation("No image loaded. Proceeding to shutdown.");
                Application.Current.Shutdown();
                return;
            }

            // If an image is loaded, verify that the user truly wants to lose their progress.
            var result = MessageBox.Show(
                "Are you sure you want to exit PhotonLab?\n\nAny unsaved changes will be lost.",
                "Exit PhotonLab",
                MessageBoxButton.YesNo,
                MessageBoxImage.Warning,
                MessageBoxResult.No);

            if (result == MessageBoxResult.Yes)
            {
                _logger.LogInformation("User confirmed exit.");
                Application.Current.Shutdown();
            }
            else
            {
                _logger.LogInformation("User cancelled exit.");
            }

            _logger.LogInformation("Successfully completed {Operation}.", taskName);
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Error during {Operation}", taskName);
            throw; // Re-throw if you want the application to handle the crash or terminate
        }
    }

    private double _zoomFactor = 1.0;

    public double ZoomFactor
    {
        get => _zoomFactor;
        set
        {
            var clamped = Math.Max(0.1, Math.Min(10.0, value));

            if (_zoomFactor != clamped)
            {
                _zoomFactor = clamped;

                ZoomPercentage = $"{clamped * 100:F0}%";

                OnPropertyChanged(nameof(ZoomFactor));
            }
        }
    }

    private void ResetZoom()
    {
        ZoomFactor = 1.0;
    }

    /// <summary>
    /// Gets or sets the width of the visible viewport viewing area.
    /// </summary>
    /// <value>The width of the viewport in device-independent units.</value>
    private double _viewportWidth;
    public double ViewportWidth
    {
        get => _viewportWidth;
        set
        {
            _viewportWidth = value;
            OnPropertyChanged();
        }
    }

    /// <summary>
    /// Gets or sets the height of the visible viewport viewing area.
    /// </summary>
    /// <value>The height of the viewport in device-independent units.</value>
    private double _viewportHeight;
    public double ViewportHeight
    {
        get => _viewportHeight;
        set
        {
            _viewportHeight = value;
            OnPropertyChanged();
        }
    }

    /// <summary>
    /// Adjusts the zoom factor to fit the image within the visible viewing area.
    /// </summary>
    /// <remarks>
    /// Calculates the scaling factor required for both dimensions and applies the smaller 
    /// <see cref="ZoomFactor"/> to ensure the entire image fits inside the viewport without distortion.
    /// </remarks>
    /// <exception cref="Exception">Re-throws any exception encountered during the scaling process after logging.</exception>
    private void FitToScreen()
    {
        const string taskName = "FitToScreen";
        _logger.LogInformation("Starting {Operation}.", taskName);

        try
        {
            if (_currentImage == null)
            {
                _logger.LogWarning("{Operation} aborted: No image loaded.", taskName);
                return;
            }

            if (ViewportWidth <= 0 || ViewportHeight <= 0)
            {
                _logger.LogWarning("{Operation} aborted: Invalid viewport dimensions ({Width}x{Height}).", taskName, ViewportWidth, ViewportHeight);
                return;
            }

            double imageWidth = _currentImage.Width;
            double imageHeight = _currentImage.Height;

            double scaleX = ViewportWidth / imageWidth;
            double scaleY = ViewportHeight / imageHeight;

            ZoomFactor = Math.Min(scaleX, scaleY);

            _logger.LogInformation("{Operation} successful: Image ({ImgW}x{ImgH}) scaled to {Zoom} to fit Viewport ({ViewW}x{ViewH}).",
                taskName, imageWidth, imageHeight, ZoomFactor, ViewportWidth, ViewportHeight);
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Error during {Operation}", taskName);
            throw;
        }
    }

    /// <summary>
    /// Regenerates the display buffer from the current processing
    /// session pixels and updates the image source shown by the UI.
    /// </summary>
    /// 
    
    private async Task RefreshDisplay()
    {
        try
        {
            var pixels = _processingService.GetCurrentPixels();

            _displayBuffer = await Task.Run(() =>
                _renderer.RenderToDisplayBuffer(
                    pixels,
                    _currentImage!.Width,
                    _currentImage.Height));

            ImageSource = CreateBitmapSource(
                _displayBuffer,
                _currentImage!.Width,
                _currentImage.Height);
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Failed to refresh display buffer");

            MessageBox.Show(
                "Failed to render image display. Please check logs for details.",
                "Render Error",
                MessageBoxButton.OK,
                MessageBoxImage.Error);
        }
    }

    //private void RefreshDisplay()
    //{
      //  ushort[] pixels =
        //    _processingService.GetCurrentPixels();

        //_displayBuffer =
       //     _renderer.RenderToDisplayBuffer(
         //       pixels,
           //     _currentImage!.Width,
             //   _currentImage.Height);

        //ImageSource =
          //  CreateBitmapSource(
            //    _displayBuffer,
              //  _currentImage.Width,
               // _currentImage.Height);
    //}

    // Gamma
    private double _gamma = 1.0;
    public double Gamma
    {
        get => _gamma;
        set
        {
            _gamma = value;
            OnPropertyChanged();
        }
    }

    /// <summary>
    /// Applies a Window/Level transformation to the active image.
    ///
    /// Window Width controls the displayed intensity range while
    /// Window Center defines the midpoint of the visible grayscale
    /// interval.
    ///
    /// Commonly used in medical imaging workflows to emphasize
    /// specific anatomical structures and tissue densities.
    /// </summary>
    /// 

    private int _windowWidth = 65535;
    public int WindowWidth
    {
        get => _windowWidth;
        set
        {
            _windowWidth = value;
            OnPropertyChanged();
        }
    }

    private int _windowCenter = 32768;
    public int WindowCenter
    {
        get => _windowCenter;
        set
        {
            _windowCenter = value;
            OnPropertyChanged();
        }
    }

    private async Task RefreshImageState()
    {
        await Task.WhenAll(
        RefreshDisplay(),
        RefreshStatistics());
        //RefreshHistogram());
    }

    private async void ApplyWindowLevel()
    {

        if (_currentImage == null)
        {
            MessageBox.Show("Please load an image first.", "warning", MessageBoxButton.OK, MessageBoxImage.Warning);
            return;
        }

        await ExecuteAsync("Adjusting Windows/Level", async () =>
        {
            await _processingService.ApplyWindowLevelAsync(WindowWidth, WindowCenter);

            await RefreshImageState();
        });
    }

    /// <summary>
    /// Applies gamma correction to the current working image and
    /// refreshes the displayed result.
    /// </summary>
    /// 
    private async void ApplyGamma()
    {
        if (_currentImage == null)
        {
            MessageBox.Show("Please load an image first.", "warning", MessageBoxButton.OK, MessageBoxImage.Warning);
            return;
        }

        _logger.LogInformation("Applying gamma correction: {Gamma}", Gamma);

        using var activity = StartTrace("ApplyGamma", new Dictionary<string, object?>
        {
            ["gamma.value"] = Gamma
        });

        await ExecuteAsync("Applying Gama Correction", async () =>
        {
            await _processingService.ApplyGammaAsync(Gamma);

            await RefreshImageState();
        });

        _logger.LogInformation("Gamma correction applied successfully");
    }

    /// <summary>
    /// Asynchronously applies a median filter to the currently loaded image.
    /// </summary>
    /// <remarks>
    /// This method logs the operation, initiates a telemetry trace, and executes the filter 
    /// through the processing service while managing the UI busy state.
    /// </remarks>
    private async void ApplyMedian()
    {
        if (_currentImage == null)
        {
            MessageBox.Show("Please load an image first.", "warning", MessageBoxButton.OK, MessageBoxImage.Warning);
            return;
        }

        _logger.LogInformation("Applying median filter");

        // Initialize telemetry trace for performance monitoring
        using var activity = StartTrace("ApplyMedian");

        try
        {
            await ExecuteAsync("Applying Median Filter", async () =>
            {
                await _processingService.ApplyMedianAsync();
                await RefreshImageState();
            });

            _logger.LogInformation("Median filter applied successfully");
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Failed to apply median filter");
            MessageBox.Show($"An error occurred while applying the median filter: {ex.Message}");
        }
    }

    /// <summary>
    /// Applies a sharpening filter to enhance local contrast and edge definition within the image.
    /// </summary>
    private async void ApplySharpen()
    {
        if (_currentImage == null)
        {
            MessageBox.Show("Please load an image first.", "warning", MessageBoxButton.OK, MessageBoxImage.Warning);
            return;
        }

        _logger.LogInformation("Applying sharpening filter");

        // Initialize telemetry trace for performance monitoring
        using var activity = StartTrace("ApplySharpen");

        try
        {
            await ExecuteAsync("Applying Sharpen Filter", async () =>
            {
                await _processingService.ApplySharpenAsync();
                await RefreshImageState();
            });

            _logger.LogInformation("Sharpening filter applied successfully");
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Failed to apply sharpening filter");
            MessageBox.Show($"An error occurred while applying the sharpening filter: {ex.Message}");
        }
    }

    /// <summary>
    /// Requests statistical analysis from the processing subsystem and
    /// updates all UI-bound statistical properties including minimum,
    /// maximum, mean, median, and standard deviation values.
    ///
    /// Statistics are calculated against the current working image state
    /// after all active processing operations have been applied.
    /// </summary>
    /// 

    private string _minimum = "-";
    public string Minimum
    {
        get => _minimum;
        set
        {
            _minimum = value;
            OnPropertyChanged();
        }
    }

    private string _maximum = "-";
    public string Maximum
    {
        get => _maximum;
        set
        {
            _maximum = value;
            OnPropertyChanged();
        }
    }
    private string _mean = "-";
    public string Mean
    {
        get => _mean;
        set
        {
            _mean = value;
            OnPropertyChanged();
        }
    }

    private string _stdDev = "-";
    public string StandardDeviation
    {
        get => _stdDev;
        set
        {
            _stdDev = value;
            OnPropertyChanged();
        }
    }

    private string _median = "-";
    public string Median
    {
        get => _median;
        set
        {
            _median = value;
            OnPropertyChanged();
        }
    }

    private async Task RefreshStatistics()
    {
        try
        {
            var stats =
                await _processingService.CalculateStatisticsAsync();

            Minimum = stats.Min.ToString();
            Maximum = stats.Max.ToString();
            Mean = stats.Mean.ToString("F2");
            StandardDeviation = stats.StandardDeviation.ToString("F2");
            Median = stats.Median.ToString("F2");
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Failed to Refresh");
        }
    }


    /// <summary>
    /// Asynchronously updates the histogram data for the currently loaded image.
    /// </summary>
    /// <remarks>
    /// This method manages the <see cref="IsHistogramUpdating"/> state, updates the <see cref="StatusMessage"/>, 
    /// and handles logging throughout the lifecycle of the operation.
    /// </remarks>
    /// 

    public IEnumerable<double> HistogramNormalized =>
Histogram == null
    ? Enumerable.Empty<double>()
    : ScaleHistogram(Histogram);

    private async Task RefreshHistogram()
    {
        if (_currentImage == null)
        {
            MessageBox.Show("Please load an image first.", "warning", MessageBoxButton.OK, MessageBoxImage.Warning);
            return;
        }

        using var activity = StartTrace("RefreshHistogram");
        activity?.SetTag("image.loaded", _currentImage != null);

        _logger.LogInformation("Histogram refresh started");

        var oldStatus = StatusMessage;

        IsHistogramUpdating = true;
        StatusMessage = "Updating histogram...";

        _logger.LogInformation("Histogram refresh completed");

        try
        {
            var sw = Stopwatch.StartNew();
            Histogram = await _processingService.CalculateHistogramAsync();
            sw.Stop();
            activity?.SetTag("duration.ms", sw.ElapsedMilliseconds);
        }
        finally
        {
            IsHistogramUpdating = false;
            StatusMessage = oldStatus;
        }
    }

    private string _statusMessage = "Ready";
    public string StatusMessage
    {
        get => _statusMessage;
        set { _statusMessage = value; OnPropertyChanged(); }
    }

    private IEnumerable<double> ScaleHistogram(uint[] data)
    {
        double max = Math.Log10(data.Max() + 1);

        if (max <= 0.0001)
            return data.Select(_ => 0.0);

        return data.Select(h =>
            (Math.Log10(h + 1) / max) * 100);
    }

    private uint[]? _histogram;

    public uint[]? Histogram
    {
        get => _histogram;
        set
        {
            _histogram = value;
            OnPropertyChanged();
            OnPropertyChanged(nameof(HistogramNormalized)); // optional but recommended
        }
    }

    private bool _isHistogramUpdating;
    public bool IsHistogramUpdating
    {
        get => _isHistogramUpdating;
        set
        {
            _isHistogramUpdating = value;
            OnPropertyChanged();
        }
    }

    /// <summary>
    /// Executes an asynchronous operation, managing the UI busy state, status updates, and error handling.
    /// </summary>
    /// <param name="taskname">A descriptive name for the task, used for status reporting and error messages.</param>
    /// <param name="action">The asynchronous function to be executed.</param>
    /// <returns>A <see cref="Task"/> representing the asynchronous operation.</returns>
    private async Task ExecuteAsync(string taskname, Func<Task> action)
    {
        using var activity = StartTrace(taskname);

        IsBusy = true;
        StatusMessage = $"{taskname}...";
        try
        {
            await action();
            await RefreshStatistics(); // Refresh stats after any operation
        }
        catch (Exception ex)
        {
            activity?.SetStatus(ActivityStatusCode.Error, ex.Message);
            activity?.AddException(ex, new TagList
            {
                { "task", taskname }
            });

            _logger.LogError(ex, "Error during {Task}", taskname);

            MessageBox.Show($"Error during {taskname}: {ex.Message}");
        }
        finally
        {
            IsBusy = false;
            StatusMessage = "Ready";
        }
    }

    private bool _isBusy;
    public bool IsBusy
    {
        get { return _isBusy; }
        set
        {
            _isBusy = value;
            OnPropertyChanged();    // Notify UI of change
        }
    }

    /// <summary>
    /// Starts a new <see cref="Activity"/> with the specified name and optional tags.
    /// </summary>
    /// <param name="name">The operation name for the activity.</param>
    /// <param name="tags">An optional dictionary of key-value pairs to attach to the activity.</param>
    /// <returns>
    /// A started <see cref="Activity"/> if sampling allows, or <see langword="null"/> 
    /// if the activity should not be recorded or an error occurred.
    /// </returns>
    private Activity? StartTrace(string name, IDictionary<string, object?>? tags = null)
    {
        try
        {
            var activity = ActivitySource.StartActivity(name, ActivityKind.Internal);

            if (activity != null && tags != null)
            {
                foreach (var tag in tags)
                {
                    activity.SetTag(tag.Key, tag.Value);
                }
            }

            return activity;
        }
        catch (Exception ex)
        {
            // Log the failure to ensure telemetry issues do not bubble up 
            // and impact the actual application execution.
            _logger.LogError(ex, "Failed to start activity: {Name}", name);
            return null;
        }
    }

    /// <summary>
    /// Displays a modal dialog box containing version information about the application.
    /// </summary>
    private void ShowAboutDialog()
    {
        MessageBox.Show(
            "PhotonLab\nVersion 1.0",
            "About PhotonLab",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }
}
