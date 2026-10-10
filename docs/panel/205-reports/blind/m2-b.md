# program

Two files, both in this directory.

`main.hero`:

```
# Asks glibc which CPU this thread runs on, and says whether it answered.

# glibc declares `sched_getcpu` only under `_GNU_SOURCE`, so this group
# comes first and defines it.
extern "gnu_source.h"
    constant GNU_SOURCE_ON: i64

extern "sched.h"
    function sched_getcpu() -> i64

function main()
    cpu = sched_getcpu()
    print(cpu >= 0)
```

`gnu_source.h` (a helper header the program needs):

```
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#define GNU_SOURCE_ON 1
```

Build: `heroes build main.hero -o main`. Output: one line, `true` or `false`.

# choice_points

1. **How to define `_GNU_SOURCE`.** glibc's `sched.h` declares `sched_getcpu`
   only when `_GNU_SOURCE` is defined. The spec (section 13) says headers are
   read in group order and that "one defining `_GNU_SOURCE` first", but gives no
   syntax for defining a macro. I chose a separate first group naming my own
   header `gnu_source.h`, which defines it. Other choices: (a) a single header of
   my own that defines `_GNU_SOURCE` and then includes `<sched.h>`, with one
   group naming it; this would also work, but the binding would no longer name
   the system's `sched.h` as the brief asks. (b) Not defining it at all; clang
   would then find no declaration of `sched_getcpu` and the header check would
   refuse the build.
2. **Giving the first group a member.** The `Extern` production requires
   `INDENT { Member } DEDENT`, and an empty body produces no INDENT, so a group
   with no members may not parse. I added `#define GNU_SOURCE_ON 1` to the helper
   header and declared `constant GNU_SOURCE_ON: i64` so the group has a body
   that clang can check. The other choice, a bare `extern "gnu_source.h"` line,
   might be accepted or might be a parse error; I avoided the risk. The constant
   is never used; the spec makes unused bindings and parameters errors, not
   unused declarations, so I expect no error.
3. **Result type of `sched_getcpu`.** C returns `int`. The spec says "a result
   may be wider than C's", so I declared `-> i64`, which lets the literal `0` in
   `cpu >= 0` take the default `i64`. Declaring `-> i32` would equally work (the
   literal would take `i32` from context) and print the same.
4. **No `link` clause.** `sched_getcpu` is in glibc's libc, linked by default.
   Adding `link "c"` should be harmless but redundant.
5. **Binding the result before printing.** I bound `cpu` and printed
   `cpu >= 0`; writing `print(sched_getcpu() >= 0)` directly is equivalent.
   Either way the function is called exactly once.
6. **What counts as "returned a CPU number".** I test `>= 0`; glibc returns -1
   on failure. Testing `!= -1` would agree on every value glibc actually returns.

# confidence

I believe it builds and prints `true` on an ordinary Linux machine with glibc
(where `sched_getcpu` succeeds), or `false` if it returns -1. I could not run
anything, so this has not been verified.

What I am least sure of:

- Whether my `_GNU_SOURCE` header really takes effect. It only works if the
  compiler's generated C reads `gnu_source.h` before any system header that
  includes `<features.h>`. The spec says groups are read in order and that the
  `_GNU_SOURCE` one goes first, which suggests this works, but if the toolchain
  includes its own system headers before the groups' headers, the macro would
  come too late and `sched_getcpu` would be undeclared.
- Whether `extern "gnu_source.h"` finds the header in this directory (I assume
  it is searched as a quoted include relative to the compiled file) and whether
  `extern "sched.h"` finds the system header.
- Whether a constant that is declared but never used is accepted (I read the
  spec as allowing it).

# context

Nothing beyond this directory's files reached my context, apart from the
harness's system prompt and an automatically attached account note (an email
address), which I did not use. I read only `brief.md` and `spec.md`, and
wrote `main.hero`, `gnu_source.h` and `report.md`. My knowledge that glibc
gates `sched_getcpu` behind `_GNU_SOURCE` and returns -1 on failure comes
from prior training, not from a file.
