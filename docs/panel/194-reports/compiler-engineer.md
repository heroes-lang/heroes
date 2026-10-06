# Panel 194, compiler-engineer

Started 2026-10-06 15:36:32 (`date`). Copy `<scratchpad>/194-compiler-engineer/`,
made 15:36:40 by `git -C /Users/joseph/Temp/heroes/heroes-lang archive 7a26a0a6 | tar -x`;
`shasum -a 256 seed/heroes.c` = `fc9751a29a1ecb8a...` (equal to the brief);
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: exit 0, `heroes 0.2.0` (15:36:47).

(written as each command runs)

## 0. The tree (15:37, read-only `git -C <repo> merge-base --is-ancestor`)

`41c5d1b4` (094's repair), `c4b27fea` (092's evidence), `f7576a01` (ir12's array
literal in one block), `43e6be50` (the seed) and `e531fac3` (ffi13's prototype base)
are all ancestors of `7a26a0a6`. So Z3 and every emitted-C number below stand on
batch 12's code, not on 178's.

## 1. Layout, the instrument's unit (`w/codelines.awk`, a replica of `suite_layout.hero:817-835`)

`heroes run tests/harness/main.hero -- ./heroes layout` on the base copy:
*layout: 5 passed, 0 failed* (15:37:47 to 15:38:10), so the replica is checked
against green, then per file:

| file | code_lines | ceiling | left |
|---|---|---|---|
| `selfhost/check/walk.hero` | 1858 | 1870 DECIDED | 12 |
| `selfhost/print/fmt.hero` | 1096 | 1175 DECIDED | 79 |
| `selfhost/ast.hero` | 550 | 550 DECIDED | **0** |
| `selfhost/ir/flatten.hero` | 1150 | 1150 DECIDED | **0** (not in the brief) |
| `selfhost/ir.hero` | 313 | 315 DECIDED | 2 |
| `selfhost/grammar_expr.hero` | 1073 | 1085 DECIDED | 12 |
| `selfhost/check/group_fields.hero` | 140 | 300 | 160 |
| `selfhost/emit/layout_sites.hero` | 85 | 300 | 215 |
| `selfhost/emit/ffi_built.hero` | 225 | 300 | 75 |
| `selfhost/data_errors.hero` | 212 | 300 | 88 |
| `selfhost/parse/` (BUDGETS row) | 8641 | 8696 | 55 |

## 2. Defect 091 on this tree (15:39:20 to 15:39:25)

The critic's ten programs (`docs/panel/178-reports/completeness-critic-work/w/x091/*.hero`,
copied to `w/x091/`), `check` then `run` each:
`elem_min` 0/0 `72`; `n_heroes_box` 0/0 `5 9 0 0`; `shapes091` 0/0 `66 68 9 5 70 0`;
`n_lend_elem` 0/134 after `13` (its own `i @ 3` past `Pt[3]`); `oob091` 0/134;
`shapes` 0/134 after `200 40 10` (its own `k @ 3` past `u8[3]`), each 134 the
read's own *panic: index out of range for a fixed array*; refused by rules of their
own: `n_index_types` `type_mismatch`, `n_loopvar` `unused_binding`, `n_map`
`no_such_field`, `n_nested` `ffi_field_type`. No output holds *unreachable*.
**091 stays closed on 7a26a0a6**, which carries ir12's rewrite of
`emit/container.hero` that the 11:37 rerun on `bef739dd` did not.

## 3. R1 built on panel 186's `build`-side judgement (15:40 to 15:57)

