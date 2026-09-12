# M-closures-verdict — CLOSED 2026-09-08, and what a later milestone must honour


The retrospective is [039](journal/039-closures-verdict.md) and the reasoning is
`docs/panel/119-the-warning-that-does-not-fit.md`. What stays here is only what
binds work that has not happened yet.

**Both items are refused and neither is on Part 7 any more.** Closures with
capture are a **Part 6 row**, refused on cost alone, and the row carries the
falsifier: a program on the §1.0 closure list, or a measured Part 11 effect, that
a named top-level function cannot express — *together with* a representation,
compiled, in which a capturing closure has one type per signature, lets the
ownership pass decide release from the type alone with no runtime descriptor
pointer, and keeps eight bytes at every `extern` position. Inline blocks are
**examined and deliberately unplaced**, in `comptime`'s shape, with three joint
return conditions: a caller-side marker that makes the construct local, a ruling
on `return`/`break`/`continue` crossing the block boundary, and the inlining pass
priced against `selfhost/ir/mono.hero`'s 368 code lines. **Part 7's numbers 1 and
12 are struck and never reused**, because they are cited across the record.

**What this binds downstream, and M-web-framework is the one that has to read
it.** That entry planned middleware *"as a chain of functions, because v1 has no
closures — or in whatever shape M-closures-verdict rules"*. It ruled: the shape is
named top-level functions, and the framework's own question — whether a framework
with no closures reaches Echo's level or collapses into `net/http` itself — is
now answerable rather than contingent. The ffi seat's prediction is scored there:
**a middleware chain over C sockets needs zero closures at the boundary**, because
every callback it would bind carries a `void *` context (41 of 43 in `sqlite3.h`),
and raylib's ten carry none at all while two use the callback as an **identity**
for removal, which a struct-valued closure does not have.

**And the capture-free narrowing is the form that returns**, named in the Part 6
row so that whoever takes it re-derives nothing: a generated name must come from
**module plus source-order index** and never from the type, because
`selfhost/emit/synth.hero`'s `content_key` renders the type and two unnamed
functions of one type would collide; and `ziglang/zig#1717` accepted that exact
proposal in 2020 and rejected it in 2023.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
