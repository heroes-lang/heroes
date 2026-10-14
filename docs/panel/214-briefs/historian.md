# Panel 214, the historian's brief

Read `00-shared.md` first. Your question is precedent, every claim with a URL you read this session,
marked `verified` or `unverified`.

How do other toolchains that compile a program's own C headers beside code they generate decide
whose mistake a C compiler's error is, and how do they keep their generated names out of the
header's way? Look at Go's cgo (its `_cgo_` names and how a preamble error is reported), Zig's
`@cImport` / translate-c, Swift's ClangImporter, Rust's bindgen, Nim's `importc` and its generated C,
Cython, and any C++ or C toolchain that reserves a prefix. For each: the rule, the year, and what it
cost them (a bug report, a migration). Say which routes of `00-shared.md` each precedent supports
or refuses. Write your report as you go.
