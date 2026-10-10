# program

```
extern "stdio.h"
    record File tag _IO_FILE
    function fopen(path: cstr lent, mode: cstr lent) -> File acquires fclose
    function fclose(stream: File consumes) -> i32

extern "jpeglib.h" package "libjpeg"
    record JpegDecompress tag jpeg_decompress_struct
    function jpeg_stdio_src(cinfo: JpegDecompress, infile: File)

function main()
    f = fopen(path: "photo.jpg".cstr(), mode: "rb".cstr())
    if f == nullptr
        print("missing")
    else
        print("opened")
        _ = fclose(f)
```

# choice_points

1. **The struct tag for `FILE`.** The spec says a handle is a record with a `tag` and no fields, and the tag is the name the header writes after the word `struct`. `FILE` is a typedef, and its struct tag depends on the C library: glibc writes `typedef struct _IO_FILE FILE`, macOS writes `struct __sFILE`, and musl writes `struct _IO_FILE` too. I chose `_IO_FILE`, the glibc/musl name, because I took that to be the "ordinary machine". On macOS clang would reject it, and `tag __sFILE` would be needed instead. I also considered `tag FILE` and `tag void`. The spec does not allow `tag FILE`, since `FILE` never comes after `struct`. `tag void` would declare `void *`, which disagrees with `fopen`'s `FILE *` result, so clang's result check should refuse it.
2. **How to link stdio.** I gave the `stdio.h` group no `link` and no `package`, because its symbols live in the C library, which is always linked. Adding `link "c"` should be redundant but harmless.
3. **How to bring in libjpeg.** I used `package "libjpeg"`, as the brief names it. Writing `link "jpeg"` instead would hard-code the library name and skip pkg-config's include and library paths, so it could fail where the headers are not in a default location.
4. **Ownership marks on `fopen`/`fclose`.** I marked `fopen`'s result `acquires fclose` and `fclose`'s parameter `consumes`, so the compiler tracks the open file and requires it to be closed. Without the marks it would build, but there would be no check that the file is closed. I added no `when 0` to `fclose`. With `when 0`, the consume would only count when `fclose` returned 0. On a failed close the handle would then still be owed, and the program would abort at the end of `main`.
5. **Parameter types.** `fclose` returns C `int`, so I declared `i32`. The spec allows a wider result, so `i64` would also be accepted and would give the same behaviour. The C strings are `cstr lent` because a `.cstr()` lend only reaches a `lent` parameter. Leaving out `lent` makes the call a compile error.
6. **Named arguments to `fopen`.** Both parameters are `cstr`, so the spec requires named arguments. I named them `path` and `mode`. Any names would do as long as the call uses the same ones.
7. **Declaring `jpeg_stdio_src`.** `j_decompress_ptr` is a pointer to `struct jpeg_decompress_struct`, so I declared it as the handle `record JpegDecompress tag jpeg_decompress_struct`. The record name is my choice. For `FILE *`, the function reuses the `File` record from the stdio group, because two records may not share a tag. I gave the parameters no ownership marks. The function is never called, so marks would change nothing, and the spec ties `borrows` to calls that hand a handle back.
8. **Discarding `fclose`'s result.** I discarded it with `_ =`, which the spec requires for a value nobody uses. The other option was to check it and print something on failure, but the brief asks for no output there.
9. **Branch order.** I tested `f == nullptr` first. Writing `f != nullptr` first would give the same behaviour.

# confidence

I think it builds and does what is asked, with moderate confidence. I could not run anything, so none of this is tested. What I am least sure of, most uncertain first:

- **The `FILE` tag.** It only works on a glibc or musl system. On macOS, clang will refuse `_IO_FILE`.
- **Whether `jpeglib.h` compiles on its own.** The header uses `FILE` and `size_t` but does not include `stdio.h` itself. The spec does not say whether all groups share one translation unit, with `stdio.h` included first, or whether each header is checked separately.
- **Null handles.** I assume a null result from an `acquires` call starts no life, so the `missing` path does not abort at the end of `main` with an unconsumed handle. The spec says this only for transfers.
- **The never-called `jpeg_stdio_src` declaration.** I assume it is not rejected as unused, since the unused rule covers bindings and parameters, not declarations.
- **Records across groups.** I assume a record declared in one group can be a parameter type in another group.

# context

Besides `brief.md` and `spec.md`, my context included the harness's environment details: the working directory path, that the platform is darwin (macOS), the date, and the user's account email. I did not read anything outside this directory. I deliberately did not use the darwin detail to choose the `FILE` tag, because the brief limits my inputs to this directory's files. If the program is built on that macOS machine, the tag must become `__sFILE`. I also drew on general knowledge of C headers that was not in the spec: glibc's and macOS's `FILE` struct tags, that `j_decompress_ptr` points to `struct jpeg_decompress_struct`, and that `jpeglib.h` expects `stdio.h` to be included before it.
