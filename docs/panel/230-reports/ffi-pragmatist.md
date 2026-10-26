# Panel 230, the ffi-pragmatist's report

Written as it goes (the brief's rule: a seat stopped by a rate limit keeps only what is on disk).
Seat: the founding constraint, design.md §1.11 (*Heroes ships a minimal runtime and nothing else*),
§1.12 (the boundary complete and defended), §4.19 (FFI), §4.20 (the runtime, in C). Method: the C
each route implies, written and compiled with clang, or shown not to build.

**Where I worked**: my worktree `.claude/worktrees/230-ffi-pragmatist`, detached at `595d92d88`
(clean, `git status --short` empty); scratch `.claude/worktrees/230-ffi-pragmatist-scratch/`.
Darwin arm64 (`uname -sm`), `Apple clang version 21.0.0 (clang-2100.3.34.2)`. **No clock was
started by this seat**: every time and every size is prepared and its command written for the
coordinator (§ 15).

## Status

Complete as of 2026-10-26: sections 0 to 16, then the seat's answer. Every claim was run on Darwin
arm64 unless the text says *unrun*; every time and size is § 15's, for the coordinator.

## 0. What I stand on, read today

- design.md §1.11 (`docs/design.md:482-588`): *FFI ergonomics rank alongside comprehension, not
  below it*; point 3, *FFI signatures are verified by clang against the real header ... a wrong
  type in an `extern` is a compile error*; Tier 2 is *written over `extern`s against a `hero_os.h`
  this project ships* (panel 036), and *the one edge that cannot be Tier 2 is `exit(code)`*.
- design.md §4.20 (`docs/design.md:3040-3048`), the rule that decides item 1: *`_Static_assert(
  HERO_RUNTIME_ABI == N)` does **not** catch it, because a stale `.o` against a new header links at
  exit 0. Adding a *function* to this ABI is self-guarding, an undefined symbol at link, while
  adding a *struct field* is not, and that asymmetry is the rule for every future addition here.*
- design.md Part 2's last non-goal (`docs/design.md:645-657`, panel 213's R10): typed inspection
  delivers *no rendering inside the stopped process ... anything computed inside the stopped
  process: printing rich values is still `print`'s job*. Written of a debugger's stop, not of an
  abort; S1 is a rendering computed inside the aborting process, so the sitting owes a word on
  whether the bullet reaches it (my reading: it does not bind an abort, but S1 is the first
  in-process renderer of a non-scalar value and the bullet is the nearest text).

## 1. My compiler

