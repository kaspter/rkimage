#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rkcrc.h"

#define MAGIC_CODE 0x4C4E524B //"KRNL"

/**
 *  uint32_t magic
 *  uint32_t size
 *  data.....
 *  uint32_t crc
 **/

struct rkimage_khdr {
    uint32_t magic;
    uint32_t length;
};

int pack_krnl(FILE* fin, FILE* fout)
{
    unsigned char       buf[1024];
    struct rkimage_khdr header = { MAGIC_CODE, 0 };

    unsigned int crc = 0;

    fwrite(&header, sizeof(header), 1, fout);

    while (1) {
        int readlen = fread(buf, 1, sizeof(buf), fin);
        if (readlen == 0)
            break;

        header.length += readlen;
        fwrite(buf, 1, readlen, fout);
        crc = rkcrc32(crc, buf, readlen);
    }

    fwrite(&crc, sizeof(crc), 1, fout);
    fseek(fout, 0, SEEK_SET);
    fwrite(&header, sizeof(header), 1, fout);

    printf("%04X\n", crc);

    return 0;
    // fail:
    fprintf(stderr, "FAIL\n");
    return -1;
}

int unpack_krnl(FILE* fin, FILE* fout)
{
    unsigned char       buf[1024];
    struct rkimage_khdr header;
    size_t              length   = 0;
    unsigned int        crc      = 0;
    unsigned int        file_crc = 0;

    fprintf(stderr, "unpacking...");
    fflush(stderr);
    if (sizeof(header) != fread(&header, 1, sizeof(header), fin)) {
        goto fail;
    }

    fseek(fin, header.length + sizeof(header), SEEK_SET);
    if (sizeof(file_crc) != fread(&file_crc, 1, sizeof(file_crc), fin))
        goto fail;

    length = header.length;
    fseek(fin, sizeof(header), SEEK_SET);

    while (length > 0) {
        int readlen = length < sizeof(buf) ? length : sizeof(buf);
        readlen     = fread(buf, 1, readlen, fin);
        length -= readlen;
        fwrite(buf, 1, readlen, fout);
        crc = rkcrc32(crc, buf, readlen);

        if (readlen == 0)
            break;
    }

    if (file_crc != crc)
        fprintf(stderr, "WARNING: bad crc checksum\n");

    fprintf(stderr, "OK\n");
    return 0;

fail:
    fprintf(stderr, "FAIL\n");
    return -1;
}

int main(int argc, char** argv)
{
    int   ret;
    FILE *fin, *fout;

    struct rkimage_khdr khdr;
    memset(&khdr, 0, sizeof(struct rkimage_khdr));

    if (argc != 3) {
        fprintf(stderr, "usage: %s <input> <output>\n", argv[0]);
        return -1;
    }

    fin = fopen(argv[1], "rb");
    if (!fin) {
        fprintf(stderr, "can't open input file '%s': %s\n", argv[2],
                strerror(errno));
        goto exit;
    }

    if (fseek(fin, 0, SEEK_SET) < 0) {
        fprintf(stderr, "fseek(0,SEEK_SET) failed\n");
        goto exit;
    }

    ret = fread(&khdr, sizeof(struct rkimage_khdr), 1, fin);
    if (ret != 1) {
        fprintf(stderr, "error fread\n");
        goto exit;
    }

    if (fseek(fin, 0, SEEK_SET) < 0) {
        fprintf(stderr, "fseek(0,SEEK_SET) failed\n");
        goto exit;
    }

    fout = fopen(argv[2], "wb");
    if (!fout) {
        fprintf(stderr, "can't open output file '%s': %s\n", argv[3],
                strerror(errno));
        goto exit;
    }

    if (khdr.magic == MAGIC_CODE) {
        unpack_krnl(fin, fout);
    } else {
        pack_krnl(fin, fout);
    }

    fclose(fin);
    fclose(fout);

    return EXIT_SUCCESS;
exit:
    if (fin)
        fclose(fin);
    if (fout)
        fclose(fout);
    return -1;
}
