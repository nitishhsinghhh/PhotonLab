// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : WindowLevelStrategy.hpp                             */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module           : Core/Processing                                */
/* Component        : Window/Level Filter                            */
/* Thread Safe      : Yes                                            */
/* Complexity       : O(n)                                           */
/* API Status       : Stable                                         */
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

#ifndef WINDOWLEVELSTRATEGY_HPP
#define WINDOWLEVELSTRATEGY_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <cstdint>

#include "IImageProcessingStrategy.hpp"

namespace PhotonLab {
/**
 * @class WindowLevelStrategy
 * @brief Concrete strategy for applying window/level adjustment.
 * *
 * * This filter maps the 16-bit source intensity range to a
 * viewable range based on defined window width and level.
 */
class WindowLevelStrategy : public IImageProcessingStrategy {
public:
  /**
   * @brief Constructs the strategy with specific contrast parameters.
   * @param window The range of intensity values to display.
   * @param level The center intensity value.
   */
  WindowLevelStrategy(int window, int level);

  /**
   * @brief Applies the window/level transformation to the buffer.
   * @param pixels Pointer to the raw 16-bit pixel data.
   * @param width Image width in pixels.
   * @param height Image height in pixels.
   */
  void Process(uint16_t *pixels, int width, int height) override;

private:
  int m_window;
  int m_level;
};
} // namespace PhotonLab

#endif // WINDOWLEVELSTRATEGY_HPP
