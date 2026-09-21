# Panel 172 — brief for the historian

Read `docs/panel/172-briefs/00-shared.md` first. You are advisory, no veto, and
**every claim you make is web-verified with its URL and the date you read it**:
an unsourced precedent is inadmissible, and this seat is the one most prone to
inventing them.

## The question

A language binds C and wants a word on a pointer PARAMETER meaning *the callee
frees, or takes ownership of, what it is handed*, so that the language's own
bytes (a lend, a copy it will free itself) are refused there. Two designs are on
the table: **A**, one word for the freeing case and silence means *keeps*; **B**,
three words (reads-and-lets-go, keeps-never-frees, frees) and silence admits
nothing the language made.

## What to verify, with the source beside each

1. **GObject-Introspection's transfer annotations**: `(transfer none)`,
   `(transfer container)`, `(transfer full)` on parameters and returns. What is
   the DEFAULT for an in-parameter when nothing is written, and what did that
   default cost — are there recorded bugs from a missing `(transfer full)` on a
   parameter? This is the closest shipping instance of route B's vocabulary.
2. **Clang's ownership attributes**: `__attribute__((ownership_takes(...)))`,
   `ownership_holds`, `ownership_returns` — read by the static analyzer's
   MallocChecker. Since when, and does the compiler proper (not the analyzer)
   ever refuse a call on them? Also Clang's 2026 `noescape`-on-pointers RFC
   from panel 171's brief, if it touches freeing.
3. **Microsoft SAL**: `_Frees_ptr_`, `_Frees_ptr_opt_`, `__drv_freesMem`.
   Which tools read them and what happens when the annotation is missing.
4. **Swift's C importer**: how `CF_RELEASES_ARGUMENT` / `cf_consumed` and
   `ns_consumed` are imported (the parameter *consumes* the reference); is the
   word literally *consumed*? Swift 5.9's `consuming` parameter modifier — same
   word, different layer; note the overlap or the collision.
5. **Rust FFI conventions**: `CString::into_raw`/`from_raw` and `Box::into_raw`
   as the give-away route (the language allocates, C frees only through a
   function the language provides) — this is `docs/measurements/037`'s R5. Is
   there any Rust-side ANNOTATION on the extern declaration, or only the
   convention? Say which.
6. **Nim's `sink`**, **D's `scope`/`return scope`**, **Vala's `owned` on
   parameters** (panel 171's brief already found Vala's `owned` on parameters
   as a positive-polarity ownership word): which of them mean *callee frees*
   and which *callee keeps*?
7. **The word itself**: which shipping systems say *consumes*/*consumed* for
   *the callee frees or takes ownership* (Swift's importer, CF's
   `CF_CONSUMED`, GLib?), and which say *transfer full*, *sink*, *owned*. Was
   any of them renamed later, and toward what?
8. **The default question, directly**: does any shipping binding system REFUSE
   an unannotated pointer parameter outright (route B's silence), or do all of
   them read silence as one of the cases? If one does, name it and what it
   cost its users.

## Report

Return the report as text (you have no write tool; the coordinator writes it
verbatim to `docs/panel/172-reports/historian.md`): verdict (advisory) · each
precedent with URL and read date · what the precedents say about the WORD and
about the DEFAULT · one falsifiable prediction about how binding authors will
use the word, naming an instrument in this repository that can score it.
Corrections to the shared brief are welcome and are what this seat is for.
