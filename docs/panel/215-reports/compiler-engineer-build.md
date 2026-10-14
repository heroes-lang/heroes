# Panel 215, the compiler-engineer builds Q1's route G, recursive

Written 2026-10-14 from 11:34, in the copy `.claude/worktrees/scratch-panel-215/ce-build` at
`7e5c02048` (`git worktree add --detach`), after reading the critic's pass 2 whole. No verdict: the
numbers. Every line below names the command that produced it; what could not be run says *unrun*.
The session was stopped at about 12:30 by a restart (my last command, the route compiler's
`--emit-c` of itself, was killed, exit 137) and resumed from the copy's state, which `git diff
--stat` read intact.

## In short

- **Route G recursive is built and closes every shape the critic measured open, at every level, on
  x86-64**: `pc.hero` from **6/5/5/5/6** MemorySanitizer reports today to **0/0/0/0/0** (the record
  in a record `:45` and the array of records lent `:50` included), and an element lent as a record
  (`el.hero`, mine) from 1/1/1/1/1 to 0. At `heroes build`'s default `-O0`, `dv.hero` reads `true 0
  7` today and **`true 0 0`** under the route, three runs each; the ffi-pragmatist's `main.hero`
  goes from `memcmp` nonzero and SHA-256 digests differing run to run to **`memcmp 0 0 0`, `sha
  true true true`**, three runs each.
- **The vouched half I called sound was not, measured**: a union bound by its narrow arm passes the
  screen, and spanning its declared fields zeroed the wide arm (`2.5` read back as `0.0`). Repaired
  in the copy by C's own `__builtin_classify_type`; my first report's prototype carries the same
  defect.
- **The half left unbuilt is built through the dump**: `partial` records and anonymous members
  (`rzpad`'s `PU` and `PP`, `pn`'s `An`, `Bf`, `Pt`, five `ffi-rest-zero-*` goldens) read 0 at
  every level under the route on both legs. One report stays: `pn.hero:76`, the padding **bits**
  of a byte a named bit-field holds, at `-O0` only.
- **Suites**: `run` 518 and 0, `check` 699 and 0, `emit` 12 and 0, the compiler's own tests 1,654
  passed; `emission` 26 moved, every moved line read by script: zeroing statements, braces and
  `#line`s only, 0 other; `layout` red on four modules past 300 lines.
- **Cost**: +480 −11 lines (`selfhost/emit/` +380, `selfhost/cli/` +68, `runtime/` +32), 0 in the
  checker, the IR and the lowering; the cold `--emit-c` of the compiler −0.5% over four runs each,
  inside the noise. My first report's predictions *"at most 250 lines"* and *"`emission` moves at
  most 10"* are falsified by the recursive route.

## The route as built

**What it is**: the critic's § 7 route. Before every `extern` call, each group record the call lends
through `@` (a whole cell, a field, an element, an `@` parameter handed on) has the bytes **no C
member holds** written zero in place; the elements of an array lent through `@` are zeroed the same
way in the region `hero_lend_take` fills, after the take and before the call. Every span is written
in the emitted C as `__builtin_offsetof(T, path)` and `sizeof(((T *)0)->path)`, so clang computes
every number. Where the membership comes from:

