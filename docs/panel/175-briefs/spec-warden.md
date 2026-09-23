# Panel 175 — spec-warden

Read `00-shared.md` first. Your seat holds design.md §1.2's cost formula and
§1.6's budget, Principle 0's burden of proof, and a veto on a budget breach.
Work in `<scratchpad>/175-spec-warden/`.

## The counts, measured 2026-09-23 while this brief was written

`./heroes measure spec/heroes-spec.md`: **6212** on the vendored maximum (a
lower bound, not the reader's tokeniser). `./heroes measure
spec/heroes-spec.md --refresh`: **8270** on `claude-opus-5`, taken 2026-09-23,
digest `5b96ebe84dac6784`. Ceiling **10240**, headroom **1970**, of which the
FFI floor mortgages **60** (panel 030 R3).

`--refresh` refuses every path but `spec/heroes-spec.md` and `CLAUDE.md`, so a
draft is priced by applying it to that path **in your copy** and refreshing
there. It needs `ANTHROPIC_API_KEY`: `. <trunk>/.env` in your shell, where
`<trunk>` is the path your prompt gives; print `${#ANTHROPIC_API_KEY}` if you
need to know it loaded, never the value. If the instrument is unavailable, say
so and write any vendored number as a **lower bound, in those words**
(`.claude/rules/spec-shape.md` § How a change to the document is made).

## The sentences to price

The § 13 sentence Question 1 is about, as it stands:

> `acquires sqlite3_finalize` after a result or `@` out-parameter reaching a
> handle says the call begins that handle's life and names the one that ends
> it, which the program owes it. The live handles are a set, so giving one back
> twice aborts on its own.

Candidates, each priced alone on the real count:

1. **Q1, a clause**: *… names the one that ends it, which the program owes it,
   and ending it with another aborts.*
2. **Q1, nothing**: the sentence already promises it, and the repair makes the
   compiler meet the document.
3. **Q2, a sentence** near *Bytes C owns come from its own allocator, with
   their disposer*: *A pointer C hands out for the program to give back is a
   handle; a `ptr` given back twice is a double free nothing catches.*
4. **Q2, nothing.**

Judge each against the payment rule — a named removal, or a registered
prediction naming an instrument that exists — and say whether Principle 0 is
met: the compiler needs it, or it serves the thesis with a measured effect, or
design.md §1.12 (robustness, rank 3 of CLAUDE.md § Precedence) carries it. A
merge into a sentence already there beats an addition (panel 122); try one.

Give a verdict, the cost, one falsifiable prediction with the milestone at which
it is checkable, and the condition under which you would change your mind.
