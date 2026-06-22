// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : HistogramCalculator.hpp                             */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module           : Core/Statistics                                */
/* Component        : Histogram Calculation Engine                   */
/* Thread Safe      : Yes                                            */
/* Complexity       : O(n)                                           */
/* API Status       : Stable                                         */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Provides functionality to generate intensity        */
/* distribution histograms for 16-bit image data.                    */
/*                                                                   */
/* Notes       : The histogram size is fixed at 65536 to support     */
/* all possible 16-bit intensity values.                             */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#ifndef HISTOGRAMCALCULATOR_HPP
#define HISTOGRAMCALCULATOR_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <array>
#include <cstddef>
#include <cstdint>

namespace PhotonLab {
/**
 * @class HistogramCalculator
 * @brief Computes frequency distribution of pixel intensities.
 * * This class processes a raw 16-bit buffer and returns a count
 * frequency array for every possible intensity level (0-65535).
 */
class HistogramCalculator {
   public:
    /**
     * @brief Calculates the intensity histogram of an image.
     * @param pixels Pointer to the 16-bit raw image data.
     * @param count Total number of pixels in the buffer.
     * @return std::array containing frequency counts for all 65536 levels.
     */
    static std::array<uint32_t, 65536> Calculate(const uint16_t* pixels, size_t count);
};
}  // namespace PhotonLab

#endif  // HISTOGRAMCALCULATOR_HPP
