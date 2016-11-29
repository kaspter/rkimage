/**
 * \file arc4.h
 *
 * \brief The ARCFOUR stream cipher
 *
 *  Copyright (C) 2006-2015, Kaspter Ju, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */
#ifndef __RKARC4_H__
#define __RKARC4_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          Rockchip ARC4 cipher function
 *
 * \param length   length of the input data
 * \param input    buffer holding the input data
 * \param output   buffer for the output data
 *
 * \return         0 if successful
 */
int rockchip_arc4(size_t length, const unsigned char* input, unsigned char* output);

#ifdef __cplusplus
}
#endif

#endif /* arc4.h */
