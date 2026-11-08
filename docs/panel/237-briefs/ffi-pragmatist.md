# Panel 237, the ffi-pragmatist's brief

Read `00-shared.md` and `proposal.md` beside this file first. Your worktree is
`.claude/worktrees/237-ffi-pragmatist/`, detached at `163912031`; build your compiler there as
`00-shared.md` says. You judge the founding constraint (design.md §1.11, §4.19: there is no standard
library, everything comes from C), with veto power on ABI breakage. Write and compile the C each
route implies, or show why it cannot be written.

## What the sitting needs from you, each run

1. **Question 1 in C, by hand**: a folder holding a `stdint.h` that reads the real one through
   `#include_next` and then redefines `__builtin_add_overflow`, compiled by clang under the orders
   each route implies: the program's folder by `-I` (today, before the refusal), by `-iquote`, and by
   `-idirafter` (`-isystem` is searched before the platform's here, `00-shared.md`); the same with a
   `math.h` and a `stdbool.h`; and **the same files reached through the environment**, `CPATH`,
   `C_INCLUDE_PATH`, and a `-D` through `CCC_OVERRIDE_OPTIONS`, which the critic measured reaching a
   wrong value at exit 0 with nothing refused. Which order lets a file of the program's
   folder stand in for `<stdint.h>`, and which never. Run it on this Mac's clang and, where it adds
   information, on Linux arm64 in the `heroes-linux-arm64:latest` container (`docker images` says
   whether it is here; the tree copied from `git ls-files`, never a `cp -r` that carries
   `.claude/worktrees`). Windows is unrun unless you say how it was reached.
2. **Question 2 over the packages here**: every package `pkg-config --list-all` lists on this Mac,
   its `--cflags`: count the `-I` folders that hold a file named like a header the runtime's opening
   reads (the list is what `clang -M` over the opening names; `selfhost/cli/shadowed.hero` says how it
   is made), and every `-D` and `-U` that names a name the guard keeps (`emit/guarded_names.hero`,
   `cli/package_macros.hero`). Name each package a refusal would stop, and say whether it is right to.
3. **The two refusals against a real binding**: a group binding a library whose package carries
   `-D` switches, built with your compiler: refused or not, and the C that shows why. The critic
   counted 23 `-D` names across the packages here, none a word the guard keeps and none
   `_REENTRANT` or `_FILE_OFFSET_BITS`; read the `.pc` files of the `heroes-linux-arm64:latest`
   container's packages for the switches a Linux build meets.
4. **Question 7's C**: SQLite on this Mac (`sqlite3 --version`, or the library `pkg-config sqlite3`
   finds): a C program that prepares a statement and then calls `sqlite3_close`, then
   `sqlite3_close_v2`, printing each result and whether the handle is usable after; and the Heroes
   programs fences B, C and D (`00-shared.md`) would make a reader write, built and run with your
   compiler: what each prints, and whether a refused close is told or silent. Defect 2291's cases
   (`git log --grep 2291`) measured `5` for a refused close on SQLite 3.54.0.

Your report: `docs/panel/237-reports/ffi-pragmatist.md` in your worktree, written as you go, ending as
`00-shared.md` § Rules says.
