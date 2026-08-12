# 001 — The post-M8a sweep: twenty defects, and the three shapes they came in

Date: 2026-08-12, after panels 033 and 034. **Nothing here was found by reading.**

**Status: all twenty fixed**, each with a case named after it. No golden moved
and no expectation changed except one sentence that M8a had made false
(`declared_twice` said *"one file is one program"*; spec line 6 says one file is
one **module**). 427 crate tests (was 414 at M8a close), 37 surface, 13 golden
harnesses, clippy clean, spec unmoved at 2434, `heroes mutate` unchanged at
1173 mutants and 93% / 78%.

## Why this file exists

Panels 033 and 034 were convened about the *language* and each found a defect in
code it was only reading as background — the second and third time in a row. That
pattern was the author's instruction to stop and look properly: *"fai delle
analisi ricorsive per scovare altri errori prima di andare avanti."*

So the three defects those panels found were **generalised into hypotheses** and
each hypothesis was given to a hunt with one rule: **compile and run; a defect you
only reasoned about is not a finding.** Four hunts, four confirmations, and this
inventory. Every entry below has a reproducer that was executed.

The project already knew the lesson and had written it down at
`crates/heroes/src/modules/tests.rs`: *"the invariant was enforced by convention
over one type, and the one caller outside that type kept the defect."* What the
sweep adds is that the sentence was **understated** — the convention leaks *inside*
the type too, because a line number formatted into a `String` note is invisible to
every guard the compiler has.

## The three shapes

**Shape A — the scope widened silently.** A `bool` or a set that meant *"this
file"* when a `Source` held one file, and since M8a means *"this program"*. Fix:
key it by module and ask per reported site — D1's `holes_in` + `hole_covers`.

**Shape B — a position assembled by hand.** A caller that builds a location from
`line_col` + `src.name` + `text.len()` instead of `Source::locate` /
`file_line_of` / `root_end`. `source/mod.rs:229` already declares the one function
mandatory. The convention is enforced by nothing.

**Shape C — the emitter asked what the program *mentioned* where it needed what a
declaration *is*.** D3's own shape. `emit/descriptors.rs::generated()` answers
"which types did the program mention inside a container" while its three callers
ask "which types does the function I am about to write name".

## The inventory

Severity: **★★★** a legal program is refused or miscompiled · **★★** a diagnostic
is wrong or teaches a wrong program · **★** cosmetic or contract-level.

### Fixed in this sweep

| # | shape | what | severity |
|---|---|---|---|
| D1 | A | §4.16's hole exemption was program-wide: a `???` in one module silenced §4.4 in another | ★★ |
| D2 | B | the hole report named the root file and a line past its end | ★★ |
| D3 | C | a variant case payload was never released when no `match` in the same compilation bound it — same module, leaking rooted at itself, clean rooted at its caller | ★★★ |
| D4 | — | `heroes fmt` was not idempotent on **its own output**: a call the 88-column rule breaks across lines grew a blank line on every re-format | ★★ |
| D5 | B | `no_entry_point` spanned `text.len()`, which lands in the appended library, so the commonest mistake in the language answered `internal error … exit 2` — the compiler blaming itself | ★★★ |
| D6 | B | `check --json` named the root file for a diagnostic in another module — D2's shape in the one consumer that cannot notice by eye | ★★ |
| D7 | B | `no binary: N holes in <file>` named the file on the command line, not the files the holes are in | ★ |

### Fixed in this sweep — the emitter (2026-08-12, second pass)

**Shape C, and one repair did retire three.**

