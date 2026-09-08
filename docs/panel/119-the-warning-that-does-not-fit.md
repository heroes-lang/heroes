# Panel 119 — the rule that would warn the reader is the one that does not fit

Date: 2026-09-08. Trigger: M-closures-verdict step 1, the milestone whose
deliverable is a **verdict** on design.md Part 7 item 1 (closures) and item 12
(inline blocks). Full five seats: the form has surface, a spec cost and at least
one diagnostic class.

**The count the sitting was owed before it sat** is
`docs/measurements/020-four-sites-in-fifty-five-thousand-lines.md`, written the
same morning, and four of the five seats were handed it. The llm-ergonomist
received only `spec/heroes-spec.md` and five label-stripped candidate clauses,
and the two corrections that reached the other seats mid-sitting were
deliberately **not** sent to it.

## Proposal (as put to the judges)

Rule on Part 7 item 1 — *"Closures — v1.5, immediately after the first running
program … capture by copy = a record plus a function pointer. Will delete the
handful of named one-line helper functions that currently exist only to be
passed around"* — and item 12, inline blocks, *"likely lands together with
closures"*. Neither is on the §1.0 closure list, so the whole warrant is
Principle 0's second branch: a measured Part 11 effect, or a §1 argument this
panel accepts.

- **A** — closures enter, capture by copy, the rule stated in full, a capturing
  value refused at every `extern` position. Measured **+118** spec tokens.
- **B** — the same feature, terse clause, the distinction left to the checker's
  diagnostic. Measured **+68**.
- **C** — only a **capture-free** unnamed function. Measured **+57**.
- **D** — only inline blocks, item 12. Measured **+93**.
- **E** — both refused, each with a Part 6 row naming what would make the
  refusal wrong (CLAUDE.md §12).
- **F** — deferred with a dated return condition written as a falsifiable claim.

Recommended provisional resolution as put to the judges: **E**. The evidence
sharpened it into something narrower and more demanding; see *Resolution*.

**Two seats put a seventh option on the ballot themselves**, which is § RUN IT's
rule that an option set is a measurement too. The spec-warden's **G1**:
*examined and deliberately unplaced*, the shape `comptime` has at
`design.md:2449-2473` (panel 039), with joint return conditions. The
compiler-engineer's **G2**: **E for item 1 and F for item 12**, on the ground
that they fail different tests and one verdict for both is a category error.

## What the sitting was handed, every number run on this Mac on 2026-09-08

- **Supply.** The compiler, 190 modules and 55,361 lines, passes a function as a
  value at **4 sites in production code**, none of them capturing; 13 of its 17
  value sites are `test` doubles. Corpus-wide: 100 value sites, 33 distinct
  functions, **21 of 308 files** hold one at all.
- **What §1.7's subtraction buys.** Under item 1's own wording — passed and
  never called — **21 declarations, 66 lines, every one in an example program**,
  in 7 files, four of which are the programs whose subject IS the higher-order
  function. In the compiler: 0, or 1 counting production uses only.
- **Budget.** The spec measures **3965** (cl100k_base binds, claude-legacy 3889,
  printed spread 76). Ceiling 4096, FFI floor mortgages 60, so **71 free**.
- **The rival claim on those 71 tokens.** The spec writes `map`'s signature and
  no other while naming `filter`, `fold`, `find`, `any`, `all`. The four missing
  ones cost **+63** — they fit. A closure clause that states its rules costs
  +118. They do not both fit.

## Verdicts

