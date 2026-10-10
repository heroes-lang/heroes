# program

```
extern "stdio.h"
    function puts(s: cstr) -> i32

extern "jpeglib.h" package "libjpeg"
    function jpeg_std_error(err: ptr) -> ptr

function main()
    print(1)
```

The only change is the order of the two `extern` groups: the `stdio.h` group now comes before the `jpeglib.h` group.

# choice_points

1. **How to give `jpeglib.h` a declaration of `FILE`.** clang refused the header at line 984 (`jpeg_stdio_dest(j_compress_ptr cinfo, FILE *outfile)`) because `FILE` was not declared. The tools' note says "a group's header is compiled as this program's own C includes it". libjpeg's header is known to need `<stdio.h>` included first; it does not include it itself. So I took the note to mean the groups' headers are included in source order, and I moved the `stdio.h` group above the `jpeglib.h` group.
   - Other choices: the note also suggests "repair the header", meaning write a local wrapper header that includes `stdio.h` and then `jpeglib.h`, and name that header in the group. The brief only lets me read and write files here, so I could have written such a header. But the specification never says how a group's header string is resolved, so I am not sure the tools would find a local file. The note's other suggestion, "name the one that declares this group's C", does not apply, because `jpeg_std_error` really is declared in `jpeglib.h`. Reordering is the smallest change, and it uses a header that is already in the program.
   - What the other order produces: the same `ffi_header_refused` error, if the tools do include headers in source order.
   - Section 4 says "Declaration order never matters". I read that as a rule about name visibility, not about the order C headers are included. If the tools do sort or ignore order, this fix does nothing (see confidence).

2. **What the program means.** `main` prints `1`. Neither extern function is called. I took the program's purpose to be building against stdio and libjpeg and printing `1`, so I kept both declarations and the body unchanged.
   - Other choice: deleting the libjpeg group would make the build succeed trivially, but it would drop the dependency the program evidently means to have.

3. **Declaring the extern signatures.** `jpeg_std_error` takes and returns `struct jpeg_error_mgr *`. I kept both as `ptr`, which section 13 allows ("what a `ptr` points at" is exempt from the width check). `puts` takes `const char *`, and I kept it as `cstr` without `lent`. That would only matter at a call site, and there is none.
   - Other choices: adding `lent` to `s` would change nothing here. Declaring `record jpeg_error_mgr` with all of its fields would be a lot of work for nothing, since it is never used.

# confidence

I am moderately confident that the program builds and prints `1`. What I am least sure of is whether the tools emit the groups' `#include` lines in source order. The note "as this program's own C includes it" points that way, but section 4's "Declaration order never matters" leaves room for doubt. If order is not kept, a wrapper header would be needed instead, and the specification does not say how such a header would be found.

I am also not sure whether an extern function that is never called counts as an unused binding. The specification limits that error to bindings and parameters, and the original output reported no such error. The original build stopped at the header, though, so later checks may never have run.

# context

Nothing outside this directory's files reached my context, apart from the harness's own system information (environment details and the user's account email), which I did not use. I relied on general knowledge that libjpeg's `jpeglib.h` expects `<stdio.h>` to be included before it.
