# 008 — How big is a compiler, measured

Date: 2026-08-14 · asked by the author, not by a milestone · **the first
measurement in this project taken against other people's source trees.**

The question was whether the Heroes bootstrap is large or small for a compiler.
Every number below was counted, not recalled — which matters here more than
usual, because panel 005 already had to demote one remembered compiler size
(CLAUDE.md §6's "Nim's compiler is ~150k lines") to an explicit assumption when
the historian could not verify it. That assumption is settled at the bottom of
this file.

## Provenance

| what | value |
|---|---|
| Heroes | `219e50d` |
| method | `wc -l` over physical lines, then an `awk` split into code / comment / blank |
| comment rule | a line whose first non-space characters open a comment (`//`, `/*`, `*`, `#`, `(*`) — an end-of-line comment counts as code, in every tree alike |
| clones | `git clone --depth 1`; the two giants via `--filter=blob:none --sparse` |

The comment rule undercounts trailing comments everywhere. That is the point: a
biased instrument applied identically to all eleven trees still ranks them
correctly, and an unbiased one was not available without a parser per language.

| tree | commit | date | what was counted |
|---|---|---|---|
| chibicc | `90d1f7f` | 2020-12-07 | root `*.c` `*.h`, `test/` excluded |
| harec | `b982097` | 2026-07-22 | `src/*.c` |
| tinycc | `2ba12e8` | 2026-08-09 | root `*.c` `*.h` — `lib/` (its libc), `tests/`, `win32/` excluded |
| lua | `7579fc9` | 2026-07-23 | `llex` `lparser` `lcode` only — the front end, not the VM or the stdlib |
| Nim | `cbb3b06` | 2026-08-14 | `compiler/**.nim` — `lib/` (the stdlib) counted separately |
| Odin | `e575197` | 2026-08-13 | `src/**.cpp` `**.h` |
| OCaml | `7c6c082` | 2026-08-14 | `parsing/` `typing/` `lambda/` `bytecomp/` `asmcomp/` `middle_end/` `driver/` |
| Zig | `738d2be9` | 2025-11-26 | `src/**.zig` — the self-hosted compiler, not `lib/` |
| Go | `195964e` | 2026-08-13 | `src/cmd/compile/**.go` minus `*_test.go` **and minus every file whose first line is `// Code generated`** |
| rustc | `a9066b3` | 2026-08-14 | `compiler/**.rs` minus `tests/`, all 78 crates |

## The table

Sorted by non-test lines. "Code" is non-blank, non-comment.

| compiler | written in | non-test lines | code | comment ratio |
|---|---|---:|---:|---:|
| lua (front end only) | C | 5,737 | 3,518 | 0.29 |
| chibicc | C | 8,916 | 6,605 | 0.15 |
| harec (Hare) | C | 20,446 | 18,188 | 0.04 |
| **Heroes bootstrap** | **Rust** | **~30,100** | **20,174** | **0.51** |
| tinycc | C | 68,619 | 51,478 | 0.22 |
| Nim | Nim | 113,168 | 89,250 | 0.16 |
| Odin | C++ | 142,998 | 108,176 | 0.14 |
| OCaml | OCaml | 163,396 | 133,051 | 0.11 |
| Go `cmd/compile` | Go | 190,139 | 133,688 | 0.27 |
| Zig `src/` | Zig | 517,489 | 470,340 | 0.04 |
| rustc `compiler/` | Rust | 861,366 | — | 0.23 (sampled) |

Two of these numbers hide something and the hiding is worth naming.

**Go's real total is 630,247 non-test lines; 440,108 of them are generated** —
mostly `rewrite*.go`, the SSA rewrite rules, and `opGen.go`. The 190,139 above is
what a human wrote. Anyone quoting "the Go compiler is 600k lines" is quoting a
code generator's output, and the same trap is set in Zig's `src/` (its per-target
backend tables) though it was not scorporated here.

**rustc's 861k is 78 crates, and no single one of them is a compiler.** The core
is smaller than the total suggests: `rustc_middle` 65,479 · `rustc_trait_selection`
50,244 · `rustc_hir_analysis` 39,602 · `rustc_borrowck` 35,388 · `rustc_parse`
32,529 · `rustc_codegen_ssa` 30,320 · `rustc_ast_lowering` 13,822. Heroes' whole
`types/` (7,887) sits against `rustc_hir_analysis` + `rustc_trait_selection`, and
the ratio there — about 11× — is roughly the ratio between the two languages'
type systems.

## What it says about Heroes

Heroes is **fourth from the bottom**, above harec and below tinycc. That is the
right neighbourhood for a compiler whose language does not self-host yet.

The uncomfortable comparison is tinycc: 68k lines for **all of C**, emitting
**x86-64, ARM and RISC-V machine code directly**, with its own linker. Heroes
spends 30k on a subset that delegates every backend question to clang. Per unit
of language covered, this compiler is expensive.

Three things are being bought with the difference, and the table can see two of
them:

1. **The comment ratio is 0.51 — nearly double the next highest (Go, 0.27) and
   more than three times the median.** Every other tree here was written for
   people who already know compilers. CLAUDE.md §11 is not a style preference in
   this light; it is the single most anomalous property of this codebase against
   its peers.
2. **`types/` is the largest directory (7,887 lines), ahead of `emit/`.** In
   chibicc the parser dominates; in tinycc the backends do. A compiler whose
   type checker outweighs its code generator is one that decided errors matter
   more than output, which is design.md §4.17 showing up as a line count.
3. Diagnostics-with-fixes (§8) and 163 golden cases have no column here, because
   no other tree in the table carries the equivalent to compare against.

**The number this project should watch is not in the table.** design.md sizes the
Heroes-in-Heroes port at 5–8k lines against 18,700 lines of Rust logic — a factor
of about 3. Where the port actually lands is the first hard evidence about
Principle 0: inside the bracket, the difference was the bootstrap's tax (§5's
Cyclone rule — owned data, indices for links, no stored closures — plus what Rust
must model by hand that Heroes has natively). At 15k, the complexity was never in
the host language, and the closure list is carrying weight it cannot justify.
That measurement is due at M-selfhost-fixpoint and nowhere earlier.

## Settled: CLAUDE.md §6's Nim assumption

Panel 005 recorded that "Nim's compiler is ~150k lines" could not be verified and
demoted it to an explicit assumption. Measured at `cbb3b06`:

- `compiler/**.nim` — **113,168 lines**, 89,250 of them code, in 175 files.
- `lib/**.nim` (the standard library, a separate thing) — **133,427 lines**.

The remembered figure was high by about a third for the compiler, and close to
right for the *stdlib* — which suggests the original number was the two conflated,
or the stdlib alone. Nothing in CLAUDE.md §6 depends on the magnitude (the rule is
"copy the surface, never the implementation"), so this closes the assumption
without changing the rule. **Amending §6's parenthetical is the author's call.**

One incidental confirmation: panel 032 cited Nim as running "171 files in one
directory". Today `ls compiler/*.nim` returns **169**. The precedent that a flat
layout survives at that scale holds, six years on and still within two files.
