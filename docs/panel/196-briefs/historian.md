# Panel 196, historian

Read `00-shared.md` first.

Your seat is advisory (no veto) and judges precedent; it must verify by web
search, and an unsourced precedent is inadmissible.

1. **How other foreign-function interfaces say that C writes N elements
   through a pointer with no count parameter**: Rust's `bindgen` and
   `std::ffi` (`&mut [u8; 32]` passed as `*mut u8`), Zig's `[*]u8` against
   `*[32]u8`, Swift's `UnsafeMutablePointer` and `withUnsafeMutableBytes`,
   Nim's `importc` with `array[32, uint8]`, Go's cgo, Ada's `Interfaces.C`,
   OCaml's ctypes. For each, what a binding writes for `SHA256_Final`, and
   whether a one-element binding of it is refused, accepted, or caught at run
   time.
2. **What C itself and its tools say**: C99's `[static N]` array parameters and
   what compilers do with them; clang's `counted_by`, `sized_by` and
   `__counted_by` for parameters (the `-fbounds-safety` work), GCC's `access`
   attribute (`access (write_only, 1, 2)`), Microsoft's SAL
   (`_Out_writes_(32)`, `_Out_writes_bytes_`). Which of these real headers
   carry: Windows SDK's, glibc's, OpenSSL's.
3. **Faults of this shape in the record**: a CVE or a published bug where a
   binding handed a library a buffer shorter than the library writes, in any
   language's FFI; cite it.

Write each claim with its source (URL and the date you read it).
Report: `<scratchpad>/p196/reports/historian.md`, written as you go.
