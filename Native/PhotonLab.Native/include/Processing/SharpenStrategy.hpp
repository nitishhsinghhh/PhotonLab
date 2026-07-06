// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : SharpenStrategy.hpp                                 */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module           : Core/Processing                                */
/* Component        : Sharpening Strategy Definition                 */
/* Thread Safe      : Yes                                            */
/* Complexity       : O(N) linear spatial convolution                */
/* API Status       : Stable                                         */
/*                                                                   */
/* Description : Defines the concrete strategy for applying spatial  */
/* high-pass filtering to enhance edge-contrast bounds               */
/* across contiguous 16-bit diagnostic image buffers.                */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#ifndef SHARPENSTRATEGY_HPP
#define SHARPENSTRATEGY_HPP

/*********************************************************************/
/* Dependencies                                                       */
/*********************************************************************/

#include <cstdint>

#include "IImageProcessingStrategy.hpp"

namespace PhotonLab {

/**
 * @class SharpenStrategy
 * @brief Concrete strategy for applying high-pass spatial filtering.
 *
 * Enhances fine high-frequency structural textures by amplifying local
 * intensity contrasts through a discrete 3x3 Laplacian kernel matrix.
 */
class SharpenStrategy : public IImageProcessingStrategy {
public:
  /**
   * @brief Default constructor initializing base structural state.
   */
  SharpenStrategy() = default;

  /**
   * @brief Default virtual destructor enforcing clean polymorphic cleanup.
   */
  ~SharpenStrategy() override = default;

  /**
   * @brief Applies a spatial 3x3 sharpening convolution to the pixel buffer.
   * @param pixels Pointer to the contiguous, raw 16-bit intensity values.
   * @param width  Horizontal image boundary constraint in pixels.
   * @param height Vertical image boundary constraint in pixels.
   */
  void Process(uint16_t *pixels, int width, int height) override;
};

} // namespace PhotonLab

#endif // SHARPENSTRATEGY_HPP
