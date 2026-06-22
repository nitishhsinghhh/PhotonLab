# ADR-002: Adoption of Native C++ Image Processing Engine

## Status

Accepted

---

## Context

PhotonLab processes high-resolution 16-bit TIFF images, requiring:

- High-throughput pixel-level computation
- Low-latency image filtering operations
- Efficient memory handling for large buffers
- Real-time responsiveness in a desktop UI environment

The managed .NET runtime provides productivity benefits but introduces limitations for:

- SIMD-level optimization
- Direct memory manipulation
- Predictable low-level performance tuning
- Fine-grained control over allocation patterns

Given the computational intensity of operations such as:

- Window/Level transformation
- Gamma correction
- Median filtering
- Sharpening filters
- Histogram generation

a managed-only implementation would introduce unacceptable performance overhead.

---

## Decision

PhotonLab adopts a **Native C++ image processing engine** responsible for all performance-critical image operations.

The engine is integrated into the managed application through a thin interoperability layer using P/Invoke.

---

## Rationale

### 1. Performance Requirements

Image processing must operate on large buffers efficiently:

- 16-bit grayscale images
- Multi-megapixel resolution datasets
- Real-time UI interaction expectations

C++ enables:

- Direct memory access
- Cache-friendly loop optimization
- SIMD (SSE/AVX) acceleration opportunities
- Minimal runtime overhead

---

### 2. Memory Control

C++ allows explicit control over:

- Buffer allocation and deallocation
- Avoiding GC pressure from large arrays
- In-place processing strategies

This is critical for maintaining responsiveness in the WPF UI layer.

---

### 3. Deterministic Execution

Native code provides:

- Predictable execution timing
- Reduced runtime variability
- Better suitability for performance-critical pipelines

This is essential for interactive imaging workflows.

---

### 4. Separation of Concerns

The architecture enforces a strict boundary:

| Layer | Responsibility |
|------|----------------|
| C# (Managed) | UI, orchestration, state management |
| C++ (Native) | Pixel-level computation |

This prevents performance logic from leaking into UI code.

---

### 5. Scalability for Future Optimization

The native engine enables future enhancements such as:

- SIMD vectorization
- Multi-threaded tiling pipelines
- GPU acceleration (future roadmap)
- Memory-mapped image processing

---

## Consequences

### Positive

- Significant performance improvement for image processing
- Reduced GC pressure in managed runtime
- Better scalability for large images
- Clear separation between UI and compute layers

---

### Negative

- Increased system complexity due to interop boundary
- Requires careful memory ownership management
- Additional build and debugging complexity (C++ toolchain)
- Platform dependency (Windows-centric implementation)

---

## Alternatives Considered

### 1. Pure C# Implementation (Rejected)

Rejected due to:

- Insufficient performance for large 16-bit images
- Garbage collection overhead
- Limited SIMD optimization compared to C++

---

### 2. GPU-Based Processing (Deferred)

Considered but deferred because:

- Adds significant infrastructure complexity
- Requires GPU compatibility management
- Not necessary for initial workstation requirements

Planned for future enhancement phase.

---

### 3. Third-Party Imaging Libraries (Rejected)

Rejected due to:

- Lack of full control over processing pipeline
- Licensing constraints
- Limited extensibility for custom algorithms

---

## Outcome

The Native C++ engine provides a high-performance core for PhotonLab, enabling:

- Real-time image manipulation
- Efficient memory usage
- Scalable processing architecture

It forms the computational backbone of the system, while the C# layer focuses on orchestration and user interaction.

---

## Notes

All native functions must adhere to the following rules:

- No exceptions cross the managed boundary
- Memory ownership must remain explicit
- Only primitive buffers are exchanged (`ushort[]`, `byte[]`)
- Processing must remain stateless where possible