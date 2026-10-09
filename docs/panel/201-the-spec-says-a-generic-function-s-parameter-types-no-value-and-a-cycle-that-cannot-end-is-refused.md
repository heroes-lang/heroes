# Panel 201: the spec says a generic function's parameter types no value, and a cycle that cannot end is refused

Convened 2026-10-09 by the coordinator on the author's instruction of about
19:35, meant as: *launch the single panels, then merge their solutions into
one lane; goal, every defect closed within four hours*, for three questions of
how the language is read: defects 488, 467 and 520. **A full panel** (what the
checker refuses and what the spec says): the compiler-engineer, the
spec-warden, the historian, the blind seat in twelve fresh `claude -p` sessions
outside the repository (2.6785 USD of the 6 the author approved at about
19:40), and the completeness critic before the seats and after them. The tree
frozen at **`36be56d0`**, worktree `lane-panel-201`. Briefs written from
19:50; the critic's first pass between 19:50 and 19:55, its repairs applied
before any seat started (Q1's framing has two readings, Q2's evidence is
confounded by its task's word *compile*, Q3's widening refuses two `run`
goldens); seats from about 19:56; the blind seat's first eight sessions from
19:57:10 to 19:58:09 and two more at 20:42 and the last two, on the critic's second pass, at 20:52; the historian's reply copied at
20:11, the spec-warden's at 20:20, the compiler-engineer's at 20:41; the
critic's second pass from 20:43 to before 20:52; this synthesis from 20:56,
every time read from `date`. Briefs in `201-briefs/`, reports in
`201-reports/`.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | Q1 **approve** a restricted inference, built, on condition design.md §4.12 is amended in the same landing; Q3 **approve** the cycle rule and a known value's self-call, built, with the replacement witnesses; Q2 **approve** the sentence as it stands; no veto | three compilers in its copy, the census of 3,121 files for Q1, `check` `fixes` `permissive` `full` `emission` `corpus` |
| spec-warden | Q1 **veto route I on Principle 0**, approve the sentence N1f (+12 on the vendored maximum, a lower bound); Q2 **object** to every rewording; Q3 approve no sentence, on condition nothing that ends is refused | `heroes measure`, 19 probes of § 9's rule, eight drafts |
| historian (advisory) | Q1 approve joint inference, `cannot_infer` kept for a letter nothing fixes; Q2 keep the sentence; Q3 approve named cycles, a function value only where syntax decides | Damas-Milner, Swift, Rust, Go 1.21 and its open shapes, TypeScript 3.4; Go, Rust, Zig, Java, Python, Nim on `main`; rustc #57965 and PR #75067, clang, GCC, Mercury, Agda |
| blind seat | Q1: today's spec, 2 of 2 predict *accepted* where the checker refuses; the narrowing sentence, 2 of 2 *refused*; Q2: 0 of 4 add a `main`; Q3: 2 of 2 repair the cycle with a base case, under the prototype's refusal and under today's run-time panic alike | `llm-ergonomist-scoring.md` |
| critic, second pass | the spec-warden's veto is neither budget nor soundness; route I is not 0 tokens (8 of the 14 refused probes stay refused) and as built refuses two correct shapes it covers; Q3's headline is false on a reachable shape and its cost +5.2% on the compiler, quadratic in declaration order | its own q1 compiler over the spec-warden's 14 probes, interleaved counts of `check selfhost/main.hero`, chains of 500 to 2,000 functions |

## What the sitting measured

- **Q1, 488**: the narrowing is panel 105's ratified rule (design.md §4.12,
  *Flat only: a generic callee's arguments are synthesised*), and the spec's
  sentence is that sitting's transcription with the clause left out (the
  spec-warden; ledger row 3750). The same rule refuses fourteen probes, nine
  of them shapes the card never names (`ok(...)`, a result-only call, a
  literal at `u8`, a concrete parameter of a generic function). Blind: the
  spec as it stands, 2 of 2 readers predict `app(f: ident, x: 20)` and
  `ns.map(ident)` *accepted*, which the checker refuses; with the
  spec-warden's sentence **N1f**, 2 of 2 predict *refused* and quote it (the
  first narrowing sentence the readers read, the critic's, was false and
  carries no weight). The compiler-engineer's restricted inference (route I),
  built, accepts 6 of the 14 and moves 2 files of the census; the critic
  measured that it leaves § 9 false on the other 8, so it owes a sentence of
  its own (draft C, +16), that it still refuses `xss.map(len_of)` and
  `first(a: double, b: ident)`, shapes its own description covers, telling
  them *this position asks for none*, and that it doubles one message by the
  order of a parameter type's parts.
- **Q2, 467**: a used module's `main` is accepted and never runs; a module
  built alone is told `no_entry_point` and the note *add `function main()`*,
  which is what measurement 040's two readers did under *compile*. Blind: 0
  of 4 added a `main` with the module beside the program, under either
  wording and either spec. Precedent (the historian): Go compiles a library's
  `main` and never calls it; Java, Python, Nim make it a feature; nobody
  documents it as a mistake.
