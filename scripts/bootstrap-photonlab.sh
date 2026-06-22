#!/usr/bin/env bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
# SYSTEM      : PhotonLab Imaging Platform                           */
# SUBSYSTEM   : Repository Bootstrap Framework                       */
# COMPONENT   : bootstrap-photonlab                                  */
# VERSION     : 1.2                                                  */
#                                                                    */
# DESCRIPTION : Generates the complete PhotonLab repository          */
#               structure including native image processing          */
#               engine, desktop application, shared contracts,       */
#               testing infrastructure, documentation hierarchy,     */
#               CI/CD configuration, and development tooling.        */
#                                                                    */
# USAGE         : ./scripts/bootstrap-photonlab.sh                   */
#                 ./scripts/bootstrap-photonlab.sh --dry-run         */
#                                                                    */
# FEATURES    :                                                      */
#                 * Native C++ engine workspace                      */
#                 * WPF/.NET desktop architecture                    */
#                 * Shared core contracts                            */
#                 * Test infrastructure provisioning                 */
#                 * Architecture documentation layout                */
#                 * CI/CD workflow scaffolding                       */
#                 * Environment dependency validation (pre-flight)   */
#                                                                    */
# SIDE EFFECTS: Verifies system tools, creates project directories,  */
#               generates placeholder source files, initializes      */
#               repository, and provisions development assets.       */
#                                                                    */
# LICENSE     : Apache License, Version 2.0                          */
#                                                                    */
# AUTHOR      : Nitish Singh                                         */
#                                                                    */
# DEPENDENCIES:                                                      */
#                 * Bash, mkdir, create_file, chmod                  */
#                 * CMake (Verified at runtime)                      */
#                                                                    */
# LOCATION    : scripts/bootstrap-photonlab.sh                       */
#                                                                    */
# REVISION HISTORY:                                                  */
# ------------------------------------------------------------------ */
# Ver  Date        Author          Description                       */
# ---  ----------  -------------   -------------------------------   */
# 1.0  2026-06-09  Nitish Singh    Initial Repository Bootstrap      */
# 1.1  2026-06-09  Nitish Singh    Added Pre-flight Env Validation   */
# 1.2  2026-06-10  Nitish Singh    Added Idempotent File Creation    */
#                                  Prevented Existing File Overwrite */
#*********************************************************************/

#*********************************************************************/
# Logging Framework                                                  */
#*********************************************************************/

set -euo pipefail

trap 'log_error "Bootstrap interrupted. Cleanup might be required."; exit 1' SIGINT SIGTERM

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

timestamp(){ date +"%Y-%m-%d %H:%M:%S"; }
log_info(){ echo -e "${BLUE}[$(timestamp)] [INFO]${NC} $1"; }
log_success(){ echo -e "${GREEN}[$(timestamp)] [SUCCESS]${NC} $1"; }
log_warn(){ echo -e "${YELLOW}[$(timestamp)] [WARN]${NC} $1"; }
log_error(){ echo -e "${RED}[$(timestamp)] [ERROR]${NC} $1"; }

#*********************************************************************/
# Dry Run Logic                                                      */
#*********************************************************************/

DRY_RUN=false
if [[ "${1:-}" == "--dry-run" || "${2:-}" == "--dry-run" ]]; then
    DRY_RUN=true
    log_warn "DRY RUN MODE ENABLED: No files or directories will be created."
fi

#*********************************************************************/
# Helper Functions                                                   */
#*********************************************************************/

create_dir() {
    if [ "$DRY_RUN" = true ]; then
        echo -e "${YELLOW}[DRY-RUN]${NC} mkdir -p $1"
    else
        mkdir -p "$1"
    fi
}

create_file() {
    if [ "$DRY_RUN" = true ]; then
        echo -e "${YELLOW}[DRY-RUN]${NC} touch $1"
    else
        [[ -f "$1" ]] || touch "$1"
    fi
}

write_file() {
    local target="$1"
    local content="$2"

    if [ "$DRY_RUN" = true ]; then
        if [[ -f "$target" ]]; then
            echo -e "${YELLOW}[DRY-RUN]${NC} Would skip existing file $target"
        else
            echo -e "${YELLOW}[DRY-RUN]${NC} Would create file $target"
        fi
    else
        if [[ ! -f "$target" ]]; then
            cat > "$target" <<EOF
$content
EOF
        else
            log_warn "$target already exists. Skipping."
        fi
    fi
}

