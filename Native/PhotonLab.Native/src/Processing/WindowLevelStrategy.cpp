// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : WindowLevelStrategy.cpp                             */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*                                                                   */
/* Module      : Core/Processing                                     */
/* Component   : Window/Level Filter                                 */
/* Thread Safe : Yes                                                 */
/* Complexity  : O(n)                                                */
/* API Status  : Stable                                              */
/* Exception Safety : Basic Guarantee                                */
/*                                                                   */
/* Description : Implements Window/Level intensity transformation    */
/* for 16-bit medical imaging data visualization.                    */
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

#include "Processing/WindowLevelStrategy.hpp"

#include <algorithm>
#include <cstdint>

namespace PhotonLab {
WindowLevelStrategy::WindowLevelStrategy(int window, int level) : m_window(window), m_level(level) {}

void WindowLevelStrategy::Process(uint16_t* pixels, int width, int height) {
    if (pixels == nullptr || width <= 0 || height <= 0) {
        return;
    }

    const int lower = m_level - (m_window / 2);
    const int upper = m_level + (m_window / 2);

    const size_t size = static_cast<size_t>(width) * height;

    for (size_t i = 0; i < size; ++i) {
        auto& pixel = pixels[i];

        if (pixel <= lower) {
            pixel = 0;
        } else if (pixel >= upper) {
            pixel = 65535;
        } else {
            pixel = static_cast<uint16_t>(((pixel - lower) * 65535) / (upper - lower));
        }
    }
}

}  // namespace PhotonLab