- **Q3, 520**: the prototype refuses `ping`/`pong` and `f = go` with a
  message naming both functions, passes `is_even`/`is_odd` and every shape
  with a way out the critic tried; the blind seat repaired the cycle with a
  base case under its message and under today's run-time panic alike. The
  critic: its headline says *every path through `pong` calls `ping`* where a
  path calls `pang` (a false message on a reachable shape); `check` of the
  compiler +5.2% (72.68e9 to 76.42e9, three interleaved runs, spread 0.25%),
  and a chain of 2,000 functions declared callers first 0.20 s to 4.09 s, the
  `while changed` loop re-walking every member to drop one a pass. The two
  `run` goldens of defect 508 become refusals; the compiler-engineer's two
  replacement witnesses (a way out written and never taken) pass `run` at all
  four of its configurations, aborting 134.

## Disagreements, stated plainly

- **The spec-warden's veto.** It rests on `.claude/agents/spec-warden.md`'s
  grant of a veto where Principle 0's burden is unmet; CLAUDE.md § 4 says seats
  veto on soundness. The critic reads it as an objection under § 4, and the
  synthesis does too; it is moot, since the resolution adopts the
  spec-warden's own route.
- **Route I or N1f.** The historian and the compiler-engineer stand on
  inference (Go dropped the same narrowing in 1.21, one shape at a time);
  the spec-warden on the sentence. Measured: route I is not the 0 tokens it
  was argued at (+16 for its own sentence), as built it refuses two shapes it
  claims and doubles a message, and the one inference that would make § 9
  true at 0 tokens, context for every argument that needs it, is unbuilt.
  N1f states the compiler as it is, its reader effect measured 2 of 2.

## The resolution, ratified by the author (below)

The most robust and complete route at every question (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, defect 488: the spec states the ratified rule, N1f.** § 9's bullet
   reads *A type parameter takes its type from the arguments, else from the
   type the context asks for, and a generic function's parameter asks for
   none; a call that says neither is an error.* +12 on the vendored maximum, a
   lower bound; the real count measured by one `--refresh` at the landing on
   the author's yes, and re-argued above +24 (the spec-warden's P1). The
   compiler is unchanged; 488 closes with the sentence. **Filed now** as an
   `improvement` for a sitting of its own: the inference that gives context to
   every argument that needs it (generic values, `ok(...)`, `[]`, a literal, a
   concrete parameter of a generic function), which would make the sentence
   unnecessary at 0 tokens, route I's prototype its first step.
2. **R2, defect 467: the sentence stays, 0 tokens; 467 closes on its
   measurement.** Filed beside it: the `no_entry_point` note naming the file
   that uses the module (`improvement`), and a used module's `main(n: i64) ->
   i64` accepted through `use` and refused checked alone (`adjacent`).
3. **R3, defect 520: a cycle of named functions, and a self-call through a
   known value, refused** as `endless_recursion`, landing only with: a
   worklist fixpoint (re-walk only the callers of a dropped function) whose
   cost on `check selfhost/main.hero` is within noise of today's, measured; a
   headline naming each member's targets, true on the critic's `pang` shapes;
   the compiler-engineer's two witnesses replacing defect 508's two `run`
   goldens; the 457 golden's correction appended. It lands in batch 17 if the
   landing lane meets all four; otherwise the next batch, 520 open until
   then. Its residuals filed (`selfloop_first`; a self-call through a
   parameter). No spec sentence (`:280` stays true).
4. **R4, the spec**: N1f alone.
5. **R5, refused**: route I as built; the blind seat's first narrowing
   sentence (false on annotations and `push`); every rewording of line 22;
   a warning on a module's `main` (design.md rules out a warning level).

**The conservative alternative, the author's to choose instead**: route I's
alternative is the robust one here and is not taken; the conservative route
would leave the spec as it is and file 488's mismatch. **A narrower
alternative for 520**: close it with the prototype's measurements and the
critic's findings, landing nothing.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| spec-warden | P1: N1f's real delta +13 to +20 at the landing's `--refresh`; P2: N1f true of `check` on every probe (14 refused, 5 accepted) | the landing |
| compiler-engineer | Q3: with R3 landed the census moves exactly the 457 golden and the two 508 goldens; no `examples/` program refused by the cycle rule | the landing's gate |
| compiler-engineer | Q2: a blind measurement on module files whose task does not say *compile* draws at most 1 added `main` in 8 or more | unrun |
| historian | Q1: a language that shipped this inference and withdrew it would turn its reading | open |

## Author's verdict

**RATIFIED, 2026-10-09**, R1 to R5 as written above, the author answering
through the question widget between 20:55 and 20:57 by the clocks read
before the question and after the answer, choosing *Ratifica R1-R5* over
*the compiler learns* (route I with sentence C) and *I want to read it first*, on the coordinator's summary of each route. The one `heroes measure --refresh` R1 needs at the landing was put to the
author in the same widget and answered *yes, one*.
Recorded as a reading (CLAUDE.md § 4). The author may overturn it.
