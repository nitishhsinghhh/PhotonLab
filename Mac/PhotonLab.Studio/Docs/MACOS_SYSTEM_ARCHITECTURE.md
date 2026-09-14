# System Architecture — macOS

## Table of Contents

1. [High-Level Overview](#high-level-overview)
2. [Architectural Layers](#architectural-layers)

   * [SwiftUI UI Layer](#swiftui-ui-layer)
   * [ViewModel Layer](#viewmodel-layer)
   * [Application Services](#application-services)
   * [Native Image Processing C++ Engine](#native-image-processing-c-engine)
   * [Core Models and Contracts](#core-models-and-contracts)
   * [Platform Integration Layer](#platform-integration-layer)
3. [Component Interaction Flow](#component-interaction-flow)
4. [Processing and Execution Flow](#processing-and-execution-flow)
5. [Developer Guidelines](#developer-guidelines)

   * [Adding a New Processing Algorithm](#adding-a-new-processing-algorithm)
   * [Dependency Rules](#dependency-rules)
6. [Key Design Principles](#key-design-principles)

---

## High-Level Overview

PhotonLab for macOS is a layered medical-imaging platform designed around **SwiftUI**, **MVVM**, **session-based processing**, and a shared high-performance **C++ image-processing engine**.

The architecture maintains a strict separation between:

* User interface
* Presentation state
* Application orchestration
* Image-processing services
* Native computation
* Platform-specific integration

The macOS application is designed to preserve the same processing semantics as the Windows/WPF implementation while providing a native macOS user experience.

The architecture ensures:

* UI responsiveness during heavy 16-bit image processing
* Preservation of 16-bit image precision throughout processing
* Zero-copy-friendly buffer ownership
* Clear separation of rendering and computation
* Reusable native C++ processing algorithms
* Testable Swift application services
* Session-based image state management
* OpenTelemetry-based observability
* Native macOS integration without coupling the processing engine to Apple frameworks

---

## Architectural Layers

## SwiftUI UI Layer

The SwiftUI layer is responsible exclusively for presentation.

It observes ViewModels and renders application state without directly invoking image-processing algorithms.

### Key responsibilities

* Image viewer
* Histogram visualization
* ROI selection and overlays
* Metadata display
* Zoom and viewport controls
* Processing controls
* Progress indicators
* Error/status presentation
* Export controls

Typical components include:

```text
PhotonLabApp
├── ContentView
├── ImageViewer
├── HistogramView
├── MetadataView
├── ROIOverlay
├── ProcessingControls
└── StatusView
```

SwiftUI views should remain declarative:

```text
View
  ↓
Observe ViewModel State
  ↓
Render UI
```

The UI does not directly communicate with the C++ engine.

---

## ViewModel Layer

The ViewModel layer acts as the presentation and orchestration boundary between SwiftUI and the application services.

A primary `ImageViewModel` or `MainViewModel` owns observable UI state.

Typical responsibilities include:

* Current image state
* Current processing operation
* Busy/progress state
* Error state
* Zoom factor
* Histogram data
* Image statistics
* ROI state
* Metadata
* Command execution

Example conceptual state:

```swift
@Observable
final class ImageViewModel {

    var image: DisplayImage?
    var histogram: HistogramData?
    var statistics: ImageStatistics?

    var zoomFactor: Double = 1.0
    var isBusy = false
    var statusMessage = ""

    func openImage() async
    func applyWindowLevel() async
    func applyGamma() async
    func applyMedianFilter() async
    func applySharpen() async
    func exportImage() async
}
```

The ViewModel should coordinate operations but should not contain:

* TIFF parsing logic
* Histogram algorithms
* Image-processing algorithms
* C++ implementation details
* File-format implementation details

Those responsibilities belong to application services.

---

## Application Services

The application-service layer contains UI-independent application logic.

This layer provides Swift-facing abstractions around image loading, rendering, processing, statistics, sessions, and export.

Possible services include:

```text
IImageLoader
IImageRenderer
IImageProcessingService
IImageSessionService
IStatisticsService
IHistogramService
IExportService
```

Swift implementations can use protocols to preserve dependency inversion:

```swift
protocol ImageProcessingService {
    func windowLevel(
        image: ImageBuffer,
        window: Double,
        level: Double
    ) async throws -> ImageBuffer

    func gamma(
        image: ImageBuffer,
        value: Double
    ) async throws -> ImageBuffer
}
```

### Responsibilities

#### Image Loading

* TIFF decoding
* 16-bit pixel extraction
* Metadata extraction
* Pixel-format validation
* Image dimension validation

#### Image Session

* Current source image
* Current processed image
* Processing history
* Original image preservation
* Session reset
* Undo/redo support where required

#### Statistics

* Minimum
* Maximum
* Mean
* Median
* Standard deviation
* Pixel-count statistics

#### Histogram

* Histogram generation
* Normalization
* Log-scale transformation
* Display-ready histogram data

#### Rendering

The rendering service converts the processed 16-bit representation into a display representation suitable for macOS rendering.

```text
16-bit Processing Buffer
        ↓
Display Transformation
        ↓
8-bit / Display Pixel Representation
        ↓
CGImage / NSImage / SwiftUI Image
```

The 8-bit conversion is therefore a **presentation concern**, not a native-processing concern.

---

## Native Image Processing C++ Engine

The C++ engine remains the performance-critical computation layer.

This layer should be platform-independent wherever possible.

The native engine operates on 16-bit image buffers:

```text
ushort / uint16_t
```

and performs pixel-level transformations without depending on:

* Swift
* SwiftUI
* AppKit
* Foundation-specific application logic
* .NET

### Core responsibilities

* Window / Level transformation
* Gamma correction
* Median filtering
* Sharpening
* Histogram-related computation where appropriate
* High-performance buffer operations
* SIMD-ready processing
* Parallel processing

Conceptually:

```text
Swift Application
       │
       ▼
C-Compatible API
       │
       ▼
C++ Processing Engine
       │
       ▼
uint16_t Image Buffer
```

The native layer should expose a narrow C ABI rather than exposing C++ classes directly to Swift.

Example:

```cpp
extern "C" {

    int photonlab_apply_window_level(
        const uint16_t* input,
        uint16_t* output,
        size_t pixel_count,
        double window,
        double level
    );

}
```

This creates a stable interoperability boundary.

---

## Core Models and Contracts

The core model layer defines application-independent data structures and service contracts.

Typical models include:

```text
ImageBuffer
LoadedImage
ImageMetadata
ImageStatistics
HistogramData
RegionOfInterest
ProcessingParameters
ProcessingResult
```

Example:

```swift
struct ImageMetadata {
    let width: Int
    let height: Int
    let bitDepth: Int
    let pixelFormat: PixelFormat
}

struct ImageStatistics {
    let minimum: UInt16
    let maximum: UInt16
    let mean: Double
    let median: Double
    let standardDeviation: Double
}
```

The model layer should not depend on SwiftUI views.

This keeps the domain/application model reusable across:

```text
SwiftUI
Unit Tests
CLI tools
Future macOS interfaces
```

---

## Platform Integration Layer

macOS-specific APIs should be isolated behind a platform-integration boundary.

This prevents Apple frameworks from leaking into the C++ engine and keeps application services easier to test.

Typical responsibilities include:

* File selection
* Save panels
* Clipboard integration
* Window management
* Native image conversion
* macOS permissions
* Drag-and-drop
* Application lifecycle
* Menu commands
* App sandbox integration

Relevant technologies include:

```text
SwiftUI
AppKit
Core Graphics
ImageIO
Uniform Type Identifiers
Grand Central Dispatch
Swift Concurrency
```

For example:

```text
SwiftUI
   ↓
Platform Service
   ↓
NSOpenPanel / NSSavePanel
   ↓
Application Service
```

The application services should not directly instantiate UI components.

---

## Component Interaction Flow

The high-level macOS architecture is:

```text
┌──────────────────────────────────────────────────────────┐
│                     SwiftUI UI                           │
│                                                          │
│  Image Viewer   Histogram   ROI   Metadata   Controls    │
└──────────────────────────┬───────────────────────────────┘
                           │
                           ▼
┌──────────────────────────────────────────────────────────┐
│                  ViewModel Layer                         │
│                                                          │
│  ImageViewModel                                          │
│  Observable State                                         │
│  Commands                                                 │
│  Async Operations                                         │
└──────────────────────────┬───────────────────────────────┘
                           │
                           ▼
┌──────────────────────────────────────────────────────────┐
│                Application Services                      │
│                                                          │
│  Image Loader                                             │
│  Session Service                                          │
│  Histogram Service                                        │
│  Statistics Service                                       │
│  Processing Service                                       │
│  Export Service                                           │
└──────────────────────────┬───────────────────────────────┘
                           │
                           ▼
┌──────────────────────────────────────────────────────────┐
│             Native Interop Boundary                     │
│                                                          │
│              C-compatible API                            │
└──────────────────────────┬───────────────────────────────┘
                           │
                           ▼
┌──────────────────────────────────────────────────────────┐
│               Native C++ Engine                          │
│                                                          │
│  Window/Level    Gamma    Median    Sharpen              │
│                                                          │
│              uint16_t Processing                          │
└──────────────────────────┬───────────────────────────────┘
                           │
                           ▼
┌──────────────────────────────────────────────────────────┐
│                 Image Models / Buffers                   │
│                                                          │
│          16-bit Source / Processed Image                 │
└──────────────────────────────────────────────────────────┘
```

---

## Processing and Execution Flow

PhotonLab uses Swift Concurrency to prevent long-running processing operations from blocking the UI.

```text
User Action
     ↓
SwiftUI Command
     ↓
ImageViewModel
     ↓
ExecuteAsync / Task
     ↓
Set isBusy = true
     ↓
Application Service
     ↓
Native C ABI
     ↓
C++ Processing Engine
     ↓
16-bit Image Buffer
     ↓
Processing Result
     ↓
Refresh Display
     ↓
Refresh Statistics
     ↓
Refresh Histogram
     ↓
Publish Observable State
     ↓
SwiftUI Re-render
     ↓
Set isBusy = false
```

The important distinction is:

```text
UI Thread
   │
   ├── State updates
   └── Rendering
       
Background Task
   │
   ├── TIFF decoding
   ├── Statistics
   ├── Histogram
   └── C++ image processing
```

Swift Concurrency provides structured asynchronous execution:

```swift
Task {
    await viewModel.applyProcessing()
}
```

Long-running work should execute outside the main actor where appropriate.

UI state mutations should return to the main actor.

---

## Memory and Buffer Ownership

The macOS implementation should explicitly define buffer ownership across the Swift/C++ boundary.

Recommended conceptual ownership model:

```text
Swift Image Session
        │
        ▼
Owned 16-bit Buffer
        │
        │ pointer + length
        ▼
C ABI
        │
        ▼
C++ Processing
        │
        ▼
Output Buffer
```

The interoperability contract should clearly define:

* Who owns the input buffer
* Who owns the output buffer
* Buffer lifetime
* Pixel count
* Image dimensions
* Stride
* Error handling
* Allocation/deallocation responsibilities

Avoid unnecessary conversions:

```text
TIFF
 ↓
uint16_t
 ↓
uint8_t
 ↓
uint16_t
```

Instead:

```text
TIFF
 ↓
uint16_t
 ↓
C++ Processing
 ↓
uint16_t
 ↓
Display Conversion
 ↓
CGImage
```

This preserves precision and reduces memory bandwidth.

---

## Developer Guidelines

## Adding a New Processing Algorithm

To add a new image-processing algorithm:

### 1. Implement the algorithm

Create the strategy in:

```text
Native/PhotonLab.Native/Include/Processing/
Native/PhotonLab.Native/Src/Processing/
```

For example:

```text
Processing/
├── IImageProcessingStrategy.h
├── GammaStrategy.h
├── GammaStrategy.cpp
├── MedianFilterStrategy.h
└── MedianFilterStrategy.cpp
```

### 2. Add the native export

Expose the operation through the C-compatible API:

```text
Native/PhotonLab.Native/Src/Interop/NativeExports.cpp
```

Example:

```cpp
extern "C" int photonlab_apply_gamma(...);
```

### 3. Add the Swift interoperability declaration

Create or update the Swift native-interop boundary.

Conceptually:

```text
Swift
  ↓
C ABI declaration
  ↓
NativeExports.cpp
  ↓
C++ Strategy
```

### 4. Update the application service

Add the operation to:

```text
ImageProcessingService
```

### 5. Update the ViewModel

Expose the operation through the appropriate ViewModel command.

### 6. Add tests

Tests should exist at multiple levels:

```text
C++ Unit Tests
       ↓
Interop Tests
       ↓
Swift Service Tests
       ↓
ViewModel Tests
```

This prevents a UI-level test from being the only validation of the processing algorithm.

---

## Dependency Rules

The architecture follows strict dependency direction.

```text
SwiftUI
   ↓
ViewModels
   ↓
Application Services
   ↓
Native Interop
   ↓
C++ Engine
```

### Rules

* SwiftUI must never directly call C++.
* SwiftUI must never contain image-processing algorithms.
* ViewModels should coordinate rather than implement processing.
* Application services must remain UI-agnostic.
* C++ must not depend on SwiftUI or AppKit.
* C++ must not depend on .NET.
* Native exports should expose a narrow C ABI.
* Core models should not depend on UI frameworks.
* Long-running processing must not block the main actor.
* Buffer ownership must be explicit across language boundaries.
* Display conversion must not mutate the authoritative 16-bit processing buffer.
* All processing algorithms must be independently testable.

---

## Testing Architecture

PhotonLab macOS should maintain testing boundaries similar to the native Windows implementation.

```text
┌──────────────────────────────┐
│ SwiftUI / ViewModel Tests    │
└───────────────┬──────────────┘
                ↓
┌──────────────────────────────┐
│ Application Service Tests    │
└───────────────┬──────────────┘
                ↓
┌──────────────────────────────┐
│ Native Interop Tests         │
└───────────────┬──────────────┘
                ↓
┌──────────────────────────────┐
│ C++ Unit Tests                │
│ GoogleTest                    │
└──────────────────────────────┘
```

Native algorithms should remain testable without launching the macOS application.

This provides:

* Fast feedback
* Deterministic algorithm tests
* Isolation of interoperability failures
* Easier CI execution
* Platform-independent native validation

---

## Key Design Principles

## MVVM Separation

SwiftUI owns presentation.

ViewModels own presentation state and orchestration.

Services own application behavior.

C++ owns high-performance computation.

---

## 16-bit Processing Preservation

The authoritative image representation remains 16-bit.

```text
Input
 ↓
uint16_t
 ↓
Processing
 ↓
uint16_t
 ↓
Display Conversion
 ↓
8-bit / CGImage
```

8-bit conversion is performed only when required for display or an explicitly requested output format.

---

## Async-First Execution

All potentially expensive operations should be asynchronous:

* Image loading
* TIFF decoding
* Histogram calculation
* Statistics
* Native processing
* Export

Swift Concurrency provides the primary execution model.

---

## Native Engine Reuse

The C++ engine should remain independent from the UI platform.

The same processing implementation can therefore support:

```text
Windows/WPF
       │
       └── C ABI
             │
             ▼
        C++ Engine
             ▲
             │
       C ABI
       │
macOS/SwiftUI
```

This is a major architectural advantage because the expensive image-processing algorithms do not need separate Windows and macOS implementations.

---

## Explicit Interoperability Boundary

The C ABI is the architectural seam between Swift and C++.

```text
Swift
  │
  │ C-compatible contract
  ▼
NativeExports
  │
  ▼
C++
```

This minimizes language coupling and makes the native engine independently testable.

---

## Session-Based Image State

The application maintains a logical image session:

```text
Original Image
      ↓
Processing Operation
      ↓
Processed Image
      ↓
Processing Operation
      ↓
Current Image
```

The original 16-bit image should remain recoverable throughout the session.

---

## Telemetry-Driven Observability

Instrumentation should cover important application boundaries:

```text
Image Open
    ↓
TIFF Decode
    ↓
Processing Request
    ↓
Native Processing
    ↓
Histogram
    ↓
Statistics
    ↓
Export
```

Useful telemetry dimensions include:

* Image dimensions
* Bit depth
* Processing algorithm
* Pixel count
* Processing duration
* Native execution duration
* Export duration
* Error category

Avoid recording sensitive image content or pixel data in telemetry.

---

## Recommended macOS Project Structure

```text
PhotonLab/
├── PhotonLab.xcodeproj
│
├── PhotonLabApp/
│   ├── App/
│   │   └── PhotonLabApp.swift
│   │
│   ├── Views/
│   │   ├── ContentView.swift
│   │   ├── ImageViewer.swift
│   │   ├── HistogramView.swift
│   │   ├── MetadataView.swift
│   │   └── ROIOverlay.swift
│   │
│   ├── ViewModels/
│   │   └── ImageViewModel.swift
│   │
│   ├── Services/
│   │   ├── ImageLoader.swift
│   │   ├── ImageRenderer.swift
│   │   ├── ImageProcessingService.swift
│   │   ├── ImageSessionService.swift
│   │   ├── HistogramService.swift
│   │   ├── StatisticsService.swift
│   │   └── ExportService.swift
│   │
│   ├── Models/
│   │   ├── ImageBuffer.swift
│   │   ├── LoadedImage.swift
│   │   ├── ImageMetadata.swift
│   │   ├── ImageStatistics.swift
│   │   └── HistogramData.swift
│   │
│   └── Platform/
│       ├── FilePicker.swift
│       ├── ImageIOAdapter.swift
│       └── MacOSImageRenderer.swift
│
├── Native/
│   └── PhotonLab.Native/
│       ├── Include/
│       │   ├── Core/
│       │   ├── Processing/
│       │   └── Interop/
│       │
│       └── Src/
│           ├── Core/
│           ├── Processing/
│           └── Interop/
│
└── Tests/
    ├── PhotonLabTests/
    ├── PhotonLabIntegrationTests/
    └── NativeTests/
```

---

## Cross-Platform Architecture

The strongest architecture for PhotonLab is therefore:

```text
                         PhotonLab
                            │
             ┌──────────────┴──────────────┐
             │                             │
        Windows Client                macOS Client
             │                             │
          WPF/MVVM                    SwiftUI/MVVM
             │                             │
             ▼                             ▼
       Application Services         Application Services
             │                             │
             │         C ABI                │
             └─────────────┬───────────────┘
                           ▼
                  ┌──────────────────┐
                  │ C++ Native Engine│
                  │                  │
                  │ Window / Level   │
                  │ Gamma            │
                  │ Median           │
                  │ Sharpen          │
                  │                  │
                  │ uint16_t         │
                  └──────────────────┘
```

The platform-specific layers own **user experience and operating-system integration**, while the C++ layer owns **performance-critical image computation**.

This gives PhotonLab a clean **cross-platform architecture with a shared native processing core**, rather than duplicating the most performance-sensitive part of the system for each operating system.
