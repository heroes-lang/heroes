# Panel 232, the compiler-engineer's report

Written as it went, 2026-10-29, in the seat's own detached worktree
`.claude/worktrees/232-compiler-engineer/` at the frozen head `afe983265`,
its scratch `.claude/worktrees/232-compiler-engineer-scratch/`. Every number
is a command's, with the command; a negative claim says what was searched.
No clock was started: a cost is counted in emitted bytes, C lines,
instructions or lines of `selfhost/`, never in seconds.

The live compiler is `selfhost/` (Heroes) and `runtime/` (C); nothing here
reads the Rust tree at `m-bootstrap-archive` except where a module's header
names its port.

## Item 1, the order of evaluation

### 1.1 Where the lowering orders what it evaluates (read, then run below)

`selfhost/ir/flatten.hero`, 1,424 lines by `wc -l`. Every position is a
sequence of `expr(...)` calls in source order, each appending instructions to
the current block; the order of evaluation IS the order of those calls.

- an operator's operands: `flatten.hero:1319-1327`, `left = expr(bin.left)`
  then `right = expr(bin.right)`, then one `.binary` instruction;
  `&&`/`||` are `short_circuit`, `flatten.hero:376`;
- a call's arguments: `lowered_from`, `flatten.hero:641-658`, the receiver
  first where there is one (`:646-650`), then `for arg in given` (`:652`),
  each through `one_argument` (`:665-702`);
- a call through a function value: `flatten.hero:583-584`, `target =
  expr(callee)` BEFORE `value_call` lowers the arguments (`:588-590`);
- a record's construction: `flatten.hero:538-550`, *Record construction IS a
  call with named arguments*, through the same `lower_args` (`:543`), so a
  record's fields are a call's arguments to this pass; a variant case's:
  `case_construct`, `flatten.hero:594-607`, `lower_args` again (`:601`);
  `ok`/`fail` the same (`:567`);
