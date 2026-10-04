/* parts/codepage.c — the code page every narrow name in the process is read
 * in, on Windows (design.md §1.11, §1.12, §4.20; panel 191, defect 238).
 *
 * WHY. Every door this runtime opens to a name is narrow: `fopen`,
 * `CreateFileA`, `FindFirstFileA`, `MoveFileExA`, `CreateProcessA`, `getenv`,
 * and `main`'s own `argv`, which the C runtime builds before `main` runs.
 * Windows reads each of those bytes in the process's ANSI code page, which is
 * 1252 on the box, so a `str` holding `café` named a file `cafÃ©`, and a `ŝ`
 * typed on a command line reached `args()` as `s`, another file's name.
 *
 * THE REPAIR IS ONE SETTING, AND IT TRAVELS IN THIS OBJECT. A manifest whose
 * `activeCodePage` is `UTF-8` makes the process's ANSI code page 65001, so
 * every narrow call in it reads UTF-8, a bound C library's own included. It is
 * carried here, as the resource section `cvtres` makes of a `.res`, so every
 * link that has the runtime has it: `heroes build`'s, the seed's, the CI's
 * and an `--emit-c` author's own line, none of which names it. Measured on the
 * box by panel 191's compiler-engineer, under lld-link 23.1.1 and link.exe
 * 14.44 alike; the XML handed to the linker instead was refused by link.exe,
 * `LNK1158: cannot run 'mt.exe'`.
 *
 * ITS PRICE, WRITTEN AS A CLAIM THAT CAN DIE: a program can link no other
 * resource. Both linkers refuse a second, by name (`LNK1241`, lld-link's
 * *more than one resource obj file*), never in silence, and `heroes build`
 * has no way to bring one today: a package's words are `-D -U -I -L -l -F`
 * (`selfhost/cli/libraries.hero`) and a `link` name is a library. The day a
 * program needs an icon, this object stops being the place.
 *
 * Elsewhere this file is empty: POSIX names are bytes, and no code page reads
 * them. */
#if defined(_WIN32)
#include <windows.h>
#include <wchar.h>

/* The XML, byte for byte panel 189's `utf8.manifest`, 368 bytes. In
 * `.rsrc$02`, the section a resource's bytes live in; `used` keeps a variable
 * nothing in C reads. */
__attribute__((section(".rsrc$02"), used)) static const char hero_codepage_manifest[] =
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<assembly manifestVersion=\"1.0\" xmlns=\"urn:schemas-microsoft-com:asm.v1\">\n"
    "  <application xmlns=\"urn:schemas-microsoft-com:asm.v3\">\n"
    "    <windowsSettings>\n"
    "      <activeCodePage xmlns=\"http://schemas.microsoft.com/SMI/2019/WindowsSettings\">UTF-8</activeCodePage>\n"
    "    </windowsSettings>\n"
    "  </application>\n"
    "</assembly>\n";

/* The tree that names those bytes, in `.rsrc$01`, the section the linker
 * places first and points the image's resource directory at: one type (24, a
 * manifest), one name (1, the process's own), one language (1033), one data
 * entry. A directory is 16 bytes and each of its entries 8; an offset with
 * its top bit set is a subdirectory's, counted from the section's start. The
 * data entry's first word is the bytes' address relative to the image, which
 * only the linker knows: `.rva` asks it for that relocation, the one C has no
 * way to write. */
__asm__(
    ".section .rsrc$01,\"dr\"\n"
    ".p2align 2\n"
    ".long 0, 0\n.short 0, 0, 0, 1\n.long 24, 0x80000018\n"
    ".long 0, 0\n.short 0, 0, 0, 1\n.long 1, 0x80000030\n"
    ".long 0, 0\n.short 0, 0, 0, 1\n.long 1033, 0x48\n"
    ".rva hero_codepage_manifest\n.long 368, 0, 0\n");

