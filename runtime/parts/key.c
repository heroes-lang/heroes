/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/key.c: the key of a file's bytes, for the compiler's build cache
 * (defect 484; `hero_compiler.h` declares it). After `os.c`, whose `hero_file_bytes`
 * reads the file and whose `hero_bytes_shown` this walks the same way, and
 * `str.c`'s `hero_utf8_sequence`, the one judge of a sequence. */

/* THE KEY OF A FILE'S BYTES, IN C (defect 484, `hero_compiler.h`). The build cache
 * keys every object it serves and every header and runtime file a build
 * reads (`selfhost/module/reading.hero`'s `key_of`), and the key was made in
 * Heroes at `-O0`: the shown read, which writes each byte that is not UTF-8
 * as three, then `cli/digest`'s loop over the text, about 1,158 instructions
 * a byte of object (lane b14-cli's count, defect 443), +13.4% on a warm build
 * of `print(1)`. This is the same key, byte for byte, made over the bytes
 * where they lie: the text the Heroes route hashed is never built.
 *
 * THE DIGEST IS `cli/digest.hero`'s, and a test holds the two to each other
 * (`reading.hero`): two polynomial hashes over the bytes, bases 131 and 137,
 * mod 2^31 - 1, each written as eight lower-case hexadecimal digits, the
 * first hash's then the second's. Each step stays far below 2^64 (a sum below
 * 2^31 times 137 plus a byte), so the remainder is exact in `uint64_t`.
 *
 * WHAT IT HASHES is what `reading.keyed` hashed. A file of UTF-8: its bytes.
 * One that is not: the shown read's answer as `hero_bytes_shown` lays it out,
 * the marks first, two characters for each byte that begins no sequence (its
 * value in upper-case hexadecimal) and `--` for each U+FFFD the file holds,
 * then the text, each such byte as U+FFFD's three bytes; walked twice over the
 * file's bytes, once for each half, with `hero_utf8_sequence` as the one judge
 * of a sequence, and the key's first character then `n`, which `digest` never
 * writes (panel 189's R6). Past INT64_MAX / 5 bytes that answer could not be
 * made, and HERO_OS_FAILED is the read's answer here as there. */
typedef struct HeroKeySums {
    uint64_t a;
    uint64_t b;
} HeroKeySums;

#define HERO_KEY_MOD UINT64_C(2147483647)

static void hero_key_byte(HeroKeySums *k, unsigned char v) {
    k->a = (k->a * 131u + v) % HERO_KEY_MOD;
    k->b = (k->b * 137u + v) % HERO_KEY_MOD;
}

static void hero_key_run(HeroKeySums *k, const char *p, int64_t n) {
    const unsigned char *u = (const unsigned char *)p;
    uint64_t a = k->a;
    uint64_t b = k->b;
    for (int64_t i = 0; i < n; i++) {
        a = (a * 131u + u[i]) % HERO_KEY_MOD;
        b = (b * 137u + u[i]) % HERO_KEY_MOD;
    }
    k->a = a;
    k->b = b;
}

/* `hex8(a) + hex8(b)`, `cli/digest.hero`'s spelling: the high digit first. */
static void hero_key_spell(const HeroKeySums *k, char out[16]) {
    static const char digits[] = "0123456789abcdef";
    for (int i = 0; i < 8; i++) {
        out[7 - i] = digits[(k->a >> (4 * i)) & 0xFu];
        out[15 - i] = digits[(k->b >> (4 * i)) & 0xFu];
    }
}

/* Whether the sequence of `n` bytes at `p` is U+FFFD, which the shown read
 * marks `--` (a U+FFFD the file itself held). */
static int hero_key_is_replacement(const char *p, int64_t n) {
    return n == 3 && (unsigned char)p[0] == 0xEF && (unsigned char)p[1] == 0xBF && (unsigned char)p[2] == 0xBD;
}

HeroStr hero_file_key(const char *path, int64_t *status) {
    int64_t got = 0;
    char *buffer = hero_file_bytes(path, &got, status);
    if (buffer == NULL) return hero_str_from_bytes("", 0);
    HeroKeySums k = {0, 0};
    char key[16];
    if (hero_utf8_valid(buffer, got)) {
        hero_key_run(&k, buffer, got);
        hero_key_spell(&k, key);
        *status = HERO_OS_OK;
    } else if (got > INT64_MAX / 5) {
        hero_release(buffer);
        *status = HERO_OS_FAILED;
        return hero_str_from_bytes("", 0);
    } else {
        static const char hex[] = "0123456789ABCDEF";
        int64_t i = 0;
        while (i < got) {
            int64_t n = hero_utf8_sequence(buffer, i, got);
            if (n == 0) {
                unsigned char v = (unsigned char)buffer[i];
                hero_key_byte(&k, (unsigned char)hex[v >> 4]);
                hero_key_byte(&k, (unsigned char)hex[v & 0x0F]);
                i += 1;
                continue;
            }
            if (hero_key_is_replacement(buffer + i, n)) {
                hero_key_byte(&k, '-');
                hero_key_byte(&k, '-');
            }
            i += n;
        }
        i = 0;
        while (i < got) {
            int64_t n = hero_utf8_sequence(buffer, i, got);
            if (n == 0) {
                hero_key_byte(&k, 0xEF);
                hero_key_byte(&k, 0xBF);
                hero_key_byte(&k, 0xBD);
                i += 1;
                continue;
            }
            hero_key_run(&k, buffer + i, n);
            i += n;
        }
        hero_key_spell(&k, key);
        key[0] = 'n';
        *status = HERO_OS_NOT_TEXT;
    }
    hero_release(buffer);
    return hero_str_from_bytes(key, 16);
}