**The design decision, and why.** Today `rest: zero` already parses: it is an
ordinary labelled argument whose value is the name `zero`, and `check` stops on
`unknown_name: zero` (`heroes check` on 178's `kinds.hero`, 15:45). Three places
could hold the words, each counted before building:

| where | what it forces | how it fails when a path forgets it |
|---|---|---|
| a new case of `resolved.Ref` | 36 match lines in 18 files (`grep` over exhaustive `Ref` matches) | n/a, too wide |
| left as an argument, recognised by the resolver | resolver, checker, lowering each skip it | **silently**: `check/lower.hero:82` makes an unresolved name the error type with no diagnostic |
| the parser takes the words off the list onto the call node (178's shape) | `ast.hero` +3, one `.call` and one `.method` construction (`grammar_expr.hero:310`, `:346`), the re-printers | **loudly**: a stripped construction that nothing else handles is one leaving fields out, which `missing_fields` (Heroes record, `walk.hero:1986`) or 186's `OMITTED` (group record, `ffi_built.hero:160`) already refuse |

Built the third, keeping the stripped `Arg` whole on the node (`rest: Arg?`) so
every printer appends it and prints it through the ordinary argument path,
comments included. On 186's base the checker needs **no change to
`construct_record`**: a group record built naming some fields already takes
`built_from_some` (`walk.hero:1776`), and C already zero-fills a designated
compound literal (`emit/aggregate.hero:146`). What R1 adds is:

- parser: `parse/rest_words.hero` (new) takes the words off the end of a call's
  or method's list; anywhere but last, `rest_not_last`; a variant case keeps them
  as an argument (`.c(rest: zero)` names a field `rest` of a Heroes variant);
- checker: `check/rest_zero.hero` (new), one flat pass from `checker.hero` beside
  `ffi_sweep`: `rest_outside_a_group` (Heroes record), `rest_is_a_field` (a group
  record with a field `rest`, the golden `fixedbugs-156` has one),
  `rest_not_a_construction` (function, UFCS, a value called, a call's result);
- IR: `record_shape` gains `rest: bool` (`ir.hero`), set at the two lowering
  sites by rewriting existing lines (`ir/flatten.hero` stays 1150);
- emitter: `extern_union.Site` carries it; `layout_sites.site_lines` writes no
  `OMITTED` assertion for it (the `SHARED` pair check is untouched);
  `aggregate.construct` writes `(T){0}` for a construction naming no field, where
  it refused before (C11 has no `(T){}`).

**Two holes my own probes found in my first build, both closed before any
number below**: UFCS `1.add(b: 2, rest: zero)` and a call of a value
`pick()(4, rest: zero)` resolved their callee `.unresolved`, which my first pass
treated as already told, so the words were dropped unsaid (fail-safe only by
luck of arity). Now every callee but a group record is told
(`w/r1/callee_kinds.hero`: four `rest_not_a_construction`, exit 1).

### Probes, `heroes-r1b` (built 15:56:15)

| program | check | run | output |
|---|---|---|---|
| `w/r1/uname_rest.hero`, Darwin `utsname`, five `i8[256]` declared, `Utsname(rest: zero)` | 0 | 0 | `Darwin` |
| `w/r1/kinds.hero` (178's: i8, f64, bool, ptr, cstr, nested record, `Pt[3]`, `f32[2]`, handle, i64) | 0 | 0 | every left-out field zero, named ones kept |
| `w/r1/own.hero`, a Heroes record | 1 | | `missing_fields` + `rest_outside_a_group` |
| `w/r1/restfield.hero`, a group record with a field `rest` | 1 | | `rest_is_a_field` |
| `w/r1/notlast.hero` | 1 | | `rest_not_last` (parser) |
| `w/r1/order.hero`, labels out of order, unlabelled, unknown, repeated | 1 | | 186's `wrong_label` / `missing_label`, unchanged |
| `w/r1/fn.hero`, `w/r1/callee_kinds.hero` | 1 | | `rest_not_a_construction` at each of the words |
| `w/r1/case.hero`, `.circle(rest: zero)` | 1 | | `unknown_name: zero`: refused, but the message does not name the words (a variant case keeps them as an argument) |

### What `rest: zero` does to a union (the critic's question; 15:57 to 16:00)

C probe `w/union/u.c` with a positive control (a struct built by member stores
on a stack dirtied with 0xAA): control **5 to 15 of 20 bytes non-zero** at every
level on every leg; `(union){0}` where the first member (`char`) is smaller than
the largest (`double`), a struct whose union is left out, and one whose union's
small member is named: **0 non-zero bytes**, at `-O0 -O1 -O2 -O3`, on Darwin arm64
(Apple clang 21.0.0), Linux arm64 and Linux x86-64 (Debian clang 22.1.8, the
`heroes-linux-arm64` and `heroes-linux` images). This is clang's behaviour on
three legs, **not** a C11 promise I verified: whether 6.7.9p10's *any padding is
initialized to zero bits* covers a union's bytes past its first member is the
historian's to cite.

Through the pipeline (`w/union/`, header `un.h`: `SA` a struct with an
anonymous `union { char c; double d; }`, `UD` a union type), each construction in
a function called right after a C function dirties 4 KB of stack:

| program | check | run | prints |
|---|---|---|---|
| `none.hero`: `SA(kind: 1, rest: zero)`, `UD(rest: zero)` | 0 | 0 (also `build` at `-O0`) | `1 0.0 0 0.0`: the union's `d` zero past the `char` |
| `one.hero`: `SA(kind: 1, d: 2.5, rest: zero)`, `UD(d: 1.5, rest: zero)` | 0 | 0 | `1 2.5 0 1.5` |
| `two.hero`: two members of one union named, with the words | 0 | 1 | `ffi_union_field`: 186's `SHARED` stands |
| `ud_two.hero`: a union type from two members, with the words | 0 | 1 | `ffi_union_field` |
| `norest.hero`: `SA(kind: 1, d: 2.5)`, no words | 0 | 1 | `missing_fields` for `x`, 186's refusal unchanged |

**So R1's union rule, as built**: with `rest: zero` a construction names **at
most one** member of each union, where 186 says exactly one; a union none of
whose members is named is all zero bytes (clang, three legs); two named is still
refused at `build`. The sentence R1 adds to § 13 has to say that, which 178's
did not.

Found beside, pre-existing and not R1's: when one program holds both a struct's
union refusal and a union type's, `build` tells only the first
(`heroes run w/union/two_base.hero` with the seed's compiler: 2 errors, both on
`SA`, none on `UD`; `UD` alone is told). Not filed by this seat.

### The re-printers (16:00 to 16:02)

`print/fmt.hero` is **not touched**: a call's words are printed by
`print/parens.hero` (one line), `print/elements.hero` (broken) and
`print/bodies.hero` (`--dump-ast`), each through `bodies.written(args, rest)`,
which appends the kept `Arg`. `heroes-r1b fmt --in-place` twice on 178's
`fmt_r1_orig.hero` and on `w/r1/fmt_comments.hero` (a comment on the line above
the words and one after them, and `Kinds(  tag:3 ,rest:  zero)`): unchanged /
normalised to `Kinds(tag: 3, rest: zero)`, second pass byte-identical, both
comments kept where written, the program runs. `parse --dump-ast` prints
`Utsname(rest: zero)`; `--dump-ir` prints `construct Utsname{rest: zero}()`
(`ir/questions.named_list`, `ir/print.hero` rewritten in place, 469 of 470).
**Unrun**: `heroes mutate` (it re-prints through `fmt`, an inference), `heroes
probe`'s reader, the two highlighters (`rest: zero` colours as a label and a
name today; whether `zero` should colour as a word is the landing's).

## 4. `missing_fields` on a Heroes record: the case (16:00)

`tests/golden/check/a-heroes-record-is-built-with-every-field.hero`, annotated
`#~ missing_fields` on `Point(x: 1)` and on `.circle(r: 2)`; its `.expected`
written by hand from `data_errors.hero:57`'s template, not from the compiler's
output. `heroes run tests/harness/main.hero -- ./heroes check` (the seed's
compiler): **check: 551 passed, 0 failed** (16:00:44 to 16:00:57), and with the
`.expected` set aside the suite refuses to run (*missing its .expected file*),
so the case is counted. Under `heroes-r1b` the same suite reads 551/0 (the net
below). **R1 does not move it**: a Heroes record ending with `rest: zero` is
told `rest_outside_a_group` *beside* `missing_fields` (`w/r1/own.hero`), so the
golden that pins `missing_fields` holds under R1 unchanged. It is owed whether R1
lands or not: today the net would stay green if `missing_fields` vanished from a
Heroes record (178's finding 2, re-run: `grep -rl missing_fields tests/golden/check`
was empty before this file).

## 5. Z3: defect 094 on this tree (16:09)

`w/z3/`, the seed's compiler at 7a26a0a6 (which carries `41c5d1b4`):
178's `cinit.hero` (`#define PT_INIT {1, 2}`) **run 0, prints `2`**; Darwin's
`PTHREAD_MUTEX_INITIALIZER` bound as a group constant, then `pthread_mutex_lock`:
**run 0, prints `0`**. The limit holds as the lane wrote it:
`PTHREAD_COND_INITIALIZER` bound as the mutex's constant builds, and `lock`
returns **22** at exit 0; `__sig` reads `850045863` for the mutex's and
`1018212795` for the condition's (`w/z3/mutex_print_sig.hero`). A wrong value
for the library, not memory: Z2's class (bytes, not validity). **Z3 stands on
the lowering**.

## 6. Defect 092's routes (16:09 to 16:16)

Read: `docs/panel/194-evidence/092-routes.md` and `092/prototype.diff`
(308 lines; per file `check/lend_extent` +11/-1, `check/lend_types` +15/-1,
`cli/header_types` +6, `cli/pointee_wants` +30, `emit/ffi` +3,
`emit/ffi_pointee` +26, `emit/field_lend` +10/-1, `emit/lend_extent` +37/-3,
counted by `awk` over the hunks).

**What I found beside them, which changes how they read.** Every route the lane
built compares the count **in bytes**, and its own table shows that passing
`poll`'s record count (`a-typed-pointer-record-count`, `nfds: 2` against an
8-byte record, exit 0, ASan *WRITE of size 2*). **The same unit is already
shipped in panel 166's field lend**, outside 092 entirely:
`w/unit/wide.hero`, `fill_ints(a: h.buf.ptr(), n: 16)` declared
`a: ptr counted_by n lent` over a header `void fill_ints(int *a, unsigned long n)`,
the field `u8[16]`: **`check` 0, `build` 0** (the `_Static_assert` reads 16 <= 16),
**`run` 0 printing `4702111234474983745`** (`0x4141414141414141`, the next
field overwritten), `build --sanitize` then run: **134, *stack-buffer-overflow,
WRITE of size 4***. Panel 166's llm-ergonomist listed it as an unrun guess
(`docs/panel/166-reports/llm-ergonomist.md:314`, *"that the extent is bytes, not
elements ... 4x wrong for `i32[N]`"*); `grep` over `issues/` finds no item for
it. **A design.md §1.12 defect on today's trunk, unfiled; it is the
coordinator's to number.**

**Built, as the cheap route: the unit, asked of the header** (`u/selfhost`, a
copy of the base tree; `u/heroes-u` built 16:15:45). Every `counted_by`
parameter becomes one more ask of panel 103's pointee check, and the check unit
asserts `__builtin_types_compatible_p(T, void) || sizeof(T) == 1` on the
header's own pointee text; a failure is `ffi_parameter_type` on the declaration.
**+65/-7 in 5 files** (`cli/pointee_ask` +6, `cli/pointee_wants` +15,
`cli/header_types` +9, `emit/ffi` +4, `emit/ffi_pointee` +31/-7, the last sharing
one marker reader with the void guard to stay at 300 of 300).

| program | base (`bm/heroes-base`) | unit route |
|---|---|---|
| `w/unit/wide.hero`, `int *` | run 0, corrupt value | **build 1**, `ffi_parameter_type` at `a`, *which C counts in units wider than a byte* |
| `w/unit/narrow.hero`, `char *` and `void *` | run 0, `7 65` | run 0, `7 65` |
| the 17 tracked `.hero` files declaring `counted_by` (`grep -rl` under `tests/golden`, `examples`) | 3 build, 14 refused | **17 of 17 the same exit and stderr** |

Its limit, said plainly: it **refuses** a correct `counted_by` against a wider
pointee (`fill_ints` with `n: 4`), where a unit-aware compare would accept it.
The complete route is the compare in the pointee's unit, `n * sizeof(T) <=
sizeof(lent)`, which needs the header's pointee text in the main unit; today
that text exists only in the probe unit (`cli/pointee.hero`'s dump), so it is
plumbing from `cli/` into `emit/`, unbuilt. The note it prints is
`unsized`'s and tells the author to declare `a: ptr`, which it already is: a
landing writes its own.

## 7. R1 on 178's `check`-side route, for comparison (16:03 to 16:07)

Built as a variant of my tree (`cs/selfhost`, compiler `cs/heroes-r1c`) that
differs only in the back half: 178's `ir/zeros.hero` ported (each left-out
field lowered to a zero of its own type, the construction then naming every
field), and `emit/storageless.hero` writing `{0}` for an empty `T[N]`.

| | `build`-side (186's judgement), built | `check`-side (178's zero-fill), built |
|---|---|---|
| lines over the base tree | **+236/-24** in 17 files (+63/-63 more for the `ast.hero` seam move) | the same front end, **+122/-5 more** (`ir/zeros.hero` +103, `ir/flatten.hero` +15/-5, `emit/storageless.hero` +4), less the ~10 build-side-only lines it would drop |
| layout unit, net | **+188** (+1 for the move) | about **+280** |
| DECIDED rows | **none moved**: `ast` 513/550 after the move, `walk` 1858/1870 untouched, `fmt` 1096/1175 untouched, `flatten` 1150/1150 (lines rewritten, none added), `ir.hero` 315/315, `grammar_expr` 1082/1085; `parse/` budget 8695/8696 | `ir/flatten.hero` **1158 against 1150**: a row moves |
| `utsname`, every field declared, `Utsname(rest: zero)` | run 0, `Darwin`; C `t1 = (struct utsname){0};` | run 0, `Darwin` |
| 178's `kinds.hero` | run 0, all zero | run 0, all zero |
| `SA(kind: 1, rest: zero)`, a union none of whose members is named | **run 0**, `1 0.0 0 0.0` | **run 1, `ffi_union_field`, naming `c` and `d`, fields the program never wrote** |
| `SA(kind: 1, d: 2.5, rest: zero)`, one member named | **run 0**, `1 2.5 0 1.5` | **run 1**, the same false refusal |
| two members named | run 1, `ffi_union_field` | run 1 |

**So the `build`-side route costs less and judges more.** The `check`-side
route zero-fills every member of a union, which 186's own `SHARED` check then
refuses, so on today's tree it cannot build any group record holding a union
with `rest: zero`, and says so by quoting fields the author never wrote. That
is not a matter of taste: 178's prototype predates 186 by eight days, and the
two do not compose.

## 8. 178's other points, each with the command that decides it (16:17 to 16:20)

- **R0** (`T.zero()`): `Utsname(rest: zero)` naming no field is the whole-record
  zero, `(struct utsname){0}` (§3). A second spelling: not built; refused as 178 refused it.
- **Z1** (a `zero` claim on the record): its premise re-run,
  `w/pts/mutex_partial.hero` (178's), `partial` + `Mutex(__sig: 0)` on Darwin:
  **run 0, prints `22`**. The state Z1 claims to guard is reachable with no claim.
- **Z2** (zero admitted on every group record, as bytes): what R1 builds, with the
  union rule of §3.
- **T** (text into a field): today's route re-run, the critic's
  `w/pts/sun_c_darwin.hero`, `strlcpy(dst: a.sun_path.ptr(), ...)` declared
  `ptr counted_by size lent`: **run 0, `0 0 0`**; an overstated literal is
  `field_lend_extent` at build, an overstated run-time extent **134** before C
  runs. `strlcpy`'s `dst` is `char *`, one byte, so the unit route of §6 leaves it
  untouched.
- **A** (`[x; N]`): not rebuilt; 178's diff added a `;` token and moved one
  golden. With R1 its own case is a non-zero repetition, which nobody measured.
- **R2** (omitted fields zero with no mark): on today's tree it is the deletion
  of one assertion, `layout_sites.site_lines`' `OMITTED` line: one line, and it
  removes the only check that a construction of a group record named what it meant.
- **D** (a zero for every type): its premise re-run against today's runtime,
  `w/pts/zstr.c`, an all-zero `HeroStr` through `hero_str_len`: **134, *read of
  an unassigned str slot***.
- **§ 13 says C's `char` is `i8` (point 5)**: `w/pts/u8char.hero` under
  `heroes-r1b`: `ffi_field_type`, note *every field is at the header's own width
  and sign: correct `sysname`*, which does **not** name `i8`
  (`selfhost/emit/ffi_field.hero:55`).
- **Padding not promised (point 9)**: not re-run by this seat (MSan on x86-64 is
  the ffi-pragmatist's and the critic's).

## 9. A shape beside R1 that my build got wrong, and one beside it that ships (16:20 to 16:36)

A **handle** (a tagged, non-`partial` group record with no fields,
`handles.hero:51`): `w/r1/handle_rest.hero`, `Opaque(rest: zero)`, under my first
build ran **0 and printed `true`**: my `(T){0}` in `aggregate.construct` wrote
`(struct opaque *){0}`, a null handle, and it did the same for `Opaque()`
without the words. Repaired in `fx/selfhost` (compiler `fx/heroes-r1b2`, built
16:35:58): `check/rest_zero.hero` tells a handle `rest_not_a_construction`
(**check 1**), and `aggregate.construct` keeps refusing an empty construction of
a record with no fields. Cost +8/-2 (`rest_zero` 94 to 100, `aggregate` 296 to 299).

**Found beside, on today's trunk**: `w/r1/handle_empty.hero`, `Opaque()`, a handle
built with no arguments, is **`check` 0 and `run` 134, *entered unreachable code
— this is a compiler bug***, on the base compiler (`bm/heroes-base`) and still
under the repaired R1 (which leaves it as it is). A handle is never built
(spec § 13); the refusal belongs in `check`. `grep` over `issues/` for a handle
construction found no item; **unfiled, the coordinator's to number**.

## 10. The route nobody listed: the `ffi_field_type` note names `i8` (16:21 to 16:23)

Built in its own copy (`k/selfhost`, compiler `k/heroes-k`): where the declared
type begins `u8` and clang refuses the field, a second note, *where the header
writes `char`, the field is `i8`: C's `char` is signed in every build, which
passes `-fsigned-char`*, and a **`guess`** fix replacing `u8` with `i8`.
**+13/-1, one file** (`emit/ffi_field.hero` 267 to 278), **0 spec tokens**.
`w/pts/u8char_base.hero` (Darwin `utsname`, `u8[256]`): run 1 with the note and
`fix (guess): declare \`sysname\` with \`i8\``; the fix applied by hand
(`u8char_fixed.hero`): **run 0, prints `68`**. `guess`, not `certain`: the
header's type could be another one-byte-signed spelling or a wider one, which
`_Generic`'s failure does not say.

## 11. The cost to the compiler, in instructions retired, and what it taught (16:36 to 17:06)

Method: `/usr/bin/time -l <compiler> build selfhost/main.hero --emit-c -o main.c`,
each run in a fresh directory holding only `selfhost/` and `runtime/` (so the
`build/` cache is **cold for every run**; 447 cache entries after each), and the
instructions count the children (clang's probe and check units) too. The C
emitted for the same input is byte-identical across compilers (`cmp`), so the
children's share is equal and a delta is the compiler's own. Then `check`
alone, which runs no clang, to isolate the front end.

**First R1 build (the stripped `Arg` kept on the node as `rest: Arg?`)**, cold
`--emit-c`, three runs each, billions:

| | run 1 | run 2 | run 3 |
|---|---|---|---|
| base compiler, base tree | 1,091.30 | 1,091.51 | 1,091.42 |
| R1 compiler, the same base tree | 1,107.90 | 1,107.85 | 1,109.25 (**+1.5%**) |
| R1 compiler, its own tree | 1,113.72 | 1,114.01 | 1,113.99 |

`check selfhost/main.hero`, twice each: base **62.25** billion, 173 MB resident;
R1 **63.49** (+2.0%), **214 MB (+24%)**. The cause, read from the emitted C:
`HeroFailure` is 32 bytes, so every `T?` is at least 40; today's largest
`ExprKind` payload is `if_expr` (8 + `Block?` 40 = 48), an `Expr` 72 bytes. An
`Arg` holds its own `Span?` (56), an `Arg?` is 64, so `method` became 96 and
**every expression in the arena grew from 72 to 120 bytes**. Panel 023's own
comment on `HeroFailure` (`runtime/heroes_runtime.h:366`) is the rule that says so.

**So R1 as built stores the words in a side list**: `.call` and `.method` carry
`rest: i64`, an index into `Ast.rests: [Arg]` or -1 (method's payload 40, under
48: no node grows). Then, each measured on `check` (base about 62.26 billion):

| step | `check` instructions vs base | resident |
|---|---|---|
| side list | +0.61 billion (+0.97%) | equal to base |
| `split` compares lengths before slicing | no change | equal |
| `split` hands an unchanged list back | -0.04 billion | equal |
| attribution: the checker's pass not run | the pass was **0.22 billion** | |
| attribution: base + only the `Ast.rests` field | **+0.03 billion** (0.05%) | |
| attribution: `split` bypassed | **equal to base** | |
| the checker's pass returns at once when `Ast.rests` is empty; `split`'s test a `bool`, no `Span?` built per argument | **+0.19 billion (+0.31%)** (62.47 / 62.46 against 62.27 / 62.27) | **equal** (172.8 MB) |

What remains is `split`'s one extra walk over every call's arguments. The
route to zero is the test inside `grammar_expr.call_args`, where each argument
is built anyway; it needs lines in a file at 1082 of 1085, **unbuilt**.

**Found beside, on today's trunk, while building the attribution**: a named
function with an `@` parameter taken as a value, `w/fnref/inout_value.hero`
(`_ = bump` where `function bump(@n: i64)`), is **`check` 0 and `run` 2,
*internal error: compiling the generated C failed*** (*incompatible function
pointer types assigning to `void (*)(long long)` from `void (int64_t *)`*), and
the same when the value is called (`inout_called.hero`). No item found in
`issues/` by `grep`; **unfiled, the coordinator's to number**.

## 12. R1 as finally built (`sl/selfhost`, compiler `sl/heroes-sl5`, 17:05:50)

The first build plus: the handle refusal (§9), the words in `Ast.rests` behind
an `i64` index (§11), the checker's pass returning at once when `Ast.rests` is
empty, `split` handing an unchanged list back and testing with a `bool`.

- **Lines, R1 proper** (against the base with only the seam move applied,
  `base-moved`): **+255/-25 in 17 files**: `check/rest_zero.hero` +116 (new),
  `parse/rest_words.hero` +62 (new), `ir/questions.hero` +15/-2,
  `grammar_expr.hero` +11/-2, `ast.hero` +9/-1, `print/bodies.hero` +9/-2,
  `emit/aggregate.hero` +8/-3, `emit/extern_union.hero` +5/-2,
  `checker.hero` +4, `ir/flatten.hero` +4/-4, `ir.hero` +3/-1,
  `emit/layout_sites.hero` +2/-1, `print/elements.hero` +2/-2,
  `print/parens.hero` +2/-2, and one line each in `emit/construct.hero`,
  `ir/owning.hero` (tests) and `ir/print.hero`. **With the seam move: +318/-88 in 18 files.**
- **Layout unit**: net **+203** (the move +1). Per file, base to R1: `ast` 550 to
  **517** (DECIDED 550), `grammar_expr` 1073 to **1082** (1085), `ir.hero` 313 to
  **315** (315), `ir/flatten` 1150 to **1150** (1150), `ir/print` 469 (470),
  `emit/extern_union` 297 to **300** (300), `emit/aggregate` 295 to **299** (300),
  `check/rest_zero` **103**, `parse/rest_words` **52**, `expr_kinds` 40 to 82;
  `parse/` budget 8641 to **8693 of 8696**. **No DECIDED row moves**, and
  `check/walk.hero` (1858 of 1870) and `print/fmt.hero` (1096 of 1175) are not
  touched at all.
- **`ast.hero`, what the landing must do**: R1 adds 7 lines there (the two
  indices, their comment, `rests` and its initialiser), so without a cut the row
  reads **557 against 550**. The seam built here: `depth`, `holes_depth` and
  `max_of`, with the test that exercises them, move to `expr_kinds.hero`
  (*what the grammar asks of an expression it has built*, used by the parser, so
  the test still runs). They have **no caller outside their own test**
  (`grep -rn 'ast\.depth\|ast\.holes_depth\|ast\.max_of'`): a walk that questions
  the tree, beside the vocabulary that defines it. 550 to 513 by the move alone.
  The alternative is the row raised to 557 with its reason, the shape `lent`,
  `counted_by` and panels 176/177 took.
- **Can the construction check leave `walk.hero`?** For a group record it
  already has: 186 put the labels in `check/group_fields.hero` and the omission
  in `emit/layout_sites.hero`; R1's own rule is `check/rest_zero.hero`, and R1
  adds **0 lines** to `walk.hero`. `missing_fields` at `walk.hero:1986` now
  serves Heroes records and variant cases only (`check_named_fields`, also
  called for cases at `:1871`); moving it would be a refactor of 186's file, not
  something R1 needs. 178's condition is met by the tree, not by this build.
- **Suites on it**: own tests **1,299, all passed** (16:07 to 16:08, from the
  copy's root); every probe of §3, §9 and the unions as before; `fmt` fixpoint and
  output identical to the first build's; `--dump-ast`, `--dump-ir`, the C as before.
- **`check selfhost/main.hero`**: **+0.31%** instructions (62.47 / 62.46 billion
  against 62.27 / 62.27), resident memory equal (172.8 MB).
- **For one Darwin `utsname`, five `i8[256]` declared** (`w/r1/uname_today.hero`
  under the base compiler, 1,280 literal zeros, against `uname_rest.hero` under
  R1), three cold runs each: `--emit-c` **4.28 billion** instructions and 5,527
  lines of C against **3.56 billion** and 1,681 (-17%); a full `build`
  **3.45 billion** against **2.91 billion** (-15.5%), both binaries printing
  `Darwin`. Source: 4,318 bytes against 433.

## 13. The last cost, and R1 as it stands (17:06 to 17:30)

**§12's build still cost +1.4% to emit the compiler** (cold `--emit-c`, R1 on
the base tree: 1,106.68 / 1,106.64 / 1,106.87 billion against base 1,091.44 /
1,091.59 / 1,091.48), although `check` was only +0.31%: so the cost was behind
the front end. `sizeof` read from each compiler's own emitted C
(`w/sz/probe_*.c`, linked against the runtime): base `Inst` **112**, `OpKind` 48,
`Shape` 24; with `rest: bool` on `record_shape`, `Shape` **32**, `OpKind` **56**,
**every IR instruction 120 bytes** (+7%). (`Expr` 72 in both: the side list held.)

Three homes for that one bit, counted: the `bool` (two readers, every
instruction +8 bytes); a new `Shape` case (no size, but **11 match lines in 9
files**, `emit/gate`, `emit/inst` and `ir/print` among them, DECIDED rows: the
core-shaped cost §1.7 names); or **the checker's own output**. Whether a
construction ended with the words is a fact `check` establishes and only
`build`'s judgement reads; C builds the same designated literal either way.
`state.Checked` already keys a per-call fact by the call's **span end**
(`instantiations`, with the reason written at `check/state.hero:55-86`: two calls
can share a start, `xs.map(f).fold(0, add)`, never an end), and 186's
`extern_union.used` already receives `Checked`. **So R1 as built (`ck/selfhost`,
compiler `ck/heroes-ck`, 17:23:17)** keeps `rested: {i64: bool}` in `Checked`,
written by `check/rest_zero.hero`, read by `extern_union.used`; **every IR file
is the base's**, and `--dump-ir` prints `construct Utsname{}()`: the words are
erased before the IR, §1.7's definition of sugar taken literally.

| | R1 as built (checker-keyed) |
|---|---|
| lines, R1 proper | **+241/-16 in 12 files** (`check/rest_zero` +119 new, `parse/rest_words` +62 new, `grammar_expr` +11/-2, `ast` +9/-1, `print/bodies` +9/-2, `check/state` +8/-1, `emit/aggregate` +8/-3, `emit/extern_union` +5/-2, `checker` +4, `emit/layout_sites` +2/-1, `print/elements` +2/-2, `print/parens` +2/-2); **+304/-79 in 13 files** with the `ast.hero` seam move |
| layout unit | net **+199** (the move +1); `ast` 550 to 517 (550), `grammar_expr` 1073 to 1082 (1085), `check/state` 242 to 249, `emit/extern_union` 297 to **300**, `emit/aggregate` 295 to 299, `check/rest_zero` 105, `parse/rest_words` 52; `parse/` **8693 of 8696**. **No DECIDED row moves**; `walk`, `fmt`, `flatten`, `ir`, `ir/print` untouched |
| `check selfhost/main.hero` | 62.44 / 62.44 billion against 62.28 / 62.28: **+0.26%**, resident equal (172.9 MB) |
| cold `--emit-c` of the compiler, same input | ck 1,091.43 against base 1,091.61 in the same round: **inside base's own spread** (1,091.30 to 1,091.61 over six base runs) |
| cold `--emit-c`, R1 compiling its own tree | 1,096.62 billion (its tree emits 1,275,829 lines of C against 1,272,814) |
| own tests | **1,299, all passed** (17:28:44 to 17:30:13) |
| probes | every one of §3, §9, the unions and `modq` as before |
| `fmt` | fixpoint, output identical to the first build's |
| 1,470 goldens and examples, `check` | **0 move** (exit and diagnostic codes, `w/move-ck.txt` against `w/move-base.txt`) |

## 14. Verdicts, one row per route

`needed_for_self_hosting` is **no** for every row: the compiler's longest fixed
array is `u8[8]` (`grep -rhoE ': [iuf][0-9]+\[[0-9]+\]'` over `selfhost/`), its 8
extern groups hold no tagged record, no whole-record `@` lend and no
`counted_by`, and no `.hero` outside `tests/`, `docs/`, `archive/` declares a
fixed field longer than 8. Whether R1 enters by the thesis route is Principle 0's
question, the spec-warden's to rule.

| route | verdict | design.md | cost (measured here) | prediction | condition |
|---|---|---|---|---|---|
| **R1** `rest: zero`, on 186's `build` judgement, the bit in `Checked` | **approve** | §1.7 and Part 5 (sugar: erased before the IR), §1.1 (fits: no DECIDED row) | +241/-16 in 12 files (+304/-79 with the `ast.hero` move), +199 layout lines; `check` +0.26%, cold `--emit-c` within noise | its landing's `git diff --numstat -- selfhost`, moved code excluded, reads **200 to 330 insertions**, moves **no DECIDED row** but at most `ast.hero`, and cold `heroes build selfhost/main.hero --emit-c` stays **within 0.5%** of its parent's instructions retired | object if the landing puts the bit in the IR (+1.4%, every `Inst` 112 to 120 bytes) or keeps the `Arg?` on the node (+24% resident); veto if built `check`-side (below) |
| **R1 built `check`-side** (178's zero-fill) | **veto** | §1.1 (breaches `ir/flatten.hero`'s DECIDED 1150: 1158); §1.12's tie-break between admissible forms | +122/-5 over R1, about +280 layout lines | landed this way, `ir/flatten.hero`'s row moves and `w/union/none.hero` is refused at `build` with `ffi_union_field` naming fields the program never wrote | lifted if a `check`-side build zero-fills without naming two members of one union and fits `flatten`'s row |
| **R0** `T(rest: zero)` naming nothing | approve as that spelling; object to a second one | §1.7 | 0 lines over R1 (`(struct utsname){0}`) | `Utsname(rest: zero)` emits `(struct utsname){0}` at the landing | a program `T(rest: zero)` cannot express |
| **Z1** a `zero` claim on the record | object | §1.12 (a claim the compiler cannot check), §1.1 | 178's +46/-9 (not rebuilt) | a Z1 build leaves `w/pts/mutex_partial.hero` at run 0 printing `22` on Darwin | the compiler checks the claim, or Z1 also gates `partial` |
| **Z2** zero admitted on every group record, as bytes | approve | §1.7 | 0 lines beyond R1 | with R1 landed, `w/r1/kinds.hero` prints every left-out field zero on the three legs (Linux unrun by this seat) | a group record whose all-zero bytes crash where an explicit `0` would not |
| **Z3** the header's initialiser as a group constant | approve; it stands | §1.12 | 0 lines owed: 094 repaired at `41c5d1b4`, in this tree | at M-buildable-structs' close `PTHREAD_COND_INITIALIZER` bound as the mutex's constant still builds (C cannot say which struct a brace list was for) | a check that tells a brace list's struct |
| **L** 091 as a lowering | approve; closed | §1.12 | 0 lines owed (097, `d64da8ff`) | the critic's ten `x091` programs show no *unreachable* at the close | none |
| **T** text into a fixed field | object (wait) | §1.7, Part 5 | 0 lines: `strlcpy` through a counted field lend runs today | the unit refusal of §6 moves 0 of the tracked `char *`/`void *` text routes | a reader test where the counted lend fails first try half the time |
| **A** `[x; N]` | object (wait) | §1.7 (a second route into Part 5's construct 3) | not rebuilt; a `;` token | if A lands, `tests/golden/check/unterminated.hero`'s diagnostic for `f = 2;` changes | a measured non-zero repetition need |
| **R2** omitted fields zero, no mark | object | §4.9 (*no default values*) | one deleted assertion (`layout_sites`' `OMITTED`) | an R2 landing turns red, or deletes, the 18 `unsupported` goldens that pin `missing_fields` on a group record | a measured thesis effect and the guard kept elsewhere |
| **D** a zero for every type | **veto** | Part 5 (a fifth per-type descriptor), §4.9, §1.12 | not built; an all-zero `HeroStr` is `hero_str_len` 134 today (`w/pts/zstr.c`) | any D prototype aborts 134 on its zero `str` | D restricted to types whose zero bytes are a runtime value, which is Z2 |
| **nothing** | object | §1.1 (the ceiling is not what decides: R1 fits) | 0 compiler lines; one Darwin `utsname`: 4,318 source bytes, 5,527 C lines, 4.28 billion instructions to emit against R1's 433, 1,681, 3.56 | at M-buildable-structs' close, without R1, that program still emits 5,527 lines of C (+-2%) | Principle 0 rules R1 out |
| **092 A**: `counted_by` on a whole `@` record, an undeclared `void *` lend refused | object as built | §1.12 (a check in the wrong unit passes the overflow it claims to check; `getsockopt` into a record becomes unbindable) | about +113 lines (lane's hunks) | landed without a unit, `092/a-typed-pointer-record-count.hero` runs 0 with ASan *WRITE of size 2* | approve with the unit (C or U) and the count through a cell admitted |
| **092 B**: `x.ptr()` lends a whole record | object as built; approve with the unit, paired with A's `void *` refusal | §1.12 | about +27 lines (lane's hunks) | `b-typed-pointer-record-count` stays 0 silent until the unit lands | the unit |
| **092 C**: the compare in the pointee's unit, `n * sizeof(T) <= sizeof(lent)` | approve | §1.12 | unbuilt: the header's pointee text exists only in the probe unit, so it is plumbing from `cli/` into `emit/` | landed, `w/unit/wide.hero` with `n: 4` runs 0 and with `n: 16` stops before C | none |
| **092 U** (built here): refuse a `counted_by` whose header pointee is wider than a byte | approve, now, as the interim | §1.12 | **+65/-7 in 5 files**, 0 of 17 tracked `counted_by` programs move | its landing is under 80 insertions and moves none of the 17 | C landing, which supersedes it |
| **092 D**: the compiler passes the size | object | §1.1 | a `CParam` word; `parse/` would be 3 lines from its budget after R1 | landed after R1, the `parse/` BUDGETS row (8696) must move | the unit ruling and a parse budget with room |
| **092 E**: refusal alone | object | §1.12 (complete boundary) | about +65 lines | `092/today-void-count-fits.hero`, a correct program, is refused | never alone |
| **the `ffi_field_type` note names `i8`** (unlisted) | approve | §1.1 (0 spec tokens, one file) | **+13/-1 in `emit/ffi_field.hero`** | lands in under 15 insertions and moves no golden | none |

**Correction, 17:33 (`awk -f w/codelines.awk` on `base-selfhost`, `base-moved`,
`ck/selfhost`'s `ast.hero`: 550, 509, 517).** §12 said the move alone takes
`ast.hero` from 550 to 513 and that R1 adds 7 lines there, so 557 without the
move. Both wrong: the move alone reads **509** (513 was my first build's, with
the `Arg?` lines), R1 as built adds **8** (`rest: i64` twice with a three-line
comment, `rests` with its comment, its initialiser), so **517 with the move and
558 without it**, against the DECIDED 550. The landing's choice stands as
written: the move, or the row raised to 558 plus room with its reason.

**The final cold counts (17:24 to 17:41), `ic4/counts.txt`**, billions of
instructions retired for `build selfhost/main.hero --emit-c`, each run in a fresh
directory, rounds interleaved:

| | round 1 | round 2 | round 3 |
|---|---|---|---|
| base compiler, base tree | 1,091.61 | 1,092.18 | 1,091.92 |
| R1 as built, the same base tree | **1,091.43** | **1,091.57** | **1,091.45** |
| R1 as built, its own tree | 1,096.62 | 1,096.52 | 1,096.70 |

R1's compiler is no slower than base in any round on the same input; the last
row is the bigger tree (1,275,829 lines of C against 1,272,814), not the form.
Peak resident within 0.1 MB of base in every pair.

## 15. Panel 178's nine points, judged again on this tree

1. **R1 lands — AMENDED.** Built on 186's `build` judgement, not 178's
   `check`-side zero-fill, which 186's own `SHARED` check now refuses for any
   record holding a union (§7). The bit lives in `Checked`, never in the IR
   (§13). Four amendments the text must carry: with the words, a construction
   names **at most one** member of each union (none: all zero bytes, clang on
   three legs; two: still refused); a **handle** is refused; `T(rest: zero)` is
   `(T){0}`; and `rest: zero` is never an argument, so a function parameter
   named `rest` passed a binding named `zero` stops compiling (0 such programs in
   the tree, `grep`; `selfhost/leading_zeros.hero:32` has a parameter `rest`).
   178's order conditions: the `missing_fields` golden on a Heroes record is
   written (§4); **no DECIDED row moves** (only `ast.hero`, by the seam move or a
   raised row); the construction check already left `walk.hero` with 186.
2. **Z2 admitted, Z1 refused — KEPT.** `w/r1/kinds.hero` (zero as bytes);
   `w/pts/mutex_partial.hero`, run 0, `22` (the state Z1 guards needs no claim).
3. **Z3 by 094's lowering — KEPT, and it stands now**: `w/z3/cinit.hero` `2`,
   `mutex_init.hero` `0`; its limit, `cond_as_mutex.hero` `22` at exit 0.
4. **L — KEPT, closed** by 097: the ten `x091` programs (§2).
5. **§ 13 says C's `char` is `i8` — AMENDED**: the half that costs no spec
   token is built (§10, +13/-1, the note and a `guess` fix); whether the
   sentence lands too is the spec-warden's measurement.
6. **T waits — KEPT**: `w/pts/sun_c_darwin.hero` writes the path at 0 lines.
7. **A waits — KEPT** (not rebuilt).
8. **R2 refused, D refused, R0 as a separate spelling refused — KEPT**:
   `w/pts/zstr.c` 134 for D; R2 is one deleted assertion.
9. **Padding not promised — not re-run by this seat.**

## 16. The suites on R1 as built (17:28:40 to 18:07:02)

`heroes run tests/harness/main.hero -- ./heroes-final <suite>`, one suite at a
time in the copy whose `selfhost/` is `ck/selfhost` (`diff -rq`: equal), the
compiler `ck/heroes-ck`: **every one exit 0, 0 failed**: check: 551 passed, 0 failed; ir: 27 passed, 0 failed; emit: 9 passed, 0 failed; unsupported: 151 passed, 0 failed; permissive: 7 passed, 0 failed; full: 10 passed, 0 failed; run: 308 passed, 0 failed; annotations: 765 passed, 0 failed; determinism: 342 passed, 0 failed; emission: 839 passed, 0 failed; descriptors: 403 passed, 0 failed; wholes: 403 passed, 0 failed; cache: 7 passed, 0 failed; units: 3 passed, 0 failed; fixes: 814 passed, 0 failed; lines: 309 passed, 0 failed; corpus: 55 passed, 0 failed; warnings: 370 passed, 0 failed; surface: 377 passed, 0 failed; special: 10 passed, 0 failed; spec: 21 passed, 0 failed; grammar: 9 passed, 0 failed; layout: 5 passed, 0 failed; canonical: 2 passed, 0 failed; probe: 27 passed, 0 failed; order: 3 passed, 0 failed; runtime: 8 passed, 0 failed.

`records` and `unseen` were not run: both ask `git ls-files`, and a
`git archive` copy has no repository (the first full run stopped there at
16:38 with exit 2, after its 18 earlier suites passed). Linux arm64, x86-64 and
Windows: **unrun** for R1; the union probe's C half ran on both Linux images (§3).

Finished 18:07 (`date`).
