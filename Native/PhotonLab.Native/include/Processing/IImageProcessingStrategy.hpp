// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : IImageProcessingStrategy.hpp                        */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module           : Core/Processing                                */
/* Component        : Strategy Pattern Interface                     */
/* Thread Safe      : Yes                                            */
/* Complexity       : O(1) interface definition                      */
/* API Status       : Stable                                         */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Defines the strategy interface for image processing */
/* algorithms, enabling interchangeable pixel manipulation logic.    */
/*                                                                   */
/* Notes       : Implements the Strategy Pattern for decoupling      */
/* processing logic from the core image buffer management.           */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#ifndef IIMAGEPROCESSINGSTRATEGY_HPP
#define IIMAGEPROCESSINGSTRATEGY_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <cstdint>

namespace PhotonLab {
/**
 * @class IImageProcessingStrategy
 * @brief Abstract base interface for image processing operations.
 * * Provides a standardized mechanism to inject diverse algorithms
 * (e.g., Gamma, Sharpen, Median Filter) into the processing pipeline.
 */
class IImageProcessingStrategy {
public:
  /** @brief Virtual destructor to ensure proper cleanup of derived types. */
  virtual ~IImageProcessingStrategy() = default;

  /**
   * @brief Performs pixel-wise processing on the provided buffer.
   * @param pixels Pointer to the raw 16-bit pixel data array.
   * @param width Image width in pixels.
   * @param height Image height in pixels.
   */
  virtual void Process(uint16_t *pixels, int width, int height) = 0;
};
} // namespace PhotonLab

#endif // IIMAGEPROCESSINGSTRATEGY_HPP
