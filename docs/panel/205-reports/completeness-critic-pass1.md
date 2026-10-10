# Panel 205, completeness critic, first pass

Copied by the coordinator at 02:48 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 205, completeness critic, first pass

Started 02:38:37 and reported at about 02:48 (`date`). I worked in my own copy of the frozen tree, `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/critic-205/tree`. I removed its `.git` file at once, ran no git inside it, and built its compiler from the seed (`heroes 0.2.0`). The probes are under `critic-205/{c,hole,linux,ex,cases}`. The Linux runs used one `heroes-linux-arm64:latest` container at a time (Debian clang 22.1.8). This Mac runs Apple clang 21.

## Repairs to the briefs

1. **"nine `-Werror=` flags (`flags.hero:114-127`)" is wrong.** There are eight, at `:115-118` and `:121-124`. Line `:114` is `-Wall`, `:111` is `-std=gnu11`, and `:112-113` are two `-D`s. `-Werror=macro-redefined` is not in the frozen tree: a grep over `selfhost/` and `runtime/` finds nothing, so panel 204's R2 is ratified but not landed. The brief also leaves out that the emitted unit raises four more warnings to errors by pragma around its probes (`-Wdouble-promotion`, `-Wimplicit-float-conversion`, `-Wfloat-conversion`, `-Wimplicit-int-conversion`). They come from `selfhost/emit/extern_probe.hero:136` and appear at lines 32-40 of `leak.c`.

2. **Which prefix headers block the switch.** On Linux arm64, `_GNU_SOURCE` defined after `<stddef.h>` and `<stdbool.h>` still declares `sched_getcpu` (0 errors). Defined after `<stdint.h>` it does not (2 errors). On this Mac, `_POSIX_C_SOURCE` defined after `<stdint.h>` alone already leaves `strlcpy` visible (`sw4.c`). So the culprits are the libc headers: `<stdint.h>` (`heroes_runtime.h:56`) and `<math.h>` (`SEEDS`). `stddef.h` and `stdbool.h` are clang's own and do not block it. `hero_os.h` includes only `heroes_runtime.h` (grep), so it adds no libc header.

3. **Missing framing: the compiler already sets feature macros before every header.**
   - The flags pass `-D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS` (`flags.hero:112-113`).
   - `heroes_runtime.h:45-52` defines the same two again, guarded by `_WIN32`.
   - `runtime/runtime.c:92-94` defines `_GNU_SOURCE` on Linux for the runtime's own unit (panel 104).
   - So "a `-D` per platform" already has precedent inside the compiler. Panel 076's refusal was of `-D_GNU_SOURCE` reaching every program through `FLAGS`, not of a program choosing it. The brief should say so.
   - Under `-std=gnu11`, `strerror_r` is `int` in plain C on Linux; with `-D_GNU_SOURCE` the int assertion fails. That re-runs 076's measurement.

4. **Defect 568's "only a hand-written prototype builds" is false. A route exists today.**
   - Setup: a `.pc` file of the program's own with `Cflags: -D_GNU_SOURCE`, and `PKG_CONFIG_PATH` pointing at it.
   - On Linux arm64, `extern "sched.h" package "gnuswitch"` then builds at exit 0 and prints `true`.
   - A wrong `sched_getcpu(x: i32) -> i64` is refused `ffi_call_shape`, so the check survives.
   - On this Mac the same channel with `-D_POSIX_C_SOURCE=200112L` hides `strlcpy` (`ffi_unknown_name`), which is what C means by that switch.
   - The `-D` word panel 198 admits therefore already puts a macro ahead of the whole prefix. Any package that answers `-D_GNU_SOURCE` already does what 076 refused for `FLAGS`.

5. **Q1's class splits in two.** Some macros are latched by the first libc header: glibc's `features.h` reads `_GNU_SOURCE`, and Darwin's `sys/cdefs.h` reads `_POSIX_C_SOURCE`. Others are tested by the header itself when it is included.
   - Measured on this Mac: `<ucontext.h>` stops with `#error` unless `_XOPEN_SOURCE` is defined. `hole/uc.hero`, through a program header that says `#define _XOPEN_SOURCE 600` and then includes `<ucontext.h>`, builds at exit 0 **today**.
   - `WIN32_LEAN_AND_MEAN` and `NOMINMAX` are read by `windows.h` itself, and no runtime header includes `windows.h` (grep). So they probably work today too, but that is unrun on Windows. The ffi-pragmatist's list should carry them as a question.

6. **`_FILE_OFFSET_BITS=64` is not a case on the platforms measured.** On Linux arm64, `sizeof(off_t)` is 8 and `sizeof(time_t)` is 8, with and without `-D_FILE_OFFSET_BITS=64 -D_TIME_BITS=64`. Linux x86-64, Windows and the Mac are unrun. Drop it from the class unless a 32-bit target is in scope.

