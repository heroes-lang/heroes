- [ ] **M-declared-extents golden ratification** | `tmpnam` is in the C standard and lives on both platforms. **Before reading: it wants a buffer. Would you expect macOS and Linux to agree on how big that buffer must be?** | `docs/measurements/035-not-one-byte-array-is-spelled-the-same-way-on-both.md` § The finding

    **Where to look after answering:** they do not agree, and not by a little.

    ```
    Darwin  _stdio.h:289   char *tmpnam(char *_LIBC_COUNT(L_tmpnam));
    glibc   stdio.h:211    extern char *tmpnam (char[L_tmpnam]);

    L_tmpnam = 1024 on Darwin,  20 on glibc      (compiled and run on each)
    ```

    Two things differ at once. **The spelling**: glibc writes an array and
    states the extent in the parameter, Darwin writes a plain pointer and puts
    the extent in an annotation. **And the number**: 1024 against 20.

    **Why it matters.** Route 6 would have an author mirror the header in front
    of them — `function tmpnam(s: i8[20])` from glibc — and the compiler would
    then CHECK that 20. The check is perfectly sound about the program and
    wrong about the world by a factor of 51 the moment the program is built on
    a Mac. A guard that is confidently wrong is worse than no guard.

    **What the language already does instead**, and it was run on both:

    ```
    extern "stdio.h"
        constant L_tmpnam: i64
    ```

    `spec § 13`: *a group's `constant` has no body — the header holds the
    value*. So the number is whatever the platform says, always.

    **The question to carry away.** Heroes declares parameters and fields *at
    the header's own width and sign*, and refuses one that disagrees. Ask why
    that rule is safe for `i32` against C's `int` and unsafe for `8` against
    `char[8]` — what is different about the two numbers?
