# panel 261, ffi-pragmatist: report (started 2026-11-26 10:18, written as it goes)

Every path below is under `docs/panel/261-reports/programs/ffi-pragmatist/` of my copy unless it says otherwise.

## 0. My compiler, and what "compiled" means here

Detached worktree `.claude/worktrees/261-ffi-pragmatist` at `df92f9281`. Seed:
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `--version` **`heroes 0.6.0`, `sources
05186f2b38e77a97`**. Stage 2: `./heroes build selfhost/main.hero -o heroes-next`, `--version` **`sources
4dcf3a9f20257573`**, the repair's key, as `00-shared.md` § 8 says it should read (`build-seed.log`, `build-stage2.log`;
both timed at a load of 19 to 38, so neither is a duration). The arm64 and x86-64 images' stage 2 read the same key
(`linux-arm64/stage2.log`, `linux-x86_64/stage2.log`).

**No compiler with the proposal exists in my copy**, and I may not read another seat's. So every C of the proposal
below is the C this stage 2 emitted for the nearest program the base accepts (the callback's body as a function called
by name, `--emit-c` and the unit `heroes build` compiled), **edited by a script** in the emitter's own shapes: the
function type's typedef (`emit/types.hero:155`), the extern's probe line (`hero_ffi_probe_<fn>`, which hands the
extern an argument of the typedef, the line the base already writes for `atexit`), the thunk
(`emit/callback_thunk.hero`) and its row in `hero_callback_of`, and panel 233's thunk-against-value assertion. Each
unit is compiled with **the words `heroes build` compiled the emitted unit with** (the unit's `.rsp` under the
program's `build/words/`, `-Werror` and `-Werror=incompatible-pointer-types` among them, `selfhost/cli/flags.hero:136-154`)
and linked with that build's runtime and library objects (`build_unit.sh`). The route was proved first: the untouched
unit rebuilt this way prints what `heroes build`'s binary prints (`nftw/visit_body.run.out` and
`visit_body.hand.run.out`, `cmp`-identical). Scripts:
`nftw/make_variants.py`, `nftw/make_fit.py`, `phdr/make_phdr.py`, `sqlite/make_geom.py`, `shapes/make_shapes.py`,
`values/make_values.py`. Linux legs: `run_leg.sh` and the steps it calls, in a `--rm` container named `p261-ffi`, the
compiler built there from `git archive df92f9281 selfhost runtime seed vendor spec` (`build-linux/base.tar` of my copy).

## 1. nftw (this Mac, Linux arm64, Linux x86-64)

`int nftw(const char *, int (*)(const char *, const struct stat *, int, struct FTW *), int, int)`, `ftw.h:59-60`
here, glibc's `__nftw_func_t` there. Programs: `nftw/`.

| program | what it writes | `heroes-next` says (the three legs alike) |
|---|---|---|
| `nftw_proposal.hero.txt` | the callback typed `(function(path: cstr, sb: Stat lent, flag: i32, @at: Ftw) -> i32)` | `error[expected_function_type_params_close]` at the `lent`, 11:74, exit 1 (the `@` behind it) |
| `nftw_today.hero` | the two pointer slots spelled `ptr` | `error[ffi_callback_type]`: *"clang read the header, and it says `int (*)(const char *, const struct stat *, int, struct FTW *)`"*, exit 1 |
| `visit_reads.hero` | the callback's body as a function, `@at: Ftw` only read, as every nftw callback reads it | **`error[never_rebound]`**, its first `guess` fix **`at: Ftw lent`**, exit 1 |

The proposal's C, five variants over the real unit (`make_variants.py`) and a sixth, F, over a second (`make_fit.py`),
each built and run on this Mac and on both Linux legs (`linux-arm64/nftw.*.out`, `linux-x86_64/nftw.*.out`), verdicts
identical on the three:

