# 038 — M-discard-refusal: `_ =` stops swallowing a failure

**Closed** 2026-09-08 · tag `m-discard-refusal` · four steps, one full sitting
(`docs/panel/118`) · chain row 39

## Goal

Close the one place a failure can be dropped without the type system noticing.
`_ = e` exists so a value can be thrown away on purpose — panel 003 put it in
the language for exactly that, and it is right for a value. For a **failure** it
is wrong: the error goes with the value, nothing in the program says so, and the
run exits 0. The witness the milestone was filed on:

```
function main()
    _ = risky(0 - 1)
    print("still here")
```

printed `still here` and exited 0 on the morning of 2026-09-08.

The author's verdict on the rule was already given (`/decide` answer `3a`,
2026-09-03). What the milestone was for was the **shape**, and the sitting
changed it in four places.

## What surprised

**A refusal can close a door beside an open window, and that is worse than no
door.** The sitting adopted refusing a discard of a type parameter, on the ground
that it costs zero migration sites — a true measurement, and the wrong question.
Run instead of read, `function drop<A>(_: A)` swallows a failure with no discard
statement anywhere, so the refusal relocates the hole. Sent back with that fact,
the blind reader showed the relocation goes one step further: `_ = [x]` is legal,
means the same, and works on a **known** fallible, so `_ = [make_dir(path: p)]`
defeats the rule itself. A refusal that manufactures the idiom defeating it
teaches the escape on its way out. So the boundary is stated in the document
instead, and it was owed regardless of generics: `_ = args_checked()` drops a
whole `[str?]` in one line, using nothing but the spec's own vocabulary.

**Two seats measured the wrong gate, and the premise they shared was the one the
resolution rested on.** *The existing vocabulary reaches all 60 sites* was
established with `heroes check`. Through `heroes build`, `_ = f().is_err()` emits
`variable set but not used` at **every** payload type, against a suite that says
of itself THERE IS NO EXEMPTION. Five seats reading the same brief would have
agreed at the cheaper gate and shipped a hatch that fails the net; differentiated
inputs are what made the disagreement carry information.

**The option set is a measurement too, and the convener is not exempt from
that.** Asked *may a `.binary` lose its destination*, the compiler seat answered
correctly and in detail, and the better answer had been in the tree since
2026-08-16: declare the temporary `__attribute__((unused))`, the idiom the
sibling half of that same defect class already wrote for a named binding's slot.
The narrow repair was built, measured clean — zero warnings, four dead
assignments gone, byte-identical on a second emission, three suites green — and
thrown away, because attacking the shapes beside it showed it closed **two of
four**: an equality on a `str` and on an array still warned, their assignment
being a runtime call rather than an operator. The question was wrong, not the
answer.

**A rule that exists to make a decision visible must not ship a repair that takes
the decision.** The same line's three repairs print **1**, **9** and a panic. So
`?` is not meaning-preserving, `.is_err()` is meaning-preserving and preserves
the bug, and `.must()` ends the program. Every fix is a guess, and that is the
ruling rather than a gap.

**`.must()` looked like the honest repair for a test fixture and is not.** A
failed fixture should be loud, so `.must()` reads well — until one notices that
it **aborts**, and an abort inside `heroes test` kills the whole run instead of
failing one case. What ships is `assert !e.is_err()`, which turns a swallowed
fixture into a test failure. Thirty-six of the sixty-three repairs are that
shape, and none is `.must()`.

**A count taken from a clever method inherits that method's blind spot.** The
census — strip `_ = ` from every discard and read the type the compiler names —
found 60 sites. It could not see two discards written inside test-program string
literals, and it never looked at the golden corpus, where there was one more.
Sixty-three. What found the missing two was the compiler's own suite going red.

## What broke and why

**A cascade, caught by an existing golden.** The refusal fired on a line already
carrying `cannot_infer` and printed the type as `??`. Cause: a `T?` whose payload
is poisoned **is** `.fallible`, so a comment written an hour earlier — claiming
poisoning "falls out by itself, because a poisoned type is not `.fallible`" — was
false when it was written. Fix: both refusals ask `table.poisoned` first, which
recurses into the payload. This is `.claude/rules/module-shape.md`'s own class: a
premise about the world expires silently while the argument around it goes on
reading as correct. It expired in under an hour, and the golden that caught it
was `check/type-parameter-needs-context`, written for something else entirely.

