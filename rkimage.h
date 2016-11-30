/**
 * \file rkimage.h
 *
 * \brief RKImageMaker
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
#ifndef _RKIMAGE_H_
#define _RKIMAGE_H_

#include <stddef.h>
#include <stdint.h>

#define RKIMAGE_MAGIC_KNRL 0x4C4E524B //"KRNL"
#define RKIMAGE_MAGIC_RKFW 0x57464b52 //"RKFW"
#define RKIMAGE_MAGIC_RKAF 0x46414b52 //"RKAF"
#define RKIMAGE_MAGIC_BOOT 0x57464b52 //"BOOT"

struct rkimage_time {
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
} __attribute__((packed, aligned(1)));
;

struct rkimage_loader {
    uint32_t magic;   // 固定为"BOOT"
    uint16_t hdrlen;  // header size
    uint32_t version; //
    uint32_t code;    // mergeversion

    // 创建时间
    struct rkimage_time time;

    uint32_t chiptype; // 芯片类型
} __attribute__((packed, aligned(1)));
;

struct rkimage_ptn {
    char     name[32];
    char     filename[60];
    uint32_t nand_size;
    uint32_t pos;
};

struct rkimage_ahdr {
    uint32_t magic;  // 固定为"RKAF"
    uint16_t hdrlen; // header size;
    int8_t   model[0x22];
    int8_t   id[0x1e];
    int8_t   mft[0x38];
    uint32_t unknown1;
    uint32_t version;
    uint32_t num_ptns;

    struct rkimage_ptn ptns[16];
    uint8_t            reserved[0x74];
} __attribute__((packed, aligned(4)));

// a as major version number, b as minor version number, c as revision number
#define IMG_VERSION(a, b, c) (((a) << 24) + ((b) << 16) + (c))

struct rkimage_header {

    uint32_t magic;   // 固定为"RKFW"
    uint16_t hdrlen;  // header size
    uint32_t version; // ROM_VERSION()
    uint32_t code;    // mergeversion

    // 创建时间
    struct rkimage_time time;

    uint32_t chiptype; // 芯片类型

    uint32_t loader_offset; //loader 偏移
    uint32_t loader_length; //loader 长度

    uint32_t image_offset; //image偏移
    uint32_t image_length; //image长度

    uint32_t unknown1;
    uint32_t unknown2;
    uint32_t system_fstype;
    uint32_t backup_endpos;

    uint8_t reserved[0x2D];
} __attribute__((packed, aligned(4)));

#endif