| # | what | symptom | severity |
|---|---|---|---|
| E1 | `equality_body` has no row for `Ty::Map`, `Ty::Fallible`, `Ty::Failure`, `Ty::Func` | `record Box { m: {str: int} }` then `==` → `panic: entered unreachable code`, exit 134. **`hash` covers maps and `eq` does not**, so CLAUDE.md §7's "eq and hash agree" is broken in the loudest direction: `{Box: int}` inserts and then aborts on lookup | ★★★ |
| E2 | `descriptors::generated()` is seeded from container elements only | `function grab() -> P?` for a record `P` → `use of undeclared identifier 'h_M_P_desc'`, exit 2. Adding `[P]` **anywhere in the program** fixes it, so the same file compiles under one root and not another | ★★★ |
| E3 | the same, for a `T?`-typed field's `hash` | `use of undeclared identifier 'h_M_opt0_desc'` | ★★★ |
| E4 | `descriptors::generated()` does not skip `mentions_generic` where its two sibling walks do | `[A?]` inside a generic → `thread 'main' panicked … every 'T?' is named before anything can mention one`, **exit 101**, which is outside §10's three codes | ★★★ |
| E5 | declared aggregates are emitted before generated option/function typedefs, and each kind can contain the other | `record Box { v: int? }` → `unknown type name 'h_M_opt0'`, exit 2. **Not a swap**: the reverse dependency is in the same file. Needs one interleaved containment order over both | ★★★ |
| E6 | a unit-typed record field | `void f_u;` → `field has incomplete type 'void'`, exit 2. CLAUDE.md §7 calls `void t0;` a hard error; the rule reached temporaries and not fields, and `emit/gate.rs` walks the IR and never a declaration's field list | ★★★ |
| E7 | the emitter's synthesised names `opt<N>` / `fn<N>` are unreserved, and take the **root** module | `record opt0` plus any `T?` → `redefinition of 'h_M_opt0'`, exit 2 — and only under some roots. The residual panel 031 R10 closed for module names and left open for the names the emitter invents | ★★★ |
| E8 | `builtins::reachable` matches `Op::Call` but never `Op::FuncRef` | a library function passed as a **value** is referenced and never defined → `use of undeclared identifier 'h_library_range'`, exit 2. One direct call from any module puts it back | ★★★ |
| E9 | `descriptors::generated` walks the interner, which the checker filled from `test` blocks too | `warning: unused variable 'h_M_P_desc'` on an ordinary program build — the zero-warning rule, and precisely what that module's doc says its worklist exists to prevent. Fixed by filtering the seed to the types the functions **this build emits** mention | ★ |

### Fixed in this sweep — the resolver and the checker (third pass)

**Shape A — scope widened.**

| # | what | symptom | severity |
|---|---|---|---|
| N1 | `missing_return`'s hole exemption is program-wide in the **checker** (`types/mod.rs:208`) | D1's twin in the pass D1's fix did not reach: an unfinished `geom.hero` suppresses `missing_return` in `main.hero` | ★★ |
| N2 | `Resolver::suggested` is program-wide | a did-you-mean offered in module B exempts that name from the unused sweep in module A — a dropped spec-line-77 error | ★★ |
| N3 | `Resolver::fields` is program-wide | a field name declared in `geom.hero` **removes a `certain` fix** from a diagnostic in `main.hero`. Another module decides whether a fix is machine-applicable (CLAUDE.md §8) | ★★ |

**Shape B — position assembled by hand.**

| # | what | symptom | severity |
|---|---|---|---|
| N4 | `check --apply` indexes concatenated-text spans into `user_text()` | `assertion failed: self.is_char_boundary(n)`, **exit 101**, on any multi-module program whose certain fix is outside the root — and `--apply` is what CI uses to assert `.fixed` files compile | ★★★ |
| N5 | ten sites format a line number **into a message** | `note: declared at line 6` where the truth is `geom.hero:1`; `the cycle is: A.b: B (line 7)`; a `shadowed_binding` whose message points *forward past its own caret*. All ten build real `Diagnostic`s, so M8a's sweep saw them and fixed only the span. Sites: `resolve/top.rs:75,108`, `resolve/scope.rs:159,190`, `resolve/decls.rs:85,101`, `types/calls.rs:190`, `types/construct.rs:41,92`, `types/sized.rs:291` | ★★ |
| N6 | `lex --dump-tokens` dumps the whole compilation | an 8-line program prints 470 lines including the library's, at concatenated line numbers, with no file marker. `--dump-ast` and `--dump-scopes` filter correctly | ★★ |
| N7 | `measure` returns exit 1 where the tool could not run | a missing vendored tokeniser table is `Exit::Diagnostics`; §10 says 2. Two adjacent `Err` arms in one function disagree | ★ |
| N8 | the tab diagnostic's caret is misaligned | `render.rs`'s doc says tabs cannot appear because the lexer rejects them — but the diagnostic *reporting* the tab prints the tabbed line | ★ |

