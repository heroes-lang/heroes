# Panel 190, the ffi-pragmatist's report

Begun 2026-10-04 at 08:09:40 (read from `date`), by the ffi-pragmatist, in
the soundness lane. Brief: `docs/panel/190-briefs/ffi-pragmatist.md`, bound by
`00-shared.md`, after the critic's first pass
(`docs/panel/190-reports/completeness-critic-briefs.md`).

My copy: `<scratchpad>/190-ffi-pragmatist/`, made with `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 703af779 | tar -x -C <copy>`.
Seed sha256 read in the copy: `2d55c5ff8309b812087f...` (the brief's prefix
holds). Compiler built in the copy from the seed, `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`, exit 0, Apple clang 21.0.0
(clang-2100.3.34.2).

Written as I go; each section names the command that settles it.

## 1. Item 1 by hand: the C a shared exit implies, on this Mac

Written 08:20:27 (`date`). All files under `<copy>/p190/`.

**The function** (`p190/item1/small.hero`): `pick(@count: i64, a: str, b:
str, early: bool) -> str`, two strings, an `@` parameter written, an early
`return s1` and a tail `return s2 + s1`; `main` calls it twice and prints the
count. The trunk's compiler emits it (`heroes build --emit-c`, exit 0) with six
swept slots (`s1`, `s2`, `$own6` to `$own9`) and two returning blocks, each
carrying `*ph0_count = h0_count;`, the rule-4 `hero_str_incref`, six
`hero_str_decref` and its own `return`.

**The hand-written shapes**, every one made from the trunk's own C so nothing
else differs:

- `small.shared.c`: the proposal. `p190/bin/exit_shared.py` rewrites each
  returning tail (copy-outs, the retain of the returned value, the sweep,
  `return`) into `hret = tN; goto hexit;` and writes the tail once at `hexit`,
  with `HeroStr hret;` first in the prologue. The script refuses (exit 2) any
  function whose tails differ in copy-outs, retain spelling or sweep; none
  did. **The C-only cleanup label is this same C**: it differs from the
  proposal in the IR (`--dump-ir` keeps the product) and not in one emitted
  byte, so every C measurement below is both routes'.
- `small.shared0.c`: the same with `hret = {0}`.
- `small.ladder.c`: a cleanup ladder written by hand. Each return stores
  `hret`, retains it and jumps to the rung of the latest slot its path may
  have written; rungs fall through. **Found by writing it**: `return s1` may
  have written `$own9` and never `$own8`, `return s2 + s1` the reverse, so
  one total order gives one of the two paths a rung for a slot it never
  wrote (null there, a no-op). A ladder is exact only on straight-line code;
  past a branch it is a may-be-written approximation, and past a loop it
  needs the analysis panel 021 refused.

**Compiled under the compiler's own words**: `p190/bin/cc.sh` passes
`flags()` (`selfhost/cli/flags.hero:91-109`, checked word for word against
the file by a script: 16 of 16, in order), the level, and
`-fsanitize=address,undefined` when sanitising, to the runtime and the unit
alike, and links with `-rdynamic` (`toolchain.hero:226-232`,
`compiling.hero:69-72`, `link.hero:128-146`).

**Apple clang 21.0.0 (clang-2100.3.34.2), this Mac**, `p190/bin/matrix.sh`:

| shape | `-O0` | `-O2` | `-O0` + ASan/UBSan | `-O2` + ASan/UBSan |
|---|---|---|---|---|
| trunk | exit 0, 0 warnings | exit 0, 0 | exit 0, no report | exit 0, no report |
| shared exit (= C-only label) | exit 0, 0 | exit 0, 0 | exit 0, no report | exit 0, no report |
| shared exit, `hret = {0}` | exit 0, 0 | exit 0, 0 | exit 0, no report | exit 0, no report |
| ladder | exit 0, 0 | exit 0, 0 | exit 0, no report | exit 0, no report |

All sixteen print the same bytes (`p-one`, `q-twop-one`, `2`; sha256
`082282ae693d...` for each), and the leak gate (`hero_runtime_check_leaks()`
in the generated `main`) stays silent in each. `-Werror=uninitialized` and
`-Werror=conditional-uninitialized` accept an `hret` with no initialiser: every
path into `hexit` writes it, and clang sees that. LeakSanitizer does not exist
here (`.claude/rules/c-boundary.md` § The instruments); the leak gate is
Darwin's instrument.

**The controls, so a pass means something** (`ctl.dropped.c`: the shared
exit with `hero_str_decref(h4_s1)` removed; `ctl.doubled.c`: with it written
twice):

- dropped: **exit 134 in all four configurations**, `panic: 2 heap blocks
  still live at exit (a missing decref)`, the message going on to call it a
  compiler bug;
