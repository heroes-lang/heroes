# Panel 209, the historian's brief

Read `00-shared.md` first, then this. You have no tree and no Bash: every
claim of precedent is verified by web search and cited, or written as
unverified in those words. Your report is
`docs/panel/209-reports/historian.md`, written as you go.

## The question

A language with value semantics and no assignment operator has two binding
forms: `x = 5` binds once, type inferred; a mutable cell is declared `v: i64
@ 0`, its type mandatory because the type is what tells the declaration from
the mutation `v @ v + 1`. The proposal gives the declaration its own symbol,
`v @= 0`, so the type becomes optional on both forms under one rule, and adds
*a cell nothing re-binds is a compile error*. The routes R0 to R7 are in the
shared brief.

## What to verify, each with a source and a date

1. **Languages that mark a mutable declaration by a symbol rather than by a
   keyword or a type**: Go's `:=` (declare) against `=` (assign), and the
   shadowing bug its pair produced inside nested scopes (the `err :=`
   complaint; find the issue or the proposal that discussed changing it, and
   what was decided); Pascal's and Oberon's `:=` as assignment only; F#'s `let
   mutable` with `<-`; OCaml's `ref` with `:=`; Nim, Zig, Kotlin, Swift and
   Rust, which all chose a keyword (`var`, `let mutable`, `let mut`): did any
   of them record why a keyword rather than a symbol? Did any ship a symbol
   and withdraw it?
2. **`@=` elsewhere**: Python's PEP 465 (2014) gave `@=` to in-place matrix
   multiplication; how widespread is it in corpora a model has seen, and did
   any language give `@` or `@=` a binding meaning? Ruby's `@v` and `@@v`
   (instance and class variables): the same glyphs, a different position.
3. **A mutable never mutated as an error**: Rust's `unused_mut` is a warning;
   Swift's *variable was never mutated; consider changing to let* is a
   warning; find a language where it is an error by default, and whether any
   project turned the warning into an error and reported what it caught.
4. **Mandatory type on a mutable, optional on an immutable**: has any shipped
   language had exactly this asymmetry? If none, say what was searched.
5. **Compound-assignment habits**: the `op=` family in C and its descendants
   means *update*; name a language that gave an `X=` token a meaning other
   than *apply X then assign*, and what readers did with it (R7's hazard, and
   R1's in the other direction).
6. **The reverse spelling `=@`** (R2): any language with a two-character
   token beginning `=` other than `==`, `=>`, `=~`? What became of `=~`'s
   readability complaints?

Verdict per route (advisory, no veto), a falsifiable prediction with the
instrument that scores it (the blind seat's sessions, the mutation operators,
a corpus count), and the condition. Dates and line counts only where a source
states them; an unsourced number is inadmissible.
