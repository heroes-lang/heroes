# program

Two files, both in this directory.

`main.hero`:

```
extern "gnu_sched.h"
    function sched_getcpu() -> i32

function main()
    cpu = sched_getcpu()
    print(cpu >= 0)
```

`gnu_sched.h` (needed because glibc declares `sched_getcpu` only when `_GNU_SOURCE` is defined):

```
#define _GNU_SOURCE
#include <sched.h>
```

# choice_points

1. **How to get `_GNU_SOURCE` defined.** glibc's `sched.h` declares `sched_getcpu` only under `_GNU_SOURCE`. The spec says a group "defining `_GNU_SOURCE`" must come first, but it gives no way to write a `#define` in Heroes, so some header has to do it. I wrote one small wrapper header that defines `_GNU_SOURCE` and then includes the system `<sched.h>`, and named that header in the only group. The function still comes from the system's `sched.h`. The other way was two groups: a first `extern "gnu_source.h"` holding only the define, then `extern "sched.h"`. The grammar seems to rule that out, though. `Member` may repeat zero times, but a group with no members has no indented line to produce the INDENT/DEDENT pair. To make it work I would have had to add a dummy member, such as a constant defined in my header, and the spec does not say whether an unused constant is an error. If that version built, it would behave the same as mine.
2. **No `_GNU_SOURCE` at all.** If I named `sched.h` directly with no define, clang would not find `sched_getcpu` in the header. The build would then fail, either in the header check or as an implicit declaration.
3. **Result type.** C returns `int`. I declared `-> i32`, matching C's width. The spec says a result "may be wider than C's", so `-> i64` would also be accepted and would behave the same.
4. **No `link` clause.** `sched_getcpu` is in glibc's libc, which is always linked. Writing `link "c"` would most likely be harmless. Leaving out the define, by contrast, would break the build.
5. **What counts as success.** `sched_getcpu` returns -1 on error and a CPU number, zero or more, on success. The test is `cpu >= 0`, so `print` gets a `bool` and writes `true` or `false`. Testing `cpu != -1` would give the same result with glibc. The literal `0` takes the type `i32` from the comparison (section 2).
6. **Binding versus inline.** I bound the result to `cpu` and then read it. Writing `print(sched_getcpu() >= 0)` would be the same program. Either way the value is used, so the rule against discarding a value does not come into play.

# confidence

I'm fairly confident the Heroes source is valid: the declaration, the binding, the comparison and `print` of a `bool` all follow the spec directly. I could not run anything, so this is unverified. The parts I'm least sure of:

- **Finding the header.** The spec does not say where the compiler looks for a header named in a group, or whether it includes it as `"..."` or `<...>`. If it searches only system paths, or looks relative to a generated C file somewhere else, `gnu_sched.h` will not be found and the build fails.
- **Include order.** `_GNU_SOURCE` works only if it is defined before any glibc header is read. If the generated C includes system headers before the group headers, such as `stdint.h`, then `features.h` has already been processed, `sched_getcpu` stays hidden, and the build fails. The spec's own advice to put the `_GNU_SOURCE` group first suggests the compiler includes group headers before anything else, but that is an inference.

If both of those hold, it builds and prints `true` on an ordinary Linux machine. It would print `false` only if the call failed.

# context

Only this directory's files reached my context: `brief.md` and `spec.md`. The harness also added the user's account email and environment details (working directory, platform, date). None of that was used in the program. I read no other files and ran nothing.
