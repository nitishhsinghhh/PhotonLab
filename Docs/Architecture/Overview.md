# Overview

This document outlines the layered architecture of the PhotonLab platform. The core philosophy is to separate concerns by using MVVM (Model-View-ViewModel) for the presentation layer and a Service-Oriented approach for the high-performance C++ backend.

## Table of Contents

1. [Design Goals](#design-goals)
2. [MVVM Rationale](#mvvm-rationale)
3. [System Overview](#system-overview)
    * [Component Responsibilities](#component-responsibilities)

---

## Design Goals

- **Performance First**
  - Offload compute-heavy image operations to a native C++ engine.
  - Preserve 16-bit precision throughout processing pipelines.
  - Optimize rendering via minimal allocation display buffers.

- **Decoupling**
  - UI layer remains independent of processing logic.
  - Native engine is isolated behind service abstractions.
  - Enables headless testing and future UI replacement.

- **Extensibility**
  - New image processing algorithms can be added without UI changes.
  - Service interfaces define clear contracts between layers.

- **Maintainability**
  - Strict dependency direction: UI → ViewModel → Services → Native.
  - Avoids leaking unmanaged complexity into presentation layer.

---

## MVVM Rationale

The MVVM pattern is used to manage complexity in binding high-throughput image data to a responsive UI.

### Key Benefits:

- **Testability**
  - ViewModels and services can be unit tested without a WPF runtime.

- **Responsiveness**
  - All heavy operations are executed asynchronously via `ExecuteAsync()`.
  - UI thread remains free from image processing workloads.

- **Separation of Concerns**
  - XAML handles presentation.
  - ViewModel handles orchestration and state.
  - Services handle processing and native communication.

---

## System Overview

PhotonLab is divided into two primary domains:

- **Managed Layer (C#)**
- **Unmanaged Layer (C++ Native Engine)**

The managed layer handles UI, orchestration, and service coordination, while the native layer performs high-performance image processing.

---

### Component Responsibilities

* UI/ViewModel (Managed): Manages user interaction, viewport state, and async command execution.
* Application Services (Managed): Orchestrates the business logic and acts as the bridge to the native engine via P/Invoke or C++/CLI.
* Native Engine (Unmanaged): The C++ core responsible for high-speed computation, direct memory access, and SIMD-accelerated image filtering.

---

### ViewModel Layer (`MainViewModel`)

Acts as the **central orchestration engine** of the application.

Key responsibilities:

- Maintains UI state:
  - `ImageSource`
  - `ZoomFactor`
  - `StatusMessage`
  - `IsBusy`
- Executes all operations via `ExecuteAsync()`
- Triggers processing workflows:
  - Gamma correction
  - Median filter
  - Sharpen filter
  - Window/Level adjustment
- Coordinates:
  - Display refresh (`RefreshDisplay`)
  - Statistics refresh (`RefreshStatistics`)
  - Histogram refresh (`RefreshHistogram`)
- Emits telemetry via OpenTelemetry (`ActivitySource`)

---

### Application Services

This layer contains business logic and processing coordination.

Key services:

- `ITiffLoader` → Loads TIFF into `ushort[]`
- `IImageProcessingService` → Applies filters and transformations
- `IImageRenderer` → Converts `ushort[] → BGRA32`
- `IImageSessionService` → Maintains image state
- `IExportService` → Handles image export (PNG/JPEG/TIFF)

Responsibilities:

- Abstract native engine access
- Maintain session-based image state
- Provide async processing APIs
- Compute statistics and histogram data

---

### Native Engine (C++)

The performance-critical layer responsible for:

- Window/Level transformation
- Gamma correction
- Median filtering
- Sharpening filters
- High-performance buffer manipulation

Characteristics:

- Operates on `ushort[]` (16-bit buffers)
- SIMD-optimized processing paths
- No UI or .NET dependencies
- Exposed via P/Invoke ABI boundary

---

## System Overview Diagram

```text 
    WPF UI
    ↓
    MainViewModel (ExecuteAsync orchestration)
    ↓
    Application Services (Processing / Rendering / Session / Export)
    ↓
    P/Invoke Boundary
    ↓
    Native C++ Engine (16-bit image processing)
```

---

## Key Architectural Principles

1. Strict Layering
- UI never directly calls services or native code.
2. Single Orchestration Point
- MainViewModel.ExecuteAsync() controls all processing flows.
3. 16-bit Processing Integrity
- All computation occurs on ushort[] buffers.
4. 8-bit Only for Rendering
- Conversion happens only inside renderer.
5. Async-First Execution
- No blocking operations on UI thread.
6. Telemetry-Aware Design
- OpenTelemetry tracks every major operation.