# Panel 150 — brief for the llm-ergonomist

## Your input, deliberately narrow

Read **`spec/heroes-spec.md`** and nothing else. Do not open `docs/`, do not open
`selfhost/`, do not open the shared brief, do not open any other file. Your
verdict is an experiment about what a reader of that one document believes, and
it stops being one the moment you learn what the compiler does. You have a veto
on non-local constructs.

## Task 1 — the untyped pointer

```
extern "pool.h"
    function opaque_open(n: i64) -> ptr
    function opaque_close(p: ptr consumes)

function main()
    p = opaque_open(n: 1)
    print(0)
    _ = p
```

1. Does this compile? Quote the sentence you used.
2. Does the program owe anything after `opaque_open`? Say what you believe and
   what you relied on.
3. If it ran to completion at exit 0 having leaked, would you say the document
   misled you? Quote what you relied on.
4. **The design question, and answer it as a reader and not as a compiler
   writer**: `§ 3` says `ptr` is *an opaque pointer*, one type for every C
   pointer in the language. If the language demanded a mark on every `ptr` a
   group hands back whenever that group consumes any `ptr`, would that help you
   or bury you? Write the binding both ways and say which you would rather read.

## Task 2 — the array of handles

```
extern "pool.h"
    record Slot tag Slot
    record Four
        a: Slot[4]
    function slot_close(s: Slot consumes)
    function four_make(n: i64) -> Four acquires slot_close

function main()
    f = four_make(n: 1)
    slot_close(f.a[0])
    slot_close(f.a[1])
    slot_close(f.a[2])
    slot_close(f.a[3])
```

1. Is this legal? Quote what you used.
2. **How many releases do you believe the program owes** — one, or four? Quote
   the sentence that decides it. If the document cannot decide it, say so
   plainly.
3. Suppose C filled only two of the four slots and left two null. How would you
   know, from the document?
4. Would you rather the language **refused** this shape than left the question
   open, and why? A refusal costs a reader a program they cannot write; an open
   question costs them a program that compiles and is wrong.

## What to return

Your answers, the sentences you quoted verbatim, a verdict on each task, and
**one falsifiable prediction** about what a model writing these bindings will get
wrong, with how it could be tested.