- a place's `place`: `flatten.hero:193-239`, `walk_place` descends to the
  root and lowers each index expression on the way out (`:222-226`, `key =
  expr(ix.index)` after the base's own walk), so a place's indices are
  evaluated outer-base first, left to right;
- a write: `mutation`, `flatten.hero:132-136`: `target = place(into)`, then
  `produced = expr(value)`, then ONE `.store(place, value)` instruction. The
  comment at `:188-192` states the intent: *Evaluation order is left to right:
  the path's index expressions are lowered before the value being stored,
  because `xs[f()] @ g()` calls f first*;
- a read of an index: `flatten.hero:1359-1376`, `base = expr(ix.base)`, `key
  = expr(ix.index)`, then one `.index` (or `.map_get`) instruction, which is
  where the read's bounds check sits (1.2);
- an `f` literal's holes: `flatten.hero:1271-1277`, `for h in ip.holes`, then
  `interp.desugar`;
- an array literal: `flatten.hero:1377-1384`; a map literal: `:1385-1396`,
  key then value per entry, alternating in one run.

So every position is the lowering's own choice, written once per form, in
the order of the syntax tree's children.

### 1.2 Where each check is emitted, and whether its place is a choice

Runner: `232-compiler-engineer-scratch/runone.sh <compiler> <file>` (build,
then the binary under `timeout 20`, output cut at 4,000 bytes), compiler
`heroes-next`, `sources 3e3c95dc246b73b3`. The critic's programs were copied
to `232-compiler-engineer-scratch/{ord,shift,exit}/` and rerun; every
printed line of F3, F4, F5, F26 reproduced (`places.hero` exit 0 and its 17
lines; `order.hero` two `wrong_label`; `f_place_bounds` `index`, `value`,
abort at 7:5; `w_at_argument_bounds` `place`, `value`, abort at 14:5;
`o_builtins` as F5 says).

- **A READ checks in its turn.** `flatten.hero:1359-1376` emits one `.index`
  instruction per level as the expression is lowered, so the bound is tested
  before anything to its right runs. Run, `ce/c_nested_read.hero`,
  `zs[noisy(i: 5)][noisy(j: 0)]` on a 2x2 array: `i`, abort at 7:11, `j` never
  printed.
- **A WRITE checks every level at the write.** `walk_place`
  (`flatten.hero:222-226`) lowers each index to a temporary and emits no check;
  `mutation` (`:132-136`) lowers the value; the one `.store` is spelled by
  `emit/element_store.hero` (370 lines), whose header says the last step's
  bound is written in the program's C *before the call* that stores
  (`element_store.hero:8-17`), each level descended at the store
  (`places.reached`, `emit/places.hero:166-228`). Run: `ord/m_nested_place`,
  `zs[i: 5][j: 0] @ v`: `i`, `j`, `v`, abort at 11:5, so the OUTER index is
  checked after the inner index and the value; `ce/d_field_of_element`,
  `ps[4].a @ v`: `index`, `value`, abort.
- **An `@` argument checks at the call.** `one_argument`
  (`flatten.hero:668-678`) hands `place(...)` to `inout.argument`; the
  emitter reaches it at the call instruction, after every argument
  (`ir/inout.hero:30-35`, in so many words: *the place carries those values to
  the emitter, which unshares and descends at the call, after every argument
  was computed*).

**Is the write's check the lowering's choice? No: the value can change the
array, and the check must see the array the write reaches.** Run, both built
and accepted by the checker:

| program | what it does | printed | exit |
|---|---|---|---|
| `ce/a_value_grows.hero` | `ys` of 2; `ys[3] @ grow(@ys)`, `grow` pushes two | `grew`, `1` | 0 |
| `ce/b_value_shrinks.hero` | `ys` of 4; `ys[3] @ shrink(@ys)`, `shrink` empties it | `shrank`, abort at 8:5 | 134 |
| `ce/f_at_arg_grows.hero` | `bump(n: @ys[3], by: grow(@ys))` on 2 | `grew`, `13` | 0 |
| `ce/g_at_arg_shrinks.hero` | `bump(n: @ys[3], by: shrink(@ys))` on 4 | `shrank`, abort at 11:5 | 134 |

So a check moved before the value, ALONE, passes `3 < 4` in
`b_value_shrinks` and the write then lands in an emptied array: the check at
the write is the memory guard (design.md §1.12; CLAUDE.md § Precedence rank
3), not a placement. Moved AND kept, it is two checks per element write and
it ABORTS `a_value_grows` and `f_at_arg_grows`, which write an element that
exists at the moment of the write and run correctly today. Spec § 9 already
says why the `@` shape is legal: the callee *works on the caller's place
itself, which no other `@` argument of the call may reach, nor, for `lent`,
one nested in another argument* (`spec/heroes-spec.md:272-276`): a NESTED `@`
in a later argument is allowed for `@` and refused for `lent` (run,
`ce/i_lent_nested.hero`, `error[aliased_mutable_arguments]`). I did not build
the moved check: the single-check form is refuted by `b_value_shrinks` and
the two-check form changes the meaning of two correct programs, so it is not
a route I would adopt (the panel skill's § 3c binds a route one would adopt).
**The sentence should say the two phases**: a place's indices, then the
value, then the write, which checks the place as it then stands; and an `@`
argument's place is reached when the call is made.

### 1.3 A position where left to right as written is NOT what happens

**`.default(v)` evaluates `v` only when the receiver is an error.**
`flatten.hero:451-472` lowers the argument inside the `default: else` block
(`:462-466`), and `flatten.hero:769-770` says why: *`.must()`, `.default(v)`
and `.is_err()` are branches, not calls (Part 5)*. Run, `ce/h_default_lazy.hero`:
`m["a"].default(noisy("fallback for a"))` printed `1` and no `fallback for a`;
`m["b"].default(noisy("fallback for b"))` printed `fallback for b`, `9`.
§ 6's table says only *extract or fall back* (`spec/heroes-spec.md:169`).
A sentence *a call's arguments are evaluated left to right* is false here:
the argument is evaluated after the receiver, or not at all, as `&&`'s right
side is. **Beside it** (CL-061): `default(m["a"], v)`, the spelling § 9's
*`x.f(y)` is sugar for `f(x, y)`* (`spec/heroes-spec.md:268`) makes the same
call, is refused at build, `unsupported[builtin]: the built-in `default` is
not emitted yet` (run, `ce/k_default_direct.hero`, exit 1), so the
direct spelling of the lazy form cannot be run to compare; a finding for the
coordinator, not filed by this seat.

Searched, every other `expr(` call of `flatten.hero` read in order
(`grep -n 'expr(@b' selfhost/ir/flatten.hero`): operands, arguments, the
receiver, the callee value, record and case fields, `ok`/`fail`, array and
map literals, holes, a field-held function (`field_call`, `:852-873`,
receiver, then the field, then the arguments), `assert`'s two sides
(`assert_condition`, `:950-957`), a write's place then value, a bind's value,
`?` (`try_expr`, `:479-500`, an early return), `.must()`. Run besides the
critic's: `ce/j_value_nested_at.hero`, `show(q, bump(@q))` with a by-value
`q` printed `1` (the copy taken in its turn, before the nested `@q` write),
and `q.a + bump(@q)` printed `101` (the read first). Every one is left to
right; the exceptions are the three that branch: `&&`/`||` (§ 7 says so),
`?` (§ 6's *propagate*), and `.default(v)` (said nowhere).

### 1.4 The built-ins that call a function value, and the labels

**Six built-ins take a function value, and all six are Heroes text**:
`selfhost/library_source.hero:76` `map`, `:83` `filter`, `:98` `fold`,
`:106` `find`, `:114` `any`, `:122` `all` (`grep -n -E '"function
[a-z_<>, A-Z]+\(.*\(function' selfhost/library_source.hero`, 6 lines; the
runtime's built-ins take none, `sort` taking one argument,
`selfhost/inventory.hero:263`). Each is `for x in xs` in order; `find`,
`any`, `all` `return` at the deciding answer; `filter` and `map` call on
every element. Their order and their stopping are those six loops, about
thirty lines of the library, so a sentence that states them freezes the
library's code (a `map` that called in another order, or `any` that called
on every element, would become a defect). They cost nothing to keep; what
the sentence buys is that a program printing inside the function is
promised its output.

**`wrong_label` is not a rule of its own: it is matching by position.**
The checker reads the parameter AT the argument's position and compares the
label with that parameter's name: a direct call, `selfhost/check/walk.hero:1617-1624`
(*A label is its position's name, where the signature needs one and where it
does not: a wrong one is a wrong argument with a comment on it*); a call
through a function value, `:1334-1369`; a record or a case, `:2075-2086`; a
built-in, `selfhost/check/labels.hero:41-76` (484 lines). The lowering
then drops the label (`flatten.hero:514-522`: *a label in Heroes does not
REORDER an argument ... "named -> positional" is erased by simply not
carrying the label*). No code anywhere reorders arguments by label, so *as
written* and *as declared* are one order whenever the program compiles.

## Item 2, `<<` and `>>`

### 2.1 What design.md rules, and what the compiler writes

design.md § 4.14, read whole (`docs/design.md:2032-2106`): the four
decisions of panel 040 (`:2065-2076`), among them *`1 << 63` is `INT64_MIN`,
not an abort. The shift is done on the unsigned bit pattern and cast back,
defined for every input ... Without the sign bit reachable, a mask set cannot
name its own top flag, which is the whole use*; *A shift count outside
`0..63` aborts*; *`>>` is arithmetic*. And `:2104`, *Integer overflow
aborts. It does not wrap*, which the shift bullet carves out by name.

The emitter, `selfhost/emit/operator.hero` (340 lines), `:162-186`: the
count check, then `t = (int64_t)((uint64_t)l << (uint64_t)r);` for `<<`
(`:179`) and `(l < 0) ? ~(int64_t)((~(uint64_t)l) >> r) : (int64_t)((uint64_t)l
>> r)` for `>>` (`:184`); its test pins that text (`:320-330`). The
constant evaluator computes the same thing with the compiler's own `<<`:
`selfhost/check/const_values.hero:166-172` (373 lines), its header saying so
(`:13-20`, *the shift itself never does* abort). The runtime has no shift.

### 2.2 The route nobody listed, built: an abort on a `<<` that loses a set bit

Built in this seat's worktree. Two edits (`git diff --stat`: `operator.hero`
+5, `const_values.hero` +32, 37 lines):

- the emitter writes, before the shift, `if ((((uint64_t)l << r) >> r) !=
  (uint64_t)l) hero_panic_at("`<<` shifted a set bit out of the 64", <place>);`
  (after the count check, so the round trip is defined);
- the constant evaluator refuses a constant whose `<<` would abort, as it
  refuses a count outside 0..63 (`int_out_of_range`, a new message, `lost_bit`),
  and asks BEFORE shifting (`loses_bit`), because generation 2 of the
  compiler runs its own `<<` under the new check and an evaluator that
  shifted a set bit out would abort the compiler itself.

Built: `heroes-next build selfhost/main.hero -o heroes-shl1`, then
`heroes-shl1` -> `heroes-shl2` -> `heroes-shl3`, each exit 0; `heroes-shl2`
and `heroes-shl3` both say `sources 14f51e54414cc101`. So the compiler's own
executed `<<` sites (`selfhost/cli/sha256.hero:135-221`, `mutate/sha1.hero`,
`measure/bpe.hero:181`, `mutate/spaces.hero:50`, `check/const_values.hero`)
did not trip it while compiling itself twice: they mask before they shift.

The shapes, each built and run under the frozen compiler and the route's
(`232-compiler-engineer-scratch/shl/{base,route}/`):

| program | frozen | route |
|---|---|---|
| `a_top_flag`: `1 << 63`, `3 << 62`, `top \| 1 << 62`, `top >> 63` | the four values, exit 0 | the same, exit 0 |
| `i_constant_kept`: `constant WRAPPED: i64` = `3 << 62` | `-4611686018427387904` | the same |
| `b_lost_positive`: `n(5) << 62` | `4611686018427387904` | abort, *`<<` shifted a set bit out of the 64*, at 6:11 |
| `k_literals_lost`: `print(5 << 62)` | `4611686018427387904` | abort at run time (built) |
| `j_constant_lost`: `constant LOST: i64` = `5 << 62` | `4611686018427387904` | build exit 1, `error[int_out_of_range]: `5 << 62` shifts a set bit out of the 64` |
| `c_negative_one`: `n(-1) << 0`, then `n(-1) << 1` | `-1`, `-2` | `-1`, then abort |
| `d_negative_three`: `n(-3) << 62` | `4611686018427387904` | abort |
| `e_mask_low_bits`: `~(-1 << 64 - k)`, k = 8 | `72057594037927935` | abort, in `low` |
| `f_count_from_call`: `1 << count(63)`, `2 << count(63)` | `-9223372036854775808`, `0` | first kept, second abort |
| `g_xorshift64`: Marsaglia's step, `x ^ x << 13` | five draws | abort at the first `<<` |
| `h_masked_xorshift`: the same, each `<<` masked first | five draws | the same five draws |
| `l_carry_adder`: `(sum & carry) << 1`, on -5 + 7 | `12`, `2` | `12`, then abort |

**What the route aborts beyond its intent**: every `<<` of a NEGATIVE
operand by 1 or more, since a negative's bit 63 is a set bit
(`c_negative_one`: `-1 << 1` is exactly `-2`, and it aborts). So under the
route `<<` is an operator on non-negative values; `-1 << n`, the usual way
to build a run of high ones and, complemented, of low ones, aborts for every
`n` past 0.

**Where the tree itself relies on the wrap** (grep `<<` over `git ls-files
'*.hero'`, comments and strings read out by hand): `tests/harness/gen_rng.hero:35`
and `:37`, Marsaglia's xorshift64, whose comment (`:4-6`) cites design.md
§ 4.14 as the reason the step *can never abort*; `gen_rng.hero:31`, the mask
idiom `~(-1 << 64 - k)`; `tests/harness/gen_contract.hero:52`, a carry
adder whose carry crosses bit 63 on a negative operand;
`tests/harness/gen_eval.hero:120-125`, the oracle of the generated programs,
which computes `a << b` to predict what a generated program prints and pins
`3 << 62` and `1 << 63` (`:450`, `:461`), and whose header (`:8`) states the
wrap. Every one is the harness of M-generated-programs. `examples/` (5 files)
and `selfhost/` mask before they shift. The net's own tests under the route:
see 2.4.

### 2.3 What the route costs, measured

- **Emitted C**: one check per `<<`, 174 bytes each. `heroes-next` against
  `heroes-shl1`, `build <p> --emit-c`: `examples/checksum/main.hero`
  87,650 -> 88,695 bytes, 6 checks for its 6 shifts; `tests/harness/main.hero`
  9,244,347 -> 9,248,196, 22 for 22; the compiler's own emission under the
  route, 28,633,244 bytes, 29 checks. No C line is added: the emitter joins a
  Heroes line's statements onto one C line (panel 219's R1), so `wc -l` is
  equal on both sides.
