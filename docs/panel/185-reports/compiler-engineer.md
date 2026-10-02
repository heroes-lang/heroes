# Panel 185, the compiler-engineer's report

Written as it goes. Tree: my copy of `03e70520` at
`<scratchpad>/185-compiler-engineer/`, compiler built from the seed there
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, real 4.12 s,
user 4.00 s). Tracked `.hero` files listed from the repository with
`git ls-tree -r --name-only 03e70520 | grep '\.hero$'`: 1,530.

Status: complete, 2026-10-02 at about 02:25; every question measured, and the
two runs still owed are named below the summary. `<ce>` below is my directory. No binary of panel 184 was run; its
prototypes were read and re-applied in my copy.

## Summary: verdict per route, and what it rests on

| question | route | verdict | rests on (measured here) |
|---|---|---|---|
| Q1 | (1b) refuse, say *macro*, name the shim | **approve (robust)** | built: macros named, every misspelling and the five population refusals unchanged; 62 + 13 + 16 code lines; one clang run on a refused build only |
| Q1 | (1d) the shim | approve, as (1b)'s repair | builds, prints 1; a wrong width refused `ffi_parameter_type` |
| Q1 | (1a) second round, `(T){0}` | object | clang on this Mac accepts `int64_t`, `uint8_t`, `double` for `WEXITSTATUS` through (1a)'s probe; `extern_probe.hero` at 297 of 300 |
| Q1 | (1a′) compiler-written wrapper | object | the wrapper accepts `int64_t`, `void *`, `double`; 4 of 4 macros unchecked, `FD_ISSET` included |
| Q1 | (1c) `macro function` | **veto** | a construct for parser, checker and emitter, to mark a hole the measurement says not to open |
| Q2 | (2a) the production follows the parser, `_ = e` admitted | **approve (robust)** | 0 files move; `_` admitted: 4 lines, every neighbour gets its block form's message |
| Q2 | (2b) the parser follows the production | object | 23 files stop, the compiler and the harness among them, 707 sites |
| Q2 | (2c) mutation in, loops out | object | 1 golden line moves; a class for no measured mistake |
| Q3 | (3a) on `Outcome.jumps`, built as (3x) | **approve (robust)**, with R4's seal | 0 files move; a55 to b8 run right; no level of depth lost on any shape legal today; `walk.hero` +20 |
| Q3 | (3b) refusal with a better message | object | refuses programs of one reading that build right under (3a) |
| Q3 | R4 as prototyped (not a route, its premise) | a defect to file | c1, c2, c3, c6 check clean then `build` exit 2; a 16-line seal repairs it, built and run |
| Q4 | (4a′) string clause of (4a), (4b) otherwise | **approve (robust)** | no probe gets a wrong program from `--apply`; 81 code lines |
| Q4 | (4a) three clauses | object | t7: certain, applied, prints `many` where `one` is meant |
| Q4 | (4b) always guesses | approve (conservative) | never wrong; one provably certain fix fewer |
| Q4 | (4c) premise stands | object | `--apply` writes 5 wrong programs today |
| Q5 | (5b) amended | **approve (robust)** | 0 false alarms, 2 true (one silent today), 0 files stop |
| Q5 | (5a) | object | 32 false alarms, 22 files stop, the compiler among them |
| Q5 | (5d) path-shaped holes | object | 27 false alarms, the same 22 files stop |
| Q5 | (5c) R2's note alone | object as the resolution | reaches 1 of 2 true sites; `resolve/state.hero` at 326 of 330 |

**The compiler's own tests**, `./heroes test selfhost-<route>/main.hero` with
my seed compiler, each prototype's tree: (1b) 993 of 993; (2a) with `_`
(`selfhost-2cu`) 993 of 993; (3x) 993 of 993; (3a) + R4 + seal (`3s`) 993 of
993; (5b) 993 of 993; **(4a′) and (4a) 992 of 993**: the one failure is
`selfhost/parse.hero:383`, *an arm opened by a minus set apart from its
literal is refused, and its fix parses*, which asserts one certain fix, and
whose first shape, `- 1 => 10` beside `_ => 20`, is t7's: the test pins the
wrong certainty, so every route but (4c) rewrites it.

**Two runs owed, not made**: the `emission` suite on R4's seal (`3s`), and
any of this on Linux x86-64 (`docker ps` read no container; none was started).

## Q1. A macro-only C name in an `extern` group

