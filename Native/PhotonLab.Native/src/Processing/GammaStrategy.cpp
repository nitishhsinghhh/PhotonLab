// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : GammaStrategy.cpp                                   */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module           : Core/Processing                                */
/* Component        : Gamma Correction Filter                        */
/* Thread Safe      : Yes                                            */
/* Complexity       : O(n)                                           */
/* API Status       : Stable                                         */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Implements Gamma correction for non-linear          */
/* luminance adjustment of 16-bit intensity data.                    */
/*                                                                   */
/* Notes       : Implements the IImageProcessingStrategy interface.  */
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

#include "Processing/GammaStrategy.hpp"

#include <cmath>
#include <cstdint>

namespace PhotonLab {
GammaStrategy::GammaStrategy(double gamma) : m_gamma(gamma) {}

void GammaStrategy::Process(uint16_t* pixels, int width, int height) {
    const size_t count = static_cast<size_t>(width) * height;

    // Apply Gamma transformation: V_out = V_in ^ gamma
    for (size_t i = 0; i < count; ++i) {
        // Normalize to [0.0, 1.0] range
        double normalized = static_cast<double>(pixels[i]) / 65535.0;

        // Apply non-linear power law adjustment
        normalized = std::pow(normalized, m_gamma);

        // Rescale back to 16-bit range [0, 65535]
        pixels[i] = static_cast<uint16_t>(normalized * 65535.0);
    }
}
}  // namespace PhotonLab
