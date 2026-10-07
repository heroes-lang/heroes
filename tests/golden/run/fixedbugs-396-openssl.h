#ifndef FIXEDBUGS_396_OPENSSL_H
#define FIXEDBUGS_396_OPENSSL_H

/* Defect 396's own function (lane b13-land-buf, 2026-10-07): OpenSSL's, read
   whole. OpenSSL 3 marks `SHA256_Init` and its two siblings deprecated, and a
   clang warning on a correct program fails the `warnings` suite, so this
   header asks OpenSSL not to mark them; nothing else is written here. */
#define OPENSSL_SUPPRESS_DEPRECATED 1
#include <openssl/sha.h>
#include <openssl/evp.h>

#endif
