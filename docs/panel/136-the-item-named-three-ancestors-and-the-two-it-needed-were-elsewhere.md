# Panel 136 — doctests: the item named three ancestors, and the two it needed were elsewhere

**Sat** 2026-09-13 · **milestone** M-deferral-ledger, step 2 · **status**
`provisional — author ratification pending`

**Lane: full five seats**, and the `ffi-pragmatist` is convened on a fact rather
than on its usual mandate: the entire repository holds **exactly one** markdown
fence inside a comment, and what it holds is an `extern` group. The gate
CLAUDE.md § 4 asks once per milestone was given at step 1; this sitting convenes
without asking again, as § 4 directs.

## The item, as design.md carries it in two places

`design.md:2793`, the Part 7 line: *"Doctests — v2, once the `test` mechanism is
proven."* And the substantive paragraph inside §4.18, `:2086-2089`: *"Doctests
(runnable examples in markdown fences inside doc comments) are deferred to v2.
They are a good feature — documentation that cannot lie, because it stops
compiling — with precedent in Rust, Python and Elixir. Deferred because they
would be a second way to write a test, which the form budget forbids until the
first mechanism is proven."*

**The stated condition has expired, and that is what made this sitting
necessary.** The `test` mechanism is proven on any reading of the word: measured
2026-09-13, **641** `test` blocks in `selfhost/` and **545** in `examples/`, with
the compiler's own tests one of the three suites that gate every step. A
condition that has been met and triggered nothing is the promise without a date
this milestone exists to end. So the deferral had to be re-argued from the
beginning, and what it now rests on is not a clock.

## The proposal, as it went out

A clause added to `spec/heroes-spec.md` § 12's first paragraph:

```
 `test` blocks run only when asked for; ordinary builds ignore them. An
 `assert` failure shows the source expression, and both sides where `print`
-takes them; where a side is an aggregate it shows the expression alone.
+takes them; where a side is an aggregate it shows the expression alone. A fenced
+block inside the comment that documents a declaration is a test too: its lines
+are statements, they run when `test` blocks do, and the declaration's own module
+is in scope.
```

## What the coordinator measured before the briefs went out, 2026-09-13

| fact | number | how |
|---|---|---|
| spec today | **5655** vendored · **7531** real (pinned 2026-09-12) · ceiling 8192, 601 headroom net of the FFI floor | `./heroes measure` |
| the diff above | **5697**, **+42** · real *unrun* | same, on the draft |
| a shorter wording | **5672**, **+17** · real *unrun* | same |
| `test` blocks | **641** in `selfhost/`, **545** in `examples/` | `grep -c '^test "'` |
| modules with a `## Tests` heading | **147** of 204 | `grep -rn '^## Tests'` |
| declarations carrying a doc comment | **1264** in `selfhost/`; **497** of 1135 in `examples/`, 43% | a walk over each declaration's preceding comment block |
| doc comments holding a backticked span | **645** in `selfhost/` | same walk |
| **markdown fences inside a comment, whole tree** | **1**, `selfhost/parse/group.hero:33-36` | a walk for a `#` line opening a fence |
| doc comments already reach the tree | `ast.hero:373,380,416` carry `doc: [token.Span]` per declaration | grep |

## The measurement that decided the sitting, found three times independently

**The repository's one instance of the form refutes the rule, and three seats
found it without conferring.** `selfhost/parse/group.hero:33-36` is a fence in
the doc run of `function extern_group` at `:52`, and it holds an `extern` group.

- The **compiler seat** ran the fence's body as statements against the live
  compiler: `error[expected_expression]` at column 5, exit 1. *"The diff, applied
  to today's tree, stops the compiler compiling itself."*
- The **FFI seat** reproduced that independently and then found **a second,
  separate failure**: repair the parse and the example still fails at name
  resolution, `error[unknown_name]: nothing named `sqlite3_open` is in scope`,
  because the module that documents it declares no `extern` group. *"A rule whose
  corpus is one instance, and that instance refutes it, is a rule written against
  the wrong evidence."*
