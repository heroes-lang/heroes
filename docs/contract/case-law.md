# The contract's case law

`CLAUDE.md` is the operating contract. This file is what each of its rules
COST to learn: the incident, the measurement that settled it, and the author's
words where the rule is theirs. One entry per rule, `CL-NNN`, in date order.

**Why the two are separate, and it is the contract's own doctrine applied to
itself** (author instruction 2026-09-07, *"study how to re-engineer CLAUDE.md
to give it more order and priority, following the best practices, and to make
sure every context takes in its rules and that there are no conflicting
rules"*). Measured that day before the split: `CLAUDE.md` was **1247 lines and
22689 tokens** by `heroes measure`, against **3871** for
`spec/heroes-spec.md`, which is the whole language. It carried **62** emphasised
amendment paragraphs, and every session loads all of it on every turn,
including the five panel judges. The rules were right; the shape was a diary.
So the rule stays in the contract in one line and the story it was born from
moves here, whole rather than summarised. Nothing is lost twice over: the text
below is the text that stood in `CLAUDE.md`, and `git log -- CLAUDE.md` holds
every version of it.

**This file is a record.** It is append-only and never rewritten, like
`DESIGN-LOG.md` and `docs/work/DONE.md` (CLAUDE.md §14), and
`tests/harness/suite_records.hero` treats `docs/contract` as one. Where a
sentence below has since been falsified, the entry says so underneath rather
than losing the sentence: a measurement that was right when it was taken is
history, and a contract that states a danger the compiler has already shut
funds the wrong decision next time.

An entry's third line says where the rule lives now. `§` alone means a section
of `CLAUDE.md`.

---

## CL-001 — Code is written to be read
2026-08-04 · author instruction · § 11

Files stay short and single-concern: split a module before it passes ~300
lines; every file opens with a module doc stating its role and citing its
design.md sections; comments teach the invariant and the why, never the diff.
The author must be able to open any file and read it without drowning.

## CL-002 — A whole milestone may be asked for in one step, and a choice surfaced twice is a question
2026-08-04, sharpened 2026-09-02 · author instruction · § 3

(2026-08-04, *"carry on until m2 is finished, and always accept your own
recommendations"*; sharpened 2026-09-02, *"carry on without asking me"*, after
one trade had been put to the author in three recaps running). Chain the
steps, close the milestone, decide the delegated questions with the recommended
resolution as the provisional default, and say which way it went, once.

## CL-003 — Milestone identifiers are names, not numbers
2026-08-12 · author instruction; panel 030 R7 as amended · `.claude/rules/records.md`

The argument lives in that sitting. `docs/ROADMAP.md` § The names carries the
map. The algorithm: two words, `M-<what-it-delivers>`, hyphenated and lowercase
after the `M-`, the tag the same string lowercased; name the deliverable, never
the area, because the area must stay free for the second milestone that touches
it, and one already exists (`M-module-namespace` and `M-separate-compilation`
are both about modules); prefer a phrase the ROADMAP entry or the journal slug
already uses; an id is never renamed once it is in the record, and a milestone
that changes shape gets a new id while the old one is retired in § The names;
order lives in the ROADMAP's § The chain table and nowhere else; an id never
reaches a diagnostic; and appending to a dated record uses that record's
vocabulary, with the new name in parentheses on first use, as in *scored at M8a
close (M-module-namespace)*.

**A one-word name appropriates a topic, and an identifier must make no claim a
later milestone can falsify.** The record is never rewritten: `docs/panel/`,
`DESIGN-LOG.md`, `docs/journal/`, `docs/measurements/`, `docs/work/DONE.md`,
`docs/book/beats.md`, `tests/golden/`, every commit subject and the twelve
legacy tags keep the numbers.

**Two lists inside that rule went stale and are corrected here.** It named a
defects directory until 2026-09-03; its seven files are six entries in
`docs/work/DONE.md` and one in `docs/work/DEFECTS.md` now. It named a reasoning
directory until 2026-09-04; its seven notes are seven entries in
`docs/work/DONE.md`. And it cited `archive/bootstrap-rs/heroes/src/emit/tests/gate.rs`
as the assertion that no id reaches a diagnostic: that file exists and nothing
builds it, so the citation could not fire. Measured 2026-09-07, the live
assertions are `tests/harness/suite_records.hero`'s `records/expected` and its
`records/numbered`.

## CL-004 — A narrowing asks the value, never the world
2026-08-12, sweep 001 · author instruction · § 11

A filter, an allow-list of kinds, or a `_ =>` arm is a decision, and its
correctness rests on something. Rest it on a fact about the value in hand
(*this* expression's extent, *this* declaration's fields) and never on a
premise about the world around it (*"a multi-line list is the only value that
does not end where it started"*; *"nothing reads this payload"*). A fact about
the value cannot expire. A premise about the world expires **silently**, and
the comment justifying it goes on reading as correct, because the argument is
still valid and only the premise died. Two of sweep 001's twenty were exactly
this, and both had been read and agreed with, which is why a convention about
comment style would not have caught either.

Where a premise is unavoidable, two things are owed: write it as a
**falsifiable claim**, not a justification; and give it **a test that fires when
it dies**, whose failure message names what depends on it
(`a_declared_type_cannot_contain_a_type_parameter` is the shape: one premise,
three dependants in three modules, one test). And put the fallback in the
**loud** direction: `_ => hero_unreachable()` beat `_ => false` by a whole class
of defect at M5c, and D3 paid for the same lesson twice.

## CL-005 — A refusal is held to the same standard as a feature
2026-08-12 · author decision; panel 039 · § 12

A design.md Part 6 row must name the program or the compiler fact that would
make it wrong. The rule's home and its argument are Part 6's own preamble.
Principle 0 binds what *enters*, so without this a permanent rejection was the
one design act under no burden of proof.

## CL-006 — Performance is not a goal, and not a licence either
2026-08-12 · author instruction · § 13

*"performance is not a goal, but it must not be a ceiling either"*. The rule
forbids reaching for speed as a **reason**; it does not make slowness
acceptable as a **ceiling**. Where a cost stops a program the closure list
needs from running at all, that is design.md §1.0 compiler-need and it goes to
the panel, not to this line.

## CL-007 — The ABI stamp catches a header from another compiler, and not a decoy
2026-08-12 · panel 034 R5, panel 037; author decision the same day · `.claude/rules/generated-c.md`

Panel 037 measured it from the other side: two runtimes with changed
*behaviour* kept the number at 10 and every generated unit accepted them. A
decoy `runtime/` that copies the number passes; what protects a build against
one is the **cache key**, which covers the whole runtime's contents. The
stamp's real job is the version skew the search makes possible, and it is worth
keeping for that alone, **so it is not extended to cover behaviour** (author
decision 2026-08-12, closing panel 037's open condition: a second guard over
what the cache key already hashes buys nothing, and the guard that would have
been asked to grow is the one that cannot see a decoy at all).

## CL-008 — A clang failure the author's own extern caused is exit 1, not exit 2
2026-08-12 · author instruction; panel 036, widened by panel 048 · `.claude/rules/c-boundary.md`

Four classes: a result type the header refutes, a `constant` that is not one, a
name the header does not have, and, since panel 048, a symbol the **linker**
cannot find because the group named no `link`. **The narrowing is
`declaration()`, not whose text it is**: every class recovers a name and asks
whether *this program* declared it `extern`, so a symbol nobody declared stays
exit 2 and the compiler's.

§7 said for a milestone that `archive/bootstrap-rs/heroes/src/emit/ffi.rs`
*"matches only the assertion messages this emitter writes"*, and that was
already false when written, since `unknown_name` matches clang's own. That is
CL-004's class, and the stronger rule was the true one all along.

## CL-009 — Three lists, split by what an item asks
2026-08-12 · author instruction · § 3

`docs/work/DECIDE.md` (what should be true), `docs/learn/LEARN.md` (what is
true), `docs/work/SCHEDULED.md` (work with a milestone that will do it).
Everything that once gated progress (predictions, spot-checks, golden
ratification, failure diagnosis, drills) became an entry in one of them.
`/decide` was split out of a `/debrief` skill deleted the same day, together
with `/learn`.

## CL-010 — In a loop, a wakeup is at most three minutes and every one writes a recap
2026-08-12 · author instruction · § 3

The recap is the point, not the schedule: an unattended session that works for
an hour and then reports once has made an hour of decisions the author could
not have redirected. Short intervals buy interruption points. Each recap says
what advanced, what was verified as already closed, and what was deliberately
not done and why. The last is the one that is easy to omit and the only one
that lets the author disagree.

## CL-011 — The ~300 governs what a reader must hold in their head, so it binds the compiler's code and not its tests
2026-08-14 · author decision · § 11

A test file is read one case at a time and a case is self-contained, which is
why `surface.rs` at 1164 lines is legible and
`archive/bootstrap-rs/heroes/src/emit/ffi.rs` at 573 was not. And the number is
a threshold to think at, not a limit to round: `toolchain.rs` stays at 400 by
the same decision, because the cut that would take it under runs through the
cache key `link` and `runtime_object` share, and a file split against its own
seam is harder to read than a long one.

**Both worked examples are now archived Rust and both numbers were wrong**
(measured 2026-09-07): `archive/bootstrap-rs/heroes-cli/tests/surface.rs` is
**2139** lines, not 1164, and
`archive/bootstrap-rs/heroes-cli/src/commands/toolchain.rs` is **388**, not 400.
The reasoning is what the entry keeps. **And the rule's unit was never written
down**, which is CL-017's own failure inside CL-011: by `wc -l`, 71 of 189
`.hero` files under `selfhost/` pass 300, while `tests/harness/suite_layout.hero`
pins `selfhost/check/walk.hero` at 1700 against 2100 physical lines, so the
instrument counts something narrower. The contract now says which unit binds.

## CL-012 — Robustness is a goal of the language, and it wins
2026-08-14 · author instruction · § Precedence, § 12

A Heroes program must not segfault and must not corrupt memory. Where a choice
runs toward that, it beats every other criterion in the contract: elegance,
token cost, ergonomics, the size of the compiler, and speed. This is the mirror
of the performance line (CL-006) rather than an exception to it: performance is
not a goal *and* not a licence, while safety is a goal *and* a tie-break. The
`heroes mutate` corpus and `--sanitize` are how it is measured rather than
asserted, and the rule does not suspend Principle 0: a form still enters only
if the compiler needs it or it serves the thesis. What it decides is the case
where two admissible forms disagree, and one of them can be made to crash.
The principle is design.md §1.12; the contract carries its operational half.

## CL-013 — gnu11, named and not inherited
2026-08-14 · panel 047, ratified · `.claude/rules/generated-c.md`

`gnu11` rather than `c11` because the two differ by one predefined macro,
`__STRICT_ANSI__`, whose only effect on glibc is to hide `M_PI`, `strdup`,
`fileno` and nine more of what design.md §1.11 says a program binds. And *only*
that: `__typeof__` and `__builtin_types_compatible_p` are clang extensions that
work under `-std=c11` too, measured at panel 092, so nothing else rests on the
`gnu`.

## CL-014 — Plain words for the author
2026-08-15 · author instruction · § 11

*"always explain things to me in this simple language, in plain words, because
sometimes I have a hard time understanding it"*. This is not a register for
summaries and status reports, it is **every** explanation, including the middle
of a working session, including a defect's cause, including why a panel ruled
as it did. The rule the `/where` skill already states is the rule everywhere:
**assume zero compiler knowledge**, name the thing before the term for it, and
say what a change means for a program somebody writes rather than for a module.
A sentence the author has to re-read is a sentence that failed, and the author
is the one person this project is being built for. design.md §1.1 makes their
comprehension **the objective**, so an explanation they bounce off is the
objective missing, not a communication style.

## CL-015 — The Nim list's last clause is retired, because its premise was false
2026-08-15 · author decision; panel 056's historian · § 6

The list used to end *"a separate package binary: never"*. It was a bare rule
with one citation and no argument, and its implied contrast was **factually
wrong**: nimble ships *with* Nim, so the thing it named as separate is not.
Worse, it was the rule a reader would reach for first to refuse a project file,
and it does not reach one, because a file is not a binary. What it was trying to
say is the one-command rule's, is already there, and is there with a reason:
**never a second binary**.

## CL-016 — There is no fourth slot, because there is no fourth input class
2026-08-15 · author decision, recording what panel 056 adopted · `.claude/rules/cli-surface.md`

A subcommand, a flag and nothing are the three shapes, and they all answer
*what does this capability do to the inputs the tool already has*: a `.hero`
file, argv, and the machine's environment. A **per-project file** is none of
them. It is a new class of input, and the rule as written does not admit one.
That refuses a project file more cleanly than design.md Part 6 could: 056
refused its Part 6 row precisely because the falsifier such a row owes becomes
constructible the day `heroes add` exists, which design.md §3.5 already
promises. Two things follow and both are the point. **The refusal is not
permanent**, it is conditional on 056's three return conditions (one key not
two; no string in the file that also appears in a `.hero`; one named binding
`package` plus one command line cannot build), which are its only amendment
path. And the `#~` annotation invariant is *evidence for the rule rather than a
second job*: the golden harness collects `.hero` only, so a new input class
would arrive with its diagnostics exempt from the one invariant a regenerator
cannot forge, which panel 020's historian already refused for `unsupported`.

## CL-017 — A number or a repository fact goes into a record only if it was measured in the session that writes it
2026-08-16 · author instruction · § Run it, or say it is unrun

Not recalled, not carried from a comment, not inferred from a document that
asserts it. **Three sittings in a row were briefed with something false and the
judges caught all three**: panel 066 was told a Rust test existed that never did
(a `grep` would have shown it), panel 067 was given a **line** count against
design.md §1.2's **token**-denominated formula and a *"the port is longer"* that
compared code+tests to code-only (it is 21% *shorter*), and panel 068 was told
`sort.c` solved NaN with totalOrder when `DESIGN-LOG:205` had superseded `:148`
four days after it was written. A fourth was a line count typed from memory
(872 for 1129).

Three obligations follow, and each one killed a specific error above: **run the
measurement**, whatever it costs, with `wc -l`, `grep`, `heroes measure`, or the
program itself; **match the unit to the rule that will judge it**, since §1.2 is
in tokens and a line count answers a different question; and **when a fact comes
from a record, read forward to the end of that record**, because DESIGN-LOG and
the panels are append-only, so the entry you found may have been overturned
below it.

The cost of the rule is minutes. The cost of breaking it is a five-judge sitting
answering the wrong question, which is what happened three times.

## CL-018 — And the rule binds CLAIMS, not only numbers
2026-08-16 · author instruction, after one session broke it four ways · § Run it, or say it is unrun

Everything in CL-017 is written about counts, and the day's worst errors carried
**no number at all**. Four shapes, each one measured in the session that named
it:

- **An inference presented as a measurement.** A brief told five judges that a
  new abort would let `runtime/parts/sort.c`'s two guards be deleted, *because
  the general rule subsumes them*. It does not, since `sort` compares through a
  function pointer no emitted `<` ever reaches, and **three seats falsified it,
  two by deleting the guards and running**: `sort` returned an arbitrary
  permutation at exit 0. **The tell is the connective**: *so*, *therefore*,
  *which means*, *it follows that*. A sentence whose truth comes from another
  sentence rather than from a command is an inference, and it is either run
  before it is written or marked as one on the page.
- **A failed search written as an impossibility.** The same day, a brief told
  three judges that C cannot detect a union naming one member, having searched
  for `__is_union`, which is C++ only. Two seats that compile found the answer
  in `__builtin_classify_type`, **a builtin this emitter already calls on every
  field**. A negative claim is the least reliable kind there is, because it
  rests on the searcher's vocabulary rather than on the world: *"X cannot be
  done"* goes to the judges as a **question**, never as a premise, and names
  what was searched for.
- **A silence read as an open question.** A sitting was convened on *"the spec
  never states this"* when the silence was **the artifact of a ruling**: panel
  035, four days earlier, three seats, and the word `integer` at `spec:149` is
  that ruling's one-word diff (`ec5558b`, `DESIGN-LOG:207`). CL-017's third
  obligation says to read forward from a record; this says **which** record.
  Before convening on something the spec does not say, grep `DESIGN-LOG.md` and
  `docs/panel/` for the thing that is **not** there. A deliberate silence and an
  oversight look identical in the document and opposite in the record.
- **A repair shipped without its adjacent shapes.** The union predicate landed
  for a two-field union and was still live for a **padded** one three hours
  later, and `SDL_Event` is padded, so the case the sitting existed for was
  never fixed. Before the commit, a repair is attacked at the shapes *next to*
  the one that provoked it: one field, none, padded, nested, tagged, generic,
  empty. The provoking case is a witness, not the class.

The cost is minutes again. The cost of breaking it was a sitting convened on a
settled question, and a defect that shipped as fixed.

## CL-019 — And plain is not flat
2026-08-16 · author instruction · § 11

*"I want you a bit more excited when you tell me the good things and the bad
ones too, we are making a new language"*. The register the author asked for is
plain **and alive**: when something works, say so like it matters, because it
does. A defect that printed a false number at exit 0 and now stops on the
author's own line is a *win*, not a line item. When something is bad, say that
with the same force and no cushioning: a wrong premise in a brief, a rule that
would have broken a working program, a number that turned out to be invented.
**The two halves are one instruction**, because an assistant that only gets loud
about good news is a flatterer, and its enthusiasm stops carrying information.
What this does not license is inventing either: the feeling attaches to a
measurement, never to a hope, and *"this is huge"* about something unmeasured is
worse than a flat sentence, because it spends the author's trust on nothing. The
excitement is for what was **run**.

Two things this does **not** relax. Everything **written into the repository**
stays English and stays precise: a journal, a panel file and a commit body are
records, and a record that trades exactness for warmth is a record that will be
wrong later. And plain does not mean vague, because the numbers, the file names
and the measurements still belong in the sentence: *"it was slower"* is not
plainer than *"it took 0.94 of the time"*, it is only emptier.

## CL-020 — And the threshold yields entirely where the language forbids the seam
2026-08-16 · author instruction · § 11

*"go past the rule for heroes"*, answering the port's measurement. Heroes
refuses module cycles (`module_cycle` fires on the `use` edge, whatever it
carries, measured both ways) and a recursive-descent grammar is mutually
recursive by construction, so its knots cannot be split at all: over
`archive/bootstrap-rs/heroes/src/syntax/` (named `crates/heroes/src/syntax/`
until 2026-08-19), `expr`+`primary`+`control`+`stmt`+`name_stmt` is 1025 lines
and `decl`+`data`+`externs`+`extern_members` is 736, each one module or nothing.

This is `toolchain.rs`'s reason at its limit: there the cut merely ran against a
seam, here **no cut exists**, and a rule that cannot be obeyed is not a standard
but a lie. Panel 031 R6 chose the cycle refusal as *"the reversible direction"*
and priced relaxing it against M-separate-compilation's topological order; the
port supplies the other half of that trade and the answer is that the file rule
yields, not the language. **What is owed in exchange is what the ~300 actually
protects**: a knot carries a module doc naming its cycle, its entry points and
which function calls which, so a reader opens it and finds a map rather than
drowning.

## CL-021 — The Heroes subset of Rust is spent, and kept as the record of what it bought
2026-08-19, M-bootstrap-archive · § 5

The Rust it governed is `archive/bootstrap-rs/`, which nothing builds and
nothing lints any more, so it constrains no code written from here on. It is not
deleted, because it is the reason the port was a transcription rather than a
rewrite: references only as function parameters, never in structs or return
types; owned data everywhere, indices for links; `BTreeMap`/`BTreeSet` only;
iterator and `Option` closures fine, *stored* closures not. It was enforced
while it was live by `archive/bootstrap-rs/clippy.toml` (the reasons live there)
plus `#![forbid(unsafe_code)]`, with every necessary violation carrying
`// PORT-DEBT: <reason>` and the count as the distance from self-hosting. The
distance is now zero. **The rule that replaces it is the language**: `selfhost/`
is written in Heroes, where value semantics and the absence of references are
not a subset anybody has to remember.

## CL-022 — The one declared exception to the one command has expired
2026-08-19, M-bootstrap-archive · `.claude/rules/cli-surface.md`

`cargo build` and `cargo test` built the compiler until the fixpoint, and the
way in is now one clang line over `seed/heroes.c`. What runs the tests is the
one command.

## CL-023 — A panel-gated step asks once and expects yes
2026-08-19 · author instruction · § 4

*"convene the panel and finish"*. The sitting is mandatory and the author has
said its cost is worth paying: ask once per milestone, then convene without
asking again, choosing only the seats whose input differs.

## CL-024 — Every decision put to the author comes with a recommendation and its reason
2026-08-23 · author instruction · § 3

*"for decisions, always tell me in plain words which option you recommend, and
why"*. A neutral list of options hands the author work the assistant has already
done. The recommendation rests on a measurement or a verified fact, never on a
hope; the decision stays the author's.

## CL-025 — Never slow the compiler down
2026-08-23, extended 2026-09-05 and 2026-09-06 · author instruction · § Verification

(2026-08-23, in capitals: *"whatever you do, do not make the compiler's
performance worse, that would be a tragedy"*). A change that makes the compiler
slower does not land on a hunch: time before and after with `/usr/bin/time -p`
(zsh's own `time` prints no `real` line, and a whole comparison was once lost to
that), look at the machine's load first (a stray `ffmpeg` once turned 188 s into
282 s), and report the number. The verification loop is already the slowest thing
in the project, measured 2026-08-23: a full emission 16m51s,
`heroes test selfhost/main.hero` 20m35s. Every added percent is paid at every
rebuild until somebody stops verifying, which is the real cost.

**AND THOSE TWO NUMBERS ARE THE COLD ONES: say which you measured** (added
2026-09-05, after a session budgeted 37 minutes for a verification that took 64
seconds). With the per-module cache warm and three files changed,
`heroes build selfhost/main.hero` is **28 s** and `heroes test
selfhost/main.hero` is **35 s**, both measured with `/usr/bin/time -p` on this
Mac. The 2026-08-23 pair stands and is not deleted: it is the cold tree, which is
what CI and a fresh clone pay. The reason to carry both is that the cold number
is the one that funds *"do not make it slower"* and the warm number is the one
that decides how a session is planned. A rebuild believed to cost 17 minutes gets
batched, deferred, and sometimes skipped, and skipping is the failure this whole
entry exists to prevent.

**AND THE LOAD THAT RUINS A MEASUREMENT IS THE ONE THE MEASURER MAKES: WHILE A
CLOCK IS RUNNING, THE MACHINE STAYS STILL** (author instruction 2026-09-06,
*"write this in CLAUDE.md"*, given after a session spoiled its own timings three
times). *"Look at the machine's load first"* is above and it was **obeyed**:
1.89, nothing heavy, checked before the first run. It looks at an **instant**,
and the load that ruined these runs did not exist at that instant. The session
created it afterwards, by building, by running three suites, and by syncing the
Windows box while its own comparison was in flight. One arm then read **937.21 s
of wall against 33.68 s of CPU** where the other arm's comparable run was 36 s,
which is 3.6% of one core, which is memory pressure from two 975,000-line
compilations at once and not a fact about the code. Three obligations follow.
**Nothing else runs while a timed run does**, not a build, not another suite, not
a container, not an `scp`; a remote box is not an exception, because the push,
the copy and the ssh all run here. **The tell is the ratio, so read it**: `real`
far above `user` + `sys` means the process was waiting, not computing, and the
run is discarded rather than explained. And **a discarded run is written down
with its reason**, because a measurement that quietly drops its inconvenient half
is worth less than none.

**The same discipline binds the TREE, and that is the worse version of it**: the
same session edited `runtime/` while the net was running over that `runtime/`, so
some checks used the old objects and some the new. That run was killed and
redone. A suite reading the tree owns the tree until it exits.

## CL-026 — The panel trigger pointed into an archive, and four of five seats died on it
2026-08-23 · § 4

The trigger for the commonest panel path named
`crates/heroes/src/{lexer,syntax,types}/` until 2026-08-23, four days after
M-bootstrap-archive moved that tree to `archive/bootstrap-rs/`, and the brief
that convenes those panels pointed there too. That is how **four of panel 087's
five seats died on a watchdog rebuilding a compiler that had moved**. Naming the
behaviour rather than the files is what the parenthesis always meant.

**And dropping the braces was not cosmetic when the rule was written**: the brace
form was one of the two citation shapes the dead-citation check could not read
(panel 086 R7's measured limit). **That limit was closed on 2026-09-06** by
`expanded` in `tests/harness/suite_records.hero`, which reads the brace form
exactly and requires every expansion to resolve, so the sentence in §4 that
described the limit as live outlived it by a day. The one shape still invisible
is a line number, because a line number is not a path (CL-037).

## CL-027 — Panel briefs keep every command short and give one copy per judge
2026-08-23, undated in the contract · panels 087 and 088 · § 4

Four of five seats died on a 600 s watchdog rebuilding the compiler, and two
seats measuring in one shared checkout contaminated each other. The seed builds
in about 4 s and is usually enough; nothing longer than about 3 minutes in one
command; a frozen snapshot every seat copies from. And before convening, `grep`
design.md, `docs/panel/` and `DESIGN-LOG.md` for the answer that may already
exist, because panel 088 re-derived panel 037.

## CL-028 — Robustness reaches furthest at the C boundary, and a shut danger is still written down
2026-08-24 · author instruction; panel 089's spec-warden · `.claude/rules/c-boundary.md`

The C boundary is where the language's own guarantees stop (design.md §1.11:
everything comes from C, so everything a program touches arrives through §4.19).
The author's instruction is that the FFI be *complete and bug-proof*: complete,
because a library Heroes cannot bind is a library the author must leave C code
around for; bug-proof, because a binding is the one place where a Heroes program
can reach an address nobody checked.

Panel 053 measured the shape of that: `to_str` on a null `cstr` is a **clean
abort** (the runtime guards it), and a null `cstr` handed straight to another C
function **was a SEGV in libsystem**, so the hole was not where three of its four
options were looking. **That second half stopped being true and the sentence is
corrected rather than deleted** (author instruction 2026-08-24, measured twice
that day, by panel 089's spec-warden and by the coordinator): the same program is
now `panic: a null `cstr` was passed to a C function`, **exit 134**, because
`guard_cstr_arguments` wraps every `cstr` argument on its way out. The date is
kept because the measurement was right when it was taken; the closure is added
because **a contract that states a danger the compiler has already shut will fund
the wrong decision next time**. And it nearly did: panel 089's whole payment, the
15 tokens that deleted `spec:229`'s null-test advice, is design.md §1.4
redundancy **paid back** only because the compiler is loud in both directions.
Under the stale sentence that removal would have looked like deleting a warning.

## CL-029 — No em dashes and no machine-written patterns
2026-08-25 · author instruction · § 11

*"NO AI SLOP, get rid of all the em dashes"*. The rulebook is `site/README.md`
§ Style guide and `site/CLAUDE.md`; the chat half is that the assistant's Italian
replies carry no em dashes either. Two exceptions, both evidence: compiler output
shown verbatim, and code copied byte for byte.

## CL-030 — The flags citation was wrong for the third time, and the copy that replaced it rotted too
2026-08-25 · `.claude/rules/generated-c.md`

The live compile-flag list had been `selfhost/` for six days and the contract
still pointed at `archive/bootstrap-rs/heroes-cli/src/commands/flags.rs`, which
nothing builds. It was found by a panel judge with no `grep`, reading the
contract as a stranger would. **That was the third time that one sentence had
been wrong**, and the paragraph below it said *"the copy that used to stand here
is deleted rather than corrected a third time"*: the copy was deleted and its
**replacement citation** then rotted, which is the same failure one level up. A
pointer is a fact about the world and expires like any other (CL-004); the only
reason this one was cheap to fix is that somebody read it.

The count in prose died **3h16m** after it was last made true: `[&str; 11]` and
*"the eleven"* at 00:31 on 2026-08-15, `flags.rs` created at 02:24 *so that a
list two documents claimed to state would be easy to find*, panel 058's twelfth
flag at 03:47, and the contract still saying *eleven* the next day. A count in
prose is the premise CL-004 says expires in silence while the sentence around it
goes on reading as correct. Measured 2026-09-07: `selfhost/cli/flags.hero`
defines **fourteen**, pinned by its own test, and the contract states none of
them.

## CL-031 — The work lists got their true names, and the rule four documents stated got an executor
2026-08-26 · author instruction · § 3

The move-to-the-record rule was already written in the contract, in
`DESIGN-LOG:240` and in the record's own preamble, and **no skill performed it**:
`/decide` ticked an item and left it in place. So the record went untouched for
eight days while `DECIDE.md` grew into 391 KB holding **138 ticked items and zero
open ones**, under eighteen headings still titled `## Open`. Measured across the
four lists that day: **71.8% of 671 KB was closed work and 2.9% was live.**

Three things follow and each is now a rule rather than a habit. The record is
called `DONE.md` because it holds finished work: its old name claimed the
opposite, and the reason given for keeping it (*"every commit subject cites that
path"*) was **measured false, 24 subjects of 792, 3.0%, over the last 463 commits
before the rename**. `LEARN.md` left `docs/work/` entirely, because comprehension
is not work owed: it blocks nothing and is opened only when the author asks. And
the directory is `docs/work/` rather than the debrief directory it replaces,
which was named after a skill deleted on 2026-08-12.

That old path is deliberately not spelled here as a citation: the dead-citation
check in `tests/harness/suite_records.hero` reads every path in a prose line, and
a path that no longer exists is exactly what it exists to catch.

## CL-032 — One notation, and only one
Date not recorded in the contract · § 3

`- [ ]` and `- [x]`. A finding written as a bare bullet is invisible to every
count in this project, which is how nine live FFI findings sat in a second,
uncounted notation under `DECIDE.md`'s panel 051 and 052 headings, two of them
measurably stale, while the file reported itself empty.

## CL-033 — The repository moved, and the old name is never reused
2026-08-30 · `.claude/rules/records.md`

The repo pushes to `origin`, github.com/heroes-lang/heroes, moved there from
github.com/giuseppearici/heroes-lang on 2026-08-30. Every old link redirects,
and the one act that would kill those redirects permanently is creating a
repository at the old name, so that name is never reused.

## CL-034 — Never let three minutes pass without telling the author where you are
2026-08-31 · author instruction, categorical · § 3

Given after a day of long silent waits: *"never let more than three minutes pass
without updating me in keywords"*. This binds EVERY working session, not only a
loop: whatever is happening, a measurement running, a CI leg in flight, a remote
machine building, a diagnosis mid-thought, the author hears it in plain Italian
keywords before three minutes are up: what is running, what is being waited on,
what was just found. A wait longer than that is split into short probes with a
one-line report between them, never one long blocking call. The recap is the
point: an unattended stretch is a stretch of decisions the author could not
redirect, and the author was reduced to shouting in caps to get a status line.
That is this rule's origin and its reason.

## CL-035 — A measured number never goes inside an image
2026-08-31 · author instruction, on the README banner · § 11

*"those age fast"*. Artwork carries identity and structure; the text beside it
carries the numbers, where `grep` and a re-count can reach them.

## CL-036 — A new surface form lands in every tool that reads the language, and the formatter is the one that lies quietest
2026-09-02 · author instruction · `.claude/rules/diagnostics-and-goldens.md`

Given after `as` shipped into six consumers one at a time and every miss was
found by an instrument rather than by the assistant. A form is not landed when
the parser accepts it. The tools that re-write or re-print a program each hold
their own copy of what the language is, and one that has not learned the new
form does not error, it **drops it**. Measured the day the rule was written:
`heroes fmt` printed a `use` line as `"use " + name`, so `fmt --in-place` deleted
`as near_scale` from a working program and the next build could not find the
module; `--dump-ast` did the same thing an hour later, and the token dump, one
layer down, was fine all along. **design.md §4.15 is what makes the formatter the
worst of them**: the canonical form exists so that *"any textual difference
between two versions is SEMANTIC"*, so a formatter that quietly normalises a form
away is not a bug in a tool, it is the one instrument in this repository whose
failure makes every diff untrustworthy.

**AND THE GUARD THAT WATCHES THE FORMATTER WAS BLIND IN THE SAME PLACE**, which
is the finding worth more than the rule above it. `heroes fmt` already refuses
its own output when that output *"holds a different tree"*, and it compares the
two trees by **dumping** them, with the very printer that had also not learned
`as`. Both renderings dropped the word, so they agreed, and the guard reported
the same tree while the formatter was deleting one. One omission, two consumers,
and the second was the instrument watching the first. **A self-check that
compares two RENDERINGS can only see what the renderer carries**, so the printer
is not one item on the list among five: it is the item the list's own enforcement
rests on. `selfhost/cli/syntax_cmds.hero` now hands that guard the exact pair the
defect produced and asserts it refuses, which fails on the day the dump goes
quiet again.

## CL-037 — A line number is the one citation shape no instrument can see
2026-09-02 · `.claude/rules/cli-surface.md`

`heroes add` and `heroes fetch` are subcommands when they arrive, and the
sentence saying so cited `design.md:637` until 2026-09-02, when a panel 099
judge checked it and found the control-flow-as-blocks bullet there instead. It
is `design.md:772` now. This is CL-004's expiring premise in the one shape the
dead-citation instrument cannot read: `without_position` in
`tests/harness/suite_records.hero` discards everything from the first colon
followed by a digit, so the path is checked and the line number never is.

## CL-038 — Every progress update carries a percentage and its breakdown
2026-09-02 · author instruction · § 3

*"when you update me, write what % you are at as well"*. The number is counted
against the author's ask, not against invented work, and comes with the parts
that produce it, done, in flight, not started, so the author can disagree with
it.

## CL-039 — There are three suites, and the third one tests the tests
2026-09-02 · § Verification

The contract's command block named two until 2026-09-02, when the third was
found red at `d08062f` and had been red for six commits:
`tests/harness/suite_layout.hero`'s ceiling assert reading 308 against a table
that said 315, after `9599d97` re-baselined it and stated both new numbers in
its own diff two screens above the assert. `DESIGN-LOG` recorded the identical
failure on 2026-08-31, *"a suite that had never been run"*, and left it with no
caller: the command lived in `docs/ROADMAP.md`'s verify block, which nothing
obliges anybody to open. It is in the step skill as well now. **The third suite
is the one that goes red when an instrument's pinned number stops matching what
the instrument reads.** The other two test the language; this one tests the
tests.

## CL-040 — The panel adopts the most robust and complete resolution, never the cheapest and never a compromise
2026-09-03 · author instruction, given mid-sitting at panel 106 · § 4

*"the panel always takes the most robust and safest resolution, never the
cheapest and never a compromise"*. It is the same instruction as the `/decide`
one of the same day, *"choose the most robust and complete solutions over the
cheaper ones"*, applied to the panel.

**This line used to read *the most conservative resolution*, and the two are not
the same thing.** Panel 106 is where the difference showed: both seats vetoed the
proposal, so the conservative resolution was *do nothing* and the ceilings would
have stayed where they were, while the sitting's own measurements had found a
repair worth **58.5% of the frame** sitting next to the question it was asked.
Conservative means *change least*; robust means *leave the fewest ways to be
wrong*. When they disagree the synthesis takes robust, says so, and records what
the conservative resolution would have been so the author can choose it. What
does **not** change: the seats still veto on soundness, a veto is still a refusal
rather than a price, and the author still ratifies.

**Measured 2026-09-07, four days later, the struck word was still live in the
skills**: `.claude/skills/panel/SKILL.md` carried *conservative* in its
description and in its synthesis step, and `.claude/skills/step/SKILL.md` in the
line that invokes a sitting, so the skill that performs the panel contradicted
the section that governs it. That is why the skills now cite § 4 instead of
restating it.

## CL-041 — Never `git add -A`, and a commit stages only what this conversation touched
2026-09-03 · author instruction, categorical · § Hard stops

Never `git add -A`, `git add .`, `git add -u` or `git commit -a`. A commit stages
ONLY the files this conversation touched, each one named on the command line.

The reason is the working tree, not tidiness: more than one session works in the
same checkout at the same time, and their half-finished files sit side by side in
one `git status`. A sweep stages everything it finds, so it commits another
session's unfinished work under this session's subject, a change nobody in this
conversation read, tested or meant to record, and one that the other session then
finds gone from its tree. Measured the day the rule was written, while it was
being written: `git status` showed **8 dirty paths, 6 modified and 2 untracked,
all under `selfhost/` and `tests/harness/`, and this conversation had touched
none of them**. A `git add -A` there would have shipped eight files of somebody
else's compiler work inside a one-paragraph edit to the contract.

The same reasoning forbids `git stash` in the shared tree: it takes every
session's changes, not one's. Before staging, `git status` is read and every path
about to be added is one this conversation edited or created; a path that is
dirty and unexplained stays out of the commit and is reported to the author
instead.

**It was prose and nothing performed it until 2026-09-07.** Measured that day,
`.claude/settings.local.json` on this machine allow-listed `Bash(git add *)`,
`Bash(git commit *)` and `Bash(git push *)`, so all three of the categorical
refusals were pre-approved, and there were no hooks at all. The rule now has an
executor in `.claude/settings.json` and `.claude/hooks/`.

## CL-042 — Committing and tagging is routine; pushing main is asked for
2026-09-03 · author instruction · § Hard stops

The site lives on the same branch, `site/public/CNAME` names `heroes-lang.org`,
so a push of `main` can publish it and carries every other session's site commits
with it. Commit and tag locally, then ask, saying how many site commits would go
along; on a yes, `git push --follow-tags`. Hard stops that remain: publishing the
site or anything else outward-facing, and destructive ops.

**And the milestone-close checklist contradicted this until 2026-09-07.** Both
§14 and the step skill listed *a tag, pushed `--follow-tags`* as a routine
closing item, and `git push --follow-tags` pushes the current branch as well as
its tags. A session following the checklist to the letter published the site
without asking. The checklist now tags locally and leaves the push to the next
authorised one.

## CL-043 — Nothing goes into the assistant's own memory
2026-09-03, the second time it was given · author instruction · § Hard stops

*"I want nothing in memory and everything in the Claude MD. I will change
computer and then I will lose the memory"*. The assistant's memory directory
lives on one machine and dies with it; the contract travels with the repository.
An instruction the author gives about how to work is written in the contract,
under the section it amends, with its date and its words, and never into a memory
file. The notes that had accumulated there were moved into the contract the
evening the rule was repeated, and the memory index now holds one line pointing
at it.

## CL-044 — A fourth list, for what is broken
2026-09-03 · author instruction · § 3

*"I do not like the defect directory ... if anything is still open in defect at
the end, make one single file called DEFECTS.md inside work, so that everything
is tidy"*. A compiler defect, a crash, a wrong answer at exit 0, a silence where
a message is owed, is an open item in `docs/work/DEFECTS.md`, with its
reproducer, its measured cause and what is owed, for as long as it is open; the
moment it is repaired the entry gains its *The repair* section and moves to
`docs/work/DONE.md` like everything else. The directory that held one file per
defect is gone: its seven files became six record entries and one open item the
evening the rule was given, and every live citation followed them.

**And this list had no reader until 2026-09-07.** Measured that day, no skill
mentioned it: the step skill filed findings into three lists, and `/where`
gathered two, so an open defect was invisible in the author's own status report.
That is CL-031's story, *a rule with no executor*, inside the section that tells
it.

## CL-045 — When the author is following live, the update comes every two minutes and in full
2026-09-03 · author instruction, given while the seven defects were being attacked · § 3

*"I want to be updated with many words and with plenty of detail, never in plain
words, every two minutes, because I am very interested in this thing"*. The
three-minute rule (CL-034) is the floor for an unattended stretch; an author who
has said they are interested gets the whole picture every two minutes, which
function, which number, which file, and the reasoning that joins them, never a
keyword line. *"Never in plain words"* there means never in FEW words; it does
not repeal the plain register (CL-014), which is about naming the thing before
the term for it. Rich and plain are compatible; terse and plain were being
confused, and the author noticed.

## CL-046 — A long compile is when the open decisions are proposed
2026-09-03 · author instruction · § 3

*"I like this idea that while you wait for the result of a compilation you put
the decide items to me"*. While a build, a suite or a measurement runs for
minutes, the open items of `docs/work/DECIDE.md` are verified against the
repository and put to the author, each with a recommendation. A silent wait
wastes the author's time twice, and deciding costs no CPU, so it distorts no
measurement in flight.

## CL-047 — Re-read the chain and the log immediately before writing a scheduling fact
2026-09-03 · `.claude/rules/records.md`

Never from the copy read at session start. On 2026-09-03 a peer session closed
M-robustness-guards and opened the next milestone while a plan was being written,
and a 22-anchor edit to `docs/ROADMAP.md` failed twice on sentences the peer had
rewritten in between. For a multi-anchor edit to a shared record: script the
pairs, assert each anchor matches exactly once, dry-run, then apply.

## CL-048 — The three platforms are measured from this Mac, before the commit
2026-09-03 · author instruction · § Verification

Given after the program that is now `examples/ctime/` (it was called filestat
until that day, and the directory of that name is gone) shipped a comment that
named three platforms and had checked one, and the Windows leg of CI was the
instrument that read it. macOS is this machine. Windows is a real box,
`docs/environment/windows/WINDOWS-MACHINE.md`. Linux is a container of the CI
leg's own architecture, `docs/environment/linux/LINUX-MACHINE.md`, built from the
`Dockerfile` beside it, one directory per platform, by author instruction the
same day. Neither file was cited anywhere alive until the paragraph that named
them: the Windows one sat orphaned for three days and a memory pointing at it
carried a dead path. **CI stays the judge; these two are the hunt**, and a
platform fact that has not been run on one of them is an inference (CL-018),
whatever the comment around it says.

## CL-049 — Shipping uncommitted files to the Windows box
2026-09-03 · § Verification

`COPYFILE_DISABLE=1 tar --no-xattrs`. macOS's tar writes `._name` AppleDouble
entries that the harness globs as programs (172 of them on 2026-09-03, seven
false failures). Committed work goes by `git push win main:main`. The box is
`docs/environment/windows/WINDOWS-MACHINE.md`: paid by the hour, usually off, and
only the author can start it, so ask, and do the machine-free work meanwhile.

## CL-050 — Measuring in a copy of the tree
Date not recorded in the contract · § Verification

A copy without `.git` cannot run the net's `records` suite, because it reads
commits and tags. Measure single suites in a copy; gate a commit on the real
tree's net.

## CL-051 — A quoted author instruction is written down in English
2026-09-04 · author instruction · § 11

*"I do not want anything in Italian ... I speak Italian to you because it is
comfortable for me, but the texts are all in English, since this is a project
that will become open source"*. Every rule in the contract is anchored to the
author's own words, because a quotation binds where a summary does not, and those
words arrive in Italian. Until this instruction they were written down as spoken,
on a doctrine `docs/book/README.md` states in one line, *quoted speech is not an
artifact*, which the reasoning notes' README then cited as precedent for its own
carve-out. **Measured 2026-09-04, that loophole had let 241 Italian quotations
into the repository, 19 of them in the contract**, in the section whose first line
says everything written is English.

The doctrine is retired in all three places: quoted speech is an artifact like
any other. What is quoted is what the author **meant**, in English, rather than a
literal gloss of the Italian, because an instruction is a ruling, not a specimen.
Nothing is lost: the original stands in the git history and in the dated records,
which is where evidence belongs and which §14 forbids anybody to rewrite. **The
records are therefore not translated**, and they hold about 210 of those 241; 40
commit bodies carry the same words and cannot be edited at all, so a translated
record would disagree with the commit that made it.

## CL-052 — The translation exceptions are a class rather than a list
2026-09-04 · § 11

Everything in CL-051 governs the repository's **working** text: the rules, the
records, the code. A text written for a reader in that reader's own language is a
product decision, and it is named so the rule cannot be read as refusing one.

- **The two books**, M-journey-book and M-guide-book (author instruction
  2026-08-11), in **Italian and English**, neither a machine translation of the
  other. The author studies from the Italian, so where the two diverge the
  Italian is fixed to be clearer rather than the English to be more faithful, and
  both are in the plain register of the `/where` skill, which assumes zero
  compiler knowledge.
- **The Italian edition of the site**, born 2026-08-18 by author instruction
  (quoted with its date in `site/README.md` § The Italian edition): **23 pages
  under `site/src/html/it/` and their 23 wrappers under `site/src/pages/it/`,
  measured 2026-09-04 and re-counted 2026-09-07**. Three things serve that
  edition and stay Italian because of it, each one Italian *data* rather than
  Italian prose: `site/CLAUDE.md`'s spelling rule, whose subject is the words
  themselves; `site/src/components/SiteNav.astro`'s table of nav labels; and the
  vocabulary `/where` speaks from, which is why that vocabulary lives in a file of
  its own with an English header saying what it is.

**The section said *"one declared exception"* until 2026-09-04, and the second one
had been 46 tracked files for 17 days**, which is CL-004's expiring premise inside
the section that states it, about that section. The exceptions are a class now
precisely so that the third one does not have to come back to be counted.

## CL-053 — A reasoning session leaves no note of its own
2026-09-04 · author instruction · `.claude/rules/records.md`

A conversation whose work is questions about the project, with no file of code,
spec or design modified, writes no note. It had a directory of notes from
2026-08-10 until 2026-09-04, when the author retired it after seven (*"I want to
remove it and turn its content into DONE, or DECIDE plus a panel where needed, or
even into ROADMAP milestones where needed. All this work goes in the direction of
simplifying the docs directory"*).

Where the parts go is where they went anyway: what the session **settled** is an
entry in `docs/work/DONE.md`, what it **left open** is an item in
`docs/work/DECIDE.md` if it asks what should be true or in
`docs/work/SCHEDULED.md` at an open milestone if it is work, a **concept** it
explained is an entry in `docs/glossary/`, a **question worth re-asking** is a
line in `docs/learn/LEARN.md`, a **change to the language** is a panel, and a
**decision taken** is a DESIGN-LOG line. What the directory alone had held was the
path between those, and the path is the git history. Measured on the day it went:
**31 of its 32 hand-offs had already landed** in the artifacts above, and the last
live thing it carried, a table two documents cited by line, moved into the sitting
that cited it.

## CL-054 — The suite you did not expect to move is the one worth running, and the moment to run it is after the last edit
2026-09-04 · author instruction · § Verification

CL-039 says which suite went red and left the lesson *run the third suite*, and
that lesson is too small: it names the suite that was missed last time. On
2026-09-04 two sessions worked this tree at once and **both missed a different
one, in the same afternoon, for the same reason**, because each ran what it
expected its own change to touch.

- One rewrote prose across fifteen files and ran the third suite plus `records`,
  `layout`, `lines` and `units`. It did not run `canonical`, on the reasoning that
  prose cannot break a formatter. `tests/harness/suite_records.hero` had one blank
  line the formatter removes, and **the net was red for everybody**: design.md
  §4.15's whole argument is that a textual difference is semantic, so a
  non-canonical file *inside the harness* means the instrument for that rule is
  not held to it.
- The other landed a large repair and ran the whole net, which is what found that
  failure, and its own `tests/harness/suite_layout.hero` pin, `DECIDED` grown to
  17 entries with the assert still reading 16. **An addition is the direction that
  assert notices least loudly**: a removal takes the table below the number and a
  raise leaves it alone.
- And an edit *after* a green run is the same fault wearing a clock. `records` was
  green fifteen minutes before an item left `docs/work/DECIDE.md`, and the recap
  was already written. The move made `records/verdicts` red, correctly:
  `docs/panel/106-the-frame-is-the-sweeps-own-temporaries.md` still said its
  ratification was queued, and nothing named it any more.

The rule is therefore not a list of suites. **A ticked item is a question, not a
task**: the third bullet is what the check caught, because `DECIDE.md` reaching
zero open items reads as tidiness and was the symptom, the item being the author's
ratification standing in for the author, and the work being built is not the work
being ratified. And *"my change cannot have touched that"* is an inference
(CL-018), so it is either run or it is written down as a guess.

## CL-055 — A program that declares an extern runs its Linux leg under sanitize
2026-09-04 · author instruction; panel 108 named the instrument · § Verification

*"write the sentence yourself, then convene the sitting"*, and the narrow form
was recommended and taken. LeakSanitizer exists on that leg and on no other, so a
leak in a C binding is invisible on this Mac in all three configurations.
Measured 2026-09-04, when `examples/ledger/` leaked 40 bytes per refused
statement: green here, red in CI, and the only instrument that could see it was
that one. It is narrow on purpose: a program without an `extern` cannot leak from
the C side, because its own allocations are counted by
`hero_runtime_check_leaks()` on every platform, so the leak the Mac cannot see
enters through an `extern` and nowhere else. The wide form would cost 44 programs
a run that concerns 8 and catch nothing more.

**Sharpened 2026-09-10, underneath rather than in place: this names the leg and
the flag and not the command, and on that leg two commands disagree.** The
invocation the gate uses is the suite's, `heroes run <case> --sanitize`
(`tests/harness/suite_run.hero`), which compiles at `-O2`, and on 2026-09-10 it
reported **29 bytes** from `hero_str_held` in CI on a case the session that
wrote it had reported silent when built `--sanitize` by hand and run, a
transcript no log corroborates. `heroes build` defaults to `-O0`, where the
lease cell is still a live local of `main` at `exit`, so LeakSanitizer finds a
pointer and calls the block reachable, which is a reading of the tool rather
than a measurement of it. So a session discharging this rule literally can read
a clean hand-built `--sanitize` run as the leg's verdict, which is CL-074. The
two live copies of the sentence, `.claude/rules/platforms.md` and
`.claude/rules/c-boundary.md`, are bound by the same line: the leg's verdict is
the suite's own command.

## CL-056 — A background monitor is never asked about; the answer is always yes
2026-09-04 · author instruction · § 3

*"put it in Claude's settings never to ask me again whether I want the monitor:
the answer is always yes"*. A monitor is how a CI run, a remote box coming up or a
long build wakes the session instead of being polled, and every prompt for one is
a wait the author has to click through. The permission lives in
`.claude/settings.json`, checked in so it travels with the repository rather than
with one machine.

## CL-057 — And the list is a measurement too: when you enumerate, say where the enumeration came from
2026-09-05 · author instruction · § Run it, or say it is unrun

*"write down somewhere what you have learnt, so you do not make the same mistake
again"*, given after a day that made the same one five times. Everything in
CL-017 and CL-018 is written about individual claims, and every one of those rules
was **obeyed** on 2026-09-05 while the day's real errors went straight past them.
The reasoning was sound each time. **The LIST it reasoned over was mine, and it
was short.** That is a distinct failure, and it is invisible in a way a wrong
number is not: a wrong number contradicts something, while a missing option
contradicts nothing at all, and nothing in a correct argument points at the row
you did not write down.

Five in one day, each measured by somebody else:

- A panel brief named **three causes** for why callbacks do not bind. A seat
  refused the sample and read the real `sqlite3.h`: **five causes over 106
  signatures**, and the one the sitting was convened about was **fourth, at
  2.6%**. The whole sitting was aimed at the wrong noun.
- That brief's *"`const` buys 3 of the 8"* was **2**, wrong in the convener's own
  favour, because `nftw` is blocked by a `struct FTW *` that is not `const` at
  all.
- The same brief's four options **omitted the adapter**, which is what the two
  languages closest to this one's architecture actually ship. The historian
  objected to the **list**, not to the options on it.
- *"CI is green on all three platforms"* was inferred from the word `success`. It
  was **one leg**; the matrix widens only on an `m-*` tag, which
  `.github/workflows/ci.yml` says in its own `on:` block.
- A defect was filed **blocked**, with two routes priced and a written
  recommendation to repair neither. Both routes were refusals. **A third existed
  in the same file, three lines above**, and it cost nothing: the author asked for
  the repair anyway and it took ten minutes.

**Three obligations, and each one kills a specific error above.** **Enumerate from
the world, not from what you can think of**, the real header, the whole corpus,
the actual job list, `gh run view --json jobs`; a list built from memory is a
guess wearing a table's clothes, and the fix is usually one command. **Say where
the list came from, wherever it is handed on**, in a brief, a recommendation, a
`/decide` item; *"the eight I could think of"* and *"the 106 in the header"* are
different objects and the reader cannot tell them apart unless it is written. And
**a recommendation is a claim about the option SET, not only about the options**
(CL-024 at its weak point): before recommending, ask what would have to be true
for a route nobody listed to exist, and if the answer is *somebody would have to
look somewhere I have not looked*, look there first.

The cost of the rule is one command per list. The cost of breaking it was a
five-judge sitting convened on 2.6% of its subject, and a defect that would have
stayed open under a recommendation that was correct about everything it named.

## CL-058 — And the record says whose idea it was; where it was the author's, it says so
2026-09-05 · author instruction · `.claude/rules/records.md`

*"in general I would like it to stay in the records when I have an intuition, so
that when this becomes public it does not read as a merely vibe-coded project but
as one the author co-authored"*. This is not courtesy and it is not decoration.
Every other rule in the contract exists to make the record **checkable**, and a
record that attributes to the assistant a finding that came from the author is
false in exactly the way those rules forbid. It just happens to be false about a
person instead of about a number.

**What provoked it, the same day.** The thread guard was measured at +6.6% and the
coordinator was treating that as one cost to haggle over. The author asked a
different question, *"if the cost is only at compile time it is not a big problem;
if it is at runtime, then yes, let us look for the compromise"*, and answering it
meant saying **where** the cost is paid, which meant asking where a foreign thread
**enters** rather than where the corruption shows. That question moved the guard
from three wrong placements to the right one, and `docs/measurements/018`'s first
draft recorded the answer with no author in it. The attribution went in only
because the author asked for it.

**It cuts both ways, and that is what keeps it honest.** Crediting the author for
something the assistant found would be the same falsehood wearing the other sign,
and it would be worse, because it is the flattering direction and nobody would
check it. The rule is accuracy: name the author where a question, a correction or
a refusal of theirs is what produced the finding, name the panel seat where a seat
found it, and name nobody where the work was ordinary. A record nobody can trust
about people is a record nobody will trust about numbers.

## CL-059 — Use the familiar second person
2026-09-05 · author instruction · § 11

*"and use tu with me"*, given in one parenthesis while answering two other
questions. Italian's formal *lei* had been the default for a month and nobody had
asked for it; the familiar form is the register of somebody working beside you
rather than reporting to you, which is what this project is.

## CL-060 — Comprehension questions are never put through the question widget
Date not recorded in the contract · author instruction · § 3

*"you have to show me the line, otherwise I cannot know"*. The snippet goes in a
fenced block, the options lettered, the whole batch in one message, answered in
one reply (`1b 2a 3c`). The widget hides the code the question is about.

## CL-061 — Run it, or say it is unrun
2026-09-06 · author instruction · § Run it, or say it is unrun

*"put this measuring thing into the rules; there should already be something, but
make it more prominent"*. There already was something, and that was the problem:
the rule's case law had grown to **136 lines** inside the section it opened
(counted, not eyeballed, and the first draft of that sentence said *two
hundred*). A rule a reader meets after 136 lines of its own exceptions is a rule
they meet tired, so the rule moved above them. On 2026-09-07 the same reasoning
moved the 136 lines out of the contract entirely, into this file.

**What earned it, and it is the reason the rule is about RUNNING and not about
thinking harder.** Three claims were corrected on the day it was written, and
**not one of them fell to a better argument**:

- A panel seat was told the morning's repair had closed a hole. It **deleted one
  line from the emitted C and rebuilt** against the same runtime: exit 132, empty
  stderr. The hole was still there and a different milestone had closed it.
- A second seat was asked whether a sentence about what a program prints could
  enter the spec. It ran **`strip`** on the binary; the function name was gone, so
  the sentence would have been false on the day it landed.
- A peer session was sent a claim about a platform. It **opened the file**, and
  the claim was an inference wearing a *so*.

**And one shape CL-018 did not have: a repair that widens a rule to cover a class
can contradict a ruling the record already made about one member of it.** *A `(`
after a foreign word means the word is called* was true of the case that provoked
it and false of an adversarial case the author had ratified on 2026-08-04, where
`(` opened a function TYPE. The net caught it by quoting that case back. So before
generalising, grep `docs/panel/`, `DESIGN-LOG.md` and `tests/golden/` for the
members: **a class is not a class until its exceptions have been looked for.**

## CL-062 — And reading is not measuring: what was seen on a screen is not what was counted or opened
2026-09-06 · author instruction, *"write this in CLAUDE.md"* · § Run it, or say it is unrun

Everything above is about facts taken from memory or from a document. These two
came from **looking at the right output and not finishing the job**, which is a
cheaper mistake to make and just as false on the page. Both were caught, in the
same session, only because somebody went back and ran the thing.

- *"Twenty-six lines of code"*, written into a commit message about a runtime
  change. The number came from **eyeballing a filtered diff** that happened to
  print about forty lines. Counted, with one `git diff` and a filter that drops
  comments and blanks, it is **33 added and 28 removed**. A count that a command
  can produce is not estimated from the shape of a screenful; CL-017's rule about
  running the measurement has no exception for a number that looks countable.
- *"Reached from `hero_f64_render` and from the parse side"*, written into three
  documents. The grep had shown two call sites and the second was **assumed** to
  be the parse half; opening it, it is `hero_f32_render`, and the `strtod` inside
  both is a round-trip check rather than a program-facing parse. **Two call sites
  is a count, not an identity.** A name goes into a sentence after the function is
  opened, not after the line number is seen.

The shape they share is the tell: a command was run, its output was read, and the
**last step**, count it or open it, was skipped because the answer felt already in
view. The rule is to finish the command, and where the sentence names a thing
rather than a number, to open the thing.

## CL-063 — The full net is run once before a push, and the named suites gate a sub-step
2026-09-06 · author instruction · § Verification

*"this step is endless, how can we be faster, maybe we do not check the whole net
at every sub-step, but only at the end"*. CL-054 says to run the suite you did not
expect to move, after the last edit rather than after the last interesting one,
and that rule is unchanged. What changes is **which** run carries it at which
moment, and the change is paid for by a count of the day it was asked.

**Measured over one session, 2026-09-06, M-declared-freer steps 1 to 4a**: the
full net ran **eight** times to completion at **11 to 15 minutes each**, close to
**two hours**. Four runs were green. Four found something, and **all four failures
were in suites that cost seconds**: `canonical` twice (a file written and not
passed through `heroes fmt`) and `emission` twice (a blessed capture that had
legitimately moved). **Not one came from the parts that cost the minutes**, the
corpus, the `run/` goldens, `mutate`, the three configurations. Those were green
in all eight.

So a sub-step is gated by **the named suites, one at a time**, plus the compiler's
own tests and the net's own tests. Together that is under a minute against
thirteen, and on the day this was measured it would have caught **four of four**.
The full net runs **once, before a push**, which is where the outward-facing hard
stop already makes somebody stop and look.

**And the cheapest saving is not in this rule at all**: two of the four failures
were code written and not formatted, which `heroes fmt <file> --in-place` costs
nothing to prevent and a 13-minute run to discover. **Format at the moment of
writing, not at the moment of verifying.** A rule about which suite to run is
worth less than the habit that stops the suite from firing.

**Measured 2026-09-07, no skill said either half.** The step skill still listed
all three suites at every step, and `heroes fmt` appeared in no skill at all, so
the amendment that bought back an hour a session was a rule performed by nothing.
The contract's § Verification is its one home now, and a `PostToolUse` hook does
the formatting half.

## CL-064 — A number that lives in two places drifts in the one nobody re-reads
2026-09-06 · § Verification

The contract's command block said 108 for the third suite while `docs/ROADMAP.md`
said 112, because M-isolated-threads' close re-measured the ROADMAP and not the
contract.

**IT HAPPENED AGAIN THE SAME DAY, AND THIS TIME BOTH COPIES DRIFTED TOGETHER**
(M-declared-freer step 1, 2026-09-06). `e61fec3b` repaired defect 015, added
harness rows and moved two counts, **583 to 584** and **1578 to 1582**, and **its
own commit body states both**, in the sentence *"Three suites green: the
compiler's own 584, the net 1582, the net's own tests 113"*. Neither the contract
nor the ROADMAP was touched, so the repair that knew the new numbers shipped
beside two documents still printing the old ones. The lesson above says a number
drifts in the copy nobody re-reads; the sharper one is that a commit which
MEASURES a number and writes it in prose is exactly the commit that can afford to
put it where the number lives, and the ten seconds it costs is the whole
difference between a record and a rumour.

**Measured 2026-09-07, it was true a third time and in the same file both ways**:
`docs/ROADMAP.md` stated the three counts correctly in its status paragraph and
still printed **108** for the third suite eleven lines below, in the *Verify it
yourself* block, which is the one a stranger runs. The contract carries no suite
counts at all now; `docs/ROADMAP.md` § Where we are is their one home.

## CL-065 — The Windows seed line and the CI leg that builds both
2026-09-06 · § Commands

On Windows the build line is `seed/README.md`'s, which adds
`-Wl,/STACK:67108864` and states why; the contract does not repeat the flag,
because each rule is written in exactly one place. **Since M-thread-stacks the CI
leg builds BOTH**, the flagged seed it then uses and the plain line, which it
asserts still compiles `selfhost/lexer.hero`, so the two cannot drift in silence
again. That step asserts rather than reports, deliberately:
`docs/work/SCHEDULED.md` recommended a reporting step, and a step that only
reports is a rule performed by nothing, which is CL-031's story told a third time.
**Measured on the box 2026-09-06**: the plain line's 1 MB now builds a compiler
that emits the whole of `selfhost/main.hero`, so the flag is headroom rather than
the thing holding the build up, which is exactly the state in which a silent
divergence goes unnoticed.

## CL-066 — The work lists get one shape, and the rule four documents stated gets an executor
2026-09-07 · author instruction, given in four parts over one session · § 3

The lists are to be more synthetic and regular, only the items with their state
and their description, no surrounding prose; a closed item does not belong in
`DECIDE.md`, because it is already in `DONE.md`; and one loud line marking where
the items begin, so that a glance says whether anything is there (*"fix `DONE`
too"*, *"fix `ROADMAP` too"*, *"format `LEARN.md` too"*).

The shape: **one line of three fields per item**,
`- [ ] **<first field>** | <what, in one line> | <where to look>`, then an
optional **body indented four spaces** opening with `**Origin:**` and its date,
and the whole item region **fenced between two lines of asterisks**, one only in
`DONE.md`, which is appended to forever, so a closing fence there would be a rule
the next append breaks. The item lists carry `**OPEN: N**` under the opening
banner, and in `docs/learn/LEARN.md` that number is the open questions alone,
because a ticked question STAYS there (the list is also its own record).
`docs/ROADMAP.md` takes the banner and not the item shape, being a chain and a
section per milestone rather than a list, and what it lost is fifty lines of
preamble that were the history of its own three reorganisations, in front of its
first line of status. **The first field is what the file's instrument reads**, and
that is why it differs: the milestone in `SCHEDULED.md` (`homed`), `panel NNN` in
`DECIDE.md` (`queued`, which scans the item LINE, so a sitting cited in a body is
invisible to it), the defect number in `DEFECTS.md`, the origin in `LEARN.md`.

**What it cost to leave this to prose, measured 2026-09-07 while the shape was
being written.** `DECIDE.md` was **3466 bytes of which zero were items**, three of
its paragraphs narrating items it no longer held. `DEFECTS.md` carried one open
defect and **2753 bytes about five repaired ones**, every one of them already in
`DONE.md`. And `SCHEDULED.md`'s 41 items were single lines averaging **2092
characters** and reaching 5068, which is 96% of the file: the surrounding prose was
not the preamble, it was the item. `LEARN.md`'s 323 were 545. The three are now 176
characters per item line on average and `LEARN.md`'s are 370, with **no item's
content lost**, because the words moved into the body rather than out of the file,
and `LEARN.md`'s 323 were re-laid-out by a script that reassembled every item from
its parsed parts and demanded the original line back byte for byte.
**`tests/harness/suite_records.hero`'s `records/lists` is the executor**: no
`- [x]` in a work list, no `- [ ]` in the record, nothing outside the banners, and
the banner's count equal to the items counted. This rule was stated in four
documents and performed by none, which is CL-031's story told a third time; the
difference is that this time a check fails.

**And the reshaping was itself caught by an instrument, twice, which is the part
worth carrying.** Splitting a long item into a header and an indented body **moves
a date off the line it was covering**, and `records/citations` reads one line at a
time: seven dead-path citations in `SCHEDULED.md` and two in `LEARN.md` went red
the moment the wrapping changed, every one of them a path that had been legal only
because the item was a single line with a date somewhere on it. So when a body is
written, **a line that carries an archived or never-written path carries its date
too**. The `LEARN.md` script was made to predict its own damage before writing: it
reported a hundred, of which ninety were its own false positives, because a record
is citable by the prefix of its slug and a crude existence test does not know
that.

## CL-067 — Release tags are a third namespace, and a release is a commit, never a milestone
2026-09-07 · author decision, from the author's own question · `.claude/rules/records.md`

*"let us work out how to handle releases and what number to start from: GitHub
releases first and then the channels? and how do we number them? are we at 0.1,
0.9, 1.0, 1.1?"*. The measurements and the six choices, each put with a
recommendation and all six taken, are the `DESIGN-LOG.md` row of that date. The
milestone tags are `m-*` and widen the CI matrix; the site's are `site-v*`
(`.github/workflows/release-site.yml`); a release of the language is `vX.Y.Z`, and
nothing else starts with `v`.

- **The number is `X.Y.Z`, and `heroes --version` prints it.** It is one constant,
  `VERSION` in `selfhost/main.hero`, and that constant is also the first line of
  every emitted C file and the build cache's fingerprint. It moves **only in the
  commit that carries the tag** and stays there until the next release: between
  releases the binary says the last release and `main` is ahead of it. There is no
  `-dev` suffix, because every move of the number regenerates the seed one
  generation further than usual and rewrites the first line of every file under
  `tests/emission/` and of every `.expected` under `tests/golden/emit/` (213 and 6
  on the day this was written; the second store was found by the net, not by the
  grep, which had printed it into an output nobody read to the end), and paying
  that twice per release buys one word.
- **While `X` is 0, `Y` moves when the spec moved and `Z` when it did not.**
  `spec/heroes-spec.md` is the whole language, by its own first line, so *the
  language changed* means `git diff vA vB -- spec/heroes-spec.md` is not empty: a
  command, not a judgement. What 0.x promises is one sentence, and it is the whole
  promise until the gate: *before 1.0.0 the language may change between minor
  versions; a patch version changes no sentence of the spec.* `1.0.0` is
  M-publication-gate's, written together with the compatibility paragraph that
  entry owes, because in every language people install `1.0` means *your programs
  keep compiling* and nobody has written that promise for Heroes. The project's
  own word `v1` (design.md §1.0: the compiler compiles itself, reached
  2026-08-18) is a milestone's name and not this number, and the two are spelled
  apart on purpose wherever a stranger might read them side by side.
- **A release is the author's act, on a clean `main`, resting on the last closed
  milestone**, and it does not coincide with one: the chain closed 37 milestones in
  35 days. The tag is annotated, and its message is the one paragraph a human
  writes; the workflow assembles the rest.
- **The instrument is the CI, not this list.** A `v*` tag runs the three-platform
  matrix like an `m-*` tag, asserts on every leg that the seed-built compiler's
  `--version` is the tag's number, asserts that a spec that differs from the
  previous release moved `Y`, and only then creates the GitHub Release: the tag's
  own source archive, with the seed inside it where `seed/README.md` says a seed
  belongs, the notes, and **no uploaded binary** (`DESIGN-LOG.md:539` refused
  prebuilt binaries as a decoy without clang, and a release is not where that
  refusal expires). The notes are the annotated tag's message, the milestone tags
  between the two releases, and the spec's diff between them, which is the
  language's changelog because the spec is the language. No CHANGELOG file: it
  would be a second copy of the record.
- **The channels pin the tag** (M-install-channels): a formula, a manifest, a flake
  and an image name `vX.Y.Z`'s archive and its checksum and run the one clang
  line. The repository is private, so a release made today is the private rehearsal
  that entry asks for; every release made before the gate becomes visible on the
  day the gate lifts, so its notes are written for a stranger from the first one.

## CL-068 — The net number of 2026-09-07 is a dirty clock, said so rather than quoted clean
2026-09-07 · § Verification

13m05s was `real 785.39` against `user 349.86 + sys 100.27`, so **57% of the wall
was waiting**, and the reason was on the same machine: Chrome, the window server
and a chat client held about three of load throughout, because the author was
using the desktop. CL-025 says a run whose `real` sits far above `user` + `sys` is
discarded rather than explained; this one was kept because it is a PASS/FAIL gate
and not a measurement, every check was green, and the duration was written down as
the shape it is. **The number to fund a *do not make it slower* argument is not
this one**, and the next session that wants one takes it on a still machine.

## CL-069 — The contract is re-engineered, and it is the first thing it ever measured about itself
2026-09-07 · author instruction · the whole contract

*"study how to re-engineer CLAUDE.md to give it more order and priority,
following the best practices, and to make sure every context takes in its rules
and that there are no conflicting rules"*.

**Measured before the change.** `CLAUDE.md` was **1247 lines, 87136 bytes, 22689
tokens** by `heroes measure`, against **3871** for the whole language spec, and it
had grown from 118 lines on 2026-08-03 through 555 on 2026-09-01 and 1139 that
morning, over 90 commits. It carried **62** emphasised amendment paragraphs, **36**
quoted author instructions, **89** dates and **171** em dashes. Claude Code's own
documentation asks for **under 200 lines per file**, warns that *"longer files
consume more context and reduce adherence"*, and says that *"if you emphasize many
lines, none of them stands out"*.

**Twelve conflicts were measured, not guessed, and three were live hazards.** The
panel skill still adopted the *conservative* resolution four days after the author
struck that word (CL-040). The milestone-close checklist pushed the branch, and
with it the site, as a routine act (CL-042). The `llm-ergonomist` seat carried a
**3000**-token spec budget from 2026-08-04 against a ceiling of 4096 since
2026-08-10 and a spec measured at 3871, and it is the one seat forbidden to grep
for the true number. The rest, with their evidence, are in the DONE entry of this
date.

**Three things did not change, and one of them is a refusal.** The section numbers
`§1` to `§15` stay, because they are cited **3280** times across the repository, of
which **2599** are in append-only records and **346** in commit bodies, and no
instrument verifies a `§N`, so a renumbering would have made every one of those
pointers wrong in silence. Every rule survives; what moved is its story. And the
author's words stay, dated, in English, here.

**What the contract now costs**, measured after: the numbers are in the DONE entry
of 2026-09-07 and the ceiling is pinned in `tests/harness/suite_records.hero`, so
the next session that grows it past the ceiling finds out from a red check rather
than from a reader.

**Corrected the same day, underneath rather than in place, which is what this
file is for.** That sentence names the wrong suite: the ceiling landed as
`spec/contract` in `tests/harness/suite_spec.hero`, because that is where the
compiler is in hand to run `heroes measure` and where a document is already
weighed on the same scale. `tests/harness/suite_records.hero` gets `records/sections`
instead, which resolves the pointers INTO the contract. The sentence was written
before the two checks had homes and it was already stale when it was committed.

**And the architecture above rested on two documented unknowns, so both were
run.** The author's instruction was to make sure every context takes in its
rules, and the documentation answers two of three questions. Measured by asking a
subagent to report its own context after reading one file under `selfhost/emit/`:

- **Path-scoped rules DO reach a non-fork subagent, and the scoping is per
  FILE.** Of seven files in `.claude/rules/`, exactly the two whose `paths:`
  match that file had arrived. `c-boundary.md`, scoped `selfhost/emit/ffi*`, sits
  in the same directory and did not load. They arrive on the tool call rather
  than at session start, with the frontmatter stripped.
- **The contract reaches a subagent as a SESSION-START SNAPSHOT.** The agent held
  the 1139-line version from this session's opening commit while the disk held
  322, five commits later, and the proof does not rest on the line count:
  `.claude/rules/` did not exist at that commit, so the rules content cannot have
  come from that snapshot. **This strengthens the split rather than threatening
  it**: a tree-local rule is not only cheaper than a paragraph here, it is
  *fresher*. What it costs is that a judge convened after a contract edit reads
  the old contract, which is why `.claude/agents/llm-ergonomist.md` now tells
  that seat to ignore this file outright.
- **Hooks fire inside a subagent**, which was documented neither way and was
  answered for free: panel 117's ergonomist seat saved its experiment as `.txt`
  and said in its verdict that the formatting hook had told it to.




## CL-070 — A commit limits itself by pathspec, because naming the paths to `git add` limits nothing
2026-09-08 · found while breaking it · § Hard stops, and `.claude/rules/records.md`

CL-041 says a commit stages ONLY the files this conversation touched, each one
named on the command line, and gives the reason: more than one session works in
this checkout, and a sweep ships somebody else's unfinished work under this
session's subject. **The procedure it implies does not achieve what it asks.**
`git add <paths>` followed by a bare `git commit` commits the whole staging
area, so a parallel session's `git add` decides the contents of this session's
commit and no amount of care at the `add` step prevents it.

Measured the day it was written, by doing it: a commit that named **fourteen**
paths carried **sixteen**, the two extra being the site's landing pages, which a
parallel session had staged while this one was editing the same files. The
commit body stated, in a sentence written before the commit ran, that those two
pages were *not* in it. They were, and that is a false record under §12, which
is the more expensive half: the sweep itself lost nothing, since the author was
driving both sessions, but the log said something untrue about itself and only
a `git show --stat` afterwards caught it.

**The form that actually limits is `git commit -- <paths>`**, which ignores the
index and commits exactly the paths given. CL-041's `git status` step stands and
is not sufficient on its own: reading the tree tells you what is dirty, and only
the pathspec keeps it out.

The general shape, and it is why this is its own entry rather than a line under
CL-041: **a rule and the command that is supposed to enforce it are two
different things, and the gap between them is invisible while the situation the
rule was written for does not occur.** This one sat unnoticed for five days
because no parallel session had staged anything in that window.

**Giving the rule an executor found a second defect, in the guard itself.**
`.claude/hooks/guard_bash.py` gained the refusal the same day, and its first run
blocked a test script that ran no git at all. The cause was one level below the
new rule: `segments()` split the raw command text on `&&`, `||`, `;` and `|`
without regard for quoting, so `echo "cd x && git commit -m y"` produced a
segment reading `git commit -m y"` and the guard read **a quoted mention as a
command**. The bug was already there and had cost nothing, because the rules
before this one watched `git add -A`, a string nobody writes in passing, while
`git commit` is written in documentation and test scripts constantly. The split
is quote-aware now, verified on seventeen shapes including the two that must
pass and the fifteen that must not change: **the same lesson as
`without_heredocs` one level up, that data is not a command line, and a guard
learns it once per layer.**

## CL-071 — Work in parallel while a gate decides, never while a clock runs
2026-09-09 · author instruction · § Verification

*"non puoi staccare un work tree dove tu vai avanti a lavorare?"* — asked while
a thirteen-minute net was running and the session was idle. The answer is yes
for correctness and no for duration, and both halves were paid for the same
week. Free: a pass-or-fail suite decides green or red, and nothing another
process does changes whether an assertion holds, which is why five panel judges
already run at once. Forbidden: a `/usr/bin/time -p` run, because CL-025 says
the machine stays still — on 2026-09-08 a timed suite read `real 1870.49`
against `user 65.37`, the ratio said it had been waiting, and the number was
discarded for an honest re-run at 67 s. And forbidden: editing what the running
suite reads. `records` reads `CLAUDE.md`, `.claude/**` and all of `docs/`, so
amending the contract mid-net is panel 056's story with the coordinator in the
judge's chair. The two routes that are better than waiting idle are the
scratchpad, where nothing in the tree moves, and a **detached worktree**, which
has its own index — the thing CL-041 and CL-070 are both about a shared one
carrying away another session's work.

## CL-072 — The named suites are a map, and the map is a command
2026-09-09 · author instruction · § Verification

*"prima testare ciò che è impattato dalla modifica, e soltanto alla fine la
suite completa"* — which § Verification and CL-063 already required. What was
missing was not the rule but the map: **"the named suites" was a judgement call
made fresh every time**, and on 2026-09-09 it was made wrong. Four new
`tests/golden/fixedbugs/` cases were gated on `annotations`, `canonical` and the
check goldens, and `emission` — which nobody had thought to name — went red,
because that suite's written premise is *"those cases are wrong FFI bindings,
and the thing that refuses them is clang, at build time"*, and the four were
refused by the CHECKER instead. The premise failed loudly, which is what a
premise written down is for. The map now lives in
`.claude/rules/verification.md` **as the command that produces it plus today's
answer**, because a table of which suite reads which directory is a premise
about the world and expires in silence (`.claude/rules/module-shape.md`). The
same run found that `golden` is not among the twenty names the net registers, so
`-- <compiler> golden` selects nothing and prints no line: a green run that
tested nothing.

## CL-073 — A push is an act on the BRANCH, so on a shared checkout the session that respects the hard stop is not the one who decides
2026-09-10 · found by measuring what a push would carry, twice, and getting two different answers · § Hard stops, and CL-042

§ Hard stops says *"Pushing `main`, publishing the site, anything outward-facing:
asked for, every time"*, and CL-042 gives it a procedure: commit and tag locally,
**say how many site commits would travel**, wait for a yes. **The procedure is
correct and it is not sufficient, because a push does not carry a session's
commits. It carries the branch.**

What happened, in the order it happened. This session pushed `09b6b01f` at 13:04
with the author's yes, and the sentence it wrote in the asking was true and
measured: **one commit ahead, zero of them touching `site/`**. At **13:47** a
parallel session committed `56edfb96` in the same checkout, 23 files, **22 of
them under `site/`** — real page copy in both editions. This session then
committed `03cefaf2` at 13:55:29 and `8a9a50bf` at 13:55:44, re-measured what a
push would now carry, and told the author the honest second answer: **three
commits, and 22 site files it had not written**. The author was given three
routes and chose to publish all three. **And then, before this session ran
`git push`, `origin/main` and `HEAD` were both `8a9a50bf`: zero ahead, zero
behind.** The parallel session had pushed, the branch carried this session's two
commits, and the outward act happened without the session that owed the question
performing it.

**It went well here only because the answer happened to be yes.** Of the three
routes offered, one was *wait*. Had the author taken it, the lockfile commit
would have reached the public branch anyway, minutes after the refusal, and
nothing in the contract or the hooks would have fired.

The general shape, and it is why this is its own entry rather than a line under
CL-042: **CL-041 is about a shared index, CL-070 about a shared staging area, and
this is one level up again — a shared branch.** All three come from the same
fact, that more than one session works in this checkout, and each one bites a
different verb. A rule about `git add` is obeyed at `git add`; a rule about
`git commit` is obeyed by a pathspec; but **a rule about `git push` cannot be
obeyed by one session at all**, because the thing published is not the thing that
session did. It can only be obeyed by every session at once, which is not a
property a session can check.

**What this does not touch.** CL-041 and CL-070 stand exactly as written, and the
pathspec did its job here: both of this session's commits carried only the paths
named on their command lines, and the parallel session's 23 files stayed out of
them. The failure is downstream of the commit and upstream of nothing — there is
no later gate.

**Three routes, named rather than chosen, because the amendment is the author's**
(author decision 2026-09-10 to record the fact and decide the rule separately).
A session that has not been given its yes keeps the work **uncommitted**, or on a
branch of its own, until it has one — which costs the local commit that CL-042's
own procedure asks for. Or **the question moves earlier on a shared checkout**,
to the commit rather than to the push, since the commit is the last moment a
session controls alone. Or **the hard stop names the condition it actually
needs**: nothing reaches the shared branch before its yes, which makes every
session's commit an outward-facing act while a peer may push, and says so.

**And one measurement for whoever writes the rule**: the window here was **eight
minutes**, 13:47 to 13:55, and neither session knew the other was in it. The
first this session learned of `56edfb96` was reading `git log` after its own
commit succeeded.

**Appended 2026-09-10, the same afternoon: the second case, and it runs the other
way.** The rule above was amended on this entry at **14:21:50** (`0a2329df`), and
that commit was pushed by THIS session minutes later. Between the sentence that
asked for the push, which said *one commit, one file, zero site files*, and the
`git push` that followed the author's yes, the parallel session committed twice:
`ede35962` at **14:26:18**, its own CL-074, and `ed1a5475` at **14:28:00**, **29
files under `site/`**.
Both went out under this session's push. **So the first case had a peer publish
this session's work and the second had this session publish a peer's**, and
neither session was careless: each measured what would travel, said the number
out loud, and was overtaken by the other between the measurement and the act.
That symmetry is the argument the entry above could only assert — the failure is
structural, and the number a session reports is true when it is written and can
be false when it is used. **What it adds to the three routes**: whichever is
taken, the count in the asking sentence has to be re-read at the moment of the
push and not at the moment of the question, or the record says something untrue
about itself (§12), which is the half CL-070 already paid for one verb down.

## CL-074 — And a claim about a gate is produced with the gate's own invocation, defaults included
2026-09-10 · author instruction, after the CI refuted it · § Run it, or say it is unrun

*"`docs/measurements/025` records today's lesson, and it is worth more than all
the rest: the CI refuted a negative claim of yours that came from a single
command"* (the author's words in English, § 11). The claim was a comment in
`tests/golden/run/lease-open-through-exit.hero`, removed the same day, the
morning's golden for panel 125 R5, *the exit path is named rather than closed*,
where a program leases the literal `"twelve bytes"` to `strlen` and leaves
through `exit(code: 0)`. In `7965174d` the comment was right, *"The Linux
`--sanitize` leg is what sees the block that is still held here"*. In
`bf625ca1` a dated correction was **appended underneath** it rather than
replacing it, `tests/golden/` being on § 14's never-rewritten list, and it
concluded **"So NOTHING accuses this program on any of the three platforms"**,
on one command in the Linux container: `heroes build --sanitize <case> -o
/tmp/lx` and then the binary, once more with `ASAN_OPTIONS=detect_leaks=1`. That
commit is stamped **09:16:16Z**; the CI's `FAIL run/lease-open-through-exit` is
stamped **09:48:57Z**, with `LeakSanitizer: detected memory leaks` and **29
bytes** in one object from `hero_str_held` under it, refused by the
`AddressSanitizer` check in `tests/harness/suite_run.hero`. Run `34461331137`,
head `cf1645a7`, two commits after the false sentence because **no run exists on
`bf625ca1` at all**; Windows and Darwin were green on that head, so the earlier
sentence was right and the correction wrong on the one leg that holds the
instrument.

**The suite does not run the command that was probed, and the difference has a
name the help text prints.** Its third configuration is `run <source>
--sanitize`; `heroes help`'s `build` section says `-O0  compile at -O0 (the
default here)` and its `run` section says `-O2  compile at -O2 (the default
here)`, read 2026-09-10. So the probe was `-O0` plus
ASan and the gate is `-O2` plus ASan, and the reading that fits the two answers,
unrun as a measurement of LeakSanitizer, is that at `-O0` the lease cell is
still a live local of `main` at `exit`, so LSan finds a pointer and calls the
block reachable. `build --sanitize -O2` in that container is the invocation
nobody listed and the one that would settle it (CL-057), and it is unrun.
Nothing here could have caught any of it, measured **2026-09-10** on the source
restored from `bf625ca1`: both commands print `12` and exit 0, and
`ASAN_OPTIONS=detect_leaks=1` answers `detect_leaks is not supported on this
platform` and aborts at **134** (panel 021).

So the invocation is part of the measurement: a claim about what a gate will say
is produced **with the gate's own invocation**, read out of the suite that runs
it and on the leg that owns the instrument, defaults included, and a silence
from any other invocation is a question. CL-017's *match the unit to the rule
that will judge it* carries the invocation beside the unit. The hedge belongs on
the sentence a reader quotes: this correction hedged the mechanism, *"that is a
reading of the tool rather than a measurement of it"*, and stated the conclusion
flat, and the flat one is what travelled. And **measurement beats opinion
(§ 12) only when it answers the ruling's question**: the ratified sentence was
about that leg's verdict, the probe about a hand-built binary at another
optimisation level.

**Why this is its own entry and not a line under CL-018 or CL-062**: the command
ran, it finished, and it answered a question nobody had asked, so the remedies
differ. CL-018's is to write a negative claim as a question naming what was
searched for, which yields a hedge and ships the same golden; this one's is to
run the suite's own command, which yields the answer. The breach bought **two
corrections instead of one**, and the false one is what a later reader trusts,
because its first sentence ends *"by running it"*. The case is gone, since it
leaks by construction and a `tests/golden/run/` case may not leak, and nowhere
else could host it: `examples/` runs a `--sanitize` configuration of its own and
refuses the same banner. Where no directory can host a fact, the payment is the
report from the leg that produces it.

Unrun, and named as such: the container transcript in `docs/measurements/025`
cannot be re-run from this Mac and the CI log corroborates only its first
command, so the hand-built silence on that leg is one session's own report and
nothing else. Two corrections that document owes, made here: it counts *"three
C programs"* over an enumeration of two, and its closing sentence, *"The suite's
own command was one line away in the file the claim was being written into"*, is
false, since what stood in another directory was that suite's header, *"NOT a
leak gate — ASan's leak detector does not exist on Darwin arm64"*, whose
platform the false sentence dropped. It and `DESIGN-LOG.md`'s row for it also
home the lesson at *"§ RUN IT's third shape"* while quoting the second shape's
sentence, which sends a reader to CL-057 where CL-018 and this entry were meant.
And nothing refuses the next member of this class locally: there is none today,
no file under `examples/` or `tests/golden/run/` pairing `lease(` with `exit(`,
and all three configurations pass one here (CL-055), so the refusal is the Linux
leg's, in CI.

## CL-075 — A number in a seat's report is a number another session measured, and the sentence that repeats it does not say so

2026-09-10 · author instruction, *"write down what you learnt today"* · § Run it,
or say it is unrun

Twice in one afternoon, in the same shape. The site panel's design seat reported
that the sticky nav occludes *"about 49px, and about 72px once its items wrap"*,
derived from the nav's box, and the session applying its findings wrote both
figures into `site/public/style.css` as the comment justifying
`scroll-padding-top: 5.5rem`. Measured afterwards by that same seat, at eighteen
widths: **51.02px** and **81.16px**, the break being the nav's own
`max-width: 800px` query and not a wrap. What the reasoned value did was leave
**36.72px** of dead space at 1200 and **6.69px** at 390, where the row it existed
to clear hugged the nav instead.

Then, in the commit that repaired that, the same session wrote that the seat had
measured the longest tracklist row *"at thirteen widths from 800 down to 480 in
both editions"*. The seat had measured thirteen in English and **seven** in
Italian. It closed the gap by running the other six rather than asking for the
sentence to be softened, so the comment is true as written, and this entry stands
regardless: the sentence became true through somebody's later work and not
through anything its author had done.

**Why this is its own entry.** CL-018's tell is a connective, *so* or
*therefore*, and there is none here: the shape is a citation rather than an
inference. CL-017 covers it exactly once, but only once somebody notices that
**a subagent is another session** and its report is one of the documents that
rule refuses. That is the step nobody takes, because the report arrives inside
this conversation, in this session's own transcript, produced by something this
session convened, and it reads as work done here. CL-074's remedy does not reach
it either: there a command ran and answered a question nobody asked, while here
the right command ran, in another session, and the sentence appropriated its
running.

**The remedy, and it is cheap.** A number from a seat, a subagent or a teammate
is either re-run before it is asserted, or attributed in the sentence carrying
it: *the seat measured X* costs four words and cannot be wrong. Prefer re-running
wherever the command is short, which both of these were, the second being one
script over thirteen widths.

**And the collaborator's over-claim is not the defect.** A seat writing *in both
editions* of a measurement half made is ordinary, and five seats produce that
constantly; the panel is convened for judgement and not for custody of the
record. What the contract asks of whoever holds the pen is that it not launder
somebody else's confidence into its own record. A comment in a stylesheet
outlives the report it came from, and nobody reading it later can tell which
number was measured and which was believed.

**One instrument this suggests and nothing implements**: nothing in the tree can
tell a figure that was run from a figure that was quoted, and this class is
invisible to every check the repository has. The three suites that judge prose
read what a sentence says, not where it came from. So the only guard is the
sentence's own grammar, which is why the remedy above is a rule about wording.
