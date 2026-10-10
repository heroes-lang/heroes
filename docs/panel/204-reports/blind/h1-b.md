# Report

Common ground for all four programs: each is one file with `function main()` (section 1), two `extern` groups and a body of `print` calls. `print` writes a number with exactly one trailing newline (section 11). Nothing is leased or acquired, so nothing aborts when `main` returns, and a `main` that returns normally gives exit code 0. An unused extern function is not a binding or a parameter, so the unused-binding rule (section 5) does not apply to it.

The key question is how the headers of two groups interact. Section 4 says "Declaration order never matters", and an `Extern` is one of the `Declaration` alternatives. Section 13 says "clang checks every result type, constant and record field against that header", meaning the group's own header. I read these together: each group is checked and compiled against its own header alone, so one group's header never sees another's macros, and swapping the groups changes nothing. The other reading is under `choice_points`.

## limit_ab

- Build: succeeds.
- Exit code: 0.
- Output:
  ```
  3
  10
  ```
- Why:
  - `half(x: i64) -> i64` against `static inline long half(long x)`. On an ordinary LP64 machine (Linux, macOS), `long` is 64-bit signed, so the parameter matches "at the header's own width and sign" (section 13). `half(6)` is `6 / 2 = 3`.
  - `cap(v: i32) -> i32` against `int cap(int v)`. This matches "`i32` where C says int".
  - Because `b.h` is processed on its own, `LIMIT` is not defined when `#ifndef LIMIT` runs, so `#define LIMIT 10` takes effect. `cap(50)` evaluates `50 > 10 ? 10 : 50`, which is `10`.
  - The `#define LIMIT 100` in `a.h` is never seen by `b.h`.
  - Calls with named arguments are allowed by `Arg = [ ident ":" ] ...` (section 9).

## limit_ba

- Build: succeeds.
- Exit code: 0.
- Output:
  ```
  3
  10
  ```
- Why: this is the same program with the two groups swapped. "Declaration order never matters" (section 4), so the result is the same as `limit_ab`.
  - `b.h` alone defines `LIMIT` as 10, so `cap(50)` is 10.
  - `a.h` alone gives `half(6) = 3`.

## jpeg_ab

- Build: fails, so there is no binary, no exit code from `./jpeg_ab` and no output.
- Why:
  - `jpeglib.h` (libjpeg and libjpeg-turbo) is not self-contained. Its documentation (libjpeg.txt) requires the includer to define `FILE` and `size_t` first, normally by including `<stdio.h>` before it.
  - The header uses `FILE *` in `jpeg_stdio_src` and `jpeg_stdio_dest`, and `size_t` in the memory-manager struct, for example `alloc_small`'s `size_t sizeofobject`.
  - Its own includes, `jconfig.h` and `jmorecfg.h`, do not pull in `<stdio.h>`.
  - When clang checks the `jpeglib.h` group against "that header" on its own, it reports errors such as `unknown type name 'FILE'` and `unknown type name 'size_t'`, and `heroes build` reports them.
  - The `stdio.h` group earlier in the file does not help, because under section 4 its position cannot matter.
  - The declarations themselves would be fine:
    - `jpeg_std_error(err: ptr) -> ptr` against `struct jpeg_error_mgr *jpeg_std_error(struct jpeg_error_mgr *)` uses `ptr` for "what a `ptr` points at".
    - `puts(s: cstr) -> i32` matches `int puts(const char *)`.
    - `package "libjpeg"` names libjpeg's pkg-config module.
  - If the build did succeed, `./jpeg_ab` would print `1` and exit 0.

## jpeg_ba

- Build: fails for the same reason as `jpeg_ab`. `jpeglib.h`, checked alone, lacks `FILE` and `size_t`. There is no binary, exit code or output.
- Under the other reading (below), this is the program that fails in any case, because `jpeglib.h` comes before `stdio.h`.

## choice_points

1. **How the headers of separate groups combine. This is the main choice.**
   - The spec never says whether the groups' headers share one C translation unit.
   - I chose per-group isolation, because section 4 says "Declaration order never matters" (an extern group is a Declaration) and section 13 says "checks ... against that header".
   - The other choice is one translation unit with the headers `#include`d in source order, which is how a straightforward C-emitting compiler behaves. It would produce:
     - **limit_ab**: `a.h` defines `LIMIT` as 100 first, so `b.h`'s `#ifndef` skips its own define. The output is `3` then `50` (`50 > 100` is false), exit 0.
     - **limit_ba**: `LIMIT` is 10 when `cap` is defined, so the output is `3` then `10`, exit 0. The later `#define LIMIT 100` in `a.h` redefines a macro with a different body, which clang reports as a warning ("'LIMIT' macro redefined"). It would be an error only under `-Werror`, which I assumed is not used.
     - **jpeg_ab**: `stdio.h` comes first, so `jpeglib.h` compiles. It prints `1`, exit 0.
     - **jpeg_ba**: the build fails with `unknown type name 'FILE'` and `size_t` errors.

   That outcome would break the section 4 guarantee, so I rejected it as a reading of the spec. It is still the most likely behaviour of a naive implementation.
2. **Calling a `static inline` function.** The C functions have no exported symbol. I assumed the generated C includes the header and calls the functions directly, so they work. If the tool linked by symbol only, both `limit_` programs would fail at link time with undefined `half` and `cap`.
3. **Width of `long`.** I assumed an LP64 machine, where `long` is 64-bit, so `i64` matches. On LLP64 Windows, `long` is 32-bit and section 13 would refuse `x: i64`, so both `limit_` builds would fail.

## confidence

- I am confident about the values: `half(6) = 3`, and `cap(50)` is 10 or 50 depending on whether `LIMIT` was already 100.
- I am confident that `jpeglib.h` needs `FILE` and `size_t` defined before it.
- I am only moderately confident about the overall outcomes, about 55 to 60 percent, because they depend on choice point 1. The spec does not state the header-combination model; I inferred it from "Declaration order never matters".

## context

Nothing from outside this directory reached my context. I read only `brief.md`, `spec.md`, the four `.hero` files, `a.h` and `b.h`. What I say about `jpeglib.h`, `stdio.h`, clang's macro-redefinition warning and LP64 type widths comes from general knowledge, not from reading those files on this machine.
