# Panel 215, the historian's brief

Read `00-shared.md` first. Every precedent with the URL you read this session, `verified` or
`unverified`.

How do languages that bind C structs treat (1) padding when a struct crosses into C by value (Rust's
`#[repr(C)]` and `MaybeUninit`, Zig's extern structs, Swift's imported C structs, Go's cgo and the
`unsafe` rules, CERT DCL39-C itself), and (2) C types that must not be copied (Rust's `!Copy`, and how
`std::sync::Mutex` wraps `pthread_mutex_t` in a box; Swift's non-copyable types, SE-0390; Zig; C++'s
deleted copy constructors)? For each: the rule, the year, and what it cost them. Say which routes of
`00-shared.md` each precedent supports or refuses. Write your report as you go.
