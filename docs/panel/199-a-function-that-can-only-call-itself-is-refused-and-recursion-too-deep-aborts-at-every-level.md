# Panel 199: a function that can only call itself is refused, and recursion too deep aborts at every level

Convened 2026-10-07 by the coordinator for defect 457 (`systemic`, it blocks
the milestone's tag), with defect 508 (the same shape runs for ever at `-O2`,
against spec `:280`) and defect 507 (clang's warning false on a correct
program) beside it, both filed at 07:48 on 2026-10-08 from the critic's first
pass. **A full panel** (CLAUDE.md § 4: a new diagnostic is what the checker
DOES): the compiler-engineer, the spec-warden, the historian, the blind seat
as fresh `claude -p` sessions outside the repository, and the completeness
critic before the seats and after them. The tree frozen at **`56def9b4`**,
worktree `lane-panel-199`. Briefs written from 23:43 on 2026-10-07 and
repaired from 07:47 to 08:05 on 2026-10-08 on the critic's first pass; the
seats from 08:09, stopped by the account's session limit at about 08:25 and
resumed at 09:50; a reboot at about 11:18 emptied the session scratchpad and
took the compiler-engineer's copy and prototype with it (the historian's and
the spec-warden's reports had been copied before); the blind seat's six
sessions at 09:54 and 10:41, 1.0042 USD of the 6 the author approved; the
compiler-engineer resumed at 23:0x and rebuilt its prototype inside the
repository's root (the author's rule of that day), its first reply about
00:13 and its second, route R4 built, at about 01:30 on 2026-10-09; the
critic's second pass from 00:15 to 00:32; this synthesis from 01:37, every
time read from `date`. Briefs in `199-briefs/`, reports in `199-reports/`,
each marked with how it was copied (a subagent's Write of a report file is
refused).

## The question, from the shared brief

Defect 457: a function whose every path calls itself passes `check` at exit 0
and runs until something stops it. The shape an author meets is a self-call
through UFCS on a name the author believes is a method: `bytes(s)` returning
`s.bytes()`, which is `bytes(s)` itself, since `x.f(y)` is `f(x, y)` (spec
`:267`) and no built-in is named `bytes`. **What does the compiler tell a
function whose every path calls itself, and where?** The routes (A) to (M)
of `199-briefs/00-shared.md`, widened by the sitting: the readings R1 to R4 of
(A), below, and (N), proper tail calls, which the spec-warden and the critic
named.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | **object** to (A) under R1 (it refuses three correct programs) and R2 (it misses p2); **adopt (A) under R4**, built, with (F) and (G) beside; no veto | two prototypes built and gated, a census of 2,980 files each |
| spec-warden | **object**, no veto: no spec sentence is owed, the budget is not at stake, and Principle 0's burden for the diagnostic is unmet (no measured rate of the mistake) | drafts priced on the `maximum` row, a lower bound; the real count unrun |
| historian (advisory) | **approve** a narrow checker error, (A) or (H), with (F) first and (G) and (L) beside; **object** to (C), (K), (D) and a warning kind | rustc, clang, GCC, Swift, MSVC, staticcheck, Error Prone, Mercury, D's 2013 UFCS bug |
| blind seat | six readings, **six correct**, three under today's output and three under the prototype's; no B reader misled, one hesitating on the note's wording | `llm-ergonomist-scoring.md` |
| critic, second pass | R3 is the cheap reading; an unlisted R4 is the robust one; (F) does not close 507; (G) is robust unless unbounded recursion is ruled a loop; the blind experiment carries no weight on adoption | its own copy, rustc 1.90.0 |

## What the sitting measured

