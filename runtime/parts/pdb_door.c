/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/pdb_door.c — the compiler's door to a Windows build's lines (panel
 * 253's R5; `hero_compiler.h`, bound by `selfhost/cli/abort_symbolized.hero`).
 *
 * `heroes run` and `heroes test` turn the addresses an aborted program told
 * (`parts/calls.c`) into the calls the build's line tables hold there, every
 * inlined call with its own line. On Windows the tables are in the PDB the
 * link wrote beside the binary, and the one reader the box carries,
 * `llvm-symbolizer`, reads them wrongly (`parts/pdb.c` says how it was
 * measured), so this door reads them with `parts/pdb.c`. It answers in
 * `llvm-symbolizer --inlines`'s words, which the compiler already reads: per
 * address, per level from the innermost, the function's name on a line and
 * `<file>:<line>:0` on the next, `??` and `??:0:0` where nothing is known,
 * and a blank line after each address. The file is named from the compile's
 * folder, which is this process's own, as `--relativenames` names it, with
 * `/` between its parts.
 *
 * It is asked on every platform and answers only for a PE32+ binary whose
 * PDB is beside it and is the one the binary names (its GUID): any other
 * binary is HERO_OS_FAILED, and the compiler asks the next reader. Nothing
 * here runs in a program that aborts: this is the parent's. Its reader is
 * Windows' alone (`parts/pdb.c`), so elsewhere, where a build writes no PE
 * binary, the door answers HERO_OS_FAILED for every binary. */

#if defined(_WIN32)

/* A text growing in the runtime's scratch memory. */
typedef struct {
    char *bytes;
    size_t used, room;
} HeroPdbText;

static void hero_pdb_text_put(HeroPdbText *t, const char *s, size_t n) {
    if (n > t->room - t->used) {
        size_t room = t->room > 0 ? t->room : 4096;
        while (room - t->used < n) room *= 2;
        char *grown = hero_alloc(room);
        if (t->used > 0) memcpy(grown, t->bytes, t->used);
        if (t->bytes != NULL) hero_release(t->bytes);
        t->bytes = grown;
        t->room = room;
    }
    memcpy(t->bytes + t->used, s, n);
    t->used += n;
}

static void hero_pdb_text_say(HeroPdbText *t, const char *s) { hero_pdb_text_put(t, s, strlen(s)); }

/* This process's folder, `/` between its parts and one after the last, into
 * `out`: 1, or 0 where it cannot be had. */
static int hero_pdb_folder(char *out, size_t room) {
#if defined(_WIN32)
    wchar_t wide[4096];
    DWORD n = GetCurrentDirectoryW(4096, wide);
    if (n == 0 || n >= 4096) return 0;
    int put = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide, (int)n, out, (int)room - 2, NULL, NULL);
    if (put <= 0) return 0;
    out[put] = '\0';
#else
    if (getcwd(out, room - 2) == NULL) return 0;
#endif
    size_t len = strlen(out);
    for (size_t i = 0; i < len; i++) {
        if (out[i] == '\\') out[i] = '/';
    }
    if (len > 0 && out[len - 1] != '/') {
        out[len] = '/';
        out[len + 1] = '\0';
    }
    return 1;
}

/* Whether `file` begins with `folder`, a drive's letter in either case. */
static int hero_pdb_under(const char *file, const char *folder) {
    size_t n = strlen(folder);
    if (n == 0 || strlen(file) <= n) return 0;
    for (size_t i = 0; i < n; i++) {
        char a = file[i], b = folder[i];
        if (i == 0 && a >= 'A' && a <= 'Z') a = (char)(a - 'A' + 'a');
        if (i == 0 && b >= 'A' && b <= 'Z') b = (char)(b - 'A' + 'a');
        if (a != b) return 0;
    }
    return 1;
}

/* A hexadecimal address from `*at`, `0x` before it or not, past the spaces
 * before it: 1, or 0 where none is left or it is not one. */
static int hero_pdb_address(const char **at, uint64_t *out) {
    const char *p = *at;
    while (*p == ' ' || *p == '\n' || *p == '\t') p++;
    if (*p == '\0') return 0;
    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) p += 2;
    uint64_t v = 0;
    int digits = 0;
    for (;; p++) {
        int d = *p >= '0' && *p <= '9' ? *p - '0' : *p >= 'a' && *p <= 'f' ? *p - 'a' + 10 : *p >= 'A' && *p <= 'F' ? *p - 'A' + 10 : -1;
        if (d < 0) break;
        if (++digits > 16) return 0;
        v = v * 16 + (uint64_t)d;
    }
    if (digits == 0 || (*p != '\0' && *p != ' ' && *p != '\n' && *p != '\t')) return 0;
    *at = p;
    *out = v;
    return 1;
}

