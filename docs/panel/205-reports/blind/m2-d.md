# program

Two files, both written in this directory.

`main.hero`:

```
# Prints whether sched_getcpu reported a CPU number.

extern "gnu_sched.h"
    function sched_getcpu() -> i32

function main()
    cpu = sched_getcpu()
    print(cpu >= 0)
```

`gnu_sched.h` (a helper header the program needs):

```c
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include <sched.h>
```

# choice_points

1. **How `_GNU_SOURCE` gets defined.** glibc declares `sched_getcpu` in
   `sched.h` only when `_GNU_SOURCE` is defined. Section 13 says headers are read
   in group order, with "one defining `_GNU_SOURCE` first", but it gives no
   syntax for defining a macro, so a header has to do it. I chose one local
   header, `gnu_sched.h`, that defines `_GNU_SOURCE` and then includes the
   system's `<sched.h>`. The function is still the one declared in the system
   `sched.h`. Other options:
   - `extern "sched.h"` alone. Unless the compiler defines `_GNU_SOURCE` itself,
     clang finds no declaration of `sched_getcpu`, so the binding is refused or
     the build fails.
   - Two groups: a first one on a header that only defines `_GNU_SOURCE`, then
     `extern "sched.h"`. The grammar lets a group have zero members, but a group
     with an empty indented body cannot be written. So the first group would
     need a member made up just to fill it. That gives the same result with
     more parts that could go wrong.
2. **Result type.** C returns `int`. Results are checked against the header
   and "may be wider than C's", so `i32` or `i64` would both be accepted. I
   chose `i32`, the exact width. With `i64` the program prints the same thing.
3. **No `link` clause.** `sched_getcpu` is in glibc's libc, which is always
   linked. Writing `link "c"` would probably do no harm, but the spec does not
   say it is needed.
4. **Binding the result before the test.** I wrote `cpu = sched_getcpu()` and
   then `print(cpu >= 0)`. Writing `print(sched_getcpu() >= 0)` would do the
   same thing. The literal `0` takes type `i32` from the other side (section
   2) either way.
5. **What counts as "did not return a CPU number".** glibc returns -1 on
   failure. I test `>= 0`, as the brief says (zero or more). Testing `!= -1`
   would only differ if some other negative value came back, which glibc does
   not do.

# confidence

I believe it builds and prints `true` on an ordinary Linux machine, but I
could not build or run it. The part I am least sure of is whether
`extern "gnu_sched.h"` finds a header in the program's directory. The spec
says that `use` paths start at the directory of the compiled file, but it
never says where an extern's header is looked up. If only system include paths
are searched, the build fails because the header is missing. If the toolchain
already defines `_GNU_SOURCE` itself, using `extern "sched.h"` directly would
avoid the question. The other parts (the `i32` result, `print` of a `bool`,
the `i32` comparison against a literal, no unused bindings) follow directly
from the spec.

# context

Nothing other than this directory's files reached my context, apart from the
harness's own system context (environment details and the account's email
address, which I did not use). I read only `brief.md` and `spec.md`, and wrote
`main.hero`, `gnu_sched.h` and `report.md`. I relied on general knowledge of
glibc (that `sched_getcpu` needs `_GNU_SOURCE` and returns -1 on failure), not
on any file.
