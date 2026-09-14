# Panel 149 — brief for the llm-ergonomist

## Your input, and it is deliberately narrow

Read **`spec/heroes-spec.md`** and nothing else. Do not open `docs/`, do not open
`selfhost/`, do not open any other file in this repository. Your verdict is an
experiment about what a reader of that one document believes, and it stops being
one the moment you read what the compiler actually does. If you find yourself
reasoning about an implementation, stop and go back to the text.

You have a veto on non-local constructs.

## Task 1 — read the program and say what you believe

Here is a Heroes program. Answer from § 13 of the document alone.

```
extern "pool.h"
    record Slot tag Slot
    record Pair
        s: Slot
        k: i64
    function slot_close(s: Slot consumes)
    function pair_make(n: i64) -> Pair

function main()
    p = pair_make(n: 7)
    print(slot_value(s: p.s))
    slot_close(p.s)
```

1. Does this program compile? Answer yes or no, then quote the sentence you used.
2. If a compiler rejected it, what would you expect the message to be about?
3. If a compiler accepted it and the program then failed at run time, would you
   say the document had misled you? Quote what you relied on.

## Task 2 — blind A/B on one sentence

Two candidate sentences for § 13. They are labelled only A and B, and you are not
told which is the current text.

> **A** — `acquires sqlite3_finalize` after a handle result or `@`
> out-parameter says the call begins that handle's life and names the one that
> ends it, which the program owes it.

> **B** — `acquires sqlite3_finalize` after a result or `@` out-parameter that
> reaches a handle — directly, or through a record's fields — says the call
> begins that handle's life and names the one that ends it, which the program
> owes it.

For each: after reading it, what do you answer to Task 1's question 1? Which
sentence would leave a reader writing the binding for `pair_make` above without
help? Say which you would keep and what it costs in words.

## Task 3 — the shape the document may not cover at all

```
extern "pool.h"
    record Slot tag Slot
    record Conn tag Conn
    record Two
        s: Slot
        c: Conn
    function slot_close(s: Slot consumes)
    function conn_close(c: Conn consumes)
    function two_make(n: i64) -> Two acquires slot_close
```

`two_make` hands back a record carrying two handles of two types, and the mark
names one releaser. From § 13 alone:

1. Is this legal? Quote what you used.
2. What do you believe the program owes after calling `two_make`?
3. If the document cannot answer, say so plainly and say what sentence would.
   Then say whether you would rather the language **refused** this shape than
   left the question open, and why — a refusal costs a reader a program they
   cannot write; an open question costs them a program that compiles and is
   wrong.

## What to return

Your answers, the sentences you quoted, and **one falsifiable prediction** about
what a model writing a binding will get wrong, with how it could be tested.
Locality is your standard: a rule a reader must hold two files open to apply is
one you should say so about.
