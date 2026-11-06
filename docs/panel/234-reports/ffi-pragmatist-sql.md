DELIVERED: 2026-11-05 23:25 CET

**The four deliverables stand** under `docs/panel/234-reports/ffi-pragmatist-sql/`:
`thin/core/database/sql.hero`; `thick/core/database/sql.hero` with
`thick/core/database/sql/sqlite3.hero`; `thin/q1.hero` to `q3.hero` and
`thick/q1.hero` to `q3.hero` with their nine case outputs each
(`qN.caseK.out`); `thin/card.md` and `thick/card.md`; and a root driver in
each shape (`driver.hero`). **Ran on this Mac and in Linux arm64**: thin 10
tests and thick 13, all passed on both; the 18 reference cases right on both,
byte for byte against `caseN.want`, each exit 0 (§ 3, § 4, § 6).

**The commands that re-run them**, run as written from the repository's root
at 2026-11-05 23:23 CET on this Mac (10 and 13 tests passed, 18 cases right,
exit 0):

```sh
# From the repository's root. H is a compiler built from the seed
# (clang -I runtime seed/heroes.c runtime/runtime.c -o heroes); `heroes`
# writes build/ in the working directory, so the shapes are copied to a
# folder git ignores before they are built.
H=${H:-$PWD/heroes}
R=docs/panel/234-reports/ffi-pragmatist-sql
T=$PWD/docs/panel/234-briefs/blind/Q/tasks
S=.claude/worktrees/234-ffi-sql-scratch/rerun
rm -rf $S && mkdir -p $S && rsync -a --exclude '*.out' $R/ $S/
for shape in thin thick; do
    (cd $S/$shape && "$H" test driver.hero)
    for n in 1 2 3; do
        (cd $S/$shape && "$H" build q$n.hero -O0 -o q$n.bin) || continue
        for k in 1 2 3; do
            A=(); while IFS= read -r l; do A+=("$l"); done < $T/Q$n/case$k.args
            if $S/$shape/q$n.bin "${A[@]}" | cmp -s - $T/Q$n/case$k.want; then echo "$shape Q$n case$k: right"; else echo "$shape Q$n case$k: WRONG"; fi
        done
    done
done
```

