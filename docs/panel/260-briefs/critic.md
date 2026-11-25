# Panel 260, the completeness critic

Two readings, each its own report under `docs/panel/260-reports/`, written in your own copy and copied to the trunk's
same path when you finish or stop. You give no verdict on any route. Your copy is a detached worktree the coordinator
makes, `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-critic` (or the path the coordinator's prompt names),
at the base `00-shared.md` names; build its compiler from the seed and stage 2 there if a finding needs one (§ 10 of
`00-shared.md`). **The first reading was made at `80e2e39ad`** (`critic-briefs.md`, done 11:14); the briefs were then
re-frozen at **`c3885b81b`** and repaired from its last section, so the second reading's copy stands at `c3885b81b`
(`git checkout --detach c3885b81b` in a copy made before), and a compiler built at `80e2e39ad` is not the base's. **You hold this Mac and both Linux images** (`heroes-linux-arm64:latest`, and `heroes-linux:latest` run
with `docker run --pull never` and no `--platform`), one container at a time, `--rm`, named `p260-critic`, removed when
idle. **No paid run is yours to start and no credential yours to print** (the blind seat's run is the coordinator's, on
the author's authorisation, after the synthesis); no GitHub write (`gh` reads only); no file outside the repository's
root (no `/tmp`); no `pkill`; never read, build or run inside a seat's copy or the blind seat's area.

## First reading: the briefs, before any seat starts (`critic-briefs.md`)

Read `00-shared.md`, the five seats' briefs (`compiler-engineer.md`, `ffi-pragmatist.md`, `spec-warden.md`,
`historian.md`, `llm-ergonomist.md`), `blind/registration.md` and `blind/task-t1.md`, this file, and what `ground/`,
`programs/` and `shapes/` hold; then the sources they cite (`docs/panel/234-*.md` R6 at lines 400 to 428;
`docs/panel/253-*.md` R3 at 727; issues 2289, 2625, 1399 (defect 134), 1786 (defect 443) with `gh issue view`; the
lane's three commits, `git show a2b288e5d`, `0097b3f26`, `dec556d50`; spec § 1 and § 13; `.claude/rules/cli-surface.md`
§ Four input classes; `selfhost/check/runtime_header.hero` and `selfhost/cli/runtime_bound.hero`). Then:

- **re-run every framing fact by command**: each number, path, line, count and quotation; the timing table of § 4.2 at
  least one row of it, interleaved as `ground/time.sh` does, its load stated; § 3.1's counts; § 6.1's nm closure counts
  on both platforms; § 6.2's shapes under the base's compiler, and its reading of the nine programs
  (`ground/own-bound.sh`; ten since the repair, the tenth two includes deep);
- **search or question every negative sentence and every claim about the option set** (*none*, *no*, *never*, *only*):
  § 3.2's *no package-side file in Rebel*, beside spec § 1's *"the package's own catalogue"*; § 6.2's table of which
  reading sees which shape; § 7.1's list of homes; § 7.5's routes;
- **read the blind registration** for what it measures (does `task-t1.md` name the headers alike, three at the first
  reading and four since the repair, and does
  anything in it tell a reader which needs a declaration?), for a leak of the project's names, and for a rule that
  cannot be graded the same way by two graders;
- **say which seat was handed a fact it cannot check**, and which fact a seat needs and was not handed (§ 4.3 and § 4.4
  are carried, not re-run: say whether any conclusion rests on them alone; the historian has the web and no shell, the
  ffi-pragmatist a shell and no web);
- **name the home or the reading nobody listed**, and the shape beside § 6.2's that nobody ran (CL-061: none, one, two
  families, nested includes, a package, a face over a floor, a generic over a bound type, a `heroes test` build, a value
  probe, an MSVC target, a Swift member's C entry);
- end with `## What the coordinator must repair before any seat starts`, most important first.

## Second reading: the reports, before the synthesis (`critic-reports.md`)

You are a **fresh agent** in this reading, launched after the seats; this file is then your brief again. Read the briefs
as repaired, your first reading, then every report under `docs/panel/260-reports/` and the programs the seats kept under
`programs/<seat>/`. **Check the repair itself against your first reading's last section, item by item** (items 1 to
12): a repair that is missing, that carried a number it says it ran, or that is wrong. Name what is missing: a home or a reading nobody listed, a claim asserted and not measured with the
command that settles it, a contradiction between seats and which side is checkable, **a repair of the briefs that is
itself wrong**. **Run the seats' programs on the platform they did not**: the compiler-engineer's in the x86-64 image,
the ffi-pragmatist's doors under the compiler-engineer's built check; and **three shapes of your own** beside § 6.2's,
each undeclared and declared, under the base and under the built check. Then: does the check the seats would adopt see
every door the ffi-pragmatist found, refuse no correct declared program, and tell every undeclared one at its writer's
line, with no fallback, on every platform it claims? Does an undeclared program pay nothing of the layer, warm and cold?
Does the spec-warden's text say what the built check refuses, clause by clause, and do its registered predictions read
the blind registration's rules? Does the historian's every `verified` precedent quote its page? End with `## What the
synthesis must settle`, most important first.