- The **warden** reached the same place from the specification's own grammar: an
  `extern` group is a **Declaration** (spec § 4) and never a **Statement**
  (spec § 5), so the clause's *"its lines are statements"* is false of the one
  case that exists.

And the FFI seat named why it is not an accident of one file: **the fence in this
project is a form exhibit**. design.md's own §4.19 illustration and spec § 13's
are the same shape, and the first line of every fence in the specification is a
declaration keyword — `extern`, `function`, `test`, `constant` — or a binding.
At the FFI, what is worth showing beside a signature *is* a declaration, because
a declaration checked against a header is the whole of what §4.19 has to show.

## Verdicts

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| `compiler-engineer` | **object**, and **veto on route R2 only** — not on the form, which under R1 is sugar erased in the frontend and adds **zero** exhaustive-match arms | §1.7, §1.1, §4.17, §4.18/§4.15 | R1, a second bounded pass over the doc spans producing a synthetic `test_decl`, is **~370–420 lines over 9–11 files** (counted estimate), of which a new module 180–250; **zero** arms move, where a new `DeclKind` case would move 129. **Subtraction: zero** — 641 `test` blocks and 147 `## Tests` sections, not one deleted by a fence. The formatter survives R1, measured twice byte-identical, because comments are re-printed verbatim off a separate stream; **R2 destroys that** and contradicts `scan.hero:83-85`'s written invariant that comments never influence terminator insertion. And **a failing `assert` carries no file and no line** — `runtime/heroes_runtime.h:300-301` is the whole surface — so what locates a failure today is the author's title, which a doctest has none of | at a doctests milestone's close, fences in `selfhost/` number **≤ 3** (today 2 lines, one fence) and `test` blocks **≥ 635** (today 641): the new form will not have replaced 1% of the old | a prototype in this tree that (a) leaves `group.hero` compiling untouched, (b) keeps `heroes fmt` on it byte-identical, (c) **deletes ≥ 50 `test` blocks** from `selfhost/`, and (d) lands under 250 added lines in `suite_layout`'s own unit |
| `ffi-pragmatist` | **object** (defer); **no veto, and it says so plainly**: the form does not touch the C ABI — layout, ownership, NUL-termination, refcount, register class and `sizeof` all unchanged | §1.11, §4.19 (`:2093-2094`), §1.12 via CLAUDE.md § Precedence rank 3 | the one fence fails **twice**, at two passes, measured. And an FFI doc example would have to become **a complete, resource-correct C program or the build goes red**: it ran the abbreviated example a binding's documentation actually carries and got `assert failed: sqlite3_step(statement) == SQLITE_ROW, left: 21` — `SQLITE_MISUSE`, because the example omits the return-code check documentation omits by nature. The binding author's two exits are full ceremony in the comment or no comment. **It withdrew one of its own concerns after measuring**: a doctest adds no link a module's `extern` group did not already impose, verified with `otool -L`. And **no `heroes doc` exists** (12 subcommands, measured), so a doctest would be the only consumer of a doc fence — *"Rust's mechanism without Rust's reason"* | under the proposal as written, `heroes test selfhost/main.hero` exits non-zero at `selfhost/parse/group.hero:34:5` and the compiler stops testing itself. And **rung 5 of §4.19's ladder cannot carry a doctest at all** on this Mac: a fence on a raylib member is `error[ffi_missing_header]` unless the doctest inherits the module's `package "raylib"` line, which the one sentence does not grant | three, any one flipping it: the diff says what a declaration-shaped fence is and the answer is not a compile error in the file documenting the FFI; a doctest inherits its module's `package` and library resolution, compiled on a raylib fence; or a second consumer of the fence exists or is scheduled |
| `llm-ergonomist` | **adopt-with-condition**, and *"unconditioned, I refuse the text"*; no veto (the locality test is met) | § 12 | counted by hand: the documented declaration is **222 characters** against the `test` block's **88** for the same claim — **2.52×**, all of it `# ` prefixes and fences, so *"a model optimising tokens keeps writing `test`"*. Six questions asked of the text, **four SILENT**: whether the file's `use` names are in scope, what a fence in a comment that documents nothing does, what a failure names, and whether an ordinary build type-checks it. **And it found the shape that looks checked and is not**, in three lines: a fence above a `use` line, since spec § 1's grammar makes a `Use` not a `Declaration`, so the clause never attaches and the assert is false forever with nothing to say so | from the text alone, over ≥ 20 generated modules of ≥ 200 lines: **≥ 25% contain a fence that is not directly above a declaration** and is therefore inert; **≥ 20% of doc fences use the value-echo idiom** rather than `assert`; first-try success runs **10 to 20 points below** the text without the form | the four silences close, which it wrote and counted: **+340 characters** of § 12 (a `hero` tag marking Heroes and any other tag prose, the test named by the declaration below it, a fence with no declaration below it an error, the file's `use` names in scope, type-checked by every build, and the column the fence's lines start at). Drops to plain adopt at zero inert fences over 20 generated modules; **moves to refuse** if the amendment is declined and the inert rate is above 10% |
| `spec-warden` | **object — defer again**, provisional (a draft's real row is unrun); no veto, since the measured budget passes and it is §1.0's burden that does not | §1.0 `:117-126`, §1.6 `:255-318`, §1.2 `:190-204`, §4.15 `:1920-1924`, §1.7 `:411-413` | **the "form budget" the item rests on is defined nowhere in this repository** — a grep over `docs/`, `spec/` and `.claude/` returns only item 6's own sentence. Drafts measured: FULL **+42**, MIN **+17**, and a COMPLETE wording that marks the fence and names the failure **+61**; R1 (panel 135's unspent −43) pays every one of them, so **the arithmetic is payable and the burden is not**. It declined panel 135's second removal on its merits: § 3's *No aliasing exists anywhere* is not a restatement, it extends the copy rule past bindings to fields and parameters | at M-doc-generator's close `selfhost/` holds **fewer than 25** runnable doctests, under 2% of its 1264 documented declarations (instrument: a grep for comment fences, which reads 1 today), and `heroes mutate` moves the `selfhost/` kill rate by **under 1 point** | adopt-with-condition when (a) ≥ 50 real fences exist and `heroes mutate` shows them raising the kill rate by ≥ 1 point, (b) the clause is the COMPLETE one, (c) `heroes fmt` formats the fence interior so §4.15's second purpose survives. Veto only if a landing `--refresh` reads past 8192 net of the FFI floor |
| `historian` | **adopt-with-condition** (advisory), *"but the condition redirects the feature rather than scheduling it"*: defer the Rust/Python form permanently, adopt the Go/D form | design.md Part 6's literate-source row (the one-direction rule), §4.18 | **the item names the wrong three ancestors.** Rust pays, by its own published measurement, **775 seconds of compile time** for one crate's doctests and **eight attributes** of dialect `#[test]` does not need, and it changed the default twice (2014, tested-by-default; Edition 2024, merged into one binary for speed). Python's own docs warn that *"if even a single character doesn't match, the test fails"*, naming set order, object addresses and float formatting. Elixir is **not** a third mechanism at all: `doctest MyModule` is a macro generating ordinary ExUnit tests inside an ordinary test module, opt-in per module. **Go and D, unnamed in the item, get design.md's exact stated benefit for nothing** — the example is a test, in the file where tests live, and the doc tool pulls it into the prose. Nobody removed doctests anywhere, and `doctest` was never on PEP 594's list | if Heroes ships the Go/D form, the implementing milestone adds **zero** new diagnostic classes and **zero** fence attributes — no `ignore`, `no_run`, `compile_fail`, `should_panic` — because the block is already type-checked, already run and already has the language's own error reporting. Score by counting both at that close | moves on any of three: a language that shipped the code→doc direction and had to add doctest attributes anyway; a documented case of Go's or D's design failing where a doctest succeeds, specifically an example **interleaved with prose** line by line, which it could not find an answer to; or a primary source showing Rust chose its direction over a Go-style alternative for a stated reason, which it searched for and did not find |

