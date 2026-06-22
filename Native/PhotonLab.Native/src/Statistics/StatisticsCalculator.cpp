// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : StatisticsCalculator.cpp                            */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module           : Core/Statistics                                */
/* Component        : Statistical Calculation Engine                 */
/* Thread Safe      : Yes                                            */
/* Complexity       : O(n)                                           */
/* API Status       : Stable                                         */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Implements statistical analysis for 16-bit image    */
/* data, calculating intensity range, mean, and deviation metrics.   */
/*                                                                   */
/* Notes       : Optimized for high-throughput image analysis.       */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include "Statistics/StatisticsCalculator.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace PhotonLab {
StatisticsResult StatisticsCalculator::Calculate(const uint16_t* pixels, size_t count) {
    if (count == 0) return {};

    StatisticsResult result{};
    result.Reset();

    if (pixels == nullptr || count == 0) {
        return result;
    }

    result.Min = pixels[0];
    result.Max = pixels[0];

    double sum = 0.0;

    /*****************************************************************/
    /* Pass 1: Calculate Min, Max, and Mean Sum                      */
    /*****************************************************************/

    for (size_t i = 0; i < count; ++i) {
        result.Min = std::min(result.Min, pixels[i]);
        result.Max = std::max(result.Max, pixels[i]);
        sum += pixels[i];
    }

    result.Mean = sum / static_cast<double>(count);

    /*****************************************************************/
    /* Pass 2: Calculate Standard Deviation                          */
    /*****************************************************************/

    double varianceSum = 0.0;
    for (size_t i = 0; i < count; ++i) {
        const double difference = static_cast<double>(pixels[i]) - result.Mean;

        varianceSum += difference * difference;
    }

    result.StandardDeviation = std::sqrt(varianceSum / static_cast<double>(count));


     /*****************************************************************/
     /* Pass 3: Median                                                */
     /*****************************************************************/

    std::vector<uint16_t> sorted(pixels, pixels + count);

    std::sort(sorted.begin(), sorted.end());

    if (count % 2 == 0) {
        result.Median = (sorted[count / 2 - 1] + sorted[count / 2]) / 2.0;
    } else {
        result.Median = sorted[count / 2];
    }

    return result;
}
}  // namespace PhotonLab
