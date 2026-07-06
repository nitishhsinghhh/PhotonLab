// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : StatisticsCalculator.hpp                            */
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
/* Description : Provides high-performance statistical analysis for  */
/* 16-bit image data, calculating intensity range and distribution.  */
/*                                                                   */
/* Notes       : Designed for integration into processing pipelines. */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#ifndef STATISTICSCALCULATOR_HPP
#define STATISTICSCALCULATOR_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <cstddef>
#include <cstdint>

#include "StatisticsResult.hpp"

namespace PhotonLab {
/**
 * @class StatisticsCalculator
 * @brief Engine for high-performance statistical analysis of image buffers.
 */
class StatisticsCalculator {
public:
  /**
   * @brief Calculates Min, Max, Mean, and Standard Deviation.
   * @param pixels Pointer to the 16-bit image pixel buffer.
   * @param count Total number of pixels in the buffer.
   * @return StatisticsResult containing the calculated metrics.
   */
  static StatisticsResult Calculate(const uint16_t *pixels, size_t count);
};
} // namespace PhotonLab

#endif // STATISTICSCALCULATOR_HPP
