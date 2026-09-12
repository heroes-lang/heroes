# Who scheduled what, and what ratified it

Every scheduling note, ratification and author decision that a chain row would
otherwise carry inside a cell, keyed **by name** so a reorder moves a number and
never a name (CLAUDE.md §14). It left `docs/ROADMAP.md` on 2026-09-12, by author
instruction, because at 334 lines it was three times the table it annotated.

Every sentence here stood **inside a cell** until 2026-09-03, where it hid the
order the table exists to carry: the longest cell measured **383 characters**
against the 91 the table's widest now runs to. They are keyed by **name**, not by
row number, because a reorder moves a number and never a name (CLAUDE.md §14).

- **M-argv-execution** — **panels 097 and 098, ratified 2026-08-30.** `sq()` is
  deleted with it, and the Windows leg of CI goes green.
- **M-package-layout** — **panels 099, 100 and 101, all three ratified the same
  day**, 2026-09-02.
- **M-selfhost-nesting** — **panel 102, ratified the same day.** What the
  milestone found was not in its own plan: the flat prefixes had been holding
  module names out of the namespace where values live, so the collisions that
  mattered were local-against-module — 0 flat and 23 nested.
- **M-corpus-coverage** — **scheduled by author instruction 2026-09-02**, and
  ordered after the nesting so that the new programs are written against the
  final layout.
- **M-documentation-site** — **taken out of chain order by author instruction
  2026-09-02**, in the same breath as the nesting and the corpus, because the
  feature it documents had just landed and the site was silent on it. It was
  planned at position 36, after M-vscode-extension. It stands at 31 because the
  table runs closed-then-scheduled since 2026-09-03; the five milestones it
  overtook keep their turn and their order among themselves. **What the table no
  longer records is the plan** — that a milestone moved is here, in a sentence,
  rather than in a row's position, which is what the author's *"I want the
  table clean"* decided.
- **M-isolated-threads** — **moved ahead of packages by author instruction,
  2026-08-25.**
