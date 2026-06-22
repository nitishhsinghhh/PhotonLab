# Repository Structure

The repository structure intentionally mirrors architectural boundaries. Documentation, native processing, desktop presentation, shared contracts, testing infrastructure, and automation tooling are organized as independent concerns to improve maintainability, scalability, and team ownership.

## Table of Contents

1. [Documentation Structure](#documentation-structure)
    * [Architecture](#architecture)
    * [ADR (Architecture Decision Records)](#adr-architecture-decision-records)
    * [Performance](#performance)
    * [Testing](#testing)
    * [Decisions](#decisions)
    * [Images](#images)
2. [Repository Bootstrap Automation](#repository-bootstrap-automation)
    * [Purpose](#purpose)
    * [Responsibilities](#responsibilities)
    * [Safety Features](#safety-features)
    * [Design Philosophy](#design-philosophy)

---

## Documentation Structure

PhotonLab treats documentation as a first-class engineering artifact. Architectural decisions, trade-offs, testing strategies, and performance considerations are documented alongside the source code to improve maintainability and onboarding.

```text
Docs/
├── Architecture/
│   ├── Overview.md
│   ├── HighLevelDesign.md
│   ├── ProcessingPipeline.md
│   ├── NativeInterop.md
│   ├── DataFlow.md
│   └── RepositoryStructure.md
│
├── ADR/
│   ├── ADR-001-MVVM.md
│   ├── ADR-002-NativeCppEngine.md
│   ├── ADR-003-16BitProcessing.md
│   └── ADR-004-StrategyPattern.md
│
├── Performance/
│   ├── LargeImageHandling.md
│   ├── MemoryManagement.md
│   ├── SIMDStrategy.md
│   └── GPUAccelerationRoadmap.md
│
├── Testing/
│   ├── UnitTestingStrategy.md
│   ├── NativeTestingStrategy.md
│   └── IntegrationTesting.md
│
├── Decisions/
│   ├── Tradeoffs.md
│   └── TechnologySelection.md
│
└── Images/
```

---

### Architecture

Contains system-level design documentation including:

- Layered architecture (UI → ViewModel → Services → Native)
- Processing pipeline design
- Interop boundaries
- Data flow diagrams

---

### ADR (Architecture Decision Records)

Captures significant architectural decisions and the rationale behind them.

Examples:

* Why MVVM was selected.
* Why image processing is implemented in native C++.
* Why processing remains 16-bit until rendering.
* Why the Strategy Pattern was adopted.

---

### Performance

Documents system performance considerations including:

- Large image handling strategies
- Memory optimization techniques
- SIMD acceleration potential
- Rendering pipeline efficiency
- Future GPU acceleration roadmap

---

### Testing

Defines validation strategies across all layers:

- Unit testing (services, ViewModels)
- Integration testing (C# ↔ C++ interop)
- Native testing (C++ engine validation)

---

### Decisions

Captures technology evaluations, trade-offs, alternatives considered, and engineering reasoning.

### Images

Stores diagrams, architecture illustrations, workflow charts, screenshots, and other documentation assets referenced throughout the repository.

---

## Repository Bootstrap Automation

PhotonLab includes an automated repository provisioning utility located at:

```text
scripts/bootstrap-photonlab.sh
```

The bootstrap script is responsible for generating the complete repository structure required for development.

### Purpose

The script standardizes repository initialization and ensures that all developers begin with an identical project layout.

Rather than manually creating folders, projects, documentation, test infrastructure, and build assets, the bootstrap process provisions them automatically.

### Responsibilities

The bootstrap framework performs the following operations:

#### Repository Initialization

Creates repository root artifacts:

```text
README.md
LICENSE
.gitignore
.editorconfig
.clang-format
<ProjectName>.sln
```

#### Documentation Provisioning

Creates the complete documentation hierarchy:

```text
Docs/
├── Architecture/
├── ADR/
├── Performance/
├── Testing/
├── Decisions/
└── Images/
```

#### Native Engine Provisioning

Creates the native image-processing workspace:

```text
Native/
└── PhotonLab.Native/
```

Including:

* Core image models
* Processing pipeline
* Statistics engine
* Interop layer
* CMake configuration

#### Desktop Application Provisioning

Creates the WPF desktop application structure:

```text
Desktop/
└── PhotonLab.Desktop/
```

Including:

* Views
* ViewModels
* Services
* Commands
* Resources
* Interop

#### Shared Core Provisioning

Creates shared contracts and domain abstractions:

```text
Desktop/
└── PhotonLab.Core/
```

#### Testing Infrastructure

Creates:

```text
Tests/
├── PhotonLab.UnitTests/
├── PhotonLab.NativeTests/
└── TestData/
```

#### Build Infrastructure

Creates:

```text
Build/
├── CMake/
├── NuGet/
├── Packaging/
└── Release/
```

#### CI/CD Provisioning

Creates GitHub workflow scaffolding:

```text
.github/workflows/
├── build.yml
├── tests.yml
├── release.yml
└── codeql.yml
```

### Safety Features

The bootstrap framework contains multiple safety mechanisms.

#### Dependency Validation

Verifies required tooling before repository generation.

Examples:

* CMake
* Homebrew (macOS)
* Operating-system-specific package managers

#### Project Name Validation

Validates repository names using predefined naming rules.

Allowed:

```text
A-Z
a-z
0-9
.
_
-
```

#### Current Directory Protection

Prevents accidental execution inside the scripts directory.

#### Dry Run Mode

Supports preview execution without modifying the filesystem.

Example:

```bash
./scripts/bootstrap-photonlab.sh --dry-run
```

### Design Philosophy

The bootstrap framework treats repository structure as code.

All architectural boundaries, documentation standards, testing assets, and build infrastructure are provisioned from a single source of truth, ensuring consistency across environments and reducing manual setup effort.
