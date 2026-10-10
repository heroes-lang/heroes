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

1. **How to name `FILE` as a handle.** The spec says a handle is `record X tag T`, meaning `T *`, and that `tag` gives the name the header writes after the word `struct`. `FILE` is a typedef. Its struct tag differs by platform (`_IO_FILE` on glibc, `__sFILE` on macOS). I chose `tag FILE`, on the reading that the tag names the C type the pointer points to. That keeps it the same on every platform, and it matches the spec's own example (`sqlite3` is both the typedef and the struct tag). Writing `tag _IO_FILE` would tie the program to glibc and fail on macOS, and `tag __sFILE` would do the reverse. Using `tag void` would declare `void *` where the header says `FILE *`, which clang's result-type check would probably refuse.
2. **One `File` record shared by both groups.** The spec forbids two records naming one tag (except `void`), so `jpeg_stdio_src` uses the stdio group's `File` and does not declare its own. Its group comes after the stdio group, so C sees `FILE` and `size_t` before `jpeglib.h`, which needs them. Putting the jpeg group first would break the header order.
3. **`cinfo` type.** I declared a handle `record Decompress tag jpeg_decompress_struct`, which is `struct jpeg_decompress_struct *`, the same as `j_decompress_ptr`. A plain `ptr` would also pass, since the width check does not apply to what a `ptr` points at, but it would accept any pointer.
4. **Lifetime marks.** `fopen` has `acquires fclose` and `fclose` has `consumes`, so the compiler tracks the handle and requires it to be closed. Leaving the marks off would build, but the compiler would not check that the file gets closed. `fclose` takes no `when 0`: C's stream is released whether or not `fclose` succeeds, so with `when 0` a failed close would leave the program owing a life it can no longer end. `jpeg_stdio_src`'s `infile` has no mark and is not `lent`, because libjpeg keeps the `FILE *` in its source manager. Marking it `lent` would claim C does not keep it, which is false.
5. **`lent` on the `fopen` strings.** Both are `lent` because `fopen` does not keep its path or mode, and `.cstr()` lends only to a `lent` parameter. Without `lent`, the calls with `.cstr()` would be refused, and I would need a lease plus `end_lease`.
6. **Result widths.** `fclose` returns `i32`, C's `int` exactly. `i64` would also be accepted, because a result may be wider. The parameter and result names (`path`, `mode`, `stream`, `cinfo`, `infile`) are my own; the spec does not tie them to the header's names. Because `fopen`'s two parameters share the type `cstr`, the call must use named arguments, so the call says `path:` and `mode:`.
7. **Discarding `fclose`'s result.** `_ = fclose(f)` drops the `i32` on purpose, which is allowed because it is not fallible. Binding it to a name and not using it would be a compile error. Checking it would mean printing extra output the task does not ask for.
8. **Testing for a null `FILE *`.** I wrote `f == nullptr`, since a handle's `==` compares addresses. A `match` would not work here because the result is not a `T?`.

# confidence

I believe it builds and behaves as asked. With `photo.jpg` present it prints `opened` and closes the file. Without it, it prints `missing` and calls nothing else. I could not run anything, so this is based on reading the spec only.

What I am least sure of:
- **`tag FILE` (choice 1).** If the compiler insists on the literal struct tag, this record is refused and the program has to be per-platform.
- **A null result from `fopen`.** The spec says a transfer into a null handle is not made, but it does not say outright that a null result from an `acquires` call starts no life. If it did start one, the `missing` path would abort when `main` returns because of an unconsumed handle. In that case there would be no correct fix, since passing null to `fclose` is undefined in C.
- **Using `File` from another group.** I assumed a parameter of the jpeg group may use a record declared in the stdio group. The spec restricts only fields to "another record of the group".

# context

Nothing beyond this directory's `brief.md` and `spec.md` reached my context, apart from the harness's own environment notes (the working directory, platform and the user's account email), which I did not use. My knowledge of the C headers (`fopen`, `fclose`, `FILE`, `jpeg_stdio_src`, `j_decompress_ptr`) comes from general training knowledge, not from any file I read.
