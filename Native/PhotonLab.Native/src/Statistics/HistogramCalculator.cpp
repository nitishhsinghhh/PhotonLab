// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : HistogramCalculator.cpp                             */
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
/* Description : Generates intensity distribution histograms         */
/*               for 16-bit image data.                              */
/*                                                                   */
/*********************************************************************/

#include "Statistics/HistogramCalculator.hpp"

/*********************************************************************/
/* Namespace: PhotonLab                                              */
/*********************************************************************/
namespace PhotonLab {
std::array<uint32_t, 65536> HistogramCalculator::Calculate(const uint16_t* pixels, size_t count) {
    std::array<uint32_t, 65536> histogram{};

    if (pixels == nullptr || count == 0U) {
        return histogram;
    }

    for (size_t index = 0; index < count; ++index) {
        ++histogram[pixels[index]];
    }

    return histogram;
}
}  // namespace PhotonLab
