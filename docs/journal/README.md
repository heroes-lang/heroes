# Journals — one per milestone, the record of what happened

One file per closed milestone, numbered in the order they closed. Each carries the
three sections `/step`'s close checklist asks for — **goal** · **what surprised**
(impersonal: shapes and rules, never scores) · **what broke and why** — and, since
2026-08-12, a fourth: **what landed, and what carried forward**, which is the
milestone's status paragraph and its chain entry, moved here from `docs/ROADMAP.md`.

The division of labour, and the reason this file exists:

- **`docs/ROADMAP.md` says what is next.** It had accumulated 512 lines about the
  past before its first line about the future, growing ~66 lines per close. The
  retrospectives live here now, and `/step`'s checklist keeps the ROADMAP's
  § Where we are at ≤15 lines.
- **These journals say what happened**, one milestone at a time, with the lesson
  next to the inventory.
- `DESIGN-LOG.md` says what was decided, `docs/panel/` why, `docs/measurements/`
  and `docs/defects/` with what numbers, and `docs/book/beats.md` what it felt like.

**The identifiers here are the ones each milestone was built under** — `M5c`, not
`M-value-aggregates`. A journal is a dated record and is never rewritten; the map
is `docs/ROADMAP.md` § The names, and the rule is CLAUDE.md §14.

| # | journal | milestone | closed | tag |
|---|---|---|---|---|
| 000 | [setup](000-setup.md) | M0 — day zero | 2026-08-03 | `m0` |
| 001 | [lexer](001-lexer.md) | M1 — the lexer | 2026-08-04 | `m1` |
| 002 | [parser](002-parser.md) | M2 — parser, tree, formatter | 2026-08-04 | `m2` |
| 003 | [resolver](003-resolver.md) | M3a — the resolver | 2026-08-04 | `m3a` |
| 004 | [checker](004-checker.md) | M3, M3b–M3d — the frontend complete | 2026-08-04 | `m3` |
| 005 | [lowering](005-lowering.md) | M4 — desugar and the IR | 2026-08-04 | `m4` |
| 006 | [scalars run](006-scalars-run.md) | M5, M5a — the first native binary | 2026-08-04 | `m5a` |
| 007 | [strings and ownership](007-strings-and-ownership.md) | M5b — `str`, and the ownership pass | 2026-08-05 | `m5b` |
| 008 | [aggregates](008-aggregates.md) | M5c — records and variants by value | 2026-08-10 | `m5c` |
| 009 | [the map and the fallible](009-the-map-and-the-fallible.md) | M5d — `T?` and `{K: V}` | 2026-08-10 | `m5d` |
| 010 | [sugar, tests, generics, library](010-sugar-tests-generics-library.md) | M6 — the language is finished | 2026-08-11 | `m6` |
| 011 | [modules, the namespace](011-modules-the-namespace.md) | M8a — a program is many files | 2026-08-12 | `m8a` |
| 012 | [the FFI ladder](012-the-ffi-ladder.md) | M-ffi-ladder — Heroes calls C | 2026-08-12 | `m-ffi-ladder` |
| 013 | [header constants](013-header-constants.md) | M-header-constants — the number leaves the file | 2026-08-12 | `m-header-constants` |
| 014 | [literal bases](014-literal-bases.md) | M-literal-bases — four ways to write a number, and one that stopped lying | 2026-08-12 | `m-literal-bases` |
| 015 | [sized integers](015-sized-integers.md) | M-sized-integers — eight widths, and `int` stops being a word | 2026-08-12 | `m-sized-integers` |
| 016 | [the program corpus](016-the-program-corpus.md) | M-program-corpus — nine programs, six compiler defects, and a CI | 2026-08-13 | `m-program-corpus` |
| 017 | [binding fidelity](017-binding-fidelity.md) | M-binding-fidelity — what an `extern` accepts | 2026-08-14 | `m-binding-fidelity` |
| 018 | [struct passing](018-struct-passing.md) | M-struct-passing — the layout that is not ours | 2026-08-15 | `m-struct-passing` |
| 019 | [complete structs](019-complete-structs.md) | M-complete-structs — every field form a C header can write | 2026-08-15 | `m-complete-structs` |
| 020 | [the selfhost probe](020-selfhost-probe.md) | M-selfhost-probe — the lexer ported, and the wall that was not there | 2026-08-15 | `m-selfhost-probe` |
| 021 | [the port](021-selfhost-port.md) | M-selfhost-port — the compiler twice over, and one hash | 2026-08-17 | `m-selfhost-port` |
| 022 | [the seed](022-selfhost-fixpoint.md) | M-selfhost-fixpoint — the seed, and four defects the fixpoint could not see | 2026-08-18 | `m-selfhost-fixpoint` |
| 023 | [the net](023-harness-port.md) | M-harness-port — the net, in Heroes, and the two defects it found on its first day | 2026-08-18 | `m-harness-port` |
| 024 | [the attic](024-bootstrap-archive.md) | M-bootstrap-archive — the third language dies, and five instruments get successors first | 2026-08-19 | `m-bootstrap-archive` |
| 025 | [the pieces](025-separate-compilation.md) | M-separate-compilation — one `.c` per module, a cache that cannot lie, and a frontend that was one function | 2026-08-26 | `m-separate-compilation` |

`git tag --list --sort=creatordate` gives the same order from git itself, and
`git checkout m5b` re-opens any milestone's code — which is what
`M-journey-book` is written from.
