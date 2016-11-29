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
#ifndef __ARC4_H__
#define __ARC4_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          ARC4 context structure
 */
typedef struct
{
    int           x;      /*!< permutation index */
    int           y;      /*!< permutation index */
    unsigned char m[256]; /*!< permutation table */
} arc4_context;

/**
 * \brief          Initialize ARC4 context
 *
 * \param ctx      ARC4 context to be initialized
 */
void arc4_init(arc4_context* ctx);

/**
 * \brief          Clear ARC4 context
 *
 * \param ctx      ARC4 context to be cleared
 */
void arc4_free(arc4_context* ctx);

/**
 * \brief          ARC4 key schedule
 *
 * \param ctx      ARC4 context to be setup
 * \param key      the secret key
 * \param keylen   length of the key, in bytes
 */
void arc4_setup(arc4_context* ctx, const unsigned char* key,
                unsigned int keylen);

/**
 * \brief          ARC4 cipher function
 *
 * \param ctx      ARC4 context
 * \param length   length of the input data
 * \param input    buffer holding the input data
 * \param output   buffer for the output data
 *
 * \return         0 if successful
 */
int arc4_crypt(arc4_context* ctx, size_t length, const unsigned char* input,
               unsigned char* output);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int arc4_self_test(int verbose);

#ifdef __cplusplus
}
#endif

#endif /* arc4.h */
