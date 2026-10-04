# The option set was short by one, and the question was never A or B

2026-09-15. Panel 153, convened without asking on `docs/work/DEFECTS.md` 042 and
043, full panel plus the completeness critic. The sitting is
`docs/panel/153-the-option-set-was-short-by-one-and-the-route-nobody-listed-already-ships.md`.

## Five seats converged, and the critic moved the resolution twice

Four of five objected to Route A and adopted Route B; the historian approved A by
precedent and objected to B as the only read path. That is a sound argument
between two options, and the critic's first finding is that **the second option
contained the first's precondition**: `one_tag_one_type` had to narrow under
either route, so Q1 was never the A-or-B question the brief posed.

Its second finding is the one that changed the answer. The brief asserted that
nothing in the language turns a handle into the struct it points at, and the
compiler-engineer confirmed it after searching the IR, the library mechanism,
generics and the `@` path. **All four searches were inside the compiler.** The
FFI is where §1.11 says everything in this language comes from, and three lines
of header — a typedef and a `static inline` returning the struct by value — make
the SHIPPED compiler read `ai_family` and walk `ai_next` with output
byte-identical to the equivalent C program. Zero compiler change, zero new words,
zero spec tokens. Twenty headers of the project's own already ship under test.

So the sitting adopted the narrowing, which both routes need and which closes
defect 042's own sentence, and **queued the read question for its own sitting
with that route on the ballot** — because a resolution cannot be the most
complete one when it is chosen from a set that was missing its cheapest member.

## The narrowing's premise held where it was written and failed where it reached

`duplicate_tag` refused two Heroes types over one C type on the ground that
clang cannot see a swap between them. Measured in C alone: two handles are
`struct s *` twice and clang is **silent**; two records with fields are
`struct s` twice and clang is silent; a handle beside a record is `struct s *`
and `struct s`, and the swap is **two clang errors in either direction**. The
rule keeps the two shapes it was written for and lets go of the one it was
catching by accident, which is how `getaddrinfo` is written.

**The critic found the narrowing shipping a defect its own source file names.**
`handles.hero` had given the diagnostic lookup as a REASON for the rule — a
message about the second record would carry the first's caret — and the
prototype removed the rule without touching the lookup. The critic built that
shape and got exactly the predicted wrong caret, on a path no suite watches
because both arms exit 1. Repaired before landing, with a golden.

## What the sitting cost, and both are the coordinator's

The tree was declared frozen from the briefs to the synthesis and it was not:
defect 043's repair landed at 17:07 while four reports are stamped 16:51, so the
spec-warden's twenty-row table is priced against a base that moved under it. And
the ergonomist's blind A/B was not blind, because the brief printed § 13 verbatim
at its foot; its ten readings per fence are one seat's narration, not ten runs of
a model, and that ratio had already been written into an append-only record as
the thing that paid for defect 043. Both are corrected underneath rather than
excused, in the record each contaminated.

## What it produced

Defect **045**: a null handle handed to a C function that reads through it
segfaults at exit 139, both streams empty, where a null `cstr` has been guarded
since M-robustness-guards. §1.12 forbids it by name. A blanket guard is refused
in advance, because `freeaddrinfo(NULL)` is legal C, so the rule must say which
handle arguments are guarded and that is a diagnostic class.

And one the sitting deliberately did not file: a handle read out of a borrowed
node and then released is a silent double free today, and it is the documented
limit of `borrows` that `abort-handle-borrows-that-gives-away.hero` already
states in its own words. Named in the sitting so the author can disagree.
