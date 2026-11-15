# Panel 250, ffi-pragmatist

Read `00-shared.md` first, whole: the base, the words, the routes (above all **M**, whose C closure is
your question, and **H**, which places every file that is not `.hero` whole so that no C closure is
computed), the rules (**the two compilers keep the names `heroes` and `heroes-next`; every other binary
you build is named `fp-...`**; a suite's output goes into a file, never through `head` or `tail`; every
program that may not end is bounded there) and the platforms (**the Windows box is off**; the Linux arm64 image is yours to run). You judge
what a module's C needs placed, with a veto on ABI breakage and on a build that compiles a header other
than the one its package published. Your folder is `.claude/worktrees/p250-scratch/250-ffi-pragmatist/`,
your tree `tree/` in it, a `git archive` of **`48f53b343`** (the base, rebased after the critic's
first pass; `core` 0.5.0 alone), your compiler built there from the seed in two stages (`sources
5bac791363384945`). Your report is `docs/panel/250-reports/ffi-pragmatist.md` in
`.claude/worktrees/p250`, in your definition's output structure (`.claude/agents/ffi-pragmatist.md`),
written as you go.

## What the base gives you

- **`core` 0.5.0 carries no C of its own**: 30 files, none `.c` or `.h` (`git ls-tree -r --name-only
  48f53b343 packages/core/0.5.0/files | grep -c -E '\.(c|h)$'`: 0), and its one group is
  `net/hero_net.hero:15`, `extern "hero_net.h"`, the platform layer's header beside the runtime (`grep -rn
  '^extern' packages/core/0.5.0/files` in your copy). So
  every C shape below is a package you write, in a scratch index of your own, placed by `heroes add` as
  panel 241's ffi-pragmatist placed its four routes (`241-*.md:102-105`, carried: *"each route a package
  in a scratch index, placed by `heroes add` and reached as `use <name>/crypto/sha256`"*). The index's
  layout and steps 3 and 4 of a publish, the listing and the fingerprint, are `examples/packages/README.md`
  § 10 (`versions`, `list`, `files/`; the two shell lines that make them at its § 10's end).
- **What a fetched unit may read**: `selfhost/cli/package_units.hero` (353 lines) and
  `package_reads.hero` (454), their opening comments: the runtime's directory first, then the folders
  its own `pkgconfig` groups name, then its own root, *"and nothing of the program's"*; everything read
  judged on clang's `-M` listing before any compile (panel 235's R9, C1 and C2, `235-*.md:661-691`). The
  build keeps those listings (`build/tu-*/main.deps.txt`, panel 235's C2 text at `235-*.md:668`); read
  where yours land.
- **The cases that already place C in a package** (`git ls-files <case>`): `tests/golden/run/fixedbugs-2351-a-fetched-header-is-found-from-its-own-root-and-runs-nothing-at-build.case`
  (`pk.h`, `pk.hero`), `fixedbugs-2351-two-fetched-packages-each-read-their-own-header-of-one-name.case`
  (package `a`: `av.hero`, `inner/deep.h`, `inner/deep.hero`, `same.h`, `heroes.toml`; package `b`:
  `bv.hero`, `same.h`), `fixedbugs-2357-two-fetched-packages-each-hold-a-third-ones-record.case` (`a`, `c`
  and `pt`, `pt.h`), `fixedbugs-2458-...`, `the-platform-layer-is-reached-from-a-fetched-packages-group.case`;
  and the `unsupported` form's eight cases with a placed tree, among them `fixedbugs-2357-a-fetched-packages-file-stands-in-for-a-c-library-header`
  and `fixedbugs-2351-a-fetched-header-climbing-out-of-its-root-is-refused`.
- **The rulings on a package's C**: `examples/packages/README.md` § 4, rule 5, *"No package ships
  platform C of its own"*; panel 241's R7 (`241-*.md:243-257`), *"An `os` package's own header with a
  platform arm is refused"*, a ruling; whether the compiler refuses such a header by itself is unread by
  these briefs. Rule 4 of § 4: a C name Heroes reserves goes *"through a `static inline` rename in a
  header of the package's own"*.

## How to measure what a pruned tree's build reads

The base refuses a pruned tree before its C is read (00-shared.md: `package_unread`, exit 1). Two ways
past that, both in your copy and both said in your report: **(1)** your compiler with the comparison in
`placed_said` (`selfhost/module/packages.hero:215-234`) answering `""` for a tree that does not sum,
a patch of your own copy and of nothing else, so a pruned fetched tree compiles under C1 and C2 as a
fetched unit does; **(2)** the `-M` listings of a build over the whole tree, read per unit. A `path` entry
is not a stand-in: its units take the program's words and search its folder *after* them
(`package_units.hero`'s comment, *"A `path` entry is the author's own code"*), so it measures another
search.

## The shapes, each built and run on Darwin arm64 and in Linux arm64

For each: the files the build reads (by `-M`), the files a placement of the reached modules must hold,
whether M's closure taken on one platform builds on the other, and what a pruned tree that lacks one of
them does (the exit, the message, and **whether another file stands in for it**).

1. **A header that includes another of the package's**: a module `a` binding `a.h`, which includes
   `"inner/deep.h"`; a module `b`, unreached, binding `b.h`, which includes the same file; and a header
   named by no group, included only by `a.h`.
2. **A header with platform arms**: `a.h` including `"posix.h"` under `#if !defined(_WIN32)` and
   `"win.h"` under `_WIN32` (and one keyed on `__APPLE__` against `__linux__`, so this Mac and the Linux
   image list different files). Does the base build it as a fetched package; does anything refuse it
   (the ruling above, a diagnostic, nothing); and what M's closure computed here omits that the other
   platform's build reads. A Windows arm compiled with `zig cc -target x86_64-windows-gnu` (`zig version` 0.15.2, at
   `/Users/joseph/.zvm/bin/zig`; `ZIG_GLOBAL_CACHE_DIR` and `ZIG_LOCAL_CACHE_DIR` inside your folder)
   stands for MinGW, never run, if you use it; the project's Windows target is MSVC's, owed to the CI's
   leg.
