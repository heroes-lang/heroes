# 025 — M-separate-compilation: one `.c` per module, and the cache that cannot lie

**Closed 2026-08-26**, tag `m-separate-compilation`.

## Goal

The second half of the modules row, moved past the fixpoint on purpose: it costs
+700–1000 lines against the namespace's ~+330, Nim has not finished its own
per-module cache since 2018, Rust shipped 1.52.1 to *disable* incremental — and,
measured, it is where §4.19's guarantee can quietly die.

Four acceptance rows lift the ffi-pragmatist's veto (panel 030 R2), and all four
are closed:

| row | what it demands | closed by |
|---|---|---|
| 1 | the header travels with the `extern`, or an `extern` never crosses a module boundary | step 5 |
| 2 | headers and link flags enter the cache key | step 7 |
| 3 | every dependency's emitted interface enters the key | by construction, measured at step 8 |
| 4 | a two-module FFI case with a wrong `extern` signature | step 10, in panel 093 R5's re-reading |

Eleven steps, 72 commits, four repairs that came before any of them.

## What surprised

**The first per-module self-build failed with 20 clang errors, and the failure
was the finding.** Every *section* of a translation unit was answering *what do I
need* for itself, and four answers cannot share one file: descriptors came from
the whole program's aggregates while typedefs came from the module's reach;
prototypes came from all 1,318 functions while the types naming them came from
the module; a record's `_hash` called its field's, which the reach walk never
reaches because it stops at a declared type; a `T?` took its payload's descriptor
address where the descriptor set was narrower. One set feeds all four now, closed
the way C reads a declaration rather than the way the arena holds a type.

**A cache key can be blind to its own input by construction.** An `extern
constant` emits as `return CONF_LIMIT;` whatever the header holds, so the emitted
C is byte-identical across a header edit and a key containing the whole emitted C
cannot see it. Measured before the repair: a program printed `1` at exit 0 with
the header on disk saying `2`, and then `1` again with it saying `7`. The repair
is clang's own dependency listing, riding the compile that is already happening.
**A build system's worst outcome is a wrong answer that looks like a right one**,
and §12 puts stopping that ahead of speed by name — the freshness check costs
+2.3% cold and nothing at all warm.

**The frontend was not slow, it was quadratic, and one function was 99.4% of it.**
The open item asked which architecture to change and the answer was that no
architecture was wrong. `cursor.line_col` answers with a line *number*, so it
walks from byte 0; `take_docs` asked it once per declaration and once per comment;
the parse runs over the whole concatenated Source. 21,119 of 21,254 `sample` hits.
The rule this leaves behind: **ask for the distance you need, not for the position
you could derive it from**. Three more instances of one field-place push followed,
found by grep rather than by profile, and `heroes check` on the compiler's own
source went **88.0 s → about 8**.

**A check nobody runs is a decoration, and the suite that says so cannot see its
own subject.** `suite_annotations` is §9's one unforgeable check, and it sweeps
two directories because its mechanism compares a `.hero` with its `.expected`.
Measured while annotating step 10's fixture: 15 annotations in 11 `fixedbugs/`
files and 4 in 3 `surface-fixtures/` files are outside it, against 181 inside —
in the directory §9 created for defects that already shipped.

## What broke and why

**A comment claimed a second code path had the same defect, and it was false.**
`cli_units.hero` said *"the fused path has the same gap today, so nothing
regresses here"* — an inference presented as a measurement. The fused path
recompiles its unit on every call, so it caught the header edit and the
per-module path was the only one that missed it. The corrected sentence stays in
the file with the falsification beside it, because the reason it was wrong is the
reusable part.

**A repair landed for the shape that provoked it and not for the one beside it.**
The build cache was fixed; the *runtime's* object had the identical hole, keyed
on the runtime's own sources and nothing else, so `runtime.c` including
`<stdio.h>` was outside the key. That failure is worse than the provoking one: a
libc ABI mismatch inside a single binary, reached with nobody touching a line of
Heroes. Caught before the commit, which is CLAUDE.md §1's fourth clause spent
rather than quoted.

**A number in the record was 1.9× the truth and a decision was about to rest on
it.** The open item carried per-module cold 261 s / warm 250 s from 2026-08-25;
re-measured at step 7 with the pre-step-7 binary rebuilt from the committed seed
as control, the same commands gave 142 s / 133 s. No cause is asserted, because
none was measured. The old pair keeps its date and the new one sits beside it.

**An inference about the file split was written and then falsified before it
reached the record**: that splitting two files would cost speed because clang
cannot inline across a translation unit. Measured both ways on one source — fused
9.90 s, per-module 10.29 s — inside this machine's noise, which is itself worth
recording: repeated runs vary 5.2 s to 12.6 s for the same binary and input.

