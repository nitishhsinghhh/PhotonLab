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
/* Complexity       : O(N), where N is the number of pixels          */
/* API Status       : Stable                                         */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Implements the histogram generation algorithm for   */
/* 16-bit grayscale image buffers by computing the frequency of      */
/* each possible intensity value (0–65535).                          */
/*                                                                   */
/* Notes       : Returns a fixed-size histogram containing 65,536    */
/* bins. Invalid input (nullptr or zero pixel count) results in an   */
/* empty histogram with all bins initialized to zero.                */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
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
