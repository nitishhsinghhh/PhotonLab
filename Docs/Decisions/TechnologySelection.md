# Technology Selection

This document records the key technology choices made in PhotonLab, along with the rationale behind each decision. The goal is to ensure long-term maintainability, performance alignment, and architectural consistency across the system.

---

## Table of Contents

1. [Frontend Technology (WPF)](#frontend-technology-wpf)
2. [Language Choice (C# and C++)](#language-choice-c-and-c)
3. [Architecture Pattern (MVVM)](#architecture-pattern-mvvm)
4. [Native Engine (C++)](#native-engine-c)
5. [Image Processing Strategy](#image-processing-strategy)
6. [Interoperability Approach](#interoperability-approach)
7. [Logging and Observability](#logging-and-observability)
8. [Testing Strategy](#testing-strategy)

---

## Frontend Technology (WPF)

PhotonLab uses **Windows Presentation Foundation (WPF)** as the UI framework.

### Reasoning

- Mature desktop UI framework for Windows
- Excellent data binding support via MVVM
- Strong rendering pipeline for image-based applications
- Seamless integration with `BitmapSource` for image visualization

### Outcome

WPF enables a responsive, highly interactive imaging UI suitable for diagnostic workloads.

---

## Language Choice (C# and C++)

PhotonLab uses a **hybrid language architecture**:

- C# → UI, orchestration, services
- C++ → performance-critical image processing

### Reasoning

- C# provides rapid development and strong ecosystem support
- C++ enables low-level memory control and SIMD optimization
- Separation ensures UI responsiveness under heavy computation

---

## Architecture Pattern (MVVM)

PhotonLab is built using the **MVVM (Model-View-ViewModel)** pattern.

### Reasoning

- Clean separation between UI and logic
- Testable ViewModels and services
- Simplified UI binding using `INotifyPropertyChanged`
- Supports asynchronous orchestration via `ExecuteAsync`

### Outcome

`MainViewModel` acts as the central orchestration layer of the application.

---

## Native Engine (C++)

The image processing engine is implemented in **C++**.

### Reasoning

- High-performance computation for 16-bit image processing
- Direct memory access for large buffers
- SIMD optimization potential
- Avoids GC overhead for heavy operations

### Responsibilities

- Window/Level transformation
- Gamma correction
- Median filtering
- Sharpening
- Histogram generation (low-level support)

---

## Image Processing Strategy

PhotonLab uses a **16-bit-first processing pipeline**.

### Key Principles

- All processing occurs on `ushort[]` buffers
- No transformation on 8-bit display buffers
- Rendering is a separate visualization step

### Reasoning

- Preserves diagnostic accuracy
- Prevents cumulative precision loss
- Supports high dynamic range imaging workflows

---

## Interoperability Approach

PhotonLab uses **P/Invoke-based native interop**.

### Reasoning

- Lightweight and fast interoperability mechanism
- Avoids overhead of COM or C++/CLI where unnecessary
- Keeps boundary explicit and controlled

### Design Rules

- No exceptions across managed/unmanaged boundary
- Only primitive arrays are passed (`ushort[]`, `byte[]`)
- Memory ownership remains clearly defined per layer

---

## Logging and Observability

PhotonLab uses structured logging and OpenTelemetry.

### Tools

- `ILogger<T>` for structured logs
- `ActivitySource` for tracing operations

### Reasoning

- Enables performance debugging
- Tracks image processing latency
- Provides observability into native execution calls

---

## Testing Strategy

PhotonLab uses a layered testing approach:

### Unit Tests
- Services
- ViewModels
- Histogram and statistics logic

### Integration Tests
- C# ↔ C++ interop validation
- TIFF load → render pipeline

### Native Tests
- C++ algorithm correctness
- Performance validation of filters

---

## Summary

Technology choices in PhotonLab are driven by three core principles:

- Performance for large 16-bit image processing
- Maintainability through clean layered architecture
- Observability through structured logging and tracing

These decisions ensure the system remains scalable, testable, and suitable for high-performance imaging workloads.