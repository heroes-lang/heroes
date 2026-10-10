# Report

## limit_ab

- Build: succeeds. Both groups are well formed. `half(x: i64) -> i64` matches `long half(long x)` (long is 64 bits on an ordinary LP64 machine). `cap(v: i32) -> i32` matches `int cap(int v)`. Neither header needs a library, so the groups have no `link`.
- Run: exit code 0. Output:

```
3
10
```

- Why: `half(6)` is `6 / 2 = 3`. `b.h` is checked as its own header. Nothing has defined `LIMIT` when it is read, so `#ifndef LIMIT` defines it as 10, and `cap(50)` returns `50 > 10 ? 10 : 50`, which is 10. The `#define LIMIT 100` in `a.h` belongs to the other group's header and does not reach `b.h`.
- Rests on:
  - Section 4: "Declaration order never changes what a program means." An `extern` group is a Declaration (`Declaration = ... | Extern`).
  - Section 13: "A group names its header" and "clang checks every result type, constant and record field against that header". Each group is tied to its own header and to nothing else.
  - Section 13: a parameter is declared "at the header's own width and sign", so `i32` for int, and `i64` for long works on LP64.
  - Section 2: "A literal takes the type its context asks for", so `50` is an `i32` and `6` is an `i64`.
  - Section 11: `print` writes a number and exactly one trailing newline.
  - C: `static inline` definitions in the header, integer division `6/2 = 3`, and the `#ifndef` guard.

## limit_ba

- Build: succeeds, for the same reasons as `limit_ab`.
- Run: exit code 0. Output:

```
3
10
```

- Why: this is the same program with the two groups swapped. Declaration order cannot change its meaning, so it prints what `limit_ab` prints. `b.h` alone gives `LIMIT` = 10, so `cap(50)` is 10, and `half(6)` is 3.
- Rests on: the same sentences as `limit_ab`, mainly section 4 ("Declaration order never changes what a program means") and section 13 ("against that header").

## jpeg_ab

- Build: fails. `heroes build` reports a C error from the `jpeglib.h` group, produces no binary, and exits non-zero. The spec does not give the number; I expect 1.
- Run: there is nothing to run. `./jpeg_ab` does not exist, so the shell reports "no such file or directory" (exit 127 in a typical shell).
- Why: each group's header is checked on its own. libjpeg's `jpeglib.h` (both IJG libjpeg and libjpeg-turbo) does not include `<stdio.h>`. Its documentation says the program must include `<stdio.h>` first, because the header uses `FILE` (for example `jpeg_stdio_src(j_decompress_ptr, FILE *infile)` and `jpeg_stdio_dest(j_compress_ptr, FILE *outfile)`) and `size_t`. With only `jpeglib.h` in front of it, clang stops with "unknown type name 'FILE'" (and errors about `size_t`). The `stdio.h` group cannot supply these, because it is a separate group with its own header.
- Rests on:
  - Section 13: "clang checks ... against that header".
  - Section 4: "Declaration order never changes what a program means." This rules out "it works because stdio.h came first".
  - Section 12: a program that does not compile produces no binary.
  - C facts: jpeglib.h's dependence on a prior `#include <stdio.h>`, and pkg-config's `libjpeg` package for `package "libjpeg"`.
  - The rest of the program is fine. `puts(s: cstr)` and `jpeg_std_error(err: ptr) -> ptr` are well formed, since a `ptr` may point at a struct. `print(1)` would print `1`.

## jpeg_ba

- Build: fails in the same way and for the same reason as `jpeg_ab`: `jpeglib.h` checked alone has no `FILE` or `size_t`. No binary is produced, and `heroes build` exits non-zero (I expect 1).
- Run: there is nothing to run.
- Rests on: the same sentences as `jpeg_ab`. Swapping the order of the groups changes nothing (section 4).

## choice_points

1. **Does each group's header get its own C context, or do all headers share one?** The spec only says each group "names its header" and is checked "against that header". It never says whether headers are combined into one translation unit, or in what order.
   - My choice: each group is isolated. This is the only reading that agrees with "Declaration order never changes what a program means".
   - The other reading: all headers go into one translation unit, in source order.
     - `limit_ab` would see `LIMIT` 100 before `b.h`, so `cap(50)` would be 50 and it would print `3` then `50`, exit 0.
     - `limit_ba` would print `3` then `10`. clang would only warn that `a.h` redefines `LIMIT`.
     - `jpeg_ab` would build (stdio.h comes first) and print `1`, exit 0.
     - `jpeg_ba` would still fail to build.
   - A shared unit in some fixed order other than source order is a third reading. It would make the pairs agree, but with values that depend on that hidden order.
2. **Calling `static inline` functions.** These have no linkable symbol, so the compiler must give C code a way to reach them (for example a generated wrapper compiled with the header). I assumed it does, because the spec says nothing against binding header-defined functions. If it only emitted external calls, both `limit_` programs would fail at link time with undefined symbols `_half` and `_cap`.
3. **The build's exit code on failure.** The spec does not state it. I assumed a conventional non-zero code, 1.

## confidence

- `limit_ab` and `limit_ba` printing `3` and `10`: moderate, about 65%. It depends on the isolation reading in choice point 1. The values 3 and 10 under that reading are certain.
- Both `jpeg_` programs failing to build: moderate, about 60%, for the same reason. Under the other reading, `jpeg_ab` builds and prints `1`.
- The C facts (jpeglib.h needs stdio.h first, and long is 64 bits): high.

## context

Nothing other than this directory's files reached my context: `brief.md`, `spec.md`, the four `.hero` files, `a.h` and `b.h`. I read no installed `stdio.h` or `jpeglib.h`. What I say about them comes from general knowledge of those headers. My system context also held an account email address and environment details (working directory, OS: macOS), which played no part in the answers.