| Judge | Verdict | Section | Cost / delta, run | Prediction | Condition |
|---|---|---|---|---|---|
| compiler-engineer | **veto A, veto B**; object C, D, F; **approve E**; proposes **G2** | §1.7 core-plus-elaboration, on §1.1 and §1.6; §1.12 via Part 5 | made the compiler count it: a new `ExprKind` is **59** arms in 18 files, a new IR op **48** in 20, a new `Ty` case **175** in 51 — **A/B together 282 arms in 74 of 190 modules**, floor 197 widened lines plus 86 real arms. **Five DECIDED ceilings breach on one line per arm.** The parser **has no legal home**: a separate module gives `error[module_cycle]`, so it goes in `grammar_expr.hero`, 1002 → ~1047 against a ceiling that already cost an author override. **No inlining pass exists** — all 18 `grep -in inlin` hits are the word in other senses | at the close of the first milestone landing A or B: `suite_layout` reports **≥5 files over a DECIDED ceiling** and the diff touches **≥280 arms across ≥70 modules**, so *"~150 lines"* understates by **≥1.8×**. Falsified if that milestone closes green with ≤200 changed lines and no ceiling raised | veto lifts on a representation, compiled, where a capturing closure (i) has **one type per signature**, (ii) lets `ir/own.hero` decide release **from the type alone** with no runtime descriptor pointer, (iii) keeps eight bytes at every `extern` position. Objection to C lifts if the author raises `grammar_expr.hero`'s ceiling explicitly, C being genuinely sugar |
| ffi-pragmatist | **veto A, veto B**; **approve C, D, E**; object F | §1.11, §4.19, decided by §1.12 | **compiled, not argued.** `hero_desc_func.size` is **8** (`runtime/parts/desc.c:206`, ABI 21); a closure over one `i64` and one `str` is **32**. The cast at the real `sqlite3_busy_handler`: clang **exit 0, zero diagnostics under all fourteen flags**, run time **exit 139**. Array route under ASan: **heap-buffer-overflow**, push silently truncated 24 of 32 bytes. Refcount route: `hero_copy_func` is a bare assignment and drop is `hero_drop_nothing`, so a captured `str` is **heap-use-after-free**. `hero_eq_func` reads 8 of 32: **either field order is silently wrong about one case** | at **M-web-framework**: a middleware chain over C sockets needs **zero** closures at the boundary, because every callback it binds carries a `void *` context — **41 of 43** in `sqlite3.h`. Falsified by one chain whose C callback has no context channel. **raylib is 10 of 10 with no context at all**, and two of its setters use the callback as an **identity** for removal, which a struct-valued closure does not have | veto on A lifts the day capture-freedom is a **type-level** fact, because the `_Static_assert` probe is generated from the declared type: one Heroes function type gives one C typedef, so a struct makes **every** callback binding a clang error and a bare pointer loses the environment. **The veto on B does not lift**: a representation split recorded only in a diagnostic's text is the inverse of §4.19 |
| spec-warden | **veto A, veto D**; object B, C, E, F; proposes **G1** | §1.6, §1.0, §1.2, §1.7; Part 6 preamble; CLAUDE.md §12 | A → 4083 + 60 = **4143 ≥ 4096**, suite red. D → 4118, red. B → 4093, green **by 3**. C → 4082. Drafted the same content classes independently: **+141** for the full statement, breaching 4096 unaided. Priced six removals (82, 152, 162, 498, 323, 649) — **none admissible reaches +118** | no drafting brings a clause stating capture-by-copy plus the `extern` refusal under **+110** on cl100k; two independent drafts read 118 and 141. Falsified by a text at +109 that still says all three parts | veto lifts on a measured removal of ≥118 admissible tokens deleting no closure-list rule and no shipped capability. **B is a verdict inside its own error bar**: 3 tokens is 4% of the printed 76 spread, and the author's own trigger of 2026-08-26 in `vendor/tokenizers/README.md` reads *"vendor it before any verdict lands within 10 tokens of a ceiling"* |
| llm-ergonomist | **veto D**; object A, B; **approve** the status quo and **C**, noting C buys nothing | the thesis; locality | 15 programs, 3 tasks × 5 arms, reading the spec alone. Blind ranking **status quo > C > A > B >> D**. Silent-wrong added: status quo **0**, C **0**, A **2**, B 2 plus one that compiles two ways | n=40/arm: A and B each **≥8 pp below** the status quo on first-try and **≥8 pp above** on silent-wrong; **≥40%** of lambda bodies land in a layout the grammar refuses absent a layout sentence; **Task 3 first-try ≤50% in every arm**, modal failure `a < b` on two `str` | A becomes approve on three amendments, the load-bearing one being that **mutating a captured binding is a compile error rather than a copy-local no-op**. **The D veto cannot be paid** without a caller-side marker: the meaning of an indented line would depend on a signature not on the screen, and `spec:16`'s *"significant and rigid"* stops being true |
| historian (advisory) | **approve** refusing item 12 (worst precedent record of the five); **approve** refusing item 1 as defensible, but **object** to any falsifier resting on *"capture by copy is hazardous"* | precedent | ~40 web operations, every citation opened or marked. **The hazard is capture by REFERENCE, which Heroes cannot express**: Go shipped a breaking change at **1.22, 2024-02-06** (*"each iteration of the loop creates new variables, to avoid accidental sharing bugs"*), C# moved the `foreach` variable inside the loop at **5.0, 2012** (Lippert: *"the single most common incorrect bug report we get"*), Java 8 forbade mutable capture outright (Goetz: *"Lambda expressions close over values, not variables"*). **No language that shipped capture by copy has reversed it.** And **no language ships a type-level capturing/non-capturing split** — C++11 and Rust RFC 1558 both do it by **coercion**, and Rust's **#44291 is still open**: a non-capturing closure does *not* coerce to `extern "C" fn` | if both are refused, **no program the closure list requires is blocked**: two self-hosting compilers exist in languages that forbid capturing function values (Oberon; Zig 0.10.0+). Falsified by a `selfhost/` site that cannot be written without capture. And if item 1 ever ships, **the first diagnostic it needs is about escape, not aliasing** — Swift needed `@escaping` in the type, Hylo's captures *"can't escape its local scope"* — and +118 tokens will not cover it | withdraws the approval on refusing item 1 if any of: a value-semantics language with no references **removes or restricts** capture-by-copy after shipping it (**Hylo is the live counter-case and it shipped them**); Hare, Odin or C3 reversing; **any sourced measurement of code deleted by closures**; or a `selfhost/` compiler need. Would harden the item 12 objection only if a Kotlin-`crossinline`-style guard, or an outright refusal of `return` in a block, were written into the proposal |

