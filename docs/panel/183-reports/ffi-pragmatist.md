# Panel 183, ffi-pragmatist (the sitting widened)

Written 2026-09-30 from 07:07, as I went. Every number below names the command
that produced it, run in my own directory, `<seat>` =
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/183-ffi-pragmatist/`.
Every output quoted is a file there: the seven probes' under `<seat>/out/`,
named `<probe>.<COMPILER>.<arm>.txt`, the last line of each the exit.

## The compilers

| name | built from | how |
|---|---|---|
| `NEW` | `<seat>/new/heroes`, `git archive 171e8c45`, `build` removed | `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`real 15.93`, `user 7.16`: the machine was loaded, so not a duration) |
| `PE` | `<seat>/pe/heroes-pe`, a second archive with `pe-selfhost.patch` applied | the patch's paths rewritten to `selfhost/...` (`<seat>/pe-stripped.patch`), `patch -p0 --dry-run` then applied; `diff -rq` against `NEW`'s `selfhost/` names `layout.hero` and `next_line.hero` only; `grep` finds no caller of the renamed `opens_a_declaration` outside the patch; built by `NEW`, `heroes build selfhost/main.hero -o heroes-pe`, exit 0 (`real 148.91`, `user 78.30`, load 39) |

The words that end a reach under `PE` (`next_line.ends_a_reach`, read in
`<seat>/pe/selfhost/next_line.hero`): `constant function record variant test
extern use return break continue assert for while` (`starts_afresh`), `if`,
`match`, `function` only before a name, `else` only strictly shallower. The
group's own words (`link`, `package`, `tag`, `partial`, `owned`, `lent`,
`consumes`, `acquires`, `borrows`, `transfers`, `retains`, `counted_by`,
`when`, `as`) are contextual, not keywords (`selfhost/keywords.hero` lines 85
to 124), so no mark on a parameter can end a reach.

## Task 1: the seven probes, both compilers, both arms

`heroes check <probe>` and `heroes check <probe> --permissive`. **The
permissive arm prints the normal arm's bytes on all 14 runs** (`cmp`), so one
column below stands for both. `PE` prints `NEW`'s bytes on four probes
(`e17_extern_below`, `extern_member_eof`, `extern_member_unclosed`,
`x2c_member_mistake_alone`) and moves three.

| probe | its mistakes | `NEW` prints | `PE` prints |
|---|---|---|---|
| `e17_extern_below` | `(` open in a body; `junk` after a member | `2:10 unclosed_bracket`, `6:33 expected_extern_signature` (`junk`) | same bytes |
| `extern_member_eof` | `(` open on a member; `junk` two members down | `2:17 unclosed_bracket`, `4:33 expected_extern_signature` | same bytes |
| `extern_member_unclosed` | the same, and `print(1 +)` | `2:17`, `4:33`, `7:14 expected_expression` | same bytes |
| `x1_extern_stray` | `(` open on `cos`; stray `)` on `sin`; `junk` on `tan`; `print(1 +)` | `2:25 expected_params_close` (*found `->`*), `4:33`, `7:14`: **the stray `)` is not told** and `sin` is read inside `cos`'s parameters | `2:17 unclosed_bracket`, **`3:25 expected_extern_signature` (*found `)`*)**, `4:33`, `7:14`: all four |
| `x2_extern_spec_stray` | design.md §4.19's own sqlite group, `(` open on `sqlite3_open`, stray `)` on `sqlite3_close` | `4:76 expected_params_close` alone | `4:26 unclosed_bracket`, `5:44 expected_extern_signature` (*found `)`*) |
| `x2b_member_mistake` | x2 and `junk` inside `sqlite3_close`'s parameters | `4:76 expected_params_close` alone: two of three untold | `4:26`, `5:44 expected_params_close` (`junk`), `5:49 expected_extern_signature` (*found `)`*): all three |
| `x2c_member_mistake_alone` | `junk` alone | `5:44 expected_params_close` | same bytes |

No diagnostic on any of the seven carries a fix or a note, under either
compiler (`check --json`, `"fixes": []`, `"notes": []` on every one), so
`--apply` changes nothing and what the author reads is the message alone.

