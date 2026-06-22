# ADR-003: End-to-End 16-Bit Image Processing Pipeline

## Status

Accepted

---

## Context

PhotonLab is designed as a high-performance imaging workstation for processing 16-bit TIFF images. These images are commonly used in medical, scientific, and diagnostic workflows where precision is critical.

Standard desktop rendering pipelines operate on 8-bit per channel data, which introduces limitations:

- Loss of intensity precision
- Banding artifacts in gradients
- Reduced dynamic range
- Cumulative degradation when applying multiple transformations

Given PhotonLab’s requirement for:

- Window/Level adjustment
- Gamma correction
- Median filtering
- Sharpening
- Histogram analysis

it is essential to preserve full 16-bit precision throughout the processing lifecycle.

---

## Decision

PhotonLab adopts a **strict 16-bit-first processing pipeline**, where all image processing operations are performed on `ushort[]` buffers.

Conversion to 8-bit is only performed at the final rendering stage for visualization purposes.

---

## Rationale

### 1. Preservation of Image Fidelity

16-bit depth provides:

- 65,536 intensity levels per pixel
- Higher dynamic range compared to 8-bit (256 levels)
- Better representation of subtle intensity variations

This is critical for diagnostic-quality imaging.

---

### 2. Non-Destructive Processing

All operations are applied to the 16-bit source buffer:

- Window/Level transformations
- Gamma correction
- Spatial filtering
- Statistical analysis

This ensures that repeated operations do not degrade image quality.

---

### 3. Separation of Processing and Rendering

PhotonLab clearly separates:

| Stage | Bit Depth | Purpose |
|------|----------|--------|
| Processing | 16-bit (`ushort[]`) | Image computation and transformation |
| Rendering | 8-bit (`byte[]`) | UI visualization |

This avoids mixing analytical and display concerns.

---

### 4. Accurate Histogram and Statistics

All analytics operate on the 16-bit dataset:

- Histogram generation
- Min/Max intensity calculation
- Mean and standard deviation
- Median filtering

This ensures statistically accurate results.

---

### 5. Compatibility with Native Engine

The C++ engine operates directly on:

- `uint16_t*` buffers

This aligns naturally with the 16-bit processing pipeline and avoids unnecessary conversions.

---

## Consequences

### Positive

- High-fidelity image processing
- No cumulative quality loss across operations
- Accurate analytical outputs (histograms, statistics)
- Consistent processing model across all filters

---

### Negative

- Increased memory usage compared to 8-bit pipelines
- Slightly higher computational cost
- Requires careful optimization in rendering conversion step

---

## Alternatives Considered

### 1. 8-Bit Processing Pipeline (Rejected)

Rejected due to:

- Loss of diagnostic detail
- Poor suitability for scientific imaging
- Artifacts introduced by repeated transformations

---

### 2. Mixed Precision Pipeline (Rejected)

Using 16-bit for input and 8-bit for processing was rejected because:

- It introduces inconsistent processing semantics
- Leads to precision loss during intermediate steps
- Complicates filter behavior and testing

---

### 3. Floating-Point Processing Pipeline (Deferred)

Considered using `float`-based processing:

- Offers high precision
- Useful for advanced imaging operations

Deferred due to:

- Higher memory and performance cost
- Not required for current feature set

---

## Outcome

The 16-bit-first pipeline ensures that PhotonLab maintains:

- Diagnostic-grade image quality
- Consistent processing accuracy
- Reliable statistical outputs
- Clean separation between processing and rendering

This decision is foundational to the system’s imaging integrity.

---

## Notes

All processing components must adhere to:

- Input: `ushort[]` (16-bit buffer)
- Output: `ushort[]` (16-bit buffer)
- No intermediate 8-bit conversions in processing pipeline
- Rendering conversion is strictly a final-stage operation