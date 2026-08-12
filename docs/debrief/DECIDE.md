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

**The order is the ranking** (2026-08-12): the three headings below are what each
group blocks, and inside a group the first item is the one the next milestone
reads first. Nine items left this file the same day for `LEARN.md` — seven
milestone-close offers and two questions the code had already answered — and one
was ticked into the record because the diagnostic it asked for exists and fires.

## Open — blocks M-program-corpus, the next milestone


## Open — ratifications and confirmations owed


## Open — blocks M-selfhost-probe and the port


## Open — language and process, blocking nothing dated

- [ ] reasoning 003 | **Does Part 6's `Macros` row cover compile-time *evaluation*?** The row's reason — *"the code you read is not the code that runs"* — reads as covering it, and Part 6's preamble gives that reading force, but `comptime`/`const-eval`/`constexpr`/`CTFE` appear in the whole record exactly **once**, at design.md:1821, and there as a remark about Zig getting an answer right by accident (re-verified 2026-08-12: still exactly one occurrence; the row is design.md:2124). A permanent-rejection list that names one spelling of a mechanism and is silent on the other is how a refusal gets rediscovered as an idea. Part 6 is design.md Parts 1–11, so amending it is a **panel** path (CLAUDE.md §4) — this item decides only whether that panel is convened and when. **It must be numbered 039**: `docs/panel/` now ends at 038, and the skill's "next NNN" is right for once, but the number is worth stating because the last sitting was numbered from a worktree | design.md:2114-2132 · design.md:1821 · docs/reasoning/003-comptime-and-macros.md § What stayed open | the cheapest possible outcome is one Part 6 row at zero spec tokens, and the expensive one is re-litigating this every time a new language ships comptime
- [x] reasoning 003 | **ANSWERED 2026-08-12 — adopted, and the branch is merged and deleted** (author instruction, given as a branch cleanup: recover the notes, then remove every branch but `main`). The merge landed as measured: `CLAUDE.md` and `QUEUE.md` clean, three append-at-EOF conflicts (`DESIGN-LOG.md`, `docs/book/beats.md`, and `OPEN-QUESTIONS.md` — one more than `merge-tree` predicted, because reasoning 003's own watch-list entries had since been appended at the same point), all three resolved chronologically with the 2026-08-10 lines in date order rather than at EOF. **The hand-off commit's seven items were dropped, every one verified as already done on `main` independently** — and done better: K's superlative is now sourced to kdb+'s STAC-M3 Antuco/Kanaga results (design.md:2555), R/MATLAB carries precedent *and* failure mode (2559), the missing memory-model rejection table exists with ARC/ORC, Zig allocators and C++ RAII (1315–1320), the non-atomic constraint is stated with its thread condition (1325), arena-plus-indices is named for Heroes (1330), Part 7.13 got its shape (DESIGN-LOG 2026-08-10), and both future-proofing rules landed (design.md:1937, 1947, 2246–2248). Appending them as open would have broken this file's own verify-before-asking rule. Repairs applied: the README's routing table now names `LEARN`/`DECIDE`/`SCHEDULED`, and rather than rewrite three dated records it gained a § A note on the vocabulary of 000–002 mapping `/debrief` and note 002's retired numbered milestone id forward. Was: This directory now holds exactly one file, numbered `003` because its README and notes `000`–`002` live on `origin/claude/project-reasoning-wdcls1` — 5 commits, +584 lines, **now 132 commits behind `main`** (was 119 when this was written). Measured with `git merge-tree`: `CLAUDE.md`, `QUEUE.md` and `OPEN-QUESTIONS.md` merge clean; `DESIGN-LOG.md` and `docs/book/beats.md` conflict as append-at-EOF (both mechanical — the branch's line is dated 2026-08-10 and goes before main's block). Repairs owed if it lands: the README and note 000 cite `/debrief`, split into `/learn` and `/decide` on 2026-08-12, and note 002 keys its obligations to a numbered milestone id that § The names has since retired. The alternative is to keep this one note and let the class stay implicit. CLAUDE.md §4 makes this **author instruction, not a panel** — the teaching process is amended by instruction | origin/claude/project-reasoning-wdcls1 · CLAUDE.md §4, §14 | a note written into a directory that does not exist adopts an artifact class by writing rather than by deciding, and this one did
- [ ] panel 038 / §11, reopened 2026-08-12 | **`emit/decls.rs` is 493 lines after the two ratified cuts, and §11 says ~300.** The author's decision was taken and the named cut made — `emit/externs.rs` (237: the four group walks and the assertion emitter) and `emit/main.rs` (87: the two shims) — with the emitted C byte-identical and every golden green. What is left is **still two concerns, not one**: the unit's opening (`prelude`, plus the string table's `live_strings`/`escape_c`, ~110 lines) and the per-function skeleton (`prototype`, `signature`, `param`, `definition`, `prologue`, `reachable`, ~330). The obvious next cut is `emit/skeleton.rs` for the second. **Not done here because it was not the cut that was ratified** — the decision named two files and this is a third, and a mechanical change that grows past its instruction is how the next one stops being reviewable | crates/heroes/src/emit/decls.rs | the residual is bigger than the §11 limit on its own, so leaving it unrecorded would make the next reader think the breach was closed