### Where it lives (read in my copy; lines by `wc -l`, code lines by `suite_layout`'s count)
- the probe line, `selfhost/emit/extern_probe.hero:171` `probe_line` (403
  lines, 297 code lines against §11's 300): `(void)(name)(a0, ...)`;
- the result assertion beside it, written as `HERO_RET_INT(name((int32_t)0))`
  (read off `--emit-c` of `probes/q1/shim/shim.hero`, line 29 of its output),
  which on macOS a macro taking `&(x)` refuses (*cannot take the address of an
  rvalue*, measured below as E1b);
- the call site, `t2 = hero_wexitstatus(t1);` (same output, line 105): an
  lvalue already;
- the message, `selfhost/emit/ffi_declared.hero:159` (288 lines, 189 code),
  reached through `selfhost/cli/produce.hero:270` `blamed` (359 lines, 286
  code) and `selfhost/emit/ffi.hero:178` `explain`;
- the second round that exists, `selfhost/cli/produce.hero:187-206`: a
  `struct_tags` set learned from round one's refusals and threaded through
  `selfhost/cli/assemble.hero` `round` (251 lines, 215 code) into the emitter.

### What clang checks through a macro-only name, measured on this Mac
`<ce>/q1x/macro-probe.sh`, Apple clang 21.0.0, the compiler's own flags
(`selfhost/cli/flags.hero:91-109`) and the probe's four pragma errors
(`extern_probe.hero:76-82`):

| unit | clang |
|---|---|
| E1 today's probe, `(void)(WEXITSTATUS)(a0)` | refused, *use of undeclared identifier* (no note that it is a macro) |
| E1b `WEXITSTATUS((int32_t)0)` | refused, *cannot take the address of an rvalue*, note *expanded from macro* |
| E2 (1a)'s probe, `WEXITSTATUS((int32_t){0})` | compiles |
| **E3, E4, E5 the same with `int64_t`, `uint8_t`, `double` declared** | **compiles, each** |
| E6 (1a')'s wrapper, `static inline int64_t w(int32_t a0) { return WEXITSTATUS(a0); }` | compiles |
| **E7, E8, E8b the wrapper with `int64_t`, `void *`, `double`** | **compiles, each** |
| E9 `(void)(htonl)(a0)` | refused, undeclared |
| **E11, E12 an `htonl` wrapper declared `int64_t`, `int32_t`** | **compiles, each** |
| **E13 an `FD_ISSET` wrapper with an `int64_t` descriptor** | **compiles** |
| control: the same `int64_t` handed to `__darwin_fd_isset`, the typed function `FD_ISSET` expands to (`sys/_types/_fd_def.h:108`), directly | **refused**, `-Wshorten-64-to-32` |
| E14 a `WIFEXITED` wrapper declared `int64_t` | compiles |
| E15 `#ifndef WEXITSTATUS` / `#error` | compiles (it is a macro) |
| E16 the same for the misspelling `WEXITSTATU` | refused, the `#error` |
| E17 (1a)'s probe with the misspelling | refused, *call to undeclared function* |

**So on this Mac, through every macro measured (4 of 4), a wrongly declared
parameter compiles under (1a) and under (1a'), a `double` and a pointer
included**: `WEXITSTATUS` and `htonl` cast their argument themselves, and
`FD_ISSET`'s conversion happens inside a system header's macro expansion,
where clang reports nothing; the same conversion written outside the macro is
refused. (1a') checks what (1a) checks, the brief says; measured, both check
nothing of the parameters here. A misspelled name stays refused under (1a)
(E16, E17), so the five `ffi_unknown_name` of the population would stay
refused. Linux x86-64 is *unrun* in this seat: glibc's `WEXITSTATUS` is
arithmetic on its argument, so what it checks there is a question.

The working repair, (1d), re-run here: `probes/q1/shim/shim.hero` with
`<ce>/heroes` prints `1`; `shim-wrong.hero` (`status: i64` against the shim's
`int`) is refused, `error[ffi_parameter_type]`. `probes/q1/macro.hero` is
refused today with *`sys/wait.h` declares no `WEXITSTATUS`*, which is false:
the header defines it.

### The adopted route, (1b), built and run
`<ce>/selfhost-1b/`, `heroes-1b` from my seed. After a round clang refused,
`cli/produce.hero` collects the names clang called *undeclared* in it and asks
clang once, in one unit of `#include`s and `#ifdef NAME` / `#warning` lines,
which are macros (`cli/macro_ask.hero`, new); `blamed` hands the answer through
`emit/ffi.hero` `explain` to `emit/ffi_declared.hero` `unknown_name`, which,
for a macro, says so (a new code in the prototype, `ffi_macro_name`; whether it
is a new code or `ffi_unknown_name` reworded is the sitting's) and names the
repair (1d) measured. A program that builds never pays the extra clang run.

| file | code lines | ceiling |
|---|---|---|
| `cli/macro_ask.hero` (new) | 62 | 300 |
| `cli/produce.hero` | 286 to **299** | 300 |
| `emit/ffi_declared.hero` | 189 to 205 | 300 |
| `emit/ffi.hero` | 194 to 194 (a parameter, and its tests) | 300 |

`build --emit-c`, base against (1b) (`HEROES_RUNTIME=<ce>/runtime`):

| case | base | (1b) |
|---|---|---|
| `probes/q1/macro.hero` (`WEXITSTATUS`) | 1, `ffi_unknown_name`, *declares no* (false) | 1, `ffi_macro_name`: *`sys/wait.h` defines `WEXITSTATUS` as a macro, not a function: a macro has no parameter types for clang to hold this declaration to, so an `extern` group cannot bind it*, with the shim as its note |
| `htonl` in `arpa/inet.h` (`<ce>/q1x/m1`) | 1, `ffi_unknown_name` | 1, `ffi_macro_name` |
| two macros in one group (m2) | 2 `ffi_unknown_name` | 2 `ffi_macro_name` |
| a macro and a misspelled function `abss` (m3) | 2 `ffi_unknown_name` | 1 `ffi_macro_name`, 1 `ffi_unknown_name` |
| a macro beside a real function (m4) | 1 | 1 `ffi_macro_name` |
| the misspelled macro `WEXITSTATU` (m5) | 1 `ffi_unknown_name` | 1 `ffi_unknown_name` |
| the population's five `ffi_unknown_name` (`docs/panel/175-briefs/popen_linux.hero`, `popen_windows.hero`, `tests/golden/fixedbugs/ffi-unknown-name.hero`, `fixedbugs-a-misspelled-handle-tag-stays-refused.hero`, `tests/golden/ir/owned-cell.hero`) | 1 each, `ffi_unknown_name` (1, 3, 1, 1, 1) | **the same**, each |

**The census, `build --emit-c` over the 412 tracked files with a line
beginning `extern `** (`<ce>/census/emit_census.py`, `emit-one.sh`'s shape,
my own base run rather than the critic's TSV): base 206 emit and 206 refused,
(1b) the same; the two TSVs are identical, and **the full stderr of all 412
differs on 0 lines**. The five `ffi_unknown_name` are the five the brief names.

A prototype's first build answered nothing: `publish.private_name` ends a
name in `.tmp` and clang reads a file by its suffix (measured, repaired by
appending `.c`). **Known gap of the prototype**: its note's shim template has
one parameter whatever the arity; the landing writes the declared parameters.

**Defect 145 rides the same unit, not (1a)'s round** (measured by hand with
clang 21 on `tests/golden/unsupported/fixedbugs-145-types.h`): one unit holding
`#ifdef WEXITSTATUS` / `#warning`, `(void)sizeof(anon_s);` and
`(void)sizeof(only_tagg);` answers all three in one run: the warning for the
macro, silence for the typedef, *use of undeclared identifier 'only_tagg'*
for the misspelling. Both are questions about what a header says of a name,
asked only after a refused round. Not built into the prototype.

### Verdicts
**Section**: design.md §4.19 (`:2229`, the FFI; panel 092's probe), §1.12
(`:571`, a boundary complete and defended); for (1c) Part 5 (`:2621`) and §1.7
(`:410`).
- **(1b), with (1d) as the repair it names: approve, as the robust route.**
  It keeps § 13's promise whole (*a parameter that disagrees is refused*):
  the shim is a C prototype, and `shim-wrong.hero` is refused for its width.
  It corrects a message that is false today. Frontend and driver only; one
  clang run, on a refused build only; 0 programs of the population move.
  design.md's four sentences promising macros (`:565`, `:645`, `:2265-2266`,
  `:2401-2402`) need correcting, for the spec-warden.
- **(1a): object.** Measured on this Mac, the check it would add holds the
  parameters of none of the four macros measured, a `double` and a pointer
  included, so § 13 would gain a parameter that is never checked, which is
  what panel 092 parenthesised the callee to prevent. Its cost lands in the
  emitter as well as the driver: the learned set threaded through every round
  like `struct_tags` (`cli/produce.hero`, at 286 of 300 code lines;
  `cli/assemble.hero`), a second probe form in
  `selfhost/emit/extern_probe.hero`, **at 297 of its 300 code lines**, so a
  seam is owed, the column arithmetic of `emit/probe_reply.hero` rebuilt for
  it, and the result assertion's argument rewritten (E1b). Unbuilt; the
  numbers are the files' and the decisive fact is clang's.
- **(1a′): object**, on the same measurement (E6 to E14): the wrapper checks
  what the macro checks, which here is nothing, and it adds an emitted
  function and a renamed call site to the backend.
- **(1c): veto.** A `macro function` member is a construct the parser, the
  checker and the emitter must all handle (design.md Part 5's line between
  core and sugar; §1.7: *this determines the size of your compiler*), added to
  mark a hole the measurement says not to open. My veto compels a written
  answer, not a stop.
- **(1d): approve** as the repair, measured working on `03e70520`; it costs the
  compiler nothing and needs no `heroes cc`, a `static inline` in a header
  being compiled with the program's own unit.

**Prediction**: with (1b) landed, `build --emit-c` over the tracked files with
an `extern` line moves exactly 0 files' exit and 0 of the five
`ffi_unknown_name` (no tracked file binds a macro); a golden binding
`WEXITSTATUS` reads `ffi_macro_name` on macOS and on Linux x86-64; checkable at
the landing batch and its Linux leg.
**Condition**: a macro measured on any supported platform through which (1a)'s
probe refuses a wrongly declared integer width; that would make (1a) a real
check there and move me to (1a) beside (1b).

## Q2. A one-line `match` arm

### Where it lives (read in my copy)
`selfhost/grammar_expr.hero` `arm` (`:1165`, file 1757 lines) reads the arm's
line with `statement`, the same function a block's lines go through, then asks
`selfhost/parse/arm_body.hero` (53 lines) `reject_declaration`, whose
`declared_name` matches every statement kind: `.bind` and `.declare` refused,
the other nine (`mutate`, `return`, `break`, `continue`, `assert`, `while`,
`for`, expression, error) through. `heroes grammar` prints the productions by
reading `spec/heroes-spec.md` at run time (`selfhost/cli/grammar.hero:38`;
`grep -rn 'Inline' selfhost/` finds only a comment), so a change to `Inline`
moves no compiler line.

### The routes, built and censused (`check --brief`, 1,530 files)

| route | compiler lines | files that change exit | sites |
|---|---|---|---|
| (2a) the production says *a statement on the arm's line, a binding excepted* | 0 | 0 (no compiler change) | |
| (2b) the parser refuses what `Inline` does not list (`mutate`, `while`, `for`) | `arm_body.hero` +13 (`<ce>/selfhost-2b/`) | **23**, every one 0 to 1, `selfhost/main.hero` and `tests/harness/main.hero` among them; 117 files' output moves | **707** distinct sites: 677 in `selfhost/`, 19 in `examples/`, 11 in `tests/` |
| (2c) `Inline` gains the mutation; `while`/`for` after `=>` refused | `arm_body.hero` +13 (`<ce>/selfhost-2cu/`, built with the `_` change below) | **1**, 0 to 1: `tests/golden/run/fixedbugs-139-value-arms-that-jump-or-give-a-value.hero:100:18` | 1 |

Builds: `heroes-2cu` and `heroes-2b` from my seed, exit 0 each. Probes on
(2c): a54, s01 to s03, s09 to s12 unchanged (check 0, run prints 5, 12, 12,
5, 11, 13, 7, `3 7`); a76, s07, s08 refused `loop_on_arm_line`; s04, s05, s13
still `declaration_in_arm`; s14 still `missing_body`.

### The question beside it: `.blue => _ = 0`
Today (`<ce>/heroes`, s06): *`_` would be bound where nothing can read it*.
**That message is false by spec § 5** (*`_` ... binds nothing*): the parser
reads `_ = e` as a `.bind` named `_` and `declared_name` does not ask the name
(`arm_body.hero:25-38`). The compiler's own source works around the refusal:
`selfhost/check/walk.hero:618-622`, `fall_through()`, *`_ = 0` is a
declaration the rule refuses*. Admitting it is 4 lines in `arm_body.hero` (the
name's text is `_`, return before refusing), in `<ce>/selfhost-2cu/`. Measured
there (`<ce>/q2x/`), every neighbour then gets the message its block form gets:

| shape on the arm's line | today | `_` admitted |
|---|---|---|
| `_ = 0` in a statement `match` (s06) | `declaration_in_arm` (false) | check 0, runs, prints 7 |
| `_ = 0` in a value `match` (u1) | `declaration_in_arm` | `no_value`, *this branch ends on a statement* |
| `_ = may(k: k)`, an `i64?` (u2) | `declaration_in_arm` | `discarded_failure` (§ 5's rule) |
| `_ = print(0)`, a `()` (u3) | `declaration_in_arm` | `bound_unit` (§ 5's *a `()` line refuses*) |
| `n = 0` (u4) | `declaration_in_arm` | `declaration_in_arm` |
| `_ = twice(k: k)` (u5) | `declaration_in_arm` | check 0, runs |

It moves no tracked file (the (2c) census above includes it).

### Verdicts
**Section**: design.md §4.7 (`:1208`; *an arm's body is one statement*,
`:1230`); spec § 5 for `_`; Principle 0 (CLAUDE.md § 2) for (2b).
- **(2a): approve, with `_ = e` admitted (4 compiler lines), as the robust
  route.** design.md §4.7 (`:1230`) and spec § 8's own prose (`:227`) already
  say *one statement*; the parser has read a whole `Statement` since panel 014
  (`grammar_expr.hero:1188-1193`, its comment). The production is the one
  artifact out of line, so the repair is the production. Cost: 0 lines for the
  production, 4 for `_`; 0 programs moved. *Conservative*: (2a) with today's
  `_` refusal kept and its message made true (also about 4 lines).
- **(2b): object** (not a veto: it adds no core construct and breaches no
  ceiling, which is all my veto covers). It breaches Principle 0 directly:
  the compiler stops checking itself (`selfhost/main.hero` 0 to 1, 677 sites in
  `selfhost/`), and the migration buys nothing a reader can name: `n @ 5` on
  the arm's line has one reading.
- **(2c): object.** One tracked line moves, and the shape it refuses
  (`.red => while n < 3` over an indented body) has one reading too; it is a
  new diagnostic class for a style preference, which design.md §1.7 prices as
  compiler size for no mistake caught. No `while`/`for` after `=>` was found
  as a mistake by any probe of this sitting.

**Prediction**: under (2a) with `_` admitted, the census of `check --brief`
over the tracked files at the landing commit moves 0 files against its parent,
and `selfhost/check/walk.hero`'s `fall_through` can be replaced by `_ = 0` on
the arm's line with `heroes test selfhost/main.hero` green; checkable at the
landing batch.
**Condition**: a measured mistake class (a blind reading or the instrument)
where a loop or a mutation on the arm's line is misread, which would make (2c)
worth its class.

## Q3. A value block whose last statement leaves on every path

### Where the refusal comes from (read in my copy)
A value block's last statement is checked as the block's value:
`selfhost/check/walk.hero` `block` (`:704`) hands its `want` to the last
statement only; `expression_statement` (`:893`) sends an `if` or `match` with
a `want` other than `nothing` to `valued_expression` (`:961`), which
synthesises or checks it as a value; its join then finds every branch jumped
and `join.refuse_leaving` (`selfhost/check/join.hero:116`) pushes `no_value`.
The same construct with `want: nothing` (a body run for effect) returns
`Outcome(jumps: all_jump)` (`walk.hero:912`, `:926`). So the refusal is the
last line read as a value where it is also a statement. Nothing in the lowering
refuses it: `selfhost/ir/flatten.hero:65` `block` stops at a terminated block,
and `selfhost/ir/emissions.hero:59` `seal_join` writes `.unreachable` for a
join no arm reaches.

### The prototype of (3a), on today's predicate (`Outcome.jumps`)
In `<ce>/selfhost-3a/`, built `./heroes build selfhost-3a/main.hero -o
heroes-3a`: real 88.44, user 78.73, sys 8.31 (load about 3.5).

| file | lines (`wc -l`) | code lines (`suite_layout`'s count) | decided ceiling |
|---|---|---|---|
| `selfhost/check/walk.hero` | 2355 to 2374 | 1836 to 1853 | 1870 |
| `selfhost/check/join.hero` | 190 to 198 | 106 to 112 | 300 |
| `selfhost/check/state.hero` | 258 to 266 | 210 to 218 | 300 |

What it is: `Checker` gains `value_tail` (where the `if`/`match` standing as
a value block's last line starts) and `tail_left`; `expression_statement`
routes that construct through a new `valued_tail`, its own function so that
`expression_statement`'s frame does not widen; `refuse_leaving`, when the
construct that every branch left is that tail, records it instead of
refusing; `valued_tail` then returns `Outcome(jumps: true)`, so the arm or
branch holding the block is a jumping branch and the join skips it, as it
skips a `return`. **Checker only: no lowering, IR, emitter or runtime line.
Not core** (Part 5: no construct is added; a refusal is lifted).

Probes, `check --brief` then `run` (`HEROES_RUNTIME=<ce>/runtime`):

| probe | base | (3a) |
|---|---|---|
| a55, a69, a73 | 1 `no_value` | 0; prints `1 2 20`, `1 20`, `1 1 20` |
| b1, b2, b3, b5 | 1 `no_value` | 0; `1 20` each |
| b6 (`break`/`continue`) | 1 | 0; `12` |
| b8 | 1 | 0; `1` |
| b4 | 1 `missing_match_arms` (a parse error, another shape) | same |
| b7 (`if` without `else`) | 1 `if_without_else` | same |
| c1, c6 (`exit`), c2 (`assert false`), c3 (`while true`) | 1 | **same**: not in today's predicate |
| c4, c5 | 0 | 0 (R4's shapes; R4 has not landed) |

Shapes beside them, mine (`<ce>/q3x/`), base then (3a):
- every arm of `x = match` a block ending in a leaving `match`/`if` (d1): two
  `no_value` inside, then **one** `no_value` at the binding's own `match`,
  which is the true place: nothing reaches the binding;
- `x: i64 = match` (the checked path, d2), `return if` (d4), a `while` with
  `break`/`continue` tails (d5), a `str?` function (d6), a record (d9), a
  variant case under `return match` (d10): 1 to 0, each prints the meant
  value (`2 20`, `1 20`, `16`, `zero many one`, `1 5`, `3`);
- `y = match j` every arm returning, inside an arm (d8): refused in both, a
  binding is not a tail;
- a statement after a leaving tail (d7): 0 in both, R4's shape.

### The census, every tracked file, `check --brief`, 3 jobs
`<ce>/census/base.json` against `<ce>/census/3a.json`: exits 705 at 0, 823 at
1, 2 aborts, identical; **0 of 1,530 files change exit**. One file's output
moves: `docs/panel/184-briefs/blind/task3b.hero`, aborting in both, *stack
exhausted in `checktable.ty_key`* becomes *in `checkwalk.user_call`*: the
frames moved. Depth is measured below.

### Depth: what (3a) costs the walks' frames
Lane flow's scripts (`<scratchpad>/lane-flow/depth.sh`, `depth_call.sh`),
copied into `<ce>/depth/` with one shape added, `tail` (value `match`es nested
as each arm's last line, every arm returning, the path `valued_tail` is on),
one working directory per compiler; the deepest nesting `check --brief`
survives (exit 0 or 1, never 134), bisected:

| shape | base (`<ce>/heroes`) | (3a) (`<ce>/heroes-3a`) |
|---|---|---|
| `if` statements | 195 | 192 |
| `match` statements | 163 | 161 |
| value `if`s | 123 | 121 |
| value tails (new) | 88 | 85 |
| `step(step(...))` | 96 | 95 |

1 to 3 levels, about 1.5 per cent. The `if` shape never reaches
`valued_tail`, so the loss there is on every path, and **`-fstack-usage`
names it** (`clang -I runtime -fstack-usage -c`, plain `-O0` as the seed is
built, on `seed/heroes.c` and on `heroes build selfhost-3a/main.hero
--emit-c`; `<ce>/frames/`):

| function (bytes) | base | (3a) |
|---|---|---|
| `checkwalk.synth` | 36,064 | 36,480 (+416) |
| `checkwalk.check` | 27,456 | 27,808 (+352) |
| `checkwalk.statement` | 26,512 | 26,944 (+432) |
| `checkwalk.expression_statement` | 11,280 | 11,328 (+48) |
| `checkwalk.valued_expression` | 6,688 | 6,832 (+144) |
| `checkwalk.arms` | 8,080 | 8,096 (+16) |
| `checkwalk.block` | 3,968 | 4,000 (+32) |
| `checkwalk.valued_tail` (new) | | 2,560 |

Every growth is a multiple of 16, the size of the two new `Checker` fields:
at `-O0` each `Checker`-typed temporary in a frame grows by them (in `synth`,
26 times 16).
Panel 184's R5 (ratified: the passes on a thread of 256 MiB) would make a
loss of this size immaterial; until it lands the loss is real.

### (3x): (3a) with no new field, built because the frames said so
`<ce>/selfhost-3x/`, `heroes-3x`: `Checker`, `join.hero` and `state.hero`
untouched; `valued_tail` (in `walk.hero` only) checks the construct, and if
the last diagnostic is a `no_value` at the construct's own span, which
`refuse_leaving` pushes last of all when every branch left, it withdraws that
one, records the construct as `()` and returns `Outcome(jumps: true)`. It
fails safe: should anything ever be pushed after that refusal, the refusal
stands. `walk.hero` 1836 to **1856** code lines (ceiling 1870), nothing else.

| | base | (3a) | (3x) |
|---|---|---|---|
| depth, `if` / `match` / value `if` / call | 195 / 163 / 123 / 96 | 192 / 161 / 121 / 95 | **195 / 163 / 123 / 96** |
| depth, value tails (legal only under the route) | 88 (refused) | 85 | 84 |
| census, files moved of 1,530 | | 1 (an abort site) | **0** |
| the 17 brief probes and my 10 | | as in the table above | **identical to (3a), every one** |

So (3x) costs no level of nesting on any shape a program can write today, and
moves nothing in the tree; it is the build I adopt for (3a). The seal belongs
to R4 and is independent of which (3a) is built; it was measured on (3a)'s
tree (`3s`). With R4 and the seal, `walk.hero` would read 1856 + 25 + 3 =
**1884, 14 over** its ceiling (arithmetic on measured parts; not built as one
tree).

### R4's consistency, measured by re-applying it
Panel 184's R4 prototype (its route `2d`, `<scratchpad>/184-compiler-engineer/tree2`:
`check/leaves.hero` 51 code lines, and edits to `walk`, `state`, `decls`,
`ir/lower`, `flow_errors`, `diag`) re-applied by hand on `03e70520`, twice:
alone (`<ce>/selfhost-r4/`, `heroes-r4`) and on top of (3a)
(`<ce>/selfhost-3r/`, `heroes-3r`). No binary of panel 184 was run.

| probe | base | R4 alone | (3a) + R4 | (3a) + R4 + the seal (`heroes-3s`) |
|---|---|---|---|---|
| a55, b6, d5, d10 | 1 `no_value` | 1 `no_value` | 0, runs right | 0, runs right |
| c4 (a statement after `return`) | 0 | 1 `unreachable_statement` | 1 | 1 |
| **c5** (a statement after a `match` every arm of which returns) | 0 | **0** | **0** | **0** |
| d7 (a statement after a leaving tail, in an arm) | 0 | 0 | 0 | 0 |
| **c1, c6** (`0 => exit(code: 3)`, a block ending in `exit`) | 1 | **check 0, then `run` exit 2**: *internal error: compiling the generated C failed: ... variable 'h2_r0' is used uninitialized* | same | 0; with `k: 0` exits **3** (c6 prints `0` first); otherwise prints `20` |
| **c2** (`0 => assert false`) | 1 | **check 0, run exit 2**, same | same | 0; with `k: 0` aborts **134**, *assert failed: false* |
| **c3** (a block ending in `while true`) | 1 | **check 0, run exit 2**, same | same | 0, prints `20` |

**So the brief's two questions, answered by running them.** (3a)'s *leaves on
every path* is `Outcome.jumps`, the predicate `missing_return` reads
(`selfhost/check/decls.hero:159-166`) and the one R4 widens with its three
path-enders: one predicate, so a value arm and a function's end can never
disagree in the checker. **R4 does not reach c5**: its refusal reads only the
three jump words (`leaves.refuses_after`, route `2d`, which is R4's text,
*after `return`, `break` or `continue`*); c5 is a reading R4 leaves legal.

**And a defect in R4 as prototyped, which (3a) does not cause and does not
cure**: R4 widens `jumps` for an `exit` call, `assert false` and an unbroken
`while true` wherever they stand, so a value arm or a value block's last line
made of one now leaves in the checker; the lowering seals only a function's
end (`ir/lower.hero` `close`), so the join reads a slot no path wrote, and
clang refuses the C at exit 2, the compiler blaming itself (CLAUDE.md § 7).
Measured on R4 alone: c1, c2, c3, c6. **The repair, built** (`heroes-3s`): the
checker records the statements its widening marks (`Checked.ends`, 4 lines in
`check/state.hero`, 3 in `check/walk.hero`), and the lowering ends a
value-wanting block after such a last line with `.unreachable`
(`ir/flatten.hero`, `sealed`, 9 lines; the IR already has the terminator and
the emitter already writes `hero_unreachable();`). Its C for c1:
`h_library_exit(t5);` then `hero_unreachable();`. It seals only a value
position after one of the three path-enders, a shape today's checker refuses,
so I expect it to move no blessed emission: **an inference, unrun** (the
`emission` suite was not run on `heroes-3s`; the landing owes it). The census
below says what it moves in `check`.

### Lines, every tree (`suite_layout`'s count)

| module | ceiling | base | (3a) | R4 | (3a) + R4 + seal |
|---|---|---|---|---|---|
| `check/walk.hero` | 1870 | 1836 | 1853 | 1861 | **1881, 11 over** |
| `check/state.hero` | 300 | 210 | 218 | 220 | 234 |
| `check/join.hero` | 300 | 106 | 112 | 106 | 112 |
| `check/decls.hero` | 300 | 235 | 235 | 237 | 237 |
| `check/leaves.hero` (new) | 300 | | | 51 | 51 |
| `ir/lower.hero` | 300 | 176 | 176 | 183 | 183 |
| `ir/flatten.hero` | 1150 | 1140 | 1140 | 1140 | 1149 |
| `flow_errors.hero` | 300 | 183 | 183 | 196 | 196 |

**(3a) with R4 breaches `walk.hero`'s decided ceiling by 11** (my prototype's
comments included, which the count counts). These columns are (3a)'s first
build, with its two `Checker` fields; the build adopted below, (3x), puts
`walk.hero` at 1856 alone and touches neither `state.hero` nor `join.hero`, so
with R4 and the seal it reads 1884, 14 over. `valued_tail` calls
`valued_expression`, so it cannot leave `walk.hero` without a module cycle; R4's
`ends` records and its `broke` bookkeeping can. A seam or a new ceiling is owed
at the landing, with its reason.

### The census, every tracked file, `check --brief`

| compiler | exits 0 / 1 / abort | files moved against base | against R4 alone |
|---|---|---|---|
| base | 705 / 823 / 2 | | |
| (3a) | 705 / 823 / 2 | **0 change exit**; 1 output (a depth probe's abort site) | |
| R4 alone | 700 / 828 / 2 | 8 | |
| (3a) + R4 + seal | 700 / 828 / 2 | 8, the same 8 | **0 change exit**; 2 outputs (the two depth probes' abort sites) |

R4's 8, read: 5 go 0 to 1, every one a dead statement panel 184 wrote on
purpose (`docs/panel/184-briefs/probes/after-return.hero:3`,
`after-bare-return.hero:4`, `after-break.hero:7`, `after-continue.hero:7`,
`docs/panel/184-briefs/blind/task2.hero:7` and `:17`); 1 is a depth probe; and
**2 goldens of defect 139 move, exit 1 to 1**:
`tests/golden/check/fixedbugs-139-a-constant-whose-body-ends-on-a-statement.hero`
(lines 17 and 20 keep `no_value`, worded as *its body jumps before it gives
one*), and
`tests/golden/check/fixedbugs-139-a-value-arm-that-ends-on-a-statement.hero`,
where **four `.blue => assert false` value arms (lines 21, 28, 35, 49) stop
being refused**: that golden pins as an error what R4's predicate makes legal.
So R4's landing owes that golden a hand rewrite (`tests/golden/check/` forbids
regeneration) and the seal above, without which those four arms are c2: check
clean, then exit 2. On `03e70520` R4 no longer stops the compiler checking
itself: `selfhost/main.hero` stays 0 under R4 (panel 184 measured 51 files
and 136 sites in `selfhost/` at `a294a6ff`, under its route `2a`, which also
refused after a leaving `match`; R4 is `2d`).

### Verdicts
**Section**: design.md §4.7 (`:1208`, the jumping arm, panel 017 R1); Part 5
(`:2621`) for what is core; §1.12 (`:571`) for R4's exit 2.
- **(3a): approve, as the robust route, on today's predicate, built as
  (3x), with R4's seal landing with R4.** Checker only (`walk.hero` +20 code
  lines, nothing else); not core: it adds no construct and lifts a refusal of programs with
  one reading; 0 tracked programs change exit, on the trunk and on R4; one
  predicate with `missing_return` and R4, measured on the brief's 17 probes and
  10 of mine (`<ce>/q3x/`). Its cost, as (3x): no
  level of depth on any shape legal today, 0 files moved; with R4 and the
  seal, `walk.hero` 14 lines over its decided ceiling.
- **(3b): object.** It keeps refusing a55, a69, a73 and b1 to b8, programs
  whose meaning no reader of this sitting disputed and which build and run
  right under (3a); a better message for a refusal with no second reading is
  the cheaper route, not the more robust one.
- **On R4, not this sitting's route but its premise**: R4 as prototyped makes
  a check-clean program fail at `build` with an internal error (c1, c2, c3,
  c6; measured alone). That is a defect in a ratified, unlanded resolution,
  and I would file it before R4 lands: *the widened predicate reaches value
  positions, and the lowering seals only a function's end*. The seal is 16
  lines, built and run here.

**Prediction**: with (3a) and R4 landed together with the seal, the census of
`check --brief` over the tracked files changes exit for exactly the files R4
alone changes (today 5, all under `docs/panel/184-briefs/`), and c1, c2, c3,
c6 and a55 to b8 (b4 and b7 excepted) build and run; checkable at the landing
batch. Without the seal, c1, c2, c3 and c6 `build` at exit 2.
**Condition**: a program of one reading that (3a) accepts and the lowering
builds wrong, or a measured depth loss above 5 per cent on a platform after
R5.

## Q4. The certain `-` in a pattern

### Where it lives
`selfhost/parse/arm_line.hero` `spaced_sign` (74 lines, 66 code lines), called
at an arm's first pattern from `grammar_expr.hero:1169`; its fix is
`list_line.against_its_value` (`selfhost/parse/list_line.hero:150`) with
`certainty` set to `.certain` (`arm_line.hero:50`). Parser only: no checker,
lowering or emitter line under any route; not core.

**What it reaches in the tracked tree**, from the base census's diagnostics:
6 sites, all in one golden,
`tests/golden/check/fixedbugs-a-match-arm-opened-by-a-spaced-minus.hero`
(lines 19, 21, 24, 32, 51, 53), whose `.fixed` the `fixes` suite holds. Under a
route that leaves some of its fixes `guess`, that case's answer becomes an
`.applied` (`tests/harness/suite_fixes.hero:30-43`: the pinned bytes of
`--apply`, which do not check clean), and under one with no `certain` fix its
`.fixed` goes.

### The routes, built
`<ce>/q4-run.sh <compiler> <files>`: the fixes offered, then `check --apply`,
then the applied program's verdict and output. `HEROES_RUNTIME=<ce>/runtime`.

| probe (meant output) | base | (4a) `heroes-4a` | (4b) `heroes-4b` |
|---|---|---|---|
| s5a bulleted ints (`two`) | 3 certain; applied prints **`many`** | 6 guesses, nothing applied | 6 guesses |
| s5b one signed among plain (`minus one`) | certain; applied `minus one` | certain; applied `minus one` | 2 guesses |
| s5c bulleted strings (`2`) | certain; applied refused, 2 `bad_operand` | certain delete; applied prints **`2`** | 1 guess each (delete) |
| s5d bulleted chars (`2`) | certain; applied prints **`3`** | 4 guesses | 4 guesses |
| s5k a string alone | certain; applied refused `bad_operand` | certain delete; applied prints `1` | 1 guess |
| t2 bulleted, with `- _` | certain on the head; applied refused `expected_pattern` | 2 guesses (the `- _` is a neighbour) | 2 guesses |
| t6 `- 1 \| - 2` | certain; applied `many` | 2 guesses (the `\|` neighbour) | 2 guesses |
| **t7 one bulleted arm beside a plain `_` (`one`), mine** | certain; applied **`many`** | **certain; applied `many`** | 2 guesses |
| t8 bulleted arms over blocks, mine | 2 certain; applied `many` | 4 guesses | 4 guesses |
| t9 `1 \| - 2`, mine | silent, prints `many` | silent | silent |
| the defect-123 golden | 7 certain (6 arms, 1 lexer) | 3 certain, 8 guesses; applied still refused (4 sites) | 1 certain (the lexer's), 12 guesses |

**(4a)'s integer clause is falsified by t7**: in a `match` with one literal
arm there is no neighbour to read, and a bulleted `- 1 => "one"` beside `_`
takes the certain fix and prints `many` where `one` is meant. The clause
decides certainty by the spelling of the arms around it, which is evidence and
not a proof; design.md §4.10 (`:1341-1343`) states the rule this breaks: *a
repair that chooses between the readings whose divergence is the whole reason
for the refusal cannot be certain; two readings make two guesses*. Before an
integer and before a character both readings compile; before a string only one
does, and both readings write the same program, so deletion is certain there.

Costs (`suite_layout`'s count of `arm_line.hero`, ceiling 300): (4a) 66 to
144 (the neighbour scan is 60 of them); (4b) 66 to 73; **(4a′), (4a)'s string
clause with (4b) for integers and characters**, 66 to 81
(`<ce>/selfhost-4ap/`).

The compiler's own tests pin today's answer: `selfhost/parse.hero:383-400`
asserts one `certain` fix for seven shapes, the first of them t7's; under
(4a′) and under (4a) it fails (992 of 993), and the landing rewrites it.

Census, `check --brief` over the 1,530 files, `heroes-4ap` against base:
exits identical (705, 823, 2); 1 file's output moves, the defect-123 golden
(its message text). No other tracked file opens an arm with a spaced `-`.

(4a′), `heroes-4ap`, on the same 19 probes: s5c and s5k take the certain
deletion and the applied programs print `2` and `1`, the meant ones; every
integer and character site gets two guesses (s5a, s5b, s5d, s5e, s5f, s5h to
s5j, t2, t5 to t8) and nothing is applied; the golden keeps one certain fix
(the lexer's, `split`'s lone `-`) beside 12 guesses. **No probe of this
sitting gets a wrong program from `--apply` under (4a′)**; under the base 5 do
(s5a, s5d, t5, t7, t8, each meant as a bulleted list; t6's meaning is
ambiguous) and under (4a) 1 does (t7).

### Verdicts
**Section**: design.md §4.15 (`:1911`, the spaced sign, defect 123), §4.10
(`:1341-1343`, two readings make two guesses), §4.17 (`:2073`, `certain` is
machine-applicable).
- **(4a′): approve, as the robust route**: (4a)'s string clause, and (4b) for
  integers and characters. It is §4.10's rule applied where it binds, and it
  keeps `certain` exactly where one program is the only one the arm can mean.
  81 code lines of `arm_line.hero`, parser only; one golden's answer moves from
  `.fixed` to `.applied`; spec § 8's `Pattern` should stop listing `[ "-" ]
  string` (`spec/heroes-spec.md:244`), which the checker already refuses
  (`bad_operand`, s5c base), a sentence for the spec-warden to price.
  *Conservative*: (4b) everywhere, 73 lines, strings a guess too.
- **(4a): object.** Its integer clause decides certainty from the neighbours'
  spelling, and t7 is a bulleted arm with no neighbour: certain, applied, wrong.
  It also costs 60 lines of text scanning for that clause.
- **(4b): approve as the conservative alternative**: never wrong, one certain
  fix fewer than (4a′) (before a string, where it is provably certain).
- **(4c): object.** The premise is measured false five times over in this
  sitting's probes under today's compiler: `--apply` writes a program that
  prints the wrong answer (s5a `many`, s5d `3`, t5 `many`, t7 `many`, t8
  `many`), and CI's `fixes` suite checks only that it compiles.

**Prediction**: under (4a′), `heroes check --apply` on every probe of
`docs/panel/185-briefs/probes/q4/` and on t7 writes no program that prints
other than its `_meant` file (where one exists) or that it printed before;
checkable at the landing batch by the `fixes` suite plus those probes run.
**Condition**: a measured case where a `-` before a string means something
other than nothing, or one where a spaced `-` before an integer has only one
reading the compiler can prove; either would change one clause.

## Q5. The forgotten `f`

### What I re-applied, and where it lands
Panel 184's prototype files, read in `<scratchpad>/184-compiler-engineer/tree`
against its untouched `corpus` (`diff -rq`): `selfhost/plain_holes.hero` and
`selfhost/resolve/plain_literal.hero` (new), the `.str_lit` arm of
`selfhost/resolve/walk.hero` and one code in `selfhost/diag.hero`'s
`is_thesis_rule`. Copied into three trees of `03e70520` and built from my seed
(no binary of panel 184 run): `<ce>/selfhost-5b/` as it was (at least one
name, every name bound where the literal stands), `<ce>/selfhost-5a/` with the
scope question removed (`if true`), `<ce>/selfhost-5d/` refusing where a hole's
root is a path, a new `is_path` (a name, `.field`, a method or a call on one;
anything else, `{0}`, `{b + 1}`, `{xs[0]}`, is not). All frontend, none core:

| file | code lines (`suite_layout`'s count) |
|---|---|
| `selfhost/plain_holes.hero` (new) | 124 (with `is_path`, about 12 of them, only (5d) needs) |
| `selfhost/resolve/plain_literal.hero` (new) | 41 ((5a) and (5d) need about 20 fewer: no scope question) |
| `selfhost/resolve/walk.hero` | 254 to 256, ceiling 300 |
| `selfhost/diag.hero` | 127 to 128, ceiling 300 |

Depth, lane flow's scripts on `heroes-5b`: `if` 195, value `if` 123,
`step(step(...))` 96, the base's three numbers: the resolver's new call costs
no level.

The census tool, `<ce>/selfhost-5d/zz_braces.hero`, built on the same
`plain_holes` so the rule and the count read holes alike: every plain literal
of the 1,530 files whose text, with an `f` before it, holds a hole the
compiler's own parser reads as one expression (3.71 s real). **35 literals, all
where an expression stands; 30 hold a path-shaped hole.**

### The census, `check --brief` over 1,530 files, base against each route
Sites deduplicated by `path:line:col` across every root that reaches them.

| route | files that change exit | sites | true | false |
|---|---|---|---|---|
| (5a) any hole that parses | **22**, every one 0 to 1: `selfhost/main.hero` and 18 more of `selfhost/`, `tests/harness/main.hero`, `suite_probe.hero`, `suite_surface.hero`, `examples/template/main.hero` | 34 | 2 | **32** |
| (5b) at least one name, every name bound here | **0**; one file's output moves | 2 | 2 | **0** |
| (5d) a path-shaped hole | **22**, the same 22 | 29 | 2 | **27** |

The two true sites are panel 184's own blind task, tracked since that sitting:
`docs/panel/184-briefs/blind/task1.hero:13` (`"{count} rows"`) and `:14`
(`"total {total} over {rows.len()} rows"`). **Today the compiler says
`unused_binding` for `count` at 6:5 and nothing at all about line 14**
(`total` is read by its own mutation). Under (5b) both are told at the
literal and the misleading `unused_binding` goes (the hole's names are counted
as read).

**Every false alarm, read and named** (the tool's listing,
`<ce>/census/braces.tsv`):

| literals | where | what it is | (5a) | (5d) | (5b) |
|---|---|---|---|---|---|
| 20 | `examples/template/main.hero:145, 146, 150, 153, 159, 163, 164, 165, 166, 178, 186, 200, 201, 202, 210, 218, 222, 223, 234, 235` | a template engine's input: `{name}`, `{role}`, `{nobody}`, `{anything}` are the program's own placeholders, map keys, no binding | false | false | spared |
| 5 | `selfhost/emit/assert_spelling.hero:188, 192, 277, 278`, `selfhost/emit/body.hero:182` | C text, C's `{0}` initialiser | false | spared | spared |
| 3 | `selfhost/escape_readings.hero:306` (two), `:307` | Heroes source inside the compiler's tests | false | false | spared |
| 1 | `selfhost/lex_interp.hero:149` | a test pinning today's `{x}}` (R1 changes it anyway) | false | false | spared |
| 2 | `selfhost/lexer.hero:452`, `selfhost/next_line.hero:314` | test programs held as strings | false | false | spared |
| 1 | `tests/golden/check/fixedbugs-135-a-backslash-before-a-bracketed-expression.hero:33` | a golden's brace on purpose; its file stops at the lexer's `unknown_escape`, so the resolver placement never reaches it (34 sites against 35 literals) | false (not reached) | false (not reached) | spared |
| 1 | `tests/harness/suite_surface.hero:463` | a test's text naming a hole | false | false | spared |

(5d) spares the emitter's five and keeps the compiler's own tests: the files
that stop are the same 22, so it buys nothing over (5a) where Principle 0
looks. Panel 184's figure was *34 false and 0 true* at `a294a6ff`; here, at
`03e70520`, 32 and 2 (re-measured, not carried): the tree gained the blind
task and lost `selfhost/emit/extern_record.hero:175`.

**(5c)**, R2's note alone, was not built. Placement read: the unused sweep,
`selfhost/resolve/state.hero` `report_unused`, at **326 code lines against its
decided 330** (`tests/harness/suite_layout.hero:450`), so it needs a cut or a
new ceiling. Its reach is measured by the base census above: it would speak at
task 1's line 6 and not at all about line 14.

### What (5b) owes R1
`with_f`, the fix's text (`<ce>/selfhost-5b/plain_holes.hero:117-136`),
doubles a text `{` only. Under R1 (ratified, not landed) a lone `}` in an `f`
literal is an error, and measured with `<ce>/heroes-5b` on `print("{x} and
{K: V}")`, the fix offered is `f"{x} and {{K: V}"` and its note says
`f"{{x}"` prints `{x}`; both are refused once R1 lands: the fix must double a text `}` too, a
few lines, and R1 lands first (its own bootstrap order).

### Verdicts
**Section**: design.md §1.3 (`:206`, as the author ruled it on 2026-10-01),
Part 7 item 7 (`:3068`, panel 121's refusal of an active brace without a
prefix, met: (5b) activates nothing, it refuses); Principle 0 for (5a) and
(5d).
- **(5b), amended: approve, as the robust route.** 0 false alarms and 0 files
  moved over the tree; both true sites caught, one of them silent today; 168
  frontend lines; no core construct (Part 5); the compiler is untouched by it.
  The blind reading's objection is to locality of compiling, which the
  author's ruling of 2026-10-01 put outside §1.3; it is not a cost I can
  measure in the compiler.
- **(5a): object.** 32 false alarms, and the compiler stops checking itself
  (`selfhost/main.hero` 0 to 1) until 12 of its literals are respelled:
  Principle 0.
- **(5d): object.** 27 false alarms and the same 22 files stopped.
- **(5c): object as the resolution** (it lands anyway as R2): it reaches one of
  the two true sites and sits at its file's ceiling.

**Prediction**: with (5b) landed after R1, the census of `check --brief` over
the tracked files moves exactly the files holding a plain literal whose hole
names a binding in scope, today 1 (`docs/panel/184-briefs/blind/task1.hero`),
and 0 changes exit; checkable at the landing batch.
**Condition**: a tracked program, or a measured model output, where (5b)
refuses a literal meant as text with a name bound in scope; one such class in
the instrument's corpus would move me to (5c) with R2.
