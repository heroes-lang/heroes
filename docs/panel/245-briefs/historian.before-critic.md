# Panel 245, historian

Read `00-shared.md` first. Verify every precedent with the page read today (URL and day), saying only
what the page states; a page that cannot be read is not cited. Q1: how compilers and FFIs that wrap C
treat a deprecation or `warning` attribute inside generated or glue code (Rust's `bindgen` and
`-sys` crates, Zig's `@cImport`, Swift's ClangImporter, Go's cgo preamble, Nim's `{.emit.}`), and
whether they quiet it, pass it through, or refuse. Q2: which systems let a program call a C format
function with a run-time format, and how they guard it (Go's cgo, Swift's `withVaList`, Rust's
`libc::printf` behind `unsafe`, D's `pragma(printf)`, GCC's and clang's `format` attribute and
`-Wformat-security`, CERT C FIO30-C, CWE-134). Q3: how build tools pair a header with its library when
the header comes from an environment path (`CPATH`, `CFLAGS -I`) rather than a package query
(pkg-config's design, Homebrew's superenv and keg-only caveats, CMake's `find_package` and imported
targets, Meson's dependency objects), and whether any checks the pairing. Your report's form is the
seat's; end with candidate prior-art lines per route, in the two forms `.claude/rules/records.md`
allows.
