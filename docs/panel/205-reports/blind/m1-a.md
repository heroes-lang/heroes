# program

One file, `main.hero`:

```
extern "sched.h"
    function sched_getcpu() -> i64

function main()
    cpu = sched_getcpu()
    print(cpu >= 0)
```

No other file was written besides this report.

# choice_points

1. **Header string.** Section 13 shows a system header written bare, `extern "sqlite3.h"`, so I wrote `extern "sched.h"`. The other choice, a local wrapper header (for example one that defines `_GNU_SOURCE` and then includes `<sched.h>`), would no longer bind straight from the system's `sched.h` as the brief asks, and the spec does not say where such a quoted path is looked up.
2. **No `link` or `package`.** `sched_getcpu` is in glibc's libc, which every program links anyway, and section 13 says `link` is needed only "when the symbols need one". Writing `link "c"` should also build, but it adds nothing.
3. **Result type.** C returns `int`. Section 13 says "a result may be wider than C's", so I declared `-> i64`. That lets me compare it against an untyped literal, which defaults to `i64`. The other choice, `-> i32`, also builds: the literal `0` would take `i32` from context and the program would behave the same.
4. **What counts as "returned a CPU number".** glibc returns -1 on failure (and sets errno). I print the result of `cpu >= 0`, which matches "zero or more" exactly. Writing `cpu != -1` would differ only if the function returned some other negative value, which glibc never does.
5. **Binding the result first.** I bound `cpu` and then printed `cpu >= 0`. I could have written `print(sched_getcpu() >= 0)` directly. Both call the function exactly once and produce the same output.
6. **Output.** `print` of a `bool` writes `true` or `false` plus one newline (section 11). Printing a string literal from inside an `if` would give the same text, with more code.

# confidence

I am fairly confident that the program is correct Heroes and does what the brief asks. The syntax follows the grammar: an `Extern` with one `Member`, a `function main()` that takes nothing, an immutable binding that is read (so it is used), and `print` of a `bool`. It calls the function once and prints `true` for a CPU number of zero or more, `false` otherwise.

The part I am least sure of is whether `sched_getcpu` is visible when clang reads `sched.h`. glibc declares it only under `__USE_GNU`, which means `_GNU_SOURCE` must be defined. The spec does not say whether the Heroes toolchain defines it when it reads a group's header. If it does not, clang would not find the declaration and the build would fail. I kept the direct binding because the brief asks for the system's `sched.h`. If the build fails this way, the fallback is a local wrapper header that defines `_GNU_SOURCE` before including `<sched.h>`. I could not test any of this, because I cannot run anything.

# context

Only this directory's files reached my context: `brief.md` and `spec.md`. The harness also attached the user's email address and the environment details (paths, platform, date) automatically. I did not use them for the program. No other files, web content or tool output reached me.
