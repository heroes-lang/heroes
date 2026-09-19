# Panel 165 — brief for the llm-ergonomist

**Your only input is `spec/heroes-spec.md`.** Read it in full. Do not read any
other file in this repository, and do not look for design documents: your verdict
is an experiment about what a reader of the specification does, and material the
reader would not have would spoil it.

## The change under consideration, as a specification diff

Today `spec § 13` says a C function's parameter may be a number, `bool`, `str`,
`ptr`, `cstr`, a function type, or a `record` declared in the same group. A fixed
array of a number — `i8[8]` — is a **field** type only, and the section already
says how a field's bytes reach C:

> `f.ptr()` lends a binding's field to a `ptr` parameter the call gives the
> extent to, and C may write back through it.

The proposal adds one admissible parameter type: **a fixed array**, written
exactly as the C header spells it, with the compiler checking that the argument's
extent matches.

```
    # today — the extent is stated by the caller
    extern "slots.h"
        record Slot tag slot
            name: i8[8]
            id: i32
        function arr_len_p(p: ptr, n: i64) -> i64

    print(to_str(arr_len_p(p: t.name.ptr(), n: 8)))

    # proposed — the extent is stated by the declaration and CHECKED
    extern "slots.h"
        record Slot tag slot
            name: i8[8]
            id: i32
        function arr_len(s: i8[8]) -> i64

    print(to_str(arr_len(t.name)))
```

Both forms would exist; the proposal adds the second.

## Three tasks. Do them before you form a view

**Task 1 — write the program.** Using only the specification, bind this C header
and print the length of the name of a slot the library hands you:

```c
struct slot { char name[8]; int id; };
long        arr_len(const char s[8]);
struct slot slot_make(void);
```

Write it **twice**: once as the specification stands today, once as it would
stand with the proposal. Report whether each compiled on your first try, and if
not, what you got wrong and which sentence of the specification would have
prevented it.

**Task 2 — the blind comparison.** Here are two declarations of one C function.
Say, for each, what a reader learns about the argument they must supply, and what
a reader could get wrong:

```
    A:  function copy_name(dst: ptr, n: i64) -> i64
    B:  function copy_name(dst: i8[8]) -> i64
```

Then say which one a reader is more likely to call correctly **without opening
the C header**, and why.

**Task 3 — the trap.** Suppose a reader writes form B for a function whose header
on *their* machine spells the buffer as 8 bytes, and the program is later built on
a machine where the same header spells it 20. The compiler checks the 8 and is
satisfied. Using only the specification, say whether anything in the language as
documented would warn that reader — and if the specification offers a different
way to name a size that comes from the header, quote it.

## What your verdict must carry

A verdict (approve / object / veto), the specification sections it rests on, and
**one falsifiable prediction** stated so a later sitting can score it — a
compile-on-first-try rate, an error rate, a count of readers who choose one form.
Say what would make you wrong.

You hold a veto on a construct whose meaning cannot be worked out from the place
it is written.
