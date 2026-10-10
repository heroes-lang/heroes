# Report

The rule that decides all four programs is in section 13: "C reads a module's headers in the order its groups are written, so one that needs another's names comes after it." So each pair is the same program with its headers included in the two possible orders.

## limit_ab

**Build:** succeeds. C reads `a.h` and then `b.h`.

**Exit code:** 0

**Output:**
```
3
50
```

**Why:**
- `a.h` runs `#define LIMIT 100` first. In `b.h`, `#ifndef LIMIT` is then false, so the `#define LIMIT 10` is skipped, and `cap` is compiled as `v > 100 ? 100 : v`.
- `half(6)` is `6 / 2` = 3 in C `long`. `cap(50)` checks `50 > 100`, which is false, so it returns 50.
- The declarations match the headers. Section 13 says a parameter is "declared at the header's own width and sign". `long` is 64-bit on an ordinary LP64 machine, so `i64` matches. `int` is `i32`. "clang checks every result type ... against that header", and `i64`/`long` and `i32`/`int` agree.
- No `link` is needed. The functions are `static inline` in the headers, and section 13 asks for `link` only "when the symbols need one".
- The literals `6` and `50` take the parameter types (section 2: "A literal takes the type its context asks for"). Writing the parameter name in `half(x: 6)` is optional but allowed (`Arg = [ ident ":" ] ...`).
- `print` writes each number followed by "exactly one trailing newline" (section 11). `main` returns normally, so the exit code is 0.

## limit_ba

**Build:** succeeds. clang may warn that `LIMIT` is redefined, but that is not an error.

**Exit code:** 0

**Output:**
```
3
10
```

**Why:**
- This time C reads `b.h` first. `LIMIT` is not defined yet, so `b.h` defines it as 10. The preprocessor expands the macro as it reads, so the body of `cap` becomes `v > 10 ? 10 : v` right there.
- `a.h` then runs `#define LIMIT 100`. This redefines the macro with a different body. By default clang only warns about this (`-Wmacro-redefined`); it does not stop. The new value cannot reach `cap`, because `cap` was already expanded.
- `half(6)` = 3. `cap(50)` checks `50 > 10`, which is true, so it returns 10.
- The type checks are the same as in `limit_ab`. Section 13's ordering sentence is what makes the two programs behave differently.

## jpeg_ab

**Build:** succeeds on a machine where libjpeg (for example libjpeg-turbo) and its `libjpeg` pkg-config entry are installed. C reads `stdio.h` and then `jpeglib.h`.

**Exit code:** 0

**Output:**
```
1
```

**Why:**
- C fact: `jpeglib.h` uses `FILE` (in `jpeg_stdio_dest` and `jpeg_stdio_src`) and `size_t` (in the memory-manager function pointers), but it does not include `<stdio.h>` itself. libjpeg's documentation says to include `stdio.h` (or something that defines `FILE` and `size_t`) before it.
- Here `stdio.h` comes first, so both names exist when `jpeglib.h` is read. This is the case section 13 describes: "one that needs another's names comes after it."
- `puts(const char *) -> int` matches `s: cstr` and `-> i32`.
- `jpeg_std_error(struct jpeg_error_mgr *) -> struct jpeg_error_mgr *` matches `ptr`/`ptr`. Section 13 makes an exception for "what a `ptr` points at", and a `ptr` result is a pointer just like C's.
- `package "libjpeg"` "asks the system where its headers and libraries are".
- Neither extern function is ever called. `main` only does `print(1)`, which writes `1` and a newline.

## jpeg_ba

**Build:** fails. clang stops while reading `jpeglib.h`, so the build produces no binary and there is nothing to run.

**Exit code / output:** none. `heroes build` exits non-zero and reports the C errors, such as `unknown type name 'FILE'` (in the `jpeg_stdio_dest`/`jpeg_stdio_src` prototypes) and `unknown type name 'size_t'` (in `struct jpeg_memory_mgr` and elsewhere).

**Why:**
- Section 13 says "C reads a module's headers in the order its groups are written". Here `jpeglib.h` is read before `stdio.h`.
- The C fact from `jpeg_ab` applies: `jpeglib.h`, `jconfig.h` and `jmorecfg.h` neither include `<stdio.h>` nor define `FILE`.
- In C, an unknown type name in a declaration is a hard error, not a warning. That rules out the check the spec describes ("clang checks every result type ... against that header"), so the program does not build.
- The `puts` group that comes after it cannot help, because it arrives too late. The spec's sentence "so one that needs another's names comes after it" describes exactly what this program gets wrong.

## choice_points

1. **Macro redefinition in `limit_ba`.** The spec does not say whether the C warnings clang gives while checking the headers stop the build.
   - My choice: clang's defaults, under which a macro redefinition is only a warning. Result: build succeeds, prints `3` then `10`.
   - The other choice: if `heroes` treats warnings as errors (`-Werror`) or uses pedantic errors, the redefinition fails the build and `limit_ba` produces no binary.
   - In neither case does it print `50` for `cap`.
2. **`static inline` functions without `link`.** The spec does not say how an extern function that exists only as a `static inline` definition in a header gets bound.
   - My choice: the compiler compiles against the header, so the inline definition is available and no library is needed.
   - The other choice: if `heroes` linked against an external symbol only, both `limit_` programs would fail at link time with undefined `half` and `cap`.
3. **The width of C `long`.** I assumed an LP64 machine, where `long` is 64 bits, so `half(x: i64)` matches the header.
   - The other choice: on LLP64 (Windows), `long` is 32 bits, the `i64` parameter "disagrees" and "is refused". Both `limit_` programs would then fail to compile.
4. **Extern functions that are declared but never called** (`puts`, `jpeg_std_error`).
   - My choice: "An unused binding or parameter is a compile error" is about bindings and parameters, not extern declarations, so unused extern functions are allowed.
   - The other choice: if they counted as unused, both `jpeg_` programs would be refused before the header order even mattered.
5. **Nothing included ahead of the user's headers.** In `jpeg_ba` I assumed the compiler puts nothing of its own (such as `<stdio.h>` or `<stddef.h>`) in front of the program's headers. The spec says headers are read "in the order its groups are written".
   - The other choice: if the generated C included `stdio.h` first, `jpeg_ba` would build and print `1`, exactly like `jpeg_ab`.
6. **libjpeg is installed.** I took "as installed on an ordinary machine" to mean libjpeg's header and pkg-config entry are present.
   - The other choice: without them, `jpeg_ab` would fail too, because the package lookup would fail.

## confidence

- `limit_ab`: high (about 90%).
- `limit_ba`: fairly high (about 75%). The main risk is choice point 1, a warning turned into an error. Either way, the answer is not `50`.
- `jpeg_ab`: high (about 85%).
- `jpeg_ba`: fairly high (about 75%) that it fails to build because `FILE` and `size_t` are undefined. The risks are choice point 5, or a libjpeg version whose `jpeglib.h` includes `stdio.h` itself. I know of no mainstream version that does this.

## context

Nothing from outside this directory reached my context. I read `brief.md`, `spec.md`, the four `.hero` files, `a.h` and `b.h`. I did not read `stdio.h` or `jpeglib.h` from any system. What I say about them (that `jpeglib.h` needs `FILE` and `size_t` defined first, and the prototypes of `puts` and `jpeg_std_error`) comes from my general knowledge of C and libjpeg, not from files. The system prompt included the user's email address as account context, and it played no part in this analysis.
