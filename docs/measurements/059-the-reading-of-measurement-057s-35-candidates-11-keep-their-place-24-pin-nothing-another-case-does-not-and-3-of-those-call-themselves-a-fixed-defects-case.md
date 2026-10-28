# 059: the reading of measurement 057's 35 candidates: 11 keep their place, 24 pin nothing another case does not, and 3 of those call themselves a fixed defect's case

Read 2026-10-28 at M-generated-programs for task 2214 (https://github.com/heroes-lang/heroes/issues/2214), the reading measurement 057 § 1 asks before any case is removed: *a reading that tries to refute its removal*. It was made by an agent the coordinator launched for it, read-only, with the brief to refute each removal by the caret's column, a note's words, an excerpt's line and the branch a future regression would meet; its report is reproduced below as it wrote it, and the coordinator re-ran two of its claims before this file was written (`suite_surface.hero` reads `shadowing` at lines 454, 461 and 611 and `indentation` at 648; `stray-carriage-return.hero` opens `FIXED DEFECT, 2026-08-12`). **No case is removed by this file**: removing one deletes files, which CLAUDE.md § Hard stops makes the author's to ask for.

**What it changes in 057's criterion**: 057 counted *the rule* by the `fixedbugs-` prefix alone. Three of the 24 the reading finds removable, `leading-zero`, `stray-carriage-return` and `int-out-of-range`, name themselves a fixed defect's case in their own header, and the rule's words are *a fixed defect gets a case named after it, carrying symptom, cause and date*, so read by those words they keep their place, and **21** are left for the author's word.

## The reading, as the agent wrote it

Read 2026-10-28 in the lane `lane-generated-programs` at `e734069f9` (its HEAD when the
reading ran; step 12, committed while it ran, touches neither `selfhost/` nor
`tests/golden/check/`, `git diff --stat 4395e3dd1 HEAD -- tests/golden/check selfhost` empty),
with the lane's `heroes-next` (`sources 16c5eddf04c4be5e`). Read-only: no case was touched.

**Reproduced first**: `heroes-next check --brief` on each of the 35, stdout and stderr together,
against its `.expected`: 35 the same bytes, 0 differ.

Method, per case, trying to refute its removal (measurement 057 § 1): its `.hero` with its header
and its `.expected`; every other `.expected` under `tests/golden/{check,full,permissive}` pinning
the same `[code]` (`unsupported/`, `control/` and `run/` hold none of these codes as diagnostics,
counted), compared on the words, the caret's line:column and the provoking shape; the compiler's
emission site and the branch the case drives; and `git grep` of the case's path across the tree,
since a living instrument or document that reads the case is a cost no coverage count sees. None
of the 35 has a twin of its name under another form (`full/`, `permissive/`, `unsupported/`,
`control/`, `run/`, `emit/`, `ir/`: none, listed).

Verdict words: **KEEP** where it pins something no other case pins (or where a living instrument
reads the file, said so); **REMOVABLE** where every pin is held elsewhere, the cases named;
*owes* names what a removal must rewrite in the same change.

## The table

| case | code | compared with | what it pins alone, if anything | verdict |
|---|---|---|---|---|
| arm-pattern | expected_pattern | fixedbugs-130-a-block-under-an-arm:27:9 (`+ =>` with no inline body over a block), fixedbugs-132-an-arm-that-begins-with-an-operator:65:5, fixedbugs-193-a-failed-arms-body-is-read:90, the 17 check and 1 permissive cases of the code | the only failed arm with an INLINE body over a deeper line (`+ => 1` over `print(k)`), the panel 014 hang shape, reaching `arms_of`'s `.indent` arm with `failed` set (`selfhost/grammar_expr.hero:1188-1196`); and `.github/workflows/ci.yml:689` runs `check` on it on every leg | **KEEP** |
| braces-as-block | missing_body | fixedbugs-131-the-brace-habit-is-one-mistake:50:17 | nothing: the same `function main() {` over a body and a `}` ending the file, the same words naming `main`, the same column | REMOVABLE |
| builtin-name-taken | builtin_name_taken | fixedbugs-455 (27:10 top level, 30:12 parameter, 37:5 `=` local), fixedbugs-501 (31:10, 37:12, 57:5), fixedbugs-422 (cells, an `@` parameter, a loop variable, a payload; inventory-only `len` `sort` `keys`), fixedbugs-460 (type parameters) | nothing: the two sites it reaches (`resolve/top.hero:92`, `resolve/state.hero:357` through `state.declare`) and both halves of `builtin_names.taken` (inventory, library) are driven elsewhere; the one combination it alone writes, a `.heroes`-tier inventory name declared at the top level (`map`), is also a name the library declares, so the library half matches it whatever the inventory half does | REMOVABLE |
| cell-redeclared-over-a-binding-is-shadowing | shadowed_binding | fixedbugs-364:53:5, fixedbugs-662-* (a `=` over a cell), shadowing:11:5; a scan of every check, full, permissive, control and unsupported case for an untyped `@=` over an earlier `=` in one function: this case alone | the only case driving `meant_rebinding`'s `!mutable` disjunct (`selfhost/resolve/meant.hero:127`): `x @= 2` over `x = 1` is the plain shadow with no certain fix, where `cell_redeclared`'s certain `@` would write a write to a `=` binding | **KEEP** |
| declaration-in-arm | declaration_in_arm | fixedbugs-153-a-name-bound-on-an-arm-s-line:13:14 to 23:14, fixedbugs-130-after-a-thesis-rule:14:14, fixedbugs-131-a-type-that-failed-in-an-annotation:34:14 | nothing: `arm_body.reject_declaration` reads the statement alone (`selfhost/parse/arm_body.hero:35-61`); its payload pattern drives nothing there; the thesis rule keeps its `annotations` witness in fixedbugs-153 | REMOVABLE |
| ffi-a-group-head-names-not-locates | machine_locked_path, unwritable_name | fixedbugs-225:13:8 (a rooted header, same words, note, column), permissive/fixedbugs-446-...-rooted-library...:16:23 and :28:23 (`/opt/...` and `:/opt/...` links, same words and note), fixedbugs-233:14:23 (a rooted link told `unwritable_name` alone in the strict form) | nothing: `head_names.judged` is one function for both arms (`selfhost/head_names.hero:27-33`), each of `rooted()`'s three conditions (`:208-215`) driven elsewhere | REMOVABLE |
| ffi-a-lend-parked-in-a-group-record | lease_escapes, cstr_escapes | lease-copied-out-of-its-cell:20:9 and 21:15, fixedbugs-a-lend-bound-instead-of-passed:24:9, fixedbugs-a-lend-that-leaves-its-frame:28:12, fixedbugs-264:30:9 | defect 067's door: a lease and a lend handed to a group record's CONSTRUCTION, refused through `keeps_its_argument` (`selfhost/check/lending.hero:159-192`, `:223`; `check/leasing.hero:121`); every other case refuses one standing in no call | **KEEP** |
| ffi-a-lend-reaches-only-a-lent-parameter | lend_kept, lend_needs_a_header, field_lend_uncounted | fixedbugs-a-lend-c-keeps-past-the-frame:34:13 (`ptr`, counted, not lent) and :38:19 (`cstr`, unmarked), fixedbugs-a-lend-laundered-through-a-helper:32:20, ffi-counted-by-shape:39:24 (unmarked and uncounted: the extent told first, alone) | nothing: each of `lend_landing.landed_on_a_header`'s outcomes (`selfhost/check/lend_landing.hero:163-196`: no header, uncounted, `lent` quiet, kept) and their order pinned elsewhere; what it admits at an unmarked parameter is no lend to that function, and is built by `run/` cases: a lease at `stash_put(s: cstr)` (run/lease-c-keeps-the-pointer), `nullptr` and a value C handed back (run/fixedbugs-396-sha256-final-fills-its-digest:40) | REMOVABLE |
| ffi-consumes-a-borrowed-handle | consumed_borrowed_handle | fixedbugs-629-a-handle-ended-through-the-dot:24:5 to 37:11 and -handed-away:17:5, 21:27 (all through the dot); ffi-transfers-a-borrowed-handle:19:34 | the named-call route's refusal (`selfhost/check/walk.hero:1210`, `consuming.refuse_borrowed`), which only it and ffi-transfers reach; defect 031's own reproducer | **KEEP** |
| ffi-parameter-width | type_mismatch | fixedbugs-2224:16:16 (`exit(code: n)`, an `i64` at a `u8` parameter), fixedbugs-a-crossed-free-between-two-void-families (an `extern` call's argument refused) | nothing: an `extern` function is checked by the same `named_call` and `user_call` as any function (`selfhost/check/walk.hero:1200-1213`) | REMOVABLE |
| ffi-transfers-a-borrowed-handle | consumed_borrowed_handle | ffi-consumes-a-borrowed-handle (the named route), fixedbugs-629-a-handle-handed-away:17:5, :21:27 (the transfer's words, through the dot) | nothing while ffi-consumes stays: the two are the only refusals through the named route | REMOVABLE, only with ffi-consumes kept |
| handle-ended-on-every-path-is-refused-at-the-read | handle_used_after_end | fixedbugs-409, -499, -511 (`if`s, a nested `if`, `match` arms, an END on `&&`'s right), -530 (`if`/`else` in a loop), fixedbugs-a-handle-read-after-the-call-that-ended-it, handle-a-consumed-part-... | the only refused READ on the right side of `&&` (70:26; no other `handle_used_after_end` line in check or full holds `&&` or `\|\|`): a regression that stops walking `&&`'s right operand (`selfhost/check/moved_walk.hero:153-161`) leaves fixedbugs-511's `shorted` green | **KEEP** |
| indentation | indentation_not_multiple_of_4, indentation_jump | fixedbugs-352:45:1 (five spaces under a function head, "found 5"), :39:1 and :52:1 (jumps with no braces), fixedbugs-130-margins-in-braces...:39:1, :43:1 ("level 0 to level 2") | as a check case nothing (one branch each, `selfhost/layout.hero:174-185`); but `tests/harness/suite_surface.hero:648` runs `probe` on it and asserts its code | **KEEP** (a suite reads it) |
| inout-on-a-by-value-parameter | not_mutable | fixedbugs-inout-on-an-immutable-corrupted-the-heap:30:10 | nothing: the same program (`Bag`, `add(@b: Bag, s: str)`, `reader(b: Bag)`, `add(@b, "x")`), words and column | REMOVABLE |
| inout-on-a-generic-by-value | not_mutable | fixedbugs-inout-...:30:10 | nothing: `inout_root`'s `.name` arm and `write_root` read the binding's kind, never its type, and the resolver's walk has no generic branch (`grep generic selfhost/resolve/walk.hero`: none) | REMOVABLE |
| inout-on-a-loop-variable | not_mutable | not-mutable:19:9 (the `.loop_var` words, through a statement), fixedbugs-inout-... (the `@` argument's `.name` arm) | nothing: `.name` then `write_root` then `errors.not_mutable(kind)`, each driven elsewhere | REMOVABLE |
| inout-on-a-plain-binding | not_mutable | not-mutable:12:5 (`.bind`), fixedbugs-inout-... | nothing, as above | REMOVABLE |
| inout-through-a-field-of-an-immutable | not_mutable | inout-on-a-match-payload:19:18 (`@s.bag`: the `.field` arm into a refusal, caret on the root), fixedbugs-inout-... | nothing | REMOVABLE |
| inout-through-a-nested-record-of-an-immutable | not_mutable | as the field case | nothing: its `@x.bag` is one field step, as `@b.items` is (the header's "two field steps" are `x.bag.items`, inside `add`) | REMOVABLE |
| inout-through-an-index-of-an-immutable | not_mutable | every `@x[...]` argument in check, full, permissive (fixedbugs-260, fixedbugs-two-mutable-arguments-of-one-place, fixedbugs-440: roots that are cells or a `str`) | the only refusal reached through `inout_root`'s `.index` arm (`selfhost/resolve/writes.hero:115`), the shape that was SIGBUS | **KEEP** |
| int-out-of-range | int_out_of_range | literal-out-of-range:24:9 (a decimal no `i64` holds, no width, same words), fixedbugs-a-literal-arm-outside-the-width-it-matches:33:9 (a literal past `u64`), run/a-literal-arm-takes-the-width-it-matches, run/bitwise:30, run/fixedbugs-382 (the boundaries, built) | nothing: `synth`'s `.int_lit` arm asks `decode_int(...).is_err()` once for both overflows (`selfhost/check/walk.hero:126-128`) | REMOVABLE |
| leading-zero | leading_zero | fixedbugs-325-a-method-on-a-leading-zero-integer (`0700`, `0_7`, `00`, and `07.5` admitted at :20), fixedbugs-329, fixedbugs-324-*, full/fixedbugs-289 (`0700`, its note and both guesses) | nothing as a pin | REMOVABLE, *owes* `selfhost/leading_zeros.hero:23` and docs/learn/2026-08/12/2026-08-12-0013-...md:3 |
| line-end-before-continuation-in-a-call | line_end_before_continuation | fixedbugs-130-a-line-going-on-below-a-bracket-never-closed:30:5 and fixedbugs-177-...:70:9 (the argument reading, before `.` only), fixedbugs-135-a-bracket-below-a-value-in-a-group-or-an-index-is-a-guess (`(` `[` in a group or an index: the operand reading), line-end-before-continuation (outside a call, `.fixed`) | the argument reading before `-`, `[` and `(`, `begins_a_value`'s three other disjuncts in a call (`selfhost/parse/line_end.hero:110-111`, `:216-218`), and, carrying no `.fixed` or `.applied`, that none of those fixes is `certain` (the `fixes` suite's *left as it is by `--apply`*): panel 180's veto on `f(a` over `-1))` | **KEEP** |
| missing-body | missing_body, empty_record, empty_variant | fixedbugs-130-a-body-told-missing-names-its-declaration:36:1 (`constant LIMIT: i64` over the next head) and :40:1 (a body missing at the file's end), fixedbugs-133-a-declaration-head-that-failed-over-no-body:22:1 (`missing_body` then `empty_record` on one position, that order), fixedbugs-315:46:1, fixedbugs-130-an-empty-declaration-above-a-refused-line:28:1 | nothing as a pin | REMOVABLE, *owes* `selfhost/parse/tails.hero:84` and docs/learn/2026-08/03/2026-08-03-0086-...md:3 |
| missing-return | missing_return | fixedbugs-409-every-function-off-its-end-is-told-in-order:11:24 (`if x > 0` over `return x`, nothing after, caret on the result type) | nothing: the same shape and caret rule | REMOVABLE |
| nested-fixed-array-tiebreak | fixed_outside_a_group | fixedbugs-405-a-nested-fixed-array-outside-a-group-is-told-once:44:8 (`x: i64[2][3] = ...`, one message, same column), fixedbugs-423:43:11 (a literal under a refused type not judged), fixedbugs-432:33:8 (`len(x)` judged as the growable) | nothing; the tie it was written for no longer arises (its own header) | REMOVABLE (full/fixedbugs-399's header, lines 16 and 22, names it as the pin of *told once*) |
| never-rebound-beside-a-refused-marker | marker_mismatch | fixedbugs-412-an-at-argument-to-a-construction (12 `marker_mismatch` and no `never_rebound` on cells whose only `@` stands in a construction: `a_record`, `a_group_record`, `a_variant_case`, `ok(@n)`) | nothing | REMOVABLE |
| never-rebound-never-read-is-unused | unused_binding | unused-bindings:14:5 (`total: i64 @= 0`, `total @ 1`, never read; same words, same column; it carries an `.applied`) | nothing | REMOVABLE |
| no-size-self | no_size | no-size-best-friend:18:18 (direct self-containment, same words, caret on the field's type), no-size-mutual:22:8 (the field at index 0, through another record), no-size-through-fallible:24:11 | nothing | REMOVABLE (its twin no-size-best-friend is `UNVERIFIED` and cites it) |
| not-a-place | not_a_place | no-place-before-the-cell-symbol:10:5 (`f() @ 5`: same words, same column, the same `cell_errors.written` call, `not_a_place(symbol: "@")`) | nothing | REMOVABLE |
| repeat-count-is-unsigned | type_mismatch | full/panel227-an-integer-of-another-width-names-its-conversion:9:24 (an `i64` binding as `repeat`'s count, with its note), run/fixedbugs-564-the-operands-of-an-operator-take-the-type-its-position-asks:46 (`push(2 + 3)`), run/fixedbugs-577-a-constant-whose-every-step-fits-runs:103 (panel 206 R2) | line 27, `repeat("-", 0 - 1)` checking clean: panel 206's R3 for `rep`, and the only `repeat` of an operator in the tree | **KEEP** |
| rest-zero-ends-only-a-construction-of-a-groups-record | rest_outside_a_group, rest_not_a_construction, rest_is_a_field | fixedbugs-187-the-words-rest-zero-are-kept-in-place:28 (`Point(x:, rest: zero)`), fixedbugs-401-a-binding-called-zero-hides-no-mistake-at-the-words:44 (a function), :47 (UFCS), :59 (`Withrest(..., rest: zero)`), fixedbugs-401-a-constant-called-zero:28 | nothing as a branch: its `pick()(...)`, the only value called, takes `ended`'s `.call` arm as `add(...)` does and `reading`'s `.unresolved` arm as `1.add(...)` does (`selfhost/resolve/rest_zero.hero:122-166`) | REMOVABLE |
| shadowing | shadowed_binding | fixedbugs-364:53:5 (`n = 4` over a parameter), fixedbugs-422:77:5 (a local over a top-level declaration, the *in scope everywhere* words) | as a check case nothing; but `tests/harness/suite_surface.hero:454`, `:461` and `:611` run `check` and `build` on it | **KEEP** (a suite reads it) |
| stray-carriage-return | stray_carriage_return | fixedbugs-141-a-stray-carriage-return-ending-the-file-is-refused:13:13 (one CR after `print(1)`, same column), fixedbugs-494-a-run-of-tabs-returns-or-foreign-words-is-told-once:27:11 to 33:11 (one CR mid-line) | nothing: one branch whatever follows the CR (`selfhost/scan.hero:65-75`) | REMOVABLE |
| unterminated-hole | unterminated_string | fixedbugs-135-crlf-an-f-literal-open-at-a-crlf-line-end:39:14 (a hole left open at a CRLF line end, same words; the same `lex_interp.line_ends`, whose one caller is `scan.hero:77`) | the only `#` standing inside a hole left open, lexed as a comment (`lex --dump-tokens`: `8:20 comment`, the error at `8:42`) | **KEEP** (weak) |

## The KEEPs, with the evidence

**arm-pattern.** `arms_of` (`selfhost/grammar_expr.hero:1158`) meets an indent where an arm
should start at `:1188`, and its comment says why the arm exists: *Else the loop spins:
`skip_line` stops BEFORE an indent (panel 014)*. Below a failed arm the block is read and not
named (`:1194` skips `orphans.read` when `failed`). An arm whose pattern fails with NO inline body
reads its block inside `arm()` (`cursor.at(c, .indent)` after the `=>`), so fixedbugs-130-a-block-under-an-arm:27
and fixedbugs-132:65 never reach `:1188`; an arm with an inline body that failed leaves the
cursor before the indent, and only then does the loop meet it with `failed` set. Scanned: every
annotated arm line in check, full and permissive with an inline body after `=>` and a deeper next
line; arm-pattern:6 is the only one whose arm failed (the others are whole arms, one of them over a
`\` the lexer drops, an `if` head that consumes its own block, a brace habit, a tab margin, a
function head). Region coverage cannot tell
this path from the `!failed` one, since the `hanging` call after the `if` is one region. Beside
that, `.github/workflows/ci.yml:684-694` (*A diagnostic reaches the log*) runs `heroes check` on
this file on every leg and asserts exit 1 and `error[` on stderr: deleting it turns that step red
until it is repointed. What would settle the branch beyond the scan: a branch-coverage run
(`llvm-cov show -show-branches=count`) of `arms_of` over the check cases without it, unrun.

**cell-redeclared-over-a-binding-is-shadowing.** An untyped `v @= e` goes to
`meant_rebinding` (`selfhost/resolve/meant.hero:124`), which returns false when no local of the
name stands OR when the one standing is not mutable (`:127`); the walk then declares, and
`state.declare` tells the shadow. The first disjunct is every first `@=` of a program; the second
only this shape. A regression dropping it would tell `cell_redeclared` here with its `certain` fix
`@` (`:185-194`), and `check --apply` would write `x @ 2`, a write to a `=` binding, refused
`not_mutable`: the case's header says *carries no certain fix*. The scan (an awk over every
`.hero` of check, full, permissive, control and unsupported, per function, for an untyped
`name @= ` after a `name = `, and for an untyped `@=` over a by-value parameter) found this case
alone. A learn question also names it (docs/learn/2026-10/10/2026-10-10-2228-...md:1, by its
name).

**ffi-a-lend-parked-in-a-group-record.** `lending.keeps_its_argument` (`selfhost/check/lending.hero:184`)
is true only where the lend's parent is a call whose callee is a `record_decl`
(`constructed_record`, `:159`), and `cstr_escapes` fires at `:223-227` on `into[at].is_err() ||
kept`; `leasing.hero:121` asks the same for a lease. Every other `cstr_escapes` and
`lease_escapes` case (lease-copied-out-of-its-cell, fixedbugs-a-lend-bound-instead-of-passed,
fixedbugs-a-lend-that-leaves-its-frame, fixedbugs-264) takes the `into[at].is_err()` side; only
this case takes `kept`, in both directions, which is defect 067 (heap-use-after-free at exit 0).
Its header is a fixed defect's (SYMPTOM, CAUSE, FIX, the date), and
`tests/golden/fixedbugs/a-header-field-that-is-not-const.hero:50-57` hands this refused shape to
it by path.

**ffi-consumes-a-borrowed-handle** (and the condition on ffi-transfers). `refuse_borrowed`
(`selfhost/check/consuming.hero:111`) has two callers: the named call,
`selfhost/check/walk.hero:1210`, and the dotted one, `:1473` through `refuse_borrowed_dotted`. The
eight other refusals of the code are all fixedbugs-629's, all dotted (`h.h_close()`,
`o.h.h_close()`, `h.h_into(...)`, `parent.h_adopt(child: h)`). So the named route's refusal
stands in these two cases and nowhere else: keep one. ffi-consumes is defect 031's own reproducer
(its header), pins position 0 and the *ends the life* words, and admits a local consumed and an
`@` parameter consumed; ffi-transfers pins position 1 and the *hands the life* words, both of
which fixedbugs-629-a-handle-handed-away pins through the dot. Hence ffi-transfers REMOVABLE only
while ffi-consumes stays.

**handle-ended-on-every-path-is-refused-at-the-read.** `moved_walk.expr`'s `.binary` arm
(`selfhost/check/moved_walk.hero:153-161`) splits the flow for `&&`'s right operand, walks it and
meets. Every `handle_used_after_end` line of check and full was read for `&&` and `||`: only
`:70` here. fixedbugs-511's `shorted` ENDS a handle on the right of `&&` and refuses a read inside
the `if`'s body, so it does not see a regression that skips walking the right operand (`r` is
still ended, `q` simply never ends) or that walks it from an unreachable state (the meet then
returns the base); this case's read on the right side is refused only while the right operand is
walked from the state before it. Its other shapes are held elsewhere (both arms: fixedbugs-511
`nested`, -499 `branches`; every `match` arm: -511 `picked`; the same turn of a loop: -530
`looped`; twice through one name: -409, -499 `again`; its admitted `one_arm_returns` reads the
same with or without the `return`, as the meet is an intersection).

**indentation.** As a diagnostic, nothing it pins is its own: `selfhost/layout.hero:174-185` has
one branch per code, *found 5* with no habit suffix is fixedbugs-352:45:1 under a function head,
a jump with no braces is fixedbugs-352:39:1 and :52:1, *level 0 to level 2* is
fixedbugs-130-margins-in-braces...:39:1 and :43:1. It is KEEP because
`tests/harness/suite_surface.hero:648` (*probe refuses a file with diagnostics at exit 1, as fmt
does*) runs `probe tests/golden/check/indentation.hero` and asserts
`error[indentation_not_multiple_of_4]`: deleting it turns `surface` red. Repoint that row (to
fixedbugs-352, which also holds that code) and it becomes REMOVABLE.

**inout-through-an-index-of-an-immutable.** `inout_root` (`selfhost/resolve/writes.hero:98`) has
three place arms: `.name`, `.field` (`:105-114`), `.index` (`:115`). Every `@x[...]` argument in
check, full and permissive was listed (a grep for `@name[` on annotated lines): fixedbugs-260 and
fixedbugs-two-mutable-arguments-of-one-place have cell roots (`aliased_mutable_arguments`),
fixedbugs-440 a `str` (`str_element_write`); this is the only one whose root is not a cell, so
the only one where the `.index` arm leads to `not_mutable`. Its header records what that was
before 2026-09-03: exit 138, SIGBUS. The `.field` arm's refusal, by contrast, stands in
inout-on-a-match-payload (`@s.bag`), which is not a candidate, which is why the field and nested
cases are REMOVABLE.

**line-end-before-continuation-in-a-call.** `other_reading` (`selfhost/parse/line_end.hero:216`)
gives the *argument* reading in a call when `begins_a_value(kind)`, which is `-`, `.`, `(` or `[`
(`:110-111`); the reading makes both fixes `guess`es and adds the `,`
(`selfhost/parse/line_end_readings.hero:57-62`, `line_end.hero:249-252`). The six messages in the
tree with the argument tail are this case's four and two `.` lines (fixedbugs-130-a-line-going-on-below-a-bracket-never-closed:30:5,
fixedbugs-177:70:9); `-`, `(` and `[` in a call are here alone (fixedbugs-135's `(` and `[` are
in a group or an index, the *operand* reading, decided by another condition). Dropping one of the
three from `begins_a_value` would make that fix `certain` (for `-1` touching its operand
`stands_alone` reads `begins_a_value` too), and since this case carries neither `.fixed` nor
`.applied`, the `fixes` suite's *a case with neither file is left as it is by `--apply`* would go
red here: `print(f(a` over `-1))` read on as a subtraction is the program that printed a sum at
exit 0 (panel 180's veto, the case's header).

**repeat-count-is-unsigned.** Its refusal, line 29, is held by
full/panel227-an-integer-of-another-width-names-its-conversion:9:24 (the same `repeat` of an
`i64` binding, with the note). Line 27, `print(repeat("-", 0 - 1))`, checks clean: the operator
takes `repeat`'s `u64` (`selfhost/check/builtins.hero:74-75`, then `:262-268`) and aborts where it
runs, panel 206's R3 for `rep`, ratified, which the panel's file says is recorded in this case's
header. Each factor is held elsewhere in general (a built-in's argument asking its type of an
operator of literals: run/fixedbugs-564-...:46, `push(2 + 3)`; literal arithmetic in a function
not refused: run/fixedbugs-577-...:103, R2), but `repeat("-", 0 - 1)` appears in no other golden
(grep over `tests/golden/*/*.hero`), so an R11a-shaped change at `repeat`'s count (the refusal
R3 did not adopt) would turn this case red and no other. Weaker than the others above, and
written as such. Two notes: run/abort-repeat-overflows.hero:14-16 hands it *the negative count*
edge in words stale since defect 564 (`0 - 1` is accepted, not `type_mismatch`); and its own
corrected header says a negative literal count is refused `int_out_of_range`, which no golden
pins (`repeat("-", -1)` was run: `-1` does not fit a `u64`, exit 1; grep for `repeat("…", -`
across the goldens: nothing). The generic rule, a negative literal at an unsigned type, is pinned
(fixedbugs-2224:15:20, fixedbugs-429:20:17, fixedbugs-564:23:13).

**shadowing.** Its two refusals are held by fixedbugs-364:53:5 and fixedbugs-422:77:5 (both
through `state.declare`), and its sibling loops reusing `x` are built everywhere. KEEP because
`tests/harness/suite_surface.hero:454` (*a file with diagnostics exits 1*), `:461` (*a diagnostic
is not an artifact*) and `:611` (*build honours it for diagnostics*) run `check` and `build` on
it; repointing those three rows makes it REMOVABLE.

**unterminated-hole** (weak). The hole message and its place at the line end come from
`lex_interp.line_ends` (`selfhost/lex_interp.hero:62-70`), whose only caller is
`selfhost/scan.hero:77`, the same for fixedbugs-135-crlf:39:14 (its column 14 is the CR the
renderer shows as the line's end). What only this case holds: its annotation stands inside the
hole it leaves open, `{n  #~ unterminated_string`, and the lexer reads it as a comment
(`heroes-next lex ... --dump-tokens`: `8:17 ident n`, `8:20 comment`, `8:42 error`); a grep for a
`#` after an unclosed `{` in an `f"` literal across the goldens and `examples/` found it alone
(fixedbugs-135-crlf:30 is `{{`, a brace written, no hole). A change to what `#` means inside a
hole would move this case and no other; spec § 2 says nothing of comments in a hole. The pin is
incidental to the header's purpose (one report, the next line clean, both held by the CRLF case),
so if the author reads it as incidental, it is REMOVABLE against fixedbugs-135-crlf:39:14.

## No UNSURE

None was left unsettled; the two readings that rest on a scan rather than a branch-coverage run
are said in their paragraphs (arm-pattern, cell-redeclared).

## What the REMOVABLE ones owe, and a note on the criterion

- **Owes a citation rewritten in the same change** (living files, outside `RECORD`; their `#` and
  `.md` lines are what `records/citations` reads, `tests/harness/suite_records.hero:75` (the
  record trees it skips), `:2934` (`is_prose`), `:3657-3678` (the walk); that it would go red is
  unrun): leading-zero (`selfhost/leading_zeros.hero:23`,
  docs/learn/2026-08/12/2026-08-12-0013-...md:3), missing-body (`selfhost/parse/tails.hero:84`,
  docs/learn/2026-08/03/2026-08-03-0086-...md:3).
- **Named by another golden** (record trees, which `records/citations` skips, so a dangling
  sentence and not a red suite): nested-fixed-array-tiebreak (full/fixedbugs-399...:16, :22),
  no-size-self (no-size-best-friend:8; and no-size-best-friend is `UNVERIFIED — pending debrief`:
  if the debrief drops it, no-size-self is the last direct self-containment case),
  leading-zero (run/literal-bases:17, full/fixedbugs-289:3).
- **The rule witness, read by its words.** Measurement 057 counted witness 2 by the `fixedbugs-`
  prefix. `.claude/rules/diagnostics-and-goldens.md` words the rule as *a fixed defect gets a case
  named after it, carrying symptom, cause and date*. Five of the 35 declare themselves such a case
  in their headers: ffi-a-lend-parked-in-a-group-record (defect 067) and
  ffi-consumes-a-borrowed-handle (defect 031's reproducer), both KEEP on their pins anyway; and
  leading-zero (*fixedbugs, M-literal-bases step 1, 2026-08-12*), stray-carriage-return (*FIXED
  DEFECT, 2026-08-12, sweep 001 audit S6*) and int-out-of-range (*the defect it records*, no
  date), REMOVABLE on their pins. If the author reads witness 2 by the rule's words, those three
  keep their place; read by the prefix, as 057 did, the table stands.
- **Removable together.** Every REMOVABLE row's comparators are cases that are not candidates
  (fixedbugs-*, not-mutable, inout-on-a-match-payload, no-place-before-the-cell-symbol,
  literal-out-of-range, unused-bindings, no-size-best-friend, no-size-mutual, ffi-counted-by-shape),
  so the 24 can go together, with the one exception said: ffi-transfers-a-borrowed-handle only
  while ffi-consumes-a-borrowed-handle stays.

## Summary

- **KEEP: 11.** On a pin no other case holds, 8: arm-pattern, cell-redeclared-over-a-binding-is-shadowing,
  ffi-a-lend-parked-in-a-group-record, ffi-consumes-a-borrowed-handle,
  handle-ended-on-every-path-is-refused-at-the-read, inout-through-an-index-of-an-immutable,
  line-end-before-continuation-in-a-call, repeat-count-is-unsigned (the weakest of the eight);
  weak, incidental to its purpose: unterminated-hole; because a living suite runs them, with no
  pin of their own: indentation, shadowing (arm-pattern is also run by the CI).
- **REMOVABLE: 24**: braces-as-block, builtin-name-taken, declaration-in-arm,
  ffi-a-group-head-names-not-locates, ffi-a-lend-reaches-only-a-lent-parameter,
  ffi-parameter-width, ffi-transfers-a-borrowed-handle (only with ffi-consumes kept),
  inout-on-a-by-value-parameter, inout-on-a-generic-by-value, inout-on-a-loop-variable,
  inout-on-a-plain-binding, inout-through-a-field-of-an-immutable,
  inout-through-a-nested-record-of-an-immutable, int-out-of-range, leading-zero (owes two
  citations), missing-body (owes two citations), missing-return, nested-fixed-array-tiebreak,
  never-rebound-beside-a-refused-marker, never-rebound-never-read-is-unused, no-size-self,
  not-a-place, rest-zero-ends-only-a-construction-of-a-groups-record, stray-carriage-return.
- **UNSURE: 0.**
- Nothing was removed; removing any of these deletes files, which is the author's to ask.
