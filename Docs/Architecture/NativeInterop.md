# Native Interoperability

This document defines the architectural contract between the managed .NET application and the native C++ engine, establishing strict boundaries for memory ownership, inter-process communication, and error propagation to ensure system stability and high-performance execution.

## Table of Contents

1. [C# ↔ C++ Architecture](#c--c-architecture)
2. [P/Invoke Boundary](#pinvoke-boundary)
3. [Memory Ownership Model](#memory-ownership-model)
4. [Error Handling Strategy](#error-handling-strategy)

---

## C# ↔ C++ Architecture

PhotonLab separates **UI orchestration** and **image processing computation** using a layered architecture:

- WPF UI handles rendering and user interaction
- `MainViewModel` orchestrates execution flow
- Application Services manage processing logic
- Native C++ engine performs CPU-intensive image operations

```text
   WPF UI
      ↓
   MainViewModel (ExecuteAsync orchestration)
      ↓
   Application Services (Processing / Rendering / Session)
      ↓
   P/Invoke Boundary
      ↓
   Native C++ Engine (16-bit image processing)
```

---

## P/Invoke Boundary

Communication between managed and native layers is implemented using a C-style ABI exposed by the native engine and consumed via P/Invoke in C#.

Key characteristics:

- Flat function exports (no C++ classes exposed)
- Stable binary contract between builds
- Explicit buffer passing (ushort[], byte[])
- No direct object serialization across boundary

Example responsibilities:

- ApplyGamma()
- ApplyMedianFilter()
- ApplySharpen()
- ApplyWindowLevel()

The boundary ensures isolation between runtime environments while maintaining high-throughput data transfer.

---

## Memory Ownership Model

PhotonLab follows a strict ownership separation model to prevent memory corruption and reduce GC pressure.

Rules:

- .NET owns managed arrays (ushort[], byte[])
- Native engine operates only on buffers passed from .NET
- No implicit memory transfer across boundary
- No shared heap allocations between runtimes

Buffer Strategy:

- Input: Managed 16-bit buffer (ushort[])
- Processing: Native in-place or output buffer
- Output: Managed buffer returned or reused

```text
   Managed ushort[]
         ↓
   Pinned (if required for native call)
         ↓
   Native processing (C++)
         ↓
   Updated buffer returned to managed layer
```

This ensures predictable memory behavior and avoids unnecessary allocations during processing.

---

## Execution Model

Unlike traditional interop designs, PhotonLab does not call native functions directly from the UI.

Instead, execution flows through a controlled orchestration pipeline:

```text
   UI Command
      ↓
   RelayCommand
      ↓
   MainViewModel.ExecuteAsync()
      ↓
   Set IsBusy / StatusMessage
      ↓
   IImageProcessingService
      ↓
   P/Invoke call to Native Engine
      ↓
   RefreshDisplay()
      ↓
   RefreshStatistics()
      ↓
   RefreshHistogram()
      ↓
   UI Update (INotifyPropertyChanged)

```

Guarantees:

- UI thread is never blocked
- All native calls are asynchronous-safe
- All operations are observable via telemetry
- Consistent state updates after each operation

---

## Error Handling Strategy

Native exceptions do not cross the managed boundary.

Instead, the system uses explicit return codes and structured error translation.

Native Layer:

- Returns integer status codes (e.g., 0 = success, non-zero = failure)
- Avoids throwing exceptions across ABI boundary

Managed Layer:

- Converts status codes into exceptions or user messages
- Logs errors via ILogger
- Attaches telemetry via OpenTelemetry (ActivitySource)

```text
   Native C++ Failure
         ↓
   Return Error Code
         ↓
   C# Service Layer Interpretation
         ↓
   Exception / MessageBox / Logging
         ↓
   ViewModel UI Feedback
```

Benefits:

- Stable interop boundary
- Predictable failure handling
- No undefined runtime behavior across C++/CLI boundary
- Improved debuggability in production environments

---

## Design Principles

- No direct UI ↔ Native communication
- Strict buffer ownership rules
- Flat ABI for native exports only
- No exceptions across interop boundary
- All processing flows through service layer
- Telemetry and logging at every boundary transition
