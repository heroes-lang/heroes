# Panel 085 — the seed, and the net after the archive

Convened 2026-08-18, at the opening of **M-selfhost-fixpoint**. Full panel minus
one seat: `compiler-engineer`, `ffi-pragmatist`, `historian`, `spec-warden`.

**Why the llm-ergonomist was not convened, and what it cost.** Its input is
`spec/heroes-spec.md` plus 1–3 programs, and both halves of this proposal change
**zero spec tokens** (measured by the warden, not assumed: `heroes measure`
today is legacy 3440 · cl100k 3512 · **max 3512 binding**, and
`spec/heroes-spec.md` is untouched at HEAD). A seat whose whole method is a
blind read of a document that does not change has nothing to be differentiated
about. What was given up is real and is named here rather than glossed: this
sitting has **no reader's-eye verdict**, and the two options it might have
weighed — whether a newcomer can tell from the repository how to build the
compiler — went unjudged.

**Process note, and it is a breach.** The panel's rule is that the working tree
is frozen from briefs out to synthesis. It was not: commit `cf91a8c` landed at
00:39, during the sitting, from a **concurrent session** working on `site/`, and
it also touched `crates/heroes-cli/tests/milestones.rs` (+7). Two judges observed
the effect independently — the ffi-pragmatist reported the tree changing under
it, and the warden's file count (1,135) disagreed with the brief's (1,128) by
exactly the seven files that commit added. Nothing a judge measured depends on
`site/`, so no verdict is withdrawn. The disagreement is kept in the record
because it is CLAUDE.md §11's premise-expiry in miniature: **the brief's number
was true when written and false thirty-nine minutes later**, and the sentence
around it went on reading as correct.

---

## The proposal

M-selfhost-port closed 2026-08-17 at the fixpoint. This milestone's row says:
the seed test first, then `crates/` → `archive/bootstrap-rs/` — *"the third
language dies here"* (`docs/ROADMAP.md:99`), *"no third language anywhere"*
(`design.md:82`).

**A — the seed.** DESIGN-LOG:183 (2026-08-11) already ruled *what* the seed is:
the generated C, compiled by any clang, *"a release artifact tested from a clean
checkout **before** the archive commit"*. Undecided: whether that C is committed
to git, in what form, and when it is refreshed.

- **A1** commit the raw C, refresh only when the committed seed can no longer
  compile today's `selfhost/`
- **A2** commit it compressed
- **A3** do not commit it; attach it to a release
- **A4** commit it and refresh on every change to `selfhost/`

**B — the net.** CLAUDE.md §10's `cargo test` exception expires *at this
milestone*. Undecided: what runs the 208 golden cases and the corpus when the
Rust is archived.

- **B1** port the harness to Heroes before the archive
- **B2** a `heroes`-shaped subcommand
- **B3** archive the harness with the Rust, accept the smaller net
- **B4** split: the seed lands here, the archive becomes its own milestone

---

## The verdict table

| judge | A | B | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **approve A1**, trigger rewritten · **VETO A2** | **VETO B2** · object B3 · **approve B4** with B1 as its content | seed: **0 compiler lines**. B1: **2,500–3,760 Heroes lines**, centre ≈3,200, against the 4,279 it replaces | third stored seed: **5.78 MiB gzipped vs 2.74 MiB raw**; the 5.66 s clang rung catches every case the 17-minute rung would | A2 veto lifts on three *real* refreshes where gzip stays under raw; B2 veto lifts on a harness capability `heroes run` cannot express |
| ffi-pragmatist | **object** — the dependency list is wrong and the missing item is inside the archived directory | **object B3** · support **B4** | seed compiles under **six dialects**, `-O2`, `-pedantic-errors`, `-Wall -Werror`: all clean | a clean checkout + the bare clang line yields a compiler that refuses `read_file`, `args`, `map`, `filter`, `fold` with `builtin_shape`, never naming the library | A flips to approve when `seed/` carries the runtime **and** the Tier-2 library, and `LIBRARY_PATH` leaves `crates/` |
| historian | **approve A1** with three amendments | **object B3**; precedent supports **B4** | Zig regenerates `zig1.wasm` **~40 times since 2023**; Nim re-cut `csources` **three times** (2020-12-07, 2022-12-15, 2025-11-09) | `archive/bootstrap-rs/` is built, run or committed to at least once despite *"never maintained again"* | reverses on A if shown a project that froze a **generated** seed for years without a chain of working binaries |
| spec-warden | **approve A1**; A2 and A3 objected | **VETO B3** · approve **B4** | **spec delta 0**, verified; 3512 of 4096, headroom 584 | if B3 lands, `SPEC_TOKENS` and `heroes measure` diverge within two spec-amending commits | veto lifts on either `measure` ported with §1.6's citations amended, **or** B4 |