- **At run time, instructions retired** (the kernel's counter read through
  `/usr/bin/time -l`, its `instructions retired` line alone, no duration
  read; three runs each), 10 million xorshift steps, two `<<` each, every
  value the same on both sides (`3039611916969981977`):
  - `-O2`, masks written as constants (`bench/xs.hero`): 67.07M-67.64M
    frozen, 67.10M-67.63M route: **no cost**, clang proves the check dead
    when the operand is masked before the shift, which is what code under
    the route must write;
  - `-O2`, masks read from an array clang cannot see through
    (`bench/xo.hero`): 97.2M-97.8M -> 137.3M-137.9M, **about 2
    instructions per `<<`**;
  - `-O0`, `heroes build`'s default (`bench/xs.hero`): 2,225.5M-2,231.2M ->
    2,633.9M-2,635.1M, **about 20 instructions per `<<`**.
- **In the compiler**: 37 lines (`operator.hero` +5, `const_values.hero` +32,
  a `loses_bit` question and a `lost_bit` message), plus `operator.hero`'s
  test, which pins the shift's text and is the one failure of the compiler's
  own tests under the route: `heroes-shl2 test selfhost/main.hero`, **1,768
  tests, 1 failed** (`FAIL "overflow guards on + - * and negation, the / and
  % pair, the shifts"`). No new IR instruction, no new type, no runtime entry:
  the check is the count check's sibling, so by § 1.7 it is not a core
  construct and does not reach the ceiling.