/* THE FLOOR, SAID RATHER THAN CROSSED (design.md §1.12: a guarantee that ends
 * quietly is not one). A Windows that does not honour `activeCodePage` answers
 * its own code page here, and every name above ASCII would then be read as
 * another in silence; so the program does not start, and says why in ASCII,
 * which every console code page shows. Called first by `hero_args_set`, the
 * call every generated `main` makes first, after the streams are set. Exit 2,
 * the contract's *could not run*: nothing of the program ran. */
static void hero_codepage_require_utf8(void) {
    UINT page = GetACP();
    if (page == CP_UTF8) return;
    fprintf(stderr,
            "error: this program reads every file name, argument and environment value as UTF-8, "
            "and this Windows reads them in code page %u: it does not honour the UTF-8 code page "
            "the program carries, so a name above ASCII would be read as another. Nothing was run.\n",
            (unsigned)page);
    exit(2);
}

/* THE DIRECTORY DOOR IS WIDE, AND THE CODE PAGE CANNOT MAKE IT SO (panel 191,
 * Q-g). A listing's narrow answer holds a name in `cFileName[MAX_PATH]`, 260
 * bytes, and a name of 100 CJK characters is 100 units on disk and 300 bytes of
 * UTF-8: under the UTF-8 code page `FindNextFileA` ENDED the listing there,
 * error 234, and every name after it went unseen (measured on the box). So a
 * listing asks `FindFirstFileW` and these two carry a name across. */

/* One character of a name at `*at`, its code point, and `*at` past it; -1
 * where the bytes begin none. The names are UTF-8, as a `str` is, or the
 * bytes `hero_win_name_bytes` below wrote for a name a listing carried back,
 * where a surrogate standing alone is the three bytes its value takes
 * (WTF-8): those three are taken back here and nowhere else (panel 191's R2).
 * Overlong forms and values past U+10FFFF begin none. */
static int32_t hero_win_char_at(const unsigned char *s, size_t *at) {
    unsigned char lead = s[*at];
    if (lead < 0x80) {
        *at += 1;
        return (int32_t)lead;
    }
    int length = lead >= 0xC2 && lead <= 0xDF ? 2 : lead >= 0xE0 && lead <= 0xEF ? 3 : lead >= 0xF0 && lead <= 0xF4 ? 4 : 0;
    if (length == 0) return -1;
    uint32_t c = (uint32_t)lead & (length == 2 ? 0x1Fu : length == 3 ? 0x0Fu : 0x07u);
    for (int k = 1; k < length; k += 1) {
        /* The name's own NUL is no continuation, so the read stops on it. */
        unsigned char next = s[*at + (size_t)k];
        if ((next & 0xC0u) != 0x80u) return -1;
        c = (c << 6) | ((uint32_t)next & 0x3Fu);
    }
    uint32_t least = length == 2 ? 0x80u : length == 3 ? 0x800u : 0x10000u;
    if (c < least || c > 0x10FFFFu) return -1;
    *at += (size_t)length;
    return (int32_t)c;
}

/* A name this runtime holds as the wide API takes it, with `suffix` after it
 * (the `\*` a listing asks for), in scratch memory the caller releases. NULL,
 * and ERROR_NO_UNICODE_TRANSLATION, where the bytes are neither UTF-8 nor a
 * listing's own: the door then fails, it never guesses. A surrogate pair
 * spelled as two three-byte halves is refused, being no listing's (one writes
 * a pair as four bytes), so a name read back is the name the listing saw.
 *
 * WHY NOT `MultiByteToWideChar`, which the route's first form called with
 * MB_ERR_INVALID_CHARS: it refuses the three bytes of a surrogate standing
 * alone, so a walk could name such a FILE and could not enter such a
 * DIRECTORY, and said *cannot read* of a readable folder (panel 191's critic,
 * measured on the box, red at `7f4c0cc5` and under the route alike). */
