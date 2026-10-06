#ifndef ABC_BUFFERS_H
#define ABC_BUFFERS_H

typedef struct Message {
    unsigned char bytes[3];
} Message;

typedef struct Digest {
    unsigned char bytes[32];
} Digest;

#endif
