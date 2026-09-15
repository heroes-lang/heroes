# Panel 154 — brief for the spec-warden

Read `00-shared.md` first. You judge the indicator (design.md §1.2, §1.6) and
Principle 0's burden of proof. You have a veto on a budget breach.

**The base, measured 2026-09-15 after defect 043 landed**: `spec/heroes-spec.md`
is **7974 real** on `claude-opus-5`, **5989 vendored**, digest
`21a9dc541cfa2fa8`, ceiling **10240**, **2266 free**. `DELTA_GATE` is 50 vendored
in one commit.

**Two removals are on the table and one of them is panel 153's critic's**, so
price both rather than re-deriving the enumeration that seat falsified: **R4**,
shortening § 13's *`acquires sqlite3_finalize`* to *`acquires`* — the section
declares no such function — which that critic measured at **−4 vendored and −9
real** and which deletes no rule. And **R3**, dropping *— sockets, maths, JSON,
databases —* from § 13's first sentence, **−15 real**, which panel 153's warden
measured and recommended against spending.

**Price each of routes A to E as spec text**, in your copy, writing each draft to
`spec/heroes-spec.md` and running `set -a && . ./.env && set +a && ./heroes
measure spec/heroes-spec.md --refresh` for the real figure and `./heroes measure
spec/heroes-spec.md` for the vendored one, restoring the base between drafts.
Route A and route D may cost zero spec tokens and you should say so plainly if
they do — a resolution that needs no sentence is the cheapest thing on this
table and §1.2 notices.

**Principle 0**: does the compiler need any of this? Count the handle parameters
in `selfhost/` yourself. And say what a route costs a READER under §1.2's
formula, not only what it costs the document.

One prediction with the command that scores it and the number it predicts.