- **Today** (`00-shared.md`'s table, re-read by the seats): `check` exits 0 on
  every shape; clang warns on `forever`, p1, p3 and `gen`, falsely on `serve`
  (a correct program ending in `exit`), and is silent on p2, `pingpong` and a
  self-call through a function value; at `-O0` each endless shape aborts 134
  with `panic: stack exhausted`, at `-O2` (the level `heroes run` builds at)
  `forever`, p3, `gen`, `pingpong`, the function value, `mixed` and `yes` run
  until `timeout` stops them (exit 124).
- **The readings of (A), what may end a path before the self-call**: R1, only
  the three written ends (`exit(code:)`, `assert false`, a `while true` with no
  `break`); R2, R1 and every abort nobody wrote (an overflow, an index out of
  bounds); R3, R1 and any call to another function; R4, R1 and a call whose
  callee **may end the program**, followed through calls to a least fixpoint
  (an `extern`, `assert false`, a `while true`, a call through a field or a
  function value, or a callee that may end; a function handed to the
  library's `map`, `filter` and `fold` judged at its call).
- **R1 refuses three correct programs**: `calleeexit`, `callback` and `fieldcb`
  print 0 1 2 and exit 0 at both levels, ending in a callee's, a callback's or
  a field function's `exit`. **R2 misses p2**, the blind seat's discriminating
  task, and its list of aborts never closes. **R3 misses four plausible
  endless functions** the critic wrote: `count(xs)` returning
  `xs.filter(is_even).count()`, `sum(xs)` returning `xs.map(double).sum()`,
  `fact(n)` returning `n * fact(pred(n))`, and `mixed`. **R4 refuses all four
  and spares all five correct programs** (`calleeexit`, `callback`,
  `fieldcb`, `serve`, `hidden2`, the last the spec-warden's: its end an `exit`
  inside a callee that otherwise returns).
- **R4's cost**: 554 code lines by `layout`'s count across six modules, every
  one at or under 300; `check` on the compiler +0.97% and on the harness root
  +1.34% in instructions; the compiler's own tests 1,432 passed, `check` 613,
  `run` 384, `emission` 1,002, `determinism` 422, `corpus` 55, `full` 22,
  `layout` 5, `permissive` 12, all 0 failed; the census over 2,980 tracked
  files moved 2 verdicts, panel 173's deliberate stack-exhaustion probes
  `q5_overflow` and `so_lease`, and no emitted C. R3's numbers beside it are
  335 lines and +0.83%.
- **(B), the rule on the IR**, does not answer the hidden-abort question by
  construction: `--dump-ir p2.hero` holds `sub! $t2, $t3` as one instruction
  with no branch, and `exit` as a plain call (the engineer and the critic).
- **(F), `exit` marked never-returning in the C**, makes `serve` build clean on
  clang 21.0.0 and 22.1.8 and leaves `hidden2`'s warning standing (the critic,
  settling the engineer's and the spec-warden's readings): it does not close
  defect 507's class.
- **(G), sibling calls kept as calls at `-O2`**: `forever`, p3, `pingpong`, the
  function value, `gen` and `yes` abort 134 at `-O2` instead of hanging;
  `check selfhost/main.hero` +0.11% in instructions; the compiler built with
  the flag builds itself and emits byte-identical C; the price, a correct tail
  recursion 10,000,000 deep aborts at `-O2` as it already does at `-O0`, and
  100,000 deep passes. Measured on Apple clang 21.0.0 and 22.1.8 only.
- **The blind seat** cannot discriminate: both variants scored three of three,
  the briefs state each program's intent, and no null control ran. Model-written
  programs of panels 147 to 199, read by the critic: 2 of 252 functions call
  themselves, both this sitting's repaired `fact`.

## Disagreements, stated plainly

- **Principle 0.** The spec-warden holds the diagnostic's burden unmet: the
  compiler needs no such rule, and no measurement shows how often a model
  writes the shape. The compiler-engineer and the historian argue it serves
  the thesis (*every plausible LLM mistake is a compile error*): the missing
  base case is the commonest fault of a recursion, and today the build either
  aborts at run time or, at `heroes run`'s level, hangs in silence. The
  resolution adopts the rule on robustness (CLAUDE.md § Precedence ranks it
  above Principle 0) and records the objection; the conservative alternative
  below leaves the rule out.
- **R3 or R4.** The engineer stood on R3 first, the critic's four programs
  moved it, and R4 was built before this synthesis was written (a route that
  does not build is not adopted, `.claude/skills/panel/SKILL.md` § 3c).
- **(F) and 507.** The engineer put (F) beside the rule as 507's repair; the
  critic measured that it repairs `serve` alone. What closes the class is not
  passing clang's warning on, with the checker the witness.
- **508's meaning.** (G) makes spec `:280` true at every level; (N) would make
  unbounded recursion a loop and `yes` a correct program; (L) writes the level
  into the spec (+17 to +18 tokens on the `maximum` row) and keeps one program
  with two meanings. Which one the language means is the author's.

## The resolution, provisional — author ratification pending

The most robust and complete route at every disagreement (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, (A) under R4: `check` refuses a function that can only call itself.**
   A function every path of whose body reaches a call of itself before it
   returns is `error[endless_recursion]`, a call before the self-call counting
   as a way out only when its callee may end the program (R4 as measured
   above). A function nobody calls is judged as if it were called. A thesis
   rule, dropped by `check --permissive` and pinned by a `permissive` case; no
   fix, neither `certain` nor `guess`. Its text is the prototype's, the one the
   blind seat read: the headline, the UFCS note where the call is written
   `x.f()`, and the note naming the ways out. The engineer's modules as built,
   `check/self_call.hero`, `check/may_end.hero`, `check/endless.hero`, and the
   lines in `flow_errors.hero`, `checker.hero` and `diag.hero`.
2. **R2, (G): recursion too deep aborts at every level.** The `-O2` words keep
   sibling calls as calls (`-fno-optimize-sibling-calls`, through
   `flags.level_words`, the runtime's and every unit's key moving once), so
   spec `:280` is true at every level. Owed at the landing: the flag's effect
   on clang 18.1.3 (the CI's Linux), 20.1.8 (the CI's Windows) and 23.1.1 (the
   box), one case each.
3. **R3, defect 507: clang's `-Winfinite-recursion` is not passed on**, once R1
   has landed (`-Wno-infinite-recursion` in the words, the checker the
   witness). `serve` and `hidden2` build with no warning; what R4 lets through
   is where clang's warning cannot tell true from false (a call that may end
   before the self-call) or does not fire (mutual recursion, a function value),
   so nothing the checker could have told is dropped. (F) is not this route:
   filed apart as an improvement, 17 blessed emissions and the seed.
4. **R4, the spec: no sentence**, the spec-warden's verdict; the sentence it
   would have admitted is false under R4, which spares `serve`. `:280` becomes
   true at every level by R2; 0 tokens.
5. **R5, the cases**: the engineer's `check` case (17 marks) with the critic's
   three in it, and its `permissive` case; `run` goldens of the five correct
   programs, exit 0 at both levels with no clang warning (`warnings` holds every
   golden to none); `run` goldens of `pingpong`, the function value and `mixed`
   aborting 134 at both levels under R2.
6. **R6, the landing**: built on its own trunk after the batch in flight, its
   seed regenerated and its fixpoint by `cmp`, the census at its gate (the
   engineer's prediction below), every new module at or under 300 code lines,
   the CI's four legs and the box.
7. **R7, the cards**: 457 closes at the landing, its rows the shapes above and
   its misses filed: mutual recursion and a self-call through a function value
   (route (I), an improvement). 508 closes with R2 after the platform legs.
   507 closes with R3. Filed beside: `stack exhausted in mixed.helper` names the
   frame that ran out, not the recursion (`adjacent`, route (M)'s ground); (F)
   (an improvement).
8. **R8, refused**: (B), which needs the same special case and stays invisible
   to `check`; (C) and (K), which refuse `hidden2` and would make a warning
   level `design.md:3725` rules out; (D) alone, which drops p1's and p3's only
   message; R1, R2 and R3 for the reasons measured above; (H), narrower than R4
   and blind to p2's shape; (L), two meanings for one program; (J), the `-O2`
   hang; (E) withdrawn at the briefs.

**The conservative alternative, the author's to choose instead**: no new
refusal (the spec-warden's reading of Principle 0), R2 alone for 508 so the
`-O2` hang becomes the abort, and 507 left open with clang's warning, false on
`serve` and `hidden2`, kept. A narrower alternative between the two: R3 in
place of R4, 219 fewer lines, the critic's four programs let through.

**And the language's question, the author's whatever is ratified**: does
unbounded recursion mean an abort (R2) or a loop (N)? Under N, `yes` is a
correct program, every shape the rule misses hangs at every level, and the
emitter must turn self tail calls into jumps, which nobody built.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | at the gate of the batch that lands R1: the census moves only the two `173-briefs` probes and the new cases; no moved function, if called, returns or ends the program; every new or changed module at or under 300 code lines; `check` on `tests/harness/main.hero` at most +2% instructions against the trunk's compiler | the landing's gate |
| spec-warden | P-A (a sentence's real delta) moot, no sentence; P-B (both B readings of p1 and p3 repaired to the key, their `confidence` naming the diagnostic) | **held** (`llm-ergonomist-scoring.md`) |
| coordinator | the blind seat's four clauses | **three held, one half falsified**: `r2-m` hesitated on the note's wording (`llm-ergonomist-scoring.md`) |
| historian | a compiler that shipped an error for unconditional recursion and reverted it over false positives would turn it against (A) as an error | open |

## Author's verdict

Pending. The ratification issue is
`issues/2026-10/09/2026-10-09-0139-panel-199-ratify-amend-or-overturn-r1-to-r8-a-function-that.md`.
