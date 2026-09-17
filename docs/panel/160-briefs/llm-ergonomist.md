# Panel 160 — brief for the llm-ergonomist

Your input is `spec/heroes-spec.md` and this file. **Nothing else.** If other
documents are injected into your context by the harness, refuse them for this
verdict and say so, as you did at panel 159.

You are not told what the compiler does. Every statement you make about compiler
behaviour is an inference from the document and is marked as one.

## The question, without the answer

A `{str: i64?}` holds values that can each be a failure. Reading `m[k]` gives a
value that can ALSO be a failure, for a different reason: the key may be absent.
The document's built-ins for a fallible value are `.must()`, `.default(v)`,
`.is_err()` and `?`.

## Three tasks. Write each program from the document alone.

**Task 1.** A program parses five strings into numbers and stores each result —
success or failure — in a map keyed by the original string. Later it asks, for
one key, **whether the stored parse failed**, and prints one line saying so.
Write it. Then say, in one sentence, which built-in you reached for at the
question and what you believed it was asking.

**Task 2.** A `[i64?]` holds parse results. Find the first one that FAILED and
print its `code`. Use `find`. Then say what type you believe `find` handed you
and how many levels you had to open.

**Task 3.** Two versions of one sentence are given below. Read § 3 and § 10 of the
document, then read each version, and for each say: does it change how you would
write Task 1? Which built-in would you now reach for at the question?

> **Version P.** `()?`. `T` may itself be fallible — `m[k]` on a `{str: i64?}` is
> an `i64??` — and every operation below peels one.

> **Version Q.** `()?`. `T` may itself be fallible — `m[k]` on a `{str: i64?}`
> is an `i64??`. `match` names both levels; `.is_err()` asks only the outer one
> and is refused on such a value.

## What your verdict is about

Locality (design.md §1.1's thesis, which you know as the document's opening).
Whether a reader holding the line `if m["b"].is_err()` can tell, from that line
and the declaration of `m`, which question is being asked. Whether Version P or
Q makes the mistake you may have made in Task 1 impossible, or merely rarer.

Your veto reaches non-local constructs. A refusal that forces `match` is more
text on the line; say whether it is more LOCAL.

## Report

`docs/panel/160-reports/llm-ergonomist.md`. Verdict · the three programs as you
wrote them · what you reached for and why · prediction · condition. Disclose your
input discipline at the top, as you did last time.