- **In the net**: `heroes-shl2 test tests/harness/main.hero`, **474 tests, 14
  failed**, every failure the new abort: 11 at `gen_rng.hero:35:13`
  (xorshift64), 2 at `gen_contract.hero:52:22` (the carry adder), 1 at
  `gen_rng.hero:31:23` (the mask idiom). The frozen compiler on the same
  tests: see below. So the route rewrites three lines of the generator and
  the contract model of M-generated-programs, the oracle `gen_eval.hero` that
  predicts what a generated program prints, and every generated program's
  expected output where a shift loses a bit.

### 2.4 Verdict on item 2

**Object to the abort route; the sentence states design.md § 4.14's rule.**
The route is cheap in the compiler and costs nothing at `-O2` on masked
code, but it does not do what its name says: it aborts every `<<` of a
negative operand (`-1 << 1`, exactly `-2`), the mask idiom `~(-1 << 64 -
k)`, and the two shift idioms the tree's own harness is built on, 14 of its
474 tests. § 4.14's reason (*a mask set cannot name its own top flag*) is
kept by the route, but its first clause, *defined for every input*, is
amended, and its `<<` becomes an operator on non-negative values, which no
sentence of § 7 or § 4.14 says today. Neither route is needed for
self-hosting (the compiler's 29 shifts mask first). The spec's sentence:
`<<` works on the 64-bit pattern, a bit shifted out is lost, `1 << 63` is
the smallest `i64`; `>>` copies the sign.

## Item 4, the stack

### 4.1 Where `stack exhausted` comes from

`runtime/parts/stack.c` (1,064 lines). On POSIX, a SIGSEGV/SIGBUS handler on
an alternate stack (`hero_stack_guard_install`, `:899-930`), claiming a fault
only when two witnesses agree (`hero_stack_handler`, `:662-692`): the faulting
address lies within `HERO_STACK_WINDOW` (`1 << 20`, 1 MiB, `:280`) below the
thread's lowest stack address, AND either the interrupted `sp` is down there
too (`sp_low`) or the fault is a probe less than 1 MiB below `sp` (`probing`,
defect 007, `:666-680`). Then `panic: stack exhausted in <module.function>`
(`:683`) and `hero_abort()`. Windows: `:497` under ASan, `:950` its own
vectored handler. The guard is entered on the main thread (`hero_args_set`)
and on every thread the runtime spawns (`hero_spawn_enter`); Heroes code on a
foreign thread is refused at its first instruction (`runtime/parts/thread.c:28-34`,
`hero_thread_guard`), and a spawned thread's stack is never smaller than
`main`'s (`runtime/parts/spawn.c:88-117`, panel 115's floor).

**F10 and the brief name `thread_deep` as a thread's smaller stack; it is
not one.** `tests/golden/surface-fixtures/recursion521/thread_deep.hero` is a
MODULE whose C name is `threaddeep` (`stack.c:311-316`), a plain recursion on
the main thread; the thread shape run below is `hero_thread_spawn`'s.

### 4.2 The shapes, run (frozen compiler, `heroes build`, `-O0`, stack 8,176 KiB by `ulimit -s`)

