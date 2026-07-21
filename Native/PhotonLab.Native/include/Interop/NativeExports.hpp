// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : NativeExports.hpp                                   */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module           : Interop                                        */
/* Component        : Native Export Interface                        */
/* Thread Safe      : Yes                                            */
/* Complexity       : O(1) (API Dispatch)                            */
/* API Status       : Stable                                         */
/* Exception Safety : Strong (Exceptions do not cross C ABI)         */
/*                                                                   */
/* Description : Defines the C ABI exported functions consumed by    */
/* .NET through P/Invoke. These APIs expose the native image         */
/* processing engine, histogram calculation, and statistics          */
/* computation in a platform-independent manner.                     */
/*                                                                   */
/* Notes       : Maintains a stable binary interface for managed     */
/* interop. All exported functions use C linkage to prevent name     */
/* mangling and simplify cross-platform dynamic loading.             */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#ifndef NATIVEEXPORTS_HPP
#define NATIVEEXPORTS_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <cstddef>
#include <cstdint>

#include "Processing/GammaStrategy.hpp"
#include "Processing/MedianFilterStrategy.hpp"
#include "Processing/SharpenStrategy.hpp"
#include "Processing/WindowLevelStrategy.hpp"
#include "Statistics/HistogramCalculator.hpp"
#include "Statistics/StatisticsCalculator.hpp"
#include "Statistics/StatisticsResult.hpp"

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#if defined(PHOTONLAB_NATIVE_EXPORTS) || defined(PhotonLab_NativeDLL_EXPORTS)
#define PHOTONLAB_EXPORT __declspec(dllexport)
#else
#define PHOTONLAB_EXPORT __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define PHOTONLAB_EXPORT __attribute__((visibility("default")))
#else
#define PHOTONLAB_EXPORT
#endif
#endif

extern "C" {
/*****************************************************************/
/* Window / Level                                                */
/*****************************************************************/

PHOTONLAB_EXPORT
int ApplyWindowLevel(uint16_t* pixels, int width, int height, int window, int level);

/*****************************************************************/
/* Gamma Correction                                              */
/*****************************************************************/

PHOTONLAB_EXPORT
int ApplyGamma(uint16_t* pixels, int width, int height, double gamma);

/*****************************************************************/
/* Median Correction                                             */
/*****************************************************************/

PHOTONLAB_EXPORT
int ApplyMedian(uint16_t* pixels, int width, int height);

/*****************************************************************/
/* Sharpen Correction                                            */
/*****************************************************************/

PHOTONLAB_EXPORT
int ApplySharpen(uint16_t* pixels, int width, int height);

/*****************************************************************/
/* Histogram                                                      */
/*****************************************************************/

PHOTONLAB_EXPORT
int CalculateHistogram(const uint16_t* pixels, size_t count, uint32_t* histogramBuffer, size_t histogramSize);

/*****************************************************************/
/* Statistics                                                     */
/*****************************************************************/

PHOTONLAB_EXPORT
int CalculateStatistics(const uint16_t* pixels, size_t count, PhotonLab::StatisticsResult* result);
}

#endif
