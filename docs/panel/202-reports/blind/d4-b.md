# Panel 202, llm-ergonomist (blind seat): folder `d4-b`

Copied by the coordinator on 2026-10-09 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 23:46:42 and ended 23:47:24 by `date`, on the second run (the first, at 23:31, stopped on the account's session limit with no report); `run.json`: cost USD and turns 0.212 10. Its inputs in `docs/panel/202-briefs/blind/d4-b/`, the spec the trunk's at `65b78f2e`.

---

# files

`c.h` (changed), whole:

```c
static inline int twice(int x) { return x + x; }
```

No other file changed or added. `main.hero`, `left.hero` and `right.hero` stay as they were.

Diagnosis: `c.h` holds a full definition of `twice` with external linkage, not just a declaration. Both `left.hero` and `right.hero` name `c.h` in their `extern` group, and each module is compiled as its own C translation unit (`left-...o` and `right-...o` in `output.txt`). So each object file defined the global symbol `_twice`, and the linker refused the second copy. With `static inline`, each translation unit gets its own private copy, so no global symbol clashes. The signature `int twice(int)` is unchanged, so the clang check of `function twice(x: i32) -> i32` against the header still passes.

Expected output when run (unchanged from what the program means to print): `left.left(x: 3)` returns `twice(3)`, which is 6, and `right.right(x: 4)` returns `twice(4)`, which is 8:

```
6
8
```

# choice_points

- **Where to fix: the header or the Heroes modules.** I made the C definition `static inline` in `c.h`. The other option was to keep `c.h` as it was and put the `extern "c.h"` group in only one module, for example a new `twice.hero` with a wrapper function that `left.hero` and `right.hero` both `use`. That also leaves a single translation unit defining `_twice`, so it would link and print the same `6` and `8`. However, it changes three `.hero` files and adds a fourth. It also stays fragile: any future module that names `c.h` would bring the duplicate symbol back. The header fix removes the cause and touches one line.
- **`static inline` versus `static` alone.** Both give internal linkage and fix the link. `static` alone could raise an unused-function warning in a translation unit that does not call it. `static inline` does not, so I chose it. The program output is the same either way.
- **Declaration-only header plus a separate `.c` file.** This was the third option: reduce `c.h` to `int twice(int x);` and define the function in a `.c` file. The specification gives no way to add a C source file to the build (only `link` a library or `package`), so this would fail at link time with an undefined `_twice`. I rejected it.

# confidence

High that the duplicate-symbol error is gone. The error names exactly `_twice` in the two module objects, and internal linkage removes exactly that clash. Medium-high that the build then succeeds with nothing else wrong, since I cannot run the tool. The `.hero` files look valid against the specification: `x: 3` takes `i32` from context, `print` takes an `i32`, and each `use` is matched by a qualified call. The remaining risk is that the Heroes toolchain treats a `static` function in a header differently in some way the specification does not describe. Nothing in the specification suggests it does.

# context

Only this directory's files reached my context: `brief.md`, `spec.md`, `output.txt`, `main.hero`, `left.hero`, `right.hero` and `c.h`. The harness also added an automatic note with the user's account email, which I did not use. I read no other files and ran nothing.
