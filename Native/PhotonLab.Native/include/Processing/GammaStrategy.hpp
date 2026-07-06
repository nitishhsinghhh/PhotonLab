// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : GammaStrategy.hpp                                   */
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

#ifndef GAMMASTRATEGY_HPP
#define GAMMASTRATEGY_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <cstdint>

#include "IImageProcessingStrategy.hpp"

namespace PhotonLab {
/**
 * @class GammaStrategy
 * @brief Concrete strategy for applying Gamma correction.
 * * This filter applies a power-law transformation to pixel
 * intensities, which is essential for adjusting image brightness
 * and contrast in 16-bit medical imaging pipelines.
 */
class GammaStrategy : public IImageProcessingStrategy {
   public:
    /**
     * @brief Constructs the strategy with a specified gamma value.
     * @param gamma The power-law exponent (e.g., 2.2 for sRGB).
     */
    explicit GammaStrategy(double gamma);

    /**
     * @brief Applies the gamma transformation to the pixel buffer.
     * @param pixels Pointer to the raw 16-bit pixel data.
     * @param width Image width in pixels.
     * @param height Image height in pixels.
     */
    void Process(uint16_t* pixels, int width, int height) override;

   private:
    double m_gamma;
};
}  // namespace PhotonLab

#endif  // GAMMASTRATEGY_HPP