- **M-held-bytes** — **§4.19's fourth case, and the reasoning a later milestone must honour is in three sentences.** A declaration-site mark for pointer retention cannot be made right, because the decision is the C function's own ARGUMENT (`sqlite3.h:4888`, `curl_easy_setopt`'s second) and the header types both spellings `const char *`; a later milestone that reaches for `kept` or `transient` on a parameter re-argues panel 124 against a measurement. The lease is a COPY and never a pin, and its release is WRITTEN and never inferred (panel 124 R3, R4): a pin aliases `.cstr()` under copy-on-write, and an inferred release is the escape analysis panel 122 refused and panel 124 priced at 1.44 MB against 81.9 MB. And the pointer lives in ONE cell (panel 125): a lease's name stands only as an argument of a call, nothing but `end_lease` writes its cell, and a runtime guard that dereferences to validate is undefined on a foreign pointer — so a later `Ty` case, a registry, or a magic word is a route already measured and refused, and the record says by how much. **What it did not close is named**: a lease C retains past the `end_lease` the program wrote, or launders through a C function that returns its argument, is undecidable, and its instrument is the Linux `--sanitize` leg.
- **M-cstr-lifetime** — **placed first and alone by author decision 2026-09-09**, chosen over the coordinator's recommended order of three milestones (the budget instrument, then the parser seam, then the interpolation implementation). Its ground is § Precedence: the defect is a silent wrong answer with memory corruption, robustness is rank 3, and a milestone is tagged only over a clean list — so nothing else can close while `docs/work/DEFECTS.md` holds 022.
- **M-interpolation-verdict** — **scheduled by author instruction 2026-09-02**, closed 2026-09-09 with the verdict and not the form. What a later milestone honours: the brace is active ONLY behind an `f`, because 211 literals in the compiler's own modules hold one and an ungated brace is a two-stage bootstrap; a hole admits ANY expression, because the narrow rule costs more tokens and refuses a `???` the spec promises anywhere; and the form is ONE AST node with the holes as children, never a payload on `.str_lit`, because 59 walks would otherwise treat it as a leaf.
- **M-interpolated-strings** — **opened and closed 2026-09-09** (panel 121 R10). What a later milestone honours: only `{{` is doubled, because a `}` in text mode is text, and a hole whose expression begins with `{` needs a space after the opener for Python's reason; the desugar is the lowering's and the printers re-print the literal verbatim; a hole's type is `to_str`'s set, refused with `to_str`'s words. The constraints it inherits, measured at the open: a new node touches **85 sites in 27 files** that enumerate `.str_lit`; `grammar_expr.hero` is at 1002 of 1002 and a helper module that calls `parse_expr` closes a `use` cycle (R5), so the parser's hook stays in the knot and the DECIDED number rises by exactly its lines with the reason written; `check/walk.hero` 1700 of 1700, `ir/flatten.hero` 1102 of 1110, `print/fmt.hero` 1136 of 1150 get one-line arms that call helper modules where a helper needs no call back into the knot. The two spec sentences R6 accepted land WITH the clause, and the clause is re-measured on today's spec first.
- **M-robustness-guards** — **opened by author instruction 2026-09-03**, ahead of
  M-isolated-threads, out of a `/decide` sitting that closed ten items at once:
  *"ratify everything … choose the most robust and complete solutions over the
  cheaper ones … favour consolidation and the better solutions, not the
  shortcuts … more history and less attention to the token … more robustness on
  every platform, do not silence errors"*. Its six steps are the six `SCHEDULED.md`
  items that name it; two of them are sittings (the FFI pointer verdict, full five
  seats; the stack guard, soundness lane). The `records/names` check found the id in
  a list before it had a row here, which is the order CLAUDE.md §14 wants.
- **M-corpus-depth** — **closed 2026-09-04.** Scheduled by author instruction 2026-09-03, *"think
  about whether to add further examples, some of them more complex too, to have
  a wider net; look at what other languages have done as well"*, from a
  plan measured and approved the same day. The author placed it after
  M-robustness-guards, which closed that afternoon, so it first stood at 34 behind
  the open M-isolated-threads. Its step 0 — the first `heroes mutate` score over
  the 35-program corpus since 2026-08-13, and the corpus leg timed alone — landed
  with the scheduling, so the "before" exists before the first program does.
  **Then step 0 found `heroes mutate` unable to read `examples/` at all**, refused
  since 2026-09-02 (measurement 014), and the author moved the milestone to 33
  and opened it the same evening — *"bring those two steps forward right now, before
  M-isolated-threads"*, with *"the gate on every branch + the full score on the
  tags"*
  for the CI question — so that the metric the thesis rests on is repaired
  before anything else is measured against it. **Two milestones are open at
  once**, in two sessions, and the table says so rather than hiding one.
- **M-core-packages** and **M-web-framework** — **scheduled by author instruction
  2026-09-03**, the same evening, out of a reasoning session
  (`DESIGN-LOG.md:537`, which carries what it measured): *"I would like
  to have every tool needed to build a web framework in the style of Rails or
  Django, or even thinner, like Go, Echo or FastAPI"*, and then, when a single
  toolkit was proposed, *"I picture several packages that combine, and then the
  web one that uses them all; the model is Go, as organisation"*. Placed after
  M-package-manager because `heroes fetch` is what makes a package a thing you
  distribute. **Reversed later the same evening** — the packages stand before the
  manager since the reorder, and the bullets below say why. **Two rows because they are two deliverables** (CLAUDE.md §14): the
  packages, and the framework that composes them. The level is Go's and Echo's —
  everything explicit — because the Rails and Django shape rests on Part 6's own
  rows, metaprogramming, dynamic dispatch and inheritance (`design.md:2382-2404`).
  **The sitting sits at the opening, not at the scheduling**, on
  M-interpolation-verdict's precedent, and the author added its sixth question the
  same night: conditional compilation — *"conditional compilation with the
  if macro that is not a macro, though, or another keyword"*.
- **The chain was re-read and reordered late on 2026-09-03, by author
  instruction** — *"let us do a big think about the roadmap and about the
  things in decide and scheduled too … let us judge whether there are steps we
  have not considered so far … then we reorder them all in a very logical order"* — out
  of a reasoning session (`DESIGN-LOG.md:539`). Three
  faults in the order were measured and each moved a row: the language's one
  scheduled ruling sat at row 41, behind the packages, the framework, QBE and
  both tools, so **51,788 lines of Heroes in 178 modules** — the size of the one
  body of that kind today, measured 2026-09-03 — would have been written
  before it; Part 7 item 1 — closures, *"v1.5, immediately after the first
  running program"*, and that program ran on 2026-08-04 — had no row at all while
  M-web-framework's entry planned its middleware around the absence; and
  M-package-manager stood ahead of M-core-packages while the packages' own step
  13 was `heroes fetch`. **Seven rows entered, every one put to the author with
  a recommendation and every one accepted, one against it.** The numbers in the
  bullets below are **the ones those rows had on 2026-09-03**; four rows entered
  on 2026-09-04 and moved every number after 34, which is why this section keys
  by name and why the table above is the only current answer to *where*.
- **M-closures-verdict, M-reflection-verdict and M-deferral-ledger** — rows 35,
  37 and 38, with **M-interpolation-verdict** moved from 41 to 36 beside them:
  every ruling on the language's shape sits before M-core-packages, because the
  packages and the framework are the largest body of Heroes that will ever be
  written against the spec after the compiler, and a form that lands after them
  is a form they were written without. The spec had **378** tokens of headroom
  that evening (`heroes measure`: 3718 of 4096).
- **M-core-packages before M-package-manager** — rows 39 and 40, reversing the
  same evening's earlier scheduling: one distributes what exists, the packages'
  step 13 was already `heroes fetch`, and question (ii) of their opening sitting
  decides where a package lives, which is what `fetch` has to know. The `fetch`
  step moves to M-package-manager together with the root-level driver.
- **M-doc-generator** — **accepted against the recommendation**:
  `heroes doc` is in the record once, as the promise in Part 6's literate-source
  row, and CLAUDE.md §10's stopping rule refused two ROADMAP-scheduled verbs
  before it (`docs/measurements/003` rider 3, `outline` and `explain`). It
  stands after M-web-framework so that its witness — the packages' own API —
  exists, and its entry names the rule as the first question of its opening.
- **M-panic-location** — the cheapest half of *"a strong runtime"*: a
  panic today prints `panic: <msg>` and aborts with no line and no function
  (`runtime/parts/panic.c`), while the emitted C already carries `#line`. Placed
  after the runtime leaves M-isolated-threads' hands; it depends on nothing else
  and may be taken the day the threads land.
- **M-thesis-harness** — Part 11's metrics 2 and 4 have never run, and
  the instrument is a Heroes program over the HTTP client M-core-packages
  delivers. **The author's decision the same night**: a small seed of tasks
  written by the author at its opening, the bulk still at M-guide-book as decided
  2026-08-24.
- **M-lsp-server takes the incremental frontend** — the `SCHEDULED.md` item that
  asked *which milestone does it* has its answer: a server that re-checks on every
  save cannot wait 8 s for `heroes check` on the compiler's own source.
- **M-qbe-backend after the tools** — moved behind them at the reorder: the proof that the IR is
  target-agnostic is worth most when the IR has stopped moving, and every ruling
  above it may move it. Its cells said *Part 7.14* and now say item 15: item 14
  has been declaration visibility since panel 033 (`design.md:2562`, `:2579`).
- **M-install-channels** — before the gate: a Homebrew tap, winget, a Nix
  flake and a Docker image, all building from the seed with the one clang line,
  because `heroes` without clang compiles nothing and a prebuilt binary alone
  would be a decoy; prepared and tested in private, published as the gate's
  outward act. `heroes --version` printed `heroes 0.0.1` that evening and the
  tags are milestone names, so a version scheme comes with it.
- **M-declared-freer, M-thread-stacks, M-discard-refusal and M-check-completeness**
  — **four rows entered together on 2026-09-04 by author instruction**, *"fix
  SCHEDULED too, so that every item is attached to a roadmap step that is still
  to be done; if you cannot find one, create it"*. The instruction is a rule about
  `docs/work/SCHEDULED.md (retired 2026-09-12)` and these rows are what it cost: of its **29** items,
  measured that day, **twelve had no home a reader could reach** — nine named a
  waiting condition instead of a milestone (*"the next panel that touches
  emission"*, *"trigger, not a milestone"*), two named **M-corpus-depth**, which
  had closed that morning, and one named **M-ffi-ladder**, closed 2026-08-12. A
  list that schedules work at a closed milestone is a list that schedules nothing,
  and §14's rule that an id is never reopened is what turns the last one into a
  name of its own.
  - **M-declared-freer** carries panel 109's ratified `owned <C function>` mark
    and the two file splits that come before it. The name is **not**
    `M-foreign-ownership` on §14's rule that an id may make no claim a later
    milestone can falsify: the `ptr owned` half is refused under a standing veto
    with measured return conditions, so the ownership area has to stay free for
    the milestone that may deliver it. What this one delivers is narrower and
    exact — the freer is named in the declaration.
  - **M-thread-stacks** carries panel 107's adopted per-thread guard **and** the
    Windows `-Wl,/STACK:67108864` divergence that hid defect 008, because they are
    one subject seen from two sides: what happens when a stack runs out, and how
    much stack there is. Plural on purpose — 107 refused a uniform number on six
    measurements, so a name in the singular would assert what the sitting refused.
    It sits behind M-isolated-threads because 107 named that milestone as the one
    that inherits the bounds.
  - **M-discard-refusal** had an alias row since 2026-09-03 and no chain row, which
    its own `SCHEDULED.md` item said in its first line. The author places it: here,
    with the language's other rulings, because the verdict is given (`/decide`
    answer `3a`) and what is left is a sitting on the spec sentence and the
    diagnostic class.
  - **M-check-completeness** is the only one of the four with **no warrant**, and
    the row says so. Principle 0 holds it — panel 082 R3 ruled the direction and
    measured that nothing on the closure list needs it — so what schedules it is
    that its two items had been parked on a `grep` since 2026-08-16 with no row to
    read them. The phrase is the record's own: *check accepts ⇒ build succeeds*,
    false today wherever a generic stands between the rule and the type.
- **M-online-compiler** — **scheduled by author instruction 2026-09-06**,
  placed immediately before the gate for M-install-channels' reason and not for
  its subject: everything here is built and tested in private, and the outward
  act is the gate's. **It reverses half of a recorded refusal, and the row says
  which half.** `DESIGN-LOG.md:539` refused *a web playground* on 2026-09-03 —
  one of five candidates `docs/work/DONE.md:2415` records as refused *so the
  candidate is not proposed again as new* — with the parenthesis *Part 2 and Part
  9: wasm breaks the FFI premise*. Both cited passages, `design.md:590` and
  `design.md:2869`, are about a **Heroes program** targeting the web, and the
  compiler is a different program: it reads text and writes diagnostics and C,
  and needs no C library on the visitor's behalf. The half that stands is the
  visitor's program, and it is the milestone's whole subject — measured the day
  the row entered, **20 of 56** programs under `examples/` declare an `extern`,
  so what a stranger may name is a decision before it is an engine.
- **M-generated-programs** — **scheduled by author instruction
  2026-09-06**, placed after M-panic-location by the author out of four options
  put with a recommendation: *"Consider adding a step to the roadmap, towards the
  end, called something like consolidation — you say when it makes sense, maybe
  after the packages. It should create as many valid Heroes programs as possible
  that nonetheless make the compiler crash: hunt for bugs, looking too at the bugs
  that were found in other similar languages and compilers. The difficult
  conditions are the interfacing with C and very deeply nested structures. Purely
  and simply with fuzzing."* Rows 49–57 each move down one, which is this
  section's own shape: the numbers in the bullets above are the ones those rows
  had when they were written. **The gap it fills was measured before the row was
  written, and it is the direction of every instrument here.** `heroes mutate` is
  the inverse one — its own module doc says it takes the corpus programs that
  check clean and makes *one plausible mistake per site*, counting how many the
  compiler catches — so everything this repository points at the compiler is
  pointed at **wrong** programs, and nothing has ever pointed a machine at it with
  right ones. Of the **fourteen** defect entries in `docs/work/DONE.md` (the
  record numbers them 001–015 and uses 014 twice, which is a fault of the record
  rather than of the count), every *found by* field names a person: writing a
  program (006, 007), a panel seat (004, 010, 013, both 014s, 015), the baseline
  net run before touching anything (002), a `fmt` sweep (003, 005), the Windows
  box (008), a program's first run (009), the post-M8a sweep (001). **Four of them
  are at the C boundary and one is depth**, which is why the author named those
  two conditions and not others: 010, 013 and both 014s are FFI, and 007 —
  `heroes check` at exit **139** with nothing on either stream — was a valid
  program nesting deeper than the compiler's own recursive descent. `selfhost/`
  holds **63** points where the compiler declares a case impossible
  (`hero_unreachable`/`unreachable()`, `emit/structural.hero` alone 12), and
  defect 006 is one of them reached by a program `heroes check` had just accepted.
  **The author took the widest option on all four questions** — the name, the
  placing, five oracles rather than crashes alone, and the harness rather than a
  new verb — so §10's stopping rule is not touched and the tool convenes no
  sitting; what may convene one is a repair that reaches the language.

- **M-typed-inspection** — **scheduled by author instruction 2026-09-06**:
  *"I would like to add a step that implements a debugger for the Heroes language,
  a first thinking over all the possibilities, and then tell me as well where you
  want to put it in the roadmap."* Rows 49–58 each move down one; the numbers in
  the bullets above are the ones those rows had when they were written. Three
  choices were put with a recommendation and the author took all three: the id,
  the row, and opening the `assert` defect below in `docs/work/DEFECTS.md` rather
  than carrying it only as a witness into another sitting. **What a stopped
  program shows was measured before the row was written, and the surprise is that
  half of it already works.** On this Mac, 2026-09-06: a breakpoint set on a
  `.hero` line is hit and the source line is printed, `bt` names Heroes frames at
  `.hero:line`, and a local carries the author's own spelling behind an index
  (`h3_base = 7`). What is broken is four things. `p p` is
  `error: use of undeclared identifier 'p'`, because the C name is `h0_p` and
  lldb's expression parser is C++. A `[T]` and a `{K: V}` are an opaque
  `HeroArrayHeader *` and nothing of the contents. A `T?` prints **both** arms,
  including a garbage `err` half, under a hashed type name. And the frame is
  flooded, because §7 hoists every local to the prologue: **139** in one blessed
  emission (`tests/emission/run-adversarial-aggregate-overwrite.c`, 111 named and
  28 temporaries) against the **247** in `syn/expr.hero::compared` that
  M-qbe-backend's own item already carries. **Two findings placed the row rather
  than the symptom list.** The promise has no instrument: the golden this file
  cites at `:452` is `archive/bootstrap-rs/heroes-cli/tests/golden.rs:1012`,
  nothing has built that tree since M-bootstrap-archive on 2026-08-19, and CI
  installs lldb on the Linux leg for a test it never runs. And design.md's own
  consolation is false — Part 2 (`design.md:594`) refuses typed inspection on the
  ground that *"printing rich values is still `print`'s job"*, while `print(p)` on
  a record is a **compile error** and `assert` shows its two sides only for what
  `print` can print, so `assert [1, 2] == [1, 3]` prints the expression and no
  sides at all, silently, against `spec:202`. **The cheap route was run and not
  argued**: thirty lines of lldb Python read `len` from the header, resolved the
  `elem` descriptor pointer to the symbol `hero_desc_str`, found the C type
  `HeroStr` and printed `"ada"` and `"grace"`, with no compiler change and no
  runtime change — and `nm` shows a user type links as `_h_desc_Room_desc`, so the
  descriptor's own symbol is the type name the runtime does not carry. **Placed
  after M-panic-location** because they are two halves of one sentence and this is
  the expensive half: a program that stops says *where*, then says *what it was
  holding*. **Placed before M-generated-programs** on that row's own argument, one
  order up — its programs are the only ones here that nobody wrote, so reading the
  source helps least exactly there. **Placed before M-vscode-extension**, whose
  Debugging bullet states as a ceiling the thing this row removes, and that bullet
  is amended in the same commit rather than left to expire in silence. **Placed
  after M-reflection-verdict**, because a `to_str` derived over a record's fields
  is that sitting's own question and this row must not take it. **No warrant**: the
  §1.1 argument is available and is not claimed, because the only instrument that
  could measure it is Part 11's metric 4, which has never run. **The row was
  re-checked on 2026-09-07 by author instruction** (*"re-evaluate now, after other
  people's commits"*) after ten commits from a peer session closed
  M-declared-freer, which is the rule about reading a scheduling fact immediately
  before writing it doing its job: rows 48 and 49 had not moved, one staleness this
  commit was going to repair had already been repaired by that session, and two of
  the numbers written above were re-run rather than carried.
- **M-cleanup-verdict, M-arm-platform, M-deployable-binary and
  M-compatibility-promise** — **four rows entered together on 2026-09-10 by
  author decision**, out of the session that asked whether the chain was missing
  steps for the language to be usable for **production code that is not mission
  critical**. What produced them is § What production-ready means, and **six of
  its ten rows turned out to be owned already**. All four were put with a
  recommendation and all four accepted; each row's section carries its own
  measurements. **Two positions were choices rather than consequences and both
  are recorded**: M-arm-platform stands before
  M-core-packages, the robust position, where the cheaper one was before
  M-install-channels at the price of every package binding owing a
  re-measurement (CL-040); and M-compatibility-promise stands before the gate
  rather than inside it, because the gate is a checklist of outward acts and this
  is a suite. **What entered no row is recorded too**, in `docs/work/DONE.md`,
  because a candidate met by a rule must not come back as new.
- **M-core-packages' step order was reversed at the top on 2026-09-10 by author
  decision**, in the same session and from a different question: the author asked
  for a step before M-web-framework delivering mini packages, JSON and a small
  HTTP server in Go's shape, and **that step was already this one, already
  there**. What the question exposed, and what it cost, is in that milestone's own
  section; the author's question about how thick a wrapper over C should be became
  its opening sitting's question (ix) rather than a decision taken here.
- **M-microcontroller-verdict** — **entered 2026-09-10 by author decision**, out
  of the session that asked whether supporting a microcontroller such as the
  ESP32, or a program under an RTOS, would be worth a step, given a compiled
  language with no garbage collector. Three recommendations were put and all
  three accepted: a row now rather than a note, a verdict rather than a target,
  and the RISC-V chips first. The record was silent on the question, measured,
  so the row was written on what the compiler and the runtime answered on this
  Mac: its section carries the findings and
  `docs/measurements/026-the-two-facts-a-32-bit-target-refuses.md` the commands.
  The conservative route, a note and no row, was on the table and declined
  (CL-040). No board exists yet; the author is ordering one.
- **M-rotated-records** — **entered 2026-09-12 by author decision**, out of the
  session that asked whether more than one step could be taken at a time, with
  worktrees or otherwise. **The conflict was measured rather than assumed**: of
  the 100 most recent commits, `DESIGN-LOG.md` is touched by **40**,
  `docs/ROADMAP.md` by **31**, `docs/work/DONE.md` by **26** and
  `docs/work/DECIDE.md` by **24** — so two sessions meet in the registers and
  never in the compiler, and the answer is a shape rather than a git workflow.
  The author asked for the historical entries to move as well as the new ones,
  and what makes that safe was measured too: the 161 line-number citations into
  the two large records resolve to **54 distinct lines**, and the 43 in
  `DESIGN-LOG.md` every one of them open an entry — so a map with the same
  geometry keeps all 161 true, and makes them checkable for the first time,
  which CL-037 says no instrument can do today. **The row stands at the end of
  the table and is taken out of chain order**, next after the milestone now
  open: putting it in position would have moved twenty-three row numbers while a
  parallel session was committing into this same checkout, and the fifth
  renumbering (2026-09-08) is the reason that is worth avoiding.

---

---

**What used to stand under this file lives in `docs/roadmap/`** since 2026-09-12,
by author instruction: this file is the chain and the status, and nothing else.
`verify.md` is how to check the project from a cold checkout, `production-ready.md`
what that phrase means here and who owns each part, `milestones.md` the reasoning
a milestone's own row cannot hold, `decisions.md` what this file has decided
about itself, and `names.md` the map from every retired identifier to the one in
use. Each keeps its own heading, so a citation repoints by file rather than by
searching.