**Neither shape.**

| # | what | symptom | severity |
|---|---|---|---|
| N9 | **fixed** — `Resolved::module_declaring` `.find()`s over a `BTreeMap` and so picks the **alphabetically first** module | with `use geom` written and both `alpha` and `geom` declaring `scale`, the compiler names `alpha` — a module the file cannot see — attaches a `guess` fix that produces `wrong_arity` if followed, and **cascades a false `unused_binding` telling the author to delete the `use geom` line that was the fix**. Rename `alpha.hero` to `zeta.hero` and the same program gets the right answer with a `certain` fix. It had exactly one possible answer when there was one module | ★★★ |
| N10 | the hole report offers functions from modules the hole's file cannot name | a hole in `geom.hero` is offered `main.tally(x: int)`, which `geom` cannot `use` without a cycle. The code states the right principle two lines above and applies it halfway | ★★ |

## What was cleared, stated plainly

A hunt that finds nothing is a result. **The emitter is not root-dependent in the
sense D3 was**: across the calculator's four roots, 277 shared C entities are
byte-identical — every `retain`/`release`/`eq`/`hash`, every descriptor, every
typedef — and the same holds for a purpose-built three-module corpus, for
monomorphisation, and for diagnostics. D3's own fix recurses correctly through
containers, nested payloads and every payload kind. `ir::is_refcounted` is
genuinely one home. The mangler survives a program whose every identifier is a C
keyword or libc symbol. `#line` is correct across modules and nothing bypasses the
writer that counts lines. Exit codes are right on unreadable, directory,
non-UTF-8, nonexistent, tab, BOM and missing-module inputs. `fmt`,
`--dump-ast` and `--dump-scopes` are correctly root-filtered. `-0.0` is normalised
before hashing. And `eq`/`hash` agree on every shape the language has **except**
the four rows E1 names.

One candidate was investigated and **is not a defect**: `check --json` printing to
stderr. `check` produces no artifact, so its diagnostics are the whole output and
`--json` says only *how* to print them (CLAUDE.md §10). The existing surface test
already pinned it; a hunt nearly filed it, and reading the contract settled it.

## What the sweep says about the instruments

Three things, and none is about any individual defect.

**The corpus is the instrument, and it had holes shaped like these defects.** D3
needed a payload owning a *computed* `str` that no `match` binds; E1 needs a map
inside an aggregate that something compares; E5 needs a record field typed `T?`.
Ninety `run/` cases and seventeen examples contain none of the three. Every one of
these is an ordinary program.

**An invariant enforced by convention is enforced by nothing.** Shape B has eight
entries and `source/mod.rs` has said *"there is one function and no caller
assembles the triple itself"* since M8a. The honest repair is not another sweep: it
is making `line_col` unavailable to callers that have no business with a
concatenated line — the only legitimate consumers are `locate` itself and the
printers' relative arithmetic.

**A comment that argues correctly for the wrong world is worse than no comment.**
D4's `spans_lines` was narrow *on purpose*, with the reason written out; the reason
was about source a person writes, and the formatter started writing source. D3's
fallback was the same. Both were read by reviewers who agreed with them.


## The premise audit, and the five roots (2026-08-12, same day)

The rule the sweep produced — *a narrowing asks the value, never the world* — was
then turned on the compiler and asked of every load-bearing premise in it. **Nine
more live defects, eleven latent**, and they group into five roots rather than
twenty problems, which is the useful half.

**Fixed here:**

