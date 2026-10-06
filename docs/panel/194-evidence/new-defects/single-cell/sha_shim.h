#include <openssl/evp.h>
#include <openssl/sha.h>
struct digest32 { unsigned char b[32]; };
#define DIGEST32_ZERO {0}
static inline int hero_evp_final(EVP_MD_CTX *c, struct digest32 *d) { unsigned int n = 0; int r = EVP_DigestFinal_ex(c, d->b, &n); return r == 1 && n == 32; }
static inline int hero_sha256_final(struct digest32 *d, SHA256_CTX *c) { return SHA256_Final(d->b, c); }
