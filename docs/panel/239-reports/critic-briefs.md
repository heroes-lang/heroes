# Panel 239, the completeness critic's first pass: the briefs

Worktree `.claude/worktrees/239-critic/`, detached at `a91d1abd9` (`git log --oneline -3`: *Panel 239:
the briefs, written before any seat*, parent `0a62bb10c`). `git show --stat HEAD` touches
`docs/panel/239-briefs/` alone, seven files, 524 lines. Everything I ran lives under `build/239/` in
this worktree: `c/` (C-level linker checks, `trace.sh`, `more.sh`), `s2363x/` and `s2364x/` (shapes
no brief names). The Linux arm64 runs mounted `build/239/` alone at `/w`, `TMPDIR` under the mount.

Written as I go. Findings that should change a brief before any seat starts come first (A); then
the framing facts re-run (B); then the negative sentences (C); then routes, questions and the lane
(D, E).

## The brief repairs, one line each (detail below)

1. **A1** `00-shared.md` question 1: its *verdict free of the order* excludes route (A) alone, which is
   the proposal's own direction (panel 238's); say so, or (A) only joined to (B) or (C).
2. **A2/A2b** `ffi-pragmatist.md` items 1 and 6, question 1(C), question 6: the duplicate sharing a
   member with a needed name. ld64 binds the other library's `pick` in both orders, and renaming the
   needed function (`b_only` to `z_only`) turns one order into a refused link; GNU ld refuses one order and the compiler tells
   it `internal error`, exit 2; (C) has no order to choose there.
3. **A2b/E3** question 3 and `ffi-pragmatist.md` item 6: a linked library replacing a name the
   **runtime** calls (`printf`) builds at exit 0 and prints nothing, both platforms.
4. **A3** `spec-warden.md` item 1 and `00-shared.md` *The specification*: line 131 (*"Declaration
   order never matters but a group's"*) and § 1's `use` lines; § 13 runs to 490, not 388.
5. **A4** question 4 and `compiler-engineer.md` items 1 and 7: another module's header leaving
   `#pragma pack(1)` set gives `pack/`'s wrong value with no macro (exit 0, `4 1 -402653184`, both
   platforms), which a token or macro comparison cannot see.
6. **A5** `compiler-engineer.md` items 1 and 7: each unit's layout of each record already meets in one
   program-wide list, last unit wins, read by the padding writer (`probing.hero:236-251`,
   `lend_type.hero:61-70`).
7. **A6** `ffi-pragmatist.md` item 1: lld 22.1.8 is in the Linux image and names a shared definition
   not taken; not on this Mac.
8. **A7** `ffi-pragmatist.md` item 2, questions 1 and 3: panel 238's own open question, a library
   that overrides a name on purpose (an allocator), is in no 239 brief.
9. **A8** question 4: a `partial` record whose hidden member another module's macro adds builds at
   exit 0 printing `4 3 0 300` for `4 3 2 302`, and reads unwritten bytes under `--sanitize`, both
   platforms.
10. **C5/C6** `00-shared.md`'s *GNU ld's trace names only the definition taken* holds for `definition
    of` lines only (GNU ld names a shared library it did not take as `reference to pick`); and a trace
    on stderr is printed to the author by `run_link` unless taken out.
11. **F** the lane's sentence on the paid run should cite the author's 10 USD of 2026-11-08
    (`5d9e832b2`) and say it was not asked for this sitting.

## A. Findings that should change a brief before any seat starts

### A1. Question 1's own criterion excludes route (A) alone, by the brief's own example

**The brief says** (`00-shared.md` § The questions, 1): a mechanism must be *"free of the order ... a
mechanism that accepts `dup_written` and refuses `dup_sorted` lets a module's name decide whether a
program builds"*, and lists (A) *"the definition the link took, compared with the library the
binding's group names"*.