**What the author would fix, and whether it is right.** On the four probes
that do not move, both compilers name every mistake and each fix is the
obvious one (close the `(`, delete `junk`, give `+` its operand), right under
both. On the three that move I applied, turn by turn, only what each message
names (`<seat>/turns/`, a script that closes a list where the message points,
before `->` for `unclosed_bracket`, the one place a `)` compiles there, deletes
a stray closer or a stray word, and fills x1's operand), until `check` exits 0:

| probe | `NEW`: turns, messages per turn | `PE`: turns | `PEL` (the narrowing below): turns |
|---|---|---|---|
| x1 | **2**, 3 then 1 (the stray `)` appears only in turn 2) | **1**, 4 | 1, 4 |
| x2 | **2**, 1 then 1 | **1**, 2 | 1, 2 |
| x2b | **2**, 1 then 2 | **1**, 3 | 1, 3 |

Every fix each compiler's messages lead to is right: there is no wrong fix in
either column, only a later one in `NEW`'s. x2's result after `PE`'s one turn
is design.md §4.19's own group, and it **builds against the real `sqlite3.h`,
links `-lsqlite3` and runs** under all three compilers, the emitted C
identical, its `_Static_assert`s over `SQLITE_OK`, `sqlite3_open` and
`sqlite3_close` included (`<seat>/turns/x2_fixed_one_turn.hero`).

**Where the stray closer's message comes from.** Before judging
`expected_extern_signature` I measured it with no opener left open above
(`<seat>/stray/`, both compilers, identical bytes on all five):

| a stray `)` after | code | message |
|---|---|---|
| a statement, `print(1))` | `expected_end_of_line` | *found `)`*, hint *one statement per line, no semicolons* |
| a function head | `expected_end_of_line` | *found `)`*, hint *a `function`'s body goes on the lines below its head* |
| **a member**, `function sin(x: f64)) -> f64` | **`expected_extern_signature`** | *expected a `function`, a `constant` or a `record`, found `)`*, hint *an `extern` group holds what the header declares, one per line* |
| a member `constant M_PI: f64)` | `expected_extern_signature` | the same |
| a field of a group's `record`, `tm_year: i32)` | `expected_end_of_line` | *after the field `tm_year`, found `)`*, hint *one field per line* |

So **`PE` does not write that message; the parser already does**, for every
stray `)` after a member, and `NEW` prints it too: one turn later (Task 3).
What `PE` changes is only whether the second mistake reaches the parser at all.

The member loop that prints it is `selfhost/parse/group.hero:312-316`: after
a complete signature it reports whatever token stands where the next member
would begin, so a stray `)` there is told as a member that is not one, where
the field's loop tells it as the end of the line (`expected_end_of_line`,
*after the field*). That is the parser's existing surface, not this rule's.

## Task 2: does anything reach the C boundary

**The census.** `git ls-tree -r --name-only 171e8c45 | grep '\.hero$'`: 1358
files, the same list `git ls-files '*.hero'` gives today; `grep -l '^extern'`
over them: **363**, every one a group head (`^extern[[:space:]]+"`, 363 of
363). `check --brief` and `check --brief --permissive` on each from its own
directory (`<seat>/census_one.sh`, `xargs -P 6`, both compilers over the same
tree `<seat>/new`):

| compiler | normal: exit 0 / 1 / 2 | permissive: exit 0 / 1 / 2 |
|---|---|---|
| `NEW` | 272 / 91 / 0 | 275 / 87 / 1 |
| `PE` | 272 / 91 / 0 | 275 / 87 / 1 |

**Outputs moved 0, exits moved 0, in both arms** (`cmp` on all 726 pairs).
The one exit 2 is `archive/bootstrap-rs/heroes/src/library/source.hero`
under `--permissive`, *internal error: a diagnostic landed inside the Heroes
library*, the same bytes under both: outside this sitting, noted below.

**The token streams**, `heroes lex --dump-tokens` on the 363: 60,552 token
lines, **0 files moved** (356 at exit 0 and 7 at exit 1 under both).

