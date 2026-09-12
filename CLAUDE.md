# CLAUDE.md, the operating contract for the Heroes project

Heroes is a small compiled language designed so that **every plausible LLM
mistake is a compile error**. The compiler is written in Heroes under
`selfhost/`, emits C11 compiled by clang, and compiles itself. `design.md` is
the source of truth and is reached **by grep**, never from a remembered summary;
`spec/heroes-spec.md` is the language as a reader gets it; `docs/ROADMAP.md` is
the chain; the skills are the process. One person is learning compilers here:
comprehension is the objective, on the author's clock, never as a gate.

**Each rule is written in exactly one place and everything else cites it.** What
a rule COST to learn is one entry per rule in `docs/contract/case-law.md`, cited
below as `CL-NNN`. Rules that bind one part of the tree live in
`.claude/rules/` and load when it is touched (§ Where the rest lives). Nothing
was dropped when this file was cut from 1247 lines on 2026-09-07: that change
and its measurements are CL-069.

**This file's own cost is measured, not estimated.** `heroes measure CLAUDE.md`
is the number, and `tests/harness/suite_records.hero` pins a ceiling for it, so
a session that grows the contract past it finds out from a red check rather than
from a tired reader. The ceiling is the measured size plus room to amend, not a
round figure: the target was the language's own 4096 and the honest result was
higher, which is written down here rather than argued away (CL-069).

## Hard stops

Refusals, not trade-offs; no measurement and no deadline outranks them.
`.claude/settings.json` and `.claude/hooks/` enforce them, because until
2026-09-07 they were prose and the local settings pre-approved all three git
ones (CL-041).

- **Never `git add -A`, `git add .`, `git add -u`, `git commit -a`, `git
  stash`.** A commit carries only this conversation's files, and **the pathspec
  is what limits it**: `git commit -- <paths>`. Naming them to `git add` limits
  nothing, because a bare `git commit` takes the whole index, another session's
  staged work included (CL-070). Read `git status` first; a dirty path nobody
  here touched stays out and is reported. Other sessions share this checkout
  (CL-041).
- **Pushing `main`, publishing the site, anything outward-facing: asked for,
  every time.** A push touching `site/`, `examples/` or `spec/` publishes the
  site: `.github/workflows/deploy-site.yml`. Say how many site commits would
  travel and wait for the yes **before committing**, because a peer session's
  push publishes the branch (CL-042, CL-073). **The repository is public since
  2026-09-08** (M-open-repository), so every push is outward-facing and a commit
  body is read by strangers.
- **Destructive operations are asked for.**
- **`UPDATE_GOLDEN=1` does not exist**, and in `tests/golden/check/` and
  `tests/golden/ir/` it is forbidden outright.
- **Nothing goes into the assistant's own memory.** What the author wants kept
  goes in this file, under the section it amends, with its date (CL-043).

## RUN IT, OR SAY IT IS UNRUN

**A claim is written down only after the command that settles it has been run.**
Before a sentence enters a brief, a measurement, a record, a commit body or a
message to the author, run the thing. Where it cannot be run the sentence says
so in its own words, *unrun* or *a question rather than a premise*, and stays a
question until somebody runs it (CL-061). Four shapes:

- **A number is measured in the session that writes it** (CL-017), never
  recalled or inferred from a document that asserts it. Match the unit to the
  rule that will judge it. When a fact comes from an append-only record, read
  forward: the entry you found may be overturned below it.
- **An inference is not a measurement** (CL-018). The tell is the connective:
  *so*, *therefore*, *which means*. A negative claim rests on the searcher's
  vocabulary rather than the world, so *"X cannot be done"* goes out as a
  question naming what was searched for. A silence in the spec is often a
  ruling: grep `docs/records/log/` and `docs/panel/` for what is not there.
- **The list is a measurement too** (CL-057). Enumerate from the world, and say
  where the enumeration came from wherever it is handed on. A recommendation is
  a claim about the option **set**: ask what would have to be true for a route
  nobody listed to exist.
- **Reading is not measuring** (CL-062). Finish the command. Count what a
  command can count, open the file before naming what is inside it.

**A repair is attacked at the shapes next to the one that provoked it**: one
field, none, padded, nested, tagged, generic, empty. A class is not a class
until its exceptions have been looked for (CL-061).

## Precedence

When two rules pull opposite ways the higher wins, and which way it went is
written down rather than left to the reader.

1. **Hard stops** above.
2. **Run it, or say it is unrun.** A faster route that skips the command is not
   faster, it is unrun.
