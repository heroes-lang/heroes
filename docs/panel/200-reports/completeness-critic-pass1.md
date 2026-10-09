# Panels 200 and 201, completeness critic, first pass: the briefs against the frozen trees

Copied by the coordinator at 19:55 on 2026-10-09 (`date`) from the critic's
reply, verbatim apart from this header and the HTML entities of the
notification turned back into their characters (a subagent's Write of a
report file is refused). One critic read both sittings' briefs, read-only on
the two frozen trees at `36be56d0`, launched after 19:50 and done before 19:55 by the clocks read around it; its scratch
`.claude/worktrees/scratch-b15/critic-200-201/` (ignored by git). Panel 201's
copy of this report is `docs/panel/201-reports/completeness-critic-pass1.md`.

---

I ran checks on both frozen trees (both at `36be56d0`) and wrote nothing inside them. One premise is false and has to go back to the author before anyone builds: the emitted C already has one exit point per function (panel 200, Q3). Panel 201's Q2 evidence is confounded by the wording of the readers' task.

## Repairs to the briefs

1. **200 `00-shared.md` Q3, and the compiler-engineer's Q3: the author's decision rests on a false premise.** The C is already one-exit:
   - `selfhost/ir/exits.hero:1-13` merges every returning block into one exit block and says so (panel 190's route A-star, defect 231).
   - It is wired at `selfhost/ir/lower.hero:56`, and landed at `6e616898` on 2026-10-04.
   - Panel 197's compiler-engineer measured the 3.7 to 4.0 growth on that C (`197-reports/compiler-engineer.md:21`: *"On today's one-exit C"*).

   Tell the author first. Then the question becomes "where does clang's memory go in a one-exit function" (text size against slot count across 200/400/800 returns, clang's per-pass memory), or closing as a known cost.
2. **200 Q1, the citation `artifact.hero:108-117`.** The paragraph runs :102-117 and the quoted sentence is at :112-113. "372 run programs" is the comment's count from 2026-10-07; `ls tests/golden/run/*.hero` gives 422 today. Mark it as carried.
3. **200 Q1, "exit 2".** The message is at `selfhost/cli/produce.hero:239`, but it goes through `whose.of_refusal` (:229-240), which can blame the author's `extern` (`generated-c.md:85-87`). Exit 2 is not automatic; the brief should ask which class an `-fsyntax-only` refusal belongs to.
4. **200 Q1, missing cost sites.**
   - `--emit-c` is called at 12 places in 5 harness suites (emission, determinism, lines, surface, golden).
   - The seed is regenerated with `--emit-c` (`seed/README.md:93`). The 20.9 billion would be paid on every regeneration, although the gate already compiles `seed/heroes.c` with clang.
5. **200 Q2, the citation `run.c:630-638`.** The comment runs :623-638; `prctl` is at :639 (that part holds). Also name the constraints a watcher thread would hit:
   - `docs/design.md:1665`: reference counts stay non-atomic only while no counted value crosses a thread.
   - The stack guard's SIGSEGV/SIGBUS handlers (`runtime/parts/stack.c:796-797`).
   - Forking from a program that now has two threads, inside `hero_run_go` itself.
6. **200 Q2, scope.** A watcher inside the child covers only children that are Heroes programs. Ask whether clang under `heroes build`, and the children of a Heroes program's own `hero_run_go` (the harness, `selfhost/mutate/child.hero:70`), must be covered too. The sentinel covers them; the watcher does not.
7. **200 Q4, the adjacent shape is missing.** Defect 335 (closed, `9d8b07ae`) is the mirror case. A merged exit carrying the function's own line made lldb land on the function's head, and the repair moved it back under the generated file (`selfhost/emit/term.hero:78-86`). Also point the seats at the code: `selfhost/emit/writer.hero:223` (`at_generated`) and `selfhost/emit/body.hero:132-161`. And name the `lines` suite as a judge.
8. **200 Q5, the citation `panel 182 :19-20`.** Those lines say a lowering bug reaches the author as `internal error:` with clang's words, and that `--emit-c` and a debugger show uninitialised temporaries. The sanitiser point is at :31-32 (ASan cannot see it; MemorySanitizer can, on Linux only) and :198. The main thing a reversal gives up is missing from the brief: `-Werror=uninitialized`, about 71,602 values returned to clang's check (:70-77), and the prediction of zero uninitialised warnings on the seed (:229).
9. **200 compiler-engineer brief, "about 31 to 66 s … at batch 16's gates".** This is not in batch 16's closing body (`git log -1 16624836`). It is a duration with no source and no "carried" mark; mark it or drop it.
10. **201 Q1, the citation `spec:276-279`.** The bullet is :275-279. The quote holds, and so does `function_value.hero:228`. The framing needs one more line: defect 402's repair already read this same sentence as allowing the narrowing (its card says *"Spec § 9 decided it"*). The sentence names the type parameters of the **call**, not the passed value's own, so "read plainly" is itself one of the readings.
11. **201 Q2, the confound.** Measurement 040's task was *"make this program compile and do what it evidently means"* (040:112-113; part 2 reused it, :171). Adding a `main` is a plausible answer to the word "compile". The 12 holds (3 module files × 4 sessions, :213-214), and :191 and :201 hold. The session reports cannot be re-read: the scratchpad at `<scratchpad>/batch14/m212` is gone (ls).
12. **201 Q3, "panel 199's R4" is ambiguous.** In panel 199's resolution, R4 is the item *"the spec: no sentence"* (:152). The rule is R1, "(A) under reading R4" (:128). The spec-warden brief's "R4 wrote no sentence" is right in that numbering, so say "resolution R1, adopting reading R4". Ratified on 2026-10-09: holds (the ratification issue's verdict line).
13. **201 Q3, golden cases at risk.** Two `run` goldens become refusals if the rule widens: `tests/golden/run/fixedbugs-508-two-functions-calling-each-other-for-ever-abort-at-every-level.hero` and `fixedbugs-508-a-self-call-through-a-function-value-aborts-at-every-level.hero`. They are R2's witnesses that recursion aborts at every level, so a replacement witness is needed. Also run `check`, `run`, `emission`, `determinism` and `corpus`: a widened refusal is judged by every golden tree (`verification.md`).
14. **201 spec-warden brief, pricing with the vendored table only.** This contradicts `.claude/rules/spec-shape.md:159-176` (*always measure with the real*): a vendored figure is written as a **lower bound, in those words**. The pinned real count is 9,831 (`selfhost/measure/pinned.hero:54-55`) and gives the base. "`--refresh` is a paid run" is a question to settle: it posts to `/v1/messages/count_tokens` (`selfhost/measure/judged.hero:49`), which Anthropic documents as free but rate-limited. I did not check that here.

## Routes nobody listed, and questions not asked

- **200 Q1:**
  - For `--emit-c`, compile the whole-program rendering as the only unit, so the written file is what clang read and no second pass is needed.
  - Or put the check in the net only: `emission` runs `-fsyntax-only` on blessed artifacts, and the seed is already compiled at every fixpoint.
  - Question: does `.claude/rules/cli-surface.md`'s exit-code contract allow `--emit-c` to start refusing?
- **200 Q2:**
  - `heroes run` could `exec` the built program instead of forking it. There is then no parent left to die, though it covers only `heroes run`.
  - The child inherits the read end of a pipe the runner holds, and a thread reads it until EOF. This works on any POSIX system without kqueue; the runner's end must be close-on-exec.
  - The kqueue watcher must close its race by checking `getppid()` after registering.
  - A loop polling `getppid()`.
  - Unasked: how the child knows it was started by a runner. An environment variable is inherited by grandchildren.
- **200 Q3:** where clang's memory goes in the one-exit C. Panel 182's critic proposed coalescing the slots of mutually exclusive arms (182:200).
- **200 Q4:** stop the prologue from emitting stores. lldb's step-in lands on the first line-table row, and that row may be a `= {0}` store.
- **200 Q5:**
  - Measure with `-Wno-uninitialized -Wno-sometimes-uninitialized -Wno-conditional-uninitialized`. If the extra cost disappears, it is the price of the very check panel 182 kept.
  - Question: does any real program reach a nesting depth of 1,000? Measure the deepest struct in the seed and corpus.
- **201 Q1:**
  - Infer only when the generic value's type is fixed by an argument that is not itself generic.
  - Unasked: the qualified form `helper.ident` (defect 421) passed to `map`; whether the `cannot_infer` fix text (a `guess`) and the `fixes` suite move.
- **201 Q2:**
  - Keep the sentence and make `check` warn on a `main` in a module that another file `use`s.
  - Unasked: what `build` says today for a file with no `main`.
- **201 Q3:** cross-module mutual recursion cannot happen. `module_cycle` refuses a `use` cycle (`selfhost/modules.hero:329`), so a cycle of named functions always sits in one file and the message can name all of them where they are. The spec is silent on this.

**200 lane check:** the soundness lane is right for all five questions. `grep` finds no `--emit-c`, `heroes run`, debugger, thread or kill sentence in the spec. Q4 does amend design.md §3.1 (:682-688), and Q1 touches the exit-code contract; both should be recorded, but neither needs a reader.

## Blind tasks for panel 201

Measured budget anchor: measurement 040 sessions cost 0.35 to 0.59 USD each on programs of 300 to 400 lines. With 6 USD, one arm cannot tell 2 in 12 from 0; say so in the plan.

- **Q1 (488):**
  - *Predict:* give `ident<A>`, `app<A>(f: (function(A) -> A), x: A)` and a `main` calling `app(f: ident, x: 20)` and `ns.map(ident)`. Ask "does this check? quote the sentence." Variant A is the spec as it stands. Variant B adds *"A generic function used as a value takes its types only from a non-generic parameter, a record field or a declared return; handed to a generic parameter it is an error."*
  - *Write:* "double each element of `ns` using `map` and an identity helper you write". Count `ns.map(ident)` under each variant.
  - *Repair:* give today's `cannot_infer` message and score whether the edit is correct.
- **Q2 (467):** a 40-line `geom.hero` with no `main`, plus a `main.hero` that does `use geom`. The task is "add `area` to geom.hero", in two wordings: with "make it compile" and without. Cross that with the spec's line 22 as it stands against a rewording, for example *"Only the file you build holds `main`; a module it uses holds none."* Count the added `main`s.
- **Q3 (520):**
  - *Predict:* `is_even`/`is_odd`, where only `is_even` has a base case, plus `ping`/`pong` with no way out. Ask which ones `check` refuses.
  - *Repair:* give the refusal of `ping`/`pong` in two message variants: the headline naming the cycle `ping -> pong -> ping` with the ways out of each function, against today's R1 text naming one function. Score whether the reader adds a base case or deletes a call.