**The emitted C.** `heroes build <file> --emit-c` on the 272 that `check`
accepts, each from its own directory (`<seat>/emit_one.sh`). `--emit-c`
writes `build/` beside the source and runs clang there (`main.o`,
`clang-stderr.txt`), so each compiler got its own fresh archive
(`<seat>/emitnew`, `<seat>/emitpe`, `diff -rq` silent between them). **My
first pass ran six at a time and is discarded**: it read 11 C files and 31
stderr files differing, and reading them showed two artifacts of the run, not
of `PE`. Two programs of one directory built the shared runtime object at the
same moment (`08-ffi.hero` under `PE`: *internal error: the runtime did not
compile*, exit 2, empty C: the false red `.claude/rules/verification.md`
names), and a clang message names `build/tu-<hash>/`, whose hash differs by
compiler. Rerun with every `build/` removed, sequential within each tree:

| compiler | exit 0 | exit 1 | exit 2 |
|---|---|---|---|
| `NEW` | 203 | 69 | 0 |
| `PE` | 203 | 69 | 0 |

**0 of 272 C files differ** (`cmp`); 15 stderr files differ raw, every
differing line a `build/tu-<hash>/` path (0 lines without one), and 0 after
the hash is normalised. The 69 at exit 1 are refused alike under both: 66
by the header-verification class, an `ffi_*` or `field_lend_*` diagnostic on
the `.hero` line (22 of them `ffi_missing_header`, a header this machine
lacks), and 3 by `unsupported[...]`; by directory, 30 in
`tests/golden/fixedbugs`, 20 surface fixtures, 9 panel briefs, 8 in
`tests/golden/unsupported` and 2 in `tests/golden/ir`.

**So no header, type, width, mark, emitted line, ABI fact or verdict of
clang moves**, on any tracked program holding an `extern` group.

## Task 3: a binding of my own, to `string.h` and `math.h`

`<seat>/t3/good.hero`: `strlen`, `strcmp` and `strspn` from `string.h`, `hypot`
from `math.h` with `link "m"`, and a `main` calling each. Under both
compilers: `check` exit 0; `build --emit-c` exit 0, 272 lines, **byte-identical**
(`cmp`); `build` exit 0; the binary prints `6`, `true`, `3`, `5.0` and exits 0.

**The C that binding needs**, written by hand from what the emitter prints
(`<seat>/t3/binding.c`: the two real headers, `macros.h` holding the
emitter's own `HERO_RET_*` lines, one `_Static_assert` over each member's
return and one probe per member whose call clang checks against the header's
parameters). `clang -std=c11 -Wall -Wextra -Werror -fsyntax-only binding.c`,
Apple clang 21.0.0: **exit 0**. The same with `strlen`'s return asserted `i32`
(`binding_wrong.c`): **exit 1**, *static assertion failed ...
heroes-ffi-return strlen i32*. Through the compilers, `strlen` declared
`-> i32` (`wrong_type.hero`): `build` exit 1, *error[ffi_return_type]:
`strlen` does not return `i32`*, then *that is what `string.h` says, and
clang read it*, at 2:5, the same bytes under `PE` once the `tu-` hash is
normalised. So the property design.md §1.11 (point 3) and §4.19 guard, *a
wrong FFI signature is a compile error*, holds under `PE` on this binding.

**The broken binding**: `strlen`'s `(` left open on line 2 and one `)` too
many below, in three placements (normal and `--permissive` identical on every
run):

| file | the stray `)` | `NEW` | `PE` |
|---|---|---|---|
| `t3a_next_member` | on the next member, `strcmp` | `2:34 expected_params_close` (*found `->`*) alone | `2:20 unclosed_bracket`, `3:48 expected_extern_signature` (*found `)`*) |
| `t3b_two_below` | on `strspn`, two members down; `strcmp` between is right | `2:34` alone: `strcmp` and `strspn` read inside `strlen`'s parameters | `2:20`, `4:53 expected_extern_signature` |
| `t3c_next_group` | on `hypot`, in the next group | `2:20 unclosed_bracket`, `7:35 expected_extern_signature` | same bytes |

So `NEW` tells one pair of mistakes two ways, by whether the stray closer sits
in the same group or the next; `PE` tells all three t3c's way.

**The turns** (design.md §4.17, *the model fixes it in one turn*), applying to
the text what each message says and nothing it does not:

- `NEW` on t3a and t3b: turn 1 closes `strlen`'s list where the message points,
  before `->`; `check` then prints `3:48` (t3a) or `4:53` (t3b)
  `expected_extern_signature`, *found `)`*, **the same bytes `PE` printed in
  its turn 1**; turn 2 deletes it and `check` exits 0. **Two turns.**
- `PE` on t3a: turn 1 applies both. With the `)` before `->` the file is
  `good.hero` byte for byte (`cmp`), `check` exit 0 under both, and it builds
  and runs as above. **One turn.** `unclosed_bracket` does not say where the
  `)` goes; placed at the end of the line instead, `strlen(s: cstr lent ->
  u64)`, it is refused under both with `2:34 expected_params_close` (*found
  `->`*): a wrong placement costs a turn and never becomes a binding.
- The wrong fix `expected_extern_signature` could invite, reading *expected a
  `function`...* as "the member ended at the first `)`" and deleting `) -> i32`
  whole: refused under both. Where the result is used, by the checker
  (`bad_operand`, *`<` ... found `()`*); where it is not, `check` exits 0 and
  **`build` refuses it**, *error[ffi_return_type]: `strcmp` does not return
  `()`*, clang reading `string.h`. No message here led to a binding that
  compiles and is wrong, because §4.19's assertion stands behind every one.

**So, to the brief's question on x1**: the message a binding's author reads
for the stray `)` is the parser's, the same bytes under both compilers, and
`PE` moves it from the second turn to the first. It is not the best message a
stray `)` gets (it names what may begin a line where the mistake ends one, and
the field beside it gets the better one), but its caret is on the `)`, it says
*found `)`*, deleting either `)` of `))` gives the same text, and it sent none
of my fixes wrong. It does not send the author to the wrong fix; it is a
wording residual of `group.hero`, which I record and do not stand on.

## The strongest case against `PE` at the boundary, and a narrowing that removes it

`PE` can misfire in a binding only on a line inside a member's brackets that
opens, at the member's margin or shallower, with a word that ends a reach. In
a binding such a line holds parameters, whose names the author often copies
from the header. `function` is safe (it ends a reach only before a name, and
a parameter name stands before `:`); `match`, `use`, `test`, `record`,
`constant`, `variant` and `assert` end one whatever follows, and all seven are
legal C names.

**How often real headers name a parameter so**, grepped with comment lines,
licences and strings removed (`<seat>`, the command in the transcript):
`sqlite3.h`, `curl/curl.h`, `zlib.h`, `raylib.h`, `pcre2.h`, `SDL3/*.h`,
`string.h`, `math.h`, `stdio.h`, `stdlib.h`, `regex.h`: **0**. The whole SDK
`usr/include`: **4**, in three headers, `libxslt/keys.h:32-33`
(`xsltAddKey`'s `match` and `use`, one parameter per line in the header),
`net-snmp/library/parse.h:235` and `apache2/ap_regex.h:279` (`match`). Every
other hit is prose.

**`xsltAddKey` bound with its C names copied** (`<seat>/kw/`), a mistake under
both compilers since both words are reserved, in four layouts:

| layout | `NEW` | `PE` |
|---|---|---|
| `L1` one line | `2:64 expected_parameter` (*found `match`*) | same bytes |
| `L2` continuation lines at column 8 | `3:9 expected_parameter` | same bytes |
| `L3` continuation lines at column 4, the member's own | `3:5 expected_parameter` | **`2:24 unclosed_bracket`, false: line 4 closes that `(`**, then `3:5 expected_parameter`, `3:5 expected_extern_signature` (*found `match`*), `4:5 expected_extern_signature` (*found `use`*) |
| `L4` aligned under the first parameter, as the header writes it | `5:25 expected_parameter` | same bytes |

`heroes fmt` joins a broken member signature onto one line (`fmt_col4.hero`,
`fmt_ragged.hero`, both compilers, `fmt` output identical), so L3 is not a
layout the tool writes; the 17 multi-line members of the tracked tree (all
surface fixtures, 14 files) print the same bytes under both. On L3 the turns
are mixed rather than worse: `NEW` tells `match` in turn 1 and `use` in turn 2;
`PE` tells both in turn 1 (the second under the wrong code) beside the false
*never closed*. Renaming both gives exit 0 under both; obeying the false
message too (a `)` added on line 2) costs `PE` a turn with four new messages,
refused, never a binding.

**The narrowing, `PEL`, prototyped** (`<seat>/pel/`, a third archive with
`PE`'s patch and 44 more diff lines in `next_line.hero`,
`<seat>/pel-over-pe.patch`; canonical under `heroes fmt`; built by `NEW`,
exit 0, `real 59.55`, `user 51.81`): **a keyword directly before one `:` (past
spaces, and not `::`) is a label, and a label ends no reach.** No declaration
or statement opens with `<keyword>:`; a parameter and a named argument do.
`else` is decided before the label test, so `else:` keeps `PE`'s rule. On
every probe of this report `PEL` prints `PE`'s bytes, x1, x2, x2b, t3a and t3b
included, **except L3, where it prints `NEW`'s one true message**,
`3:5 expected_parameter`, and nothing else.

**`PEL`'s gates, all run** (the census over the tree `<seat>/new`; the rest
in `<seat>/pel`, a full archive, or its own fresh tree `<seat>/emitpel`):

