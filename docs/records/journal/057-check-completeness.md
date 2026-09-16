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