Generated by `232-compiler-engineer-scratch/tools/gen_stack.py` into
`stack/`; runner as in item 1; the last three rows run with stdout and
stderr captured apart.

| program | shape | stdout | stderr | exit |
|---|---|---|---|---|
| `small2000` | 2,000 functions, none recursive, `f_k` calls `f_{k-1}`, `i64` frames | `1999` | empty | 0 |
| `records2000` | d435ca009's words: `R_k` holds `R_{k-1}` and a `str`, `m_k` returns `R_k` built from `m_{k-1}` | | `panic: stack exhausted in records2000.m1962` | 134 |
| `thread_recursion` | `down(n)` recursing 10^9 deep on a `hero_thread_spawn` thread | | `panic: stack exhausted in thread_recursion.down` | 134 |
| `bigframe16` | `R_k` holds TWO `R_{k-1}` (2^k x 16 bytes), `make16`, no recursion | `before` | `panic: stack exhausted in bigframe16.make14` | 134 |
| `bigframe18` | the same to `R18` (4 MiB), `make18`'s frame alone passes the stack | **empty, the `before` it printed lost** | **empty** | **139** (SIGSEGV) |
| `bigframe20` | to `R20` (16 MiB) | empty | empty | **139** |
| `bigframe18_thread` | `make18` on a `hero_thread_spawn` thread | **empty** | **empty** | **138** (SIGBUS) |
| `bigframe18` at `-O2` | the same source | `before`, `1` | empty | 0 |

(`records2000`'s program was written from the commit's words, the program
itself never committed (d435ca009: *written when the suite runs*); the
commit names `m1`, this one `m1962`, so the shapes are not byte for byte the
same, and both abort `stack exhausted`.)

**So *a call chain deeper than the stack aborts* is not true of every one.**
A call whose frame passes the stack by more than the handler's 1 MiB window,
made with more than 1 MiB of stack left, dies at 139 on `main` and 138 on a
spawned thread with nothing on stderr, and loses the output it had buffered:
clang's probe (`___chkstk_darwin`) faults in the guard while `sp` is still
megabytes above it, so neither witness holds and the handler passes the
fault on (`stack.c:681`). It needs no recursion, no FFI and no `extern`:
twenty records and twenty functions. At `-O2` the copies are elided and the
same program runs, so it is the `-O0` of `heroes build` and `heroes test`
that meets it. This is design.md § 1.12's class (a segfault, empty stderr),
`blocking` by `.claude/rules/verification.md` § Bounded discovery; a finding
for the coordinator to file, not filed by this seat.

**Unrun, and a question rather than a premise**: on Linux, `flags()` carries
no `-fstack-clash-protection` (`selfhost/cli/flags.hero:135-151`), and the
comment at `stack.c:278-280` says clang emits no probes there; a frame larger
than the guard would then move `sp` past the guard page whole, and its first
store lands below it, in whatever is mapped there. Whether that is a silent
139 or a write into another mapping was not run on Linux by this seat.

**Verdict on item 4**: the sentence the draft proposes is the right promise
(§ 1.12 makes it the language's), and the compiler has the bug (CLAUDE.md
§ 12, spec beats compiler): adopt *a call chain deeper than the stack
aborts*, and file `bigframe18` as a defect owed before `1.0.0`. Narrowing the
sentence to fit the crash would promise a segfault.

