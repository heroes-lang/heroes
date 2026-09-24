# Panel 176 — spec-warden

Read `00-shared.md` first, then `llm-ergonomist.md`, whose variants V1, V2, V3,
R1 and A2 are the drafts you price. Your seat holds design.md §1.2 and §1.6,
Principle 0's burden, and a veto on a budget breach. Work in
`<scratchpad>/176-spec-warden/`.

## The counts, measured 2026-09-23 at `a747e5a2`

`./heroes measure spec/heroes-spec.md`: **6282** on the vendored maximum, a
lower bound. `./heroes measure spec/heroes-spec.md --refresh`: **8361** on
`claude-opus-5`, digest `0159e9b26bd998a2`. Ceiling **10240**, **1879 free**,
1819 net of the FFI floor. The last row of `docs/measurements/010-spec-budget-ledger.md`
is panel 175's, +91 real, over `DELTA_GATE` on the author's word to pay the
tokens, and it registered three predictions this sitting's llm-ergonomist scores
(task 6 of that seat's brief).

`--refresh` refuses every path but `spec/heroes-spec.md`, so price a draft by
applying it to that path in your copy and refreshing there; `. <trunk>/.env`
gives the key, and never print its value. If the instrument is unavailable, say
so and label any vendored number a lower bound.

## What to price and judge

1. **V1, V2, V3, R1 and A2**, each alone on the real instrument, and each with
   the grammar change it needs: V1 a new word in `CParam`'s mark alternation, V2
   `"acquires" ident { "|" ident }` in both productions (panel 175 priced that
   alone at +14 real, and at +29 with a consequence clause), V3 a `"into" ident`
   after `consumes`, R1 a new result mark.
2. **The combination the sitting is likeliest to adopt**, once the other seats
   report — price it last, merged rather than appended where a merge deletes a
   clause.
3. **Principle 0** for each: the compiler needs none of them (`selfhost/`
   declares no handle mark), so each rests on §1.12 or a measured Part 11
   effect, and say which. The author's word for panel 175 was to pay the tokens;
   say whether this sitting's text owes the same payment rule regardless, and
   what pays each row.
4. **Anything § 13 now says that one of these would make false**, since panel
   175 rewrote four of its sentences an hour ago.

Reply with your full report as your final message (you have no file-write tool):
a verdict per draft, its real delta, what pays it, one falsifiable prediction
with the milestone at which it is checkable, and your condition.
