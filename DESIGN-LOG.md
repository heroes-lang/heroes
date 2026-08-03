# DESIGN-LOG — append-only, one line per decision

Format: `date | decision | reason | design.md § | panel`

2026-08-03 | Bootstrap compiler in Rust, Cyclone-rule subset, retired at fixpoint | author preference; §3.4 comfort list; port must be mechanical | §3.4 | 000
2026-08-03 | Backend: emit C11 via clang, replacing QBE | FFI is the founding constraint; clang verifies signatures against real headers; rationale is §1.11/§4.19, NOT optimisation | §1.11, §3.1, §4.19 | 001
2026-08-03 | Keep three-address IR with explicit basic blocks; emit C from it | Part 10 step 6 is the heart; C emitted with goto+labels keeps CFG visible | Part 10 | 001
2026-08-03 | QBE becomes a scheduled second backend (post-fixpoint), not "maybe" | one backend proves nothing about IR agnosticism; preserves the register lesson | Part 7 item 14 | 001
2026-08-03 | One executable `heroes` with subcommands; never a second binary | corollary of §3.3; the zig model | §3.3 | 004
2026-08-03 | Panel mandatory on design decisions; redesigned with differentiated inputs + falsifiable predictions | author decision; rev-1 design produced correlated verdicts | — | 000
2026-08-03 | Principle 0: self-hosting is necessary-not-sufficient for a v1 form | the strict version would delete the thesis features (???, same-typed args, rich errors) | §1.1 | 000, 005 pending
2026-08-03 | Spec v0 written at M0 as the pre-amendment measurement baseline | design.md L77 "build the harness early"; spec-warden needs a measurable number | §1.6, Part 11 | 000
2026-08-03 | Two new compiler passes: type descriptors (copy/drop/eq/hash) + ownership (refcount insertion in lowering) | C has no copy ctors/dtors; §4.3 structural ==; §4.8 copy-out on every exit edge | §4.3, §4.8, §4.10 | 000
2026-08-03 | Container representation + descriptor ABI fixed by spike 04 (header + inline elements, per-type descriptor struct) | decided by hand-written ASan-clean code before any compiler code | §4.10, §4.20 | 000
2026-08-03 | Fixpoint acceptance = byte-identical generated C (diff B.c C.c), not Mach-O binaries | binary identity depends on clang/ld noise (LC_UUID, DWARF paths) | Part 0 | 000
2026-08-03 | Emitter always enters via explicit `goto bb0` | spike 02 finding: entry label is never a jump target → unused-label warning | — | journal 000
2026-08-03 | Errata applied to design.md: Part 2 heading restored; `&`→`&&` in §4.3/§4.8 examples | document already legislates && at §4.14; these were internal inconsistencies | Part 2, §4.3, §4.8 | 000
