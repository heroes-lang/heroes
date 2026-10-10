# Panel 202, llm-ergonomist (blind seat): folder `d1-b`

Copied by the coordinator on 2026-10-09 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 23:46:42 and ended 23:47:37 by `date`, on the second run (the first, at 23:31, stopped on the account's session limit with no report); `run.json`: cost USD and turns 0.2428 9. Its inputs in `docs/panel/202-briefs/blind/d1-b/`, the spec the trunk's at `65b78f2e`.

---

# Report

Program: `main.hero` uses two modules, `left` and `right`. Each declares an
`extern` group with a C function named `twice`, from different headers and with
different signatures:

- `left.hero`: `extern "a.h"` with `function twice(x: i64) -> i64`; `a.h` says
  `static inline long twice(long x) { return 2 * x; }`.
- `right.hero`: `extern "b.h"` with `function twice(x: i32) -> i32`; `b.h` says
  `static inline int twice(int x) { return x + x; }`.

`main` prints `left.left(x: 3)` and `right.right(x: 4)`.

## check

Exit code: 0. Output: nothing.

Why:
- `use left` and `use right` bind the two modules, and `main.hero` calls them
  qualified (`left.left(...)`, `right.right(...)`): "`use geom` binds `geom` to
  `geom.hero`'s declarations, written qualified". Nothing else in `main.hero`
  takes the name `left` or `right`, so "a `use` binds its name for the whole
  file, so nothing else in the file may take it" is respected. Inside
  `left.hero` there is no `use left`, so a function named `left` there shadows
  nothing.
- Each `twice` is a declaration of its own module, reached unqualified only
  inside that module. Nothing in the specification forbids two modules from
  each declaring an extern function of the same C name, and "Every module you
  name needs its own `use`" means neither module sees the other's `twice`.
- Widths agree with the headers: "A **parameter** and a **field** are declared
  at the header's own width and sign: `i32` where C says int". On darwin (LP64,
  both arm64 and x86_64) C's `long` is 64-bit signed, so `x: i64` and the `i64`
  result match `long`; `x: i32` and the `i32` result match `int`. "clang checks
  every result type, constant and record field against that header" passes.
- No `link` is needed: "and `link` a library when the symbols need one"; both
  functions are `static inline` in their headers.
- Literals: "A literal takes the type its context asks for", so `3` is an `i64`
  (parameter of `left.left`) and `4` is an `i32` (parameter of `right.right`).
  The named arguments `x:` are allowed by `Arg = [ ident ":" ] ...`.
- `print` "takes ... a number", so `print` of an `i64` and of an `i32` is fine;
  both lines of `main` are `()` calls standing alone.
- No `???` holes, every parameter is read, every function with `->` returns.

The specification says nothing of what a successful `check` prints; I take it to
print nothing.

## build_and_run

`heroes build main.hero -o main`: exit code 0, no output; it produces `./main`.

`./main`: exit code 0, output:

```
6
8
```

Why:
- `left.left(x: 3)` calls `twice(3)` from `a.h`: `2 * 3` as C `long` is `6`.
- `right.right(x: 4)` calls `twice(4)` from `b.h`: `4 + 4` as C `int` is `8`.
- "`print` writes its values with no separator and exactly one trailing
  newline", so each call prints one number on its own line.
- No overflow is near (C facts: `long` is 64-bit on darwin, `int` 32-bit), and
  `main` returning normally ends with exit 0. No leases or handles exist, so the
  "aborts when `main` returns" rule does not apply.
- C fact: a `static inline` function has internal linkage, so the two
  definitions named `twice` do not collide at link time even when both reach
  the final binary, as long as each is compiled in a translation unit that
  includes only its own header (see choice_points).

## test

Exit code: 0. Output: no test results, since the program has no `test` blocks
(possibly a summary line reporting zero tests; the specification gives no
format).

Why: "`test` blocks run only when asked for"; none of `main.hero`,
`left.hero`, `right.hero` holds a `test` declaration, so there is nothing to run
and nothing to fail. `main` is not run by `heroes test`.

## choice_points

1. Two modules each binding a C function of the same name (`twice`) from
   different headers with different signatures. The specification makes
   module names qualified and does not forbid this, and both C definitions are
   `static inline` (internal linkage). I chose: it is accepted, and each group
   is checked and compiled against its own header. The other reading: if the
   implementation emits one C translation unit including both `a.h` and `b.h`,
   clang reports a redefinition of `twice` with conflicting types (`long(long)`
   vs `int(int)`); then `check` (which runs clang's header check) and `build`
   would exit nonzero with that diagnostic, no binary is produced, `./main`
   does not exist (the shell reports it not found, exit 127), and `test` fails
   to compile the same way, exit nonzero.
2. What `check` and `build` print on success: unspecified; I chose nothing.
3. What `heroes test` prints with zero tests: unspecified; I chose no output
   (or at most a zero-test summary) and exit 0.
4. The width of C's `long`: I took the platform here (darwin, LP64), where it is
   64 bits and matches `i64`. On an LLP64 target (Windows), `long` is 32 bits and
   `x: i64` would be refused as a parameter that disagrees with the header,
   making `check` fail.

## confidence

Moderate. The values `6` and `8` are certain if the program compiles. The main
risk is choice point 1, whether the toolchain tolerates two same-named C
functions from different headers in two modules; I put about 70% on acceptance.

## context

Only this directory's files reached my context: `brief.md`, `spec.md`,
`main.hero`, `left.hero`, `right.hero`, `a.h`, `b.h`. Besides them, the harness
supplied environment information (working directory, platform darwin, date) and
the user's account email, which I did not use in the analysis.
