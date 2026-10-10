# Panel 202, historian's brief

Read `00-shared.md` first. Precedent, sourced and dated: C's one-definition
rule and what compilers and linkers say of two `static inline` definitions
across translation units; Rust's `clashing_extern_declarations` lint; Go's
cgo with two packages binding one C name; Zig's `@cImport` in two files; Nim.
Which refuse at compile time, which at link, which never, and why.
