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

`jpeg_stdio.h` (new file, written in this directory next to `main.hero`):

```c
/* jpeglib.h uses FILE and size_t without including stdio.h itself,
   so every includer must include stdio.h first. */
#ifndef JPEG_STDIO_H
#define JPEG_STDIO_H
#include <stdio.h>
#include <jpeglib.h>
#endif
```

What it evidently means: declare a handle for C's `FILE *` and one libjpeg function (`jpeg_std_error`), link libjpeg through its package, and print `1`. Built, it prints `1` and exits.

# choice_points

1. **How to answer `ffi_header_refused`.** The tool said clang refuses `jpeglib.h` at line 984 because `FILE` is unknown, and offered two fixes: "repair the header, or name the one that declares this group's C". libjpeg's `jpeglib.h` is known not to include `<stdio.h>` itself; its users have to include it first. The `stdio.h` group already sits above the jpeg group, and the error still happened, so the order of groups does not decide what clang sees before `jpeglib.h`. The system header is outside this directory and I cannot change it. So I took the second fix: a small wrapper header, `jpeg_stdio.h`, that includes `<stdio.h>` and then `<jpeglib.h>`, and the jpeg group names it. `jpeg_std_error` is declared in it (through `jpeglib.h`), so clang can check the declaration. The other choices: editing the system `jpeglib.h` would break the "inputs here only" rule and every other user of that header. Removing the group, or moving `jpeg_std_error` into the `stdio.h` group, would fail, because `stdio.h` does not declare it and clang checks every member against its group's header.
2. **Name of the wrapper.** I called it `jpeg_stdio.h` and not `jpeglib.h`. A local `jpeglib.h` would shadow the system one under a quoted include, and its own `#include <jpeglib.h>` could find itself again (or, depending on search paths, not find the real one). Any other distinct name works the same way.
3. **Where the wrapper is found.** The spec says nothing about how a group's header string is looked up. I assumed a quoted name is looked up as C does for `#include "..."`, starting from the directory of the compiled file (like `use` paths, which "start at the directory of the file you compile"). If the tool only searches system include paths, the header would not be found. The fallback would then be to put the same wrapper somewhere on the include path, or to pass that path to the build.
4. **`package "libjpeg"` kept.** `libjpeg` is the pkg-config name that libjpeg and libjpeg-turbo install. The tool reported no trouble with it (it got as far as compiling the header). Using `link "jpeg"` instead would link `-ljpeg` directly without asking the system for include and library paths.
5. **`record CFile tag FILE` kept.** `FILE` is a typedef and not a struct tag. Going by the spec's own example (`record Db tag sqlite3` is `sqlite3 *`, where `sqlite3` is also a typedef name), `tag FILE` makes the handle `FILE *`. The tool did not object to it. The alternative, `tag __sFILE` (macOS) or `tag _IO_FILE` (glibc), would tie the program to one libc.
6. **Unused declarations kept.** `CFile` and `jpeg_std_error` are never used. The spec's unused-name error covers bindings and parameters, not declarations, so I left them in rather than change what the program declares. Deleting the jpeg group would also have made the build pass, but it would drop what the program evidently sets out to do.
7. **Signature of `jpeg_std_error`.** C writes `struct jpeg_error_mgr * jpeg_std_error(struct jpeg_error_mgr *err)`. The parameter is `ptr`, which is allowed ("except ... what a `ptr` points at"), and so is a `ptr` result. I kept it. A partial `record` for `jpeg_error_mgr` would be more precise but is not needed for this program.

# confidence

I'm fairly confident it builds and prints `1`. With `<stdio.h>` included first, `jpeglib.h` compiles; this is the standard documented requirement of libjpeg. The other declarations match the header. What I'm least sure of is choice point 3: whether the tool finds a header named by a relative quoted string in the program's directory. The spec never says how header names are looked up. After that, choice point 5: whether `tag FILE` is accepted for a typedef'd struct. The tool stopped at the first error, so it may not have checked the `stdio.h` group yet. I could not run anything to check either point.

# context

Nothing outside this directory's files reached my context, apart from the harness's own system information (environment details, and an account email that I did not use). My reasoning also drew on general knowledge: libjpeg's `jpeglib.h` needs `<stdio.h>` first, and how C include lookup and pkg-config names work. That knowledge did not come from any file.
