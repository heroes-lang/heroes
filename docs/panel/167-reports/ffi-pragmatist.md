# Panel 167 — ffi-pragmatist: compiled, not opined

Every number below comes from a command run in this sitting on
`arm64-apple-darwin25.6.0`, Apple clang 21.0.0 (clang-2100.1.1.101), against a
COPY of the tree at M-declared-extents step 6. The copy was made with `rsync -a`
(a `cp -r` + `rm -rf` failed to remove `build/`) and the compiler built from the
seed in **4.13 s real, 4.00 s user**. The repository itself was not modified;
this file is the one write.

Real headers used, not mocks: `MacOSX.sdk/usr/include/sqlite3.h` (565231 bytes),
`/usr/lib/libsqlite3.dylib` (sqlite 3.51.0), `/opt/homebrew/include/raylib.h`,
`MacOSX.sdk/usr/include/curl/curl.h`.

---

## 0. The defect, reproduced twice — and the second one is worse than the brief's

**The brief's shape, re-run:** `keep.hero` prints `10 / 0`, exit 0, no
diagnostic; `--sanitize` fires
`AddressSanitizer: stack-use-after-scope ... keep.hero:18 in h_keep_main`.
Reproduced exactly.

**The shape beside it, which is the one that matters.** `sqlite3_bind_blob` is
`const void*` + `int n` + a retention argument — **the field lend's own shape,
in a shipped library**. Bound against the real header and linked against the
real dylib:

```
function bind_it(st: CStmt) -> i64
    r: Row @ row_make()                      # payload = 10,20,30,40,50,60,70,80
    return sqlite3_bind_blob(statement: st, column: 1, p: r.payload.ptr(), n: 8,
                             destructor: SQLITE_STATIC)
```

It **builds** (`wrote blobprog`, no diagnostic). Running `select hex(?1)` after
the frame dies:

| build | printed | honest answer |
|---|---|---|
| plain, 5 runs of 5 | `AAAAAAAAAAAAAAAA` | `0A141E28323C4650` |
| `--sanitize` | `0A141E28323C4650` | — |

`grep -c AddressSanitizer` on the sanitized output: **0**. Exit 0 both ways.

**So `--sanitize` is not the net here.** `otool -L` shows the load happens
inside `/usr/lib/libsqlite3.dylib`, which ASan does not instrument. In
`keep.hero` the retainer is a `static inline` compiled into the program, so ASan
sees the load and fires; the moment the retainer is a real library — which is
§1.11's entire point — **the sanitizer goes quiet and the program silently
writes eight wrong bytes into a database.** That is the silent-wrong-answer
class this language exists to kill, and today it has no instrument at all.

---

## 1. Question 1: is retention ever visible to the toolchain? No.

`__has_attribute`: `noescape` 1, `lifetimebound` 1, `lifetime_capture_by` 1,
`counted_by` 1, `noalias` 0. Availability is not the problem; **meaning is.**

| candidate | what clang 21 did |
|---|---|
| `lifetimebound` on a param of a `void` function | **hard error**: *"cannot be applied to a parameter of a function that returns void"* |
| `noescape` on a `const void *`, body parks it in a static | compiles clean, `-Weverything`, **zero diagnostics** — the lie is free |
| `lifetime_capture_by(s)`, C, stack array into a global sink | compiles clean, `-Weverything -Wdangling -Wdangling-capture`, **zero diagnostics** |
| same, C++20 | **zero diagnostics** |
| same, C++ with a *temporary* | warns — `-Wdangling-capture`. The only shape that fires, and it is not ours |
| `__counted_by(n)` | `#define __counted_by(N)` → **empty** (`sys/cdefs.h:1007`, `malloc/_ptrcheck.h:33`); `-fbounds-safety` is accepted silently and `__counted_by` still fails to parse |

**The corpus census, since "no header states it" is a claim about the option
set.** I searched all **3120** `.h` files under the macOS SDK for `noescape`,
`lifetimebound`, `lifetime_capture_by` and `__counted_by`:

