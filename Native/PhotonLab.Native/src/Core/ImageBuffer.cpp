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
/* Complexity       : O(1) for data accessors, O(N) allocation       */
/* API Status       : Stable                                         */
/* Exception Safety : Strong Guarantee (Constructor move semantics)  */
/*                                                                   */
/* Description : Explicit wrapper encapsulating raw 16-bit medical   */
/* and scientific pixel buffers, providing contiguous                */
/* memory interfaces for native interop layers.                      */
/*                                                                   */
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
uint16_t *ImageBuffer::Data() noexcept { return m_pixels.data(); }

/**
 * @brief Const read-only pointer access optimized for pipeline evaluation.
 */
const uint16_t *ImageBuffer::Data() const noexcept { return m_pixels.data(); }

/**
 * @brief Returns complete underlying contiguous element cardinality.
 */
size_t ImageBuffer::Size() const noexcept { return m_pixels.size(); }

} // namespace PhotonLab