**The new rule masked an older diagnostic, caught by the annotations suite.**
`ffi_freer_type` is an emitter-stage refusal — clang is what refuses a wrong
freer — so a checker error stops the pipeline before the probe runs, and
`fixedbugs/ffi-a-freer-that-cannot-free-a-string` stopped exercising what it was
written for. Fix: answer the discard in that case's own program, with a comment
saying why. The FFI diagnostic came back at the line its annotation names.

**A substring helper that aborts on non-ASCII text, caught by the compiler's own
tests.** `panic: string slice splits a character`, twice, from a helper written
in the shape of the ten others in this compiler: slide a window, slice, compare.
Every one of those ten searches emitted C and is safe by a premise nobody wrote
down; a diagnostic **message** carries `—` and `…`. The repair compares bytes,
which cannot split a character. The other ten are untouched and the premise is
now written down in the eleventh.

**An off-by-one in the number that exists so nobody has to do arithmetic.**
Defect 017's repair printed *"71 usable"* where the largest green addition is
**70**, because the check's condition is `>=` and the subtraction assumed `>`.
Caught by comparing the printed number against the check rather than against the
intention. What shipped states both sides of the comparison and leaves no
arithmetic at all.

**An apostrophe inside a shell single-quote, caught by the test that was meant to
make the check fire.** The contract's own sentence carries one, and inside
`echo '…'` it closes the quote and eats the rest of the line. Two of that test's
own author's mistakes in a row, both found by the test.

## What landed, and what carried forward

The rule closes **two positions** — `_ = e` on a written `T?`, and a `_`
parameter written one — and states the third rather than pretending to close it.
The refusal reads the discarded expression's **outermost written type**, so
`_ = f()?` and `_ = f().is_err()` stay legal and a fallible inside a container
does not. No fix is `certain`; three guesses name their consequences, and `?` is
offered only where the enclosing function can fail, `.default(v)` only where the
payload is a value somebody could write.

The third half of the proposal was in nobody's owed list: `discarded_value`'s
`_ = ` fix is **withdrawn** on a fallible rather than downgraded, because a guess
that cannot compile is still wrong, and `heroes check --apply --in-place` was run
beforehand and wrote exactly the form the rule now refuses.

**The measurement the milestone is for.** `heroes mutate --operator
drop-question --survivors`: **236 mutants, 236 killed, no survivors**, against 9
of 84 in measurement 014. The column beside it is the one worth keeping:
`--permissive` kills 193, so **43 of the 236 are caught only by the thesis
rules** — a direct reading of what design.md §1 buys on this operator, on a
corpus of 118 programs.

Two instruments were repaired on the way, and both were about a number this
project trusts. Defect 017: `heroes measure` told a session it had 193 tokens of
spec headroom where the net goes red at 132, and shouted BREACH at a contract 111
tokens inside its own ceiling. Each file is now judged by the ceiling that
governs it, and `spec/spendable` asserts the tool's printed sums are the
harness's computed ones.

And the author's rule landed with an executor rather than as prose: **a milestone
is tagged only over a clean list**, zero open defects and nothing open in
`DECIDE.md` that is not a `panel NNN` ratification. `records/tagged` reads the
newest tag's own commit, so an open defect mid-milestone stays legal and a tag
over one does not.

**Carried forward**, and each one is filed rather than remembered: the `ignore`
built-in, which two seats argued for and which is blocked by
`selfhost/check/builtins.hero` sitting at 374 of a DECIDED 374 — a ceiling raise
is a sitting of its own; the class the rule does not reach, a fallible stashed in
a container and never inspected, which needs reachability rather than a type
judgment; the ten substring helpers whose premise is now written down in one
place only; and the signature smell underneath ten of the repaired sites, where a
function hands a fallible to callers that cannot use it. The four open items in
`docs/work/DECIDE.md` are this sitting's ratification and three findings the
seats named rather than papered over.
