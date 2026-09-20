# Panel 169 — brief for the spec-warden

Read `docs/panel/169-briefs/00-shared.md` first.

## Your axis

design.md §1.2's cost formula and §1.6's budget, and Principle 0's burden of
proof. An addition owes a named removal or a registered falsifiable prediction
naming an instrument that exists.

## The measured baseline, run while this brief was written

`./heroes measure spec/heroes-spec.md`, today:

| | |
|---|---|
| **real**, `claude-opus-5`, the binding number | **8154** |
| vendored maximum, a lower bound | 6126 |
| spread between the two vendored tables | 126 |
| ceiling | **10240** |
| headroom | **2086** |
| the FFI floor mortgages | 60, so what is measured against the ceiling is **8214** |

**`.env` is present in the tree**, so `--refresh` is available and a draft can be
priced on the instrument that judges it: apply to the real path, `--refresh`, and
revert if the sitting rejects it. `.claude/rules/spec-shape.md` is the order of
work and the rule that a vendored delta is not a price.

## Your own record in this milestone, and it is the reason this sitting exists

At panel 167 you registered: ***"routes A, B and C each close zero of two
reproductions when landed alone."*** Panel 168 measured it and **scored it
CORRECT**, on this repository's own shipped example with one line moved. Two
sittings and a defect entry said the opposite, and nothing read your prediction
until a critic went looking.

**So this sitting asks you for the same thing again**, and asks the coordinator
to score it rather than file it: what does each of the four routes close, and
what does it not?

## The four routes to price

R1 the handle route · R2 the caller-side rule for 068 · R3 a group-level mark ·
R4 withdrawing the field lend. All four are described in the shared brief.

Price each as a **spec § 13 diff**, in real tokens, and say for each:

1. what sentence of `spec § 13` it **changes**, not only what it adds. Today's
   text says a lease *"is a COPY of the bytes that C may read for as long as the
   program says"*. **That sentence is false as measured** — C may read past
   `end_lease` — so at least one route is a correction rather than an addition,
   and a correction may be free or may cost less than the sitting assumes;
2. whether **merging beats appending** here (panel 122: three drafts measured,
   the merged one cheapest);
3. the named removal or the registered prediction that pays for it.

## The questions

1. Which route is cheapest **per defect closed**, which is a different ranking
   from cheapest?
2. R4 removes text rather than adding it. Panel 167 recorded it at **−80**.
   Re-price it, and say what a removal that closes a corruption class is worth
   against an addition that closes the same one.
3. **Is there a route nobody listed?** Ask it of the SPEC: is there a sentence
   already in § 13 that, read strictly, already forbids one of these programs and
   is simply not enforced? That would make a route a compiler defect rather than
   a language change, which is the cheapest outcome available and nobody has
   looked for it.
4. **Register a falsifiable prediction** with an instrument that exists today,
   and make it the kind that gets read: yours was right and invisible.

## Working rules

- Build in a copy if you build at all. A compiler in 3.80 s:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
- Every number from a command run in this session, named. If `--refresh` is
  unavailable, say the number is a **lower bound, in those words**.
- Report to `docs/panel/169-reports/spec-warden.md`. Veto on budget breach.