7. **GMP and libavutil answer `-I`, not `-isystem`.** pkg-config on this Mac gives `-I/opt/homebrew/Cellar/gmp/6.3.0/include` and `-I/opt/homebrew/Cellar/ffmpeg/9.0.2_1/include`.
   - Panel 198's "`-isystem` handed on as `-I`" is therefore not what refuses them, and reversing it changes neither verdict.
   - The Q2 route "`-isystem` for a package's directories" must mean turning a package's `-I` into `-isystem`, a wider route than the brief describes.
   - The policy that refuses them is the `-Werror` set applied to the whole unit, which `header_refused.hero:290` states.

8. **Defect 569 depends on the platform, and the brief does not say so.**
   - The same `g.hero` builds at exit 0 and prints `6` on Linux arm64, where `gmp.h` sits in a system directory (`/usr/include/aarch64-linux-gnu`). On this Mac it is refused.
   - Linux's `gmp.h` copied into a `-I` directory is refused at line 1882.
   - `-I/usr/include/aarch64-linux-gnu` gives 0 errors: clang keeps a system directory system even when `-I` names it.
   - `/opt/homebrew/include` is not in this Mac's search list (`clang -E -v`). So every Homebrew header is judged as the program's own code, and every apt header under `/usr/include` as system code.
   - The ffi-pragmatist's Linux census "with `-I` and with `-isystem`" will read the same for any header under `/usr/include`. The brief should say how to avoid that, for example by copying the headers into a user directory.

9. **The note at `header_refused.hero:290` and panel 188's premise are false for system directories.** The note says *no warning is turned off for a header*. `package_words.hero:40-42` records panel 188's premise: *`-isystem` is refused, so no bound header is in a system directory*. Both are false for every header found in a system directory: Linux's `gmp.h`, and every SDK header on the Mac.
   - An environment variable also decides the verdict on this Mac. With `C_INCLUDE_PATH=/opt/homebrew/include`, `extern "gmp.h" link "gmp"` builds at exit 0 and prints `6`.
   - With `CPATH`, the variable `ffi_missing_header`'s own note recommends, the same program is refused.
   - Re-running with `CPATH` after the cached success is still refused, so there is no stale verdict.

10. **Panel 204's R2 also depends on the platform.** I gave two headers that define `EXTERN` differently to `-Werror=macro-redefined`. With `-I` the build is refused; with `-isystem` it passes at exit 0. Same result on Mac and Linux.
    - So R2's "0 newly refused of 7,140 Linux pairs" may measure clang staying silent in `/usr/include` rather than an absence of redefinitions. That is a question for the coordinator.
    - A Q2 route that moves packages to `-isystem` would blind R2 to Expect with libjpeg.
    - `expect.h` and `jpeglib.h` are not installed in the container, so the Linux side is unrun.

11. **The workaround the brief carries has a soundness hole. It is a defect today, and I recommend filing it `blocking`.**
    - The brief says a program's own header with a diagnostic pragma around the include builds. That is true, but such a header can also turn checks off.
    - A program header that writes `#pragma clang diagnostic ignored "-Wsign-conversion"` with no push/pop turns the parameter sign check off for the rest of the unit.
    - `hole/leak.hero` binds `abs(x: u32)` against `int abs(int)`. It builds at exit 0, prints `5`, and leaves a raw clang `-Wabsolute-value` warning on stderr.
    - The same binding through a plain header is refused `ffi_parameter_type`.
    - The width check survives (`abs(x: i64)` is still refused), because the probe region raises `-Wimplicit-int-conversion` again after the headers.
    - So the sign check rests on the command line's `-Werror=sign-conversion`, which any header (a package's too) can switch off.
    - Any Q2 pragma route must raise again, after the groups' headers, every warning a check depends on.

12. **"Which warnings" has at least three rows today, and Q2 names one.**
    - The eight `-Werror` warnings refuse the program.
    - A plain `-Wall` warning inside a header prints clang's raw text at exit 0. `hole/wh.hero` (an unused variable in the program's header) builds and prints `5`.
    - A header attribute can fire on the compiler's own emitted line. `uc.hero` prints `-Wdeprecated-declarations` at the result check's `_Static_assert`, at exit 0.
    - The route "`-Werror` scoped to the compiler's own lines" would refuse that correct program unless `deprecated` is excluded.

13. **A group with a header and no member is refused.** `sw/empty.hero` fails with `expected_extern_block` at exit 1. The route "a header of the program's own placed before the prefix" therefore needs a member or a new form. The brief should state that a group naming a header only for its macros cannot be written today.

