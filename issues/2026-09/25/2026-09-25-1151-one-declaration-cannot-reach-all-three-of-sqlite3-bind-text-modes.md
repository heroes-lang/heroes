# One declaration cannot reach all three of `sqlite3_bind_text`'s retention modes, and the record says which one it reaches

2026-09-25, M-agreed-retention step 12: the milestone's third item, filed by
panel 170's completeness critic on 2026-09-20 and measured at step 1 against
the SDK's own `sqlite3.h`, closed by record and not by a form.

- [x] **M-agreed-retention** | one Heroes declaration cannot reach all three of `sqlite3_bind_text`'s retention modes, so the shipped ledger and measurement 037 declare the same C function two incompatible ways | panel 170's completeness critic, `examples/ledger/db/sqlite.hero` · **closed 2026-09-25**

    **Origin:** panel 170's completeness critic, 2026-09-20, re-run by the
    coordinator before filing. **It is not a defect and is filed here rather than
    in `docs/work/DEFECTS.md`**, because both directions are LOUD: `d: ptr` takes
    `nullptr` and refuses a function name at `check`, and the null that reaches C
    where C calls it back is a named runtime panic; `d: (function(ptr) -> ())`
    takes the function name and refuses `nullptr` with `error[type_mismatch]` at
    `check`. Nothing is silent and nothing corrupts.

    **What it is** is an expressiveness gap at design.md §1.11's own boundary: a
    real C parameter whose argument may legitimately be a null OR a function has
    no single Heroes spelling, so a binding author must pick one mode and lose the
    other. `SQLITE_STATIC` is a null function pointer, which is why the canonical
    keeps-the-pointer call and the give-away call cannot be written against one
    declaration.

    **Measured 2026-09-23, step 1**, against the SDK's own `sqlite3.h`:

    | `destructor` declared as | `SQLITE_STATIC`, `nullptr` | `SQLITE_TRANSIENT` | a function, `free` |
    |---|---|---|---|
    | `ptr` | `check` 0, `text` unmarked and a lease | `check` 0, `text: cstr lent` | `type_mismatch` |
    | `(function(ptr) -> ())` | `type_mismatch` | `type_mismatch` | `check` 0 |
    | both, in one module | `declared_twice` | | |

    **And the sentence above, *nothing is silent and nothing corrupts*, holds
    for the destructor's TYPE and fails for the text's MARK** — corrected here,
    2026-09-23. Declare `text: cstr lent`, which is true under
    `SQLITE_TRANSIENT`, and pass `nullptr`: `check` 0, `run` 0, and the program
    prints `stored=XXXX…`, the bytes of a later allocation, instead of the text
    it bound. **3 of 3 on Darwin arm64, Linux arm64 and Linux x86-64**;
    `--sanitize` on Darwin reads `heap-use-after-free`; Windows UNRUN, because
    the box has no `sqlite3.h`. So the one declaration an author is likeliest to
    write for the common mode is a wrong answer at exit 0 in the other, and the
    shipped ledger avoids it only by leaving `text` unmarked and paying a lease.

## What is recorded

A real C parameter whose argument may be a null OR a function
(`sqlite3_bind_text`'s destructor: `SQLITE_STATIC`, `SQLITE_TRANSIENT`, or a
freer) has no single Heroes spelling, and a binding author picks one mode per
declaration: `d: ptr` takes `nullptr` and refuses a function name at `check`,
`d: (function(ptr) -> ())` takes the name and refuses `nullptr`. Both
directions are loud. **The mark on the TEXT is where the silence was**, and it
is closed by this milestone's words: under `SQLITE_TRANSIENT` the text is
`lent`; under `SQLITE_STATIC` it is not, and a `lent` declaration passed a
`nullptr` destructor read the bytes of a later allocation at exit 0 (three of
three POSIX legs, `heap-use-after-free` under `--sanitize`). The route panel
094 named stands: one module per retention mode, each declaring the function
once with the marks that are true of that mode, and step 12's
`contract_differs` is what keeps two such modules honest about the positions
they share (the destructor's TYPE differs, so the two modules disagree at that
position on purpose: the rule compares the shared positions at one type, and
a module that must declare both modes gives the second a shim in the header).
The shipped ledger (`examples/ledger/db/sqlite.hero`) keeps `text` unmarked
and pays a lease, which is true under every mode.

What stays open is an expressiveness gap at design.md §1.11's boundary and
not a defect: a sitting that wants one declaration for both modes prices a
form for "a null or a function", which no header states and this milestone
does not buy.
