<!-- Copied by the coordinator from the seat's REPORT.md in its own directory, 2026-09-24,
unchanged below this comment. The prototypes it names are summarised as diffs in
`compiler-engineer-work/`. -->

# Panel 178, compiler-engineer: the ceiling (design.md §1.1, §1.7, Part 5)

Seat directory: `<scratchpad>/178-compiler-engineer/`, a `git archive` of HEAD `57679005`. Every number below
was produced in this directory in this session, with the command named beside it; "unrun" marks the rest.
The live compiler measured is `selfhost/` (Heroes); `archive/bootstrap-rs/` was not opened.

## Verdict, one row per route

| route | verdict | design.md § | cost, in the unit measured | prediction | what changes the verdict |
|---|---|---|---|---|---|
| **R1** `rest: zero` | **object** | §1.1 (the ceiling, as `tests/harness/suite_layout.hero` DECIDED measures it); §1.7 / Part 5 passed (it is sugar) | built: **+435 / -27** selfhost lines (`git diff --numstat`), **+351** layout-unit lines; layout suite **red on 3 DECIDED rows**: `ast.hero` 531/527, `check/walk.hero` 1921/1870, `print/fmt.hero` 1188/1175 | landing needs >= 380 insertions and moves those 3 rows (or splits walk.hero) | the landing names the 3 rows in advance and moves `construct_record` out of walk.hero; or a census program `partial` + A cannot build is run |
| **R0** `T.zero()` | **approve as `T(rest: zero)`**, object to a second spelling | §4.15 (one spelling), §1.7 | built as R1 with no field named: **0 lines** over R1 (`uname_r1.hero` prints `Darwin`, `Linux` on both legs); `Utsname.zero()` today is `error[unknown_function]`, a separate spelling needs a resolver rule: unrun | none separate | a program `T(rest: zero)` cannot express |
| **R2** omitted fields zero, no mark | **object** | §4.9, §1.7 | not built; R1 minus parser, formatter and AST (about -57 layout lines of R1's 351, an inference); deletes the only guard, `missing_fields`, which **no golden, annotation or unit test exercises at HEAD** (grep below) | a landing of R2 stays green on every suite while removing the check | a golden for `missing_fields` on a group record exists first, and a measured thesis effect |
| **A** `[x; N]` | **approve** | §1.7 / Part 5 (erased in lowering), §1.1 | built: **+96 / -22** in 12 files, **+58** layout lines; layout red on 3 rows by **7** in total (`ast` 529, `ir/flatten` 1153, `print/fmt` 1177); **1 of 520** programs moves (`tests/golden/check/unterminated.hero`: `unexpected_character` becomes `expected_end_of_line`, because `;` becomes a token) | landing <= 120 insertions, one check golden re-annotated | the census route "a field whose length is the header's" is adopted: A restates N, R1 does not |
| **Z1** `zero` on the record | **object** | §1.1, §1.12 | built over R1: **+46 / -9**, **+31** layout lines, a contextual word, a diagnostic class; the claim is **unchecked**: a false claim on Darwin's mutex compiles and `pthread_mutex_lock` returns **22** at exit 0; and `partial` **at HEAD** already builds that zero mutex with no claim (22, exit 0) | Z1 lands and the Darwin zero mutex stays reachable at exit 0 | the compiler checks the claim, or Z1 also gates `partial` (6 golden/example files construct a partial record at HEAD) |
| **Z2** every group record admits zero | **approve** | §1.1 | **0 lines**: it is what `partial` does at HEAD (C11 designated initialiser) | none | a group-record zero that crashes where an explicit `nullptr`/`0` would not |
| **T** `s.to_fixed()` | **object as spelled**; approve a place form `s.copy_into(@a.sun_path)` | §1.7 / Part 5; §3.1 and panel 062 (a `T[N]` has no C storage) | not built; as a value it needs `T[N]` storage inside `T[N]?`, reversing `selfhost/emit/ctype.hero:380` (`"C has no assignable array"`), plus 2 new `storageless` producers and an element-wise initialiser; the three files it must touch sit at **0 headroom** (ctype 395/395, gate 365/365, check/builtins 378/378); reference class, its mirror `validated_bytes`: **+221 selfhost, +93 runtime** | the value form lands and moves ctype.hero:380 | built without touching ctype.hero:380 or gate.hero |
| **L** defect 091 as a lowering | **approve** | §1.12; spec § 5 line 121 | built: **+27 / -6** in `selfhost/emit/container.hero` only, 278 -> **296** layout lines (under §11's 300); `elem_min` 134 -> 0 (`72`), `sunpath_*_ascii` 134 -> 0, on **all three legs** | none | none |
| **S** status quo | **object alone**, approve S + L | §1.12 | 0 lines, and defect 091 stays `check` 0 / run 134 | none | none |
| **D** a zero default for every type | **VETO** | §1.12, §4.13, §4.9, Part 5 | not built; an all-zero `HeroStr` is the runtime's non-value (`hero_str_len` aborts, **exit 134**, measured), and a function value (§4.13: only a named top-level function) has no zero; D needs a fifth per-type descriptor | none | D restricted to types whose all-zero bytes are a runtime value, which is Z2 and not D |

## The structured answer

- `verdict`: **object** to the composition R1 + Z1 + T as proposed; **approve** L and A; **veto** D.
- `section`: §1.7 and Part 5 (core versus sugar), §1.1 (the ceiling, in the unit `suite_layout` counts).
- `implementation_cost`: R1 +435/-27 lines in 10 selfhost files, 2 of them new (`check/rest.hero` 123, `ir/zeros.hero` 92): parser `grammar_expr.hero` +43/-8, AST +4, checker `check/walk.hero` +71/-5 and `check/rest.hero`, diagnostics `data_errors.hero` +45 (five new codes counting the parser's `rest_not_last`, and `missing_label` reused), lowering `ir/flatten.hero` +18/-4 and `ir/zeros.hero`, backend `emit/storageless.hero` +4, formatter `print/fmt.hero` +25/-8, dump `print/bodies.hero` +10/-2. Z1 +46/-9 more. A +96/-22. L +27/-6. HEAD is 46677 layout-unit lines in 229 files (65328 by `wc -l`).
- `needed_for_self_hosting`: **no**. The compiler's own longest fixed array is `u8[8]` (`grep -rhoE ': [iuf][0-9]+\[[0-9]+\]' selfhost`); L is owed to §1.12, not to Principle 0.
- `argument` (110 words): R1 is sugar and it is sound: `selfhost/ir/zeros.hero` erases it in lowering, every refusal exits 1, none of 520 programs changes its `check` outcome, and run 146/0, corpus 55/0, own tests 676/0 with my build; its probes pass on three legs. It costs +435/-27 lines, 351 in the layout unit, over three DECIDED ceilings by 68, six times A's 58, for the one thing `partial` plus A cannot do: zero declared fields left unwritten. Z1 buys no check: a false `zero` claim compiles to Darwin's EINVAL 22 at exit 0, and `partial` already reaches it with no claim. T as spelled is not sugar: it gives `T[N]` storage, reversing `emit/ctype.hero:380`.
- `prediction`: if R1 (with or without Z1) lands at the M-buildable-structs close, `git diff --stat -- selfhost` for it reads **at least 380 insertions**, and `suite_layout` is green only because the DECIDED rows of `selfhost/ast.hero`, `selfhost/check/walk.hero` and `selfhost/print/fmt.hero` moved, or walk.hero was split (measured here: 531/527, 1921/1870, 1188/1175).
- `condition`: R1 becomes approve when the landing commits to its three rows before building (the panel 160 / 171 shape in `suite_layout.hero`'s own history) and moves the construction check into its own module, or when a program from the census that needs `==` or a map key (so not `partial`) and whose long arrays make naming every field the cost is run and `partial` + A is shown worse. Z1 becomes approve when the compiler checks the claim or Z1 also gates `partial`. T becomes approve when the value form is built without moving ctype.hero:380.

## What was measured, and how

### 1. Defect 091 repaired as a lowering (route L)

The repair is in `selfhost/emit/container.hero` `write_element`: an index step on a `T[N]` is a guarded
subscript on the lvalue reached so far, no unshare, and the read and the write now share one spelling
(`checked_subscript`, factored out of `read_element`). The emitted store, `elem_min.hero`:

```
h0_s.name[((uint64_t)(t7) >= UINT64_C(4) ? (hero_panic("index out of range for a fixed array"), (int64_t)0) : (t7))] = t8;
```

| program | HEAD (seed) | repaired | legs |
|---|---|---|---|
| `elem_min.hero` | run 134, `hero_unreachable` | run 0, `72` | Darwin arm64, Linux arm64, Linux x86-64 |
| `sunpath_darwin_ascii.hero` / `sunpath_linux_ascii.hero` | 134 | 0, `/tmp/heroes-178.sock` | all three |
| `shapes091.hero`: a fixed field of a nested record, a field of a record inside `Pt[3]`, a whole element of `Pt[3]`, read-modify-write, through an `@` parameter, inside a `[Slot]` element with a copy sharing it | 134 | 0, `66 68 9 5 70 0` (the copy's `0` shows copy-on-write held) | all three |
| `oob091.hero`, index 4 and -1 into `i8[4]` | not run at HEAD | 134, `panic: index out of range for a fixed array` | Darwin |
| `shapes091.hero`, `sunpath_darwin_ascii.hero` under `--sanitize` | | 0, clean | Darwin |
| `sunpath_darwin_utf8.hero` | not run at HEAD | 134, `does_not_fit`: the program's own `.to_i8().must()` on a byte >= 0x80 | Darwin |
| `sunpath_utf8_l.hero`, the byte folded by arithmetic `(b - 256).to_i8()` | | 0, `/tmp/hèroes-178.sock` | Darwin |

So L writes every byte, a non-ASCII one through two lines of arithmetic, because there is no wrapping
conversion (spec lines 304-306).

### 2. R1 prototyped (`rest: zero`)

Where it landed: the parser strips the words into a `rest: token.Span?` on the `.call` and `.method` nodes
(one construction site each, so no exhaustive match moved; a new expression kind would have touched the
50 exhaustive `ExprKind` matches, counted by `grep -rn '\.try_expr' selfhost | grep -c '\.match_expr'`).
The checker's `check/rest.hero` maps each written label to a declared field in order; `ir/zeros.hero`
lowers every field left out to a zero of its own type: `0`, `0.0`, `false`, `nullptr`, a handle's `nullptr`,
a nested record of zeros, and for a `T[N]` an **empty** `construct array` of that type, which
`emit/storageless.hero` writes `{0}` (a literal of `T[N]` always has N elements, so the empty one means
nothing else). After lowering the IR holds only Part 5's existing construction, which is what makes it sugar:

```
$t1: i8[256] = construct array()
$t2: Utsname = construct Utsname($t1)
```
```
t2 = (struct utsname){.sysname = {0}};                                    (Darwin, and Linux with [65])
t3 = (struct sockaddr_un){.sun_family = t1, .sun_path = {0}};
```

- Every field kind a group record may hold (`kinds.hero`: `i8`, `f64`, `bool`, `ptr`, `cstr`, a nested
  record, `Pt[3]`, `f32[2]`, a handle, `i64`): run 0 on all three legs, all zero; no clang warning on
  Darwin (Linux warnings not read).
- Refusals, all `check` exit 1 on all three legs: a Heroes record (`rest_outside_a_group`), a function call
  and a UFCS call (`rest_not_a_construction`), a variant case (the same, from the parser), fields out of
  order, an unlabelled value, an unknown label, a repeated one (`field_out_of_order`, `missing_label`),
  `rest: zero` not last (`rest_not_last`), and a record with a field called `rest` (`rest_is_a_field`, the
  ambiguity the `name: value` spelling creates).
- `missing_fields` still fires on a group record and on a Heroes record without the words (`missing.hero`).
- A module-qualified construction `geom.Pt(x: 7, rest: zero)` works; `geom.origin_y(rest: zero)` is refused.
- `heroes fmt` is a fixpoint on single-line and broken constructions; `parse --dump-ast` prints the words.
- Suites, `./net ./heroes-r1 <suite>` (the net built by the seed): check 135/0, ir 24/0, emit 8/0,
  unsupported 15/0, **run 146/0**, **corpus 55/0**; `./heroes-r1 test selfhost/main.hero` **676 tests, all
  passed**; `./net ./heroes-r1 layout` **1 failed** (the three rows in the verdict table).

### 3. Padding, and how R1 zeroes it

R1 emits the compound literal the emitter already writes; it adds no `memset`. Measured (`pad/shapes.c`,
the destination in another translation unit pre-filled with 0xAA, so `-O2` cannot see it):

| shape | control: members stored, no initialiser | today's `t = (T){..}; h0 = t;` | R1's `(T){.i = i}` | `(T){0}` then stores | memset then stores |
|---|---|---|---|---|---|
| Darwin arm64 `-O1` `-O2` `-O3` | **3 of 3** 0xAA left | 0 | 0 | 0 | 0 |
| Linux arm64 and x86-64 `-O2` | **3 of 3** | 0 | 0 | 0 | 0 |

The control is positive at `-O1`, `-O2` and `-O3` on Darwin and at `-O2` on both Linux legs (Linux `-O1` and `-O3` unrun; `-O0` reads 0 everywhere, so `-O0` measures nothing here), so unlike the brief's stack method this one measures at `-O2`.
The mechanism, read from Apple clang 21.0.0's IR (`clang -O0 -S -emit-llvm pad/shapes.c`): a compound
literal writes its padding with an explicit `llvm.memset(..., 0, 3)`, and a struct copy is `llvm.memcpy` of
all 8 bytes. Through the whole pipeline (`w/padh/pad.hero`, an `addrinfo` built with `rest: zero`, 4 padding
bytes read by C after a dirtied stack): 0 of 4 non-zero at `-O0` and `-O2` on all three legs, and the same
for today's `partial` construction. **This is clang's behaviour, not a promise I verified in C11's text**:
no copy of the standard was read in this session, so whether the spec's "and so is every byte between
fields" rests on C or on clang is a question, not a premise. Making it independent of clang would be a
`memset` per construction and a `memcpy` per copy (unrun).

### 4. Route A prototyped (`[x; N]`), to price the alternative that keeps §4.9 whole

Lexer: a `;` token (`token.hero`, `scan.hero`, and its three exhaustive `TokenKind` matches in
`describe.hero`, `layout.hero`, `grammar_expr.hero`). Parser: `[x; N]` with N an integer literal. Checker:
N against the fixed length (`fixed_array_length`), `repeat_outside_a_fixed` for a `[T]` or no expectation.
Lowering: x once, its value id N times. Emission unchanged (`{t1, t1, ...}`). `uname_a.hero`
(`Utsname(sysname: [0; 256])`) prints `Darwin`; `[Pt(x: 3, y: 4); 3]` and `[1.5; 2]` work; refusals exit 1;
`heroes fmt` a fixpoint; own tests 676 passed. It moves `tests/golden/check/unterminated.hero`
(`./net ./heroes-a check`: 134 passed, 1 failed), whose `f = 2;` now reads `expected_end_of_line: ... one
statement per line, no semicolons` instead of `unexpected_character`. `run` and `corpus` for A: **unrun**.

### 5. Z1 prototyped over R1

`zero` as a contextual word after `partial` (`parse/tails.hero`, `keywords.hero`, `ast.hero`, `fmt`, `dump`,
five `record_decl` constructions in `handles.hero` and `resolve/types.hero`), refused by `rest_unclaimed`.
Claimed `uname`: 0, `Darwin`; unclaimed: exit 1. The `grammar` suite caught the constant out of declaration
order until moved (9/0 after). **What the claim does not do**, both run on Darwin:

- `w/z/mutex_claimed.hero`, `record Mutex tag _opaque_pthread_mutex_t partial zero` plus `Mutex(rest: zero)`:
  exit 0, prints **22** (EINVAL). The compiler takes the claim on trust.
- `w/mutex_partial.hero` **at HEAD, with the seed compiler**: `Mutex(__sig: 0)` on a `partial` record, no
  claim anywhere: exit 0, prints **22**. The silent zero Z1 guards already exists wherever `partial` does.

### 6. Which of the 520 programs move

`find tests/golden examples -name '*.hero' | wc -l` reads **520** (134 check, 7 emit, 36 fixedbugs, 23 ir,
146 run, 40 surface-fixtures, 14 unsupported, 120 examples). `movecheck.sh` runs `check` on every one and
records the exit code and every diagnostic code; diffed against a baseline compiler built the same way
(`heroes-base`): **L 0, R1 + L 0, R1 + L + Z1 0, A 1** (`unterminated.hero`).

### 7. `check` time

`/usr/bin/time -p ./<compiler> check base-sh/selfhost/main.hero`, five rounds interleaved, all four
compilers built by the same command (`./heroes-o2 build <tree>/main.hero -o <name>`). Medians of `real`:
baseline **19.80 s**, R1 + L **19.79**, A **19.77**, R1 + L + Z1 **19.79**; R1 checking its own larger tree
19.84. `real` is within 1% of `user + sys` in every run, so none was waiting, but the machine was **not
still**: other sessions held the load average at 2.3 to 3.2 (`uptime` before and after). No change is
measurable at that noise; the spread across rounds is 0.5 s.

## Found while measuring

1. **A group `constant` whose header value is a struct initialiser is `check` 0 and then `internal error:
   compiling the generated C failed`, exit 2.** `w/cinit/cinit.hero` + `cinit.h` (`#define PT_INIT {1, 2}`,
   `constant PT_INIT: Pt`), with the seed compiler at HEAD: Darwin and Linux arm64 both (x86-64 unrun). The
   emitter writes `return PT_INIT;` and a probe `__typeof__(PT_INIT)` at file scope; a compound literal
   `(struct pt)PT_INIT` would be C. This matters to the sitting beyond being a defect: **the header's own
   initialiser (`PTHREAD_MUTEX_INITIALIZER`) is the route to "who says a value is valid" that is not on the
   list**, because it lets C say it rather than the binding's author (Z1) or nobody (Z2). The same program
   with Darwin's mutex, `w/mutex_init.hero`, fails the same way. Not filed by this seat: the number is the
   coordinator's to agree.
2. **`missing_fields` is unguarded.** No file under `tests/` or `examples/` names it, and no `test` block in
   `selfhost/` asserts it (`grep -rln missing_fields` returns only its definition, its call site, the seed,
   the archive and the records). §4.9's "no default values" is enforced by a diagnostic the net would not
   miss if it vanished: R2, or an R1 that leaked into Heroes records, would stay green. Whatever lands, a
   golden for it on a Heroes record and on a group record should land first.
3. **R1 composes with the census's unpriced route and A does not.** `rest: zero` never states N (the `T[N]`
   zero is `{0}`), so a field whose length is the header's would make one `Utsname(rest: zero)` binding
   portable; `[0; N]` restates the N the platform decides. Cost of the header-length field: unrun.

## Not done, and why

- **T not built.** Its value form needs `T[N]` storage inside an option (reversing `emit/ctype.hero:380`),
  a `.payload` producer and an element-wise initialiser in `storageless`, and refusals in `gate.hero`; ctype,
  gate and `check/builtins.hero` are each at 0 headroom, and the question the brief put, whether it escapes
  panel 163's veto, is answered by that structure: it escapes "C cannot return an array" (a struct holding
  one can be returned) and does not escape the storage model. The place form needs no fixed value to exist
  and was not built either (time went to R1, A, Z1 and three legs).
- **R2 and a separate `T.zero()` not built**; their costs above are inferences from R1's parts.
- **`run` and `corpus` for A and Z1 unrun** (only `check` over all 520, the `check` golden suite, `grammar`
  for Z1, and own tests for A).
- **Windows unrun** for everything.
- **No spec-token cost**: that is the spec-warden's instrument.

## Files

- Prototypes: `selfhost/` (R1 + L), `proto-a/selfhost/` (A alone), `proto-z/selfhost/` (R1 + L + Z1),
  `base-sh/selfhost/` (HEAD). Compilers: `heroes-base`, `heroes-091` (L), `heroes-r1`, `heroes-a`, `heroes-z`.
- Probes: `w/` (`uname_r1*.hero`, `sunpath_r1*.hero`, `kinds.hero`, `shapes091.hero`, `oob091.hero`,
  `ref/*.hero`, `a/*.hero`, `z/*.hero`, `padh/`, `cinit/`, `mutex_partial.hero`, `mutex_init.hero`,
  `linux-leg.sh`, `linux-pad.sh`), `pad/shapes.c` + `pad/main2.c`, `dz/zstr.c`, `movecheck.sh`,
  `codelines.sh` (the layout suite's unit), `suites-r1.log`, `owntests.log`, `timing.txt`, `mv-*.txt`,
  `w/linux-x86.log`, `w/linux-x86-pad.log`.
