// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ImageBuffer.hpp                                     */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module      : Core/Imaging                                        */
/* Component   : Image Buffer Management                             */
/* Thread Safe : No                                                  */
/* Complexity  : O(1) for access, O(n) for allocation                */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Provides a container for 16-bit raw image data,     */
/* handling buffer ownership and dimension metadata.                 */
/*                                                                   */
/* Notes       : Designed for high-performance processing pipelines. */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#ifndef IMAGEBUFFER_HPP
#define IMAGEBUFFER_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <cstddef>
#include <cstdint>
#include <vector>

namespace PhotonLab {
/**
 * @class ImageBuffer
 * @brief Manages a 16-bit pixel buffer with associated dimensions.
 * * Represents a single frame of image data, providing memory-safe
 * access to pixel values for native processing algorithms.
 */
class ImageBuffer {
   public:
    ImageBuffer() = default;

    /**
     * @brief Constructs an image buffer with specified dimensions and data.
     * @param width The image width in pixels.
     * @param height The image height in pixels.
     * @param pixels The initial pixel vector.
     */
    ImageBuffer(int width, int height, std::vector<uint16_t> pixels);

    int Width() const noexcept;

    int Height() const noexcept;

    uint16_t* Data() noexcept;

    const uint16_t* Data() const noexcept;

    size_t Size() const noexcept;

   private:
    int m_width{};
    int m_height{};
    std::vector<uint16_t> m_pixels;
};
}  // namespace PhotonLab

#endif  // IMAGEBUFFER_HPP
