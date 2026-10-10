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

1. **Group order.** The `stdio.h` group comes first and `jpeglib.h` second, because the spec says C reads a module's headers in the order the groups are written and `jpeglib.h` uses `FILE` and `size_t` without including `stdio.h` itself. The other order would have clang read `jpeglib.h` with no `FILE` declared, and the build would fail.
2. **Tag for `FILE`.** I wrote `tag FILE`, the typedef name that every platform's `stdio.h` provides. The alternative was the struct tag itself, `_IO_FILE` on glibc or `__sFILE` on macOS. That would tie the program to one libc and fail to check on the other. The spec's own example, `record Db tag sqlite3` for `sqlite3 *`, also uses the name C code writes, which supports this choice. Even so, the spec's wording ("the header writes it after the word struct") leaves some doubt about whether a typedef name is accepted (see confidence).
3. **Tag for the decompress object.** `j_decompress_ptr` is `struct jpeg_decompress_struct *`, so I wrote `record Decompress tag jpeg_decompress_struct`. `tag void` would also be accepted for a pointer parameter, but it would let any `void *` handle be passed and gives up type checking. The record names (`File`, `Decompress`) are free choices and change nothing outside the program.
4. **`lent` on `fopen`'s strings.** `fopen` does not keep `path` or `mode`, so both are `lent`. That lets `"...".cstr()` lends reach them. Without `lent`, a lend could not reach them and I would have to use `lease()` and `end_lease`.
5. **`acquires fclose` on `fopen` and `consumes` on `fclose`.** Together these give the stream an owned life that ends at `fclose`. Leaving them out would make the handle untracked, so a forgotten close would go unnoticed. I put no `when 0` on `fclose` because C frees the stream whatever the result is. With `when 0`, a failed close would count as not consuming the handle, and the program would abort at exit with a live handle.
6. **Result widths.** `fclose` returns `i32`, matching C's `int`. The spec allows a wider result such as `i64`, which would make no difference here. `jpeg_stdio_src` returns `void`, so it has no `->`.
7. **`jpeg_stdio_src` parameters left unmarked.** libjpeg keeps `infile` in its source manager, so neither parameter is `lent`. That is the default ("taken to keep what it is handed"). I added no `consumes` or `transfers` because the call does not end either life. Marking `infile` `consumes` would wrongly end the stream's life at a call that never closes it.
8. **Parameter names.** `fopen` has two `cstr` parameters, so named arguments are mandatory. I used `path` and `mode`, which the call site repeats. Other names would only change the call site.
9. **The null test and the `fclose` result.** I test `f == nullptr` with `if`/`else`, and throw away `fclose`'s result with `_ =`. The brief does not ask for close errors to be reported. Printing them would add output the task does not request.
10. **Package vs link.** The brief gives the pkg-config package, so the jpeg group uses `package "libjpeg"`. `stdio.h` needs no `link` because libc is always linked.

# confidence

I believe this builds and does what is asked: it prints `opened` and closes the file when `photo.jpg` can be opened in mode `rb`, and prints `missing` otherwise. I could not run anything. What I am least sure of:

- **Whether `tag FILE` is accepted.** `FILE` is a typedef, not a struct tag. The spec's wording ("the header writes it after the word struct") could be read as requiring the true tag, which differs between platforms. If so, a portable declaration is not possible, and `tag void` would be the fallback.
- **Whether a null result from an `acquires` call creates an owed life.** I assume it does not, so that the `missing` path does not abort at exit for an unconsumed handle. The spec says this directly only for `transfers` ("not made when that handle is null").
- **Whether declaring `jpeg_stdio_src` with an unmarked `File` parameter is fine.** The spec says that where any extern consumes a handle type, "every call handing one back says which it is". `jpeg_stdio_src` hands nothing back, so I believe the rule does not apply to it.

# context

Only this directory's `brief.md` and `spec.md` reached my context, plus the harness's system context, which included the user's account email (not used). Everything else I relied on came from general knowledge of C, `stdio.h` and libjpeg, not from any file outside this directory.
