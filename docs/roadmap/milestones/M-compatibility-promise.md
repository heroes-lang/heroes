# M-compatibility-promise — the paragraph `1.0.0` rests on, and its instrument


**Scheduled 2026-09-10 by author decision**, from § What production-ready means
row 1. M-publication-gate owns the paragraph — *"Silence reads as a promise"* —
and `README.md:117` already says the version stays below `1.0.0` until it is
written; this row delivers the paragraph **and** the instrument, and discharges
that bullet.

**What warrants the instrument.** Go 1 published *Go 1 and the Future of Go
Programs*, Nim 1.0 promised that code compiling under 1.0 compiles under any 1.x,
and Rust 1.0 put *stability without stagnation* on a train where an unstable
feature is an error on stable. **In all three the sentence has a mechanism**, and
in a repository whose contract is that a claim is written down only after the
command that settles it has run, a paragraph alone is the one unrun claim left.

**What it delivers**, and both halves exist already: the corpus at a release tag
recompiled by today's compiler, so a program that stops compiling is a **broken
promise** rather than a discovery; and the trigger, since `git diff vA vB --
spec/heroes-spec.md` already moves `Y` and its emptiness moves `Z`
(`.claude/rules/records.md` § Release tags). No new verb is owed.

**The hardest sentence is a consequence of §10.** A program cannot declare the
language version it was written for — no fourth input class, and panel 056
refused a per-project file — so the promise is **one-directional**: the compiler
must not break old programs, and an old program cannot ask for an old compiler.
**And the paragraph says the deployment model out loud**: crash and be restarted,
because Part 6 refuses exceptions permanently and every abort `spec:166-169`
lists is uncatchable. Defensible as a model, indefensible as a surprise.

**Why here.** Immediately before the gate, which then publishes the `1.0.0` this
row earns.

**What it does not deliver**: the `1.0.0` tag itself, which is the author's act
on a clean `main` and never a milestone's.

**Re-ordered 2026-10-14, by the author's instruction**: from 79 to 73, closing
the consolidation block and ahead of the packages, so that the instrument this
page describes runs while the largest body of Heroes after the compiler is
written. The paragraph is still published at the gate. It carries one promise
more, written the day the history was rewritten once to keep the seed only at
the releases (M-bounded-seed): `main` and the published tags are never
rewritten again.

**Corrected 2026-10-15**: the history keeps the seed at every tag, not only at
the releases, by the author's decision of that day (M-bounded-seed's page);
the promise is unchanged.

**The order of evaluation, the first item of the next sitting on the
specification** (the author's *4a* of 2026-10-25). The specification says
nothing of the order in which a call's arguments and an operator's operands
are evaluated, while the compiler lowers them left to right
(`selfhost/ir/flatten.hero:190`, `:619` and `:639`, `selfhost/ir/inout.hero:30`,
and a record's fields in source order, `selfhost/ir/containers.hero:223`); a
program whose output depends on the order means one thing today and is
promised nothing. Panel 010 found the word ambiguous once named arguments may
be written in any order, left to right of the call or of the signature
(`docs/panel/010-repeated-mutable-arguments.md:46-47`). The promise `1.0.0`
rests on states it, so the next sitting on the specification takes it first,
priced on the reader's tokeniser with its removal or prediction named
(design.md § 1.6). A comparison read the same day: Goose's specification fixes
it in one sentence, "Evaluation order is left-to-right everywhere (operands,
arguments, field initializers ...)" (github.com/aardappel/goose,
[goose/docs/goose_spec.md](https://github.com/aardappel/goose/blob/master/docs/goose_spec.md) § 2).
