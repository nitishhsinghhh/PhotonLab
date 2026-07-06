// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : StatisticsResult.hpp                                */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module           : Core/Statistics                                */
/* Component        : Statistical Data Structure                     */
/* Thread Safe      : Yes                                            */
/* Complexity       : O(1)                                           */
/* API Status       : Stable                                         */
/* Exception Safety : N/A (Plain Old Data)                           */
/*                                                                   */
/* Description : Defines the result container for image statistical  */
/* analysis, holding intensity range and distribution metrics.       */
/*                                                                   */
/* Notes       : Used by statistics calculators to return computed   */
/* metrics from image processing pipelines.                          */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#ifndef STATISTICSRESULT_HPP
#define STATISTICSRESULT_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <cstdint>

namespace PhotonLab {
/**
 * @struct StatisticsResult
 * @brief Encapsulates statistical metrics derived from an image.
 * @brief Minimum intensity value found in the buffer.
 */
struct StatisticsResult {
    uint16_t Min{0};
    uint16_t Max{0};
    double Mean{0.0};
    double StandardDeviation{0.0};
    double Median{0.0};

    // Reset method to clear results
    void Reset() {
        Min = 0;
        Max = 0;
        Mean = 0.0;
        StandardDeviation = 0.0;
        Median = 0.0;
    }
};
}  // namespace PhotonLab

#endif  // STATISTICSRESULT_HPP
