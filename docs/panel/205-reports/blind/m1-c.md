# program

Two files, both in this directory.

`main.hero`:

```
# `sched_getcpu` from glibc's `sched.h`, reached through `getcpu.h`, which
# defines `_GNU_SOURCE` first so that the header declares it.
extern "getcpu.h"
    function sched_getcpu() -> i32

function main()
    cpu = sched_getcpu()
    print(cpu >= 0)
```

`getcpu.h`:

```c
/* glibc declares sched_getcpu in sched.h only under _GNU_SOURCE. */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <sched.h>
```

Build: `heroes build main.hero -o main`. It prints `true` when `sched_getcpu`
returns 0 or more and `false` when it returns -1 (its failure value).

# choice_points

1. **Which header the group names.** glibc declares `sched_getcpu` in
   `sched.h` only when `__USE_GNU` is set, which means `_GNU_SOURCE` has to be
   defined before the first glibc header is read. The spec has no way to define
   a macro, so I wrote the small wrapper header `getcpu.h` that defines
   `_GNU_SOURCE` and then includes the system's `<sched.h>`. The group names
   that wrapper. The function still comes from the system's `sched.h`.
   The other choice was `extern "sched.h"` with no wrapper. That works only if
   the compiler's C context already defines `_GNU_SOURCE` (for example by
   compiling as C++, where g++ and clang++ define it). Under plain C, clang
   would find no declaration of `sched_getcpu`, and the build would fail at
   the header check.
2. **Result type.** C returns `int`. The spec says a result "may be wider than
   C's", so either `i32` or `i64` is allowed. I chose `i32` to match C exactly.
   With `i64` the program would behave the same, because the literal `0` takes
   its type from the comparison either way.
3. **`link` / `package`.** `sched_getcpu` is in glibc's libc, which is always
   linked, so I wrote neither. Writing `link "c"` would most likely change
   nothing.
4. **Binding the result before printing.** I bound the result to `cpu` and then
   printed `cpu >= 0`. `print(sched_getcpu() >= 0)` would do the same. The spec
   says the function is called once, and both versions call it once.
5. **Output.** `print` of a `bool` writes `true` or `false` followed by one
   newline, which matches what was asked. Using `if` with two `print` calls
   would print the same thing.
6. **Doc comment.** The comment above `extern` is optional. Leaving it out would
   not change the build.

# confidence

I think the Heroes source is valid and does what is asked. It has a single
`extern` group with one function that takes no parameters (so the width rules
for parameters don't come into play), an `i32` result checked against C's
`int`, an immutable binding that gets used, and a `bool` passed to `print`.

What I am least sure of is how the header is found and processed, which the
spec does not cover:
- **Lookup path.** I am assuming `extern "getcpu.h"` finds a header next to
  `main.hero`. The spec says `use` paths start at the compiled file's
  directory, but it says nothing about where header lookup starts. If the
  compiler does not search the source directory, the build fails with
  "header not found".
- **Order of includes.** The wrapper only works if nothing has already pulled
  in `<features.h>` without `_GNU_SOURCE`. If the generated C includes a system
  header (for example `stdio.h` or `stdint.h`) before the groups' headers,
  `__USE_GNU` stays unset and `sched_getcpu` is not declared. The spec says C
  reads a module's headers in the order the groups are written, but not what
  comes before them.

If either problem happens, the fallback is `extern "sched.h"` directly, which
depends on the toolchain defining `_GNU_SOURCE` itself. I could not run
anything, so none of this has been tested.

# context

Nothing outside this directory reached my context except the harness's system
context. That context held the session environment details (working
directory, platform, date, model) and the user's account email, and none of it
was used. I read only `brief.md` and `spec.md`. The facts about glibc
(`sched_getcpu` needing `_GNU_SOURCE`, returning -1 on failure, living in
libc) come from my own prior knowledge, not from any file.