- `lifetimebound`: **0 files**.
- `noescape`: **11 files**, and **every occurrence is on a block parameter**
  `int (^)(...)` — `fts.h`, `glob.h`, `dirent.h`, `_stdlib.h`. Grepping for a
  bare `__attribute__((noescape))` / `__noescape` as a data-pointer spelling
  returns **1** occurrence in 3120 headers.
- `sqlite3.h`: **0** of all four. `raylib.h`: **0**.

Route H worked because `const` is a *type qualifier the ABI already carries*.
Retention is not in the type system, not in the SDK, and not in clang's
diagnostics. **There is no route H for lifetime.** Anything that answers 066
must be a word a Heroes author writes, or a copy.

---

## 2. Question 3 first, because it decides route B: retention is per-CALL

The experiment (`routeB/b2.c`, clang `-Wall -Wextra -Wpedantic -Werror`, clean).
**One declaration, two calls differing only in argument five, frame dead:**

```
SQLITE_TRANSIENT (C copies) : 0A141E28323C4650      <- honest
SQLITE_STATIC    (C keeps)  : 0001BFFB01000000      <- garbage   (ASan build: 707AEAFB01000000)
```

ASan silent in both builds.

**A mark on parameter 3 cannot be right for both**, and the honest one is the
one every sane binding writes first. Census of the retention shapes, so this is
not one API's quirk:

| shape | count in `sqlite3.h` | example |
|---|---|---|
| retention decided per CALL (destructor sentinel) | **13** | `sqlite3_bind_blob/_text/_text16/_pointer`, `sqlite3_result_*`, `sqlite3_set_auxdata`, `sqlite3_set_clientdata` |
| retention unconditional per PARAMETER | **10** | `sqlite3_progress_handler`, `*_hook`, `sqlite3_create_function/_collation/_module`, `set_authorizer`, `trace_v2` |

**Both shapes in one library**, so no single mechanism serves it. And `curl` is
worse than sqlite: `curl_easy_setopt(CURL *, CURLoption, ...)` is **variadic**
(`curl/easy.h:42`) — the retained pointer is not a named parameter at all, and
`CURLOPT_POSTFIELDS` (`curl.h:1149`) keeps while `CURLOPT_COPYPOSTFIELDS`
(`curl.h:1688`) copies, same slot.

**And design.md already ruled on this**, which the brief does not cite. Line
**2345**, inside §4.19 (which opens at 2157):

> no declaration-site mark can express retention at all: `sqlite3.h:4888` puts
> the decision in the fifth argument of one declaration, `curl_easy_setopt` in
> its second, and 0 of 71 `cstr` parameters in this tree are decidable from a
> header.

Route B is not a new idea. It is the shape panel 124 priced and rejected, on a
measurement I have now independently re-run against a real library. (Minor
drift: the cited `sqlite3.h:4888` is `4895` in today's SDK header; the sentence
is unchanged.)

---

## 3. Question 2: the copy costs nothing a binding author would notice

10,000,000 iterations, `-O2`, one timing at a time (CL-025):

| | ns/call |
|---|---|
| lend (take an address) | **0.0** (the optimiser removes it) |
| field lease, 8 bytes | **21.4** |
| field lease, 4096 bytes | **25.9** |

The 512× size difference costs **4.5 ns**: the price is `malloc`/`free`, not the
`memcpy`. And the denominator, measured on the same box:

| | ns/call |
|---|---|
| `sqlite3_bind_blob(..., SQLITE_STATIC)` | 6.7 / 7.0 |
| `sqlite3_bind_blob(..., SQLITE_TRANSIENT)` — **SQLite's own copy** | 15.6 / 15.1 |

**The library already charges 8.5 ns for a copy and expects you to take it.** A
binding author choosing Heroes' 21 ns over SQLite's 15 ns is choosing between two
rounding errors; a `sqlite3_step` is microseconds. This is not a cost, and
CLAUDE.md § Precedence puts performance at rank 13 anyway.

---

