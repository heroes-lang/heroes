# The collision was not in the type, it was in the rendering

2026-09-16. Panel 158, on a doubly-fallible value: `i64??` is tracked by the
checker, printed by the compiler, refused by the parser. Four repairs were on
the ballot, two seats cast vetoes on two different ones, and the resolution
adopted is none of the four.

## What the vetoes were, and why only one was negotiable

The llm-ergonomist vetoed **flattening** on locality, with the specification's
own example line as the exhibit: `print(m["b"].default(0))` on a `{str: i8?}` is
loud today and, flattened, prints `0` for both *key absent* and *key present
holding a failure* — a distinction the program was written to make.

The compiler-engineer vetoed **making `T??` writable**, and it built the patch to
earn the right: under it, `heroes fmt` on `v: i64?? ?` prints **`error: this is a
compiler bug`** at exit 2, because the type renderer emits three levels as
`i64???` and the lexer max-munches that as the hole token. On the seed the same
file is a clean parse error, so the patch **creates** the breakage — §4.15, in
the one tool whose failure makes every diff untrustworthy.

The first veto is about what a program can express. The second is about a
perimeter, and its own words say so: *"it does not reach option 3 with the lexer
half priced and landed."*

## The route nobody named dissolves the second

**The collision is not intrinsic to legalising the type. It is intrinsic to
rendering `?` adjacent to `?`.** The `Prefix` production has no `"(" Type ")"`
alternative; add one and `(i64?)?` and `((i64?)?)?` are writable at any depth,
with no lexer change and a formatter that round-trips. The llm-ergonomist had
recorded the missing alternative while looking for an escape hatch, and did not
see what it was for.

That is what the sitting adopted, and what it owes before landing is named: the
canonicalisation duty `fmt` acquires when `(i64)?` becomes writable, and the walk
of every tool that re-prints a program.

## The sitting had a zero-veto answer and left it off its own ballot

The critic found it. The ffi seat's amendment — move the `pointer_element`
refusal from the emitter into the checker — and the compiler seat's condition are
**two different missing halves of the same option, and they are additive**.
Option 3 with both carries no veto and one objection. Nobody scored that version,
because each seat wrote its own half and no seat read the other's.

## And the framing was wrong, which the critic showed by running a program

*Consistency, not safety* has two boxes. This compiles at exit 0 today, before
any repair:

```
if m["b"].is_err()
    print("this reads as: the stored value failed")
```

`.is_err()` peels the **outer** level and answers *the key was not there*. It is
neither a safety failure nor a consistency wart: it is a plausible mistake that
compiles, runs, and answers confidently the wrong question — which is
CLAUDE.md's opening sentence falsified. It is **defect 050**, and of the four
repairs weighed, only the one that protects zero programs would have closed it.

## Two numbers that decided prices

**A fallible has no C ABI at any depth**, measured: `error[ffi_type]` refuses it
as an extern parameter, a result and an extern record field. So every repair
costs bindings zero and the question is wholly internal.

**And the corpus count Swift used cannot be borrowed.** SE-0230 bought its
flatten by enumerating 613 sites and finding *zero* that used the distinction it
destroyed. The equivalent count here returns **two files, both written to probe
this refusal**. That is not zero-of-many; it is an empty corpus, and it licenses
nothing — the difference the critic insisted on and the synthesis records.

## What the specification owes, and it is not what the warden priced

The spec-warden argued the document is already right — value yes, spelling no —
and that a qualification would therefore cost nothing because there is nothing to
correct. The first half survives: § 10 rules on the type an expression **has**,
§ 3 on what a program **writes**, and those are different claims. The second does
not. `grep` over the specification for any phrase saying a type may have no
spelling returns **zero**, and `??` appears nowhere in it. **The relation between
the two halves is stated zero times**, so a sentence here is a first statement
and not a repetition — and the warden's own **+9** merge is the price its
argument supports.