## Where they disagree, unsmoothed

**They do not disagree about the verdict, and the sitting says so rather than
staging dissent.** All five arrive at wait, by four different routes: the
corpus refutes the wording (engineer, FFI), the burden is unmet and the cited
rule does not exist (warden), the text is silent where it must not be
(ergonomist), and the ancestry is wrong (historian). Two of the five write their
verdict as `adopt-with-condition`, and in both cases the condition is what the
verdict is: the ergonomist says *"unconditioned, I refuse the text"*, and the
historian's condition is not a schedule but a different feature.

**The one real disagreement is about what returns.** The engineer, the warden and
the FFI seat each wrote a return condition for **the doctest** — 50 test blocks
deleted, 50 fences with a measured kill-rate gain, four fences whose body is not
a declaration. The historian says the doctest should never return in that
direction at all, and that what returns is the Go/D form. The resolution below
takes the historian's frame and keeps all four conditions, because they are not
in competition: three of them measure whether the corpus ever wants a fence, and
if it never does, the fourth is what to build instead.

**A disagreement of method, recorded because it will recur.** The ergonomist
would close the silences with **+340 characters** of § 12; the warden priced the
complete clause at **+61 tokens** and found it payable. Those are the same act
measured in two units, and neither seat saw the other's number. The unit §1.6
judges is tokens, so the warden's is the operative one, and the ergonomist's
character count is what a reader of the section pays.