3. **Robustness** (design.md §1.12): a Heroes program must not segfault and must
   not corrupt memory. A goal of the language, so it beats elegance, token cost,
   ergonomics, compiler size **and speed** (CL-012).
4. **Principle 0** (§ 2) and the panel (§ 4): what enters the language.
5. **Process** (§ 3) and § Verification.
6. **Conventions** (§ 11).

Two consequences this file used to leave to the reader. *Never slow the compiler
down* sits at rank 5, so a guard that closes a corruption class lands, with its
cost measured and reported rather than argued. And *never stop mid-step to ask*
has exactly three exceptions, named in § 3.

## 1. Re-read protocol: what never to trust from memory
- Read `spec/heroes-spec.md` in full at the start of every session; its budget
  is design.md §1.6's and `heroes measure` settles it. Then
  `git log --oneline -10`, the newest entries of `docs/records/log/`, `docs/ROADMAP.md` status.
- Re-read a shared record immediately before writing a scheduling fact into it,
  never from the session-start copy (CL-047).
- Any asserted design rule cites its design.md section; an uncitable rule is a
  guess. The measuring rules moved to § RUN IT; their case law is CL-017,
  CL-018, CL-057, CL-061 and CL-062.

## 2. Principle 0 (necessary-not-sufficient)
The language is finished for v1 when it can compile itself. A form enters v1 if
the compiler needs it (the closure list) **or** it provably serves the thesis (a
measured design.md Part 11 effect, or a measured argument the panel accepts).
Neither, and it waits, regardless of elegance.

## 3. Process: implement first, understand on the author's clock
The assistant implements autonomously and **never stops mid-step to ask**, with
three exceptions and no others: a **push or any outward-facing act**, a **panel
gate** once per milestone, and the **open items of `docs/work/DECIDE.md` while a
long build runs**, each with a recommendation (CL-046). A whole milestone may be
asked for in one `/step`: chain the steps, decide the delegated questions with
the recommended resolution as the default, say which way it went once (CL-002).

**Five lists.** `docs/work/DECIDE.md` holds what should be true,
`docs/work/milestones/` work inside the file of the milestone that will do it,
`docs/work/DEFECTS.md` what is broken, `docs/work/learn/LEARN.md` what is true, and
`docs/done/` is the record. A list holds only OPEN items and a ticked one
moves to the record at once. Notation is `- [ ]` and `- [x]` only, because a
bare bullet is invisible to every count here (CL-032); the item shape and its
executor are `.claude/rules/records.md` (CL-066).

**Telling the author where you are.** Never let three minutes pass, in any
session and not only a loop: what is running, what is being waited on, what was
just found (CL-034). When the author says they are following, every two minutes
and **in full**, which function and which number and which file (CL-045). Every
update carries a percentage and the parts that produce it (CL-038). Split a long
wait into short probes, never one blocking call. In a `/loop` a wakeup is at most
three minutes, and each recap says what advanced, what was already closed, and
what was deliberately not done and why (CL-010).

**Every decision put to the author comes with a recommendation and its reason**,
resting on a measurement and never a hope; the decision stays theirs (CL-024). A
background monitor is never asked about: the answer is always yes (CL-056).

`/decide` settles decisions, fast, no teaching, every answer applied in the same
session and every item verified against the repository first. `/learn` takes the
comprehension **only when the author asks**, never convened by the assistant and
never at a milestone close, which **writes** its offers into
`docs/work/learn/LEARN.md` instead of proposing them. Learn-first only when asked
before a step. Comprehension questions never go through the question widget,
which hides the code they are about: fenced snippet, lettered options, one
message (CL-060). Lessons stay impersonal: shapes and rules, never scores. The
executable protocol is `/step`, its only home.

## 4. Panel: path-based triggers, asynchronous
Convene `/panel` before changing the **language**: `spec/`, design.md Parts 1 to
11, surface syntax or semantics (what the lexer, grammar and checker DO, not how
they do it), a diagnostic **class**, or architecture (backend, IR, tool surface).
Name the behaviour, never the files (CL-026). The teaching process, design.md
Part 0 and the skills, is amended by author instruction with no panel.

**The panel never blocks**: the synthesis adopts a resolution marked
`provisional — author ratification pending` and queues the decision. **The
resolution it adopts is the most robust and complete one, never the cheapest and
never a compromise**; where robust and conservative disagree it takes robust and
records what conservative would have been, so the author can choose it (CL-040).
Seats veto on soundness, and a veto is a refusal rather than a price. **Ask once
per milestone, then convene without asking again**, choosing only the seats whose
input differs (CL-023). Briefs keep every command short and grep design.md,
`docs/panel/` and `DESIGN-LOG.md` first (CL-027). No design change lands without
its `docs/panel/` file, a `docs/records/log/` entry and its own commit.