## 4. Verdicts

### Route A — the field lease: **approve**

**Section: design.md:2337-2350 (§4.19, the fourth case).** Route A is not a new
mechanism; it is the lease the document already built for `cstr`, applied to the
second of the two things that cross. The document's own reason for the first one
is verbatim the reason for this one.

**Experiment** (`routeA/a_main.c`): the emitted call site written by hand —
`hero_field_held(&h0_r.payload[0], 8)` into the lease cell, `sqlite3_bind_blob(st, 1, cell, 8, SQLITE_STATIC)`,
`hero_field_release(&cell)` after `finalize`. Compiles **clean** under
`-Wall -Wextra -Wpedantic -Werror`. Prints `0A141E28323C4650` in the plain build
**and** the ASan build. The corruption is gone by construction. The runtime
function also compiles clean against the real `heroes_runtime.h`.

**ABI: untouched.** The argument is still `const void *`. No boxing, no
marshalling, no shim. The Lua lesson does not apply.

**And route C's assertion survives it.** The emitter writes, today:

```c
_Static_assert(0 <= (int32_t)(INT64_C(8)) && (int32_t)(INT64_C(8)) <= (int32_t)sizeof(h1_r.payload),
               "heroes-ffi-extent sqlite3_bind_blob p n 1089 1104 1109 1110 8");
```

It names the **source field**, which a lease does not move. I wrote the
equivalent into both prototypes and both compiled.

**The flaw I found, and it is the shape beside the one that provoked the
repair.** `hero_str_held` puts its header **immediately before** the bytes. A
field lease in that shape cannot be handed to any C API that takes ownership and
frees — which is the third of §4.19's own three reserved cases (line 2334, *"a
buffer that C takes ownership of"*), still unbuilt. Run
(`routeA/a_free.c`, `sqlite3_bind_blob(..., sqlite3_free)`):

```
value=0A141E28323C4650
[SIGABRT, exit 134]
ASan: attempting free on address which was not malloc()-ed: 0x6030000021e0
  #1 vdbeMemClearExternAndSetNull  #4 sqlite3_finalize  #5 main a_free.c:28
  0x6030000021e0 is located 16 bytes inside of 25-byte region
```

Route A does not *create* this — it is already true of today's `cstr` lease —
but it would propagate it to a second type and to the exact API family (13
declarations in sqlite3.h) where giving away is the idiom.

### Route B — a mark the binding author writes: **object**

**Section: design.md:2345 (§4.19).** The document already measured that no
declaration-site mark can express retention, and §2 of this report re-measures
it against the real library: one declaration, two behaviours, decided by
argument five. A `keeps` on `p` refuses `SQLITE_TRANSIENT` — a correct, safe,
idiomatic call — and would force a copy on the 15-ns path the library already
provides. That is forced marshalling where C does not need it.

Not a veto: B is *sound*, merely over-strict, and it would still be right for
the 10 unconditional retainers. But it cannot be the answer alone, and the brief
presents it as if the per-call finding were new. It is not; it is at line 2345.

### Route C — write the hole into the specification: **veto**

**Section: design.md §1.12 (line 571), and §1.11 (line 467).** Three seats
vetoed this shape at panel 166 for the *write* direction. The read direction is
strictly worse, and here is the number that makes it so: **`--sanitize` reports
nothing.** `grep -c AddressSanitizer` = 0 on the real-library reproducer, exit 0,
wrong data into a database, five runs of five. Route H at least left clang as
the judge; route C leaves **no instrument at all** the moment the retainer is a
`.dylib` — and §1.11 says every real program's libraries are `.dylib`s. A
refusal is held to the same standard as a feature (CLAUDE.md §12) and this one
cannot name a program fact that makes it safe.

### Route D — withdraw the field lend: **veto**

**Section: design.md §1.11 (line 467), §4.19.** This is the clause my veto
exists for: it makes bindings categorically harder.

**Experiment:** with `.ptr()` removed, I passed the field itself:

```
error[type_mismatch]: expected `ptr`, found `u8[8]`
  17 |  ... p: r.payload, n: 8, destructor: SQLITE_TRANSIENT)
```

**Nothing else in the language reaches a `const void *`.** So withdrawing the
lend makes every buffer-plus-length API unbindable without a hand-written C
shim. Counted in real headers: **28** such declarations in `sqlite3.h`, **7** in
`raylib.h` (`SetShaderValue`, `SetShaderValueV`, `UpdateMeshBuffer`,
`UpdateSound`, `SaveFileData`, …). That is not a corpus migration; it is the
`raylib` and `sqlite` bindings losing their buffer APIs. Panel 166 already
refused this as route F at −44 real; the FFI ground is stronger than the token
one.

### Route E — nobody listed it: the lease whose pointer IS the allocation base

**Approve, and this is what I recommend the sitting adopt in place of bare A.**

A field's length is a **compile-time constant** — unlike a `str`'s — so the
emitter can pass it to the release. That single fact makes a **trailing** header
findable, and a trailing header means the pointer handed to C is the `malloc`
base. Layout: `bytes | NUL | pad to _Alignof | trailer{magic,len}`.

**Experiment** (`routeE/e_main.c`), clang `-Wall -Wextra -Wpedantic -Werror`,
**clean**, linked against the real `libsqlite3.dylib`:

```
lease, C keeps (SQLITE_STATIC) : 0A141E28323C4650
lease, C owns  (sqlite3_free)  : 0A141E28323C4650
survived
```

Identical under ASan, no error. **It closes 066 and it closes §4.19's third
reserved case — the one open since the document was written — in the same
mechanism.** Cost against route A, same instrument: 23.9 ns vs 23.1 ns at 8
bytes, 26.7 ns vs 27.7 ns at 4096. Within noise: **route E is free relative to
A and strictly more capable.** It also inherits `malloc`'s alignment guarantee
on the pointer C receives, which A (base+16) only happens to have.

Route E is *more robust and more complete*, which is what CLAUDE.md § 4 and
§ Precedence direct the panel to take over the cheaper option.

**The gap route E does not close, stated plainly:** A and E both give the author
a *tool*, not a *rule*. `r.payload.ptr()` with `SQLITE_STATIC` still compiles.
Making the wrong choice a compile error needs something more — and after §2 I do
not believe it can live on the parameter. The only place it can live is the
**value that decides**: a mark on the header constant (`SQLITE_TRANSIENT`
declares *this value means C copies*, everything else retains). That generalises
to curl, where `CURLOPT_POSTFIELDS`/`CURLOPT_COPYPOSTFIELDS` is exactly the same
discriminator. I did not build it; **it is unrun**, and I flag it as the shape
worth pricing rather than as a recommendation.

---

## 5. Prediction, falsifiable, with its instrument

**Under route E, binding `sqlite3_bind_blob` needs no shim for all three of its
retention modes — `SQLITE_TRANSIENT`, `SQLITE_STATIC` and a real destructor —
and the emitted C compiles under `-Wall -Wextra -Wpedantic -Werror`.**

Instrument: the three call sites in
`scratchpad/e1/routeE/e_main.c` plus a TRANSIENT third, built with
`clang -std=c11 -Wall -Wextra -Wpedantic -Werror e_main.c -lsqlite3` and run;
all three must print `0A141E28323C4650` and the program must reach `survived`
with exit 0, plain and under `-fsanitize=address`.

**Falsified if** any mode needs a hand-written C wrapper, or if `-Werror`
rejects the emitted call.

**The counter-prediction for route A as briefed:** the same three modes under a
*leading*-header lease abort on the third. Already run: **SIGABRT, exit 134**,
ASan `attempting free on address which was not malloc()-ed ... 16 bytes inside
of a 25-byte region`.

---

## 6. What would change each verdict

- **A → object**, if the leading-header layout is kept *and* no diagnostic
  refuses handing a lease to a parameter whose sibling argument can be a
  destructor. Instrument: `a_free.c` must stop aborting, or the program must
  stop compiling.
