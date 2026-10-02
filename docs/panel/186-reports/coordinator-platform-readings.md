# Panel 186: the ffi-pragmatist's § 6 requests, run by the coordinator

Run 2026-10-02 from 12:01 to 12:03 (the output files' mtimes) on the three files of
`<scratchpad>/186-ffi-pragmatist/work/plat/` (`plat.c`, `plat_posix.c`,
`plat_lay.c`, quoted whole in `ffi-pragmatist.md` § 6), with the seat's own
commands. Outputs kept in `<scratchpad>/p186/plat-out/`.

| clang | where | `plat.c`: what it flags |
|---|---|---|
| Debian clang 18.1.8, aarch64 | `docker run silkeh/clang:18` (not the CI's Ubuntu 18.1.3; the same major, the floor's) | P01 P03 P05 P06 P07 P11 P12 P13 P14 P18, the overrides errors at :54 (P21) and :56 (P23), *missing field 'c'* at :68 (P28), *excess elements* warnings at :69 (P29) and :70 (P30), and at :62 *unknown warning group '-Wmissing-designated-field-initializers', ignored* |
| Debian clang 20.1.8, aarch64 | `docker run silkeh/clang:20` (pulled at 12:03), Linux target | the same set, without the :62 line |
| the same clang 20.1.8 | `--target=x86_64-pc-windows-msvc -ffreestanding` and `--target=x86_64-w64-windows-gnu -ffreestanding` | the same set, both targets |
| clang 23.1.1 | the Windows box, `ssh win` | the same set, without the :62 line |

So on 18, 20 and 23, as on Apple 21 and Homebrew 22 by the seat's reading:
**P25, P26 and P27 are silent** (no clang this project meets reports a field
a designated initializer leaves out in C; on 18 the newer flag does not even
exist); **P08 and P16 pass** (the GNU zero-sized members, on a Linux target
and on both Windows targets); P21 and P23 are errors under the armed
`-Winitializer-overrides`.

`plat_posix.c` on 18: X01 fails and X04 is the overrides error, the same as
the seat's two clangs (not run on 20 or on the Windows box, which has no
`sigaction`).

`plat_lay.c`, `-Xclang -fdump-record-layouts` filtered as the seat's command
filters it: **two texts**. 18.1.8 and 20.1.8 print `union (anonymous at
plat_lay.c:5:41)`, as Apple 21 did; 23.1.1 prints `union (unnamed
struct)::(unnamed struct)::(anonymous at plat_lay.c:5:41)`, as Homebrew 22
did. The JSON dump holds **5 `MemberExpr`** on 18, 20 and 23, as on 21 and 22,
with the same `name` lines.

Not run: the CI's own Ubuntu 18.1.3 and its Windows leg's own 20.1.8 build;
these are the same majors from other builders.

**Corrected 2026-10-02 at 12:05**, on the ffi-pragmatist's reading of
the raw outputs: `plat_posix.c` WAS run on Debian clang 20.1.8 (the same
`run.sh` as on 18), and `clang20.txt` holds its section; the sentence above
that says it was not run on 20 is false. On 20, as on 18, the range check
sees through glibc's `sa_handler` macro (X01). The Windows box, which has no
`sigaction`, did not run it.

## The compiler-engineer's § 6 and § 9 requests, run 2026-10-02 from 13:10 to 13:17

The files of `<scratchpad>/186-compiler-engineer/work/request/` (`lay.c`,
`screen.c`, `check.c`, `control.c` and their headers), the seat's commands,
outputs in `<scratchpad>/p186/ce-out/` (`c18.txt`, `c20.txt`, `u18.txt`, and
`lay.<v>.out`, `control.<v>.out`).

| clang | where | lay | screen | check | control |
|---|---|---|---|---|---|
| Debian 18.1.8 | `silkeh/clang:18` | exit 0, empty stderr, equal to `lay.usr.out` with every parenthesised span masked | exit 1, exactly `hero_ly_screen:` 1 2 3 5 6 7 9 11 12 | exit 1, exactly EXPECT-FAIL-1, -6, -7, -9, -11 | exit 0, the seven lines the seat expects |
| Debian 20.1.8 | `silkeh/clang:20` | the same | the same | the same | the same |
| **Ubuntu 18.1.3 (1ubuntu1)**, the CI's own | `ubuntu:24.04` with `apt-get install clang` (pulled 13:15) | the same | the same | the same | the same, and its masked text equal to 18.1.8's |
| 23.1.1 | the Windows box | **unrun**: `ssh win` timed out at 13:16 (the box's last answer, the Windows leg on `ae08ed93`, exit 0 at 11:46) | | | |

The ffi-pragmatist's `plat.c` on Ubuntu 18.1.3 in the same container flags
exactly what it flagged on 18.1.8 (P01 P03 P05 P06 P07 P11 P12 P13 P14 P18,
:54, :56, :62's unknown warning group, :68, :69, :70): P25 to P27 silent on
the CI's own clang.
