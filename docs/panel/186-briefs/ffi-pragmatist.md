# Panel 186, the ffi-pragmatist's brief

Read `00-shared.md` in this directory first, whole: the defects, what the
compiler does today, the precedent, the four questions and what every route
must keep. It was repaired on the critic's first pass
(`docs/panel/186-reports/completeness-critic-briefs.md`); read that too,
above all its §§ 2, 4, 6, 7 and 9, whose C you may build on.

## Your input

The proposal's routes and the C each implies. Your own copy is
`<scratchpad>/186-ffi-pragmatist/` (`git archive 779139d0 | tar -x -C
<copy>`, then `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`
inside it), and the probes of `docs/panel/186-briefs/probes/` (copy them in).
Two clangs are on this Mac for you: `/usr/bin/clang` (Apple clang 21.0.0)
and `/opt/homebrew/opt/llvm@22/bin/clang` (Homebrew clang 22.1.8).

## Your task

1. **Write and compile the C each route implies**, by hand, outside the
   compiler, on both clangs of this Mac: (1a)'s range assertions; (1b)'s
   designated probe under `-Werror=initializer-overrides`; (1e)'s layout
   from clang, both the JSON AST dump (`-Xclang -ast-dump=json`) and `-Xclang
   -fdump-record-layouts`, and what each shows for an anonymous union, an
   anonymous struct, a bit-field and a flexible array member; (1f)'s classify
   OR sum form; and for Q2 any form that reports a field a record leaves out
   and names it (the critic measured that no warning reports a designated
   omission on 21 and 22: confirm, and look further). Shapes: `u.h`'s `SA`
   complete, missing `x`, missing `kind`, naming `i` alone, naming `f` alone;
   `SB` by one arm; `S3` missing its middle; `W` naming both, one, none;
   `critic/bf.h`'s `BF`, `S2U` and `DEEP`; `PADU`; a padded union; a GNU
   zero-sized member; a flexible array member; an array member.
2. **Real headers**: find structs holding an anonymous union, and bit-fields,
   in the headers on this Mac (the SDK, `/opt/homebrew/include/SDL3`,
   `raylib.h`; say what you searched and with which command). For each, what
   a binding of it does today on `779139d0`'s compiler, and under each route.
   Write panel 073's three readings that must stay legal against SDL3's real
   `SDL_Event` (reading a union's members, one record per arm by `tag`, a
   one-member binding) and run them today.
3. **The platforms**: list, for the coordinator, the exact C files and
   commands whose verdicts must be read on clang 18.1.3 (the CI's Linux legs
   and the floor), 20.1.8 (the CI's Windows leg) and 23.1.1 (the Windows
   box); ask in your report before your verdict, and the coordinator runs
   them and hands you the output, or records them as owed.
4. Every route of Q1 to Q4 judged: approve, object or veto, with what you
   compiled and ran for each. Your veto is on ABI breakage and on a program
   that runs today refused; name the programs.

Write your report into `docs/panel/186-reports/ffi-pragmatist.md` in the
trunk as you go.
