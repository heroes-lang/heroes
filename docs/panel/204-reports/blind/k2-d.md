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
        _ = fclose(stream: f)
```

# choice_points

1. **How to name `FILE` as a handle.** A handle needs a `tag` and no fields, and the spec says `tag` gives the name the header writes after the word `struct`. `FILE` is a typedef, and the struct behind it differs by platform (`struct _IO_FILE` on glibc, `struct __sFILE` on macOS). I wrote `record File tag FILE`, using the typedef name every `stdio.h` provides. If I had used `tag _IO_FILE` or `tag __sFILE`, the program would match only one platform's header and clang would refuse it on the other. Using `tag void` would declare `fopen` as returning `void *`, which clang's result check against `FILE *` would likely refuse.
2. **Group order.** `jpeglib.h` uses `FILE` and `size_t` without including `stdio.h`, so the `stdio.h` group comes first. In the other order the C side would fail to compile `jpeglib.h`.
3. **Library vs package.** `stdio.h` gets no `link` because libc is always linked. `jpeglib.h` uses `package "libjpeg"` as the brief says. `link "jpeg"` would also usually work, but it skips pkg-config's include and library paths.
4. **Lifetime marks.** `fopen` is `acquires fclose` and `fclose` is `consumes`, so the compiler makes sure the opened file gets closed. Leaving the marks out would compile too, but it would give up that check. I put no `when 0` on `fclose`, because C frees the stream even when `fclose` fails. With `when 0`, a failed close would leave the handle counted as live, and the program would abort at exit.
5. **`fclose` result width.** I declared it `i32` to match C's `int`. The spec allows a wider result (`i64`), and that would behave the same here. The result is dropped on purpose with `_ =`.
6. **`fopen` parameters.** Both are `cstr lent`: C reads the strings only during the call, and a `.cstr()` lend can only reach a `lent` parameter. Without `lent` the lend would be refused, and I would need `lease`/`end_lease` instead. Both parameters have type `cstr`, so the call has to name them (`path:`, `mode:`).
7. **`jpeg_stdio_src` parameters.** `j_decompress_ptr` is `struct jpeg_decompress_struct *`, so I declared a handle `Decompress tag jpeg_decompress_struct`. The function returns `void`, so it has no `->`. I left both parameters without `lent` or other marks because the function is never called, and libjpeg does keep `infile`, so `lent` would be wrong. Adding `borrows` to `infile` was another option, but it changes nothing when the function is never called.
8. **Null test.** I wrote `f == nullptr` before any close, so `==` never touches a handle that has already been given back. Only the non-null branch closes the file. Calling `fclose` on null would be undefined behavior in C.
9. **Mode string.** `"rb"` gives binary read mode, as the brief asks.

# confidence

I believe this program builds and does what the brief asks, but I can't be sure because I could not run anything. The thing I am least sure of is `record File tag FILE`. The spec defines `tag` as the struct name, not a typedef name, so a strict compiler might emit `struct FILE *` and fail the header check. The fix would be a platform-specific tag. The second risk is whether a null result from an `acquires` function counts as a live handle at the exit check. The spec suggests null handles start no life (a transfer into a null handle is not made), but it never says this outright. The third risk is that the spec does not say clearly whether a parameter in the `jpeglib.h` group may use a handle declared in the `stdio.h` group. Only fields are explicitly limited to records of their own group.

# context

Nothing reached my context except `brief.md` and `spec.md` from this directory, plus the session's environment information (working directory, platform, date, and an automatically attached user email that I did not use). I also used my own general knowledge of C's `stdio.h` and libjpeg's `jpeglib.h` declarations.
