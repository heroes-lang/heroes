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

## 2. Prediction (author, uncued — under the original protocol)

The author drew the control-flow graph for the program above before opening
`tools/spike/02-loop.c`. (The prediction files and their SHA-256 seals were
retired with the protocol inversion of 2026-08-03; the sealed record lives
in git history.)

## 3. What diverged — the lesson

The prediction and the spike disagreed on the loop's *shape*:

| Aspect | Predicted shape | Spike (naive lowering) |
|---|---|---|
| basic blocks | 5 — the rotated form (guard + test at the bottom) | **4** — bb0 init · bb1 test · bb2 body · bb3 print+return |
| where `i < 5` lives | end of the body block | **a block of its own** (bb1) |
| in-edges of the test block | 3 | **2** — one from bb0 (first arrival), one back-edge from bb2 |
| output | 10 | 10 — agreement |

The predicted shape is not nonsense — it is the *rotated* loop (do-while
form, test at the bottom) that optimising compilers produce, usually plus a
guard block for the may-run-zero-times case, which is exactly how one gets
to 5 blocks. Heroes' M4 lowering deliberately keeps the naive shape instead:
one test block entered twice — from above on first arrival, from below on
every iteration — because it is uniform to emit and easy to verify, and
clang -O2 rotates it by itself (see the spike header). Core lesson,
distilled into `docs/glossary/000-basic-block.md`: **the two in-edges of bb1
are the loop** — a loop is nothing but a cycle in the block graph, and the
block count follows from the shape, it is not the shape.

One divergence already found by the machine rather than by anyone's intuition:
the **entry block's label is never a jump target**, so clang warns
(`-Wunused-label`). The fix that keeps the emitter uniform: always enter
through an explicit `goto bb0;`. Recorded in CLAUDE.md rule 7 and DESIGN-LOG.

## 4. What broke + diagnosis

Nothing broke in the code path. One process miss caught mid-flight: the first
spec-v0 commit message understated the token estimate (~1250 vs the computed
~1496); amended. Lesson: the budget is AT the edge, exactly as design.md §1.6
says ("the budget is nearly spent") — every future addition needs a removal.

## 5. What spike 04 decides — walkthrough

*(Closed 2026-08-04 as an assistant walkthrough, not an author
explain-it-back: the author chose to spend the session on the M1 concepts
instead. Labelled honestly so the record does not overstate what happened.)*

Spike 04 fixes **two decisions that every later milestone depends on**, and
it fixes them in hand-written C so they are decided by something that
compiles and runs rather than by an argument:

1. **The container representation.** `[T]` is a pointer to a heap header
   (refcount, len, cap, element descriptor) followed by the elements
   in-line. The array is Heroes' *only* indirection (§4.10) — which is
   precisely what makes a recursive type like `Expr = variant { num(v: int),
   sum(children: [Expr]) }` finite: the recursion passes through the one
   pointer the language has.
2. **The descriptor ABI.** C has no copy constructors, no destructors and no
   generic equality, while §4.3 demands structural `==` on everything and
   §4.10 demands value-semantics copies and drops. So for every reachable
   type the compiler will *generate* ordinary C functions
   (`h_T_copy` / `h_T_drop` / `h_T_eq`) plus a descriptor struct pointing at
   them; the runtime works through descriptors and clang still type-checks
   every call. The alternative — a type-erased `void*` runtime — would have
   voided that property, which is the whole reason the backend emits C.

**Why before the compiler and not after:** these are not implementation
details the emitter can choose later, they are the *shape of the target*.
M5c will generate code that must fit them, the ownership pass (M5b) must
insert increfs against them, and the fixpoint at M8c compares generated C
byte for byte — so a representation discovered late would invalidate every
emitter test written before it. Deciding it by hand costs one afternoon;
discovering it at M5c costs the emitter. The spike also runs under
AddressSanitizer, so "the drops are complete" is a fact the machine
asserted, not a claim anyone made.

---

Verified today: `cargo test` green (golden harness, zero cases) · clippy green
· `heroes doctor` all ok · spikes 01→`20`, 02→`10`, 03→`1`, 04→`1 0 -42`
with ASan+UBSan reporting zero leaks.

Open at close of M0 — now tracked in `docs/debrief/QUEUE.md`: author's
explain-it-back (§5), harness baseline run (n=20, needs API key or manual
sessions). Panel decisions 002/003/005/006: taken 2026-08-03, applied to
design.md.
