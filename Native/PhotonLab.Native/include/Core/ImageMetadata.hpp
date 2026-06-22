// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ImageMetadata.hpp                                   */
/* Author      : Nitish Singh                                        */
/* Created     : 2026-06-10                                          */
/*                                                                   */
/* Copyright (c) 2016-2026 Nitish Singh                              */
/* Licensed under the Apache License, Version 2.0                    */
/* See LICENSE file in project root for license information          */
/*********************************************************************/
/*                                                                   */
/* Module           : Core/Imaging                                   */
/* Component        : Image Metadata Definition                      */
/* Thread Safe      : Yes                                            */
/* Complexity       : O(1)                                           */
/* API Status       : Stable                                         */
/* Exception Safety : N/A (Plain Old Data)                           */
/*                                                                   */
/* Description : Defines structural metadata for image buffers,      */
/* including dimensions, bit depth, and statistical intensity data.  */
/*                                                                   */
/* Notes       : Used for UI binding and native algorithm tuning.    */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
/*********************************************************************/

#ifndef IMAGEMETADATA_HPP
#define IMAGEMETADATA_HPP

/*********************************************************************/
/* Dependencies                                                      */
/*********************************************************************/

#include <cstdint>

namespace PhotonLab {
/**
 * @struct ImageMetadata
 * @brief Encapsulates diagnostic and geometric image properties.
 * * This structure serves as the data contract between the native image
 * processing engine and the managed .NET UI layer, ensuring consistent metadata
 * reporting.
 */
struct ImageMetadata {
    int Width{};

    int Height{};

    int BitDepth{16};

    uint16_t MinIntensity{};

    uint16_t MaxIntensity{};

    double MeanIntensity{};
};
}  // namespace PhotonLab

#endif  // IMAGEMETADATA_HPP
