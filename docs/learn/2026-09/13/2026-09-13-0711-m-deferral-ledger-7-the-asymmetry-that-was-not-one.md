- [ ] **M-deferral-ledger 7** | design.md called it an asymmetry: you build a variant case one way and match it another. Write a `match` arm that destructures a record — `Point(x: a, y: b) =>` — and read what the compiler says. Then say why that one diagnostic makes the word *asymmetry* wrong, and what a symmetric variant pattern would have owed the language the day after it landed.

    **Where to look:** the compiled refusal in
    `docs/panel/142-there-is-no-asymmetry-to-restore-because-there-is-no-destructuring-anywhere.md`
    § The reframing; `spec/heroes-spec.md` § 8's `Pattern` production beside § 7's
    `Primary`; and design.md Part 7 item 16 with the verdict beneath it.

    **Why it matters:** five judges argued about the cost of making two spellings
    one. None of them stepped back one level to ask whether the two spellings were
    an exception or the rule. Build by label, read by dot is what records do too,
    and once you see that, the proposal is not removing an inconsistency — it is
    introducing one. The habit worth taking from this is cheap: before pricing a
    repair, check whether the thing being repaired is a special case or an instance.
