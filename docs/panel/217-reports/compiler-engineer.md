# Panel 217, the compiler-engineer: what a read-only lend costs the compiler, built

Written 2026-10-16 from 22:05, in my own copy `<scratchpad>/217-compiler-engineer/` (`git -C
/Users/joseph/Temp/heroes/heroes-lang worktree add --detach … f57af53da`, HEAD
`f57af53dae65fedb66d8915311738b8bb05dd38e`, then `rm -rf build`). `<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad`.
No paid run, no API call, no credential printed. Written as I go; every number names its command,
and what I could not run says *unrun*.

## The log as it happened

- 22:05, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: `real 8.30 user 8.10 sys
  0.16`.
- 22:07, `./heroes build selfhost/main.hero -o heroes-r2`: `real 83.42 user 63.16 sys 14.94`, exit
  0 (load 6.91 when it ended, so the duration is not a measurement of the compiler, only a log line).
- The compiler at `f57af53da`: 646 modules, 168,858 lines (`find selfhost -name '*.hero' | wc -l`,
  `… | xargs cat | wc -l`).

## READY at 22:31: the prototype compiler and F1's key

- **The compiler**: `<scratchpad>/217-compiler-engineer/heroes-f1`, sha256 `e2127c87e9a3…`, built
  by `./heroes-r2 build selfhost/main.hero -o heroes-f1` (`real 46.09 user 43.21 sys 1.16`, exit 0)
  from my copy with the F1 prototype; `./heroes-r2 check selfhost/main.hero` exit 0 before it.
