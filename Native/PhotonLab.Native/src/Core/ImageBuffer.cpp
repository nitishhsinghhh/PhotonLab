// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ImageBuffer.cpp                                     */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module           : Core                                           */
/* Component        : Image Memory Container                         */
/* Thread Safe      : No (Instance isolation managed by caller)      */
/* Complexity       : O(1) for accessors, O(N) for allocation/move   */
/* API Status       : Stable                                         */
/* Exception Safety : Strong Guarantee (Move construction)           */
/*                                                                   */
/* Description : Implements the ImageBuffer class, providing a       */
/* lightweight wrapper around contiguous 16-bit image pixel storage  */
/* for native image processing and managed interoperability.         */
/*                                                                   */
/* Notes       : Designed for high-performance image processing.     */
/* Uses std::vector<uint16_t> to guarantee contiguous memory         */
/* required by native algorithms and P/Invoke interfaces.            */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#include "Core/ImageBuffer.hpp"

/*********************************************************************/
/* Namespace: PhotonLab                                              */
/*********************************************************************/

namespace PhotonLab {

/**
 * @brief Direct Resource Allocation Constructor via Rvalue Move
 */
ImageBuffer::ImageBuffer(int width, int height, std::vector<uint16_t> pixels)
    : m_width(width), m_height(height), m_pixels(std::move(pixels)) {}

/**
 * @brief Returns pixel width bounding constraints.
 */
int ImageBuffer::Width() const noexcept { return m_width; }

/**
 * @brief Returns pixel height bounding constraints.
 */
int ImageBuffer::Height() const noexcept { return m_height; }

/**
 * @brief Non-const mutable raw pointer interface for in-place modifications.
 */
uint16_t* ImageBuffer::Data() noexcept { return m_pixels.data(); }

/**
 * @brief Const read-only pointer access optimized for pipeline evaluation.
 */
const uint16_t* ImageBuffer::Data() const noexcept { return m_pixels.data(); }

/**
 * @brief Returns complete underlying contiguous element cardinality.
 */
size_t ImageBuffer::Size() const noexcept { return m_pixels.size(); }

}  // namespace PhotonLab
