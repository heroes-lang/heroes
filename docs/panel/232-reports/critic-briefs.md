# Panel 232, the completeness critic's first pass: the briefs

Written as it went, 2026-10-29, against the frozen tree `afe983265` in the
critic's own detached worktree `.claude/worktrees/232-critic/`. No verdict on
any proposal. Every finding names the brief and the sentence, the command
run, what it printed, and the finding. Commands run from the critic's
worktree unless said otherwise; programs run from
`.claude/worktrees/232-critic/critic-scratch/`.

Kinds of finding: **FALSE** (the brief states it and the tree says
otherwise), **UNVERIFIED** (could not be settled by a command run here),
**SEARCH** (a word that claims a search: what was actually searched),
**ROUTE** (an item or a route nobody listed), **TRUE** (checked and it
holds; listed only where the check was costly or the claim load-bearing).

## Findings

### 1. SEARCH, `00-shared.md` item 1: *The specification says nothing of it (`grep -n -i 'order'` ...: declarations, map keys, a function type's labels, headers; no evaluation)*

Command: `grep -n -i 'order' spec/heroes-spec.md`, then one `grep -n -i -E` per
word: `left`, `right`, `evaluat`, `side.effect`, `sequenc`, `in turn`,
`as written`, `before`, `then `, `first`, `once`.

