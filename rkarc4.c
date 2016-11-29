/*
 *  An implementation of the ARCFOUR algorithm
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

#include "arc4.h"

#include <stdio.h>
#include <string.h>

int rockchip_arc4(size_t length, const unsigned char* input, unsigned char* output)
{
    arc4_context ctx;

    const unsigned char arc4key[] = {
        0x7C, 0x4E, 0x03, 0x04, 0x55, 0x05, 0x09, 0x07,
        0x2D, 0x2C, 0x7B, 0x38, 0x17, 0x0D, 0x17, 0x11
    };

    arc4_init(&ctx);
    arc4_setup(&ctx, arc4key, 16);
    return arc4_crypt(&ctx, length, input, output);
}

int __attribute__((weak)) main(void)
{
    //arc4_self_test(1);

    unsigned char cipher[256]  = { 0 };
    unsigned char decrypt[256] = { 0 };

    const unsigned char plain[256] = "Welcome to RC4 Test! \n"
                                     "Email: kaspter@kmatt.net\n";

    rockchip_arc4(strlen(plain), plain, cipher);
    printf("%s\n", cipher);

    rockchip_arc4(strlen(cipher), cipher, decrypt);
    printf("%s", decrypt);
}