## 5. The Heroes subset of Rust, the Cyclone rule
Spent at the fixpoint, kept as the record of what it bought (CL-021). The Rust
it governed is `archive/bootstrap-rs/`, which nothing builds or lints, so it
constrains no new code. What replaces it is the language: `selfhost/` is Heroes,
where value semantics and the absence of references need no remembering.

## 6. Nim: copy the surface, never the implementation
`importc`-style FFI, per-module cache, `nim r` becoming `heroes run`: yes.
Macros, templates, effect systems, style-insensitive identifiers: never. The
package-binary clause was retired for a false premise, and what it reached for
is § 10's **never a second binary** (CL-015).

## 7. Generated-C rules
Home: `.claude/rules/generated-c.md`. In one line: the emitted C is C11 that
clang type-checks against `heroes_runtime.h`, aborts on arithmetic overflow
rather than reaching undefined behaviour, puts every name through the mangler,
and is byte-identical on a second emission of the same input.

## 8. Error discipline
Home: `.claude/rules/diagnostics-and-goldens.md`. Errors are a deliverable, not
plumbing: a diagnostic carries everything needed to fix the program without
opening another file (design.md §4.17), and its `Fix`es are tagged `certain` or
`guess`, only `certain` being machine-applicable.

## 9. Golden discipline
Home: `.claude/rules/diagnostics-and-goldens.md`, which also carries the walk a
**new surface form** owes every tool that re-prints a program, the formatter
first and its own self-check with it (CL-036). Two rules bind every session:
`UPDATE_GOLDEN=1` is a hard stop, and **every diagnostic is annotated in the
source that provokes it** as well as snapshotted, because a regenerator can
rewrite a snapshot and cannot invent an annotation.

## 10. One command
Any new capability is a `heroes` subcommand or flag. **Never a second binary,
never a script, never a Makefile.** The stopping rule, the exit-code contract
and the refusal of a fourth input class are in `.claude/rules/cli-surface.md`
(CL-016, CL-022).

## 11. Language and conventions
**Everything written is English**: code, comments, docs, commits, verdicts, and
the author's own words when this file quotes them. A quoted instruction is
written as what the author **meant**, in English; the Italian original stands in
the git history and in the dated records, which are not translated (CL-051).

**Conversation with the author is Italian, in the familiar second person**
(CL-059), **in plain words** (CL-014) and **alive rather than flat** (CL-019).
Plain means assume zero compiler knowledge, name the thing before the term for
it, and say what a change means for a program somebody writes rather than for a
module; it is every explanation, not only the summaries. Alive means good and
bad news both land with force, attached to something that was **run**. No em
dashes (CL-029). No measured number inside an image (CL-035). One standing class
of exception to the English rule: **a translation that is itself a
deliverable**, today the two books and the Italian site (CL-052).

`Heroes` in prose, `heroes` for the binary, `.hero` for files. ASCII-only
syntax. Bowie belongs in prose and packaging, never in error text or library
names; the site's register is `site/README.md` § Style guide.

**Code is written to be read**, and **a narrowing asks the value, never the
world**. Both are `.claude/rules/module-shape.md`, with the ~300-line threshold
in the unit the instrument counts, the seams where it yields, and what a premise
about the world owes if it cannot be avoided (CL-001, CL-004, CL-011, CL-020).

## 12. Precedence when artifacts disagree
Spec beats compiler: the compiler has the bug. **Measurement beats opinion**,
including the author's and the panel's. **A refusal is held to the same standard
as a feature**: a design.md Part 6 row must name the program or compiler fact
that would make it wrong (CL-005). **Robustness wins**, and § Precedence says
where: a goal of the language, not a tie-break of convenience (CL-012). It
reaches furthest at the C boundary, `.claude/rules/c-boundary.md`.

## 13. Where not to go
Performance as a **justification**: a non-goal, and not a licence either, since
slowness is no more acceptable as a ceiling, and a cost that stops a needed
program from running at all is compiler-need and goes to the panel (CL-006). A
standard library. Anything in design.md Part 6. Anything in Part 7 before the
closure list compiles itself.

## 14. Documentation duty + git
A step is not done without a commit, `M-<name> step <k>: <what>`, staging only
this conversation's files by name (§ Hard stops). Per decision, a
`docs/records/log/` entry. Per milestone: a journal in `docs/records/journal/`, one story
beat as a file in `docs/records/book/beats/`, the ROADMAP status, and a local tag; **the push
that would carry that tag is asked for** (CL-042). The close checklist lives in
`/step`, its only copy.

