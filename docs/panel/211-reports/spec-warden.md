# Panel 211, spec-warden: `check --apply --in-place` over a tree (defect 607)

Seat: spec-warden. Tree: `.claude/worktrees/scratch-b15/211-spec-warden/tree/`,
five files hashed against `git show 1dd890751:<path>` and identical
(`spec/heroes-spec.md`, `docs/design.md`, `selfhost/cli/apply_answer.hero`,
`selfhost/cli/table.hero`, `.claude/rules/cli-surface.md`); compiler built
there from the seed, exit 0, at 02:15. Opened 02:11 by `date`. Written as the
sitting went; every number carries the command that produced it. No paid run:
every token price below is **vendored, a lower bound** on the instrument
design.md §1.6 names (`.claude/rules/spec-shape.md` § How a change to the
document is made).

## Verdict block

- `verdict`: **approve N** (a new flag beside `--in-place`), **object W**,
  **object S as the resolution** (it is the conservative alternative), **object
  L** (nothing to refer to); a **veto held in reserve** against any route that
  adds a spec sentence unpaid. Not provisional on the spec's count: the spec
  moves 0 tokens on every route, measured as the unchanged file.
- `section`: design.md §1.6 (budget, payment rule), §4.17 (panel 193's
  ratified paragraph, design.md:2229-2254), `.claude/rules/cli-surface.md`
  (stopping rule, invocation contract), CLAUDE.md § 10.
- `spec_token_delta`: **0 on every route**, `heroes measure
  spec/heroes-spec.md` 7502 legacy / **7636** cl100k / **10024** real (pinned
  2026-10-10, digest matching). Were a sentence put in spec § 1, offline:
  W +19, W-min +17, N +27, S +16, L +23 on the vendored maximum, against 156
  real tokens free beside the FFI floor.
- `removal`: nothing from the spec, and nothing is owed there. design.md
  §4.17, which carries no ceiling: W rewrites three ratified clauses (+39
  cl100k), N amends two and adds one refusal (+68).
- `needed_for_self_hosting`: **no**. The fixpoint holds without it, and
  `check selfhost/main.hero --json` holds **0** `elsewhere` fixes today. But
  the compiler's own migration needed the capability once, by a script
  (§ 3 below).
- `argument`: The spec holds no tool fact, and panel 193's R7 put this one in
  design.md §4.17, so no route touches §1.6. W's cost is in design.md:
  §4.17's ratified R4 makes `"output"` *the program `--apply` prints or
  writes, byte for byte*. W widens only `--in-place`, so `--apply` would print
  one fix set and write another, and the flag that says *where* would change
  *what*. `fmt --in-place` would still write one file. N leaves every existing
  invocation as it is, and §4.17's own rule refuses it at exit 2 where no
  single answer can hold it. Principle 0: the stopping rule's letter is unmet
  (0 metric-2 tasks, no harness types it); CLAUDE.md § 10 is met (step 1's
  one-off script).
- `prediction`: at the landing commit, whichever route lands, `heroes measure
  spec/heroes-spec.md` reads 7636 / 7502 and the real pin 10024 with its digest
  unchanged. And if W lands as briefed, `check probe/stage2/main.hero --apply
  --json` has 1 entry under `"not_applied"` without `--in-place` and 0 with
  it, which R4 says cannot happen unless §4.17 is amended in the same commit.
- `condition`: I approve W instead if the sitting widens **`--apply` itself**
  (printing and writing alike) and gives stdout an artifact that can hold
  several files, which the stopping rule then makes a subcommand. Or if a
  measured metric-2 effect shows a model failing to use a named flag. Any
  sentence entering the spec turns my verdict into a veto unless it names a
  removal or a registered prediction.

## 0. The ceiling, read today

`grep -n '1\.6' docs/design.md` at 02:11: design.md:255-256, *must fit in
10240 tokens, measured by `claude-opus-5` through `POST
/v1/messages/count_tokens`*. The payment rule is unconditional
(design.md:311-314): every amendment owes a named removal or a registered
prediction naming an instrument that exists today.

