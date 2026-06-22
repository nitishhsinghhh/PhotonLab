// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : NativeExports.cpp                                   */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016–2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module           : Interop                                        */
/* Component        : Native Export API (P/Invoke Boundary)          */
/* Thread Safe      : Yes (stateless functions only)                 */
/* Complexity       : O(n) or O(k) depending on operation            */
/* API Status       : Stable                                         */
/* Exception Safety : No-throw guarantee (exceptions trapped)        */
/*                                                                   */
/* Description : Provides C-style exported functions for             */
/* interoperability between managed C# layer and native C++ engine.  */
/* These functions act as a thin ABI boundary over internal          */
/* Strategy-based image processing implementations.                  */
/*                                                                   */
/* Notes:                                                            */
/* - This layer does NOT implement processing logic.                 */
/* - All heavy computation is delegated to Strategy classes.         */
/* - Exceptions are caught and converted into error codes.           */
/* - Designed for P/Invoke compatibility.                            */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial interop layer        */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "Interop/NativeExports.hpp"

#include <algorithm>
#include <array>

extern "C" {

PHOTONLAB_EXPORT int ApplyWindowLevel(uint16_t* pixels, int width, int height, int window, int level) {
    try {
        if (pixels == nullptr) {
            return -1;
        }

        PhotonLab::WindowLevelStrategy strategy(window, level);

        strategy.Process(pixels, width, height);

        return 0;
    } catch (...) {
        return -1;
    }
}

PHOTONLAB_EXPORT int ApplyGamma(uint16_t* pixels, int width, int height, double gamma) {
    try {
        if (pixels == nullptr) {
            return -1;
        }

        PhotonLab::GammaStrategy strategy(gamma);

        strategy.Process(pixels, width, height);

        return 0;
    } catch (...) {
        return -1;
    }
}

PHOTONLAB_EXPORT int ApplyMedian(uint16_t* pixels, int width, int height) {
    try {
        if (pixels == nullptr) return -1;

        PhotonLab::MedianFilterStrategy strategy;

        strategy.Process(pixels, width, height);

        return 0;
    } catch (...) {
        return -1;
    }
}

PHOTONLAB_EXPORT int ApplySharpen(uint16_t* pixels, int width, int height) {
    try {
        if (pixels == nullptr) return -1;

        PhotonLab::SharpenStrategy strategy;

        strategy.Process(pixels, width, height);

        return 0;
    } catch (...) {
        return -1;
    }
}

PHOTONLAB_EXPORT int CalculateHistogram(const uint16_t* pixels, size_t count, uint32_t* histogramBuffer,
                                        size_t histogramSize) {
    try {
        if (pixels == nullptr || histogramBuffer == nullptr || histogramSize != 65536U) {
            return -1;
        }

        PhotonLab::HistogramCalculator calculator;

        auto histogram = calculator.Calculate(pixels, count);

        std::copy(histogram.begin(), histogram.end(), histogramBuffer);

        return 0;
    } catch (...) {
        return -1;
    }
}

PHOTONLAB_EXPORT int CalculateStatistics(const uint16_t* pixels, size_t count, PhotonLab::StatisticsResult* result) {
    try {
        if (pixels == nullptr || result == nullptr) {
            return -1;
        }

        PhotonLab::StatisticsCalculator calculator;

        auto statistics = calculator.Calculate(pixels, count);

        *result = statistics;

        return 0;
    } catch (...) {
        return -1;
    }
}

}  // extern "C"
