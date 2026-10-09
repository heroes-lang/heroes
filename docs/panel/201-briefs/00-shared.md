# Panel 201, the shared brief: how the language is read (488, 467, 520)

Written by the coordinator on 2026-10-09 from 19:50 (`date`), on the tree
frozen at `36be56d0` (worktree `lane-panel-201`: the trunk as pushed after
batch 16). Convened on the author's instruction of about 19:35, meant as:
*launch the single panels, then merge their solutions into one lane; goal,
every defect closed within four hours*. Every fact names the command or file
it comes from; a number marked **carried** is a card's, not re-run by the
coordinator, a question for the seat.

**Lane: full** (CLAUDE.md § 4: what the checker refuses and what the spec
says): the compiler-engineer, the spec-warden, the historian, the blind seat
as fresh `claude -p` sessions outside the repository run by the coordinator
(6 USD, the author's yes of about 19:40; its folders in the session
scratchpad, deleted at the sitting's end, the author's exception of
2026-10-09 to the root rule), and the completeness critic before the seats
and after them.

## The three questions

**Q1, defect 488**: a generic function value handed to a generic function's
parameter is refused `cannot_infer`: `app(f: ident, x: 20)` with `ident<A>`
and `app<A>(f: (function(A) -> A), x: A)`, `ns.map(ident)` and
`ns.fold(19, keep)` (the card; reproduced by the coordinator on 2026-10-07).
The spec, `spec/heroes-spec.md:276-279` (§ 9): *Generics: on functions only,
no constraints, always inferred ... A type parameter takes its type from the
arguments, else from the type the context asks for; a call that says neither
is an error. `xs.map(double)` takes both from `double`'s signature.* The
refusal's fix names the narrowing (`selfhost/check/function_value.hero:228`:
*hand it to a non-generic function's parameter, a record's field or a
`return` whose declared type says what they are*), defect 402's design.
**Which is the rule, the spec's sentence read plainly or the compiler's
narrowing?** If the compiler: what the spec must say instead, and its token
cost. If the spec: what inference the checker must do (unify `A` of `app`
with `A` of `ident` through `x: 20`), its cost, and the shapes beside it
(`map`, `fold`, a generic value stored in a record field, returned).

**Q2, defect 467**: spec line 22, § 1: *One file is one module; the file you
compile holds `function main()`, which takes nothing and produces nothing.*
Two of the 12 blind sessions of measurement 040 on module files added a
`main` (`docs/measurements/040-thirty-first-answers-and-none-left-the-mistake-the-compiler-did-not-tell.md:191`
and `:201`: *appended a `main` ... using the binding: an addition*), which
`check` accepts. **Is the sentence misread, and is a reworded sentence worth
its tokens** (Principle 0, design.md §1.6), or is the extra `main` harmless
and the sentence right?

**Q3, defect 520**: panel 199's R4 (ratified 2026-10-09) refuses, as
`endless_recursion`, a function every path of whose body reaches a call of
itself; it lets through mutual recursion (`ping` calling `pong` calling
`ping`) and a self-call through a function value (`f = go` then `f(n)`),
which since defect 508's repair abort 134 at every level (panel 199's R7,
route (I), filed apart as an improvement). The sitting:
`docs/panel/199-a-function-that-can-only-call-itself-is-refused-and-recursion-too-deep-aborts-at-every-level.md`.
**Should the rule follow calls through a cycle of named functions, and
through a function value whose callee the checker knows**, at what cost, with
what message, and with which correct programs at risk (mutual recursion with
a way out in either function)?

## The rules every seat works under

- **Your own copy, inside the repository's root**: `rsync -a --exclude
  .claude/worktrees --exclude .git
  /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-201/
  /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/201-<seat>/tree/`,
  your compiler from its seed (`clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes`). **No file is ever written outside the
  repository's root**; TMPDIR inside your folder. Never build, run or write in
  the frozen tree, the trunk, any `lane-*` worktree or another seat's folder.
- **Running notes** in your folder's `notes.txt`; your final reply is your
  report, copied by the coordinator into `docs/panel/201-reports/<seat>.md`.
- Bounded runs, times only from `date`, counts and not durations unless the
  machine is still. **No paid run** but the blind seat's, which the
  coordinator runs.
- Lane b17-fix (the harness, the hooks, the parser) and panel 200 (the
  emitter and the runtime) work beside you: never touch them.
- **Time box**: report by 21:00 with what you have, the unmeasured said
  plainly; the author's goal is every defect closed by about 23:40.

## Corrections and additions from the critic's first pass

Read 2026-10-09 between 19:50 and 19:55 by the clocks read around it (`completeness-critic-pass1.md`
beside the reports), every one from a command it ran; applied before any seat
starts, binding over the text above where they disagree.

- **Q1 (488)**: the bullet is `spec/heroes-spec.md:275-279`. **The framing
  needs its other half**: defect 402's repair read this same sentence as
  allowing the narrowing (its card: *Spec § 9 decided it*). The sentence names
  the type parameters of the **call**, not the passed value's own, so *read
  plainly* is itself one of the readings, not a fact. **Route not listed**:
  infer only when the generic value's type is fixed by an argument that is not
  itself generic. **Unasked**: the qualified form `helper.ident` (defect 421)
  passed to `map`; whether `cannot_infer`'s `guess` fix text and the `fixes`
  suite move.
- **Q2 (467)**: **the evidence is confounded**: measurement 040's task was
  *make this program compile and do what it evidently means* (040 `:112-113`,
  reused at `:171`), and adding a `main` is a plausible answer to *compile*.
  The 12 holds (3 module files by 4 sessions, `:213-214`); the sessions'
  reports cannot be re-read, their scratchpad gone. **Route not listed**: keep
  the sentence and have `check` warn on a `main` in a module another file
  `use`s. **Unasked**: what `build` says today for a file with no `main`.
- **Q3 (520)**: in panel 199's resolution the rule is **R1, adopting reading
  R4** (`:128`); R4 there is *the spec: no sentence* (`:152`). **Two `run`
  goldens become refusals if the rule widens**:
  `tests/golden/run/fixedbugs-508-two-functions-calling-each-other-for-ever-abort-at-every-level.hero`
  and `fixedbugs-508-a-self-call-through-a-function-value-aborts-at-every-level.hero`,
  panel 199 R2's witnesses that recursion aborts at every level, so a
  replacement witness is owed (a shape the rule cannot see). A widened refusal
  is judged by every golden tree (`check` `run` `emission` `determinism`
  `corpus`, `.claude/rules/verification.md`). **A fact that shapes the
  message**: a cross-module cycle cannot exist, `module_cycle` refusing a `use`
  cycle (`selfhost/modules.hero:329`), so a cycle of named functions always
  sits in one file and the message can name every function where it is.
- **The spec-warden prices with the vendored table, written as a lower bound
  in those words** (`.claude/rules/spec-shape.md:159-176`); the pinned real
  count is 9,831 (`selfhost/measure/pinned.hero:54-55`). Whether `heroes
  measure --refresh` is a paid run (it posts to the token-counting endpoint,
  `selfhost/measure/judged.hero:49`) is a question this sitting does not
  settle: no `--refresh` here; one at the landing if a sentence is adopted, on
  the author's yes.
