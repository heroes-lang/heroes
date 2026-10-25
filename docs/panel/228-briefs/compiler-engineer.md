# Panel 228, the compiler-engineer's brief

Read `00-shared.md` in this folder first, and `../228-reports/critic-briefs.md` for the commands
behind its facts. This brief adds what only you are asked.

**Your worktree**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/228-compiler-engineer`,
detached at `9a38f1b26`, made by the coordinator. Build your own compiler inside it, `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes` (8.29 s in the lane), then `./heroes build
selfhost/main.hero -o heroes-next` (82.24 s real in the lane) to measure a change to `selfhost/`.
Never use the lane's, the trunk's or another seat's compiler; never read, build or run in another
seat's worktree; write nothing outside the repository's root; print no credential; start no paid run.

**What you are asked**, each answer with the file, the line and the command that settles it:

1. **The price of each route** (A, A', B, C, D, E) in the unit `tests/harness/suite_layout.hero`
   counts, not `wc -l`, and which touched file would cross its ceiling or its `DECIDED` row:
   `selfhost/check/holes.hero`, `selfhost/cli/check.hero`, `selfhost/cli/check_json.hero`, any module
   a route adds, and for C the grader, `tests/harness/thesis_grade.hero`.
2. **One value, two renderings**: today the report is a string built line by line. What a hole's
   record holds so the text and the document are two renderings of one value, and where each of the
   three defects is repaired once in it: **2203**, whether the resolver can answer *the names in scope
   at this position* (its scopes, `--dump-scopes`) so the report stops modelling visibility itself;
   **2204**, whether the checker knows, at a hole, the type a `for`'s array, a branch's value or an
   unannotated binding's later use requires; **2205**, how `render_ty` would write a type the way the
   hole's file writes it (`nearby` already qualifies a function by the file's `use` name, panel 100).
   Their repairs are not this sitting's to rule; where they land, and whether the document should
   wait for them (question 5), are yours to measure.
3. **Holes beside errors** (question 1): whether `c.holes` exists, and is right, when the type stage
   reported a diagnostic; what reporting it would cost; and whether a hole can be reported when an
   earlier stage (parse, resolve) spoke.
4. **The readers**, the shared brief's list: what each route moves in `selfhost/mutate/json_read.hero`,
   `judged.hero`, `tests/harness/fix_names.hero:97-99`, `suite_surface.hero:315`,
   `apply_tree.hero:91-92` (question 4's own witness), `thesis_sha256.hero:14` and
   `thesis_grade.hero:151-171`, and in the twelve golden programs that hold `???`.
5. **Build the route you would adopt**, in your worktree, far enough that the document is real, with
   the keys of the shared brief's proposal or the reason for others (the blind seat reads those keys):
   run it on `.claude/worktrees/rechecked-issues-scratch/hole/` (`main.hero`, `both.hero`, `s1.hero`
   to `s7.hero`) and on the two-module tasks of `.claude/worktrees/rechecked-issues-scratch/blind228b/`
   (`w1/`, `w2/`), copying them, never writing there; then the compiler's own tests (`./heroes test
   selfhost/main.hero`) and the `check` and `full` golden forms (`./heroes run
   tests/harness/main.hero -- ./heroes-next check`, then `full`). A route that does not build is not
   adopted.
6. **Time**: `heroes check selfhost/main.hero` before and after, `/usr/bin/time -p`, `real` against
   `user` plus `sys`; never slow the compiler (CLAUDE.md § Verification).

**Output** as `.claude/agents/compiler-engineer.md` asks: verdict per route, design.md section, cost,
a falsifiable prediction with the milestone it is checkable at, and your condition. **Write
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rechecked-issues/docs/panel/228-reports/compiler-engineer.md`
as you go**, not only at the end, so a stopped seat leaves what it had.