## Findings that changed the proposal

**The coordinator's own central finding was wrong, and a seat ran it down.** The
brief and a mid-sitting message to three seats claimed that
`examples/interpreter/run/expr.hero` would go silently wrong under capture by
copy, because the left operand is evaluated with `f` and the right with
`first.frame`. **`f` is a plain immutable parameter** — `grep -c '@f:'` is 0 —
so a captured copy can never diverge, and the mistake described is a different
binding being named, writable today with no closures. It was an inference
dressed as a measurement, CL-018's exact shape.

**What the engineer found in its place is unrefusable, and it is the `@` cell.**
A `total: i64 @ 0` captured by copy freezes at creation and a later read yields
the stale value. Every language a model has seen captures by reference — Python,
JavaScript, Rust's `FnMut`, Swift, C# — so the accumulating closure is the
textbook idiom and Heroes would compile it and print the wrong number. **A write
to a captured cell is refusable; a read is not**, because whether the frozen
value was wanted is undecidable, and §1.2 cannot even price it: there is no
error round-trip.

**And that is the sitting's title.** The only draft of the clause that states
what happens to a captured `@` cell is the **+118** one, which breaches §1.6 by
47 measured tokens. The rule that would warn the reader is the rule that does
not fit.

**Two seats reached that class from inputs that do not overlap.** The
llm-ergonomist, holding only the spec, wrote it as its silent case for the table
task: a captured `i: i64 @ 0` incremented inside the body to supply the index
`map` does not give, which compiles precisely because the counter is read inside
the closure so the unused-binding rule never fires.

**The premise the emitter states about itself dies by construction.**
`selfhost/emit/callback_guard.hero:24-45` writes its own falsifiable claim: *"a
Heroes function reaches C only through an address some instruction in this
program took. It dies the day a function value can be produced without a
`func_ref`."* A closure is **constructed**, not address-taken. A and B falsify
that sentence, and the guard exists because panel 111 measured a shared `str`
across 32 threads as use-after-free or double free in 9 ASan runs of 10. **C
does not falsify it**: an unnamed capture-free function is still a function whose
address `func_ref` takes.

