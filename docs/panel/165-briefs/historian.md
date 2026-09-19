# Panel 165 — brief for the historian

**Read `docs/panel/165-briefs/00-shared.md` first.** You are advisory and hold no
veto. **Every claim you make must be verified by web search and carry its source
URL**; this seat is the most hallucination-prone in the panel and unsourced
precedent is inadmissible. Where you cannot verify something, say *unverified*
in those words — panel 162's historian did exactly that about a C11 paragraph
number and it was the right answer.

## Your question

The proposal lets a foreign-function declaration state an array parameter's
**extent**, and has the binding language check it — where C itself throws that
extent away (C11 §6.7.6.3p7: a parameter of array type is adjusted to pointer).

**Survey how other languages that bind C handle a C parameter written
`T name[N]`.** Panel 164's historian surveyed nine languages on a neighbouring
question — Rust, Zig, Go, D, Odin, Swift, Ada, Cyclone, Nim — and found eight of
nine refuse the automatic decay. This is a different question: not *may an array
decay into a pointer parameter*, but **does the binding carry the extent at all,
and does anything check it?**

For each language, answer three things with a source:

1. **Does the FFI declaration have a place to write `N`?** Rust's `bindgen` on
   `void f(char b[8])`, Zig's `@cImport`, Go's cgo, D's `extern(C)`, Swift's
   importer, Ada's binding pragmas, Nim's `importc`.
2. **If it does, is `N` checked against the argument, or is it decoration?**
3. **What happens to a caller who gets `N` wrong?**

## The fact that makes this more than taxonomy

Measured on two real platforms while this brief was written:

```
Darwin  _stdio.h:289   char *tmpnam(char *_LIBC_COUNT(L_tmpnam));
glibc   stdio.h:211    extern char *tmpnam (char[L_tmpnam]);
L_tmpnam = 1024 on Darwin, 20 on glibc
```

The same standard function is spelled as a pointer on one platform and an array
on the other, and the extent is a different number. So a binding that mirrors
*the header in front of the author* is not portable.

**Find out how other ecosystems handle exactly this.** Two threads worth pulling,
and say if they lead nowhere:

- **Generated versus hand-written bindings.** A generator reads the local header,
  so it bakes in the local number. Do Rust's `libc`, Go's `syscall`/`x/sys`, or
  Zig's std handle `L_tmpnam`-class constants per platform, and how?
- **Annotation-based extents.** Apple's `_LIBC_COUNT`/`_LIBC_SIZE` (bounds safety
  attributes, `__counted_by` / `__sized_by` in recent clang) put the extent on a
  pointer instead of in the array brackets. Is that a live direction? Does any
  binding language read those attributes today? This is the route no sitting in
  this project has listed and the panel would like to know whether it exists in
  the world.

## What your report must carry

A table, one row per language, each cell with a source URL and a
verified/partially-verified/unverified marker. Then a verdict in the advisory
sense: what precedent suggests, and **one falsifiable prediction** a later sitting
could score.

You have no write tool; your report is written out verbatim by the coordinator to
`docs/panel/165-reports/historian.md`, so write it as a finished document.