**Run**: `repro/mac.out` and `linux/repro.out` (the coordinator's), re-read: `dup_written` binds
`liba`'s `pick` (prints `1 10 20`), `dup_sorted` binds `libb`'s (prints `2 10 20`), on both platforms,
static and shared. (A) compares the definition taken with the group's library: it accepts the first and
refuses the second by construction.

**And it is the proposal's own direction.** `proposal.md` item 1: *"Panel 238's provisional
direction: refuse a name a group binds that the link took from another library than its group's,
read from the link itself"*, which is (A). Panel 238's ffi-pragmatist wrote of it that it *"closes
`dup` and `sig` whatever the order"* (`docs/panel/238-reports/ffi-pragmatist.md:331-334`), in the
sense that no wrong program passes; the 239 brief raises the bar to a verdict free of the order, which
that direction does not meet on `dup`.

**Verdict: missing from the brief.** (A) is admissible under the brief's criterion only joined to
something that fixes the line's order the same way for both programs (C), or to a reading of every
definition (B). The brief should say so in question 1, and say that the criterion departs from
panel 238's direction, so the ffi-pragmatist does not build (A) alone and the synthesis does not
adopt it; or it should ask the seats to show the criterion wrong.

### A2. On ld64, which archive supplies a name depends on the C names' spelling, not only on the line's order; route (C) has no order to choose in one shape

**No brief names this shape**: the duplicate `pick` sits in the **same member** as a name the program
needs from the other library (one member, `b2.o`, defining `pick` and `b_only`; `libb2.a` holds it
alone). `build/239/c/trace.sh` and `more.sh`, C only, `dup_main.c` calling `pick`, `a_only` and
`b_only`:

| link | this Mac, ld-27106.1 | Linux arm64, GNU ld 2.44 |
|---|---|---|
| `-la -lb2` | linked, **`pick=2`** (`-why_load`: `'_b_only' caused load of libb2.a[2](b2.o)`, no load of `pick_a.o`) | **`multiple definition of 'pick'`**, link fails |
| `-lb2 -la` | linked, `pick=2` | linked, `pick=2` |
| the needed name renamed `z_only` (`z_main.c`, `libz2.a`), `-la -lz2` | **`duplicate symbol '_pick'`**, link fails | `multiple definition of 'pick'`, link fails |
| `-lz2 -la` | linked, `pick=2` | linked, `pick=2` |

So on this Mac, renaming the other library's needed function from `b_only` to `z_only` turns a silent
`pick=2` into a refused link, with the line unchanged (that ld64 walks undefined names in name order
is my inference from these two runs, not measured further). On GNU ld the same shape refuses one
order and binds `libb2`'s `pick` silently in the other.

**What it changes**: route (C) (*"the compiler orders the link line so that each bound name resolves
to its group's library"*) has no order to choose on ld64 when the duplicate shares a member with a
name sorting before it: both orders bind `libb2`'s. Question 6's *"a `multiple definition` where there
was none"* is already reached without R3's group words, by this shape. And the defect's failure path
meets 2365's: what the Heroes compiler says when GNU ld refuses the link is in A2b below.

**Verdict: a shape at depth one (padded: one member) that the ffi-pragmatist's items 1 and 6 and
question 1(C) should name.**

### A2b. The same shapes through the Heroes compiler: an `internal error` one way, a silent wrong value the other, and the runtime's own output gone

`build/239/s2363x/`: `picker.hero` (panel 238's, `link "a"`, `pick`) and `extra2.hero` (`link "b2"`
binding `b_only`, `libb2.a` holding the one member `b2.o` that defines `pick` and `b_only`);
`one_written` (`use picker` / `use extra2`) and `one_sorted` (the two lines swapped). And
`runtime_name.hero`: `loud.hero` links `"r"` for `r_only`, `libr.a` holds `r_only.o` and a
`printf_r.o` defining `int printf(const char *f, ...) { return 0; }`; no group binds `printf`. The
program prints `loud.only()` (30) and `7`. Run by `build/239/s2363x/run.sh <compiler>`, outputs
`build/239/run2363-linux.out` (the coordinator's Linux compiler, copied) and `build/239/run2363-mac.out` (mine,
`sources 3d44313d62fb67a3`).

| program | Linux arm64 (GNU ld 2.44) | this Mac (ld64) |
|---|---|---|
| `one_written` | **BUILD exit 2: `internal error: linking failed:`** `/usr/bin/ld: .../libb2.a(b2.o): in function 'pick': ... multiple definition of 'pick'; .../liba.a(pick_a.o) ... first defined here`, then `error: the linker refused the objects clang compiled` | **built exit 0, prints `2 20`**: `libb2`'s `pick` (`nm -m`: `external _pick` defined in the binary) |
| `one_sorted` | **built exit 0, prints `2 20`**: `libb2`'s `pick`, no word | **built exit 0, prints `2 20`** |
| `runtime_name` | **built exit 0, run exit 0, prints two empty lines**: `30` and `7` gone, the runtime's `printf` replaced by the library's | **built exit 0, prints two empty lines** (`nm -m`: `external _printf` defined in the binary) |

So on GNU ld the padded shape is defect 2363 in one order and an `internal error` at exit 2 (the
author's duplicate blamed on the compiler, design.md §4.17) in the other; on ld64 it is defect 2363
in **both** orders, `liba`'s `pick` reachable by no order of the two `use` lines (A2's C runs said so
of the line; this says it of the program); and a library can replace a
name the **runtime** calls with no group binding it, silently. Neither is named by a brief. The
second is the reach question 3 asks about one step further (a name no group binds, needed by the
runtime rather than by a third library), and it is also the allocator question of A7 from the wrong
side: what the robust reading refuses here is exactly what an allocator does on purpose.

**Verdict: both are shapes at depth one that the ffi-pragmatist's item 6 should run and question 3
should name; the `internal error` row is 2365's failure path met by a duplicate rather than an
undefined reference, which question 1 should say any mechanism must turn into a message.**

### A3. The specification's line 131 rules on order, and § 13 runs to line 490, not 388

**The brief says** (`00-shared.md`, *The specification*): § 13 *"says nothing of the libraries' order
or of another module's record in a unit (read, lines 373-388; a question, not a ruling, until the
spec-warden reads it again)"*; `spec-warden.md` item 1 quotes line 375 alone.

**Run**: `grep -n '^## ' spec/heroes-spec.md` (§ 13 at 373 is the last section) and `wc -l
spec/heroes-spec.md` (**490**): § 13 runs 373 to 490, packages at line 479. `grep -n -i
'\border\b\|\blink\b\|defin' spec/heroes-spec.md` prints line **131**: *"Declaration order never
matters but a group's (section 13); mutual recursion needs no forward declarations."* Lines 22 to 31
(§ 1) state what a `use` binds and say nothing of its order.

**Verdict: missing from both briefs.** Line 131 is the sentence that decides whether `pw_first`,
`dup_sorted` and `sig_sorted` are, by the document's own words, programs whose meaning the order of two
lines may not change (if a `use` line is a declaration in its sense), or whether `use` order falls
outside it. The spec-warden's item 1 asks exactly *"which of them the document's own words call a
correct program"*; its brief should point at line 131 and at § 1's `use` lines, and the shared brief's
*"read, lines 373-388"* should read *"373-490"*. The word counts it gives do reproduce (B9).

### A4. A pragma left set by another module's header reaches the silent member of 2364 with no macro at all

**No brief names this shape.** `emit/header_state.hero:1-23` (defect 582) gives back the state a
header leaves set (`#pragma pack` among it) **after all of a unit's group includes**, never between
two of them. So a header of another module that leaves `#pragma pack(1)` set lays out the next
header's record packed in a unit that reads both, while the record's own module reads it padded.

**Run**: `build/239/s2364x/pragma/`: `pk.h` is `s2364/pack/`'s with its macro replaced by a bare
`#pragma pack(1)` at its end; `rp.h` is a plain `typedef struct Rp { char c; int b; } Rp;` with no
macro and no pragma; the `.hero` files are `pack/`'s, copied. Linux arm64, the coordinator's compiler
copied into `build/239/ltree/` (`--version`: `sources 3d44313d62fb67a3`), `build/239/run2364.sh`, its
output `build/239/run2364-linux.out`:

| program | Linux arm64 |
|---|---|
| `pragma/pk_first.hero` (`use pk` / `use rp`) | **exit 0, prints `4 1 -402653184`**, no message |
| `pragma/rp_first.hero` | exit 0, `4 1 1000` |

This Mac, my compiler (`build/239/run2364-mac.out`): the same two rows, `pk_first` **exit 0, `4 1
-402653184`**, `rp_first` `4 1 1000`.

**What it changes**: question 4's routes must be judged against it. A route that compares the
record's preprocessed **tokens**, or the macros the headers define (panel 204's R2 refuses a macro
two headers define two ways), sees nothing here: `rp.h`'s tokens are the same in both units and no
macro is involved. Only a comparison of the **layout** (route B) or a unit that reads no foreign header
before the record's (route D, or C if the owner's prefix is all that is read) closes it. A route
adopted for `pack/` that leans on the macro would leave this one open.