3. **A header pruned that a same-named file elsewhere can replace**: today's build refuses a package
   file that shadows a C library header (`fixedbugs-2357-a-fetched-packages-file-stands-in-for-a-c-library-header`,
   a package shipping its own `stdint.h`, `#~ shadowed_header`, read from its `main.hero`). The reverse is
   pruning's: a package header included by quote from a reached header, whose name another folder of
   the unit's search also holds (the runtime's directory, a `pkgconfig` group's folder, the SDK's or the
   C library's: find such a name, or say none can be found and where you looked). With the package's
   file removed, does clang find that other file, and does the build say so or compile it in silence?
4. **`#embed` and `__has_include`** of a file inside the package's root, read by a reached header: is the
   data file in `-M`'s listing, and what does `__has_include` answer when the file is pruned (a build
   that compiles another branch in silence is the finding to look for).
5. **A record of another package's group**: the 2357 shape, a program and two packages holding a third
   one's record; which of the third package's files a placement must hold when only the record is
   reached.
6. **The platform layer**: core's `net/hero_net` (`extern "hero_net.h"`), reached from `examples/server`
   and from the `the-platform-layer-is-reached-from-a-fetched-packages-group` case: confirm that no file
   of the package is read for it and that the archive beside the runtime is linked as at the base,
   under a pruned tree.
7. **`link` and `pkgconfig` in a package**: whether a fetched package's `link "x"` group can find an
   archive the package carries. By the source read, a question: `package_units.hero:298`'s message names
   headers alone; `grep -n -E '"-L"|"-L|-L ' selfhost/cli/*.hero` finds `-L` built in
   `selfhost/cli/link.hero:167` for each folder of its `search_library` and at `:819` in a test, and
   elsewhere only in `pkg-config` answers' filters, tests and `clang_jobs.hero:222`'s word list, none
   traced to a placed root (the coordinator's and the critic's reading, § G of
   `docs/panel/250-reports/critic-briefs.md`). Run it, and if it cannot, say so with the run; a `pkgconfig` group's folders are the
   machine's (panel 235's C2 ruling), so nothing of them is placed.

## What your verdict owes

For M and for H: which files a reached module's C needs placed, as a rule a verb can apply **without
running clang** or only by running it, and on which platform; whether M's closure is the same on Darwin
and Linux arm64 for every shape above (and what Windows would add, owed); every shape where a pruned tree
builds against a file its package did not publish, which is a veto's ground; what the fingerprint must
cover so that a header changed, removed or added under the placed tree is refused at build (F1 against
F4, 00-shared.md); and whether your recommendation needs a spec sentence (spec § 13, its line 377, says *"A group
names its header"*, and `grep -n 'heroes.packages' spec/heroes-spec.md` prints nothing) or a widened diagnostic (00-shared.md's escape, in those words).
