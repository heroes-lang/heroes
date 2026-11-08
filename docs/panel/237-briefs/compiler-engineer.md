# Panel 237, the compiler-engineer's brief

Read `00-shared.md` and `proposal.md` beside this file first. Your worktree is
`.claude/worktrees/237-compiler-engineer/`, detached at `163912031`; build your compiler there as
`00-shared.md` says. You judge implementation cost and core against sugar (design.md §1.1, §1.7,
Part 5), with veto power; cite files and line counts.

## Where the landed work lives (`wc -l` at `163912031`)

- `selfhost/cli/shadowed.hero` (345): the listing of the runtime's own opening under the unit's
  words (`order_trial.opening`), the directory that found each file, the refusal and its note.
- `selfhost/cli/package_macros.hero` (101): which `-D` and `-U` words a package's answer may carry.
- `selfhost/emit/guarded_names.hero` (273) and `selfhost/emit/c_words.hero` (274): the names kept.
- `runtime/heroes_reserved_open.h` and `runtime/heroes_reserved_close.h` (79 each): the reserved
  names saved and given back around a group's headers.
- `selfhost/cli/compiling.hero`'s `search_words`: the one search list every unit and probe compiles
  under (defect 2324 put the runtime's directory first).
- The cases: `tests/golden/run/fixedbugs-2333-*` (4), `tests/golden/unsupported/fixedbugs-2333-*`
  (3), `tests/harness/suite_special.hero`'s `a_library_header_in_a_named_folder_is_refused`
  (line 647). The lane that landed it reported a warm build of a two-line program at 0.17 s before
  and 0.20 s after, best and median of 11 alternated pairs (its report, relayed on issue 2333; the
  commit body does not carry it): re-measure it in instructions before you rest on it.

## What the sitting needs from you, each measured

1. **The cost of the refusal**: instructions retired by `heroes build` of a two-line program and of a
   program with one `extern` group, with the listing and without it (a build of your own compiler
   with the call taken out is the honest comparison), warm cache, the first run discarded.
2. **Question 1's other routes, built**: prototype in your worktree the route that changes where the
   library's headers are found instead of refusing (`-idirafter` for the named folders; `-iquote` for
   the program's own folder, which needs a group's header emitted as a quoted include, since it is
   emitted `#include <money.h>` today; `-isystem` is no route, `00-shared.md`), and measure on it what
   the refusal measures: the `run` and `unsupported` cases of 2333 and `special`'s check. Say which
   programs each route refuses or changes that the other does not, beyond the opening's 36 headers
   (the readline and poppler names, `00-shared.md`). A route that does not build is not adopted (the
   `/panel` skill, 3c). **And the environment**: what `CPATH`, `C_INCLUDE_PATH`, `CPLUS_INCLUDE_PATH`,
   `OBJC_INCLUDE_PATH` and `CCC_OVERRIDE_OPTIONS` do to every clang the compiler runs, and what it
   costs to clear them, count them in the refusal, or refuse a build where one is set.
3. **Question 2, counted**: which tracked programs the refusal refuses today. Run `heroes build`
   over every `examples/*/main.hero` and the `run` goldens, or say why a subset suffices, and name
   any refused.
4. **Question 5**: what it would take for `heroes check` to answer the refusal (it exits 0 today on
   the program `build`, `run`, `--emit-c` and `test` refuse, `00-shared.md`), its cost, and whether
   `check` asks any other C question today (`grep` the `check` path for a clang call) or the line is
   principled. Read feature 974 (`gh issue view 974 --repo heroes-lang/heroes`) and panel 082's R3
   (`docs/panel/082-*.md`) first: they are this question asked before, and refused once on cost.
5. **Question 6**: whether the `unsupported` form (`tests/harness/suite_golden.hero`'s
   `shape_unsupported`, `verb: "build"`, `want_code: 1`) reads a `.case` folder today
   (`tests/harness/cases.hero` knows the suffix at line 91), so a folder holding a `stdint.h` can be
   a golden case annotated in its source; build one in your worktree and run the form narrowed to it.
6. **The shapes beside it, depth one**: a file standing in found through a subfolder
   (`sys/cdefs.h`), a symlink, a folder named twice, a header reached only under a switch a group's
   header sets (the module doc of `shadowed.hero` says this one is not seen: measure what happens).
   A real defect of another cause goes in your report with its reproducer.

Your report: `docs/panel/237-reports/compiler-engineer.md` in your worktree, written as you go, ending
as `00-shared.md` § Rules says.
