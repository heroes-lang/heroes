# Panel 154 — brief for the historian

Read `00-shared.md` first. You have no file write; your report is your final
message and the coordinator files it. Advisory, no veto. **Every claim of date,
version or design is verified by web search and carries its source URL**;
unsourced precedent is inadmissible.

**The question, in the lineage's terms.** A language with no standard library
binds C headers. A pointer the program holds may be null, and the C function it
is handed to may or may not accept null — a property of the function, not of the
type, that no header states. Find what other systems do:

1. **Nim's `importc`**, Zig's `@cImport` and `?*T`, Go's cgo, Rust's bindgen and
   `Option<NonNull<T>>`, D's `extern(C)`, Ada's `not null access`. For each: is a
   null check inserted at the boundary, is nullability part of the type, or is it
   the programmer's business? Cite the manual.
2. **Ada 2005's `not null`** in particular, and Ada's `Access_Check`: a language
   that made null-ness a declared property of a parameter. What did it cost and
   what did it buy? Did anyone measure it?
3. **`_Nonnull` / `_Nullable` in clang**, and Objective-C's audited regions
   (`NS_ASSUME_NONNULL_BEGIN`). Apple annotated whole SDKs this way. How much of
   this Mac's own SDK is annotated, and did the annotations come from the vendor
   or from a tool?
4. **Did any language ever ship a blanket null check at the C boundary and then
   remove it, or ship without one and add it?** That is route A's precedent
   either way.
5. **The default direction**: of the systems you find, how many are safe by
   default (check unless told not to) versus unsafe by default?

One prediction the panel can score, with the instrument.
