# Panel 184: a brace is written both ways, a statement after a jump is refused, and depth is the compiler's to hold

2026-09-30, at the trunk `a294a6ff`, which every seat copied by `git
archive`. **The full panel**, five seats: compiler-engineer, ffi-pragmatist,
spec-warden, historian, and the llm-ergonomist as fresh sessions outside the
repository (`184-briefs/llm-ergonomist.md`: how they ran, what reached them,
and what they cost, 7.20 USD by the CLI's own reports, the isolation probe and
a first attempt that gave no reading included). The completeness critic read
the briefs before any seat started (sixteen items, every one repaired before
the seats ran, `184-reports/completeness-critic-briefs.md`) and the reports
after (`completeness-critic.md`; its header says *to 22:30*, a slip of its
clock: the file's last modification is 21:41:10 by `stat`). Briefs:
`docs/panel/184-briefs/`, with `probes/` and the blind seat's `blind/`.
Reports: `docs/panel/184-reports/`. Convened by the author's decision of the
morning (`docs/records/log/2026-09-30-1038-four-answers-the-blind-seat-runs-as-a-fresh-session-and-one-sitting-takes-three-questions.md`),
three language questions in one sitting, design.md §4.17 their home. The
seats were stopped twice by the account's session limit (17:10 and 20:17) and
once by the stream watchdog (18:20, the machine at load 150 with three lanes'
gates beside the sitting); each resumed from its own report, and each report
says what it lost (nothing measured).

## The proposal

The three questions and their routes, as `184-briefs/00-shared.md` states
them after the critic's first pass: **(1)** a literal whose `f` was
forgotten: (1a) a plain literal whose braces would parse as a hole is
refused; (1b) the same only where the hole's names are bound; (1c) no new
rule, `unused_binding` says the literal holds the name in braces; (1d)
today's rule. **(2)** a statement after a jump: (2a) one predicate, *this
statement always leaves its block*, read by the refusal and by
`missing_return`; (2b) the refusal after `return`, `break` and `continue`
only; (2c) today's rule. **(3)** how deep a source may nest: (3a) a limit
counted where the parser opens a bracket, a block or a hole; (3b) no limit,
every walk holding any depth; (3c) a limit on the depth of the tree the parser
builds; (3d) today's abort, exit 134 at a depth the machine decides.

**Routes the seats added**, under labels that do not collide (the critic's
second pass, § What the synthesis must not get wrong, 2): **(1e)**, the
ffi-pragmatist's, the brace escape made symmetric; **(1f)**, the
historian's, a hole spelling illegal in every plain literal (Java's `\{`),
named and not recommended; **(2d)**, the compiler-engineer's, (2b)'s refusal
and `missing_return` satisfied by `exit`, `assert false` and `while true`
with no `break` of its own; **(2e)**, the historian's, Wirth's *no jump in
mid-block*, named and not recommended; **(3e)**, the spec-warden's and the
historian's, stack exhaustion reported as the tool's failure, exit 2, naming
the source position; **(3f)**, the critic's, a FLOOR rather than a ceiling:
every source nested up to N compiles on every platform, none is refused for
its depth, and past the compiler's own stack (3e) answers.

## The verdict table

