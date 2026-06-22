# Processing Pipeline

## Table of Contents

1. [Rendering Flow 16-bit to 8-bit](#rendering-flow-16-bit-to-8-bit)
2. [Processing Principle](#processing-principle)
3. [Window and Level](#window-and-level)
4. [Filter Pipeline](#filter-pipeline)
    * [Gamma Correction](#gamma-correction)
    * [Median Filter](#median-filter)
    * [Sharpen Filter](#sharpen-filter)
5. [Pipeline Design Principles](#pipeline-design-principles)

---

## Rendering Flow 16-bit to 8-bit

PhotonLab preserves image fidelity by maintaining **16-bit precision (`ushort[]`) throughout all processing operations**. Conversion to 8-bit occurs only at the rendering stage for WPF visualization.

Rendering is handled by the `IImageRenderer` service and produces a `BitmapSource` for UI binding.

```text
    16-bit TIFF File
            │
            ▼
    ushort[] Raw Image Buffer
            │
            ▼
    IImageProcessingService (Filters / Transformations)
            │
            ▼
    Processed ushort[] Buffer
            │
            ▼
    IImageRenderer (Window/Level Mapping)
            │
            ▼
    BGRA32 byte[] Display Buffer
            │
            ▼
    BitmapSource
            │
            ▼
    WPF Image Control
```

---

### Processing Principle

* Load images as 16-bit grayscale data.
* Perform all image processing operations in 16-bit precision.
* Avoid intermediate precision loss.
* Convert to 8-bit only for screen rendering.

This approach ensures maximum image quality and preserves diagnostic information during processing.

---

## Window and Level

Window/Level is a display transformation stage, not a destructive operation.

It is typically applied during rendering inside the renderer.

Window
Defines the intensity range visible on screen.

- Wider window → lower contrast, more grayscale range visible
- Narrow window → higher contrast, focused intensity range
Level

Defines the center of the intensity range.

- Higher level → image appears brighter
- Lower level → image appears darker

```text
    Processed ushort[]
            │
            ▼
    Window / Level Transformation (Renderer)
            │
            ▼
    Mapped 8-bit Display Values
            │
            ▼
    BitmapSource (UI)
```

---

## Filter Pipeline

PhotonLab applies image processing as a chain of transformations on a 16-bit working buffer.

All filters are executed via IImageProcessingService and coordinated by MainViewModel.ExecuteAsync().

```text
    Original ushort[]
            │
            ▼
    Window/Level (optional visualization stage)
            │
            ▼
    Gamma Correction
            │
            ▼
    Median Filter
            │
            ▼
    Sharpen Filter
            │
            ▼
    Processed ushort[]
```

---

### Gamma Correction

Gamma correction adjusts perceived brightness in a non-linear intensity space.

Purpose
- Enhance darker regions
- Improve visual contrast
- Preserve highlights while adjusting mid-tones

Characteristics
- Applied on 16-bit pixel values
- Non-destructive transformation
- Fully reversible only if original buffer is preserved

---

### Median Filter

The median filter is a noise reduction operator.

Purpose
- Remove salt-and-pepper noise
- Preserve edges better than averaging filters

Characteristics
- Operates on local pixel neighborhoods
- Replaces pixel with median value of surrounding window
- Reduces noise without blurring sharp structures significantly

---

### Sharpen Filter

Sharpening enhances edge contrast and local detail visibility.

Purpose

- Improve structural clarity
- Emphasize edges and boundaries
- Increase perceived sharpness

Characteristics
- Enhances high-frequency components
- Applied on 16-bit buffer
- May amplify noise if overused

---

### Pipeline Design Principles

1. 16-bit First Principle
All processing is performed on ushort[] buffers.

2. Renderer Isolation
Rendering logic is separated from processing logic.

3. Non-Destructive Design
Window/Level affects only visualization.

4. Service-Based Execution
Filters are implemented in IImageProcessingService.

5. ViewModel Orchestration
All operations are triggered via ExecuteAsync().

6. Stateless Rendering
UI state is derived, not stored in rendering pipeline