`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` exit 0; `./heroes build
selfhost/main.hero -o heroes-next` exit 0; `./heroes-next --version` prints `heroes 0.5.0`,
`sources 413da33e7969760a` (the shared brief's key). Not timed. Every `heroes-next` below is it.

## 2. Item 5, the statuses, as a C caller sees them (run)

Programs built with `heroes-next build`, binaries run directly, Darwin arm64.

`print("before")` then `exit(code: N)`, `$?` read in the shell; stderr empty in every row:

| N | status |
|---|---|
| 1 | 1 |
| 134 | **134**, the status an abort gives |
| 256, 512, -256 | **0** (defect 2224) |
| -1 | 255 |
| 4294967296 (2^32) | **0** |
| 4294967297 | 1 |

**Why 2^32 matters beyond defect 2224**: `hero_exit` is `exit((int)code)` (`runtime/parts/os.c:1004-1010`),
and its comment calls the truncation deliberate, *"A shell reads the low 8 bits anyway"*
(`os.c:1000-1003`). The `(int)` conversion of an `int64_t` past `INT_MAX` is implementation-defined
in C11 6.3.1.3p3; clang takes the low 32 bits. So 2^32 becomes 0 **before** any platform sees it,
and Windows, whose statuses are 32 bits wide, reads 0 for it too. That last point is an inference
from clang's conversion, **unrun on the box**. Defect 2224 therefore has a form that does not
depend on the platform, and a repair that only fixes the 8-bit case on POSIX leaves it open.

**What a C parent can tell apart.** `c/parent.c` in my scratch, built with `clang -std=gnu11 -Wall
-Werror` (accepted, exit 0). It spawns each binary with `posix_spawn`, reads `waitpid`'s status, and
runs it again through `system()`:

```
before/index.bin             WIFSIGNALED WTERMSIG=6
                             system() raw=0x8600 WIFEXITED=1 WEXITSTATUS=134
before/assertint.bin         WIFSIGNALED WTERMSIG=6
                             system() raw=0x8600 WIFEXITED=1 WEXITSTATUS=134
before/deep.bin              WIFSIGNALED WTERMSIG=6
                             system() raw=0x8600 WIFEXITED=1 WEXITSTATUS=134
defects/exit_1.bin           WIFEXITED   WEXITSTATUS=1
                             system() raw=0x0100 WIFEXITED=1 WEXITSTATUS=1
defects/exit_134.bin         WIFEXITED   WEXITSTATUS=134
                             system() raw=0x8600 WIFEXITED=1 WEXITSTATUS=134
defects/exit_256.bin         WIFEXITED   WEXITSTATUS=0
                             system() raw=0x0000 WIFEXITED=1 WEXITSTATUS=0
```

- With `posix_spawn` or `fork` plus `waitpid`, an abort (signal 6) can be told apart from every
  `exit` status. That includes a `main` that fails under M1 (exit 1).
- Through `system()` or any shell (`$?`), **an abort and `exit(code: 134)` give the same status**,
  134: `sh` turns a death by signal *n* into 128+*n*. So the range 128 to 255 is ambiguous to every
  caller that goes through a shell, and 126 and 127 are the shell's own "cannot execute" and "not
  found". A range ruling of "0 to 255" (the brief's first option for 2224) still lets a program
  tell a script it crashed when it did not.
- On Windows, `exit(code:)` keeps 32 bits (unrun), and an abort there exited `0xC0000409`
  (panel 174's brief, 2026-09-21, not re-run). `exit(code: 3221226505)` is that same number, so
  the ambiguity has a Windows form, also unrun.

**Abort's status, per platform.** Darwin: 134 (`$?`) and `WTERMSIG=6` (`waitpid`), run above.
Linux: **not run in this sitting**. Two earlier sittings measured it in the Linux arm64 image:
panel 104's scoring row says *the deep golden prints the line at exit 134 on the Mac and in the
Linux image at `-O0`*, and panel 173's table reads *134 ×10* on Linux arm64. The CI's green Linux
legs prove less than that: the `run` suite's `.panics` arm asks only `code != 0`
(`tests/harness/expectation.hero:160-166`). Windows: `0xC0000409`, panel 174's brief, by
`__fastfail`, which `os.c:241-247` keeps; a re-run is owed on the author's box.

## 3. Item 1, the place record in C (written, compiled, linked, run)

**The C.** A copy of `runtime/` in my scratch, `l1/rt31/`, with `HERO_RUNTIME_ABI` 30 → 31 and this
block added after `hero_array_at_mut`'s declaration (`heroes_runtime.h:698`), where every type it
names is already declared:

```c
typedef struct HeroPlace {
    const char *file;     /* the path the program's modules name it by */
    const char *function; /* `module.function` */
    int32_t line;
    int32_t column;       /* the start of the expression whose evaluation stopped */
} HeroPlace;

__attribute__((cold)) _Noreturn void hero_panic_at(const HeroPlace *at, const char *msg);
__attribute__((cold)) _Noreturn void hero_panic_overflow_at(const HeroPlace *at);
__attribute__((cold)) _Noreturn void hero_panic_index_at(const HeroPlace *at, const HeroArrayHeader *a, int64_t index);
__attribute__((cold)) _Noreturn void hero_panic_must_at(const HeroPlace *at, HeroFailure f);
__attribute__((cold)) _Noreturn void hero_panic_assert_at(const HeroPlace *at, HeroStr text);
__attribute__((cold)) _Noreturn void hero_panic_assert_sides_at(const HeroPlace *at, HeroStr text, HeroStr left, HeroStr right);
/* kind 2, the sitting's three shapes, on hero_str_slice and the impure place */
HeroStr hero_str_slice_at(const HeroPlace *at, HeroStr s, int64_t from, int64_t to);   /* L1a */
void *hero_array_at_mut_at(const HeroPlace *at, HeroArrayHeader *a, int64_t index);   /* L1a */
bool hero_str_slice_ok(HeroStr s, int64_t from, int64_t to);                           /* L1b */
bool hero_str_slice_try(HeroStr *out, HeroStr s, int64_t from, int64_t to);           /* L1c */
__attribute__((cold)) _Noreturn void hero_panic_slice_at(const HeroPlace *at, HeroStr s, int64_t from, int64_t to);
```

The definitions are a new part, `l1/rt31/parts/place.c`, included last by `runtime.c`. They print
the first line exactly as today, then `\n  at <file>:<line>:<column>, in <function>`, **in the
same single `fwrite` through panel 192's failure printer** (`hero_failure_write`,
`parts/failure.c:165-171`). I send the file through that printer as well: a file name is bytes
the author chose, so a line feed in one would forge a line of the report (defect 2225's class).

**The emitted side.** `transform.py` rewrites the units `heroes-next` emitted for `index`,
`overflow`, `must`, `assertint` and `slice`. Each anchor is asserted to match exactly once. A
record goes before the function's `#line`, for example
`static const HeroPlace h_at_index_pick_0 = {"index.hero", "index.pick", 2, 12};`, and the site
becomes `hero_panic_index_at(&h_at_index_pick_0, t1, t2)` in place of `((void)hero_array_at(t1,
t2), hero_unreachable())`. The initialiser is **positional**, so no member name appears in the C
written after a group's headers (see the guard below). `slice` gets three units, one per shape:

- L1a: `hero_str_slice_at(&p, t2, t3, t4)`.
- L1b: `hero_str_slice_ok(t2, t3, t4) ? hero_str_slice(t2, t3, t4) : (hero_panic_slice_at(&p, ...),
  hero_str_empty())`.
- L1c: `if (!hero_str_slice_try(&t5, t2, t3, t4)) hero_panic_slice_at(&p, ...)`.

**Compiled** with `cc.sh` (`flags()` as written at `595d92d88`, then the level and its debug word
from `flags.debug_words`): the runtime and the 7 units, each at `-O0 -g` and `-O2
-gline-tables-only`, each without and with `-Werror`. **32 compiles: every one exit 0, 0
warnings, 0 errors.** 14 links exit 0. Every binary, at both levels:

```
panic: array index out of range
  at index.hero:2:12, in index.pick
panic: integer overflow
  at overflow.hero:2:12, in overflow.bump
panic: .must() on an error: odd: not divisible by two
  at must.hero:7:11, in must.main
assert failed: a == b
  at assertint.hero:4:12, in assertint.main
  left:  3
  right: 4
panic: string slice splits a character        (L1a, L1b and L1c alike)
  at slice.hero:3:11, in slice.main
```

Exit 134 for every one. The `-O0` and `-O2` texts are identical byte for byte.

**What the ABI move breaks, run:**

- (A1) A unit stamped 31 against the header of 30: `error: static assertion failed due to
  requirement '30 == 31': heroes_runtime.h is from another compiler`.
- (A2) The committed seed's style of unit (30) against 31: the same assertion, `'31 == 30'`. The
  stamp holds in both directions, **at compile time only**.
- (B1) **`abi-put-back`** (`seed/README.md:119-131`) compiles the *old* seed against the *new*
  runtime with the number put back. I did exactly that: `clang -I l1/rt31put seed/heroes.c
  l1/rt31put/runtime.c`, where `rt31put` is `rt31` with 30 restored. Build **exit 0**, and the
  compiler it made prints `sources 413da33e7969760a`. **That works only because the located entry
  points were ADDED beside the old ones.**
- (B2) The same header with `hero_panic_overflow` **changed in place** to take the place: `exit 1`,
  `selfhost/main.hero:131:100: error: too few arguments to function call, single argument 'at' was
  not specified`, once for each of the 7,568 calls in the seed's code (§ 12; `-ferror-limit=3` stopped it). Through
  `#line`, clang names a `.hero` line of the compiler, so the error reads as a compiler bug.
- (C) A unit built against a `hero_panic` changed in place (the place as its first parameter),
  linked with a runtime object of the old shape: **link exit 0**, run exit 134, and stderr is
  `panic: }` followed by four bytes of the record's pointer read as the message (`od -c` of
  `stale/inplace.err`). This is design.md §4.20's sentence, run: *a stale `.o` against a new header
  links at exit 0*.
- (D) By accident, the converse. My runtime objects were compiled before I added
  `hero_panic_slice_at`. The `slice31b` and `slice31c` units, built against the newer header, then
  failed to link: `Undefined symbols: "_hero_panic_slice_at"`. Both sides said 31, which an object
  cannot show the `_Static_assert`. An **added** function refused the stale object at link, which is
  §4.20's *self-guarding* half.

**So the C says one thing about how L1 lands: new names beside the old, never a new parameter on an
old name.** That is §4.20's asymmetry rule (`docs/design.md:3045-3048`) and also the condition for
`abi-put-back`. The old names are retired only after the refresh whose seed no longer calls them
(`seed/README.md:133-135`, *anything the compiler's own code uses enters `selfhost/` only after the
refresh whose seed knows it*). An L1 that changes the signatures of `hero_panic`,
`hero_panic_overflow`, `hero_array_at` and the rest in place gets my **veto** (§ 16).

**The guard.** Every word `heroes_runtime.h` writes is saved before a unit's groups and restored
after them: 546 `push_macro` lines in `runtime/heroes_guard_open.h`, member and parameter names
included (`ptr`, `len`, `code`, `msg`, `left`, `right`, `text`, `index`). `macro_guard`'s tests
hold `selfhost/emit/guarded_names.hero` to the header. So `HeroPlace`, its four members, the new
entry points and their parameter names (`at`, `out`, `from`, `to`, `s`, `a` where not listed
already) are owed to three files. That is a mechanical landing item, and the existing tests turn a
forgotten one red. The positional initialiser means no member name is written after the groups.

**Where the records live, and what they cost (structure read, sizes not measured).** `nm -m
must31_-O0.bin`: each record is in `(__DATA_CONST,__const)`, 0x18 = 24 bytes apart. `dyld_info
-fixups` shows **two `rebase` fixups per record**, one per pointer. The strings are shared: every
record of `must.hero` points at one file string, and each function's records at one name string.
At `-O2` the four records of `must.half` are **gone**: clang proved the constant divisor's checks
dead and dropped the unreferenced records. So a site clang removes costs nothing. A site that stays
costs 24 bytes of `__DATA_CONST` and two load-time rebases.

The seed is compiled with no `-O` word (`clang -I runtime seed/heroes.c runtime/runtime.c`, so
`-O0`), and `heroes build` defaults to `-O0`. **By arithmetic, an inference and not a
measurement**: the 17,277 call sites in the seed's code (§ 12) would add 17,277 × 24 = 414,648
bytes of `__DATA_CONST` and 34,554 rebases to the compiler. A refinement that removes both per
site: a per-function record `{file, function}` (two rebases per function) plus the line and column
packed into an immediate argument, so a site carries no data and no fixup. **Unbuilt.** It is a
condition I name in § 16, not a veto.

## 4. The `-O2` question that decides L2 and L5 (run)

`heroes run` builds at `-O2` (`selfhost/cli/table.hero:159`), the dev loop. I wrote `mix.hero`,
one function with three overflow sites:

```
function mix(a: i64, b: i64, c: i64) -> i64
    x = a + b
    y = x * c
    z = y - a
    return z
```

I built it with `heroes-next build -O0` and `-O2`. For each `bl _hero_panic_overflow` inside
`_h_mix_mix` (`otool -tV -p _h_mix_mix`), I put its address through `atos -o <bin> -l
0x100000000`, which follows the debug map into `build/`:

```
== O0
call at 0x0000000100016350 -> h_mix_mix (in mix_O0.bin) (mix.hero:2)
call at 0x0000000100016390 -> h_mix_mix (in mix_O0.bin) (mix.hero:3)
call at 0x00000001000163c8 -> h_mix_mix (in mix_O0.bin) (mix.hero:4)
== O2
call at 0x000000010000f0b4 -> h_mix_mix (in mix_O2.bin) (mix.hero:0)
```

**At `-O2` clang folds a function's identical `_Noreturn` calls into one, and the line table gives
that call line 0.** Every route that finds the site from an address therefore names **line 0** at
the dev loop's level for any function with two or more aborts of one kind: L2, L5, a frame walk's
pc, and lldb itself. Two counter-experiments, both compiled and run:

- **`__attribute__((nomerge))` on the declaration** (one word in a copy of `heroes_runtime.h`,
  which changes no symbol and no calling convention): at `-O2` three calls stay, mapped to
  `mix.hero:2`, `:3` and `:4`. So an address route can be rescued at `-O2` by the header. The
  unfolded calls are the price; their size goes to the coordinator (§ 15).
- **L1** (`mix31.c`, three records, `hero_panic_overflow_at`): at `-O2` the run prints `panic:
  integer overflow` / `  at mix.hero:3:9, in mix.mix`, the multiplication, which is right.
  `__attribute__((cold))` let clang's hot/cold splitting move the three failing paths **out** of
  `_h_mix_mix` into `_h_mix_mix.cold.1` to `.cold.3`. The hot function keeps no abort call.

**And the column is out of reach for L2 and L5 by construction.** `#line` remaps lines, not
columns. `dwarfdump --debug-line` on the `-O0` object gives `mix.hero` lines 2 to 4 the columns 10,
8, 21, 19, 54, 58, 31, 68, 98 and 96, which are positions in the **emitted C line**. The `.hero`
lines are 13 characters long. The column the proposal prints (`at report.hero:13:44`) can come only
from the emitter writing it (L1). The proposed § 6 sentence names *the file, line and function*
and no column (the critic, finding 38): under L2 or L5 it could not promise one.

## 5. L0, the issues' own route (compiled and run)

I prepended a macro to the emitted `must.c` and `index.c` that writes `__FILE__`, `__LINE__` and
`__func__` before the entry point runs (`l0/` in my scratch). Both compile with 0 warnings under
`flags()`, link against the unchanged runtime, and exit 134:

```
  at must.c:139, in h_must_main        <- .must(): the generated file's #line, on purpose
  at index.hero:2, in h_index_pick     <- the right line, the C name, no column
```

These are the critic's three reasons, measured. L0 also touches no ABI, and in that it is the
cheapest route at the boundary. But its text is false for two of the five emitter-side classes and
mangled for all of them. I object to it as a route; it is no ABI ground for a veto.

## 6. Item 2, kind 3: what a handler can print async-signal-safely (built and run)

The handlers already print by `write(2)` alone (`hero_stack_say`, `runtime/parts/stack.c:285-292`,
and its comment: *inside a signal handler, `fprintf` is not ours to call*). They hold `pc`, `fp`,
`sp` and `lr` from the `ucontext`. They name functions by `dladdr` (`hero_stack_blame`,
`stack.c:374`; `hero_stack_names`, `:544-561`), **and `dladdr` is not on POSIX's async-signal-safe
list**. That is today's practice (panels 104 and 199), and L4 extending it to every abort adds no
class of risk the stack guard does not already take. The lend fault's report already names the
lent value, its owner and the C parameter from what the emitter registered at the lend
(`hero_lend_tell`, `lend.c:645-660`), a kind-3 abort that says *what* without a call site.

**L5, prototyped in a copy of the runtime** (`l5/rt/`). `stack.c` gains `hero_l5_say`, called from
the stack-exhausted branch before its `\n`; `os.c`'s `hero_args_set` gains one call after
`hero_stack_guard_install()`:

- the base is a symbol the linker defines, taken by address with no call: Darwin
  `_mh_execute_header`; ELF `__executable_start` (GNU ld and lld) and PE `__ImageBase` are written
  in the `#if` and **unrun**;
- the program's sources key is **copied once in `hero_args_set`**, because `hero_program_sources()`
  returns through `hero_str_from_bytes`, which allocates (the emitted line is `HeroStr
  hero_program_sources(void) { return hero_str_from_bytes("3f1e146c5dd9dd96", 16); }`). A handler
  that called it would allocate;
- the image's own identity, `LC_UUID`, is read from its load commands in memory.

Compiled with `-Werror` at `-O0` and `-O2`: exit 0, 0 warnings. `deep.hero` (the recursion that
never reaches its base case), run:

```
-O0  panic: stack exhausted in deep.down
       at image+0x6bc, entered from image+0x70b, sources 65b8e80c72a28ab7, image 93DC22FC437843FB8092C0F462C290F3
-O2  panic: stack exhausted in deep.down
       at image+0x688, entered from image+0x69f, sources 65b8e80c72a28ab7, image F6F470BD56B03677B7D9023DDCB84D12
```

Exit 134 for both. The UUIDs equal `dwarfdump --uuid` of each binary. **The sources key is the
same for two images whose offsets differ.** It says which sources, not which image, so it cannot
key a resolver: the level, the flags and the clang that made the binary all move the offsets. **L5
as the brief words it (*the site's image-relative address beside the program's sources key*) prints
the wrong key. It needs the image's own identity**: Darwin's `LC_UUID`, run here; ELF's
`NT_GNU_BUILD_ID` where the link asks for one, and the PDB's GUID and age on Windows, both unrun.

**Resolving it on Darwin (run).** `atos -o <bin> -l 0x100000000 <0x100000000 + offset>`. Three
fresh binaries, each a new UUID, each looked up for the first time in the state under test:

| state at the first lookup | `atos` answers |
|---|---|
| objects where the debug map names them | `h_deep_down (in deep3.bin) (deep.hero:2)`, `(deep.hero:4)` |
| the unit's object moved away | `h_deep_down (in deep4.bin) + 52`, `+ 131`: **the function, no line** |
| `dsymutil` run, then the object moved away | `(deep.hero:2)`, `(deep.hero:4)`; the `.dSYM` is 156 KB here (`du -sk`) |

**A trap for whoever pins this in a test.** `atos` (CoreSymbolication) **caches per UUID, hits and
misses alike**. A binary whose first lookup found its objects kept answering `deep.hero:2` after
they were moved away. A binary first looked up without its object kept answering `+ 52` after the
object came back. My first reading said L5 survives a cleaned `build/`. It was the cache, and I
corrected it by repeating each state with a fresh UUID. A golden of L5's resolver on Darwin must
build a fresh image per state, or it can pass from a cache.

At `-O2` the faulting `pc` (the prologue) resolves to `deep.hero:1` and the entering `lr` to
`deep.hero:4`, the call `down(n + 1)`. The call that entered the frame is the useful half.

**A side effect, not mine to clean.** macOS's crash reporter wrote `.ips` reports for my aborting
binaries into `~/Library/Logs/DiagnosticReports/` (`mdfind -name deep_-O0` lists two), outside the
repository. The OS writes one for every process that dies by a signal, so every seat and every
`!panic:` golden run on this Mac leaves them. I did not delete them: that would be a destructive
act outside the tree, which is asked for. The coordinator may want to say whether CLAUDE.md's
*no file written outside this repository's root* reaches files the OS writes about a process the
session ran.

## 6b. Kind 3 at exit, and in the lease handler: the place is the acquisition's (read)

The exit check (`hero_runtime_check_leaks`, `runtime/parts/alloc.c:272-404`) is **not** a handler.
It runs in normal context when `main` returns and may print anything. But no address at exit says
where a lease was taken, and the lease handler records none: *WHICH lease is not recorded:
`hero_live_held` is a counter, and a name per lease is a pointer per `.lease()`, priced at panel 173
and not built* (`runtime/parts/os.c:179-180`). So for a lease or a handle nobody ended, **the place
that tells the reader something is the `.lease()` or acquiring call**, and only a pointer stored at
that moment can give it. L1's record is the natural thing to store: the same `HeroPlace` the emitter
already writes, one pointer per live lease. That is a store on the path that does not abort, so the
milestone's *passed, never computed* rule has to rule on it. Unbuilt here; it is the one kind-3
place L4 and L5 cannot reach either.

## 7. Item 3, the line table on each platform

- **Darwin (run)**: the binary carries no `__DWARF` segment (`otool -l`), and its debug map names
  each object by absolute path (`dsymutil -s ... | grep N_OSO`). Lines come from those objects or
  from a `.dSYM` (§ 6's table). `selfhost/` runs no `dsymutil`. A `dsymutil` step is therefore the
  price of any line read away from `build/`: its time and the bundle's size per program are the
  coordinator's to measure (§ 15).
- **Linux**: **unrun in this sitting.** By the ELF toolchain's documented behaviour, `-g` and
  `-gline-tables-only` put `.debug_line` in the linked executable itself unless it is stripped, so
  an offset from `__executable_start` resolves with `addr2line -e <bin>` or `llvm-symbolizer`. Panel
  104 measured that glibc's `dladdr` names nothing without `-rdynamic` (its compiler-engineer row),
  so L4's names on Linux rest on that link word.
- **Windows**: **unrun.** The line table is a PDB beside the `.exe` when the link carries a `-g`
  word (`flags.hero`'s own comment, panel 197). Reading it in-process means `dbghelp`
  (`SymFromAddr`, `SymGetLineFromAddr64`): single-threaded, allocating, and never callable from a
  vectored handler in the sense `write` is. `stack.c:850-857` already records *Windows has no
  walk*. An off-line resolver (L5) needs only `__ImageBase` in the handler and the PDB on the
  resolving machine.

**L2 in-process, judged on these facts.** On Darwin the reader must parse the Mach-O symbol table's
`N_OSO` entries, open the objects under `build/` (gone after a clean, measured above), and parse
their DWARF line programs in a handler without `malloc`. On Windows it needs `dbghelp`. On every
platform it names **line 0** at `-O2` for a folded call and **never the `.hero` column** (§ 4).
That is runtime C the size of a DWARF reader, against a 14,436-line runtime (`wc -l runtime/runtime.c
runtime/*.h runtime/parts/*.c`) whose founding text asks it to stay minimal (design.md §1.11, *Tier
1 ... A few hundred lines*). It buys less than L5 (an offset and a UUID, about 30 lines, run above)
and much less than L1 for kinds 1 and 2.

## 8. Item 4, S1 at the C boundary: a group record as a side (built and run)

**The premise holds today.** `s1/probe.h` (mine) declares `probe_rec` with an `int32_t`, a `const
char *name` that `probe_forget` frees, a `void *data`, a `uint8_t tag[8]` and a `probe_word` union of
`int64_t` and `void *`, each as wide as the union. The binding in `s1/side.hero`: `record probe_word`
with `as_int: i64` and `as_ptr: ptr`; `record probe_rec` with `id: i32`, `name: cstr`, `data: ptr`,
`tag: u8[8]` and `word: probe_word`. The program calls `a = probe_make(1)` and `b = probe_make(2)`,
then `probe_forget` on both, then `assert a == b`.

`heroes-next check` exit 0; `build` exit 0; the run prints `assert failed: a == b`, exit 134, and
so does the `--sanitize` build. Spec § 13 allows such a union where its members are as wide as it
(`spec/heroes-spec.md:396-399`). `==` compares the `cstr` **by address** (spec § 7,
`:195-197`; `selfhost/emit/structural.hero:118-123`). A side may therefore hold a `cstr` C has
already freed. That is legal and ordinary after any `free`-style call.

**What the lldb formatter does with it (run).** S1 asks for one notation with
`editors/lldb/heroes_lldb.py`, but that formatter is keyed on the runtime's own C type names and
steps aside for a header's struct (its own head: *never in a C struct*). lldb then shows its own C
display. I stopped at line 19 with the formatter imported (`lldb --batch`, `frame variable`):

```
(probe_rec) h0_a = {
  id = 1
  name = 0x0000000100630a60 "f\U00000010"
  data = 0x0000000100630a60
  tag = "abcdefg"
  word = (as_int = 1, as_ptr = 0x0000000000000001)
}
```

lldb reads through the dangling `cstr` and shows freed bytes as if they were the field's text.
From outside the process that read is harmless and the text is false. **So "one notation with the
formatter" has no formatter notation to meet at the C boundary.** Where lldb's C notation shows a
`cstr`'s text, an in-process renderer must not. S1's own clause (*a `ptr`, a `cstr`, a function
value and a handle written as an address and never read through*) is the rule there, and the test
that holds the two printers to one corpus has to exempt group records by name.

**The renderer S1 would emit, written in C** (`s1/render.c`): a bounded writer of 1,024 bytes,
`TEXT_BYTES` as lldb's, and per field its kind:

```c
static void h_side_probe_rec(HeroSide *o, const probe_rec *v) {
    side_text(o, "probe_rec(id: ");   side_i64(o, v->id);
    side_text(o, ", name: ");         side_address(o, v->name);   /* never read through */
    side_text(o, ", data: ");         side_address(o, v->data);
    side_text(o, ", tag: [");         /* each byte as a number, at most SHOWN */
    ...
    side_text(o, "], word: ");        h_side_probe_word(o, &v->word); /* both members, union punning */
    side_text(o, ")");
}
```

It reads only the struct's own members: no byte through a pointer, no padding byte. A union member
is read only where spec § 13 lets the record be compared at all, an integer or pointer as wide as
the union, where C11 6.5.2.3 makes every bit pattern a value. A bit-field cannot be reached: it is
`partial`, and a `partial` record is never a side. **The positive control** writes `name` as text.

| build | the renderer | the control (reads the `cstr`) |
|---|---|---|
| `-O0 -fsanitize=address,undefined -Werror`, ASan's free fill (the harness's `FILL`) | exit 0, the report below | **exit 134, `heap-use-after-free ... READ of size 1`, `render.c:17 in side_text`** |
| `-O2`, the same | exit 0, the same report | the same report |
| `-O0`, Guard Malloc (`env DYLD_INSERT_LIBRARIES=/usr/lib/libgmalloc.dylib`, as `tests/harness/freed_bytes.hero:56`) | exit 0 | **exit 139, nothing on stderr** |

```
assert failed: a == b
  left:  probe_rec(id: 1, name: 0x602000000970, data: 0x602000000970, tag: [97, 98, 99, 100, 101, 102, 103, 0], word: probe_word(as_int: 1, as_ptr: 0x1))
  right: probe_rec(id: 2, name: 0x602000000990, data: 0x602000000990, tag: [97, 98, 99, 100, 101, 102, 103, 0], word: probe_word(as_int: 2, as_ptr: 0x2))
```

The Guard Malloc row is the cost of getting this wrong. An S1 renderer that read through a `cstr`
turns today's clean `assert` failure (exit 134 and its line) into **a silent segfault at 139**. That
is the one outcome design.md §1.12 forbids by name, reached on the failing path, where no instrument
of the `run` suite's three legs would see it unless a case planted it.

**Question 7, determinism (run).** Two runs of `render_O0.bin` print `name: 0x10324d550` and then
`name: 0x1022dd550`. A side holding a `ptr`, a `cstr`, a handle or a function value is **not the
same text on two runs** under ASLR, so S1's sentence *written as this document writes a value*
cannot also promise a reproducible abort text. A golden pins such a side only by `contains` of its
other parts, which `expectation.hero:159-166` already does for `!panic:`. I would write the address,
because spec § 7 compares addresses and so the address is the reason two such sides differ. The
sitting has to say that this text varies by run.

**S2 and S3 at the boundary.** S2 (*records and variants of scalars alone*) excludes `cstr` and
`ptr` only if "scalar" is read to exclude them. A `ptr` is a number-sized value, so the sitting has to
say which reading it means. S3 (the first differing field) meets the same `cstr` and needs the same
address rule. S4 (today) reads nothing, measured clean above (`side_san.bin`, `side.bin` under Guard
Malloc, exit 134 each).

## 9. Item 5 continued: M1's `exit 1` against § 13's exit check (built and run)

`main -> ()?` is refused today (`main_returns`), so I wrote `m1/lease.hero`. `run(name: str) ->
()?` takes `x: cstr @= s.lease()`, hands it to `strlen` (a `lent` binding), and ends it only on a
branch never taken (`if name == "never"` then `end_lease(@x)`). It returns `fail(code: "no_key",
msg: "port " + name)`. **Today's shape (M3)**, run: `main` matches the failure and prints `error:
no_key: port 8080` **on stdout**, returns, and the exit check aborts with `panic: 1 lease(s) never
ended ...`, exit 134.

**M1 as C.** I emitted the unit and replaced its `main` with what M1's emitter would write: call
the fallible function, read its `()?` (`h_0opt_a8ea2`, `tag != 0` is the failure), write the
failure on stderr, and exit 1. There are three orders. Each compiles with `-Werror`, 0 warnings, and
links against the unchanged runtime:

| order | stderr | exit |
|---|---|---|
| **A**: write, release the failure, then `hero_runtime_check_leaks()`, then `return 1` | `error: no_key: port 8080` / `panic: 1 lease(s) never ended — ...` | **134** |
| **B**: write, release, `return 1`, the check skipped as `exit(code:)` skips it | `error: no_key: port 8080` | 1 (the lease is never told) |
| **C**: the check first, the failure still held | `panic: 1 heap blocks still live at exit (a missing decref) — this is a compiler bug` | 134, **a false message**, and the failure is never written |

My first run of A printed *3 heap blocks*. That was my prototype's own three unreleased text
temporaries, repaired before the table above. **The reading:**

- Only A agrees with § 13 as written: *aborts when `main` returns*, and a `main` that returns a
  `fail` has returned, its frames released. The very reason `hero_exit` skips the check (*it would
  report a leak for every live value in every frame the exit unwinds past*, `runtime/hero_os.h:145-150`)
  is absent here.
- **C is wrong in a way the runtime would blame on the compiler.** A failure built by concatenation
  holds heap strings, so the check has to come after the failure is released.
- B silently contradicts § 13.

**M1 must therefore state A: the failure's line, then § 13's abort where a lease or a handle is
live, at 134.** A C parent with `waitpid` tells A's two outcomes apart (signal 6 against exit 1, § 2
above). I wrote the line through `hero_write_err`, which writes bytes raw; M1's text names the
failure printer of defect 2225's repair, a new runtime entry point (an *added* function, so §4.20's
self-guarding half). The C ABI of `main` itself, `int main(int, char **)`, does not move under M1.

## 10. Item 6, M5's route as C (built and run)

Three ways a program reaches stderr today, each built with `heroes-next` and run:

| binding | build | run |
|---|---|---|
| `extern "hero_os.h"` `function hero_write_err(text: str)`, then `exit(code: 1)` | exit 0 | exit 1, stdout empty, stderr `error: no_key: port` |
| `extern "stdio.h"` `constant stderr: ptr` and `fputs` | **exit 1, `error[ffi_not_constant]`**, whose note says how: *a value computed in C is reached through a function of the program's own that returns it, written in a header beside this file* | |
| a header of the program's own, `static inline int own_eprint(const char *s) { return fputs(s, stderr); }`, bound as `function own_eprint(s: cstr lent) -> i32` | exit 0 | exit 1, stdout empty, stderr `error: no_key: port` |

- **The `hero_os.h` route** is checked by clang against the real header like any binding, so a
  change of `hero_write_err`'s shape would be a compile error, never a wrong run. The thesis holds at
  this door. It also ships on the three platforms the compiler builds on: the compiler binds it from
  88 code lines in 21 modules (the critic's count) because `write(2)` is not on MSVC
  (`selfhost/cli/io.hero:12-32`). But **`hero_os.h` appears 0 times in the specification** (`grep -c
  hero_os spec/heroes-spec.md`), and its parameter type is the runtime's own `HeroStr`, which only a
  runtime header can offer a binding. It is a door no sentence promises: neither an accident nor
  a supported route.
- **The own-header route** is C89 (`fputs`, `stderr`), so it is portable to MSVC by the standard. It
  is **unrun** on Linux and Windows here. It is design.md §1.11 point 2's shape, and the compiler's
  own diagnostic teaches it.
- **M5 at the boundary costs nothing**: a Tier-2 function in `selfhost/library_source.hero` over the
  `hero_write_err` the library's own group already binds. That is how `exit` is written over
  `hero_exit` (`library_source.hero:264-268`), with no new runtime line and no ABI step. It is panel
  036's precedent for the program's edges (design.md §1.11, *Tier 2 ... written over `extern`s
  against a `hero_os.h` this project ships*). Whether it enters is Principle 0's and the thesis
  seats' to judge. From the boundary, it is the one route for issue 1001 that touches nothing.

## 11. Question 2 at the boundary: an abort inside the library's own Heroes (run)

`args()[0]` with the argument `$'\xff\xfe'` prints `panic: hero_str_from_bytes: not well-formed
UTF-8`, exit 134. The words name a runtime C function. The library's `args` is emitted under `#line
196 "<heroes library>"` as `h_library_args`. An L1 record at that site would read `at <heroes
library>:<n>, in library.args`, which tells the author nothing. The same holds for `exit(code:)`'s
range check if it lands in `hero_exit` or in the library's `exit`, and for every abort inside `map`
or `find`. For these, **the place that tells the reader something is the call into the library**.
L1 reaches it only if the emitter passes the caller's place into a library function that can abort,
or locates the call site instead of the library's line. L4's walk reaches it by naming the first
frame that is not the library's. That is the compiler-engineer's to settle. The C fact it rests on
is that the library's frames carry the file name `<heroes library>` in the line table.

## 11b. Question 10, a thread, and the `hero_os.h` door (run)

`thread/tidx.hero` binds `hero_thread_spawn` and `hero_thread_join` from `hero_os.h`, as
`tests/golden/run/fixedbugs-382-*` do, and its worker reads `xs[n]` with `n = 7`. Today: `panic:
array index out of range`, exit 134. Under the L1 prototype (the worker's checked read rewritten,
compiled with `-Werror`, 0 warnings): `panic: array index out of range` / `  at tidx.hero:7:12, in
tidx.worker`, exit 134. The record travels with the call on whatever thread makes it. L4's walk on
a spawned thread stops at that thread's own entry, since the bounds are per thread
(`stack.c:276-277`); not run.

**Who binds `hero_os.h` from a program** (`grep -rl 'extern "hero_os.h"'`, the frozen recovery corpus
left out): **10 programs under `examples/`**, every one for `hero_thread_spawn` (`wordbands`,
`dotproduct`, `mandelbrot`, `montecarlo`, `threads`, `collatz`, `matmul`, `firsthit`, `nqueens`,
`histogram`); **30 under `tests/`**; 12 files of `selfhost/`. The specification names the header 0
times. So the door § 10 found for `hero_write_err` is the door the project's own examples teach for
threads, not an accident a program stumbled on.

## 12. L1 on a program with many sites: the compiler itself (built and checked, not measured)

**The seed is the compiler's own C.** `heroes-next build selfhost/main.hero --emit-c` equals
`seed/heroes.c` byte for byte (`cmp`), so a transform of the seed is a transform of what L1 would
make the compiler carry.

**The brief's 17,297 sites, counted on code alone** (`seedl1/scan.py` in my scratch, which sets
string and character literals and comments apart): **17,277 call sites in code, 20 inside the
emitter's own string literals**. The 20 are the templates the compiler writes into every program,
and the two `hero_panic_unmatched_*` occur only there.

| entry | in code | in literals |
|---|---|---|
| `hero_panic_overflow()` | 7,568 | 2 |
| `hero_array_at(` (all in the checked form) | 6,827 | 1 |
| `hero_panic_must(` | 2,381 | 1 |
| `hero_panic(` | 268 | 10 |
| `hero_array_at_mut(` | 168 | 2 |
| `hero_panic_assert_sides(` / `hero_panic_assert(` | 36 / 29 | 1 / 1 |
| `hero_unreachable(` | 23,954 | 42 |
| kind 2 on the path that does not abort: `hero_str_slice(` / `hero_array_set(` / `hero_args_at(` | 582 / 253 / 2 | 0 |

**The transform** (`seedl1/seed_l1.py`) rewrites only code. Each site gets its `_at` form and a
record built from the `#line` file and line and the enclosing function, for example `h_at_0 =
{"selfhost/main.hero", "main.command", 131, 0}`. The column is 0 because the C does not know it;
the module part comes from the mangled name (`clicertain` where the emitter would write
`cli/certain`). All records go on the stamp's own line, line 63, so no line of the file moves:
271,388 lines in and out. Results: **`seed_L1.c`** has 17,277 records; **`seed_L1a.c`** has 17,859,
the 582 slices added.

- Both compile under `flags()` with and without `-Werror` (`-fsyntax-only`): **0 warnings, 0
  errors**, like the committed seed.
- Six compilers built, each exit 0 with 0 warnings: `heroes-before`, `heroes-L1` and `heroes-L1a` by
  the seed's documented command (`clang -I <rt> <seed> <rt>/runtime.c`), and `heroes-next-before`,
  `heroes-next-L1` and `heroes-next-L1a` with `flags()` at `-O0 -g`, as `heroes build` compiles.
  Each prints `sources 413da33e7969760a`.
- **`heroes-next-before`, `heroes-next-L1` and `heroes-next-L1a` each emit `selfhost/` as
  `seed/heroes.c` byte for byte.** The located compilers do the same work, and only their own abort
  sites carry a record, so a before/after timing of them compares like with like. Every size and
  time is § 15's.

## 13. Kind 2 on the path that does not abort: what is ready to be timed (built and checked)

**An indexed write is kind 2.** `xs[i] @ v` is `hero_array_set(slot, i, &v)`, and the index check
is the runtime's (`runtime/parts/cow.c:117`, *array index out of range*). So an L1 that locates
kinds 1 and 3 and not kind 2 leaves **the commonest write mistake unlocated**. The brief's kind 1
covers the read check alone.

Two programs, each checked to print the same answer in every variant at both levels, and each
compiled with 0 warnings under `-Werror`:

- `bench/slicebench.hero`: 20 million `s.slice(from: 1, to: 5)`, printing `80000000`. Variants:
  before (`hero_str_slice`), L1a (`hero_str_slice_at(&p, ...)`, one more argument),
  L1b (`hero_str_slice_ok(...) ? hero_str_slice(...) : (hero_panic_slice_at(&p, ...), ...)`, the
  bounds and boundary checks made twice), L1c (`if (!hero_str_slice_try(&t, ...))
  hero_panic_slice_at(&p, ...)`, one check, a status read, the result through memory).
- `bench/placebench.hero`: 20 million `g.rows[i % 2].cells[i % 4] @ i`, printing `19999999`.
  Variants: before (`hero_array_set`), L1a (`hero_array_set_at(&p, ...)`), L1b (the slot hoisted
  once by a statement expression, the index checked in the program's C with its place, then
  `hero_array_set` checking again).

Each at `-O0` and `-O2`: 10 binaries in `bench/`, their commands in § 15.

## 14. Two routes that change a shape C sees (compiled and run)

- **M4 through the failure.** If the place rides inside `HeroFailure` (`m4/failure_size.c`,
  `sizeof` printed): `HeroFailure` goes from 32 to 40 bytes, and **every `T?` from 40 to 48**:
  `i64?`, `str?` and `()?` alike. That is a field added to a struct `hero_panic_must(HeroFailure f)`
  takes **by value**, the shape §4.20 says links at exit 0 against a stale object. It is also an
  8-byte tax on every fallible value in every program: design.md §4.20 records that `m[k]` already
  returns through memory at 40, and a `T?` of a 40-byte record is 48 today. **M4 is clean only
  where the place is passed at `main`'s own exit** (the `return fail(...)` or the `?` that leaves
  `main`, a record beside it as L1 writes one), never stored in the failure. Stored in the failure
  is my **veto** (§ 16).
- **L3, a thread-local site cell** (`l3/tls.c`, the cell written before each of two calls, at
  `-O2` with `-Werror`, 0 warnings). On Darwin each function makes **one indirect call to the TLV
  getter** (`ldr x9, [x0]; blr x9`, the address then kept in `x19`) and **three instructions per
  site** (`adrp`, `add`, `str`) on the path that does not abort. That is computation where nothing
  aborts, which the milestone's rule refuses. The cell also says nothing about kind 3 that the
  handler's own `pc` does not.

## 14b. A real binding under L1: `examples/sqlite` (built and run)

I copied `examples/sqlite/` into my scratch, built it with `heroes-next` and ran it (`rows: 3`,
`longest: 6`, exit 0). Then I emitted its unit, applied the same transform as the seed's (2
`hero_panic_overflow()` sites, 2 records), and compiled it under `flags()` with `-Werror` at `-O0`
and `-O2`: **0 warnings**. I linked it against the ABI-31 runtime and the system's `-lsqlite3` and
ran it: **stdout identical to today's at both levels**, exit 0. Outside the stamp's line, the only
lines that differ are the two that hold an abort call. The binding's 26 `sqlite3_` references and
43 type-check lines (`HERO_RET_*`, `heroes-ffi-*`) are byte for byte today's. **L1 (added names,
positional records) leaves the C a binding is checked against untouched.**

## 15. For the coordinator: what this seat built to be measured, and the commands (none run here)

`S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/230-ffi-pragmatist-scratch`,
`W=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/230-ffi-pragmatist`. The machine is held
still, `/usr/bin/time -p`, the best of N stated runs with the first discarded, and a noise floor
measured first (CLAUDE.md § Verification). Every binary below was checked by this seat for its
output, never for its time.

**Sizes** (deterministic, but the brief makes them the coordinator's):

```sh
wc -c $W/seed/heroes.c $S/seedl1/seed_L1.c $S/seedl1/seed_L1a.c       # against M-bounded-seed's 10 MB
size $S/seedl1/heroes-before $S/seedl1/heroes-L1 $S/seedl1/heroes-L1a  # the seed's own build command
size $S/seedl1/heroes-next-before $S/seedl1/heroes-next-L1 $S/seedl1/heroes-next-L1a   # flags() -O0 -g
for b in heroes-next-before heroes-next-L1 heroes-next-L1a; do otool -l $S/seedl1/$b | grep -A4 'segname __DATA_CONST' | grep vmsize; dyld_info -fixups $S/seedl1/$b | grep -c rebase; done
size $S/merge/mix_O2.bin $S/merge/mix_nm.bin $S/merge/mix31.bin         # -O2: folded, nomerge, L1; whole binaries
# link three different runtime objects, so the fair comparison is the hot function's, the next line
for b in mix_O2 mix_nm mix31; do otool -tV -p _h_mix_mix $S/merge/$b.bin | wc -l; done   # the hot function
```

One number I saw while verifying the transform, given as a reading and not as the measurement: the
stamp's line of `seed_L1.c`, which holds all 17,277 records, is 1,726,186 bytes (`wc -c` of that
one line).

**Times:**

```sh
# 1. the compiler on its path that does not abort (three compilers that emit the same bytes, § 12)
cd $W && /usr/bin/time -p $S/seedl1/heroes-next-before build selfhost/main.hero --emit-c -o $S/timing-before.c
cd $W && /usr/bin/time -p $S/seedl1/heroes-next-L1     build selfhost/main.hero --emit-c -o $S/timing-L1.c
cd $W && /usr/bin/time -p $S/seedl1/heroes-next-L1a    build selfhost/main.hero --emit-c -o $S/timing-L1a.c
# 2. clang over C that carries the records (what the whole build adds), at both levels
/usr/bin/time -p bash $S/cc.sh -O0 no -I $W/runtime -c $W/seed/heroes.c -o $S/t0.o
/usr/bin/time -p bash $S/cc.sh -O0 no -I $S/l1/rt31 -c $S/seedl1/seed_L1.c -o $S/t1.o
/usr/bin/time -p bash $S/cc.sh -O2 no -I $W/runtime -c $W/seed/heroes.c -o $S/t2.o
/usr/bin/time -p bash $S/cc.sh -O2 no -I $S/l1/rt31 -c $S/seedl1/seed_L1.c -o $S/t3.o
# 3. kind 2 on the path that does not abort (20 million calls each), -O0 and -O2
for l in -O0 -O2; do for b in slicebench_before slice_L1a slice_L1b slice_L1c placebench_before place_L1a place_L1b; do /usr/bin/time -p $S/bench/${b}_$l.bin; done; done
# 4. the dSYM step L2 and L5 need on Darwin, on a compiler-sized program whose objects were kept
/usr/bin/time -p dsymutil $S/dsym/heroes-kept -o $S/dsym/heroes-kept.dSYM && du -sk $S/dsym/heroes-kept.dSYM
```

**What each number decides.** 1 is CLAUDE.md's *never slow the compiler down* for L1 and L1a
(the records' load-time rebases and the compiler's own aborts). 2 is the build cost of 17,277
records. 3 separates L1a, L1b and L1c for kind 2, where the boundary gives me no ground to prefer
one. 4 is the price of any line read away from `build/` on Darwin.

**Owed elsewhere, unrun by this seat**: Linux, where an offset from `__executable_start` resolves
through the ELF's own `.debug_line`; Windows, where `__ImageBase`, the PDB and `exit(code: 256)`
being 256 there must be run on the author's box, with `0xC0000409` re-run. And `exit(code:
4294967296)` on Windows, which is 0 by clang's `(int)` conversion, an inference.

## 15b. Question 11, `-O2` inlining (run)

In `mix_O2.bin` `_h_mix_main` calls no `_h_mix_mix`. The function was inlined, constant folding
found `mix(a: 9223372036854775807, b: 0, c: 2)` overflowing, and **the abort call sits in
`_h_mix_main`**. `atos -i` on it answers `h_mix_mix (mix.hero:3)` inlined into `h_mix_main
(mix.hero:9)`: an inline-aware resolver (L5) keeps the place, and **`dladdr` (L4's names) would
say `mix.main`**. L1's record printed `mix.mix` at line 3 under the same build (§ 4).

## 16. Verdicts, route by route

I stand on design.md §1.11 (`docs/design.md:557-588`), §4.20's asymmetry (`:3040-3048`) and §1.12
(`:590-627`). "No ground" means the boundary gives no reason either way and the route is other
seats' to judge.

| route | verdict | the C that decides it |
|---|---|---|
| **L0** | object | § 5: `at must.c:139, in h_must_main` and `at index.hero:2, in h_index_pick`. No ABI cost; a false file for two classes and a mangled name for all |
| **L1** | **approve, with conditions**; **veto** of the variant that changes an existing entry point's signature in place | § 3: 32 compiles clean, 14 binaries right at `-O0` and `-O2`, a thread (§ 11b), the compiler (§ 12), SQLite (§ 14b). (B2) the old seed fails with *too few arguments*, and (C) a stale object links at exit 0 and prints `panic: }` and four bytes |
| L1a / L1b / L1c | no ground between them | all three compile clean and print the same text; § 15's timing 3 decides. L1c is §1.11's *errors as values* shape and leaves a runtime function usable without aborting |
| **L2** | object | § 4: line 0 at `-O2` for folded calls; the column is the C's; § 6: on Darwin the objects of `build/` or a dSYM; Windows `dbghelp` is not handler-safe; a DWARF reader in a runtime §1.11 keeps minimal |
| **L3** | object | § 14: a TLV getter call per function and three instructions per site where nothing aborts |
| **L4** | approve **for kind 3 only** | it exists (`hero_stack_names`), already uses `dladdr` in a handler, and names the outer function after inlining (§ 15b); no Windows walk |
| **L5** | approve **for kind 3**, with a condition | § 6: `write` alone, `-Werror` clean, run. The condition: print the image's identity (`LC_UUID`, run; ELF build-id and PDB GUID, unrun), never the sources key alone (same key, different offsets, measured). Object as the place of kinds 1 and 2: no column, line 0 at `-O2` without `nomerge` |
| **S1** | **approve, with conditions**; **veto** of any renderer that reads through a `cstr`, a `ptr`, a handle, a function value or a union's pointer member | § 8: the address renderer is clean under ASan, UBSan and Guard Malloc; the reading one is a heap-use-after-free and a silent 139. The one-notation test must exempt group records (lldb reads the dangling `cstr`, measured), and an address makes the side vary by run (measured) |
| S2 / S3 | approve on S1's address rule | the same fields reach them |
| S4 | no ground | reads nothing; clean under `--sanitize` and Guard Malloc (§ 8) |
| **M1** | **approve, with a condition** | § 9: only order A (the line, the release, then § 13's check: 134 with a lease live) agrees with § 13; C prints a false *compiler bug*. `main`'s C ABI does not move; `waitpid` tells 1 from an abort (§ 2) |
| M2 | no veto | C's own shape; it inherits the range ruling below through the same `(int)` |
| **M4** | **veto** of the place stored in `HeroFailure`; approve it passed at `main`'s exit | § 14: every `T?` 40 → 48 bytes and a by-value runtime struct changes shape |
| **M5** | approve | § 10: Tier 2 over the `hero_write_err` the library already binds, as `exit` over `hero_exit`: no runtime line, no ABI step |
| M3, M6, M7 | no ground | none touches the boundary beyond the range below |
| **`exit(code:)`'s range** | approve a compile-time refusal of a constant outside 0 to 255 and an abort otherwise; **object** to *"the low eight bits"* | § 2: 2^32 is 0 through clang's `(int)` before any platform sees it, and Windows keeps 32 bits (unrun), so that sentence is false on Windows. I record, without vetoing, that 128 to 255 is an abort's status to every shell |

**One fact for every route at once.** The `hero_os.h` door is how 10 `examples/` and 30 `tests/`
programs bind `hero_thread_spawn` and the rest (§ 11b), and the specification names it 0 times.
Whichever M is taken, the sitting should say whether a program may bind that header. Today the only
thing guarding the door is clang's check of the binding, which is the thesis working (§ 10).

## The seat's answer, in the shape the panel asks

- **verdict**: approve L1 (with conditions), L4 and L5 for kind 3, S1, M1 and M5. Object to L0, L2,
  L3 and *the low eight bits*. **Veto** three variants: L1 with signatures changed in place; an S1
  renderer that reads through a `cstr`, `ptr`, handle, function value or union pointer; M4 with the
  place stored in `HeroFailure`.
- **section**: design.md §4.20 (`docs/design.md:3045-3048`, the asymmetry: a function added
  self-guards at link, a shape changed links at exit 0), §1.11 (`:557-588`, FFI ergonomics beside
  comprehension; a minimal runtime), §1.12 (`:590-627`, no segfault).
- **experiment**: an ABI-31 runtime with `HeroPlace` and twelve entry points added (ten named
  `_at`, plus L1b's `_ok` and L1c's `_try`); 7 emitted
  units, the whole seed (17,277 sites) and `examples/sqlite` rewritten to L1. All compiled under
  `flags()` with and without `-Werror` at `-O0` and `-O2` (clang accepted every one: 0 warnings, 0
  errors) and run. Today's seed was built against it by `abi-put-back` (exit 0). The in-place
  variant failed (`too few arguments`) and a stale object linked at exit 0 printing garbage. An L5
  handler printing offsets and the image `LC_UUID` by `write` alone, run. An S1 renderer for a group
  record holding a freed `cstr`: clean under ASan and Guard Malloc, while the reading control was a
  use-after-free and a silent 139. M1's `main` in three orders, run.
- **argument**: The C says L1 must arrive as new names beside the old. A changed signature breaks
  the old seed's build (the release route) and links silently against a stale object, printing
  garbage. Named that way, L1 touches nothing a binding sees: SQLite's 43 type checks are byte for
  byte today's, and the output is identical. The address routes (L2, L5) cannot give the column and
  give line 0 at the dev loop's `-O2`. They belong to kind 3, where only an address exists. S1 is
  sound only if it never reads through foreign pointers. M4 must not widen every `T?`. M5 costs the
  boundary nothing.
- **prediction**: when L1 lands as added `_at` names with positional records, `examples/sqlite` and
  `examples/ledger` (both `extern "sqlite3.h"`) build and pass their `.expected` with **zero edits
  to their `.hero` files**. Their emitted C differs from today's only on lines holding an abort call
  and on the records' line, falsified by any other changed line in `heroes build --emit-c`'s
  output. And a program whose failing `assert` compares two `struct passwd` records (`pw_name: cstr`)
  after `endpwent()` prints both `pw_name`s as addresses under an S1 build, with no AddressSanitizer
  report and exit 134 under Guard Malloc, never 139.
- **condition**: the L1 veto lifts never for an in-place change; it is a rule, §4.20's. If § 15's
  timing 1 shows `heroes-next-L1` slower than `heroes-next-before` beyond the noise floor, my L1
  approval holds only with the refinement (one record per function, line and column as an immediate
  argument, no per-site rebase), which then becomes a requirement. The S1 veto lifts only if spec § 7 stops comparing a
  `cstr` by address, never by a renderer's choice. The M4 veto lifts if the place leaves
  `HeroFailure`. L2 would gain my approval only on a measured in-process reader that gives the
  `.hero` column and a non-zero line at `-O2` on Darwin with `build/` gone, which § 4 and § 6 say
  it cannot.
