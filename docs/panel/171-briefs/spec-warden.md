# Panel 171 — brief for the spec-warden

Read `docs/panel/171-briefs/00-shared.md` first. The direction is the author's;
you price the sentence and the word, and you hold Principle 0's burden.

## Baseline, live

`./heroes measure spec/heroes-spec.md`, 2026-09-20: **real 8201**, vendored
**6159**, ceiling 10240, `DELTA_GATE` **50 in VENDORED tokens**. `.env` is
present; price by applying to the real path, `--refresh`, revert, **in a copy**.

## What § 13 says today, and what it must say

Today: *A lend lives for its call and no longer: C keeping the pointer reads
bytes the program may have changed or freed since, and nothing checks it.* —
landed at step 15, +47 real, on your own R0.

**Under the flip the last clause becomes false**: something now checks it. The
sentence must say the default and the word. Draft it, price it, and say what it
CHANGES rather than only what it adds — the *nothing checks it* clause is a
removal you can spend.

`CParam` gains the word in its own slot, beside `counted_by`. Panel 170 priced
the own-slot at +3 real over the alternation.

## The questions

1. **Price the sentence in two or three drafts**, merged and appended, and say
   which is cheaper. At panel 170 merging LOST by 24%; at panel 122 it won by
   36%. Say which this is.
2. **Price the word.** Every candidate is one token in the grammar and a few in
   the prose; the difference is in how many words of prose it takes to make
   the word's meaning unambiguous to a reader who has never seen it.
3. **Principle 0, from the tree, and it is different this time.** At panels 169
   and 170 you measured that `selfhost/` never lends a field and never leases.
   **It does lend a `cstr`, at twelve real call sites into nine functions.**
   Under the flip the compiler cannot compile itself without the word. So this
   enters on the **closure list**, not the thesis branch. Say so if you agree,
   and what that changes about the burden.
4. **What pays.** The *nothing checks it* removal, and registered predictions.
5. **Register a falsifiable prediction** with an instrument that exists today.

Copy named for your seat. Never rebuild from `selfhost/`. Report to
`docs/panel/171-reports/spec-warden.md`. Veto on budget breach.
