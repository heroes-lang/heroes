#ifndef HOSTBUF_H
#define HOSTBUF_H

/* A fixed buffer for gethostname to write into. */
struct host_buf {
    char name[256];
};

#endif
