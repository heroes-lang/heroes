# limit_ab

**Build:** succeeds. **Exit code of `./limit_ab`:** 0. **Output:**

```
3
10
```

Why:
- `half(x: 6)`: `a.h` declares `long half(long x)` and returns `x / 2`. On an ordinary 64-bit Unix machine (LP64, so macOS or Linux) `long` is 64 bits, so `x: i64` sits "at the header's own width and sign" (section 13) and the result `i64` matches. C's `6 / 2` is `3`, and `print` writes `3` followed by one newline (section 11).
- `cap(v: 50)`: `b.h` declares `int cap(int v)`, and `v: i32` is "`i32` where C says int". `b.h` defines `LIMIT 10` only `#ifndef LIMIT`. Section 13 says "A group names its header" and "clang checks every result type, constant and record field against that header", and section 4 says "Declaration order never changes what a program means". An `extern` group is a Declaration (the `Declaration` production lists `Extern`). So I read each group as checked and compiled against its own header alone, and `a.h`'s `#define LIMIT 100` never reaches `b.h`. `LIMIT` is 10, `50 > 10`, so `cap` returns `10`.
- Naming the argument (`half(x: 6)`) is allowed: in `Arg = [ ident ":" ] [ "@" ] Expression` the name is optional, and names are only *required* when two parameters share a type.
- Unused extern declarations are not bindings, so nothing here is "unused" (section 5).

# limit_ba

**Build:** succeeds. **Exit code:** 0. **Output:**

```
3
10
```

Why: this is `limit_ab` with the two groups swapped. Section 4 says "Declaration order never changes what a program means", so it must mean the same as `limit_ab`. And here, even if the headers did share one translation unit in file order, `b.h` comes first, sees no `LIMIT`, and defines it as 10 anyway. `half(6)` = 3, `cap(50)` = 10.

# jpeg_ab

**Build:** fails. `heroes build jpeg_ab.hero -o jpeg_ab` reports a C header error from clang and writes no binary, so `./jpeg_ab` does not exist and nothing runs. The build exits nonzero.

Why:
- The C fact: libjpeg's `jpeglib.h` (libjpeg and libjpeg-turbo alike) does not include `<stdio.h>`. It uses `FILE` (in `jpeg_stdio_dest(j_compress_ptr, FILE *)` and `jpeg_stdio_src`) and `size_t` (in the memory manager's `alloc_small`/`alloc_large` and the `jpeg_mem_*` interfaces), and its own documentation (libjpeg.txt) tells applications to include `<stdio.h>` (or something else that defines `FILE` and `size_t`) before including it. Parsed on its own, it gives errors like "unknown type name 'FILE'" and "unknown type name 'size_t'".
- I read each group as checked against its own header only, for the same reasons as in `limit_ab` ("A group names its header", "against that header", and "Declaration order never changes what a program means"). So the `stdio.h` group above does not help the `jpeglib.h` group. clang cannot parse `jpeglib.h`, so it cannot check `jpeg_std_error` against it, and the build fails.
- Without that failure, nothing else in the program would be wrong. `jpeg_std_error(err: ptr) -> ptr` fits the C function `struct jpeg_error_mgr *jpeg_std_error(struct jpeg_error_mgr *)`, because section 13 does not hold "what a `ptr` points at" to a width. `puts(s: cstr) -> i32` fits `int puts(const char *)`. `package "libjpeg"` is the pkg-config name libjpeg-turbo installs. A build that got that far would print `1` and exit 0.

# jpeg_ba

**Build:** fails, as `jpeg_ab` does. No binary and no output from `./jpeg_ba`. The build exits nonzero.

Why: here `jpeglib.h`'s group comes first, so `stdio.h` cannot come before it under any reading. Under mine (each group alone) it fails for the same reason as `jpeg_ab`: `FILE` and `size_t` are undefined when `jpeglib.h` is parsed. Section 4 ("Declaration order never changes what a program means") also requires `jpeg_ab` and `jpeg_ba` to behave the same.

# choice_points

1. **How the headers of separate `extern` groups relate.** The specification does not say whether the groups' headers go into one C translation unit (in file order) or are each processed alone.
   - My choice: each group alone. I base this on "A group names its header", "clang checks ... against that header", and above all "Declaration order never changes what a program means", which covers `extern` groups because they are Declarations.
   - The other choice: one translation unit in file order. `limit_ab` would then print `3` and `50`, because `a.h`'s `LIMIT 100` reaches `b.h`'s `#ifndef` and 50 is not above 100. `limit_ba` would still print `3` and `10`. `jpeg_ab` would build (stdio.h first defines `FILE` and `size_t`) and print `1`, exit 0. `jpeg_ba` would still fail to build. That reading breaks section 4's promise, which is why I rejected it.
   - A third possibility: a compiler that keeps order-independence by refusing programs whose meaning depends on order. That would be an error neither program here triggers under my reading.
2. **The size of `long`.** I assumed an ordinary LP64 machine (macOS or 64-bit Linux), so `long` is 64 bits and `x: i64` matches. On LLP64 (64-bit Windows) `long` is 32 bits. Section 13 then refuses `x: i64` (a parameter must be at the header's width, and narrowing i64 to a 32-bit long is not a conversion "C converts exactly"), so neither `limit_` program would build.
3. **Whether `jpeglib.h` includes `stdio.h` itself.** Every libjpeg and libjpeg-turbo release I know leaves this to the includer, and that is the basis of my `jpeg_` answers. An installed `jpeglib.h` patched to include `<stdio.h>` would let both `jpeg_` programs build and print `1` with exit 0.

# confidence

- `limit_ba`: high (3 and 10, exit 0). Every reading gives the same result.
- `limit_ab`: moderate to high (3 and 10). The answer depends on choice point 1, and section 4 decides it fairly clearly.
- `jpeg_ab` and `jpeg_ba`: moderate (both fail to build). This depends on choice point 1 and on the C fact about `jpeglib.h`, which I am fairly sure of. `jpeg_ba` failing is the more certain of the two.

# context

Nothing outside this directory reached my context except the harness's automatic session information (working directory, platform Darwin, date, and the user's account email, which I did not use). I read only `brief.md`, `spec.md`, the four `.hero` programs, `a.h` and `b.h`. What I say about `jpeglib.h`, `stdio.h`, `long` widths and pkg-config names comes from general knowledge, not from files I read.