(Item 2.3's baseline, run after: `heroes-next test tests/harness/main.hero`,
the frozen compiler, **474 tests, all passed**, so the route's 14 failures
are the route's alone.)

## Item 5, `exit`

F7's programs copied to `232-compiler-engineer-scratch/exit/` and rerun with
this seat's compiler; every edge reproduces:

| program | written | result |
|---|---|---|
| `a_minus_one` | `exit(code: -1)` | build exit 1, `error[int_out_of_range]: `-1` is not a status ...`, its note *would reach it as 255* |
| `f_hex_literal`, `i_max`, `j_unlabelled` | `0x100`, `9223372036854775807`, `exit(256)` | build exit 1, the same code |
| `b_operator_known` | `exit(code: 255 + 1)` | built; `before`, then `panic: .must() on an error: does_not_fit: ...given 256`, `at b_operator_known.hero:3:5, in b_operator_known.main`, 134 |
| `c_name_known`, `d_constant_known`, `e_cell_known` | a `=` name 256, a `constant` 256, a cell written 300 | built; the same abort at the program's call, 134 |
| `g_computed` | `len(args()) + 299` | the same abort, 134 |
| `h_in_range` | `exit(code: 255)` | exit 255 |
| `k_u8_cell` | `c: u8 = 3`, `exit(code: c)` | build exit 1, `type_mismatch` (no widening, § 3) |
| `l_through_value` | `leave = exit`, `leave(code: len(args()) + 299)` | the abort `at <heroes library>:232:1, in library.exit` |

Where each comes from: the refusal is `selfhost/check/exit_code.hero` (127
lines), which asks `contextual.number_literal` of each value handed to the
library's `exit` (`:44-66`): *A literal written as the code, under its minus
signs, is refused ... Anything else is a computed code and stops the program
where it runs, an operator on literals included* (`:19-25`). The abort is the
library's Heroes, `selfhost/library_source.hero:272-277` (library line 232):
`status: i64? = fail(code: "does_not_fit", ...)` then `_ = status.must()`,
which is why the report names a `.must()` the program never wrote; the place
is the program's call because the emitter hands a library function its
caller's place (`selfhost/emit/stop_place.hero:38-40`), and through a function
value no call is known, so the library's declaration is named
(`exit_code.hero:10-13`, by design, panel 230's R1).

**What a sentence of § 11 would misdescribe.** Today's, *a code outside 0 to
255 aborts* (`spec/heroes-spec.md:338`), misdescribes the literal: such a
program is refused and never runs. A sentence *a literal outside them is
refused* is right for `256` and wrong as § 7 reads `-1`: the grammar makes it
`Unary = "-" Unary` on a literal (`:212`), and § 2 (`:42-47`) never calls a
minus sign part of a literal, while every pass of the compiler does
(`exit_code.hero:20`; `flatten.hero:1285-1289`, *A negative literal is a
literal, and it is folded HERE*). And a reader holding § 4's *known*
(`:115`: a literal, an operator on known values, a `=` name or a constant
holding one) predicts that `255 + 1`, a `=` name and a constant are refused
too; § 4 defines *known* for a constant's body alone, and here they abort.
So the sentence that is true: *a number written as the code, its minus sign
included, outside 0 to 255 is a compile error; any other code outside them
aborts at the call*. The report's `.must()` and `<heroes library>:232:1`
are no sentence of the specification's (§ 6 promises *saying why*, not the
words), but they matter to the paragraph's *and where* (item 6): a call
through a function value names a place the program does not hold. Changing
the words costs the library's `exit` an abort that is not a `.must()`, and
the place-handing of `stop_place` reaches only the `.must()` path today; not
built, since no seat's route asks for it.

## Item 7, a diagnostic's code

### 7.1 How many codes the compiler can hand a tool, against the 242 pinned

The program: `232-compiler-engineer-scratch/tools/codes2.py` and `codes3.py`
(read-only), over `selfhost/**/*.hero` cut to its code by measurement 060's
own `code_of` (test blocks and comments out). A diagnostic is built by
`diag.new_diagnostic` or `diag.new_unsupported` (`selfhost/diag.hero:81`,
`:115`); its code reaches them as a literal (380 calls, 224 codes), through a
helper that forwards one (13 calls: the parser's `cursor.error`,
`cursor.hero:257`, and `expect`, `expect_closer`, `expect_after`; the gate's
notes, `emit/gate.hero:50,154`; `head_names.windows_said`;
`separator_runs.said`; the `Missing` and `Said` records; two `CODE`
constants), through a variable (`code: str @= "..."`, `code @ "..."`, six
sites, e.g. `parse/unclosed.hero:223-291`), or by concatenation, once:
`parse/heads.hero:207`, `"expected_" + what`, `what` being `field` or `case`.
Every `code: "..."` literal was grouped by the call it is an argument of
(`codes/by_call.txt`), `fail` (172 codes, a `T?`'s) left out, and three
test helpers' `c` and `x` dropped by hand (`cli/certain.hero:294`,
`excerpt_window.hero:195`, `diag_render.hero:199`).

- **285 codes** can be handed to a diagnostic by this reading;
- **all 242** that measurement 060 counts (every `#~`/`#~v` of
  `tests/golden/`, its regex, rerun here at `afe983265`: 242) are among them,
  none unexplained;
- **43 are pinned by no `#~`**, 5 of them named `error[...]`/`unsupported[...]`
  in some other file under `tests/` (`builtin`, `ffi_missing_library`,
  `module_cycle`, `no_entry_point`, `unknown_module`) and 38 nowhere
  (`codes/unpinned.txt`, each with the module that writes it): 18 of the
  parser's `expected_*`, the module system's six (`module_binding_taken`,
  `module_is_not_a_value`, `module_name_reserved`, `module_names_collide`,
  `not_a_module`, `used_twice`), the flow checker's five (`if_without_else`,
  `missing_value`, `not_fallible`, `not_indexable`, `returns_nothing`),
  `not_bool`, `not_a_function`, `variant_not_callable`, `builtin_shape`,
  `repeated_parameter_name`, `rest_is_a_field`, two `ffi_*`, two gate notes
  (`hole`, `missing`), `empty_fixed_array`, `one_library_per_group`.
- **Reachable, run** (`heroes-next check --brief`, `reach/`): `not_indexable`,
  `if_without_else`, `missing_value`, `not_bool`; `no_entry_point` by
  `build`. My guesses for ten others drew a neighbouring code
  (`expected_index_close` -> `unclosed_bracket`, `not_a_function` ->
  `not_callable`, `returns_nothing` and `not_fallible` -> `bad_operand`),
  so 5 is a lower bound and *the rest are reachable* is unrun.

So a promise of codes written on 060's 242 promises 85% of what a tool can be
handed, and no instrument here sees the other 43 move. `heroes check --json`
hands them: run, `x = 5` / `print(x[0])` answers `"schema": 2`, `"code":
"not_indexable"`.

### 7.2 What `"schema"` promises today, and the route

design.md § 4.17 (`docs/design.md:2321-2363`): *When `"schema"` moves: when a
member's meaning changes or a member leaves. A member is added with what its
absence means*. That is the document's shape; a code is a member's VALUE, and
a renamed, split or removed code moves no member's meaning, so today the
schema promises nothing about codes. Its number is one constant,
`SCHEMA = 2`, `selfhost/cli/check_json.hero:39-40` (183 lines), which the
help, the model's page and the errors page are held to. **The route F16 and
F24 named costs no line of the compiler**: a sentence of § 4.17, *a code
renamed, split or withdrawn moves `"schema"`*, and one instrument to make it
true, since today nothing would see a code vanish: a list of the codes in the
tree, 285 rows, which the net holds to the reading above (a `test` in
`selfhost/diag.hero` or a check of the `spec` suite), so that removing one
fails until the list and `SCHEMA` move together. That list is exactly what
060 says `diag.hero` does not keep. **Verdict on item 7: promise the code,
not the words, through the schema's rule, and only once the list exists**;
a promise over 242 of 285 with no instrument for the 43 is a promise nobody
can see broken.

