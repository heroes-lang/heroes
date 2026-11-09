# Panel 239, the ffi-pragmatist's brief

Read `00-shared.md` and `proposal.md` beside this file first. Your worktree is
`.claude/worktrees/239-ffi-pragmatist/`, detached at the sitting's briefs commit on `p239`; build
your compiler there as `00-shared.md` says. You judge the founding constraint (design.md §1.11,
§4.19: there is no standard library, everything comes from C), with veto power on ABI breakage and on
what makes bindings categorically harder. Write and compile the C each route implies, or show why it
cannot be written. **Your build is 2363's refusal**; the compiler-engineer builds 2364's route.

## What the sitting needs from you, each run on this Mac and on Linux arm64

1. **2363's routes in C first** (question 1's A to D), with `p239-scratch/repro/link/`'s libraries
   and `repro/s2363/`'s: what each linker can say about which library supplied a name, in the same
   link and outside it (GNU ld `--trace-symbol`, `-Map`, `-t`; ld64 `-map`, `-t`, `-why_load`, `nm
   -m` of the output; `nm` and `ar t` of the libraries), for archives and shared libraries, in both
   orders and inside one `-Wl,--start-group ... --end-group` group (R3's words, panel 238's
   `group.diff`); and **lld**, which is in the Linux image (`ld.lld`, Debian LLD 22.1.8, the family
   Windows' `lld-link` belongs to) and names a shared definition it did not take. GNU ld's trace names
   one `definition of` line, the one taken, and a shared definer after a static one only as `reference
   to` (`00-shared.md`); no linker names an archive member it did not load: say what reads the
   others, and at what cost. Run the critic's whole-archive probe (route D) and the padded member
   (`repro/critic/s2363x/`, `repro/critic/c/`), where ld64 binds by the names as well as the line.
2. **The false refusals of each route, on real libraries**: glibc's pairs (`ldexp` and five more in
   both `libc.so.6` and `libm.so.6`; `pthread_create` in `libc.so.6` behind an empty `libpthread.a`;
   `libm.so` a linker script), this Mac's re-exports (`libm.tbd` is `libSystem.tbd`), versioned and
   weak definitions, a library two modules both name, two folders each holding a library of one name,
   SQLite static and shared, zlib, curl or OpenSSL through `package`, any library on either machine
   that bundles a copy of another's names, and **a library linked on purpose to replace a C library
   name** (an allocator's `malloc` and `free`; none is installed on either machine, so write one),
   which is panel 238's open question for this sitting: what each route does with it, and how a
   route tells it from `interpose`. Count, per route, the tracked programs and the real
   bindings it would refuse; a route that refuses `link "m"` binding `sqrt` or `link "pthread"`
   binding `pthread_create` is refusing correct programs (the tree holds 20 `link "m"` and 4 `link "pthread"`
   lines, `00-shared.md`).
3. **Build the refusal you judge most robust**, as a compiler change in a copy of `selfhost/` under
   `build/239/`, with its test, on both platforms: `dup_*` and `sig_*` static and shared, `interpose`,
   `remath`, every `link`-bearing program under `examples/` and `tests/golden/run/` on this Mac, and
   the same programs with R3's group words applied (`group.diff`). Its verdict must not depend on the
   order of the `use` lines; show it does not, both orders. **A route that does not build is not
   adopted.** Its cost in instructions retired on a warm build of `sig_written` and of a program with
   no group, with and without it.
4. **What a program that needs both libraries does** (question 2): the C an author would write to
   keep `liba`'s `pick` and `libb`'s other names in one program (a wrapper library, `objcopy
   --localize-symbol`, a version script, a shared library with `-Bsymbolic`, two-level namespace),
   and which of them a Heroes author can reach without a tool the language does not name. The fix your
   message offers must lead somewhere.
5. **2364 in C**: the `pack` shape as two C translation units laid out two ways and linked, to show
   what C itself says (C11 6.2.7's compatible types across units); and whether panel 204's order
   search, asked of other modules' headers, chooses an order that changes what a bound function means
   on a real library pair (GLFW with a GL header, libjpeg after `stdio.h`, readline), where each is
   installed.
6. **The shapes beside 2363, depth one, run**: a name no group binds that a linked library needs and
   two define; **a name the runtime calls that a linked library defines** (the critic's
   `runtime_name`, `printf`, silent on both platforms) and a name the runtime's object defines
   (`runtime/`, prefixed `hero_`); the duplicate in a member loaded for another name (`one_*`) and a
   weak definition there; a library `pkg-config` names twice; a `.so` whose own dependency defines the
   name; `-l` resolving to a shared library on one platform and an archive on the other. A real defect of
   another cause goes in your report with its reproducer, for the coordinator to file.

Windows (`lld-link`) is unrun unless you say how it was reached (`.claude/rules/platforms.md:98-101`:
the box is paid by the hour, usually off, and only the author can start it); say what its
`/VERBOSE` or `/MAP` would need, as a question, and what lld on Linux tells you about it.

Your report: `docs/panel/239-reports/ffi-pragmatist.md` in your worktree, written as you go, ending
as `00-shared.md` § Rules says.
