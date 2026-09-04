# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item here asks **what should be true**, and until it
is answered the compiler goes on behaving some way by default. That default is
the cost of leaving an item open, so each item names it.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/work/DONE.md`. A file that keeps its
own closed items stops being a list of what is owed — this one held **138 ticked
items and zero open ones** on 2026-08-26, under eighteen headings still titled
`## Open`, which is why the rule now lives in `/decide` itself instead of in
prose about `/decide`.

**One notation: `- [ ]`.** No section headings, no prose sections, no
strikethrough. A finding written as a bare bullet is invisible to every count in
this project, and nine of them sat in exactly that shape under two panel
headings here until 2026-08-26 — **five already closed** and two of those
measurably stale, while the file reported itself empty.

Two rules bind this file, both learned the hard way:

- **Verify before asking.** An entry is a claim from the day it was written, and
  entries outlive their causes. Asking the author about a settled question is the
  one cost this list cannot pay. Of the nine findings recovered on 2026-08-26,
  five were closed and were ticked with what closed them rather than asked.
- **Rank by what it blocks**, never by age, and say the blocker in the question.

Format: `- [ ] <origin> | <what> | <where to look> | <why it matters>`

- [ ] **panel 109 — ratify the split resolution: `cstr owned <function>` is ADOPTED with eight conditions, `ptr owned <function>` is REFUSED under a standing veto with return conditions, the third case is queued** | `docs/panel/109-the-string-c-hands-you-and-the-handle-it-keeps.md`, full panel 2026-09-04, one veto (compiler-engineer, on the `ptr` half as a new core type), four approvals with conditions on the `cstr` half | **What your yes settles**: (1) an `extern` function may mark a `cstr` result, or a `@` parameter the header spells `char **`, `owned <C function>`; the compiler frees it by name, hands it over as a `str?`, the `@` cell is out-only, and **your own call of the freer on it is a compile error** — four seats wrote that sentence independently; (2) the wording Wt2 (+70 tokens, measured) paid by the `spec:225-227` platform-commentary removal (−28): **+42 net** against the ledger's +31 mean, the eleven being the `@` cell and the `char **` clause two seats made conditions; plus `spec:239` gains *"for that call"* (+3); (3) `ir/lower.hero` (1511/1511) and `parse/decl.hero` (638/638) are split BEFORE the feature, `DECIDED` rows lowered; (4) the `ptr` half returns only with a compiled prototype giving shared release without a new `Ty` case in ≤ 120 lines and no ABI move, plus a corpus program that cannot release its handle explicitly — the warden's live witness (`examples/ledger/main.hero`'s five `?` between `prepared` and `finalized`, `sqlite3_close` → 5 at exit 0, measured) is the program to point that at. **Recommended: ratify.** The fact under the file is measured three ways: a caller-owned C string can be freed or read today, never both (`@error: cstr` is `ffi_writable_parameter`, `@error: ptr` then `validated` is `type_mismatch`), so the leak that convened panel 108 was a program the language cannot express; the lowering that fixes it was hand-emitted and run leak-free in twelve lines of C. **What a no would mean**: `design.md:2124`'s shim route stays the only answer, and every C string a library hands the program is either leaked or unread | docs/panel/109-the-string-c-hands-you-and-the-handle-it-keeps.md · design.md:2120-2125 · spec/heroes-spec.md:213-215, 225-227, 239 · docs/work/SCHEDULED.md (the M-ffi-ladder items this queues) | until ratified the mark is provisional and the splits it requires have no author's word behind them