**Adopted, provisional — author ratification pending: A1 as amended, and B4.**
It is the most conservative resolution on both halves: A1 is the only option that
puts the artifact inside the checkout that the archive commit creates, and B4 is
the only option that does nothing irreversible.

---

## R1 — A1, raw, and the compression question is settled by a measurement

**A2 is not a small loss; it inverts.** The brief priced storage on **one**
commit — 2.55 MiB raw against 2.35 MiB gzipped, an 8% saving — and one commit is
the case that never recurs. The compiler-engineer priced three, by emitting
perturbed seeds and committing each into fresh repositories with `git gc
--aggressive`:

| stored form | 1 seed | +2nd (2,696 C lines changed) | +3rd (26,856 changed) |
|---|---|---|---|
| **raw `.c`** | 2.55 MiB | **2.57** | **2.74** |
| `.gz -9` | 2.35 MiB | 3.42 | **5.78** |
| `.xz -9` | 1.41 MiB | 2.83 | **4.24** |

Git delta-compresses plain text and cannot delta a compressed stream. A2 buys
0.20 MiB once and pays 1.07–2.36 MiB per refresh — **2.1× worse by the third
copy**. And the brief's own sentence *"a third again of the entire repository's
history, **per copy stored**"* was an inference presented as a measurement
(CLAUDE.md §1's first named shape): the second raw copy cost **+0.02 MiB**, 0.8%
of the first.

The historian adds the independent half: Zig originally committed
`zig1.wasm.zst` at 637 KB *because a recompiled wasm binary deltas badly anyway*,
and **today's tree carries the raw `.wasm`**. Heroes' seed is deterministic C
text from a fixpoint-proven emitter — the one input class that deltas *well*.
A2 also breaks `docs/ROADMAP.md:106` outright: a `.gz` needs a decompressor on
the one path that must not need one.

**A3 is the xz-utils shape.** CVE-2024-3094 lived in the release tarball's
`build-to-host.m4` and **not in the git repository**. A3 as written — build at
release time, attach to a release — is that shape exactly. It is admissible only
as rustc's: a committed sha256 beside a named URL. It is separately blocked today
by CLAUDE.md §14's hard stop on publishing.

**A4 loses on work, not on bytes.** Its storage objection evaporates (+0.02 MiB
per refresh) but M-selfhost-port was 152 commits, and 152 seed reviews is a
review nobody performs.

## R2 — the refresh trigger, because A1 as written rots in silence

*"Refresh only when the seed can no longer compile today's `selfhost/`"* is a
**negative condition, and nobody executes one.** The record is the historian's:
Go declared its 1.4 seed frozen and respun it at least twice
(`go1.4-bootstrap-20161024`, `go1.4-bootstrap-20171003`, *"plus accumulated fixes
to keep the tools running on newer operating systems"*), and Yocto was still
carrying a patch to drop it in **2024**.

And Heroes cannot do what Go did. **Go 1.4 is hand-written C that anyone can
patch; generated C cannot be hand-patched at all**, so a stale Heroes seed is
unrecoverable without a working binary. This is why the ROADMAP's citation of
ancestors is corrected below: the true ancestors are **Nim's `csources`** and
**Pascal-P4's kit**, not Go 1.4.

The trigger is therefore Zig's, executed Zig's way — **refreshed in the same
commit as the change that breaks it, by whoever has a working compiler in hand** —
and it is enforced by a rung cheap enough to run always. Measured, cheapest
first:

1. `HERO_RUNTIME_ABI` in the committed seed vs `runtime/heroes_runtime.h:31` —
   microseconds; catches the one skew the stamp exists for;
2. **`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed &&
   ./heroes-seed --version` — 5.66 s.** This is the trigger, executable;
3. full self-emission and hash — **17 min 02 s** at `-O0` (994 s user) and
   **12 min 29 s** at `-O2` (716 s user, peak RSS 2.55 GiB). Milestone close only.

## R3 — the seed's dependency list, which the brief got wrong

The ffi-pragmatist enumerated it and the brief's *"no flags, two inputs"* is
imprecise in three ways.

**Build time is 18 files**: `heroes.c` · `runtime/heroes_runtime.h` ·
`runtime/hero_os.h` · `runtime/runtime.c` · **14** × `runtime/parts/*.c` (which
`runtime.c` `#include`s — shipping it alone fails). And `-I runtime` **is** a
flag, mandatory, because `#include <hero_os.h>` is angle-bracketed.

**Run time needs two more**, and neither was in the brief: `runtime/` again (the
compiler compiles it), and `crates/heroes/src/library/source.hero` — which is R5.

**Dialects, measured on one clang** (Apple clang 21.0.0; `/usr/bin/gcc` on this
machine *is* that clang, so "any clang" is six dialects of one compiler):
`c11` · `gnu11` · `c17` · `gnu17` · `c23` · `gnu23` all exit 0 with identical
binary size; `-O2` exit 0 in 34 s; `-pedantic-errors` and `-Wall -Werror` give
**zero diagnostics**. `-Wall -Wextra -Werror` fails on 20 `-Wunused-parameter`
and `-Wconversion -Werror` on 20 `-Wimplicit-int-conversion`.

**One open prediction, stated as open**: the seed carries **no feature-test
macro** (`grep -c` for `_POSIX_C_SOURCE|_GNU_SOURCE|_DEFAULT_SOURCE` = 0) while
`runtime/runtime.c:78` carries a `__linux__` guard, so `-std=c11` is predicted to
fail on glibc at the seed's `unistd.h` extern. It could not be tested — no
container runtime on this machine — and it is recorded as untested rather than
asserted. The bare line passes no `-std`, which is why it is safe.

**Nothing in the seed is Darwin-conditional**: one hit for
`__APPLE__|__MACH__|TARGET_OS|__darwin`, and it is the string literal `"Darwin"`.
`nm -u` shows no libm symbol, so `-lm` is unneeded.

## R4 — B4: the seed lands here, the archive gets its own milestone

Every seat objected to B3 and the compiler-engineer vetoed B2. The
**precedent is unanimous and the historian searched for a counterexample**: no
self-hosting project in the sample (Go, Rust, Zig, Nim, GCC, LLVM, CHICKEN,
Guile) deleted its test harness at the archive. **Zero cases.** Go removed C from
the implementation at 1.5 (2015) and its test harness became an ordinary `go
test` at **1.21** (2023) — six years apart — and the thing it replaced was
`errchk`, **a Perl script**, which is Heroes' `#~` job in another language.
GCC's harness is DejaGnu/Tcl and predates its own C→C++ switch untouched.
`all.bash` is still bash today.

**What B3 actually deletes is not a smaller net — it is no net**, and the
compiler-engineer measured all three legs:

1. **Nothing runs `selfhost/`'s own tests.** `grep -rn "selfhost/main.hero"`
   across `*.rs *.yml *.toml *.sh` → **zero hits**. `.github/workflows/ci.yml`
   has **four** cargo steps (`:200` build, `:208` `run -- doctor`, `:212`
   clippy, `:217` test), and all four die at the archive — CI becomes empty. The
   tests do pass: `heroes test selfhost/main.hero` → **447 tests, all passed**.
2. **No selfhost test touches a golden.** `grep 'read_file("tests/' selfhost/*.hero`
   → zero. All **208** cases go dark: 65 check · 86 run · 20 ir · 13 fixedbugs ·
   11 unsupported · 6 emit · 7 surface-fixtures.
3. **`surface.rs`'s 68 tests have 17 in-process successors**, and they test
   functions rather than the binary: `cli_argv.parse(...)` returning a value is
   not `heroes check x.hero --brief` exiting 1 with that stderr.

Named tests with **no successor whatever**: `golden.rs:344`
(**CLAUDE.md §9's `#~` invariant**), `golden.rs:220` (panel 019's explicit
condition for having no desugared tree), `golden.rs:169` (§8's
`certain`-is-machine-applicable promise), `golden.rs:652` (**the double-emit
determinism test CLAUDE.md §7 says stays green at all times**), `golden.rs:570`,
`:811`, `:1012` (lldb), `corpus.rs:274` (14 programs × 3 configurations),
`corpus.rs:496` (the falsifier design.md wart 17 owes), `milestones.rs` ×5,
`layout.rs` ×1.

The warden's veto is narrower and sharper: **`heroes measure` is not in the
port** — `selfhost/main.hero:65-67` exits 2 saying so — and
`crates/heroes/src/measure/gate.rs` is the spec budget's only enforcer while
`design.md:322` and `:349` cite that file as live. Archiving it makes a Part 1
sentence false, which is itself a panel path. And **86 of 208 golden `.hero`
files carry 184 `#~` annotations** (recounted by the coordinator: 86 and 184,
identical), enforced solely by `golden.rs:344`. B3 does not forge that invariant.
It makes it **unobservable**, which §9's own words make worse: an invariant that
cannot fail is a decoration.

**B2 is refused by §10's own text.** The stopping rule's third clause is
*"nothing if two existing invocations already compose to it"*, and the
composition **runs**: the compiler-engineer ported `golden.rs`'s check-case
runner to **79 lines of Heroes** and got **65 of 65 cases green**, using only
what the language has today. B2 buys ~9 lines of table surface and moves the same
~3,200 lines **inside `selfhost/`**, where the §11 file rule binds them, where
they ship in every user's binary, and — the sharp one — where **every edit to a
test changes `gen1.c` and invalidates the committed seed**. That couples A to B
for no gain.

**B1 is milestone-sized, measured three ways.** Port ratio code-only **0.766**;
kind-matched on the OS-facing modules **0.675** and **0.586**; direct, from the
79-line probe, **0.878**. Against a denominator of **4,279 lines** (the brief's
4,165 omitted `expectation/mod.rs`, 109 lines, which both `golden.rs` and
`corpus.rs` `mod`-in), that is **2,500–3,760 Heroes lines, centre ≈3,200** —
3–13× the largest budget in the ROADMAP's own price list (M-separate-compilation
at +700–1000, QBE at ~500, LSP at ~250). Plus `layout.rs` is a **rewrite, not a
port**: it walks `crates/**/*.rs`, the compiler becomes `selfhost/*.hero`, and
**29 of 143 selfhost files already exceed §11's 300 lines**, so the rewritten
test goes red on day one.

## R5 — the blocker: `LIBRARY_PATH` points inside the directory being archived

Found independently by two seats and reproduced by the coordinator.

```
selfhost/main.hero:44   constant LIBRARY_PATH: str
selfhost/main.hero:45       "crates/heroes/src/library/source.hero"
selfhost/main.hero:77       library = read_file(LIBRARY_PATH).default("")
```

The Rust bootstrap does the opposite — `crates/heroes/src/library/mod.rs:37` is
`include_str!("source.hero")`, and `source.hero:4` says in its own words *"it is
`include_str!`d into the binary, so there is no discovery"*. **The port reversed
that decision silently**, and `.default("")` makes the absence quiet: the library
becomes empty and every program using it is refused **on the author's line**.

Measured, same seed-built compiler, one three-line program, run outside the
repository root:

```
error[builtin_shape]: `range` does not take 2 argument(s) of those types
  at p.hero:2:14   → exit 1        (the port)
                     exit 0        (the bootstrap, same directory)
```

The ffi-pragmatist reproduced the same shape for `read_file`, `map`, `filter` and
`fold`. This is not only an archive blocker: **it is live today** — the
self-hosted compiler is unusable outside the repository root, and it reports the
compiler's missing file as the author's mistake, which is the failure class this
project exists to abolish.

`selfhost/cli_verbs.hero:204` reads the same path inside a test block, and the
string appears once in `gen1.c` — a committed seed would ship the dead path baked
in.

**The archive commit does not happen until this is repaired.** That alone decides
B4 independently of every argument above.

## R6 — three more divergences between the two compilers, none of which any existing instrument sees

The coordinator compared the **emitted C** of the bootstrap and the port over
**127 programs** (`tests/golden/{run,emit,ir}` + every `examples/*/main.hero` and
`whole.hero`). This comparison had never been run: the M-selfhost-port
differential compared **streams** over 257 inputs, and `--emit-c -o file` writes
an artifact rather than a stream, so emitted C was compared for exactly one
input — `selfhost/main.hero` itself.

| | programs |
|---|---|
| byte-identical | **26** |
| differ **only** in the `#line` restore file name | **81** |
| differ in more | **20** — 19 by float-literal spelling, 3 by a fixed-array read |

**D-A — the `#line` restore names a file that does not exist.** The bootstrap
writes `#line 43 "whileloop.c"` (`emit/mod.rs:198` passes `module_of`, the
alphanumeric-sanitised stem); the port writes `while-loop.c`
(`selfhost/emit.hero:147` passes `source.stem_of`, the raw stem). The port is
**internally inconsistent rather than merely different**: the file it actually
writes on disk is `build/<key>/ffiachararraymember.c`, sanitised — measured from
its own error message. The bootstrap is right and the port has the bug.

**D-B — the port emits an undeclared identifier, and `heroes build` is exit 2**
on three programs (`tests/golden/run/ffi-a-char-array-member.hero`,
`run/ffi-a-c-array-member.hero`, `examples/raylib/main.hero`):

```
internal error: compiling the generated C failed:
  error: use of undeclared identifier 't20'
   44 |     t22 = t20[((uint64_t)(t21) >= UINT64_C(4) ? … : (t21))];
```

`t20` appears **once** in the whole file, on the right of a read. The cause is
written in the port's own hand at `selfhost/emit_container.hero:77-79`:

> `# PORT note: the base spelling routes through storageless.fixed_text when that
> module lands; the Rust fallback (the plain temporary) is what this writes
> meanwhile — the same bytes for every case the tests cover today.`

`emit_storageless.hero` **did** land (`emit_construct.hero:53` calls it) and this
call site was never rewired. This is the same shape that was the last obstacle
before the fixpoint — a note written to the port and left unpaid — with the
difference that the port wrote this one to itself. And the clause *"the same
bytes for every case the tests cover today"* is CLAUDE.md §11's premise that
expires in silence: it was true only because **no test covered it**, because the
goldens have never been run under the port.

`selfhost/emit_gate.hero:271-273` names both consuming positions — *"a subscript
base, a construction argument"* — and the code honours one.

**D-C — float literals.** The bootstrap emits C hex floats (`0x1.4p+3`,
round-trip-exact, design.md §3.1); the port emits the spelled text (`10.0`). 19
of the 127 programs carry it, `examples/json/main.hero` among them. Already filed
at `docs/debrief/DECIDE.md:367` as an open author decision — recorded here
because it is the third member of a class, and because the ffi-pragmatist
measured why nothing catches it: the seed C contains **zero** hex float literals,
no `tests/golden/emit/*.expected` contains one, and `corpus.rs` compares program
*output*, where the two values are equal.

## R7 — two more boundary defects the ffi-pragmatist found on the way

**D1 — the bootstrap and the port disagree on what FFI is legal, and the port's
check is reachability-dependent.** With an identical `extern` group binding
`dirent.h`:

| body | port | bootstrap |
|---|---|---|
| reads `e.d_name[0]` | **exit 1** `ffi_parameter_type` | exit 0 |
| does not read it | exit 0 | exit 0 |

Whether a **declaration** is legal turns on an unrelated expression elsewhere.
§4.19 checks a declaration against a header; that must be a property of the
declaration. And it means the archive is **not behaviour-preserving at the C
boundary**.

The finding underneath it answers the brief's question, which was put as a
question precisely because a negative claim rests on the searcher's vocabulary:
**a Heroes program can list a directory today, with no shim and no `system()`** —
`readdir_r` with `record Dirent tag dirent partial` and `d_name: i8[1024]`, run
against the bootstrap, printing real names, **clean under ASan and UBSan**. The
brief's premise (*"the language has no readdir"*, taken from
`selfhost/cli_toolchain.hero`'s own module doc) is **false under the bootstrap and
true under the port**. Under the port there is no second route either: `readdir
-> Dirent` is `ffi_return_type`, a `ptr` into a record is `type_mismatch`,
`to_str(ptr)` is `bad_operand`, `d_name: cstr` is `ffi_field_type`.

**And the shell fallback is unsound — measured, not argued.** In a directory
holding `plain.txt`, `with space.txt`, `with\nnewline.txt` and `quote's.txt`, the
`readdir` route returns **6 entries** with the newline name arriving as one entry
of length 16; `ls -a > file` + `read_file` returns **7 "entries"**, splitting one
name in two, **at exit 0**. A silent wrong answer is exactly what §1.12 and
CLAUDE.md §12 exist to kill, so the ten-line fallback the brief proposed is
refused on its own measurement.

**The constructive half, and it needs no new surface**: `popen`/`fgets`/`pclose`
from `stdio.h` plus `malloc`/`free`, measured working **on both compilers with
zero warnings**, captures a command's combined output as a `str` and returns the
raw wait status (`768` for `exit 3`, so the code is `status / 256`). That is a
golden harness's entire requirement — run `heroes`, capture both streams, learn
the exit code — with no temp file, no shell redirection and no third language.

**D3 — §7's exit-1 narrowing has a fifth class, in both compilers.** `d_name: i8`
— a scalar where the header says `char[1024]`, the first thing an author would
try — is **exit 2, `internal error: compiling the generated C failed`**: the
compiler blaming itself for the author's `extern`. Adjacent shapes checked, and
this is the discipline CLAUDE.md §1 asks for: `i64` → the same exit 2; `cstr` →
correct exit 1; `i8[256]` → correct exit 1; a function-pointer member as `ptr` →
correct exit 0.

## R8 — corrections to the record, each measured today

- **`docs/ROADMAP.md:105-106` and `DESIGN-LOG:183` cite the wrong ancestors.**
  *"as Go shipped 1.4 and Zig ships its bootstrap"*: Go 1.4 is a hand-written C
  toolchain distributed as ordinary source, never a generated artifact and never
  committed as one; Zig ships a 3,183,342-byte WebAssembly binary converted to C
  at build time by a separate tool. The true ancestors are **Nim's `csources`**
  (generated C, committed, re-cut 2020-12-07, 2022-12-15, 2025-11-09) and
  **Pascal-P4** (1976: the compiler *and* its self-translation, still fetchable).
  The ruling stands; the citation is corrected, because the difference is
  load-bearing — Go could patch its seed and Heroes cannot.
- **SQLite's amalgamation is struck from the comparison**, not merely
  distinguished: `sqlite3.c` is generated at release time and is **not in version
  control at all**, so it supplies no evidence about committing anything.
- **`selfhost/` has 447 tests, not 27,230.** `heroes test selfhost/main.hero`
  reports `447 tests, all passed` in 10.3 s (release bootstrap). `docs/ROADMAP.md:26`
  and `docs/journal/021-selfhost-port.md` (three places) carry 27,230. This
  session could not reproduce that figure by any counting method it tried and does
  not know what it counts. The live status is corrected; the journal is a dated
  record and is not rewritten (CLAUDE.md §14).
- **The Rust harness is 4,279 lines, not 4,165** — `expectation/mod.rs` (109) was
  omitted, and `milestones.rs` gained 5 during the sitting.
- **CI has four cargo steps, not one.**
- **`HEROES_RUNTIME` means two different things.** The bootstrap honours the
  environment variable; the port reads a **file** of that name — measured both
  ways — while the diagnostic both print says *"set `HEROES_RUNTIME=<dir>`"*,
  which reads as an environment variable. §4.17 makes that the compiler's bug.
- The `spec-warden` seat's brief asserted its own ceiling and **grepped
  `design.md` §1.6 rather than trusting it**, per the repair panel 069 made to
  that seat's definition. The ceiling is 4096 and the brief was right — the point
  is that it was *checked*.

---

## What a veto would compel

**A2's veto (compiler-engineer)** compels nothing to be built: it forbids a
storage form. Lifting it needs three consecutive real refreshes where the
gzipped pack stays under the raw pack.

**B2's veto (compiler-engineer)** compels the harness to be a Heroes *program*
rather than a subcommand. Lifting it needs one golden-harness capability that
`heroes run <harness>.hero` cannot express and a subcommand can — not reachable
through `system()`, `popen`, `read_file`, `write_file`, `args()`. The probe found
none for the `check/` class; the lldb and ASan classes were **not** tried, and the
seat said so.

**B3's veto (spec-warden)** compels one of two things before any archive commit:
`measure` ported with the ledger gate re-homed and `design.md:322`/`:349` amended
in the same commit, **or** B4.

---

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | a third stored seed costs **> 5 MiB** gzipped against **2.74 MiB** raw; rung 2 (`clang` + `--version`) runs under 15 s and catches every case rung 3 would | the second seed refresh after M-selfhost-fixpoint |
| compiler-engineer | without repairing `selfhost/main.hero:45`, `heroes run` on any program calling `range` exits 1 with `builtin_shape` | **reproduced today** — score as confirmed at this milestone's close |
| compiler-engineer | the ported harness lands between **2,500 and 3,800** lines against the 4,279 it replaces | the close of the milestone carrying B1 |
| ffi-pragmatist | `-std=c11` on **glibc** fails at the seed's `unistd.h` extern, for want of a feature-test macro | the first CI run of the seed on Linux |
| ffi-pragmatist | `readdir_r` is still unbindable under the port at M-struct-passing; SQLite keeps working throughout (measured today under the **seed-built** compiler: `rows: 3`) | M-struct-passing |
| ffi-pragmatist | if `golden.rs` is not ported before the archive, a **second** emitted-C divergence lands within one milestone | the milestone after this one |
| historian | `archive/bootstrap-rs/` is built, run or committed to at least once despite *"never maintained again"* | two milestones after M-selfhost-fixpoint |
| spec-warden | if B3 lands, `SPEC_TOKENS` and `heroes measure` diverge within two spec-amending commits, drift ≥ +30 by M-separate-compilation | M-separate-compilation close |
| spec-warden | under A1, `git count-objects -vH` ≤ 10.0 MiB and the seed has ≤ 2 blobs at M-separate-compilation close; more means A1 was A4 in practice | M-separate-compilation close |

## Predictions scored — at M-bootstrap-archive close, 2026-08-19

Measured in the session that writes this, and the two that could not be scored say
so rather than being renewed (panel 046 R2).

| judge | prediction | outcome |
|---|---|---|
| compiler-engineer | `heroes run` on any program calling `range` exits 1 with `builtin_shape` unless `selfhost/main.hero:45` is repaired | **held, and the repair is in.** It was reproduced at the sitting; `main.hero` now reads the library through `library_source.text()` — compiled in, not read from disk — and the archive cannot break it |
| ffi-pragmatist | `-std=c11` on **glibc** fails at the seed's `unistd.h` extern | **instrumented, not yet scored.** A CI step exists for it now (Linux only, reporting), which is the only way a prediction pays. Measured on Darwin the same day: `-std=c11` builds the seed in 3.6 s and produces a working binary — as panel 047 predicts, since `__STRICT_ANSI__` hides POSIX names on glibc and not on Darwin's headers |
| ffi-pragmatist | if `golden.rs` is not ported before the archive, a second emitted-C divergence lands within one milestone | **void: the antecedent was removed.** `golden.rs` was ported at M-harness-port, one milestone after this sitting, so the condition never obtained. Recorded rather than dropped, because the reason it is void is the thing the sitting asked for |
| spec-warden | if B3 lands, `SPEC_TOKENS` and `heroes measure` diverge within two spec-amending commits | **void: B4 landed, not B3.** The successor check exists (`suite_spec.hero`) and panel 086 R2 added the ledger agreement it did not have |
| compiler-engineer | a third stored seed costs **> 5 MiB** gzipped | **not yet due** (its checkpoint is the second seed refresh after M-selfhost-fixpoint; this milestone is the first). Today's data point, measured: one seed is **21.01 MiB raw and 2.69 MiB gzipped**, git holds **3** versions of it, and `git count-objects -vH` reports **12.99 MiB** |
| spec-warden | under A1, `git count-objects -vH` ≤ 10.0 MiB and the seed has ≤ 2 blobs at M-separate-compilation close | **not due, and already past both numbers**: 12.99 MiB and 3 versions today, one milestone early. Written down here so the M-separate-compilation close scores it against a measurement rather than a memory |
| historian | `archive/bootstrap-rs/` is built, run or committed to at least once despite *"never maintained again"* | **open, and the clock starts now.** The directory exists as of 2026-08-19 and its own README is the only thing committed into it since. Nothing builds it and CI does not mention it |
| compiler-engineer | the ported harness lands between 2,500 and 3,800 lines | scored at M-harness-port close: **3,630** |

---

## Author's verdict

**Ratified 2026-08-23** (author instruction, *"ratifica tutto"* — blanket over
panels 085, 086 and 087, recorded as one; this section says what the yes does and
does not settle here).

**R1 (A1, the seed committed raw) and R4 (B4, the archive as its own milestone)
are final**, and so are R2, R3, R5–R8 as they landed. The yes is not a rubber
stamp on a closed milestone: the artifact was re-verified in the session that
recorded this verdict — `clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes` in **3.75 s**, and the compiler that line produces then ran the whole net
at **845 passed, 0 failed, exit 0**. That is R1's claim executed four days after
the tag, on a machine that had nothing else of the compiler on it.

**What the yes does not settle, and both keep their conditions unchanged.** The
compiler-engineer's two vetoes stay armed: **A2** (compressing the seed) lifts only
on three consecutive real refreshes where gzip stays under raw, and **B2** (a
`heroes golden`-shaped subcommand) lifts only on a harness capability that `heroes
run <harness>.hero` cannot express — §10's third clause refuses it until then. And
**R7's mirror is still open**: a fixed array declared where the header has a scalar
(`d_type: i8[4]` against `unsigned char`) is `internal error` at exit **2**, the
compiler blaming itself for the author's `extern`, and it waits in `DECIDE.md` with
its repair undecided.

---

**Scored at M-separate-compilation close, 2026-08-26.** This sitting's
predictions are scored in `docs/journal/025-separate-compilation.md`
§ *Predictions, scored*, with what each was measured against. Predictions that
could not be scored are marked **lapsed** there, and the clause each one bought
is back in `docs/work/DECIDE.md` to be re-argued under the removal branch, never
renewed under a new milestone name (panel 046 R2).
