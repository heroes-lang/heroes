# 057 — M-check-completeness

*what `heroes check` accepts, `heroes build` compiles — through a generic too*

## Goal

Close the gap the milestone is named for: four measured faces where `heroes
check` said yes and something later said no. Three sittings, 155, 156 and 157,
and four defects — 046, 047, 048 opened here, and 047 opened by the author
asking why CI was red.

What the milestone actually delivered is not what its name promised, and the
journal says so first because every other section reads differently once it is
said. **Only one of the four faces was ever a `check`/`build` gap**, and it had
been closed for a month by a sitting held the same day as the one that named it.
For the two the work went into, `check` and `build` both return 0 — they agree —
and the divergence is at *run*. The fourth belongs to another milestone.

## What surprised

**A closed rule with no golden behind it is a rule waiting to reopen.** The
sortable face was closed on 2026-08-16 by panel 084 and its only witness was a
unit test asserting `is_refusable` on a `.generic` type id — a fact about a
table entry, where the rule is about a program. A compiler that stopped refusing
it would have passed every suite.

**The same shape came back twice more the same day, and one of the two was
written by the session that had just criticised it.** A test asserted `called
from node_value` and was green on one platform; `node_value` is the FALLBACK
`hero_stack_blame` returns when the frame walk finds nothing, so the suite had
been pinning a failure as the required answer. And the blame name turned out not
to be a platform fact at all: the same program on the same machine says
`node_value` at `-O0` and `main` at `-O2`, because the Heroes frame is inlined
away.

**Two walks abandoned past a nesting bound and the argument defending them was
true.** `if depth > 16` terminates by giving up, so a record chain 16 deep was
`check` 0 where 15 deep was `check` 1 — with no generic anywhere. The comment
said absence was safe because the runtime guard sits behind the rule, and the
guard does sit there and does fire. What it missed is that **a guard licensing a
compile-time silence is a guard the compiler leans on to be wrong quietly.**

**A brief assembled from documents rather than from the tree misleads five
judges at once.** Six numbers across three sittings were carried from files
instead of measured: a ceiling that had moved, a count that was a third of the
truth, a seam that was inside a test block, a *provably* that traced to an
archived tree, an `.expected` file that does not exist, and a platform marked
unrun hours after the machine was powered. Seats caught every one, and one of
them ran against its own argument and said so.

**And the language's own answer was already written down.** Panel 155 spent five
seats pricing a call-site pass at 284 lines against an estimate of 90-110, and
the completeness critic found `design.md:1721`: *"No constraints. No `where`, no
bounds. If an operation on `T` is needed, pass it as a parameter."* Measured, the
body rule that sentence implies deletes **0 of the 50** generic functions in this
tree.

## What broke and why

**`setvbuf(stdout, NULL, _IOLBF, 0)` kills a process on Windows.** Correct on
POSIX, green on two machines of three; Microsoft's CRT hands a size of 0 to its
invalid-parameter handler, which terminates at once and silently. The compiler
built from that file did not survive `heroes doctor`. Three probes, one per call
so no abort could hide another. And passing a size repairs nothing there, because
that CRT implements `_IOLBF` as full buffering — `_IONBF` is the only mode on
that platform that keeps the promise.

**A step landed without regenerating the seed.** `seed/README.md` states it in
one line — if the diff touches `selfhost/`, the seed is regenerated in the same
commit — and step 3 did not. CI builds the compiler from the seed, so the rule
landed in `selfhost/` did not exist for the suite that judges it, and a second CI
step compares the seed to what today's source emits. Both would have been red.
Found before the push, by running the net against a seed-built compiler rather
than the one built from source.

**The `--emit-c` repair dropped into a check it had been jumping over.** Removing
the early return put `--emit-c` under `needs_main`, and 23 emission cases went
red at once — on files whose whole purpose is to produce C for another program,
which that diagnostic's own note recommends.

**A record entry was written for a defect still standing in the open list**, and
`records/numbering` refused the tree for issuing 046 twice. The instrument was
right and the bookkeeping was not.

**And the panel's own instrument was damaged once, by the coordinator.** Two
seats killed by the watchdog were resumed with another seat's findings in hand,
so panel 155's split is two independent readings plus two informed concurrences
rather than four of five. The rule that came out of it — a resumed seat is given
its own brief again, never another seat's answers — held at 156 and 157, where no
seat died at all.

## What landed, and what carried forward

**M-check-completeness closes 2026-09-17 with every list at zero**, which is the
first thing to say because for most of the milestone it could not have. The tag
rule refuses a close over an open defect, and the defects kept arriving from the
sittings convened to answer other questions — ten opened, ten closed, six
sittings, 155 to 160.