`./heroes measure spec/heroes-spec.md` at 02:15, in the tree, no `--refresh`:
claude-legacy **7502**, cl100k_base **7636** (the vendored maximum), real
**10024** (`claude-opus-5`, pinned 2026-10-10, shown because the digest
matches). Headroom 216 against 10240, **156** once the FFI floor's 60 is
deducted (the tool's own line).

## 1. Does the spec state what `--apply` writes? No, and it states no tool fact at all

`grep -n -- '--apply\|--in-place\|in-place\|in place' spec/heroes-spec.md`:
one match, :303, *grows in place* (a `push`), not the tool.
`grep -n -i -E 'heroes (check|build|run|fmt|test)|`check`|certain|guess|--json|flag|subcommand|command line|the tool' spec/heroes-spec.md`:
**zero** matches. The specification holds no sentence about the `heroes`
command, its verbs, its flags or its fixes. That is a ruling and not a
silence: panel 193's R7 (*the terms have one home, design.md §4.17 ... the
specification moves 0 tokens*), ratified 2026-10-06, and
`.claude/rules/spec-shape.md` (*the whole language and nothing else*).

**What the spec does say that decides this sitting** is spec § 1, :29-30:
*Every `use` starts at the directory of the file you compile.* That sentence
is why a nested module cannot be checked alone (it becomes *the file you
compile*, and its `use` lines are written from the root's directory), and it
is why a certain fix in a nested module is certain **relative to a root**,
not to a file. Two facts measured beside it:

- **A `use` cannot leave the root's directory.** In
  `probe/up/inner/main.hero`, `use ../geom` is refused `module_path_climbs`
  (*a module above that directory belongs to a different program*) and `use
  /geom` is refused `expected_module_name`; the production is `Use = "use"
  ident { "/" ident } ...` (spec § 1, :40). So every module a root reaches
  lies under the root's directory, except through a symlink. `examples/` from
  `tests/` is not a reachable shape.
- **Static closure of every root** (`scripts/closure.py`, run with `python3
  -I` over the 3,498 tracked `.hero` files of `git ls-tree -r 1dd890751`; a
  root is a file with `function main(` at column 0, each `use` resolved from
  the root's directory as § 1 says): **2,528 roots**, **792 nested modules**
  reached, 782 of them by one root, 8 by two, 1 by three, 1 by seven; **0**
  modules reached from roots in two different directories, so **0** whose
  own `use` lines resolve differently by root; **0** reached modules outside
  their root's directory after `realpath`; 1 root also reached through
  another's `use`. (Lane b20-tools counted 3,473 files and 2,504 roots at its
  own commit and definition; mine are the frozen tree's, by the definition
  above.)

So the cross-root hazard, a fix certain under root A written into a file root
B reads differently, has no instance today, and the language permits it (a
root in `X/` and one in `X/sub/` both reaching `X/sub/m.hero`). Whatever
route widens the write owes that premise a test that fires when it dies
(`.claude/rules/module-shape.md`); it is not a spec sentence.

## 2. Does design.md state it? Yes, twice, in a ratified paragraph

`grep -n -- '--apply\|--in-place\|in-place' docs/design.md`: design.md §4.17
(heading :2186), the paragraph panel 193 wrote:

- :2233-2235, `"output"`: *the program `--apply` prints or writes, byte for
  byte* ... *as `"not_applied"` the certain fixes in another module, which it
  never writes*;
- :2245-2246, `first_round`'s `elsewhere`: *another module's, which `--apply`
  never writes*;
- :2252-2254, *a flag no one answer can hold is refused at exit 2 rather than
  dropped*, `--in-place` without `--apply` among them.

**The paragraph's own status line is stale.** design.md:2229 says *panel 193,
provisional, author ratification pending*, and so does
`selfhost/cli/apply_answer.hero:2`; panel 193's synthesis reads **RATIFIED
2026-10-06**, *recorded as a reading* (its file, :217-220), and its decision
issue `issues/2026-10/06/2026-10-06-0237-panel-193-ratify-amend-or-overturn-r1-to-r11-a-fix-says.md`
holds `- [x]`. The shared brief's *panel 193's provisional `not_applied`
field* inherits the stale line. So route W amends **ratified** text: R2's
`elsewhere`, R4's `not_applied` and R4's *prints or writes, byte for byte*.
The two stale lines should be corrected whichever route lands; I file
nothing, the coordinator's to class.

The help says the same: `selfhost/cli/table.hero:96`, `--in-place`: *with
--apply, rewrite the file instead of printing it*; and `fmt`'s `--in-place`
(:124), *rewrite the file instead of printing it*, one file. Both are read
by `site/src/lib/claims.ts` (`.claude/rules/verification.md`'s map), so a
route that moves `table.hero` owes the site's build before its push.

## 3. The defect as the finder saw it, and the shape beside it (CL-078)

The lane's sentence is *no invocation, and no two composed, writes a certain
fix that sits in a nested module with a `use` line*. Probed in
`probe/stage/`, a root `use lib/geom`, `lib/geom.hero` holding `use
lib/util` and `y: i64 @ util.one()` (the old cell symbol, a **parser-stage**
certain fix): `heroes check lib/geom.hero --apply --in-place` answers
`rewrote lib/geom.hero`, exit 0, `@` to `@=`, run from the root's directory
and from `lib/` alike. The parser speaks first and `--apply` stops at the
stage that spoke first (design.md:2249-2250), before any `use` is resolved.

The lane's own case reproduces in `probe/stage2/` (`y @= util.one()` never
re-bound, a **checker-stage** certain fix): the root's `--apply --in-place`
names `lib/geom.hero:4` not applied and writes nothing, exit 0; the module
alone writes nothing (`no certain fix applies to its 1 diagnostic(s)`); the
root's `check --json` marks the fix `"first_round": "elsewhere"`.

So the gap is **stage-bound**: a certain fix from a stage that runs before
`use` resolution is writable one file at a time today; one from name
resolution or later, in a module with a `use` line, is writable by no
command. That is exactly how M-inferred-cell went: step 2's parser fix moved
1,181 files one invocation at a time (`git show --stat 65cd1145d`: 1,181
files, 454 of them `.hero` files in subdirectories of `selfhost/`), while
step 1's **checker** fix, 391 `@` to `=`, was applied *from `check --json`'s
byte spans ... with a one-off in the lane's scratch, which is not a project
instrument (CLAUDE.md § 10)* (task `issues/2026-10/10/2026-10-10-1923-…`,
its item). The one-off also wrote fixes design.md:2247 tells a tool not to
write (*the certain fixes marked `written` and nothing else*).

**Present demand**: `check selfhost/main.hero --json` and `check
tests/harness/main.hero --json`, exit 0 each, hold **0** `elsewhere` fixes
(and 0 certain fixes at all). The capability is needed at the next language
change that brings a checker-stage certain fix, not today.

**What the defect's title measures and no route as briefed moves**: *a
migration over a tree is one command per root*. W and N write a root's
nested modules; neither removes the per-root count. By the static closure,
2,527 of the 2,528 roots are reached by no other root, so a tree-wide
migration stays about 2,527 invocations under W or N; what they change is
that the 792 nested modules' checker-stage fixes become writable at all. For
`selfhost/` (one root) it is one command.

## 4. The routes, from this seat

**(W) widen `--in-place` to every module the root reaches: object.** Spec 0
tokens; design.md §4.17 +39 cl100k (`drafts/design-W.md`, three edits: the
`output`, `not_applied` and `elsewhere` clauses). The objection is not the
price. Since W widens `--in-place` and not `--apply`, by the route's own
definition `check --apply` prints the root with the root's fixes while
`check --apply --in-place` writes the root and every reached module. R4's
ratified *the program `--apply` prints or writes, byte for byte* stops holding,
and `"not_applied"` and `first_round` would answer differently according to
whether a flag that only says *where* is present. Each round re-reads the
modules from disk with only the root's text held (`selfhost/cli/check.hero:186`),
so writing the others means holding them across rounds: `--in-place` would
change the computation, not only its destination. The three-slot contract
holds formally (stdout empty, every written file named on stderr, exit 2 on a
failed write). What breaks is cli-surface's *a mutating flag is `--in-place`*,
which `fmt` keeps meaning one file (table.hero:124), and R4's identity of
print and write. The fixture comment `tests/harness/suite_fixes.hero:320-321`
pins root-only (*`applyx/main.hero`, which is left as it is*). Whether that
suite goes red under W is the compiler-engineer's to run, unrun here.

**(N) a new flag beside `--in-place`: approve, on conditions.** Spec 0
tokens; design.md §4.17 +68 cl100k (`drafts/design-N.md`, the flag spelled
`--reached` for pricing only; its name is not this seat's). It fits the
stopping rule's flag shape: the same input, the same question (apply every
certain fix), answered over the root's closure. Every existing invocation
keeps its bytes, so `elsewhere` and `not_applied` keep their ratified meaning
by default. Without `--apply --in-place` it is refused at exit 2 by §4.17's
own rule (:2252-2254), since stdout holds one program. Conditions: (a) its one
home is design.md §4.17, nothing in the spec; (b) the root-only note (defect
621's message) names the flag, so the step after the note is one invocation
and no hand edit; (c) the cross-root premise of § 1 gets a test that fires when
a module is reached from roots in two directories; (d) the stale *provisional*
lines are corrected in the same landing.

**(S) the root only, the message naming the modules: object as the
resolution; it is the conservative alternative.** 0 tokens anywhere. It
leaves the capability outside the binary. The next checker-stage migration
repeats step 1's one-off script against CLAUDE.md § 10, and §4.17's *a
`certain` fix ... may be applied mechanically* stays false for checker-stage
fixes in any of the 792 reachable nested modules. Its message half has
already landed (defect 621, `apply_answer.hero:104-120`).

**(L) leave a package's library modules alone: object, under Principle 0.**
It has no referent. No package of Heroes modules exists (design.md:832-834,
*no package manager exists before modules do*; :3266, *no package
hierarchy*). A `use` cannot climb (`module_path_climbs`, measured), and 0
reached modules lie outside their root's directory after `realpath`. An
exception with no instance waits. When `heroes add` arrives it is that
sitting's question.

**Routes nobody listed** (CL-057; enumerated from the stopping rule's three
shapes and from what spec § 1 makes a module's meaning depend on):

- **(D) a directory operand**, `check <dir> --apply --in-place`, every root
  under it. That is what *one command for a tree* needs (§ 3), and `mutate`
  and `probe` already take an optional directory operand
  (`table.hero:158-159`). It is a separate question for the stopping rule,
  outside 607's ask; the sitting should know that W and N do not deliver the
  title without it.
- **(R) name the root a module is read from**, e.g. `check lib/geom.hero
  --root main.hero --apply --in-place`, writing only the file named. It keeps
  `--in-place` meaning the named file. But a migration then becomes one
  command per module, which is a loop, which is a script (CLAUDE.md § 10).
  Weaker than N.
- **(P) change spec § 1 so a module checked alone resolves its `use` from its
  root**. A language change, spec tokens against 156 free, and it would make
  a module's meaning depend on a search. Refused from this seat.
- **(J) let a JSON consumer write `elsewhere` fixes**. design.md:2247 forbids
  it, and for the project it is the script CLAUDE.md § 10 refuses.

## 5. §1.2 and Principle 0

§1.2 (tokens × (1 + rewrite rate)): **unmeasured.** Metric 2 holds **0**
tasks (`docs/metrics/tasks.md`, *Status: 0 tasks*), so no route can claim a
measured Part 11 effect. As a question, not a premise: under S a model meets
a checker-stage fix in a nested module as a hand edit (§1.2 prices a round
trip at 500 to 2,000 tokens); under N as one more invocation named by the
note; under W as none. W's advantage over N is that word, unmeasured; N's
advantage is that no existing invocation's answer moves.

Principle 0 / the stopping rule: *a capability enters the surface only if the
fixpoint invocation, the golden harness or the design.md Part 11 harness must
type it, or it has a measured Part 11 effect*. By its letter, **unmet**. The
fixpoint invocation does not type it, the harness judges `applyx/geom.hero` as
its own root instead, and Part 11 has no tasks. **The document does not
cover this case**: the compiler's own source migrated across a language
change with a checker-stage certain fix is none of the three. What does cover
it is CLAUDE.md § 10, *any new capability is a `heroes` subcommand or flag ...
never a script*: the capability already existed once, as step 1's one-off.
That is the burden met, by one measured instance. I'd have the sitting say
which rule it rests on rather than stretch the stopping rule (panel 179's
precedent, cli-surface.md:31-38).

## 6. Recommendation

**N.** It costs 0 spec tokens and 68 vendored on design.md §4.17, its one
home. Every ratified field keeps its meaning by default, the new flag is
refused at exit 2 without `--apply --in-place`, the defect 621 note names the
flag, and the cross-root premise gets its test. *Conservative*: S, which
leaves the next checker-stage migration to a script. *What W would need to
become robust*: widen `--apply` itself, with an artifact that can hold several
files, which the stopping rule makes a subcommand rather than a flag. The
defect's title, one command for a tree, is route D's question, to be filed
apart if the sitting wants it.

## Files this seat wrote

All under `.claude/worktrees/scratch-b15/211-spec-warden/`: `drafts/spec-{base,W,W-min,N,S,L}.md`,
`drafts/design-{S,W,N}.md`, `probe/{up,stage,stage2,applyx}/`, `probe/*.json`,
`build.log`, and `scripts/` (`closure.py`, `drafts.py`, `design_drafts.py`,
`tracked.txt`). The four `scripts/` files were first written in the session's
scratchpad, which is outside the repository root, against CLAUDE.md § Hard
stops; they were moved here between 02:22:47 and 02:24:19 by `date` and the
scratchpad holds nothing of this seat's. No paid run, no commit.
