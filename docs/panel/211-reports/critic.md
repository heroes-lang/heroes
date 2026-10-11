# Panel 211, completeness critic: what the reports leave out

Seat: completeness critic, one pass after the three reports, no verdict of my
own. Opened 02:44 by `date` (2026-10-11). Read: the four briefs, the three
reports, CLAUDE.md § RUN IT and § Precedence, `.claude/rules/cli-surface.md`,
design.md §4.17 (`grep -n 'never writes' docs/design.md`: :2235, :2246), panel
193's file, panel 099's file, defect 607, defect 380, M-inferred-cell's task
`…-1923-…`. Tree: `.claude/worktrees/scratch-b15/211-critic/tree/`
(`1dd890751`), the compiler built there from the seed at 02:46 (`build.log`,
exit 0), copied to `bin/heroes-base`; the compiler-engineer's built route
copied from its directory to `bin/heroes-w` and run only in mine. Probes under
`probe/`, the one script under `scripts/`. No paid run, no commit. Every number
carries its command.

## The findings that would change the resolution, most important first

1. **The two seats that disagree on W were judging two different W's, and the
   built one falsifies the spec-warden's prediction and meets half of its own
   condition.** The brief's W widens `--in-place`; the compiler-engineer's
   `route.diff` widens `--apply`'s computation itself. Run on the spec-warden's
   own probe (`probe/stage2`): `heroes-w check main.hero --apply --json` holds
   `"not_applied": []` and `"modules": [{"file": "lib/geom.hero", ...}]` with
   and without `--in-place`, and the two documents are byte-identical (`cmp
   out1.json out4.json`, exit 0); `"output"` equals the plain `--apply` stdout
   (`python3 -I`, `True`). The spec-warden predicted 1 and 0. What survives of
   its objection is narrower: the plain `--apply` stdout still holds the root
   alone while `--in-place` writes every module, and §4.17's two *never
   writes* clauses are falsified (both seats agree on that half).
2. **The built W also changes `check --json` with no `--apply` at all, under
   the same `"schema": 2`, and no seat priced it.** `heroes-w check main.hero
   --json` marks the module's fix `"first_round": "written"` where the base
   says `"elsewhere"` (stage2 and p2, below; `route.diff` keeps `"schema": 2`,
   its lines 118 and 131). §4.17's ratified rule tells a tool to write *the
   certain fixes marked `written` and nothing else*, so under W every schema-2
   consumer that obeys it writes into modules too, and the root-relative hazard
   reaches it with neither `--apply` nor `--in-place`. Panel 193's R3 gave the
   number to exactly this: a consumer refusing an answer whose meaning moved.
   N leaves it as it is by default.
3. **The compiler-engineer's soundness finding is panel 099's "leaf-test
   hazard", ratified 2026-09-03, with a standing historian prediction and a
   pinned golden; no seat and no brief cited it** (`grep -n '099\|leaf'` over
   the three reports and four briefs: 0 lines). The project ruled the READ: the
   decision `issues/2026-09/03/2026-09-03-0527-compiling-a-file-below-a-nested-program-s-root-re.md`
   (closed, M-robustness-guards step 6) pinned `tests/golden/surface-fixtures/nested/decoy/`,
   and `tests/harness/suite_surface.hero:674` names it *beside a decoy module
   the same leaf checks clean against the wrong file, which is the stated
   hazard and not a defect*. p2 is that decoy plus a certain fix WRITTEN from
   the wrong reading, which neither the row nor the decision covers, and
   panel 187's promise (ratified 2026-10-03, a month later) is about writing.
   The compiler-engineer's condition 2 asks to pin p2 as a new case; its
   sibling exists. Panel 099 R5:
   *compiling a file below a program's root re-bases its paths; that is the
   rule's own consequence*, its owed mitigation a diagnostic for a path that
   reads nothing there; its historian's prediction, *the first defect filed is
   Zig #13970's shape*, is marked standing (`docs/panel/099-where-a-use-path-starts.md`
   :100-112, :168, :213-215). The owed diagnostic has landed (`check
   examples/shapes/render/ascii.hero` names *from the directory of ..., the
   file you compile*, run at 02:49), and p2 is the case it cannot see: a
   DIFFERENT file exists at the leaf's directory, so nothing is unknown. I reproduced it on the base
   for a named leaf (`probe/p2-named`): `check lib/m.hero --apply --in-place`
   from `d/` answers `rewrote lib/m.hero`, exit 0, and `check a.hero` then
   exits 1 on `type_mismatch`. `.claude/rules/verification.md` § Bounded
   discovery lists *a `certain` fix that writes a program meaning something
   else* as `blocking`. Whether this is that, or a consequence §4.17 must
   state, is the question the sitting had to answer BEFORE W against N (§ 7).