#*********************************************************************/
# Project Configuration                                              */
#*********************************************************************/

DEFAULT_PROJECT_NAME="PhotonLab"
INPUT_NAME=""

# Parse arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        --dry-run)
            DRY_RUN=true
            shift
            ;;
        *)
            INPUT_NAME="$1"
            shift
            ;;
    esac
done

echo "---------------------------------------------------------------"
echo "Bootstrap: PhotonLab Repository Generator"
echo "Note: If you select '.' (current directory), this folder will be"
echo "transformed into the project root without creating a new subfolder."
echo "---------------------------------------------------------------"

# Prompt if no name was provided via CLI
if [[ -z "$INPUT_NAME" ]]; then
    read -rp "Enter project name [${DEFAULT_PROJECT_NAME}] (or '.' for current directory): " INPUT_NAME
fi

# Default to default name if input is empty
INPUT_NAME="${INPUT_NAME:-$DEFAULT_PROJECT_NAME}"

# Handle current directory vs new directory
if [[ "$INPUT_NAME" == "." ]]; then
    PROJECT_NAME="$(basename "$PWD")"
    log_info "Using current directory ('$PWD') as project root."
    # We do NOT cd or mkdir, we stay here
else
    PROJECT_NAME="$INPUT_NAME"
    log_info "Creating new project directory: ${PROJECT_NAME}"
    create_dir "$PROJECT_NAME"
    cd "$PROJECT_NAME"
fi

# Remove leading/trailing spaces from name for validation
PROJECT_NAME="$(echo "$PROJECT_NAME" | xargs)"

# validate project name
if [[ ! "$PROJECT_NAME" =~ ^[A-Za-z][A-Za-z0-9._-]*$ ]]; then
    log_error "Invalid project name: '$PROJECT_NAME'"
    log_error "Project name must start with a letter and contain only:"
    log_error "A-Z a-z 0-9 . _ -"
    exit 1
fi

log_info "Bootstrapping ${PROJECT_NAME}..."

readonly DIR_NATIVE="Native/${PROJECT_NAME}.Native"
readonly DIR_DESKTOP="Desktop/${PROJECT_NAME}.Desktop"
readonly DIR_CORE="Desktop/${PROJECT_NAME}.Core"
readonly DIR_TESTS="Tests/${PROJECT_NAME}.UnitTests"
readonly DIR_NATIVE_TESTS="Tests/${PROJECT_NAME}.NativeTests"

#*********************************************************************/
# Environment Validation & Safety Guards                            */
#*********************************************************************/

check_dependencies() {
    log_info "Verifying dependencies..."

    # Check for CMake
    if ! command -v cmake &> /dev/null; then
        log_warn "CMake is not installed."
        
        # Detect OS and suggest install
        if [[ "$OSTYPE" == "darwin"* ]]; then
            log_info "Detected macOS. Attempting to install via Homebrew..."
            if command -v brew &> /dev/null; then
                brew install cmake
            else
                log_error "Homebrew not found. Please install Homebrew or install CMake manually."
                exit 1
            fi
        elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
            log_info "Detected Linux. Please run: 'sudo apt-get install cmake' (Debian/Ubuntu) or 'sudo dnf install cmake' (Fedora)."
            exit 1
        elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
            log_info "Detected Windows. Please download the installer from: https://cmake.org/download/"
            exit 1
        fi
    fi
    log_success "Dependencies verified."
}

check_dependencies

protect_self() {
    # Check if the script is running inside the 'scripts' directory
    if [[ "$PWD" == *"/scripts" ]]; then
        log_error "Safety Trigger: You are running this script inside the 'scripts' folder."
        log_error "Please run this script from the project root using: ./scripts/bootstrap-photonlab.sh"
        exit 1
    fi
}
protect_self


# Only create and enter the directory if we aren't already there
if [[ "$PWD" != *"/${PROJECT_NAME}" ]]; then
    create_dir "$PROJECT_NAME"
    cd "$PROJECT_NAME"
fi

#*********************************************************************/
# Phase 1 - Repository Initialization                                */
#*********************************************************************/

log_info "Initializing repository root..."
create_file "README.md"
create_file "LICENSE"
create_file ".gitignore"
create_file ".editorconfig"
create_file ".clang-format"
create_file "${PROJECT_NAME}.sln"

