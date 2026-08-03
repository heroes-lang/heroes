# 000 — Day zero: scaffolding, spec baseline, four spikes

Milestone M0 · 2026-08-03 · repo bootstrap

## 1. Goal + input example

Stand up everything the project needs before the first compiler code: git,
the Cargo workspace (library + one CLI), the Cyclone-rule tooling, the golden
harness (green with zero cases), the v0 spec as the pre-amendment measurement
baseline, the C runtime's first four functions, and four hand-written C spikes
that fix the emitter's target shape before the emitter exists.

Input example (spike 2 — the one with your prediction attached):

```
main = function: ()
    i: int @ 0
    total: int @ 0
    for i < 5
        total @ total + i
        i @ i + 1
    print(total)
```

## 2. Prediction (author — see 000-prediction.md, uncued)

→ `docs/journal/000-prediction.md`. Draw the control-flow graph for the
program above **before** opening `tools/spike/02-loop.c`.

## 3. What diverged — the lesson

(To be filled after the author's prediction is committed and compared.)

One divergence already found by the machine rather than by anyone's intuition:
the **entry block's label is never a jump target**, so clang warns
(`-Wunused-label`). The fix that keeps the emitter uniform: always enter
through an explicit `goto bb0;`. Recorded in CLAUDE.md rule 7 and DESIGN-LOG.

## 4. What broke + diagnosis

Nothing broke in the code path. One process miss caught mid-flight: the first
spec-v0 commit message understated the token estimate (~1250 vs the computed
~1496); amended. Lesson: the budget is AT the edge, exactly as design.md §1.6
says ("the budget is nearly spent") — every future addition needs a removal.

## 5. Explain-it-back (author, ≤10 lines, from memory)

(To be written by the author at the M0 close: what does spike 04 decide, and
why does it exist before any compiler code?)

---

Verified today: `cargo test` green (golden harness, zero cases) · clippy green
· `heroes doctor` all ok · spikes 01→`20`, 02→`10`, 03→`1`, 04→`1 0 -42`
with ASan+UBSan reporting zero leaks.

Open at close of M0: author's prediction (this file §2), author's
explain-it-back (§5), panel decisions 002/003/005/006, harness baseline run
(n=20, needs API key or manual sessions).
