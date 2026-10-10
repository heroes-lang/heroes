# program

```
extern "stdio.h"
    function puts(s: cstr) -> i32

extern "jpeglib.h" package "libjpeg"
    function jpeg_std_error(err: ptr) -> ptr

function main()
    print(1)
```

The only change is the order of the two `extern` groups. The `stdio.h` group now comes before the `jpeglib.h` group.

# choice_points

1. **How to fix the refused header.** The tools reported that `jpeglib.h` does not compile at line 984 (`unknown type name 'FILE'`). libjpeg's header expects the includer to have included `<stdio.h>` first. The note in `output.txt` says a group's header "is compiled as this program's own C includes it". I read that as: the headers are included in the order the groups appear in the program. So I moved the `stdio.h` group above the `jpeglib.h` group, which puts `FILE` in scope before `jpeglib.h` needs it.
   - Alternative A: delete the `jpeglib.h` group. The program would print `1` exactly the same, since nothing calls `jpeg_std_error`. But it would drop the libjpeg binding the program evidently meant to declare, and the dependency on the `libjpeg` package along with it.
   - Alternative B: the note's "name the one that declares this group's C" could mean naming some other header for the jpeg group. No header in this directory would do that, and any other header would still be libjpeg's own, so I did not take this route.
2. **The signature of `jpeg_std_error`.** C declares `struct jpeg_error_mgr *jpeg_std_error(struct jpeg_error_mgr *err)`. Section 13 exempts "what a `ptr` points at" from the width rule, and it allows a result to be a `ptr`, so I kept `err: ptr` and `-> ptr` unchanged. The alternative would be to declare a `record ... tag jpeg_error_mgr` with all its fields. That would bring in function-pointer fields, which a group's record cannot hold except through `partial`. It would add risk and no behaviour.
3. **The unused extern functions (`puts`, `jpeg_std_error`).** The spec's unused-name rule covers bindings and parameters, not extern members. The original build also got past them, failing only on the header. So I left both in place and did not add calls.
4. **Program behaviour.** `main` prints `1`, followed by a newline. I kept that as the evident meaning.

# confidence

I am fairly confident it builds and prints `1`. What I am least sure of is whether the tools really include the group headers in source order. The spec never says this outright. My reading comes from the note "as this program's own C includes it", together with the fact that the first group was the one refused. If the order is fixed some other way, for example alphabetically or jpeg first regardless, the reorder changes nothing and only Alternative A (dropping the jpeg group) would build. A smaller doubt is whether `package "libjpeg"` resolves on the machine. The original error got as far as compiling the header, which suggests the package was found.

# context

Only this directory's files reached my context: `brief.md`, `spec.md`, `main.hero` and `output.txt`. On top of those, the harness supplied its usual environment details: working directory, platform, date, and an account email that I did not use. What I said about libjpeg's header expecting `<stdio.h>` first and about the C signature of `jpeg_std_error` comes from my general knowledge, not from any file I read.