- doubled: exit 134 at `-O0` and `-O2` with the runtime's own `a str's block
  has lost its mark` panic, and under ASan `heap-use-after-free`, **caught in
  `memchr` inside `fwrite`**, an intercepted libc call, not in the program's
  code. That is the instrument's reach in one line: ASan sees a freed block
  read by instrumented code or by a libc function it intercepts, and nothing
  else (section 3 below runs the case it cannot see).

## 2. Item 2: what an exit's order means for memory C still reads, this Mac

Written 08:26:34 (`date`). Files under `<copy>/p190/item2/`.

**The premise, from the spec and not from the brief.** Spec § FFI: *"A lend
lives for its call and no longer: a parameter is taken to keep what it is
handed unless declared `lent`, and a lend reaches only one so declared."* So a
correct binding never lets C keep a `str`'s bytes; what C keeps is a lease,
a copy the program ends itself. A *retained lend* exists only where a
binding's `lent` is false of its function, the shape M-agreed-retention
measured on `sqlite3_bind_text` with a null destructor (`docs/records/done/2026-09-25-1151-one-declaration-cannot-reach-all-three-of-sqlite3-bind-text-modes.md`:
*"a `lent` declaration passed a `nullptr` destructor read the bytes of a later
allocation at exit 0"*). For that program **the moment a `str` is released
is the whole difference between right and wrong**, and the trunk's moment is
panel 122's, in `docs/records/log/2026-09-09-0004-what-actually-keeps-a-lend-alive-and-why-no-position.md`:
*one synthetic owner slot per expression site, released at function exit or
when its site re-executes, whichever comes first*. (`.claude/rules/c-boundary.md`,
which my brief cites for *a retained lend*, does not contain the phrase:
`grep -n -i 'retain'` there finds nothing.)

**The program** (`keeper.hero`), against a C library of my own, `keep.h`
and `keep.c`, compiled **without** sanitizers as a system library is: it KEEPS
the pointer it is handed and later walks it a byte at a time, as SQLite's own
UTF-8 length loop does, never through a libc function a sanitizer intercepts.
The binding declares `text: cstr lent`, false on purpose. `lend_then` has
three exits (two early returns, one tail); `two_arms` lends from two arms of
an `if` inside a loop, each an owning temporary in a synthetic slot of its own
(the shape panel 182's deferred coalescing would give one slot); `main` prints
`lend_then(alpha, 1)`, `lend_then(beta, 2)`, `two_arms(one, two)`,
`lend_then(gamma, 0)`, then `keep_len()` after that last call has returned.
Right answers: `10 109 9010 -1 10`.

**Four C shapes of the same program**, all from the trunk's own C:

- `trunk`: as emitted;
- `shared`: `exit_shared.py` (three returns into one exit in `lend_then`);
  **this is also the C-only label's C**;
- `lastuse`: by hand, `text` and the temporaries released right after the
  lend, their last use, and nulled, the exits sweeping null: the release a
  *liveness-directed* sweep makes (panel 106 R5's word). The pruned-at-exit
  reading of "liveness" (panel 021's, which slots an exit releases) moves no
  release and is the ladder's case;
- `coalesced`: by hand, `two_arms`' two synthetic slots made one, so arm B's
  rule-3 store releases what arm A lent.

**Run**, each built with `cc.sh` at `-O0` plain and under ASan/UBSan, linked
with the uninstrumented `libkeep.a`:

| shape | plain | ASan/UBSan | ASan, `max_free_fill_size=4096:free_fill_byte=85` | Guard Malloc (`DYLD_INSERT_LIBRARIES=/usr/lib/libgmalloc.dylib`) |
|---|---|---|---|---|
| trunk | `10 109 9010 -1 0`, exit 0 | `10 109 9010 -1 10`, exit 0, 0 reports | `... -1 11`, exit 0, 0 reports | `10 109 9010 -1`, **exit 139** |
| shared exit (= C-only label) | `10 109 9010 -1 0`, exit 0 | identical to trunk | identical to trunk | identical to trunk, **exit 139** |
| release at last use | **`0 100 0 -1 0`**, exit 0 | `10 109 9010 -1 10`, exit 0, **0 reports** | `11 110 10011 -1 11`, exit 0, 0 reports | nothing printed, **exit 139** |
| coalesced synthetic slots | `10 109` **`10`** `-1 0`, exit 0 | `10 109 9010 -1 10`, exit 0, **0 reports** | `10 109 10010 -1 11`, exit 0, 0 reports | `10 109`, **exit 139** |

What it says, each line from the table:

- **The shared exit moves no release, on any path**: its four columns are the
  trunk's, byte for byte, including the trunk's own existing fault (the fifth
  value, C reading `gamma-kept` after `lend_then` returned, `0` for `10`). A
  return's store and jump put no C call between the last instruction of the
  path and the sweep, so the order of every C call against every `free` is
  the trunk's. The same holds for the C-only label (same C) and for a ladder
  (it releases at the same exits, a subset of the same slots).
- **The two routes that release earlier turn a working program into a wrong
  answer at exit 0**: release at last use breaks all three reads made while
  the function still runs (`0 100 0` for `10 109 9010`), and coalescing breaks
  `two_arms` (`10` for `9010`, the first arm's bytes freed by the second
  arm's store). Both are panel 106's finding again, now on the routes this
  sitting lists: *fewer slots* and *a sweep pruned by liveness* read as
  release-at-last-use.
- **ASan hides every one of them**: right answers, zero reports, exit 0, on
  all four shapes, because its quarantine keeps the freed bytes intact and
  the read is in uninstrumented code. Panel 106's carried warning holds
  word for word.
- **What sees it on this Mac**: **Guard Malloc**, which faults at the read
  itself (exit 139; lldb puts the coalesced case's fault at `walk(p="") at
  keep.c:8:12`, inside the C library); and ASan with its own
  `free_fill_byte`, which still reports nothing but turns the hidden wrong
  read into a visible wrong value (`11` where `10` is right). `MallocScribble=1`
  is NOT an instrument here: on a plain C probe (`uaf_probe.c`, malloc, keep,
  free, read) it printed `1` with and without `MallocNanoZone=0`, the same as
  without it.

**The same, against the real library** (`sqlite_lend.hero`): the SDK's own
`sqlite3.h` and `-lsqlite3`, `sqlite3_bind_text(..., destructor: nullptr)`
with `text: cstr lent` (false under `SQLITE_STATIC`), `select length(?1)`
computed inside SQLite. `bound_length` has four exits (a refused bind at
column 5, a bind left for the caller to step, a step finding no row, a step
here); `two_arms` binds `?1` and `?2` from two arms of a loop. Right answers:
`21 -1 0 21 9010`. Apple clang 21, `-O0`:

| shape | plain | ASan/UBSan | ASan + fill byte 85 | Guard Malloc |
|---|---|---|---|---|
| trunk | `21 -1 0 0 9010`, exit 0 | `21 -1 0 21 9010`, exit 0, 0 reports | `21 -1 0 22 9010`, exit 0 | `21 -1 0`, exit 139 |
| shared exit (= C-only label) | identical to trunk | identical | identical | identical, exit 139 |
| release at last use | **`0 -1 0 0 0`**, exit 0 | `21 -1 0 21 9010`, exit 0, **0 reports** | `22 -1 0 22 10011` | nothing printed, exit 139 |
| coalesced synthetic slots | `21 -1 0 0` **`10`**, exit 0 | `21 -1 0 21 9010`, exit 0, **0 reports** | `21 -1 0 22 10010` | `21 -1 0`, exit 139 (the trunk's own fault comes first) |

The trunk's fourth value is today's fault under a false `lent`: SQLite steps
on `text` after `bound_length` returned and reads `0` for `21`. Every route
that releases at the exit leaves it exactly where it is; the two that release
earlier add faults where the trunk is right, and ASan, on SQLite as on my
keeper, returns the right answers and reports nothing.

## 3. Items 1 and 2 on Linux arm64 (Debian clang 22.1.8 and 18.1.8)

Written 08:31:44 (`date`). One container of `heroes-linux-arm64:latest`
(`docker ps -q` empty before it), my copy mounted read-only, the inner script
`p190/bin/linux-inner.sh`, log `p190/linux/arm64-items12.log`. Clang 22.1.8 is
the image's; clang 18.1.8 (`18+b1`) and Valgrind were installed in the
container by `apt-get`, the platform scripts' own way. **The seed, built in
the container, emits `small.hero`, `keeper.hero` and `sqlite_lend.hero` byte
for byte as this Mac's compiler did** (`cmp`, three of three, under both
clangs), so the hand shapes made here are the C that platform would compile.

**Item 1, both clangs**: the sixteen builds read as on the Mac, 0 warnings
each, exit 0, the same sha256 of the output; the controls fire the same way
(the dropped release: the leak gate's panic, exit 134, in all four
configurations; the doubled one: the runtime's mark panic at exit 134, and
ASan's `heap-use-after-free ... in fwrite` at exit 1). LeakSanitizer runs
inside ASan here and reported nothing on the trunk's `pick` (exit 0, zero
`LeakSanitizer` lines); it never reached the dropped control, which the
runtime's own gate aborts first.

**Item 2, clang 18.1.8**; clang 22.1.8 printed the same five columns, the
Valgrind counts included, in a third pass once Valgrind was installed:

| program, shape | plain | ASan/UBSan | ASan + fill 85 | `MALLOC_PERTURB_=85` | Valgrind memcheck |
|---|---|---|---|---|---|
| keeper, trunk | `10 109 9010 -1 10` | same, 0 reports | `... -1 11` | same as plain | **1 invalid read**, `walk (keep.c:8)`, exit 99 |
| keeper, shared exit | identical | identical | identical | identical | **1**, identical |
| keeper, last use | `10 109` **`10010`** `-1 10` | `10 109 9010 -1 10`, 0 reports | `11 110 10011 -1 11` | as plain | **5** |
| keeper, coalesced | `10 109 9010 -1 10` | same, 0 reports | `... 10010 -1 11` | as plain | **2** |
| SQLite, trunk | `21 -1 0 21 9010` | same, 0 reports | `21 -1 0 22 9010` | same as plain | **2**, in `libsqlite3.so.0.8` |
| SQLite, shared exit | identical | identical | identical | identical | **2**, identical |
| SQLite, last use | `21 -1 0 21` **`10010`** | `... 9010`, 0 reports | `22 -1 0 22 10011` | as plain | **6** |
| SQLite, coalesced | `21 -1 0 21 9010` | same, 0 reports | `... 10010` | as plain | **4** |

(Valgrind's own run prints the right values, its freed blocks being held
back; `--error-exitcode=99` is what makes its findings an exit status.)

What Linux adds to the Mac's answer:

- **On glibc the trunk's own fault is invisible in a plain run**: `21` where
  Darwin printed `0`, right by accident, because glibc's free writes its
  bookkeeping over the block's header and not over the text. So a program
  with a false `lent` can pass on one leg and fail on another, today.
- **`MALLOC_PERTURB_` sees nothing here** (every column as plain), so neither
  allocator's scribble option is an instrument on these two legs.
- **Valgrind is the instrument that names it**: every invalid read, in the
  library, with the free that preceded it, and the counts move exactly where
  a route moves a release. The shared exit's counts equal the trunk's on
  both programs; the two earlier-release routes add three to four reads.
- **Coalescing's fault is invisible on Linux in the plain run** (`9010`, right
  by accident) and visible on Darwin (`10`): a shape a Linux leg passes and
  a Darwin leg fails, or the reverse, with ASan blind on both.

## 4. Item 3: the shape at `-O2` and under `--sanitize`, this Mac (first part)

Written 08:37:57 (`date`). **The bounds, named before the first run**
(`p190/item3/chain-trunk.sh`, written 08:22:47): each build must finish
within **1,800 s of wall clock** and **8 GiB (8,388,608 KiB) of resident
memory summed over the build's process tree** (the compiler and the clang it
starts), held by `p190/bin/bounded.sh`, which polls the tree every 3 s, stops
it at either bound, and writes FINISHED or NOT FINISHED with the bound that
stopped it. No duration is written anywhere (the shared brief). The shapes
are the emit lane's, copied and checked identical (`slots-returns-{100,200,400,800}.hero`).

**The trunk's compiler, `heroes build <shape> -O2` (the level `heroes run`
uses) and `heroes build <shape> --sanitize` (`-O0` with ASan/UBSan)**, one
build at a time, each binary run after:

| N | `-O2` | peak tree RSS seen | `--sanitize` | peak tree RSS seen |
|---|---|---|---|---|
| 100 | finished, exit 0, prints `3` | (between two polls) | finished, exit 0, prints `3` | 228,672 KiB |
| 200 | finished, exit 0, prints `3` | 740,400 KiB | finished, exit 0, prints `3` | 672,752 KiB |
| 400 | finished, exit 0, prints `3` | 2,705,984 KiB | finished, exit 0, prints `3` | 2,659,872 KiB |
| 800 | **finished**, exit 0, prints `3` | **7,058,224 KiB** | running | |

**So clang at `-O2` finishes the 800 shape on this Mac within the bound**; the
binary is 5,354,272 bytes. What stands near the bound is memory: 6.7 GiB of
the 8 GiB at peak, of which the compiler process itself held about 1.75 GB
(read with `ps` while clang ran: `heroes` 1,752,624 KiB beside a clang `-cc1`
at 3,508,992 KiB and climbing). Lane irverify's *"at 800 clang does not finish
in 300 s"* is not contradicted: that is a duration, at a level it does not
name, and I took none.

**The shared exit, made by hand from the trunk's own artifacts**
(`heroes build <shape> --emit-c -O2`, which hits the `-O2` objects already in
the cache, so no second compile; the artifacts read 97,108, 374,008 and
1,467,808 lines with 30,603, 121,203 and 482,403 `hero_str_decref(`, the
shared brief's table cell for cell), then `exit_shared.py`:

| N | trunk lines | shared-exit lines | trunk `hero_str_decref(` | shared-exit `hero_str_decref(` |
|---|---|---|---|---|
| 100 | 97,108 | **6,514** | 30,603 | **603** |
| 200 | 374,008 | **12,814** | 121,203 | **1,203** |
| 400 | 1,467,808 | **25,414** | 482,403 | **2,403** |

The release count is exactly **6N + 3**, the critic's unrun figure, now run:
3N rule-3 store releases, 3N in the one sweep, two in `main`, one in the
optional's generated release helper. The lines are 63N + 214 on these three.
Each built with `cc.sh` at `-O0`, `-O2`, and both under ASan/UBSan: twelve of
twelve exit 0, 0 warnings, the same output.

**Every path, not only the tail.** The shapes' `main` calls `f(3)`, which
takes only the last return. I replaced `main` in the C with a loop calling
`f` once for each of its N + 1 returns and releasing each result
(`sr{100,400}.{trunk,shared}.every.c`): at 100 the trunk and the shared exit
both print `803`, and at 400 the shared exit prints `3203` (N x 8 + 3), each
exit 0 at `-O0`, `-O2` and both under ASan/UBSan, no report, the leak gate
silent. So on this shape **each of the 401 paths releases its slots exactly
once under the shared exit**: a slot missed is the gate's panic and a slot
released twice is the mark's panic or ASan's report, and the controls of
section 1 show both firing.

## 5. Items 1 to 3's small shapes on the Windows box (clang 23.1.1)

Written 08:44:39 (`date`). Folder `/c/w/190-fp-win/` (new, mine; nothing
removed there or anywhere). The bundle (`p190/win/bundle1.tgz`, the runtime,
the seed, my scripts, the C files and the shapes) went in 1 MB parts, each
part's size checked on the box, as `<scratchpad>/platforms/windows2-u.sh`
does; sha256 `a6117e0ef5295540` on both sides; the seed's prefix
`2d55c5ff8309b812` read there. Log: `p190/win/win-items.log`. **The box**:
`clang version 23.1.1`, target `x86_64-pc-windows-msvc`, MSYS2 bash, **2
logical processors and 1,286,938,624 bytes of physical memory** (531 MB free
when read, `Get-CimInstance`). ASan links and catches a C use-after-free there
(exit 1); `-fsanitize=undefined` alone does not link (`clang_rt.ubsan_standalone`
lacks `CommandLineToArgvW` and a dozen `Sym*` symbols), but
`-fsanitize=address,undefined`, which is what `--sanitize` passes, does. No
`gflags.exe`, so no page heap.

- **The seed** builds with `seed/README.md`'s stack flag (0 warnings), and
  `heroes.exe` emits `small.hero` and `keeper.hero` byte for byte as this Mac's.
- **Item 1**: sixteen of sixteen exit 0, 0 warnings, the same output's sha256.
- **The controls, and a finding beside the sitting**: the dropped release is
  the leak gate's panic (exit 127 there) in all four configurations. **The
  doubled release is silent without ASan on Windows**: exit 0, the right
  output, at `-O0` and `-O2`, where Darwin and Linux both stopped it with the
  runtime's mark panic; under ASan it is `heap-use-after-free ...
  runtime\parts\str.c:77 in hero_str_hdr_checked`. Read from the code, not
  run: the mark (`str.c:76-86`) sees a freed block only where the allocator
  wrote over its first bytes, and a second decref that finds the count at 0
  writes -1 into freed memory and returns. So an over-release, by any route
  or by the trunk, is caught by ASan alone on that leg. It moves with no
  route; it is a property of the instrument every route's correctness leans
  on.
- **Item 2, the keeper** (no `sqlite3.h` on the box, so no SQLite there):

| shape | plain | ASan/UBSan | ASan + fill 85 |
|---|---|---|---|
| trunk | `10 109 9010 -1 10` (right by accident) | same, 0 reports | `... -1 11` |
| shared exit | identical | identical | identical |
| last use | `10 109` **`10010`** `-1 10` | `... 9010 ...`, **0 reports** | `11 110 10011 -1 11` |
| coalesced | `10 109 9010 -1 10` (hidden) | same, **0 reports** | `... 10010 -1 11` |

- **Item 3's shared-exit shapes**: `sr{100,200,400}.shared.c` twelve of
  twelve, and every return of the 100 shape (trunk and shared, `803`) and the
  400 shape (shared, `3203`), each at `-O0`, `-O2` and both under ASan/UBSan,
  exit 0, no report.

## 6. Q1's shapes at the C level, the shared exit by hand, this Mac

Written 08:44:39 (`date`). Files under `<copy>/p190/q1/`. The slots shape
returns only `str`, so I wrote `q1.hero`: a `[str]` returned from inside a
loop, a `{str: i64}` returned from `match` arms with a slot stored on one
arm alone, a record with a counted field after an `if` used as a value, a
variant with a counted payload behind a `.must()`, a `str?` with an `@`
parameter written and then a `?`, and a `()?` with an `@` parameter and a
`?`; plus the critic's two beside-shapes copied (`params-returns-20b.hero`,
20 `@` parameters by 21 returns; `try-chain-20.hero`, twenty `?`). The trunk
prints the right eighteen lines for `q1` (`seen` holds 3 after the `?`
returned early: the copy-out ran on that path).

`exit_shared.py` rewrote all fourteen functions with returns and refused
none, so the tails were uniform in every one: the retain at the exit is
`hero_array_incref`, `hero_map_incref`, `h_q1_Named_retain(&hret)`,
`h_q1_Token_retain(&hret)`, `h_0opt_..._retain(&hret)` as the type asks, and
the copy-out moves into the exit (in the 20-by-21 shape, 420 copy-out lines
become 20; its C from 3,183 lines to 1,531; the try chain from 10,528 to
2,838). For the beside-shapes I replaced `main` in the C with one that takes
every return (the chain for `x` from 0 to 19 and 99; the 20-parameter
function once per return, the same twenty cells each time).

All six (trunk and shared, three programs) built at `-O0`, `-O2` and both
under ASan/UBSan, twenty-four of twenty-four exit 0 with 0 warnings and no
report, the leak gate silent; `q1`'s eighteen lines identical between trunk
and shared exit; the chain prints `43` and `20` (one success of length 43,
twenty failures) and the 20-parameter shape `190` and `440` (the returns' sum
and twenty cells of `x` plus 21 `!`), each the same under both shapes. A
struct-typed `hret` with no initialiser (`h_q1_Named hret;`) is accepted by
`-Werror=uninitialized` and `-Werror=conditional-uninitialized` as the `str`
one was.

## 7. Item 3, second part: the 800 shape at `-O2` and `--sanitize`, and what the shared exit does to clang

Written 09:03:12 (`date`). Same bounds as section 4 (1,800 s, 8 GiB of tree
RSS, named at 08:22:47).

**This Mac, the trunk's compiler, `heroes build --sanitize`**: 100, 200, 400
and **800 finished** (800: exit 0, the binary prints `3`, peak tree RSS seen
**6,942,816 KiB**). With section 4's `-O2` row: **on this Mac the trunk's 800
shape builds at `-O2` and under `--sanitize` within the bound, at about 6.6 to
6.7 GiB of the 8.**

**This Mac, the shared exit at 800** (`sr800.shared.c`, made from the trunk's
own `--emit-c -O2` artifact of 5,815,408 lines and 1,924,803
`hero_str_decref(`): **50,614 lines and 4,803 `hero_str_decref(`**, 801
returns into one exit sweeping 2,400 slots. Built with `cc.sh` under the
bounds, all finished and ran right: `-O2` with the shape's `main` (prints
`3`), and with the every-return `main` at `-O2`, `-O0`+ASan/UBSan and
`-O2`+ASan/UBSan (each `6403`, 800 x 8 + 3, exit 0, no report, the leak gate
silent).

**The finding: the shared exit does not take the product out of clang at
`-O2`.** Its 50,614 lines peaked at **5,921,472 KiB** of tree RSS at `-O2`
(4,784,096 with the every-return `main`; 4,947,872 at `-O2` with the
sanitizers), against 7,058,224 for the trunk's whole build of 115 times the
text, which includes the compiler process's own 1.75 GB. At `-O0`, with or
without the sanitizers, the shared exit's build ended between two 3-second
polls. Linux arm64 says the same at 400: the trunk's `heroes build -O2`
peaked at 2,118,428 KiB and the shared exit's `cc.sh -O2` at 1,495,164 KiB,
and the shared exit's peaks grow about threefold per doubling (485,424 at 200,
1,495,164 at 400). **My reading of why, unrun**: at `-O2` clang promotes the
2,400 slot variables to SSA values, and at the one exit with 801 predecessors
each live slot needs a phi of 801 incoming values, so the product the C text
lost comes back as phi operands inside clang. If that is right, a ladder,
whose every rung has two predecessors, is linear inside clang as well;
section 8 runs it.

**Linux arm64, the same bounds** (`p190/linux/arm64-item3.log`, clang 22.1.8,
a container of 7,834 MB): the trunk's `heroes build -O2` finished at 100,
200, 400 and **800, at a peak tree RSS of 7,294,428 KiB**, 93% of the
container's memory; `--sanitize` finished at 100, 200 and 400 (800 running
when written). The shared exit's every-return runs print `803`, `1603` and
`3203` at `-O2` and under ASan/UBSan with LeakSanitizer, exit 0, no report.

## 8. What each exit's C costs clang at `-O2`, measured like for like

Written 09:09:51 (`date`). Section 7's comparison set a whole `heroes build`
against `cc.sh`; this one compiles each C file the same way, clang alone
through `cc.sh` (the runtime, the unit, the link; the unit is nearly all of
it), one build at a time on this Mac, under the same bounds
(`p190/item3/mem/`). **The ladder** is `p190/bin/ladder_slots.py`, written
from the shared exit's C for this shape: return k retains its value and jumps
to rung k, rungs fall through from N-1 to 0, each releasing one string's three
slots, so it releases exactly what a path wrote (6N + 3 releases written,
101 returns into 100 rungs at N = 100, 801 into 800 at N = 800). Every return of the ladder ran right
first: `803` at 100 in the four configurations, `6403` at 800 under
ASan/UBSan, no report, the gate silent.

| clang `-O2`, peak tree RSS seen | trunk's C | shared exit | ladder |
|---|---|---|---|
| N = 400 | 1,258,752 KiB | **1,765,840 KiB** (+40%) | 1,169,728 KiB (-7%) |
| N = 800 | 4,687,008 KiB | **5,921,472 KiB** (+26%) | 4,663,984 KiB (-0.5%) |

Every one finished within the bounds and its binary printed `3`. The
lines being compiled at 800: 5,815,408, 50,614 and 52,213.

**So, measured: at the level `heroes run` uses, the proposal makes clang's
peak memory on this shape larger, not smaller**, although its text is 115
times shorter, and the ladder only matches the trunk. All three grow about
fourfold per doubling of N (trunk x3.72, shared exit x3.35, ladder x3.99
from 400 to 800). My phi reading of section 7 is therefore not the mechanism,
or not all of it; it stays unrun as a cause and I withdraw it as an
explanation. **A second reading, also unrun, that the numbers fit**: under
every route that releases at the exit, each of the 3N slots is live from its
store to the end of the function, so the live values summed over the
function's blocks grow as N squared whatever shape the exit has, and only a
route that releases before the exit shortens them, which is exactly what
section 2 shows breaking the C boundary. Section 9 removes one suspect,
`-g`, to test it.

**Linux arm64**, from the same log: the trunk's `heroes build --sanitize`
finished at 800 at a peak tree RSS of **7,588,512 KiB, 97% of the
container's 7,834 MB**; at `-O2` 7,294,428 (section 7). So on that leg the
800 shape builds under both, with 3% and 7% of the container's memory to
spare.

**Corrected 09:10:19 (`date`), two percentages above.** `free -m` in the
container reads MiB, so its 7,834 are 8,022,016 KiB: the trunk's 800 shape
peaked at **91%** of the container at `-O2` (7,294,428 KiB; section 7 said
93%) and at **95%** under `--sanitize` (7,588,512 KiB; section 8 said 97%),
leaving 9% and 5% to spare, not 7% and 3%. The KiB figures stand.

## 9. Linux arm64 item 3 complete, and the first half of the `-g` experiment

Written 09:15:09 (`date`).

**Linux arm64, clang 22.1.8, the whole of item 3** (`p190/linux/arm64-item3.log`;
the container's memory is 8,022,016 KiB):

| N | trunk `-O2` | trunk `--sanitize` | shared exit `-O2` (`cc.sh`) | shared exit, every return |
|---|---|---|---|---|
| 100 | finished, 200,412 KiB | finished | finished | `803`, `-O2` and ASan/UBSan+LSan, no report |
| 200 | finished, 542,720 | finished, 597,308 | finished, 485,424 | `1603`, same |
| 400 | finished, 2,118,428 | finished, 2,235,616 | finished, 1,495,164 | `3203`, same |
| 800 | **finished, 7,294,428 (91%)** | **finished, 7,588,512 (95%)** | **finished, 6,254,968 (78%)** | `6403`, same |

Each trunk binary printed `3` at exit 0. The trunk's columns are whole
`heroes build` trees and the shared exit's are clang alone, so the rows
compare only loosely; section 8's like-for-like table is this Mac's.

**The `-g` experiment, 400** (`p190/bin/cc-nog.sh`: `cc.sh` without `-g`,
EXPERIMENT ONLY, not the compiler's words; this Mac, same bounds):

| clang `-O2` at N = 400 | trunk's C | shared exit | ladder |
|---|---|---|---|
| with `-g` (section 8) | 1,258,752 KiB | 1,765,840 KiB | 1,169,728 KiB |
| without `-g` | 1,480,496 KiB | **771,888 KiB** | **ended between two polls** |

So **for the two routes that put the sweep at one end, most of clang's
`-O2` memory is the debug information `-g` asks for**; for the trunk it is
not. My unrun reading of section 8 (live ranges to one shared end) fits this
only through `-g`: the variable locations clang tracks for slots that all live
to the same end. `-g` is on every build by `flags()`'s own comment (*"design.md
§2 promises lldb steps .hero lines"*), so the with-`-g` row is the one a
program pays. The 800 row is running.

## 10. The `-g` experiment complete, and the compiler's own C under the shared exit

Written 09:17:43 (`date`).

**Clang `-O2`, peak tree RSS seen, this Mac, clang alone, same bounds; every
build finished and its binary printed `3`:**

| | trunk's C | shared exit | ladder |
|---|---|---|---|
| N = 400, `-g` (the compiler's words) | 1,258,752 | 1,765,840 | 1,169,728 |
| N = 400, no `-g` (experiment) | 1,480,496 | 771,888 | (between two polls) |
| N = 800, `-g` (the compiler's words) | 4,687,008 | **5,921,472** | 4,663,984 |
| N = 800, no `-g` (experiment) | 4,238,240 | 3,168,768 | **153,568** |

Read across: **without `-g` the ladder is linear inside clang** (153,568 KiB at
800, thirty times below the others) **and the shared exit is not** (fourfold
per doubling, 771,888 to 3,168,768), which is what my first reading in
section 7 predicted from the join of 801 predecessors against rungs of two,
so I reinstate it as consistent with the data, still unrun as a direct cause
(I have not read clang's IR). **With `-g` all three are dominated by the
variable locations of 3N long-lived slots** and the ladder only matches the
trunk. design.md Part 2 (`docs/design/design.md:630-632`): *"No typed
variable inspection in v1. Line-level debugging works ... But `p x` shows a
mangled C temporary"*: the memory goes on what the design calls a non-goal.
Section 11 measures `-gline-tables-only` (line tables kept, variable
locations dropped) as an experiment, and the block shape under the real `-g`.

**The compiler's own C** (`p190/seed/`, chain `chain-seed.sh`):
`exit_shared.py` over `seed/heroes.c` **rewrote 2,777 functions and refused
none**, so every function of the compiler has uniform returning tails;
1,358,630 lines become **1,089,637** (19.8% fewer); `hero_str_decref(` 53,284
to 22,741, `hero_array_decref(` 20,850 to 12,877, `hero_map_decref(` 990 to
692, generated `_release(` 60,037 to 24,262, copy-out lines 2,208 to 878.
Compiled under `flags()` with `cc.sh`, same bounds:

| the seed, clang alone, peak tree RSS seen | trunk's seed | shared-exit seed |
|---|---|---|
| `-O0` | 896,224 KiB, 0 warnings | **776,064 KiB** (-13%), 0 warnings |
| `-O2` | 3,473,920 KiB, 0 warnings | **3,050,960 KiB** (-12%), 0 warnings |

So on a real program the shared exit **lowers** clang's memory at both levels,
and all 2,777 `hret` declarations pass `-Werror=uninitialized` and
`-Werror=conditional-uninitialized` with no initialiser. Section 8's
regression belongs to the extreme shape (hundreds of returns over thousands of
slots in one function), where `-g`'s variable locations dominate.

**Corrected at the time printed by `date` just before this line was
appended** (09:17 to 09:18): section 10's *"thirty times below the others"*
is wrong; 153,568 KiB is **20.6 times** below the shared exit's 3,168,768 and
**27.6 times** below the trunk's 4,238,240 (`python3`, from the cells).

## 11. The fixpoint under the shared exit, the block shape, and a slot's address at C

Written 09:19:47 (`date`).

**The fixpoint holds under the shared exit.** The compiler built from the
rewritten seed (`p190/seed/heroes-shared-O0`, `cc.sh` under `flags()` at
`-O0`; 2,777 functions, every returning tail moved to one exit) ran
`build selfhost/main.hero --emit-c -o p190/seed/again.shared.c`, exit 0 with
its own leak gate silent, and **`cmp seed/heroes.c p190/seed/again.shared.c`
is silent**: it emits the trunk's seed byte for byte (`seed/README.md:168-172`'s
check). Its own tests (`heroes test selfhost/main.hero`) are running.

**The block shape** (`p190/bin/slots_block.py`): the shared exit with all 3N
counted slots of `f` in one `HeroStr hslots[3N] = {0}`, each slot name a macro
for its element, and the exit's sweep one loop. It is the C of the critic's
*one sweep over the function's counted slots held in one addressable block*.
Every return right under ASan/UBSan (`803` at 100, `6403` at 800, no report,
the gate silent), and **clang `-O2` with the compiler's `-g` peaks at
227,632 KiB at 800** (400 ended between two polls): about 20 times below the
trunk's 4,687,008 and 26 times below the shared exit's 5,921,472, the only
exit shape here that is cheap inside clang under the compiler's own words.

**A slot's address does reach C, which I had been about to deny.** The spec's
§ 13 list of extern parameter types does not name `str`, but `heroes check`
accepts both `take(s: str)` and `take(@s: str)` (exit 0, `p190/probe/`), and
against a header declaring `int64_t take(HeroStr *s)` the second builds (exit
0, prints `1`) and the emitted call is `t2 = take(&h0_v);`: the address of
`main`'s counted slot `v`, handed to C. The shared exit, the C-only label and
the ladder leave that slot a separate local with the same lifetime, so
nothing changes for C. The block shape makes it `&hslots[k]`, an element with
other slots beside it; section 12 runs what that means.

## 12. The block shape at the boundary: a C overrun ASan names, and then does not

Written 09:21:17 (`date`). Files under `<copy>/p190/probe/`.

`over.h` holds `static inline int64_t poke(HeroStr *s) { s[1] = s[0]; return
s->len; }`, a C function that writes one value past the one it is handed;
`static inline`, so it compiles into the program's own unit and ASan
instruments it. `over.hero` binds `poke(@s: str)`, makes `v: str @ "x"` and
`w: str @ "y" + "z"`, prints `poke(s: @v)` and then `w`. Right output: `1`,
`yz`. The trunk's C keeps `v`, `w` and the temporary's slot as three separate
locals and calls `poke(&h0_v)`; `over.block.c` is the same C with the three in
one `HeroStr hslots[3]`, nothing else changed. `cc.sh` at `-O0`:

| | plain | ASan/UBSan |
|---|---|---|
| separate slots (trunk; equally the shared exit, the C-only label, the ladder) | `1 yz`, exit 0 (the overrun landed harmlessly) | **exit 134, `stack-buffer-overflow` at `over.h:6`**, the C line of the bug |
| one block | `1 x`, **exit 134, `panic: 1 heap blocks still live at exit`** | the same: `1 x`, the leak panic, **ASan silent** |

So under the block shape a C overrun of an `@ str` argument stops being named
at its line by ASan and becomes a wrong value plus the leak gate's panic,
whose own text calls it a missing decref and a compiler bug: the instrument
points away from the binding at the one place §1.12 says the guarantees end.
The exposure is narrow: only a counted slot whose address reaches C, and the
one such path I found is an extern parameter of type `@ str`.

**Found beside the sitting, a question rather than a premise**: `heroes check`
accepts `take(s: str)` and `take(@s: str)` in a program's own group (exit 0;
the second builds against `HeroStr *`), while spec § 13's list of what a
parameter and a field may be names numbers, `bool`, `ptr`, `cstr`, the group's
records, handles and callbacks, and not `str`; no golden or example binds one
(`grep` over `tests/golden/run/*.hero` and `examples/*/*.hero`), and the three
sittings my `grep -i` for *str parameter / across the boundary / HeroStr by
value / a str argument* found (048, 058, 122) are about `s.cstr()`, not a
`str`-typed parameter. Whether a program may hand C a `str` or its address
is the spec-warden's to read; I searched with those words and found no ruling.

## 13. The shared-exit compiler passes its own tests, and what it does to frames

Written 09:22:15 (`date`). From `p190/seed/report.txt` and `p190/seed/frames-O*.tsv`.

**The compiler built from the shared-exit seed runs its own tests:
`1158 tests, all passed`** (`p190/seed/heroes-shared-O0 test
selfhost/main.hero`, within the bounds). With section 11's fixpoint, that is
2,777 rewritten functions, every kind of return the compiler holds (`?`
chains, `.must()`, copy-outs of records, arrays and maps, returns in loops and
`match` arms), exercised by the compiler compiling itself and by its own
tests, under its own leak gate.

**Frames, panel 106's instrument** (`clang -fstack-usage` under `flags()`,
per function, trunk's seed against the shared-exit seed):

| | functions | frames' sum, trunk | shared exit | unchanged | larger | smaller | largest growth | largest shrink |
|---|---|---|---|---|---|---|---|---|
| `-O0` | 5,626 | 8,735,712 B | 8,766,944 B (+0.36%) | 4,096 | 992 | 538 | +400 B | -352 B |
| `-O2` | 5,469 | 2,452,816 B | 2,453,856 B (+0.04%) | 4,556 | 440 | 473 | +1,344 B | -704 B |

At `-O0` the growth is the return slot itself: 574 frames grow by exactly 16
bytes (a `HeroStr hret`), and the large ones return large records
(`h_resolvequalified_a_function_decl` +400 B); the shrinks are fewer
per-call temporaries, one sweep instead of several (`h_irflatten_expr`, a
recursive function, -352 B). At `-O2` the sum barely moves but single
functions swing: `h_emitexternfield_assertions` 3,088 to 4,432 B,
`h_emitoperator_binary` 1,696 to 2,816 B, `h_punctuation_one_byte` 1,344 to
640 B. **Unrun**: whether any of the grown `-O2` frames sits on a recursion
that sets a nesting ceiling (panel 106's other instrument, the interpreter's
nesting ceiling, I did not run).

## 14. Panel 106's other instrument: the interpreter's nesting ceiling

Written 09:24:33 (`date`). Files under `<copy>/p190/interp/`.
`examples/interpreter/main.hero` emitted by the trunk's compiler (54,610
lines), rewritten by `exit_shared.py` (121 functions, none refused; 34,436
lines), each built with `cc.sh` at `-O0` and `-O2`. All four print the tour
script's 21 lines as `main.expected` holds them and as `heroes run` does.
`ceiling.py` binary-searches the deepest `print ((...(1)...))` each binary
evaluates (exit 0 and `1`) on this Mac's 8 MB main thread:

| | trunk | shared exit |
|---|---|---|
| `-O0` | **315** | **315** |
| `-O2` | **829** | **805** (-24, -2.9%) |

Every first failing depth is the stack guard's named abort (`panic: stack
exhausted in synexpr.parsed`, `syncursor.here`, `synexpr.unary`,
`synexpr.compared`), never a bare crash, so §1.12 holds on both; what the
shared exit costs here is 2.9% of the `-O2` ceiling, consistent with
section 13's per-function swings at `-O2`. Not run under `--sanitize`, and
not on Linux or Windows.

## 15. `-gline-tables-only`, and a caveat on my own instrument

Written 09:26:00 (`date`). EXPERIMENT, not the compiler's words:
`p190/bin/cc-glt.sh` is `cc.sh` with `-gline-tables-only` in place of `-g`,
which keeps the line tables design.md §2 and Part 2 promise (lldb steps
`.hero` lines) and drops the variable locations Part 2 calls a non-goal. This
Mac, clang alone, same bounds, every build finished and printed `3`:

| clang `-O2`, peak tree RSS seen | trunk's C | shared exit | ladder | block |
|---|---|---|---|---|
| N = 400, `-g` | 1,258,752 | 1,765,840 | 1,169,728 | (under one poll) |
| N = 400, `-gline-tables-only` | 1,571,616 | 950,272 | (under one poll) | not run |
| N = 400, no `-g` | 1,480,496 | 771,888 | (under one poll) | not run |
| N = 800, `-g` | 4,687,008 | 5,921,472 | 4,663,984 | 227,632 |
| N = 800, `-gline-tables-only` | 5,764,320 | 3,529,632 | (under one poll: 2,512) | not run |
| N = 800, no `-g` | 4,238,240 | 3,168,768 | 153,568 | not run |

**A caveat on every RSS figure in this report, found by this table**: the
trunk reads higher with less debug information (5,764,320 against 4,687,008),
which no mechanism I know explains. Resident memory is what macOS keeps
uncompressed, and other sessions were compiling 800-sized shapes on this
Mac beside mine (seen with `ps`), so the memory compressor lowers a
process's RSS by different amounts from run to run. **The contrasts of an
order of magnitude stand** (the block and the ladder against the rest);
**differences of tens of percent between single runs, such as the shared
exit's +26% over the trunk at 800 with `-g`, are not settled by this
instrument.** Section 16 re-measures them with clang's own *peak memory
footprint* (`/usr/bin/time -l`, which counts compressed pages), clang's
`-cc1` kept in one process by `-fintegrated-cc1`, the unit compile alone,
only the memory lines kept.

## 16. What waits on the compiler-engineer's routes

Written 09:26:47 (`date`). The compiler-engineer's report in the trunk
(`docs/panel/190-reports/compiler-engineer.md`, read at 09:26) says two
routes are built in its copy: **route A**, the shared exit in the IR (every
way out calls `emissions.leave`, which stores into `$ret0`, an ordinary
counted synthetic slot taken by rule 3, and jumps to one exit that copies out,
loads `$ret0`, retains it by rule 4 and sweeps every slot, `$ret0` included;
`hero_str_decref(` 7N + 5 against my hand shape's 6N + 3, the difference being
`$ret0`'s rule-3 release of its null old value at each return site and its
own sweep), and **route G**, the C-only label (`shared_exit.shared_sweep` in
the emitter, the same C as my hand shape by its description). **No path to
either has been sent to me, and my brief forbids reading inside another
seat's copy, so I have not run either route's own C.** What waits, each
ready to run on any C file the moment a path arrives (`p190/bin/`):

- `matrix.sh` over each route's C for `small.hero`, `q1.hero` and the two
  beside-shapes with every return (section 6), on this Mac, Linux arm64
  under clang 22.1.8 and 18.1.8, and the Windows box if it answers;
- item 2's `keeper.hero` and `sqlite_lend.hero` built by each route's
  compiler, under plain, ASan, ASan with the fill byte, Guard Malloc (Darwin)
  and Valgrind (Linux), against the trunk's columns of sections 2 and 3;
- clang's peak memory footprint at `-O2` with `-g` on each route's 400 and
  800 shapes (section 17's instrument), and the interpreter's nesting
  ceiling (section 14).

**Argument from the engineer's description, not run**: route A's exit block
holds copy-outs, a load, rule 4's retain and releases, and its return sites
hold a rule-3 store (a retain, a release of the null old value) and a jump:
runtime calls only, so no C library call can stand between a path's last
instruction and its sweep, and section 2's answer (no release earlier or
later on any path) carries over. Its exit is the same join of N + 1
predecessors as my hand shape's, so section 7's and 15's clang behaviour
should carry over too; that is a prediction, below.

## 17. Every exit shape under Valgrind, Linux arm64

Written 09:29:19 (`date`). One container (`docker ps -q` empty before it),
`p190/bin/linux-valgrind.sh`, log `p190/linux/arm64-valgrind.log`; Debian
clang 22.1.8, `valgrind-3.24.0` installed by `apt-get`, `--error-exitcode=99
--leak-check=full`, each program built with `cc.sh` at `-O0`:

| program | trunk | shared exit | ladder | block |
|---|---|---|---|---|
| `small` (item 1) | clean | clean | clean | |
| `q1` (eighteen lines, every counted return type) | clean | clean | | |
| `try-chain-20`, every return | clean | clean | | |
| `params-returns-20b`, every return | clean | clean | | |
| `slots-returns-100`, every return | clean | clean | clean | clean |

*Clean* is: exit 0 (no Valgrind error), the trunk's output byte for byte
(sha256 prefixes `082282ae693d`, `4f339f837eca`, `722713747fab`,
`12bbbba9f45f`, `3e583abfac5f`), **0 invalid reads, 0 invalid writes, 0 uses
of an uninitialised value** (so the return slot written on every path into
the exit and never initialised is never read unwritten, the run-time half of
what `-Werror=conditional-uninitialized` says at compile time), and nothing
definitely lost.

## 18. Clang's own peak footprint: the comparison that settles section 8

Written 09:34:05 (`date`). `p190/item3/mem/chain-precise.sh`: the unit
compile alone (`-c`, the output to `/dev/null`), `clang -fintegrated-cc1`
so one process holds the whole compile, under `flags()` at `-O2` (with the
compiler's `-g`), wrapped in `/usr/bin/time -l` of which **only the memory
lines are kept** (*peak memory footprint*, which counts compressed pages, and
*maximum resident set size*), one compile at a time inside the same bounds.
All eight finished within them. Bytes:

| clang `-O2`, `-g`, peak footprint | trunk's C | shared exit | ladder | block |
|---|---|---|---|---|
| N = 400 | 1,617,250,248 | 1,998,915,504 (**+23.6%**) | 1,398,162,824 (-13.5%) | 317,309,528 (-80.4%) |
| N = 800 | 5,896,444,312 | **7,921,801,648 (+34.4%)** | 5,406,101,704 (-8.3%) | 886,686,776 (-85.0%) |
| growth, 400 to 800 | x3.65 | x3.96 | x3.87 | x2.79 |
| max RSS at 800 | 6,407,929,856 | 7,306,346,496 | 5,492,719,616 | 986,415,104 |

**This supersedes the tens-of-percent readings of sections 7, 8 and 15**,
taken from 3-second RSS polls under the memory compressor; their order-of-
magnitude contrasts stand and this table agrees with them. Measured, then:
**under the compiler's own words at the level `heroes run` uses, the shared
exit makes clang need a third more memory on the 800 shape than the trunk
does (7.9 GB against 5.9 GB), and a quarter more at 400**; the ladder needs a
little less than the trunk; the block needs about a sixth of it. All four
still grow faster than N (the block least), so none is linear inside clang
under `-g`, and section 10's no-`-g` and section 15's `-gline-tables-only`
rows are where the ladder becomes nearly free. What the shared exit's
`-O2` memory costs on a whole program is section 10's seed row: 12% less
than the trunk. The regression is the extreme shape's.

## 19. Verdicts, in the charter's form

Written 09:35:17 (`date`). Each argument counted under 120 words with `awk`
(`p190/verdict-arguments.txt`).

### Q1. Is each route sound at the C boundary?

- `verdict`: **approve** the routes that release at the function's exit (the
  shared exit, the C-only label, the ladder, the block); **veto** coalescing
  the synthetic slots (*fewer slots*) and any release before the exit (*a
  sweep pruned by liveness* read as release at last use; read as panel 021's
  which-slots-an-exit-releases, it moves no release and is the ladder's
  case).
- `section`: design.md §1.11 and §1.12 (a program must not corrupt memory,
  and the C boundary is where the guarantees end), §4.19; spec § 13 (*"A lend
  lives for its call and no longer"*); panel 106's veto ground, carried.
- `experiment`: `p190/item2/keep.h` and `keep.c` (a C library that keeps the
  pointer it is handed and walks it in its own loop, compiled uninstrumented)
  and the SDK's own `sqlite3.h` with `sqlite3_bind_text(..., destructor:
  nullptr)`, both bound `text: cstr lent`, false under a kept pointer; four C
  shapes of each program (sections 2, 3, 5). Clang accepted every one, 0
  warnings under `flags()`: Apple clang 21.0.0, Debian clang 22.1.8 and
  18.1.8, clang 23.1.1 on the box (keeper only; no `sqlite3.h` there).
- `argument`: Every route that releases at the function's exit releases
  after the same last C call as the trunk on every path: its output equals the
  trunk's under plain runs, ASan, ASan's fill byte, Guard Malloc and Valgrind,
  the trunk's own false-`lent` fault included, on three platforms. Coalescing
  synthetic slots and releasing at last use free a string the library still
  holds: `10` for `9010`, `0 -1 0 0 0` for `21 -1 0 21 9010`, at exit 0, ASan
  silent everywhere; only Guard Malloc, Valgrind or a fill byte see it, and
  none of them is in the net. Panel 106 vetoed slot sharing on exactly this
  shape; those two routes reopen it.
- `prediction`: built by route A's compiler, `p190/item2/sqlite_lend.hero`
  prints `21 -1 0 0 9010` on this Mac, and Valgrind on Linux arm64 counts
  exactly **2** invalid reads in `libsqlite3.so.0.8`, the trunk's figures; with
  the two arms' synthetic slots coalesced it prints `21 -1 0 0 10` here and
  Valgrind counts **4**.
- `condition`: the veto lifts only behind panel 106's whole-function gate (no
  `.cstr()` lend, no lease and no `@` argument to C in a function whose slots
  move), with a run golden per FFI shape; my approval of an exit route falls
  if its built C puts any call other than the runtime's between a return site
  and its sweep, or if Valgrind's count on `sqlite_lend.hero` moves from the
  trunk's.

### Q2. The cost of each route

- `verdict`: **object** to adopting the shared exit as the repair of the
  compile cost at `-O2`; **approve** it as the repair of size.
- `section`: the document does not cover compile memory; CLAUDE.md §
  Precedence ranks *never slow the compiler down* at 5, and § 13 makes a cost
  that stops a needed program from building compiler-need. No binding gets
  harder under any exit route, so §1.11's *serious cost* clause is not engaged.
- `experiment`: sections 4, 8, 10, 13, 18: the shapes at 100 to 800 and the
  compiler's own seed, each as trunk, shared exit, ladder and block C, built
  under `flags()`; clang's peak footprint at `-O2` read by `/usr/bin/time -l`.
- `argument`: The shared exit shrinks the C 115-fold at 800 and the
  compiler's own C by a fifth, and lowers clang's memory on the compiler by 12
  to 13 percent. On the shape that motivated it, by clang's own footprint under
  the compiler's own words, it raises clang's `-O2` memory by a quarter at 400
  and a third at 800, 7.9 GB against 5.9; why is my unrun reading, its one
  join of N+1 predecessors. The ladder needs 8 to 14 percent less than the
  trunk, the block 80 to 85 percent less. The shared exit repairs size, not
  compile memory.
- `prediction`: route A's own 800 C (`$ret0` swept, 61,031 lines by the
  engineer's count), compiled as section 18 compiles, has a peak footprint
  **above the trunk's 5,896,444,312 bytes**.
- `condition`: that footprint at or below the trunk's; or the sitting taking
  the size repair alone and saying the `-O2` regression on many-return
  functions out loud, with the ladder or the block priced as the memory
  repair (the ladder's may-be-written analysis, section 1; the block's
  section 12).

### Q3. What the instruments say

- `verdict`: **object** to reading *ASan/UBSan clean* or *the leak gate
  silent* as settling any route that moves a release or a slot.
- `section`: design.md §1.12; `.claude/rules/c-boundary.md` § The
  instruments (*"`--sanitize` adds `-fsanitize=address,undefined`, which
  catches use-after-free and double-free"*): true of reads in the program,
  not of reads in a library.
- `experiment`: sections 1 to 3, 5, 12 and 17: the controls (one release
  dropped, one doubled), the keeper and SQLite programs, `over.h`.
- `argument`: ASan does not see a C library read freed bytes: zero reports
  where the answer was wrong, on three platforms. On Windows the runtime's
  mark misses a doubled release unless ASan runs. Under the block shape ASan
  stops naming a C overrun of an `@ str` argument and the leak gate blames the
  compiler instead. What sees a library reading freed memory is Guard Malloc
  on Darwin, Valgrind on Linux, and ASan's `free_fill_byte` everywhere as a
  visible wrong value; none of them is in the net, so no green net settles a
  route that moves a release or a slot.
- `prediction`: a compiler that coalesces exclusive arms' synthetic slots
  passes the `run` form whole: of the 19 run goldens holding a `cstr lent`,
  none keeps the pointer past a release (`grep` for `destructor: nullptr`,
  `SQLITE_STATIC`, `keep`, `retain`, `stash`; the one match,
  `fixedbugs-c-writes-over-a-strings-header`, is a writer over the header).
- `condition`: the net's own instruments catching item 2's coalesced or
  last-use fault with no Guard Malloc, Valgrind or fill byte; or such an
  instrument entering the net.

### Q4. Size, and the 800 shape at `-O2` and `--sanitize`

- `verdict`: **object** to the premise that clang cannot finish the 800
  shape; **approve** the measured sizes as the route's case.
- `section`: none; a measurement. CLAUDE.md § RUN IT for the bound named
  before the run (1,800 s, 8 GiB of tree RSS, 08:22:47).
- `experiment`: sections 4, 7, 9 and 18 (this Mac; Linux arm64, clang
  22.1.8). Windows unrun: the box stopped answering at about 09:03 (`ssh`
  timeouts to `apponfly-vps`, 100.119.62.47, port 22 closed), after its
  items 1 and 2 had run.
- `argument`: Measured, not argued: on this Mac and on Linux arm64 the trunk
  builds the 800 shape at `-O2` and under `--sanitize` within 1,800 s and 8
  GiB of tree memory, at 91 and 95 percent of the Linux container's memory;
  the shared exit builds it too. So the premise that clang cannot finish the
  800 shape does not hold on two of the three platforms. The third, the
  Windows box, has 1.2 GB of memory and stopped answering before its run; it
  is the platform where what clang must hold matters most, and it is unrun.
- `prediction`: on the Windows box (1,286,938,624 bytes, 2 processors),
  `heroes.exe build slots-returns-800.hero -O2` does **not** finish within
  1,800 s, and neither does `cc.sh -O2` on my shared exit's 800 C.
- `condition`: the box's run finishing either within the bound.

### In one line

The shared exit (and its C-only twin, the ladder, the block) keeps every
release where the trunk has it, which is all the C boundary asks: approved,
with a third more clang memory at `-O2` on the motivating shape named as its
price. Coalescing slots and releasing at last use break §1.12 at the boundary
where no instrument in the net can see it: vetoed.

**Corrected at the time printed just before this line was appended**
(09:35 to 09:36): Q1's `experiment` says *"0 warnings under `flags()`"* for
item 2's programs on every clang. Counted only on this Mac (`grep -c
warning` over `p190/item2/mac*/` build files: none). On Linux arm64 and the
Windows box those builds succeeded under the same `-Werror` flags, but my
scripts did not count their warnings, so there that half of the sentence is
unrun. Item 1's matrix did count them on all four clangs: 0 each.

**Corrected at the time printed just before this line was appended**
(09:3x, read from `date`): three cross-references and one unsupported list.
Section 10 sends the reader to *"Section 11"* for `-gline-tables-only` and
the block shape: the block is section 11, `-gline-tables-only` is section 15.
Section 15 sends the reader to *"Section 16"* for clang's own footprint, and
section 16 calls it *"section 17's instrument"*: it is **section 18**.
Section 13 lists the kinds of return the compiler holds without having
counted them; counted now in `selfhost/` (raw `grep`, comments and tests not
excluded): 22 map-typed, 90 array-typed and 780 record-typed `@` parameters,
93 lines ending in `?`, 2,034 `.must()`. Returns inside loops and `match`
arms I did not count.

(The correction just above was appended at **09:37:15**, read from `date`
before it.)

## 20. The compiler-engineer's routes A and G, sent at 12:28: setup, and this Mac

Written 12:36:30 (`date`). The coordinator's message of 12:28 sent the
routes' neutral folder, `<scratchpad>/190-shared/routes/` (`README.txt`,
each route's `selfhost.diff` against `703af779` and its generation-2
`heroes`). Both binaries' sha256 match the recorded ones (`shasum -a 256`: A
`1a462638dba9e8d3...`, G `8538d840fe88716a...`). Each route got a folder of
its own, `<scratchpad>/190-ffi-pragmatist-rA/` and `-rG/`: a fresh `git
archive 703af779` extract with the route's diff applied (`patch -p2`, exit 0
both) and the trunk's runtime, so neither route's build cache meets the
trunk's or the other's. From there each route's compiler emitted the C of my
programs at the same relative paths as my trunk copy (`p190/bin/route-emit.sh`),
and its own seed:

| | A: `$ret0`, one exit in lowering | G: one sweep at `bb_exit` in the emitter | trunk |
|---|---|---|---|
| `slots-returns-400`, lines / `hero_str_decref(` | 30,631 / 2,805 | 28,212 / 2,403 | 1,467,808 / 482,403 |
| `slots-returns-800` | 61,031 / 5,605 | 56,212 / 4,803 | 5,815,408 / 1,924,803 |
| the interpreter example, lines | 39,917 | 36,974 | 54,610 |
| its own seed, lines | 1,197,652 | 1,148,344 | 1,358,630 |

Route A's figures equal the compiler-engineer's report (section 3 there).
**What each route's C does at a return**, read in its `pick`: A stores the
value into `h6_ret0` by a rule-3 store (retain the new, release the old) and
jumps to the exit block, which copies out, loads `$ret0`, retains it and
sweeps every slot, `$ret0` included; G keeps the copy-out and the retain in
each returning block, writes `t_ret = tN; goto bb_exit;`, and sweeps once at
`bb_exit`. **In both, nothing but runtime calls stands between a return site
and the sweep**: Q1's condition holds on their C as it did on my hand shape.

**The matrix, this Mac** (`p190/routes/mac-matrix.txt`; each route's own C
for `small`, `q1`, and the two beside-shapes with the every-return `main` of
section 6, made by `p190/bin/every_beside.py`, which reproduces my earlier
trunk files byte for byte, checked by `cmp`): **32 of 32 built at `-O0`,
`-O2` and both under ASan/UBSan, 0 warnings, exit 0, no report, the leak gate
silent, and every output the trunk's** (sha256 prefixes `082282ae693d`,
`4f339f837eca`, `722713747fab`, `12bbbba9f45f`: `43 20` and `190 440` on the
beside-shapes).

**Item 2, this Mac, built by each route's compiler** (`p190/routes/item2/`,
eight builds, 0 warnings):

| | plain | ASan/UBSan | ASan + fill 85 | Guard Malloc |
|---|---|---|---|---|
| keeper, route A | `10 109 9010 -1 0`, exit 0 | `10 109 9010 -1 10`, 0 reports | `... -1 11` | `10 109 9010 -1`, exit 139 |
| keeper, route G | identical | identical | identical | identical |
| SQLite, route A | `21 -1 0 0 9010`, exit 0 | `21 -1 0 21 9010`, 0 reports | `21 -1 0 22 9010` | `21 -1 0`, exit 139 |
| SQLite, route G | identical | identical | identical | identical |

Every cell equals the trunk's of section 2. **The Mac half of my Q1
prediction holds**: built by route A's compiler, `sqlite_lend.hero` prints
`21 -1 0 0 9010` here. The Valgrind half is Linux's, running now.

## 21. Routes A and G on Linux arm64, their clang footprint, and the interpreter's ceiling

Written 12:46:53 (`date`).

**Linux arm64** (one container, `docker ps -q` empty before it; my copy and
the two route folders mounted read-only; `p190/bin/linux-routes.sh`, log
`p190/linux/arm64-routes.log`). **Each route's compiler was built there from
its own seed** (emitted on this Mac by its generation-2 compiler: 0 warnings
both) **and emits all six of my programs byte for byte as on this Mac**
(`cmp`, twelve of twelve), so the routes' C compiled below is what that
platform's own route compiler writes.

- **The matrix**, each route's C for `small`, `q1` and the two beside-shapes
  with every return: **32 of 32 under clang 22.1.8 and 32 of 32 under clang
  18.1.8**, exit 0, 0 warnings, no report, the trunk's outputs (the same four
  sha256 prefixes as this Mac's).
- **Item 2**, both clangs, both routes, 0 warnings: the keeper reads `10 109
  9010 -1 10` plain, the same under ASan with 0 reports, `... -1 11` with the
  fill byte, as plain under `MALLOC_PERTURB_=85`, and **Valgrind counts 1
  invalid read, in `walk (keep.c:8)`**; SQLite reads `21 -1 0 21 9010`, the
  same under ASan with 0 reports, `21 -1 0 22 9010` with the fill byte, as
  plain under `MALLOC_PERTURB_`, and **Valgrind counts exactly 2 invalid reads,
  in `libsqlite3.so.0.8`**. Every cell is the trunk's of section 3.

**My Q1 prediction is scored and holds in full**: built by route A's compiler
`sqlite_lend.hero` prints `21 -1 0 0 9010` on this Mac (section 20) and
Valgrind counts exactly 2 invalid reads in `libsqlite3.so.0.8` on Linux arm64,
the trunk's figures; route G reads the same. Neither route moves a release a
library can see. (The prediction's second half, a coalescing route's `... 10`
and 4, has no built compiler to score it; my hand shape's figures stand.)

**Clang's peak footprint, section 18's instrument** (unit alone,
`-fintegrated-cc1`, `flags()` at `-O2` with `-g`, `/usr/bin/time -l`, only the
memory lines kept, one compile at a time, the same bounds; this Mac while
batch 9's gate ran its net beside it, which the footprint, counting compressed
pages, is the instrument for; `p190/routes/mem/precise.txt`, 0 warnings each):

| clang `-O2`, `-g`, peak footprint, bytes | trunk's C | route A | route G | my hand shared exit (section 18) |
|---|---|---|---|---|
| N = 400 | 1,617,250,248 | 1,972,537,240 (+22.0%) | 1,967,572,912 (+21.7%) | 1,998,915,504 (+23.6%) |
| N = 800 | 5,896,444,312 | **7,874,648,472 (+33.5%)** | **7,561,386,392 (+28.2%)** | 7,921,801,648 (+34.4%) |

**My Q2 prediction is scored and holds**: route A's own 800 C needs more than
the trunk's 5,896,444,312 bytes, by a third. Route G's 800 compile came
within 4% of my 8 GiB tree bound (8,079,472 KiB seen) and finished.

**The interpreter's nesting ceiling under each route** (section 14's
`ceiling.py`, this Mac's 8 MB main thread; every build 0 warnings and printing
the tour script's 21 lines; every first failing depth the stack guard's named
abort at `-O0` and `-O2`, and ASan's own stack-overflow report under
`--sanitize`'s words, alike on all three):

| | trunk | route A | route G | my hand shared exit |
|---|---|---|---|---|
| `-O0` | 315 | **295** (-6.3%) | 316 | 315 |
| `-O2` | 829 | **694** (-16.3%) | 829 | 805 |
| `-O0` with ASan/UBSan | 166 | **158** (-4.8%) | 166 | not run |

**Why, from panel 106's other instrument** (`-fstack-usage` under `flags()`
at `-O2`, the interpreter's 327 functions, `p190/interp/frames-*-O2.tsv`):
route A grows the frames' sum from 89,008 to **100,288 bytes (+12.7%; 68
larger, 3 smaller)**, most of it in the recursive parser (`h_synexpr_named`
1,536 to 1,936, `h_synexpr_compared` 1,952 to 2,224, `h_synexpr_parsed` 144
to 288); route G shrinks it to **87,600 (-1.6%; 13 larger, 27 smaller)**, its
parser frames moving by 16 to 48 bytes. §1.12 holds under all three (the
guard names every exhaustion), so this is capacity, not soundness: **route A
takes a sixth off the deepest recursion a program reaches at `-O2`; route G
takes nothing.**

## 22. The Windows box answered again: the bound, named before its run

The box answered `ssh` at 12:34:17 (the coordinator's *try it once*; it had
not answered from about 09:03 to 09:42). Read then: 1,211,047,936 bytes of
physical memory (1,286,938,624 at 08:40: the machine came back smaller), 152
MB free, a 2,176 MB page file, 34 GB of disk free, no `clang` or `heroes`
running. Before running, at the time above this section: **each run on the
box is bounded by 1,800 s of wall clock, stopped with `taskkill /T` at the
bound** (`p190/win/wbounded.sh`, which also records the largest summed working
set of `clang`, `heroes` and linker processes seen every 5 s). The runs score
section 19's Q4 prediction: my shared exit's 800 C through `cc.sh -O2`, then
the trunk's `heroes.exe build slots-returns-800.hero -O2`.

**Corrected at the time printed just before this line was appended**
(12:48), every sentence above that calls the Windows box *a 1.2 GB box*
(sections 5, 19 and 22, the `section 19` Q4 argument and prediction among
them): **the box's memory is not fixed.** `Win32_ComputerSystem`'s
`TotalPhysicalMemory` read 1,286,938,624 bytes at 08:40, 1,211,047,936 at
12:34, and **4,256,112,640 at 12:48:38**, twenty seconds into the run of
section 22, with `clang.exe` holding 1,296,164 K and 2,328,256 KB free: the
machine is given memory as it asks for it, to a ceiling I have not read. So
the Q4 prediction's ground (*the platform where what clang must hold matters
most*) was an inference from one reading; the run scores it either way.

**The box, first half** (read from `/c/w/190-fp-win/item3.log` at 12:49:08):
my shared exit's 800 C (`sr800.shared.c`, 50,614 lines) through `cc.sh -O2`
under clang 23.1.1 for `x86_64-pc-windows-msvc`: **FINISHED within the
1,800 s bound, exit 0, peak summed working set seen 2,473,976 KB.** **That
half of my Q4 prediction is falsified.** Clang there held about a third of
what Apple clang 21 needed on this Mac for the same file (7,921,801,648 bytes
of footprint, section 18), by a different instrument (a 5-second working-set
poll, not a peak footprint), so the figures compare loosely. Why the gap is
unrun; one difference worth naming is that `-g` for the MSVC target writes
CodeView, not DWARF, so the variable-location work of sections 10 and 15 may
not be the same work there.

## 24. The Windows box: the trunk's 800 shape kills clang there, every exit route's builds; the box released

Written 13:09:36 (`date`). **My work on the box ended at 13:09:00**; `ps -ef`
and `tasklist` read at 13:09:23 to 13:09:31 show nothing of mine running
there, and **I start nothing new on it** (the coordinator's message of 13:09:
batch 9's Windows leg needs it). Logs on the box: `/c/w/190-fp-win/item3.log`
and `routes800.log`; copies of what they printed are below.

Under section 22's bound (1,800 s each, named at 12:47:56), clang 23.1.1 for
`x86_64-pc-windows-msvc`, one run at a time:

| run | verdict | peak summed working set seen | binary |
|---|---|---|---|
| my hand shared exit's 800 C, `cc.sh -O2` | finished, exit 0 | 2,473,976 KB | prints `3`, exit 0 |
| **the trunk, `heroes.exe build slots-returns-800.hero -O2`** | **ended inside the bound, exit 2**: *"clang died on the C instead of judging it (a signal), so nothing in it was refused"* | 7,542,176 KB | none |
| route A's own 800 C, `cc.sh -O2` | finished, exit 0 | 5,347,728 KB | prints `3`, exit 0 |
| route G's own 800 C, `cc.sh -O2` | finished, exit 0 | 2,703,112 KB | prints `3`, exit 0 |

**Why clang died, from Windows' own record** (`events.ps1`, the System log):
at 13:02:46, event 2004, *"Windows successfully diagnosed a low virtual memory
condition. The following programs consumed the most virtual memory: clang.exe
(856) consumed 9540055040 bytes, heroes.exe (3328) consumed 2179330048
bytes"*, with an *Out of Virtual Memory* popup (event 26), after an earlier one
at 12:53:43 saying Windows was growing its page file. The machine is a
Hyper-V virtual machine (event 1801: *"Hyper-V UEFI ... Virtual Machine"*),
its memory read at 4,256,112,640 bytes at 12:48 and 8,588,828,672 at 13:00.
So **on this box the trunk's 800 shape does not build at `-O2`: clang asked
for more than 9.5 GB of virtual memory beside the compiler's 2.2 GB, and the
machine could not give it.** Every exit shape's 800 C built there and runs.

**This scores section 19's Q4 prediction**: its second half (*"neither does
`cc.sh -O2` on my shared exit's 800 C"*) is **falsified**; its first half
(*"`heroes.exe build slots-returns-800.hero -O2` does not finish within 1,800
s"*) is **wrong in its letter** (the run ended inside the bound) and **right in
substance** (the shape does not build: clang died of memory exhaustion).

**And it turns my Q4 objection**: the premise *clang cannot build the 800
shape* does not hold on this Mac or on Linux arm64, and **does hold on the
Windows box, where every exit route is the difference between building and
not**. The box's numbers also cut against section 18's ordering: there the
exit shapes' clang held 2.5 to 5.3 GB of working set where the trunk's needed
more than 9.5 GB of virtual memory, while Apple clang 21 here needed more for
the shared exit than for the trunk. Two clangs, two targets (CodeView there,
DWARF here), and two instruments (a working-set poll there, a peak footprint
here) stand between those readings; which of them reverses the order is
unrun.

## 25. Route A-star (the coordinator's build of 13:09): setup, and this Mac

Written 13:14:09 (`date`). The coordinator's message of 13:09 sent
`<scratchpad>/190-shared/routes/As/`: the compiler-engineer's diff (four
files: `ir/exits.hero` new, `ir/lower.hero`, `ir/own.hero`,
`ir/released.hero`), a generation-2 `heroes` (sha256 `d266f065f6054638...`,
checked) and its seed. My folder `<scratchpad>/190-ffi-pragmatist-rAs/` is a
fresh extract with that diff applied (`patch -p2`, exit 0), and **its
`selfhost/` equals the neutral `tree/selfhost/`** (`diff -rq`: nothing).

**What A-star does to ownership, read in the diff**: an exit block only
where a function has two or more ways out, made by a new pass after
lowering; and **the return slot borrows**: a store into `$ret0` is a plain
copy (no rule-3 retain or release), the slot is left out of the sweep in
`own.hero` and in the verifier's `released.hero`, and the exit's rule-4
retain gives the caller its reference before the sweep releases the value's
real owner (the slot it was loaded from, or rule 5's synthetic slot). The
diff marks the slot by its name (`slot.name != "$ret0"`) and calls itself a
prototype; `$` cannot start a program's own name, so no user slot can be
taken for it. In its emitted `pick`: `h6_ret0 = t14; goto bb4;` and
`h6_ret0 = t11; goto bb4;`, then `bb4` copies out, loads `$ret0`, retains,
sweeps the five other slots, returns. **Nothing but a copy and a jump stands
at a return site**, so no release can move against a C call.

**What it emits** (`route-emit.sh As`): `slots-returns-400` 29,418 lines and
2,403 `hero_str_decref(`, `slots-returns-800` 58,618 and 4,803 (6N + 3, as
route G and my hand shape), the interpreter 37,365, its seed 1,160,979 lines
and **byte for byte the coordinator's generation-2 `s2.c`** (`cmp`).

**The matrix, this Mac**: `small`, `q1` and the two beside-shapes with every
return, **16 of 16** at `-O0`, `-O2` and both under ASan/UBSan, 0 warnings,
exit 0, no report, the leak gate silent, every output the trunk's.

**Item 2, this Mac, built by A-star's compiler** (four builds, 0 warnings):
the keeper `10 109 9010 -1 0` plain, `10 109 9010 -1 10` under ASan with 0
reports, `... -1 11` with the fill byte, `10 109 9010 -1` and exit 139 under
Guard Malloc; SQLite `21 -1 0 0 9010`, `21 -1 0 21 9010` with 0 reports,
`21 -1 0 22 9010`, `21 -1 0` and exit 139. **Every cell is the trunk's**:
the Mac half of my Q1 prediction holds for A-star too. Linux arm64 and
Valgrind are running.

## 26. A-star on Linux arm64, its clang footprint, and the interpreter's ceiling under it

Written 13:21:34 (`date`).

**Linux arm64** (one container, `docker ps -q` empty before it;
`p190/bin/linux-routes-As.sh`, log `p190/linux/arm64-routes-As.log`):
A-star's compiler, built there from its own seed (0 warnings), **emits all
six programs byte for byte as on this Mac**; the matrix is **16 of 16 under
clang 22.1.8 and 16 of 16 under 18.1.8**, 0 warnings, no report, the trunk's
four outputs; item 2 under both clangs reads the keeper `10 109 9010 -1 10`
(1 invalid read under Valgrind, in `walk (keep.c:8)`) and SQLite `21 -1 0 21
9010` (**exactly 2 invalid reads under Valgrind, in `libsqlite3.so.0.8`**),
ASan 0 reports, the fill byte `... 11` and `... 22 ...`, `MALLOC_PERTURB_` as
plain: every cell the trunk's. **My Q1 prediction, which the engineer's
condition also names, holds for A-star**: the borrowed return slot moves no
release a library can see.

**Clang's peak footprint at `-O2` with `-g`** (section 18's instrument; this
Mac; with a trunk reading taken in the same session, so drift is read rather
than assumed):

| | trunk's 800 C | A-star's own 800 C | A-star's own 400 C |
|---|---|---|---|
| peak footprint, bytes | 5,896,444,312 (09:30) and **5,804,874,136 (13:21, -1.6%)** | **7,905,286,528** | 1,967,671,192 (trunk 400: 1,617,250,248) |
| against the trunk | | **+34.1% (09:30) / +36.2% (13:21)** | +21.7% |

Against the 13:21 trunk the other routes read: route A +35.7%, route G
+30.3%, my hand shared exit +36.5%. The compiler-engineer read A-star at
7,592,696,192 against a trunk of 6,369,597,560 (+19.2%) on its own run; my
instrument gives the trunk 5.8 to 5.9 GB and A-star 7.9 GB, and why the two
runs differ on the trunk by about half a gigabyte is unrun (its command line
is not in its report as I read it).

**The interpreter's nesting ceiling under A-star** (section 14's
`ceiling.py`; both builds 0 warnings and printing the tour's 21 lines; every
failure the guard's named abort, ASan's `stack-overflow` under its build):

| | trunk | A | **A-star** | G |
|---|---|---|---|---|
| `-O0` | 315 | 295 | **313** | 316 |
| `-O2` | 829 | 694 | **795** (-4.1%) | 829 |
| `-O0` with ASan/UBSan | 166 | 158 | **171** | 166 |

A-star's frames at `-O2`: the sum falls from 89,008 to 87,040 bytes (13
larger, 30 smaller), though some recursive parser frames still grow
(`h_synexpr_compared` 1,952 to 2,192, `h_synexpr_named` 1,536 to 1,616).
**So A-star gives back most of what route A took**: a 4.1% lower `-O2`
ceiling where A's was 16.3%, close to my hand shape's 2.9%; route G costs
none.

**Corrected at the time printed just before this line was appended**
(13:22): section 26 says the compiler-engineer's command line *"is not in its
report as I read it"*. I had read that report at 09:26; **its final text
(`compiler-engineer.md:607-640`, read at 13:22) gives the method, and it is
mine** (`clang -fintegrated-cc1`, `flags()` at `-O2` with `-g`, `-c` to
`/dev/null`, `/usr/bin/time -l`'s memory lines, 1,800 s, one at a time), with
the trunk's C taken from `<scratchpad>/190-facts/sr800.c`, the coordinator's
artifact, whose `#line` directives carry long absolute paths (450,463,753
bytes against my copy's 224,036,774 for the same 5.8 million lines). Its
readings: trunk 6,369,597,560, route A 7,854,512,536 (+23.3%), A-star
7,592,696,192 (+19.2%), route G 7,891,360,128 (+23.9%). Mine on the same
routes: A 7,874,648,472, A-star 7,905,286,528, G 7,561,386,392, each within
4% of its reading, and the routes' order among themselves flips between the
two runs, so **the three routes are not distinguishable from each other by
this instrument; all three sit a fifth to a third above the trunk**. Whether
the longer paths are what lifts its trunk reading by about half a gigabyte is
unrun.

(One figure in the correction just above, corrected at the time printed
before this line: the file my footprint compiled, `p190/item3/c/sr800.trunk.c`,
is **224,044,066** bytes by `wc -c`, not 224,036,774, which is the cached
unit of the same build; the coordinator's is 450,463,753.)

## 27. What moved in my verdicts and predictions, after routes A, G and A-star

Written 13:23:05 (`date`). Everything below is measured in sections 20 to
26; nothing here is new.

**Predictions, scored:**

- **Q1 holds in full, on all three built routes.** Built by route A's, G's
  and A-star's compilers, `sqlite_lend.hero` prints `21 -1 0 0 9010` on this
  Mac and Valgrind counts exactly 2 invalid reads in `libsqlite3.so.0.8` on
  Linux arm64 under clang 22.1.8 and 18.1.8, the trunk's figures; the
  keeper reads the trunk's too (1). Its second half, about a compiler that
  coalesces slots, stays unscored: none was built.
- **Q2 holds**: route A's own 800 C needs more clang footprint at `-O2` than
  the trunk's (+33.5% on my 09:30 trunk reading, +35.7% on the 13:21 one;
  the engineer's run, +23.3%); A-star and G likewise.
- **Q3 is unscored**: no compiler that coalesces slots exists to run the
  `run` form with.
- **Q4, scored on the box** (section 24): its second half is **falsified**
  (my shared exit's 800 C built at `-O2` there, 2,473,976 KB of working set
  seen; so did route A's and route G's own 800 C); its first half is **wrong
  in its letter and right in substance**: the trunk's `heroes.exe build
  slots-returns-800.hero -O2` ended inside the bound, exit 2, *"clang died on
  the C"*, at 7,542,176 KB of summed working set seen, and Windows' own log
  says clang had asked for 9,540,055,040 bytes of virtual memory beside the
  compiler's 2,179,330,048. Its ground, *a 1.2 GB box*, was one reading of a
  machine whose memory grows with demand.

**Verdicts:**

- **Q1 does not move: approve the routes that release at the exit; veto
  coalescing slots and releasing at last use.** Its footing does: the
  approval now rests on the three built routes' own C, every boundary column
  the trunk's on this Mac (plain, ASan, the fill byte, Guard Malloc) and on
  Linux arm64 under two clangs (the same, plus `MALLOC_PERTURB_` and
  Valgrind). **A-star's borrowed return slot included**: the ownership change
  the coordinator flagged moves no release a library can see.
- **Q2 moves, from *object* to *approve with the price named*.** The box is
  why: there the trunk's 800 shape does not build at `-O2` and every exit
  route's does, so on that platform the exit is the difference between a
  program that builds and one that does not, which is robustness and beats
  memory (CLAUDE.md § Precedence). The price stays measured and must be
  said: on Apple clang 21 here, clang needs a fifth to a third more memory
  at `-O2` on a function of hundreds of returns over thousands of slots
  (routes within noise of each other), while it needs 12 to 13% less on the
  compiler's own seed (my hand shape). The ladder (-8 to -14% here) and the
  block (-80 to -85%) stay the routes that would lower `-O2` memory
  everywhere, each with its own price (the ladder's analysis, the block's
  section 12), unmeasured on the box.
- **The frame is a new cost that separates the routes** (sections 21 and
  26): the interpreter's deepest nesting at `-O2` falls by 16.3% under route
  A, by 4.1% under A-star, and not at all under route G; §1.12 holds under
  all (the guard names every exhaustion). If the sitting adopts A-star, the
  4.1% is its price to name; from the C alone, G is the cheaper route, while
  A-star's gains in the IR and the verifier are the compiler-engineer's to
  weigh.
- **Q3 does not move**: the built routes show ASan blind again (0 reports on
  today's false-`lent` fault under all three) and Valgrind exact (the
  trunk's counts).
- **Q4 moves with Q2**: the premise *clang cannot build the 800 shape* holds
  on the Windows box and not on this Mac or Linux arm64, and every exit
  route removes the failure where it holds.

**The box**: my work there ended at 13:09:00; I have started nothing on it
since, and start nothing until the coordinator says batch 9's leg is done.
A-star's own 800 C on the box is the one run that waits for that message.