write_file "README.md" "# PhotonLab

High-Performance 16-Bit TIFF Review and Processing Workstation

Components:
- PhotonLab.Native
- PhotonLab.Desktop
- PhotonLab.Core
- PhotonLab.UnitTests
- PhotonLab.NativeTests"

write_file ".editorconfig" "root = true

[*]
charset = utf-8
indent_style = space
indent_size = 4
end_of_line = lf
insert_final_newline = true"

write_file ".clang-format" "BasedOnStyle: Google
IndentWidth: 4
ColumnLimit: 120"

#*********************************************************************/
# Phase 2 - Documentation Infrastructure                              */
#*********************************************************************/

log_info "Provisioning documentation hierarchy..."

# 1. Create the directories
for dir in Architecture ADR Performance Testing Decisions Images; do
    create_dir "Docs/$dir"
done

# 2. Map files to directories explicitly
for dir in Architecture ADR Performance Testing Decisions; do
    case "$dir" in
        Architecture) files="Overview HighLevelDesign ProcessingPipeline NativeInterop DataFlow RepositoryStructure" ;;
        ADR)          files="ADR-001-MVVM ADR-002-NativeCppEngine ADR-003-16BitProcessing ADR-004-StrategyPattern" ;;
        Performance)  files="LargeImageHandling MemoryManagement SIMDStrategy GPUAccelerationRoadmap" ;;
        Testing)      files="UnitTestingStrategy NativeTestingStrategy IntegrationTesting" ;;
        Decisions)    files="Tradeoffs TechnologySelection" ;;
    esac

    for file in $files; do
        create_file "Docs/$dir/${file}.md"
    done
done

# Assets
for d in Icons Logos Screenshots Demo; do
  create_dir "Assets/$d"
done

#*********************************************************************/
# Phase 3 - Native Processing Engine                                 */
#*********************************************************************/

log_info "Provisioning Native Engine..."

create_dir "$DIR_NATIVE/Include"
create_dir "$DIR_NATIVE/Include/Core"
create_dir "$DIR_NATIVE/Include/Processing"
create_dir "$DIR_NATIVE/Include/Statistics"
create_dir "$DIR_NATIVE/Include/Interop"

create_dir "$DIR_NATIVE/Src"
create_dir "$DIR_NATIVE/Src/Core"
create_dir "$DIR_NATIVE/Src/Processing"
create_dir "$DIR_NATIVE/Src/Statistics"
create_dir "$DIR_NATIVE/Src/Interop"

write_file "$DIR_NATIVE/CMakeLists.txt" "cmake_minimum_required(VERSION 3.25)
project(${PROJECT_NAME}.Native)
set(CMAKE_CXX_STANDARD 20)
add_library(${PROJECT_NAME}.Native SHARED)"

create_file "$DIR_NATIVE/Src/Core/ImageBuffer.cpp"
create_file "$DIR_NATIVE/Src/Core/ImageMetadata.cpp"
create_file "$DIR_NATIVE/Src/Core/PixelUtilities.cpp"
create_file "$DIR_NATIVE/Src/Processing/WindowLevelStrategy.cpp"
create_file "$DIR_NATIVE/Src/Processing/GammaStrategy.cpp"
create_file "$DIR_NATIVE/Src/Processing/MedianFilterStrategy.cpp"
create_file "$DIR_NATIVE/Src/Processing/SharpenStrategy.cpp"
create_file "$DIR_NATIVE/Src/Processing/ProcessingPipeline.cpp"
create_file "$DIR_NATIVE/Src/Statistics/HistogramCalculator.cpp"
create_file "$DIR_NATIVE/Src/Statistics/StatisticsCalculator.cpp"
create_file "$DIR_NATIVE/Src/Interop/NativeExports.cpp"

#*********************************************************************/
# Phase 4 - Desktop Application                                      */
#*********************************************************************/

log_info "Provisioning PhotonLab.Desktop..."

for d in \
    Views \
    Views/Panels \
    Views/Dialogs \
    Views/ToolWindows \
    Controls \
    ViewModels \
    Models \
    Services \
    Commands \
    Converters \
    Behaviors \
    Resources \
    Themes \
    Interop \
    Infrastructure \
    Infrastructure/Navigation \
    Infrastructure/Messaging \
    Infrastructure/Logging
