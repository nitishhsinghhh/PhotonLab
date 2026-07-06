// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : SharpenStrategy.cpp                                 */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module          : Core/Processing                                 */
/* Component       : Sharpen Filter Strategy                         */
/* Thread Safe     : Yes                                             */
/* Complexity      : O(W * H)                                        */
/* API Status      : Stable                                          */
/* Exception Safety: Basic Guarantee                                 */
/*                                                                   */
/* Description : Implementation of a 5-tap Laplacian sharpening      */
/* kernel for edge enhancement in 16-bit image buffers.              */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#include "Processing/SharpenStrategy.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>

namespace PhotonLab {

/**
 * @brief Applies a sharpening filter using a 5-tap Laplacian kernel.
 * @param pixels Pointer to the 16-bit raw image data.
 * @param width The image width in pixels.
 * @param height The image height in pixels.
 */
void SharpenStrategy::Process(uint16_t *pixels, int width, int height) {
  if (pixels == nullptr || width <= 0 || height <= 0) {
    return;
  }

  // Create a local copy to preserve source data during kernel convolution
  std::vector<uint16_t> original(
      pixels, pixels + (static_cast<size_t>(width) * height));

  for (int y = 1; y < height - 1; ++y) {
    for (int x = 1; x < width - 1; ++x) {
      const int center = original[y * width + x];
      const int top = original[(y - 1) * width + x];
      const int bottom = original[(y + 1) * width + x];
      const int left = original[y * width + (x - 1)];
      const int right = original[y * width + (x + 1)];

      // Apply Laplacian sharpening kernel:
      // [ 0 -1  0 ]
      // [ -1 5 -1 ]
      // [ 0 -1  0 ]
      int value = (5 * center) - top - bottom - left - right;

      pixels[y * width + x] =
          static_cast<uint16_t>(std::clamp(value, 0, 65535));
    }
  }
}

} // namespace PhotonLab
