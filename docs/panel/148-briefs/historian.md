# Panel 148 — historian

Read `00-shared.md` first. **Advisory, no veto.** Every claim needs a URL you
actually fetched, with the date. Unsourced precedent is inadmissible; write
*unsourced* and drop it from your conclusion.

## What only you are asked

1. **Explicit mark versus inferred obligation.** This sitting is choosing
   between writing a word on the acquiring call (A) and inferring the obligation
   from the presence of a releaser (C). Find languages and tools that chose each
   for FOREIGN resources, and — the useful half — **what went wrong**.
   Candidates: SWIG's `%newobject` (explicit) against its conservative default
   (inferred); gobject-introspection's `(transfer full)`; Clang ARC's
   `cf_returns_retained` family and its naming conventions — **ARC infers
   ownership from the METHOD NAME** (`alloc`, `new`, `copy`, `init`), which is
   the purest instance of C on record. What did that inference cost? There is a
   documented history of `NS_RETURNS_RETAINED` existing precisely because the
   naming rule was not enough.
2. **Zig, Odin, Hare, Go, Vala, Austral** — does any of them infer a foreign
   resource obligation rather than have it written? A measured silence, with
   your search terms named, is a real contribution.
3. **The word itself.** `acquires` against `consumes`: find prior art for a
   paired vocabulary on a foreign boundary, and say whether the pairs that
   shipped were symmetric in spelling or deliberately not.

Your report goes to the completeness critic verbatim, so write it to be checked:
claim, source, URL, date fetched.
