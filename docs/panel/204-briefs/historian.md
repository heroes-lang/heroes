# Panel 204, historian's brief

Read `00-shared.md` first. Precedent, sourced and dated: how languages that
bind C headers state and keep the order of their includes (cgo's preamble,
Zig's `@cImport`, Nim's `header` pragma, Rust's bindgen, Swift's module
maps, D's ImportC, Odin's `foreign import`), whether any reorders or sorts
them, and what happened when one did (a tool or a formatter that sorted
`#include` lines and broke a build: clang-format's `SortIncludes`, its
`IncludeBlocks`, and their documented failures); and whether a language that
claims declaration order never matters makes an exception for foreign
headers, and in what words.
