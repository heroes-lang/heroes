# Panel 147 — ffi-pragmatist

- **verdict**: **veto** on Route A *as specified* (two clauses: the releaser is
  keyed on the TYPE, and the compiler refuses the author's own call).
  **approve** Route B. **Route C's falsifier already fired** — E1 below is the
  program the refusal would have to say is impossible, and it runs.
- **section**: design.md §1.11 ("FFI ergonomics rank alongside comprehension"),
  §4.19's ladder and its borrow finding, §1.12 (must not segfault, must not
  corrupt memory), design.md Part 6 borrow-checker row (*"no header-derived rule
  can separate a call that invalidates a handle from one that reads it"*).

Everything below was measured 2026-09-14 on this Mac, arm64, Apple clang
21.0.0, sqlite 3.51.0, in a frozen-tree copy at
`/private/tmp/.../scratchpad/panel-147/`. The seed built in **3.03 s**.

---

## 1. ABI: the veto is NOT triggered by either route, and here is the proof

Measured from the compiler's own output, not assumed —
`./heroes build examples/sqlite/main.hero --emit-c`:

```
HERO_TU_LOCAL bool h_main_Stmt_eq(sqlite3_stmt * const *a, sqlite3_stmt * const *b);
int64_t h_main_run(sqlite3 * h0_db, HeroStr h1_sql);
    sqlite3_stmt * h2_statement;
```

A Heroes handle is a **bare `sqlite3 *`** in a parameter, in a return and in a
slot. `e10_abi.c`:

```
sqlite3_stmt *   : 8 bytes
handle today     : 8 bytes
handle with flag : 16 bytes
```

Neither route needs to change that: both can null the slot after release and
keep the released-ness in the frame. **What WOULD trigger the veto** is a
"released" bit or a refcount inside the handle VALUE — `clang -std=c11 -Wall
-Wextra e10_abi.c -lsqlite3` then says
`error: passing 'HandleWithFlag' to parameter of incompatible type
'sqlite3_stmt *'`, which is a shim per binding. No seat has proposed that; I am
naming it so nobody proposes it later.

---

## 2. The number the sitting asked me for (brief item 2)

**Method**: clang parses the real header (`clang -fsyntax-only -Xclang
-ast-dump=json`), I walk the JSON AST and keep `FunctionDecl`s whose source file
belongs to the library. Arity and pointer depth come from clang; only the
*name* vocabulary is mine (`free|destroy|close|release|finali[sz]e|cleanup|
delete|dispose|unref|unload|deinit|_done|done_|_end$|discard`). Scripts:
`ffi/survey.py`, `ffi/handles.py`, `ffi/returners.py`.

### 2a. Function level — 21 libraries, 13 596 declared functions read

| shape | count | share of the 1 073 release-named functions |
|---|---|---|
| **exactly one pointer, nothing else** | **596** | **55.5%** |
| … of which return `void` | 493 | |
| … of which return a signed int | 97 | |
| … of which return `size_t` (all 6 are zstd, **unbindable today**, §4.19/panel 041) | 6 | |
| two or more arguments | 161 | 15.0% |
| one **pointer-to-pointer** (a slot address, not a handle) | 246 | 22.9% |
| no pointer at all (a global, or a struct by value) | 70 | 6.5% |

So **44.5% of real release functions do not have Route A's shape.**

### 2b. Type level — the unit Route A actually binds

Route A writes `record X tag Y released F`, so the question is per opaque handle
type, not per function. Of **262** opaque handle types that carry a release
obligation across 16 libraries:

| | count | share |
|---|---|---|
| exactly one single-argument releaser — **Route A fits** | **212** | **80.9%** |
| more than one candidate (author picks; some are name noise, e.g. `cairo_close_path`) | 29 | 11.1% |
| **only a multi-argument releaser — Route A cannot express it** | 21 | 8.0% |

The 21 inexpressible ones: SDL3's eight GPU resource types plus `SDL_AsyncIO`
(`SDL_ReleaseGPUTexture(device, texture)` — the resource cannot be released
without its device), libuv 5, OpenSSL 7.

**Two corrections to my own table, found by hand after running it** (CLAUDE.md
§ RUN IT). My type collector misses a typedef that swallows the star, so zlib,
freetype, libjpeg and most of libxml2 are undercounted. Direction of the error:
it undercounts both buckets, so 80.9% is if anything conservative in Route A's
favour — **except** that zlib's missed type is a counterexample:
`struct gzFile_s *` has **three** one-argument releasers, `gzclose`,
`gzclose_r`, `gzclose_w`, and **which one is correct depends on how the file was
opened**, which the type cannot know.

### 2c. Two libraries on §1.11's own table where Route A reaches ZERO

- **raylib** (`/opt/homebrew/include/raylib.h`, read in full for this):
  **0 of 21** release obligations have Route A's shape. Fifteen take a struct
  **by value** (`UnloadTexture(Texture2D)`, `UnloadFont(Font)`,
  `UnloadModel(Model)`), two take **nothing** (`CloseWindow(void)`,
  `CloseAudioDevice(void)`), four free raw memory blocks. raylib has exactly two
  opaque pointer types and neither has a releaser.
- **libpng**: `png_destroy_read_struct(png_structpp, png_infopp, png_infopp)` —
  three pointers-to-handle. Route A cannot name it. **Route B can, and I
  compiled it in Heroes**, see §5.

---

## 3. THE ARGUMENT: a handle type does not determine ownership

This is why I veto Route A rather than merely object.

`ffi/returners.py`, clang-parsed, real headers:

| type | functions returning it | at least one is a **borrow** |
|---|---|---|
| `sqlite3 *` | 2 | **both**: `sqlite3_db_handle`, `sqlite3_context_db_handle` |
| `sqlite3_stmt *` | 1 | **it is**: `sqlite3_next_stmt` |
| `sqlite3_value *` | 2 | `sqlite3_column_value` borrowed, `sqlite3_value_dup` owned |
| `cairo_surface_t *` | 13 | `cairo_get_target`, `cairo_get_group_target` |
| `cairo_pattern_t *` | 12 | `cairo_get_source` |
| `hb_face_t *` | 10 | `hb_font_get_face`; `hb_face_get_empty` is immortal |
| `hb_font_t *` | 5 | `hb_font_get_parent` |
| `xmlDocPtr` | 25 | several |

design.md's Part 6 row already says this at the *acquisition* end (panel 139:
`sqlite3_column_count` is not const-qualified, so a mutating step and a pure
read have the identical signature). **Route A is the same rule at the release
end**: a `released` mark on the type applies to every value of that type
whatever call produced it.

