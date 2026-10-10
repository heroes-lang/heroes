# program

`main.hero`:

```
extern "stdio.h"
    record CFile tag FILE

extern "jpeg_stdio.h" package "libjpeg"
    function jpeg_std_error(err: ptr) -> ptr

function main()
    print(1)
```

`jpeg_stdio.h` (new, in this directory, next to `main.hero`):

```c
/* jpeglib.h uses FILE without including stdio.h; include it first. */
#include <stdio.h>
#include <jpeglib.h>
```

The program declares libjpeg's `jpeg_std_error` and a `FILE *` handle, and when run it prints `1`.

# choice_points

1. **How to fix the refused header.** The build failed because `jpeglib.h` uses `FILE` (line 984) without including `stdio.h` itself. libjpeg expects every includer to include `stdio.h` first. The tool's note says each group's header is compiled on its own, so the separate `stdio.h` group above it does not help. The note offers two fixes: repair the header, or name a header that declares the group's C. I chose to name a new header, `jpeg_stdio.h`. It includes `<stdio.h>` and then `<jpeglib.h>`, and the group names it with `package "libjpeg"` so the libjpeg include and link flags still apply. Other options:
   - Editing the installed `jpeglib.h` is outside this directory and would affect the whole system.
   - Dropping the jpeg group would build, but it would lose what the program declares.
   - Moving or merging groups changes nothing, because each header is compiled independently.
2. **Where the wrapper header is looked up.** The spec says module `use` paths start at the directory of the compiled file, but it says nothing about where an `extern` header string is searched. I put `jpeg_stdio.h` next to `main.hero` and named it by its bare file name, assuming a quoted include finds it there. If the tools only search system and package include paths, the build would report this header as not found instead.
3. **Inside the wrapper, `<jpeglib.h>` versus `"jpeglib.h"`.** I used angle brackets so the lookup goes through the system and `libjpeg` package include paths and cannot loop back to a local file. Quotes would also work as long as no file of that name exists here.
4. **The `stdio.h` group and `jpeg_std_error`'s signature.** I left both unchanged. `record CFile tag FILE` is a handle (`FILE *`). `jpeg_std_error(err: ptr) -> ptr` matches C's `struct jpeg_error_mgr *jpeg_std_error(struct jpeg_error_mgr *)`, since the spec allows what a `ptr` points at to differ. Neither is used by `main`. The spec makes unused bindings and parameters errors, but it does not say that about extern declarations, and the reported error was not about them. If unused extern members were refused, the fix would be to remove them, but nothing in the output suggests that.

# confidence

I'm fairly confident the header error is fixed: `FILE` is now declared before `jpeglib.h` is read, which is exactly what clang complained about. Once it builds, the program prints `1`. I'm least sure of two things:
- whether the tools find a header in the program's own directory (choice point 2);
- whether `tag FILE` is accepted, since `FILE` is a typedef name and not a name written after `struct` (on macOS the struct is `__sFILE`). The tools reported no error for that group, but they may have stopped at the first error.

I could not run the build.

# context

Nothing reached my context except this directory's files (`brief.md`, `spec.md`, `main.hero`, `output.txt`) and the session's environment information (working directory, platform, date, and an account email that I did not use).
