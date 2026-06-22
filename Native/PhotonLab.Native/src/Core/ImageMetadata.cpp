// SPDX-License-Identifier: Apache-2.0

/*********************************************************************/
/* File        : ImageMetadata.cpp                                   */
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
/* Description : Provides explicit compilation scope linkage for     */
/* the structural data contract backing native image                 */
/* pipelines and .NET managed UI interop states.                     */
/*                                                                   */
/*********************************************************************/

#include "Core/ImageMetadata.hpp"

/*********************************************************************/
/* Namespace: PhotonLab                                              */
/*********************************************************************/

namespace PhotonLab {

// This data structure is a standard C-ABI compliant layout contract.
// It intentionally exposes unencapsulated properties with value
// initializers to ensure direct, zero-overhead memory copying
// across P/Invoke boundaries to a matching C# [StructLayout] type.

}  // namespace PhotonLab
