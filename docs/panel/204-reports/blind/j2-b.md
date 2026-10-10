# Report

## limit_ab

`heroes build limit_ab.hero -o limit_ab` succeeds. `./limit_ab` exits with code 0 and prints:

```
3
50
```

Why:
- Spec section 13: "C reads a module's headers in the order its groups are written, so one that needs another's names comes after it." The headers go to C in the order `a.h`, then `b.h`.
- `a.h` defines `LIMIT` as 100. When C reads `b.h`, `#ifndef LIMIT` is false, so the `#define LIMIT 10` is skipped. `cap` is defined with `LIMIT` = 100, and nothing is redefined, so there is no warning.
- `half(x: 6)`: C `long` is 64 bits on an ordinary LP64 machine, so `x: i64` matches "a parameter ... declared at the header's own width and sign". The result is `6 / 2` = 3.
- `cap(v: 50)`: `v: i32` matches C `int` ("`i32` where C says int"). `50 > 100` is false, so the result is 50.
- Writing `x:` and `v:` as argument names is allowed (`Arg = [ ident ":" ] ...`). `print` takes a number and adds one trailing newline (section 11). Both functions are `static inline` in the headers, so no `link` is needed.

## limit_ba

`heroes build limit_ba.hero -o limit_ba` succeeds. clang warns that `LIMIT` is redefined, but in C that is only a warning. `./limit_ba` exits with code 0 and prints:

```
3
10
```

Why:
- The same sentence of section 13 applies, but now C reads `b.h` first. `LIMIT` is not defined yet, so `b.h` defines it as 10, and the body of `cap` is parsed right then with `v > 10 ? 10 : v`.
- C then reads `a.h`, whose `#define LIMIT 100` redefines the macro. clang reports `-Wmacro-redefined`, which is a warning by default. The change cannot reach `cap`, because a macro is expanded when the function body is parsed, not when the function is called.
- `cap(50)`: `50 > 10` is true, so the result is 10. `half(6)` is still 3.

## jpeg_ab

`heroes build jpeg_ab.hero -o jpeg_ab` succeeds, provided libjpeg and its pkg-config entry `libjpeg` are installed (true for libjpeg-turbo on an ordinary machine). `./jpeg_ab` exits with code 0 and prints:

```
1
```

Why:
- C reads `stdio.h` first, then `jpeglib.h`. libjpeg's `jpeglib.h` uses `FILE` (in `jpeg_stdio_src` / `jpeg_stdio_dest`) and `size_t`, and it does not include `<stdio.h>` itself. Its documentation says the program must include `<stdio.h>` before it. With this order that requirement is met.
- `puts(const char *) -> int` matches `s: cstr` with result `i32`. `jpeg_std_error(struct jpeg_error_mgr *) -> struct jpeg_error_mgr *` matches `ptr` for both the parameter and the result, under "except ... what a `ptr` points at". Section 13 says `package "libjpeg"` "asks the system where its headers and libraries are".
- The program never calls the extern functions. The rule against unused names covers bindings and parameters (section 5), not declarations. `main` only runs `print(1)`.

## jpeg_ba

`heroes build jpeg_ba.hero -o jpeg_ba` fails. clang reports errors when it reads `jpeglib.h`, such as `unknown type name 'FILE'` (and `size_t`, depending on what `jconfig.h`/`jmorecfg.h` pull in). The build exits with a nonzero code, and no binary `jpeg_ba` is written, so there is no `./jpeg_ba` to run and nothing is printed by it.

Why:
- Section 13: "C reads a module's headers in the order its groups are written, so one that needs another's names comes after it." `jpeglib.h` needs `FILE` from `stdio.h`, but its group comes first. Both groups are in one module, so C sees the headers in that order: `jpeglib.h` before `stdio.h`.
- Section 13: "clang checks every result type, constant and record field against that header". The header itself fails to compile, so this check cannot pass and the compiler refuses the program.

## choice_points

- **Macro redefinition in limit_ba.** I took clang's default: `'LIMIT' macro redefined` is a warning, so the build continues, and the value printed is 10, fixed when `cap` was parsed. If the Heroes toolchain turned warnings into errors (for example with `-Werror`), `limit_ba` would fail to build and print nothing. If the headers were instead compiled separately, one per group, `b.h` would also see `LIMIT` = 10, so the output would be the same. The spec's sentence about one module's headers being read in order points to a single shared C context, and that is how I read it.
- **Width of C `long`.** I assumed an ordinary LP64 machine (64-bit Linux or macOS), where `long` is 64 bits and `half(x: i64) -> i64` is accepted. On LLP64 (Windows), `long` is 32 bits, so the `i64` parameter would be refused and both `limit_` programs would fail to build.
- **Whether jpeglib.h includes stdio.h.** I relied on the standard libjpeg / libjpeg-turbo `jpeglib.h`, which expects `<stdio.h>` to be included before it. If a distribution's copy included `<stdio.h>` itself, `jpeg_ba` would build and print `1` with exit code 0, exactly like `jpeg_ab`.
- **pkg-config name.** I assumed the package `libjpeg` resolves, as `libjpeg.pc` from libjpeg-turbo does. If it did not, both `jpeg_` programs would fail to build.

## confidence

- limit_ab: high.
- limit_ba: fairly high. The value 10 follows from C's preprocessing rules, and the remaining risk is a toolchain that treats warnings as errors.
- jpeg_ab: high.
- jpeg_ba: moderately high. It depends on the installed `jpeglib.h` not including `<stdio.h>` itself, and on the spec's ordering sentence being meant literally.

## context

Nothing other than this directory's files reached my context. I read `brief.md`, `spec.md`, the four `.hero` programs, `a.h` and `b.h`. The facts about `stdio.h`, `jpeglib.h`, clang's warnings and C type widths come from my own general knowledge, not from any file. The only other thing in my context was the user's email address in the session metadata, and it had no bearing on the answers.
