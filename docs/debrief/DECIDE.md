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

- [ ] panel 022, moved from `SCHEDULED.md` 2026-08-12 | **`// ORDER:` is a rule with zero instances, and the sentence that states it also states its own zero.** design.md:1233 reads *"Every ordering-sensitive walk is still marked `// ORDER:` at the write site, and there are **zero** such marks in the tree today"* — so the document is honest, but the marker is a convention nothing follows and nothing tests. Two facts found while verifying it (2026-08-12): the tree still has **zero** `// ORDER:` marks, and the § 4.9 port note that names **`types/holes.rs`** as the bootstrap's one map iteration is **obsolete** — `holes.rs` today walks `resolved.locals`, `fields` and `cases`, all vectors. The live site is elsewhere and it is user-visible: `resolve/mod.rs:198` `names_in` iterates the `BTreeMap top` (`mod.rs:167`) straight into the *did-you-mean* candidate list and `--dump-scopes`, via `resolve/exprs.rs:297-312`. Decide which of the three §11 shapes this takes: mark the sites now, or turn design.md's sentence into a falsifiable claim with a test that fires when it dies, or withdraw the marker | design.md:1225-1236 · crates/heroes/src/resolve/mod.rs:167,198 · crates/heroes/src/resolve/exprs.rs:297 | the note's own last line says it "becomes an explicit `sort` in the Heroes port, or the M-selfhost-fixpoint diff breaks", and the marker was the only thing that was going to say where

## Open

- [ ] found while verifying the unsigned item, 2026-08-12 | **Heroes has no hexadecimal literal, and the bitwise set just made that visible.** Measured, not assumed: `x = 0xFFFFFFFFFFFFFFFF` fails as `expected_end_of_line` on `xFFFFFFFFFFFFFFFF` (a *name*), the lexer contains no hex scanning, and the spec's 191 lines never write `0x`. Panel 040 landed `& | ^ ~ << >>` on the argument that a flag union spelled `+` is a silent wrong answer — and a mask must now be written in decimal, which is the notation every C header does **not** use, so a reader comparing `curl.h` to a `.hero` file converts by hand. This is a **separate and much cheaper** question than sized integers (a literal notation, no new type, no conversion rules — spec cost is one clause), and it is only worth asking now because the operators exist. Decide whether it rides in the unsigned panel's sitting, gets its own, or waits | crates/heroes/src/lexer/scan.rs · spec § Types · docs/panel/040 | a language whose bitwise operators cannot read the constants they exist for has the operators and not the use case


## Closed by panel 039


- [x] reasoning 003 — ANSWERED, see `docs/panel/039-comptime-and-part-6.md` and QUEUE.md. Was | **Does Part 6's `Macros` row cover compile-time *evaluation*?** The row's reason — *"the code you read is not the code that runs"* — reads as covering it, and Part 6's preamble gives that reading force, but `comptime`/`const-eval`/`constexpr`/`CTFE` appear in the whole record exactly **once**, at design.md:1821, and there as a remark about Zig getting an answer right by accident (re-verified 2026-08-12: still exactly one occurrence; the row is design.md:2124). A permanent-rejection list that names one spelling of a mechanism and is silent on the other is how a refusal gets rediscovered as an idea. Part 6 is design.md Parts 1–11, so amending it is a **panel** path (CLAUDE.md §4) — this item decides only whether that panel is convened and when. **It must be numbered 039**: `docs/panel/` now ends at 038, and the skill's "next NNN" is right for once, but the number is worth stating because the last sitting was numbered from a worktree | design.md:2114-2132 · design.md:1821 · docs/reasoning/003-comptime-and-macros.md § What stayed open | the cheapest possible outcome is one Part 6 row at zero spec tokens, and the expensive one is re-litigating this every time a new language ships comptime


- [ ] author question 2026-08-12 / PANEL OWED | **Are the base types wrong — does Heroes need sized or unsigned integers (`u64`, `i64`, …)?** Asked by the author while reading panel 039's findings, and it is the largest open language question. **The evidence already exists and is compiled**: an ffi-pragmatist found two real header constants Heroes cannot express — `CURLAUTH_ANY` = `(~((unsigned long)1<<4))` and `SIZE_MAX` = `((size_t)-1)` — and classified them itself as *"a type-vocabulary gap (Part 7 item 10), not an evaluation gap"*. **Two citations in this item were wrong and are corrected here** (verified 2026-08-12, third `/decide` session): the two constants are in **`docs/panel/040-the-bitwise-set.md` § finding 4 and prediction 3**, not in panel 039 — 039 contains neither name — and they are not `ffi_constant_type` diagnostics: that code exists and fires (`types/decls.rs:244`), but nothing has been written that makes these two produce it, so what the record holds is a judge's compiled reading, not a diagnostic transcript. **Part 7 item 10 mis-locates the pressure**, which is the finding to open with: it defers sized integers as *"needed for any binary format"*, when the live case is the **FFI**, which is §1.11's founding constraint and is inside v1 — the same shape as the `Macros` row panel 039 repaired, a correct decision filed under a reason that does not describe the real case. Against: one signed `int` with abort-on-overflow removes the silent-wraparound class by construction, and conversion rules are the most expensive spec item that exists (item 10's own words); after the mangler fix the closure list needs no unsigned. Note that **the bitwise set does not close this**, and the reason is not the one panel 040 wrote: `0xFFFFFFFFFFFFFFFF` is **not** `int_out_of_range`, because **Heroes has no hexadecimal literal at all** — the lexer scans `0`, then `xFFFFFFFFFFFFFFFF` as a name, and the program dies with `expected_end_of_line` at column 10 (measured 2026-08-12; zero occurrences of hex scanning in `lexer/`, zero mentions of `0x` in the spec). The decimal form is the one that reaches the type checker: `18446744073709551615` → `int_out_of_range`, with the note naming the signed range. So the unsigned gap and the **notation** gap are two gaps, and only the second is cheap | design.md Part 7 item 10 (line 2287) · docs/panel/040 § finding 4, prediction 3 · spec § Types | §1.11 makes the FFI the standard library, and a standard library that cannot say `size_t` has a hole in it
- [ ] §11, standing | **Twenty-two non-test files are over ~300 lines, and three of them were pushed there on 2026-08-12.** The two the author's decisions named are closed — `emit/decls.rs` 768 → 171 across `externs.rs`, `main.rs`, `signature.rs` and `body.rs`, and `resolve/cycles.rs` 343 → 241 across `constbody.rs` — with the emitted C byte-identical both times. **What this session added to the list, and it is owed rather than incidental**: `emit/ops.rs` 378 → 433 (the bitwise emission), `crates/heroes-cli/src/commands/compile.rs` ~300 → 317 (`level_from`), `ir/inst.rs` → 303. The largest standing breaches are `emit/perfn.rs` (628), `emit/aggregate.rs` (511) and `printer/fmt_stmt.rs` (457), none of which any decision has touched. Decide whether this is a **standing sweep** with its own commit, a per-milestone obligation in `/step`'s checklist, or a number §11 should restate — because "split a module before it passes ~300 lines" is currently a rule twenty-two files are in breach of, and a rule with twenty-two exceptions is a preference | CLAUDE.md §11 · `find crates -name '*.rs' | xargs wc -l | sort -rn` | the per-file ceiling is the only live one after the global number was refused (design.md §1.6), so it is now the whole of what watches the compiler's size
