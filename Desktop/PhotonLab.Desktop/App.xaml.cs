/**************************************************************************************************
 * File         : App.xaml.cs
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
 * Description  : Application entry point and Dependency Injection (DI) bootstrap.
 * Configures the service collection for the PhotonLab desktop environment, ensuring
 * centralized lifetime management for core services, view models, and interop bridges.
 *
 * Author       : Nitish Singh <me.singhnitish@yandex.com>
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version    Date         Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0        2026-06-11   Nitish Singh     Initial implementation of DI container and startup logic.
 * 1.1        2026-06-12   Nitish Singh     Enhanced logging, lifecycle management, and OnExit cleanup.
 **************************************************************************************************/

using Microsoft.Extensions.DependencyInjection;
using PhotonLab.Core.Contracts;
using PhotonLab.Desktop.Interop;
using PhotonLab.Desktop.Services;
using PhotonLab.Desktop.ViewModels;
using PhotonLab.Desktop.Views;
using System;
using System.Windows;
using Serilog;
using Microsoft.Extensions.Logging;
using OpenTelemetry.Trace;
using OpenTelemetry.Metrics;
using OpenTelemetry.Resources;
using OpenTelemetry.Exporter;

namespace PhotonLab.Desktop
{
    /// <summary>
    /// Interaction logic for the main application. 
    /// Acts as the Composition Root for Dependency Injection and lifecycle management.
    /// </summary>
    public partial class App : Application
    {
        /// <summary>
        /// Gets the global service provider used for dependency resolution throughout the application.
        /// </summary>
        public IServiceProvider Services { get; }

        /// <summary>
        /// Gets the global logger factory instance.
        /// </summary>
        public static ILoggerFactory LoggerFactory { get; private set; } = null!;

        /// <summary>
        /// Initializes a new instance of the <see cref="App"/> class.
        /// Configures the global logger and initializes the Dependency Injection container.
        /// </summary>
        public App()
        {
            // 1. Configure Serilog (GLOBAL LOGGER)
            Log.Logger = new LoggerConfiguration()
                .MinimumLevel.Debug()
                .WriteTo.Console()
                .WriteTo.File("logs/photonlab-.log",
                    rollingInterval: RollingInterval.Day)
                .CreateLogger();

            var serviceCollection = new ServiceCollection();

            // 2. Add logging bridge (Microsoft ILogger → Serilog)
            serviceCollection.AddLogging(builder =>
            {
                builder.ClearProviders();
                builder.AddSerilog();
            });

            // 3. Configure Dependency Injection services
            ConfigureServices(serviceCollection);

            var provider = serviceCollection.BuildServiceProvider();

            Services = provider;
            LoggerFactory = provider.GetRequiredService<ILoggerFactory>();
        }

        /// <summary>
        /// Registers all application services, view models, and infrastructure components into the DI container.
        /// </summary>
        /// <param name="services">The <see cref="IServiceCollection"/> instance to configure.</param>
        private void ConfigureServices(IServiceCollection services)
        {
            // Logging
            services.AddLogging(builder =>
            {
                builder.ClearProviders();
                builder.AddSerilog();
            });

            // Infrastructure and Business Services
            services.AddSingleton<ITiffLoader, TiffLoaderService>();
            services.AddSingleton<IHistogramService, HistogramService>();
            services.AddSingleton<IStatisticsService, StatisticsService>();
            services.AddSingleton<IImageProcessingService, ImageProcessingService>();
            services.AddSingleton<IImageSessionService, ImageSessionService>();
            services.AddSingleton<IExportService, ExportService>();
            services.AddSingleton<IImageRenderer, ImageRenderingService>();

            // Native Interop Layer
            services.AddSingleton<IImageProcessor, NativeImageProcessor>();

            // ViewModels
            services.AddSingleton<MainViewModel>();

            // Configure OpenTelemetry
            services.AddOpenTelemetry()
                .ConfigureResource(resource =>
                    resource.AddService("PhotonLab.Desktop"))

                .WithTracing(tracing =>
                {
                    tracing
                        .AddSource("PhotonLab.MainViewModel")

                        .AddHttpClientInstrumentation()

                        .AddConsoleExporter()

                        .AddOtlpExporter(opt =>
                        {
                            opt.Endpoint = new Uri("http://localhost:4317");
                        });
                })

                .WithMetrics(metrics =>
                {
                    metrics
                        .AddMeter("PhotonLab.Desktop")
                        .AddConsoleExporter();
                });
        }

        /// <summary>
        /// Handles the startup logic of the application. 
        /// Resolves the main window and applies initial configuration.
        /// </summary>
        /// <param name="e">The <see cref="StartupEventArgs"/> providing information about the startup process.</param>
        protected override void OnStartup(StartupEventArgs e)
        {
            Log.Information("PhotonLab starting up...");

            try
            {
                base.OnStartup(e);

                var vm = Services.GetRequiredService<MainViewModel>();
                var window = new MainWindow(vm);

                MainWindow = window;

                window.Width = 800;
                window.Height = 600;
                window.WindowStartupLocation = WindowStartupLocation.CenterScreen;

                window.Show();

                Log.Information("Main window loaded successfully");
            }
            catch (Exception ex)
            {
                Log.Fatal(ex, "Application startup failed");
                MessageBox.Show(ex.ToString(), "Startup Error",
                    MessageBoxButton.OK, MessageBoxImage.Error);
                throw;
            }
        }

        /// <summary>
        /// Handles the cleanup and shutdown logic of the application.
        /// Disposes of the service provider and flushes logging buffers.
        /// </summary>
        /// <param name="e">The <see cref="ExitEventArgs"/> providing information about the exit process.</param>
        protected override void OnExit(ExitEventArgs e)
        {
            try
            {
                Log.Information("Application exiting.");

                // Dispose the ServiceProvider if it implements IDisposable
                if (Services is IDisposable disposable)
                {
                    disposable.Dispose();
                }
            }
            catch (Exception ex)
            {
                Log.Error(ex, "Error during application shutdown");
            }
            finally
            {
                // Essential to ensure logs are written to disk
                Log.CloseAndFlush();
            }

            base.OnExit(e);
        }
    }
}
