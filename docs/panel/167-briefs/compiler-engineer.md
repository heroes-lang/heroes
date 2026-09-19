# Panel 167 — brief for the compiler-engineer

Read `00-shared.md` first. It carries the reproducer, the refusals that already
ship, and every number this file does not repeat.

**Build in a COPY.** `cp -r <tree> <scratch>/tree && cd <scratch>/tree && rm -rf
archive build target heroes`, then `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` — about 4 seconds. Rebuilding from `selfhost/` is
about 100 seconds and is not needed to measure anything here. Never
`archive/bootstrap-rs/`: nothing builds it.

## What this seat is asked

**What each of the four routes costs the compiler, in files and lines, and
whether any of them touches the core** — Part 5's seven constructs, the IR, the
descriptor pass, the ownership pass, the backend. Route A (the field lease) is
the one that looks like it might: it needs storage the program owns and a
release, and the `cstr` lease already has both. Read `check/leasing.hero` and
say what a field lease would share with it and what it cannot.

**And price the thing nobody has priced: where do the copied bytes live?** The
`cstr` lease copies into an allocation the runtime owns. A field lease's bytes
are a fixed run of C's own width. Is that the same allocation, a different one,
or a `[u8]` the language already has? Say what the emitter would write for
`p: ptr @ s.name.pin()` and `end_pin(@p)`, or say the route is not carriable
and why.

**Say plainly which routes the compiler can carry and which it cannot**, and
under what condition each verdict would change.

## Pointers, each a live path in this tree

- `selfhost/check/leasing.hero` — the `cstr` lease's whole rule.
- `selfhost/check/lending.hero` — the lend's position rule and its four clauses.
- `selfhost/check/lend_extent.hero` — route C's mark, landed this afternoon.
- `selfhost/emit/field_lend.hero` — what a lend emits: one cast at the use site.
- `selfhost/emit/bytes_text.hero` — the other direction, which COPIES.
- `selfhost/handles.hero` — the live-handle set and the abort at `main`, which is
  the mechanism a lease's own abort shares.
- `runtime/heroes_runtime.h` — what the runtime already offers.

## What would make your verdict wrong

Name it. If you cannot construct the program that falsifies you, say the claim
is about your vocabulary rather than about the world.