14. **The compiler-engineer's brief has two gaps.**
    - Panel 204's Expect case (`ex/ej.hero`, `je.hero`) is refused `ffi_missing_header` for `expect.h` on the frozen compiler with no include directory. `expect.h` is at `/opt/homebrew/include/expect.h`, so the brief should name the `--include` it needs.
    - "Every tracked `.hero` with an `extern`" should come with its count and command. My grep, of files on disk rather than `git ls-files`, finds 1,401 files with an `extern "` group head, 79 of them naming `package "`.

15. **The cases, re-run.**
    - **sw (this Mac):** exit 0, prints `1`. In plain C, `cfg.h` first hides `strlcpy`; after `math.h` (`sw2.c`) or after `stdint.h` it is visible.
    - **gmpmac g and av:** refused at `gmp.h:1882` and at `libavutil/common.h:212`. `g2` builds and prints `6`.
    - **gnu (container):** plain, own and first fail `ffi_unknown_name`; redecl builds and prints `true`; strv fails `ffi_unknown_name`.
    - **`-isystem` keeps the type checks:** with `-isystem`, a gmp unit still refuses a wrong pointer (`int64_t *` to `mpz_ptr`) and a wrong sign (`uint64_t` to `long`), since both errors are located in the emitted lines.

## Routes nobody listed

- **A. A package's `-D`, or a `.pc` of the program's own.** It works today (item 4). Asked before adoption:
  - is it scoped per unit or per program?
  - does the cache key carry the answer?
  - may a package `-D` break `heroes_runtime.h` (say `-Dbool=int`)?

  All three are unrun.
- **B. A prefix that reads no libc header before the groups.** `heroes_runtime.h` would spell its integer types with clang's `__INT64_TYPE__` and its siblings (C11 6.7p3 permits a repeated identical typedef), and `<stdint.h>` and `<math.h>` would come after the groups. In plain C on Linux arm64 (typedefs, `stddef`, `stdbool`, `_GNU_SOURCE`, `sched.h`, `stdint.h`, `math.h`) it compiles with the flags (`linux/pc/td.c`, rc 0). For it to hold: the same types on the Mac and Windows (unrun), and the guard's `FP_*` pushes and `HUGE_VAL`'s users moved.
- **C. Raise again, in the probe region after the groups, every warning a check depends on.** Starting with `sign-conversion` and `shorten-64-to-32`. Four warnings already work this way. It closes item 11 and is what makes any Q2 relaxation safe.
- **D. Judge a header by whose it is.** A header found through the program's own directory keeps every `-Werror`; a package's or a system directory's header is judged as clang judges system headers, which is what Linux already does for `/usr/include`. For it to hold: the compiler knows each header's directory (the `deps.txt` files list header paths).
- **E. The opposite direction, `-Wsystem-headers`.** One verdict on every platform. For it to hold: glibc and the SDK compile clean under the eight `-Werror` flags (unrun; it would likely refuse GMP on Linux too).
- **F. The environment, as it works today (item 9).** Recommend it, or refuse to depend on it.

## Questions the sitting does not ask

- Should a correct program's verdict depend on where its library is installed (Homebrew against apt)? Today it does, for 569 and for R2.
- Which `-Werror` flags guard undefined behaviour in code the program *runs*, such as a header's `static inline` with no return or an uninitialised read (`return-type`, `uninitialized`)? Which only judge the header author's style (`sign-conversion`, `shorten-64-to-32`)? Splitting them per flag would be a route.
- Is a feature macro per module or per program? Two modules binding `strerror_r` with and without `_GNU_SOURCE` would bind two C symbols (unrun).
- Is item 11 filed now, apart from the sitting? Is the raw `-Wdeprecated-declarations` output on a correct program (`uc.hero`) already a filed defect? I did not grep `issues/` for it.
- Is R2's Linux zero absence or suppression? Re-running its census with the headers in a user directory, or with `-Wsystem-headers`, would settle it.

## What I did not run

- Windows, for `WIN32_LEAN_AND_MEAN`, `NOMINMAX` and MSVC's `/external`.
- Linux x86-64.
- Any census.
- Route B on the Mac and Windows.
- The scope and cache key of a package's `-D`.
- R2 on Linux with Expect and libjpeg (neither is installed in the container).
- Any `heroes measure`. I only grepped panels 203 and 204 for the brief's headroom figures: 203's file has +38/+40 at `:32`, `:81` and `:155`, and 204's has +33/+34 at `:133`.
- The historian's precedents: Zig's `@cDefine` and its 0.16 change, and MSVC's `/external` dates "since 2019". These remain claims to verify, not premises.
- No web search, no paid run, nothing written outside my folder.
