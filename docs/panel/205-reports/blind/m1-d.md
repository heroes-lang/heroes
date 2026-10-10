# program

One file, `main.hero`. No other file is needed.

```
extern "sched.h"
    function sched_getcpu() -> i32

function main()
    cpu = sched_getcpu()
    print(cpu >= 0)
```

# choice_points

1. **Library clause on the group.** `sched_getcpu` lives in glibc's libc, which every program links, so I wrote no `link`. Writing `link "c"` would most likely also build, because it just names libc again. Writing `package` would ask the system about a package that does not exist and fail.
2. **Result type.** C declares `int sched_getcpu(void)`, so I wrote `-> i32`. The spec says "a result may be wider than C's", so `-> i64` would also be accepted. The only difference is that the literal `0` in the comparison would then be an `i64` instead of an `i32`. The output would be the same.
3. **Header name.** I wrote `extern "sched.h"`, the system header named in the brief. The other option was to write a local wrapper header (for example `gnu_sched.h` containing `#define _GNU_SOURCE` followed by `#include <sched.h>`) and name that in the group. That would make the declaration visible even if the compiler does not define `_GNU_SOURCE`. But it would no longer bind straight from the system's `sched.h`, and the spec does not say whether a group's header is looked up in the program's directory. So I did not do it (see confidence).
4. **Binding vs inline call.** I bound the result to `cpu` and then compared it. Writing `print(sched_getcpu() >= 0)` would behave the same. The brief asks for exactly one call, and both forms make one.
5. **Literal type in `cpu >= 0`.** The `0` gets the type `i32` from its context (section 2), so the comparison has no mixed widths. Writing `0` against an `i64` result would also type-check. A typed mismatch, such as comparing against an `i64` binding, would be a compile error.
6. **Output form.** `print` on a `bool` writes `true` or `false` and one newline (section 11), which is exactly what the brief asks for. Converting the result to a string by hand would add nothing.

# confidence

I believe it builds and does what is asked. On success `sched_getcpu` returns a CPU number of 0 or more, and on failure it returns -1. So `cpu >= 0` prints `true` in the first case and `false` in the second. On an ordinary Linux machine it will print `true`.

What I am least sure of is feature macros. glibc declares `sched_getcpu` in `sched.h` only when `_GNU_SOURCE` is defined (through `bits/sched.h` under `__USE_GNU`). The spec says clang checks every result type against the header, but it does not say which feature macros the compiler defines when it reads the header. If Heroes does not define `_GNU_SOURCE`, the check would not find the declaration and the build would fail. The fix would be the wrapper header described in choice point 3, assuming a group can name a header in the program's directory. That is another thing the spec leaves open.

# context

Only `brief.md` and `spec.md` from this directory reached my context, plus the harness's standard system context (environment details and the user's account email, which I did not use). What I know about glibc's `sched_getcpu` signature, its -1 failure return and its `_GNU_SOURCE` guard comes from my general training knowledge, not from any file. I read no other file and ran nothing.
