# Panel 261, the completeness critic

Two readings, each its own report under `docs/panel/261-reports/`, written in your own copy and copied to the trunk's
same path when you finish or stop. You give no verdict on any route. Your copy is a detached worktree the coordinator
makes, `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/261-critic` (or the path the coordinator's prompt
names), at the base `00-shared.md` names (`df92f9281` since the repair; your first reading read `07e747bc6`); build its compiler from the seed and stage 2 there if a finding
needs one (`00-shared.md` § 8: the seed's binary is older than `selfhost/` and prints `STALE` on the spec). **You hold
this Mac and both Linux images** (`heroes-linux-arm64:latest`, and `heroes-linux:latest`, run with `docker run --pull
never`), one container at a time, `--rm`, named `p261-critic`, removed when idle. **No paid run is yours to start and
no credential yours to print** (the blind seat's run is the coordinator's, on the author's authorisation of 2026-11-26,
after the synthesis); no GitHub write (`gh` reads only); no file outside the repository's root (no `/tmp`); no `pkill`;
never read, build or run inside a seat's copy or the blind seat's area.

## First reading: the briefs, before any seat starts (`critic-briefs.md`)

Read `00-shared.md`, the five seats' briefs (`compiler-engineer.md`, `ffi-pragmatist.md`, `spec-warden.md`,
`historian.md`, `llm-ergonomist.md`), `blind/registration.md`, `blind/task-t1.md` to `task-t3.md` and the references,
this file, and `ground/`; then the sources they cite (spec § 3, § 5, § 9, § 11, § 13 at the lines named; design.md
§4.8 lines 1521 to 1631, §4.13 lines 2062 to 2121, Part 6 lines 3643 and 3644, Part 8 item 19 lines 4582 to 4640, each
+4 from your first reading's base;
`docs/panel/109-*.md:378-386`, `112-*.md:293-299`, `217-*.md:105-126` and its reports, `252-*.md:565-579`,
`254-*.md:157`; issue 1734 with `gh issue view`; `rebel/library/0.1.0/files/web.hero:1-60` and `:100-335`,
`http.hero:2330-2350`; the example files of `00-shared.md` § 5.3 and § 5.4). Then:

- **re-run every framing fact by command**: each number, path, line, count and quotation, the probes of § 3.3 and
  § 3.4 under your own stage 2 (`ground/probe/`), § 5.2's census, § 5.3's route and dispatch counts (notes' 35 `if
  name ==` against the coordinator's 34), § 4's line counts, § 3.6's counts, and the references' three runs;
- **search or question every negative sentence and every claim about the option set** (*none*, *no*, *never*,
  *only*, *not a form*): § 2 question 4's *"`Router<S>` is not a form"*; § 3.5's list of who weighed the form before
  (searched by `grep` over `docs/panel/` and `gh issue list --search` for the words named there: run wider words); § 5.4's
  *"unread"* for `agent`, `iothub` and `monitor`; § 6.2's *"none compiled"*; § 7's list of routes;
- **read the blind registration** for what it measures (can a `now` reader meet each task at all; does any task's
  wording tell the reader which arm it reads or which form to use; is *bound* graded the same by two graders on the
  references; does D3 read a refusal the landed rule may not make; does t2 or t3 push a reader into `never_rebound`
  under `adopted` by its wording rather than by the text), for a leak of the project's names, and for a rule that
  cannot be graded the same way by two graders;
- **say which seat was handed a fact it cannot check**, and which fact a seat needs and was not handed (the historian
  has the web and no shell, the ffi-pragmatist a shell and no web; the counts of design.md Part 8 item 19 and panel
  112 are *carried*: say whether any conclusion rests on them alone);
- **name the route or the shape nobody listed** (CLAUDE.md § RUN IT: none, one, nested, generic, a field, a map, a
  callback, a thread, a library function's twin, a function value crossing modules, a value read from the place its
  own call lends), and the program of § 5 the briefs read least;
- end with `## What the coordinator must repair before any seat starts`, most important first.

## Second reading: the reports, before the synthesis (`critic-reports.md`)

You are a **fresh agent** in this reading, launched after the seats; this file is then your brief again. Read the
briefs as repaired (the repair moved the base to `df92f9281` and says in `00-shared.md` what that changed), your first
reading, then every report under `docs/panel/261-reports/` and the programs the seats
kept under `programs/<seat>/`. **Check the repair itself against your first reading's last section, item by item**: a
repair that is missing, that carried a number it says it ran, or that is wrong. Name what is missing: a route nobody
listed, a claim asserted and not measured with the command that settles it, a contradiction between seats and which
side is checkable, **a repair of the briefs that is itself wrong**. **Run the seats' programs on the platform they did
not**: the compiler-engineer's route under the x86-64 image, the ffi-pragmatist's bindings under the
compiler-engineer's built compiler; and **three shapes of your own** beside question 2's (a callee read from the
place its call lends), each under the base and under the built route. Then: does the route the seats would adopt
refuse every call that hides an `@`, every alias the exclusivity rule names, and no correct program of § 5's rewrites,
on every platform it claims? Do the emissions of programs that do not use the form keep their bytes? Does the
spec-warden's text say what the built route does, clause by clause, and do its registered predictions read the blind
registration's rules? Does the historian's every `verified` precedent quote its page? End with `## What the synthesis
must settle`, most important first.
