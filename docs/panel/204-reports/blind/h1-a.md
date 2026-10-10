# Report

Main reading used throughout: each `extern` group is checked and compiled against its own header alone, so one group's header never sees another's macros or types. The reasons are in `choice_points`.

## limit_ab

- Build: succeeds and writes the binary `limit_ab`.
- Exit code of `./limit_ab`: 0.
- Output:
  ```
  3
  10
  ```
- Why:
  - `half(x: 6)`: `a.h` defines `long half(long x) { return x / 2; }`. On an ordinary LP64 machine `long` is 64 bits and signed, so `x: i64` and `-> i64` match the header ("A parameter and a field are declared at the header's own width and sign"). 6 / 2 = 3, and `print` writes `3` plus one newline.
  - `cap(v: 50)`: `b.h` is read by itself. `LIMIT` is not defined there, so `#ifndef LIMIT` defines it as 10, and `cap` is `v > 10 ? 10 : v`. `int` is `i32`, which matches ("`i32` where C says int"). The literal `50` takes type `i32` from the parameter ("A literal takes the type its context asks for"). 50 > 10, so the result is 10, printed as `10`.
  - The `LIMIT 100` in `a.h` never reaches `b.h` because each group names its own header ("A group names its header", "clang checks every result type, constant and record field against that header"). Group order cannot matter either ("Declaration order never matters"; an `extern` group is a `Declaration`).
  - `print` "writes its values with no separator and exactly one trailing newline". `main` returns normally, so the exit code is 0. Neither group has a lease or a handle, so nothing aborts at the end of `main`.
  - No `link` is needed: both functions are `static inline` definitions in the headers ("`link` a library when the symbols need one").

## limit_ba

- Build: succeeds and writes the binary `limit_ba`.
- Exit code: 0.
- Output:
  ```
  3
  10
  ```
- Why: the declarations are the same as in `limit_ab`, only in the other order. Each header is read alone, so `b.h` again gets `LIMIT` 10: `cap(50)` is 10 and `half(6)` is 3. Under the other reading (one translation unit, headers in file order) this program also prints `3` and `10`. `b.h` comes first, so `cap` is compiled with `LIMIT` 10. When `a.h` then redefines `LIMIT` as 100, clang only warns (`-Wmacro-redefined`), and `cap`'s body has already been expanded.

## jpeg_ab

- Build: fails. `heroes build` reports clang errors from `jpeglib.h` and writes no binary, so there is no `./jpeg_ab` to run. The exit code is that of `heroes build`, which is nonzero. No program output exists.
- Why:
  - C fact: libjpeg's `jpeglib.h` (IJG libjpeg and libjpeg-turbo alike) is not self-contained. It uses `FILE` (for example in `jpeg_stdio_src(j_decompress_ptr, FILE *)` and `jpeg_stdio_dest`) and `size_t` without including `<stdio.h>`. libjpeg's own documentation tells users to include `<stdio.h>` before `jpeglib.h`. Parsed alone, it fails with errors such as `unknown type name 'FILE'`.
  - Under the main reading the `jpeglib.h` group is checked against that header alone, so the `stdio.h` group above it does not help. "clang checks every result type, constant and record field against that header", and a header that does not parse cannot be checked, so the build is refused.
  - Nothing else in the program is wrong. `puts(s: cstr) -> i32` matches `int puts(const char *)`. `jpeg_std_error(err: ptr) -> ptr` takes `struct jpeg_error_mgr *`, and "what a `ptr` points at" is exempt from the width rule. Functions that are declared but never called are not bindings, so the "unused" rule does not apply. `print(1)` is valid. `package "libjpeg"` is the pkg-config name that libjpeg and libjpeg-turbo install.
  - If the header problem were absent, the program would print `1` and exit 0.

## jpeg_ba

- Build: fails for the same reason as `jpeg_ab`. `jpeglib.h` is parsed without `FILE` and `size_t` defined, clang reports errors (`unknown type name 'FILE'`, and so on), and no binary `jpeg_ba` is written. `heroes build` exits nonzero and there is no program output.
- Why: as for `jpeg_ab`. Here the order would not save it under either reading, because `jpeglib.h` comes before `stdio.h` in the file.

## choice_points

1. **How the headers of several `extern` groups are combined.** The specification says only "A group names its header" and that clang checks declarations "against that header". It never says whether the groups share one C translation unit.
   - **My choice:** each group is checked against its own header alone. This follows "that header", and it is the only reading that keeps section 4's "Declaration order never matters" true for `extern` groups, which are Declarations.
   - **The other choice:** one translation unit with the headers in source order.
     - `limit_ab` would print `3` then `50` and exit 0. `a.h`'s `LIMIT 100` makes `b.h` skip its `#define`, so `cap(50)` is 50.
     - `limit_ba` would print `3` then `10` and exit 0, with a macro-redefinition warning at build time.
     - `jpeg_ab` would build (`stdio.h` supplies `FILE` and `size_t` first) and print `1`, exit 0.
     - `jpeg_ba` would still fail to build with the `FILE` errors.
   - Under that reading the `ab` and `ba` programs differ, which contradicts "Declaration order never matters". A third possibility is a compiler that detects the order dependence and refuses both `limit` programs. The specification gives no grounds for that, so I did not adopt it.
2. **`static inline` functions with no external symbol.** `half` and `cap` exist only as inline definitions in the headers. I assumed the compiler, which already runs clang on the header, compiles them (for example through a generated wrapper), since the specification asks for `link` only "when the symbols need one". The alternative is that a call to them finds no symbol and the `limit` programs fail to link, with no binary and a nonzero exit from `heroes build`.
3. **The width of `long`.** I assumed an ordinary LP64 machine, where `long` is 64 bits. On LLP64 (Windows) `long` is 32 bits, so `x: i64` would disagree with the header and `half` would be refused. That would fail both `limit` builds.
4. **The exact error text and exit code of a failed build.** The specification does not give them. I state only that the build fails with clang's diagnostics for `jpeglib.h`, writes no binary, and exits nonzero.

## confidence

- Moderate for the `limit` programs. The arithmetic is certain (3; 10 or 50), but which reading of header combination applies is a judgement. I rate `limit_ba` printing `3` and `10` as high confidence, because both readings agree.
- High that `jpeg_ba` fails to build, because both readings agree.
- Moderate for `jpeg_ab`: it fails under my reading but prints `1` under the single-translation-unit reading.

## context

Only this directory's files reached my context: `brief.md`, `spec.md`, the four `.hero` programs, `a.h` and `b.h`. I did not read the system's `stdio.h` or `jpeglib.h`. What I say about them (that `jpeglib.h` uses `FILE` and `size_t` without including `<stdio.h>`, and the pkg-config name `libjpeg`) comes from my general knowledge of C and libjpeg, not from any file. The harness also supplied the user's account email as session context. It plays no part in this report.
