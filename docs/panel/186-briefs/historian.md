# Panel 186, the historian's brief

Read `00-shared.md` in this directory first: the defects, the four questions
and the routes. You need no build; your evidence is the web, and every
claim of precedent carries its source (URL, and the version or date it
describes). Unsourced precedent is inadmissible.

## Your task

How did the tools that bind C from another language treat **a struct holding
an anonymous union**, and **a bit-field member**, and what did their users
then do? At least, each with its source:

- **rust-bindgen**: the `__bindgen_anon_1` field of a generated union type,
  and its bit-field accessors (`_bitfield_1`, the generated getters and
  setters); and Rust's own unnamed fields (RFC 2102), its status today;
- **cgo**: how Go's cgo represents a C union and an anonymous union inside a
  struct (a byte array?), and its documented treatment of bit-fields;
- **Zig's `translate-c`** and `@cImport`: anonymous unions and bit-fields;
- **Swift's ClangImporter**: anonymous union members imported as properties
  of the struct, and bit-fields as computed properties;
- **D** (`extern (C)`, anonymous unions native to the language), **Nim**
  (`importc` objects, `{.union.}`), **Odin** (`#raw_union`), **Python's
  ctypes** (`_anonymous_`), **LuaJIT's FFI**;
- any of them that refuses such a struct, and whether its users routed
  around the refusal (panel 073's historian's Go precedent: users reached a
  union through a hand-written C accessor or an opaque pointer).

Then: which of the sitting's routes has a precedent that ran for years, and
which has none; and for panel 073's filed *construction-arity form* (declare
every member, construct naming exactly one: *Rust, D, Zig and Swift all
converged on it*, `docs/panel/073-the-fields-that-share-one-address.md:119-123`),
verify that claim against the sources, since it is a sitting's record and not
yet a measurement.

Write your report into `docs/panel/186-reports/historian.md` in the trunk as
you go: a verdict per route (approve, object; you hold no veto), the
precedent with its sources, a falsifiable prediction and the condition that
would change your verdict. English, no em dashes. No paid run.
