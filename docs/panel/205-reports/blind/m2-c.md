# program

Two files, both in this directory.

`main.hero`:

```
extern "cpu.h"
    function sched_getcpu() -> i32

function main()
    cpu = sched_getcpu()
    print(cpu >= 0)
```

`cpu.h` (a helper header the program needs):

```c
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include <sched.h>
```

# choice_points

1. **How to get `_GNU_SOURCE` defined.** glibc declares `sched_getcpu` in
   `sched.h` only when `_GNU_SOURCE` is defined. The spec (section 13) says
   that a group defining `_GNU_SOURCE` comes first, but it never says how a
   group defines it, and a group cannot be empty: `INDENT { Member } DEDENT`
   needs at least one indented line. So I wrote my own header, `cpu.h`. It
   defines `_GNU_SOURCE` and then includes `<sched.h>`, and one group names
   it. The `#ifndef` guard avoids a redefinition warning in case the
   toolchain already passes `-D_GNU_SOURCE`.
   - Alternative A: `extern "sched.h"` alone. Without `_GNU_SOURCE`, clang
     would find no declaration of `sched_getcpu` to check against, so the
     build would most likely be refused.
   - Alternative B: two groups, a `gnu.h` that only defines `_GNU_SOURCE`
     followed by `extern "sched.h"`. The first group would need some member
     to be well formed, for example `constant _GNU_SOURCE: i64`. That name
     clashes with the SCREAMING_CASE rule because of its leading underscore,
     so I avoided this option.
2. **Result type.** C returns `int`. The spec says a result may be wider
   than C's, so `i32` and `i64` would both be accepted. I chose `i32`, which
   matches C exactly. With `i64` the comparison literal would simply become
   an `i64`, and the output would be the same.
3. **No `link` clause.** `sched_getcpu` is in glibc's libc, which is always
   linked. Writing `link "c"` would most likely be harmless but redundant.
4. **Binding then printing, versus `print(sched_getcpu() >= 0)` inline.**
   Both satisfy the rules and behave the same: one call, prints `true` or
   `false`. The named binding is used, so it is not an unused binding.
5. **The test.** "Returned a CPU number (zero or more)" maps to `cpu >= 0`.
   On failure glibc returns -1, which prints `false`. The literal `0` takes
   `i32` from the other side of the comparison (section 2).

# confidence

I believe the program builds and prints `true` on an ordinary Linux machine.
Prints `false` only if the call fails, for example on a kernel without
getcpu support.

What I am least sure of is how the compiler resolves the quoted header name
`"cpu.h"`. If it generates C in another directory and does not add the
source file's directory to the include path, `cpu.h` will not be found. The
spec says `use` paths start at the directory of the compiled file, and the
brief invites writing extra files here, so I expect the header is looked up
there too. The spec does not say this outright, though. A smaller doubt is
whether the compiler pulls in its own system headers before mine. If it did,
defining `_GNU_SOURCE` inside `cpu.h` would come too late. The spec's
statement that the module's headers are read in group order, with the
`_GNU_SOURCE` one first, suggests it does not.

# context

Only this directory's files reached my context: `brief.md` and `spec.md`,
plus the environment information the harness supplies (working directory,
platform, date, and an account email that I did not use). I read no other
files and ran nothing.
