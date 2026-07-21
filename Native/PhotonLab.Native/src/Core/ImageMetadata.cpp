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
/* Description : Provides the compilation unit for the               */
/* ImageMetadata data contract used by the native image processing   */
/* engine and managed .NET interop layer.                            */
/*                                                                   */
/* Notes       : The implementation is intentionally empty because   */
/* ImageMetadata is a Plain Old Data (POD) structure. This file      */
/* exists to provide a dedicated translation unit and maintain a     */
/* consistent project layout.                                        */
/*                                                                   */
/* Revision History:                                                 */
/* ----------------------------------------------------------------- */
/* Version    Date       Author         Description                  */
/* ----------------------------------------------------------------- */
/* 1.0        2026-06-10 Nitish Singh   Initial implementation       */
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