do
    create_dir "$DIR_DESKTOP/$d"
done


# Themes controls 
create_file "$DIR_DESKTOP/Themes/Colors.xaml"
create_file "$DIR_DESKTOP/Themes/Typography.xaml"
create_file "$DIR_DESKTOP/Themes/Controls.xaml"
create_file "$DIR_DESKTOP/Themes/DarkTheme.xaml"

# Custom controls 
create_file "$DIR_DESKTOP/Controls/ImageViewerControl.xaml"
create_file "$DIR_DESKTOP/Controls/ImageViewerControl.xaml.cs"

create_file "$DIR_DESKTOP/Controls/HistogramControl.xaml"
create_file "$DIR_DESKTOP/Controls/HistogramControl.xaml.cs"

create_file "$DIR_DESKTOP/Controls/RoiOverlayControl.xaml"
create_file "$DIR_DESKTOP/Controls/RoiOverlayControl.xaml.cs"

create_file "$DIR_DESKTOP/Controls/PixelInspectorControl.xaml"
create_file "$DIR_DESKTOP/Controls/PixelInspectorControl.xaml.cs"

create_file "$DIR_DESKTOP/Controls/ZoomCanvasControl.xaml"
create_file "$DIR_DESKTOP/Controls/ZoomCanvasControl.xaml.cs"

# Add panels 

create_file "$DIR_DESKTOP/Views/Panels/MetadataPanel.xaml"
create_file "$DIR_DESKTOP/Views/Panels/MetadataPanel.xaml.cs"

create_file "$DIR_DESKTOP/Views/Panels/StatisticsPanel.xaml"
create_file "$DIR_DESKTOP/Views/Panels/StatisticsPanel.xaml.cs"

create_file "$DIR_DESKTOP/Views/Panels/HistogramPanel.xaml"
create_file "$DIR_DESKTOP/Views/Panels/HistogramPanel.xaml.cs"

create_file "$DIR_DESKTOP/Views/Panels/ProcessingPanel.xaml"
create_file "$DIR_DESKTOP/Views/Panels/ProcessingPanel.xaml.cs"

create_file "$DIR_DESKTOP/Views/Panels/ExportPanel.xaml"
create_file "$DIR_DESKTOP/Views/Panels/ExportPanel.xaml.cs"

# Add dialoques 

create_file "$DIR_DESKTOP/Views/Dialogs/SettingsDialog.xaml"
create_file "$DIR_DESKTOP/Views/Dialogs/SettingsDialog.xaml.cs"

create_file "$DIR_DESKTOP/Views/Dialogs/AboutDialog.xaml"
create_file "$DIR_DESKTOP/Views/Dialogs/AboutDialog.xaml.cs"

create_file "$DIR_DESKTOP/Views/Dialogs/ExportDialog.xaml"
create_file "$DIR_DESKTOP/Views/Dialogs/ExportDialog.xaml.cs"


# Provision Interop files
create_file "$DIR_DESKTOP/Interop/NativeMethods.cs"
create_file "$DIR_DESKTOP/Interop/NativeImageProcessor.cs"
create_file "$DIR_DESKTOP/Interop/NativeStructures.cs"

# Provision Services files
create_file "$DIR_DESKTOP/Services/TiffLoaderService.cs"
create_file "$DIR_DESKTOP/Services/HistogramService.cs"
create_file "$DIR_DESKTOP/Services/StatisticsService.cs"
create_file "$DIR_DESKTOP/Services/ImageProcessingService.cs"
create_file "$DIR_DESKTOP/Services/ImageRenderingService.cs"
create_file "$DIR_DESKTOP/Services/ImageSessionService.cs"
create_file "$DIR_DESKTOP/Services/ExportService.cs"

# Provision Models files
create_file "$DIR_DESKTOP/Models/LoadedImage.cs"
create_file "$DIR_DESKTOP/Models/HistogramResult.cs"
create_file "$DIR_DESKTOP/Models/RoiSelection.cs"

# Boilerplate Application Files
create_file "$DIR_DESKTOP/App.xaml"
create_file "$DIR_DESKTOP/App.xaml.cs"
create_file "$DIR_DESKTOP/Views/MainWindow.xaml"
create_file "$DIR_DESKTOP/Views/MainWindow.xaml.cs"
create_file "$DIR_DESKTOP/ViewModels/MainViewModel.cs"

