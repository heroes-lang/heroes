# Panel 242, shared brief: whether the compiler may carry a package, what the package manager's transport becomes, and where the environment's write lives

Written 2026-11-10 by the coordinator, before the completeness critic's first pass. Every number
and path below was produced by a command run while writing, named beside it; a fact carried from
another document says so and is a claim for the seats to re-run.

## Why the sitting sits

Panel 241 (`docs/panel/241-the-compiler-takes-no-package-yet-crypto-sha256-is-heroes-alone-and-the-package-managers-rewrite-waits-for-its-packages-and-a-full-sitting.md`,
decision 2415, provisional) was the reduced sitting the author asked for on M-core-packages' step 6,
feature 2315 (`gh issue view 2315`: *"The package manager rewritten over the core packages, every
golden of its interface unchanged"*, open). It adopted nothing past the soundness lane and sent
three questions to a full sitting: its R4 (whether the compiler may carry a package at all, a
design.md Part 1 sentence), its R6 (the transport) and its R7 (the environment's write). Its R8
ordered *packages first, then this sitting*; the coordinator's reading of 2026-11-10, put to the
author with 2415, is the reverse: this sitting first, since the packages' admitting program is the
package manager, and whether the package manager may take a package is this sitting's question 1.
The author's panel gate for M-core-packages was asked once (2026-11-10, about 07:35, *first a
reduced sitting*), so this sitting is convened without asking again (CLAUDE.md § 4).

**Seats**: compiler-engineer, ffi-pragmatist, spec-warden, historian; the completeness critic on
the briefs before any seat and on the reports after. **The blind seat is not convened**: the
llm-ergonomist reads the specification alone, and no route below moves a token of it (`grep -n -i
'standard library\|compiled into\|no package' spec/heroes-spec.md` names one line, 34, a program's
catalogue); where a seat's route would move one, the seat says so and the blind seat, a paid run
the author decides, is owed before that route is adopted. **No paid run by anyone.**

## The tree

`main` at `1edabb9573a00db79fadda2bff695e98371a3290` (`git rev-parse HEAD`, tagged
`m-package-manager`). The two seats that compile have a detached worktree each at that commit,
`.claude/worktrees/242-compiler-engineer` and `.claude/worktrees/242-ffi-pragmatist`, and build
their own compiler there from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`, then `./heroes build selfhost/main.hero -o heroes-next`). No seat reads, builds or runs in
another seat's worktree or in the trunk. Scratch folders: `.claude/worktrees/p242-scratch/<seat>/`.

## The four questions

1. **May the compiler carry a package of the index?** design.md:570-571 reads *"No package is
   compiled into the compiler or the runtime (panel 097's condition 5: such source is the standard
   library this section forbids)"*; panel 097's condition 5 reads *"Nothing reaches
   `selfhost/library_source.hero` -- the prelude is the standard library design.md §1.11:443-445
   forbids permanently"* (`sed -n 128,130p docs/panel/097-*.md`). Routes, none adopted:
   - **(A)** the compiler takes packages as any program does, a catalogue entry in
     `selfhost/heroes.toml`, a placed tree, a fingerprint, and design.md:570-571 is amended to its
     source's reading (no package reaches the runtime or a program's prelude); under panel 241's
     R3 conditions (carried: it replaces the live copies of a contract and deletes them, the hook
     mirror follows `use <name>/`, the walks know the placed tree, each version the compiler takes
     is a seed refresh);
   - **(B)** the compiler takes none, design.md:570-571 stands by its letter, the compiler's hand
     helpers stay its own modules (`cli/strings`, used by 125 modules, `grep -rl '^use
     cli/strings$' selfhost | wc -l`), and feature 2315 and M-core-packages' acceptance condition
     are rewritten or withdrawn;
   - **(C)** the compiler carries a copy of a package's files as its own modules, held to the
     index by a check of the net rather than by a fingerprint at every build (panel 241 measured
     the plain copy at `selfhost/core/` refused, exit 1, `unknown_module` and `private_module`,
     and a `path` entry over it told *unverified* on every check: carried);
   - a route nobody listed, which a seat or the critic names.
2. **What the second version's transport becomes.** Today `fetch/` reaches the system through
   `cli/process`, used by 96 modules of the compiler, 11 of them among `fetch/`'s 21 (`grep -rl
   '^use cli/process$' selfhost | wc -l`; the same over `selfhost/fetch`; `ls selfhost/fetch/*.hero
   | wc -l`), and through four C groups (`git grep -n '^extern' -- selfhost/fetch
   selfhost/module/fingerprint.hero`: `index_source.hero:39,43`, `made_repo.hero:20`,
   `transport.hero:46`, `fingerprint.hero:23,27`). Panel 235's R10 runs `git` and `curl` as
   programs and says *"The second version, over the core packages, meets the seed's question
   again"*; its R19 ends *"The second version replaces the transport alone"* (carried, `sed -n
   693,700p` and `grep -n 'replaces the transport alone' docs/panel/235-*.md`). M-core-packages'
   page plans `os` as `selfhost/cli/process.hero:37-56` lifted out of the compiler (line 26) and a
   `net/http` client over libcurl (line 33) (`grep -n '^| `os`\|^| `net/http`'
   docs/roadmap/milestones/M-core-packages.md`). Routes: (a) the transport stays over
   `cli/process` and v2 replaces nothing there; (b) an `os` package over `hero_os.h` that the
   compiler's `cli/process` moves onto, all 96 modules; (c) a `net/http` package over libcurl, the
   seed's one clang line gaining libcurl; (d) `git` and `curl` stay programs and only the layer
   that runs them moves to a package.
3. **Where the environment's write lives.** Panel 241's R7 (carried) refused an `os` package's own
   header with a platform arm (the ffi-pragmatist's veto) and a package binding `hero_compiler.h`,
   and recorded `hero_env_put` moved into `hero_os.h` over `_wputenv_s` for this sitting, with
   decision 2228's promise of every `hero_os.h` function (design.md:3641-3645) and task 2263 (`gh
   issue view 2263`: open, M-publication-gate) and an empty value unrun on the Windows box.
4. **The order** of what follows, whatever 1 to 3 answer: which package the compiler would take
   first (decision 2404 names `path/filepath` the likeliest; defect 2406, open, lists `path`-shaped
   copies in `fetch/`), and whether `crypto/sha256` in Heroes (panel 241's R5) comes before or
   after.

## What every seat owes

- Its report written as it goes, to
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p242/docs/panel/242-reports/<seat>.md`.
- A verdict per question (approve a route, object, veto), its cost in lines and seconds where it
  builds something, a falsifiable prediction with the milestone where it becomes checkable, and the
  condition that would change its verdict.
- Every number from a command it ran, named beside it; a negative claim written as what it searched
  and where.
- No paid run of any kind; never print a credential (`gh auth status` if it must, never `gh auth
  token`); no file written outside `/Users/joseph/Temp/heroes/heroes-lang/`; no `git push`, no write
  to GitHub, no commit in the trunk, no edit of the trunk's hooks. A program that might not end is
  built and its binary run under `timeout`, its output bounded. The Windows box (`ssh win`) is
  reachable since about 11:40; a seat that uses it works in a folder of its own under `C:/w/`,
  named `p242-<seat>`, and touches no other.