### The binding that proves it is writable TODAY

`tree/e6_borrow.hero`, one added line against the shipped SQLite group:

```
    function sqlite3_db_handle(statement: CStmt) -> CDb
```

`./heroes run e6_borrow.hero`:

```
borrowed == owned: true
count: 2
```

`true` is only observable because defect 032 was fixed this morning. Now the C
Route A would emit for that scope, `ffi/e7_double_close.c`:

```
clang -std=c11 -Wall -Wextra e7_double_close.c -lsqlite3 -o e7_double_close
```
compiled clean, and ran:

```
borrowed == owned : true
count             : 2
sweep closes borrowed -> 0
sweep closes db       -> 21          # SQLITE_MISUSE
reached the end
exit=0
```

And `ffi/e3_routeA_borrow.c`, the sweep inside a function that only borrowed:

```
path  =
step  = 100 (SQLITE_ROW=100)
value = 0            # the table holds 3 rows
exit=0
```

**The count came back 0 instead of 3, at exit 0, with nothing under
`-fsanitize=address,undefined`.** That is the silent-wrong-answer class this
language exists to kill, manufactured by the compiler rather than by the
program.

---

## 4. Two more measured facts that sink Route A as specified

### 4a. The acquiring scope is not the owning scope

`examples/ledger/db/sqlite.hero:212` `function opened() -> Db?` acquires into a
local `db: CDb @ nullptr` through an `@out` parameter and returns the handle
**inside a record field**. Route A's rule — *"on every path out of the scope
that acquired the handle"* — is `ffi/e2_routeA_escape.c`:

```
clang -std=c11 -Wall -Wextra e2_routeA_escape.c -lsqlite3 -o e2_routeA_escape   # clean
./e2_routeA_escape
handle = 0x1054679a0
exec after the sweep = 21 (no message)          # SQLITE_MISUSE, exit 0
```

Under ASan: same output, **no report**. The only way out is escape analysis,
which §4.19 records as refused by name: *"the release is written by the author
and never inferred, since an inferred release is the escape analysis panel 122
refused."* `opened()` and `prepared()` are the two commonest shapes in any
binding; the file's own comment says *"a binding that opens something is the
commonest shape there is"*.

### 4b. Route A drops a return code the program must react to

`ffi/e4_routeB_block.c`, shape (2), run against real `-lsqlite3`:

```
(2) close answered 5 (BUSY=5) — Route A would drop this
(2) after finalizing, close answered 0
```

`sqlite3_close` answers `SQLITE_BUSY` while a statement is open. Route A's
generated call is `(void)sqlite3_finalize(...)`; the program then exits 0
believing it closed. design.md:1869 already names this: *an ignored C return
code is C's own classic silent bug (§1.11)*.

### 4c. It makes the shipped corpus uncompilable at 7 sites

Route A refuses the author's own call. Measured with `grep -rn --include='*.hero'`
over `examples/` and `selfhost/`:

