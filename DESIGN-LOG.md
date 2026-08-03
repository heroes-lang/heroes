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

2026-08-03 | design.md is the LIVING document: every decided amendment is applied directly, history lives in git, open questions are inline OPEN QUESTION markers | author instruction | — | —
2026-08-03 | spec v0 stays FROZEN until the pre-amendment baseline run; then decided amendments produce spec v1 | preserves the only unrecoverable measurement | §1.6, Part 11 | 000
2026-08-03 | End goal declared: a mini-book on how the language came to be; story beats collected per step in docs/book/beats.md | journal/DESIGN-LOG/panel are the sources; beats keep the human texture | Part 0 | —
2026-08-03 | Showcase site stub in site/ for heroes-lang.org (domain owned); deployment deferred, ask before publishing | personality in the packaging, precision in the substrate | §"The name" | —
2026-08-03 | Part 0 teaching protocol switched to low-typing form: closed-question predictions (answers transcribed verbatim, still committed before src/), comprehension checks after each step, dictated explain-back, golden cases approved (with stated purpose) rather than authored | author instruction — typing cost was blocking the protocol; retrieval practice kept, written production dropped | Part 0 | —
2026-08-03 | Friction glossary: every comprehension gap produces a docs/glossary/NNN-<concept>.md entry (numbered in birth order = reading order) — English like all artifacts (rule 11, no exceptions), using the /where canonical analogies, origin cited, historicized, never deleted; absorbs docs/theory | author instruction — good explanations were evaporating in chat; first entry: 000-basic-block | Part 0 | —
2026-08-03 | Prediction records are private: raw answers live in git-ignored docs/journal/private/, the committed NNN-prediction.md carries the questions plus a SHA-256 seal of the raw record (precedence stays provable); journals record divergences as impersonal lessons — shapes and rules, never scores | author instruction — personal performance does not belong in a future-public history | Part 0 | —
2026-08-03 | Teaching gates attach to concepts, not steps: prediction + spot-checks fire when a new concept enters (typically once per milestone); plumbing steps chain autonomously; journal + story beat per milestone, DESIGN-LOG per decision | author feedback — the per-step ceremony felt slower than the learning it bought | Part 0 | —