**The language chose where code lives, twice.** `cli_toolchain` had to ask
`cli_deps`, `cli_deps` had to hash a file, and `digest` lived in `cli_toolchain`:
a module cycle, which Heroes refuses on the `use` edge whatever it carries. So
`digest`/`hex8` became `cli_digest.hero`, which is where §11's single-concern rule
put them anyway. Panel 031 R6's *"reversible direction"* paying for itself in the
small.

## Predictions, scored

Scored at this close, with what each was measured against. A prediction is scored
or it lapses; none is renewed under a new milestone name (panel 046 R2).

| seat | prediction | verdict |
|---|---|---|
| 093 compiler-engineer | the milestone diff lands ≤1000 lines | **false** — `selfhost/` alone is +3,343/−750; with harness and goldens +6,148/−1,292. The milestone absorbed four repairs, the frontend and eleven steps the sitting did not price |
| 093 compiler-engineer | the two-module harness case compiles per-TU with zero clang warnings | **true** — `warnings` 111/111 on a green net |
| 093 compiler-engineer | the tag-reorder golden prints the correct case on a warm cache | **true**, and it is an instrument now: three orderings put `blue` at tag 2, 0 and 1 in the *caller's* C, each under a different key |
| 093 compiler-engineer | panel 030 prediction 8 scores false — placement near-zero, cost in key/interface machinery | **true**, and therefore **030 #8 is false**: placement is a macro plus one reach filter, while the key machinery is `cli_deps.hero` (307 lines) + `cli_digest.hero` (59) + the rewrite of `cli_units.hero` |
| 085 spec-warden | `SPEC_TOKENS` and `heroes measure` diverge, drift ≥ +30 | **false** — both read **3592**, divergence zero. The mechanism it doubted works |
| 085 spec-warden | `git count-objects -vH` ≤ 10.0 MiB and the seed has ≤ 2 blobs | **false, and by a wide margin** — 45.02 MiB, 19 seed versions in history. The sitting had already noted itself past both numbers one milestone early |
| ledger 3541 | a `run/` golden handed one ill-formed argument among three prints the other two and exits 0 | **false** — no file in `tests/golden/` or `examples/` uses `args_checked` at all, and `suite_run` has no way to pass argv. The +11 tokens this bought go back to `DECIDE.md` |
| ledger 3560 | every C `size_t` parameter in `examples/` is declared `u64`; no FFI golden declares an integer parameter narrower or differently signed than its header | **vacuously true on the first half** (no `size_t` parameter exists in `examples/`), **true on the second** — the four probe errors are live and the net is green |
| ledger 3592 | the four probe errors produce zero false fires, and no fifth value-changing direction is found | **true so far** — net green, and `git log` over `tests/golden/fixedbugs/` shows no case falsifying the *except* list |
| 087 spec-warden | `heroes measure` prints `maximum 3512`; this sitting spends zero | the count is **3592** today, and the delta is other sittings' recorded spending (092's +12 and +32, the ledger's +11), not 087's. **Zero-spend half stands; the absolute number is superseded** |
| 092 × 4 | the parenthesised callee's 146 files / 922 lines; `-Werror=incompatible-pointer-types`; P2's shim; S3e's 11 tokens | the landed ones scored in the step-3 record; the two that name **options the sitting did not adopt** are **lapsed** — they cannot be scored without building what was refused |
| 092 llm-ergonomist | generated callers convert at a size argument in a majority of samples | **lapsed** — it needs a sampling experiment nobody ran, and the clause it bought returns to `DECIDE.md` |
| 092 historian | no system checks a hand-written parameter width against a real header without parsing it | **not falsified** — no counter-example offered |

## What landed, and what carried forward

Steps 1–10, plus four repairs that preceded them: `docs/defects/002`, a dead
descriptor in every unit that named a container type it never built, the
compiler's last POSIX binding (which made the self-hosted compiler unbuildable on
Windows), and the C text boundary closed over two sittings — which found, on the
way, that the shipping compiler could be killed by an environment variable.

At the close: the compiler is **39,739 lines of Heroes in 165 files**, builds
itself as **157 translation units**, and emits **793,526 lines** of C that
rebuild it byte for byte. `heroes check` on its own source is **about 8 seconds**,
down from 88. The net is **1,178 checks**, the compiler's own tests **512**.

**What did not get delivered, and it is measured rather than glossed**: the
per-module build still does not beat the fused one. Re-measured at the close —
per-module cold 57.9 s, warm 45.2 s; fused emit 40.4 s plus one clang line 3.7 s.
The gap is about a second, where it was 8.7 before the frontend repair and where
the record once carried 127. **What dominates now is the per-TU emission, and
that is architectural rather than a defect**: panel 093 R4 puts the emitted text
*in* the cache key, so a warm build must emit all 157 units to learn that it may
reuse their objects. A cache cannot skip the work that computes its own key. That
is the shape the next sitting on this should argue about.
