# Panel 167 — brief for the historian

Read `00-shared.md` first. **Verify every precedent with web search**: unsourced
precedent is inadmissible, and this seat is the one the procedure names as the
most hallucination-prone. Date every claim and link it.

## What this seat is asked

**How have other languages answered *how long may a foreign function keep a
pointer we gave it*?** Not *how do they pass a pointer* — that is settled here —
but what stops the callee keeping it past the call.

Six places worth checking, and name any you find that these miss:

1. **C# / .NET**: `fixed` statements and `GCHandle.Alloc(..., Pinned)` — the
   distinction between pinning for a call and pinning for a lifetime, and what
   the runtime does with an unpinned handle.
2. **Rust**: `&'a T` across an `extern "C"` boundary, and what the language does
   NOT promise there; `Box::into_raw` and the leak-on-purpose idiom.
3. **Go**: **cgo's pointer-passing rules**, which are the closest match in the
   survey — *"C code may not keep a copy of a Go pointer after the call
   returns"* — and, crucially, **how it is enforced**: the `cgocheck` runtime
   check, what it catches, what it misses, and whether it was ever turned off by
   default.
4. **Swift**: `withUnsafePointer` / `withUnsafeBytes` and their documented
   escape rule, and whether the compiler enforces it or the documentation asks.
5. **Ada**: `pragma Convention` and access-type accessibility levels, which
   refuse some escapes statically.
6. **Java / JNI**: local vs global references, `NewGlobalRef`, and what the
   programmer owes.

## The question the sitting needs you to answer

**Is there a language that enforces this statically, and at what cost?** If
every survivor documents the rule and checks it at run time or not at all, say
so with the sources, because that is the finding and it decides the sitting.

And: **has any language shipped a pin-and-release pair for exactly this** — a
copy the foreign side may keep, whose release the program owes? Name it, date
it, and say whether it survived.
