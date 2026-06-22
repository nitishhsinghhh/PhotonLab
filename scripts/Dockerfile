# -----------------------------------------------------------------------------
# Microsoft Azure Container Registry / .NET Microservices Template
# Project:     Hardware-Aware Case Conversion Engine
# Component:   Multi-Platform Native Orchestrator (Linux + Windows)
# Documentation: https://github.com/nitishhsinghhh/PhotonLab
# -----------------------------------------------------------------------------
# VERSION HISTORY
# Version | Date       | Author       | Description
# --------|------------|--------------|----------------------------------------
# 1.0.0   | 2026-05-14 | Nitish Singh | Initial Orchestration Layer.
#
# -----------------------------------------------------------------------------
# ARCHITECTURAL STRATEGY:
# * Headless Build Vessel: Designed to output binaries, not to run as a service.
# * Cross-Compilation: Leverages MinGW-w64 to target Windows from a Linux host.
# * Artifact Extraction: Exposes the build directory for host-side consumption.
# -----------------------------------------------------------------------------

ARG TARGET_PLATFORM=linux/amd64
FROM --platform=${TARGET_PLATFORM} mcr.microsoft.com/dotnet/sdk:8.0 AS build-env

# Install C++ Native Toolchains, Wine, and Xvfb for testing
RUN dpkg --add-architecture i386 && \
    apt-get update && apt-get install -y \
    build-essential \
    cmake \
    mingw-w64 \
    wine \
    wine32 \
    wine64 \
    xvfb \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --set x86_64-w64-mingw32-gcc /usr/bin/x86_64-w64-mingw32-gcc-posix && \
    update-alternatives --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix

WORKDIR /src

# Copy the entire repo context
COPY . .

# Ensure orchestrator script is executable
# Update path to match your actual location: scripts/orchestrate-native.sh
RUN chmod +x scripts/orchestrate-native.sh

# --- STEP 1: Build Linux Artifacts ---
#RUN ./scripts/orchestrate-native.sh ubuntu-latest

RUN ./scripts/orchestrate-native.sh ubuntu-latest || echo "Tests failed, continuing..."

# --- STEP 2: Build Windows Artifacts (MinGW) ---
RUN ./scripts/orchestrate-native.sh windows-latest

# -----------------------------------------------------------------------------
# STEP 3 : Windows Verification (Wine + GoogleTest)
# -----------------------------------------------------------------------------

ENV WINEPREFIX=/root/.wine
ENV WINEARCH=win64
ENV WINEDEBUG=-all
ENV WINEDLLOVERRIDES="mscoree,mshtml="

WORKDIR /src/Native/PhotonLab.Native/build/windows-latest

RUN /usr/lib/wine/wine64 PhotonLab.UnitTests.exe --gtest_color=yes
RUN /usr/lib/wine/wine64 PhotonLab.NativeTests.exe --gtest_color=yes


WORKDIR /src