4. **The L dispute is settled by the language, and the historian's L, and its
   prediction, rest on a shape the brief named and the language refuses.**
   `use ../ex/m` is refused `module_path_climbs` (`probe/climb`, exit 1), and
   `git ls-tree -r 1dd890751 | awk '$1=="120000"' | wc -l` reads **0** tracked
   symlinks. So *`examples/` from `tests/`* (shared brief, question 3) cannot
   happen, a package has no referent (spec-warden), and the only escape is a
   link (compiler-engineer's `p5`): L is a landing guard, not a package rule.
   Cargo rejected a path-prefix boundary (#13792, as the historian read it,
   not fetched here) because path dependencies and `#[path]` leave the
   workspace; Heroes has no
   climb and no search path, so here the prefix IS the closure. And L does not
   touch p2: every file of p2 is under the root's directory.

Two more that bind the landing rather than the verdict: **W's landing moves an
outward-facing promise** (`site/src/llms.txt:63-66`, *the certain fixes it left
in other modules*, held by no instrument: `grep -n -i 'apply\|in-place\|llms'
site/src/lib/claims.ts`, 0 lines), and a push touching `site/` publishes the
site: CLAUDE.md § Hard stops asks the author's yes before the commit, so a
truthful W cannot *land tonight* whole while the author sleeps; and **neither W nor N delivers 607's title**, one
command for a tree (the spec-warden: about 2,527 invocations under either),
which two seats reached independently by routes the brief did not list (the
spec-warden's D, the historian's Go pattern).

## 1. Framing facts found false

| the framing said | what is true | how settled |
|---|---|---|
| panel 193's `not_applied` *provisional* (shared brief) | **RATIFIED 2026-10-06**, recorded as a reading | panel 193's file :217-220, read; design.md:2229 and `selfhost/cli/apply_answer.hero:2` still say *provisional*, both stale (read at 02:51). W amends ratified text, R2's `elsewhere` and R4's `not_applied` |
| a module reached through `use` outside the root's directory, *`examples/` from `tests/`* (question 3) | unreachable: no climb, no search path, 0 tracked symlinks | `probe/climb` (`module_path_climbs`, exit 1); `git ls-tree` above; the compiler-engineer grepped the loader for `package` (2 comment lines) |
| *no invocation and no two composed writes a certain fix in a nested module with a `use` line* (607's measured line, the brief) | true for a fix of name resolution or later; a parse-stage fix in such a module is written today by naming it | the spec-warden's `probe/stage`, the compiler-engineer's `probe/old`; not re-run here |
| M-inferred-cell's 391 cells *ran the verb over every root one at a time* (607's body) | step 1 applied them with a one-off from `check --json`'s spans | the task file's item, read; the script `.claude/worktrees/scratch-b15/lane-inferred-cell/apply_json_fixes.py`, read: it writes every certain `never_rebound` fix in whatever `file` the root's answer names, a module's as readily as the root's, which is W's behaviour by hand (how many of the 391 sat in a module is not measured here); the task says a census found no old form afterwards (not re-run) |
| the M-inferred-cell task proposed a flag and asked *whether an invocation may write files it did not name* | the brief carried the lane's *no new flag* and not the task's `--all-modules` or its question | the task file, read |
| 3,473 tracked `.hero` files, 2,504 roots | 3,498 at `1dd890751`; 2,528 roots with `function main(` (spec-warden), 3,498 checked as roots (compiler-engineer) | `git ls-tree -r --name-only 1dd890751 \| grep -c '\.hero$'`: 3498 |
| W, as one route | two: the brief's (widen `--in-place`) and the built one (widen `--apply` and `check --json`) | finding 1 above |

## 2. The central contradiction, run on the built route

The spec-warden objects to W because *`--apply` would print one fix set and
`--in-place` write another*; its prediction: on `probe/stage2`, `--apply
--json` has 1 `not_applied` entry without `--in-place` and 0 with it. Run in
my copies (`probe/s2-base`, `probe/s2-w`) at 02:47:

| invocation | base | built W |
|---|---|---|
| `check main.hero --apply --json` | `not_applied: ["lib/geom.hero:4"]`, `rounds: 0`, the module untouched | `not_applied: []`, `modules: [lib/geom.hero, its text with y = util.one()]`, `rounds: 1`, the module untouched (`cmp`, SAME) |
| the same with `--in-place` | the same document, nothing written | **the same document** (`cmp`, exit 0), `rewrote lib/geom.hero` |
| plain `check main.hero --apply` | the root on stdout, the note *not applied* | the root on stdout, the note *1 other module(s) hold certain fixes, which `--in-place` writes and `--json` holds* |
| `check main.hero --json` | `"first_round": "elsewhere"` | `"first_round": "written"` |

So on the built route the JSON artifact is one and the flag only says where,
and R4's `"output"` (the root's bytes, printed or written) still holds
literally; what moves is `not_applied`, `elsewhere` and `rounds`, each a
ratified meaning under an unchanged schema number. The spec-warden's own
condition for approving W was *widen `--apply` itself and give stdout an
artifact that can hold several files*: the built route does the first and, for
`--json`, the second; the plain stdout cannot hold several files and still
prints one. The sitting should say which half it requires.

