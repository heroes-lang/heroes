#define OPENSSL_SUPPRESS_DEPRECATED 1
#include <openssl/sha.h>
struct box { unsigned char m; unsigned char rest[7]; long long after; };
