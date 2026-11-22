# Panel 256, spec-warden

Read `00-shared.md` first, whole, then `docs/panel/256-reports/critic-briefs.md` (its findings 1, 16, 20
to 25 and 29 bear on you). **Your agent file is the frozen commit's** (`.claude/agents/spec-warden.md` in
your copy), whose vetoes yield to the most robust resolution (`00-shared.md` § 1). Your copy is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p256-scratch/spec-warden/`, a detached worktree of
the frozen commit; build its compiler there (§ 2 of the shared brief). Your report:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p256/docs/panel/256-reports/spec-warden.md`,
written as you go.

## The counts

- The frozen spec, read by the coordinator's stage 2 (`heroes-next measure spec/heroes-spec.md` from the
  copy's root, 21:15): claude-legacy **9,585**, cl100k_base **9,761**, the vendored maximum 9,761, and
  **real 12,809**, `claude-opus-5`, the pin of 2026-11-22; headroom 3,575 against the 16,384 ceiling
  (design.md §1.6, by grep). sha256 `15804f3fb52a62c2...`.
- The arms' vendored maxima (`heroes-next measure <arm>`): `q1` 9,812 (+51), `gc` 9,812 (+51), `gi` 9,847
  (+86), `gr` 9,824 (+63). **Their real counts are yours**: `--refresh` reads only `spec/heroes-spec.md`, so
  apply an arm to that path in your copy, refresh, and restore it (`.claude/rules/spec-shape.md` § How a
  change to the document is made). **At most six refreshes**, each a count, never a `claude -p`
  session: the four arms, your own repaired text of the route you would adopt, and one spare *(critic 25)*; the key from the main checkout's `.env` (`set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env;
  set +a`), its length alone printed, never its value.

## What you judge

1. **Every arm's text** (`arms/spec-*.diff`): does each sentence say one thing, in its home (spec § 9 or
   § 11), with the words the document already uses? A sentence a reader can take two ways is a defect of
   the text, named and repaired in your own `spec-w*.md` beside your report. In particular: `gc`'s *"emptied
   whatever that type holds"* against § 5's rule on an `@` parameter nothing ends; `gi`'s *"refused at a call
   that gives it the other kind"*; `gr`'s collision with design.md §4.12's *"pass it as a parameter"*; `q1`'s
   fifth operation and the UFCS sentence.
2. **Principle 0 and the budget**, as costs under the rule of 2026-11-22 (`691531908`): no veto; the cost
   measured and written.
2b. **`gc`'s two readings** *(critic 21)*: a rule on the writing (`m.send(v)` refused, the row of program 1)
   or on the meaning (every send of such a value empties it, program 1 accepted). Say which the text says,
   and repair it. **And § 4.6**: is `gi` what § 11 already says *(critic 16)*?
3. **Where the generic rule belongs**: spec § 9 (generics) or § 11 (threads), and whether one sentence can
   say it for every rule on what a type holds (§ 4.2's `e2` is § 13's `partial`).

## Predictions registered before the blind run

**Your report must hold the heading `## Predictions registered before the blind run`**, with your
predictions under it, **before the blind run starts**: its runner refuses to start without them and copies
your report, hashed, as it starts (panel 251's critic's finding: that sitting's were written 13 seconds
after the run began). Read `256-briefs/llm-ergonomist.md` (the cells, the rows, D1 to D3) and the tasks in
`256-briefs/blind/`; **you may read the tasks and the arms, never a session's folder or its answers**.
**The run is built after the synthesis** (its arms the proposal's, with the texts the synthesis names,
hashed in `blind/registration.md` before any session), so predict on each arm's RULE as
`llm-ergonomist.md` states it; a text the synthesis repairs keeps the rule your prediction names, or the
synthesis says it does not. Predict, at least: `right(w)`, `right(m)`, `lost(w)` and `lost(m)` on t1; for t2, the right count on each
program under each of `now`, `gc`, `gi`, `gr`; for t3, `right` per arm; and whether D1, D2 and D3 hold.
Each a number and a range.

End with the verdict structure of your agent file, then one line per question: the route you would adopt,
and the one measurement that would change your mind.