- **B → approve as a supplement**, if it is scoped to the 10 unconditional
  retainers and *never* fires on the 13 destructor-sentinel ones. Instrument: a
  `SQLITE_TRANSIENT` call of `sqlite3_bind_blob` with `.ptr()` must still
  compile.
- **C → object instead of veto**, if somebody shows me an instrument that
  catches the escape when the retainer is an uninstrumented `.dylib`. I searched
  clang 21's attribute family, `-fbounds-safety`, `-Wdangling`,
  `-Wdangling-capture`, `-Weverything` and ASan, and found none. That is a claim
  about what I searched, not about the world.
- **D → object instead of veto**, if a route other than `.ptr()` can reach
  `const void *`. I tried passing the field directly; `error[type_mismatch]`. I
  did not search the built-in list exhaustively — **unrun**.
- **E → object**, if the trailing header breaks a platform where `_Alignof`
  padding differs. Measured on darwin arm64 only; Linux and Windows are
  **unrun**, and `.claude/rules/platforms.md` says a platform fact run on one
  box is an inference.

---

## 7. Commands, so every number re-runs

```sh
rsync -a --exclude 'archive/' --exclude 'build/' --exclude 'target/' \
      --exclude '/heroes' --exclude '.git/' <repo>/ $T/
cd $T && clang -I runtime seed/heroes.c runtime/runtime.c -o heroes     # 4.13 s

HEROES_RUNTIME=$T/runtime $T/heroes run keep.hero                       # 10 / 0
HEROES_RUNTIME=$T/runtime $T/heroes run keep.hero --sanitize            # stack-use-after-scope
HEROES_RUNTIME=$T/runtime $T/heroes run blob2.hero                      # AAAAAAAAAAAAAAAA
HEROES_RUNTIME=$T/runtime $T/heroes run blob2.hero --sanitize | grep -c AddressSanitizer   # 0
HEROES_RUNTIME=$T/runtime $T/heroes build blob2.hero --emit-c -o blob2.c
grep -n 'heroes-ffi-extent' blob2.c

SDK=$(xcrun --show-sdk-path)
find $SDK/usr/include -name '*.h' | wc -l                               # 3120
grep -rl 'lifetimebound' $SDK/usr/include --include='*.h' | wc -l       # 0
grep -rl 'noescape'      $SDK/usr/include --include='*.h' | wc -l       # 11
grep -rhoE '__attribute__\(\(noescape\)\)|\b__noescape\b' $SDK/usr/include --include='*.h' | wc -l  # 1
grep -c 'noescape\|lifetimebound\|__counted_by\|lifetime_capture_by' $SDK/usr/include/sqlite3.h     # 0
grep -c 'noescape\|lifetimebound\|counted_by' /opt/homebrew/include/raylib.h                        # 0
grep -n 'define __counted_by' $SDK/usr/include/sys/cdefs.h              # empty expansion
grep -cE '^SQLITE_API .*void *\( *\* *\) *\( *void *\* *\)' $SDK/usr/include/sqlite3.h              # 13

clang -std=c11 -Wall -Wextra -Wpedantic -Werror routeB/b2.c   -lsqlite3 -o b2   && ./b2
clang -std=c11 -Wall -Wextra -Wpedantic -Werror routeA/a_main.c -lsqlite3 -o a  && ./a
clang -std=c11 -Wall -Wextra -Wpedantic -Werror routeA/a_free.c -lsqlite3 -o af && ./af   # exit 134
clang -std=c11 -Wall -Wextra -Wpedantic -Werror routeE/e_main.c -lsqlite3 -o e  && ./e
clang -std=c11 -O2 cost/cost.c -o cost && ./cost
clang -std=c11 -O2 cost/denom.c -lsqlite3 -o denom && ./denom
```

Working files: `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e1aa4272-a6a7-4d83-88ef-86172e5d351e/scratchpad/`
(`t2/` the built copy, `work/` the Heroes reproducers, `e1/` the C experiments).
