# Panel 221, compiler-engineer: a record constant whose header value is no constant

Seat: compiler-engineer (the ceiling: design.md §1.1, §1.7, Part 5). Written 2026-10-17 as the
work went. Worktree `.claude/worktrees/221-compiler-engineer`, detached at `bc978b09d`, the lane
`lane-land215`'s uncommitted C2 files copied in by `cp` (the fifteen paths its `git status
--short` names). Compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed`
(real 8.87), then `./heroes-seed build selfhost/main.hero -o heroes`. Scratch:
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad/221-ce/`.

## 0. What was read

- `docs/panel/063-the-direction-nobody-checked.md` condition 1 and R6: the struct `constant` hole
  written as a known cost, owed *a falsifiable claim with a test that fires when it dies*.
- Panel 218 (ratified 2026-10-17), R1 (C2), R2 item 3 (*a call leaf runs under the checks*), R4's
  third bullet (this defect, filed apart).
- `selfhost/emit/assert_spelling.hero:143` `asks_for_a_value` (`.named => false`);
  `selfhost/emit/extern_assert.hero:209-224` `constant_assertions` (the scalar's
  `_Static_assert(__builtin_constant_p(NAME), "heroes-ffi-const NAME")`);
  `selfhost/emit/ffi_declared.hero:86-115` `not_constant` (the `ffi_not_constant` it reads back);
  `selfhost/emit/record_constant.hero` (the record accessor, `hero_constant_value = NAME;`);
  the lane's `selfhost/cli/value_init.hero` (C2's walk), `clang_tree.hero` (the reader),
  `value_c2.hero` (the step), `value_told.hero` (the telling).

## 1. The shapes, and three predicates asked of C itself

The header is `scratchpad/221-ce/progs/shapes.h`: 37 record constants of five struct shapes (`pt`,
a nested `line`, a float `v2`, a pointer pair `pp`, an array field `arr`) and 27 scalar twins
(`#define S_X (leaf)` beside `#define R_X { leaf, 2 }`). The leaves: a literal, an enumerator,
the defect's counter `g_next()`, a call returning a literal (`g_ok`), an `__attribute__((const))`
call, `__builtin_abs(-3)`, `static const` and `static` and `static const volatile` objects, `sizeof`
of a call, a call in the dead arm of `?:`, a comma, a statement expression with and without a
call, `errno`, `rand()`, a member of a const and of a mutable struct object, and seven adversaries
aimed at a classifier of node kinds: `*k_holder.p` (a mutable object read through a const object's
pointer), `0 && g_next()`, `__builtin_choose_expr(1, 3, g_next())`, `k_arr[1]`, `*k_ip`, an
`extern const` object, `__builtin_constant_p(g_next()) ? 1 : 2`. Whole-record spellings: raylib's
`(struct pt){ 1, 2 }`, the same with a call, `make_pt()` (the 094 golden's `PT_CALL`), a struct
returned by a counting call, a const and a mutable struct object; nested, array and pointer
brace lists; `INFINITY`, `HUGE_VALF`, `NAN`; `{ "abc", &g_obj }`; `{ getenv("HOME"), 0 }`.

**Predicates asked of clang directly** (`shapes/preds_fn.sh <clang>`, each inside a function, as
the accessor stands; `-std=gnu11 -fsyntax-only`):

- *scalar*: `_Static_assert(__builtin_constant_p(S_X), "")`, the scalar accessor's own question
  (`selfhost/emit/extern_assert.hero:220-223`);
- *static*: `static const T hero_v = R_X;`, C's rule for an object of static storage, C11 § 6.7.9
  p4 (*all the expressions in an initializer for an object that has static or thread storage
  duration shall be constant expressions or string literals*);
- *field bcp*: `_Static_assert(__builtin_constant_p(((T)R_X).field), "")` for every field.

Results, files `shapes/predsfn-apple21.txt`, `predsfn-debian22.txt`, `predsfn-ubuntu18.txt`:

- **static agrees with scalar on 27 of 27 leaf twins** on Apple clang 21.0.0, Debian clang
  22.1.8 (container `heroes-linux-arm64`) and Ubuntu clang 18.1.3 (container `ubuntu:24.04`, clang
  18 installed). The one leaf the clangs part on is `__builtin_abs(-3)`: kept by 21 and 22,
  refused by 18, **by the static rule and the scalar's builtin alike**, so parity holds inside
  every clang and the verdict moves between clangs exactly as the scalar's already does.
- **Apple 21 and Debian 22 identical on all 74 rows** (44 records, 30 scalars); clang 18 differs
  on `R_BI`/`S_BI` alone.
- **The one record shape where static and the scalar's question part**: `{ "abc", &g_obj }`, an
  address constant. Static keeps it; the scalar twin `constant S_ADDR: ptr` of `(&g_obj)` is
  refused today (`__builtin_constant_p` answers 0 for an address the linker places). Two reads of
  it give one address, so keeping it accepts no wrong program; it is the scalar that over-refuses.
- At file scope instead of in a function a statement expression is refused for standing at file
  scope (the first run, `preds-apple21.txt`): the question must be asked where the accessor is.
- *field bcp* agrees with static on every row but `P_ADDR`, and is not pursued: it can ask only
  the fields a binding names, so a `partial` record's unnamed bytes (`fixedbugs-094-the-systems-
  mutex-initialiser-builds-a-record` binds `record hero_mutex partial` over a whole
  `pthread_mutex_t`) stay unasked; what it does on a union is unrun.

## 2. What the compiler does today, and under each route

Every program built with `heroes build`, then run twice-reading, bounded 10 s and 400 bytes
(`scratchpad/221-ce/run.sh <compiler> <dir> [-O2]`). **Today** is the lane's compiler with C2,
`heroes-c2` (`bc978b09d` + the lane's fifteen uncommitted paths). **S** is route S as built in
section 3, `heroes-s`. The dump route's two columns are computed from clang's own JSON dump of
the same accessors (`shapes/classify.py`, run on Apple 21 and Debian 22 with identical verdicts):
*naive* refuses a leaf holding any `CallExpr` or a `DeclRefExpr` to a `VarDecl` that is not
value_init's `constant_object` (file scope, `const `, no `*`); *refined* exempts `__builtin_*`
callees, `sizeof` operands and `&` operands.

| shape | today (two reads) | scalar twin today | static, 21/22 | static, 18 | dump naive | dump refined | S as built |
|---|---|---|---|---|---|---|---|
| `R_LIT` | built: 1 1 | built: 1 1 | keep | keep | keep | keep | built: 1 1 |
| `R_ENUM` | built: 3 3 | built: 3 3 | keep | keep | keep | keep | built: 3 3 |
| `R_CNT` | built: 1 2 | refused `ffi_not_constant` | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_OK` | built: 7 7 | refused `ffi_not_constant` | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_CF` | built: 7 7 | refused `ffi_not_constant` | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_BI` | built: 3 3 | built: 3 3 | keep | refuse | refuse | keep | built: 3 3 |
| `R_KOBJ` | built: 5 5 | built: 5 5 | keep | keep | keep | keep | built: 5 5 |
| `R_KOBJX` | built: 6 6 | built: 6 6 | keep | keep | keep | keep | built: 6 6 |
| `R_GOBJ` | built: 5 5 | refused `ffi_not_constant` | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_VOBJ` | built: 5 5 | refused `ffi_not_constant` | refuse | refuse | keep | keep | refused `ffi_not_constant` |
| `R_SIZEOF` | built: 4 4 | built: 4 4 | keep | keep | refuse | keep | built: 4 4 |
| `R_TERN` | built: 2 2 | built: 2 2 | keep | keep | refuse | refuse | built: 2 2 |
| `R_COMMA` | built: 3 3 | refused `ffi_not_constant` | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_STMT` | built: 3 3 | built: 3 3 | keep | keep | keep | keep | built: 3 3 |
| `R_STMTCALL` | built: 1 2 | refused `ffi_not_constant` | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_ERRNO` | built: 0 0 | refused `ffi_not_constant` | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_CASTCALL` | built: 1 2 | refused `ffi_not_constant` | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_KPTX` | built: 7 7 | built: 7 7 | keep | keep | keep | keep | built: 7 7 |
| `R_GPTX` | built: 7 7 | refused `ffi_not_constant` | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_RAND` | built: 16807 282475249 | refused `ffi_not_constant` | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_CLIT` | built: 1 1 | - | keep | keep | keep | keep | built: 1 1 |
| `R_CLITCALL` | built: 1 2 | - | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_MAKE` | built: 5 5 | - | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_NEXTPT` | built: 1 2 | - | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_KPT` | built: 7 7 | - | keep | keep | keep | keep | built: 7 7 |
| `R_GPT` | built: 7 7 | - | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `L_CNT` | built: 1 2 | - | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `L_LIT` | built: 1 1 | - | keep | keep | keep | keep | built: 1 1 |
| `V_INF` | built: inf inf | built: inf inf | keep | keep | refuse | keep | built: inf inf |
| `V_LIT` | built: 1.5 1.5 | - | keep | keep | keep | keep | built: 1.5 1.5 |
| `V_HUGE` | built: inf inf | - | keep | keep | refuse | keep | built: inf inf |
| `V_NAN` | built: nan nan | - | keep | keep | refuse | keep | built: nan nan |
| `P_STR` | built: true true | built: true | keep | keep | keep | keep | built: true true |
| `P_ADDR` | built: true true | refused `ffi_not_constant` | keep | keep | refuse | keep | built: true true |
| `P_ENV` | built: true true | - | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `A_LIT` | built: 1 1 | - | keep | keep | keep | keep | built: 1 1 |
| `A_CNT` | built: 1 2 | - | refuse | refuse | refuse | refuse | refused `ffi_not_constant` |
| `R_THRU` | built: 5 5 | refused `ffi_not_constant` | refuse | refuse | keep | keep | refused `ffi_not_constant` |
| `R_SHORT` | built: 0 0 | built: 0 0 | keep | keep | refuse | refuse | built: 0 0 |
| `R_CHOOSE` | built: 3 3 | built: 3 3 | keep | keep | refuse | refuse | built: 3 3 |
| `R_KARR` | built: 2 2 | built: 2 2 | keep | keep | keep | keep | built: 2 2 |
| `R_KIP` | built: 5 5 | built: 5 5 | keep | keep | refuse | refuse | built: 5 5 |
| `R_EXTK` | refused `ffi_constant_type` | refused `ffi_not_constant` | refuse | refuse | keep | keep | refused `ffi_not_constant` |
| `R_BCPCALL` | built: 2 2 | built: 2 2 | keep | keep | refuse | refuse | built: 2 2 |

Read across: **today, all 44 record shapes build and 8 give two values on two reads** (`R_CNT`,
`R_CLITCALL`, `R_NEXTPT`, `R_CASTCALL`, `R_STMTCALL`, `R_RAND`, `L_CNT`, `A_CNT`); of their 27
leaf twins plus `S_INF`, `S_STR` and `S_ADDR`, 14 are refused and 16 kept. **S as built refuses 20
of the 44 and keeps 24, exactly the static column on clang 21, and agrees with the scalar twin on
27 of 27 leaves**, at `-O0` and at `-O2` alike (`run.sh ... -O2`: `table-s-O2.txt` identical to
`table-s.txt` on the first 60 programs, `table2-s-O2.txt` equal to `table2-s.txt` on the 14 adversaries). The dump
route, naive, disagrees with the scalar on 10 of 27 leaves; refined, on 8: it **keeps three the
scalar refuses** (`R_VOBJ`, volatile; `R_THRU`, a mutable `g_obj` read through `k_holder`, a const
object holding its address, whose two reads differ as soon as C writes `g_obj`; `R_EXTK`) and
**refuses five the scalar keeps** (`R_TERN`, `R_SHORT`, `R_CHOOSE`, `R_BCPCALL`, `R_KIP`), each a
call C never evaluates or a pointer clang folds. Naive also refuses `INFINITY`, `HUGE_VALF` and
`NAN` (`__builtin_huge_valf()` on Darwin, `__builtin_inff()` on glibc, `__builtin_nanf("")`) and
`P_ADDR`.

**What the dump route would have to become to agree.** Each of the eight is a rule of C11 § 6.6
or a clang folding extension: a volatile read, a dereference through an object, the unevaluated
operand of `?:`, `&&`, `||`, `__builtin_choose_expr` and `__builtin_constant_p`, the builtins
clang folds (which clang 18 and clang 21 do not agree on: `R_BI`), the const objects it folds. A
classifier that reads node kinds agrees with C only by re-implementing that evaluator in
`selfhost/cli/value_init.hero`, which is the route panel 218 R5 refused as *an evaluator of the
compiler's own (vetoed, §1.1, §1.11)*. Stopping short of it leaves `R_THRU`, a mutable global
behind a const pointer, accepted, which is defect 2116's own hole one indirection away.

## 3. Route S, built: C's own rule asked in the accessor

*As first built, with `static const`; the form that would land writes `static` alone and moves
the builder (sections 6b and 6c), with the same verdict on every program.*

**What it is.** `selfhost/emit/record_constant.hero:122` declares the accessor's one local
`static const <struct> hero_constant_value = NAME;` instead of `<struct> hero_constant_value =
NAME;`. C11 § 6.7.9 p4 then holds the header's value to a constant expression, judged by clang's
own constant evaluator in the program's own compile, at the constant's own `#line`. clang answers
`error: initializer element is not a compile-time constant` there for every shape whose scalar
twin `__builtin_constant_p` refuses (section 1). `selfhost/emit/ffi_value.hero`'s
`refused_value`, which already reads every error at a record constant's line (defect 094), tells
that one as `ffi_not_constant`, the scalar's class and the scalar's words, through a builder split
out of `ffi_declared.not_constant`, the scalar's, with one change: the function the note offers for a
record declares the value before returning it, since C takes a brace list only as an initialiser.
As the scalar's note stood, it offered `{ return R_CNT; }`, which is not C for a brace list.

**What it reads.** clang's words, `not a compile-time constant`, as `ffi_value.hero` already reads
`incomplete` and `[-Werror,` at the same line, and as `ffi_declared.hero` reads `static assertion
failed` (`ffi_lookup.FAILED`) for the scalar. Measured identical on Apple clang 21.0.0, Debian
clang 22.1.8, Ubuntu clang 18.1.3 and the Windows box's clang 23.1.1 (section 5). It never asks
`__builtin_constant_p` of a struct, and it parses no C.

**Where it runs.** In the program's own unit compile, which every build already runs. **No new clang
invocation, no new pass, no probe, nothing cold-only**: the verdict is the compile's, so it holds
warm and cold alike and at `-O0` and `-O2` alike (C's static initialiser is the front end's
constant evaluator, not the optimiser's; `table-s-O2.txt` equals `table-s.txt`). A refused unit
stops the round before panel 216's probe and C2's dump are asked (`selfhost/cli/assemble.hero:
216-237`, *told once the units compile*), so the dump never meets an initialiser it cannot read.

**Cost as built** (`git diff --numstat` in the worktree, counts in `layout`'s unit by
`scratchpad/221-ce/code_lines.py`, a mirror of `tests/harness/suite_layout.hero:966`'s
`code_lines` that agrees with the suite's own 303 below):

| file | at `bc978b09d`+C2 | as built | diff |
|---|---|---|---|
| `selfhost/emit/record_constant.hero` | 119 | 119 | 2 changed (the word, its unit test's needle) |
| `selfhost/emit/ffi_value.hero` | 201 | 212 | +13 (the constant, its comment, the branch) |
| `selfhost/emit/ffi_declared.hero` | 297 | **303** | +11 / -4 (the builder split out, `body` passed) |

**The first build breaches §11's ceiling**: `heroes run tests/harness/main.hero -- ./heroes-s layout
ffi_declared` reads *1 failed: selfhost/emit/ffi_declared.hero: 303 lines of code, past §11's
300* (base tree: 0 failed). The builder moves to `ffi_value.hero`, which has 88 lines of room;
section 6c has the move, measured.

**What it owes beyond the compiler's code**, counted and not yet written: the comments that become
false, `assert_spelling.hero:134-142` (*the constancy question stays unasked of a struct*),
`extern_assert.hero:212-215`, `record_constant.hero:44-46`, and panel 063 R6's test that fires when
its premise dies, which flips from *a struct cannot be asked* to *a struct is asked by its
initialiser*; the blessed emissions under `tests/emission/` holding `hero_constant_value`, 8 to re-bless
by one word and 1 to retire (the `emission` suite, section 6c; `grep -rl hero_constant_value
tests/emission tests/golden/emit` read 8 and 0 before the lane's 2109 emission entered); the
golden `run/fixedbugs-094-a-constant-the-header-spells-as-an-expression-still-builds`, whose
`PT_CALL` (`make_pt()`) is refused under S (section 4); and the `unsupported/` and `run/` cases for
2116's shapes. The seed changes only by the compiler's own code: the compiler binds no record
constant (`grep -c hero_constant_value seed/heroes.c` reads 1, the string constant itself).

**Core or sugar.** Neither. No construct, no checker rule, no IR, no lowering: one storage-class
word in an emitted declaration and one branch in a reader of clang's errors that already exists.
§1.7's question, *does it add a construct the type checker AND the lowering AND the backend must
all handle*, answers no.

## 4. The other routes

**D, the dump route (the brief's): refused, on parity and on the ceiling.** Section 2's columns are
its verdicts as clang's own JSON dump gives the nodes, with value_init's own `constant_object`
predicate. To agree with the scalar it must know C11 § 6.6's evaluation rules and clang's folding
extensions: volatile (`R_VOBJ`), a read through an object's pointer (`R_THRU`), the operands C never
evaluates (`?:`, `&&`, `__builtin_choose_expr`, `__builtin_constant_p`), the builtins clang folds
(which clang 18 and 21 disagree on, `R_BI`), the const objects it folds (`R_KPT`, `R_KIP`, and not
`R_EXTK`). Each is a rule of the evaluator; written in `selfhost/cli/value_init.hero` (281 of 300
lines by `layout`'s count, 19 lines of room) it is a second constant evaluator beside clang's, the
route panel 218 R5 vetoed (*an evaluator of the compiler's own (vetoed, §1.1, §1.11)*). Its cost
**unbuilt in Heroes**: the classifier, both predicates and the dump's index, is 47 lines of Python (`wc -l`) in
`scratchpad/221-ce/shapes/classify.py` and already misses eight; where it would land besides is a
placement, not a measurement: a new verdict kind through `value_c2.lines_of` (163) and
`value_told.initialiser` (267, 33 of room). And it runs only
where C2 runs: cold, on the program's own headers, behind `value_text.reaches`; S runs in every
compile.

**F, per-field `__builtin_constant_p`: refused.** Agrees with S on every row but `P_ADDR`, and can
ask only the fields a binding names: a `partial` record's unnamed bytes, where a header's call can
sit, stay unasked (section 1). One `_Static_assert` per leaf field, generated from the checker's
record descriptor recursively. Unbuilt in the compiler; that it is more code than S is an
inference from that shape, and that it asks less is by construction (a call in a byte no field
names is never asked).

**K, keep the hole written down (panel 063's status quo): refused.** 2116 is `blocking`; CLAUDE.md §
Precedence ranks 3 and 4 read together put *that* it closes beyond this seat's choice.

**M, memoise the first read (`static T v; static bool done;`): unbuilt, refused on parity.** It makes
two reads agree by keeping the first, so `R_RAND` becomes a random constant and `R_CNT` a constant
whose value depends on which read came first, where the scalar twins of both are refused. One
program, one rule for a group's `constant` (§4.19's *the header must hold a value, not an object*).

## 5. Four clangs, the real headers, and C2's own shapes

- **The 74-row predicate table on four clangs**: Apple clang 21.0.0 (this Mac), Debian clang
  22.1.8 (`heroes-linux-arm64`), Ubuntu clang 18.1.3 (`ubuntu:24.04`, clang 18 installed into a
  container of this seat's, removed at the end), and clang 23.1.1 on the Windows box (MSVC target,
  run in `/c/Users/Administrator/p221-ce`, removed after). Apple 21, Debian 22 and Windows 23 are
  identical on all 74 rows; clang 18 differs on `R_BI`/`S_BI` alone, for the record and the scalar
  alike. clang's words for the refusal are `initializer element is not a compile-time constant`
  on all four (read on one refusal each; `real/glibc.sh` on 18 and 22).
- **raylib 6.0** (`/opt/homebrew/include/raylib.h`): 26 `CLITERAL(...)` macros, all `Color`; the
  static rule keeps 26 of 26 with no warning under `-Wall`, and the program binding all 26
  (`real/colors.hero`) builds under `heroes-s` and prints the same 26 numbers as under `heroes-c2`.
  Panel 063's measured cost of asking a struct, *26 `CLITERAL(Color)` refused*, is 0 on this route.
- **SDL3 3.4.18**: no record macro (every `SDL_` macro of a brace list or a cast is a scalar;
  grep in `real/`).
- **Darwin's SDK**: 12 record initialisers (`PTHREAD_{,RECURSIVE_,ERRORCHECK_}MUTEX_INITIALIZER`,
  `PTHREAD_RWLOCK_INITIALIZER`, `PTHREAD_COND_INITIALIZER`, `PTHREAD_ONCE_INIT`,
  `OS_UNFAIR_LOCK_INIT`, `GSS_C_EMPTY_BUFFER`, `OS_ATOMIC_FIFO_QUEUE_INIT`, `ATOMIC_FLAG_INIT`,
  `IN6ADDR_ANY_INIT`, `IN6ADDR_LOOPBACK_INIT`): 12 kept. **glibc** (Debian 22, Ubuntu 18): 10
  record initialisers, the pthread family with glibc's `_NP` three, `ATOMIC_FLAG_INIT`, the two
  `IN6ADDR_*_INIT`: 10 kept on both. The list is this seat's, grepped from the SDK for `#define X {`
  and completed by hand with the `in6` pair; the ffi-pragmatist's populations are the census.
- **The Windows SDK** (the box, clang 23.1.1, `real/win.sh`, `win2.sh` with `winsock2.h` first, a
  folder of this seat's removed after): `SRWLOCK_INIT`, `CONDITION_VARIABLE_INIT`,
  `INIT_ONCE_STATIC_INIT`, `IN6ADDR_ANY_INIT`, `IN6ADDR_LOOPBACK_INIT`: 5 kept;
  `in6addr_any`, `IID_IUnknown`, `GUID_NULL` (extern const objects, `DEFINE_GUID` without
  `INITGUID`): refused, `not a compile-time constant`.
- **What S refuses in a real header**: the extern const objects `in6addr_any` and
  `in6addr_loopback`, on Darwin and glibc alike, and `IID_IUnknown` and `GUID_NULL` on Windows, for the reason a scalar `extern const` object is
  refused today (`R_EXTK` and `S_EXTK`, section 2). Their brace-list twins `IN6ADDR_ANY_INIT` and
  `IN6ADDR_LOOPBACK_INIT` stay, and so does the escape the note offers (below). No tracked program
  binds either (the census, section 6).
- **C2's own shapes are untouched**: seven leaves C leaves undefined (`1 << 31`, `INT_MIN / -1`,
  `INT_MAX + 1`, `(int32_t)1e20`, an enumerator over `1 << 31`, a `static const` holding `INT_MAX +
  1`, `-INT_MIN`) as record leaves and as scalars, 14 programs: under `heroes-c2` and `heroes-s` the
  same 14 refusals, `ffi_constant_type`, and **the diagnostics from `error[` on byte-identical, 14
  of 14** (`out3-c2/`, `out3-s/`). clang's static rule folds them (with its own `-Winteger-overflow`
  warning), so S does not take them from C2, and C2 still tells them in its own words.
- **What S does take from panel 218**: R2's item 3, *a call leaf runs under the checks*, for every
  call clang does not fold, since such a leaf no longer compiles and never reaches the probe. That
  is the scalar's order already: its `__builtin_constant_p` assertion stands in the accessor and
  refuses a call before panel 216's probe runs. The golden written for item 3,
  `unsupported/fixedbugs-2075-a-record-whose-leaf-calls-undefined-arithmetic-is-refused` (`{
  calls_divmin(), 2 }`), moves from `ffi_constant_type` to `ffi_not_constant`, the scalar twin's
  class.
- **The escape the note offers, built**: a header of the program's own with `static inline struct
  pt hero_R_CNT(void) { struct pt value = R_CNT; return value; }`, bound as `function hero_R_CNT()
  -> Pt`, builds under `heroes-s` and its two calls print `1` and `2` (`escape/`), a function
  honestly returning two values. The scalar's note as written offered `{ return R_CNT; }`, which C
  refuses for a brace list.

## 6. The corpus: which tracked programs bind a record constant

Enumerated from the world, not recalled: `git ls-files '*.hero'` (3,928 files at `bc978b09d`),
each file's `extern` blocks read for the `record`s they declare and the `constant`s typed by one
(`scratchpad/221-ce`, the inline scan), widened by a second scan of every `constant` whose type is
no scalar word, so a record declared in another module is not missed. **24 files bind 34 record
constants: 8 files and 14 constants under `tests/golden/run/` (the `fixedbugs-094-*` family), 7
and 9 under `tests/golden/unsupported/` (the `094` refusals and `2075`), 9 and 11 under
`docs/panel/` work folders (panels 178 and 194). None under `examples/`**, so no example program moves. Their header values,
read: literal brace lists, `PTHREAD_MUTEX_INITIALIZER`, `{0}`, a compound literal of literals,
`make_pt()`, `pt_origin` (a `static const` struct), and the deliberate type and value errors of
the `unsupported/` cases. Under S the one that changes verdict is `PT_CALL` (`make_pt()`), and the
one whose class changes is `2075`'s call leaf. The second scan also shows types named `int` (4)
and `Token` (1) under `tests/golden/`: written wrong on purpose or no record, not read further.

## 6a. Found beside the sitting: a record constant that hands back a dead frame

`scratchpad/221-ce/dangle/`: `struct pp { const char *s; int32_t *p; }` and `#define P_CLPTR {
"a", (int32_t[]){ 5 } }`, bound as `constant P_CLPTR: Pp`, read after one other call through
`static inline int32_t hero_deref(struct pp v) { return *v.p; }`. Inside a function a compound
literal has automatic storage (C11 § 6.5.2.5 p5), and today's accessor
(`struct pp hero_constant_value = P_CLPTR; return hero_constant_value;`, read with `--emit-c`)
returns the address of an array in its own frame.

- **Today** (`heroes-c2`): builds at exit 0 and reads **`1` at `-O0` and `1000` at `-O2`** where
  the header wrote `5`; under `--sanitize` with `ASAN_OPTIONS=detect_stack_use_after_return=1`,
  `AddressSanitizer: stack-use-after-return ... dangle.h:4 in hero_deref`, exit 134. A read of a
  dead frame: design.md §1.12's *must not corrupt memory*, CLAUDE.md § Precedence rank 3.
- **S** (`heroes-s`): refused, `ffi_not_constant`, exit 1. The address of an automatic object is no
  address constant, so C's static rule refuses it.
- **The dump route**, naive and refined: **keeps it** (`classify.py` on its dump: no call, no
  reference to an object). Field bcp refuses it.

It is not 2116's shape (no two reads differ by a call), so it is reported for the coordinator to
file with its own class; the seat's reading is `blocking` (a memory fault at exit 0). S closes it
by the storage class itself, with no rule about node kinds: the initialiser of an object of static
storage cannot take an automatic object's address. A route that keeps the automatic local closes it
only by a rule of its own for that shape, which the dump route as framed does not have.

## 6b. The golden forms, route S against the lane's tree

`unsupported` (`heroes run tests/harness/main.hero -- <compiler> unsupported`, each tree with its
own compiler, run side by side; files `suite-uns-base.txt`, `suite-uns-s.txt`):

- **base** (`221-ce-base`: `bc978b09d` + the lane's fifteen paths, its compiler from its own
  sources): **335 passed, 1 failed**, `fixedbugs-2109-a-value-clang-folds-in-an-initialiser-is-
  refused`, the lane's own case in progress. Its failure text is the same under S but for one
  build directory's hash, so it is not this seat's to read.
- **S with `static const`**: **333 passed, 3 failed**: that 2109 case; `fixedbugs-2075-a-record-
  whose-leaf-calls-undefined-arithmetic-is-refused`, predicted (section 5), now `ffi_not_constant`
  with the repaired note; and **`fixedbugs-094-a-field-the-struct-does-not-have-is-told`, not
  predicted**: clang's quoted words became *field designator 'z' does not refer to any field in
  type '**const** struct pt'*. The qualifier this seat added leaked into a message about another
  mistake.
- **The repair, measured before it is applied**: C's rule is the storage duration (§ 6.7.9 p4), not
  the qualifier. `static T hero_v = R_X;` without `const` gives the same verdict as `static const`
  on all 74 rows on Apple clang 21, Debian clang 22 and Ubuntu clang 18 (`shapes/preds_static.sh`,
  `predsstatic-*.txt` against `predsfn-*.txt`, three empty diffs). So route S writes `static`
  alone; section 6c re-measures it.

## 6c. Route S as it would land: `static` alone, the builder moved

A third detached worktree, `.claude/worktrees/221-ce-s2`, at `bc978b09d` with the lane's C2 files
(the lane committed them as `e92f33a2f` while this seat worked; that commit equals the copied
files but for `tests/harness/platform_skips.hero`, compared by `cmp` file by file), route S's
three files, `scratchpad/221-ce/move_builder.py` (the builder `told_not_constant` moved from
`ffi_declared.hero` into `ffi_value.hero`; `ffi_value` no longer uses `ffi_declared`, and walking
the `use` lines shows `ffi_value` does not reach `ffi_declared` without that line, so no cycle),
and `static` for `static const`. Compiler from the seed, then from its sources (`real 92.15`, the
machine at load 11 to 17 with other sessions' gates).

| file (`layout`'s unit) | at `bc978b09d`+C2 | route S | `git diff --numstat` |
|---|---|---|---|
| `selfhost/emit/record_constant.hero` | 119 | 119 | +2 / -2 |
| `selfhost/emit/ffi_value.hero` | 201 | 233 | +36 / -0 |
| `selfhost/emit/ffi_declared.hero` | 297 | 282 | +2 / -18 |
| **total** | 617 | 634 | **+40 / -20, net +17 in `layout`'s unit** |

`heroes run tests/harness/main.hero -- ./heroes layout <file>`: exit 0 for all three. Nothing in
`selfhost/cli/`, nothing in the checker, the IR, the runtime or the descriptors.

**Every program re-run with this compiler** (`scratchpad/221-ce`): the 74 shapes, sorted, equal
to route S with `static const` row for row (20 records refused, 24 kept); `-O2` equal to `-O0`;
the 14 undefined shapes refused as today, the diagnostics from `error[` on identical to today's
for 14 of 14; the dangling compound literal refused `ffi_not_constant`; raylib's 26 colours built,
output byte-identical to today's; the escape built, `1 2`.

**`run`, narrowed to `fixedbugs-094`**: 7 passed, 1 failed, the one predicted:
`fixedbugs-094-a-constant-the-header-spells-as-an-expression-still-builds`, whose `PT_CALL` is
`make_pt()`, a call, now `ffi_not_constant` with the repaired note. Its scalar twin is refused
today, `S_OK`. The case's own header says it pins *the spellings that built before the repair*
of defect 094, a fact about that repair and no ruling that a call must build; under S its
`PT_CALL` line becomes an `unsupported/` case of 2116, and `PT_LIT` and `PT_OBJ` (a `static
const` struct object, folded) stay.

**`unsupported` whole, this tree** (`suite-uns-f.txt`): **334 passed, 2 failed**: `fixedbugs-2109-
a-value-clang-folds-in-an-initialiser-is-refused`, the lane's own red that the base tree shares,
and `fixedbugs-2075-a-record-whose-leaf-calls-undefined-arithmetic-is-refused`, predicted (its call
leaf is now `ffi_not_constant`). `fixedbugs-094-a-field-the-struct-does-not-have-is-told`, red
under `static const`, is green: clang's words are back to `'struct pt'`. So against the base
tree's 335 and 1, route S moves exactly one `unsupported` case, the one it was meant to move.

**The compiler's own tests, this tree**: `./heroes test selfhost/main.hero`, **1706 tests, all
passed**, exit 0 (route S with `static const` in the first tree: the same, 1706, all passed).

**`run` whole** (`heroes run tests/harness/main.hero -- <compiler> run`, both trees side by side;
`suite-run-base.txt`, `suite-run-s.txt`; route S with `static const`, whose verdicts equal the final
form's on every program measured): **base 534 passed, 0 failed; S 533 passed, 1 failed**, the one
predicted, `fixedbugs-094-a-constant-the-header-spells-as-an-expression-still-builds` (`PT_CALL`).
Every other `run` golden, the seven other `094` cases and the lane's `2109` case among them,
builds and prints what it printed.

**`emission` whole, the final form** (`suite-em-f.txt`): **1289 passed, 9 failed**. Eight are
accessors whose blessed line gains the one word: **13 lines in 8 emissions, every pair `blessed`
/ `now` differing by `static ` and nothing else, and every file's line count unchanged** (no
`#line` moves; checked by script over the suite's output). The ninth is the `094` expression case,
now refused, whose emission the suite asks to retire. My first grep counted 8 emissions with an
accessor; the ninth, `run-fixedbugs-2109-a-value-c-defines-in-an-initialiser-builds.c`, entered the
lane's tree after that grep and binds two record constants.

**A clang that rewords the refusal** (`.claude/worktrees/221-ce-nobranch`, the final form with the
branch that reads `not a compile-time constant` removed): the same 20 of 44 records refused at
exit 1, every exit equal to the final form's on the 74 programs, each told `ffi_constant_type`
quoting clang (*clang refuses the header's value at line 29: initializer element is not a
compile-time constant*). So the premise that clang words it so, once dead, costs the class and
never the refusal: the loud direction.

## 7. The strongest case against route S, put by this seat

- **It reads clang's words.** `not a compile-time constant` is a premise about the world (CLAUDE.md
  § 11, `.claude/rules/module-shape.md`): four clangs say it today (section 5). Its death is
  loud by construction: the error stands at the constant's line whatever its words, and
  `ffi_value.refused_value` falls through to `told`, `ffi_constant_type` quoting clang (section
  6c's variant without the branch, run). So a clang that rewords it degrades the class, never the
  refusal. Owed: the claim written as such beside `NOT_FOLDED`, and a `unsupported/` case of 2116
  annotated `#~ ffi_not_constant`, which is the test that fires when the words move.
- **It refuses const objects the scalar also refuses** (`in6addr_any`, `IID_IUnknown`, `GUID_NULL`,
  `R_EXTK`): correct programs by §4.2's letter (a `const` object is not written), refused because
  C cannot read their value while compiling. That is the scalar's rule today
  (`docs/design.md:2599-2606`, `S_EXTK`), and one rule for both is the point; a ruling that const
  objects build would have to move the scalar's rule with the record's. The escape is the note's
  function, built (`escape/`).
- **It moves verdicts by clang version where the scalar already does** (`__builtin_abs(-3)`: clang
  18 refuses, 21, 22 and 23 keep, record and scalar alike). Not new, and not S's to fix.
- **It changes the emitted C of every record constant** (13 lines in 8 blessed emissions gain one word, 1 retires) and the
  storage of the value (static instead of automatic): the value is C's folded constant, so two
  reads return one object's bytes. What that does to a record's padding bytes is unmeasured here.
  The `lldb` suite pins frames and C names
  (`.claude/rules/verification.md`'s `selfhost/emit/**` row): **unrun** by this seat.

## 8. The seat's answer

| route | verdict | why, in one line |
|---|---|---|
| **S**, C's static-initialiser rule in the accessor, `static` alone | **approve** (the seat's recommendation) | the scalar's verdict on 27 of 27 leaves on four clangs; +40/-20 lines in `selfhost/emit/`, no pass, no probe, no class, no token; also closes 6a's dead frame |
| **D**, the dump route as a node-kind classifier | **object**; **veto** if it grows into what it needs | 8 of 27 leaves against the scalar even refined, `R_THRU` and `P_CLPTR` accepted; agreeing needs an evaluator of the compiler's own (panel 218 R5) |
| **F**, `__builtin_constant_p` per named field | object | asks only the fields a binding names; unbuilt |
| **K**, the hole kept, written down | object | 2116 is `blocking`: CLAUDE.md § Precedence ranks 3 and 4 |
| **M**, memoise the first read | object (unbuilt) | a record and its scalar twin would get two rules |

- `verdict`: **approve S** (`static` alone, the builder in `ffi_value.hero`); **object** D, F, K, M;
  **veto** D grown into an evaluator of C's constant expressions, the veto panel 218's
  compiler-engineer cast, held.
- `section`: design.md §1.1 (simplicity sets the ceiling), §1.7 and Part 5 (S is neither core nor
  sugar: no construct, no IR), §4.19 (`docs/design.md:2599-2606`, *the header must hold a
  **value**, not an object*, applied to a record unchanged), §1.12 (the class closes, and 6a's dead
  frame with it); CLAUDE.md § Precedence ranks 3 and 4. design.md's §4.19 names only
  `__builtin_constant_p` as the mechanism; the record's twin is a sentence it does not have yet,
  a design.md change this sitting carries.
- `implementation_cost`: **+40 / -20 lines, three files, all in `selfhost/emit/`**:
  `record_constant.hero` 119 code lines (unchanged), `ffi_value.hero` 201 to 233, `ffi_declared.hero`
  297 to 282, `layout` green on all three (section 6c; the patch is
  `scratchpad/221-ce/route-s.patch`). No new clang run, pass, probe, class or spec token. Beside it:
  three comments made false, panel 063 R6's test inverted, 8 emissions re-blessed (13 lines, one
  word each) and 1 retired, `run/fixedbugs-094-...-still-builds` split (`PT_CALL` to an
  `unsupported/` case), `unsupported/fixedbugs-2075-...` re-annotated `ffi_not_constant`, the 2116
  cases and 6a's. D, for comparison: unbuilt in Heroes, landing in `selfhost/cli/value_init.hero`
  (281 of 300) behind a classifier that is 47 lines of Python and still disagrees with the scalar
  on 8 of 27 leaves.
- `needed_for_self_hosting`: **no**. The compiler binds no record constant (`grep -c
  hero_constant_value seed/heroes.c` reads 1, the string constant itself). Rank 3 decides that the
  class closes; Principle 0 decides how, and S adds no form.
- `argument`: A record constant and its scalar twin should get one verdict. The scalar asks clang
  `__builtin_constant_p`, which answers 0 for every struct; C's rule for an object of static
  storage asks a record the same question through its storage class. On each of four clangs the
  two agree on 27 of 27 leaf twins (clang 18 parts from the others on one builtin, for both alike).
  The change is one word and one branch, inside the compile every build already makes, and keeps
  raylib's 26 colours and all 27 real initialisers measured. The dump route agrees only by becoming
  a second constant evaluator; short of that it accepts a mutable global behind a const pointer
  and a dead frame.
- `prediction`: When route S lands with 2116's repair, the repair's `git diff --numstat --
  selfhost/` reads **at most +60 / -30 lines, every one under `selfhost/emit/`**, with `layout` green
  on every file it touches. Against the merge base, `run` moves exactly **one** case (`094`'s
  `PT_CALL`), `unsupported` exactly **one** (`2075`), and `emission` exactly **8 files gaining one
  word on their accessor lines plus 1 retired**, with no other golden moving. raylib's 26 colours
  and the Darwin and glibc initialisers of section 5 build. Falsified by any file outside
  `selfhost/emit/`, by more than +60 lines, or by any other golden moving. Scored at 2116's closing
  batch gate.
- `condition`: The verdict on S turns to object on any one of three things. (1) A header value C
  defines as a constant (folded by clang, its scalar twin kept) that S refuses on any of the four
  legs: none in 74 shapes, 26 raylib macros, 27 real initialisers. (2) A real program the author
  rules must build that binds an extern const struct object (`in6addr_any`, `IID_IUnknown`): then
  the scalar's rule and the record's move together, never one without the other. (3) A
  `lldb`-suite failure from the static local, unrun by this seat. The objection to D lifts if D is
  shown to agree with the scalar on these 27 leaves and to refuse `R_THRU` and `P_CLPTR` without a
  constant evaluator in `selfhost/`.

## Cleanup

Removed when the work ended: the four detached worktrees `.claude/worktrees/221-compiler-engineer`,
`221-ce-base`, `221-ce-s2`, `221-ce-nobranch` (`git worktree remove --force`, the route's patch kept
as `scratchpad/221-ce/route-s.patch`), the container `p221-ce-u18` (`docker rm -f`), and the
Windows box's folder `/c/Users/Administrator/p221-ce` (twice, after each run; `ls -d` read *No such
file*). Every program, table and dump named above stays in
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad/221-ce/`.
