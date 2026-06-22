// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : MedianFilterStrategy.cpp                           */
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
/* Description : Implementation of a 3x3 median filter for spatial   */
/* noise reduction in 16-bit image buffers.                          */
/*                                                                   */
/* Notes       : The strategy uses a copy of the input buffer to     */
/* prevent read-after-write contamination during processing.         */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#include "Processing/MedianFilterStrategy.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

namespace PhotonLab {

/**
 * @brief Applies a 3x3 median filter to the provided 16-bit pixel buffer.
 * @param pixels Pointer to the 16-bit raw image data.
 * @param width The image width in pixels.
 * @param height The image height in pixels.
 */
void MedianFilterStrategy::Process(uint16_t* pixels, int width, int height) {
    if (pixels == nullptr || width <= 0 || height <= 0) {
        return;
    }

    if (width < 3 || height < 3) {
        std::copy(pixels,
              pixels + (size_t(width) * height),
              pixels);
        return;
    }

    // Create a local copy to ensure read-only access to source data
    std::vector<uint16_t> original(pixels, pixels + (static_cast<size_t>(width) * height));

    // Process spatial neighborhood excluding the image border (1-pixel padding)
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            std::array<uint16_t, 9> window;
            int index = 0;

            // Collect 3x3 neighborhood intensity values
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    window[index++] = original[(y + ky) * width + (x + kx)];
                }
            }

            // Determine median via partial or full sort
            std::sort(window.begin(), window.end());

            // Assign median to the destination buffer
            pixels[y * width + x] = window[4];
        }
    }
}

}  // namespace PhotonLab
