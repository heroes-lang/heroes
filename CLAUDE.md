# CLAUDE.md — operating contract for the Heroes project

Heroes is a small compiled language designed so that **every plausible LLM
mistake is a compile error**. Bootstrap compiler in Rust, backend emits C11
compiled by clang, self-hosting is the v1 finish line (fixpoint on generated
C). `design.md` is the source of truth for the language; the milestone chain
lives at `docs/ROADMAP.md`; the process lives in the skills (`/step`,
`/decide`, `/learn`, `/panel`, `/where`) — each rule is written in exactly one
place,
everything else cites it. One person is learning compilers through this
project; comprehension is the objective, but it runs **on the author's
clock, never as a gate** (rule 3).

---

## RUN IT, OR SAY IT IS UNRUN

**A claim is written down only after the command that settles it has been run**
(author instruction 2026-09-06, *"put this measuring thing into the rules; there
should already be something, but make it more prominent"*). There is already
something: **§1 below is this rule's case law, and it is 136 lines long**
(counted, not eyeballed — the first draft of this sentence said *two hundred*).
That is why the rule is here instead of in there. A rule a reader meets after
136 lines of its own exceptions is a rule they meet tired.

Before a sentence enters a panel brief, a record, a commit body or a message to
the author: **run the thing**. Where it cannot be run, the sentence says so in
its own words — *unrun*, *not measured*, *a question rather than a premise* —
and stays a question until somebody runs it. §1 has the shapes this goes wrong
in; they are worth reading and they are not what this line is for.

**What earned it, and it is the reason the rule is about RUNNING and not about
thinking harder.** Three claims were corrected on the day it was written, and
**not one of them fell to a better argument**:

- A panel seat was told the morning's repair had closed a hole. It **deleted one
  line from the emitted C and rebuilt** against the same runtime: exit 132, empty
  stderr. The hole was still there and a different milestone had closed it.
- A second seat was asked whether a sentence about what a program prints could
  enter the spec. It ran **`strip`** on the binary; the function name was gone,
  so the sentence would have been false on the day it landed.
- A peer session was sent a claim about a platform. It **opened the file**, and
  the claim was an inference wearing a *so*.

**And one shape §1 did not have: a repair that widens a rule to cover a class can
contradict a ruling the record already made about one member of it.** *A `(`
after a foreign word means the word is called* was true of the case that provoked
it and false of an adversarial case the author had ratified on 2026-08-04, where
`(` opened a function TYPE. The net caught it by quoting that case back. So
before generalising, grep `docs/panel/`, `DESIGN-LOG.md` and `tests/golden/` for
the members: **a class is not a class until its exceptions have been looked
for.**

---

## 1. Re-read protocol — what never to trust from memory
- Read `spec/heroes-spec.md` in full at the start of every session (budget
  4096 tokens, measured — never estimated; that is the point of the budget).
- Reach `design.md` **by grep**, never from a remembered summary. Any asserted
  design rule must cite its section; an uncitable rule is a guess.
- Session start: `git log --oneline -10`, DESIGN-LOG tail, ROADMAP status.
- **A number or a repository fact goes into a panel brief, a measurement or a
  record only if it was measured in the session that writes it** (author
  instruction 2026-08-16). Not recalled, not carried from a comment, not
  inferred from a document that asserts it. **Three sittings in a row were
  briefed with something false and the judges caught all three**: panel 066 was
  told a Rust test existed that never did (a `grep` would have shown it), panel
  067 was given a **line** count against §1.2's **token**-denominated formula
  and a "the port is longer" that compared code+tests to code-only (it is 21%
  *shorter*), and panel 068 was told `sort.c` solved NaN with totalOrder when
  `DESIGN-LOG:205` had superseded `:148` four days after it was written. A
  fourth was a line count typed from memory (872 for 1129).
  Three obligations follow, and each one killed a specific error above:
  **run the measurement**, whatever it costs — `wc -l`, `grep`, `heroes
  measure`, or the program itself; **match the unit to the rule that will judge
  it**, since §1.2 is in tokens and a line count answers a different question;
  and **when a fact comes from a record, read forward to the end of that record**
  — DESIGN-LOG and the panels are append-only, so the entry you found may have
  been overturned below it.
  The cost of the rule is minutes. The cost of breaking it is a five-judge
  sitting answering the wrong question, which is what happened three times.
- **And the rule binds CLAIMS, not only numbers** (author instruction 2026-08-16,
  after one session broke it four ways). Everything above is written about counts,
  and the day's worst errors carried **no number at all**. Four shapes, each one
  measured in the session that names it here:
  - **An inference presented as a measurement.** A brief told five judges that a
    new abort would let `runtime/parts/sort.c`'s two guards be deleted, *because
    the general rule subsumes them*. It does not — `sort` compares through a
    function pointer no emitted `<` ever reaches — and **three seats falsified it,
    two by deleting the guards and running**: `sort` returned an arbitrary
    permutation at exit 0. **The tell is the connective**: *so*, *therefore*,
    *which means*, *it follows that*. A sentence whose truth comes from another
    sentence rather than from a command is an inference, and it is either run
    before it is written or marked as one on the page.
  - **A failed search written as an impossibility.** The same day, a brief told
    three judges that C cannot detect a union naming one member — having searched
    for `__is_union`, which is C++ only. Two seats that compile found the answer
    in `__builtin_classify_type`, **a builtin this emitter already calls on every
    field**. A negative claim is the least reliable kind there is, because it
    rests on the searcher's vocabulary rather than on the world: *"X cannot be
    done"* goes to the judges as a **question**, never as a premise, and names
    what was searched for.
  - **A silence read as an open question.** A sitting was convened on *"the spec
    never states this"* when the silence was **the artifact of a ruling** — panel
    035, four days earlier, three seats, and the word `integer` at `spec:149` is
    that ruling's one-word diff (`ec5558b`, `DESIGN-LOG:207`). The obligation
    above says to read forward from a record; this says **which** record: before
    convening on something the spec does not say, grep `DESIGN-LOG.md` and
    `docs/panel/` for the thing that is **not** there. A deliberate silence and an
    oversight look identical in the document and opposite in the record.
  - **A repair shipped without its adjacent shapes.** The union predicate landed
    for a two-field union and was still live for a **padded** one three hours
    later — and `SDL_Event` is padded, so the case the sitting existed for was
    never fixed. Before the commit, a repair is attacked at the shapes *next to*
    the one that provoked it: one field, none, padded, nested, tagged, generic,
    empty. The provoking case is a witness, not the class.

  The cost is minutes again. The cost of breaking it was a sitting convened on a
  settled question, and a defect that shipped as fixed.

**AND THE LIST IS A MEASUREMENT TOO — WHEN YOU ENUMERATE, SAY WHERE THE
ENUMERATION CAME FROM** (author instruction 2026-09-05, *"write down somewhere
what you have learnt, so you do not make the same mistake again"*, given after a
day that made the same one five times). Everything above is written about
individual claims, and every rule above was **obeyed** on 2026-09-05 while the
day's real errors went straight past them. The reasoning was sound each time.
**The LIST it reasoned over was mine, and it was short.** That is a distinct
failure, and it is invisible in a way a wrong number is not: a wrong number
contradicts something, while a missing option contradicts nothing at all —
nothing in a correct argument points at the row you did not write down.

Five in one day, each measured by somebody else:

- A panel brief named **three causes** for why callbacks do not bind. A seat
  refused the sample and read the real `sqlite3.h`: **five causes over 106
  signatures**, and the one the sitting was convened about was **fourth, at
  2.6%**. The whole sitting was aimed at the wrong noun.
- That brief's *"`const` buys 3 of the 8"* was **2**, wrong in the convener's own
  favour — `nftw` is blocked by a `struct FTW *` that is not `const` at all.
- The same brief's four options **omitted the adapter**, which is what the two
  languages closest to this one's architecture actually ship. The historian
  objected to the **list**, not to the options on it.
- *"CI is green on all three platforms"* was inferred from the word `success`.
  It was **one leg**; the matrix widens only on an `m-*` tag, which
  `.github/workflows/ci.yml` says in its own `on:` block.
- A defect was filed **blocked**, with two routes priced and a written
  recommendation to repair neither. Both routes were refusals. **A third existed
  in the same file, three lines above**, and it cost nothing — the author asked
  for the repair anyway and it took ten minutes.

**Three obligations, and each one kills a specific error above.** **Enumerate
from the world, not from what you can think of** — the real header, the whole
corpus, the actual job list, `gh run view --json jobs`; a list built from memory
is a guess wearing a table's clothes, and the fix is usually one command.
**Say where the list came from, wherever it is handed on** — a brief, a
recommendation, a `/decide` item; *"the eight I could think of"* and *"the 106
in the header"* are different objects and the reader cannot tell them apart
unless it is written. And **a recommendation is a claim about the option SET,
not only about the options** (§15's rule that every decision comes with one, at
its weak point): before recommending, ask what would have to be true for a
route nobody listed to exist — and if the answer is *somebody would have to look
somewhere I have not looked*, look there first.

The cost of the rule is one command per list. The cost of breaking it was a
five-judge sitting convened on 2.6% of its subject, and a defect that would have
stayed open under a recommendation that was correct about everything it named.

**AND READING IS NOT MEASURING — WHAT WAS SEEN ON A SCREEN IS NOT WHAT WAS
COUNTED OR OPENED** (author instruction 2026-09-06, *"write this in CLAUDE.md"*).
Everything above is about facts taken from memory or from a document. These two
came from **looking at the right output and not finishing the job**, which is a
cheaper mistake to make and just as false on the page. Both were caught, in the
same session, only because somebody went back and ran the thing.
- *"Twenty-six lines of code"*, written into a commit message about a runtime
  change. The number came from **eyeballing a filtered diff** that happened to
  print about forty lines. Counted — one `git diff` and a filter that drops
  comments and blanks — it is **33 added and 28 removed**. A count that a command
  can produce is not estimated from the shape of a screenful; §1's rule about
  running the measurement has no exception for a number that looks countable.
- *"Reached from `hero_f64_render` and from the parse side"*, written into three
  documents. The grep had shown two call sites and the second was **assumed** to
  be the parse half; opening it, it is `hero_f32_render`, and the `strtod` inside
  both is a round-trip check rather than a program-facing parse. **Two call sites
  is a count, not an identity.** A name goes into a sentence after the function
  is opened, not after the line number is seen.

The shape they share is the tell: a command was run, its output was read, and the
**last step** — count it, open it — was skipped because the answer felt already
in view. The rule is to finish the command, and where the sentence names a thing
rather than a number, to open the thing.

## 2. Principle 0 (necessary-not-sufficient)
The language is finished for v1 when it can compile itself. A form enters v1
if the compiler needs it (the closure list) **or** it provably serves the
thesis (measured Part 11 effect, or a §1-derived argument the panel accepts).
Neither → it waits, regardless of elegance.

## 3. Process: implement first, understand on the author's clock
The assistant implements autonomously and never stops mid-step to ask.
Everything that once gated progress (predictions, spot-checks, golden
ratification, failure diagnosis, drills) becomes an entry in
**three lists, split by what an item asks** (author instruction 2026-08-12):
`docs/work/DECIDE.md` (what should be true), `docs/learn/LEARN.md` (what is
true), `docs/work/SCHEDULED.md` (work with a milestone that will do it).
**A list holds only OPEN items; the moment one is ticked it moves to
`docs/work/DONE.md`, which is the record** — append-only, never rewritten (§14),
and read by grep rather than by eye.

**The layout and the names changed on 2026-08-26, by author instruction, and
what forced it was a rule with no executor.** The move-to-the-record rule was
already written here, in `DESIGN-LOG:240` and in the record's own preamble, and
**no skill performed it**: `/decide` ticked an item and left it in place. So the
record went untouched for eight days while `DECIDE.md` grew into 391 KB holding
**138 ticked items and zero open ones**, under eighteen headings still titled
`## Open`. Measured across the four lists that day: **71.8% of 671 KB was closed
work and 2.9% was live.** Three things follow and each is now a rule rather than
a habit. The record is called `DONE.md` because it holds finished work — its old
name claimed the opposite, and the reason given for keeping it (*"every commit
subject cites that path"*) was **measured false: 24 subjects of 792, 3.0%, the
last 463 commits before the rename**. `LEARN.md` left `docs/work/` entirely,
because comprehension is not work owed: it blocks nothing and is opened only
when the author asks. And the directory is `docs/work/` rather than the debrief
directory it replaces, which was named after a skill deleted on 2026-08-12.
(That old path is deliberately not spelled here as a citation: the dead-citation
check in `tests/harness/suite_records.hero` reads every backticked path in this
file, and a path that no longer exists is exactly what it exists to catch.)

**And a fourth list, for what is BROKEN: `docs/work/DEFECTS.md`** (author
instruction 2026-09-03, *"I do not like the defect directory … if anything is
still open in defect at the end, make one single file called DEFECTS.md inside
work, so that everything is tidy"*). A compiler defect — a crash, a
wrong answer at exit 0, a silence where a message is owed — is an open item
there, with its reproducer, its measured cause and what is owed, for as long as
it is open; the moment it is repaired the entry gains its *The repair* section
and moves to `DONE.md` like everything else. The directory that held one file per
defect is gone: its seven files became six record entries and one open item the
evening the rule was given, and every live citation followed them.

**One notation, and only one: `- [ ]` and `- [x]`.** A finding written as a bare
bullet is invisible to every count in this project — which is how nine live FFI
findings sat in a second, uncounted notation under `DECIDE.md`'s panel 051/052
headings, two of them measurably stale, while the file reported itself empty.

**AND ONE SHAPE, THE SAME IN ALL SIX FILES, WITH AN INSTRUMENT THAT HOLDS IT**
(author instruction 2026-09-07, given in four parts over one session: the lists
are to be more synthetic and regular — only the items with their state and their
description, no surrounding prose; a closed item does not belong in `DECIDE.md`,
because it is already in `DONE.md`; and one loud line marking where the items
begin, so that a glance says whether anything is there — *"fix `DONE` too"*,
*"fix `ROADMAP` too"*, *"format `LEARN.md` too"*). The shape: **one line of
three fields per item**,
`- [ ] **<first field>** | <what, in one line> | <where to look>`, then an
optional **body indented four spaces** opening with `**Origin:**` and its date,
and the whole item region **fenced between two lines of asterisks** — one only in
`DONE.md`, which is appended to forever, so a closing fence there would be a rule
the next append breaks. The item lists carry `**OPEN: N**` under the opening
banner, and in `docs/learn/LEARN.md` that number is the open questions alone,
because a ticked question STAYS there (`/learn`'s rule: the list is also its own
record). `docs/ROADMAP.md` takes the banner and not the item shape — it is a
chain and a section per milestone, not a list — and what it lost is fifty lines
of preamble that were the history of its own three reorganisations, in front of
its first line of status. **The first field is what the file's instrument
reads**, and that is why it differs: the milestone in `SCHEDULED.md` (`homed`),
`panel NNN` in `DECIDE.md` (`queued`, which scans the item LINE — a sitting cited
in a body is invisible to it), the defect number in `DEFECTS.md`, the origin in
`LEARN.md`.

**What it cost to leave this to prose, measured 2026-09-07 while the shape was
being written.** `DECIDE.md` was **3466 bytes of which zero were items** — three
of its paragraphs narrated items it no longer held. `DEFECTS.md` carried one open
defect and **2753 bytes about five repaired ones**, every one of them already in
`DONE.md`. And `SCHEDULED.md`'s 41 items were single lines averaging **2092
characters** and reaching 5068, which is 96% of the file: the surrounding prose
was not the preamble, it was the item. `LEARN.md`'s 323 were 545. The three are
now 176 characters per item line on average and `LEARN.md`'s are 370, with **no
item's content lost** — the words moved into the body rather than out of the
file, and `LEARN.md`'s 323 were re-laid-out by a script that reassembled every
item from its parsed parts and demanded the original line back byte for byte.
**`tests/harness/suite_records.hero`'s `records/lists` is the executor** — no
`- [x]` in a work list, no `- [ ]` in the record, nothing outside the banners,
and the banner's count equal to the items counted. This rule was stated in four
documents and performed by none, which is this section's own story about
`DECIDE.md` reaching 391 KB told a third time; the difference is that this time a
check fails.

**And the reshaping was itself caught by an instrument, twice, which is the part
worth carrying.** Splitting a long item into a header and an indented body
**moves a date off the line it was covering**, and `records/citations` reads one
line at a time: seven dead-path citations in `SCHEDULED.md` and two in
`LEARN.md` went red the moment the wrapping changed, every one of them a path
that had been legal only because the item was a single line with a date somewhere
on it. So when a body is written, **a line that carries an archived or
never-written path carries its date too**. The `LEARN.md` script was made to
predict its own damage before writing: it reported a hundred, of which ninety
were its own false positives, because a record is citable by the prefix of its
slug and a crude existence test does not know that.

`/decide` takes the decisions the compiler is waiting on: fast, no teaching, every answer applied in the same session, and every item
verified against the repository before it is put to the author. `/learn` takes
the comprehension, **only when the author asks for it** — never convened by the
assistant, never at a milestone close — and each question arrives with the code
on screen and a preamble long enough to make it answerable. Learn-first (questions before implementing) only when the author
explicitly asks before a step. The executable protocol lives in `/step` —
its only home. Lessons stay impersonal: shapes and rules, never scores.

**NEVER LET THREE MINUTES PASS WITHOUT TELLING THE AUTHOR WHERE YOU ARE**
(author instruction 2026-08-31, categorical, given after a day of long silent
waits: *"never let more than three minutes pass without updating me in
keywords"*). This binds EVERY working session, not only a `/loop`: whatever is
happening — a measurement running, a CI leg in flight, a remote machine
building, a diagnosis mid-thought — the author hears it in plain Italian
keywords before three minutes are up: what is running, what is being waited
on, what was just found. A wait longer than that is split into short probes
with a one-line report between them, never one long blocking call. The recap
is the point: an unattended stretch is a stretch of decisions the author
could not redirect, and the author was reduced to shouting in caps to get a
status line. That is this rule's origin and its reason.

**WHEN THE AUTHOR IS FOLLOWING LIVE, THE UPDATE COMES EVERY TWO MINUTES AND IN
FULL** (author instruction 2026-09-03, given while the seven defects were being
attacked: *"I want to be updated with many words and with plenty of detail,
never in plain words, every two minutes, because I am very interested in this
thing"*). The three-minute rule above is the floor for an unattended stretch; an
author who has said they are interested gets the whole picture every two minutes
— which function, which number, which file, and the reasoning that joins them —
never a keyword line. *"Never in plain words"* there means never in FEW words;
it does not repeal §11's plain register, which is about naming the thing before
the term for it. Rich and plain are compatible; terse and plain were being
confused, and the author noticed.

**NOTHING GOES INTO THE ASSISTANT'S OWN MEMORY; EVERYTHING THE AUTHOR WANTS KEPT
GOES INTO THIS FILE** (author instruction 2026-09-03, the second time it was
given: *"I want nothing in memory and everything in the Claude MD. I will change
computer and then I will lose the memory"*). The assistant's memory directory lives on
one machine and dies with it; this file travels with the repository. An
instruction the author gives about how to work is written here, under the
section it amends, with its date and its words, and never into a memory file.
The notes that had accumulated there were moved into §15 the evening the rule
was repeated, and the memory index now holds one line pointing here.

**In a `/loop`, a wakeup is at most three minutes and every one of them writes
a recap** (author instruction 2026-08-12). The recap is the point, not the
schedule: an unattended session that works for an hour and then reports once has
made an hour of decisions the author could not have redirected. Short intervals
buy interruption points. Each recap says what advanced, what was verified as
already closed, and what was deliberately not done and why — the last is the one
that is easy to omit and the only one that lets the author disagree.

## 4. Panel — path-based triggers, asynchronous
Convene `/panel` before changing the *language*: `spec/**`, design.md Parts
1–11, surface syntax or semantics (what `selfhost/`'s lexer, grammar and checker
DO, not how they do it), a diagnostic *class*, or architecture (backend,
IR, tool surface). **The parenthesis named `crates/heroes/src/{lexer,syntax,types}/`
until 2026-08-23**, four days after M-bootstrap-archive moved that tree to
`archive/bootstrap-rs/`: the trigger for the commonest panel path pointed into an
archive, and the brief that convenes those panels pointed there too — which is how
four of panel 087's five seats died on a watchdog rebuilding a compiler that had
moved. Naming the behaviour rather than the files is what the parenthesis always
meant, and dropping the braces is not cosmetic: the brace form is one of the two
citation shapes `tests/harness/suite_records.hero`'s dead-citation check cannot
read (panel 086 R7's measured limit), so this sentence is now checked by an
instrument instead of by memory. The teaching process (design.md Part 0, the skills) is
amended by author instruction, no panel. The panel never blocks: the
synthesis adopts a resolution marked `provisional — author
ratification pending` and queues the decision; the author's verdict is
appended when given.

**AND THE RESOLUTION IT ADOPTS IS THE MOST ROBUST AND COMPLETE ONE, NEVER THE
CHEAPEST AND NEVER A COMPROMISE** (author instruction 2026-09-03, given
mid-sitting at panel 106: *"the panel always takes the most robust and safest
resolution, never the cheapest and never a compromise"*; it is the same
instruction as the `/decide` one of the same day — *"choose the most robust and
complete solutions over the cheaper ones"* — applied to the panel). **This line used to read *the
most conservative resolution*, and the two are not the same thing.** Panel 106 is
where the difference showed: both seats vetoed the proposal, so the conservative
resolution was *do nothing* and the ceilings would have stayed where they were —
while the sitting's own measurements had found a repair worth **58.5% of the
frame** sitting next to the question it was asked. Conservative means *change
least*; robust means *leave the fewest ways to be wrong*. When they disagree the
synthesis takes robust, says so, and records what the conservative resolution
would have been so the author can choose it. What does **not** change: the seats
still veto on soundness, a veto is still a refusal rather than a price, and the
author still ratifies. No design change lands without `docs/panel/NNN-*.md` +
a DESIGN-LOG line + its own commit citing the verdict.

## 5. The Heroes subset of Rust — the Cyclone rule
**Spent, and kept as the record of what it bought** (M-bootstrap-archive,
2026-08-19). The Rust it governed is `archive/bootstrap-rs/`, which nothing
builds and nothing lints any more, so this section constrains no code that is
written from here on. It is not deleted, because it is the reason the port was a
transcription rather than a rewrite: references only as function parameters, never
in structs or return types; owned data everywhere, indices for links;
`BTreeMap`/`BTreeSet` only; iterator/`Option` closures fine, *stored* closures
not — enforced while it was live by `archive/bootstrap-rs/clippy.toml` (the
reasons live there) + `#![forbid(unsafe_code)]`, with every necessary violation
carrying `// PORT-DEBT: <reason>` and the count as the distance from
self-hosting. The distance is now zero. **The rule that replaces it is the
language**: `selfhost/` is written in Heroes, where value semantics and the
absence of references are not a subset anybody has to remember.

## 6. Nim: copy the surface, never the implementation
`importc`-style FFI, per-module cache, `nim r` → `heroes run`: yes.
Macros, templates, effect systems, style-insensitive identifiers: never.
**The list used to end *"a separate package binary: never"* and that clause is
retired** (author decision 2026-08-15, panel 056's historian). It was a bare rule
with one citation and no argument, and its implied contrast was **factually wrong**:
nimble ships *with* Nim, so the thing it named as separate is not. Worse, it was the
rule a reader would reach for first to refuse a project file, and it does not reach
one — a file is not a binary. What it was trying to say is §10's, is already there,
and is there with a reason: **never a second binary**.

## 7. Generated-C rules
C11; `int64_t`/`double`/`bool`; `#include "heroes_runtime.h"` (clang
type-checks every runtime call) and one `_Static_assert` on
`HERO_RUNTIME_ABI`. **What that stamp catches is a header from another
compiler, and not a decoy** — corrected 2026-08-12 (panel 034 R5, and panel
037 measured it from the other side: two runtimes with changed *behaviour*
kept the number at 10 and every generated unit accepted them). A decoy
`runtime/` that copies the number passes; what protects a build against one
is the **cache key**, which covers the whole runtime's contents. The stamp's
real job is the version skew the search makes possible, and it is worth
keeping for that alone — **so it is not extended to cover behaviour** (author
decision 2026-08-12, closing panel 037's open condition: a second guard over
what the cache key already hashes buys nothing, and the guard that would have
been asked to grow is the one that cannot see a decoy at all).
`#line` when an instruction's line differs from the **current
effective line** (`#line N` anchors the *next* line), restored to the
generated file around synthetic code — the restore carries the printer's own
output line count. Emitter debugging is the test helper's job: `--no-line`
is refused by §10's stopping rule (panels 016, 020). Arithmetic aborts via
`__builtin_*_overflow` — never C UB — and `%` is guarded like `/`
(`INT64_MIN % -1` does not trap on arm64). `INT64_C(n)` for every `int`
literal. One `goto`+label per basic block, explicit entry `goto bb0` (spike 2
finding), a label **only where an edge targets it**, all locals hoisted to the
prologue, and a unit-typed temporary never declared at all (`void t0;` is a
hard error). An `@` parameter is a pointer parameter (§4.8's copy-out is
`*p_l = l;`). `hero_unreachable()` at every type-system-proven-unreachable
point. Compile flags: **the list in `selfhost/cli/flags.hero::flags()`, and this sentence names
one of them** — `-std=gnu11`, **named and not inherited**, `gnu11` rather than `c11`
because the two differ by one predefined macro, `__STRICT_ANSI__`, whose only effect
on glibc is to hide `M_PI`, `strdup`, `fileno` and nine more of what §1.11 says a
program binds (panel 047, **ratified 2026-08-14**; and *only* that — `__typeof__` and
`__builtin_types_compatible_p` are clang extensions that work under `-std=c11` too,
measured at panel 092, so nothing else in this file rests on the `gnu`).
**The path was the bootstrap's until 2026-08-25**, when the live list had been
`selfhost/` for six days and the sentence still pointed at
`archive/bootstrap-rs/heroes-cli/src/commands/flags.rs`, which nothing builds — and it was found by a panel judge with no
`grep`, reading this contract as a stranger would. **That is the third time this one
sentence has been wrong**, and the paragraph below it says *"the copy that used to stand
here is deleted rather than corrected a third time"*: the copy was deleted and its
**replacement citation** then rotted, which is the same failure one level up. A pointer is
a fact about the world and expires like any other (§11); the only reason this one is
cheap to fix is that somebody read it. The rest are in that file with
their measured reasons, and **the copy that used to stand here is deleted rather
than corrected a third time**: this file's own preamble says each rule is written in
exactly one place and everything else cites it. The copy died **3h16m** after it was
last made true — `[&str; 11]` and *"the eleven"* at 00:31 on 2026-08-15, `flags.rs`
created at 02:24 *so that a list two documents claimed to state would be easy to
find*, panel 058's twelfth flag at 03:47, and this section still saying *eleven* the
next day. A count in prose is the premise §11 says expires in silence while the
sentence around it goes on reading as correct.
A clang
failure is exit 2 and says the *compiler* is wrong, **with one named exception**
(author instruction 2026-08-12, panel 036; widened by panel 048): a failure the
**author's own `extern` declaration** caused is exit 1 and a diagnostic on the
`.hero` line. Four classes now — a result type the header refutes, a `constant`
that is not one, a name the header does not have, and, since panel 048, a symbol
the **linker** cannot find because the group named no `link`. **The narrowing is
`declaration()`, not whose text it is**: every class recovers a name and asks
whether *this program* declared it `extern`, so a symbol nobody declared stays
exit 2 and the compiler's. (§7 said for a milestone that `archive/bootstrap-rs/heroes/src/emit/ffi.rs` "matches
only the assertion messages this emitter writes" — already false when written,
since `unknown_name` matches clang's own; §11's class, and the stronger rule was
the true one all along.) Every other verdict on generated C is still the
compiler's.
**A refcounted slot is the
one exception to the no-initialisation rule** — zero-initialised so cleanup is
unconditional, with `ptr == NULL` as the non-value every runtime entry point
rejects (panel 021). `--sanitize` adds `-fsanitize=address,undefined`, which
catches use-after-free and double-free; **leaks are caught by
`hero_runtime_check_leaks()`**, because ASan's leak detector does not exist on
Darwin arm64. A generated `eq` or `hash` walks **fields, never bytes** (padding
makes two equal records hash differently, silently), `hash` is never null, and
COW is **one unshare per step** of a mutated place — one at the primitive lets a
nested store alias, measured, with every instrument in this project reporting
success (panel 022). Every name through the
mangler (`h_<module>_<name>[_<typehash>]`; fields, variant cases and labels
too; the module component sanitised to `[A-Za-z0-9]` so the first `_` ends it;
`extern` FFI names pass through unmangled by design, and `extern` reaches no
binary before M-ffi-ladder because only the `#include` verifies it) — **with one
exception, and it is the mirror of the rule** (panel 038, ratified 2026-08-12): an
`extern constant`'s accessor **is** mangled, because unmangled `int64_t
SQLITE_OK(void) { return SQLITE_OK; }` has the macro eat its own definition. A
linker name must survive the mangler; a preprocessor name must never appear outside
the accessor's body. **The double-emit
determinism test stays green at all times**, and the emitted C never mentions
the output path.

## 8. Error discipline
Every `Diagnostic` carries `Fix`es tagged `certain | guess`; only `certain` is
machine-applicable. Golden convention: `x.hero` + `x.expected` (+ `x.fixed`
where a certain fix exists — CI asserts the applied fix compiles). Errors are
a deliverable, not plumbing: they carry everything needed to fix the program
without opening another file (design.md §4.17).

## 9. Golden discipline
`UPDATE_GOLDEN=1` never turns a red test green without the diff being read and
quoted in the commit body. It is **forbidden in `tests/golden/check/`** and in
`tests/golden/ir/`. The assistant writes all cases; each milestone's 5
adversarial cases stay marked `# UNVERIFIED — pending debrief` until the author
reads them in `/learn`; bulk regression cases are labelled as such. The marker
keeps its wording — it is in 39 files and CLAUDE.md §14 does not rewrite a
record.

**A NEW SURFACE FORM LANDS IN EVERY TOOL THAT READS THE LANGUAGE, AND THE
FORMATTER IS THE ONE THAT LIES QUIETEST** (author instruction 2026-09-02, after
`as` shipped into six consumers one at a time and every miss was found by an
instrument rather than by the assistant). A form is not landed when the parser
accepts it. The tools that re-write or re-print a program each hold their own
copy of what the language is, and one that has not learned the new form does not
error — it **drops it**. Measured the day the rule was written: `heroes fmt`
printed a `use` line as `"use " + name`, so `fmt --in-place` deleted
`as near_scale` from a working program and the next build could not find the
module; `--dump-ast` did the same thing an hour later, and the token dump — one
layer down — was fine all along. **design.md §4.15 is what makes the formatter
the worst of them**: the canonical form exists so that *"any textual difference
between two versions is SEMANTIC"*, so a formatter that quietly normalises a form
away is not a bug in a tool, it is the one instrument in this repository whose
failure makes every diff untrustworthy.

**AND THE GUARD THAT WATCHES THE FORMATTER WAS BLIND IN THE SAME PLACE**, which
is the finding worth more than the rule above it. `heroes fmt` already refuses
its own output when that output *"holds a different tree"* — and it compares the
two trees by **dumping** them, with the very printer that had also not learned
`as`. Both renderings dropped the word, so they agreed, and the guard reported
the same tree while the formatter was deleting one. One omission, two consumers,
and the second was the instrument watching the first. **A self-check that
compares two RENDERINGS can only see what the renderer carries**, so the printer
walked above is not one item on the list among five: it is the item the list's
own enforcement rests on. `selfhost/cli/syntax_cmds.hero` now hands that guard
the exact pair the defect produced and asserts it refuses, which fails on the day
the dump goes quiet again.

The list to walk, and it is short enough that there is no excuse: **the
formatter** (`selfhost/print/fmt.hero`), **every `--dump-<stage>` printer**
(`selfhost/print/dump.hero` and the IR and scope printers), **`heroes mutate`**
(a form it cannot re-print is a form it silently declines to mutate, so the rate
flatters itself), **the diagnostics that quote a program back** (§8's `Fix`
replacements above all — a `certain` fix built from the wrong half of a new form
is machine-applied into a program that does not parse), and **`heroes measure`**
where the form has spec text. What the walk owes each one is a test, not a
reading: the formatter's is a round trip — `format(parse(text)) == text` for the
form and for the shape beside it — and the printers' is one line of expected
output.

**Every diagnostic is annotated in the source that provokes it** — `#~ <code>`
for this line, `#~v <code>` for the next — *in addition to* the `.expected`
snapshot. rustc's rule and rustc's reason: the redundancy exists because
snapshots are auto-generated and absorb mistakes, and because the annotation
shows where the span points without opening a second file. It is what turns the
paragraph above from a convention into an invariant: a regenerator can rewrite
`x.expected`, but it cannot invent an annotation in `x.hero`. A **fixed defect
gets a case named after it**, carrying symptom, cause and date (Go's
`test/fixedbugs`); every **verifier check has a test that makes it fire**
(LLVM's `test/Verifier`); and an invariant that must hold on every accepted
program is asserted over `heroes mutate`'s corpus rather than over cases somebody
thought of (`llvm-opt-fuzzer`'s `verifyModule`).

## 10. One command
Any new capability is a `heroes` subcommand or flag. Never a second binary,
never a script, never a Makefile. **The one declared exception has expired and is
spent** (M-bootstrap-archive, 2026-08-19): `cargo build`/`cargo test` built the
compiler until the fixpoint, and the way in is now one clang line over
`seed/heroes.c` — `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
3.4 s, measured. What runs the tests is the one command: `heroes test
selfhost/main.hero` for the compiler's own, and `heroes run
tests/harness/main.hero -- <compiler>` for the net.

**The stopping rule** (panel 016): a capability enters the surface only if the
fixpoint invocation, the golden harness or the Part 11 harness must type it, or
it has a measured Part 11 effect. Its shape is then mechanical — a **subcommand**
if it answers a different question (a different artifact class), a **flag** if it
changes how one question is answered about the same input, and **nothing** if two
existing invocations already compose to it. A new top-level verb needs a *proven
overload* of an existing one, which is what every recorded split was (`git
checkout` → `switch`/`restore`, `go get` → `go install`) — never a new capability.
**The contract**: the artifact on stdout, diagnostics on stderr; exit 0 clean · 1
the input has diagnostics · 2 the tool could not run. Inspection is `--dump-<stage>`
(§3.5's own list); `--json` says *how* to print, never *what*; a mutating flag is
`--in-place`; `--emit-c` is an output, not a dump. One argv table parses and
prints the help, so they cannot disagree.

**There is no fourth slot, because there is no fourth input class** (author
decision 2026-08-15, recording what panel 056 adopted). The three shapes above all
answer *what does this capability do to the inputs the tool already has* — a `.hero`
file, argv, and the machine's environment. A **per-project file** is none of them:
it is a new class of input, and §10 as written does not admit one. That is the rule,
and it refuses a project file more cleanly than design.md Part 6 could — 056 refused
its Part 6 row precisely because the falsifier such a row owes becomes constructible
the day `heroes add` exists, which §3.5 already promises. Two things follow and both
are the point: **the refusal is not permanent**, it is conditional on 056's three
return conditions (one key not two · no string in the file that also appears in a
`.hero` · one named binding `package` plus one command line cannot build), which are
its only amendment path; and §9's `#~` invariant is *evidence for the rule rather
than a second job* — the golden harness collects `.hero` only, so a new input class
would arrive with its diagnostics exempt from the one invariant a regenerator cannot
forge, which panel 020's historian already refused for `unsupported`.

**And never a second binary** — which is what CLAUDE.md §6's retired *"a separate
package binary"* clause was reaching for, without the false premise it carried
(nimble ships *with* Nim, so it was never the contrast that clause claimed).
`heroes add`/`heroes fetch` are subcommands when they arrive (design.md:772 — it
said `:637` until 2026-09-02, when a panel 099 judge checked it and found the
control-flow-as-blocks bullet there instead; §11's expiring premise, in the one
shape the dead-citation instrument cannot see, because a line number is not a
path).

## 11. Language and conventions
**Everything written is English** — code, comments, docs, commits, verdicts,
**and the author's own words when this file quotes them**. Conversation with the
author is Italian, **in the familiar second person** (author instruction
2026-09-05: *"and use tu with me"*, given in one parenthesis while answering two
other questions — Italian's formal *lei* had been the default for a month and
nobody had asked for it; the familiar form is the register of somebody working
beside you rather than reporting to you, which is what this project is),
**and always in plain words** (author instruction 2026-08-15:
*"always explain things to me in this simple language, in plain words, because
sometimes I have a hard time understanding it"*). This is not a
register for summaries and status reports — it is **every** explanation, including
the middle of a working session, including a defect's cause, including why a panel
ruled as it did. The rule the `/where` skill already states is the rule
everywhere: **assume zero compiler knowledge**, name the thing before the term for
it, and say what a change means for a program somebody writes rather than for a
module. A sentence the author has to re-read is a sentence that failed, and the
author is the one person this project is being built for — §1.1 makes their
comprehension **the objective**, so an explanation they bounce off is the
objective missing, not a communication style.

**And plain is not flat** (author instruction 2026-08-16: *"I want you a bit more
excited when you tell me the good things and the bad ones too — we are making a
new language"*). The register the author asked for is plain **and alive**: when
something works, say so like it matters, because it does — a defect that printed a
false number at exit 0 and now stops on the author's own line is a *win*, not a
line item. When something is bad, say that with the same force and no cushioning:
a wrong premise in a brief, a rule that would have broken a working program, a
number that turned out to be invented. **The two halves are one instruction** — an
assistant that only gets loud about good news is a flatterer, and its enthusiasm
stops carrying information. What this does not license is inventing either: the
feeling attaches to a measurement, never to a hope, and *"this is huge"* about
something unmeasured is worse than a flat sentence, because it spends the author's
trust on nothing. The excitement is for what was **run**.

Two things this does **not** relax. Everything **written into the repository**
stays English and stays precise (this section's first line): a journal, a panel
file and a commit body are records, and a record that trades exactness for
warmth is a record that will be wrong later. And plain does not mean vague — the
numbers, the file names and the measurements still belong in the sentence, because
*"it was slower"* is not plainer than *"it took 0.94 of the time"*, it is only
emptier.

**A QUOTED AUTHOR INSTRUCTION IS WRITTEN DOWN IN ENGLISH** (author instruction
2026-09-04: *"I do not want anything in Italian … I speak Italian to you because
it is comfortable for me, but the texts are all in English, since this is a
project that will become open source"*). Every rule in this file is anchored to
the author's own words, because a quotation binds where a summary does not, and
those words arrive in Italian. Until this instruction they were written down as
spoken, on a doctrine `docs/book/README.md` states in one line — *quoted speech
is not an artifact* — which the reasoning notes' README then cited as precedent
for its own carve-out (that directory was retired on 2026-09-04, §14). **Measured 2026-09-04, that loophole had let 241 Italian
quotations into the repository, 19 of them in this file** — in the section whose
first line says everything written is English. The doctrine is retired in all
three places: quoted speech is an artifact like any other. What is quoted is
what the author **meant**, in English, rather than a literal gloss of the Italian
— an instruction is a ruling, not a specimen. Nothing is lost: the original
stands in the git history and in the dated records, which is where evidence
belongs and which §14 forbids anybody to rewrite. **The records are therefore
not translated**, and they hold about 210 of those 241; 40 commit bodies carry
the same words and cannot be edited at all, so a translated record would
disagree with the commit that made it.

**Two declared exceptions, and they are a class rather than a list: a
translation that is itself a deliverable.** Everything above governs the
repository's **working** text — the rules, the records, the code. A text written
for a reader in that reader's own language is a product decision, and it is
named here so the rule above cannot be read as refusing one.
- **The two books**, M-journey-book and M-guide-book (author instruction
  2026-08-11), in **Italian and English**, neither a machine translation of the
  other. The author studies from the Italian, so where the two diverge the
  Italian is fixed to be clearer rather than the English to be more faithful —
  and both are in the plain register of the `/where` skill, which assumes zero
  compiler knowledge.
- **The Italian edition of the site**, born 2026-08-18 by author instruction
  (quoted with its date in `site/README.md` § The Italian edition): **23 pages
  under `site/src/html/it/` and their 23 wrappers under `site/src/pages/it/`,
  measured 2026-09-04**. Three things serve that edition and stay Italian
  because of it, each one Italian *data* rather than Italian prose:
  `site/CLAUDE.md`'s spelling rule, whose subject is the words themselves;
  `site/src/components/SiteNav.astro`'s table of nav labels; and the vocabulary
  `/where` speaks from, which is why that vocabulary now lives in a file of its
  own with an English header saying what it is.

**This section said *"one declared exception"* until 2026-09-04, and the second
one had been 46 tracked files for 17 days** — §11's own expiring premise (below),
in §11, about §11. The exceptions are a class now precisely so that the third
one does not have to come back here to be counted.

`Heroes` in prose, `heroes` for the binary, `.hero` for files.
ASCII-only syntax. Bowie belongs in prose and
packaging, never in error text or library names; the site's register and its
rules live in `site/README.md` § Style guide.
**Code is written to be read** (author instruction 2026-08-04): files stay
short and single-concern — split a module before it passes ~300 lines; every
file opens with a module doc stating its role and citing its design.md
sections; comments teach the invariant and the why, never the diff. The
author must be able to open any file and read it without drowning.
**The ~300 governs what a reader must hold in their head, so it binds the
compiler's own code and not its tests** (author decision 2026-08-14): a test file
is read one case at a time and a case is self-contained, which is why
`surface.rs` at 1164 lines is legible and `archive/bootstrap-rs/heroes/src/emit/ffi.rs` at 573 was not. And the
number is a threshold to think at, not a limit to round: `toolchain.rs` stays at
400 by the same decision, because the cut that would take it under runs through
the cache key `link` and `runtime_object` share, and a file split against its own
seam is harder to read than a long one.

**And the threshold yields entirely where the *language* forbids the seam**
(author instruction 2026-08-16, *"go past the rule for heroes"*, answering the
port's measurement). Heroes refuses module cycles — `module_cycle` fires on the
`use` edge, whatever it carries, measured both ways — and a recursive-descent
grammar is mutually recursive by construction, so its knots cannot be split at
all: over `archive/bootstrap-rs/heroes/src/syntax/` (named `crates/heroes/src/syntax/` until 2026-08-19), **`expr`+`primary`+`control`+`stmt`+
`name_stmt` is 1025 lines and `decl`+`data`+`externs`+`extern_members` is 736**,
each one module or nothing. This is `toolchain.rs`'s reason at its limit: there
the cut merely ran against a seam, here **no cut exists**, and a rule that cannot
be obeyed is not a standard but a lie. Panel 031 R6 chose the cycle refusal as
*"the reversible direction"* and priced relaxing it against
M-separate-compilation's topological order; the port supplies the other half of
that trade and the answer is that the file rule yields, not the language.
**What is owed in exchange is what the ~300 actually protects**: a knot carries a
module doc naming its cycle, its entry points and which function calls which, so
a reader opens it and finds a map rather than drowning.

**A narrowing asks the value, never the world** (author instruction
2026-08-12, sweep 001). A filter, an allow-list of kinds, or a `_ =>` arm is a
decision, and its correctness rests on something. Rest it on a fact about the
value in hand — *this* expression's extent, *this* declaration's fields — never
on a premise about the world around it ("a multi-line list is the only value
that does not end where it started"; "nothing reads this payload"). A fact
about the value cannot expire. A premise about the world expires **silently**,
and the comment justifying it goes on reading as correct, because the argument
is still valid and only the premise died. Two of sweep 001's twenty were
exactly this, and both had been read and agreed with — which is why a
convention about comment style would not have caught either. Where a premise
is unavoidable, two things are owed: write it as a **falsifiable claim**, not a
justification; and give it **a test that fires when it dies**, whose failure
message names what depends on it (`a_declared_type_cannot_contain_a_type_parameter`
is the shape — one premise, three dependants in three modules, one test). And
put the fallback in the **loud** direction: `_ => hero_unreachable()` beat
`_ => false` by a whole class of defect at M5c, and D3 paid for the same lesson
twice.

## 12. Precedence when artifacts disagree
Spec beats compiler (the compiler has the bug). Measurement beats opinion —
including the author's and the panel's: comprehension is the objective (§1.1)
and it is measured, not asserted. **And a refusal is held to the same standard as
a feature**: a design.md Part 6 row must name the program or the compiler fact
that would make it wrong (author decision 2026-08-12, panel 039; the rule's home
and its argument are Part 6's own preamble). Principle 0 binds what *enters*, so
without this a permanent rejection was the one design act under no burden of proof.

**Robustness wins** (the principle is design.md **§1.12**; this is its operational
half). A Heroes program must not segfault and must not corrupt
memory — that is a **goal of the language**, stated by the author 2026-08-14, and
where a choice runs toward it, it beats every other criterion in this file:
elegance, token cost, ergonomics, the size of the compiler, and speed. This is the
mirror of §13's performance line rather than an exception to it: performance is
not a goal *and* not a licence, while safety is a goal *and* a tie-break. The
`heroes mutate` corpus and `--sanitize` are how it is measured rather than
asserted, and the rule does not suspend Principle 0 — a form still enters only if
the compiler needs it or it serves the thesis. What it decides is the case where
two admissible forms disagree, and one of them can be made to crash.

**It reaches furthest at the C boundary, which is where the language's own
guarantees stop** (design.md §1.11 — everything comes from C, so everything a
program touches arrives through §4.19). The author's instruction is that the FFI
be *complete and bug-proof*: complete, because a library Heroes cannot bind is a
library the author must leave C code around for; bug-proof, because a binding is
the one place where a Heroes program can reach an address nobody checked. Panel
053 measured the shape of that: `to_str` on a null `cstr` is a **clean abort**
(the runtime guards it), and a null `cstr` handed straight to another C function
**was a SEGV in libsystem** — so the hole was not where three of its four options
were looking. **That second half stopped being true and the sentence is corrected
rather than deleted** (author instruction 2026-08-24, panel 089's spec-warden;
measured twice that day, by that seat and by the coordinator): the same program
is now `panic: a null `cstr` was passed to a C function`, **exit 134**, because
`guard_cstr_arguments` wraps every `cstr` argument on its way out. The date is
kept because the measurement was right when it was taken; the closure is added
because **a contract that states a danger the compiler has already shut will fund
the wrong decision next time** — and it nearly did: panel 089's whole payment,
the −15 tokens that deleted `spec:229`'s null-test advice, is §1.4 redundancy
**paid back** only because the compiler is loud in both directions. Under the
stale sentence that removal would have looked like deleting a warning.

## 13. Where not to go
Performance (a non-goal, never a justification — **and never a licence either**,
and see §12's robustness rule, which is its mirror and outranks it:
author instruction 2026-08-12, *"performance is not a goal, but it must not be a
ceiling either"*. The rule forbids reaching for speed as a **reason**;
it does not make slowness acceptable as a **ceiling**. Where a cost stops a
program the closure list needs from running at all, that is §1.0 compiler-need
and it goes to the panel, not to this line). A standard library. Anything
in design.md Part 6. Anything in Part 7 before the closure list compiles
itself.

## 14. Documentation duty + git
A step is not done without a commit (`M-<name> step <k>: <what>`). Per milestone:
journal (3 sections) + one story beat in `docs/book/beats.md` + a tag (pushed
`--follow-tags`) + ROADMAP status. Per decision: a DESIGN-LOG line.

**AND THE RECORD SAYS WHOSE IDEA IT WAS. WHERE IT WAS THE AUTHOR'S, IT SAYS SO**
(author instruction 2026-09-05: *"in general I would like it to stay in the
records when I have an intuition, so that when this becomes public it does not
read as a merely vibe-coded project but as one the author co-authored"*). This is
not courtesy and it is not decoration. Every other rule in this file exists to
make the record **checkable**, and a record that attributes to the assistant a
finding that came from the author is false in exactly the way those rules forbid —
it just happens to be false about a person instead of about a number.

**What provoked it, the same day.** The thread guard was measured at +6.6% and the
coordinator was treating that as one cost to haggle over. The author asked a
different question — *"if the cost is only at compile time it is not a big
problem; if it is at runtime, then yes, let us look for the compromise"* — and
answering it meant saying **where** the cost is paid, which meant asking where a
foreign thread **enters** rather than where the corruption shows. That question
moved the guard from three wrong placements to the right one, and
`docs/measurements/018`'s first draft recorded the answer with no author in it.
The attribution went in only because the author asked for it.

**It cuts both ways, and that is what keeps it honest.** Crediting the author for
something the assistant found would be the same falsehood wearing the other sign,
and it would be worse, because it is the flattering direction and nobody would
check it. The rule is accuracy: name the author where a question, a correction or
a refusal of theirs is what produced the finding, name the panel seat where a seat
found it, and name nobody where the work was ordinary. A record nobody can trust
about people is a record nobody will trust about numbers.

**A reasoning session leaves no note of its own** — a conversation whose work is
questions about the project, with no file of code, spec or design modified. It
had a directory of notes from 2026-08-10 until **2026-09-04**, when the author
retired it after seven (*"I want to remove it and turn its content into DONE, or
DECIDE plus a panel where needed, or even into ROADMAP milestones where needed.
All this work goes in the direction of simplifying the docs directory"*). Where
the parts go is where they went anyway: what the session **settled** is an entry
in `docs/work/DONE.md`, what it **left open** is an item in `docs/work/DECIDE.md`
if it asks what should be true or in `docs/work/SCHEDULED.md` at an open
milestone if it is work, a **concept** it explained is an entry in
`docs/glossary/`, a **question worth re-asking** is a line in
`docs/learn/LEARN.md`, a **change to the language** is `/panel`, and a **decision
taken** is a DESIGN-LOG line. What the directory alone had held was the path
between those, and the path is the git history. Measured on the day it went:
**31 of its 32 hand-offs had already landed** in the artifacts above, and the
last live thing it carried — a table two documents cited by line — moved into the
sitting that cited it. The milestone-close checklist lives
in `/step` — its only copy. The repo pushes to `origin`
(github.com/heroes-lang/heroes, moved there from github.com/giuseppearici/heroes-lang
on 2026-08-30 — every old link redirects, and the one act that would kill those
redirects permanently is creating a repository at the old name, so that name is
never reused). Hard stops that remain: publishing the
site or anything else outward-facing, and destructive ops.

**NEVER `git add -A`, `git add .`, `git add -u` or `git commit -a`. A commit
stages ONLY the files this conversation touched, each one named on the command
line** (author instruction 2026-09-03, categorical). The reason is the working
tree, not tidiness: more than one session works in the same checkout at the same
time, and their half-finished files sit side by side in one `git status`. A sweep
stages everything it finds, so it commits another session's unfinished work under
this session's subject — a change nobody in this conversation read, tested or
meant to record, and one that the other session then finds gone from its tree.
Measured the day the rule was written, while it was being written: `git status`
showed **8 dirty paths, 6 modified and 2 untracked, all under `selfhost/` and
`tests/harness/`, and this conversation had touched none of them** — a `git add
-A` here would have shipped eight files of somebody else's compiler work inside a
one-paragraph edit to this contract. The same reasoning forbids `git stash` in
the shared tree: it takes every session's changes, not one's. Before staging,
`git status` is read and every path about to be added is one this conversation
edited or created; a path that is dirty and unexplained stays out of the commit
and is reported to the author instead.

**Milestone identifiers are names, not numbers** (author instruction 2026-08-12;
panel 030 R7 as amended — the argument lives there). This is the algorithm's only
home; `docs/ROADMAP.md` § The names carries the map and cites this.
- **Two words**, `M-<what-it-delivers>`, hyphenated and lowercase after the `M-`;
  the tag is the same string lowercased (`m-ffi-ladder`).
- **Name the deliverable, never the area** — the area must stay free for the second
  milestone that touches it, and one already exists: `M-module-namespace` and
  `M-separate-compilation` are both about modules. A one-word name appropriates a
  topic, and an identifier must make no claim a later milestone can falsify.
- Prefer a phrase the ROADMAP entry or the milestone's own journal slug already
  uses over an invented one.
- **An id is never renamed once it is in the record.** A milestone that changes
  shape gets a *new* id; the old one is retired in § The names.
- **Order lives in the ROADMAP's § The chain table and nowhere else** — the id
  claims nothing about position, and no other heading may repeat it (which is why
  that file's milestone sections carry no numbers: they carry the order by their
  position and the table carries the numbers). `git tag --list --sort=creatordate`
  gives the chronology.
- An id never reaches a diagnostic or any user-visible output (§8, asserted by
  `archive/bootstrap-rs/heroes/src/emit/tests/gate.rs`).
- **Appending to a dated record uses that record's vocabulary**, with the new name
  in parentheses on first use — `scored at M8a close (M-module-namespace)`. The
  record is never rewritten: `docs/panel/`, `DESIGN-LOG.md`, `docs/journal/`,
  `docs/measurements/`, `docs/work/DONE.md`, `docs/book/beats.md`,
  `tests/golden/`, every commit subject and the twelve legacy tags keep the numbers.
  (The list named a defects directory until 2026-09-03; its seven files are six
  entries in `docs/work/DONE.md` and one in `docs/work/DEFECTS.md` now, §3. It
  named a reasoning directory until 2026-09-04; its seven notes are seven entries
  in `docs/work/DONE.md`.)

**Release tags are a third namespace, and a release is a commit, never a
milestone** (author decision 2026-09-07, from the author's own question: *"let us
work out how to handle releases and what number to start from: GitHub releases
first and then the channels? and how do we number them? are we at 0.1, 0.9,
1.0, 1.1?"*; the measurements and the six choices, each put with a
recommendation and all six taken, are the `DESIGN-LOG.md` row of that date). The
milestone tags above are `m-*` and widen the CI matrix; the site's are `site-v*`
(`.github/workflows/release-site.yml`); a release of the language is `vX.Y.Z`,
and nothing else starts with `v`.
- **The number is `X.Y.Z`, and `heroes --version` prints it.** It is one constant,
  `VERSION` in `selfhost/main.hero`, and that constant is also the first line of
  every emitted C file and the build cache's fingerprint. It moves **only in the
  commit that carries the tag** and stays there until the next release: between
  releases the binary says the last release and `main` is ahead of it. There is
  no `-dev` suffix, because every move of the number regenerates the seed one
  generation further than usual and rewrites the first line of every file under
  `tests/emission/` and of every `.expected` under `tests/golden/emit/` (213 and
  6 on the day this was written; the second store was found by the net, not by
  the grep, which had printed it into an output nobody read to the end), and
  paying that twice per release buys one word.
- **While `X` is 0, `Y` moves when the spec moved and `Z` when it did not.**
  `spec/heroes-spec.md` is the whole language, by its own first line, so *the
  language changed* means `git diff vA vB -- spec/heroes-spec.md` is not empty:
  a command, not a judgement. What 0.x promises is one sentence, and it is the
  whole promise until the gate: *before 1.0.0 the language may change between
  minor versions; a patch version changes no sentence of the spec.* `1.0.0` is
  M-publication-gate's, written together with the compatibility paragraph that
  entry owes, because in every language people install `1.0` means *your
  programs keep compiling* and nobody has written that promise for Heroes. The
  project's own word `v1` (design.md §1.0: the compiler compiles itself, reached
  2026-08-18) is a milestone's name and not this number, and the two are spelled
  apart on purpose wherever a stranger might read them side by side.
- **A release is the author's act, on a clean `main`, resting on the last closed
  milestone**, and it does not coincide with one: the chain closed 37 milestones
  in 35 days. The tag is annotated, and its message is the one paragraph a human
  writes; the workflow assembles the rest.
- **The instrument is the CI, not this list.** A `v*` tag runs the three-platform
  matrix like an `m-*` tag, asserts on every leg that the seed-built compiler's
  `--version` is the tag's number, asserts that a spec that differs from the
  previous release moved `Y`, and only then creates the GitHub Release: the tag's
  own source archive, with the seed inside it where `seed/README.md` says a seed
  belongs, the notes, and **no uploaded binary** (`DESIGN-LOG.md:539` refused
  prebuilt binaries as a decoy without clang, and a release is not where that
  refusal expires). The notes are the annotated tag's message, the milestone
  tags between the two releases, and the spec's diff between them, which is the
  language's changelog because the spec is the language. No CHANGELOG file: it
  would be a second copy of the record.
- **The channels pin the tag** (M-install-channels): a formula, a manifest, a
  flake and an image name `vX.Y.Z`'s archive and its checksum and run the one
  clang line. The repository is private, so a release made today is the private
  rehearsal that entry asks for; every release made before the gate becomes
  visible on the day the gate lifts, so its notes are written for a stranger
  from the first one.

## 15. Working instructions that lived in the assistant's memory until 2026-09-03
Each of these was an author instruction kept in a per-machine memory file until
the author said, for the second time, that nothing lives there (§3). Dates and
words are the originals. Anything the sections above already state is not
repeated; what follows is what they did not.

- **Every decision put to the author comes with a recommendation and its reason**
  (2026-08-23, *"for decisions, always tell me in plain words which option you
  recommend, and why"*). A neutral list of options hands the author work the
  assistant has already done. The recommendation rests on a measurement or a
  verified fact, never on a hope; the decision stays the author's.
- **A long compile is when the open decisions are proposed** (2026-09-03, *"I like this idea that while you wait for the result of a
  compilation you put the decide items to me"*). While a build, a suite or a measurement runs for minutes, the
  open items of `docs/work/DECIDE.md` are verified against the repository and put
  to the author, each with a recommendation. A silent wait wastes the author's
  time twice, and deciding costs no CPU, so it distorts no measurement in flight.
- **Every progress update carries a percentage and its breakdown** (2026-09-02,
  *"when you update me, write what % you are at as well"*). The number is counted against
  the author's ask, not against invented work, and comes with the parts that
  produce it — done, in flight, not started — so the author can disagree with it.
- **Never slow the compiler down** (2026-08-23, in capitals: *"whatever you do, do not make the compiler's
  performance worse, that would be a tragedy"*). A change that
  makes the compiler slower does not land on a hunch: time before and after with
  `/usr/bin/time -p` (zsh's own `time` prints no `real` line, and a whole
  comparison was once lost to that), look at the machine's load first (a stray
  `ffmpeg` once turned 188 s into 282 s), and report the number. The verification
  loop is already the slowest thing in the project — measured 2026-08-23: a full
  emission 16m51s, `heroes test selfhost/main.hero` 20m35s — and every added
  percent is paid at every rebuild until somebody stops verifying, which is the
  real cost. §13 says performance is not a goal and not a licence; this is the
  same rule seen from the compiler's side.
  **AND THOSE TWO NUMBERS ARE THE COLD ONES: say which you measured** (added
  2026-09-05, after a session budgeted 37 minutes for a verification that took
  64 seconds). With the per-module cache warm and three files changed,
  `heroes build selfhost/main.hero` is **28 s** and `heroes test
  selfhost/main.hero` is **35 s**, both measured with `/usr/bin/time -p` on this
  Mac. The 2026-08-23 pair stands and is not deleted: it is the cold tree, which
  is what CI and a fresh clone pay. The reason to carry both is that the cold
  number is the one that funds *"do not make it slower"* and the warm number is
  the one that decides how a session is planned — a rebuild believed to cost 17
  minutes gets batched, deferred, and sometimes skipped, and skipping is the
  failure this whole paragraph exists to prevent.
  **AND THE LOAD THAT RUINS A MEASUREMENT IS THE ONE THE MEASURER MAKES: WHILE A
  CLOCK IS RUNNING, THE MACHINE STAYS STILL** (author instruction 2026-09-06,
  *"write this in CLAUDE.md"*, given after a session spoiled its own timings
  three times). *"Look at the machine's load first"* is above and it was
  **obeyed** — 1.89, nothing heavy, checked before the first run. It looks at an
  **instant**, and the load that ruined these runs did not exist at that instant:
  the session created it afterwards, by building, by running three suites, and by
  syncing the Windows box while its own comparison was in flight. One arm then
  read **937.21 s of wall against 33.68 s of CPU** where the other arm's
  comparable run was 36 s — 3.6% of one core, which is memory pressure from two
  975,000-line compilations at once and not a fact about the code. Three
  obligations follow. **Nothing else runs while a timed run does** — not a build,
  not another suite, not a container, not an `scp`; a remote box is not an
  exception, because the push, the copy and the ssh all run here. **The tell is
  the ratio, so read it**: `real` far above `user` + `sys` means the process was
  waiting, not computing, and the run is discarded rather than explained. And
  **a discarded run is written down with its reason**, because a measurement that
  quietly drops its inconvenient half is worth less than none.
  **The same discipline binds the TREE, and that is the worse version of it**:
  the same session edited `runtime/` while the net was running over that
  `runtime/`, so some checks used the old objects and some the new. That run was
  killed and redone. A suite reading the tree owns the tree until it exits.
- **No em dashes and no machine-written patterns, on the site and in the chat**
  (2026-08-25, *"NO AI SLOP — get rid of all the —"*). The rulebook is `site/README.md`
  § Style guide and `site/CLAUDE.md`; the chat half is that the assistant's
  Italian replies carry no em dashes either. Two exceptions, both evidence:
  compiler output shown verbatim, and code copied byte for byte.
- **A measured number never goes inside an image** (2026-08-31, on the README
  banner: *"those age fast"*). Artwork carries identity and
  structure; the text beside it carries the numbers, where `grep` and a re-count
  can reach them.
- **Committing and tagging is routine; pushing `main` is asked for.** The site
  lives on the same branch, `site/public/CNAME` names `heroes-lang.org`, so a
  push of `main` can publish it and carries every other session's site commits
  with it (§14's hard stop). Commit and tag locally, then ask, saying how many
  site commits would go along; on a yes, `git push --follow-tags`.
- **A whole milestone may be asked for in one `/step`, and a choice surfaced
  twice is a question** (2026-08-04, *"carry on until m2 is finished, and always accept your
  own recommendations"*; sharpened 2026-09-02, *"carry on without asking me"*,
  after one trade had been put to the author in three recaps running). Chain the
  steps, close the milestone, decide the delegated questions with the recommended
  resolution as the provisional default, and say which way it went — once.
- **Re-read the chain and the log immediately before writing a scheduling fact**,
  never from the copy read at session start. On 2026-09-03 a peer session closed
  M-robustness-guards and opened the next milestone while a plan was being
  written, and a 22-anchor edit to `docs/ROADMAP.md` failed twice on sentences
  the peer had rewritten in between. For a multi-anchor edit to a shared record:
  script the pairs, assert each anchor matches exactly once, dry-run, then apply.
- **Comprehension questions are never put through the question widget** (*"you have to show me the
  line, otherwise I cannot know"*): the snippet in a fenced block, the
  options lettered, the whole batch in one message, answered in one reply
  (`1b 2a 3c`). The widget hides the code the question is about.
- **A panel-gated step asks once and expects yes** (2026-08-19: *"convene the panel
  and finish"*). §4 makes the sitting mandatory and the author has said its cost
  is worth paying; ask once per milestone, then convene without asking again,
  choosing only the seats whose input differs.
- **Panel briefs keep every command short and give one copy per judge** (panels
  087 and 088): four of five seats died on a 600 s watchdog rebuilding the
  compiler, and two seats measuring in one shared checkout contaminated each
  other. The seed builds in ~4 s and is usually enough; nothing longer than ~3
  minutes in one command; a frozen snapshot every seat copies from. And before
  convening, `grep` design.md, `docs/panel/` and `DESIGN-LOG.md` for the answer
  that may already exist — panel 088 re-derived panel 037.
- **Shipping uncommitted files to the Windows box: `COPYFILE_DISABLE=1 tar
  --no-xattrs`.** macOS's tar writes `._name` AppleDouble entries that the
  harness globs as programs (172 of them on 2026-09-03, seven false failures).
  Committed work goes by `git push win main:main`. The box is
  `docs/environment/windows/WINDOWS-MACHINE.md`: paid by the hour, usually off,
  and only the author can start it — ask, and do the machine-free work meanwhile.
- **Measuring in a copy of the tree**: a copy without `.git` cannot run the net's
  `records` suite (it reads commits and tags). Measure single suites in a copy;
  gate a commit on the real tree's net.
- **A background monitor is never asked about; the answer is always yes**
  (2026-09-04, *"put it in Claude's settings never to ask me again whether I
  want the monitor: the answer is always yes"*). A monitor is how a CI run, a
  remote box coming up or a long build wakes the session instead of being
  polled, and every prompt for one is a wait the author has to click through.
  The permission lives in `.claude/settings.json`, checked in so it travels with
  the repository rather than with one machine, and this line is why it is there.

## Commands
```
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes   # the compiler, from C alone (2.7 s)
./heroes build selfhost/main.hero -o heroes-next             # the compiler, from Heroes
./heroes test selfhost/main.hero                             # its own tests (588, 2026-09-07; 48 s WARM)
./heroes run tests/harness/main.hero -- ./heroes             # the net (1610 checks, 2026-09-07; 12m55s on a desktop in use, ~13m with runtime/ changed)
./heroes test tests/harness/main.hero                        # THE NET'S OWN TESTS (119, 2026-09-07; 26 s) — the third suite

./heroes doctor                                              # toolchain check
./heroes <cmd>                                               # the one command
```

**On Windows the first line is `seed/README.md`'s**, which adds
`-Wl,/STACK:67108864` and states why; this block does not repeat the flag,
because this file's own preamble says each rule is written in exactly one place.
**Since M-thread-stacks the CI leg builds BOTH** — the flagged seed it then uses,
and the line printed above, which it asserts still compiles `selfhost/lexer.hero`
— so the two cannot drift in silence again. That step asserts rather than
reports, deliberately: `docs/work/SCHEDULED.md` recommended a reporting step and
a step that only reports is a rule performed by nothing, which is §3's story
about `DECIDE.md` reaching 391 KB told a third time. **Measured on the box
2026-09-06**: the plain line's 1 MB now builds a compiler that emits the whole of
`selfhost/main.hero`, so the flag is headroom rather than the thing holding the
build up — which is exactly the state in which a silent divergence goes unnoticed.

**AND THE 2026-09-07 NET NUMBER IS A DIRTY CLOCK, SAID SO RATHER THAN QUOTED
CLEAN.** 13m05s is `real 785.39` against `user 349.86 + sys 100.27` — 57% of
the wall was waiting, and the reason was on the same machine: Chrome, the
window server and a chat client held about three of load throughout, because
the author was using the desktop. §15 says a run whose `real` sits far above
`user` + `sys` is discarded rather than explained; this one is kept because it
is a PASS/FAIL gate and not a measurement, all 1609 checks are green, and the
duration is written down as the shape it is. **The number to fund a *do not
make it slower* argument is not this one**, and the next session that wants one
takes it on a still machine.

**And the three numbers above were a milestone stale until 2026-09-06**: this
block said 108 for the third suite while `docs/ROADMAP.md` said 112, because
M-isolated-threads' close re-measured the ROADMAP and not the contract. A number
that lives in two places drifts in the one nobody re-reads.

**IT HAPPENED AGAIN THE SAME DAY, AND THIS TIME BOTH COPIES DRIFTED TOGETHER**
(M-declared-freer step 1, 2026-09-06). `e61fec3b` repaired defect 015, added
harness rows and moved two counts — **583 -> 584** and **1578 -> 1582** — and
**its own commit body states both**, in the sentence *"Three suites green: the
compiler's own 584, the net 1582, the net's own tests 113"*. Neither this block
nor `docs/ROADMAP.md` was touched, so the repair that knew the new numbers
shipped beside two documents still printing the old ones. The lesson above says
a number drifts in the copy nobody re-reads; the sharper one is that a commit
which MEASURES a number and writes it in prose is exactly the commit that can
afford to put it where the number lives, and the ten seconds it costs is the
whole difference between a record and a rumour.

**There are THREE suites and this block named two until 2026-09-02**, when the
third was found red at `d08062f` and had been red for six commits —
`suite_layout`'s ceiling assert reading 308 against a table that said 315, after
`9599d97` re-baselined it and stated both new numbers in its own diff two screens
above the assert. `DESIGN-LOG` recorded the identical failure on 2026-08-31,
*"a suite that had never been run"*, and left it with no caller: the command
lived in `docs/ROADMAP.md`'s verify block, which nothing obliges anybody to open.
It is in `/step`'s § 2 as well now. **The third suite is the one that goes red
when an instrument's pinned number stops matching what the instrument reads** —
the other two test the language; this one tests the tests.

**THE SUITE YOU DID NOT EXPECT TO MOVE IS THE ONE WORTH RUNNING, AND THE MOMENT
TO RUN IT IS AFTER THE LAST EDIT RATHER THAN AFTER THE LAST INTERESTING EDIT**
(author instruction 2026-09-04). The paragraph above says which suite went red
and left the lesson *run the third suite*, and that lesson is too small: it
names the suite that was missed last time. On 2026-09-04 two sessions worked
this tree at once and **both missed a different one, in the same afternoon, for
the same reason** — each ran what it expected its own change to touch.
- One rewrote prose across fifteen files and ran the third suite plus `records`,
  `layout`, `lines` and `units`. It did not run `canonical`, on the reasoning
  that prose cannot break a formatter. `tests/harness/suite_records.hero` had
  one blank line the formatter removes, and **the net was red for everybody**:
  design.md §4.15's whole argument is that a textual difference is semantic, so
  a non-canonical file *inside the harness* means the instrument for that rule
  is not held to it.
- The other landed a large repair and ran the whole net, which is what found
  that failure — and its own `tests/harness/suite_layout.hero` pin, `DECIDED`
  grown to 17 entries with the assert still reading 16. **An addition is the
  direction that assert notices least loudly**: a removal takes the table below
  the number and a raise leaves it alone.
- And an edit *after* a green run is the same fault wearing a clock. `records`
  was green fifteen minutes before an item left `docs/work/DECIDE.md`, and the
  recap was already written. The move made `records/verdicts` red, correctly:
  `docs/panel/106-the-frame-is-the-sweeps-own-temporaries.md` still said its
  ratification was queued, and nothing named it any more.

The rule is therefore not a list of suites. **A ticked item is a question, not a
task** — the third bullet is what the check caught: `DECIDE.md` reaching zero
open items reads as tidiness and was the symptom, because the item was the
author's ratification standing in for the author, and the work being built is
not the work being ratified. And *"my change cannot have touched that"* is an
inference (§1), so it is either run or it is written down as a guess.

**AND THE FULL NET IS RUN ONCE BEFORE A PUSH, NOT BEFORE EVERY COMMIT — THE
NAMED SUITES ARE WHAT GATE A SUB-STEP** (author instruction 2026-09-06: *"this
step is endless, how can we be faster — maybe we do not check the whole net at
every sub-step, but only at the end"*). The paragraph above says to run the
suite you did not expect to move, after the last edit rather than after the last
interesting one, and that rule is unchanged. What changes is **which** run
carries it at which moment, and the change is paid for by a count of the day it
was asked.

**Measured over one session, 2026-09-06, M-declared-freer steps 1 to 4a**: the
full net ran **eight** times to completion at **11–15 minutes each**, close to
**two hours**. Four runs were green. Four found something — and **all four
failures were in suites that cost seconds**: `canonical` twice (a file written
and not passed through `heroes fmt`) and `emission` twice (a blessed capture
that had legitimately moved). **Not one came from the parts that cost the
minutes** — the corpus, the `run/` goldens, `mutate`, the three configurations.
Those were green in all eight.

So a sub-step is gated by **the named suites, one at a time** — `heroes run
tests/harness/main.hero -- <compiler> <name>` — plus the compiler's own tests
and the net's own tests. Together that is under a minute against thirteen, and
on the day this was measured it would have caught **four of four**. The full net
runs **once, before a push**, which is where §14's hard stop already makes
somebody stop and look.

**And the cheapest saving is not in this rule at all**: two of the four failures
were code written and not formatted, which `heroes fmt <file> --in-place` costs
nothing to prevent and a 13-minute run to discover. **Format at the moment of
writing, not at the moment of verifying.** A rule about which suite to run is
worth less than the habit that stops the suite from firing.

**And the three platforms are measured from this Mac, BEFORE the commit**
(author instruction 2026-09-03, after the program that is now `examples/ctime/`
— it was called filestat until that day, and the directory of that name is
gone — shipped a comment that named three platforms and had checked one, and
the Windows leg of CI was the instrument that read it). macOS is this machine. Windows is a real box,
`docs/environment/windows/WINDOWS-MACHINE.md`. Linux is a container of the CI
leg's own architecture, `docs/environment/linux/LINUX-MACHINE.md`, built from
the `Dockerfile` beside it — one directory per platform, by author instruction
the same day. Neither file was cited
anywhere alive until this paragraph — the Windows one sat orphaned for three
days and a memory pointing at it carried a dead path — which is why they are
named here, where the dead-citation check reads. **CI stays the judge; these two
are the hunt**, and a platform fact that has not been run on one of them is an
inference (§1), whatever the comment around it says.

**And a program that declares an `extern` runs its Linux leg under
`--sanitize`** (author instruction 2026-09-04, *"write the sentence yourself,
then convene the sitting"*, after panel 108 named the instrument; the narrow
form was recommended and taken). LeakSanitizer exists on that leg and on no
other (§7), so a leak in a C binding is invisible on this Mac in all three
configurations. Measured 2026-09-04, when `examples/ledger/` leaked 40 bytes per
refused statement: green here, red in CI, and the only instrument that could see
it was the one this sentence names. It is narrow on purpose: a program without an
`extern` cannot leak from the C side, because its own allocations are counted by
`hero_runtime_check_leaks()` on every platform, so the leak the Mac cannot see
enters through an `extern` and nowhere else — the wide form would cost 44
programs a run that concerns 8 and catch nothing more.
