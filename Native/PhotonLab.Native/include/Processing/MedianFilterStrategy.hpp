// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : MedianFilterStrategy.hpp                            */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module          : Core/Processing                                 */
/* Component       : Median Filter Strategy                          */
/* Thread Safe     : Yes                                             */
/* Complexity      : O(W * H * K^2 log K^2)                          */
/* API Status      : Stable                                          */
/* Exception Safety: Basic Guarantee                                 */
/*                                                                   */
/* Description : Implements spatial noise reduction using a 3x3      */
/* median filter for 16-bit intensity data.                          */
/*                                                                   */
/* Notes       : Implements the IImageProcessingStrategy interface.  */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#ifndef MEDIANFILTERSTRATEGY_HPP
#define MEDIANFILTERSTRATEGY_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <cstdint>

#include "IImageProcessingStrategy.hpp"

namespace PhotonLab {
/**
 * @class MedianFilterStrategy
 * @brief Concrete strategy for applying a 3x3 median filter.
 * * This filter is effective at removing impulsive (salt-and-pepper)
 * noise while preserving structural edges in medical images.
 */
class MedianFilterStrategy : public IImageProcessingStrategy {
   public:
    /**
     * @brief Constructs the strategy.
     */
    MedianFilterStrategy() = default;

    /**
     * @brief Applies the median filter transformation to the pixel buffer.
     * @param pixels Pointer to the raw 16-bit pixel data.
     * @param width Image width in pixels.
     * @param height Image height in pixels.
     */
    void Process(uint16_t* pixels, int width, int height) override;
};
}  // namespace PhotonLab

#endif  // MEDIANFILTERSTRATEGY_HPP
