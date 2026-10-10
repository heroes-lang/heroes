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

1. **How to name `FILE`.** I used `record File tag FILE`, a handle that stands for `FILE *`. The spec says `tag void` means `void *` and `tag sqlite3` means `sqlite3 *`, so I read `tag` as naming the C type exactly as it is spelled. Other options:
   - The platform struct tag (`_IO_FILE` on glibc, `__sFILE` on macOS) would tie the program to one C library and fail to build on the other.
   - `record FILE` with no tag and no fields is not a handle, because a handle needs a tag. It would declare an empty struct, which clang would reject.
2. **Which group owns `File`.** `jpeg_stdio_src` needs a `FILE *` as well. The spec forbids two records with the same tag unless the tag is `void`, so a second `FILE` record in the jpeg group would be refused. I therefore declared it once, in the stdio group, and the jpeg group reuses it. Declaring it only in the jpeg group would also work.
3. **The jpeg group.** I used `package "libjpeg"`, as the brief asks. `link "jpeg"` would only work where the headers and library sit on the default search paths.
4. **The `cinfo` type.** `j_decompress_ptr` is `struct jpeg_decompress_struct *`, so I declared it as a handle with that tag. A `ptr` (`tag void` style) would also be accepted, because the spec does not check what a pointer points at. It would be less precise.
5. **Ownership marks.**
   - `fopen` is marked `acquires fclose` and `fclose` is marked `consumes`, so the compiler tracks the stream's life.
   - Without these marks the build would still work, but nothing would check that the file gets closed.
   - I left out `when 0` on `fclose`, because the stream is released even when `fclose` fails. With `when 0`, a failed close would leave the handle counted as still open.
6. **`jpeg_stdio_src` parameters.** I gave them no `lent`, because libjpeg keeps `infile` in its source manager. I gave them no ownership word either, so the call only borrows the `File`. Marking `infile` `lent` would wrongly promise that C does not keep it. A `transfers` mark would hand the stream's life to libjpeg, but there is no receiver for it here.
7. **Result widths.** `fclose` returns `i32`, which is exactly C's `int`. The spec also allows `i64`, since a result may be wider than C's.
8. **Parameter names and call style.** Both `fopen` parameters are `cstr`, so named arguments are required. I picked `path` and `mode`. I passed the strings with `.cstr()`, which lends them for the call and needs `lent`. A `lease()` would also work, but I would then have to call `end_lease` myself.
9. **Discarding `fclose`'s result.** I wrote `_ = fclose(f)`, which the spec requires for a non-`()`, non-fallible value. Binding the result to a name and not reading it would be an unused-binding error.

# confidence

I think it is likely, but not certain, that the program builds and prints `opened` or `missing` correctly. What I am least sure of:

- **`jpeglib.h` needs `stdio.h` first.** libjpeg's header uses `FILE` and `size_t` without including `<stdio.h>` itself. I am assuming the compiler puts each group's header into one translation unit in source order, which is why the stdio group comes first. The spec does not say this. If each group is checked on its own, clang could reject `jpeglib.h`.
- **`tag FILE`.** If the compiler writes `struct FILE *` instead of `FILE *`, the declaration would not match on glibc or macOS.
- **Using `File` in a second group.** The spec only says that fields must be "another record of the group". It says nothing explicit about parameters, so reusing `File` in the jpeg group is my reading.
- **A null result from `fopen`.** I assume a null result from an `acquires` call starts no life. If it did, the `missing` path would abort at the end of `main` for an unconsumed handle.

# context

Nothing beyond this directory's files reached me. I read only `brief.md` and `spec.md`, plus the user email that the harness attached automatically, which I did not use. The details about libjpeg and the C library (the `FILE` typedefs, and `jpeglib.h` needing `stdio.h` first) come from my own background knowledge, not from any file.
