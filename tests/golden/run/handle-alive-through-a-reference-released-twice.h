/* OpenSSL's `X509_up_ref` shape: a status-returning reference count, 1 for
 * success, and a free that ends the life only at zero. Over a static cell. */
#include <stdint.h>
typedef struct x509 { int64_t refs; } x509;
static inline __attribute__((noinline)) x509 *cert_new(void) { static x509 one; one.refs = 1; return &one; }
static inline __attribute__((noinline)) int32_t cert_up_ref(x509 *a) { a->refs++; return 1; }
static inline __attribute__((noinline)) void cert_free(x509 *a) { a->refs--; }
static inline __attribute__((noinline)) int64_t cert_refs(x509 *a) { return a->refs; }
