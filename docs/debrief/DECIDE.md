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

## Open — waiting on a panel

- [ ] reasoning 003 | **Does Part 6's `Macros` row cover compile-time *evaluation*?** The row's reason — *"the code you read is not the code that runs"* — reads as covering it, and Part 6's preamble gives that reading force, but `comptime`/`const-eval`/`constexpr`/`CTFE` appear in the whole record exactly **once**, at design.md:1821, and there as a remark about Zig getting an answer right by accident (re-verified 2026-08-12: still exactly one occurrence; the row is design.md:2124). A permanent-rejection list that names one spelling of a mechanism and is silent on the other is how a refusal gets rediscovered as an idea. Part 6 is design.md Parts 1–11, so amending it is a **panel** path (CLAUDE.md §4) — this item decides only whether that panel is convened and when. **It must be numbered 039**: `docs/panel/` now ends at 038, and the skill's "next NNN" is right for once, but the number is worth stating because the last sitting was numbered from a worktree | design.md:2114-2132 · design.md:1821 · docs/reasoning/003-comptime-and-macros.md § What stayed open | the cheapest possible outcome is one Part 6 row at zero spec tokens, and the expensive one is re-litigating this every time a new language ships comptime

**Panel 039 was convened on 2026-08-12** by the author's decision in `/decide`
(option (a), *convene now*). This item stays open until the sitting's synthesis
lands as `docs/panel/039-*.md` with its provisional resolution; the verdict on
Part 6 is then the author's to ratify like any other.