## The resolution adopted, provisionally

**Item 6 is DEFERRED AGAIN, dated, with a return condition — and its reason is
replaced rather than repeated, because the one on the page has expired.** This is
the most robust and complete resolution and not the cheapest: ENTERS would land a
form that stops the compiler compiling itself on the day it lands, and a bare
*deferred, v2* would leave the same expired clock the ledger exists to remove.

1. **The verdict goes into design.md in both places** — Part 7 item 6 at `:2793`
   and the substantive paragraph at `:2086-2089` — as a correction beneath the
   original (CLAUDE.md § 14).
2. **The expired condition is retired in its own words.** *"Until the first
   mechanism is proven"* is met: 641 + 545 blocks, one of the three suites. The
   sitting records that the condition expired **and triggered nothing**, which is
   the shape this milestone exists to end.
3. **What survives is not a clock.** §1.7's subtraction returns **zero** — no
   `test` block, of 641, is deleted by a fence — and the corpus refutes the
   wording three times over. The *"form budget"* the paragraph cites is **defined
   nowhere in this repository**; the rule it was reaching for is §1.7's
   subtraction test and §4.15's *exactly one correct way*, and those are named
   instead.
4. **The three ancestors are corrected.** Rust's cost is its own published 775
   seconds and eight attributes; Python's own documentation warns that one
   character breaks a doctest; **Elixir is not a third mechanism** but a macro
   generating ordinary tests, opt-in per module. And the two the item needed were
   elsewhere: **Go and D obtain the identical stated benefit with no second test
   mechanism at all**, because the example is a test and the doc tool reads it.
5. **The form that returns is named, and it is not the doctest.** A **documented
   test block** — Go's and D's shape, the example living where tests live and
   `heroes doc` pulling it into the prose — is the direction design.md Part 6
   already chose in writing: *"`heroes doc` generates the document instead — one
   direction only"*. A doctest reverses that arrow; a documented test block
   extends it. **Its home is M-doc-generator** (chain row 62), which is also the
   missing second consumer the FFI seat named: today a doctest would be the only
   reader of a doc fence, which is Rust's mechanism without Rust's reason.
6. **Four return conditions, all falsifiable, all naming instruments in this
   tree**, and any one of them reopens the question:
   - `grep -rn '^test "' selfhost/ | wc -l` falls from **641 to ≤ 591** under a
     candidate implementation while `heroes test selfhost/main.hero` stays green
     and `heroes fmt selfhost/parse/group.hero` stays byte-identical (engineer);
   - a grep for comment fences finds **four or more whose first body line is not
     a declaration keyword** — today zero of one (FFI seat);
   - **≥ 50 real fences exist** and `heroes mutate` shows them raising the
     `selfhost/` kill rate by **≥ 1 point** (warden);
   - M-doc-generator ships and the Go/D form is measured there, at which point
     the question is whether a fence adds anything the documented test block does
     not (historian).
