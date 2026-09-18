# Panel 163 — spec-warden

Read `00-shared.md` first, then `spec/heroes-spec.md` § 5, § 9 and § 13, and
`.claude/rules/spec-shape.md`. You have a veto on budget breach and you carry
Principle 0's burden.

## The budget, measured this session

`./heroes measure spec/heroes-spec.md`: **6032** vendored, **8030** real
(`claude-opus-5`), ceiling **10240**. Panel 162's own row landed at +46 real
hours ago and its ledger row is `docs/measurements/010-spec-budget-ledger.md`'s
newest.

`--refresh` refuses every path but `spec/heroes-spec.md` and `CLAUDE.md`;
`ANTHROPIC_API_KEY` is in the repository's `.env` (`. ./.env`). Price in a
**scratchpad copy**, never in the repository. A vendored delta is not a price.

## What you are asked

**Which rule is being spent, and is it worth it?** Three of the four routes bend
a rule this document states plainly:

- Route 1 (a call typed by context) bends nothing in the document but adds a
  form. **Price it, and note that panel 162 already spent +46 on this milestone.**
- Route 2 (a zero default) contradicts design.md §4.9's *no default values*
  directly. An amendment to a Part 4 rule is not a § 13 sentence and you should
  say what it costs in design.md as well as in the spec.
- Route 3 (an uninitialised out-parameter) contradicts `spec § 5`'s **All
  bindings are initialised.** That sentence is four words and it is load-bearing
  — say what it buys and what removing an exception to it costs.
- Route 4 (refuse) costs a design.md Part 6 row and no spec text.

## The three questions that decide your verdict

1. **Where would each route's rule live?** One home, the section of the
   operation it governs. A rule about building a value is § 9 or § 13; a rule
   about initialisation is § 5. Say which, and say whether any route would put
   one rule in two homes — panel 162's did, and the critic caught it.

2. **Measure the cheapest honest wording for each**, real, in a copy.

3. **Is there a named removal?** Panel 162 declined one and paid with a
   registered prediction instead, on a premise its critic falsified. If a
   removal exists here, it is the better payment.

## The thing to insist on

**Refusing is a live verdict.** Zero programs in `examples/` bind a struct with
a long array field; the compiler does not need this; and the one motivating
struct needs a per-platform source file anyway because its arrays are `char[65]`
on one platform and `char[256]` on another. If Principle 0 says this has not
earned its way in, say so AS YOUR VERDICT and hold the refusal to Part 6's
standard.

## Deliver

Verdict · the section it rests on · the real count of every draft you priced, or
*lower bound* in those words · a falsifiable prediction · any budget veto.