| seat | Q1, a forgotten `f` | Q2, a statement after a jump | Q3, how deep a source may nest | veto |
|---|---|---|---|---|
| compiler-engineer | refuse (1a): 34 tracked literals refused, none a forgotten `f` (20 template data, 6 C `{0}`, 7 Heroes source in tests, 1 golden), 22 files stop checking, the compiler among them; approve (1b) amended (a hole counts only if it names a binding and every name it names is bound; its names count as read; a thesis rule; a `guess` fix): 0 of 1,389 files move in both arms, the 17 silent `forget-f` sites caught, 23 of 25 at the literal, 152 lines | approve (2d): 0 files move in both arms, silent `over-indent` 68 to 49 of 2,000; approve (2b); object (2a): 142 statements refused in 51 files | approve (3c) amended with a thread: one limit, 256, checked on the tokens and on the tree, every pass on a thread whose 256 MiB stack the compiler chooses; 221 Heroes lines and 18 of C; nothing in the table aborts to 10,000, 0 files move, its own source checks at `ulimit -s 512`, where today it aborts | none |
| ffi-pragmatist | object (1a) and (1b) unless (1e) lands with them (`f"[0-9]{{3}}"` hands C `[0-9]{3}}`: a POSIX regex rejects `555-1234` and accepts `555}-1234}` at exit 0; FTS5 returns nothing); approve (1c), (1d); approve (1e) | approve (2a) on a mark `build` verifies for an `extern` that never returns; approve (2b), (2c) | approve (3a), (3c) with the limit measured against `build`; approve (3b); no depth reaches the C (every shape emits C nesting 4 deep) | none |
| spec-warden | object (1a) (+8, +12 with the brace; the compiler stops compiling itself; its benefit true by construction only); object (1b) (+18 or +26; §1.3); approve (1c) (0 tokens); F1, the closing brace, +4 (F1-strict, a lone `}` an error, +15) | approve (2b) (+16); approve (2a) narrow (+34); object (2a) wide (+56; it refuses the `return` after `exit` that today's compiler requires) | object (3a) (+15) and (3c) (+28) on Principle 0, not a budget veto (the compiler nests at most 14 deep; a count is uniform only in what it refuses); approve (3b) (0) and (3e) (0) | none (no route breaches the ceiling) |
| historian | approve (1a) only if the census agrees (every check that fires on a bare `{` was pulled back over false alarms: Clippy, Ruff, Pylint); object (1b); approve (1c) (rustc since 1.65, 2022-11-03); names (1f) | approve (2a), one predicate (Java's *can complete normally* since 1996), `exit` in it as Go names `panic`; object (2b) (C#'s split); names (2e) | approve (3c) as Lua and SQLite, flat chains parsed in a loop, with (3e) (javac's exit 3); object (3b) as the whole answer (rustc's MCP 1011, *Let the OS handle stack growth*, accepted 2026-07-31) | none (no veto by definition) |
| llm-ergonomist, three blind readings | M, (1a) worded with a symmetric escape: approve; K, (1b) without the engineer's amendments: **veto**, a literal's legality depending on a name bound elsewhere; L, today: object | P, (2b): approve; R, (2a) wide: object (a defensive `return` after `exit` refused); Q, today: object; (2d) never shown | Y, a fixed limit with every chain held: approve (two counting questions); Z, today's abort stated: **veto** (the deciding quantity is not in the source; lifted *if it names a floor*); X, today's silence: object | K; Z |

## What the sitting measured

- **The closing brace, found by four seats apart** (the blind seat, the
  spec-warden, the ffi-pragmatist, the compiler-engineer) and measured by the
  coordinator: `f"{{best}} = {best}"` prints `{best}} = 30`; `f"{{best} =
  {best}"` prints `{best} = 30`; `f"a}b"` prints `a}b`; `f"}}"` prints `}}`
  (`scratchpad/p184/braces/`). Python 3.14.7 and rustc 1.90.0 print `{x}` for
  `{{x}}` (the spec-warden and the ffi-pragmatist, run). Panel 121's proposal
  had `{{` and `}}` both write one brace (`121-...md:28`); the spec that
  landed names `{{` alone (`spec/heroes-spec.md:52`), and a golden pins the
  stray brace (`4 and {braces}} and 5`, the spec-warden). The blind seat's
  own M program, written from a brief that said `{{` and `}}`, prints the
  stray brace too (the critic, A1).
- **Question 1's census** (the engineer, the real lexer over 1,389 files): 34
  literals (1a) refuses, 0 of them a forgotten `f`; (1b) amended refuses none
  of the 34. The 25 `forget-f` sites hold 17 silent under today's rule; under
  both (1a) and (1b) they are the rule's class, so their capture is true by
  construction (the warden, the critic, A6). **How often a model forgets the
  `f` or writes brace text in fresh code is unmeasured** by every seat (the
  critic, A7): the blind transcripts show 0 forgotten `f` and 1 hole-shaped
  text literal, induced by the task.
- **The veto on K, and what it reached** (the critic, A2): K lacked the
  engineer's amendments; with *its names count as read*, one of the blind
  seat's two examples (obeying `unused_binding` rewrites the brace rule's
  verdict) no longer happens; the other stands (renaming `best` to `top` flips
  the literal's legality), as does the ffi-pragmatist's FTS5 case (a parameter
  named `title`). design.md §1.3's test is *the meaning of a line*; what (1b)
  moves is a literal's LEGALITY, and § 5's `unused_binding` already makes a
  line's legality depend on other lines. **No seat ruled whether §1.3 covers
  legality**; the one (1b) the blind seat named as acceptable, a hole naming
  only the signature's parameters, reaches 0 of the 17 silent sites (every one
  is in `main()`, the critic's regex estimate).
- **Question 2**: a `match` statement counts as leaving whatever its arms do,
  so `missing_return` goes silent and `build` fails with *internal error:
  compiling the generated C failed ... non-void function should return a
  value*: found by the engineer and by lane 135b's gate apart, widened by the
  critic to any block holding a `match` statement (B1: inside one branch of an
  `if` too), reproduced by the coordinator (`scratchpad/p184/matchleave/`).
  **A defect, filed apart and repaired under every route.** The spec states no
  return rule at all (`grep` over the spec for *every path*, *must return*,
  *returns nothing*: `missing_key` alone), so today `missing_return` refuses
  programs the spec permits (the critic, B8). The narrow (2a) refuses about
  134 tracked statements, most in `selfhost/` (the engineer's census, the
  critic's reading, B2). A Heroes `assert` cannot be switched off, by a search
  of the compiler and the runtime (B6). By its own stated condition the blind
  task 2 reading prefers R to P, since the compiler requires a `return` after
  `exit`; (2d) keeps both spellings legal and was never put to it (B3).
- **Question 3**: the depths in the shared brief's table are this Mac's at its
  default stack; the function a panic names moves with the size of the
  environment on one binary (the critic, C6), so no golden may pin it. A 256
  MiB stack alone moves every abort from 100 to 250 up to 2,000 to 10,000 and
  removes none (the critic's stand-in, C1): `g(g(...))` aborts from 5,000 and
  eight shapes at 10,000. The compiler cannot check its own source at `ulimit
  -s 512` today and can under the thread, **closed by the thread and by
  nothing else** (the engineer; the critic, C2). The engineer's built (3c)
  counts chains, so a 256-term sum and 128 nested `if`s are refused (C4);
  `fmt` is a ninth walk that aborts (`printparens.render` on a 600-term chain,
  C8); types nest outside every counter (1,000 chained records abort
  `emitsynth.collect`; at 10,000 clang itself crashes, C11). (3e) is unbuilt:
  naming the position needs the compiler to record the node before each
  descent and a handler of its own over the runtime's guard (C9).

## Disagreements, unsmoothed

- **(1b)**: the compiler-engineer approves it amended, measured at 0 false
  alarms and 17 of 17; the blind seat vetoed its unamended wording; the
  spec-warden and the historian object on locality and on precedent. The
  veto's reach over a rule of legality is the question no seat settled.
- **(1a)**: the blind seat approves it as worded, with a symmetric escape the
  language does not have; the historian's own condition turns its approval to
  *wait* (the census found template-kind literals, the critic, A3); the
  engineer, the warden and the ffi-pragmatist are against it as the compiler
  would build it today.
- **(2a) against (2b)**: the historian wants one predicate (Java), the
  warden and the blind seat the narrow refusal; (2d) answers both on the
  engineer's measurement, and the historian's remaining point, one fact read
  by two predicates, changes no class on 1,532 shared mutants (B7).
- **(3c) against Principle 0**: the engineer and the historian want a limit;
  the warden objects that no compiler need carries a number past Principle 0,
  and the critic measured that the need the engineer found is the thread's,
  not the limit's (C2). The blind seat vetoes a machine-decided depth and
  names the floor as the way out.

## The resolution: `provisional, author ratification pending`

The most robust and complete one, not the cheapest and not a compromise
(CLAUDE.md § 4); what conservative would have been is written beside each
item, so the author can choose it.

**R1. In an `f` literal, `{{` and `}}` each write one brace, and a lone `}`
is an error** (route (1e), F1-strict): every seat that looked found the
stray brace, and it is the one silent wrong output of this sitting's shapes
that no refusal reaches. Priced +15 vendored by the spec-warden (its § 2
sentence *`{{` and `}}` write one brace, and a lone `}` is an error*); it
changes the output of 3 tracked literals in 2 goldens and refuses one example
line (`examples/gallery/12-interpolation.hero:29`, `{{like this}`), which is
rewritten. **It lands in two stages**: the compiler learns `}}` and the seed
is regenerated before any literal of the compiler's own uses it, panel 121's
bootstrap hazard. *Conservative*: F1-lang, `}}` writes one brace and a lone
`}` stays text (+4); it leaves `f"{{x}"` meaning `{x}`, a spelling no other
language teaches.

**R2. `unused_binding` on a name that a plain literal in the same function
holds in braces says so, with the `f` as a `guess` fix** (route (1c)): no new
refusal and 0 spec tokens; it turns the 6 of 25 `forget-f` sites that today
cost only a misleading `unused_binding` into one message whose fix is the
right one, and rustc has shipped the same note since 1.65. Approved by three
seats and dominated by (1b) on reach only (the engineer).

**R3. No refusal of the forgotten `f` in this sitting; the question goes to
the author with its measurement.** (1a) is refused as the compiler would build
it (34 false alarms, 0 true ones, 22 files stopped). (1b) amended catches the
17 silent sites at 0 false alarms and is held under the blind seat's veto on
the wording it judged; whether a locality veto reaches a rule of legality,
when § 5's `unused_binding` is already such a rule, is the author's to rule
(the critic, A2), and the reading that would settle it, one blind session of
task 1 with the amended wording and R1's escape stated, is sized (one
session, a 3 USD cap) and not run. *Robust, if the author rules that §1.3
covers meaning and not legality*: (1b) amended, on the engineer's prototype
and census, landing after R1.

**R4. A statement after `return`, `break` or `continue`, in the same block,
is a compile error; and `missing_return` is satisfied by a call to
`exit(code:)`, by `assert false` and by a `while true` with no `break` of its
own** (route (2d)), all three read by their syntax and not by a value
(`assert OFF` with a constant `OFF`, and a `while true` whose only `break`
sits under `if false`, are the landing's cases, the critic's B9). **The spec
states the return rule for the first time**: *A statement after a jump, in
its block, is a compile error. A function with a `->` must `return` on every
path that reaches its end, and `exit(code:)`, `assert false` and a `while
true` with no `break` of its own end a path*, +64 vendored (the critic's B4,
by the warden's method), which closes the spec's silence about a rule the
compiler already enforces (B8). **The `match` defect is repaired first**,
under its own number: (2d)'s refusal reads the same predicate. *Conservative*:
(2b) alone, +16, today's `missing_return` and its over-rejection of `exit` and
`while true` unchanged and still unstated.

**R5. The compiler runs its passes on a thread whose stack it chooses**
(256 MiB in the engineer's prototype, 18 lines of C): it enters by compiler
need, measured, since today the compiler cannot check its own source at
`ulimit -s 512` and under the thread it can (C2). It is the compiler's, not a
program's: panel 107's refusal of `main` on a created thread was for PROGRAMS
(it breaks `examples/sdl/`).

**R6. The spec states a floor, not a ceiling** (route (3f)): *every source
nested up to N compiles on every platform, and no source is refused for its
depth*, N measured on this Mac, Linux x86-64 and the Windows box under R5
before the sentence is written (the critic's stand-in passes every shape at
2,000 and fails `g(g(...))` at 5,000, C1). It meets the blind seat's own
condition for lifting its veto on Z, keeps panel 107's refusal of a refusing
number, states only what is uniform (the warden's condition), and leaves
chains legal at any length N allows. **Past the floor, today's abort stands
until (3e) is built and measured** (C9: the position recorded before each
descent in the nine walks, a handler of the compiler's own over the runtime's
guard, its cost timed on a still machine): (3e) is not adopted here, because a
route that does not build is not adopted (`.claude/skills/panel/SKILL.md`
§ 3c). *Conservative*: (3e) alone and no floor, once built.

**R7. What lands with R5 and R6**: `fmt` measured under the thread (C8); the
depth of types, outside every counter, filed apart as a defect of its own
(1,000 chained records abort `build`, C11); no golden, test or spec sentence
pins a panic's function name or a depth one level from the boundary (C6).

**R8. Where the rules are written**: R1, R4 and R6 in `spec/heroes-spec.md`
(§ 2, § 8 and § 9), priced above and paid by the predictions below, each
naming committed goldens rather than the scratchpad's instrument (the warden,
§ 2); the real count by `heroes measure --refresh` in the landing's tree (a
paid call, the landing's to make); design.md §4.17 gains one line per rule
where it lists what a mistake costs.

## Predictions to score

- **R1** (the ffi-pragmatist): a regex or FTS5 binding written against the
  landed escape hands C the bytes its author wrote; scored by a golden that
  builds `f"[0-9]{{3}}"` and prints what C received.
- **R4** (the engineer): silent `over-indent` mutants fall from 68 to 49 of
  2,000 on the instrument's pinned plan, and 0 tracked files move in either
  arm; scored at the landing batch.
- **R6** (the critic, C1): under R5 every shape of the shared brief's table
  checks at 2,000 on this Mac; the number on Linux x86-64 and Windows is the
  landing's measurement.
- **R3** (the blind seat, unscored): *15 to 25 of 100* one-turn tasks with an
  interpolation carry a forgotten `f`, and today 10 to 17 of them compile and
  print braces; unmeasured, the run that would score it sized by the critic
  (A7) and not made.

## Author's verdict

**RATIFIED 2026-10-01**, on the author's answer of the evening to six
recommendations put to them with their reasons, meant as: *1a*, the first
being this sitting. **Recorded as a reading**, CLAUDE.md § 4's default; not
`by delegation`.

**What the yes settles**: R1 to R8 as the resolution above states them, and
R3's question, ruled by the author: **design.md §1.3's locality test speaks
of what a program means, not of what is legal**, so it does not bar a refusal
of a forgotten `f`. Route (1b) as amended therefore lands, after R1, **on one
condition the author set**: the blind reading of task 1 with (1b)'s amended
wording and R1's escape stated (the critic's § E, item 6, prepared in the
coordinator's scratchpad as `rdr/t4/`), one session capped at 3 USD, approves
the wording; if it does not, the wording goes back to a sitting. The reading of
task 2 with (2d) (§ E item 7, `rdr/t5/`) is authorized the same way and scores
R4's last question. Both run once the author has updated the `claude` command
(the same answer, its sixth item), so that the blind seat runs on the sitting's
model: the installed 2.1.274 refuses `claude-opus-5-5`.

**What it does not settle**: the landing, a lane after the gates of
2026-10-01's round, in R1's two stages, then R2, then R4, whose `match`
predicate defect 139's repair supplied (closed 2026-10-01, `32e9dd18`), then
R5 to R7, with R8's sentences priced by `heroes measure --refresh` in the
landing's tree; and R6's N, measured on this Mac, Linux x86-64 and the Windows
box under R5 before its sentence is written.

**The two readings, run 2026-10-01 at 22:38** (one session each, `claude`
2.1.285 on `claude-opus-5-5`, 0.33 and 0.40 USD; `184-reports/llm-ergonomist-task1-second-reading.md`
and `-task2-second-reading.md`). **Task 2**: S approved, which is R4 as
resolved, R approved, P objected to, Q, today's silence, vetoed; R4 lands as
it stands. **Task 1**: K, (1b) with its amended wording, objected to, M
approved, L objected to; the objection is a locality of COMPILING (whether a
plain literal compiles depends on which names the function binds, not on the
line), which the reading itself calls an objection and not a veto. **So the
author's condition is not met**: (1b) does not land, and its wording goes back
to a sitting, panel 185, beside the reading's approval of M, route (1a), which
this sitting measured at 34 false alarms and 0 true ones over the tracked
files. R1 and R2 land as ratified.

## The critic's two passes

First pass, on the briefs (`completeness-critic-briefs.md`): sixteen items,
seven false as written (among them the brief's own nesting table, a line
number from the wrong commit, a load-dependent corpus called fixed), six
missing facts (the `missing_return` collision, the emitter's dropped blocks,
the method chain), three isolation gaps in the blind seat (settled by a probe,
`llm-ergonomist-probe.md`); every one repaired before the seats ran. Second
pass, on the reports (`completeness-critic.md`): twelve points, above, and
the commands of its § E that would settle what is still open, four of them
paid.
