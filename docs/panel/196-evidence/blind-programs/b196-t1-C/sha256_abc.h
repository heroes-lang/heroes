#ifndef SHA256_ABC_H
#define SHA256_ABC_H

#include <stddef.h>
#include <openssl/sha.h>

/* A buffer for one SHA-256 digest, so the program can lend its field to C. */
typedef struct sha256_digest {
    unsigned char md[SHA256_DIGEST_LENGTH];
} sha256_digest;

/* SHA256() with the output extent passed beside the output buffer.
   Returns 1 on success, 0 when md is too small or OpenSSL fails. */
static inline int sha256_into(const char *data, size_t data_len,
                              unsigned char *md, size_t md_len)
{
    if (md_len < SHA256_DIGEST_LENGTH)
        return 0;
    return SHA256((const unsigned char *)data, data_len, md) != NULL;
}

#endif