- **The key**: `<scratchpad>/217-compiler-engineer/key-f1.hero`, sha256 `c8e6de270182…`, the kit's
  `counter.h` copied beside it. It is F0's key with two lines changed: `function doubled(c: Counter
  lent) -> i64` returning `counter_value(c) * 2`, and `function counter_value(c: Counter lent) ->
  i64` (the header's `const struct counter *`), the calls `doubled(s.first)`, `doubled(s.second)`.
  `./heroes-f1 build key-f1.hero -o key-f1.bin` exit 0; `timeout 20 ./key-f1.bin` prints `6`, `4`,
  `5`, exit 0. `./heroes-f1 fmt key-f1.hero` prints it back byte for byte.
- **No copy, read from the emitted C** (`./heroes-f1 build key-f1.hero --emit-c`): `int64_t
  h_keyf1_doubled(struct counter *ph0_c)`, called `h_keyf1_doubled(&h0_s.f_first)`, and inside
  `counter_value(&(*ph0_c))`. The judge's `byval` grep finds no by-value `struct counter` parameter
  (count 0).
- **Corrected 22:26**: the heading's *22:31* was written without reading the clock; `date +%H:%M`
  read `22:26` after the key had built and run, so both artefacts existed before 22:26.

## A second compiler, `heroes-f1b`, after a shape the brief did not name (22:26)

`heroes-f1` (above) is unchanged and stays the arm's compiler unless the coordinator chooses
otherwise. `<scratchpad>/217-compiler-engineer/heroes-f1b` (sha256 `8495e9cb21c5…`) differs from it
in one rule: it refuses a read-only lend of a place that an `@` argument **nested inside the same
call's arguments** writes. Measured with `heroes-f1` on `<scratchpad>/217-ce-probes/nested_*.hero`:
`both(p, b: bump(@p))` prints `1` then `2` with `both(a: Pt, …)` and `2` then `2` with `both(a: Pt
lent, …)`, from one call's text, because the lent place is read after `bump(@p)`, which runs before
the call. `heroes-f1` builds both at exit 0; `heroes-f1b` refuses the lent one,
`aliased_mutable_arguments`. The key builds and prints `6`, `4`, `5` under both. The difference
reaches an arm's program only if it writes `@x` inside a call that lends `x` read-only.

## The final prototype, `heroes-f1c` (22:40), and what it refuses

`<scratchpad>/217-compiler-engineer/heroes-f1c` (sha256 `0736f9e5012c…`, built by `./heroes-r2
build selfhost/main.hero -o heroes-f1c`) is `heroes-f1b` plus one more refusal (`@p: T lent` on a
Heroes function, a word nothing reads) and three moves that change no behaviour (below, *where the
lines land*). The key builds and prints `6`, `4`, `5` under it. Every row below is a program in
`<scratchpad>/217-ce-probes/`, built with `./heroes-f1c build <file> -o <bin>`, the code its build
printed, and the binary run under `timeout 10` where it built:

| shape | probe | told |
|---|---|---|
| written, `c.n @ c.n + 1` | `w1_write` | `not_mutable`: *`c` is lent read-only, the caller's own place, so it cannot be written — where the function changes it, take it `@c` …; where it changes a copy, bind one, `n @= c`* |
| `@`-lent onward, `bump(@c)` | `w2_at_lend` | `not_mutable`, the same sentence (the root of an `@` argument is written) |
| returned, `-> Counter` / `return c` | `w3_return` | `ffi_lock_copied` twice, R2's result and copy rules unchanged |
| copied, `d = c` | `w4_copy` | `ffi_lock_copied`, R2's binding rule unchanged |
| beside `@` of the same place, `f(a: @p, b: p)` | `w5_alias` | `aliased_mutable_arguments`: *`p` is lent read-only, the caller's own place, beside this call's `a` argument, `@p`, which writes the same value — the callee reads it after that write, where a copy would not* |
| beside `@p.y`, `p` whole | `w5b_fields` | the same, twice; `@q.y` beside `p` builds |
| an `@` nested in an argument, `both(p, b: bump(@p))` | `nested_lent` | the same code, *`@p` in a call among this call's arguments, which runs before it*; by value (`nested_value`) builds and prints `1`, `2` |
| named as a value, `f = doubled` | `w6_value` | `lent_parameter_as_value` (new code): no function type carries `lent` |
| marked at the call, `doubled(@s)` | `w7_marked` | `marker_mismatch`, existing: *`doubled` does not change this argument* |
| `lent` with no `@` on a C `str` | `w8_c_str` | `lent_shape`, its reason widened: *not a `cstr`, a `ptr` or a group's record* |
| a lent handle ended, `slot_close(s)` | `w9_consume` | `consumed_borrowed_handle`, existing: a parameter without `@` is borrowed |
| `@p: Pt lent` on a Heroes function | `w10_at_lent` | `lent_shape`: `@` hands the place to change, `lent` to read; write one |

What builds and runs (`ok1_shapes`, `ok2_handle`): a lent `Counter` handed on to another lent
parameter, to itself recursively, and to C's `const struct counter *` with no `@`; an `@` parameter
handed to a lent one; the dotted form `pair.second.doubled()` (UFCS reaches a lent first
parameter, lowered as its place); a temporary `label(make(5))` and an element `label(pts[1])`, each
held in a slot of its own and that slot's place handed (an element is never handed in place, so an
array is never unshared for a read); a `for` variable; `str`, `[i64]` and a generic `[T]` lent; a
lent handle read by C; `k = keep(p)` returning a copy that a later `p.x @ 99` does not reach. Under
`--sanitize` the same program exits 0 with an empty stderr and the same output (`cmp`).

**Corrected 22:35**: the heading's *22:40* above was again written without reading the clock;
`date +%H:%M` read `22:35` just after the section was written. From here every time is a `date`.

## Where the lines land, in the layout suite's own unit

Counted with a replica of `code_lines` (`tests/harness/suite_layout.hero:966`: every non-blank
line outside a `test` block, comments included), `<scratchpad>/217-ce-probes/code_lines.py`, over
`f57af53da` (extracted by `git archive` into `<scratchpad>/217-ce-before/`) against my copy. The
replica agrees with the suite where the suite prints a number: `parse/` 8,737 and `flatten.hero`
1,163 after, both from `./heroes-f1c run tests/harness/main.hero -- ./heroes-f1c layout`.

| where | before → after | Δ |
|---|---|---|
| `selfhost/read_lend.hero` (new): which parameter is lent read-only, which argument is handed in place, a call's callee and position, whether a local is a lent parameter | 0 → 131 | +131 |
| `selfhost/check/read_lends.hero` (new): the aliasing sweep, a nested `@` included, and `@` with `lent` | 0 → 182 | +182 |
| `check/lock_copies.hero` (R2): a lent parameter and an argument handed in place are no copy; the fix and the note offer `lent` first | 229 → 249 | +20 |
| `check/markers.hero`: a function with a lent parameter is no value | 77 → 94 | +17 |
| `check/lend_extent.hero` + `check/ffi.hero`: `lent` with no `@` admitted on a group's record | 550 → 555 | +5 |
| `checker.hero`: the sweep wired | 170 → 174 | +4 |
| `data_errors.hero`: two messages | 239 → 260 | +21 |
| `resolve/errors.hero`, `writes.hero`, `walk.hero`: a write to a lent parameter told it is lent | 814 → 826 | +12 |
| `resolve/written_nowhere.hero`: `never_rebound` on an `@` parameter only read names the lent form in a note | 163 → 177 | +14 |
| `ir/lower.hero`: the parameter's slot is the `@` parameter's pointer | 190 → 194 | +4 |
| `ir/flatten.hero` + `ir/inout.hero`: the argument is its place, or a held slot's | 1,265 → 1,286 | +21 |
| `parse/members.hero`: `lent` read after a Heroes function's parameter | 237 → 239 | +2 |
| `emit/`, `runtime/`, `print/` (the formatter), the lexer, `cli/` (the pointee check) | | **0** |
| **total**, 16 files, 2 of them new | | **+433** |

**By pass**: checker 248 (57%), the predicate module 131 (30%), resolver 26, IR 25, parser 2,
emitter 0. Raw `git diff --numstat -- selfhost` at `heroes-f1b` read 508 added and 23 removed; the
table is `heroes-f1c`'s.

**Why the emitter is zero, from the code**: since panel 196's R1 an `@` parameter is a pointer to
the caller's place (`emit/places.hero` `slot_lvalue`, `by_pointer`), and every downstream reader asks
the IR slot, not the AST: the C signature (`emit/signature.hero`), the guarded local and its flow to C
(`emit/lend_local.hero` `flows`), the pointee check (`cli/pointee_wants.hero`), the probe and the
assertions (`emit/probe_params.hero`, `extern_assert.hero`). So `ir/lower.hero` making a read-only
parameter `param_slot(mutable: true)` and `ir/flatten.hero` handing its argument as `.inout_arg`
is the whole back end. The ownership pass needs nothing either: a parameter, `@` or not, is out of
the exit sweep (`ir/own.hero` rules 1 and 2), and an `.inout_arg` makes `ir/survival.hero` assume
its root written, which for a read is conservative, never wrong.

**Rows that move.** `BUDGETS` `selfhost/parse/` 8,735 → **8,737** (it stood exactly at its budget, so
any parser line moves it); `DECIDED` `selfhost/ir/flatten.hero` 1,150 → **1,163** (the file stood at
1,148). `resolve/writes.hero` went to 303 at `heroes-f1b` and is 291 at `heroes-f1c`, the one
function moved to `read_lend.hero`. Both moved rows could be paid: `flatten.hero`'s two argument
loops, `lower_args` and `with_receiver`, are one loop written twice (their own comments say so), and
F1 adds its branch to each; merging them is the seam, unbuilt here.

**Corrected at once, 22:37**: the checker's share is **249** (182 + 20 + 17 + 5 + 4 + 21, the table's
own rows), not 248; the total 433 stands. And *their own comments say so* is false: the comments of
`lower_args` and `with_receiver` say no such thing; what I read is that their loops over the written
arguments are the same lines (`ir/flatten.hero`, both `for arg in given` blocks, read in my copy).

**Corrected 22:36**: the *22:37* of the line above was written before `date` read 22:36. Times below are taken by `date` in the command that writes them.

## F0 is not nothing: two of the compiler's fixes send the reader back and forth (measured 22:38)

Built in `<scratchpad>/217-ce-probes/`. A helper that reads a field of the kit's `Counter`,
`return c.n * 2`, the shape R2's own note teaches (*a field is read in place, `c.n`*):

| compiler | helper | told | its fix |
|---|---|---|---|
| `heroes-r2` | `doubled(c: Counter)` (`f0_removed`) | `ffi_lock_copied` twice | *take it by place, and lend it at each call with `@`* (guess) |
| `heroes-r2` | `doubled(@c: Counter)` (`f0_note`) | `never_rebound` | *remove the `@`: `c` is read, never written* (guess) |
| `heroes-f1c` | `doubled(@c: Counter)` | `never_rebound`, plus the note *take it read-only in place … `c: Counter lent`* | the same |
| `heroes-f1c` | `doubled(c: Counter lent)` (`f1_note`) | builds, exit 0 | |

So at `f57af53da` each refusal's fix produces the other refusal, and a function that only reads a
lock-holding record's field has no spelling but a dummy write or a lend to C through `@`, which
counts as a write (`resolve/written_nowhere.hero` line 7). The kit's key escapes only because its
helper calls `counter_value(@c)`. This is panel 210's rule meeting panel 215's R2, and F0 keeps it:
F0's compiler cost is zero lines, and what it leaves is this loop, which a landing of F0 would have
to break in some other way (R2's note rewritten to stop recommending `c.n` through `@c` at least).

And `heroes check` sees only half of the loop (measured 22:39 with `heroes-r2`): `check f0_removed.hero`,
the by-value copy, **exits 0**, since R2 is told at `build` alone (`cli/rounds.hero:268` calls
`lock_copies.copies` once clang has answered `cli/locks.hero`); `check f0_note.hero`, the honest
`@c` that only reads, is refused `never_rebound`. A reader or a tool iterating on `check` is steered
toward the copy.

## F2 and F3, from the code (unbuilt: F1 did not turn out unsound once the nested shape was refused)

**F2, no word: every by-value parameter handed by its place.** In Heroes a parameter without `@`
is never written (`resolve/writes.hero` refuses it, `not_mutable`), so *a by-value parameter the
function never writes* is **every** by-value parameter, and F2 is a change of calling convention
for all of them. Its lowering is F1's two edits with the predicate widened (`ir/lower.hero`'s slot,
`ir/flatten.hero`'s argument), so the back end is again zero; what it costs is elsewhere, and three
things are measured or read: (1) **the aliasing sweep F1 needed would apply to every call that hands
a place by value.** `f(a: @p, b: p)` prints `1` today (critic's probe; mine, `w5_byvalue`, `1` and
`2`), and `both(p, b: bump(@p))` prints `1` (`nested_value`); under F2 both would print `2` unless
refused, so F2 either changes what legal programs print or refuses them, and how many programs of
this tree that is is a census nobody has run (*unrun*). The sweep I built scans every `@` argument
of the program for each call that lends in place; the compiler's own source has about 6,950 `@`
arguments and 64,047 call openings (`grep -oE` counts outside comments, not the parser's), so under
F2 that scan is quadratic on the compiler itself and needs an index first. (2) **Function values.**
F1 refuses a function with a lent parameter as a value (`check/markers.hero`, +17), since no function
type carries the mode; F2 cannot refuse every function with a by-value parameter, so every function
value would have to pass places too, and an `extern` named as a value, which builds today
(`<scratchpad>/217-ce-probes/extern_value.hero`: `f = strlen` was accepted and only its `.cstr()`
refused), would pass a record by value to C behind the same type: a thunk per such `extern`, or a
refusal, unbuilt. (3) **It does not reach C.** An `extern`'s `c: Counter` with no `@` is still a
struct by value against `const struct counter *`, and `counter_value(@c)` on a by-value `c` is
`not_mutable` (critic's `byval_at_to_c`), so task C's helper still copies to lend to C unless F1's
C half comes with it. What F2 leaves: no surface, and a call that reads the same everywhere but
whose meaning depends on refusals the reader cannot see in the signature.

**F3, `@` admitted on a parameter only lent onward**, is the language at `f57af53da` (the critic's
finding, `resolve/written_nowhere.hero` line 7: an `@` argument counts as a write), so it costs zero
and is F0. **The form the critic named U5, `@` admitted on a parameter only read**, is panel 210's
`never_rebound` relaxed: in `resolve/written_nowhere.hero`'s `report` a few lines for all types,
which reverses a rule ratified 2026-10-11; for lock-holding types only it cannot live where the rule
lives, because the resolver has no types and lock-ness is known at `build` after clang
(`cli/rounds.hero:268`), so `never_rebound` on an `@` record parameter would move to the build's
rounds and `check` would stop telling it for every group record. What it leaves: `doubled(@c)` and
every call `doubled(@s.first)` say the counter changes, which nine of ten notes read as false (the
critic's reading), and the loop above becomes one fix instead of two.

## A hole in my own prototype at the C boundary, closed in `heroes-f1d` (22:45)

**Found by probing it, not by reading**: `heroes-f1c` built `<scratchpad>/217-ce-probes/w11_writable.hero`,
an immutable binding `s = Counter(n: 7, rest: zero)` lent with no `@` to `counter_init(c: Counter
lent)`, whose header takes a writable `struct counter *` and sets `n` to 0: exit 0, and the binary
printed `0`. A `=` binding changed by C, through a parameter declared read-only. The cause is the
erasure that makes the back end free: the pointee check asks `cli/pointee_wants.hero`'s `read_only`
of the IR slot, and the slot of a read-only lend is the `@` parameter's, so the `const` question
`emit/ffi_read_only.hero` asks of every non-`@` `cstr` (defect 411) was never asked.

`<scratchpad>/217-compiler-engineer/heroes-f1d` (sha256 `2342aee9146a…`) asks it: a new ask kind,
`pointee_ask.PLACE`, for a group's record declared `lent` with no `@`, read from the declaration
(`cli/pointee_wants.hero` `lent_record`), answered by the `const` assertion alone (no byte question,
no cast: `emit/ffi_read_only.hero` `const_assertion`), and refused as `ffi_writable_parameter` with
words for a record: *`c` of `counter_init` is lent read-only as a `Counter`, and the header says
`struct counter *` — C does not promise to leave it alone*, fix (guess) *take it `@c`*. Its first
build wrongly asked the `cstr`'s byte question too and refused the key (`ffi_parameter_type`); the
separate ask kind is the repair. A header taking the struct by value, `counter_peek(struct counter
c)` (`w12_byval_header`), is refused by the existing probe call, `ffi_parameter_type`.

Under `heroes-f1d` all eighteen probes answer as in the table above plus `w11_writable` refused, the
key prints `6`, `4`, `5`, and `ok1_shapes` prints what `heroes-f1` printed (`cmp`). **This is the
compiler the F1 arm should be judged with if the coordinator has not started**; `heroes-f1` accepts
`w11` and `nested_lent`, which a reader's program reaches only by lending a record to a writable C
pointer with no `@`, or writing `@x` inside a call that lends `x`.

**The `emission` form's one failure is the lane's, not F1's**: `emission/run/ffi-a-record-holding-a-lock-is-lent-in-place`
says *nothing blessed at `tests/emission/run-ffi-a-record-holding-a-lock-is-lent-in-place.c`*; that
case arrived at `f57af53da` itself (`git log -1` on it prints the step-7 commit) and `tests/emission/`
there holds no file for it (`ls | grep -c` reads 0), so it fails with any compiler. `emission` takes
no case filter, so the same run with `heroes-r2` is *unrun*.

**The cost table, recounted at `heroes-f1d` (22:45, same replica, same trees)**: **+486** over 20
files. Added to the table above: `cli/pointee_wants.hero` 169 → 188 (+19), `cli/pointee_ask.hero`
57 → 62 (+5), `cli/header_types.hero` 300 → 300 (+0, one condition lengthened), and
`emit/ffi_read_only.hero` 247 → 276 (+29). So *emitter 0* above is true of the code the program
runs (`emit/signature.hero`, `places.hero`, `lend_local.hero` untouched) and **false of `emit/` as a
directory** from `heroes-f1d` on: the C boundary's `const` question, asked of the header in the
pointee check's unit, is 29 lines there and 24 in `cli/`. By pass at `heroes-f1d`: checker 249,
predicate module 131, the pointee check 53, resolver 26, IR 25, parser 2. No `DECIDED` row moves for
these files (`header_types.hero` stays at 300).

## What R2's message says under F1, on the readers' own programs (22:46)

The ten programs of panel 215's re-run (copied from the coordinator's `readers-r2/r*/c.hero` into
`<scratchpad>/217-ce-probes/readers/`, each beside its `counter.h`), built with `heroes-f1d`: the same
counts as under R2, 4, 7, 4, 4, 5, 4, 4, 4, 4, 4 `ffi_lock_copied`, now with the parameter's fixes
in this order: *read it in place, the caller's own cell and no copy: `c: Counter lent`* (guess),
then *or, where the function changes it, take it by place and lend it at each call with `@`*
(guess), and the note *lend the place instead: a function taking `c: T lent` reads the caller's own
cell and changes nothing, `f(c)`; one taking `@c` may change it, `f(@c)`; a field is read in place,
`c.n`*. **The first fix applied by `sed` to each program** (`function doubled(c: Counter)` →
`… lent)`, one line changed in each) clears the parameter and both call sites everywhere: what
remains is each program's own local copy, one refusal in eight programs (`view @= c` in r01) and 4
and 2 in r02 and r05 (`return ok(c)`, `copy @= c`, a construction from `new_counter().must()`; `c @
Counter(…)`, `local @= c`). None builds after that one fix, since each copied the counter to lend it to
C with `@`, and `counter_value(@c)` on a lent `c` is `not_mutable`; the C declaration has to become
`counter_value(c: Counter lent)` too. That is the arm's to measure; it is not a paid run of mine.

## The walk a new surface form owes (`.claude/rules/diagnostics-and-goldens.md` § A new surface form), 22:47

- **The formatter**: no line. `print/signature.hero`'s `parameter_words` already prints `lent` on
  any parameter; `./heroes-f1 fmt key-f1.hero | cmp - key-f1.hero` is silent. The round-trip *test*
  in `print/fmt.hero` is owed, unwritten.
- **`--dump-ast`** keeps `lent` (`./heroes-f1c parse key-f1.hero --dump-ast`, line 14 `function
  doubled(c: Counter lent) -> i64`). **`--dump-ir` prints it as `@`**: `function doubled(@c: Counter)`
  and `call heroes doubled(@s.first)` (`./heroes-f1c build key-f1.hero --dump-ir`). That is §1.7's
  evidence that the form is erased in lowering, and a dump that says `@` where the source says
  `lent`. Carrying the mode in the IR slot (`ir/containers.hero`'s `param_slot` gains a field: 10
  construction sites in 6 files, 16 matches reading fields by name, `grep -rn`) would let
  `ir/print.hero` say `lent`, let `ir/verify.hero` refuse a store through such a slot, and let
  `cli/pointee_wants.hero` read the slot instead of the declaration, where `heroes-f1c`'s hole was:
  about ten lines net, an estimate from the code, **unbuilt**. I recommend it for a landing (§1.12):
  both holes I found in my own prototype were a consumer that could not see the mode.
- **The highlighters**: `editors/vscode/syntaxes/heroes.tmLanguage.json` line 95 paints `lent` only
  in the `extern-member` rule, a line of an `extern` group, so `doubled(c: Counter lent)` would show
  `lent` unpainted; `site/src/lib/highlight.ts` holds no `lent` (`grep -n lent` empty). Both owed,
  unbuilt.
- **`heroes probe`, `heroes mutate`, `heroes measure`**: *unrun*. `lent` is a word token `fmt`
  neither adds nor drops, so the probe's reader should need nothing (a reading, unrun).
- **The diagnostics that quote a program back**: the one fix with a replacement that writes the new
  form, R2's *`c: Counter lent`*, inserts ` lent` at the end of the parameter's type span; it is a
  `guess`, so `check --apply` does not apply it. It could be `certain` where the body never writes
  the parameter (the resolver counts writes), unbuilt.

## Where *read-only* stops: C writing through an address it was allowed to keep (22:48)

`<scratchpad>/217-ce-probes/keptpt_{value,lent}.hero`, a header `pt.h` whose `pt_keep(struct pt *)`
keeps the pointer and `pt_poke(void)` adds 1 through it; the binding `pt_keep(@p: Pt)` carries no
`lent`, which `m-g-at` line 415 reads as *C keeps it*, and that is allowed. `main` lends `@s` to
`pt_keep`, then calls `twice(s)`, which reads `p.x`, calls `pt_poke()`, and reads `p.x` again.
Built with `heroes-f1d`: **by value `twice(p: Pt)` prints `10`; lent `twice(p: Pt lent)` prints `11`**;
both then print `s.x` as `6`. So C changing a Heroes cell through a kept address is the language at
`f57af53da` (`main` sees `6` either way), and F1 carries it into the callee: `lent` promises that
the **Heroes** body neither writes the place nor sees an `@` of the same call write it, not that C
cannot. The Counter version (`kept_lent`, `11`) builds; its by-value twin is R2's refusal. Refusing
it would need to know which places C kept, a flow question this compiler does not ask; so it is a
sentence for the spec-warden (§ 13's `lent` and the new form's paragraph should both say it), not a
rule I can price.

## The forms on my copy (22:50)

- **The compiler's own tests**: `./heroes-f1b test selfhost/main.hero` **1680 tests, all passed**
  (exit 0); `./heroes-f1d test selfhost/main.hero` **1680 tests, all passed** (exit 0). The prototype
  adds no test of its own; a landing owes them.
- **`check`**: `./heroes-f1c run tests/harness/main.hero -- ./heroes-f1c check` and the same with
  `heroes-f1d`: **698 passed, 1 failed** each, the failure `check/ffi-lent-is-read-on-a-lend-and-nowhere-else`,
  whose expected `lent_shape` text (*only those two are ever lent*) I changed on purpose for `n: i64
  lent` and `db: Db lent` (now *not a `cstr`, a `ptr` or a group's record, and only those are lent
  without `@`*); both lines still refuse, only the words moved. A landing rewrites that golden by
  hand.
- **`emission`**: `./heroes-f1c … emission` **1280 passed, 1 failed**, the lane's unblessed case above.
- **`layout`**: `./heroes-f1c … layout` **4 passed, 2 failed**: `parse/` 8,737 against 8,735 and
  `ir/flatten.hero` 1,163 against 1,150, the two rows the cost section names; the untouched tree
  reads 6 and 0 (`heroes-r2` in `<scratchpad>/217-ce-before/`).
- **`run`**: running with `heroes-f1c` at the time of writing.

## A third hole, one F1 opens itself: a lock lent into a generic (22:53), closed in `heroes-f1e`

`<scratchpad>/217-ce-probes/generic_copy_{value,lent}.hero`: `function apply<T>(x: T, f: (function(T)
-> i64)) -> i64` returning `f(x)`, called `apply(s, f: counter_peek)` with `s` a `Counter` and
`counter_peek` an `extern` taking `struct counter` by value (`counter4.h`). By value it is refused,
`ffi_lock_copied` on the argument, by `heroes-r2` and `heroes-f1d` alike. **With `x: T lent`,
`heroes-f1d` builds it at exit 0 and the binary prints `4`: the lock reached C by value.** R2 judges
the generic body where `x` is a `T`, which holds no lock it can see, and F1 took away the outer copy
R2 used to catch. `heroes-f1e` (sha256 `01570a32807f…`) keeps a lock-holding record lent to a
parameter whose type is a bare type parameter what it was by value, a copy (`check/lock_copies.hero`
`to_a_letter`, conservative: it refuses `peek<T>(x: T lent)` given a lock even where the body copies
nothing). `[T] lent` is untouched, since no array of a lock-holding record can be built. Under
`heroes-f1e` all 25 probes answer as intended (`generic_copy_lent` now `ffi_lock_copied`), the key
prints `6`, `4`, `5`, `ok1_shapes` is `cmp`-identical, and the two-module probe prints `p:1`,
`p:2/2`. **`heroes-f1e` supersedes `heroes-f1d` as the compiler I would judge the arm with**; for
task C's programs the two differ only on a lock lent into a generic.

**The cost at `heroes-f1e` (22:53, same replica)**: **+510** over 20 files: `check/` +248 (`lock_copies.hero`
229 → 273, +44 with `to_a_letter`), top-level `selfhost/` +156, `emit/` +29, `resolve/` +26, `ir/` +25,
`cli/` +24, `parse/` +2. Raw: `git diff --stat -- selfhost` 631 insertions, 28 deletions; the patch
is `<scratchpad>/217-ce-probes/f1e-prototype.patch` (sha256 `1d52a86a02d8…`), the earlier one
`f1d-prototype.patch`. The two moved rows are unchanged (`lock_copies.hero` is under 300).

## Answers to the critic's questions, as the prototype stands (22:54)

- **Aliasing**: refused, `aliased_mutable_arguments` (panel 010's code), for every `@` written inside
  the call's text, its own arguments and every call nested in them; places compared with the
  resolver's own steps (`resolve/aliased_places`). Measured shapes above.
- **`s.first.doubled()`**: dispatches to a lent first parameter and lowers the receiver as its place
  (`ok1_shapes`, `pair.second.doubled()`). §4.8 refuses UFCS on `@` because it hides a change;
  nothing changes here, so the prototype lets it through. Refusing it is one line in
  `check/markers.hero`'s `ufcs_hides_the_marker` and a message; the spec-warden's to rule.
- **Which types**: on a Heroes function, any (a record, `str`, `[i64]`, `[T]`, `i64`, a handle, a
  bare `T` with the lock rule above, all built in `ok1_shapes`, `ok2_handle`); on an `extern`, a
  group's record that is no handle (`check/lend_extent.hero`). **A consequence worth a ruling**: with
  the three refusals a lent parameter is observationally a copy taken at the call, so on any type
  that may be copied `lent` changes only whether bytes are copied, which CLAUDE.md §13 does not admit
  as a reason. Refusing `lent` on a type that is not a record would cost about ten lines in
  `check/read_lends.hero`'s `declared` (an estimate, unbuilt); lock-ness itself cannot be the test at
  `check`, since it is known at `build` after clang.
- **Copying out of one**: allowed where the type may be copied (`k = keep(p)` then `p.x @ 99` leaves
  `k` at 1, `ok1_shapes`), refused by R2 where it may not (`w3_return`, `w4_copy`, `w13_box`). The
  brief asked for *refused … copied* outright; I did not build that, because for a copyable type
  §4.10 already makes the copy independent and a refusal there would be a new rule with no program
  it protects.
- **C returning a pointer into what it was lent as `const`**: `lent` on a C parameter already means
  C keeps nothing (`m-g-at` line 415), `@` or not; F1 changes nothing there, and the kept-address
  probe above is the case where the binding said C may keep it.

**Why `run` with `heroes-f1c` speaks for `heroes-f1e` too** (22:56): the three compilers after
`heroes-f1c` differ only on a parameter written `lent` without `@` on a type other than `cstr` or
`ptr`. A Python scan of the extracted `f57af53da` (`<scratchpad>/217-ce-before/`), 1,470 `.hero` files
under `tests/golden/{run,emit,ir,fixedbugs}`, `examples`, `tests/harness` and `selfhost`, finds 363
`lent` parameters (the sanity count) and **none** of that shape in any program; its 11 hits are all
inside `selfhost/` and none is a declared parameter (named arguments such as `declared:
lent.must()`, and `print/signature.hero`'s test string `b: i64 lent`). Two `grep` searches before
it printed 0 for everything, the `cstr lent` lines included, because BSD `grep -E` and `git grep -E`
did not read `\b` here; their zeros were the instrument's and are not cited.

**r01 carried through, by hand, with `heroes-f1e`** (22:56, `<scratchpad>/217-ce-probes/readers/r01/`): from
its first program, three edits (`doubled(c: Counter lent)`, `counter_value(c: Counter lent)` with no
`@` in the C declaration, `view @= c` dropped for `return counter_value(c) * 2`) build and print
`6`, `4`, `5` (`c_fix2.hero`). Keeping the C declaration's `@` and writing `counter_value(@c)`
(`c_fix2b.hero`) is refused `not_mutable` with the prototype's generic sentence, *`c` is lent
read-only … take it `@c` … or bind a copy*, which says nothing of the route that works, the C
declaration's `@` dropped because its header reads through `const`. **A gap in my prototype's
message, for the landing**: `resolve/built_marks.hero` knows the callee where the `@` is read, so
an `@` argument rooted at a lent parameter and handed to an `extern` could say so; about fifteen
lines, unbuilt.
- **`run`** (23:05): `./heroes-f1c run tests/harness/main.hero -- ./heroes-f1c run` **525 passed, 0 failed**,
  two `SKIP` (`fixedbugs-437-…`, `fixedbugs-589-…`, the suite's own platform skips).

## Cold `--emit-c` of the compiler, before and after (23:20)

`<scratchpad>/217-ce-probes/timing.sh`: each run `rm -rf build` then `/usr/bin/time -p <compiler> build
selfhost/main.hero --emit-c`, the three runs interleaved, two rounds; my own side still (no build, no
suite), other sessions not (load 3.3 to 4.6 by `uptime` at each run; Amphetamine held the machine
awake, `pmset -g assertions`). The ratio holds: `real` is about 1.06 × `user + sys` in every run.

| run | round 1: real / user / sys | round 2 | emitted bytes |
|---|---|---|---|
| **before**, `heroes-r2` on `f57af53da`'s source | 87.10 / 67.47 / 14.87 | 86.75 / 67.33 / 14.64 | 52,082,284 |
| **same source**, `heroes-f1e` | 87.90 / 68.46 / 14.76 | 87.96 / 68.73 / 14.57 | 52,082,284 |
| **after**, `heroes-f1e` on its own source | 87.48 / 68.13 / 14.55 | 87.81 / 68.20 / 14.81 | 52,397,259 |

**The C is byte-identical** for the same source (`cmp before.1.c same.1.c` silent; each compiler
deterministic over its two rounds), and so is the IR (`--dump-ir`, `cmp` silent): F1 changes no
generated code for a program that does not use it. **The user time is not identical**: +1.0 and
+1.4 s, about +1.8%, the two rounds' ranges apart. Apportioned: `check` alone, three rounds, 7.07 /
7.34 / 7.13 s user against 7.51 / 7.39 / 7.25 (about +0.2 s); `--dump-ir`, two rounds, 15.81 / 15.50
against 15.68 / 16.35, inside its own noise. The +0.2 s is the checker's sweep, which walked every
expression of a program that lends nothing; the rest I could not place and is not shown to be F1's.
**`heroes-f1f`** (sha256 `fe0128507a63…`) adds the early exit, `read_lend.any_declared`: a program
with no read-only parameter pays one pass over its declarations. Its 26 probes answer exactly as
`heroes-f1e`'s (each probe's codes compared), and its `check` of the compiler is no slower than
`heroes-r2`'s in three rounds out of three (9.68 / 8.22 / 7.97 against 8.73 / 8.09 / 7.74 s user, the
machine busier by then). Its cold `--emit-c` is running at the time of writing.

**The second session, `heroes-r2` against `heroes-f1f`, is noise-bound** (23:27): cold `--emit-c`, two
interleaved rounds, `heroes-r2` 93.89 / 73.74 / 15.82 then 88.33 / 68.75 / 15.01 (real / user /
sys), `heroes-f1f` 92.47 / 72.31 / 15.63 then 103.19 / 79.27 / 18.98, load 3.1 to 4.7, the C again
byte-identical (`cmp`). A round-to-round swing of 5 s in the same compiler, and `sys` up 4 s in the
last run, is contention from other sessions, larger than the 1 to 2% being asked about. **So whether
F1 costs the compiler's own build any user time is unsettled tonight**: the first session read about
+1.8% for `heroes-f1e`, of which about 0.2 s was the checker's sweep, which `heroes-f1f` removes for a
program that declares no read-only parameter; the rest was not placed. What is settled is that the
generated C and the IR are byte-identical. A still machine settles the rest; *unrun*.

## Where it ends: `heroes-f1f`, the prototype I stand behind (23:27)

`<scratchpad>/217-compiler-engineer/heroes-f1f` (sha256 `fe0128507a63…`), patch
`<scratchpad>/217-ce-probes/f1f-prototype.patch` (sha256 `835578efa8ce…`, `git diff --stat`: 20 files,
648 insertions, 28 deletions). **+524 lines in the layout suite's unit**: `check/` +252, top-level
`selfhost/` +166 (`read_lend.hero` and two messages in `data_errors.hero`, the wiring in `checker.hero`),
`emit/` +29 and `cli/` +24 (the `const` question), `resolve/` +26, `ir/` +25, `parse/` +2. Layout with
`heroes-f1f`: 4 passed, 2 failed, the two rows named above and no other. The compilers in that folder,
each superseding the one before: `heroes-f1` (the arm's, if already started), `-f1b` (nested `@`),
`-f1c` (`@` with `lent`, moves), `-f1d` (the `const` question), `-f1e` (the generic lock), `-f1f` (the
early exit). On task C's programs `heroes-f1` and `heroes-f1f` differ only where a program lends a
record to a writable C pointer without `@`, writes `@x` inside a call lending `x`, or lends a lock
into a generic.

## Verdicts (23:27)

### F1, `lent` on a Heroes function's parameter and on a C parameter with no `@`

- `verdict`: **approve**, with three refusals that are part of the form and not options, each a hole
  my own prototype had until a probe found it: every `@` written inside the call's text is held
  against a value it lends in place (`nested_lent`: `1` by value, `2` lent); the C half asks the
  header's pointer `const` (`w11_writable`: a `=` binding changed by C); a lock-holding record lent
  to a bare type parameter stays R2's copy (`generic_copy_lent`: a lock reached C by value, exit 0).
  Recommended for the landing, unbuilt: the IR slot carries the mode, so `ir/verify.hero` can refuse a
  store through it and `--dump-ir` says `lent`.
- `section`: **§1.7** (erased on the way into the IR: `--dump-ir` prints `@c`, generated C
  byte-identical, so sugar by that section's test); **§4.10** (*every value behaves as a copy*: with
  the three refusals a lent parameter is observationally a copy taken at the call, without them it is
  not, measured); **§4.8** (*`@` parameters never alias*, which the aliasing rule extends to a read).
- `implementation_cost`: **+524 lines** in `suite_layout.hero`'s `code_lines` unit over 20 files
  (`check/` 252, `read_lend.hero` and messages 166, the pointee check in `emit/` + `cli/` 53, `resolve/`
  26, `ir/` 25, `parse/` 2); **0** in code generation, ownership and the runtime; **two layout rows
  move**, `BUDGETS selfhost/parse/` 8,735 → 8,737 and `DECIDED selfhost/ir/flatten.hero` 1,150 →
  1,163; cold `--emit-c` of the compiler +1.8% user in one session (0.2 s placed, in the sweep, removed
  in `heroes-f1f`), unsettled in a second, noisier one.
- `needed_for_self_hosting`: **no**. The compiler declares no read-only parameter and lends nothing
  in place; this is a thesis form (task C, and the loop under F0).
- `argument`: F1 is sugar by §1.7's test: it lowers to the `@` parameter's pointer and an
  `.inout_arg`, `--dump-ir` shows `@c`, the C is byte-identical where it is unused, and code
  generation, ownership and the runtime gain nothing. Its cost is rules, +524 lines, half in the
  checker. The strongest case against it is that the erasure is also where it breaks: three holes in
  my own prototype, each a consumer that could not see the mode (a nested `@` printing 2 where a copy
  prints 1, a writable header changing a `=` binding, a lock reaching C through a generic). Closed,
  §4.10 holds. And F0 is not free: its two fixes chase each other.
- `prediction`: landed with the three refusals, F1's diff under `selfhost/` measures **450 to 700
  lines** in `code_lines`' unit (this prototype 524), leaves `emit/signature.hero`, `emit/places.hero`,
  `emit/lend_local.hero`, `ir/own.hero` and `runtime/` untouched, and moves exactly the `parse/`
  budget and the `flatten.hero` row unless `lower_args` and `with_receiver` are merged; checked at
  the commit that lands it in M-buildable-structs (or the milestone that takes it), by
  `heroes run tests/harness/main.hero -- <compiler> layout` and `git diff --stat`.
- `condition`: I object if a landing drops any of the three refusals, or keeps the mode erased in the
  IR with no verifier rule refusing a store through a read-only slot; or if a still machine shows the
  compiler's own cold `--emit-c` more than 2% slower in user time with no `lent` in the program.

### F0, nothing

- `verdict`: **object**. `section`: §4.17 (*the model fixes it in one turn, without opening
  anything*). Cost 0 lines; what it leaves is measured: at R2 a helper that only reads a lock-holding
  record's field is refused by value with the fix *`@c`*, and with `@c` by `never_rebound` with the
  fix *remove the `@`*, and `check` exits 0 on the copy, so the cheap command steers toward it. A
  landing of F0 owes at least R2's note rewritten. `condition`: R2 and `never_rebound` made to agree
  without a form, measured on `f0_note` and `f0_removed`.

### F2, no word, every by-value parameter by its place

- `verdict`: **object**. `section`: §4.10 and §4.8. Cost from the code: F1's lowering widened to every
  by-value parameter, the aliasing sweep on every call (quadratic on the compiler itself without an
  index: about 6,950 `@` arguments, 64,047 call openings by `grep`), function values and `extern`
  values passing records two ways, and a census of programs whose output changes (`f(a: @p, b: p)`
  prints `1` today), *unrun*. And it does not reach C: task C's copy stays. `condition`: that census
  at zero, and a design for function values.

### F3 as listed, and U5

- F3 as listed builds today (`written_nowhere.hero` line 7), so it is F0 and takes F0's verdict.
- U5, `@` admitted on a parameter only read: **object**. `section`: §4.8 (the `@` at the call says
  the argument will change). A few lines in `resolve/written_nowhere.hero` for all types, reversing
  panel 210's R1 of 2026-10-11; for lock-holding types only it cannot live in the resolver, since
  lock-ness is known at `build` after clang (`cli/rounds.hero:268`). It leaves every call
  `doubled(@s.first)` saying a change that does not happen.

The worktree `<scratchpad>/217-compiler-engineer/` is kept, with every compiler and the key, until
the coordinator says the arm has been judged.
