# 037 — the give-away case was writable the moment the callback was

2026-09-20, M-declared-extents step 16, after defect 069's repair landed at
step 12.

Three sittings — 166, 167, 168 — asked **how Heroes can hand C bytes that
survive the call**, and each answered with a different pointer: a lend with a
lifetime rule, a lease that copies, a header at one end of the block or the
other. Panel 169's ffi-pragmatist reframed it: **the answer is that Heroes
should not.** The author allocates, C frees with the function the author names,
and nobody has to know how long.

That route is **R5**, and this record measures that it needs no new form: it was
blocked by defect 069 alone, and 069 was repaired six hours before this was run.

## The program, and it is nineteen lines with the C side

    extern "stdlib.h"
        function malloc(size: u64) -> ptr
        function free(p: ptr)

    extern "string.h"
        function memset(dest: ptr, c: i32, n: u64) -> ptr

    extern "r5.h"
        function lib_keep(p: ptr, n: i64, d: (function(ptr) -> ()))
        function lib_read() -> i64
        function lib_done()

    function main()
        b: ptr @ malloc(size: 8)
        _ = memset(dest: b, c: 72, n: 8)
        lib_keep(p: b, n: 8, d: free)
        print(to_str(lib_read()))
        lib_done()

The C side keeps the pointer and the disposer, reads the bytes later, and
disposes of them with the function it was handed — `sqlite3_bind_blob`'s shape
with the library written small enough to ship beside the case.

## The measurement

| | |
|---|---|
| `./heroes check` | **exit 0** |
| `./heroes run` | **exit 0**, prints `72` and then the line after the disposal |
| `./heroes build --sanitize`, AddressSanitizer lines | **0** |

## Why it survives what killed the other routes

Panel 168 killed the trailing header by replacing SQLite's allocator through
`sqlite3_config(SQLITE_CONFIG_MALLOC)` and measuring `134 134 134 133 133`. That
measurement does not reach this route, and the reason is one sentence: **the
author's destructor is paired with the author's allocator.** The buffer comes
from the `malloc` the program declared and goes to the `free` the program
declared, and no part of the exchange guesses which heap the other side uses.

## What it changes for defect 066, and what it does not

**It changes**: a program that needs C to keep bytes now has a way to be right,
and until 2026-09-20 it had none. Panel 167 measured that `.ptr()` is on no
closure list and that the compiler never lends a field; what the corpus needed
was not a better lend but a route that is not a lend.

**It does not change**: an unmarked parameter that retains a LEND is still a
silent wrong answer, because nothing in the declaration says it retains. That is
the half no ecosystem in panel 167's survey of ten enforces, and it is what
defect 066 still names.

## Unrun, and named

Whether this route survives on Linux and Windows. `.claude/rules/platforms.md`:
a platform fact run on one platform is an inference about the other two, and
this was run on Darwin arm64 only.
