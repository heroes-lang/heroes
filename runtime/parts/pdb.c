/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/pdb.c — a Windows program's functions, lines and inlined calls, read
 * from the PDB its link wrote beside it (panel 253's R5; design.md §1.12).
 *
 * WHY A READER OF ITS OWN. A PE executable holds no table of its functions'
 * names: they are in the PDB, and the system's reader of it is DbgHelp. An
 * abort tells the calls that led to it (`parts/calls.c`), and on Windows its
 * walk names each frame from here, never from DbgHelp: loading `dbghelp.dll`
 * at an abort takes the loader's lock, which a thread inside a DLL's
 * constructor holds for ever, the shape defect 2622 measured on Linux for
 * `dladdr`. Nothing here asks the loader: the file is read by the caller and
 * handed over as bytes, and every read below is a copy out of those bytes.
 *
 * And the program's parent, `heroes run` and `heroes test`, turns an abort's
 * addresses into `.hero` lines with it (`parts/pdb_door.c`), because
 * `llvm-symbolizer`, the one reader the box has, reads a Heroes function's
 * lines wrongly: measured on the box (clang and LLVM 23.1.1), the call of
 * `outer` to `middle` at `-O0`, which the PDB's own table puts on `.hero` line
 * 18 (`llvm-pdbutil dump --l`: `18 0001E0BB`, the generated C's block
 * starting at `1E0D4`), was answered as the generated C's line 42 for every
 * address from `1E0BB` to `1E0D4` (defect 509's measurement, again); `lldb`
 * does not start there (it asks for `python3.dll`), and DIA's reader is not
 * registered. So the line tables are read here, as the format says.
 *
 * WHAT IT READS, all little-endian (LLVM's documentation of the formats,
 * the `.rst` pages of `llvm/docs/PDB/`, and the records `llvm-pdbutil dump` prints):
 *   - the MSF container: a superblock, a directory of streams, each stream a
 *     list of blocks anywhere in the file;
 *   - stream 1, the PDB's identity (its GUID, held to the one the executable
 *     records, so a PDB of another link is never read as this one's) and its
 *     named streams, `/names` among them, the file names' strings;
 *   - stream 3, the DBI stream: its modules, and which module each piece of a
 *     section came from;
 *   - a module's own stream: its procedures (`S_GPROC32`, `S_LPROC32` and
 *     their `_ID` forms), their inlined calls (`S_INLINESITE`, whose binary
 *     annotations give the inlined lines), and its line tables
 *     (`DEBUG_S_LINES`, `DEBUG_S_FILECHKSMS`, `DEBUG_S_INLINEELINES`);
 *   - stream 4, the IPI stream, where an inlined function's name is.
 *
 * WHAT IT TRUSTS. Nothing: every offset, count and size read from the file is
 * checked against what holds it before it is followed, and a file that is
 * not what it claims gives no answer rather than a wild read. Every loop is
 * bounded by the bytes it walks.
 *
 * COMPILED ON WINDOWS ALONE, where a build writes a PE binary and its PDB:
 * elsewhere no program reads one, and every program would carry the reader
 * for nothing (10,864 bytes more `__text` in the runtime's object at `-O2`
 * on this Mac). The C is portable and asks nothing of the platform, so it
 * was measured on this Mac too, over the box's own PDBs. */

#if defined(_WIN32)

#include <stdint.h>
#include <string.h>

/* The MSF superblock's first 32 bytes (LLVM's `MSF.rst`). */
static const unsigned char hero_pdb_magic[32] = {'M', 'i', 'c', 'r', 'o', 's', 'o', 'f', 't', ' ', 'C',
                                                 '/', 'C', '+', '+', ' ', 'M', 'S', 'F', ' ', '7', '.',
                                                 '0', '0', '\r', '\n', 0x1a, 'D', 'S', 0, 0, 0};

/* The file, whole, in the caller's memory, and its container read. */
typedef struct {
    const unsigned char *bytes;
    size_t size;
    uint32_t block;      /* bytes a block */
    uint32_t blocks;     /* blocks the superblock says the file holds */
    uint32_t dir_size;   /* the directory's bytes */
    uint32_t dir_blocks; /* the blocks it takes */
    size_t dir_map;      /* where in the file its block numbers stand */
    uint32_t streams;    /* how many streams the directory lists */
} HeroPdb;

/* One stream: its size, and where its block numbers stand in the directory
 * (`list`), or the directory itself (`is_dir`). */
typedef struct {
    const HeroPdb *pdb;
    uint32_t size;
    uint32_t list;
    int is_dir;
} HeroPdbStream;

static uint16_t hero_pdb_le16(const unsigned char *p) { return (uint16_t)(p[0] | (p[1] << 8)); }

static uint32_t hero_pdb_le32(const unsigned char *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static uint64_t hero_pdb_le64(const unsigned char *p) {
    return (uint64_t)hero_pdb_le32(p) | ((uint64_t)hero_pdb_le32(p + 4) << 32);
}

static int hero_pdb_read(const HeroPdbStream *s, uint32_t offset, void *out, uint32_t n);

/* The file's number of the `k`th block of `s`, or 0 where there is none. */
static int hero_pdb_block_of(const HeroPdbStream *s, uint32_t k, uint32_t *number) {
    const HeroPdb *p = s->pdb;
    if (s->is_dir) {
        if (k >= p->dir_blocks) return 0;
        *number = hero_pdb_le32(p->bytes + p->dir_map + 4 * (size_t)k);
        return 1;
    }
    HeroPdbStream dir = {p, p->dir_size, 0, 1};
    unsigned char raw[4];
    if (k > (UINT32_MAX - s->list) / 4 || !hero_pdb_read(&dir, s->list + 4 * k, raw, 4)) return 0;
    *number = hero_pdb_le32(raw);
    return 1;
}

/* `n` bytes of `s` from `offset` into `out`, or 0 where any of them is not
 * in the stream or not in the file. */
static int hero_pdb_read(const HeroPdbStream *s, uint32_t offset, void *out, uint32_t n) {
    const HeroPdb *p = s->pdb;
    if (offset > s->size || n > s->size - offset) return 0;
    unsigned char *to = (unsigned char *)out;
    while (n > 0) {
        uint32_t k = offset / p->block;
        uint32_t within = offset % p->block;
        uint32_t take = p->block - within;
        if (take > n) take = n;
        uint32_t number = 0;
        if (!hero_pdb_block_of(s, k, &number) || number >= p->blocks) return 0;
        uint64_t at = (uint64_t)number * p->block + within;
        if (at > p->size || take > p->size - at) return 0;
        memcpy(to, p->bytes + at, take);
        to += take;
        offset += take;
        n -= take;
    }
    return 1;
}

static int hero_pdb_u16(const HeroPdbStream *s, uint32_t offset, uint16_t *v) {
    unsigned char raw[2];
    if (!hero_pdb_read(s, offset, raw, 2)) return 0;
    *v = hero_pdb_le16(raw);
    return 1;
}

static int hero_pdb_u32(const HeroPdbStream *s, uint32_t offset, uint32_t *v) {
    unsigned char raw[4];
    if (!hero_pdb_read(s, offset, raw, 4)) return 0;
    *v = hero_pdb_le32(raw);
    return 1;
}

/* A string of `s` at `offset`, ended by its NUL inside the stream and before
 * `end`, into `out` of `room` bytes (cut where it ends, always ended): its
 * length, or -1 where no NUL ends it. */
static int64_t hero_pdb_string(const HeroPdbStream *s, uint32_t offset, uint32_t end, char *out, size_t room) {
    if (end > s->size) end = s->size;
    size_t put = 0;
    for (uint32_t at = offset; at < end; at++) {
        unsigned char c;
        if (!hero_pdb_read(s, at, &c, 1)) return -1;
        if (c == 0) {
            if (room > 0) out[put < room ? put : room - 1] = '\0';
            return (int64_t)(at - offset);
        }
        if (put + 1 < room) out[put++] = (char)c;
    }
    return -1;
}

/* The container of `bytes`, or 0 where it is not an MSF 7.00 file. */
static int hero_pdb_open(HeroPdb *p, const unsigned char *bytes, size_t size) {
    memset(p, 0, sizeof *p);
    if (size < 56 || memcmp(bytes, hero_pdb_magic, sizeof hero_pdb_magic) != 0) return 0;
    p->bytes = bytes;
    p->size = size;
    p->block = hero_pdb_le32(bytes + 32);
    p->blocks = hero_pdb_le32(bytes + 40);
    p->dir_size = hero_pdb_le32(bytes + 44);
    uint32_t map_block = hero_pdb_le32(bytes + 52);
    if (p->block != 512 && p->block != 1024 && p->block != 2048 && p->block != 4096 && p->block != 8192 &&
        p->block != 16384 && p->block != 32768)
        return 0;
    if ((uint64_t)p->blocks * p->block > size || map_block >= p->blocks) return 0;
    p->dir_blocks = (uint32_t)(((uint64_t)p->dir_size + p->block - 1) / p->block);
    if (p->dir_blocks == 0 || p->dir_blocks > p->block / 4) return 0;
    p->dir_map = (size_t)map_block * p->block;
    HeroPdbStream dir = {p, p->dir_size, 0, 1};
    if (!hero_pdb_u32(&dir, 0, &p->streams)) return 0;
    if (p->streams > (p->dir_size - 4) / 4) return 0;
    return 1;
}

/* Stream `index`, or 0 where the directory has none (a nil stream is one of
 * no bytes). */
static int hero_pdb_stream(const HeroPdb *p, uint32_t index, HeroPdbStream *out) {
    if (index >= p->streams) return 0;
    HeroPdbStream dir = {p, p->dir_size, 0, 1};
    uint64_t list = 4 + 4 * (uint64_t)p->streams;
    uint32_t size = 0;
    for (uint32_t i = 0; i <= index; i++) {
        if (!hero_pdb_u32(&dir, 4 + 4 * i, &size)) return 0;
        if (size == UINT32_MAX) size = 0;
        if (i == index) break;
        list += 4 * (((uint64_t)size + p->block - 1) / p->block);
        if (list > p->dir_size) return 0;
    }
    uint64_t need = 4 * (((uint64_t)size + p->block - 1) / p->block);
    if (list > p->dir_size || need > p->dir_size - list) return 0;
    out->pdb = p;
    out->size = size;
    out->list = (uint32_t)list;
    out->is_dir = 0;
    return 1;
}

/* The PDB's GUID (stream 1, after its version, signature and age). */
static int hero_pdb_guid(const HeroPdb *p, unsigned char guid[16]) {
    HeroPdbStream info;
    return hero_pdb_stream(p, 1, &info) && hero_pdb_read(&info, 12, guid, 16);
}

/* THE DBI STREAM'S PIECES: where its module records and its section
 * contributions stand (LLVM's `DbiStream.rst`: a 64-byte header, then the
 * module records, then the contributions). */
typedef struct {
    HeroPdbStream dbi;
    uint32_t modules_at, modules_size;
    uint32_t contributions_at, contributions_size;
} HeroPdbDbi;

static int hero_pdb_dbi(const HeroPdb *p, HeroPdbDbi *d) {
    if (!hero_pdb_stream(p, 3, &d->dbi)) return 0;
    unsigned char h[64];
    if (!hero_pdb_read(&d->dbi, 0, h, sizeof h)) return 0;
    if (hero_pdb_le32(h) != UINT32_MAX) return 0;
    d->modules_at = 64;
    d->modules_size = hero_pdb_le32(h + 24);
    d->contributions_size = hero_pdb_le32(h + 28);
    if (d->modules_size > d->dbi.size - 64) return 0;
    d->contributions_at = 64 + d->modules_size;
    if (d->contributions_size > d->dbi.size - d->contributions_at) return 0;
    return 1;
}

/* The module that contributed the byte at `off` of section `seg`, or 0. */
static int hero_pdb_module_of(const HeroPdbDbi *d, uint16_t seg, uint32_t off, uint16_t *module) {
    uint32_t version = 0;
    if (d->contributions_size < 4 || !hero_pdb_u32(&d->dbi, d->contributions_at, &version)) return 0;
    uint32_t entry = 0;
    if (version == 0xeffe0000u + 19970605u) {
        entry = 28;
    } else if (version == 0xeffe0000u + 20140516u) {
        entry = 32;
    } else {
        return 0;
    }
    uint32_t count = (d->contributions_size - 4) / entry;
    for (uint32_t i = 0; i < count; i++) {
        unsigned char c[28];
        if (!hero_pdb_read(&d->dbi, d->contributions_at + 4 + i * entry, c, sizeof c)) return 0;
        uint16_t section = hero_pdb_le16(c);
        uint32_t start = hero_pdb_le32(c + 4);
        uint32_t size = hero_pdb_le32(c + 8);
        if (section == seg && off >= start && off - start < size) {
            *module = hero_pdb_le16(c + 16);
            return 1;
        }
    }
    return 0;
}

/* A module's own stream and the extent of its symbols and its C13 lines. */
typedef struct {
    HeroPdbStream stream;
    uint32_t symbols_end; /* symbols run from 4 to here */
    uint32_t lines_at, lines_size;
} HeroPdbModule;

static int hero_pdb_module(const HeroPdb *p, const HeroPdbDbi *d, uint16_t wanted, HeroPdbModule *m) {
    uint32_t at = d->modules_at;
    uint32_t end = d->modules_at + d->modules_size;
    for (uint32_t i = 0; at < end; i++) {
        unsigned char h[64];
        if (end - at < sizeof h || !hero_pdb_read(&d->dbi, at, h, sizeof h)) return 0;
        uint32_t next = at + 64;
        /* Two names after the record, each ended by its NUL, then 4-aligned. */
        for (int name = 0; name < 2; name++) {
            char skip[1];
            int64_t len = hero_pdb_string(&d->dbi, next, end, skip, sizeof skip);
            if (len < 0) return 0;
            next += (uint32_t)len + 1;
        }
        next = (next + 3u) & ~3u;
        if (i == wanted) {
            uint16_t index = hero_pdb_le16(h + 34);
            uint32_t symbols = hero_pdb_le32(h + 36);
            uint32_t c11 = hero_pdb_le32(h + 40);
            uint32_t c13 = hero_pdb_le32(h + 44);
            if (index == UINT16_MAX || !hero_pdb_stream(p, index, &m->stream)) return 0;
            if (symbols < 4 || symbols > m->stream.size || c11 > m->stream.size - symbols ||
                c13 > m->stream.size - symbols - c11)
                return 0;
            m->symbols_end = symbols;
            m->lines_at = symbols + c11;
            m->lines_size = c13;
            return 1;
        }
        at = next;
    }
    return 0;
}

/* The symbol record kinds read here (LLVM's `CodeViewSymbols.def`). */
#define HERO_CV_S_END 0x0006u
#define HERO_CV_S_LPROC32 0x110Fu
#define HERO_CV_S_GPROC32 0x1110u
#define HERO_CV_S_LPROC32_ID 0x1146u
#define HERO_CV_S_GPROC32_ID 0x1147u
#define HERO_CV_S_INLINESITE 0x114Du
#define HERO_CV_S_PROC_ID_END 0x114Fu
#define HERO_CV_S_LPROC32_DPC 0x1155u
#define HERO_CV_S_LPROC32_DPC_ID 0x1156u
#define HERO_CV_S_INLINESITE2 0x115Du

static int hero_pdb_is_proc(uint16_t kind) {
    return kind == HERO_CV_S_LPROC32 || kind == HERO_CV_S_GPROC32 || kind == HERO_CV_S_LPROC32_ID ||
           kind == HERO_CV_S_GPROC32_ID || kind == HERO_CV_S_LPROC32_DPC || kind == HERO_CV_S_LPROC32_DPC_ID;
}

/* A procedure whose code holds an address: its record in its module's
 * stream, where its records end, and its code's section, start and size. */
typedef struct {
    HeroPdbModule module;
    uint32_t record, end;
    uint16_t seg;
    uint32_t start, size;
} HeroPdbProc;

/* The procedure whose code holds byte `off` of section `seg`, its name into
 * `name` (of `room` bytes, cut where it ends): 1, or 0 where none does. */
static int hero_pdb_proc_at(const HeroPdb *p, uint16_t seg, uint32_t off, HeroPdbProc *out, char *name, size_t room) {
    HeroPdbDbi d;
    uint16_t module = 0;
    if (!hero_pdb_dbi(p, &d) || !hero_pdb_module_of(&d, seg, off, &module)) return 0;
    if (!hero_pdb_module(p, &d, module, &out->module)) return 0;
    const HeroPdbStream *s = &out->module.stream;
    uint32_t at = 4;
    while (at < out->module.symbols_end && out->module.symbols_end - at >= 4) {
        uint16_t len = 0, kind = 0;
        if (!hero_pdb_u16(s, at, &len) || !hero_pdb_u16(s, at + 2, &kind) || len < 2) return 0;
        uint32_t next = at + 2 + len;
        if (next > out->module.symbols_end) return 0;
        if (hero_pdb_is_proc(kind) && len >= 2 + 37) {
            unsigned char r[39];
            if (!hero_pdb_read(s, at, r, sizeof r)) return 0;
            uint32_t size = hero_pdb_le32(r + 16);
            uint32_t start = hero_pdb_le32(r + 32);
            uint16_t section = hero_pdb_le16(r + 36);
            if (section == seg && off >= start && off - start < size) {
                if (hero_pdb_string(s, at + 39, next, name, room) < 0) return 0;
                out->record = at;
                out->end = hero_pdb_le32(r + 8);
                out->seg = section;
                out->start = start;
                out->size = size;
                return 1;
            }
        }
        at = next;
    }
    return 0;
}

/* THE LINE TABLES (LLVM's `ModiStream.rst`, the C13 subsections). */
#define HERO_CV_DEBUG_S_LINES 0xF2u
#define HERO_CV_DEBUG_S_FILECHKSMS 0xF4u
#define HERO_CV_DEBUG_S_INLINEELINES 0xF6u

/* The first subsection of `kind` in the module's C13 lines from `*cursor`,
 * its data's offset and size; `*cursor` moved past it. 0 where none is left. */
static int hero_pdb_subsection(const HeroPdbModule *m, uint32_t kind, uint32_t *cursor, uint32_t *data, uint32_t *size) {
    uint32_t end = m->lines_at + m->lines_size;
    uint32_t at = *cursor < m->lines_at ? m->lines_at : *cursor;
    while (at < end && end - at >= 8) {
        uint32_t k = 0, len = 0;
        if (!hero_pdb_u32(&m->stream, at, &k) || !hero_pdb_u32(&m->stream, at + 4, &len)) return 0;
        if (len > end - at - 8) return 0;
        uint32_t next = at + 8 + len;
        next = next > end - 3 ? end : (next + 3u) & ~3u;
        if ((k & 0x7FFFFFFFu) == kind) {
            *data = at + 8;
            *size = len;
            *cursor = next;
            return 1;
        }
        at = next;
    }
    return 0;
}

/* A line number a table may hold that names no line (`0xFEEFEE`, `0xF00F00`,
 * MSVC's hidden lines), or none at all. */
static int hero_pdb_line_named(uint32_t line) { return line != 0 && line != 0xFEEFEEu && line != 0xF00F00u; }

/* The line the procedure's own table gives byte `off` (of its section), and
 * the offset of its file's checksum record: 1, or 0 where no entry holds it.
 * The entry is the one of every block whose start is the greatest not past
 * `off`, whichever file's block it stands in. */
static int hero_pdb_proc_line(const HeroPdbProc *proc, uint32_t off, uint32_t *line, uint32_t *file) {
    const HeroPdbModule *m = &proc->module;
    uint32_t cursor = 0, data = 0, size = 0;
    while (hero_pdb_subsection(m, HERO_CV_DEBUG_S_LINES, &cursor, &data, &size)) {
        unsigned char h[12];
        if (size < sizeof h || !hero_pdb_read(&m->stream, data, h, sizeof h)) return 0;
        uint32_t reloc = hero_pdb_le32(h);
        uint16_t seg = hero_pdb_le16(h + 4);
        uint16_t flags = hero_pdb_le16(h + 6);
        uint32_t code = hero_pdb_le32(h + 8);
        if (seg != proc->seg || off < reloc || off - reloc >= code) continue;
        uint32_t want = off - reloc;
        int found = 0;
        uint32_t best = 0;
        uint32_t at = data + 12, end = data + size;
        while (at < end && end - at >= 12) {
            unsigned char b[12];
            if (!hero_pdb_read(&m->stream, at, b, sizeof b)) return 0;
            uint32_t name = hero_pdb_le32(b);
            uint32_t lines = hero_pdb_le32(b + 4);
            uint32_t bytes = hero_pdb_le32(b + 8);
            uint64_t need = 12 + (uint64_t)lines * ((flags & 1u) ? 12 : 8);
            if (bytes < need || bytes > end - at) return 0;
            for (uint32_t i = 0; i < lines; i++) {
                unsigned char e[8];
                if (!hero_pdb_read(&m->stream, at + 12 + 8 * i, e, sizeof e)) return 0;
                uint32_t start = hero_pdb_le32(e);
                if (start <= want && (!found || start >= best)) {
                    found = 1;
                    best = start;
                    *line = hero_pdb_le32(e + 4) & 0x00FFFFFFu;
                    *file = name;
                }
            }
            at += bytes;
        }
        return found;
    }
    return 0;
}

/* The name of the file whose checksum record stands at `file` in the
 * module's checksums, from `/names`, into `out`: 1, or 0. */
static int hero_pdb_file_name(const HeroPdb *p, const HeroPdbModule *m, uint32_t file, char *out, size_t room) {
    uint32_t cursor = 0, data = 0, size = 0;
    if (!hero_pdb_subsection(m, HERO_CV_DEBUG_S_FILECHKSMS, &cursor, &data, &size)) return 0;
    uint32_t name = 0;
    if (file > size || size - file < 6 || !hero_pdb_u32(&m->stream, data + file, &name)) return 0;
    /* `/names`, by the PDB's table of named streams (stream 1, after its
     * 28-byte head: the names' bytes, then a hash table of present slots). */
    HeroPdbStream info;
    uint32_t bytes = 0;
    if (!hero_pdb_stream(p, 1, &info) || !hero_pdb_u32(&info, 28, &bytes) || bytes > info.size - 32) return 0;
    uint32_t table = 32 + bytes;
    uint32_t present_words = 0, capacity = 0;
    if (!hero_pdb_u32(&info, table + 4, &capacity) || !hero_pdb_u32(&info, table + 8, &present_words)) return 0;
    if (present_words > info.size / 4) return 0;
    uint32_t deleted_at = table + 12 + 4 * present_words;
    uint32_t deleted_words = 0;
    if (!hero_pdb_u32(&info, deleted_at, &deleted_words) || deleted_words > info.size / 4) return 0;
    uint32_t pairs = deleted_at + 4 + 4 * deleted_words;
    uint32_t names_stream = UINT32_MAX;
    uint32_t kept = 0;
    for (uint32_t slot = 0; slot < capacity && slot / 32 < present_words; slot++) {
        uint32_t word = 0;
        if (!hero_pdb_u32(&info, table + 12 + 4 * (slot / 32), &word)) return 0;
        if ((word & (1u << (slot % 32))) == 0) continue;
        uint32_t key = 0, value = 0;
        if (!hero_pdb_u32(&info, pairs + 8 * kept, &key) || !hero_pdb_u32(&info, pairs + 8 * kept + 4, &value)) return 0;
        kept++;
        char text[8];
        if (key < bytes && hero_pdb_string(&info, 32 + key, 32 + bytes, text, sizeof text) == 6 &&
            strcmp(text, "/names") == 0) {
            names_stream = value;
            break;
        }
    }
    HeroPdbStream names;
    uint32_t magic = 0, strings = 0;
    if (names_stream == UINT32_MAX || !hero_pdb_stream(p, names_stream, &names)) return 0;
    if (!hero_pdb_u32(&names, 0, &magic) || magic != 0xEFFEEFFEu || !hero_pdb_u32(&names, 8, &strings)) return 0;
    if (strings > names.size - 12 || name >= strings) return 0;
    return hero_pdb_string(&names, 12 + name, 12 + strings, out, room) >= 0;
}

/* THE INLINED CALLS. A compressed number of a binary annotation (LLVM's
 * `CVUncompressData`), from `*at` below `end`: 1, or 0 where it is cut. */
static int hero_pdb_compressed(const unsigned char *a, size_t end, size_t *at, uint32_t *v) {
    if (*at >= end) return 0;
    unsigned char b0 = a[*at];
    if ((b0 & 0x80u) == 0) {
        *v = b0;
        *at += 1;
        return 1;
    }
    if ((b0 & 0xC0u) == 0x80u) {
        if (end - *at < 2) return 0;
        *v = ((uint32_t)(b0 & 0x3Fu) << 8) | a[*at + 1];
        *at += 2;
        return 1;
    }
    if ((b0 & 0xE0u) == 0xC0u) {
        if (end - *at < 4) return 0;
        *v = ((uint32_t)(b0 & 0x1Fu) << 24) | ((uint32_t)a[*at + 1] << 16) | ((uint32_t)a[*at + 2] << 8) | a[*at + 3];
        *at += 4;
        return 1;
    }
    return 0;
}

static int32_t hero_pdb_signed(uint32_t v) { return (v & 1u) ? -(int32_t)(v >> 1) : (int32_t)(v >> 1); }

/* The line an inlined call's annotations give the byte `want` bytes past its
 * procedure's start, and its file's checksum record, from the inlined
 * function's first line and file: 1, or 0 where no range of the call holds
 * it. Read as LLVM writes them (`MCCodeView::encodeInlineLineTable`): a code
 * offset opens a range at the cursor moved by it, with the line and the file
 * as they stand, and ends the range open before it there; a code length ends
 * the open range that many bytes past its start. */
static int hero_pdb_site_line(const unsigned char *a, size_t n, uint32_t want, uint32_t first_line, uint32_t first_file,
                              uint32_t *line, uint32_t *file) {
    uint32_t cursor = 0, cur_line = first_line, cur_file = first_file;
    int open = 0;
    uint32_t open_at = 0, open_line = 0, open_file = 0;
    size_t at = 0;
#define HERO_PDB_CLOSE(stop)                                                                                         \
    do {                                                                                                              \
        if (open && want >= open_at && want < (stop)) {                                                               \
            *line = open_line;                                                                                        \
            *file = open_file;                                                                                        \
            return 1;                                                                                                 \
        }                                                                                                             \
        open = 0;                                                                                                     \
    } while (0)
#define HERO_PDB_OPEN()                                                                                               \
    do {                                                                                                              \
        open = 1;                                                                                                     \
        open_at = cursor;                                                                                             \
        open_line = cur_line;                                                                                         \
        open_file = cur_file;                                                                                         \
    } while (0)
    while (at < n) {
        uint32_t op = 0, u = 0, w = 0;
        if (!hero_pdb_compressed(a, n, &at, &op) || op == 0) break;
        switch (op) {
        case 1: /* CodeOffset: the cursor set */
            if (!hero_pdb_compressed(a, n, &at, &u)) return 0;
            HERO_PDB_CLOSE(u);
            cursor = u;
            HERO_PDB_OPEN();
            break;
        case 3: /* ChangeCodeOffset */
            if (!hero_pdb_compressed(a, n, &at, &u)) return 0;
            HERO_PDB_CLOSE(cursor + u);
            cursor += u;
            HERO_PDB_OPEN();
            break;
        case 4: /* ChangeCodeLength */
            if (!hero_pdb_compressed(a, n, &at, &u)) return 0;
            if (open) {
                HERO_PDB_CLOSE(open_at + u);
                cursor = open_at + u;
            } else {
                cursor += u;
            }
            break;
        case 5: /* ChangeFile */
            if (!hero_pdb_compressed(a, n, &at, &u)) return 0;
            cur_file = u;
            break;
        case 6: /* ChangeLineOffset */
            if (!hero_pdb_compressed(a, n, &at, &u)) return 0;
            cur_line = (uint32_t)((int64_t)cur_line + hero_pdb_signed(u));
            break;
        case 11: /* ChangeCodeOffsetAndLineOffset: four bits of code, the rest the line */
            if (!hero_pdb_compressed(a, n, &at, &u)) return 0;
            cur_line = (uint32_t)((int64_t)cur_line + hero_pdb_signed(u >> 4));
            HERO_PDB_CLOSE(cursor + (u & 0xFu));
            cursor += u & 0xFu;
            HERO_PDB_OPEN();
            break;
        case 12: /* ChangeCodeLengthAndCodeOffset */
            if (!hero_pdb_compressed(a, n, &at, &u) || !hero_pdb_compressed(a, n, &at, &w)) return 0;
            if (open) {
                HERO_PDB_CLOSE(open_at + u);
                cursor = open_at + u;
            }
            cursor += w;
            HERO_PDB_OPEN();
            break;
        case 2:  /* ChangeCodeOffsetBase */
        case 7:  /* ChangeLineEndDelta */
        case 8:  /* ChangeRangeKind */
        case 9:  /* ChangeColumnStart */
        case 10: /* ChangeColumnEndDelta */
        case 13: /* ChangeColumnEnd */
            if (!hero_pdb_compressed(a, n, &at, &u)) return 0;
            break;
        default:
            return 0;
        }
    }
#undef HERO_PDB_CLOSE
#undef HERO_PDB_OPEN
    return 0;
}

/* The first line and file of inlined function `inlinee`, from the module's
 * `DEBUG_S_INLINEELINES`: 1, or 0. */
static int hero_pdb_inlinee_start(const HeroPdbModule *m, uint32_t inlinee, uint32_t *line, uint32_t *file) {
    uint32_t cursor = 0, data = 0, size = 0;
    while (hero_pdb_subsection(m, HERO_CV_DEBUG_S_INLINEELINES, &cursor, &data, &size)) {
        uint32_t signature = 0;
        if (size < 4 || !hero_pdb_u32(&m->stream, data, &signature)) return 0;
        uint32_t at = data + 4, end = data + size;
        while (at < end && end - at >= 12) {
            unsigned char e[12];
            if (!hero_pdb_read(&m->stream, at, e, sizeof e)) return 0;
            uint32_t next = at + 12;
            if (signature == 1) {
                uint32_t extra = 0;
                if (end - next < 4 || !hero_pdb_u32(&m->stream, next, &extra) || extra > (end - next - 4) / 4) return 0;
                next += 4 + 4 * extra;
            }
            if (hero_pdb_le32(e) == inlinee) {
                *file = hero_pdb_le32(e + 4);
                *line = hero_pdb_le32(e + 8);
                return 1;
            }
            at = next;
        }
    }
    return 0;
}

/* The name of item `id` of the IPI stream, an `LF_FUNC_ID` or an
 * `LF_MFUNC_ID`, into `out`: 1, or 0. */
static int hero_pdb_item_name(const HeroPdb *p, uint32_t id, char *out, size_t room) {
    HeroPdbStream ipi;
    unsigned char h[20];
    if (!hero_pdb_stream(p, 4, &ipi) || !hero_pdb_read(&ipi, 0, h, sizeof h)) return 0;
    uint32_t header = hero_pdb_le32(h + 4);
    uint32_t first = hero_pdb_le32(h + 8);
    uint32_t last = hero_pdb_le32(h + 12);
    if (id < first || id >= last || header > ipi.size) return 0;
    uint32_t at = header;
    for (uint32_t index = first; index < id; index++) {
        uint16_t len = 0;
        if (!hero_pdb_u16(&ipi, at, &len) || len < 2) return 0;
        at += 2u + len;
    }
    uint16_t len = 0, kind = 0;
    if (!hero_pdb_u16(&ipi, at, &len) || !hero_pdb_u16(&ipi, at + 2, &kind) || len < 10) return 0;
    if (kind != 0x1601u && kind != 0x1602u) return 0;
    return hero_pdb_string(&ipi, at + 12, at + 2 + len, out, room) >= 0;
}

/* AN ADDRESS'S LEVELS: the calls inlined at byte `off` of section `seg`,
 * innermost first, each its function, its file's checksum record and its
 * line, and last the procedure itself, at the line its own table gives (the
 * line of the outermost inlined call, which LLVM writes there for every
 * inlined byte). */
#define HERO_PDB_LEVELS 16
#define HERO_PDB_NAME 512

typedef struct {
    char function[HERO_PDB_NAME];
    uint32_t file;  /* the checksum record of its file */
    uint32_t line;  /* 0 where none is known */
} HeroPdbLevel;

typedef struct {
    HeroPdbProc proc;
    HeroPdbLevel levels[HERO_PDB_LEVELS];
    int count;
} HeroPdbPlace;

static int hero_pdb_place(const HeroPdb *p, uint16_t seg, uint32_t off, HeroPdbPlace *out) {
    HeroPdbLevel proc_level;
    memset(out, 0, sizeof *out);
    if (!hero_pdb_proc_at(p, seg, off, &out->proc, proc_level.function, sizeof proc_level.function)) return 0;
    if (!hero_pdb_proc_line(&out->proc, off, &proc_level.line, &proc_level.file)) proc_level.line = 0;
    const HeroPdbStream *s = &out->proc.module.stream;
    uint32_t want = off - out->proc.start;
    /* The inlined calls holding the byte, outermost first: a call's own
     * records lie between it and its end, its inlined calls among them. */
    HeroPdbLevel chain[HERO_PDB_LEVELS];
    int depth = 0;
    uint16_t first_len = 0;
    if (!hero_pdb_u16(s, out->proc.record, &first_len)) return 0;
    uint32_t at = out->proc.record + 2 + first_len;
    uint32_t end = out->proc.end;
    if (end > out->proc.module.symbols_end) end = out->proc.module.symbols_end;
    while (at < end && end - at >= 4) {
        uint16_t len = 0, kind = 0;
        if (!hero_pdb_u16(s, at, &len) || !hero_pdb_u16(s, at + 2, &kind) || len < 2) break;
        uint32_t next = at + 2 + len;
        if (next > end) break;
        if ((kind == HERO_CV_S_INLINESITE || kind == HERO_CV_S_INLINESITE2) && len >= 14) {
            unsigned char head[16];
            uint32_t annotations = kind == HERO_CV_S_INLINESITE ? 16 : 20;
            if (len + 2u < annotations || !hero_pdb_read(s, at, head, sizeof head)) break;
            uint32_t site_end = hero_pdb_le32(head + 8);
            uint32_t inlinee = hero_pdb_le32(head + 12);
            unsigned char a[0x10000];
            uint32_t n = len + 2u - annotations;
            uint32_t first_line = 0, first_file = 0, line = 0, file = 0;
            int holds = n <= sizeof a && hero_pdb_read(s, at + annotations, a, n) &&
                        hero_pdb_inlinee_start(&out->proc.module, inlinee, &first_line, &first_file) &&
                        hero_pdb_site_line(a, n, want, first_line, first_file, &line, &file);
            if (holds && depth < HERO_PDB_LEVELS - 1) {
                HeroPdbLevel *l = &chain[depth++];
                if (!hero_pdb_item_name(p, inlinee, l->function, sizeof l->function)) l->function[0] = '\0';
                l->line = line;
                l->file = file;
                /* Its own records are the inner calls: read them, and none past its end. */
                if (site_end > at && site_end < end) end = site_end;
            } else if (site_end > at && site_end < end) {
                uint16_t end_len = 0;
                if (!hero_pdb_u16(s, site_end, &end_len)) break;
                next = site_end + 2 + end_len;
            }
        }
        at = next;
    }
    for (int i = depth - 1; i >= 0; i--) out->levels[out->count++] = chain[i];
    out->levels[out->count++] = proc_level;
    return 1;
}

/* THE EXECUTABLE'S SIDE: a PE32+ image's headers, as its file holds them
 * (`mapped` 0) or as the system mapped them (`mapped` 1, the bytes from the
 * image's base and an address an RVA), from Microsoft's PE format page:
 * the DOS header's `e_lfanew`, the COFF header, the optional header (its
 * `ImageBase`, `SizeOfImage` and the debug directory, the seventh), the
 * section table. */
typedef struct {
    const unsigned char *bytes;
    size_t size;
    int mapped;
    uint64_t image_base;
    uint32_t image_size;
    size_t sections;
    uint16_t section_count;
    uint32_t debug_rva, debug_size;
} HeroPe;

/* The headers of `bytes`, or 0 where they are not a PE32+ image's. Mapped,
 * `size` is what may be read before the headers say the image's size: the
 * headers' own page. */
static int hero_pe_open(HeroPe *pe, const unsigned char *bytes, size_t size, int mapped) {
    memset(pe, 0, sizeof *pe);
    if (size < 64 || bytes[0] != 'M' || bytes[1] != 'Z') return 0;
    uint32_t nt = hero_pdb_le32(bytes + 0x3C);
    if (nt > size || size - nt < 24 + 112 || memcmp(bytes + nt, "PE\0\0", 4) != 0) return 0;
    uint16_t count = hero_pdb_le16(bytes + nt + 6);
    uint16_t optional = hero_pdb_le16(bytes + nt + 20);
    const unsigned char *o = bytes + nt + 24;
    if (hero_pdb_le16(o) != 0x20Bu || optional < 112) return 0;
    size_t sections = (size_t)nt + 24 + optional;
    if (sections > size || (size - sections) / 40 < count) return 0;
    pe->bytes = bytes;
    pe->size = size;
    pe->mapped = mapped;
    pe->image_base = hero_pdb_le64(o + 24);
    pe->image_size = hero_pdb_le32(o + 56);
    pe->sections = sections;
    pe->section_count = count;
    if (hero_pdb_le32(o + 108) > 6 && optional >= 112 + 8 * 7) {
        pe->debug_rva = hero_pdb_le32(o + 112 + 48);
        pe->debug_size = hero_pdb_le32(o + 112 + 52);
    }
    if (mapped) {
        if (pe->image_size < size) return 0;
        pe->size = pe->image_size;
    }
    return 1;
}

/* The `n` bytes of the image at `rva`, or NULL where they are not there. */
static const unsigned char *hero_pe_at(const HeroPe *pe, uint32_t rva, uint32_t n) {
    if (pe->mapped) return rva <= pe->size && n <= pe->size - rva ? pe->bytes + rva : NULL;
    for (uint16_t i = 0; i < pe->section_count; i++) {
        const unsigned char *s = pe->bytes + pe->sections + 40 * (size_t)i;
        uint32_t va = hero_pdb_le32(s + 12);
        uint32_t raw = hero_pdb_le32(s + 16);
        uint32_t at = hero_pdb_le32(s + 20);
        if (rva < va || rva - va >= raw || n > raw - (rva - va)) continue;
        uint64_t file = (uint64_t)at + (rva - va);
        if (file > pe->size || n > pe->size - file) return NULL;
        return pe->bytes + file;
    }
    return NULL;
}

/* The section, numbered from 1 as a PDB numbers it, and the offset in it, of
 * `rva`: 1, or 0 where no section holds it. */
static int hero_pe_section_of(const HeroPe *pe, uint32_t rva, uint16_t *seg, uint32_t *off) {
    for (uint16_t i = 0; i < pe->section_count; i++) {
        const unsigned char *s = pe->bytes + pe->sections + 40 * (size_t)i;
        uint32_t span = hero_pdb_le32(s + 8);
        uint32_t va = hero_pdb_le32(s + 12);
        uint32_t raw = hero_pdb_le32(s + 16);
        if (raw > span) span = raw;
        if (rva >= va && rva - va < span) {
            *seg = (uint16_t)(i + 1);
            *off = rva - va;
            return 1;
        }
    }
    return 0;
}

/* The PDB the image names in its CodeView record (`RSDS`, its GUID, its age
 * and the path the link wrote, here the PDB's own last part, `/PDBALTPATH`,
 * `selfhost/cli/link_output.hero`): the GUID and the path into `path`, cut
 * where it ends: 1, or 0 where the image names none. */
static int hero_pe_pdb(const HeroPe *pe, unsigned char guid[16], char *path, size_t room) {
    const unsigned char *dir = hero_pe_at(pe, pe->debug_rva, pe->debug_size);
    if (dir == NULL || room == 0) return 0;
    for (uint32_t i = 0; i + 28 <= pe->debug_size; i += 28) {
        const unsigned char *e = dir + i;
        uint32_t size = hero_pdb_le32(e + 16);
        if (hero_pdb_le32(e + 12) != 2 || size < 25) continue;
        const unsigned char *d = NULL;
        if (pe->mapped) {
            d = hero_pe_at(pe, hero_pdb_le32(e + 20), size);
        } else {
            uint32_t file = hero_pdb_le32(e + 24);
            d = file <= pe->size && size <= pe->size - file ? pe->bytes + file : NULL;
        }
        if (d == NULL || memcmp(d, "RSDS", 4) != 0) continue;
        memcpy(guid, d + 4, 16);
        size_t put = 0;
        for (uint32_t at = 24; at < size && d[at] != 0 && put + 1 < room; at++) path[put++] = (char)d[at];
        path[put] = '\0';
        return put > 0;
    }
    return 0;
}

/* Where a path's last part begins, past Windows' two separators. */
static size_t hero_pdb_last_part(const char *path) {
    size_t at = strlen(path);
    while (at > 0 && path[at - 1] != '/' && path[at - 1] != '\\' && path[at - 1] != ':') at--;
    return at;
}

#endif