**The contract each seat reads.** The compiler-engineer's reason for W over N
is `selfhost/cli/check.hero`'s comment on `settle`, *this flag's contract is
EVERY certain fix* (read, :159-162 of the frozen tree). design.md §4.17,
ratified, says *which it never writes*. The design document is the source of
truth (CLAUDE.md, its first paragraph); a comment is the implementation's.
That does not decide W, since a sitting may amend §4.17, but the sitting should
not read the comment as the contract.

## 3. The L dispute and the counts behind it

- **Reached from roots in two directories: 0 (spec-warden) against 1
  (compiler-engineer).** Both measured; the definitions differ. The one module
  is `examples/shapes/render/scale.hero`, reached from
  `examples/shapes/main.hero` and from `examples/shapes/render/ascii.hero`
  checked as a root. `ascii.hero` has no `function main(` (`git show
  1dd890751:<file> | grep -nE '^use |^function main\('`), and checked alone it
  is refused `unknown_module` on `use geom/point` (run, exit 1). And
  `render/scale.hero` holds **no `use` line** (same grep), so its meaning
  cannot depend on the root. Exposure: 0 by either definition.
- **Do the roots that share a module disagree about it today?** Over the
  compiler-engineer's census (3,498 `check --json` documents, read, not
  re-run), `scripts/disagree.py`: 447 modules read by two roots or more; **0**
  whose diagnostics differ between roots; **0** whose certain fixes differ. The
  whole census locates 21 of 9,996 diagnostics outside the root, in 14 files.
- **A shape beside the directory one**: generics have no constraints, so I
  asked whether a module's diagnostic depends on the root that instantiates it
  (`probe/gen`, `twice<A>(x: A)` doing `x + x`, one root passing `2`, one
  `true`): both roots get the same `bad_operand` at `lib/g.hero:2:9`, checked
  at the definition. So root-relativity comes from `use` resolution only, as
  far as this probe reaches; other shapes are unprobed.
- **L.** No climb, 0 symlinks, no search path: L reduces to the compiler-
  engineer's condition 3, a landing outside the root's directory refused.
  The historian's prediction (*the first defect against W without L is a write
  outside the root's package*) can come true only through a link the tree does
  not hold; as written it is close to unscorable, and its source shape
  (`examples/` from `tests/`) came from the brief.

## 4. Claims asserted and not measured

| claim | seat | status |
|---|---|---|
| `--apply` prints one set and `--in-place` writes another; 1 against 0 | spec-warden | **run now**: false on the built route (§ 2); true of the plain stdout only |
| `--in-place` would change the computation, the modules held across rounds only then | spec-warden | **run now**: the built route holds them with or without `--in-place` (`--apply --json` without it carries `modules`, `rounds: 1`) |
| the class exists today for a named file | compiler-engineer | **reproduced** (`probe/p2-named`) |
| p2 under W breaks a clean program | compiler-engineer | **reproduced** (`probe/p2-w`: `rewrote lib/m.hero`, then `a.hero` exit 1) |
| a full disk mid-set leaves a mixed set, each file whole | compiler-engineer | unrun, said so; not run here |
| N about 10 lines more, W1 under 5, the landing guard about 10, an identity door about 20 | compiler-engineer | priced, unbuilt; not settled here |
| 0 modules reached from two root directories | spec-warden | measured under its definition; reconciled above |
| W is precedent (cargo fix, rustfmt write the closure) | historian | sourced; but its own condition, *if the closure is not one program the way a crate's `mod` tree is*, bears here: 447 Heroes modules are read by two roots or more (the census) and p2 shows one file meaning two programs, where a Rust module file is ordinarily declared by one parent `mod` (my inference, not fetched). The precedent's unit may not match; the historian should say whether its reading changes |
| a grep of `docs/panel/` and `issues/` for an earlier sitting | historian | unrun (no shell); it would have found panel 099 R5 and panel 193's own line *on `applyx` it writes a module `--apply` never writes* (193's file :44-46) |
| 607 is `improvement`, *no message is false* | the card | the base's p2 is a question of class (finding 3), not settled here |

