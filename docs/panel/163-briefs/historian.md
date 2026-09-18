# Panel 163 — historian

Read `00-shared.md` first. Advisory, no veto. **Unsourced precedent is
inadmissible**: every date, name, version and claim carries a URL you fetched in
this session. Today is 2026-09-18.

## The question

A language with value semantics and a rule that every binding is initialised
binds a C API that FILLS a caller-owned struct — `uname`, `stat`, `getrusage`,
`sigaction`. The struct has a 256-element array field, so writing a literal
means writing 256 elements.

**How do such languages let a program produce that struct, and what did each
choice cost?**

## What to verify, in this order, and stop if time is short

1. **The zero-value route.** Go's `var uts unix.Utsname` and C's
   `struct utsname u = {0}`. Find the spec text for each. Then the question that
   matters: **has a zero default at an FFI boundary ever been reported as a
   defect** — a struct a C function reads before writing, where a zero was
   wrong? `addrinfo` hints and `sigaction` are where to look.

2. **The uninitialised route.** Zig's `= undefined` and Rust's
   `MaybeUninit`. Find when `MaybeUninit` stabilised and what it REPLACED —
   `mem::uninitialized` was deprecated for a reason, and that reason is exactly
   this sitting's route 3. Quote the deprecation's own words if you can reach
   them.

3. **The context-typed constructor.** A call whose result type comes from the
   context rather than its arguments: Swift's `init` with contextual type
   inference, Rust's `Default::default()` under an expected type, Ada's
   `(others => 0)` aggregate. Find one that a language ADDED after shipping
   without it, and what it cost to add.

4. **Any language that REFUSED** to let such a struct be built, requiring it to
   come from the library. If you cannot find one, say so in those words with
   what you searched.

## What is NOT wanted

A history of struct initialisation. What is wanted is what happened at this
boundary and what each choice cost the language that took it.

## Deliver

Verdict (advisory) · every claim with its fetched URL · what each precedent cost
· a falsifiable prediction · and explicitly, anything in `00-shared.md` you found
misstated.