static wchar_t *hero_win_wide(const char *name, const wchar_t *suffix) {
    const unsigned char *s = (const unsigned char *)name;
    size_t units = 0;
    int after_high = 0;
    for (size_t at = 0; s[at] != 0;) {
        int32_t c = hero_win_char_at(s, &at);
        if (c < 0 || (after_high && c >= 0xDC00 && c <= 0xDFFF)) {
            SetLastError(ERROR_NO_UNICODE_TRANSLATION);
            return NULL;
        }
        after_high = c >= 0xD800 && c <= 0xDBFF;
        units += c >= 0x10000 ? 2u : 1u;
    }
    size_t extra = suffix != NULL ? wcslen(suffix) : 0;
    wchar_t *out = hero_alloc((units + extra + 1) * sizeof *out);
    size_t put = 0;
    for (size_t at = 0; s[at] != 0;) {
        uint32_t c = (uint32_t)hero_win_char_at(s, &at);
        if (c >= 0x10000u) {
            out[put++] = (wchar_t)(0xD800u + ((c - 0x10000u) >> 10));
            out[put++] = (wchar_t)(0xDC00u + ((c - 0x10000u) & 0x3FFu));
        } else {
            out[put++] = (wchar_t)c;
        }
    }
    for (size_t k = 0; k < extra; k += 1) out[put++] = suffix[k];
    out[put] = 0;
    return out;
}

/* A wide name as bytes: UTF-8, and a surrogate standing alone, which no UTF-8
 * can carry, as the three bytes its value would take, so the shown read names
 * it by its bytes (defect 239's `not UTF-8: rename it`) and never as another
 * name. The length, or -1 where `room` is short; 3 bytes a unit and one is
 * always enough. */
static int64_t hero_win_name_bytes(const wchar_t *wide, char *out, size_t room) {
    size_t at = 0;
    for (size_t i = 0; wide[i] != 0; i += 1) {
        uint32_t c = (uint32_t)wide[i];
        if (c >= 0xD800 && c <= 0xDBFF && wide[i + 1] >= 0xDC00 && wide[i + 1] <= 0xDFFF) {
            c = 0x10000 + ((c - 0xD800) << 10) + ((uint32_t)wide[i + 1] - 0xDC00);
            i += 1;
        }
        if (at + 5 > room) return -1;
        if (c < 0x80) {
            out[at++] = (char)c;
        } else if (c < 0x800) {
            out[at++] = (char)(0xC0 | (c >> 6));
            out[at++] = (char)(0x80 | (c & 0x3F));
        } else if (c < 0x10000) {
            out[at++] = (char)(0xE0 | (c >> 12));
            out[at++] = (char)(0x80 | ((c >> 6) & 0x3F));
            out[at++] = (char)(0x80 | (c & 0x3F));
        } else {
            out[at++] = (char)(0xF0 | (c >> 18));
            out[at++] = (char)(0x80 | ((c >> 12) & 0x3F));
            out[at++] = (char)(0x80 | ((c >> 6) & 0x3F));
            out[at++] = (char)(0x80 | (c & 0x3F));
        }
    }
    out[at] = '\0';
    return (int64_t)at;
}

/* A wide answer a door RETURNS, a link's target, the module's own path, an
 * environment value, as `hero_win_name_bytes` writes it, in scratch memory the
 * caller releases, its length in `*length`. The narrow doors these replace
 * answered a surrogate standing alone as U+FFFD under the UTF-8 code page,
 * valid text naming another file, where these bytes are not UTF-8 and the
 * shown read names them. */
static char *hero_win_bytes(const wchar_t *wide, int64_t *length) {
    size_t units = wcslen(wide);
    if (units > (SIZE_MAX - 5) / 3) hero_panic("a name longer than memory");
    size_t room = 3 * units + 5;
    char *out = hero_alloc(room);
    *length = hero_win_name_bytes(wide, out, room);
    return out;
}
#else
static void hero_codepage_require_utf8(void) {}
#endif