| site | what it is |
|---|---|
| `examples/ledger/db/sqlite.hero:230` | `_ = sqlite3_close(db)` on a **failed open** — the branch the census documented at length this session |
| `examples/ledger/db/sqlite.hero:235` | `closed()` wrapper |
| `examples/ledger/db/sqlite.hero:280` | `finalized()` wrapper |
| `examples/sqlite/main.hero:85, 101, 107` | three direct calls |
| `examples/curl/main.hero:82` | `curl_easy_cleanup(handle)` |

Plus **15** call sites of `finalized(...)`/`closed(...)` in
`examples/ledger/main.hero`. `selfhost/cli/process.hero:162`'s
`hero_dir_release()` takes **no argument at all**, so Route A cannot reach the
one release obligation in the compiler's own source.

### 4d. brief item 4 — `consumes` is not redundant, it is load-bearing

There is exactly **one** `consumes` mark in the corpus,
`examples/curl/main.hero:53`, and it is on the releaser. Route A kills that mark
and its one call site. But Route A then *depends* on `consumes` for soundness:
any C function that takes ownership (`xmlAddChild` adopts its node;
`curl_slist_append` adopts its head) must be marked, or the sweep double-frees
a handle whose life already ended. **How many functions a binding would have to
mark for that is unrun.** Panel 145's 2-of-17 measured a different question.

---

## 5. Route B, compiled against four release shapes Route A cannot spell

`ffi/e4_routeB_block.c`, one program, three libraries:

```
clang -std=c11 -Wall -Wextra -I/opt/homebrew/include -I/opt/homebrew/include/libpng16 \
      e4_routeB_block.c -lsqlite3 -lcurl -L/opt/homebrew/lib -lpng -o e4_routeB_block
```
compiled (two `-Wunused-label` warnings, from my hand-written sweep labels), ran:

```
(1) curl handle live
(2) close answered 5 (BUSY=5) — Route A would drop this
(2) after finalizing, close answered 0
(3) block allocated
done
exit=0
```

Covering: one handle + void return; a result the program checks; a two-argument
releaser (`png_free(png, block)`); a double-pointer releaser
(`png_destroy_read_struct(&png, 0, 0)`).

**And the hard one binds in Heroes today** — `tree/e11_png.hero`:

```
function png_destroy_read_struct(@png: CPng, @info: CPngInfo, @end: CPngInfo) -> ()
```
```
./heroes run e11_png.hero --include /opt/homebrew/include --library /opt/homebrew/lib
created
destroyed
exit=0
```

Route B's cost at the boundary is **zero new verification machinery**: a
`cleanup` statement is an ordinary call, so the existing
`hero_ffi_probe_*` + `_Static_assert(HERO_RET_*)` pair checks it against the
real header for free, and a misspelled releaser is already `ffi_unknown_name`.
Route A needs its own check — though that is cheap: `selfhost/check/freer.hero`
already verifies that a named freer is *"an `extern` of THIS module, taking
exactly one parameter"* (275 lines, `refusal_for_mark`). **I concede Route A's
verification is the cheap part. Its semantics are the expensive part.**

---

## 6. The easy case, so nobody thinks I am arguing against the goal

`ffi/e1_routeA_scope.c` — Route A applied to `examples/sqlite/main.hero`'s
`first_int`, three exit edges including the two failures:

```
rows            = 3
syntax error    = -1
no rows         = -1
statements open = 0          # asked of sqlite3_next_stmt, the census's method
close           = 0 (SQLITE_OK=0, SQLITE_BUSY=5)
exit=0
```

**Route A closes the leak, perfectly, in the flat case.** Route B emits the same
C. The leak is real and worth closing; the disagreement is only about who names
the obligation.

### The sweep's guard, measured rather than documented

`ffi/e5_null_release.c`, `SIGSEGV`/`SIGBUS`/`SIGABRT` trapped:

```
sqlite3_finalize / sqlite3_close / sqlite3_free / curl_easy_cleanup /
curl_slist_free_all / gzclose / xmlFreeDoc / cairo_destroy /
cairo_surface_destroy / hb_font_destroy / hb_blob_destroy
```
**11 of 11 survived NULL.** So the sweep's `!= nullptr` guard is a cheap
correctness margin, not a requirement — for these eleven. Do not generalise it;
I tested eleven.

### And the double release is NOT always survivable

sqlite3, curl and harfbuzz all survived a double release even under ASan
(`e7`, `e8`). But `ffi/e9_double_free2.c`:

```
clang -std=c11 -g -fsanitize=address e9_double_free2.c -lcurl -o e9_asan
./e9_asan   -> exit=134
AddressSanitizer: SEGV ... #1 free ... #2 curl_slist_free_all
```

`curl_slist_free_all` twice is a segfault. §1.12: *a Heroes program must not
segfault and must not corrupt memory.*

