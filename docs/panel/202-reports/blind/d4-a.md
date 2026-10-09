# Panel 202, llm-ergonomist (blind seat): folder `d4-a`

Copied by the coordinator on 2026-10-09 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 23:46:42 and ended 23:47:40 by `date`, on the second run (the first, at 23:31, stopped on the account's session limit with no report); `run.json`: cost USD and turns 0.2651 11. Its inputs in `docs/panel/202-briefs/blind/d4-a/`, the spec the trunk's at `65b78f2e`.

---

## files

`c.h` (changed; the only file changed or added), whole:

```c
static inline int twice(int x) { return x + x; }
```

`main.hero`, `left.hero` and `right.hero` are unchanged.

Diagnosis: `c.h` *defines* `int twice(int x)` with external linkage. Both
`left.hero` and `right.hero` have an `extern "c.h"` group, so each module's
translation unit includes the header. Each object file (`left-...o`,
`right-...o`) then carries its own global `_twice`, and the linker refuses the
duplicate. That is the error in `output.txt`. With `static inline`, every
translation unit gets a private copy with internal linkage, so no symbol
collides. `inline` also stops clang from warning about an unused static
function in any unit that does not call it. The signature (int in, int out)
is unchanged, so the `i32` parameter and result declared in both groups still
match the header as section 13 requires.

Output when built and run, unchanged from what the program means today:
```
6
8
```
(`left.left(x: 3)` = twice(3) = 6, `right.right(x: 4)` = twice(4) = 8.)

## choice_points

1. **Fix in C or fix in Heroes.** The other option was to leave `c.h` alone
   and move the `extern "c.h"` group into a single new module (for example
   `cdefs.hero`). `left.hero` and `right.hero` would then `use` it and call
   `cdefs.twice(x: x)`, so only one translation unit would include the header.
   I chose the header fix because it does not depend on something the spec
   leaves unsaid: whether the compiler includes a used module's headers in
   other translation units (for example main's or a caller's). If it does, the
   Heroes-only fix would reproduce the same duplicate symbol. If it does not,
   both fixes produce the same binary behaviour (prints 6 then 8).
2. **`static inline` vs plain `static`.** Plain `static` also links. The only
   risk is an unused-function warning in a unit that includes but does not
   call `twice`, which would matter only if the toolchain treats warnings as
   errors. `static inline` avoids that warning and behaves the same at run
   time.
3. **Declaring instead of defining.** Another option was to turn `c.h` into a
   prototype and add a `.c` file with the definition. I rejected it: the
   grammar's `link`/`package` names a library, not a source file, and nothing
   in the spec says a loose `.c` file in the directory gets compiled. That
   would most likely end in an undefined-symbol link error.

## confidence

High that this removes the duplicate-symbol link error and that the program
prints `6` and `8`. Medium-high overall: I could not run the build, and I am
assuming the compiler includes `c.h` textually in each module's unit, which
the `.o` names in `output.txt` strongly suggest.

## context

Apart from this directory's files (`brief.md`, `spec.md`, `output.txt`,
`main.hero`, `left.hero`, `right.hero`, `c.h`), my context received only the
harness's automatic environment information: the working directory path,
platform, date, and the user's account email address. None of it affected
the fix. I read no other files and used no tools besides reading and writing
files here.
