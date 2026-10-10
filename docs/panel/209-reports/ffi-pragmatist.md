# Panel 209, the ffi-pragmatist: the C boundary under `name @= value`

Started 2026-10-10 16:48:19 CEST (`date`). Folder:
`.claude/worktrees/scratch-b15/209-ffi-pragmatist/`, a detached worktree of
the trunk at `87794631` made by the coordinator; no git command run in it.
Compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` in that
folder (its timing below, once built). Every `./heroes` below is that one;
every `clang` is the system's (`clang --version` below). Time box 45 minutes,
so to about 17:33; what is not reached is written as unrun, in those words.

Written as it goes; sections appended in the order the checks ran. Every
number names its command. Programs under `ffi-probes/` in my folder; a program
using a form the trunk's compiler does not read (`@=`) is written by heredoc.

Compiler built 16:48:44 (`date` after it): `/usr/bin/time -p` read `real 13.92
user 11.13 sys 0.34`; `clang --version` is Apple clang 21.0.0
(clang-2100.3.34.2). Probes `p1` to `p11` and the programs `a_*`, `b_*`,
`c_*` are under `ffi-probes/` in my folder; each `./heroes run <file>` below
is from that directory with `../heroes`.

## 1. What C's values infer to today, which is what R1 would give a cell

The brief's premise is that under R1 a cell's type is *inferred from the value
as an immutable's is*. So the first measurement is what today's inference says
of the values a binding puts into cells. Each row is one probe, exit code and
message by `./heroes run`:

| value | probe | today's verdict | what R1 would type the cell |
|---|---|---|---|
| `nullptr` | `p1b_nullptr_type.hero`: `db = nullptr` then `sqlite3_close(db)` | exit 1, `type_mismatch: expected Db, found ptr` at the call | **`ptr`**, never a handle, never `cstr` |
| `nullptr` into an `i64` cell | `p1c_nullptr_cell.hero` | exit 1, `type_mismatch: expected i64, found ptr` at the write | the same |
| `"...".lease()` | `b_lease_inferred.hero`: `label = "row-0-payload".lease()`, passed to `keep_label(s: cstr)` | builds; its binary exits 134 `panic: 1 lease(s) never ended` (an immutable lease can never be ended, as the spec says: a `@` name) | **`cstr`**, the annotation's type |
| `[9, 9, 9]` | `p7_array_literal_infers.hero`: `md = [9, 9, 9]` handed to a `[u8]` parameter | exit 1, `type_mismatch: expected [u8], found [i64]` | **`[i64]`**, where today's buffers say `[u8]`, `[i32]` |
| `200` | `p4b_i64_to_u8_param.hero`: an `i64` cell born `200` handed to `put(b: u8)` | exit 1, `type_mismatch: expected u8, found i64` at the call, **no fix line** | `i64` |
| a `u8` value into an `i64` cell | `p2_u8_into_i64_cell.hero` | exit 1, `type_mismatch: expected i64, found u8` | no silent widening into a cell |
| an `i64` cell handed to `@n: u64` | `p3_i64_cell_to_u64_out.hero` | exit 1, `type_mismatch: expected u64, found i64` at the `@n` argument, no fix | the out-parameter's width is refused at the call |
| `u8 @ 300` | `p6_u8_literal_wide.hero` | exit 1, `int_out_of_range`, note *a u8 holds 0 through 255* at the declaration | today's annotation catches the width AT THE LINE |
| `[[], []]` | `p11_nested_empty.hero` | see below | |

Two facts of this table matter at the boundary. **`nullptr` is not
`cannot_infer`: it is a `ptr`.** The shared brief counts 1559 cells that keep
their annotation under R1 by today's `cannot_infer` rule (`[]`, `{}`, `fail(`,
`ok(`); a cell born `nullptr` is not among them, and it is the shape every
out-parameter handle has (`db: Db @ nullptr` then `sqlite3_open(..., @db)`,
the spec's § 13 fence and design.md §4.19's). Under R1 as drafted, `db @=
nullptr` compiles its line as a `ptr` cell and the program is refused at the
C call, *expected Db, found ptr*, two lines below the mistake, with no fix
and no word of the declaration. And **a numeric array literal is `[i64]`**,
so a buffer born `[1, 2]` for a `[u8] counted_by` parameter is refused the
same way, at the call. Neither is an unsound compile: every drift is refused
before C runs. Both are an error moved from the line that is wrong to the
line that is right.

## 2. The three programs, today's form built and run, R1's form as text

**A. A handle acquired through an out-parameter and freed**
(`a_handle_today.hero`, the spec's § 13 fence plus `suite_special.hero:419`'s
main): `./heroes run` prints `0`, exit 0. Its emitted C
(`./heroes build a_handle_today.hero --emit-c -o a_handle_today.c`), the
cell's lines:

```
sqlite3 * *const hero_lend_h0_db = (sqlite3 * *)hero_lend_local(sizeof(sqlite3 *), "ahandletoday.main", "db");
t4 = sqlite3_open(hero_cstr_nonnull(t3), &h0_db);
hero_handle_acquired(h0_db, "sqlite3_close");
```

The cell is a `sqlite3 *` lent local because its TYPE is `Db`; nothing of the
declaration's spelling reaches this C. R1's text is `a_handle_R1.hero`: `db
@= nullptr` infers `ptr` (section 1), today's annotation says `Db`, and the
program is refused at `sqlite3_open(..., @db)` with p1b's message. **The
annotation does not drop on this shape; the compiler fails to ask for it.**
The reader loses nothing on the line only because the line must keep `: Db`
to compile at all.

**B. A lease** (`examples/gallery/13-lease.hero`, `./heroes run`: three lines
`C still reads 13 bytes`, exit 0; its emitted C, `b_lease_today.c:106`,
`const char * h1_label;`). R1's text is `b_lease_R1.hero`: `label @=
f"row-{at}-payload".lease()` infers `cstr`, the annotation's type, and `at @=
0` infers `i64`. The reader loses nothing: `.lease()` says `cstr` in its name
and the spec's own sentence defines it so. The lease's only write is
`end_lease(@label)`, which is section 6's point for R1b.

**C. A buffer C fills through an `@` argument**
(`c_buffer_today.hero`, a copy of
`tests/golden/run/fixedbugs-396-a-buffer-c-fills.hero` with its header beside
it): `./heroes build`, the binary under `timeout 20`, exit 0, first line `1 32
1 218 40 9` as its `.expected`. Its emitted C declares `HeroArrayHeader *
h0_md`, `h4_ints`, `h7_small` alike (`c_buffer_today.c:249-256`): the C does
not see the element width on the declaration, the type does. R1's text is
`c_buffer_R1.hero`, twelve cells: 8 born `[]` or `[[], []]` keep their
annotation by `cannot_infer`; `h @= Holder(...)` infers `Holder`, the same; and
**3 born from a numeric array literal (`md @= [9, ...]`, `ints @= [7]`, `small
@= [1, 2]`) infer `[i64]` where today's annotations say `[u8]`, `[i32]`,
`[u8]`**, each refused at its C call (`digest32(@md)`, `fill_ints4(@ints)`,
`double_bytes(@small)`) with p7's message. So on this golden R1 removes the
annotation from 1 cell of 12 and moves 3 errors from the declaration to the
call.

## 3. The widths (the brief's measurement 2)

`grep -rhE ': (u8|u16|u32|i8|i16|i32|f32|f64) @ [0-9"-]' examples | wc -l`
reads **25**, of 31 non-`i64` width declarations
(`^[[:space:]]*[a-z_][a-z0-9_]*: (u8|…|f64) @ `). By file: **22 are `f64 @
0.0` or `@ 1.0`** (nbody, spectral, floats, ledger, query, json, spreadsheet),
which R1 infers `f64`, the same type; **3 are `u8 @ 200`**
(`examples/widths/main.hero:105,173,182`), which R1 infers `i64`. Where the
error lands for those three: not on the declaration (`b @= 200` is a legal
`i64` cell) but at the first place a `u8` is asked, a parameter or a field,
with `type_mismatch: expected u8, found i64` and no fix (p4b), or nowhere, if
the cell never meets a `u8`: then `b @ b + 100` holds `300` where today's
`u8` cell aborts (`p8_u8_cell_arith.hero`: `b: u8 @ 200`, `b @ b + 100`,
exit 134 `panic: integer overflow`). That last shape is not a boundary
defect, it never reaches C, and it is the compiler-engineer's and the
ergonomist's to price; I name it because the brief asked where the error
lands: in that case it does not land.

In the 21 extern files of `examples/` no width cell is born from an integer
literal narrower than `i64` (section 7's table: 88 `int-literal :: i64`, 13
`float-literal :: f64`, 0 of `u8`, `i32`, `f32`). The C-fixed widths there
live in parameters and fields, which R1 does not touch.

## 4. The annotation that stays (the brief's measurement 3)

`examples/ledger/db/sqlite.hero:277`, `problem: str? @ fail(code: "none",
msg: "sqlite has not answered yet")`, the `owned sqlite3_free` out-parameter
of `sqlite3_exec` (`:103`), is the binding that accumulates a C result into a
`fail(`-born cell; `./heroes run examples/ledger/main.hero` is the net's
`corpus` row and I did not rebuild it (time box; its module compiled as part
of the census every batch). Under R1 it reads `problem: str? @= fail(code:
"none", msg: "...")`: the annotation kept, one character more. Beside the
`[]`-born buffers of program C (`other: [u8] @= []`), it reads as today's
line does; nothing is worse. What is worse is the mix inside one function:
program C's `main` under R1 holds `md @= [...]` and `named: [u8] @= []` three
lines apart, one bare and one annotated, where today both say `[u8]`. A reader
of a binding then learns the element width from the extern line below instead
of the declaration. Small, and real.

## 5. The group's grammar (the brief's measurement 4)

- **Inside an `extern` group's lines `@` and `=` never meet.** Over every
  `.hero` of `selfhost/`, `examples/`, `tests/` and the spec (`find … |
  LC_ALL=C xargs awk` printing a group's indented lines holding `=`, comment
  lines dropped): **0 lines**, in **697** files holding an `extern` group
  (`grep -lE '^extern '`). A group's member has no default, no body and no
  value (spec § 13, *a declaration is a signature, not a definition*), so no
  `=` stands in one; `@out: Db` follows `, ` or `(`, never `=`.
- **A C header is never read by the Heroes lexer.** `grep -lE
  'lexer\.|tokens_of|lex\('` over `selfhost/emit/ffi*.hero`,
  `header_reach.hero`, `extern*.hero` prints nothing; the header reaches
  clang through the emitted `#include` and the `_Static_assert` probes
  (`a_handle_today.c:33-34,77-78`). `@` is not a token of C11, so a header
  holding `@=` already fails under clang, Heroes or not. **No route touches
  the boundary's grammar.**
- **Where a binding's caller writes the token by mistake.** Today
  (`p10_at_eq_argument.hero`, `sqlite3_open(path: …, @=db)`):
  `expected_expression` at the `=` AND `not_a_place` at the same column, two
  messages for one keystroke. Under R1 the lexer reads `@=` whole where an
  argument stands, so the parser owes one message, *an `@=` where an argument
  stands: write `@db`*, a `certain` fix. It is a repair of today's two
  messages, not a new hole.
- **R2's token is a planted mutant beside every `@` argument.**
  `tests/golden/recovery/plan-singles.jsonl:4460,4552` plant `out=@db` and
  `tail=@tail` (`named-arg-equals`); today (`p9_named_arg_equals.hero`) that
  is `expected_args_close: expected ), or , and another argument, found =`
  at the `=`. Under R2 `=@` is one token and `out=@db` lexes `out`, `=@`,
  `db`: the message moves to whatever the parser says of a declaration
  operator inside a call. `:` and `=` are one keystroke apart on the label of
  every `@` argument (500 sites in `examples/`, the critic's count), so R2
  places its token on a typo path the metric already plants; R1's `@=` is on
  none (`grep -rF '@='` over the tree: 0, the critic's).

## 6. A cell re-bound only through an `@` argument (R1b)

Over the 21 extern files of `examples/`, every cell whose name is never on
the left of a mutation (`^[[:space:]]+name(\.f|\[i\])* @ `), with how many
times it is passed as `@name` (`(\(|, |: )@name\b`):

```
ledger/db/sqlite.hero  db 2  held 1  problem 1  statement 4  tail 2
sqlite/main.hero       db 1  statement 2  tail 2
gallery/13-lease.hero  label 1
curl/main.hero         url 1
nbody/main.hero        text 0        spectral/main.hero  text 0
```

**12 cells; 10 are written only through `@name`**: the four handles born
`nullptr` and filled by `acquires` out-parameters, the two `cstr` tails, the
`owned` message, and three leases ended by `end_lease(@x)`. Under B2's
sentence as drafted, *a cell nothing re-binds is a compile error*, each of
these ten is refused unless an `@` argument counts as a re-binding: every
out-parameter binding in the tree, the spec's own § 13 program included
(`suite_special.hero:419`), and **every lease without exception**, since the
spec forbids any other write of a lease's cell. The two `text` cells
(`nbody/main.hero:301`, `spectral/main.hero:156`, `text: str @ whole.to_str()
+ …`, never written again) are what B2 exists to refuse, and rightly.

## 7. The 165 cells of the 21 extern files, by first value

`grep -hE '^[[:space:]]+[a-z_][a-z0-9_]*: [^=@]+ @ '` over the 21 files reads
**165**; classed by the value's first characters and the annotation (the awk
in my shell history; a reading of patterns, as the shared brief's table is):

| class | cells | under R1 |
|---|---|---|
| `int-literal :: i64` 88, `float-literal :: f64` 13, a name or call 27, a string 2, a bool 2, a lease 3, a constructor 1, a non-empty array of constructors 1 | **137** | annotation dropped, type the same |
| `[]` 20, `{}` 1, `fail(` 1 | **22** | kept by `cannot_infer`, as the shared brief counts |
| `nullptr :: Db`, `CDb`, `Stmt`, `CStmt`, `cstr`, `cstr` | **6** | kept, but NOT asked for: inferred `ptr`, refused at the C call |

The tree's `@ nullptr` cells, line-anchored: `selfhost/` 0, `examples/` 6,
`tests/` 26 (`find <dir> -name '*.hero' | xargs grep -hE '… @ nullptr' | wc
-l`); immutable `= nullptr` bindings: **0** in `selfhost/`, `examples/`,
`tests/` and the spec. So a rule that made a bare `nullptr` birth
`cannot_infer` would cost no program in the tree, and it is the one rule that
puts the six, and the 26, back under the brief's premise.

Two corrections to the above from probes that finished while it was written:
`p11_nested_empty.hero` (`rows = [[], []]`) is `cannot_infer` today, *the
array literal is empty*, fix `guess`, so program C's `rows` line is counted,
not presumed, and the table's last row reads `cannot_infer`; and
`./heroes build examples/ledger/main.hero` was run after all, `wrote
ffi-probes/ledger_bin`, `real 1.17 user 0.84 sys 0.24`, so section 4's binding
is built, not carried.

## 8. Verdicts, one per route, from the C boundary

The boundary itself is the same under every route: no `extern` line changes
(section 5, 0 `=` in 697 files' groups), no emitted C changes for a cell of a
given type (sections 2A to 2C: `sqlite3 *`, `const char *`, `HeroArrayHeader
*` are the TYPE's, not the spelling's), clang's header verification is not
in the path. What differs is where a binding's error lands and whether the
compiler asks for the type it needs.

- **R0** approve. Every width and handle stands on the declaration line and
  `int_out_of_range` lands there (p6).
- **R1** **object**, not veto. On the 165 cells of the 21 FFI example files
  it drops a restated type from 137 and changes no meaning; but the 6 cells
  born `nullptr`, the shape of every out-parameter handle and of §4.19's own
  fence, are typed `ptr` by today's inference and refused at the C call with
  no fix and no word of the declaration (p1b, section 2A), and a buffer born
  from a numeric literal is `[i64]` and refused at its `counted_by` call
  (p7, section 2C). An error that moves from the wrong line to a right one,
  on the exact shape the FFI chapter teaches first, is design.md §4.17's
  promise broken at the boundary §1.11 calls the project's leverage.
- **R1b** **object** as drafted: 10 of the 12 never-mutated cells in the FFI
  examples are written only through `@name` (section 6), every lease among
  them by the spec's own rule; B2's sentence must count an `@` argument as a
  re-binding or it refuses the spec's § 13 program. With that sentence, and
  R1's condition below, approve.
- **R2** object. `=@` is the `named-arg-equals` mutant beside an `@`
  argument (section 5, p9), one keystroke from `name: @x` at 500 sites of
  `examples/`; R1's `@=` stands on no such path.
- **R3**, **R4**, **R5** object by inheritance: each is R1's inference with
  another spelling, so each carries the `nullptr` finding; none touches the
  boundary. `@@` also collides with `at_prefix.hero:63`'s recovery.
- **R6** object on design.md §4.19's first sentence, *treat its ergonomics as
  a priority*: the 21 FFI files hold 85 inferred `=` bindings, 17 of them the
  result of an extern function whose type the group's line already states and
  clang already verifies (`grep -cE '^[[:space:]]+[a-z_]+ = (<the group's
  functions>)\('`). Writing C's result type a second time at every call is
  glue, and it opens no hole, so it is a cost with no purchase.
- **R7** object: `db @ nullptr` becomes a declaration, so the typo §4.4 names
  declares a cell again, and the `nullptr` finding stands unchanged.
- **R8** object, and more firmly than R1: an `@` argument of a C call would
  type the cell (`db @= nullptr` then `sqlite3_open(…, @db)` giving `Db`), so
  a binding's wrong parameter width would land its error on a declaration
  above it or widen the cell in silence, against design.md `:1144`'s *errors
  stay local*; a cell handed to two parameters of different handle types has
  no type at all.
- **R9** approve. Nothing at the boundary moves; it repairs a two-message
  mistake, the same class as `(@=x` today (p10).
- **R10** approve. `db: Db @= nullptr`, `md: [u8] @= [9, …]`, `b: u8 @= 200`
  keep every handle and width on the line, so every finding of sections 1 to
  3 is moot; the one item it owes is the message for `@=` where an argument
  stands (section 5), which replaces today's two with one.
- **R11** refuse, the critic's reason: `@v` before a name is occupied by
  `at_prefix.hero`'s recovery and its `fixedbugs-131` goldens.

**Veto: none.** No route breaks the C ABI (the emitted C of a cell is
determined by its type, measured on three programs), none changes the
extern group's grammar or clang's verification (section 5), and none makes a
wrong binding compile that today's form refuses: every type R1's inference
would give that differs from today's annotation (`ptr`, `i64`, `[i64]`) is
refused at the C call by `type_mismatch` before C runs (p1b, p3, p4b, p5,
p7). The cost is the error's distance from the mistake, which is an
objection and not a refusal.

## 9. Prediction, with its instrument

Run the compiler-engineer's R1 prototype, unchanged in its inference, on my
`ffi-probes/a_handle_R1.hero` and on `tests/harness/suite_special.hero`'s
`specs_program` with its main's line rewritten `db @= nullptr`: it exits 1
with `type_mismatch: expected Db, found ptr` at the `@db` argument of
`sqlite3_open` (line 11 of my file) and prints no `cannot_infer` at the
declaration (line 10). The `special` suite is the instrument for the second,
`heroes check` for the first. And after any migration of the tree to R1 that
drops only annotations today's inference reproduces, `grep -rcE
'^[[:space:]]+[a-z_][a-z0-9_]*: [A-Za-z]+ @= nullptr'` over `examples/` and
`tests/` reads 6 and 26, and `grep -rcE '^[[:space:]]+[a-z_][a-z0-9_]* @=
nullptr'` reads 0 and 0: not one `nullptr` cell loses its type. If the
prototype prints `cannot_infer` on line 10 with a fix naming `db: Db @=
nullptr`, the first prediction is false and my objection to R1 is discharged.

## 10. The condition that changes the verdict

R1, and R3 to R5 with it, become approve from this seat when: (1) a bare
`nullptr` as the first value of a `@=` declaration is `cannot_infer` at that
line, its message of `ok(`'s shape (*`nullptr` is every handle's null, and
which one comes from the context*) with a `guess` fix naming the annotation,
which costs no program in the tree (0 immutable `= nullptr` bindings, section
7); and (2) `@=` where an argument stands has one message with a `certain`
fix, `write @name`. Recommended beside them, not a condition: a
`type_mismatch` at an `@` argument of an extern call gains a `guess` fix
naming the cell's declaration line with the parameter's type (`declare md:
[u8] @= …`), so the error that R1 moves to the call points back at the line
that is wrong. R1b additionally needs its sentence to count an `@` argument
as a re-binding, as spec § 5 `:138` counts it as a use.

## 11. Unrun, in those words

- The R1 texts (`*_R1.hero`) were not compiled: no compiler in my copy reads
  `@=`; their per-line types are today's inference of the same values
  (section 1), each a probe that ran.
- `bump(@=n)`'s message under R1: unrun, no lexer; today's two messages are
  p10's.
- The 26 `@ nullptr` cells of `tests/` were counted, not classed by the
  parameter they feed.
- The ledger's emitted C for the `owned` cell was not read; the module
  builds (section 4's correction).

Finished 2026-10-10 at the `date` read after this write.
That read 2026-10-10 16:58:11 CEST; the sitting's 45 minutes ran to about 17:33, so this report is inside its box by about 35 minutes.
