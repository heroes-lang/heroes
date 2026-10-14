/* ffi-pragmatist, panel 215, route D over a `partial` record: OpenSSL's SHA256_CTX bound as
   tests/golden/run/fixedbugs-396-sha256-final-fills-its-digest.hero binds it, `partial` naming
   `num` alone. Route D copies the fields the record names into a temporary zeroed whole, hands C
   the temporary, and copies the named fields back. */
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>
static SHA256_CTX *route_d(SHA256_CTX *z, const SHA256_CTX *s) { memset(z, 0, sizeof *z); z->num = s->num; return z; }
static void hex(const char *what, const unsigned char *md) { printf("%s ", what); for (int i = 0; i < 4; i++) printf("%02x", md[i]); printf("\n"); }
int main(void) {
    SHA256_CTX c, t1, t2, t3; unsigned char md[32], md2[32] = {0};
    SHA256_Init(&c); SHA256_Update(&c, "abc", 3); SHA256_Final(md, &c); hex("today  ", md);
    SHA256_Init(route_d(&t1, &c)); c.num = t1.num;
    SHA256_Update(route_d(&t2, &c), "abc", 3); c.num = t2.num;
    int r = SHA256_Final(md2, route_d(&t3, &c)); printf("route D Final returned %d; ", r); hex("route D", md2);
    return 0;
}
