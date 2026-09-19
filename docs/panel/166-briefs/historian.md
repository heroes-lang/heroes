# Panel 166 — brief for the historian

**Read `docs/panel/166-briefs/00-shared.md` first.** You are advisory and hold no
veto. **Every claim must be verified by web search and carry its source URL.**
Where you cannot verify something, say *unverified* in those words — you did that
at panel 165 about a macro and the coordinator settled it with one command, which
is exactly how this seat should work.

## Your question

A binding language lends the address of a fixed-size struct field to a C function
that takes a pointer and a separate length. Two things can go wrong and both do,
in this language, today:

- the caller states a length larger than the field, and C reads or writes past it;
- the value lent belongs to an **immutable** binding, and C writes into it.

**Who has solved either, and how?**

## Thread A — the counted pointer at an FFI boundary

For each of Rust, Zig, Go, D, Odin, Swift, Ada, Nim, and C# / P/Invoke: when a
binding hands C a pointer plus a length, **is there a way to declare that the
length belongs to that pointer, and does anything check it?**

Specifically worth chasing, and say if a thread leads nowhere:

- **Rust**: `&mut [u8]` to `*mut u8` + `len()` is the idiom — is the length ever
  *declared* in the binding, or always passed by the caller? What does `bindgen`
  do with `__counted_by`?
- **Swift**: panel 165 established that `SafeInteropWrappers` reads
  `__counted_by` and produces `UnsafeBufferPointer` / `Span<T>`, behind an
  experimental flag. **Is it still experimental?** And its own page says *"Bounds
  annotations on global variables or struct fields are ignored: only parameters
  and return values are considered"* — find out **why**, because Heroes' case is
  a field passed to an annotated parameter.
- **Ada**: `Interfaces.C` passes an array as `t*`; is there a convention for the
  length, and does `Constraint_Error` reach it?
- **C# P/Invoke**: `[MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)]` is a
  declared relationship between a pointer parameter and a length parameter. **Is
  it checked, and when — at marshal time, or never?** This is the closest thing
  to route C the panel knows of and nobody has read its documentation.

## Thread B — writing through an immutable value

Every language in this survey has some notion of immutability and all of them
bind C. **How does each stop, or fail to stop, C writing into a value the
language calls immutable?**

- Rust's `&T` to `*const T` to a C function that casts away `const` — what does
  the community consider this, and is there a documented soundness rule?
- Zig's `const` pointers across `extern fn`.
- Ada's `in` parameters with `Convention => C`.
- Nim's `let` and `importc`.

The interesting answer is not *"they refuse it"* — C can always cast. It is
**where each language draws the line between what it promises and what it
disclaims**, and whether any of them says so in the language reference rather
than in folklore.

## What your report must carry

A table per thread, one row per language, each cell with a source URL and a
verified / partially-verified / unverified marker. Then an advisory verdict on
the seven routes in the shared brief, and **one falsifiable prediction**.

You have no file-write tool; write your report as a finished document in your
final message and the coordinator will write it out verbatim to
`docs/panel/166-reports/historian.md`.
