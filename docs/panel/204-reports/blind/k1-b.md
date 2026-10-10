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

1. **How to represent `FILE *`.** I used a handle record (`record File tag ...` with no fields). The other option was a plain `ptr` result and parameter. Section 13 says a `void *` that C hands out for the program to give back "is declared as one" (a handle), and section 13 also uses `transfers fclose` as its example. By that reasoning a `FILE *` should be a handle too. With `ptr` there would be no lifetime tracking, and clang might refuse the result type if it doesn't count `ptr` as matching `FILE *`.
2. **The tag of `FILE`.** Section 13 says the tag is the name the header writes after the word `struct`. `FILE` is a typedef, so I can't use `tag FILE` with confidence. I chose `_IO_FILE`, which is the struct name in glibc and musl, so it targets an ordinary Linux machine. On macOS the struct is `__sFILE`, so that platform needs `tag __sFILE`. `tag FILE` would only work if the compiler accepts typedef names as tags, and the spec does not say it does. `tag void` would give `fopen` a `void *` result, which clang would likely reject against `FILE *`.
3. **Two extern groups vs one.** The brief names the functions per header, so I used one group for `stdio.h` and one for `jpeglib.h` with `package "libjpeg"`. stdio comes first because `jpeglib.h` needs `FILE` and `size_t` declared before it. Putting everything in one `jpeglib.h` group would have checked `fopen` against a header that doesn't declare it.
4. **Where `File` is declared.** Two records may not share a tag, so `File` is declared only in the stdio group. The jpeglib group's `jpeg_stdio_src` uses it as a parameter type. Declaring a second record with the same tag in the jpeglib group would be refused.
5. **The type of `j_decompress_ptr`.** I used a handle `record JpegDecompress tag jpeg_decompress_struct`, since `j_decompress_ptr` is `struct jpeg_decompress_struct *`. A `ptr` parameter would also pass, because of the "what a ptr points at" exception, but it would be untyped.
6. **Ownership marks.**
   - `fopen` uses `acquires fclose` and `fclose` uses `consumes`. Without these, an opened handle would have no disposer. Also, "where any extern consumes a handle type every call handing one back says which it is", so `fopen` must carry a mark once `fclose` consumes.
   - I gave `fclose` no `when 0`. C ends the stream even when `fclose` fails, so tying consumption to the return value would be wrong.
   - The `fopen` arguments are `lent` because fopen does not keep them. Leaving them unmarked would mean "kept", and a `.cstr()` lend would then be refused.
   - The `jpeg_stdio_src` parameters are left unmarked, which means "kept". That is true of `infile`, which the source manager stores. It doesn't matter here because the function is never called.
7. **Parameter names.** Both `fopen` parameters are `cstr`, so named arguments are mandatory. I picked `path` and `mode`. The header's own names (`__filename`, `__modes` in glibc) would also work, but they are ugly.
8. **Result width of `fclose`.** I used `i32`, matching C's `int`. `i64` would also be allowed, since a result may be wider.
9. **The `fclose` result.** I discarded it with `_ =`. The brief asks for nothing on a close failure. Checking it and printing something would add output the brief does not ask for.
10. **The null test.** `f == nullptr` compares the address, which is allowed on a handle before it is given back. A `match` is not possible on a handle.

# confidence

I'm moderately confident it builds and prints `opened` or `missing` correctly on a glibc Linux machine with libjpeg's pkg-config file installed. I could not run anything. What I'm least sure of:

- **The `FILE` tag.** `_IO_FILE` is platform-specific. On macOS it must be `__sFILE`, and the spec gives no portable spelling.
- **Header order.** I'm assuming the compiler includes `stdio.h` before `jpeglib.h` because the stdio group comes first, which `jpeglib.h` needs.
- **Null handles.** I'm assuming a null handle from `fopen` begins no life, so the `missing` path does not abort at the end of `main` with an unconsumed handle. The spec only says this explicitly for transfers.
- **Cross-group parameter types.** I'm assuming a handle record from one extern group may be a parameter type in another group.

# context

Only `brief.md` and `spec.md` from this directory reached my context, plus the harness's automatic environment details: working directory, platform (darwin) and the user's email, which I did not use. I read no other files and ran nothing.