## 5. What a W landing owes that no seat counted

- **Published text.** `site/src/llms.txt:63-66` says `--apply --json` holds
  *the certain fixes it left in other modules*; `site/src/html/docs/errors.html`
  :42-48 and its Italian twin show `check --apply --in-place program.hero`.
  No instrument holds the llms.txt sentence (the `claims.ts` grep above, 0).
  The spec-warden named only `table.hero` through `claims.ts`. A truthful W
  amends `site/`, and that push publishes the site: CLAUDE.md § Hard stops,
  asked for, every time. N, its default unchanged, leaves these true.
- **The harness on the tracked tree.** `grep -n -- '--in-place'
  tests/harness/*.hero | grep apply`, then each row read: `suite_fixes.hero`
  :477 and :522 and `suite_surface.hero` :313 and :370 run on a scratch copy;
  `suite_surface.hero:684`, `:685` and `:693` name a tracked path in their
  argv. Under the base
  they write nothing only because those roots hold no fix; :684 rewrote a
  tracked fixture under W (compiler-engineer). The shape is today's, whatever
  route lands; under N it stays dormant unless a row passes the new flag.

## 6. Routes nobody listed

Enumerated from the reports' own route lists (spec-warden D, R, P, J;
compiler-engineer R, W1, identity; historian P, V) and from what p2 needs.

- **(C) consensus over a set of roots.** With a directory operand (the
  spec-warden's D, the historian's Go pattern), check every root under it and
  write a module's certain fix only where every root that reads the module
  computes the same fix, else refuse and name the roots that disagree. It
  refuses p2 (`a.hero` holds 0 diagnostics in `m.hero`, `b.hero` 1 certain fix;
  run above) and, measured on the census, would refuse 0 of today's 447 shared
  modules. Sound within the operand, the root set given by argv, so no project
  file and no fourth input class. Unbuilt, unpriced.
- **(X) write, re-check, back out** (cargo's, which the historian cited and no
  seat turned into a route): after writing, check every root under the operand
  that reads a written module and restore the files on a new diagnostic.
- **(T) two invocations that already compose**, the stopping rule's third
  shape, asked rather than assumed: `check --apply --json` already returns
  every module's fixed text under the built route's `modules`; whether *write
  that document's files* is a flag or a composition is the stopping rule's
  question, and nobody asked it.

## 7. The question the sitting should have asked

**Is a `certain` fix a fact about a file, or about a program, the root's
directory included?** Panel 099 made a module's meaning a function of the
directory compiled from, and ruled the leaf's wrong READ *not a defect*
(`suite_surface.hero:674`); panel 187 promises a certain fix *repairs the
mistake its message names and writes nothing else*; p2 shows the two meet in
one file today, by naming it, and no ruling says whether 099's *not a defect*
extends from reading to writing. If certainty is per program, then no spelling of the flag
(W or N) makes writing a shared module sound, and what does is a refusal, a
consensus over a stated set of roots, or §4.17 saying the limit out loud; if
it is per file, the base's p2 is a `blocking` defect of `certain`. Only then
does W against N mean anything. The second question it should have asked is
607's acceptance test: *one command for a tree* is a directory operand's
question, and neither briefed route answers it.

## Files

Under `.claude/worktrees/scratch-b15/211-critic/`: `build.log`,
`bin/heroes-base`, `bin/heroes-w` (a copy of the compiler-engineer's),
`probe/{stage2,s2-base,s2-w,p2,p2-base,p2-w,p2-named,p2orig,gen,climb}/`,
`scripts/disagree.py`. Closed at 02:54 by `date`, 2026-10-11.