# Commands Files
create_file "$DIR_DESKTOP/Commands/RelayCommand.cs"

#*********************************************************************/
# Phase 5 - Shared Core Layer                                        */
#*********************************************************************/

log_info "Provisioning PhotonLab.Core..."
for d in Contracts Domain DTOs Exceptions Constants; do
  create_dir "$DIR_CORE/$d"
done

create_file "$DIR_CORE/Contracts/IImageProcessor.cs"
create_file "$DIR_CORE/Contracts/IHistogramService.cs"
create_file "$DIR_CORE/Contracts/IStatisticsService.cs"
create_file "$DIR_CORE/Contracts/ITiffLoader.cs"
create_file "$DIR_CORE/Contracts/IExportService.cs"
create_file "$DIR_CORE/Contracts/IImageSessionService.cs"
create_file "$DIR_CORE/Contracts/IImageRenderer.cs"
create_file "$DIR_CORE/Contracts/IImageProcessingService.cs"

#*********************************************************************/
# Phase 6 - Testing Infrastructure                                   */
#*********************************************************************/

log_info "Provisioning testing assets..."

# Unit Tests

create_dir "$DIR_TESTS/Core"
create_dir "$DIR_TESTS/Processing"
create_dir "$DIR_TESTS/Statistics"

create_file "$DIR_TESTS/Core/ImageBufferTests.cpp"

create_file "$DIR_TESTS/Processing/WindowLevelTests.cpp"
create_file "$DIR_TESTS/Processing/GammaStrategyTests.cpp"
create_file "$DIR_TESTS/Processing/ProcessingPipelineTests.cpp"

create_file "$DIR_TESTS/Statistics/HistogramCalculatorTests.cpp"
create_file "$DIR_TESTS/Statistics/StatisticsCalculatorTests.cpp"

# Native DLL Tests

create_dir "$DIR_NATIVE_TESTS"

create_file "$DIR_NATIVE_TESTS/NativeExportsTests.cpp"
create_file "$DIR_NATIVE_TESTS/DllLoadTests.cpp"
create_file "$DIR_NATIVE_TESTS/InteropTests.cpp"

# Integration Tests
INT_TEST_DIR="Tests/${PROJECT_NAME}.DesktopIntegrationTests"
create_dir "$INT_TEST_DIR"

# Integration test files
INT_TEST_FILES=(
    "NativeImageProcessorIntegrationTests.cs"
    "HistogramServiceIntegrationTests.cs"
    "StatisticsServiceIntegrationTests.cs"
    "ImageSessionServiceIntegrationTests.cs"
    "ImageProcessingServiceIntegrationTests.cs"
    "IntegrationCollection.cs"
)

# Generate files
for file in "${INT_TEST_FILES[@]}"; do
    create_file "$INT_TEST_DIR/$file"
done

# Test Data

create_dir "Tests/TestData"

create_file "Tests/TestData/Sample16Bit.tiff"
create_file "Tests/TestData/BrainScan.tiff"
create_file "Tests/TestData/SmallImage.raw"

#*********************************************************************/
# Phase 7 - Build & Deployment Configuration                         */
#*********************************************************************/

log_info "Provisioning build pipelines..."

create_dir "Build/CMake"
create_dir "Build/NuGet"
create_dir "Build/Packaging"
create_dir "Build/Release"

# scripts
create_dir scripts
for f in bootstrap-photonlab.sh Dockerfile gh-automate.sh mingw-w64.cmake orchestrate-native-docker.sh orchestrate-native.sh orchestrate-service.sh; do
    create_file "scripts/$f"
    if [ "$DRY_RUN" = true ]; then
        echo -e "${YELLOW}[DRY-RUN]${NC} chmod +x scripts/$f"
    else
        chmod +x "scripts/$f"
    fi
done

#*********************************************************************/
# Phase 8 - CI/CD & Samples                                          */
#*********************************************************************/

log_info "Finalizing repository structure..."
create_dir Samples/TIFF
create_dir Samples/Expected

# GitHub
#create_dir .github/workflows
#create_file .github/workflows/build.yml
#create_file .github/workflows/tests.yml
#create_file .github/workflows/release.yml
#create_file .github/workflows/codeql.yml

echo
echo "===================================================="
echo "PHOTONLAB BOOTSTRAP COMPLETE"
echo "===================================================="
log_success "Repository generated successfully."
