/*
 *  RKImageMaker
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Copyright (C) 2016-2017, Kaspter Ju, All Rights Reserved
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

#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <limits.h>
#include <stdarg.h>
#include <sys/mman.h>

#include "md5.h"
#include "rkimage.h"

int rkimage_hdr_dump(struct rkimage_header* hdr)
{
    struct rkimage_time* time = &hdr->time;

    printf("   hdr->magic = %s\n", (char*)&hdr->magic);
    printf("   hdr->hdrsize = 0x%x\n", hdr->hdrlen);
    printf("   hdr->version = %x.%x.%x\n", (hdr->version >> 24) & 0xFF, (hdr->version >> 16) & 0xFF, hdr->version & 0xFFFF);
    printf("   hdr->code = 0x%x\n", hdr->code);

    printf("   build time: %d-%02d-%02d %02d:%02d:%02d\n",
           time->year, time->month, time->day, time->hour, time->minute, time->second);

    printf("   hdr->chiptype = 0x%x\n", hdr->chiptype);

    printf("   hdr->unknown1 = 0x%x\n", hdr->unknown1);
    printf("   hdr->unknown2 = 0x%x\n", hdr->unknown2);
    printf("   hdr->system_fstype = 0x%x\n", hdr->system_fstype);
    printf("   hdr->backup_endpos = 0x%x\n", hdr->backup_endpos);
    return 0;
}

int rkimage_ahdr_dump(struct rkimage_ahdr* ahdr)
{

    printf("   hdr->magic = %s\n", (char*)&ahdr->magic);
    printf("   hdr->hdrsize = 0x%x\n", ahdr->hdrlen);

    return 0;
}

int rkimage_md5(FILE* fin)
{
    return 0;
}

int rkimage_dump(FILE* fin, uint32_t offset, uint32_t length)
{
    int            ret;
    FILE*          fout;
    unsigned char* buff;
    char           filename[32] = { 0 };
    unsigned char  md5sum[16]   = { 0 };

    buff = malloc(length);
    if (!buff) {
        printf("error malloc \n");
    }
    memset(buff, 0, length);

    fseek(fin, offset, SEEK_SET);

    ret = fread(buff, 1, length, fin);

    if (!memcmp(buff, "BOOT", 4)) {
        sprintf(filename, "rkloader.bin");
    } else if (!memcmp(buff, "RKAF", 4)) {
        sprintf(filename, "rkupdate.bin");
    } else {
        sprintf(filename, "rkraw.bin");
    }

    md5(buff, length, md5sum);
    printf("   dump %s done, image offset = 0x%08x, length = 0x%08x, md5=%s\n", filename, offset, length, md5sum);

    fout = fopen(filename, "wb");
    ret  = fwrite(buff, length, 1, fout);

    free(buff);
    fclose(fout);

    return ret;
}

int rkimage_pack(int argc, char* argv[])
{
    return 0;
}

int rkimage_rkaf_unpack(FILE* fin)
{
    int   ret;
    off_t filesize;

    struct rkimage_ahdr rkimg_ahdr;
    memset(&rkimg_ahdr, 0, sizeof(struct rkimage_ahdr));

    if (fseek(fin, 0, SEEK_SET) < 0) {
        fprintf(stderr, "fseek(0,SEEK_SET) failed\n");
        goto exit;
    }

    ret = fread(&rkimg_ahdr, sizeof(struct rkimage_ahdr), 1, fin);
    if (ret != 1) {
        fprintf(stderr, "error fread\n");
        goto exit;
    }

    rkimage_ahdr_dump(&rkimg_ahdr);

    return 0;
exit:
    return -1;
}

int rkimage_rkfw_unpack(FILE* fin)
{
    int   ret;
    off_t filesize;

    struct rkimage_header rkimg_hdr;
    memset(&rkimg_hdr, 0, sizeof(struct rkimage_header));

    if ((filesize = lseek(fileno(fin), 0, SEEK_END)) < 0) {
        perror("lseek");
        goto exit;
    }

    if (fseek(fin, 0, SEEK_SET) < 0) {
        fprintf(stderr, "fseek(0,SEEK_SET) failed\n");
        goto exit;
    }

    ret = fread(&rkimg_hdr, sizeof(struct rkimage_header), 1, fin);
    if (ret != 1) {
        fprintf(stderr, "error fread\n");
        goto exit;
    }

    rkimage_hdr_dump(&rkimg_hdr);

    rkimage_dump(fin, rkimg_hdr.loader_offset, rkimg_hdr.loader_length);
    rkimage_dump(fin, rkimg_hdr.image_offset, rkimg_hdr.image_length);

    return 0;
exit:
    return -1;
}

int rkimage_unpack(int argc, char* argv[])
{
    int      ret;
    FILE*    fin;
    uint32_t magic;

    if ((fin = fopen(argv[1], "rb")) == NULL) {
        perror("Open img file");
        goto exit;
    }

    ret = fread(&magic, sizeof(magic), 1, fin);
    if (ret != 1) {
        fprintf(stderr, "error fread\n");
        goto exit;
    }

    printf("   magic = %s - 0x%x\n", (char*)&magic, magic);

    if (magic == RKIMAGE_MAGIC_RKFW) {
        rkimage_rkfw_unpack(fin);
    } else if (magic == RKIMAGE_MAGIC_RKAF) {
        rkimage_rkaf_unpack(fin);
    } else {
        printf("wtf error magic\n");
    }

    fclose(fin);

    return 0;
exit:
    return -1;
}

int main(int argc, char* argv[])
{
    if (!strcmp(basename(argv[0]), "rkimage_pack"))
        return rkimage_pack(argc, argv);

    if (!strcmp(basename(argv[0]), "rkimage_unpack"))
        return rkimage_unpack(argc, argv);

    return EXIT_SUCCESS;
}