| variant | `struct stat` slot / `struct FTW` slot | clang (the compiler's words) | run |
|---|---|---|---|
| **B** | `const struct stat *` in typedef, thunk **and the function's own signature** / `struct FTW *` | **accepted** | **nftw walks the tree** (`0 1 128 tree`, `1 1 96 tree/sub`, `2 0 10 tree/sub/b.txt`, `1 0 5 tree/a.txt`, nftw answers `0`); `--sanitize` exit 0, the three legs |
| A | `const` in typedef and thunk, the function as `signature.one_param` writes a `lent` slot today, `struct stat *` | **refused**: panel 233's assertion *and* `incompatible function pointer types initializing 'h_0fn_nftwcb'` | — |
| C | no `const` anywhere (a `lent` slot as the base writes it) | **refused at the probe**: `passing 'h_0fn_nftwcb' (aka 'int (*)(const char *, struct stat *, ...)') to parameter of type 'int (*)(const char *, const struct stat *, ...)'` (arm64: `'__nftw_func_t'`) | — |
| D | `at: Ftw lent`, the `never_rebound` fix: `const struct FTW *` | **refused at the probe** (the header's `struct FTW *` is not const) | — |
| E | B's typedef and thunk, the function left non-const, reached only through `hero_callback_of`'s casts, no assertion, the thunk forwarding `(struct stat *)ph1_sb` | accepted | as B; sound **only** because `ir/read_only.hero` proves no store through a read-only slot |
| **F** (`make_fit.py` over `visit_lent.hero`) | the function takes **both** slots `lent` (route B's `const` on each); the callback type asks `@at`; the value is a **twin** with the type's own signature, handing its `struct FTW *` to the function's `const struct FTW *` (C adds the qualifier, no cast), its thunk what C is handed | **accepted**, both assertions hold (thunk = twin, twin = the typedef) | as B, a callback that writes nothing and needs no self-write; `--sanitize` exit 0; **the three legs** (§ 8) |

**What the table says.** (1) **A mode in a callback type keeps the importc check whole, in both directions**: a `lent`
slot whose `const` does not reach the typedef (C), and an `@`/`lent` that disagrees with the header's `const` (D), are
clang refusals at the probe, the refusal `nftw_today.hero` turns into `ffi_callback_type` (`emit/ffi_narrowed.hero:258`; that a
mode-carrying typedef's refusal maps to it the same way is **unrun**, no proposal compiler existing). (2) **`lent` on a
group record in a callback must be `const T *` in C, and the base writes it `T *`** (`signature.one_param`, read; the
emission `int32_t h_visitbody_visit(const char * h0_path, struct stat *ph1_sb, ...)`): the landing changes
`one_param` for a lent group record (route B) or writes a twin (route F); forgetting is a clang refusal (A), never a
silent build, unless the value bypasses the typedef (E). (3) **C headers do not write `const` on every pointer a
callback only reads**, and `never_rebound` refuses the only identical spelling, `@`; its own fix (`lent`) is refused by
the header (D). **Three of three real bindings meet this** (§ 2, § 3): nftw's `struct FTW *`, glibc's `struct
dl_phdr_info *`, sqlite's `sqlite3_rtree_dbl *aCoord`. Under the proposal without a fit, a read-only callback of any
of them builds only with the self-write the critic found (`c1`, `c8`), which is what `visit_body.hero`,
`phdr_body.hero` and `geom_body.hero` had to write to be emitted at all. **Route F removes the wall** at the cost of a
twin per function used at a mode-wider type.

## 2. dl_iterate_phdr (Linux, my choice): `int (*)(struct dl_phdr_info *, size_t, void *)`

Programs: `phdr/`; outputs `linux-arm64/phdr.*` and `linux-x86_64/phdr.*`, the same on both. `phdr_reads.hero` (`@info` only read): **`error[never_rebound]`**,
exit 1. The proposal's C (`make_phdr.py`): **`@info`** (`struct dl_phdr_info *`) **accepted, run: the callback is
called once per loaded object, `dl_iterate_phdr` answers 0, `--sanitize` exit 0**; **`info: PhdrInfo lent`**
(`const struct dl_phdr_info *`) **refused at the probe**: `passing 'h_0fn_phdrcb' (aka 'int (*)(const struct
dl_phdr_info *, unsigned long, void *)') to parameter of type 'int (*)(struct dl_phdr_info *, size_t, void *)'`.

## 3. sqlite3 (this Mac's SDK header, 3.54.0; the two Linux images' Debian header)

### 3.1 The census, re-run, and it does not read what Part 8 item 19 carries

`census/callbacks.py` over clang's JSON AST of `sqlite3.h` (`clang -Xclang -ast-dump=json -fsyntax-only` of a unit
including it; output `census/sqlite3-census.txt`): every function-pointer type the header writes, nested ones included,
each parameter classified against today's vocabulary, then with `@` (`T *` to ONE element) and `lent` (`const T *` of
a group record). **112 distinct callback types** (panel 112 read 106 by a method the record does not keep). Two facts
panel 112's table predates or did not split:

- **Handles** (panel 145, 2026-09-13, eight days after panel 112): a pointer to a header struct binds today as a
  fieldless `record X tag x`. By the classification **69 of 112 bind today** where Part 8 says 17 of 106;
- **92 of the 112 appear only as a struct's field** (the vtables C reads: `sqlite3_io_methods`, `sqlite3_vfs`,
  `sqlite3_module`, `Fts5ExtensionApi`, ...), which no binding can fill whatever a callback's parameters can spell:
  spec § 13 line 427, *"A callback is a **parameter**"*, and a group record's field is *"a number, `bool`, `ptr`,
  `cstr`, another record of the group, or a fixed array of one"*. **20 are reachable as an API function's
  parameter.**

| the 20 API-parameter callback types | count |
|---|---|
| bind today (classification) | **15** |
| with `@` in a callback type | **17** (+2) |
| with `lent` | **15** (+0) |
| with both | **17** |

**The two `@` adds are arrays**, which `@` reads as ONE element: `xFunc`'s `sqlite3_value **argv` (`argc` of them) and
`xGeom`'s `sqlite3_rtree_dbl *aCoord` (`nCoord`, `sqlite3.h:11183`). clang accepts the type, the callback sees
`argv[0]` alone. The three that stay unbindable: `sqlite3_exec`'s row callback (`char **`), and the two collation
callbacks (`const void *`). **Held to clang**: three of the fifteen bound for real and run today
(`census/today_sample.hero`: `sqlite3_busy_handler`, `sqlite3_wal_hook` receiving `sqlite3 *` as a handle,
`sqlite3_rtree_query_callback` receiving `sqlite3_rtree_query_info *` as a handle), exit 0. The handle binds but
reaches no field: rtree's query callback must write `eWithin` and `rScore` to answer anything, which only a record
reached through `@info` can. **That is the real unlock of `@` in sqlite3's API: field access through a non-`const`
struct pointer, and one scalar out-parameter (`xGeom`'s `pRes`)**, not the +64 a pointer-to-record spelling was
carried as.

**And the classification is this Mac's, not Linux's** (a finding beside the sitting's subject): `census/update_hook.hero`,
`sqlite3_update_hook`'s callback with its `sqlite3_int64` written `i64`, **binds and runs here** (`0`, `true`, `0`)
**and is refused on both Linux legs**, `error[ffi_callback_type]`: *"clang read the header, and it says `void (*)(void *,
int, const char *, const char *, long long)`"*, `i64` being `int64_t`, `long` there; inside a function-pointer type
the two are not one type. Three of the twenty API types carry `sqlite3_int64` or `sqlite3_uint64` (`update_hook`'s,
`void (*)(void *, sqlite3_int64, int)`, `void (*)(void *, const char *, sqlite3_uint64)`), so on Linux the counts read
**12 today and 14 with `@`** by the same classification, one of the three held to clang on both Linux legs. No Heroes word spells `long
long` on Linux. design.md covers a C `long` **field** and an argument past `...` (lines 4060 to 4082), **not a `long
long` inside a callback type**; `gh issue list --search` for *"long long" callback*, *sqlite3_int64*, *long long
int64_t function pointer* and *update_hook* found no issue naming it: a question for the coordinator to file, not a
finding of absence.

### 3.2 `sqlite3_exec`: no spelling under the proposal

`sqlite/exec_typedef.c`, the probe line's shape against the real header, the compiler's words: `@values: cstr`
(`const char **`), `@values: ptr` (`void **`), `@values: i8` (`signed char **`): **refused, all three**
(`incompatible function pointer types passing 'h_0fn_rowcb' ... to parameter of type 'int (*)(void *, int, char **,
char **)'`). `char **` has no Heroes spelling, `@` or not, and `argv` is `argc` strings, not one. Part 8 item 19's
28-line `static inline` route stays the binding.

### 3.3 rtree's geometry callback, the out-parameter (`int *pRes`)

Programs: `sqlite/`. `geom_reads.hero` (`@coord` only read): **`error[never_rebound]`**, exit 1, the third binding of
three. The proposal's C (`make_geom.py`; driver block in plain C: an rtree of three rows, the registration, `SELECT id
... MATCH left_of_five()`): **accepted; run: ids `1` and `3`, the right rows, `@res` written through C's `int *`;
`--sanitize` exit 0; the same on both Linux legs** (`linux-*/geom.*`). The shape beside it, C handing a **null
`pRes`** (as sqlite's own `unixOpen` tests `if( pOutFlags )` for an optional out-parameter): **exit 134**, `panic: a
null pointer was read through, at offset 0x0, called from geom_body.left_of_five`, then *"Two things put a null where
C read it: a handle or `ptr` holding `nullptr` handed to a C function ... This runtime raised neither."*: robust, and
**the message blames the program for C's null** (design.md §4.17), on the three legs.

## 4. What `@` and `lent` promise at the boundary, measured (`shapes/`; the three legs alike)

`shapes.h` (C drivers) and `shapes_body.hero` (bodies legal today), `make_shapes.py`, thunks as the base writes them
for a function C can call (a `@` parameter *"forwarded as the pointer it is and its cell left alone"*,
`callback_thunk.hero:75`):

| shape | result | reading |
|---|---|---|
| C hands **one pointer to two `@` slots** (`twice`, `both(@a, @b)`: `a @ a + 1; b @ b * 10; return a`) | **`40040`**, at `-O0` and `-O2`, the three legs: `a` answers 40 | §4.8's model (copy in, copy out) answers 4; the emitted body works through the pointers. **No UB today**: the emitter writes no `restrict` on a parameter (`grep -rn restrict selfhost/emit`: a test's string in `ffi_read_only.hero`, a list of C's keywords in `guarded_names.hero`, and `lend_type.hero` and `macro_guard.hero` reading or stripping a header's), but **§4.8 lines 1620-1621's invariant, "`@` parameters never alias, so ... `restrict` stay[s] legal", is false for a callback** |
| C hands a **null** to an `@` slot (`nulled`, `set_five(@n)`) | **exit 134**, the same misattributed `null pointer was read through` panic | robust by the runtime's fault handler, the message false |
| the callback writes **a handle it gave back, through the emptied binding**, into C's `@h` (`take`) | exit 134, `a dead C handle was used inside C ... called from shapes_body.main` | robust: the binding holds the poison |
| the same through **a copy taken before the give-back** (`take_copy`) | **exit 0, printing `4702111234474983745`** (`0x4141414141414141`, C's bytes read back from freed memory), the three legs; `--sanitize`: **`heap-use-after-free ... shapes.h:34 in hand_over`** | **a §1.12 hole the base's thunk leaves open the day C can call an `@` function** |
| the same two with **the thunk clearing `*ph0_h` on entry and `hero_handle_alive(*ph0_h, ...)` after the call** (variant `checked`, as it checks a result today) | **exit 134**, `panic: a C handle reached the handle the callback \`shapes_body.take_copy\` left in its \`@h\` at an address a call marked \`consumes\` ... ended`, the three legs, under `--sanitize` too | closed |
| C hands **its own node at an address the program gave back** a moment before, through a slot the callback only reads (`reuse/`: a pool allocator whose free list returns the last address first, `peek_at(h: Node lent)`, `node **` in C, read by `node_peek`), the thunk as the base writes it | **exit 134, a correct program refused**: `panic: a C handle reached the argument \`n\` of \`node_peek\` at an address a call marked \`consumes\` ... ended ... a copy made before the ending call`, **the three legs** | `callback_thunk.hero`'s own danger (lines 4-14, `cb_reuse`), which it closes for a by-value handle and not for a pointer slot |
| the same, the thunk **clearing `*ph0_h` on entry** | **`11`, exit 0**, the three legs | closed |
| a callback **writing back through `@h` what C handed** (`pass_back(@h: Node)`, `h @ h`), at the reused address | **`11`, exit 0**, with the base's thunk and with the checked one, the three legs | the check after the call passes a live handle because the clear on entry came first |
| C's **8,192-byte record** through `@b` (`fill_big`, `grow(@b: Big)`) | **`42`**, the three legs | an `@` group record is `T *` in both value conventions: no twin (panel 236's `large_value.crosses` counts plain parameters only) |
| a **fixed array** through `@` (`fixed_at.hero`) | `error[fixed_outside_a_group]` | refused today, stays refused |
| `char **` through `@` | refused (§ 3.2) | no spelling |
| a callback's **`@` place handed onward** to an extern that keeps it (`keeps_body.hero`: `onward(@s)` calling `keep_it(@s)`, no `lent`) | accepted today, exit 0, prints `0 2` | § 13 leaves *"may keep only while the place does not move"* to the binding; under the proposal the place may be C's own (nftw's `struct FTW`), dead when nftw returns: today's class, wider reach; **the dangling run is unrun** |
| a **`lent` number** in a callback type | a Heroes function's `n: i64 lent` is `int64_t *ph0_n` in C (`values/lent_scalar.hero`), while § 13's `lent` on an extern number is no pointer | **two readings; the spec owes one** (clang refuses whichever disagrees with the header) |

## 5. A Heroes value with an `@` slot, no extern (`values/`)

`values_body.hero` (base emission: `fs = [get]` as `hero_array_new(&hero_desc_func, 1)` of the typedef, the callee
loaded into a temporary before the call, as `00-shared.md` § 2's `q2b` reads), `make_values.py` beside it:
`typedef int64_t (*h_0fn_at)(h_valuesbody_App *, int64_t)`, an array `[bump, peek]`, `hero_f(&h0_a, 5)`. **Accepted,
run: `9 9` (`bump` through the value writes `a.n`) and `14 9` (`peek`, lent, in the `@` table); `--sanitize` exit 0;
the three legs.**
For a program record the `lent`-to-`@` fit is free in C: `peek` and `bump` are both `h_valuesbody_App *` today. **For
a group record under route B it is not**: `nftw/fit_const.c`, `int32_t visit_lent(const struct stat *)` assigned to
`int (*)(struct stat *)`, **refused** (`incompatible function pointer types initializing 'h_0fn_at'`): that fit needs
route F's twin. A library function's value twin already carries the pointer slot
(`hero_value_h_library_dist(int64_t h0_a, int64_t *ph1_b)`, `selfhost/emit/signature.hero:342`'s assertion, **read,
not run by me**).

**Byte-identity, judged by hand** (no proposal compiler exists): the typedef's name is `content_key` over
`render.render_ty`'s text, so a renderer that prints a mode only where one stands keeps every mode-free typedef's name,
as the labels run kept its own (`git show -s 2724a4f9a`). What moves bytes beyond it is route B: `one_param` writing
`const` for a lent group record changes every function with one, **3 `run` goldens** by `census/lent_group.py` over `git
ls-files '*.hero'` (21 hits in 10 files, the rest `check`, `full` and `unsupported` cases, which emit nothing, and one
generic false hit), whether or not they use a mode in a function type.

## 6. Threads

`threads/s1.hero`, `s2.hero` (the critic's, re-run with my stage 2): `mutable_parameter_as_value`,
`lent_parameter_as_value`, exit 1 each: **the refusal is the value's, so the proposal retires it and `spawn` needs one
of its own.** What the runtime is handed (`runtime/parts/spawn.c:693`, `heroes_runtime.h:1164-1175`): `HeroThreadBox`,
whose `arg` is a `HeroCross` the program's `run` rebuilds on the new thread, a copy. An `@` body would write a copy
nobody reads and a `lent` one would read a copy, not the place: both lie, and § 11 line 383 (*"threads share nothing but
a mailbox"*) is the rule they would break. At the C door the check holds by itself: `threads/thread_door.c`, an `@` body
re-declared against `hero_thread_spawn_sized`, **refused** (`'long long (*)(long long *)' to parameter of type
'int64_t (*)(int64_t)'`). A function value sent through a mailbox is an address and carries no place: nothing to
refuse.

## 7. What the thunk owes, from the measurements above

`callback_thunk.hero`'s premise (*"C cannot call a function that takes one, and a Heroes call through a value never
arrives here"*, line 75) is what the proposal removes. A landing that lets C call an `@` or `lent` function owes, per
pointer slot: (a) **`hero_handle_lent(*p)` on entry and `hero_handle_alive(*p, ...)` after the call** where the slot
reaches a handle: without the first a correct program is refused at run time with a false message (`reuse`, § 4),
without the second C writes through freed memory at exit 0 (`take_copy`, § 4); (b) **a null check naming the callback and the
slot**, the panic's message otherwise false (§ 3.3, § 4); (c) **a pairwise compare of `@` slots of one type**, or
§4.8's invariant restated without callbacks; (d) **`const` for a lent group record** in the typedef and the thunk,
and in the function (route B) or a twin (route F), else the build is refused (A) or sound only by the IR verifier (E).
One compare each per call C makes; nothing on a Heroes call through a value, which never reaches the thunk.

**And a diagnostic's text**: `ffi_callback_type`'s second note, *"a pointee the header marks `const` has no spelling here
unless it is `const char *`, which is `cstr`"* (`selfhost/emit/ffi_narrowed.hero`, the `if ffi_lookup.has(... "const ")`
arm), becomes false the day `lent` spells `const struct X *`, and `never_rebound`'s first `guess` fix (`at: Ftw lent`)
is refused by clang inside a callback type whose header is not `const` (§ 1, D): both are design.md §4.17's to
rewrite with the landing. Part 8 item 19's fifth cause names `expected_function_type_params_close` where the base
says `expected_type` (`00-shared.md` § 2 question 7, re-run there).

## 8. Linux x86-64

`heroes-linux:latest` run without `--platform` (`uname -m` in it reads `x86_64`, under emulation, docker warning that
the image's platform does not match the host's); with `--platform linux/amd64` docker answered *"image with reference
heroes-linux:latest was found but does not provide the specified platform (linux/amd64)"* and ran nothing. Debian clang
22.1.8; stage 2 built there reads **`sources 4dcf3a9f20257573`** (`linux-x86_64/stage2.log`), the key of this Mac and
of arm64. **Every verdict of § 1 to § 5 reads the same there** (`linux-x86_64/*.out`): nftw B, E and F build and walk
the tree, A, C and D refused with the same clang lines (C and D against `'__nftw_func_t'`), `--sanitize` exit 0;
`dl_iterate_phdr` `@info` builds and runs, `info lent` refused; rtree's ids `1` and `3`, the null `pRes` the same
misattributed panic; `twice` **40040** at `-O0` and `-O2`, `fill_big` **42**, `take_copy` **exit 0 printing
`4702111234474983745`** and `heap-use-after-free ... WRITE of size 8` under `--sanitize`, the checked thunk's panic;
`reuse` **exit 134** with the base's thunk, exit 0 with the clear, `pass_back` exit 0 either way; the value with an
`@` slot `9 9` and `14 9`; the three refusals of `never_rebound` (§ 1 to § 3). The hand links of the rtree programs print
`ld` lines naming `ferror`, `setenv` and `hero_file_read_str` at exit 0, on both Linux legs (`geom.build.out`,
`geom.null.build.out`, `geom.san.build.out`): the `-Wl,--trace-symbol=` words `selfhost/cli/supplied_probe.hero:249`
writes into the link of a program that names a library, which `heroes build` reads and does not print
(`geom.body.build.out`: none) and `build_unit.sh` prints raw; not the proposal's. **Linux arm64's
run of the steps written after its first** (`run_later.sh`: the value of § 5, route F, the reuse and pass-back shapes,
the census's two samples), its stage 2 again `sources 4dcf3a9f20257573` (`linux-arm64/later.stage2.log`): every
verdict as on the other two legs (`linux-arm64/values.*`, `fit.*`, `reuse.*`, `census.*`).

## 9. The verdict

- **`verdict`: object** to the proposal as `00-shared.md` § 1 writes it (identity alone, no fit, the thunk unchanged),
  with a **conditional veto**: I veto any landing that lets C call an `@` or `lent` function through the thunk the base
  writes, which forwards an `@` slot unchecked: measured, `take_copy` through `hand_over` exits **0** after C wrote 64
  bytes into freed memory, on the three legs, `heap-use-after-free` under `--sanitize` (§ 4). I **approve**
  a mode in an `extern` callback's type, **meaningful rather than refused**, `@T` as `T *` to one element and `T lent`
  of a group record as `const T *`, on the landing that carries § 7's four duties with their cases and **route F**,
  question 5's fit of a `lent` function where an `@` type is asked, through a twin. **`spawn(f, x)` with a mode:
  refused, by a code of `spawn`'s own** (§ 6).
- **`section`**: design.md §1.11 and §4.19 (the FFI is every package's door; a wrong signature a compile error);
  Part 8 item 19 (lines 4582-4640; its counts are stale, § 3.1, and its fifth cause's code too, `expected_type`);
  §4.8 lines 1620-1621 (`@` never aliases), which a C caller breaks (§ 4) and **which the document does not cover for
  a callback**; §1.12 for the thunk's duties. **Not covered by design.md**: that `never_rebound` meets C headers' missing
  `const` (§ 1), what `lent` on a number means in a callback type (§ 4), and a `long long` inside a callback type on
  Linux, which no Heroes word spells (§ 3.1, beside this sitting's subject).
- **`experiment`**: nftw (this Mac, Linux arm64, Linux x86-64), `dl_iterate_phdr` (both Linux legs), sqlite3's rtree geometry
  callback and `sqlite3_exec`, the shapes of § 4 and the value of § 5, each the base's own emission edited in its
  shapes and compiled with its words against the real header. **clang accepted** B, E, F, `@info`, the geometry
  callback, the alias, null, handle and 8,192-byte shapes of § 4, and § 5's value; **clang refused** A, C, D, `info
  lent`, all three `sqlite3_exec` typedefs, the `lent`-to-`@` assignment for a `const` group record, and an `@` body at
  the thread door; the base's checker refuses a fixed array through `@` (`fixed_outside_a_group`).
- **`argument`**: A mode in a callback type keeps importc's check whole: every disagreement between `@`, `lent` and the
  header's `const` was a clang refusal at the probe, never a silent build. What it unlocks is real but narrow: nftw,
  `dl_iterate_phdr`, rtree's out-parameter and struct fields; not `sqlite3_exec`, not the 92 vtable types. Two things
  make it wrong as written. The thunk's premise that C never calls an `@` function becomes false, and a stale handle
  written back crosses at exit 0. And all three real bindings hand a non-`const` pointer the callback only reads, which
  `never_rebound` refuses, its own fix refused by clang: without a `lent`-to-`@` fit the binding builds only with a
  self-write.
- **`prediction`**: (1) On a landing carrying route F, **nftw binds with no shim and no self-write** on this Mac and
  both Linux images, its callback written `(path: cstr, sb: Stat lent, flag: i32, at: Ftw lent)` and handed where the
  type asks `@at: Ftw`; without F it builds only with `at @ at` or a field's self-write. (2) **`sqlite3_exec`'s row
  callback still needs Part 8's `static inline` shim under every route of this sitting**: `char **` has no spelling.
  (3) Of `sqlite3.h` 3.54.0's **20** API-parameter callback types, `@` and `lent` make **17** pass clang on this Mac
  (**14** on Linux, `sqlite3_int64` being `long long` there, § 3.1) and **0** of the **92** vtable types, and the two
  that `@` adds read one element of an array.
- **`condition`**: to **approve**: a landing whose thunk (a) clears and checks every handle an `@` slot reaches,
  (b) refuses a null pointer slot naming the callback and the slot, (c) compares `@` slots of one type or the spec
  restates §4.8's invariant without callbacks, (d) writes `const` for a lent group record in the typedef, the thunk
  and the function or its twin; and route F or another rule that lets a read-only callback over a non-`const` C pointer
  be written without a self-write; each with a `run` case on the three legs. To **veto**: a landing whose thunk
  forwards an `@` slot reaching a handle unchecked. Unchanged by sketches A, B and C of § 6.2 and the routes of § 7,
  none of which reaches C on its own, but (c), closures, which I object to at the boundary (§ 9.1).

### 9.1 Per route, from the C boundary (`00-shared.md` § 1 item 4, § 6.2, § 7)

| route | verdict | cost at the boundary (C, thunk, runtime) | resting on |
|---|---|---|---|
| a mode in an `extern` callback's type (§ 1 item 4) | **meaningful, approve on § 9's condition** | the thunk's four duties (§ 7); `const` for a lent group record; diagnostics' text | § 1 to § 4, three legs |
| `spawn(f, x)` with a mode | **refuse, a code of `spawn`'s own** | none: the runtime's `HeroCross` copy is what a body gets (§ 6) | `s1`, `s2`, `thread_door.c` |
| sketch A, the program keeps the table | **no objection** | a typedef with a pointer slot and a call handing `&place`: builds and runs (§ 5) | `values/` |
| sketch B, a generic face over the program's table | **no objection, one condition**: `content_key` and `instance_key` (`emit/synth.hero:35`, `:47`) must render the mode, or `(function(@App) -> ())` and `(function(App) -> ())` share one C typedef name, panel 112's R6 class and defect 2650's | a mode in `render.render_ty`'s text | reading; **unrun** (no proposal compiler) |
| sketch C / (d), a generic record | not the boundary's question | — | — |
| (c) closures | **object at the boundary**: a capturing value is no longer the eight bytes of a C function pointer (`emit/ctype.hero`'s `function_ty` arm: *"A Heroes function value and a C callback are the same eight bytes"*), so every callback would need a trampoline and an environment C cannot see | — | reading; **unrun** |
| (f), (g), (k), (l), (n), (e), (i), (j) | no objection | none of them reaches C | — |
| (h) `lent` coerced to `@`, (m) `lent` adapted to by-value | **(h) wanted, through a twin** (route F); (m) no objection, an adapter passing an address is C's own convention | a twin per function used at the wider type | § 1 F, § 5 `fit_const.c` |
| (p) a callback C runs on another thread | no new objection | a function C may call carries `hero_thread_guard` today (`h_visitbody_seen` in § 1's emission); the thread door refuses an `@` body by itself (§ 6) | § 6 |

## 10. What stays unrun

- **No compiler with the proposal**: every C of the proposal is hand-edited emission (§ 0), so the emitter's own
  output, the checker's diagnostics for a mode mismatch, and **byte-identity** of a mode-free program are judged by hand
  (§ 5), not measured.
- **Windows**: not my legs; nftw and `dl_iterate_phdr` do not exist there.
- The library twin's pointer slot (`signature.hero:342`) is **read**, its test not run by me.
- The census's 15/17/15/17 are a **classification** held to clang on three of the twenty (§ 3.1), not twenty compiles;
  Part 8's 106/153/102 were not reproduced, the method that produced them not being in the record.
- The other two `sqlite3_int64`/`sqlite3_uint64` callback types of § 3.1 on Linux: **unrun** (the third, `update_hook`'s,
  is refused on both Linux legs).
- The dangling run of an `@` place C owns, kept by a third library past the callback (§ 4): **unrun**.
- A null pointer slot at an offset past the first unmapped pages (a group record past 64 KiB on Linux): **unrun**; the
  thunk's null check (§ 7 (b)) answers it whatever the offset.