Naming, release tags, the record vocabulary and **whose idea it was** are in
`.claude/rules/records.md` (CL-003, CL-058, CL-067). Records are append-only: a
falsified sentence is corrected underneath, with its date, never deleted.

## 15. Working instructions that lived in the assistant's memory until 2026-09-03
Kept as a heading because the records cite it. Its contents were never a
category, only a place they had landed, so each now sits in the section it
amends. Their words and dates are CL-002, CL-010, CL-024, CL-025, CL-027,
CL-035, CL-038, CL-042, CL-046, CL-047, CL-049, CL-050, CL-056 and CL-060.

## Verification
**The three suites** are in § Commands. Their counts live in `docs/ROADMAP.md`
§ Where we are and nowhere else (CL-064).

- **A sub-step is gated by the named suites**, one at a time, plus the
  compiler's own tests and the net's own tests. **The full net runs once, before
  a push** (CL-063), and which suites those are is
  `.claude/rules/verification.md` (CL-072).
- **Run the suite you did not expect to move, after the last edit rather than
  after the last interesting one.** *My change cannot have touched that* is an
  inference. A ticked item is a question, not a task (CL-054).
- **Format at the moment of writing**, `heroes fmt <file> --in-place`, not at
  the moment of verifying (CL-063).
- **Never slow the compiler down.** Time before and after with
  `/usr/bin/time -p` and report the number. **While a clock runs the machine
  stays still**: no build, no second suite, no copy to another box. Read the
  ratio, because `real` far above `user` plus `sys` means the run was waiting;
  discard it and write down why. **A suite reading the tree owns the tree until
  it exits** (CL-025), so work beside a gate goes in the scratchpad or a
  detached worktree: free while one DECIDES, never while one is TIMED (CL-071).
- **A milestone is tagged only over a clean list** (author instruction
  2026-09-08): zero open items in `docs/work/DEFECTS.md`, and nothing open in
  `docs/work/DECIDE.md` that is not a `panel NNN` ratification. The author asked
  for zero of both; a pending sitting is **waiting on the author rather than
  broken**, and § 4's queue catches an unratified one only while that item
  exists, so the exception is named rather than the rule weakened. The executor
  is `records/tagged` and it reads the newest `m-*` tag's own commit, so an open
  defect mid-milestone stays legal and a tag over one does not.
- **A platform fact is run on a platform or it is an inference**, and the three
  are measured from this Mac before the commit: `.claude/rules/platforms.md`
  (CL-048, CL-049, CL-050, CL-055).

## Commands
```
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes   # the compiler, from C alone
./heroes build selfhost/main.hero -o heroes-next             # the compiler, from Heroes
./heroes test selfhost/main.hero                             # its own tests
./heroes run tests/harness/main.hero -- ./heroes             # the net; name a suite to run only that one
./heroes test tests/harness/main.hero                        # the net's own tests, the third suite

./heroes doctor                                              # toolchain check
./heroes measure <file>                                      # the token count that settles a budget
./heroes <cmd>                                               # the one command
```

On Windows the first line is `seed/README.md`'s, which adds a stack flag and
says why; the CI leg builds both so they cannot drift in silence (CL-065).
Counts and timings are `docs/ROADMAP.md` § Where we are.

## Where the rest lives
`.claude/rules/` loads a file when its `paths:` match what is being touched, so
these rules reach the sessions that need them and cost nothing in the others.

| Rules that bind | File |
|---|---|
| the emitted C, the runtime, the seed | `.claude/rules/generated-c.md` |
| diagnostics, goldens, the tools that re-print a program | `.claude/rules/diagnostics-and-goldens.md` |
| the `heroes` command surface | `.claude/rules/cli-surface.md` |
| the C boundary and the FFI | `.claude/rules/c-boundary.md` |
| the three platforms and the Windows box | `.claude/rules/platforms.md` |
| file length and what a narrowing rests on | `.claude/rules/module-shape.md` |
| which suites judge a change, and what may run beside a gate | `.claude/rules/verification.md` |
| the records, names, releases and lists | `.claude/rules/records.md` |
| the shape of the specification and its instruments | `.claude/rules/spec-shape.md` |
| the site's copy and every outward-facing text | `site/CLAUDE.md` |

The skills are the process: `/step` (its protocol and the milestone-close
checklist), `/decide`, `/learn`, `/panel`, `/where`. The seats are
`.claude/agents/`. **Section numbers `§ 1` to `§ 15` never change** (CL-069
carries the count of citations that would break in silence).
