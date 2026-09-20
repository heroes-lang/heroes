# Panel 171 — brief for the historian

Read `docs/panel/171-briefs/00-shared.md` first. Advisory, no veto. **Verify
every date, count and claim by web search**; where a search returns nothing, say
so and name the vocabulary.

## What this sitting owes you

Your panel 170 finding — *the lesson is the default, not the mark* — is the one
the author acted on. Swift's SE-0103 made the foreign default pessimistic in
2016 and never withdrew it. **This sitting follows that precedent, and your job
now is the part Swift's proposal does not tell us: what did it cost the people
who wrote the bindings, and what did they call the mark.**

## The questions

1. **The WORD, in every precedent.** Swift: `noescape` in the C header,
   imported as the absence of `@escaping`. Clang: `__attribute__((noescape))`.
   C#: `scoped`. Hylo: `let`. Rust: a lifetime on the reference, no word.
   Objective-C ARC: `__attribute__((ns_consumed))`'s opposite is the default.
   **Which of these is on the CALLEE's declaration, written by the binding
   author rather than the library author, and read at the caller?** That is
   Heroes' shape: the word goes in the `.hero` file, not the C header. Find the
   closest precedent for a word the BINDING author writes about a library they
   did not write, and say what it was called and whether it was ever renamed.
2. **What the pessimistic default cost Swift's binding authors.** After SE-0103,
   every imported C function-pointer parameter was `@escaping` unless the header
   said `noescape`. How many Apple SDK headers gained `noescape` annotations, and
   over what period? Was there a migration guide, a fixit, a complaint thread?
   **This is the cost the author has accepted and it should be on the record with
   a number if one exists.**
3. **The `getenv` question**, which the ffi seat is also asked: `getenv` returns
   a pointer into the process environment. Does the C standard say anything
   about whether it KEEPS the pointer it is handed? (It should not; the argument
   is a lookup key.) A precedent for a standard-library function whose retention
   is documented in the standard would anchor the word's meaning.
4. **The negative.** Has any language shipped a *does-not-keep* mark on the
   binding side and then removed it, or flipped the default back? Clang's
   `noescape` semantics were revised in 2025 (the LLVM RFC you found at panel
   170): what changed and why?

Report to `docs/panel/171-reports/historian.md` if you can write; otherwise
return the full text and the coordinator writes it out.