Printed for `order`: six lines, 113 (declaration order), 195 (*a map's
insertion order does not affect* `==`), 198 (*ordering one* [a `nan`] *aborts*),
266 (a function type's labels), 313 (`keys` in no order), 355 (headers). None
is an order of evaluation, so the brief's reading of its own grep holds. The
other words found no evaluation order either (`evaluat`, `as written`,
`sequenc`, `side.effect` print nothing).

Finding: *says nothing of it* is wider than the search. The word `order`
misses three places where the specification does fix an order a program can
observe: § 7 line 187, *`&&` and `||` short-circuit*, which fixes that their
left operand is evaluated first (the compiler-engineer's brief says so itself:
*§ 7 already says they short-circuit*); § 11 line 321, *`fold` (left ...)*
and *`find` (the first it accepts ...)*, which fix the order two built-ins
call a function value, while `map`, `filter`, `any` and `all` say nothing of
theirs. The last is a position nobody listed (finding 5).

### 2. FALSE in part, `00-shared.md` item 1 and `proposal.md` item 1: *no sentence of § 9 was found saying the labels keep that order*

Command: `sed -n '257,294p' spec/heroes-spec.md`.

Printed, § 9 lines 263 to 267: *When two parameters in a signature share a
type, named arguments are mandatory at the call site ... The names are part of
the type: the other order is another type, and a call through a value names
each in its own position.*

Finding: for a call THROUGH A VALUE, § 9 already says each label stands in its
own position. What no sentence says is the same of a direct call to a named
function: the call-site sentence says only that the labels are mandatory when
two parameters share a type. So the question put to the compiler-engineer and
the spec-warden (`spec-warden.md` item 1, *whether § 9 owes "labels keep the
signature's order"*) is narrower than the briefs state: half of it is written,
at line 266, which `00-shared.md` itself lists as *a function type's labels*.

### 3. TRUE, the probes and the blind truths, run by the frozen compiler

Commands: in the critic's worktree, `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes-stage1`, `./heroes-stage1 build selfhost/main.hero
-o heroes`, `./heroes build selfhost/main.hero -o heroes-next`;
`./heroes-next --version` printed `heroes 0.5.0` / `sources 3e3c95dc246b73b3`
(the brief's key), `./heroes --version` `sources 00577d2a5957da82` (so the
second generation is needed, as `00-shared.md` says). Then `heroes-next build`
and the binary run, for the three probes and the three tasks' programs copied
verbatim from `blind/tasks/`.

Printed: `places.hero` exit 0, `left right`, `first second`, `a b`, `x0 x1`,
`h0 h1`, `index value`, then `9`; `order.hero` build exit 1, two
`error[wrong_label]` (*`pair` has nothing called `second` at this position —
it is `first`*, the proposal's quotation word for word, and the same for
`first`); `shift.hero` exit 0, `-4611686018427387904`, `-9223372036854775808`;
task O exit 0, `left right a b 10`; task S exit 0, the two values and `done`;
task X build exit 1, `error[int_out_of_range]: `256` is not a status `exit`
can end a program with, which is 0 to 255`. Every stated truth holds.

### 4. FALSE as a reader would take it, `blind/README.md` P sentence 1: *... and a place before the value written into it*

Command: in `critic-scratch/ord/`, `heroes-next build` and the binary run
(`critic-scratch/runone.sh`, output bounded by `head -c`, time by `timeout 20`)
of

```
ys: [i64] @= [0, 0]
ys[noisy(label: "index", value: 5)] @ noisy(label: "value", value: 1)
```

Printed: `index`, `value`, then `panic: array index out of range at
f_place_bounds.hero:7:5`, exit 134.

Finding: the place's INDEX is evaluated before the value, but the place's
bounds CHECK runs after the value has been evaluated: the value's effect
happens, then the place aborts. A READ does it the other way: `xs[noisy(label:
"index", value: 5)] + noisy(label: "right", value: 1)` printed `index` then
aborted, `right` never printed (probe `b`). So P's *a place before the value
written into it* leads a reader to predict `index` then the abort, and the
compiler prints `value` between them. The places probe tried only an index in
bounds, which cannot see it. Either the sentence says *a place's index, then
the value, then the write, which checks the place*, or the compiler moves the
check, and the compiler-engineer's item 1 does not name this shape. (Go's
specification orders an assignment in two phases, operands first and the
writes after, which is this compiler's behaviour; a precedent for the
historian, unverified here.)

The other abort orderings hold left to right, run the same way: a bounds abort
on an operator's left (`index`, abort), an overflow (`big`, abort, `big` a
call's result so the compile-time check of § 4 cannot see it), a division by
zero (`z`, abort), a bounds abort in a first argument (`first`, abort); and
`v @= 1; total = v + bump(@v)` with `bump` writing `c @ c + 1` printed `1`
then `2`: the cell is read before the call that writes it, so a lowering that
hoists calls does not reorder a read here.

### 5. ROUTE, item 1: positions the probe did not try, run; one order nobody listed is a built-in's

Command: `critic-scratch/runone.sh` over `critic-scratch/ord/g_*` to `t_*` (each
program in that folder).

Printed, every one left to right as written: `?` in the middle of a sum
(`left`, `mid`, then the error `bad`; `right` never printed); a call through a
function value (`callee` before `arg`); a map literal (`k0 v0 k1 v1`); an
object before its index (`object`, `index`); a map write (`key`, `value`); the
UFCS receiver (`recv`, `arg`); `print("before", xs[...])` with the second
value aborting (the index's `i`, then the abort, `before` never written); a
variant case's fields (`w`, `h`); labels partly omitted (`arg_a arg_b`). A
record's fields, a variant case's fields and a call's labels written in
another order than declared are all refused `wrong_label`, a call's whose two
parameters do NOT share a type included (`add2(b: ..., a: ...)`), so *as
written* is the declared order in every shape tried.

Finding, the order nobody listed: a built-in that calls a function value. `[1,
2, 3].map(shout)` called it on `1 2 3` in order; `any(big)` stopped at the
first `true` (`test 1`, `test 2`, never `test 3`); `all(big)` at the first
`false` (`test 1` alone); `find(big)` at the first it accepts; `filter(big)`
called it on all three. § 11 says `fold` is left and `find` gives *the first
it accepts*; it says nothing of `map`'s order, nor that `any`, `all` and
`find` stop calling. A program printing in the function observes all of it,
and P's sentence 1, being about an expression, does not reach it. Neither the
probe, the compiler-engineer's list nor P names it.

### 6. ROUTE, item 2: `>>` of a negative number, the shape beside `<<`, which § 7 does not state

Command: `critic-scratch/runone.sh` over `critic-scratch/shift/*.hero`.

Printed: `n >> 1` with `n` a call's `-8` printed `-4`, `n >> 63` printed `-1`
(an arithmetic shift, the sign copied in); `x: u8 = 1; print(x << 7)` and the
same at `i32` refused `error[bad_operand]: `<<` takes `i64``; a count of 64 or
-1 from a call aborted `shift count outside 0..63`; `print(1 << 64)` with two
literals BUILT and aborted at run time (the § 4 rule that refuses a known
abort binds a constant's body alone, and `constant TOO_FAR: i64` / `1 << 64`
is refused `int_out_of_range`); `-3 << 62` printed `4611686018427387904`, a
negative made positive; `constant WRAPPED: i64` / `3 << 62` builds and prints
the wrapped value.

Finding: § 7 says of the shifts *i64 only; shift count 0..63 or it aborts* and
nothing of `>>` on a negative operand, which the compiler answers as an
arithmetic shift. It is the shape next to item 2 (CLAUDE.md § RUN IT, *a
repair is attacked at the shapes next to the one that provoked it*), and no
brief names it. Also: `00-shared.md`'s *of `<<`, only "shift count 0..63 or
it aborts"* leaves out the same parenthesis's *i64 only*, which matters to P's
sentence 2: *Overflow aborts at every width, but `<<` shifts the bits and
wraps* sets `<<` against every width, while `<<` exists at one.

### 7. TRUE with two edges, `blind/README.md` P sentence 3: *a code outside 0 to 255 aborts, a literal one is refused*

Command: `critic-scratch/runone.sh` over `critic-scratch/exit/*.hero`; `sed -n
'1,50p' selfhost/check/exit_code.hero`.

Printed: refused `int_out_of_range` for `256`, `0x100`,
`9223372036854775807`, `exit(256)` unlabelled, AND `-1`; built and aborted at
the call, after `before`, for `255 + 1`, a `=` name holding `256`, a
`constant` holding `256`, a cell written `300`, and `len(args()) + 299`;
`exit(code: 255)` exit 255. The abort reads `panic: .must() on an error:
does_not_fit: `exit(code:)` takes 0 to 255, the statuses a shell reads, and
was given 256`, at the program's call. The check's header: *A literal written
as the code, under its minus signs, is refused ... Anything else is a computed
code and stops the program where it runs, an operator on literals included*.

Finding: the sentence holds for task X. Two edges a reader of P would get
wrong: `-1` is refused, though the grammar makes it a unary minus on a literal
(§ 7's `Unary`), not a literal; and a value § 4 calls *known* (a constant, a
`=` name, an operator on literals) is not refused, which a reader of § 4's
vocabulary might predict. And the run-time abort names a `.must()` the program
never wrote, which item 5's *at what price* does not ask about.

### 8. FALSE as framed, `00-shared.md` item 2 and `compiler-engineer.md` item 2: *design.md § 4.14 defines `1 << 63` as `INT64_MIN`* ... *Both directions are routes: the sentence says the wrap, or the compiler aborts*

Command: `sed -n '2032,2090p' docs/design.md`; `sed -n '165,200p'
selfhost/emit/operator.hero`.

Printed, design.md § 4.14: *`1 << 63` is `INT64_MIN`, not an abort. The shift
is done on the unsigned bit pattern and cast back — defined for every input,
where C leaves a signed left shift into the sign bit undefined. Without the
sign bit reachable, a mask set cannot name its own top flag, which is the
whole use.* And: *`>>` is arithmetic, written explicitly rather than relying
on C's implementation-defined choice.* The emitter writes `(int64_t)((uint64_t)l
<< (uint64_t)r)` for `<<` and an explicit sign-propagating form for `>>`, its
comment: *defined for every input, which is what makes 1 << 63 INT64_MIN
rather than a trap*.

Finding: design.md, the source of truth (CLAUDE.md), does not only define one
value: it rules that `<<` is defined for EVERY input and gives the reason
(masks naming their top flag). The briefs quote one value and so present the
abort route as open. The compiler-engineer's route *aborting on a shift that
loses a set bit or changes the sign* aborts `1 << 63` itself (it changes the
sign), which § 4.14 refuses by name with its reason; a route that aborts only
on a LOST set bit keeps `1 << 63` and is a third route nobody listed. Any
abort route amends design.md § 4.14 and must answer its reason; the briefs
should say so before a seat prices it. Finding 6's `>>` is likewise a
design.md rule the specification does not carry, not an open question.

### 9. TRUE with a correction of reach, `00-shared.md` item 3 and `ffi-pragmatist.md` item 3: *`hero_handle_eq` answers by the address* and the golden *pins `null: false`*

Command: `sed -n '860,930p;738,860p' runtime/parts/alloc.c`; the golden case
copied to `critic-scratch/handle/`, built, run, and `--emit-c` grepped.

Printed: `hero_handle_eq` (alloc.c:903) ABORTS when either side is the dead
page (*panic: `==` read a dead C handle ...*), and returns `a == b` otherwise.
The emitted C empties the binding a `consumes` call gave back
(`h0_a = hero_handle_dead()`), and compiles `kept == nullptr` to
`Outer_eq(&t8, &t9)`, which calls `hero_handle_eq`. The run printed `before:
42`, `null: false`, then the remembered-address panic, as `.expected` pins.
The runtime already keeps the given-back addresses (`hero_dead_mark_locked`,
`hero_dead_holds_locked`, a window of `1 << 20` marks, `HERO_DEAD_WINDOW`),
consulted under a lock at a crossing into C and NOT at `==`.

Finding: *answers by the address* is true of a COPY only; `==` on the handle
itself after it is given back already aborts, so half of § 13's clause is
kept. The route *the runtime made to abort* has a mechanism already in the
tree (the remembered set, a lock and a hash probe per `==`), which the brief
does not point the seat at; and § 13's *a million other lives* is the window
`1 << 20` = 1,048,576, which a narrowed clause would restate.

### 10. TRUE, with a detail the brief drops, `00-shared.md` item 4: *a program of 2,000 functions, none recursive, aborts `stack exhausted` (d435ca009)*

Command: `git show --stat d435ca009`; `git merge-base --is-ancestor d435ca009
afe983265` (yes); `grep -n -i 'stack exhausted' -r runtime selfhost`.

Printed: *A chain of 2,000 FUNCTIONS each returning a record of the one before
builds and aborts at run time, `stack exhausted in records2000.m1`, its frames
needing about 16 MB*. The words come from `runtime/parts/stack.c`, at three
lines read in context: 497, Windows' handler under the sanitizer
(`hero_stack_asan_veh`); 683, the POSIX fault handler that reads the guard
page; 950, Windows' own (`hero_stack_veh`).

Finding: the chain's frames grow with the depth (each record holds the one
before by value), so the case exhausts the stack by the frames' SIZE as much
as by their number. A 2,000-deep chain of small frames is a different shape,
and a single call whose frame alone exceeds the stack, or a thread's smaller
stack (`stack.c:313` names `thread_deep`), are shapes beside it nobody
listed: the draft *a call chain deeper than the stack aborts* should be
tested on all three before it is priced.

### 11. FALSE at the frozen head, `00-shared.md` item 7: *measurement 060: of 242 codes, none moved its pinned output in the last week*

Command: `head -24` of `docs/measurements/060-*.md`; `git diff
--diff-filter=MD --name-status 0122c9d1c afe983265 -- tests/golden`, then
`git diff 0122c9d1c afe983265 --` the three `.expected` files it named in
`check/` and `full/`.

Printed: 060 was measured *on the trunk at `0122c9d1c`* (step 0). Step 1
(`fafece48e`, the same day) then changed `check/fixedbugs-2224-...expected`'s
two `int_out_of_range` lines (*`256` does not fit a `u8`* became *`256` is not
a status `exit` can end a program with, which is 0 to 255*) and REMOVED its
`type_mismatch` line (*expected `u8`, found `i64`*), and rewrote
`full/fixedbugs-2230-...expected`'s `int_out_of_range` cases (one `missing_return`
line moved a column only, which 060's method does not count).

Finding: by 060's own definition (*a line of an `.expected` naming `[<code>]`
changed or removed*, places left out) at least `int_out_of_range` and
`type_mismatch` moved their pinned output in the last week, by this
milestone's own step 1. The number is true of `0122c9d1c` and stated in the
shared brief as true of the tree the sitting reads; item 7 should date it, or
re-run 060's program at `afe983265`. Its *242* is also narrower than the brief
says: *every code a golden case annotates*, 060's line 7, not every code the
compiler can write (finding 16).

### 12. TRUE, `00-shared.md` item 8 (*69 programs at `v0.5.0`*) and `paragraph-draft.md` (*the release corpora since `v0.3.0` lost none to a repair, the one loss being `exit`'s byte*)

Command: in `critic-scratch/compat/`, for each tag `v0.1.0` to `v0.5.0`: `git
archive --prefix=compat-<tag>/ <tag> examples`, untarred, then `heroes-next
check` on every `.hero` holding a line `^function main(`, the first `error[...]`
of each refusal counted; and `git grep -l '^function main(' <tag> -- examples |
wc -l`.

Printed: `v0.1.0` 67 programs, 60 refused (58 `old_cell_symbol`, 1
`lend_kept`, 1 `callback_name`); `v0.2.0` the same; `v0.3.0`, `v0.4.0`,
`v0.5.0` 69 programs, 0 refused. `git grep` 67, 67, 69, 69, 69.

Finding: holds at the frozen head. The milestone page's *1 of 69 at each of
`v0.3.0`, `v0.4.0` and `v0.5.0`* (`examples/interpreter/main.hero:63`, the
`u8` `exit` of 737e99535) was measured before step 1; step 3's body re-measured
`v0.5.0` alone; v0.3.0 and v0.4.0 now check whole as predicted. One caveat the
draft's sentence hides: the loss it names was not a repair but a language
change (`exit(code: u8)`, M-panic-location), and it was healed by changing
`exit` back, which after `1.0.0` the author's rule would not allow in the
other direction; and step 1 itself refuses a program step 0 accepted
(`c: u8 = 3; exit(code: c)` is now `type_mismatch`, run in
`critic-scratch/exit/k_u8_cell.hero`), a loss no release corpus can see since
no release had `exit(code: u8)`.

### 13. ROUTE and a missing source, `00-shared.md` item 3 and `ffi-pragmatist.md` item 3: *§ 13 says it aborts; ... `hero_handle_eq` answers by the address*

Command: `gh issue view 2245 --repo heroes-lang/heroes --json body`; `grep -n
'first crossing' docs/design.md`, `sed -n '4215,4245p' docs/design.md`, the
warts' numbers by `awk` over Part 8 (line 3864 on); `git show c8b1bc878 --
spec/heroes-spec.md`.

Printed: issue 2245, item 2: *Default running: no abort, design.md Part 8
wart 20's reading (a copy made before the call is refused at its first
crossing)*, and *the specification wins by CLAUDE.md § 12 unless the sitting
rules otherwise*. design.md Part 8, wart 20 (*One copy of a value holding a
`ptr` can free what every other copy holds*), narrowed at panel 177: *every
crossing of a handle into C is checked ... and `==` on a dead value aborts. And
the runtime REMEMBERS the address itself, so a copy made before the call is
refused at its first crossing, until C hands the address out again or 2^20 =
1,048,576 more lives have ended*. § 13's clause was added by `c8b1bc878`
(2026-09-25), the same commit that wrote that design.md text.

Finding: neither brief cites wart 20, which is design.md's own statement of
this behaviour and the issue's ground for its default. Read with it, § 13's
*handing it to C again aborts, `==` on it aborts, and so does a copy made
before* has a second reading, *so does* attaching to the crossing, under which
the runtime keeps the sentence exactly (finding 9: `==` on the given-back
handle aborts, a copy is refused at its crossing, `kept == nullptr` is not a
crossing). So a third route nobody listed: the sentence is ambiguous rather
than false, and the repair is its wording, neither runtime nor clause changed.
The briefs also leave out CLAUDE.md § 12's *spec beats compiler*, which the
issue names as the default precedence for exactly this item.

### 14. FALSE, `paragraph-draft.md`: *It runs one way*: rests on *CLAUDE.md § 10, no fourth input class, and panel 056's refusal of a per-project file*

Command: `sed -n '1,34p;40,80p' docs/panel/056-what-a-project-file-would-carry.md`;
`grep -n -i -B2 -A8 'fourth input class' .claude/rules/cli-surface.md`;
`awk '/^## 2\. Principle 0/,/^## 3\./' CLAUDE.md` (frozen head).

Printed: panel 056 (ratified 2026-08-15): *A is refused* (A being *a Part 6
row: never*), *C is refused*, *B is the only option no judge opposed*, B being
*examined and deliberately unplaced (panel 039's precedent): a greppable
paragraph naming the conditions that return it*. cli-surface.md: *A
per-project file is none of them, so this rule does not admit one (CL-016).
The refusal is not permanent: it is conditional on panel 056's three return
conditions, which are its only amendment path.* CLAUDE.md § 2: *Before
`1.0.0`, no form enters to keep an older program compiling ... no declared
version*.

Finding: panel 056 refused the permanent refusal. The per-project file is
unplaced with three return conditions, and the rule that excludes it says it
is not permanent; the author's rule binds BEFORE `1.0.0` only. So the
paragraph promises for all of `1.x` that *a program cannot say which version
of the language it was written for*, and nothing in the tree makes that true
after `1.0.0`: a flag (argv) and the environment are two of the three input
classes cli-surface.md admits, and a project file may return. A route nobody
listed: the paragraph's sentence rests on a permanent ruling (a design.md
Part 6 row for a declared version, with its falsifier, CL-005), or it is
written as what `1.0.0` itself does rather than what `1.x` will never do.
Also, `00-shared.md`'s frame quotes § 2 without its second sentence, *A change
that refuses a `0.x` program is told, never undone for it*, which reaches
further than forms: it refuses a route that reverts a change to heal an old
program.

### 15. ROUTE, `ffi-pragmatist.md` item 6 and `paragraph-draft.md` (*those of `hero_os.h`*): the header task 2237 changes is one programs bind too

Command: `gh issue view 2237 --json body`; `grep -n HERO_RUNTIME_ABI
runtime/*.h`; `git grep -l -E '^extern "hero_os\.h"' -- examples|tests|selfhost
| wc -l`, the same at `v0.5.0`; `git grep -n -E '^extern "heroes_runtime\.h"'`.

Printed: task 2237's *where to look* is *`runtime/heroes_runtime.h` (every
`_at` and the old name beside it)*, not `hero_os.h`; `HERO_RUNTIME_ABI` is
defined in `heroes_runtime.h:108` (31). `hero_os.h` is bound by 10 files of
`examples/` (10 at `v0.5.0`), 66 of `tests/` (35 of them the recovery
corpus's frozen copies), 11 of `selfhost/` (design.md Part 7 item 2 said 12 on
2026-10-27). `heroes_runtime.h` is bound by `selfhost/cli/help.hero:28`,
`selfhost/cli/io.hero:41` and two `check` goldens.

Finding: the brief points the seat at `hero_os.h` for 2237's break, while
2237 removes names from `heroes_runtime.h`; and a program CAN bind that
header, as the compiler itself does. The paragraph names `hero_os.h` and is
silent on `heroes_runtime.h`, so whether the promise covers a program that
binds the runtime's own header (and so whether 2237 must land before `1.0.0`)
is a question no brief asks. `hero_os.h` itself declares about 57 functions
(`grep -c` of declaration lines), many the compiler's own helpers
(`hero_file_stage`, `hero_fs_deny_delete`, `hero_run_serial`); the paragraph
promises all of them.

### 16. ROUTE, `00-shared.md` item 7: *of 242 codes* is the codes the goldens pin, not the codes `1.0.0` would promise

Command: in the critic's worktree, every `code: "<name>"` literal of
`selfhost/*.hero` outside `test` blocks, comments and `fail(code:` lines
(`critic-scratch/codes/written.txt`, 318 names, some of them `T?` failure codes
and other records' fields, so an over-count) against every `#~`/`#~v` mark
under `tests/golden/` (`annotated.txt`, 241 names; 060 counts 242 by its own
program); then `git grep -l -F "[<code>]" -- tests` per candidate; then
`heroes-next check` on two three-line programs.

Printed: of the candidates that read as diagnostics, `module_name_reserved`,
`module_names_collide`, `module_binding_taken`, `module_is_not_a_value`,
`if_without_else`, `missing_value`, `not_indexable`, `returns_nothing`,
`not_fallible`, `ffi_missing_tag`, `ffi_flexible_array_member`,
`not_a_function`, `variant_not_callable`, `builtin_shape`, `not_bool`,
`used_twice`, `not_a_module`, `repeated_parameter_name`, twelve `expected_*`
codes of the parser (`expected_map_colon`, `expected_hole_close`, ...) and two
more of its (`one_library_per_group`, `empty_fixed_array`) appear
`[bracketed]` in no file under `tests/`. Run: `x = 5; print(x[0])`
gives `error[not_indexable]`, and `x = if c` with no `else` gives
`error[if_without_else]`.

Finding: 060 measures the churn of the codes a golden pins (its line 7 says
so, and that `selfhost/diag.hero` keeps no list of them all). A promise of
codes covers every code `heroes check --json` can hand a tool, among them
codes no case pins at all, whose stability no instrument here can see. The
sitting is asked to promise codes on a measurement of a subset; the subset's
size against the whole is unmeasured (my count is a grep, not a list, so it
is a question with a lower bound of two codes run, not a number).

### 17. FALSE path, `compiler-engineer.md` item 1: *`selfhost/ir/containers.hero` for a record*

Command: `sed -n '1,30p' selfhost/ir/containers.hero`; `git grep -n -i -E
'record (construction|literal)|construct' -- selfhost/ir`.

Printed: containers.hero's header, *The IR's containers: what a function, a
block and a program ARE*; its records are `Slot`, `Block`, `Function`,
`Program`, `Lowered`. A record's construction is lowered in
`selfhost/ir/flatten.hero:538`, *Record construction IS a call with named
arguments*, through `lower_args` (line 543), the same routine as a call's.

Finding: the brief sends the seat to a file that holds no lowering of a
record. The other paths it names exist (`flatten.hero` 1,424 lines, its
`place` at line 193; `check/labels.hero` 484, which with `label_errors.hero`
writes `wrong_label`; `check/exit_code.hero` 127; `runtime/parts/stack.c`
1,064; `tests/harness/suite_compat.hero` 598). That a record's fields go
through a call's own routine is itself the answer the brief asks for there.

### 18. FALSE, `spec-warden.md` item 6: *the README is measured by nothing; design.md by nothing either*; and a cost of the README as a home no brief lists

Command: `heroes-next measure README.md` and `measure docs/design.md` (offline,
no key in the environment); `sed -n '40,80p'
.github/workflows/deploy-site.yml`; `grep -n -F README site/src/lib/*.ts`;
`awk '/^## The README follows the site/,...' site/CLAUDE.md`.

Printed: `measure` counts both (README.md 4,540 on the vendored maximum,
design.md 102,113) and says *No ceiling judges `README.md` (panel 123 R5), so
this is a count and not a verdict*. deploy-site.yml's `paths:` holds
`README.md`, `spec/heroes-spec.md`, `docs/panel/**`, `docs/measurements/**`,
and not `docs/design.md`; its comment: *README.md, the repository's front
page, whose shared facts it checks*. `site/src/lib/claims.ts:64` reads
`README.md`, and `checkReadme` holds its rows at every site build.
site/CLAUDE.md, *The README follows the site*: *A change to what the site
says carries the README's matching change, in the same commit ... a claim, a
number or a promise the README repeats does.*

Finding: no CEILING judges the README or design.md, which is what the brief
means; but the README is read and checked at every build of the site, and a
push touching it publishes the site (CLAUDE.md § Hard stops). So the homes
differ in what they owe beyond tokens: the README, a promise outward-facing
under site/CLAUDE.md (its register, *no em dashes ... absolute*, a reader never
assumed an expert, the site's matching page in both editions, the Italian a
deliverable by CLAUDE.md § 11) and a site publish; the specification, the
budget and a site publish; design.md, neither. The brief prices only tokens.

### 19. TRUE, `spec-warden.md`: *`10,343` real on `claude-opus-5` and `7,890` on the vendored maximum, against a ceiling of 16384; step 1's § 11 sentence spent +15 real*

Command: `env -u ANTHROPIC_API_KEY heroes-next measure spec/heroes-spec.md`
(no `--refresh`, nothing paid); `grep` of `docs/measurements/010-spec-budget-ledger.md`;
design.md § 1.6 (line 253 on).

Printed: `maximum 7890`, `real 10343 claude-opus-5, 2026-10-29`, *Headroom:
6041 against the 16384 ceiling — but the FFI floor mortgages 60 of it (panel
030 R3), so what is measured against the ceiling is 10403*. Ledger row 237:
*10,343 real ... against 10,328 at row 120 (+15)*. design.md § 1.6: 16384.

Finding: true. Two details for the seat: the headroom is 5,981 once the
mortgage is counted, not 6,041; and ledger row 237's right-hand column already
says *§ 11 says ... a literal one is refused where it is written*, which the
§ 11 sentence it prices does not say (item 5's question): the record
describes the sentence as saying it.

### 20. FALSE, a negative claim, `paragraph-draft.md`: *an error a program does not handle ends it at once, saying where, and nothing can catch that stop*

Command: `sed -n '1,70p' runtime/parts/panic.c`; then in
`critic-scratch/catch/`, a header of the program's own, `catcher.h`, whose
`catcher_install()` calls `signal(SIGABRT, ...)` with a handler that writes a
line and calls `_exit(0)`, bound by `extern "catcher.h"` / `function
catcher_install() -> i64`; `main` installs it, then reads `xs[len(args()) +
4]` of a two-element array; `heroes-next build`, binary run under `timeout 10`.

Printed: panic.c, *Every death the runtime initiates is a line on stderr
followed by `hero_abort()`, which is SIGABRT*; `hero_abort` is `abort()`. The
run: stdout `installed: 0`; stderr `panic: array index out of range` / `at
catch.hero:8:11, in catch.main` / `caught: the stop did not stop`; **exit 0**.

Finding: no Heroes code can catch the stop, but the program's C can, through
the FFI the specification gives every program (§ 13; a callback,
`atexit(f: ...)`, is § 13's own example): the abort ran a handler of the
program's choosing and the process reported success. A supervisor that
restarts on a failure status, the paragraph's own deployment model, would see
`0`. The sentence must say *no Heroes code* or the promise must say what a
SIGABRT handler bound at the C boundary is (the ffi-pragmatist's question,
not named in its brief). The runtime installs one itself (the lease handler of
`parts/os.c`), which a program's `signal` replaces.

### 21. FALSE paths and lessons not carried, `blind/README.md` and `blind/design.md`

Command: `find docs/panel/232-briefs -name '*design*'`; `git ls-files | grep
run_one`; `sed -n '88,110p;150,215p' docs/panel/230-briefs/blind/README.md`;
`sed -n '160,200p' .claude/skills/panel/SKILL.md`; the sections of
`docs/panel/230-briefs/blind/brief-m.md` and `brief-p.md`; `claude --help`
(local, nothing spent), `claude --version`.

Printed: no `blind-design.md` exists (the README cites it; the file is
`blind/design.md`). No `run_one.sh` of panel 230 is in the tree (`git ls-files`
finds 215's, 217's and 228's), though `blind/design.md` says *run as panel
230's `run_one.sh` ran it*; 230's README records its digest only, the file
lived in a scratch folder. 230's README: *Changed after the first session ...
`run_one.sh` gained `< /dev/null` on the `claude` line* (session 1 had waited
on stdin), and its command ends `2> ...`; *10 family M sessions (Sonnet 5.5)
were refused by its safeguards, category `reasoning_extraction` ... the
brief's `choice_points` section*, after which `brief-m.md` lost that section
(it holds only `context` today) and family M ran again. Every flag of the
command exists in claude 2.1.308.

Finding: panel 232's command has neither `< /dev/null` nor a stderr
redirect, and its single `brief.md` keeps a `## choice_points` section for all
84 sessions, 42 of them on `claude-sonnet-5-5`, the model and section 230
measured refused (the panel skill still names `choice_points` as the safe
wording, from panel 184; 230's finding is newer than that line). Whether
Sonnet refuses it on a PREDICTION task is unrun and cannot be run without a
paid session; it is a question the coordinator owns before the first session,
since a refused session is scored as no answer and never re-run. Also
unverifiable here: whether the account's usage cap that refused 42 of 230's
sessions still stands, and whether 2.1.308 accepts both model names (the
skill records 2.1.274 refusing one).

### 22. FALSE by its own words, and a route nobody listed, `paragraph-draft.md`: *so a newer compiler never refuses an older program* and *does what it did*

Command: the draft read against itself; in `critic-scratch/keys/`, a program
filling a `{str: i64}` with `key0` to `key11` and printing `join(keys(m),
",")`, built once and run five times.

Printed: five identical lines,
`key0,key1,key11,key4,key10,key8,key5,key2,key7,key3,key9,key6`. § 10:
*`keys(m) -> [K]` gives the keys in no order*.

Finding, first: the draft's first paragraph excludes *a program that depends
on a defect of the compiler, which a 1.x release may repair*, and a repair of
a defect of the class *a wrong one accepted* (verification.md's `blocking`)
refuses an older program; its second paragraph then says *a newer compiler
never refuses an older program*, joined to its premise by *so*, the connective
CLAUDE.md § RUN IT names as the tell of an inference. The two sentences
contradict each other. *An older compiler is never asked to read a newer one*
is a statement of what people will do; what it can mean is that the promise
does not cover that direction. Second: *does what it did* is broken by any
change of the runtime's hash, for a program that prints `keys(m)` unsorted,
whose output is deterministic today though § 10 calls it *in no order*. The
draft names no exclusion for behaviour the specification leaves open, which
Go 1's document does by name (*unspecified behavior*, an exclusion the
historian's brief lists; its fuller list, *specification errors* among it,
is read in finding 27). Routes nobody listed: the paragraph
excludes what the specification leaves open; or the runtime makes such an
order vary between runs so that no program comes to depend on it.

### 23. ROUTE, `00-shared.md` item 8 and `compiler-engineer.md` item 8: corpora nobody listed, and the count the brief asks for

Command: `git ls-tree -r --name-only v0.5.0 -- tests/golden/run | grep -c
'\.hero$'` and `git grep -l '^function main(' v0.5.0 -- tests/golden/run | wc
-l`; the same for `selfhost`; `git ls-files tests/thesis | grep -E '\.hero$' |
grep -c -i ref`; `git grep -n heroes-spec.md -- tests/harness/suite_special.hero`;
§ 4's code block cut out of the spec, a `main` added, built and run
(`critic-scratch/specex/s4.hero`); per-file expected outputs of `v0.5.0`'s
69 programs.

Printed: `run/` at `v0.5.0`, 563 `.hero` files, 544 holding `function
main(`; `selfhost/` at `v0.5.0`, 684 modules; 10 reference programs under
`tests/thesis/`; `suite_special.hero:349`, the sqlite example *CUT OUT OF
`spec/heroes-spec.md` at run* time, the one block of the specification the
net compiles that I found; § 4's block builds and prints `4105`. Of
`v0.5.0`'s 69 programs, 67 have an expected output (`main.expected` or the
gallery's per-file one); `examples/curl/main.hero` and
`examples/gallery/09-holes.hero` have none.

Finding: the briefs offer two corpora, `examples/` and the golden `run/`
programs. Three more exist and nobody names them: the specification's own
code blocks (the promise's own text, of which one is compiled today), the
thesis suite's reference programs, and the compiler's own sources at each
tag, the largest Heroes program there is. What would have to be true for
each: the spec's blocks are complete programs or can be made so (§ 4's can);
the references are held to every case already; `selfhost/` at a `1.x` tag
compiles under a later `1.x` compiler, which is not today's bootstrap rule (a
compiler is built by the seed before it). For *does what it did*, the
`.expected` files cover 67 of 69 programs, not all.

### 24. ROUTE, `00-shared.md` § What each seat is for: item 7 has no seat that reads the tree, and the paragraph's sentences have no seat that tests them

Command: the assignments read against the items; `sed -n '2278,2366p'
docs/design.md` (§ 4.17).

Printed: the compiler-engineer holds items 1, 2, 4, 5, 8; the ffi-pragmatist
3 and the C boundary; the spec-warden prices 1 to 6; the historian brings
precedent for 7 among others; the blind seat 1, 2, 5. design.md § 4.17: *When
`"schema"` moves: when a member's meaning changes or a member leaves. A member
is added with what its absence means*.

Finding: item 7 (whether `1.0.0` promises a code) is measured in the tree by
no seat: the only input is the historian's precedent and a measurement taken
before step 1 (finding 11) over a subset (finding 16). A route nobody listed
for it: the project already versions what it hands tools, `"schema": 2`, and
its rule moves the number for a member, not for a code's value, so a promise
of codes could be written as part of that schema's rule (a renamed or split
code moves `schema`) rather than as a list. And no seat is asked whether the
paragraph's sentences are TRUE of the compiler; the spec-warden prices them,
the ffi-pragmatist reads its C-boundary reach. Findings 14, 20 and 22 are
that test, done here once; the sitting should name who owns it.

### 25. Smaller ones, each run

- **UNVERIFIED, `00-shared.md`**: *By the author's yes of that day, asked once
  for the milestone: the full panel, and the blind seat ... within 12 USD*. No
  file at `afe983265` records the yes (the milestone page says the gate is
  *asked once of the author*); it is a conversation's fact, to be written in
  the sitting's file.
- **MOVED, `00-shared.md`**: *The lane `compat-promise` at `afe983265`*:
  `git worktree list` shows the lane at `89ebb0035` since 20:12, one commit
  re-blessing `tests/emission/run-fixedbugs-437-...c` and `-589-...c`
  (Linux arm64). Nothing the compiler's key covers moved; the four seats'
  worktrees are detached at `afe983265` (`git worktree list`, read from git's
  metadata, no seat's files opened).
- **INCONSISTENT, `proposal.md`**: its frame lists two kinds of item (*a
  sentence saying what the language already does, or a promise about
  `1.0.0`*), while `00-shared.md` lists three, the third *a sentence the
  compiler does not keep and one of the two repaired*; items 2 and 3 offer
  routes that change the compiler, which the proposal's frame hides.
- **AMBIGUOUS, `00-shared.md` item 6**: *the sentence § 10 forces* means
  CLAUDE.md § 10 (issue 1066's body: *because §10 admits no fourth input
  class*), written bare in a sitting whose every other `§` is the
  specification's, where § 10 is *Strings, arrays, maps*.
- **NOT WHAT IT RESTS ON, `paragraph-draft.md`**: *ends it at once, saying
  where* rests on § 6's *saying why*; and `leave: (function(code: i64) -> ())
  = exit` then `leave(code: len(args()) + 299)` aborts `at <heroes
  library>:232:1, in library.exit` (run, `critic-scratch/exit/
  l_through_value.hero`), a place the program does not contain (panel 230's
  R1 by design). *Saying where* holds only as *naming a place*.
- **TRUE**: § 13's quotation in `ffi-pragmatist.md`, word for word
  (spec line 440); design.md Part 6's row *Exceptions*; Part 7 item 2's
  amendment by decision 2228, quoted exactly; § 6's *An abort ends the
  program at once, saying why*; ROADMAP row 74; issues 1066, 2245, 2217
  (closed, the author's *A* of 2026-10-25), 2250, 2228, 2237, 2213 exist with
  the described subjects; `--refresh` refuses every path but the spec and
  CLAUDE.md (`selfhost/measure/pinned.hero:138`); `/build` ignored
  (`.gitignore:7`); 3 × 2 × 2 × 7 = 84 and 84 × 0.14 = 11.76; 14 P sessions
  per family; panel 010's ambiguity is the one the briefs describe (*"left-to-
  right" is ambiguous between call order and signature order*), and labels
  through a function value are refused out of order too (`u_value_labels`).
- **NOT MEASURED, by rule**: *about 40 s a generation* (`compiler-engineer.md`)
  and the blind sessions' cost; no clock was started and no paid call made.

### 26. FALSE as a reader would take it, finding 4 reaches calls: an `@` argument's place is checked after the arguments after it, `blind/README.md` P sentence 1 (*arguments ... a place before the value*)

Command: `critic-scratch/runone.sh` over `critic-scratch/ord/v_at_arguments.hero`
and `w_at_argument_bounds.hero`: `bump(@n: i64, by: i64)` called
`bump(n: @ys[noisy(label: "place", value: K)], by: noisy(label: "value", value:
5))`, and `bump2(by: i64, @n: i64)` with the `@` argument second.

Printed: in bounds, `place value` and `value2 place2` (left to right as
written, whichever side the `@` stands), then `57`. With `K = 5` on a
two-element array: `place`, `value`, then `panic: array index out of range at
w_at_argument_bounds.hero:14:5`, the call's column, exit 134. A by-value
argument `xs[noisy(label: "first", value: 5)]` (probe `e`, finding 4) aborted
before the second argument was evaluated.

Finding: an `@` argument's index is evaluated in its turn, but its bounds
check runs after the later argument (one was tried; *every* later argument is
unrun), at the call's column; a by-value index checks at once. So *arguments [left to right]* holds for the expressions and not for
the aborts, and the two kinds of argument differ. Neither the probe, the
compiler-engineer's list (*`@` arguments*, asked of the order and not of the
check) nor P's sentence reaches it.

### 27. PARTIAL, `historian.md` item 1: Go 1's document *excludes (bugs, unspecified behaviour, struct literals without names, a security fix)*

Command: `git grep -n -i -E 'go1compat|Future of Go Programs' -- docs` (no
record in the tree quotes it); then `curl -s --max-time 20
https://go.dev/doc/go1compat | head -c 200000` into
`critic-scratch/go1compat.html`, read 2026-10-29, its exclusions printed with
the tags stripped.

Printed, the page's own headings of its exclusions, in order: *Security*,
*Unspecified behavior*, *Specification errors* (*If it becomes necessary to
address an inconsistency or incompleteness in the specification, resolving
the issue could affect the meaning or legality of existing programs. We
reserve the right to address such issues, including updating the
implementations.*), *Bugs*, *Struct literals*, *Methods*, *Dot imports*, *Use
of package unsafe*.

Finding: the brief names four of eight. The one it leaves out that matters
most is *Specification errors*: items 2, 3 and 4 are exactly that, a
specification and a compiler that disagree, and the paragraph's draft
excludes a defect of the COMPILER but not an error of the SPECIFICATION, so
after `1.0.0` the next item 2 would have no exclusion to land under. *Use of
package unsafe* is the analogue of the C boundary the paragraph covers
(`hero_os.h`, finding 15). A route for the paragraph nobody listed: an
exclusion for errors of the specification, in its own words. The historian
must still read and mark the page `verified` itself; this is the critic's
reading, not a citation.

### 28. Precision to finding 8: the third route keeps both values the briefs cite

Command: arithmetic on the bit patterns, checked against the run of
`critic-scratch/shift/g_negative_left.hero` (`5 << 62` printed
`4611686018427387904`, its top set bit gone).

Finding: `3 << 62` moves bits 0 and 1 to bits 62 and 63 and `1 << 63` bit 0
to bit 63: neither loses a set bit, both change the sign. So the compiler-
engineer's route as written (*a shift that loses a set bit or changes the
sign*) aborts both programs of task S, and design.md § 4.14 refuses it for
`1 << 63` by name; a route that aborts only on a lost set bit aborts neither,
keeps § 4.14, and aborts `5 << 62`. A reader's prediction for task S is the
same under that route and under the wrap.

### 29. FALSE premise handed to the historian, `historian.md` item 2: *which languages fix left to right everywhere (Java, C#, Go's statement on function calls and operands)*

Command: `curl -s --max-time 30 https://go.dev/ref/spec | head -c 2000000`
into `critic-scratch/gospec.html`, read 2026-10-29, its section *Order of
evaluation* printed with the tags stripped; compared with probe `a`
(finding 4's last paragraph).

Printed: *all function calls, method calls, receive operations, and binary
logical operations are evaluated in lexical left-to-right order ... However,
the order of those events compared to the evaluation and indexing of x and the
evaluation of y and z is not specified*, and the example `x := []int{a, f()}
// x may be [1, 2] or [2, 2]: evaluation order between a and f() is not
specified`, with the same for a map literal's key against its value.

Finding: Go does not fix left to right everywhere; it orders calls among
themselves and leaves a read beside a call unspecified. The brief lists it
among the languages that do, so the historian is handed a premise the page
contradicts. It matters for the sentence: Heroes ORDERS that case (`v +
bump(@v)` read `v` first, printed `1` then `2`; a map literal printed `k0 v0
k1 v1`), so it is stricter than Go, and a sentence modelled on Go's would
under-describe the compiler. The historian marks its own reading `verified`;
this is the critic's reading of the page, given so the brief can be
repaired before the seat starts.

### 30. FALSE as written, `00-shared.md` and `spec-warden.md`: *`heroes measure --refresh` is the token-counting endpoint every landing has called* / *every landing in the ledger has called*

Command: in `docs/measurements/010-spec-budget-ledger.md`, the rows (`^\| *[0-9]+
*\|`) counted, those naming `refresh` and those naming ` real`; the last row
naming neither, by `awk`.

Printed: 121 rows; 34 name `refresh`, 52 name a `real` count; the first row
naming `refresh` is at line 139 (the rows begin at line 34, vendored counts
such as `| 2231 | panel 023 |`); the last row not naming it is line 228 (the
ceiling's raise of 2026-10-14), with 9 rows after it.

Finding: at least 69 of the 121 landings carry no real count, so the
endpoint is what the RECENT landings called, not every one. The sentence
justifies the spec-warden's paid call, so the error changes no route; it
should read *every landing since ...*, with the day the record gives.

### 31. Addendum to finding 4, its parenthesis now run

Command: `sed -E 's/<[^>]+>//g' critic-scratch/gospec.html` (the page of
finding 29, read 2026-10-29), lines 6709 to 6745.

Printed: *The assignment proceeds in two phases. First, the operands of index
expressions and pointer indirections (including implicit pointer
indirections in selectors) on the left and the expressions on the right are
all evaluated in the usual order. Second, the assignments are carried out in
left-to-right order.* And its example: `x[1], x[3] = 4, 5  // set x[1] = 4,
then panic setting x[3] = 5.`

Finding: finding 4's *unverified here* is now read: Go's specification states
in words what this compiler does for `ys[i] @ v` and for an `@` argument
(finding 26), the index first, the value, and the bounds panic at the write.
So a two-phase sentence has a precedent that says it outright; the historian
re-reads and marks it.

## Ranked, most consequential first

1. **20** `paragraph-draft.md`: *nothing can catch that stop* is false; a SIGABRT handler in the program's own header turned an abort into exit 0 (run).
2. **14** `paragraph-draft.md` (from issue 1066): *It runs one way* rests on *panel 056's refusal*; 056 refused the permanent refusal, cli-surface.md says it is not permanent, and § 2's rule binds before `1.0.0` only.
3. **22** `paragraph-draft.md`: *a newer compiler never refuses an older program* contradicts its own defect clause; *does what it did* breaks on a hash change (`keys(m)` is deterministic, § 10 says *no order*); no exclusion for what the spec leaves open.
4. **27** `historian.md`: four of Go 1's eight exclusions; *Specification errors*, this sitting's own case, is missing, and the paragraph has no such exclusion.
5. **4, 26** `blind/README.md` P sentence 1: a place's bounds check (assignment, nested place, `@` argument) runs after the value and later arguments; a read checks at once.
6. **8, 28** `00-shared.md`, `compiler-engineer.md` item 2: design.md § 4.14 rules `<<` defined for every input, with its reason; the abort route as written aborts `1 << 63` and `3 << 62`; a lost-bit-only route keeps both.
7. **13, 9** item 3: design.md Part 8 wart 20 uncited; § 13's clause has a reading the runtime keeps; `==` on the given-back handle already aborts; CLAUDE.md § 12 unmentioned.
8. **21** blind seat: `choice_points` kept for 42 Sonnet sessions, refused 10 times in panel 230; no `< /dev/null`, no stderr redirect; `blind-design.md` and 230's `run_one.sh` absent.
9. **29** `historian.md` item 2: Go does not fix left to right everywhere; Heroes orders what Go leaves open.
10. **11** item 7: 060's *none moved in the last week* is false at `afe983265`; step 1 moved `int_out_of_range` and `type_mismatch`.
11. **16, 24** item 7: 242 is the pinned subset (`not_indexable`, `if_without_else` pinned nowhere, run); no seat reads the tree for item 7; `"schema"` is a route.
12. **18** `spec-warden.md` item 6: the README is read and checked by every site build, a push publishes, site/CLAUDE.md binds it.
13. **15** `ffi-pragmatist.md`: 2237 touches `heroes_runtime.h`, which the compiler binds; `hero_os.h` promises ~57 functions.
14. **5** item 1: `map`'s order and `any`/`all`/`find` stopping early are unstated (run).
15. **6** item 2: `>>` sign-extends, design.md's rule, the spec's silence; *of `<<`, only* drops *i64 only*.
16. **10** item 4: d435ca009's frames grow; a single huge frame and a thread's stack unlisted.
17. **2, 1** item 1: § 9 line 266 already orders labels through a value; *says nothing* misses short-circuit, `fold`, `find`.
18. **7** P sentence 3: true for X; `-1` refused, known non-literals abort, the abort names a `.must()`.
19. **23** item 8: spec blocks, thesis references, `selfhost/` at tags unlisted; `run/` 544 programs at `v0.5.0`.
20. **17** `compiler-engineer.md`: `containers.hero` lowers no record; `flatten.hero:538` does.
21. **30, 19, 12, 25, 3, 31** the rest: `--refresh` on 34 of 121 rows; counts true (headroom 5,981); compat 69/69 at v0.3.0 to v0.5.0; smaller items; truths and Go's two phases.

### 32. ROUTE, `spec-warden.md` items 1 and 6: the one-home rule has no single home to give either sentence

Command: `sed -n '49,70p' .claude/rules/spec-shape.md`.

Printed: *Every rule has exactly one home, and the home is the section of the
operation it governs*; and *The definition of abort is in § 6 and nowhere
else ... no section lists every abort*.

Finding: P puts the order of evaluation in § 7, yet it governs § 7's
operands, § 9's arguments and record fields, § 5's places and § 2/§ 10's
literals; the brief's *in the section of the operation it governs* names no
section for a rule that crosses four, so the spec-warden is asked to apply a
rule that does not decide this case (the routes: § 7 with pointers from § 5 and
§ 9, priced as words; a sentence per section; the preamble). And if the
paragraph's home is the specification, its third sentence (*an error a program
does not handle ends it at once ... nothing can catch that stop*) is a second
statement of what an abort is, which spec-shape.md refuses outside § 6: that
home costs a merge into § 6, not only tokens.

### 33. TRUE, the premise behind item 8's question (the milestone page: *before `1.0.0` a case is changed with the language it tests*)

Command: `git log --since=2026-09-29 --diff-filter=M --name-only --format=
afe983265 -- 'tests/golden/run/*.hero' | sort -u | wc -l`, the commits the
same way, and `git ls-files 'tests/golden/run/*.hero' | wc -l`.

Printed: 248 of the 622 `run/` programs modified in the 30 days to the
frozen head, by 22 commits.

Finding: the premise holds, by a wide margin; the window is stated (from
2026-09-29), and which of the 22 commits are the language moving rather than a
case repaired is the compiler-engineer's to split (its brief asks this).

### 34. Addendum to finding 12, its last clause now run

Command: in `critic-scratch/step0/`, `git archive 0122c9d1c seed runtime
selfhost` untarred, then `clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes-stage1`, `./heroes-stage1 build selfhost/main.hero -o heroes`,
`./heroes build selfhost/main.hero -o heroes-next`, and `heroes-next check` of
`critic-scratch/exit/k_u8_cell.hero` (`c: u8 = 3` / `print("before")` /
`exit(code: c)`).

Printed: `heroes 0.5.0`, `sources 1500196879cda0c8`; `check exit 0`. The
frozen compiler (finding 12) refuses the same file `error[type_mismatch]:
expected `i64`, found `u8``.

Finding: measured, not inferred: step 1 refuses a program step 0 accepted.
Before `1.0.0` that is allowed and told (CLAUDE.md § 2); it is the shape a
`1.x` release could not take, and no release corpus holds a program that
would show it.

## Placed after the ranking was written

- **32** ranks beside 18: spec-shape.md's one-home rule names no section for an order of evaluation that crosses four, and a paragraph homed in the specification would restate § 6's definition of an abort, which that file refuses.
- **33, 34** close two premises as TRUE and run: 248 of 622 `run/` programs changed in 30 days; step 0 accepted the `u8` `exit` program step 1 refuses.
