# program

```
extern "stdio.h"
    record File tag FILE
    function fopen(path: cstr lent, mode: cstr lent) -> File acquires fclose
    function fclose(stream: File consumes) -> i32

extern "jpeglib.h" package "libjpeg"
    function jpeg_stdio_src(cinfo: ptr, infile: ptr)

function main()
    f = fopen(path: "photo.jpg".cstr(), mode: "rb".cstr())
    if f == nullptr
        print("missing")
    else
        print("opened")
        _ = fclose(f)
```

# choice_points

1. **How to name `FILE`.** I declared a handle `record File tag FILE`, so it stands for `FILE *`. The spec says `tag` gives the name "the header writes after the word struct". By that reading the tag would be `_IO_FILE` on glibc and `__sFILE` on macOS, which ties the program to one platform. The spec's handle examples (`tag sqlite3` meaning `sqlite3 *`, `tag void` meaning `void *`) use a C type name, so I used the typedef name `FILE`, which is the same everywhere. If the compiler takes a tag only as a struct tag, `tag FILE` may be refused, and the platform-specific tag would be needed instead.
2. **Handle versus `ptr` for `fopen` and `fclose`.** I used a handle so I could add the lifetime marks (`acquires fclose`, `consumes`). With a plain `ptr` the program would also build, but the compiler would not check that the file gets closed.
3. **`lent` on the `fopen` strings.** `fopen` does not keep `path` or `mode`, so they are marked `lent`, which lets `"...".cstr()` lend them for the call. Without `lent` the spec assumes C keeps the pointer, a lend is refused, and I would need `lease()` and `end_lease`.
4. **Result width of `fclose`.** I wrote `i32`, which is C's int. The spec allows a wider result such as `i64`, which would behave the same.
5. **No `when 0` on `fclose`.** `fclose` frees the stream even when it fails, so the handle's life ends no matter what it returns. With `when 0`, a non-zero return would leave the handle alive, and the program would abort at exit for an unconsumed handle.
6. **`jpeg_stdio_src` parameters as `ptr`.** `cinfo` is `j_decompress_ptr`, which is `struct jpeg_decompress_struct *`, and `infile` is `FILE *`. The spec lets a `ptr` parameter match whatever a pointer points at, so `ptr` avoids two open questions: whether a struct-only tag works as a handle, and whether one extern group may use another group's record. The other choice is `record JpegDecompress tag jpeg_decompress_struct` plus `infile: File`, which checks types more strictly if the compiler accepts it. Because the function is never called, the marks and `lent` on it change nothing at runtime.
7. **`package "libjpeg"` versus `link "jpeg"`.** The brief names the pkg-config package, so I used `package`. `link "jpeg"` would also work where the headers are in default paths, but it would not pick up nonstandard include or library paths.
8. **Group order.** The stdio group comes first because `jpeglib.h` needs `FILE` and `size_t` declared before it is included. The spec says declaration order never changes meaning, so this is a precaution in case the generated C includes headers in source order.
9. **Mode string.** I used `"rb"` for binary read, as the brief asks. `"r"` is the same on POSIX but not on Windows.
10. **Checking for null.** I compared the handle with `f == nullptr`, which the spec allows for handles. The alternative is to use `ptr` and compare that, with the same effect.

# confidence

I believe it builds and does what is asked: it prints `opened` or `missing` and closes the file only when it was opened. What I am least sure of, in order:
- Whether `tag FILE` is accepted for a typedef name with no `struct FILE` behind it (point 1).
- Whether a null result from an `acquires` call starts no handle life. The spec says this outright only for `transfers`. If a null result still counted as a live handle, the `missing` path would abort at the end of `main` because the handle was never consumed.
- Whether the generated C includes `stdio.h` before `jpeglib.h` (point 8).
- That declaring `jpeg_stdio_src` without calling it is not an unused-declaration error. The spec's unused rule covers bindings and parameters, not top-level declarations, so I believe it is fine.

# context

Nothing reached my context except this directory's files, `brief.md` and `spec.md`, plus the session's environment information (working directory, platform, date, and the user's account email, which I did not use). I wrote `main.hero` and `report.md` and ran nothing. The facts about C headers (glibc and macOS `FILE` tags, `jpeglib.h` needing `stdio.h` first, `j_decompress_ptr`) come from my own background knowledge, not from any file.