## Item 8, the compatibility suite's corpus

`tests/harness/suite_compat.hero` (598 lines) checks, never runs, every
file of `examples/` holding `function main(` as a release tag holds it,
against every tag of the compiler's own major version no newer than it
(`:39-42`), so **the k-th `1.x` release checks k corpora** and the cost per
release grows with the releases. Each corpus below was unpacked at `v0.5.0`
(`git archive v0.5.0 ...` into `232-compiler-engineer-scratch/corpus/v050/`)
and every program in it checked by today's compiler, four at a time
(`tools/check_corpus.sh`; the `run/` rerun with each name passed as an
argument after `xargs -I` refused the long names), with each corpus's churn
over the 30 days to `afe983265` (`git log --since=2026-09-29
--diff-filter=M --name-only`):

| corpus at `v0.5.0` | checks | lines checked | today's compiler | `.hero` files modified in 30 days | what its programs are |
|---|---|---|---|---|---|
| `examples/` (today's) | 69 | 18,823 (120 files) | 69 of 69 check | 105 of 120, 16 commits | programs written to be used; 66 expected outputs, none holding `panic:` |
| `tests/golden/run/` | 544 | 45,845 | 544 of 544 check | 248 of 622, 22 commits | tests of the compiler: 380 of 563 are `fixedbugs-*`, and 129 of 544 `.expected` pin an abort's `panic:` words |
| `selfhost/` | 1 root, 684 modules | 177,905 | checks | 648 of 691 | the largest program there is |
| `tests/thesis/` references | 10 | 791 | 10 of 10 check | 1 of 50 | programs held to every case; the folder's other 40 files are mutants wrong on purpose (13 refused) |
| the specification's code blocks | 8 blocks | 48 | not checkable as they stand | the spec: 37 commits | fragments: none holds `function main(`, one is the operator table, one an `extern` needing `sqlite3.h` |

**What changes with the language it tests.** Before `1.0.0` every corpus
moves with the language, because a change is applied to the tree in the
commit that makes it: `fcec76d8d` (M-inferred-cell's `@=`) rewrote 221
`run/` goldens and 102 files of `examples/` in one commit, `dddea0cf3` 32
more goldens; of the 22 commits that moved `run/`, those two moved 253 of
the file-changes and the other twenty one or two each. That is not what
disqualifies a corpus, since the suite reads it frozen at the tag. **What
disqualifies one is whether a later `1.x` may legitimately make it red**:
the paragraph lets a release repair a defect (a wrong program accepted
becomes refused, verification.md's `blocking`), and `run/` is mostly
repairs' witnesses (380 `fixedbugs-*` of 563), so checking it would make a
promise-keeping repair read as a broken promise; and for a suite that RUNS
(the draft's *does what the specification says*), 129 of its expected
outputs pin an abort's words, which no sentence promises. `examples/` has
neither (no `panic:` in its 66 expected outputs). `selfhost/` at a tag is a
valid program, checks clean today, and costs one invocation over 9.5 times
`examples/`'s lines; but it is the compiler's own source and moves with
every repair (648 of 691 files in 30 days), and a later `1.x` compiler
compiling an older compiler is not today's bootstrap rule, which builds
each compiler from the seed before it. The thesis references are small (10,
791 lines) and valid; the spec's blocks need a wrapping rule each (§ 4's,
F23's, made a `main` by hand) before one can be checked.

**Verdict on item 8**: `examples/` as the suite has it, **plus the thesis
references (10 checks, 4% of the lines)**, which are valid programs held
to every case and moved once in 30 days; not `run/` (a repair the promise
allows would read red), not `selfhost/` (the bootstrap rule, and 9.5 times
the lines per tag per release). The spec's blocks are the promise's own
text and the better corpus in principle, and are unpriced: none is a
program until a wrapping rule exists, which no suite has but
`suite_special`'s one cut of the sqlite block.

### 4.3 Correction to 4.2, 2026-10-29, and the repair priced

**4.2 is wrong in one clause, corrected here rather than rewritten**: *and
loses the output it had buffered* and the table's *the `before` it printed
lost* are false. In `bigframe18` and `bigframe18_thread` the oversized frame
is `main`'s (or `worker`'s) own, since it holds `r: R18`, so the fault comes
in its prologue and `before` was never printed (the repaired runtime below
names `bigframe18.main` and `bigframe18_thread.worker`). Run with a small
`main` that prints `before` and then calls `go()`, which holds the record
(`stack/bigframe18b.hero`), the frozen runtime prints `before`, keeps it,
and dies at **139 with an empty stderr**, so the crash stands and nothing is
lost.

**The repair, priced and run (not adopted by this seat; the defect's lane
owns it).** A third witness in `hero_stack_handler`: a fault in the guard,
below `sp`, whose `pc` is the platform's probe routine (`dladdr` names
`__chkstk_darwin`, which only a prologue calls), is a frame too large for
what is left, whatever `sp`'s distance; 12 lines of `runtime/parts/stack.c`
(`232-compiler-engineer-scratch/repair-stack-third-witness.patch`), built
into programs from this worktree's `runtime/` (`heroes doctor`: `runtime
runtime`, read from the working directory), then the file restored:

| program | frozen runtime | repaired |
|---|---|---|
| `bigframe18`, `bigframe20` | 139, empty stderr | 134, `stack exhausted in bigframe18.main` (`bigframe20.main`) |
| `bigframe18_thread` | 138, empty stderr | 134, `stack exhausted in bigframe18_thread.worker` |
| `bigframe18b` | `before`, 139, empty stderr | `before`, 134, `stack exhausted in bigframe18b.go` |
| `bigframe16`, `records2000`, `thread_recursion`, `small2000` | as 4.2 | the same lines and exits |
| the five recursion fixtures `suite_surface.hero:194-198` holds (`behind`, `mutual`, `direct`, `hidden`, `thread_deep`) | (the suite's lines) | each the suite's line, word for word |

The wild store panel 104 rejected is not a probe, so its `pc` is never the
probe routine and it still re-raises by construction; not run here (no
fixture at hand). On Linux the witness does not exist (no probe routine; see
the unrun question above), so the repair is Darwin's and Linux needs
`-fstack-clash-protection` or its own witness, priced by nobody here.

## Corrections, 2026-10-29, added underneath as a record takes them

- **4.1 and 4.2 cite `stack.c:280` and `:278-280` wrongly.**
  `HERO_STACK_WINDOW` is defined at `stack.c:281`, and its comment
  (`:279-280`) reads *clang emits no stack probes on **Darwin**, so a frame
  larger than the guard page can land past it*: about Darwin, not Linux, as
  4.2's unrun question said. That comment contradicts the same file's
  defect-007 text (`:69`, `:669`, *clang's prologue calls
  `___chkstk_darwin`*), and the run of 4.3 sides with `:69`: the repair's
  witness fired only because the faulting `pc` was `__chkstk_darwin`. So the
  window's comment is a premise that expired (CLAUDE.md § 11's *a narrowing
  asks the value, never the world*), and the Linux question stands on the
  seat's knowledge of clang (no probes without `-fstack-clash-protection`),
  which is a question, unrun here, not a reading of this file.
- **The opening's *No clock was started* is too wide.** 2.3 ran
  `/usr/bin/time -l`, which measures a duration as well; its output was cut
  to the `instructions retired` line before it was printed, and no duration
  was read, printed or written.

## The seat's verdicts

- **verdict**: no veto (no item adds a core construct: every route here is a
  sentence, a check beside an existing check, or 12 lines of the runtime's
  handler). Per item: **1 approve a two-phase sentence, object to one line**;
  **2 object to the abort route, approve § 4.14's rule as the sentence**;
  **4 approve the draft sentence and file the crash it does not yet keep**;
  **5 approve a corrected § 11 sentence**; **7 object to promising codes on
  the 242, approve the schema route once a list exists**; **8 approve
  `examples/` plus the thesis references, object to `run/` and `selfhost/`**.
- **section**: design.md § 1.1 and § 1.7 (the ceiling: nothing here is
  core), § 1.12 (item 4's crash; item 1's check at the write is a memory
  guard), § 4.14 (item 2, `docs/design.md:2071-2076`), § 4.17 (item 7,
  `:2321-2363`). The document does not cover item 8; nothing in design.md
  names a compatibility corpus.
- **implementation_cost**: item 1, 0 lines (`flatten.hero` already does
  it); item 2's abort route 37 lines (`emit/operator.hero` +5,
  `check/const_values.hero` +32) plus `operator.hero`'s test, 3 harness
  sites and `gen_eval.hero`'s oracle, the sentence route 0; item 4, 12 lines
  of `runtime/parts/stack.c` on Darwin (run), Linux unpriced; item 5, 0; item
  7, a list of 285 codes and one check holding it, unbuilt; item 8, a second
  corpus in `suite_compat.hero` (598 lines), unbuilt.
- **needed_for_self_hosting**: no, for every item: the compiler self-hosts
  at `afe983265` without them, and under the abort route its 29 shifts
  never fire (fixpoint by `cmp`).
- **argument**: Each sentence must say what the compiler does, and running
  the shapes beside each shows where a one-liner lies: a write checks its
  place after the value because the value can change the array
  (`a_value_grows` runs, `b_value_shrinks` needs the late check);
  `.default(v)` is lazy; a frame larger than the stack segfaults silently at
  `-O0`; 43 codes reach a tool unpinned. The `<<` abort route is cheap but
  aborts every negative operand and 14 of the net's own 474 tests, so it
  buys a thesis effect nobody has measured with the harness's own idioms.
- **prediction**: a `run/` case of `stack/bigframe18b.hero` (a small `main`,
  one call whose frame passes the stack) reads exit 139 with an empty stderr
  on the trunk at every commit until `runtime/parts/stack.c` changes, and
  exit 134 `stack exhausted in bigframe18b.go` with the 12-line witness;
  checkable at that defect's repair gate on this Mac, and on the CI's Linux
  legs it stays red with the witness alone.
- **condition**: item 2, a blind-seat measurement that readers predict an
  abort for `5 << 62` and a census showing no wrap-reliant `<<` in written
  programs would move me to the route; item 1, a refusal of a write whose
  value lends the write's root (`ys[i] @ f(@ys)`) would make a check before
  the value sound with one check; item 7, the list of codes existing and
  held by the net.