| gate | `NEW` | `PEL` | moved |
|---|---|---|---|
| `check --brief`, the 363 `extern` files, normal; `--permissive` | 272 / 91; 275 / 87 / 1 | the same | 0 outputs, 0 exits |
| the same over **all 1358 tracked files** | 651 / 707; 729 / 628 / 1 | the same | 0 outputs, 0 exits, both arms |
| `lex --dump-tokens`, all 1358 | | | **0 of 1358** |
| `build --emit-c`, the 272, sequential | 203 / 69 | 203 / 69 | **0 C files of 272**; stderr 0 once the `tu-` hash is normalised |
| the compiler's own tests, `heroes-pel test selfhost/main.hero` | | 911 tests, all passed | |
| the harness's `check` form, `heroes-pel run tests/harness/main.hero -- ./heroes-pel check` | | 284 passed, 0 failed | |
| §4.19's ladder built and run: gallery `08-ffi` (libm; it declares `puts`, which it never calls, so only the assertion checks that one), `nbody`, `sqlite`, `curl`, `raylib`, `ctime`, `sdl` | build 0, run 0 each | the same stdout byte for byte, and `PE`'s too | 0 |
| my binding: `good.hero` emitted, built, run; `wrong_type.hero` built | 272 lines; `6 true 3 5.0`; `ffi_return_type` | the same C (`cmp`), output and refusal | 0 |

**Its cost**: `next_line.hero` 214 to 225 lines (four of the eleven comment),
`layout.hero` 202, in the unit `suite_layout` counts (my count reproduces the
engineer's 207, 214, 201 and 202 on the files he measured); the ceiling is
300. **Its speed, not a duration**: `check selfhost/main.hero`, `PE` against
`PEL` (both built by `heroes build`), six alternations with the order reversed
in half: `user` medians 5.22 and 5.25 s once one pair whose ratio shows waiting
is discarded (`real 8.23`, `user 6.74`), each compiler varying about ±10%
across its own runs, load 5 to 7 with a VM at 99% beside, so no difference
shows above the noise and a still-machine timing is owed where it lands. My
first six alternations were `PEL` only, a zsh loop over an unsplit `$order`,
and are discarded. `NEW` against `PEL` read 4.965 and 5.025 s, but `NEW` is the
seed built by bare clang (6,985,064 bytes) and `PEL` came out of `heroes build`
(9,241,568), so that pair differs in more than the patch; the engineer's `NEW`
against `PE` timing has the same confound.

The whole-SDK count of parameter names, finished: OpenSSL 3 and 4 (142 and
144 headers), ncurses (Homebrew's and the SDK's) and Homebrew's SQLite hold
**0** parameters named with the seven words as well. libsodium is not on this
machine, and neither is cJSON, so both are unmeasured.

## The shapes beside, run

`<seat>/beside/`, all three compilers, `--brief`, the permissive arm identical
on every run:

| shape | `NEW` | `PE` | `PEL` |
|---|---|---|---|
| B, a callback's function type left open, `body: (function(i64) -> i64, arg: i64) -> i64` | 4 messages | same bytes | same bytes |
| C, raylib's `params: i32[4` above a member, `junk` on the member below | the opener, `6:5 expected_array_length`, `7:5 expected_field` (*found `function`*: the member read as a field); `junk` untold | the opener, `6:1 expected_array_length` (the same cascade of the one `]`, the engineer's residual), `7:56` the true `junk` | = `PE` |
| D, x2 with a `constant` and a `record` member between the two mistakes | one, *found `->`* | the opener and the stray `)` | = `PE` |
| F, a `partial` record with two fields between, `junk` in a field | the opener, **`3:5 empty_record`, false**, and the two fields read as members; `junk` untold | the opener, `5:21` the true `junk` | = `PE` |
| G, F with a stray `)` after a field instead | one, *found `->`* | the opener and the stray `)`, *expected the end of the line after the field* | = `PE` |
| E, the `xsltAddKey` names at column 0 | **the landed rule's own false *never closed*** (`use` at column 0 ends a reach whatever follows it), then *expected the module's name after `use`, found `:`*: 4 messages | 4, the false one kept | **one**, the true `3:1 expected_parameter` |
| A, the label in a body's call: `show(pattern: 1,` then `match: 2)` at the statement's margin, `print(1 +)` below | `6:10`, `7:5 missing_match_arms` (a cascade); `7:14` untold | **a false *never closed* at 5:9**, `6:10`, `7:5`, and `7:14` true | = `NEW` |

