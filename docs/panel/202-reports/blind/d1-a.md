# Panel 202, llm-ergonomist (blind seat): folder `d1-a`

Copied by the coordinator on 2026-10-09 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 23:46:42 and ended 23:47:39 by `date`, on the second run (the first, at 23:31, stopped on the account's session limit with no report); `run.json`: cost USD and turns 0.2589 9. Its inputs in `docs/panel/202-briefs/blind/d1-a/`, the spec the trunk's at `65b78f2e`.

---

# Report on main.hero

The program: `main.hero` uses two modules. `left.hero` declares an extern group
on `a.h` with `function twice(x: i64) -> i64` and defines `left(x: i64) -> i64`
returning `twice(x: x)`. `right.hero` declares an extern group on `b.h` with
`function twice(x: i32) -> i32` and defines `right(x: i32) -> i32` returning
`twice(x: x)`. `a.h` holds `static inline long twice(long x) { return 2 * x; }`
and `b.h` holds `static inline int twice(int x) { return x + x; }`. `main` prints
`left.left(x: 3)` and `right.right(x: 4)`.

The one real question is that both modules bind the same C name `twice` with
different C signatures. In Heroes terms this is legal (each module has its own
namespace, and the two `twice` are reached only inside their own files). In C
terms it is legal only if the two headers are never in one translation unit.
See `choice_points`.

## check

- Exit code: 0.
- Output: nothing.
- Why:
  - Layout and `use`: "`use geom` binds `geom` to `geom.hero`'s declarations,
    written qualified" and "Every `use` starts at the directory of the file you
    compile." `use left` and `use right` find `left.hero` and `right.hero`;
    `left.left(...)` and `right.right(...)` are qualified calls. A function named
    `left` inside `left.hero` is not shadowing, because `left.hero` has no `use
    left`; in `main.hero` the name `left` is bound only once ("A file may not
    bind one name twice").
  - Literals: "A literal takes the type its context asks for ... otherwise
    `i64`." So `3` is an `i64` (parameter of `left`) and `4` is an `i32`
    (parameter of `right`).
  - FFI widths: "A **parameter** and a **field** are declared at the header's
    own width and sign: `i32` where C says int". C fact: on this platform
    (Darwin, LP64) `long` is 64-bit signed, so `x: i64` matches `long x`, and
    `x: i32` matches `int x`. Results: "clang checks every result type ...
    against that header, and a result may be wider than C's"; `i64` against
    `long` and `i32` against `int` are exact.
  - No library needs linking: both functions are `static inline` in the headers,
    and "`link` a library when the symbols need one."
  - Every parameter is used, every computed value is used (`print` and
    `return`), each `-> T` function returns on its only path, and `print` takes
    a number ("takes the types this language renders as text: a number, `str`
    or `bool`"), so an `i64` and an `i32` are both accepted.
  - Named arguments with a single parameter are allowed by the grammar (`Arg =
    [ ident ":" ] [ "@" ] Expression`).
  - The specification defines no message for a successful check, so I give it
    none.

## build_and_run

- `heroes build main.hero -o main`: exit 0, no output, binary `main` written.
- `./main`: exit 0, output:

```
6
8
```

- Why:
  - `left.left(x: 3)` calls `twice` from `a.h`: C computes `2 * 3L` = `6`
    (a `long`, returned as `i64` 6). No overflow.
  - `right.right(x: 4)` calls `twice` from `b.h`: C computes `4 + 4` = `8`
    (an `int`, returned as `i32` 8). No overflow.
  - "`print` writes its values with no separator and exactly one trailing
    newline", so each call prints its number and a newline.
  - `main` "takes nothing and produces nothing"; it returns normally, there are
    no leases or handles owed ("a lease nobody ends, like a handle nobody
    consumes, aborts when `main` returns"; none exist here), so the exit code
    is 0.

## test

- Exit code: 0.
- Output: no test results, because there are no `test` blocks in `main.hero`,
  `left.hero` or `right.hero`. The specification gives no format for the test
  runner's report, so I cannot quote one; at most a summary saying zero tests
  ran and none failed.
- Why: "`test` blocks run only when asked for; ordinary builds ignore them."
  There are none to run, so nothing can fail. The same FFI and type reasoning
  as in `check` applies, so compilation succeeds.

## choice_points

1. **Two extern groups binding the same C symbol `twice` with different
   signatures.** The specification says each file is its own module with
   qualified access, and forbids duplicate tags for records ("two records may
   not name one tag but `void`") but says nothing that forbids two groups in
   different modules naming the same C function. It also does not say whether
   the compiler emits one C translation unit for the whole program or one per
   module.
   - My choice: the program is valid and each module's group is compiled
     against its own header (separately, as `static inline` functions have
     internal linkage per translation unit), giving `6` and `8`, all exit 0.
   - The other choice: if the compiler includes `a.h` and `b.h` in one
     translation unit, clang reports a conflicting redefinition of `twice`
     (`long twice(long)` against `int twice(int)`). Then `check` (if it runs
     clang, as "clang checks every result type ... against that header"
     suggests) and `build` would fail with a non-zero exit and a clang
     diagnostic naming `twice`, no binary would be written, `./main` would fail
     from the shell with "no such file or directory" (exit 127 in zsh), and
     `test` would fail to compile the same way.
2. **Width of C `long`.** I took LP64 (Darwin, the environment's platform), so
   `long` is 64-bit and `x: i64` matches. On an LLP64 target (Windows) `long`
   is 32-bit and the `i64` parameter would be refused ("one that disagrees is
   refused").
3. **Output of `check` and `test` on success.** The specification gives no
   text; I chose silence for `check` and at most a zero-test summary for
   `test`, exit 0 for both.

## confidence

- Values `6` and `8`, if the build succeeds: high.
- That the build succeeds (choice point 1): moderate, about 65%. The language
  design (modules with their own namespaces, a C name used only inside its own
  file) points to acceptance, but an implementation generating one C file
  would trip over the duplicate `static inline twice`, and this program looks
  built to probe exactly that.
- `test` exit 0 with no test failures: high given a successful compile; the
  exact text is unknown.

## context

Only this directory's files reached my context: `brief.md`, `spec.md`,
`main.hero`, `left.hero`, `right.hero`, `a.h` and `b.h`. I ran nothing. Apart
from those, the session carried only the harness's environment information
(working directory, platform Darwin, date) and an account email address, which
played no part in the answer.
