# Panel 242, shared brief: whether the compiler may carry a package, what the package manager's transport becomes, and where the environment's write lives

Written 2026-11-10 by the coordinator, **rewritten the same day on the completeness critic's first
pass** (`../242-reports/critic-briefs.md`, 18 items), the first version kept beside this file as
`00-shared.before-critic.md`; what changed is listed at the end. Every number and path below was
produced by a command run while writing or by the critic's first pass, named beside it; a fact
carried from another document says so and is a claim for the seats to re-run.

## Why the sitting sits

Panel 241 (`docs/panel/241-the-compiler-takes-no-package-yet-crypto-sha256-is-heroes-alone-and-the-package-managers-rewrite-waits-for-its-packages-and-a-full-sitting.md`)
was the reduced sitting the author asked for on M-core-packages' step 6, feature 2315 (`gh issue
view 2315`: *"The package manager rewritten over the core packages, every golden of its interface
unchanged"*, open). **It is ratified**, R1 to R8, by the author's reading of 2026-11-10 at about
11:55, with R8's order reversed (`git show 5bb631868` on this sitting's branch `p242`; decision 2415
closes when that commit reaches `main`): the full sitting first, since whether the compiler may take
a package at all decides whether the package manager can be the program that admits the packages
R8 lists. It sent this sitting three questions: its R4 (the compiler carrying a package), its R6
(the transport) and its R7 (the environment's write).

**Seats, by the author's answer of 2026-11-10 at about 11:55**, put through the question form with
the coordinator's recommendation, *four seats, no blind seat*: the compiler-engineer, the
ffi-pragmatist, the spec-warden, the historian, and the completeness critic before and after. This
is neither of `/panel`'s two lanes; it is the author's choice, and it is the question R8 asked to be
put (*"the blind seat's paid run put to the author first"*). What it gives up: the llm-ergonomist's
reading of the specification alone. **No route listed moves a token of the specification** (the
critic's wider grep, item 2); the sentence every route of questions 2 and 3 sits under is spec § 13,
lines 374-375: *"Anything beyond this document ... comes from C libraries; threads, processes and
files from the language's `hero_os.h`"* (`sed -n 374,375p spec/heroes-spec.md`). A route that needs
that sentence changed (for instance *"and the environment"* added) owes the blind seat before it is
adopted. **No paid run by anyone.**

## The tree

`main` at `1edabb9573a00db79fadda2bff695e98371a3290` (`git rev-parse HEAD`, tagged
`m-package-manager`). The two seats that compile have a detached worktree each at that commit,
`.claude/worktrees/242-compiler-engineer` and `.claude/worktrees/242-ffi-pragmatist`, and build
their own compiler there from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`, then `./heroes build selfhost/main.hero -o heroes-next`). A compiler of the tree, built at
`-O2` for task 2419's probe, stands at `.claude/worktrees/probe-2419/heroes-probe` (`sources
45824e7205ca021e`, the tree's key): a seat that only reads may run it, never build with it in the
trunk. No seat reads, builds or runs in another seat's worktree, or builds in the trunk.

## The four questions

1. **May the compiler carry a package of the index?** design.md:570-572 reads *"No package is
   compiled into the compiler or the runtime (panel 097's condition 5: such source is the standard
   library this section forbids)"* (`sed -n 565,575p docs/design.md`). **Its source is panel 234's
   R1**, ratified 2026-11-06, which vetoed **route B5, the canonical tree compiled into the compiler**
   for programs to use, panel 028's P-embed applied to `core/` (`sed -n 200,201p
   docs/panel/234-briefs/00-shared.md`; the sentence entered at `79a42b509` and `b8f29508b`, the
   critic's `git log -S`); and **panel 234's R13**, ratified the same day, already lets the compiler
   take a copy: *"the compiler takes a copy under `selfhost/core/` only if the admission test admits
   it"* (`sed -n 488,499p docs/panel/234-*.md`). So the first question is **(i) whether the compiler
   depending on a package, as any program does, falls inside R1's veto of B5 at all**: if not,
   question 1 is a reading of the sentence back to its source, written in words; if so, route (A)
   reverses a ratified veto. Routes, none adopted:
   - **(A)** the compiler takes packages as any program does, a catalogue entry in
     `selfhost/heroes.toml` (which does not exist today), a placed tree, a fingerprint, under panel
     241's R3 conditions (carried: it replaces the live copies of a contract and deletes them, the
     hook mirror follows `use <name>/`, the walks know the placed tree, each version the compiler
     takes is a seed refresh); its wording either a reading of R1 or an amendment;
   - **(B)** the compiler takes none, the sentence stands by its letter, the compiler's hand helpers
     stay its own modules (`cli/strings` is used by 125 modules, `grep -rl '^use cli/strings$'
     selfhost | wc -l`; 114 at 87d77dec2, the b28 round's repairs having moved copies onto it), and
     feature 2315 and M-core-packages' acceptance condition are rewritten or withdrawn;
   - **(C)** R13's form: a copy of a package's files as the compiler's own modules, held to the index
     by a check of the net rather than by a fingerprint at every build (panel 241 measured the plain
     copy at `selfhost/core/` refused, exit 1, `unknown_module` and `private_module`, and a `path`
     entry over it told *unverified* on every check: carried);
   - **(D)** programs outside the compiler admit the packages first, so the sentence is not reached:
     the hooks rewritten in Heroes (`gh issue view 2082`, open, M-core-packages), the net
     (`tests/harness/`, no catalogue today), the HTTP server of the milestone page (line 183); the
     provenance mirror's reach over a program that is not the compiler is unread (the critic,
     item 11);
   - a route nobody listed.

   **And (ii) the one package (A) would delete code for today**: the compiler already carries
   SHA-256 twice, 292 lines in Heroes (`wc -l selfhost/cli/sha256.hero`; used by
   `module/fingerprint.hero`, `module/placed.hero` and `cli/check_json.hero`) and the C doors
   `hero_text_sha256` and `hero_file_sha256` (`module/fingerprint.hero:23-25`,
   `fetch/index_source.hero:39-41`), with a test asserting they agree (`module/fingerprint.hero:76`).
   Panel 241's R5 sent here *"whether its own fingerprint moves to (d)"*. **A soundness question,
   unrun, put as a question**: if the compiler's fingerprint check became a package's code, can the
   compiler verify the placed tree of the package that implements its verification?
2. **What the second version's transport becomes.** **Under every route `git` stays a program**:
   panel 235's R19 fixes the catalogue's `git`, `tag` and `commit` fields and a local git source
   among the interface's goldens, and its R10 refused libgit2 (carried, `sed -n 693,700p` and `sed
   -n 788,796p docs/panel/235-*.md`); so a route over libcurl moves `fetch/index_source.hero`'s HTTP
   read alone. Today `fetch/` reaches the system through `cli/process`, used by 96 modules, 11 of
   them among `fetch/`'s 21 (`grep -rl '^use cli/process$' selfhost | wc -l`, the same over
   `selfhost/fetch`, `ls selfhost/fetch/*.hero | wc -l`), and through six C groups in four files,
   four of `hero_compiler.h` and two of `hero_os.h` (`git grep -n '^extern' -- selfhost/fetch
   selfhost/module/fingerprint.hero`: `index_source.hero:39,43`, `made_repo.hero:20`,
   `transport.hero:46`, `fingerprint.hero:23,27`). Routes:
   - (a) the transport stays over `cli/process` and v2 replaces nothing there;
   - (b) an `os` package over `hero_os.h` that the compiler's `cli/process` moves onto, all 96
     modules. The milestone page's row cites `cli/process.hero:37-56`, stale: the `extern
     "hero_os.h"` group runs 40-64, and lines 66-69 bind `hero_dir_scan`, `hero_dir_at` and
     `hero_dir_release` under `extern "stdlib.h"`, which `hero_os.h:342-352` declares, open defect
     2413's shape (`grep -n '^extern' selfhost/cli/process.hero`);
   - (c) a `net/http` package over libcurl for the index read. **Its costs, counted by the critic
     (item 7)**: the seed's build line is spelled in 336 tracked files, 382 lines (`git grep -c
     'seed/heroes.c runtime/runtime.c' | wc -l`), the living ones `CLAUDE.md`, `README.md`,
     `seed/README.md`, `ci.yml` (three), `codeql.yml`, `deploy-site.yml`, ten site files with
     `site/src/lib/claims.ts`' `STAGE_ONE`, three hooks and `tests/harness/suite_latency.hero`; the
     Windows CI leg has no curl library (`ci.yml:517`); `heroes` links `libSystem` alone today
     (`otool -L ./heroes`), so a linked libcurl loads for every verb; ROADMAP row 83,
     M-standalone-compiler, is scheduled. And the binding: the tree's libcurl program binds
     `curl_bound.h`, a two-line `#undef` over `curl/curl.h` (`examples/curl/main.hero:40`), since
     libcurl 8.18 (the CI's x86-64 leg) makes `curl_easy_setopt` a macro holding a `_Pragma` (defect
     2229, closed); this Mac has 8.7.1 and the container 8.14.1 (`pkg-config --modversion libcurl`),
     so neither shows it, and a package carrying such a header ships C of its own (design.md:572);
   - (d) `git` and `curl` stay programs and only the layer that runs them moves to a package.
3. **Where the environment's write lives, and whether it should exist.** Every caller of
   `hero_env_put` writes a variable just before starting a child (`grep -rn 'hero_env_put('
   selfhost tests/harness --include='*.hero'`: `cli/symbolized.hero:57`, `fetch/made_repo.hero:68-69`,
   `fetch/transport.hero:160-161`, `tests/harness/suite_fetch.hero:86`,
   `tests/harness/case_setting.hero:214,223`), and the spawn takes no environment of its own
   (`runtime/parts/run.c:1083`, `execvp`; `run.c:1359`, `CreateProcessA` with no block). Routes:
   - panel 241's recorded route, `hero_env_put` moved into `hero_os.h` with its Windows arm over
     `_wputenv_s`, which adds a door to decision 2228's promise (design.md:3641-3645) and owes task
     2263 (`gh issue view 2263`: open, M-publication-gate);
   - **(e), the critic's (item 5)**: the child's environment given at the spawn, a
     `hero_run_env(name, value)` staged as `hero_run_arg` is (`envp` on POSIX, an environment block
     on Windows), so no process-wide write exists and neither Windows question arises; the harness
     does the POSIX half by hand today (`case_setting.hero:60-63`); unbuilt; and git's own `-c` for
     `GCM_INTERACTIVE`, beside `transfer.fsckObjects` at `transport.hero:150-151`, unread;
   - the write kept in `hero_compiler.h` as today.

   The empty value: `case_setting.hero`'s settable variables are `PATH` and
   `GIT_CEILING_DIRECTORIES` alone (lines 50-53), *"the variables whose empty value reads as
   unset"* (line 85), so that caller is not at risk; whether `_wputenv_s(name, L"")` removes the
   variable stays a question about `hero_env_put`'s promise.
4. **The order** of what follows: which package comes first and through which program (decision
   2404, closed, names `path/filepath` the likeliest; defect 2406, open, `improvement`, lists
   `path`-shaped copies in `fetch/` and outside it), and where `crypto/sha256` stands (question 1
   (ii)).

## What every seat owes

- Its report written as it goes, to
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p242/docs/panel/242-reports/<seat>.md`.
- A verdict per question (approve a route, object, veto), its cost in lines, and in instructions
  retired where it measures a cost (`/usr/bin/time -l`), a falsifiable prediction with the milestone
  where it becomes checkable, and the condition that would change its verdict. **Wall-clock seconds
  are planning figures only**: the two compiling seats build on this one Mac at once, so no clock is
  still (CL-025), and `heroes run tests/harness/latency.hero -- compare` is the only comparison the
  rule admits as a breach and is not asked here.
- Every number from a command it ran, named beside it; a negative claim written as what it searched
  and where.
- No paid run of any kind; never print a credential (`gh auth status` if it must, never `gh auth
  token`); no file written outside `/Users/joseph/Temp/heroes/heroes-lang/`; no `git push`, no write
  to GitHub, no commit in the trunk, no edit of the trunk's hooks (the hooks that run are the
  trunk's, so a patched mirror in a worktree changes nothing). Never build in the trunk: a compile
  there writes its `build/` cache. A program that might not end is built and its binary run under
  `timeout`, its output bounded. The Windows box (`ssh win`) is reachable; a seat that uses it works
  in a folder of its own under `C:/w/`, named `p242-<seat>`, and touches no other.

## What changed from the first version, on the critic's first pass

Panel 241 is ratified, not provisional (item 1); the seats are the author's answer, recorded, and the
spec sentence to watch is § 13's 374-375, not line 34 (items 1, 2); question 1 names its source,
panel 234's R1 veto of B5, and R13, and asks first whether the compiler's dependency is inside that
veto (item 3); question 1 (ii) carries the compiler's own SHA-256 and the verifier-among-the-verified
question (item 4); route (D) added (item 11); question 2 says `git` stays a program, counts route
(c)'s costs and names defect 2229's binding shape (items 7, 8, 9), and corrects the stale
`cli/process` citation (item 10); question 3 adds route (e) and reads the empty-value caller
rightly (items 5, 6); six groups in four files, lines 570-572, a compiler of the tree named, clocks
ordered (items 12 to 15).