**The sitting was tuning the wrong clause, and the seat that reads only the spec
is the one that said so.** What gated all three of its tasks in all five arms
was not the absence of closures: `filter`'s signature is written nowhere, nor
`fold`'s, `find`'s, `any`'s or `all`'s — only `map`'s (`spec:121`, against the
list at `spec:188`). Checked afterwards against `selfhost/library_source.hero`,
its invented `filter<A>(xs: [A], f: (function(A) -> bool)) -> [A]` was exactly
right, and **`find`'s `-> A?` is the one no reader can derive**. Separately, `<`
refuses `str` while `sort` orders `str`.

**Panel 013's condition is discharged and sharpened.** Its ffi-pragmatist wrote
in 2026-08-04 that *"the type system will have to distinguish capture-free at the
boundary."* Compiled today, that is too weak: the `_Static_assert` probe is
generated from the **declared type**, so it is not *distinguish at the boundary*
but **distinguish everywhere**. And four indirect routes — a local, a parameter,
a record field, an array element — all reach a C callback parameter at exit 0
today, while `selfhost/emit/callback_guard.hero:28` says the emitter *"does not
chase values"*.

**Both live statements of where a function type may cross are wrong.**
`docs/work/SCHEDULED.md` said `selfhost/check/ffi.hero:45` *"refuses
`.function_ty` in every extern position"*; that file's own comment says *"refused
at two of its three positions"*. Run: it is refused at **four** — extern result
(`ffi_type`), `@` parameter (`ffi_parameter_type`), `extern constant`
(`ffi_constant_type`), group record field (`ffi_field_type`) — and admitted at
exactly **one**, a plain `extern` parameter. The coordinator verified two of the
five directly, the parameter admitted with an empty stderr and the result refused
with `error[ffi_type]`; the other three are the seat's runs.

**The clause the coordinator drafted for option D collided with a built-in**, and
the seat reading only the spec caught it: `repeat(times: 3)` as the inline-block
example sits beside `repeat(s, n)` at `spec:185`.

**Option D is immune to the `@` trap and cannot express the shape.** The engineer
verified that `expr.hero:57-66`'s `evaluated` is a recursive dispatcher threading
`call` down into four functions that each call it again; an inline block is
expanded at one call site, cannot be stored in a parameter, cannot cross a
recursive call. **D removes zero of those five signatures' parameters**, and the
ffi seat measured it contributing **zero** new C symbols. It is immune to a trap
in a shape it cannot express.

