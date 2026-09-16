# Panel 157 — compiler-engineer report (SOUNDNESS LANE)

**verdict: object** — no veto. Nothing here adds a construct to the checker, the
lowering or the backend; the ceiling (design.md §1.1, §1.7) is only reachable by
ONE of the proposed routes, and I object to that one by name.

All numbers measured 2026-09-16 in a scratchpad copy of the frozen tree. The
seed compiler built in **real 3.22 s**.

## 1. The defect reproduces

```
./heroes build tests/golden/surface-fixtures/structtag/main.hero --emit-c -o st.c   # exit 0, 271 lines
clang -I runtime -I tests/golden/surface-fixtures/structtag st.c runtime/runtime.c  # exit 1
```
`grep -c "error:"` = **20** (clang's default cap of 20; the brief's 15 is a
different invocation, not a contradiction). All are
`must use 'struct' tag to refer to type 'probe'`.

## 2. The measurement that decides R2: NO clang-free rule is right

Three header shapes against three spellings, each exercising a real call so
pointer compatibility is tested, `clang -std=c11
-Werror=incompatible-pointer-types -fsyntax-only`:

| header | bare `probe *` (today) | `struct probe *` | `typedef struct probe probe;` shim |
|---|---|---|---|
| `struct probe {…};` only | **FAIL** must use 'struct' tag | ok | ok |
| `typedef struct probe {…} probe;` | ok | ok | ok |
| `typedef struct {…} probe;` (anonymous) | ok | **FAIL** incompatible pointer types | **FAIL** typedef redefinition with different types |

**The brief's unlisted shim route is dead, measured.** It fails on the
anonymous-typedef header with `typedef redefinition with different types
('struct probe' vs 'probe')` — the same shape that kills always-`struct`. It is
the always-qualify route wearing a different hat and it costs a line on every
emission to buy nothing. Panels 151 and 152 are confirmed independently here:
**clang is the only oracle the compiler can afford.**

### What would have to be true for a fourth route to exist

A fourth route needs an oracle that separates row 1 from row 3 without running
clang. That oracle is a **C declaration parser inside Heroes** — and that is the
one answer my seat vetoes on the ceiling: design.md §1.7, "anything in the core
must be implemented in the type checker *and* the lowering *and* the backend",
§1.1, "implementation simplicity sets the ceiling". A C frontend is not 50 lines
in `emit/ffi_tag.hero`; it is a second language in the compiler. Panel 151 had
already measured the cheap version of it wrong (`record Db tag sqlite3_stmt`:
two false `error[ffi_parameter_type]`). `clang -E` plus a textual scan is the
same family and inherits the same refutation.

## 3. Costs, grounded in files

| module | lines now | what the repair does to it |
|---|---|---|
| `selfhost/cli/compile.hero` | 399 | the `--emit-c` write and halt at `:257-266` moves OUT: about -10 |
| `selfhost/cli/produce.hero` | 268 | gains the gated probe loop and the write: **+25 to +35** |
| `selfhost/cli/assemble.hero` | 213 | unchanged — the build's round is not touched |
| `selfhost/emit/ffi_tag.hero` | 311 | unchanged — `needs_struct` is reused verbatim |
| `tests/harness/suite_emission.hero` | 572 | comments at `:70` and `:203-206` are rewritten; **+20 to +30** for the R4 leg |

**The bug cannot be fixed where it is.** `compile.hero`'s `use` list (`:17-40`)
has no `cli/toolchain`; it is the frontend and knows nothing about clang.
`cli/toolchain.hero` uses only `io deps digest flags process runtime_key`, so no
cycle would form — but the seam that respects the module docs is to move the
`--emit-c` write into `produce.hero`, which already holds the toolchain, the
build directory, the runtime object and the resolved package flags (`:104-141`).
Net compiler delta: **under 40 lines, entirely in `cli/`**. No lexer, no
checker, no descriptors, no ownership, no new IR node. Pascal-P4 scale is not
threatened.

## 4. The cost the objection to R2 rests on is 0.74 s, and gated it is zero

`seed/heroes.c` is **820,861 lines**. `clang -std=c11 -I runtime -fsyntax-only`
over it: **real 0.74, user 0.72, sys 0.02** (`/usr/bin/time -p`, machine still).
That is the *ungated* worst case of an advisory probe. **Gated on the presence
of a tagged handle it is zero**, because `selfhost/` declares **0**
`record … tag` at column 0 (24 loose grep hits, none a declaration). The seed
recipe (`seed/README.md:85`, re-emitted in CI at
`.github/workflows/ci.yml:692`) pays nothing today and pays 0.74 s on the day
the compiler binds a tagged handle — the day it would otherwise emit a seed that
does not build.

## 5. R4, and it found three live files

240 blessed emissions in `tests/emission/`. Syntax-checked each with `-I runtime`
plus every directory under `tests/golden/` and `examples/` holding a `.h`:

- **28 fail** `clang -fsyntax-only`;
- **3 fail with `must use 'struct' tag`**, and all three are `run/` programs that
  build and run green today:
  - `tests/emission/run-fixedbugs-a-tag-that-needs-struct.c`
  - `tests/emission/run-fixedbugs-a-tag-names-a-handle-and-a-record.c`
  - `tests/emission/run-fixedbugs-getaddrinfo-is-bindable.c`
- 17 of the remaining 25 are `fixedbugs-ffi-*`, deliberately wrong bindings whose
  refusal is the point; 2 (`examples-raylib`, `examples-sdl`) lack the library;
  4 need package flags I did not resolve; `ir-owned-cell.c` and
  `ir-regression-extern-parameter-types.c` I did **not** run down — UNRUN.

**Defect 048 is already committed, blessed, and compared byte-for-byte on every
emission run.** The store is a byte comparison, so it cannot see that the bytes
it is protecting do not compile.

## 6. Verdicts

**R1 — `--emit-c` owes C that compiles.** But I will not rest that on the shared
brief's "two documents that agree": design.md:717's own words are *"stop at the
C and read it (stdout, or -o)"*, which is ambiguous between an output and a
dump. **design.md does not settle R1**, and saying so is cheaper than inventing
a rationale. What settles it is the seed: `--emit-c` is how `seed/heroes.c` is
made and CI re-emits it. Redefining `--emit-c` as "the first emission" makes the
bootstrap artifact one whose compilability is nobody's contract.

**R2 — an ADVISORY probe, gated on the presence of a tagged handle.** Emit, run
`clang -fsyntax-only`, harvest `ffi_tag.needs_struct` from its stderr,
**discard its verdict**, re-emit, write. It never turns a success into a
failure: on a machine lacking the header it degrades exactly to today's output.
**I reject the refusal (exit 2)**: it removes a capability, and it removes it
precisely on the day the compiler binds a tagged C library, which is the day
`--emit-c` must work or there is no seed. That is the cheapest option and, by
CLAUDE.md § Precedence ("the most robust and production-ready resolution, never
the easiest, the compromise, or the cheapest in tokens"), disqualified.

**R3 — the fourteen keep emitting, and get STRONGER.** The probe's verdict is
discarded, so a wrong FFI binding still emits. The premise's *substance* holds;
its stated *reason* at `suite_emission.hero:70` ("`--emit-c` never calls clang")
becomes false and must be rewritten to "`--emit-c` never lets clang's verdict
decide whether to write." Better: the R4 leg turns those cases from *they emit*
into *they emit AND clang refuses them*, which is the assertion they always
wanted. Re-blessing is bounded and the mechanism exists: `UPDATE_EMISSION=1`
(`suite_emission.hero:128`, `:263`), which is not the forbidden
`UPDATE_GOLDEN=1`.

**R4 — yes, in `emission`, and it is the one change I would make even under
R1-as-dump.** One `clang -fsyntax-only` per blessed file, with the expected
verdict read off the golden tree: `run/`, `emit/`, `ir/` must pass; `fixedbugs/`
must fail. It costs the suite about 0.1 s per case and it is the only thing in
the tree that would have seen 20 clang errors that every suite was blind to.

## 7. Prediction, condition, veto

**Prediction (falsifiable, at the close of the milestone that lands defect
048).** With the advisory probe and the R4 leg: the compiler grows **fewer than
40 net lines**, all under `selfhost/cli/`, with `selfhost/emit/ffi_tag.hero`
unchanged at 311 lines; **exactly 3** files in `tests/emission/` change bytes,
the three named in §5; the `emission` suite's case count does not fall; and
`heroes build selfhost/main.hero --emit-c` stays within 1 s of its pre-repair
time, because `selfhost/` declares no tagged handle and the probe never fires.
If more than 3 blessed files change, my gating analysis was wrong.

**Condition that flips me.** Two. (1) A measurement showing a header shape my
table missed where `-fsyntax-only` harvesting gives a spelling the linking build
would not — then the probe is not equivalent to the round and I would demand the
full round. (2) A measured `-fsyntax-only` cost above ~5 s on any program in
`examples/` — then the gate must be narrower than "has a tagged handle".

**Veto: not cast.** It is held, and it is aimed at exactly one thing: a C
declaration parser inside Heroes to learn tags without clang. That breaches
design.md §1.7 and §1.1 and I would veto it on sight.

## 8. UNRUN

- The repair itself. `./heroes build selfhost/main.hero -o heroes-next` (59 s)
  after applying it, then `./heroes run tests/harness/main.hero -- ./heroes-next
  emission`, is what would settle the line count and the 3-file prediction.
- Why `tests/emission/ir-owned-cell.c` (`call to undeclared function 'describe'`)
  and `ir-regression-extern-parameter-types.c` (`-Werror,-Wshorten-64-to-32`)
  fail syntax-only. Likely my include set and warning flags rather than the real
  build's; settled by re-running with the flags `cli/units.hero` actually passes.
- Whether `heroes test`'s fused-text path (`compile.hero:233-238`) is reached by
  the move. It takes `wants_text` but NOT the `options.emit_c` branch at `:257`,
  so on reading it is untouched; settled by `./heroes test selfhost/main.hero`
  after the repair.