**Verdict: a shape at depth one of 2364's silent member; the compiler-engineer's items 1 and 7
should run it, and question 4 should name it beside `pack`.**

**And two shapes I wrote that do not reproduce**, which narrow the class rather than widen it.
`headers_for`'s first loop pushes another module's extern **functions** into `others`
(`externs.hero:133-138`), which made me write a constant and a function binding read across modules
(`build/239/s2364x/const/`, `fn/`, `rs.h`'s value under `TWO_OTHERS_WIDE` 16, else 8). Linux arm64:
`const/pw_first` and `rs_first` both exit 0 printing `4 8 8` (the root reads `rs.RS_SIZE` as `rs`'s
own unit does); `fn/` is refused in both orders, `error[extern_across_modules]: rs_val is an extern of
module rs; an extern is never reachable across a module boundary` at `pw_first.hero:7:14` (the
message's dash written here as a semicolon). So the
brief's *"the headers of other modules' group records the unit spells"* stands for what a module's
unit can reach; I withdraw my first reading of line 158.

### A8. A `partial` record whose hidden member another module's macro adds: wrong values and an uninitialised read at exit 0

**The brief names the shape** (`compiler-engineer.md` item 7, *"a `partial` record"*) as one to check
beside the CE's route; **it is not among the reproduced shapes**, and it is a silent member of 2364
of its own, so I ran it. `build/239/s2364x/partial/`: `pw.h` as `two_others`' (defines
`TWO_OTHERS_WIDE`); `rx.h` lays `Rx` out `{ int a; int extra; int b; }` under the macro and `{ int a;
int b; }` without it; `rx.hero` binds `record Rx partial` with `a: i32`, `b: i32` (both true in both
layouts, so no field assertion can fail), `rx_make` and `rx_sum(@r: Rx)`, and wraps them in `make`
and `sum(r: Rx)`; the root prints `p.w`, `r.a`, `r.b`, `rx.sum(r: r)`.

| program | this Mac | Linux arm64 |
|---|---|---|
| `rx_first` (`use rx` / `use pw`) | exit 0, `4 3 2 302` | exit 0, `4 3 2 302` |
| `pw_first` (`use pw` / `use rx`) | **exit 0, `4 3 0 300`** | **exit 0, `4 3 0 300`** |
| `pw_first`, `heroes build --sanitize` | **exit 0, `4 3 -1094795586 300`**, no sanitizer word | the same |

The root's unit reads `Rx` as 12 bytes and `rx`'s as 8: `r.b` is read at offset 8 of a value `rx`'s
unit wrote 8 bytes of, so under `--sanitize` it reads `-1094795586` (`0xBEBEBEBE`, bytes nobody wrote),
and `rx.sum(r: r)` hands a 12-byte value to a function compiled for an 8-byte one, `300` for `302`
(that the two units disagree on how the value is passed is my reading of arm64's convention, not
measured). design.md §1.12's class at exit 0, with no macro in the record's own module and no
refusable field.

**Verdict: question 4 should list `partial/` beside `pack/` as a member a field-type assertion cannot
see**, and the compiler-engineer's item 1 should run it with every route; A5's padding table is one
more place the two layouts meet in this program. It is the same cause as 2364, so it belongs to the
item, not to a new filing.

### A5. One program-wide table of each record's padding membership, last unit wins

**No brief names this.** `selfhost/cli/probing.hero:236-251` runs the layout check **per unit**
(`check_records(... records: ok(one.records), @held)`, `held` fresh for each probing) and pushes each
unit's membership of each record into **one** list, `answered`, keyed by declaration index alone
(`pointee_ask.Pointee(name: membership.KEY, ..., index: decl, declared: pointee_ask.PAD ...)`).
`selfhost/emit/lend_type.hero:61-70` (`lend_units`) folds that list into one map, `out[membership.KEY +
index] @ one.header_type`, so the last unit's membership stands for every unit; `cli/artifact.hero:122`
and `cli/rounds.hero:257` compute that map once per program. `emit/padding.hero:171` reads it to decide
which bytes of a lent record are padding, written zero before an extern call (panel 215's R1).

**What it changes**: two things for the compiler-engineer. (1) The data route (B) needs (each unit's
layout of each record it spells) is already computed per unit and already meets in one list; a
duplicate key with two texts is where the cross-unit comparison could be read at no clang cost. (2)
A shape at depth one: a record whose **members** another module's macro changes (a `partial` record,
whose membership is the dump's leaves), lent through `@` in the unit that reads it the other way: the
padding writer of one unit may take the other unit's membership and write zero over bytes that are a
member there. **Unrun**: a question with its code path, not a premise.

**Verdict: the compiler-engineer's items 1 and 7 should name it.**

### A6. lld is installed in the Linux arm64 image and names a shared definition the link did not take

**The brief says** (`ffi-pragmatist.md` item 1): the tools to ask are GNU ld's `--trace-symbol`,
`-Map`, `-t`, ld64's `-map`, `-t`, `-why_load`, `nm -m`, `nm` and `ar t`; Windows' `lld-link` a
question.

**Run** (container, `build/239/c/trace.sh`, `more.sh`): `command -v ld.lld ld64.lld lld` prints all
three; `ld.lld --version`: **Debian LLD 22.1.8**. `clang -fuse-ld=lld` with `liba.a` then shared
`libb.so`, `-Wl,--trace-symbol=pick`: `liba.a(pick_a.o): definition of pick` **and** `libb.so: shared
definition of pick`. `--why-extract=<file>` lists each extracted member with the symbol that pulled
it. On this Mac `clang -fuse-ld=lld` is *invalid linker name*, and no `ld.lld` or `ld64.lld` is on the
path (`which`).

**Verdict: missing from this sitting's briefs**, though not new: panel 238's ffi-pragmatist linked
under lld in the container (`docs/panel/238-reports/ffi-pragmatist.md:123-136`). lld is a third
linker the seats can ask on Linux, the family Windows' leg uses as `lld-link`, and the only one of the
three that names a shared definition not taken in so many words. It names an archive definition not
loaded no more than GNU ld does (`-la -lb`: one definition line).

### A7. Panel 238's open question for this sitting, a library that overrides a name on purpose, is in no 239 brief

**Panel 238 said** (`docs/panel/238-reports/ffi-pragmatist.md:334-335`): the refusal is *"a new
diagnostic class, so a sitting's; its open question is a library that overrides a name on purpose (an
allocator), which the robust reading refuses"*; its spec-warden likewise
(`docs/panel/238-reports/spec-warden.md`, item 4: *"after it rules the open question (a library that
overrides a name on purpose, an allocator)"*).

**Run**: `grep -n -i 'allocator\|on purpose\|interpos\|malloc' docs/panel/239-briefs/*.md` prints only
the `interpose` shape (a library redefining `puts` with another signature) and the historian's *"ELF
symbol interposition and why it is allowed"*. No brief asks a seat to count or build the correct
program on the other side: a library linked **in order to** replace a C library name (an allocator
replacing `malloc` and `free`, a profiler, a test double). And the runtime calls those names itself:
`grep -rohE '\b(malloc|free|calloc|realloc|write|fwrite|printf|snprintf|memcpy)\(' runtime/ | sort |
uniq -c` counts `memcpy` 51, `snprintf` 30, `write` 19, `free` 11, `fwrite` 5, `printf` 2, `malloc` 1,
`calloc` 1.

**Verdict: missing; it should go to the ffi-pragmatist's item 2 (false refusals) and to question 1's
(A) and (B)**, where it is the one false refusal both routes owe an answer to, and to question 3,
since an allocator's names are bound by no group. None is installed on either machine: `ls
/opt/homebrew/lib | grep -iE 'mimalloc|jemalloc|tcmalloc'` and `pkg-config --list-all | grep` print
nothing on this Mac, and the same `ls` of the container's `/usr/lib/aarch64-linux-gnu` prints nothing;
so the seat would write a small library defining `malloc` and `free` over the C library's, which is
what such a library is.

## B. Framing facts, re-run

Each reproduces unless its verdict says otherwise.

- **B1. The tree.** `git log --oneline -3` in this worktree: `a91d1abd9` (the briefs), parent
  `0a62bb10c` *Merge branch 'p238' into b25-round*. `git worktree list`: `b26-fmt`, `b26-index`,
  `b26-link` each at `0a62bb10c`; `git rev-list --count 0a62bb10c..<branch>` prints 0 for the three
  and 1 for `p239`. **Reproduces.**
- **B2. Stage 2 in `p239`.** `p239-scratch/stage2.log`: `737386717036 instructions retired`, `216.35
  real`, `130.36 user`, `28.64 sys`, `stage2 exit 0`, `sources 3d44313d62fb67a3`. **Reproduces from
  the log**; the load of about 15 is in no log and could not be re-read. My own stage 2 in this
  worktree: in B12.
- **B3. Toolchains.** `repro/mac.out`: `ld-27106.1`, `Apple clang version 21.0.0`; `linux/repro.out`:
  `GNU ld (GNU Binutils for Debian) 2.44`, `Debian clang version 22.1.8`, `ldd (Debian GLIBC
  2.41-12+deb13u3) 2.41`. **Reproduces.**
- **B4. The 2363 table.** Every row reads as the brief says in `repro/mac.out` and `linux/repro.out`
  (`sig_sorted` Bus error 138 / SIGSEGV 139, `dup_sorted` `2 10 20` static and shared, `dep_sorted`
  `41 2` on the Mac and `internal error: linking failed` on GNU ld, `only_top` exit 2 on both).
  **Reproduces from the outputs.** The trace lines (`liba.a(pick_a.o): definition of pick`, the ld64
  map, `nm -m ... (from liba)` / `(from libb)`) **reproduce from the outputs.**
- **B5. `interpose`, `remath`, glibc.** `mac.out`: interpose Bus error 138, `nm -m` `external _puts`
  defined in the binary; remath `4.0` and `(undefined) external _sqrt (from libSystem)`.
  `linux/repro.out`: interpose SIGSEGV 139; `ldexp`, `frexp`, `scalbn`, `modf`, `copysign`, `finite`
  one definition each in `libc.so.6` and `libm.so.6`, `sqrt` libm alone, `pthread_create` 2 in libc;
  `libpthread.a` 8 bytes (`ls -la`, `od -c` prints `! < a r c h > \n`); `libm.so` the `GROUP (...
  AS_NEEDED ( /lib/aarch64-linux-gnu/libmvec.so.1 ) )` script; `ldexp` traced to `libm.so.6` with
  `-lm` and to `libc.so.6` with `-lc -lm`. `ls -la $(xcrun --show-sdk-path)/usr/lib/libm.tbd` on this
  Mac: `libm.tbd -> libSystem.tbd`. **Reproduces.**
- **B6. Tracked `link` names.** `git grep -h -oE 'link "[^"]*"' -- '*.hero' | sort | uniq -c`:
  `sqlite3` 36, `m` 20, `app` 7, `z` 5, `pthread` 4, then `a'b`, `<name>`, `-x`, `-out:pwn188`, `""`
  2 each and `ws2_32`, `sub/m` 1 each. **Reproduces.**
- **B7. Source lines.** `wc -l`: `externs.hero` 349, `unit_plan.hero` 200, `extern_field.hero` 404,
  `ffi_field.hero` 554, `ffi_layout.hero` 246, `cli/layout.hero` 586, `header_order.hero` 342,
  `order_trial.hero` 179, `one_file.hero` 261, `link.hero` 688, `link_warning.hero` 163,
  `link_flags.hero` 192, `renamed.hero` 216, `whose.hero` 340, `defined_twice.hero` 220,
  `two_ways.hero` 300. `grep -n '^function '`: `headers_for` 129 (its `return seen` 161), `sort(mine)`
  155, `sort(others)` 158, `libraries` 207, `named` 213 (its `return seen` 257), `plan` 62,
  `headers_for` asked at `unit_plan.hero:144`, `check_records` 90 with its comment at 80 to 84,
  `search` 219, `link_line` 151, `linked_into` 172, `run_link` 37. **Reproduces**; what line 158
  holds is A4.
- **B8. The FFI codes and the member list.** `grep -rhoE '"ffi_[a-z_]+"' selfhost | sort -u | wc -l`:
  **34**. `.claude/rules/c-boundary.md:47-138`: twelve bullets, the twelfth the
  `redefine_extname` label; lines 159 to 160 hold *"a symbol nobody declared stays exit 2"*. **Reproduces.**
- **B9. The specification's words.** `grep -ci` over `spec/heroes-spec.md`: `archive` 0, `link line`
  0, `translation unit` 0, `unit` 0, `linker` 0, `library` 2 (lines 375 and 479). § 13 opens at 373,
  the quoted sentence at 375 to 378. **Reproduces**; the range read is A3. The `heroes measure`
  counts are in B12.
- **B10. design.md.** `grep -n '^### 4.17\|^### 4.19'`: `2320:### 4.17 Errors written to be read by a
  model`, `2582:### 4.19 FFI`. **Reproduces.**
- **B11. Panel 238's diffs.** Extracted from `docs/panel/238-reports/evidence.tar.xz` into
  `build/239/ev238/`: `prototype.diff` 803 lines, `group.diff` 114 (`wc -l`); `git apply --check -p1`
  of each against this worktree prints nothing, exit 0. `prototype.diff` touches `print/fmt.hero`,
  `print/guard.hero`, `probe/judge.hero`, `modules.hero`, `resolve/top.hero`, `use_key.hero`,
  `print/use_order.hero`; `group.diff` `cli/link.hero` and `cli/link_flags.hero` (its `+++` lines
  read `base/selfhost/...`, which `-p1` strips). Disjoint files, so the two apply to one copy.
  The tarball holds `238-ffi-pragmatist/build/238/link/` (373 entries) and `units/two_others/`.
  **Reproduces.**
- **B13. The selfhost groups.** `git grep -h '^extern' -- selfhost/ | sort | uniq -c`: 9
  `hero_compiler.h`, 16 `hero_os.h`, 3 `stdlib.h`, 28 in all; `git grep -l` over 22 files; no
  `link`. **Reproduces.**
- **B14. The open defects beside.** `gh issue view` 2362 to 2367: all OPEN; 2363, 2364, 2365, 2366
  `class:blocking`, 2367 `class:adjacent`, titles as the brief gives them. **Reproduces.**
- **B12. My compiler and the counts.** `build/239/stage2.log`: `stage2 exit 0`, `sources
  3d44313d62fb67a3` (the brief's key), `724285308435 instructions retired` against the brief's
  737,386,717,036 (1.8% fewer, same sources; a cost read across two machines' states is not a delta),
  `real 1257.77` against `user 233.28` plus `sys 53.82`, the load at 218 (`uptime`), so the duration is
  discarded. `./heroes-next measure spec/heroes-spec.md`: `claude-legacy 8178`, `cl100k_base 8325`,
  `real 10900` (`claude-opus-5, 2026-11-08`), headroom 5484. **Reproduces.**
- **B15. `fixguess` on Linux: the brief's output did not run it; my run reproduces it.** The brief's 2364 table
  heads *"both platforms, the same"* and lists `fixguess/` (`a: i64`, both orders, exit 1 on *`Rs.a` is
  `int` in `rs.h`*). `repro/mac.out` holds both `fixguess` lines; `linux/repro.sh` has no `fixguess`
  line at all (`grep -c fixguess linux/repro.sh linux/repro.out`: 0 and 0). My run
  (`build/239/run2364-linux.out`): `fixguess/pw_first` and `rs_first` both exit 1 on *`Rs.a` is `int` in
  `rs.h`*, as on this Mac. **Reproduces now**, on both. And the guess walks in a circle, which the
  brief does not say in words: `pw_first` (`a: i32`) is told `fix (guess): declare a with i64`, and
  the program so fixed is told `fix (guess): declare a with i32`, both orders, both platforms.
- **B16. The quoted 2364 messages' places and fix.** The brief gives `at rs.hero:3:9`, `fix (guess):
  declare a with i64`, `prefix/` at `rs3.hero:6:9`. `mac.sh` and `repro.sh` print `head -1` of the
  output for `pw_first`, `own` and `prefix`, so neither output file holds the place or the fix line;
  `both/` shows `at rb.hero:3:9` and `at pa.hero:3:9`. My runs (`run2364-mac.out`,
  `run2364-linux.out`): `pw_first` and `own` `at rs.hero:3:9`, `fix (guess): declare a with i64`;
  `prefix` `at rs3.hero:6:9`, `fix (guess): declare a with i32`. **Reproduces**, both platforms.

## C. The negative sentences

- **C1. *"No check compares one record's layout in two units"*** (handed to the compiler-engineer as a
  question). The brief's instrument was `grep -rn offsetof selfhost/emit/extern_field.hero`, one file.
  Widened: `grep -rln offsetof selfhost` names six files, `extern_field`, `padding`, `layout_check`,
  `layout_text`, `layout_compare` and `guarded_names`; `layout_compare.hero`'s header (lines 1 to 28)
  is panel 186's comparison of a record holding a union (`==`, `hash`), not of two units. None reads
  two units' answers. **Stands as the brief puts it, a question**, with A5's nuance: the per-unit
  memberships already meet in one program-wide list, unread as a comparison.
- **C2. *"The compiler starts no symbol-table reader today"*.** `grep -rhoE 'program: "[^"]+"'
  selfhost | sort | uniq -c`: `clang` 59, `pkg-config` 5, `git` 3, `uname` 2, `xcode-select`, `env`,
  `date`, `curl`, `cc` 1 each, and fixtures (`build/selftest-unplaced/` 4, `t.hero`, `app/`, ...).
  Widened, because 36 calls pass `program: program` and others a variable: `grep -rnE
  '"(nm|llvm-nm|otool|objdump|readelf|dyld_info|ar|llvm-ar|lipo|ld|ld\.lld|lld)"' selfhost` prints
  nothing, and `grep -rnE 'trace-symbol|-Wl,-map|-Wl,-Map|why_load|--cref|-Wl,-y|print-map|why-extract'
  selfhost` prints one line, `cli/package_words.hero:10`, a comment on refusing `-Wl,-Map=<file>` as a
  package's word. **Reproduces** for every reader and every trace flag I could name; the non-literal
  `program:` values were not enumerated one by one.
- **C3. *"No golden case builds a C library"*.** `git ls-files tests/golden | grep -E '\.c$'` prints
  nothing; `git ls-files tests | grep -E '\.c$'` prints only `tests/emission/*.c`, emitted C;
  `grep -rn '"-shared"\|"-dynamiclib"\|"ar"\|"rcs"\|"llvm-ar"\|libtool' tests/harness selfhost`
  prints `tests/harness/shell.hero:2332` alone, `clang -shared ... lib238.dll`, the Windows DLL.
  **Reproduces.** What it leaves open for question 7: 327 tracked files stand in `.case/` folders
  (`git ls-files tests | grep -c '\.case/'`; 75 under `check`, 77 `fetch`, 50 `run`, 125
  `unsupported`), and 634 `.h` files under `tests/`; a case's header can hold an external
  **definition**, which reaches question 3's *"a name a library defines and the program's own objects
  define too"* with no library built, but not two libraries.
- **C4. *"The spec says nothing of the libraries' order or of another module's record in a unit"*.**
  `grep -n -i '\border\b\|\blink\b\|defin' spec/heroes-spec.md` over the whole file: no sentence on a
  library's order (line 375's *"`link` a library when the symbols need one"* is the duty, not an
  order); `grep -n -i module`: lines 22 to 31 and 376 only. **Reproduces as to libraries and units**;
  but line 131 rules on declaration order (A3), which the brief's range did not reach.
- **C5. *"GNU ld's trace names only the definition taken"*.** True of the lines that say `definition
  of`: one each in every link I ran. **Not true of the trace as a whole**: with `liba.a` then shared
  `libb.so`, GNU ld 2.44's `--trace-symbol=pick` prints `liba.a(pick_a.o): definition of pick` and
  **`libb.so: reference to pick`**, naming the second library as a reference although it defines the
  name (`build/239/c/trace.sh`, container). The libm/libc pair names libc in no line (the brief's
  run). An archive member not loaded is named by no line on either GNU ld or lld; lld names the
  shared one as `shared definition of pick` (A6). So: **reproduces for archives and for two shared
  libraries; does not reproduce for a static definition taken over a shared one**, where the other
  library is named under the wrong word.
- **C6. Where a trace's lines go, a negative no brief states and a mechanism meets.** In the
  container, `clang ... -Wl,--trace-symbol=pick 2>/dev/null | wc -l` prints 0 and `2>&1 >/dev/null`
  prints 2: GNU ld writes the trace on **stderr**; lld writes it on **stdout** (2 and 0); ld64's
  `-why_load` on stderr (this Mac, 0 and 3). `cli/link_warning.hero:37-56` (`run_link`) reads stderr
  on Linux and macOS, drops only glibc's section warnings, and **prints every other line to the author**
  (`io.eprint_raw(text: kept)`, its comment: *"every other line the linker says passes as it did"*).
  So a trace flag added to the real link prints its lines to the author unless the mechanism takes
  them out; with lld, stdout is not read at all off Windows. For the compiler-engineer's item 5.

## D. Routes nobody listed

**For 2363** (question 1 lists A to D):

- **D1. A whole-archive probe.** A second link of the same objects with every archive loaded whole
  (`-Wl,--whole-archive ... -Wl,--no-whole-archive` on GNU ld, `-Wl,-all_load` on ld64) makes the
  linker itself refuse every duplicate among archive members, **in both orders**, naming both members
  (`build/239/c/more.sh`: `-la -lb` and `-lb -la` each `multiple definition of 'pick'` on GNU ld,
  `duplicate symbol '_pick' in: liba.a[2](pick_a.o) ...` / `libb.a[2](pick_b.o)` on ld64). What would
  have to be true for it to be a route: that it reads only the bound names (it refuses every
  duplicate, a name no group binds included, so it needs filtering), that loading every member
  refuses no correct program by an undefined reference a member never loaded would have made
  (unmeasured), and that its cost, one more link, is paid. It sees no shared library and no C-library
  interposition (`interpose`'s `puts`), so it is half of (B) without a symbol reader. **Stands as a
  question; its two measured halves are above.**
- **D2. "The group's library" defined by a probe link of the group's own words.** (A)'s open
  question (*what the group's library is* for a group with no `link`, a re-export, glibc's pairs)
  has an operational answer no brief names: link the group's bound names against that group's `link`
  and `package` words alone, plus the platform's defaults, and record what that link takes (`libc.so.6`
  for `pthread_create` behind `-lpthread`, `libSystem` for `sqrt` behind `-lm`, the C library for
  `printer`'s `puts`); then compare with the real link. It inherits A1: alone, it refuses `dup_sorted`
  and accepts `dup_written`. **Unrun.**
- **D3. The link's own refusal, where it already happens.** A2's shape is refused by the linker
  itself in one order on GNU ld and, by the names' spelling, on ld64. What Heroes says of that refusal
  today is in A2b; a route that tells it on the binding is part of whatever closes the class, since
  (A), (B) and (C) all assume the link succeeds.

**For 2364** (question 4 lists A to E):

- **D4. A preprocessor comparison of the record's tokens** (`clang -E` of the owner's header list
  alone against the unit's, the record's definition compared token for token) is the cheap route a
  seat will reach for, and **a shape defeats it**: another module's header that leaves `#pragma
  pack(1)` set lays out a third header's record packed in the unit that reads both, with the record's
  tokens identical. `emit/header_state.hero:6-23` (defect 582) gives the packing back **after all the
  groups' includes**, not between two of them. Program written, `build/239/s2364x/pragma/`, result in
  A4b. If it builds silently, only a layout comparison (route B) or isolation (D) sees it, and the
  compiler-engineer's item 7 should name it.

## E. Questions the sitting does not ask, and seats handed what their input cannot answer

- **E1. Question 6 assumes GNU ld's archive order alone decides; ld64 decides by the names too (A2).**
  *"Whether the group loads a member the bare line did not"* is asked of GNU ld; the ld64 half, where
  `-la -lb2` binds `libb2`'s `pick` with `liba` first, is not asked.
- **E2. Weak definitions are listed as a false-refusal source (ffi-pragmatist item 2), not as a
  silent-binding shape.** Measured (`build/239/c/trace.sh`, `libbw.a` holding a weak `pick` beside
  `b_only`): GNU ld `-la -lbw` takes `liba`'s strong `pick` (`pick=1`) and `-lbw -la` the weak one
  (`pick=3`); ld64 takes the weak one (`pick=3`) in both orders. A weak definition in a member loaded
  for another name (`b_only`, which sorts before `pick`) pre-empts the group's library on ld64 in
  both orders; with a name sorting after `pick` it is unrun.
- **E3. The runtime's own calls.** Question 3 asks of *"a name no group binds that two linked libraries
  define and a third needs"*. The nearer shape, depth one from `interpose`: a name no group binds,
  which **the runtime** calls (A7's counts) and a linked library defines. Program written,
  `build/239/s2363x/hero/runtime_name.hero` (`libr.a` holds a `printf` that prints nothing), result in
  A2b's table.
- **E4. The spec-warden's item 1 cannot be answered from line 375 alone** (A3: line 131).
- **E5. The historian's item 2** asks for *"dates where the change was a correction after a reported
  crash"*: answerable only from the web, fine; but no brief asks the historian whether any toolchain
  **refuses a duplicate between two archives by default**, which is what D1 rests on as prior art.
  A question to add, not a gap that blocks.
- **E6. Windows.** `.claude/rules/platforms.md:98-101`: the box is *"paid by the hour, usually off, and
  only the author can start it"*; the CI's Windows leg runs after a push. So `lld-link` stays a
  question in every seat, as the briefs say; nothing to repair, and the brief could cite that line.

## F. The lane: a full panel without the blind seat

**What it loses.** The llm-ergonomist is the one seat whose verdict is an experiment on a reader who
has only the specification. This sitting's deliverables that face such a reader are the **words** of
two refusals (questions 2 and 5) and a possible spec sentence (question 8). Without the seat, those
are judged by the spec-warden's word counts (item 4: does a message lean on a word the spec never
uses) and by the seats' reading, which is an argument, not a measurement of whether a model fixes the
program from the message alone (design.md §4.17's promise).

**What a blind task would have to be to measure it.** A copy of the specification outside the
repository, and for each refusal two label-stripped variants of the message (the candidate words and
today's: for 2364 the false `ffi_field_type`, for 2363 none at all, the program building), each with
the refused program: *"this program is refused with this message; change it so it builds and means
what its author meant"*. Scored by building the answer with the sitting's compiler: built, and the
fix the message names (not the `i64` the false fix asks for). A second task for question 8: given
only the spec, predict whether `pw_first` and `dup_sorted` build, which scores whether a sentence is
owed. Size: the `/panel` skill's 3 USD cap per session.

**Whether it may run.** The briefs say the author has not allowed a paid run for this sitting.
`git show 5d9e832b2` (an ancestor of the briefs): the author's *"ok to 10 dollars of extra spending"*
at panel 237's ratification, of 2026-11-08, its body saying *"R6's payment run at the landing falls
within the extra"*. Whether any of it remains or reaches this sitting is the author's to say; the
brief should cite it and say it was not asked, rather than leave it unmentioned. Panel 238 sat the
same way (`docs/panel/238-the-use-lines-*.md:439-444`) and recorded what it gave up.