In the Linux arm64 container these commands were **not** run as written: §
6's run was `234-ffi-sql-scratch/linux-run.sh`, over copies of the same
sources (byte-identical to the delivered files by `cmp`, and the 18 case
outputs identical to this Mac's), with a compiler built there under another
name, `heroes-linux`; its whole output is `linux-run.log` beside it.

# Panel 234, the ffi-pragmatist's first brief: SQLite, thin and thick

(Report written as I go. The `DELIVERED:` line goes at the top once the four
deliverables stand and run.)

## Setup

- Worktree `.claude/worktrees/234-ffi-sql/`, detached at `c935ff008`; compiler
  built there from the seed, `clang -I runtime seed/heroes.c runtime/runtime.c
  -o heroes`, and `./heroes --version` read `heroes 0.6.0` / `sources
  1a1e340778db789a` (run 2026-11-05).
- Scratch: `.claude/worktrees/234-ffi-sql-scratch/`.
- Read: my brief, `00-shared.md`, `conventions-draft.md`, `probe/README.md`,
  `blind/README.md` and its templates, the three tasks Q1 to Q3 with their
  cases, the specification whole, design.md §1.11 and §4.19, and the tree's
  two SQLite programs (`examples/sqlite/main.hero`,
  `examples/ledger/db/sqlite.hero`). I read no other seat's report or worktree.

## Log of what was run

### 1. The C facts, against the real header (this Mac)

`.claude/worktrees/234-ffi-sql-scratch/c/facts.c`, compiled `clang -std=c11
-Wall -Wextra -Werror -pedantic facts.c -lsqlite3`: **clang accepted it, exit
0**, against `MacOSX.sdk/usr/include/sqlite3.h`, header and library both
SQLite 3.54.0. What it printed, each line a fact the cards must agree with:

- `select count(*), sum(n)` over no rows: one row; `count` type 1 (INTEGER)
  value 0; `sum` **type 5 (NULL)**, `sqlite3_column_int64` answers **0**,
  `sqlite3_column_text` answers a **null pointer**. (F21's
  `sum_type_is_null=1`, `sum_as_int64=0`, reproduced.)
- After `SQLITE_DONE`, a second `sqlite3_step` with no `sqlite3_reset`
  answers **21, `SQLITE_MISUSE`**. `sqlite3_compileoption_used("OMIT_AUTORESET")`
  answers 1 (`opts.c`): Apple builds this library with `SQLITE_OMIT_AUTORESET`
  and `SQLITE_ENABLE_API_ARMOR`. (Linux: see below, run there.)
- Text `"9000000000"` and `"-1"` bound with `sqlite3_bind_text` into an
  `INTEGER` column are stored as `integer`; `sum` reads 8999999999. So binding
  every argument as text is enough for Q1 (F1's premise holds).
- A text threshold `"49"` bound against `age integer` in `age > ?` compares
  numerically; `ORDER BY name` gives `10`, `Zed`, `o'neil`, `zoe`, bytes.
- `insert ... values ('o'neil', 50)` written into the SQL text: `rc 1`, `near
  "neil": syntax error`, the failure Q2's hidden case is built to provoke.
- A `UNIQUE` refusal through a prepared insert: `sqlite3_step` answers **19**
  (`SQLITE_CONSTRAINT`), `sqlite3_extended_errcode` **2067**
  (`SQLITE_CONSTRAINT_UNIQUE`), and the **`sqlite3_reset` after it answers 19
  too**, not 0: a thin program that asserts `reset == SQLITE_OK` aborts on the
  first duplicate.
- `sqlite3_close` with a statement still prepared answers **5
  (`SQLITE_BUSY`) and closes nothing**; after `sqlite3_finalize` it answers 0.
- `sqlite3_open("/no-such-directory-here/x.db")` answers 14 with a
  **non-null** handle that must still be closed (closing answers 0), as
  `examples/ledger/db/sqlite.hero` measured.

### 2. The Heroes compiler across the package boundary (scratch `p1` to `p3`)

A first thin module, `p1/core/database/sql.hero` (the group and one wrapper
beside each member), and a program in another module, `p1/main.hero`:

- **A program reads a group's constant and spells a group's handle across the
  module boundary**: `sql.SQLITE_OK`, `db: sql.Db @= nullptr`, `query:
  sql.Stmt @= nullptr` all compile; an `@` wrapper carries `acquires` and
  `consumes` through (`open(filename: str, @db: Db)`, `finalize(@statement:
  Stmt)`). `heroes run main.hero` printed `42`, exit 0.
- **A statement never finalised**: `panic: 1 C handle(s) never given back ...
  1 taken at core/database/sql.hero:21:12, in sql.prepare_v2`, exit 134. The
  runtime catches it, and **names the package's line, never the program's**.
- **A close before the finalise, with `sqlite3_close(db: Db consumes) -> i64`
  as `examples/sqlite` and `examples/ledger` bind it**: `close` answered 5,
  `finalize` 0, **exit 0, silent**. SQLite closed nothing (`SQLITE_BUSY`), and
  the language counted the connection given back: a leaked connection no
  instrument of this Mac reported. *(Measured after this line was written:
  `leaks --atExit` on its `-O0` binary reads `0 leaks for 0 total leaked
  bytes`, the program's own `42`, `5`, `0` in the log; and in Linux arm64,
  built `--sanitize -O0`, LeakSanitizer reports nothing, exit 0. **None of
  the three instruments sees the connection left open**; why `leaks` and
  LeakSanitizer count it reachable is unrun.)*
- **The same with `-> i64 when 0` on `sqlite3_close`** (`p2`): `panic: 1 C
  handle(s) never given back ... taken at core/database/sql.hero:15:12, in
  sql.open`, exit 134. **`when 0` is the right binding of `sqlite3_close`**,
  and the tree's two SQLite programs lack it (a finding beside this brief:
  each is silent on a close with a live statement, which neither program
  performs today).
- **A statement used after `finalize`**: through the package's wrapper it is a
  **runtime** abort, `panic: a dead C handle reached the argument `statement`
  of `sqlite3_step` ... at core/database/sql.hero:24:12, in sql.step`, exit
  134; with the extern called in the same function (`p3/direct.hero`) it is
  a **compile error**, `error[handle_used_after_end]`, whose second note says
  why: *this is decided one function at a time ... an end ... inside a helper
  is the runtime's to stop, before C*. Panel 033 R5 makes every call of a
  package over C go through such a helper, so **no program using any package
  over C gets the compile-time use-after-end check**, thin or thick; it gets
  the runtime's abort, located in the package.
- **A status compared with the wrong constant** (`while sql.step(query) ==
  sql.SQLITE_OK`): prints `0`, **exit 0, silent**. Nothing refuses it: both
  sides are `i64`.
- **Go's parameter names collide** (`p4`): Go's `Query(query string, args
  ...any)` keeps neither name in Heroes. A parameter `query` in a module that
  declares a function `query` is `error[shadowed_binding]` (*`query` is the
  name of the declaration at line 17, which is in scope everywhere*), and
  `args` is `error[builtin_name_taken]`. Conventions draft §2.4 (*Go's
  parameter names where the contract is Go's*) cannot hold here; the thick
  shape names them `sql` (SQLite's own name in `sqlite3_exec`'s prototype)
  and `arguments`.
- A refused `prepare_v2` (`"selec 1"`) answers 1 and leaves the statement
  `nullptr`, which is **not counted as acquired** (no panic at exit unfinalised),
  and `finalize` on it answers 0; an empty one (`"  -- nothing"`) answers 0
  with a `nullptr` statement, and `step` on that answers 21, no crash.

### 3. The thin shape, built and run on this Mac

`thin/core/database/sql.hero`: ten functions, five constants (four a program
reads), two handles, six `test` blocks. **Its rule of thinness**, stated in its
header: each wrapper keeps C's name less `sqlite3_`, C's parameters in C's
order, and C's integer answer; it adds a `str` lent or leased as `cstr` and a
text validated into `str?`; and it fills itself, with SQLite's documented
default, the seven C parameters a program could only get wrong (both lengths,
always `-1`, since a longer one sends SQLite past the bytes; the destructor,
always `SQLITE_TRANSIENT`, since the bytes are a lease the wrapper ends; the
tail, and `sqlite3_exec`'s callback, context and message, `nullptr`); `heroes fmt --in-place` added two
blank lines and nothing else. `heroes test driver.hero` (the root driver):
**6 tests, all passed**, exit 0. The three references, built as the grader
builds (`heroes build qN.hero -O0`) and run on each case's arguments by
`runcases.sh` (scratch): **9 of 9 right**, each exit 0, byte for byte against
`caseN.want`. *(Later the same evening: four more `test` blocks, one for each
card claim still untested, `exec` over two statements, `not_text`, `finalize`
after a refused step, `reset` keeping bindings: **10 tests, all passed**, and
the references re-run, 9 of 9.)*

### 4. The thick shape, built and run on this Mac

`thick/core/database/sql.hero` (the face: four types, six functions a program
calls, five private helpers, seven `test` blocks) over
`thick/core/database/sql/sqlite3.hero` (the floor: sixteen wrappers, ten
constants, two handles, three `test` blocks). `heroes test driver.hero`:
**10 tests, all passed**, exit 0. `@db.connection`, a field of an `@`
parameter, passes as an `@` argument to the floor's `close`. The three
references, built `-O0` and run by `runcases.sh`: **9 of 9 right**, each exit 0.
*(Later the same evening: three more `test` blocks for card claims, the first
statement alone run, `not_text` and a blob's `wrong_type`, a primary key and
`query` failing `unique` with `rows_affected` of an update: **13 tests, all
passed**, and the references re-run, 9 of 9.)*

### 5. What each shape does with a programmer's mistake (this Mac, measured)

Each row is a program in the scratch (`p1/`, `p2/`, `thin/mut/`,
`thick/mut/`), run with `heroes run`; the verdict is the compiler's or the
runtime's, quoted.

| mistake | thin | thick |
|---|---|---|
| a status compared with the wrong constant (`while sql.step(q) == sql.SQLITE_OK`) | **silent**: prints `0`, exit 0 (`p1/m_wrongconst`) | no status reaches the program |
| its analogue, an error code compared with the wrong string (`e.code == "constraint"`) | (n/a) | **silent**: `added: 2` / `duplicates: 0`, exit 0 (`t_wrongcode`) |
| a constant or a function misspelled (`sql.SQLITE_OKAY`) | **compile error** `unknown_in_module`, fix `sql.SQLITE_OK` (`m_typo`) | a misspelled code string is the row above: silent |
| a statement never finalised | **runtime abort at exit**, `panic: 1 C handle(s) never given back`, exit 134, at the package's line (`m_nofinal`) | impossible: no statement reaches the program |
| a database never closed | **runtime abort at exit**, exit 134, at the package's line, also after a failed `open` (`m_badopen`: `1 taken at core/database/sql.hero:73:12, in sql.open`) | **runtime abort at exit**, exit 134, at the floor's line (`t_noclose`) |
| close before finalise | with `consumes` alone (the tree's binding): **silent**, exit 0, connection never closed, and `leaks` and LeakSanitizer silent too (`p1/m_closefirst`); with `when 0` (this package): abort at exit (`p2`) | impossible |
| a handle used after it was given back | **runtime abort**, `panic: a dead C handle reached the argument ...`, exit 134, at the package's line (`m_afterfinal`); a compile error only where the extern is called in the same function (`p3/direct`), which a package's user never does | **runtime abort**, same panic, at the floor's line (`t_afterclose`) |
| `reset` forgotten in an insert loop, statuses unread | **silent here**: `count: 1`, `sum: 3`, exit 0, every step after the first answered 21 under Apple's `OMIT_AUTORESET` (`m_noreset`); Linux below | impossible: `exec` prepares and finalises per call |
| a value written into the SQL text, status unread | **silent**: `o'neil`'s row missing, `zoe` printed, exit 0 (`m_written`) | **abort at the program's line**: `.must() on an error: refused: ... near "neil": syntax error` (`t_written`); a dropped `T?` is a compile error (`t_drop`, `discarded_failure`) |
| a `?` never bound | **silent**: SQLite binds NULL, `0` printed, exit 0 (`m_unbound`) | **abort at the program's line**: `refused: ... 2 parameters, and 1 arguments given` (`t_unbound`) |
| `reset`'s answer asserted `SQLITE_OK` after a refused insert | **runtime abort**, `assert failed ... left: 19 right: 0`, at the program's line (`m_resetok`) | impossible |
| Q1's NULL `sum` read with `.must()` | `column_int64` reads 0: right by SQLite's conversion, with nothing to get wrong | **abort at the program's line**: `.must() on an error: null_value: column 1 is NULL` (`t_must_null`) |
| a bare call line (thin `sql.exec(db, ...)`; thick `sql.exec(...).must()`) | **compile error** `discarded_value`, and its **fix (certain) writes `_ = sql.exec(...)`, which discards the status**: the machine-applicable repair produces `m_written`'s silent shape (`m_bare`) | **compile error** `discarded_value`, fix (certain) `_ = ` in front of `.must()`, which keeps the abort (`t_bare`); `_ =` on the `T?` itself is refused, `discarded_failure` (`t_drop`) |

### 6. Linux arm64 (`heroes-linux-arm64:latest`), run 2026-11-05

**The image has SQLite**: `/usr/include/sqlite3.h` and
`/usr/lib/aarch64-linux-gnu/libsqlite3.so`, version **3.46.1** (header and
library), Debian clang 22.1.8. One `docker run --rm -v <worktree>:/w -v
<scratch>:/s -v <tasks>:/t:ro -w /w heroes-linux-arm64:latest bash
/s/linux-run.sh`, its whole output in `234-ffi-sql-scratch/linux-run.log`
(236 lines), on source-only copies of both shapes (`scratch/linux/`):

- The compiler built in the container from the seed as `heroes-linux` (the
  Mac binary untouched): `heroes 0.6.0`, `sources 1a1e340778db789a`.
- `facts.c`, `facts2.c`, `opts.c`: **clang accepted all three** against
  Linux's header; every fact of § 1 reads the same **but one**: a `step`
  after `SQLITE_DONE` with no `reset` answers **100** there, where this Mac
  answers 21. `OMIT_AUTORESET used: 0`: Debian's SQLite resets by itself.
- **Thin**: `heroes test driver.hero` **10 tests, all passed**; references
  **9 of 9 right**, each exit 0.
- **Thick**: `heroes test driver.hero` **13 tests, all passed**; references
  **9 of 9 right**, each exit 0.
- Every mutant of § 5 gave the same verdict as on this Mac (the same compile
  errors, the same aborts at the same lines, the same silences), **but one**:
  `m_noreset` (thin, `reset` forgotten, statuses unread) prints **`count: 4`
  / `sum: 12`** there and **`count: 1` / `sum: 3`** here; the truth is
  `count: 4` / `sum: 16`. `noreset.c`, run on both machines, says why: after
  `DONE` with no `reset`, `sqlite3_bind_text` answers 21 on both, so the new
  value is **not bound**; then `sqlite3_step` answers 21 here and 101 there,
  re-running the insert with the old value. **One thin program, two
  platforms, two different silent wrong answers**, from a build option of
  the C library (`SQLITE_OMIT_AUTORESET`) that the thin shape hands straight
  to the program. The thick shape cannot reach it: `exec` prepares and
  finalises inside each call.

### 7. Costs, measured

| | thin | thick |
|---|---|---|
| package files | 1 | 2 (face and floor) |
| package code lines (no comment, blank or `test` block) | 47 | 98 + 70 = 168 (3.6x) |
| package bytes | 12,967 | 13,955 + 8,166 = 22,121 |
| `test` blocks | 10 | 10 + 3 |
| C members bound (all verified by clang against the header, both machines) | 10 functions, 5 constants, 2 handles | 16 functions, 10 constants, 2 handles (floor) |
| reference code lines, Q1 / Q2 / Q3 | 19 / 27 / 22 = 68 | 10 / 14 / 15 = 39 (57%) |
| card | 33 lines, 3,034 bytes, **898** tokens | 33 lines, 2,429 bytes, **727** tokens |

The tokens are `heroes measure`'s vendored tables, **a lower bound and not
the reader's tokeniser** (the instrument says so; `--refresh` reads only the
specification and `CLAUDE.md`); the S experiment's `card-H.md` reads 585 on
the same tables. *(The thin card read 859 before its first line was
corrected, at the last read-through: it said "with C's parameters in C's
order", and seven C parameters are filled by the package, so it now names
them.)* Not measured, by the sitting's rule: any duration. Thick
compiles one statement per `exec` (Q1 with N arguments compiles N + 2; thin
compiles 3, one of them inside `sqlite3_exec`) and holds a query's every row at once: a cost in time and memory
that grows with the data, unrun here.

### 8. Two more runs for the judgement (`p5`, `p6`)

- **Codes exported as constants** (`p5/core/codes.hero`, `constant UNIQUE:
  str` with the body `"unique"`): `e.code == codes.UNIQUE` runs (`true`);
  `codes.UNIQE` is `error[unknown_in_module]` with a **certain** fix; the
  literal `"uniqe"` still runs silently (`false`, exit 0).
- **The face and the floor in one program** (`p6/mixed.hero`: `sql.open`,
  then `sqlite3.prepare_v2(db.connection, ...)`, bind, step and finalize
  through the floor, then `sql.query` and `sql.close`): prints `count: 4` /
  `sum: 16`, exit 0. A program can reach the floor through the face's `Db`.
- **A Go reader's cursor** (`p6/gonext.hero`, `while sql.next(rows)`):
  `error[unknown_in_module] ... declares nothing called next — it has text`,
  `fix (guess): write sql.text`. A compile error, never a wrong answer; its
  near-name guess names a function of another contract (a finding for the
  diagnostics, minor).

## Judgement

### verdict

**approve**: thick as the package's face, on four conditions (below). No
veto: neither shape changes how a value crosses the C boundary. The thick
floor is the thin layer, the same group verified the same way.

### section

design.md §1.11, whose own list of decisions made for the FFI reads
*"**Errors as values** — maps onto C return codes without machinery (4.6)"*
(design.md:564); §4.6, which refused exceptions because they produce
*"precisely the class of bug that does not raise a compile error: an
unhandled error path"*; §1.11's third point, on §4.19's mechanism, *"a
wrong type in an `extern` is a *compile* error, which is this project's
thesis applied to the boundary"* (design.md:587); §1.12, robustness. **design.md does not rule on thin against
thick**: no section and no file of `docs/panel/` uses the word *thick*, and
panel 033 R5 (`extern_across_modules`) says only that every package over C
has the thin layer.

### experiment

The C: `facts.c`, `facts2.c`, `noreset.c`, `opts.c` (scratch `c/`), `clang
-std=c11 -Wall -Wextra -Werror -pedantic` against the real `sqlite3.h`:
**accepted** on this Mac (SQLite 3.54.0) and in Linux arm64 (3.46.1). The
Heroes: both shapes over one kind of group, 16 and 10 functions bound, clang
verifying every one on both machines; 23 tests and 18 reference cases right
on both; 22 programs of mistakes run on both (§ 5, § 6).

### cost

Measured (§ 7): the package, thin 47 code lines in one file against thick
98 + 70 in two (3.6 times), the thick floor binding 16 functions where thin
binds 10; the programs, thin 68 code lines over the three references against
thick 39; the card, thin 898 tokens against thick 727 (vendored tables, a
lower bound). The binding itself costs the same in both: one group, one
wrapper per member, clang verifying each against the header, no shim, no
marshalling beyond a `str` lent or leased and a text validated. Unmeasured
here (no clock in this sitting): thick compiles a statement per `exec` and
holds a query's every row.

### argument

Measured on two machines, thin is silent on five mistakes: a wrong
constant, a value written into the SQL with its status ignored, an unbound
`?`, a forgotten `reset` (two different wrong answers on two platforms), a
close before a finalise under the tree's binding. Thick is silent on one: a
code compared with the wrong string. Decisive: a dropped `T?` is a compile
error (`discarded_failure`), a dropped `i64` is not, and for a thin bare call
the compiler's certain fix writes the discard. A thin face opts every
program out of what errors-as-values exist for at this boundary. Thick costs
the package's author 3.6 times the code and breaks nothing: its floor is the
thin layer, verified identically.

### What each shape gets wrong for a programmer (the brief's three, measured)

- **An integer status compared with the wrong constant**: thin **runs
  silently** (`0`, exit 0); a misspelled constant is a **compile error**.
  Thick has no status; its analogue, a code compared with the wrong string,
  **runs silently** too (`t_wrongcode`), and so does a misspelled code
  (`p5/literal`), where thin's misspelling is refused. This is the one class where thin is stronger.
- **A statement never finalised**: thin, the **runtime aborts at exit**
  (`1 C handle(s) never given back`, 134), naming the package's line, never
  the program's; thick, impossible.
- **A handle used after close**: both shapes, the **runtime aborts**
  (`a dead C handle reached the argument ...`, 134) at the package's or
  floor's line. **Never a compile error through any package over C**: the
  compiler decides it one function at a time, and an end *"inside a helper
  is the runtime's to stop"*, while panel 033 R5 makes every package call
  such a helper (`p3/direct` against `m_afterfinal`).

And beside them: thin alone is silent on a value written into the SQL text
with its status ignored, a `?` never bound, and a forgotten `reset`, the last
platform-divergent; thick alone aborts on Q1's empty case where a program
writes `.must()` on the NULL `sum` (`null_value`), at the program's own line.

### Would a third shape beat both?

- **Thick with its codes exported as constants** (`sql.UNIQUE`): measured,
  a misspelled constant becomes a compile error with a certain fix, while a
  string literal stays silent. It narrows thick's one silent class and does
  not close it. Six constants on this card. Worth adopting as a convention
  (conventions-draft §3.2), and not a different face.
- **Thick, with its floor published and carded** (the brief's *unless it
  asks*): measured, a program reaches the floor through `db.connection`.
  For the tasks it is thick. For a program that needs what this face lacks
  (a bound integer or blob, a statement reused over many rows, a cursor over
  a result too large to hold), it gives thin's capability with thin's
  failure modes confined to the program that asked. **This beats both**, at
  the price of a floor that is the full thin layer and not only what the
  face needs (this thick floor has no `reset` or `exec`), and of a second
  card. It needs conventions-draft §1.3 and §1.4 amended: the floor is never
  the face, but it is reachable.

### F23's boundary for names

Adopted, with two amendments from measurement. (1) **Go's parameter names
give way where the language refuses them**: `query` inside a module declaring
`query` is `shadowed_binding`, and `args` is `builtin_name_taken`; the page
should say what replaces them (here SQLite's own `sql`, and `arguments`). (2)
**A function that cannot fail keeps Go's name though Go's answers an
`error`** (`close` here): the boundary is about how a failure or an absence
is told when there is one. And `query` keeps its name while answering
`[Row]` where Go answers a cursor: a Go reader's `rows.Next()` is a compile
error, never a wrong answer (`p6/gonext`). `exec` keeps Go's `Result`, whose
measured price is one compile error a reader may meet: `sql.exec(...).must()`
alone is `discarded_value`, with a certain fix.

### NULL in each shape (the brief's question)

Thin hands a NULL over as C does: `column_int64` reads it as **0** (Q1's
want, by SQLite's conversion), and `column_text` fails `null_cstr`. Thick hands
it over as a value, `.null_value`, and `integer` and `text` **fail
`null_value`** on it, so Q1's empty case asks the program to say what NULL
means (`.default(0)` in the reference). `.must()` there aborts at the
program's line, measured.

### prediction

In experiment Q, summed over the two models: (1) **thick's `silent` share is
no higher than thin's on any of the three tasks, and if any answer is
`silent` on Q2 or Q3, at least one is thin's**; (2) **Q1 is the only task
where thick's `right@1` can fall below thin's**, and where it does, the cause
is `.must()` on the NULL `sum` aborting case 2 (`null_value`), while no thin
answer fails case 2 for its NULL. Falsified by a thick-only `silent` answer on
Q2 or Q3, or by thick below thin on Q2 or Q3 for any other reason. And for
the binding: **SQLite, rung 3 of §4.19's ladder, needs no shim in either
shape on the Windows box**, unrun (the box is off).

### condition

**To object to thick**: the reader test showing thick's `right@2` below
thin's on Q2 and Q3 on both models (the face costs a reader more than the
statuses do); or a C package of this milestone whose thick face needs a C
shim, or copies every value on every call where thin would not (the Lua
lesson), for example a callback-driven library such as libcurl's write
callback. **To veto**: a thick face that changes how a value crosses the
boundary, its layout, ownership, `str`'s zero byte or a handle's marks. These
two do not.

**The four conditions of my approval**: (1) `sqlite3_close` and every
consumer that may refuse to end its handle bound with `when 0`. The tree's
two SQLite programs lack it, measured silent on a close before a finalise:
**a finding to file as a defect** (`examples/sqlite/main.hero`,
`examples/ledger/db/sqlite.hero`; neither program performs that close
today). (2) The floor is published with its own card and holds the full thin
layer (third shape). (3) Codes exported as constants, or the measured silent
class recorded as accepted. (4) Conventions-draft §2.4 amended for the
parameter names the language refuses. **And one finding beside them**: the runtime's handle panics name
only the package's line (`taken at core/database/sql/sqlite3.hero:61:12, in
sqlite3.open`). A reader of a program using a package over C is never told
which of its own lines took or reused the handle. **A finding for the
compiler-engineer** (unrun: whether the runtime can carry the caller's
frame); not a condition of this verdict.
