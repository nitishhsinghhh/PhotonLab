# System Architecture

## Table of Contents

1. [High-Level Overview](#high-level-overview)
2. [Architectural Layers](#architectural-layers)
    - [WPF UI Layer](#wpf-ui-layer)
    - [ViewModel Layer](#viewmodel-layer)
    - [Application Services](#application-services)
    - [Native Image Processing (C++ Engine)](#native-image-processing-c-engine)
    - [Core Models and Contracts](#core-models-and-contracts)
3. [Component Interaction Flow](#component-interaction-flow)
4. [Developer Guidelines](#developer-guidelines)
    - [Adding a New Processing Algorithm](#adding-a-new-processing-algorithm)
    - [Dependency Rules](#dependency-rules)
5. [Key Design Principles](#key-design-principles)

---

## High-Level Overview

PhotonLab is a layered imaging platform designed around **MVVM principles**, **session-based processing**, and a strict separation between UI, orchestration, and native computation.

The architecture ensures:

- UI responsiveness during heavy 16-bit processing
- Zero-copy friendly design in the processing pipeline
- Clear separation of rendering, processing, and session state
- Observability through OpenTelemetry instrumentation

---

## Architectural Layers

### WPF UI Layer

This layer is purely representational and binds directly to ViewModel properties.

Key responsibilities:

- Image rendering via `BitmapSource`
- Histogram visualization (normalized log-scale data)
- ROI selection and overlays
- Metadata display (dimensions, bit depth, pixel format)
- Zoom and viewport rendering

The UI never directly accesses processing logic.

---

### ViewModel Layer

The `MainViewModel` acts as the **central orchestration layer** of PhotonLab.

From your implementation:

- Implements `INotifyPropertyChanged`
- Exposes `RelayCommand` bindings (Open, Export, Filters, Histogram, etc.)
- Manages UI state:
  - `IsBusy`
  - `StatusMessage`
  - `ZoomFactor`
  - `ImageSource`
- Coordinates all processing via `ExecuteAsync()`

---

### Application Services

This layer contains the core application logic and orchestration services:

From your codebase:

- `ITiffLoader`
- `IImageRenderer`
- `IImageProcessingService`
- `IImageSessionService`
- `IExportService`

Responsibilities:

- TIFF decoding into `ushort[]`
- Session-based image state management
- Histogram generation
- Statistical analysis (min, max, mean, median, std dev)
- Image export (PNG, JPEG, TIFF)
- Native engine coordination

This layer is **UI-agnostic** and fully testable.

---

### Native Image Processing (C++ Engine)

The native engine performs **high-performance pixel-level operations** on 16-bit buffers.

Key responsibilities:

- Window / Level transformation
- Gamma correction
- Median filtering
- Sharpening
- Optimized buffer processing (SIMD-ready design)

Important constraint:

- All processing operates on `ushort[]` buffers (16-bit)
- No 8-bit operations are performed in native layer

---

### Core Models and Contracts

Defined in `PhotonLab.Core`:

- `LoadedImage`
- `ImageStatistics`
- Service interfaces (`IImageProcessingService`, etc.)

Responsibilities:

- Define contracts between layers
- Ensure dependency inversion
- Enable mocking for unit tests

---

## Component Interaction Flow

*Note: The flow follows a strict downward dependency path to ensure the UI remains responsive during heavy image processing tasks.*

```txt
                        ┌─────────────────────────────────────┐
                        │              WPF UI                 │
                        │  Image Viewer                       │
                        │  Histogram Panel                    │
                        │  ROI Overlay                        │
                        │  Metadata Display                   │
                        └─────────────────┬───────────────────┘
                                          │
                                          ▼
                        ┌─────────────────────────────────────┐
                        │             ViewModels              │
                        │  MainViewModel                      │
                        │  Commands                           │
                        │  UI State                           │
                        │  Async Operations                   │
                        └─────────────────┬───────────────────┘
                                          │
                                          ▼
                        ┌─────────────────────────────────────┐
                        │         Application Services        │
                        │  TIFF Loader                        │
                        │  Histogram Service                  │
                        │  Statistics Service                 │
                        │  ROI Service                        │
                        │  Processing Orchestrator            │
                        └─────────────────┬───────────────────┘
                                          │
                                          ▼
                        ┌─────────────────────────────────────┐
                        │      Native Image Processing        │
                        │  Window / Level                     │
                        │  Gamma Correction                   │
                        │  Median Filter                      │
                        │  Sharpen Filter                     │
                        └─────────────────┬───────────────────┘
                                          │
                                          ▼
                        ┌─────────────────────────────────────┐
                        │            Image Models             │
                        └─────────────────────────────────────┘
```

### Processing and Execution Flow

PhotonLab uses a centralized execution pipeline implemented in MainViewModel.ExecuteAsync().

```text
    User Action (RelayCommand)
            ↓
    ExecuteAsync(taskName)
            ↓
    Set IsBusy = true
    Set StatusMessage
            ↓
    Call Processing Service
            ↓
    RefreshDisplay()
            ↓
    RefreshStatistics()
            ↓
    RefreshHistogram()
            ↓
    Update UI via INotifyPropertyChanged
            ↓
    Reset IsBusy / StatusMessage
```

Key guarantees:

No UI thread blocking
Consistent error handling
Centralized telemetry tracking
Deterministic UI state updates

---

## Developer Guidelines

### Adding a New Processing Algorithm

To add a new image transformation to PhotonLab:

1. **Define the Logic**: Create the strategy in `Native/PhotonLab.Native/Include/Processing/`.
2. **Expose the Interface**: Update `Native/PhotonLab.Native/Src/Interop/NativeExports.cpp` to map the C++ function to the C-style export.
3. **Update Services**: Add the corresponding method to the `IImageProcessor` contract in the `.Core` project.

### Dependency Rules

- UI must never directly access services or native code
- Services must remain UI-agnostic
- Native layer must not depend on .NET types
- All processing must be async-safe
- ViewModel is the only orchestration entry point

---

### Key Design Principles

- MVVM Strict Separation
- Session-Based Image State
- 16-bit Processing Preservation
- 8-bit Only for Display
- Async-First Execution Model
- Telemetry-Driven Observability
- Service-Oriented Architecture
