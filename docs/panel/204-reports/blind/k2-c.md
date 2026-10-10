# program

```
extern "stdio.h"
    record File tag FILE
    function fopen(path: cstr lent, mode: cstr lent) -> File acquires fclose
    function fclose(stream: File consumes) -> i32

extern "jpeglib.h" package "libjpeg"
    record Decompress tag jpeg_decompress_struct
    function jpeg_stdio_src(cinfo: Decompress, infile: File)

function main()
    f = fopen(path: "photo.jpg".cstr(), mode: "rb".cstr())
    if f == nullptr
        print("missing")
    else
        print("opened")
        _ = fclose(f)
```

# choice_points

1. **The tag for `FILE`.** A handle needs a `tag` and no fields. The spec says
   `tag` gives the name "the header writes after the word struct". The struct
   behind `FILE` differs by platform (`struct _IO_FILE` on glibc, `struct __sFILE`
   on macOS), while the typedef `FILE` is the same everywhere. I wrote
   `tag FILE`, reading `tag` as naming the C type the handle points at (the
   sqlite example `tag sqlite3` works either way because sqlite's typedef and
   struct share a name). The alternative, `tag _IO_FILE`, would build only on
   glibc and fail on macOS (and vice versa for `__sFILE`); `tag void` would make
   it `void *`, which clang might refuse against `FILE *` in `fopen`'s result,
   and would let any `void *` handle be passed in its place.
2. **Record names.** `File` and `Decompress` are my names; the C names live in
   the tags. Naming the record `FILE` would change nothing but readability.
3. **Group order.** `stdio.h` comes first because `jpeglib.h` uses `FILE` and
   `size_t` without including `stdio.h` itself. The other order would likely make
   clang fail on `jpeglib.h`.
4. **Linking.** No `link` for `stdio.h` (libc is linked anyway); `package
   "libjpeg"` for `jpeglib.h`, as the brief says. Using `link "jpeg"` instead
   would also work where headers are in the default paths, but not where they
   are not.
5. **`fclose`'s result width.** `i32`, C's `int`. A result may be wider, so `i64`
   would also be accepted; behaviour is the same.
6. **No `when 0` on `fclose`.** `fclose` ends the stream's life even when it
   returns `EOF`, so `consumes` should count always. With `when 0`, a failing
   `fclose` would leave the handle counted as live and the program would abort
   at the end of `main`, reporting an unended handle.
7. **`lent` on `fopen`'s strings.** `fopen` does not keep the path or mode, so
   `lent` lets `.cstr()` lend them. Without `lent` a lend could not reach the
   parameter and I would need `lease()`/`end_lease` cells.
8. **`jpeg_stdio_src`'s parameters.** Declared without `lent`, `consumes` or
   `transfers`: it keeps both pointers (cinfo stores a source manager that holds
   the `FILE *`), and the default is that a parameter keeps what it is handed.
   Since it is never called, the alternatives (`lent` on `cinfo`, or `transfers`)
   would only matter if it were; `transfers fclose` on `infile` would hand the
   file's life to `cinfo`, which needs `cinfo` to be a handle with a matching
   mark, and would be refused here.
9. **Discarding `fclose`'s result.** `_ = fclose(f)`. Checking it and printing
   something on failure was possible but not asked for.
10. **The test for failure.** `f == nullptr`, since `==` on a handle compares the
    address and `nullptr` is its null. A `match` is not available for handles.

# confidence

I believe it builds and prints `opened` or `missing` as asked, closing the file
only when it was opened. What I am least sure of, in order:

- Whether `tag FILE` is accepted for a typedef name rather than a struct tag.
  If the compiler insists on the struct's own tag, the program needs a
  platform-specific `_IO_FILE` or `__sFILE`.
- Whether `acquires fclose` begins an owed life even when `fopen` returns null.
  The spec says a transfer into a null handle is not made, but says nothing
  explicit about `acquires` on a null result. If a null result counted as a life
  owed, the `missing` path would abort at the end of `main` for an unended
  handle; I read it as not counting, since `fclose(NULL)` is not something the
  program could owe.
- Whether a record of the `stdio.h` group (`File`) may be a parameter type in the
  `jpeglib.h` group. The spec restricts fields to "another record of the group"
  but says nothing like that for parameters, so I took it as allowed.

# context

Only `brief.md` and `spec.md` from this directory reached my context, along
with the harness's own system context (environment details and the user's
account email, which I did not use). I read no other files and ran nothing.