**One recommendation independent of every option, and it is measured.**
`-Werror=cast-function-type-strict` fires on exactly the cast that produced exit
139, on Apple clang 21, where `-fsanitize=function` segfaults with no report. It
is absent from `selfhost/cli/flags.hero`'s fourteen, and `grep -rn
cast-function-type selfhost/ runtime/` is 0. The coordinator measured whether
adding it is free: the seed plus the runtime compile **exit 0** with it, and so
do `examples/threads/main.hero`, which hands a Heroes function to a C thread
spawn, and `examples/sqlite/main.hero`, which binds a real library. Whether it is
free on **every** program the emitter can produce is **unrun** — the full net
would settle it.

**The refusal is a mainstream position in this language family, and the null
result the brief asked for came back not null.** Among languages with Heroes'
constraints — manual memory, a C ABI, no hidden allocation — **four refuse
capture in writing**: Zig (`ziglang/zig#229`, closed 2020-11-03 — *"It would be
too easy to accidentally close over a pointer to stack memory and clobber your
stack"*), Odin (its own FAQ: *"Odin only has non-capturing lambda procedures"*),
C3 (its FAQ: *"do not capture any state … makes it easier for C3 to retain a
simpler lifetime model"*), and **Oberon, since 1988 and never reversed** — the
report's §6.5 forbids assigning a *nested* procedure to a procedure variable, in
a language that self-hosts. Only Hare leaves the door open, and has for five
years without walking through it.

**And the single most decision-relevant fact in the sitting is about the cheap
option, not the expensive one.** `ziglang/zig#1717`, the **capture-free**
anonymous function proposal — whose own Non-goals section reads *"Closures"* —
was **accepted in 2020 and then rejected**, closed `not_planned` on 2023-07-09.
That is direct adverse precedent for **option C**, the one option in this sitting
that carries no veto and fits the budget.

**The one-liner-helper claim has zero external support, and the seat says so
rather than finding some.** Asked for any sourced measurement of how much code
closures deleted in any language, it found none: the literature counts *sites
converted* (Gyori et al. ESEC/FSE 2013: 1263 anonymous-class conversions across
9 projects) and *lambdas written* (Mazinanian et al. OOPSLA 2017: 100,540
lambdas across 241 projects), never lines removed. The one randomised controlled
trial on the ergonomic half points the other way: Uesbeck, Stefik et al., ICSE
2016, found participants *"spent more time with compiler errors, and have more
errors, when using lambdas as compared to iterators"*. So this sitting's **21
declarations** is the only measured number in the room.

**The interval, for the record.** Java shipped without closures on 1996-01-23
and added them at Java 8 on 2014-03-18 — **18 years and 2 months** — and nothing
broke on arrival, because Goetz refused a structural function type and reused
functional interfaces. Heroes is two months old.

**A limitation of this sitting, disclosed by the seat rather than found
afterwards.** The historian has `Read`, `WebSearch` and `WebFetch` and no grep,
could not locate the proposal document, and states that its reading of the
options rests on the coordinator's brief and not on a file it opened. It also
corrected its own brief: BGGA, CICE and FCM had **no JSR numbers** — JSR 335 is
Project Lambda — so the brief's request for them was a false premise.

## Resolution — PROVISIONAL, author ratification pending

**Neither item enters, and they do not get the same verdict, because they fail
different tests.** This is the compiler-engineer's G2 and the spec-warden's G1
applied where each fits, and it is more demanding than the **E** the coordinator
put on the ballot rather than a compromise between the options (CLAUDE.md §4).

**R1 — Part 7 item 1, closures with capture: REFUSED.** A design.md **Part 6
row**, refused on **cost alone**, which Part 6's own text licenses and §1.1
makes legitimate by making simplicity the ceiling. Three seats vetoed it. What
the row must carry, all measured on 2026-09-08: **282 exhaustive-match arms in
74 of 190 modules** with a floor of 197 widened lines plus 86 real arms; **five
DECIDED file ceilings breached** on one line per arm; **no legal home for the
parser**, a separate module giving `error[module_cycle]`; **+118 spec tokens**
for the clause that states the rules, against **71** free, with **six candidate
removals priced and none admissible**; and four **compiled** corruption classes
at the C boundary, the sharpest being clang at exit 0 with zero diagnostics under
all fourteen flags and **exit 139** at run time through a real
`sqlite3_busy_handler`.

**R2 — the row's falsifier, and it deliberately does NOT rest on capture being
hazardous.** The historian objected to that ground and it was right to: history
says the hazard is capture by *reference*, which this language cannot express,
and no language that shipped capture by copy has reversed it. So the falsifier
is the engineer's, unamended by any claim about copying: **a program on the §1.0
closure list, or a measured Part 11 effect, that a named top-level function
cannot express — together with a representation, written down and compiled, in
which a capturing closure has one type per signature, lets the ownership pass
decide release from the type alone with no runtime descriptor pointer, and keeps
eight bytes at every `extern` position.** The ffi seat's veto lifts on the same
representation and adds that capture-freedom must be a **type-level** fact,
because the `_Static_assert` probe is generated from the declared type.

**R3 — the `@` cell is named in the row as the reader-facing cost, with its
precedented repair.** A `total: i64 @ 0` captured by copy freezes at creation and
a later read is stale; a write is refusable and a read is not. **Java 8 solved
exactly this by forbidding mutable capture outright**, and that is the repair to
write down rather than invent — the llm-ergonomist independently asked for it as
the load-bearing one of its three amendments. It costs spec tokens this language
does not have, and the only draft that states the rule is the +118 one.

**R4 — Part 7 item 1's capture-free narrowing (option C) is named in the same
row as the form that returns, and two things a future sitting need not
re-derive.** It carries no veto and fits at +57 of 71, and it is refused here
only by Principle 0: no measured Part 11 effect exists, and the ergonomist,
which approved it, measured that it buys **nothing** on all three of its tasks.
Recorded for whoever returns to it: **the generated name must come from module
plus source-order index**, never from the type — `selfhost/emit/synth.hero`'s
`content_key` is a function of the type's rendering, so two unnamed functions of
one type collide, and `selfhost/emit/mangle.hero:83-87` already carries the
right derivation with its reason. And **Zig accepted this exact proposal in 2020
and rejected it in 2023**.

**R5 — Part 7 item 12, inline blocks: EXAMINED AND DELIBERATELY UNPLACED**, the
shape `comptime` has at `design.md:2449-2473` (panel 039). Neither list fits, and
the paragraph must say why both were refused. **Not Part 6**: the llm-ergonomist
vetoed the form as drafted and then named a form it would accept — a caller-side
marker — so a permanent refusal would refuse something a seat said it would
approve. **Not Part 7**: that preamble claims its items lose *only* on
simplicity, and this one also loses on **locality**, which is a veto and not a
price, because the meaning of an indented line would depend on a signature that
is not on the screen and `spec:16`'s *"significant and rigid"* would stop being
true. **Three conditions return it, jointly**: a caller-side marker that makes
the construct local; a ruling on `return`, `break` and `continue` crossing the
block boundary, which item 12's own text never asks and which Kotlin, Ruby and
Smalltalk answer three different ways; and **the inlining pass priced** against
`selfhost/ir/mono.hero`'s 368 code lines, the engineer having reported the pass
as **unrun** rather than estimating it.

**R6 — three sentences in design.md are falsified by this sitting and are
corrected in the amending commit, the old text left readable** (`.claude/rules/records.md`).
*"~150 lines"* understates by ≥1.8× measured. *"~60 spec tokens"* is right only
for a sentence that states no rule; two independent drafts of the sentence that
states them read **+118** and **+141**. And *"no shared cells, no lifetime
problems, no refcount interaction"* is false: `selfhost/check/counted.hero:91`
answers `false` for `.function_ty`, so a captured `str` is a leak, and
`runtime/parts/desc.c` gives `hero_desc_func` a bare-assignment copy and
`hero_drop_nothing`.

**R7 — one repair that belongs to no option and is measured.** Add
`-Werror=cast-function-type-strict` to `selfhost/cli/flags.hero`. It fires on
exactly the cast that produced exit 139 on Apple clang 21, where
`-fsanitize=function` segfaults with no report, and it is absent from the
fourteen. Measured free today: the seed plus the runtime compile **exit 0** with
it, and so do `examples/threads/main.hero`, which hands a Heroes function to a C
thread spawn, and `examples/sqlite/main.hero`, which binds a real library.
Whether it is free on **every** program the emitter can produce is **unrun**; the
full net settles it, and this lands with its own measurement rather than inside
this sitting's commit.

**What conservative would have been, so the author can choose it** (CLAUDE.md
§4, CL-040): **option F for both items** — a dated deferral, no design.md row,
no unplaced paragraph, nothing corrected. It is cheaper by two documents. The
spec-warden objected that a *dated* condition is the mechanism §1.6 names as the
one the record shows failing, and this project's own panel 036 is an instance;
the ffi seat objected that a deferral leaves three pieces of prose reading as
true when they are not. Both objections are why it was not adopted.

**What a veto would compel.** Nothing here is vetoed: no seat vetoed E, and the
adopted resolution refuses what the vetoes refused. Had R1 tried to admit
closures, three vetoes would have blocked it and the ffi seat's would have been
unliftable in option B's shape at any price.

## Predictions to score

| Judge | Prediction | Scored at |
|---|---|---|
| compiler-engineer | landing A or B puts ≥5 files over a DECIDED ceiling and touches ≥280 arms in ≥70 modules; *"~150 lines"* understates by ≥1.8× | the first milestone that lands A or B |
| compiler-engineer | design.md's *"no refcount interaction"* is measurably false, `check/counted.hero:91` answering `false` for `.function_ty` | **restated by the coordinator, because as written it was unscoreable**: the seat asked for `grep -c "no refcount interaction" design.md` to be 0, and it is **already** 0 — the sentence wraps across `design.md:1706-1707`, and a second spelling sits at `:1686`, *"no refcounting interaction"*. Score it by reading both lines, not by grep |
| ffi-pragmatist | a middleware chain over C sockets needs zero closures at the boundary; 41 of 43 `sqlite3.h` callbacks carry a `void *` | M-web-framework |
| ffi-pragmatist | SQLite step 3 of §4.19's ladder needs no shim under any option, binding no callback at all | M-web-framework |
| spec-warden | no clause stating capture-by-copy plus the `extern` refusal comes in under +110 on cl100k_base | the next milestone that amends the spec |
| spec-warden | if B or C lands, gross headroom sits below the instrument's own printed spread | M-guide-book close |
| llm-ergonomist | A and B ≥8 pp below the status quo on first-try and ≥8 pp above on silent-wrong, n=40/arm | M-thesis-harness |
| llm-ergonomist | ≥40% of lambda bodies land in a layout the grammar refuses, absent a layout sentence | M-thesis-harness |
| llm-ergonomist | Task 3 first-try ≤50% in every arm, modal failure `a < b` on two `str` | M-thesis-harness |
| historian | with both refused, no program the §1.0 closure list requires is blocked; falsified by a `selfhost/` site that cannot be written without capture | M-check-completeness, and again at every fixpoint |
| historian | if item 1 ever ships, the first diagnostic it needs is about **escape**, not aliasing, and +118 tokens will not cover it | the first milestone that lands item 1 |
| historian | Hylo, the live counter-case, does not remove or restrict its value-semantics captures; if it does, that is evidence FOR this refusal | M-deferral-ledger |

## Author's verdict

**RATIFIED 2026-09-08** (author instruction, *"all the decisions as you
suggested, all a"*), in full, on the day the sitting was held — **so the first
Part 7 item this project ever judged is refused, and the refusal is ratified
within hours of being written.**

The alternative the coordinator named and the author declined was R5: deferring
inline blocks with a date instead of leaving them **examined and deliberately
unplaced**, which would have saved a design.md paragraph. It was declined in the
same word as the rest, and the reason the recommendation gave stands as this
sitting's: the spec-warden objected that a dated condition is precisely the
mechanism §1.6's own record shows failing, and the ffi seat objected that a
deferral leaves three pieces of design.md prose reading as true when they are
not. **What conservative would have been is preserved in this file's own
section** and the author chose against it with both objections in front of them.

What the `yes` settles: that Part 7 item 1 leaves Part 7 for a **Part 6 row**
refused on cost alone (R1); that the row's falsifier rests on compiler need, a
measured Part 11 effect and a compiled representation, and **not** on capture by
copy being hazardous, which the historian showed history contradicts (R2); that
the `@` cell is named in the row as the reader-facing cost, with Java 8's
forbid-mutable-capture recorded as the precedented repair rather than a new one
invented here (R3); that the capture-free narrowing is named in the same row as
the form that returns on a measured Part 11 effect, carrying the
module-plus-index naming rule and Zig's 2023 rejection so a future sitting
re-derives neither (R4); that item 12 becomes **examined and deliberately
unplaced** in `comptime`'s shape, with three joint return conditions (R5); and
that *"~150 lines"*, *"~60 spec tokens"* and *"no shared cells, no lifetime
problems, no refcount interaction"* are corrected in design.md with the old text
left readable (R6).

What it does **not** settle, and each has its own home: whether
`-Werror=cast-function-type-strict` joins the fourteen flags — measured free on
everything compiled here, unrun on the full net, and it lands with its own
measurement rather than inside this sitting's commit (R7); whether the spec
should spend its remaining **71** tokens on the four higher-order signatures it
never wrote, at **+63**, which is a spec amendment and therefore its own sitting;
defects **018** and **019**, filed and unrepaired; and the two live sentences
about where a function type may cross the FFI, both of which are wrong in
different directions and neither of which this sitting corrected in the tree.

A `no` on R1 reopens the option set at the point three seats vetoed, and the
compiler seat's and the ffi seat's conditions are the two texts that would have
to be produced first: one representation, compiled, with one type per signature,
release decidable from the type alone, and eight bytes at every `extern`
position.