- **a record the layout screen vouches for** (not `partial`, its declared fields exactly the
  struct's members, `emit/layout_screen.hero`): its declared fields; a field that is another record
  of the group recurses into that record's members under its path (`inner.c`, `inner.d`); a field
  that is a fixed array of records (`Cd[2]`) is covered whole and its elements zeroed in a loop
  over `sizeof(member) / sizeof(element)`, recursively;
- **every other record** (flagged by the screen: an anonymous member, a bit-field, a construction
  leaving a field out; and **every `partial` record**, which the screen proves nothing of and which
  is now added to the dump unit beside the flagged ones, without entering the check unit): the
  **leaves of clang's layout dump**, read by the existing reader (`layout_text.members`): each
  member with no members below it by its path; a union's arms each a span, so a byte is written
  only where it is padding in every arm; a named bit-field by the bytes its bits reach, the one
  number read from the dump's offset column (`byte:lo-hi`, the last byte `byte + hi / 8`), since C
  has no `offsetof` for it; an unnamed bit-field as padding (C11 6.7.2.1p12); a flexible array
  member covering everything from its offset to the end. A record whose dump the reader cannot read
  (the control record fails, no block, an unreadable bit-field column) gets nothing written, which
  is today's behaviour.

**A route nobody listed, checked and closed**: GCC's `__builtin_clear_padding`, which would make
clang compute the padding itself, recursion and unions included. `clang -std=c11 -Wall` on a
four-line use, `build/cp/cp.c`: Apple clang 21.0.0 *"error: use of unknown builtin
'__builtin_clear_padding'"*; Debian clang 22.1.8 in `heroes-linux-arm64`, the same. Not available.

**What the dump shows of the shapes**, measured (`clang -fsyntax-only -Xclang -fdump-record-layouts`
over `build/cp/shapes.h`, Apple clang 21): a nested record is expanded with its members
(`struct cd inner` then `char c`, `double d`), a named and an anonymous union both with their arms,
an anonymous struct with its members, a bit-field as `1:0-2 | unsigned int b` and an unnamed one
with an empty name; **an array of records is ONE line with nothing below it** (`struct cd[2] a`,
`struct cd[2][3] m`). So the recursion into array elements cannot come from the dump: it comes from
the binding's own field type (`Cd[2]`, a record of the group), and an array of C structs the
binding does not declare so stays covered whole (below, what stays open).

## Which lends the route reaches, enumerated from the emitter

Read in `selfhost/emit/` and `selfhost/check/lend_types.hero`, each a reading and not a run unless
the shape's probe below says so:

- **`@` to an `extern` parameter of a record type**, whatever the place: a whole cell (in its
  `hero_lend_local` region), a field (`@x.h`), an element (`@xs[1]`), a Heroes function's `@`
  parameter handed on (`(*ph0_p)`): `ops.hero` calls `padding.zero` for every `.inout_arg` before
  the call, and the place is spelled by `aggregate.place`, the spelling C is handed, so the bytes
  zeroed are the bytes C receives. Probes: `pc`, `fieldlend`, `small`, `cret`, `dv`, `pn`, `el`.
- **`@` to an `extern` parameter of a `[T]` of records with `counted_by`**: the buffer
  `hero_lend_take` fills, zeroed after the take (`lend_buffer.before`). Probes: `pc.hero:50`, the
  ffi-pragmatist's `main.hero` (`write`, `send`, `memcmp`, `SHA256_Update` over `@buf: rec.Cd
  counted_by n lent`, a record lent whole, so the first bullet).
- **`.ptr()`**: not a padding path. `check/lend_types.hero` types it only on a fixed run of 8-bit
  integers (`fixed_byte_run`, *"a fixed run of bytes, `i8[N]` or `u8[N]`"*), which holds no padding.
- **A `[T]` of records handed without `@`**: asked of the checker below (`nv.hero`).
- **A record passed BY VALUE to C**: not a lend, and not reached by the route. Its bytes travel in
  registers or in a copy C's caller code makes; what C's callee then does with the copy's padding is
  outside every route this sitting listed (*a question, unrun*).

## Lines, by directory

`git diff --numstat` in the copy, against `7e5c02048` (the Q2 `pinned` hunks of my first prototype
were NOT applied: `git apply --include` of `runtime/*`, `selfhost/cli/*`, `selfhost/emit/*` only):

| directory | file | + / - |
|---|---|---|
| `selfhost/emit/` | `padding.hero` **new**: 331 lines by `wc -l`, **267** by the `layout` suite's count (non-blank, outside `test` blocks, comments included; mirrored by `awk`) | +331 |
| | `layout_text.hero` (a bit-field's last byte, `last_byte`, two test lines) | +29 -1 |
| | `lend_buffer.hero` (the buffer's zeroing after the take) | +17 -1 |
| | `ops.hero` (the call) | +2 |
| | `guarded_names.hero` (`hero_pad_zero` in the guard list) | +1 -1 |
| `selfhost/cli/` | `layout.hero` (vouched or laid out, `partial` records dumped, `lay`) | +57 -7 |
| | `probing.hero` (the membership rides the pointee answers) | +9 -1 |
| | `header_types.hero` (`%pad <decl>` into `lend_units`) | +2 |
| `runtime/` | `parts/lend.c` (`hero_pad_zero` with a count of objects) | +28 |
| | `heroes_runtime.h`, `heroes_guard_open.h`, `heroes_guard_close.h` | +2, +1, +1 |
| `selfhost/check/`, `selfhost/ir/`, `selfhost/parse/`, `selfhost/print/`, the lowering, the spec | | **0** |

**By directory: `selfhost/emit/` +380 -3, `selfhost/cli/` +68 -8, `runtime/` +32; total +480 -11**
(`git diff --numstat` plus `wc -l` of the new file, after the union repair below). **My first
report's prediction, *"at most 250 lines across `selfhost/emit/`, `selfhost/cli/` and `runtime/`"*,
is falsified by the recursive route**, as the critic's pass 2 said it would be (it priced a
non-recursive G); its *"0 in `selfhost/check/` and `selfhost/ir/`"* holds. Not core (design.md
§1.7, Part 5): no construct the checker, the IR or the lowering handles; everything is in the
emitter, the build's layout round and one runtime function.

## Built

- Today's compiler from the seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
  real 14.83, user 14.33 (load 4.7), `heroes 0.4.0`.
- The route's: `./heroes build selfhost/main.hero -o build/heroes-g`, real 101.36, user 78.28, sys
  17.44, `wrote build/heroes-g`, rebuilt after the union repair below (real 42.82, warm), after `./heroes check selfhost/main.hero` read no error (the
  write-time hook caught one first: a local named `table` shadowing the module `check/table`; and
  the checker ten `needs_label`s on a helper of two `[str]` parameters, repaired by labels).

## Per shape: what the route writes, read in the emitted C

`build/heroes-g build <probe>.hero --emit-c`, the `hero_pad_zero` lines read by `grep`:

| probe | shape | today | route G |
|---|---|---|---|
| `pc.hero:42` | `struct tp`, **tail** padding, field lend | no line | `{offsetof(tp, d), sizeof d, offsetof(tp, c), sizeof c}`, count 1 |
| `pc.hero:45` | `struct ocd { struct cd inner; }`, **a record nested in a record**, field lend | no line (the prototype: one span `inner`, the whole 16 bytes, so nothing written) | `{offsetof(ocd, inner.c), sizeof inner.c, offsetof(ocd, inner.d), sizeof inner.d}` |
| `pc.hero:48` | `struct cd`, field lend | no line | `c`, `d` |
| `pc.hero:50` | `[Cd] counted_by 2 lent`, **an array of records** | no line (the prototype: none, `.array` not a record) | after `hero_lend_take`: `hero_pad_zero(hero_lend_b23_0, sizeof(struct cd), (size_t)hero_lend_e23_0, ...)` |
| `uni.hero` | anonymous union `{int32_t fd; void *h;}`, bound `kind`, `fd` | no line | spans `kind`, `fd`, **`h`** (the arm the binding does not name, from the dump): only bytes 4..7 written |
| `unimid.hero` | the same union followed by `int32_t tail` | no line | `kind`, `fd`, `h`, `tail` |
| `rzpad.hero` `PU` | anonymous union `{char c; double d;}` | no line | `kind`, `c`, `d` |
| `rzpad.hero` `PP` | **`partial`**, a named union `u` and `t` | no line (the prototype skipped `partial`) | `kind`, `u.c`, `u.d`, `t`, from the dump |
| `cret`, `fieldlend`, `small`, `small_rz`, `dv` | `struct cd`, `ci`, `fc` lent whole, by field, by `@` parameter | no line | `c`, `d` (and `ci`, `fc`'s two), as the prototype |

## Per shape: values on this Mac (arm64), no sanitizer (before the union repair; `uni`, `unimid`, `rzpad`, `pc` re-run after it, the same)

`heroes build <probe>.hero` with each compiler, run once (`build/padc/`):

| probe | today | route G |
|---|---|---|
| `uni.hero` | `1234605616436508552` | `1234605616436508552` |
| `unimid.hero` | `1234605616436508552` | `1234605616436508552` |
| `rzpad.hero` | ` 2 2 1 1` | ` 2 2 1 1` |
| `cret.hero` | `@16` | `@16` |
| `fieldlend.hero` | `@ @ @48` | `@ @ @48` |
| `pc.hero` | `16 16 16 32` | `16 16 16 32` |
| `dv.hero` | `true 0 0` | `true 0 0` |

So on the two union probes the route keeps the arm C wrote, where the naive G printed `1432778632`
(my first report) and the ffi-pragmatist's gap macro the same on `unimid` (the critic's pass 2).

## The shapes beside the critic's, built (`build/padc/pn.hero`, `el.hero`, `nv.hero`)

Each record of `pn.hero` is returned by C by value from a local whose members alone C wrote (so its
padding is uninitialised in C's own value), stored in a Heroes record's field and lent from there.
What the route writes, read in `heroes-g build pn.hero --emit-c` (8 calls of `hero_pad_zero`):

| shape | header | what the route writes |
|---|---|---|
| a fixed array of records in a record | `struct arr { struct cd a[2]; int32_t n; }`, bound `a: Cd[2]` | the top level with `a` covered whole, then a loop zeroing each element's gaps, count `sizeof(((struct arr *)0)->a) / sizeof(struct cd)` |
| a record two levels deep | `struct deep { char k; struct ocd2 { struct cd inr; char z; } o; }` | `k`, `o.inr.c`, `o.inr.d`, `o.z`, with C's union test at `o` and `o.inr` |
| an anonymous struct | `struct an { char k; struct { char x; int64_t y; }; }` | from the dump: `k`, `x`, `y` |
| `partial` with bit-fields | `struct bf { char a; unsigned b:3; unsigned :2; unsigned c:4; double d; }`, bound `a`, `d` | from the dump: `a`, byte `1` (b), bytes `1..2` (c), `d`; the unnamed bit-field adds nothing |
| `partial` with tail padding | `struct pt { char k; double d; int32_t extra; char tail[3]; }`, bound `k`, `d` | from the dump: `k`, `d`, `extra`, `tail` |
| a union whose arm is a padded record | `struct uk { int32_t kind; union { struct cd s; char one; }; }` | `kind`, `s.c`, `s.d`, `one`: bytes 4..7 and 9..15, padding in both arms |
| an element lent as a record | `w_cd(@xs[1])`, `xs: [Cd]` | at the call's own address expression, bounds check included |

On this Mac each prints the same with both compilers: `pn` `40 32 24 16 24 24 2.0 2.0 2.0`, `el`
`16 2.0`. **`nv.hero`, a `[Cd]` handed to C without `@`, is refused by the checker** with both
(`error[ffi_type]: [Cd] cannot cross the FFI boundary`), so the `@` buffer is the only way a record
array reaches C.

## A soundness defect in the vouched half, found, measured and repaired in the copy

**A group record whose C type is a union, bound naming one arm, is vouched by the screen**: a
union's initialiser `{0}` fills one member, so `-Wmissing-field-initializers` has nothing to say,
and `extern_union.hero` itself calls this *"the shape a binding to a single arm actually has"*.
`build/padc/uu.hero` over `uu.h`: `record UN` over `typedef union { char c; double d; } UN;`
naming `c`, and `record Nu tag nu` holding `via: V`, `V` a typedef'd union bound by `fd: i32`
over `{ int32_t fd; void *h; }`. C writes the wide arm, the record is lent again, C reads it back
(`heroes build`, this Mac):

| compiler | prints |
|---|---|
| today | `2.5 1234605616436508552` |
| route G as first built | **`0.0 1432778632`**: the wide arm zeroed, at exit 0 |
| route G repaired | `2.5 1234605616436508552` |

The first build spanned `c` (and `via.fd`) alone and zeroed the rest of the union: **the vetoed
class, in the half I had called sound**. My first report's prototype has the same defect (its
`spans_of` spans the declared fields of any vouched record, a union one included; unrun on it, a
reading of `padding.hero` in `compiler-engineer-work/`). The repair is C's answer and no reading of
mine: for every record taken by its declared fields, at the top and at each recursion, a span
`{offsetof(T, path), __builtin_classify_type(((T *)0)->path) == 13 ? sizeof(...) : 0}`, the
predicate `emit/extern_union.hero` already relies on, so a union is covered whole and a struct adds
a span of no bytes (`padding.whole_if_union`, +13 lines). A union laid out by the dump needs
nothing: its leaves are all its arms. After the repair `uni`, `unimid`, `rzpad` and `pc` print
today's values again (table above, re-run).

## MemorySanitizer, Linux x86-64 (`heroes-linux`, emulated, Debian clang 22.1.8)

The same script, the same two compilers built in the container (output `build/padc/msan-x86-ce2.txt`,
tabulated by `build/tab.sh`; run 12:44 to 13:19). Columns `-O0`/`-O1`/`-O2`/`-O3`/`-O2 -fno-inline`:

| probe | shape | today | route G recursive |
|---|---|---|---|
| `ctlprobe` | positive control | 1/1/1/1/1 | 1/1/1/1/1 |
| `small` (the critic's), `small_rz` | lent local after a by-value call | 3/0/0/0/0 | **0/0/0/0/0** |
| `cret` | C returns the record by value | 3/0/0/0/0 | **0/0/0/0/0** |
| `fieldlend` | a field of a Heroes record | 5/2/2/2/2 | **0/0/0/0/0** |
| `rzpad` | an anonymous union (`PU`) and a `partial` record (`PP`) | 4/0/0/0/0 | **0/0/0/0/0** |
| **`pc`** (the critic's) | tail padding `:42`, **record in a record `:45`**, field `:48`, **array of records lent `:50`** | **6/5/5/5/6**: 1 at each of 42, 45, 48 at every level, 50 two at `-O0` and `-fno-inline`, one above | **0/0/0/0/0** |
| `uni`, `unimid` (the critic's), `uu` (mine) | union arms the binding does not name | 0, prints `1234605616436508552` | 0, prints `1234605616436508552` (and `uu` `2.5`) |
| **`pn`** (mine) | the shapes beside | 6/5/5/5/5, lines 70 to 80 | **1/0/0/0/0**, line 76 (`struct bf`, bit-field padding bits) |
| **`el`** (mine) | an element `@xs[1]` lent as a record | **1/1/1/1/1**, line 16 | **0/0/0/0/0** |
| five `ffi-rest-zero-*` goldens | | 0 but `partial-record` 2/0/0/0/0; every `.expected` the same | **0 at every level**; every `.expected` the same |

What the probes print shows it without a sanitizer: `cret` writes the record's bytes to stdout and
today prints **`y@16`** at `-O0`, `fieldlend` **`z@@@48`** at `-O0` and **`YU@@@48`** at `-O2
-fno-inline`, a padding byte showing as a character; under the route every level prints `@16` and
`@@@48`. So the two shapes the critic measured open on the prototype at every level (`pc.hero:45`,
`:50`) read 0 at every level, and so does one no seat built that loses at every level today
(`el.hero:16`, an element of a Heroes array lent as a record).

**The value probes, no sanitizer, `heroes build -O0` (the default) and `-O2`, three runs each:**

| probe | today `-O0` | today `-O2` | route `-O0` | route `-O2` |
|---|---|---|---|---|
| `dv.hero` (the critic's dirtied stack) | `true 0 7` three times | `true 0 0` three times | **`true 0 0`** three times | `true 0 0` three times |
| `main.hero` (the ffi-pragmatist's) | `memcmp -224 0 -122`, `sha false true false`; then `memcmp 0 -121 -15`, `sha true false false`; then the first again (`eq true true true` each) | `memcmp 0 0 0`, `sha true true true` three times | **`memcmp 0 0 0`, `sha true true true`** three times | the same three times |

So at the level `heroes build` uses by default, two records `==` calls equal give two SHA-256
digests today, differently from one run of the same binary to the next, and the stack's own `0xAA`
reaches C seven bytes at a time; under the route both read equal and zero, every run.

## MemorySanitizer, Linux arm64 (`heroes-linux-arm64`, Debian clang 22.1.8)

`build/padc/msan_ce2.sh` in the container, the copy mounted read-only: today = the seed at
`7e5c02048` with that commit's runtime (`git archive`), route = `build/g2c/heroes.c` (the route
compiler's own C, emitted by itself after the union repair) with the copy's runtime; each compiler
built there with `clang -O1`; each probe `--emit-c` as one file, then compiled with the build's
flags and `-fsanitize=memory -fsanitize-recover=memory` at five levels; reports counted by
`WARNING: MemorySanitizer`. Output `build/padc/msan-arm64-ce2.txt`, tabulated by `build/tab.sh`.
Columns: `-O0`/`-O1`/`-O2`/`-O3`/`-O2 -fno-inline`.

| probe | today | route G | output, both |
|---|---|---|---|
| `ctlprobe` (positive control) | 1/1/1/1/1 | 1/1/1/1/1 | `48` |
| `small`, `small_rz`, `cret`, `fieldlend`, `rzpad`, `pc`, `uni`, `unimid`, `uu`, `el` | 0 at every level | 0 at every level | the same, both |
| **`pn`** (the shapes beside, records C returns with uninitialised padding) | **6/5/5/5/5**, at lines 70, 72, 74, 76 (`-O0` only), 78, 80 | **1/0/0/0/0**, at line 76 | `40 32 24 16 24 24 2.0 2.0 2.0` |
| the five `ffi-rest-zero-*` goldens | 0, every `.expected` the same | 0, every `.expected` the same | |
| `dv.hero`, `heroes build -O0` and `-O2`, three runs | `true 0 0` six times | `true 0 0` six times | |
| `main.hero` (the ffi-pragmatist's), `-O0` and `-O2`, three runs | `eq true true true`, `memcmp 0 0 0`, `sha true true true` six times | the same six times | |

So on arm64, where no by-value crossing loses bytes, the padding C itself leaves uninitialised
(`pn.hero`'s `mk_*` write members only) reaches C again through every Heroes lend today, at every
level, and the route closes it but for line 76: `w_bf(@b.h)`, `struct bf`'s padding **bits** inside
the bytes its named bit-fields hold, which the route leaves on purpose (what stays open, item 2),
here measured at `-O0` and not above.

## The suites, the route's compiler judged by itself

`build/heroes-g run tests/harness/main.hero -- build/heroes-g <suite>`, each into a file read whole
(`build/suites.sh`), after the union repair:

| suite | my first report's prototype (non-recursive G with Q2's B), same base | route G recursive |
|---|---|---|
| `emission` | 1,258 passed, 7 failed | **1,239 passed, 26 failed** |
| `emit` | 12 passed, 0 failed | **12 passed, 0 failed** |
| `check` | 699 passed, 0 failed | **699 passed, 0 failed** |
| `layout` | *unrun* then; at `7e5c02048` the four modules count 300, 298, 299, 300 by the `awk` mirror | **5 passed, 1 failed**: `cli/header_types.hero` 302, `cli/layout.hero` 339, `emit/layout_text.hero` 321, `emit/ops.hero` 302 lines of code past 300, exactly the `awk` mirror's four |
| the compiler's own tests, `heroes-g test selfhost/main.hero` | 1,653, all passed | **1,654 tests, all passed**, exit 0 |
| `run` | 518 passed, 0 failed | **518 passed, 0 failed**, two skipped for `sys/prctl.h`, which this Mac lacks (12:52 to 13:34, load up to 101 from other sessions): no program's output moved, the 26 moved emissions included |

**Every line the 26 moved emissions changed, read by script** (`build/moved.sh`: each failed label
re-emitted by `heroes-g build <case> --emit-c`, `diff` against its blessed file, each line
classified): **+423 -59**; the 423 added are **91** zeroing statements (`{ static const size_t
hero_pad_sN[] = {...}; hero_pad_zero(...);`), **91** closing braces and **241** `#line`
directives, **0 other**; the 59 removed are all `#line` restores, **0 other**. The records zeroed,
counted over the 26 (`grep -o 'hero_pad_zero(...'`): `struct keeper` 16, `SHA256state_st` 9,
`struct one` 7, `z_stream_s` 6, `SA` 6, `regex_t` 3, `SDL_Event` 2, `hero_mutex` 2, `lock_t` 2,
`struct pfd`, `struct pad`, `struct outer`, `struct big`, `pattern_t` 2 each, `uv_timer_s`,
`rlimit`, `linger`, `addrinfo`, `span`, `slot`, `pair`, `UD`, `SP` 1 each, and two array lends with
a count of elements (`struct pollfd`, `struct pair`). **My first report's prediction, *"`emission`
moves at most 10 blessed emissions"*, is falsified by the recursive route: 26.** The 19 more than
the prototype are the records it skipped: `partial` ones (`SHA256state_st`, `z_stream_s`,
`regex_t`), those with an anonymous member (`SA`, `SDL_Event`), and the array lends.

## What stays open, named (each a reading of the route as built unless a probe says otherwise)

1. **An array of C structs the binding does not declare as a fixed array of a group record** (an
   undeclared member of a `partial` record, or a member of a dumped record whose element type the
   program does not bind): the dump prints it as one leaf (`struct cd[2] a`, measured above), so it
   is covered whole and its elements' padding is left. Reaching it would need a second dump of the
   element type, `__typeof__(((T *)0)->a[0])`, once the first dump has named the member: a second
   clang run, unbuilt.
2. **Padding BITS inside a byte a named bit-field holds** (`struct bf`'s bits 3 and 4 of byte 1 and
   bits 1 to 7 of byte 2): the route writes whole bytes and never a byte a member holds, so those
   bits stay what they were. Zeroing them is a read-modify-write of a member's byte.
3. **A union taken by its declared arms is covered whole** (the repair above), so the bytes past its
   widest arm (`union { char c[5]; int32_t i; }`, bytes 5 to 7) are left. A union laid out by the
   dump has them zeroed.
4. **A record passed to C BY VALUE** is not a lend and the route does not reach it (above).
5. **Bytes dirtied after the zeroing and read by C through an address it kept** (a libuv handle, a
   `z_stream`): the route zeroes before each `extern` call that lends the place; a Heroes store into
   the place between two calls (a by-value assignment at `-O0` copies the source's padding) reaches
   a C reader that does not go through a call (a callback, another thread) dirty. *A reading,
   unrun.*
6. **Threads**: the route writes only bytes no member holds, so a mutex another thread is locking
   keeps its bytes; a padding byte written while another thread copies the whole struct is a race
   on bytes no member holds (my first report's residue, unchanged).
7. **A record whose dump this reader cannot read** (the control record fails, no block, a
   bit-field's column of another shape) gets nothing written: today's behaviour, and silent.
8. **Every `partial` record is now dumped** (one more clang run on a cold build of a program that
   binds one, cached after): the depth-cube the screen exists to avoid (`layout_screen.hero`'s
   head, defect 140) reaches a `partial` record holding a deep by-value nest. Its cost on such a
   program is *unrun*.
9. **The 300-line ceiling**: by the `layout` suite's own count (non-blank lines outside `test`
   blocks, mirrored by `awk`), four modules cross it: `emit/layout_text.hero` 299 to 321,
   `emit/ops.hero` 300 to 302, `cli/layout.hero` 298 to 339, `cli/header_types.hero` 300 to 302
   (the suite's own verdict, in the suites' table above, names the same four). A landing moves `lay` and `last_byte` into `padding.hero` or a
   module of their own.
10. **Windows** has no MemorySanitizer target (my first report); the critic's sentinel shows the box
    keeps the bytes, so there the route writes zeros over zeros. **macOS x86-64**: *unrun*, no such
    machine.
11. **The run-time cost of a lend**: the recursive form adds C's union test as a span of no bytes
    per recursion and keeps the out-of-line `hero_pad_zero` (gaps found once per call, O(spans²));
    *unmeasured* this pass (my first report: about 15 ns a lend out of line, 5 ns inline, load 8).

## The compiler's own cost: a cold `--emit-c` of the compiler

`build/cold.sh`: each run from an empty build directory (`heroes` takes `build/` under the working
directory, `cli/toolchain.hero:132`), `heroes build <copy>/selfhost/main.hero --emit-c -o out.c`,
under `/usr/bin/time -p`, today and the route alternating, the load read before and after each.
**First pass, 12:33 to 12:40** (other seats' builds and a lane's tests running, load 3.6 to 5.1):

| run | today (`heroes`, the seed built by plain `clang`) user + sys | route (`build/heroes-g`, built by `heroes build`) user + sys |
|---|---|---|
| 1 | 75.37 + 15.86 = **91.23 s** (real 99.31) | 76.76 + 14.96 = **91.72 s** (real 96.61) |
| 2 | 77.81 + 16.36 = **94.17 s** (real 99.23) | 85.43 + 17.08 = **102.51 s** (real 107.96) |

Both emitted **the same 109,535,616 bytes** (`cmp` silent): the compiler binds no group record, so
the route changes nothing in its own C. **This pass compares two binaries built two ways** (the
seed under `clang`'s defaults, the route under `heroes build`'s flag list), so it does not isolate
the route; a second pass with today's compiler built by `heroes build` from pristine sources
(`git archive 7e5c02048 selfhost runtime`, `build/heroes-today`) is below.

**Second pass, 13:40 to 13:50, like with like** (`build/cold2.sh`: today = `build/heroes-today`,
built by `heroes build` from `git archive 7e5c02048 selfhost runtime`; route = `build/heroes-g`;
load 8.1 falling to 5.4, other sessions' gates and a `git pack-objects` running):

| run | today user + sys | route user + sys |
|---|---|---|
| 1 | 120.20 + 20.34 = **140.54 s** (real 149.54) | 119.88 + 20.41 = **140.29 s** (real 147.13) |
| 2 | 109.88 + 22.99 = **132.87 s** (real 138.08) | 112.40 + 24.30 = **136.70 s** (real 142.49) |

Both wrote the same 109,563,954 bytes (`cmp` silent). Means: today 136.71 s, route 138.50 s, a
difference of +1.79 s (+1.3%), smaller than the 7.67 s between today's own two runs.

**Third pass, 13:50 to 14:01, two more pairs the same way** (`build/cold3.sh`, load 5.3 to 7.8):

| run | today user + sys | route user + sys |
|---|---|---|
| 3 | 112.94 + 24.35 = **137.29 s** (real 143.48) | 111.84 + 24.66 = **136.50 s** (real 142.52) |
| 4 | 111.39 + 24.93 = **136.32 s** (real 142.17) | 108.69 + 21.88 = **130.57 s** (real 136.22) |

**Over the four like-with-like runs: today 136.75 s mean (sd 3.16), the route 136.01 s (sd 4.03),
a difference of −0.74 s (−0.5%)**, inside each compiler's own spread, every run's output the same
109,563,954 bytes from both. The condition my first report set (*"my G turns to veto ... where a
cold `--emit-c` of the compiler costs over 1% CPU"*) is not met. The two passes' sizes differ
(109,535,616 then 109,563,954) because the input did: the copy's own `padding.hero` grew by the
union repair between them, and that file is part of the compiler being emitted (`grep -c` of the
build directory's name in the output: 0, so not a path).

## Where the files are

Kept in `docs/panel/215-reports/compiler-engineer-work/build/`, no binary: `route-g-recursive.diff`
(the whole route against `7e5c02048`, the new `padding.hero` included) and `padding.hero`;
`probes/` (`pn`, `el`, `nv`, `uu`, mine); `dump-shapes/` (the layout dump's shapes and the
`__builtin_clear_padding` probe); `scripts/` (`msan_ce2.sh`, `cold.sh`, `cold2.sh`, `cold3.sh`,
`moved.sh`, `tab.sh`, `suites.sh`); `out/` (both legs' sanitizer runs, the three timing passes,
every suite's output, `moved.txt`); `moved/` (the 26 emission diffs the script read). The critic's
probes it ran are theirs, in `critic-work/pass2/padc/` and `mech/main/`. Nothing was written
outside the repository's root (the containers' `/tmp` is the container's); the copy is removed
with `git worktree remove --force` at the end.
