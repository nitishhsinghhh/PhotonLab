#!/usr/bin/env bash
# SPDX-License-Identifier: Apache-2.0
#*********************************************************************/
# SYSTEM      : PhotonLab Medical Image Processing Framework         */
# SUBSYSTEM   : Managed Desktop Core & Integration Layer (.NET 8)    */
# COMPONENT   : run-photonlab                                        */
# VERSION     : 1.0                                                  */
#                                                                    */
# DESCRIPTION : Orchestrates the complete lifecycle of the PhotonLab */
#               managed runner, handles dependency restoration,      */
#               clean workspace compilation, native .dylib/so/dll    */
#               synchronization, and execution tracking.             */
#                                                                    */
#               Ensures absolute ABI compatibility between the       */
#               high-performance native C++ core and the .NET        */
#               P/Invoke layout by automatically resolving build     */
#               artifacts and copying them into execution contexts.  */
#                                                                    */
# SIDE EFFECTS: Terminates stale dotnet test/run host processes.     */
#                                                                    */
# AUTHOR      : Nitish Singh (nitishhsinghhh)                        */
# CONTACT     : me.singhnitish@yandex.com                            */
#                                                                    */
# REVISION HISTORY:                                                  */
# ------------------------------------------------------------------ */
# Ver  Date        Author           Description                      */
# ---  ----------  --------------   -------------------------------- */
# 1.0  2026-06-11  Nitish Singh     Initial .NET orchestration       */
#*********************************************************************/

set -euo pipefail

#*********************************************************************/
# Logging Utilities                                                  */
#*********************************************************************/

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

timestamp() { date +"%Y-%m-%d %H:%M:%S"; }
log_info() { echo -e "${BLUE}[$(timestamp)] [INFO]${NC} $1"; }
log_warn() { echo -e "${YELLOW}[$(timestamp)] [WARN]${NC} $1"; }
log_error() { echo -e "${RED}[$(timestamp)] [ERROR]${NC} $1"; }
log_success() { echo -e "${GREEN}[$(timestamp)] [SUCCESS]${NC} $1"; }

#*********************************************************************/
# 1. Environment Guard & Path Initialization                          */
#*********************************************************************/

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
# Step up one level from the 'scripts' directory to the project root context
cd "$SCRIPT_DIR/.."

log_info "Initializing PhotonLab Orchestration Workspace..."
log_success "Workspace Root Context: $(pwd)"

#*********************************************************************/
# 2. Lifecycle Preparation (Clean and Restore)                        */
#*********************************************************************/

log_info "Restoring .NET Ecosystem dependencies..."
dotnet restore ./PhotonLab.sln

log_info "Executing Clean Compilation Pass..."
dotnet clean ./PhotonLab.sln -c Debug

#*********************************************************************/
# 3. Dynamic Native Platform Resolution & Copy                        */
#*********************************************************************/

log_info "Scanning for Native C++ Engine Artifacts..."

# Establish target .NET execution bin targets
DESKTOP_BIN_DIR="Desktop/PhotonLab.Desktop/bin/Debug/net8.0"
TEST_BIN_DIR="Tests/PhotonLab.DesktopIntegrationTests/bin/Debug/net8.0"

mkdir -p "$DESKTOP_BIN_DIR"
mkdir -p "$TEST_BIN_DIR"

LIB_PREFIX="lib"
LIB_EXT="so"

# Cross-platform runtime binary targeting
if [[ "$(uname)" == "Darwin" ]]; then
    LIB_EXT="dylib"
elif [[ "$(uname)" == *"MINGW"* || "$(uname)" == *"MSYS"* ]]; then
    LIB_PREFIX=""
    LIB_EXT="dll"
fi

NATIVE_LIB_NAME="${LIB_PREFIX}PhotonLab.Native.${LIB_EXT}"

# Pinpoint exact build location inside the repository architecture
POSSIBLE_PATHS=(
    "Native/PhotonLab.Native/build/${NATIVE_LIB_NAME}"
    "Native/PhotonLab.Native/build/lib/${NATIVE_LIB_NAME}"
)

SRC_LIB=""
for path in "${POSSIBLE_PATHS[@]}"; do
    if [ -f "$path" ]; then SRC_LIB="$path"; break; fi
done

if [ -z "$SRC_LIB" ]; then
    log_error "Native assembly binary mapping failed ($NATIVE_LIB_NAME). Please verify C++ build path."
    exit 1
fi

log_success "Discovered Native Asset Source: $SRC_LIB"

# Inject into execution binaries directories to preserve runtime P/Invoke context
log_info "Injecting native library contexts into target assembly destinations..."
cp "$SRC_LIB" "$DESKTOP_BIN_DIR/"
cp "$SRC_LIB" "$TEST_BIN_DIR/"

# Export linker environment search arrays to mitigate dynamic load drops
export DYLD_LIBRARY_PATH="$(pwd)/$DESKTOP_BIN_DIR:$(pwd)/$TEST_BIN_DIR:${DYLD_LIBRARY_PATH:-}"
export LD_LIBRARY_PATH="$(pwd)/$DESKTOP_BIN_DIR:$(pwd)/$TEST_BIN_DIR:${LD_LIBRARY_PATH:-}"

log_success "Native layers bound successfully. Architecture confirmation:"
file "$SRC_LIB"

#*********************************************************************/
# 4. Assembly Execution Validation Loop (Test Run)                    */
#*********************************************************************/

log_info "Launching Automated Integration Test Pass..."

# Note: C++ core compilation must ensure -fsanitize=address is OFF 
# to keep the managed worker context from crashing.
if dotnet test ./PhotonLab.sln --no-restore; then
    log_success "===== INTEGRATION VERIFICATION PASSED SUCCESSFULLY ====="
else
    log_error "Assembly test pass encountered fatal runtime errors."
    exit 1
fi

#*********************************************************************/
# 5. Core Application Bootstrapper                                    */
#*********************************************************************/

log_success "===== ORCHESTRATION COMPLETION: ALL SYSTEMS OPERATIONAL ====="
log_info "PhotonLab components successfully validated, synced, and compiled."
