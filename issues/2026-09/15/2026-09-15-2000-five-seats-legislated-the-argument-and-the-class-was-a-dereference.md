# Five seats legislated the argument, and the class was a dereference

2026-09-15. Panel 154, convened without asking on `docs/work/DEFECTS.md` 045,
full panel plus the completeness critic. The sitting is
`docs/panel/154-the-class-was-never-argument-shaped-and-one-flag-closed-it.md`.

## The convergence was sound and the frame was wrong

Four of five seats adopted the same route: guard every handle argument by
default, with a word permitting null on the parameters where C accepts it. Two
vetoed the alternative that infers from the marks already on the binding, and
§1.12 refuses it in advance in words no seat had quoted before today — *"they
check at the boundary even where the value is known to be good, because the
alternative is a rule that has to reason about where a value came from, and
provenance is a premise about the world."*

Every one of the five routes legislated the ARGUMENT at a Heroes call site. The
class is a **dereference**, and the completeness critic measured that it reaches
three places an argument rule cannot: a null `ptr`, which no route could refuse
without refusing a golden already in the tree; a null reached THROUGH a non-null
handle, which is defect 042's own list-walk shape; and a field past the
runtime's window.

## One string in the flag list closed it

`-fno-delete-null-pointer-checks`. The standard lets clang assume a pointer it
dereferences is non-null and delete every check downstream, so the fault was
folded away and the program lied: measured at `-O2`, a single call through a
null handle **printed `8372224` and exited 0**, five runs of five identical — a
deterministic wrong answer, the shape a golden would have recorded and passed on
forever. With the flag, every witness the sitting could build says what it did.

It refuses nothing, because it guards the dereference and not the argument:
`free(NULL)`, `sqlite3_close(NULL)` and the shipped `getaddrinfo(hints:
nullptr)` golden are all still legal. It costs nothing measurable — twenty
compiler runs at a median of 2.87 s against 2.87 s, 10^9 iterations of a walk
built to cost the most at 1.04 s against 1.04 s, the emitted C byte-identical,
the seed 0.38% larger.

**So the resolution is not the seats' convergence**, and the critic's own
sentence is why: *here the cheapest in tokens and the most complete are the same
route, which is the tell that the frame, not the price, decided.* Route A's
guard is adopted and unlanded, because it buys the blame line and not the class.
Route B's fourth word is deferred on the rule the compiler-engineer stated and
the sitting was held to: two is a class and one is a witness.

## And the critic falsified my own brief three times

The `-O2` row was false of the fixture it cited. *No runtime guard can ever
reach it* was false twice — the trap is a deliverable signal and one flag returns
the fault to the handler the runtime already owns. And the window under the
repair I had landed an hour earlier rested on *a field's offset is smaller than a
page in every struct a header can lay out*, which is a premise about the world
that `.claude/rules/module-shape.md` forbids a narrowing to rest on. Three lines
of header falsified it; the seat then laid out 275 records from 21 headers of
this SDK and found exactly one over a page. The floor is now the platform's own,
Darwin's measured 4 GiB here, and the message prints the offset so a reader can
tell a null plus a field from a small wild pointer.

A fourth correction, smaller and older: `guard_cstr_arguments` does not exist.
The function is `guard_arguments`, and the stale name sat in
`.claude/rules/c-boundary.md` until today — from where I copied it into this
defect's entry and into both of the sitting's briefs. A citation nobody greps
propagates into the framing of a sitting.

## Open defects: zero

Five were closed today — 037, 042, 043, 044 and 045 — and two were opened and
repaired on the same day. What is waiting is the author's, on two sittings:
whether the language should read a struct behind a pointer when a header already
does (panel 153), and whether the null guard is worth landing for its message
alone (panel 154).
