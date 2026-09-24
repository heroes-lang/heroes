# M-agreed-retention — one C function, one story about who frees what it is handed

**Opened 2026-09-21**, at M-declared-extents's close, because that milestone's
last four open items had nowhere to live: a closed milestone's file is a place
nobody looks, and `records/homes` says so in as many words. Every one of them
was found by a completeness critic attacking a landed rule at the shapes beside
it, and not one is a defect: each is a shape the language ADMITS and says
nothing about.

**What the milestone before it left standing.** M-declared-extents gave the
boundary two marks and a report. `counted_by` relates the number a lend crosses
with to the field it crosses into; `lent` says a C parameter keeps nothing of
what it is handed, under a default the author flipped so that silence means
*keeps*; and where no word can reach — C frees the bytes from a callback, or
from a later call with no pointer at all — the runtime names the leases that
were live, on all three platforms. What it did NOT do is make two declarations
of one C function agree, or say whose job it is to free a pointer C made.

**Why these are one milestone and not four filings.** Three of the four are the
same question asked at three distances: *whose word about a C function is the
one that counts*. A binding's marks are a claim nothing checks, so two modules
may claim opposite things; one declaration cannot carry a contract chosen per
call; and a pointer C made has no rule at all about who frees it once. The
fourth is a process rule that the closed file could not keep, parked here with
its reason rather than lost.

*******************************************************************************
**OPEN: 2**

- [ ] **M-agreed-retention** | two modules may declare one C function with contradictory retention marks, and the compiler accepts both at `check` 0 | panel 171's completeness critic, the historian's B.4, `selfhost/check/marks.hero`

    **Origin:** panel 171's completeness critic, 2026-09-20, attacking the
    `lent` rule at the shapes beside it. **Filed rather than fixed**, because it
    needs a rule ACROSS modules that no sitting has priced, and because the mark
    it concerns lands in commit A of the flip and does not exist yet.

    **What it is.** Module one declares `function keep(s: cstr lent)`, module two
    declares `function keep(s: cstr)`, both against the same header. Each is
    internally consistent, the two disagree about C, and nothing compares them.
    **This is the exact shape that broke upstream Clang's `noescape` on its first
    day** (2017-09-19): a third-party re-declaration disagreed with the SDK
    header's mark and the build failed. Heroes has no header to disagree with,
    only two `.hero` files, so the disagreement is silent.

    **Measured 2026-09-23, step 1, every mark and not only `lent`**, Darwin
    arm64, module `alt` against module `main` over one local header:

    | the two modules disagree on | `check` | `run` |
    |---|---|---|
    | `lent` against unmarked, panel 171's shape | 0 | 0 |
    | `owned my_free` against `owned other_free` | 0 | 0 |
    | `owned my_free` against no `owned` | 0 | 0 |
    | two `record`s over one tag | **1**, `duplicate_tag`, program-wide | — |
    | `consumes` against unmarked, the second declaring over `alt.H` | 0 | **134**, the live set |
    | `borrows` against `acquires`, over `alt.H` | 0 | **134**, the live set |
    | `acquires h_close` against `acquires h_close2` | 0 | 0, **and the same inside ONE module**, which is defect 075 |
    | `counted_by` naming two different siblings | UNRUN: it needs two numeric siblings, and none was written | |

    **The census**, `awk` over every `extern` group in `git ls-files '*.hero'`
    outside `archive/`: **423** declarations in **187** files. Inside one program
    no C function is declared twice in `examples/` or `selfhost/`; the one
    program that does is `tests/golden/surface-fixtures/twoarity/`, `printf` at
    two arities with both formats `lent`, **legal by design**: its README is
    panel 094's refutation of a rename clause, *one C symbol, two bindings, no
    new syntax*. So a rule relating two declarations moves no shipped file and
    must admit that fixture, and **it pulls against the third item**, whose
    natural route is exactly panel 094's: one module per retention mode. And it has a place to stand: `checker.hero` checks one resolved
    program, and `one_tag_one_type` (`selfhost/check/decls.hero:334`) is already
    program-wide.

- [ ] **M-agreed-retention** | one Heroes declaration cannot reach all three of `sqlite3_bind_text`'s retention modes, so the shipped ledger and measurement 037 declare the same C function two incompatible ways | panel 170's completeness critic, `examples/ledger/db/sqlite.hero`

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

*******************************************************************************
