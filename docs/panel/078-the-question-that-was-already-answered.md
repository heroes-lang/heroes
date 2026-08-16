# Panel 078 — the question that was already answered

**Convened** 2026-08-16, M-selfhost-port, on `docs/debrief/DECIDE.md:352`.
**Two seats reported before the sitting was stopped by the author** —
llm-ergonomist and spec-warden. The compiler-engineer, ffi-pragmatist and
historian were cancelled mid-flight, and this file records that rather than
presenting a full lane's authority for a two-seat result.

**It did not need the other three.** The sitting's premise was false, and the
seat that reads the record found out why.

## What was asked

Should a `nan` be stopped where it is **born** — `0.0 / 0.0` — rather than only
where it is compared, which is what panel 075 landed earlier the same day?

## The finding, and it ends the sitting

**The word `integer` at `spec:149` is panel 035's own artifact.** Commit
`ec5558b`, 2026-08-12, three seats, and the diff is exactly one word:

```
-  … integer overflow aborts; division by zero aborts.
+  … integer overflow aborts; integer division by zero aborts.
```

`DESIGN-LOG.md:207` records it. So the specification is **not silent by
oversight; it is silent by decision**, and panel 035's historian had already
found the precedent running the opposite way to this brief's:
*"Zig shipped that abort and moved off it; Python traps and NumPy overrode it."*

**The coordinator's brief asserted the opposite** — *"an inconsistency the spec
never states"* — and it took that framing from `DECIDE.md:352` without checking
whether the silence was a ruling. CLAUDE.md §1's third obligation says to read
forward from a record; the coordinator read forward in the **spec** and not in
the **decision**.

## Two more of the brief's claims, measured false by the same seat

- **"In pure Heroes the routes are division."** False four ways: a 400-digit
  literal reaches `inf`; `to_f32` of an over-large `f64` reaches it; **eleven
  squarings** reach it (reproduced by the coordinator); and `extern sqrt(x: -1.0)`
  reaches `nan` at exit 0. `nan` has five routes, `inf` four.
- **"Candidate 2 contradicts `spec:61` and owes it a repair."** False:
  `spec:55-62` is one bullet about **conversions**, and `inf` survives by three
  non-division routes anyway. The repair the brief demanded was priced at **+9
  tokens for nothing**.

## Verdict table

| judge | verdict | the finding that decides it |
|---|---|---|
| **spec-warden** | object 1 · **veto 2** · object 3 · **approve 4** | Found the ruling above. Then vetoed the option its **own indicator** made free: deleting the word `integer` measures **−1 token**, and it refused it anyway, on design.md §1.1's own warning that the token count is *"frequently a misleading one"* — the **program** cost moves the other way (**+25** in the callee, **+6** per call site, measured), and §1.1 lets tokens break ties only when comprehension is indifferent. Candidates priced at **+14 / +25 / −1 / +12 / +31**; the cheapest was the refuted one |
| **llm-ergonomist** (spec-only, blind; contamination disclosed) | approve 2 · object 1 · object 3 · object 4 | Preferred candidate 2 for a reason worth keeping even though it loses: it **deletes a qualifier instead of adding a clause**, and changes **zero characters** of the program a model writes. **Corrected the brief**: candidate 2 does not oblige `spec:61` to change. And found what the sitting was really about — *"the document already answers 'what does a degenerate input do' twice, in two sections, differently"*: `xs[0]` on an empty list **aborts** while `mean([])` is a **value**, and neither sentence mentions the other |

## The resolution — provisional, author ratification pending

1. **Nothing enters. Spec unchanged at 3506.** The document is true; the question
   was decided on 2026-08-12; this sitting adds a re-measurement of that ruling's
   counter-example and nothing else.
2. **`DECIDE.md:352` closes citing panel 035**, not by being re-argued.
3. **The falsifier is written down** (CLAUDE.md §12, a refusal is held to a
   feature's standard): a Heroes program containing **no `extern`** in which a
   `nan` reaches a site that is neither an ordering (panel 075), nor `sort`, nor a
   map key, and yields a wrong answer at exit 0. `tests/golden/run/` is where it
   would belong.
4. **What this sitting actually bought is a rule**, and it is now CLAUDE.md §1's
   fourth obligation: **a silence in a document and a silence in the record are
   opposite things**, and a sitting convened on the first must grep the second.
   Three other shapes landed with it, each from a different failure of the same
   day.

**What a veto at ratification would compel**: nothing was landed, so nothing is
reverted.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| spec-warden | implementing candidate 3 changes the exit status of **0** of the 78 run-goldens and **0** of the 14 examples while emitting a runtime guard in **≥60** of the 146 float-touching `.hero` files — zero measured benefit against a blast radius two orders above panel 075's measured zero guards. **Falsified by ≥1 golden or example changing status**, and one such case is a live defect that reopens this | M-ffi-ladder |
| llm-ergonomist | under candidate 2, ≥90 % textual agreement on the `mean` body and **100 %** of the programs lacking an empty guard move from *prints `nan`, exit 0* to *aborts* on `[]`; first-try rate delta ≈ 0 | M-program-corpus |

## Conditions on the record

- **spec-warden**: flips on candidate 3 only if a blind seat, given the spec
  alone, writes a program that assumes `1.0 / 0.0` aborts — which is metric 2,
  and `harness/tasks/` holds **one README and zero tasks**, so it cannot be
  measured today and **cannot be promised as payment** (panel 046 R1 as amended).
  Flips on candidate 2 only if the author overturns panel 035 explicitly, which
  is an author decision and not a panel's.
- **llm-ergonomist**: moves to candidate 4 if models under a candidate-2 spec
  begin writing defensive guards at >15 % above baseline — *"that would mean
  candidate 2 converted a loud abort into a silent wrong answer at the source
  level"*.

## What the missing three seats would have judged

Stated so the record does not pretend the lane was full: the compiler-engineer
would have priced the emitter change and the port's share of it; the
ffi-pragmatist held the question that may matter most — **whether stopping a
`nan` at the Heroes source reduces the surface at all, when 21 of 29 probed C
expressions return one** — and the historian owed the cost side of Erlang's and
Zig's rules, which it flagged at panel 075 as unpriced. **None of the three could
have changed the outcome**, because the outcome is that the question was settled
before it was asked.

## Author's verdict

*Pending.*