**F is the case for the rule at the boundary.** `NEW`'s `empty_record` is false,
since the record has two fields and the reach laid them out inside the bracket,
and its advice, *give it its fields, or drop `partial` and name the type for a
HANDLE*, followed literally (`record Broken tag tm`, no fields;
`F_followed_advice.hero`) **checks, builds and runs** under both compilers:
a false message leading to a binding that compiles and has lost what its author
declared. `PE` never prints it. (My own intended F binding was wrong as well,
`mktime(t: Broken)` by value against `struct tm *`, and clang refused it,
`ffi_parameter_type`: §4.19 working. `mktime(@t: Broken)` checks, builds and
prints `1.0` under all three.)

**E says the label test repairs the landed rule, not only `PE`**, and **A is
`PEL`'s one trade**: in a body, `PE`'s reach end at the label gives the parser
a line to resynchronise on, so it tells `7:14` beside its false message, where
`PEL`, like `NEW`, lets the parser's recovery from `match:` inside a call take
line 7. That is a parser recovery (`match` read as an expression among a
call's arguments), not a word the reach should end at, and it is off the
boundary.

## Outside the sitting, found on the way

- `archive/bootstrap-rs/heroes/src/library/source.hero` under `check
  --permissive` exits **2**, *internal error: a diagnostic landed inside the
  Heroes library, at its line 112: [contract_differs] `hero_file_read` is
  declared here and at source.hero:111, and the two disagree about the marks
  on `path`*, under `NEW`, `PE` and `PEL` alike: a user file that redeclares a
  library `extern` with other marks is told as the compiler's fault. The file
  is archived and nothing builds it (CLAUDE.md § 5). I searched
  `docs/work/DEFECTS.md` for *landed inside the Heroes library*,
  `bootstrap-rs/heroes/src/library` and `source.hero` and found no item for
  it; the message's class is panel 171's, where the library's own `extern`s
  disagreed. Whether this is a defect is a question for the coordinator, not a
  premise.
- `selfhost/parse/group.hero:312-316`'s wording for a stray token after a
  complete member (above), and `check` exiting 0 on a member declared with no
  result over a C function that returns one, which only `build` refuses
  (Task 3). Both pre-exist this rule; the second is §4.19's own design, clang
  being the verifier and running at build.

## The verdict, in the seat's form

- `verdict`: **approve**. The `extern` half (a member line ends the reach of
  a bracket a member above left open) is right for a binding's author, and
  nothing reaches the C boundary. I add one amendment, which I recommend on
  measurement and do not make a condition: **a keyword directly before `:` is
  a label, and a label ends no reach** (`PEL`). It repairs the landed rule as
  well. Not a veto: no layout, width, mark, header, emitted line or clang
  verdict moves under `PE` or `PEL`.
- `section`: design.md §1.11 (*"FFI ergonomics rank alongside comprehension,
  not below it"*, and its point 3, *"a wrong type in an `extern` is a
  compile error"*), §4.19 (the `importc` mechanism and the `_Static_assert`
  that makes it true), §4.17 (*"The model fixes it in one turn"*), §4.15 line
  1944, the sentence the rule amends. design.md has no sentence that a
  diagnostic must not assert something false of the program; I say so rather
  than invent it, and my case against `E` and `L3` rests on the turn it costs
  and not on such a rule.
- `experiment`: `<seat>/t3/binding.c`, the C my `string.h` and `math.h`
  binding needs, written by hand from the emitter's own macros (one
  `_Static_assert` over each of `strlen`, `strcmp`, `strspn`, `hypot`, one
  probe per member): `clang -std=c11 -Wall -Wextra -Werror -fsyntax-only`
  **accepted it, exit 0**, and **refused it, exit 1**, with `strlen` asserted
  `i32`. The same binding in Heroes emits byte-identical C under `NEW`, `PE`
  and `PEL`, builds, and prints `6 true 3 5.0`; declared `-> i32` it is
  `ffi_return_type` under all three. Broken (a `(` left open, a stray `)`
  below), it reaches exit 0 in two turns under `NEW` and one under `PE`. Over
  the tree: 363 `extern` files, and all 1358 for `PEL`, 0 outputs, exits or
  token streams moved; 272 emitted C files, 0 moved; seven ladder programs,
  the same stdout.