**Its subject was `heroes check` accepting what `heroes build` refuses.** Face 1
was found already closed and pinned on the morning it opened; faces 2 and 3 went
to panel 155 and wait under Principle 0; face 4 is M-package-manager's. What the
milestone actually delivered is the ten defects that hunt turned up, and they
are better than the subject: three of them were not what their own entry said.

**049** was filed as *the spelling of a tag*. It was `--emit-c` writing the
artifact of a build that never passed — exit 0 and 10468 bytes where `build`
said `ffi_missing_header` at exit 1. **050** was filed as *the four readers of a
fallible value*. It is the one reader that returns a `bool`: the other three
hand the second level back in their type. **056** was filed as a hole in the
generics. It is a hole the library cannot reach, since not one of `find`, `any`,
`all`, `map`, `filter` or `fold` applies a reader to its type parameter — which
moves it from urgent to filed.

**Three numbers worth carrying.** The document ends the milestone **smaller than
it started while gaining two rules**: panel 160's refusal is −11 vendored and
−12 real, the first ledger row ever to shrink. Defect 054's correction is **+4
vendored and −2 real**, the first where the two instruments disagree on the
SIGN — no factor can bridge that, which is the author's instruction of
2026-09-16 arriving with its own worked example. And `emission` fell by 22
because defect 049 retired that many blessed artifacts, **fifteen of which were
blessing C that does not compile**.

### Predictions scored at this close

- **Ledger row 85's, from panel 110's historian, HELD.** It said the first
  generic instantiated at `()` would need either a value expression for `()` or
  a rule refusing it, and named its falsifier as a generic taking `()`
  everywhere with no special case. Measured: `fold(xs, (), step)` is
  `error[expected_expression]` — there is no value expression for `()` — and
  `map(xs, nothing)` where the callback returns `()` is `check` 0 and `build` 1,
  `unsupported[unit_element]`. The disjunction's second branch is what the
  compiler took, and the special case exists.
- **Panel 160's compiler-engineer, HELD on all four clauses.** `suite_layout`
  reads `selfhost/check/builtins.hero 378`; `nested.hero` is 53 lines against
  its ≤ 60; `nested_read` fires on **0** files across `examples/` and
  `tests/golden/run/`; and the landing touched no file under `selfhost/ir/` or
  `selfhost/emit/`, which was its own named falsifier.
- **Panel 155's compiler-engineer, LAPSED and not falsified.** It said
  `-- ./heroes run` would go red on exactly two files *if the float half lands*.
  That half did not land — panel 155 R3 left `float_map_key` through a generic
  waiting under Principle 0 — so the prediction's precondition is false and it
  is marked lapsed rather than scored, which panel 046 R2 asks for explicitly.
- **Panel 158's spec-warden, LAPSED.** It predicted 7974 real / 5989 vendored
  unchanged under a +0 resolution; panel 159 moved the base with three unrelated
  sentences before it could be read. Unscorable as written, and the synthesis
  says so rather than scoring it against a base it did not price.
- **Panel 160's spec-warden, SCORED EXACT ON BOTH INSTRUMENTS.** It said
  E3+Ra+Re lands at **7986 real / 5993 cl100k**, and it does — the first
  prediction in that ledger to hit a real count to the token.

### What carried forward

**Panel 157's R4** — build both ways, compare exit code and stdout, and LINK —
stays adopted and unbuilt at M-package-manager. It is the instrument that would
have FOUND 048 and 049; what this milestone did is repair what it would have
found. **Panel 155's R3**, `float_map_key` through a generic, waits under
Principle 0. **Panel 160's open question**, whether the presence test on a
`{K: V?}` deserves a spelling of its own, is a sitting of its own — the
historian found Go named it `v, ok` and Kotlin `containsKey`, both as additions
beside a form that still compiled, and predicted the request arrives before the
next tag. And **defect 056's costing** is half done: the shape that fits the
existing call-site seam is described and neither shape is built.

### Three rules the author gave, all with an executor

*Always measure with the real* (`spec-shape.md`): a vendored delta is not a
price, and the offline route cannot reach the binding number at all.
*A live list is a preamble, a count and its items* (`records.md`): `DEFECTS.md`
went from 982 words of preamble to 135, and the two new checks — a preamble
ceiling and a comparison of the ROADMAP's counts against the lists' own banners
— were each made to fail before being trusted. *And the lane* (`records.md`): a
lane is how work continues while this tree is owned, which is what four of the
ten defects were repaired under.
