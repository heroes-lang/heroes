---
name: panel
description: Convene the Heroes design panel on a proposal. Two lanes: the SOUNDNESS lane (compiler-engineer + ffi-pragmatist) for a change with no surface, no diagnostic and no spec token; the full five judges with differentiated inputs and falsifiable predictions for everything else. Mandatory before changing the language (spec/, design.md Parts 1-11, surface syntax/semantics, a diagnostic class, architecture). Never blocks: adopts the most robust and complete provisional resolution, never the cheapest and never a compromise, and queues the author's ratification. Writes docs/panel/<NNN> and a DESIGN-LOG line.
---

# /panel <proposal> — the design panel

Five judges, **differentiated by input, not by instruction** — that is what
makes their verdicts carry information instead of correlated opinion.

## Two lanes, and picking the wrong one is the failure this section exists for

**The full panel is expensive**: five judges, every one of them compiling. Panel
037 cost **thirty minutes of wall clock and four hand-built runtimes** for a
question two judges answered identically, and the author stopped it mid-flight.
The instrument was right and the gear was wrong.

**Soundness lane** — `compiler-engineer` and `ffi-pragmatist` only. Use it when
the proposal changes **no surface, no diagnostic and no spec token**: a runtime
primitive, an emitter mechanism, an internal representation. Those two are the
judges who compile, and a question with no reader-facing half has nothing for the
other three to be differentiated *about*. The synthesis is written the same way,
records which lane ran, and says what the lane gave up.

**Full panel** — everything else, and the trigger list in CLAUDE.md §4 is
unchanged. Surface syntax, a diagnostic class, anything the spec must state,
anything a reader of the spec could get wrong.

**When in doubt take the full panel.** The lane's cost is real: panel 037 ran
full, and the llm-ergonomist — reading only the spec — produced the session's
other finding, that the document's single cost sentence implies the quadratic
answer and a reader overrides it on a hunch. The soundness lane would have lost
that. A question that *looks* internal but has a sentence in the spec behind it
is a full panel.

## Procedure

