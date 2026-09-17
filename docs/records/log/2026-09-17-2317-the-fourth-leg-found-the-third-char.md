# The fourth leg found C's third `char`, and the prediction that sent it looking was wrong about why

2026-09-17. M-arm-platform, step 1. Every number below was measured in this
session, on two containers built from `docs/ref/environment/linux/` and on the
author's Mac.

## The decision

| | |
|---|---|
| date | 2026-09-17 |
| decision | arm64 Linux becomes the fourth measured platform: two named Dockerfiles where there was one bare `Dockerfile`, a `ubuntu-24.04-arm` matrix leg, and the `char` prediction scored |
| reason | three legs were all signed-`char` platforms and could not have shown the divergence; the fourth found it in its first hour, on a golden case this repository has shipped since 2026-08-16 |
| design.md § | §1.11 (everything comes from C), §1.12 (robustness) |
| panel | none for the machine; defect 058 goes to a five-seat sitting, author's answer of this date |

## The prediction, scored, and the half that was false

`docs/work/milestones/M-arm-platform.md` registered it on 2026-09-10 so it could
be scored: *plain `char` is unsigned on the ARM ABI and signed on x86-64*. A
three-line C probe, run on all three machines this project can reach from here:

| machine | plain `char` | |
|---|---|---|
| Linux arm64 | **UNSIGNED**, `CHAR_MIN 0`, `CHAR_MAX 255` | as predicted |
| Linux x86-64 | SIGNED, `-128` … `127` | as predicted |
| Darwin arm64, this Mac | **SIGNED**, `-128` … `127` | **against the prediction** |

**The divergence is real and the stated cause is false.** The generic AAPCS
declares plain `char` unsigned and Debian arm64 follows it; Apple's own arm64
ABI deviates and declares it signed. So the fact belongs to the **platform's
ABI**, not to the architecture — and a fourth leg chosen for its architecture
alone could have been an arm64 Darwin and shown nothing at all. What made this
leg informative is the pair *arm64 + Linux*, which no one argued for by that
name when the row was scheduled.

**This is the shape `.claude/rules/module-shape.md` names**: a premise about the
world expires in silence, and the argument resting on it goes on reading as
correct. The premise here was one clause of a scheduling note, and it was right
enough to send the leg to the right place for the wrong reason.

## What the machine is, and the one axis

`silkeh/clang:22` publishes both architectures under one tag (amd64
`sha256:42cebd4a…`, arm64 `sha256:d315ac7f…`), so the two containers came out
**identical on everything but the architecture**, measured rather than intended:
Debian 13.6, Debian clang 22.1.8, lld and lldb 22.1.8, glibc 2.41, pkg-config
1.8.1, git 2.47.3, sqlite 3.46.1, libcurl 8.14.1 on both. One axis, so a
divergence has one candidate cause. That discipline was bought on this machine's
x86-64 sibling on 2026-09-03, when a finding that looked like a platform fact
turned out to be a clang-version fact.

The arm64 machine then said what it was worth in its own output: the seed built
from C alone, `heroes doctor` six rows `ok` with `arch aarch64`, **654 tests,
all passed** for the compiler's own — the same 654 measured on the Mac in this
session — the full net at **1825 passed, 3 failed**, and the net's own tests at
**157, all passed**.

**The three failures are one program and one cause.** `run`, `determinism` and
`emission` each fail on `tests/golden/run/ffi-a-char-array-member.hero`; every
other suite is 0 failed, `corpus` included at **53 passed**. That last number
answers a question the prediction got wrong twice over: the corpus's 20 `extern`
programs of 55 were named as where the divergence would show, and **none of them
binds a plain-`char` member**. It showed in a golden case instead.

## The two defects, and why they are two

**058: a plain `char` field or parameter has no portable spelling.** Four
probes, two architectures, and the answer inverts on every one — `i8` binds on
x86-64 and Darwin, `u8` on Linux arm64, for a scalar field, a fixed-array field,
a parameter and a result alike. `signed char` and `unsigned char` bind portably
at `i8` and `u8`; the hole is C's third `char` type alone.

**The checker is not wrong, and that is what makes it a language question.**
`spec § 13` says a field and a parameter are declared at *the header's own width
and sign*; `selfhost/emit/extern_field.hero:154` asks exactly that and answers
correctly on both machines. What has no answer is the author's question — which
of Heroes' eight integers do I write — because all eight carry a fixed sign and
C's plain `char` does not.

**059: on a platform where `char` is unsigned, the diagnostic names the spelling
it has just refused.** `ffi_parameter_type` on `i8` against a `char` parameter
says *"Declare it `i8`"*. The cause is one row:
`selfhost/emit/c_spellings.hero:59` tables `char → i8` with `no_caveat` on every
target. **That file's own header comment records having learned this exact
lesson one type over** — `long` taught it that *"a width is not a constant to be
tabled; it is a question about the target"*, and `word_bits` arrives as a
parameter because of it. The SIGN of plain `char` is the same kind of question
and never got the same treatment. The `Spelling` record already carries a
`caveat` field for the case where the width is the platform's rather than the
header's, and nothing uses it for sign.

They are filed apart because the repairs differ: 058 asks what the language
should offer and needs a sitting; 059 asks the target a question its own file is
already shaped to ask, and its repair is bounded by whatever 058 resolves,
because a note cannot name a portable spelling before one exists.

## Two things renamed, and one number corrected

**`Dockerfile` became `Dockerfile.amd64`.** With a second Linux machine beside
it the bare name was the one that lies: `docker build <that directory>` would
have handed a reader x86-64 without their choosing it. Neither file carries the
default name now, so a build that does not say which architecture it wants fails
instead of picking one. `MOVED` in `tests/harness/suite_records.hero` carries the
rename so a citation in a record still resolves.

**§ The chain's opening sentence said rows 1–57 were done** over a table whose
row 58 read `done 2026-09-17` eight lines below. M-check-completeness's close
did not move it and the OPENING of row 59 is what found it — which is the second
time that sentence has been wrong, and why it restates its own history and is
re-read at every close *and* every opening.

**§ Where we are says the net's own tests are 156, and both machines read 157.**
The test that made the difference entered in `1029ee3c`, defect 057's own close,
one commit before this one: `tests/harness/suite_warnings.hero` gained one
`test` block and the ROADMAP's count was not re-measured. A count carried is a
count that drifts (CLAUDE.md §1); it is corrected at this milestone's close,
with § Where we are re-measured whole.

## What the CI leg owes a run

The leg is `ubuntu-24.04-arm`, pinned because **there is no `latest` for arm** —
read from GitHub's runner reference this date: the labels are
`ubuntu-22.04-arm`, `ubuntu-24.04-arm` and `ubuntu-26.04-arm`, and the same page
states standard runners are free and unlimited on public repositories, which
this one has been since 2026-09-08.

Two things are **unrun** and are written as unrun rather than implied. That the
label is accepted for this repository has not been executed — the same debt
`macos-26` carried when it was added, and the same instrument settles it, the
first push. And which release `ubuntu-latest` resolves to today is unmeasured,
so whether the two Linux legs are one axis apart or two is a question; if the
first green run reads two clang versions, the pin moves to match rather than the
difference being absorbed.

**The leg is not green and will not be pushed until it is.** A red leg teaches
people to ignore red, and a leg that cannot fail is the *toolchain report that
gated nothing* this workflow already wrote down as a loss. The author's decision
of this date is that M-arm-platform closes with 058 and 059 closed, so the
fourth leg is a judge from its first push — the robust route of CLAUDE.md
§ Precedence, and the only one the rule against tagging over an open defect
allows.