7. **If the author overturns this and rules ENTERS**, the sitting records what
   that compels, so nothing is re-derived: route **R1 only** (the engineer's veto
   stands on R2, which contradicts `scan.hero:83-85`'s written invariant); the
   declaration-shaped fence decided **in the spec diff and not in the
   milestone**, because `selfhost/` stops building otherwise; the clause the
   COMPLETE one at **+61**, paid by panel 135's unspent **−43**; a doctest
   inheriting its module's `package` and library resolution, or §4.19's ladder
   above rung 3 is undocumentable; and a name for a failing example, since the
   runtime's assert carries no file and no line.

**What conservative would have been, so the author can choose it**: the same
deferral with the expired clock left in place and only a date added, which is
cheaper and leaves the next sitting to re-derive everything above.

## Found beside the sitting

**A failing `assert` names no file and no line, and the milestone that owns
panics does not cover it.** `runtime/heroes_runtime.h:300-301` is the whole
surface — `hero_panic_assert(HeroStr text)` and `hero_panic_assert_sides(text,
left, right)` — so what locates a failure today is the author-written title of
the `test` block it sat in. M-panic-location's census (2026-09-03) measured
`hero_panic` and named four abort classes; the assert path is a **fifth surface
it did not name**, and `selfhost/emit/ffi.hero:58` records the compiler's own
authors hitting it: *"stopped the compiler with `assert failed:
!found.is_err()` and no line"*. Filed as an item in that milestone's file rather
than repaired here. It also decides part of any future doctest: a form with no
title cannot be located at all until this is fixed.

## Author's verdict

Pending — queued in `docs/work/DECIDE.md` as `panel 136`.

## Predictions to score

| prediction | instrument | scored at |
|---|---|---|
| engineer: fences in `selfhost/` ≤ 3 and `test` blocks ≥ 635 | `grep -c` | a doctests milestone's close, if any |
| warden: `selfhost/` holds < 25 runnable doctests, and `heroes mutate` moves its kill rate < 1 point | a grep for comment fences; `heroes mutate` | M-doc-generator's close |
| FFI: under the proposal as written, `heroes test selfhost/main.hero` exits non-zero at `group.hero:34:5` | the command itself | any commit that lands the clause |
| FFI: a raylib fence is `error[ffi_missing_header]` unless it inherits `package "raylib"` | `heroes build` on this Mac | any commit that lands the clause |
| ergonomist: ≥ 25% of generated fences inert, ≥ 20% using the value-echo idiom, first-try 10–20 points below | metric 2's runner, when it exists | M-thesis-harness |
| historian: the Go/D form needs **zero** new diagnostic classes and **zero** fence attributes | counting both at that close | M-doc-generator |
| the return conditions' own clock: 641 → ≤ 591; four fences whose body is not a declaration; ≥ 50 fences with a ≥ 1-point kill-rate gain | `grep -c`, `heroes mutate` | every later sitting of this ledger re-reads them |

## What the seats could not source or could not run

The engineer compiled no prototype (the twenty-minute rebuild a sitting forbids),
so its line figures are counted estimates by analogy; the arm counts, the two
formatter runs and the `expected_expression` measurement are real, and it did not
run `suite_layout` to confirm the two ceilings hold. The warden could not run the
real tokeniser on a draft (no pin; `--refresh` is never run in a sitting). The FFI
seat's raylib `package` probe is compiled but its `TraceLogCallback` path is
unrun, and the behaviour on the other two platforms is unmeasured. The historian
marks unverified: Python `doctest`'s exact stdlib version (secondary sources
only), the DMD version that introduced documented unittests, any explicit
pytest-maintainer statement against doctests-as-suite, and whether
`cargo test --no-run` builds doctests; and its *no language removed doctests* is a
negative from its own vocabulary, so it stands as a question.
