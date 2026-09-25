/* Panel 178 ffi-pragmatist: SHA256_Final writes 32 bytes through an
   `unsigned char *` with no length beside it, so the digest crosses as a
   record holding the array; the deprecation of the low-level API in
   OpenSSL 3 is silenced here, not in the program. */
#define OPENSSL_SUPPRESS_DEPRECATED 1
#include <openssl/sha.h>
struct digest { unsigned char b[32]; };
static inline int hero_sha256_final(struct digest *d, SHA256_CTX *c) { return SHA256_Final(d->b, c); }
