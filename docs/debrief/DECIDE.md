# Decisions the compiler is waiting on

Read by **`/decide`**. Every item asks **what should be true**, and until it is
answered the compiler goes on behaving some way by default — which is the whole
difference from `LEARN.md`.

Two rules bind this file, both learned the hard way:

- **Verify before asking.** An entry is a claim from the day it was written, and
  entries outlive their causes: eight were found already settled in the first two
  sessions. Asking the author about a settled question is the one cost this list
  cannot pay.
- **Rank by what it blocks**, never by age, and say the blocker in the question.

Format: `- [ ] <origin> | <what> | <where to look> | <why it matters>`

**Split from `docs/debrief/QUEUE.md` on 2026-08-12, by author instruction.** One
file held 192 open items of three different kinds, and a list you have to filter
before you can read it is a list nobody opens. The closed items — the record —
stayed in `QUEUE.md`, because every commit subject in this repository cites that
path and CLAUDE.md §14 does not rewrite a record.

An answer is applied in the same session it is given — spec, design.md or
CLAUDE.md amendment, DESIGN-LOG line, the item ticked with the verdict written
into it. A decision recorded and not applied is the same open question with more
paperwork.

**The order is the ranking**: the headings are what each group blocks, and inside
a group the first item is the one the next milestone reads first.

**Sixteen items were answered on 2026-08-12 in one `/decide` session** and are in
`QUEUE.md` with their verdicts written into them. Before a single question was
put, nine items left for `LEARN.md` (seven milestone-close offers and two
questions the code had already answered) and one was ticked because the
diagnostic it asked for already existed and fired. Of the sixteen, four changed
the compiler, one changed the spec, four were ratifications, and one — the
`emit/decls.rs` split — is **back here**, because the ratified cut was made and
the file is still over the limit.

## Open — blocks M-selfhost-probe and the port

- [ ] panel 038 / §11, reopened 2026-08-12 | **`emit/decls.rs` is 493 lines after the two ratified cuts, and §11 says ~300.** The author's decision was taken and the named cut made — `emit/externs.rs` (237: the four group walks and the assertion emitter) and `emit/main.rs` (87: the two shims) — with the emitted C byte-identical and every golden green. What is left is **still two concerns, not one**: the unit's opening (`prelude`, plus the string table's `live_strings`/`escape_c`, ~110 lines) and the per-function skeleton (`prototype`, `signature`, `param`, `definition`, `prologue`, `reachable`, ~330). The obvious next cut is `emit/skeleton.rs` for the second. **Not done here because it was not the cut that was ratified** — the decision named two files and this is a third, and a mechanical change that grows past its instruction is how the next one stops being reviewable | crates/heroes/src/emit/decls.rs | the residual is bigger than the §11 limit on its own, so leaving it unrecorded would make the next reader think the breach was closed

## Open

- [ ] panel 039 / process | **Should a Part 6 row be falsifiable by the compiler?** The spec-warden's proposal, queued rather than adopted because the panel was not convened on it and amending CLAUDE.md or Part 0 is author instruction. The gap is real: Principle 0 (§1.0) is a burden on what **enters**, so a *permanent rejection* is the one design act in this project subject to no stated burden of proof — the cheapest possible way to make an expensive commitment. The proposed rule: a Part 6 row must name the program or the compiler fact that would make it wrong, turning the preamble's two asserted properties into a test. It has teeth already — under it, Part 6's preamble is false of **four** of its own rows (`Subtyping`, `Higher-kinded types, dependent types`, `Coroutines`, `Metatables`), all cost-only rejections sitting under *"Each violates locality"* | docs/panel/039 § The resolution item 6 · design.md:2126 | the historian's sourced warning is that CLAUDE.md §14 forbids rewriting a dated record, which makes Part 6 the PEP-3103 case (still reading `Rejected` about a language that shipped the feature) rather than the Go-FAQ case

## Closed by panel 039


- [x] reasoning 003 — ANSWERED, see `docs/panel/039-comptime-and-part-6.md` and QUEUE.md. Was | **Does Part 6's `Macros` row cover compile-time *evaluation*?** The row's reason — *"the code you read is not the code that runs"* — reads as covering it, and Part 6's preamble gives that reading force, but `comptime`/`const-eval`/`constexpr`/`CTFE` appear in the whole record exactly **once**, at design.md:1821, and there as a remark about Zig getting an answer right by accident (re-verified 2026-08-12: still exactly one occurrence; the row is design.md:2124). A permanent-rejection list that names one spelling of a mechanism and is silent on the other is how a refusal gets rediscovered as an idea. Part 6 is design.md Parts 1–11, so amending it is a **panel** path (CLAUDE.md §4) — this item decides only whether that panel is convened and when. **It must be numbered 039**: `docs/panel/` now ends at 038, and the skill's "next NNN" is right for once, but the number is worth stating because the last sitting was numbered from a worktree | design.md:2114-2132 · design.md:1821 · docs/reasoning/003-comptime-and-macros.md § What stayed open | the cheapest possible outcome is one Part 6 row at zero spec tokens, and the expensive one is re-litigating this every time a new language ships comptime


- [ ] author question 2026-08-12 / PANEL OWED | **Are the base types wrong — does Heroes need sized or unsigned integers (`u64`, `i64`, …)?** Asked by the author while reading panel 039's findings, and it is the largest open language question. **The evidence already exists and is compiled**: panel 039's ffi-pragmatist found two real header constants Heroes cannot express — `CURLAUTH_ANY` = `(~((unsigned long)1<<4))` and `SIZE_MAX` = `((size_t)-1)`, both `ffi_constant_type` — and classified them itself as *"a type-vocabulary gap (Part 7 item 10), not an evaluation gap"*. **Part 7 item 10 mis-locates the pressure**, which is the finding to open with: it defers sized integers as *"needed for any binary format"*, when the live case is the **FFI**, which is §1.11's founding constraint and is inside v1 — the same shape as the `Macros` row panel 039 repaired, a correct decision filed under a reason that does not describe the real case. Against: one signed `int` with abort-on-overflow removes the silent-wraparound class by construction, and conversion rules are the most expensive spec item that exists (item 10's own words); after the mangler fix the closure list needs no unsigned. Note that **the bitwise set does not close this** — `0xFFFFFFFFFFFFFFFF` is `int_out_of_range` whatever the operators are. Full panel: it changes the spec, the surface, a diagnostic class and the FFI's reach at once | design.md Part 7 item 10 · docs/panel/039 § the ffi-pragmatist's table · spec § Types | §1.11 makes the FFI the standard library, and a standard library that cannot say `size_t` has a hole in it
