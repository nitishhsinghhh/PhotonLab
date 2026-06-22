# ADR-004: Adoption of Strategy Pattern for Image Processing Operations

## Status

Accepted

---

## Context

PhotonLab supports multiple image processing operations, including:

- Window/Level adjustment
- Gamma correction
- Median filtering
- Sharpening
- Future extensible filters (CLAHE, edge detection, etc.)

These operations share a common characteristic:

- They operate on 16-bit image buffers (`ushort[]`)
- They produce transformed output buffers
- They may evolve independently over time

Without a structured approach, the system would risk:

- Tight coupling between processing logic and orchestration
- Large monolithic processing classes
- Difficulty adding or modifying algorithms
- Increased regression risk when updating filters

A flexible design pattern is required to support extensibility and maintain separation of concerns.

---

## Decision

PhotonLab adopts the **Strategy Pattern** for implementing image processing operations in the native engine and managed service layer.

Each image processing algorithm is encapsulated as an independent strategy that can be selected and executed at runtime.

---

## Rationale

### 1. Encapsulation of Algorithms

Each processing operation is isolated:

- GammaCorrectionStrategy
- MedianFilterStrategy
- SharpenStrategy
- WindowLevelStrategy

This ensures that each algorithm:

- Has a single responsibility
- Can be modified independently
- Does not affect other processing logic

---

### 2. Runtime Flexibility

The Strategy Pattern allows dynamic selection of processing behavior:

- UI triggers a processing command
- ViewModel selects appropriate strategy
- Service layer executes selected algorithm

This enables flexible workflows without modifying core orchestration logic.

---

### 3. Extensibility

New algorithms can be added without modifying existing code:

Example additions:

- CLAHE (Contrast Limited Adaptive Histogram Equalization)
- Edge detection filters
- Noise reduction techniques

This follows the Open/Closed Principle:

> Open for extension, closed for modification

---

### 4. Separation of Concerns

The pattern ensures a clean separation:

| Component | Responsibility |
|----------|----------------|
| Strategy | Implements a specific image operation |
| Context (Service Layer) | Selects and executes strategy |
| ViewModel | Orchestrates user-driven commands |

---

### 5. Testability

Each strategy can be tested independently:

- Unit tests for individual filters
- Validation of output buffers
- Performance benchmarking per algorithm

This improves reliability of the processing pipeline.

---

## Consequences

### Positive

- Highly extensible processing architecture
- Improved maintainability of image algorithms
- Easier unit testing of individual filters
- Reduced coupling between processing operations

---

### Negative

- Slight increase in number of classes and interfaces
- Requires clear strategy selection logic
- Additional abstraction layer may add minor overhead

---

## Alternatives Considered

### 1. Monolithic Processing Class (Rejected)

Rejected due to:

- Poor scalability
- Difficult maintenance
- High risk of regression when modifying filters

---

### 2. Static Utility Methods (Rejected)

Rejected because:

- No runtime flexibility
- Harder to test and extend
- Violates single responsibility principle

---

### 3. Inheritance-Based Design (Rejected)

Rejected due to:

- Tight coupling between base and derived classes
- Limited flexibility compared to composition
- Difficult to extend multiple orthogonal behaviors

---

## Outcome

The Strategy Pattern provides PhotonLab with a modular and extensible processing architecture that:

- Supports future algorithm growth
- Maintains clean separation of concerns
- Enables independent development of image filters
- Improves testability and maintainability

It serves as the foundation for all image processing operations within both the managed and native layers.

---

## Notes

All processing strategies must adhere to:

- Input: `ushort[]` (16-bit buffer)
- Output: `ushort[]` (16-bit buffer)
- Stateless execution preferred
- No UI or ViewModel dependencies