- `argument`: It moves nothing a binding compiles to: 0 of 363 `extern`
  files' diagnostics or tokens, 0 of 272 emitted C files, seven ladder
  programs unchanged, and clang still refuses a wrong `strlen`. It moves what
  a refused group says, for the better: my `string.h` binding with a `(`
  unclosed and a stray `)` below reaches exit 0 in one turn, not two, and a
  fielded `partial` record loses `NEW`'s false `empty_record`, whose advice,
  followed, builds a binding without its fields. Its one false message,
  *never closed* over a C parameter named `match` or `use` at the member's
  margin, the landed rule shares; it is rare (4 SDK parameters, none in the
  §1.11 libraries I could grep), and `PEL` removes it.
- `prediction`: at the batch gate that lands the rule, SQLite's rung of
  §4.19's ladder needs nothing: `examples/sqlite/main.hero` emits C
  byte-identical to `171e8c45`'s and prints the two lines of its
  `main.expected`, `rows: 3` and `longest: 6`; and §4.19's
  own sqlite group with `sqlite3_open`'s `)` left out and one `)` too many on
  `sqlite3_close` (`x2_extern_spec_stray.hero`) draws exactly two diagnostics
  in one `check`, `unclosed_bracket` at 4:26 and the stray `)` at 5:44, where
  `171e8c45` draws one; fixed as those two say, it builds against
  `sqlite3.h` and links `-lsqlite3` at exit 0. If the label test lands with
  it, `xsltAddKey` bound with its C names on lines at column 4
  (`L3_col4.hero`) and at column 0 (`E_label_col0.hero`) draws exactly one
  diagnostic each, `expected_parameter` at 3:5 and 3:1.
- `condition`: **veto** if the landing's census moves the tokens, the emitted
  C or the clang verdict of any tracked program holding an `extern` group, or
  refuses a binding that compiles today. **Object** (the label test a
  condition rather than a recommendation) if a header of a library in
  §1.11's table names a parameter `match`, `use`, `test`, `record`,
  `constant`, `variant` or `assert` (libsodium and cJSON are not on this
  machine, so I could not grep them), because L3 is then a shape real bindings reach. **Object** as well if
  the landing keeps column 0 alone and drops the member line: x1, x2, x2b and
  t3 then stay at two turns, and F keeps a false message whose advice builds
  a binding that has lost its fields. The engineer's own condition, *drop the
  `extern` half if the ffi-pragmatist judges x1's and x2's messages worse
  than `NEW`'s single `expected_params_close`*: I judge them better, one turn
  against two, with no wrong fix in either.
