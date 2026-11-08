/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sha256.c: the SHA-256 of a file's bytes, for a package's fingerprint
 * (panel 235's R6, ratified 2026-11-07; `hero_compiler.h` declares it). After
 * `os.c`, whose `hero_file_bytes` reads the file.
 *
 * WHY IN C. A package's fingerprint covers every regular file under its root,
 * `.hero` or not, since a header a fetched `extern` reads is code the program
 * trusts, and a file that is not UTF-8 is no `str`: `cli/sha256.hero` hashes a
 * text, and a PNG in a package has none (the ffi-pragmatist's report, § 4,
 * measured: `not_text`). It costs about 1,603 instructions a byte at `-O0`
 * there (the compiler-engineer's § 5), and the tree is hashed at every build
 * (R7), so the bytes are hashed here, where they lie, and the listing of the
 * files' hashes, which is text, by `sha256.of` (`selfhost/module/
 * fingerprint.hero`).
 *
 * THE ALGORITHM IS FIPS 180-4's, written out: eight 32-bit words of state,
 * 64-byte blocks, the message padded with a 1 bit, zeros and its length in
 * bits, big-endian. A test holds it to `cli/sha256.hero` over texts and to
 * the standard's own vectors (`fingerprint.hero`). */

static const uint32_t hero_sha256_k[64] = {
    0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u, 0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
    0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u, 0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
    0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu, 0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
    0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u, 0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
    0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u, 0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
    0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u, 0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
    0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u, 0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
    0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u, 0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u
};

static uint32_t hero_sha256_rotr(uint32_t x, unsigned n) {
    return (x >> n) | (x << (32u - n));
}

static void hero_sha256_block(uint32_t h[8], const unsigned char *p) {
    uint32_t w[64];
    for (int i = 0; i < 16; i++) {
        w[i] = ((uint32_t)p[4 * i] << 24) | ((uint32_t)p[4 * i + 1] << 16) | ((uint32_t)p[4 * i + 2] << 8) | (uint32_t)p[4 * i + 3];
    }
    for (int i = 16; i < 64; i++) {
        uint32_t s0 = hero_sha256_rotr(w[i - 15], 7) ^ hero_sha256_rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
        uint32_t s1 = hero_sha256_rotr(w[i - 2], 17) ^ hero_sha256_rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], k = h[7];
    for (int i = 0; i < 64; i++) {
        uint32_t s1 = hero_sha256_rotr(e, 6) ^ hero_sha256_rotr(e, 11) ^ hero_sha256_rotr(e, 25);
        uint32_t ch = (e & f) ^ (~e & g);
        uint32_t t1 = k + s1 + ch + hero_sha256_k[i] + w[i];
        uint32_t s0 = hero_sha256_rotr(a, 2) ^ hero_sha256_rotr(a, 13) ^ hero_sha256_rotr(a, 22);
        uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        uint32_t t2 = s0 + maj;
        k = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
    h[5] += f;
    h[6] += g;
    h[7] += k;
}

/* The digest of `n` bytes at `p`, written as 64 lower-case hexadecimal
 * digits into `out`. */
static void hero_sha256_bytes(const unsigned char *p, uint64_t n, char out[64]) {
    uint32_t h[8] = {0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au, 0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u};
    uint64_t whole = n - n % 64u;
    for (uint64_t at = 0; at < whole; at += 64u) hero_sha256_block(h, p + at);
    unsigned char tail[128];
    uint64_t left = n - whole;
    memset(tail, 0, sizeof tail);
    if (left > 0) memcpy(tail, p + whole, (size_t)left);
    tail[left] = 0x80u;
    uint64_t room = left + 1u + 8u <= 64u ? 64u : 128u;
    uint64_t bits = n * 8u;
    for (int i = 0; i < 8; i++) tail[room - 1u - (uint64_t)i] = (unsigned char)(bits >> (8 * i));
    hero_sha256_block(h, tail);
    if (room == 128u) hero_sha256_block(h, tail + 64);
    static const char digits[] = "0123456789abcdef";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) out[8 * i + j] = digits[(h[i] >> (28 - 4 * j)) & 0xFu];
    }
}

/* THE DOOR (`hero_compiler.h`): the SHA-256 of the bytes of the file at
 * `path`, whatever they are, as 64 lower-case hexadecimal digits, with
 * `*status` HERO_OS_OK; or "" with the read's own failure, HERO_OS_NOT_FOUND
 * or HERO_OS_FAILED. A message length past what `n * 8` holds in 64 bits is
 * no file this machine reads whole, and `hero_file_bytes` fails it first. */
HeroStr hero_file_sha256(const char *path, int64_t *status) {
    int64_t got = 0;
    char *buffer = hero_file_bytes(path, &got, status);
    if (buffer == NULL) return hero_str_from_bytes("", 0);
    char out[64];
    hero_sha256_bytes((const unsigned char *)buffer, (uint64_t)got, out);
    hero_release(buffer);
    *status = HERO_OS_OK;
    return hero_str_from_bytes(out, 64);
}

/* The same over a text in hand, the listing the fingerprint hashes, so the
 * listing is hashed by the code that hashed its lines. */
HeroStr hero_text_sha256(HeroStr text) {
    hero_str_require(text);
    char out[64];
    hero_sha256_bytes((const unsigned char *)text.ptr, (uint64_t)text.len, out);
    return hero_str_from_bytes(out, 64);
}