---

## verdict per route, with the measured cost

| route | verdict | measured cost |
|---|---|---|
| **A — the handle names its releaser** | **veto as specified** | inexpressible for **21 of 262** handle types and **0 of 21** raylib obligations; **7** shipped call sites become compile errors; produces a silent wrong answer (`count 3 -> 0`, exit 0, ASan silent) on a borrow that is writable today; drops `SQLITE_BUSY`; needs the escape analysis panel 122 refused |
| **B — a scope-bound statement** | **approve** | 0 ABI change; 0 new FFI verification (an ordinary call, already probed); all four release shapes compiled and ran; the 3-double-pointer case binds in Heroes today; +108 vendored spec tokens the sitting already measured |
| **C — refuse to Part 6** | **its falsifier has fired** | E1 is the program a Part 6 row would have to name as impossible, and it runs at 0 statements open. A Part 6 row must name the program fact that would make it wrong (CLAUDE.md §12) and that fact is now in this report |

## prediction (falsifiable)

**Under Route A, `examples/ledger/db/sqlite.hero` cannot be compiled without
deleting `opened()`'s failed-open `sqlite3_close` at line 230 — the exact leak
this session's census repaired — and step 3 of §4.19's ladder therefore needs a
C shim it does not need today.** Under Route B the same file compiles unchanged
and the 23 escaping paths close with `cleanup finalized(@statement)` written
once per acquiring block. Falsify either half by compiling it.

Second, narrower: **`record CDb tag sqlite3 released sqlite3_close` plus
`function sqlite3_db_handle(statement: CStmt) -> CDb` will compile and print a
wrong row count at exit 0.** I have run the C half (`e7`, count 2 then MISUSE
21) and the Heroes half (`e6_borrow.hero`, `borrowed == owned: true`); the
prediction is that joining them changes nothing.

## condition — what turns my veto into approval

Route A becomes approvable if **all three** hold, each compiled not argued:

1. The mark moves off the type and onto the **acquiring call**
   (`function sqlite3_prepare_v2(... @statement: CStmt acquires sqlite3_finalize)`),
   so a borrow from `sqlite3_db_handle` carries no obligation. This is §4.19's
   own lesson restated: the fact lives in the call, not in the header's type.
2. A handle that leaves its scope inside a record, a `T?` or an array either
   **suppresses** the sweep by a rule the author can read, or is a compile
   error. `opened()` must keep compiling.
3. The author's own call of the releaser stays **legal** wherever the compiler
   is not already making it — `opened()`'s failed-open branch is the test case.

If a fourth route exists it is this: **`cleanup` (Route B) as the form, plus a
`consumes`-style mark on the acquiring call so that a scope holding an acquired
handle with no `cleanup` is a compile error.** That gives the loudness the
census is asking for without the compiler ever guessing which C call gave you
ownership. I did not price it; it is a question, not a premise.

## headers I actually read (for every negative claim above)

Parsed by clang in `survey.py` / `handles.py` / `returners.py`: `sqlite3.h`,
`curl/curl.h`, `zlib.h`, `ncurses.h`, `stdio.h`, `stdlib.h`, `dirent.h`,
`dlfcn.h`, `regex.h`, `libxml/{tree,parser,xpath}.h` (all SDK
`MacOSX.sdk/usr/include`); `openssl/{ssl,evp,bio}.h`, `pcre2.h`, `raylib.h`,
`SDL2/SDL.h`, `SDL3/SDL.h`, `uv.h`, `png.h`, `jpeglib.h`, `ft2build.h` +
`FT_FREETYPE_H`, `GLFW/glfw3.h`, `cairo.h`, `glib.h`, `hb.h`, `gmp.h`,
`zstd.h`, `yaml.h` (all `/opt/homebrew/include`). `raylib.h` was additionally
read by hand with `grep` for the by-value claim.

**Not on this Mac, therefore unread**: `archive.h` (libarchive), `git2.h`
(libgit2), `sodium.h`, `cjson/cJSON.h`. Two of those — libsodium and cJSON —
are named on §1.11's table, so the table is **not fully covered** by this
survey and I say so rather than generalising.

## files

- scripts and C: `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/d2c6e340-fa30-4088-9630-1b1df8597855/scratchpad/panel-147/ffi/`
  (`survey.py`, `classify.py`, `handles.py`, `returners.py`, `e1`–`e11`,
  `survey.json`, `handles.json`, `sqlite_emitted.c`)
- Heroes programs: `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/d2c6e340-fa30-4088-9630-1b1df8597855/scratchpad/panel-147/tree/e6_borrow.hero`, `.../tree/e11_png.hero`
- the frozen tree was never written to; the copy is `.../scratchpad/panel-147/tree`, built with the seed in 3.03 s
