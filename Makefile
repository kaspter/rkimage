CC=gcc
CFLAGS := -g -O3 -Wall

TARGETS := mkkrnlimg rkimage_unpack rkimage_pack

all: ${TARGETS}


mkkrnlimg: mkkrnlimg.c
	echo "  CC    mkkrnlimg.c"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) mkkrnlimg.c $(LOCAL_LDFLAGS) $(LDFLAGS) -o $@

rkimage_unpack: rkimage.c md5.c
	echo "  CC    rkimage.c md5.c"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) rkimage.c md5.c $(LOCAL_LDFLAGS) $(LDFLAGS) -o $@

rkimage_pack: rkimage.c md5.c
	echo "  CC    rkimage.c md5.c"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) rkimage.c md5.c $(LOCAL_LDFLAGS) $(LDFLAGS) -o $@


clean:
	rm ${TARGETS}