| # | what | why it was silent |
|---|---|---|
| **S1** | a **NaN as a map key**. `{f64: int}` is legal, `0.0 / 0.0` is legal, and the probe rests on the key's `eq` being reflexive — which nothing said, and which `desc.c` does not defend (it defends the neighbouring rule, *equal keys hash equally*, which is what the `-0.0` normalisation is for). Inserted twice, `len` was 2; the lookup answered `missing_key`; `{nan: 1} == {nan: 1}` was false. **Exit 0, ASan clean, leak counter balanced** — every instrument reported success. Now aborts, as `sort` already did for the same value | ★★★ |
| **L1** | the lexer walked to `text.len()` under a doc comment saying *"one source file"*. Since M8a the indent stack and the open-bracket list **crossed the file boundary**: an unclosed `(` in `main.hero` swallowed all of `geom.hero` and reported itself inside the library, and so did `y =` — a half-written line. The commonest state a file is ever in answered `internal error … exit 2` | ★★★ |
| **S2** | a user file named **`library.hero`** had its declarations resolvable *unqualified* from every module; renaming it `util.hero` refused the same program correctly. Four sites identify the library by module *name* rather than by the `is_library` flag, and the collision check skips a pair whose modules are equal — a hole exactly at identity | ★★★ |

**Then all of the rest, the same day, grouped by root — because that is how they were fixed:**

- **"one byte is one column"** — three faces, one fix. S4: `fmt`'s 88-column test
  counted bytes, so a **56-column** line of 96 bytes was broken across four lines
  by a formatter whose own constant is called `WIDTH`. S5: the caret was one `^`
  per byte, underlining `return "ààà"` eight wide for five columns. And the third,
  which the tab repair had created: `line_col`'s column was bytes while the
  caret's padding had become characters, so a message named column 21 above a
  caret standing at 18. `line_col` counts characters now — its doc justified
  bytes by *"how the generated C's `#line` … reports positions"*, and `#line`
  carries a file and a line and **never a column**, so the premise defending the
  premise was dead too.
- **"the printer's input adjacency is its output adjacency"** — S3, the fixed
  `spans_lines` defect's twin three hundred lines away in the same file. Run
  membership is decided by the printer's own one-line test now, which is the only
  answer that survives the round trip.
- **"the runtime may trust its caller"** — the largest cluster, and every one of
  them asked about *the world that produced the value*. S11: `hero_str_from_bytes`
  validated nothing, while `slice` aborts on a split character, `chars` walks
  continuation bytes and `len` is documented over a valid encoding — three rules
  on an unchecked premise, which held only because the gate refuses `extern`
  today and **dies at M7**. It validates UTF-8 now, at the one place foreign bytes
  become a `str`. L5: growth freed the old block before the retry re-read the
  caller's key and value; the caller releases it after. L3: `key->hash` was
  guarded and `val->hash` was not, though `hero_map_hash` calls it. S7: every
  digit of the test index was validated and the accumulation was not — signed
  overflow, which is UB and which CLAUDE.md §7 forbids in the generated code.
- **counting as a proxy for identity** — S8. `phases.rs` had rejected counting for
  the decref sweep twenty lines away, and `check_copy_out` still counted: two
  copy-outs of one parameter and none of the other read as "2 of 2". It compares
  the multiset now, and `copying_one_parameter_out_twice_and_the_other_never_is_caught`
  is the case §9 asks for.
- **hand-maintained lists** — S13 is the interesting one. `is_thesis_rule`'s codes
  are what `--permissive` drops, so a **missing** entry silently understates the
  very number the thesis is argued from, and nothing could see it. The set of
  codes the golden corpus annotates is now pinned to a constant, `measure::gate`'s
  shape: adding a diagnostic turns it red until somebody writes the code down and
  decides which side of the control arm it is on. **It fired on its first day** —
  S6's new golden — which is the only evidence worth having. S12: hand-typed
  literal lengths became `sizeof(b) - 1`. S9: a null `newlocale` aborts instead of
  silently rendering `3,5`. S10: the key array's fill is bounded. S6: a lone `\r`
  is a diagnostic like a tab. L2: the root's module was stored **sanitised** while
  every other module's was raw, so `a_b.hero` saying `use ab` answered *"modules
  may not form a cycle: ab uses ab"*.
- **L4 is not a defect**, tested and stated: `to_str` on a `bool` prints `true`,
  and the `_ =>` entry-point fallback is genuinely enforced by `types/builtins.rs`.

**What the roots say.** Twenty premises, five claims. A premise depended on in
three places is **one thing to test, not three** — which is the shape
`a_declared_type_cannot_contain_a_type_parameter` already takes, and the reason
the remaining work is five tests rather than eleven fixes.
