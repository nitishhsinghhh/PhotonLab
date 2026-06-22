# Data Flow

To maintain system-wide performance, PhotonLab is designed around a **low-allocation, session-based processing pipeline**. Large 16-bit pixel buffers are kept in memory and passed through managed services without repeated copying wherever possible. Native interoperability is handled through tightly controlled buffer ownership, ensuring the UI remains responsive while image processing executes asynchronously.

## Table of Contents

1. [Purpose](#purpose)
2. [End-to-End Flow](#end-to-end-flow)
3. [Processing Lifecycle](#processing-lifecycle)
    * [Image Load](#image-load)
    * [Metadata Analysis](#metadata-analysis)
    * [Image Processing](#image-processing)
    * [Display Conversion](#display-conversion)
4. [Processing Orchestration Flow](#processing-orchestration-flow)
5. [Histogram Flow](#histogram-flow)
6. [ROI Analytics Flow](#roi-analytics-flow)
7. [Design Principles](#design-principles)

---

## Purpose

This document describes how image data flows through PhotonLab from image acquisition to final rendering. The goal is to maintain 16-bit image fidelity during processing while providing responsive 8-bit visualization for desktop display.

---

## End-to-End Flow

```text
        16-Bit TIFF File
                │
                ▼
        TIFF Loader Service
                │
                ▼
        Raw Image Buffer (ushort[])
                │
                ▼
        Metadata Extraction
                │
                ├── Image Dimensions (Width, Height)
                ├── Bit Depth (16-bit)
                ├── File Metadata
                │
                ▼
        IImageSessionService (Session Initialization)
                │
                ▼
        IImageProcessingService (Working Buffer)
                │
                ├── Window / Level
                ├── Gamma Correction
                ├── Median Filter
                └── Sharpen Filter
                │
                ▼
        Processed ushort[] Buffer
                │
                ▼
        IImageRenderer (Display Mapping)
                │
                ▼
        BGRA32 byte[] Display Buffer
                │
                ▼
        BitmapSource (WPF)
                │
                ▼
        Image Control (UI)
```

---

## Processing Lifecycle

### Image Load

- TIFF file is loaded using ITiffLoader
- Image is converted into a 16-bit ushort[] buffer
- This buffer represents the source of truth
- Stored in an image session for further processing

---

### Metadata Analysis

Immediately after loading, metadata and image statistics are calculated.

Generated statistics include:

* Image Width
* Image Height
* Bit Depth (16-bit)
* File size
* Pixel format
* File Size
* File Name

These values are displayed in the user interface and reused by analytics services.

---

### Image Processing

All image processing operations occur on the working 16-bit buffer, managed by IImageProcessingService.

Processing operations include:

* Window / Level transformation
* Gamma correction
* Median filtering
* Sharpening filter

```text
        Original ushort[]
                │
                ▼
        Processing Pipeline
                │
                ▼
        Processed ushort[]
```

No processing is performed on the rendered 8-bit display buffer to avoid precision loss.

---

### Display Conversion

Once processing is complete, the renderer converts the 16-bit buffer into a display-friendly format:

```text
        Processed ushort[]
                ↓
        IImageRenderer (Window/Level mapping)
                ↓
        BGRA32 byte[] display buffer
                ↓
        BitmapSource (WPF)
```

This conversion is strictly for visualization and does not affect the underlying image data.

---

## Processing Orchestration Flow

All user-triggered operations are coordinated through the MainViewModel.ExecuteAsync() pipeline.

```text
        User Command (UI Button)
                ↓
        RelayCommand
                ↓
        MainViewModel.ExecuteAsync()
                ↓
        Set IsBusy + StatusMessage
                ↓
        IImageProcessingService Operation
                ↓
        RefreshDisplay()
                ↓
        RefreshStatistics()
                ↓
        RefreshHistogram()
                ↓
        UI Update (INotifyPropertyChanged)
                ↓
        Reset IsBusy + StatusMessage
```

This ensures:

* Non-blocking UI execution
* Consistent status reporting
* Centralized error handling
* Telemetry tracing per operation

---

## Histogram Flow

Histogram generation is performed independently of rendering.

```text
        Processed ushort[]
                ↓
        IImageProcessingService.CalculateHistogramAsync()
                ↓
        uint[] Histogram Bins
                ↓
        Log Scaling (log10 normalization)
                ↓
        HistogramNormalized (0–100 range)
                ↓
        UI Chart Binding
```

Histogram generation remains independent of image rendering and processing concerns.

---

## ROI Analytics Flow

Region-of-interest (ROI) analytics operate on the current processed image buffer:

```text
        User ROI Selection
                ↓
        IImageSessionService / Processing Layer
                ↓
        Statistical Computation
                ├── Min
                ├── Max
                ├── Mean
                ├── Median
                └── Standard Deviation
                ↓
        MainViewModel Properties
                ↓
        UI Display
```

## Design Principles

1. Single Source of Truth
- The 16-bit ushort[] working buffer is the authoritative image state.
2 No Processing on Render Buffer
- 8-bit display buffers are strictly for visualization.
3. Separation of Concerns
- Loader, processing, rendering, and session management are independent services.
4. Async-First Execution
- All heavy operations execute via ExecuteAsync() without blocking the UI thread.
5. Deterministic UI State
- IsBusy and StatusMessage control user feedback during processing.
6. Telemetry-Driven Diagnostics
- OpenTelemetry traces each major processing operation for observability.