1. **Number the session**: next NNN in `docs/panel/`. State the proposal in
   ≤10 lines, as a spec diff where possible. If the decision is already taken
   (author's call), mark the session `retro-record` — record real objections,
   do not stage dissent.

2. **Prepare each judge's input** (this is the step that matters):
   - **Every brief is written to disk before any seat starts, and the disk is
     `docs/panel/NNN-briefs/<seat>.md` plus one `00-shared.md`, committed with
     the sitting** — author decision 2026-09-13, on panel 140's completeness
     critic, who found that two of five sittings that night carried framing
     facts no later reader could check because a brief existed only as a
     prompt; **the home amended to this one by author decision 2026-09-14**, at
     panel 147, because the scratchpad this line named until then is
     session-specific and **goes away with the session**, so panels 145 and 146
     wrote briefs no later reader can open and the finding the rule exists to
     answer was still true of them. `find docs/panel -maxdepth 1 -type d`
     returned one directory, from 2026-08, on the day this was changed. **The
     seats' REPORTS go beside them**, `docs/panel/NNN-reports/<seat>.md`,
     including the historian's, which the coordinator writes out because that
     seat has no write tool — panels 143 and 144 both recorded that it could not
     be audited for exactly that reason.
     Each seat's prompt tells it to read its brief first; the critic (below)
     reads all of them and says whether a seat was handed a framing fact it did
     not check. The first sitting to write briefs at all was panel 145; the
     first to keep them was 147. A seat that is launched
     before its brief exists returns UNRUN rather than substituting material —
     panel 137 recorded exactly that — which is the behaviour this bullet makes
     unnecessary. **`is_sitting` in `tests/harness/suite_records.hero` knows
     about these two directories**: a sitting is a file directly in
     `docs/panel/`, never one under a subdirectory of it, and that clause was
     added the day the first briefs landed there.
   - **EVERY NUMBER, PATH AND COUNT IN A BRIEF IS PRODUCED BY A COMMAND RUN
     WHILE THE BRIEF IS BEING WRITTEN, AND THE BRIEF SAYS WHICH COMMAND**
     (author instruction 2026-09-16, `docs/records/contract/case-law.md` CL-077).
     A figure copied from `docs/`, from a milestone file, from an earlier
     sitting, or from this session's own earlier paragraph is **carried**, and
     CLAUDE.md § RUN IT already forbids it — this line is that rule at the one
     place it kept failing. Six numbers across panels 155, 156 and 157 were
     carried: a ceiling that had moved (1708 against 1861), a count that was a
     third of the truth (17 against 50), a seam that was inside a `test` block,
     a *provably* traced to `archive/bootstrap-rs/`, an `.expected` file that
     does not exist, and a platform marked unrun hours after the author powered
     the box. **Every one was caught by a seat and none by the coordinator**,
     which is the finding rather than the count: a brief is the one document in
     a sitting nobody is assigned to check, because the seats check the world
     against the brief and nothing checks the brief against the world.

     **AND A BRIEF'S NEGATIVE SENTENCES ARE RUN, OR THEY GO OUT AS QUESTIONS**
     — panel 163's completeness critic, 2026-09-18, naming a pattern CL-077 does
     not cover. That rule binds every NUMBER in a brief to a command and says
     nothing about the other half. Six of panel 163's premises were false and
     three were the same shape, *the compiler does not have X*: that a call typed
     by context does not exist (there are seven such arms and two ARE calls),
     that `zero_of` already provides a zero-default mechanism (it is a C11 type
     probe inside an expression that is never evaluated), and that `partial` does
     not help (it cuts a literal from 4052 tokens to 903). **Every one was
     written by a coordinator who had a shell and did not run it.** A fourth —
     that one route was a refusal — handed every seat a false framing of the
     option the sitting would adopt. CLAUDE.md § RUN IT already says a negative
     claim rests on the searcher's vocabulary rather than the world; a brief is
     where that failure is invisible, because the seats check the world against
     the brief and nothing checks the brief against the world.

     Two shapes are named because both appeared. **A number that was true when
     it was written** — a brief inherits the date of the document it copied
     from, never the date of the sitting. And **a word that is a claim about the
     option set**: *provably*, *the only*, *never* each assert that a search was
     done, so write what was searched and where, or write the weaker sentence.
     Four of the six ran in the direction the coordinator was already leaning,
     which is why the remedy is mechanical — run the command — and not *be
     careful*.
   - `compiler-engineer` ← the proposal + pointers into `selfhost/` and
     `runtime/` (it must cite files and line counts). **Never `crates/`** (archived 2026-08-19): that
     tree is `archive/bootstrap-rs/`, nothing builds it, and a seat sent there
     measures a compiler that no longer ships. Give it the cheap route in the
     brief too — the seed builds in 3.4 s (`clang -I runtime seed/heroes.c
     runtime/runtime.c -o heroes`), while rebuilding from `selfhost/` was ~20
     minutes and killed a seat on the watchdog, which is what happened to four of
     five at panel 087.

     **That number is stale, measured again 2026-09-24 on the author's
     instruction** (panel 177's compiler-engineer found it, and the brief had
     carried it). From the plain seed, `./heroes build selfhost/main.hero -o
     heroes-next` reads **real 60.97 s**; from a seed built with `-O2`
     (`clang -O2 -I runtime seed/heroes.c runtime/runtime.c -o heroes-o2`, 19.22
     s), the same rebuild reads **31.57 s**; the plain seed itself read 3.34 s,
     each with `real` equal to `user` plus `sys`. So a seat that must prototype
     in `selfhost/` can afford to, and the brief says so rather than steering it
     away: two of panel 177's seats built edited compilers this way and the
     sitting's resolution rests on what they ran.
   - `llm-ergonomist` ← ONLY a copy of the specification, the proposal as a
     spec diff, and 1–3 concrete tasks. **Never design.md, never the repo.** Where
     a status-quo-vs-proposal comparison is possible, present the two variants
     label-stripped (blind A/B).

     **The copy lives outside the repository**, in the seat's own
     `<scratchpad>/<NNN>-llm-ergonomist/`, and the brief names that path and no
     path inside the tree (panel 175 item 7, ratified 2026-09-28 by author
     instruction). Reading `spec/heroes-spec.md` in place loads every rule file
     whose `paths:` names `spec/**` into the seat's context, and
     `.claude/rules/spec-shape.md` carries the spec's token counts, which this
     seat is never given. So the blind seat was not blind. **The brief also asks
     the seat to say whether any project rule, contract or memory reached its
     context**, and the synthesis records the answer: that is the measurement
     the sitting asked for, and a yes voids the reading until it is re-run
     from a copy.

     **And the seat runs as a fresh session outside the repository, never as
     a subagent** (author instruction 2026-09-30, given on panel 183's
     finding). The copy was not enough: at panel 183 the seat, launched as a
     subagent of the coordinator's session and reading nothing outside its
     folder, answered `context: yes`, because a subagent is handed the
     repository's `CLAUDE.md`, the memory index and a git status whose commit
     subjects named the sitting's own area (*told once*), whatever its brief
     says. The sitting re-ran it clean, and that is the route from now on:
     a folder of its own under the scratchpad, outside the repository and
     outside any git tree, with no `CLAUDE.md` in or above it; in it the
     spec's copy at the sitting's commit, the tasks, and a brief carrying the
     seat's method and output structure (`.claude/agents/llm-ergonomist.md`)
     without the project's names; then `claude -p "Read brief.md in this
     directory and follow it exactly. Your inputs are the files in this
     directory only. Write report.md here. Answer in English."
     --allowedTools "Read,Write" --disallowedTools
     "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent"`, run from that
     folder, one session per task when the tasks must not see each other.
     The coordinator copies the report into the sitting's reports directory
     with a header saying how it was run; panel 183's
     `llm-ergonomist-clean.md` and `-clean-task2.md` are the first two, and
     their briefs are in `docs/panel/183-briefs/blind/`.
   - `spec-warden` ← the measured token count of the spec before/after (run
     the counter; if only an estimate exists, say so — its verdict will be
     provisional).
   - `ffi-pragmatist` ← the proposal + the task of writing and compiling the
     binding C it implies.
   - `historian` ← the proposal; it must verify precedent via web search.

3. **Run the five as parallel subagents** (one message, five Task calls).
   **Tell every judge that builds to build in a copy**, and say it in the brief
   rather than assuming it: `cp -r` the tree to the scratchpad and work there.
   Panel 054's compiler-engineer was asked to *"prototype it far enough that the
   number is real"* and prototyped **in the repository** — five files of a
   half-built `repeat`, which the coordinator then committed and pushed inside an
   unrelated commit, landing a language feature before its own panel had ruled
   (CLAUDE.md §4). Panel 053's engineer had copied the tree unprompted, which is
   how the difference showed. A judge's measurement is worth having and its
   working tree is not.

   **The copy is not sufficient on its own, and this is the second time that has
   been paid for** (author decision 2026-08-15, panel 056's process notes). Say
   **`rm -rf target build` after the copy**: a copied `target/` leaves
   `env!("CARGO_MANIFEST_DIR")` pointing at the **real** repository, so a judge's
   golden run silently measures the tree it was told not to touch. Panel 056's
   compiler-engineer found this in its own first run, discarded it and rebuilt from
   scratch — which is the only reason it is known rather than a wrong number in a
   verdict.

   **And a copy is one seat's, never two** (author instruction 2026-09-23, on
   panel 170's completeness critic). *The scratchpad* is one place, and this
   paragraph said no more than that until then. At panel 170 two seats worked in
   one copy and one rebuilt the other's compiler underneath it, which produced an
   emission divergence a seat reported as a question and the critic traced to a
   stale binary; in the same sitting the compiler-engineer found its own first
   copy six commits behind and re-ran everything. So **every brief names the
   seat's own directory, `<scratchpad>/<NNN>-<seat>/`**, copied from the frozen
   tree after the coordinator has read `git status` clean and `git log -1` at
   the HEAD the briefs name; **the seat builds its own compiler inside it from
   the seed**, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
   three seconds, and never uses the trunk's `./heroes` or another seat's; and
   **no seat reads, builds or runs inside another seat's directory**. It is
   `.claude/rules/verification.md` § *The compiler that judges is a build
   artifact*, which a gate had learned and a sitting had not.

   **And the rule binds the coordinator during a sitting.** In panel 056 the
   coordinator put `-Werror=missing-include-dirs` into `FLAGS` between 23:37 and
   23:45 while judges were measuring, and one of them observed the effect from
   inside its own run. It was reverted before any commit and nothing landed wrong,
   but a rule that binds judges and not the seat that convened them has its hole
   exactly where the most privileged actor stands. The working tree is frozen from
   the moment the briefs go out until the synthesis is written.

   **3b. Run a completeness critic over the five reports**, after the seats and
   before the synthesis. Not a sixth judge and it gives no verdict: it reads the
   five reports and the six briefs and names what is MISSING — a route nobody
   listed, a claim asserted and not measured with the command that settles it, a
   contradiction between seats with which is checkable, and the question the
   sitting should have asked and did not. It was added at panel 137 (2026-09-13)
   and **changed the resolution in every sitting it ran** through panel 144: at
   144 it falsified the compiler-engineer's own conclusion and found the spelling
   four seats had missed. Five seats differentiated by input still share one blind
   spot — each generalises from the shape it happened to write — and the critic is
   the instrument for that. The historian has no file write by its own definition,
   so its report is handed to the critic verbatim inside the critic's prompt
   (panel 145's shape); until then panels 143 and 144 both recorded that the
   historian could not be audited. Recorded here 2026-09-13 as what the procedure
   has done, so the skill describes the sittings that actually sat.

   **3c. What a sitting's clock is spent on, measured over sittings 160 to
   182 (2026-09-29, author instruction, CL-079).** An incident-free sitting
   took 36 to 111 minutes, median 50, the seats 14 to 44 of them in parallel
   and the critic the longest phase; four sittings took 135 to 947 minutes,
   every one of those hours lost to rate limits, the 600 s watchdog, a Mac
   asleep for three and a half hours (177) and a reboot that emptied the
   scratchpad (179), not to judging; in 13 of 22 sittings the seats found two
   to seven errors in their briefs and paid for them in their own time; and in
   7 of 22 the adopted resolution was narrowed, corrected or measured false
   before it landed, three of them (165, 166, 167) on a mechanism that did not
   build, which convened 168. So, from that day:
   - **the critic reads the briefs FIRST, before any seat is launched**, and
     returns the framing facts it could not verify with a command; the
     coordinator repairs the brief and only then launches the seats. Its
     second pass, over the reports, stays as 3b says. A brief is the one
     document nobody was assigned to check, and the seats were paying for it;
   - **a seat with Bash builds the adopted route in its own copy and runs the
     cases the sitting named, before the synthesis is written** (31 to 61 s
     to rebuild, § 2 above). A route that does not build is not adopted; this
     is the one point where the sitting gets MORE robust and faster at once;
   - **every seat writes its own report file** under
     `docs/panel/NNN-reports/<seat>.md`, as it goes and not only at the end,
     so a killed or stalled seat leaves what it had; `compiler-engineer`,
     `spec-warden` and `historian` gained `Write` for this on 2026-09-29, and
     the historian's report no longer passes through the coordinator's hands
     (the 3b sentence about it stands as history);
   - **the seats are launched staggered, a few seconds apart, and nobody
     polls while they run**: the coordinator waits on the notification. A
     seat the watchdog killed is relaunched from its own report file, not from
     another seat's findings (160, 172);
   - **no seat and no critic starts a paid run its brief does not name**,
     with the run's size and budget written in the brief (author
     instruction 2026-09-30, after panel 183's critic launched 80 `claude -p`
     sessions of its own, 12.62 USD by the CLI's report, to score two
     predictions a named, bounded experiment could have scored): a `claude
     -p` session, an API call, a cloud run. A seat that finds one worth
     running says so in its report, with its size, and the coordinator
     decides;
   - **the sitting runs under `caffeinate -i`** (`.claude/rules/verification.md`
     § A long run holds the machine awake): this Mac sleeps after one minute
     idle, and sitting 177 lost three and a half hours to it;
   - the seats' working copies stay in the scratchpad, which a reboot empties;
     what must survive is in the sitting's own briefs and reports directories
     under `docs/panel/`, in the tree.
4. **Synthesize** into `docs/panel/NNN-<topic>.md`:
   - the proposal, verbatim
   - the verdict table (verdict · section · cost/delta · prediction ·
     condition, per judge)
   - disagreements, stated plainly — do not smooth them over
   - **the decision is the author's, asynchronously.** The synthesis adopts
     **the resolution CLAUDE.md § 4 requires**, which is the most robust and
     complete one, never the cheapest and never a compromise; where robust and
     conservative disagree it takes robust and records what conservative would
     have been, so the author can choose it. This line read *the most
     conservative resolution* until 2026-09-07, four days after the author
     struck that word mid-sitting, and the two are not the same thing
     (`docs/records/contract/case-law.md` CL-040). It is marked `provisional — author
     ratification pending`, records what a veto would compel, and appends an
     **open** item to `docs/work/DECIDE.md` naming the sitting as `panel NNN`.
     Work proceeds on the provisional
     default; the author's verdict is appended to this file when given (in
     `/decide` or whenever), with follow-up work if they overturn it.

     **The sitting is the item's FIRST FIELD, since 2026-09-07** —
     `- [ ] **panel 091** | <the question> | <where to look>`, with any
     reasoning in the body indented under it. That is not layout: the check
     below scans the item LINE, so a sitting cited only in a body is a sitting
     no check can see.

     **The file and the spelling are both load-bearing, and this line named the
     wrong file until 2026-08-26.** `tests/harness/suite_records.hero`'s
     `verdicts` check reads `docs/work/DECIDE.md` and scans its `- [ ]` lines
     for `panel NNN`: a `Pending` verdict is allowed to stand for as long as the
     author likes *provided an open item names it*, and that pair is the whole
     invariant. This line said `QUEUE.md` — the record — so a sitting that
     obeyed the skill was invisible to the check built to catch it, which is how
     panels 069-079 sat unratified for eight days. Write `panel 091`, not
     `panel 91`; both parse, but the padded form is what the repository uses.
   - every judge's `prediction` is copied into a "Predictions to score"
     section with the milestone at which each becomes checkable. When the
     harness next runs, score them and append the results — judges accrue
     track records.

5. Write the `docs/records/log/` entry as its own file. Commit the panel file (and, separately, any
   resulting `spec/` or `design.md` amendment, citing the sitting's own file).