/* The levels of the address `va`, in the words above. */
static void hero_pdb_tell_address(HeroPdbText *t, const HeroPdb *pdb, const HeroPe *pe, uint64_t va, const char *folder) {
    HeroPdbPlace place;
    uint16_t seg = 0;
    uint32_t off = 0;
    int found = va >= pe->image_base && va - pe->image_base < pe->image_size &&
                hero_pe_section_of(pe, (uint32_t)(va - pe->image_base), &seg, &off) && hero_pdb_place(pdb, seg, off, &place);
    if (!found) {
        hero_pdb_text_say(t, "??\n??:0:0\n\n");
        return;
    }
    for (int i = 0; i < place.count; i++) {
        const HeroPdbLevel *l = &place.levels[i];
        hero_pdb_text_say(t, l->function[0] != '\0' ? l->function : "??");
        hero_pdb_text_say(t, "\n");
        char file[4096];
        if (!hero_pdb_line_named(l->line) || !hero_pdb_file_name(pdb, &place.proc.module, l->file, file, sizeof file)) {
            hero_pdb_text_say(t, "??:0:0\n");
            continue;
        }
        for (char *c = file; *c != '\0'; c++) {
            if (*c == '\\') *c = '/';
        }
        const char *named = hero_pdb_under(file, folder) ? file + strlen(folder) : file;
        char digits[24];
        size_t n = hero_calls_decimal(l->line, digits);
        hero_pdb_text_say(t, named);
        hero_pdb_text_say(t, ":");
        hero_pdb_text_put(t, digits, n);
        hero_pdb_text_say(t, ":0\n");
    }
    hero_pdb_text_say(t, "\n");
}

HeroStr hero_pdb_symbolized(const char *binary, const char *addresses, int64_t *status) {
    *status = HERO_OS_FAILED;
    int64_t got = 0, read = 0;
    char *image = hero_file_bytes(binary, &got, &read);
    if (image == NULL) return hero_str_from_bytes("", 0);
    HeroPe pe;
    unsigned char guid[16];
    char recorded[1024];
    if (!hero_pe_open(&pe, (const unsigned char *)image, (size_t)got, 0) || !hero_pe_pdb(&pe, guid, recorded, sizeof recorded)) {
        hero_release(image);
        return hero_str_from_bytes("", 0);
    }
    /* The PDB beside the binary, by the last part of the name it recorded. */
    size_t folder_len = hero_pdb_last_part(binary);
    const char *last = recorded + hero_pdb_last_part(recorded);
    size_t last_len = strlen(last);
    char *beside = hero_alloc(folder_len + last_len + 1);
    memcpy(beside, binary, folder_len);
    memcpy(beside + folder_len, last, last_len + 1);
    int64_t pdb_got = 0;
    char *pdb_bytes = hero_file_bytes(beside, &pdb_got, &read);
    hero_release(beside);
    HeroPdb pdb;
    unsigned char pdb_guid[16];
    char folder[4096 + 2];
    HeroPdbText t = {NULL, 0, 0};
    if (pdb_bytes != NULL && hero_pdb_open(&pdb, (const unsigned char *)pdb_bytes, (size_t)pdb_got) &&
        hero_pdb_guid(&pdb, pdb_guid) && memcmp(guid, pdb_guid, 16) == 0) {
        if (!hero_pdb_folder(folder, sizeof folder)) folder[0] = '\0';
        const char *at = addresses;
        uint64_t va = 0;
        int asked = 0;
        while (hero_pdb_address(&at, &va)) {
            hero_pdb_tell_address(&t, &pdb, &pe, va, folder);
            asked++;
        }
        const char *rest = at;
        while (*rest == ' ' || *rest == '\n' || *rest == '\t') rest++;
        if (asked > 0 && *rest == '\0' && hero_utf8_valid(t.bytes, (int64_t)t.used)) *status = HERO_OS_OK;
    }
    if (pdb_bytes != NULL) hero_release(pdb_bytes);
    hero_release(image);
    HeroStr out = *status == HERO_OS_OK ? hero_str_from_bytes(t.bytes, (int64_t)t.used) : hero_str_from_bytes("", 0);
    if (t.bytes != NULL) hero_release(t.bytes);
    return out;
}

#else

HeroStr hero_pdb_symbolized(const char *binary, const char *addresses, int64_t *status) {
    (void)binary;
    (void)addresses;
    *status = HERO_OS_FAILED;
    return hero_str_from_bytes("", 0);
}

#endif
