# ADR-001: Adoption of MVVM Architecture

## Status

Accepted

---

## Context

PhotonLab is a high-performance 16-bit TIFF imaging workstation built using WPF. The application requires a clear separation between:

- UI rendering logic
- Image processing workflows
- Native C++ computation engine
- Application state management

The system must support:

- Real-time UI updates during image processing
- Asynchronous execution of heavy computations
- Testable and maintainable presentation logic
- Clean separation between UI and business logic

Given the complexity of image processing operations (filters, window/level adjustments, histogram generation), embedding logic directly into UI code-behind would lead to tight coupling and poor maintainability.

---

## Decision

We adopt the **MVVM (Model-View-ViewModel)** pattern as the primary architectural pattern for the PhotonLab desktop application.

---

## Rationale

### 1. Separation of Concerns

MVVM clearly separates responsibilities:

| Layer | Responsibility |
|------|----------------|
| View | UI rendering (XAML) |
| ViewModel | State management + UI logic |
| Model | Data structures and domain objects |

This ensures that UI changes do not affect business logic.

---

### 2. Testability

ViewModels can be unit tested without requiring:

- WPF runtime
- UI rendering
- Native engine execution

This is critical for validating:

- Image processing orchestration
- Histogram and statistics logic
- Command execution flow

---

### 3. Data Binding Efficiency

WPF data binding with `INotifyPropertyChanged` enables:

- Automatic UI updates
- Reduced boilerplate code
- Reactive UI behavior

This is particularly useful for:

- Zoom level updates
- Histogram refresh
- Pixel intensity display
- Image metadata updates

---

### 4. Asynchronous Processing Support

MVVM supports clean integration with asynchronous workflows:

- Image loading (`LoadAsync`)
- Processing operations (`ApplyGammaAsync`, `ApplyMedianAsync`)
- Histogram/statistics refresh

The ViewModel acts as a coordination layer without blocking the UI thread.

---

### 5. Maintainability

Separating logic into ViewModels ensures:

- UI designers can modify XAML independently
- Developers can work on processing logic independently
- Reduced merge conflicts in large-scale development

---

## Consequences

### Positive

- Clean separation between UI and logic
- Improved testability of core workflows
- Easier onboarding for new developers
- Scalable architecture for additional imaging features

---

### Negative

- Increased number of classes (ViewModels, Commands, Services)
- Requires strict discipline to avoid logic leaking into Views
- Slight learning curve for developers unfamiliar with MVVM

---

## Alternatives Considered

### 1. Code-Behind (Rejected)

Embedding logic in WPF code-behind was rejected due to:

- Tight coupling of UI and business logic
- Poor testability
- Difficult maintenance for complex workflows

---

### 2. MVC (Rejected)

MVC was not suitable because:

- Not optimized for desktop data-binding scenarios
- Requires manual UI synchronization
- Less efficient for real-time UI updates

---

### 3. MVP (Partially Considered)

MVP provides separation but:

- Requires more manual UI updates
- Does not integrate as naturally with WPF data binding

---

## Outcome

MVVM provides the best balance of:

- Maintainability
- Testability
- UI responsiveness
- Integration with WPF data binding

It forms the foundation of PhotonLab’s presentation layer architecture, with `MainViewModel` acting as the central orchestration component.

---

## Notes

All ViewModels in PhotonLab must adhere to the following principles:

- No direct UI manipulation
- All state changes must raise property notifications
- All long-running operations must be asynchronous
- Business logic must be delegated to services