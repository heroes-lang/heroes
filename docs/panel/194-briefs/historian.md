# Panel 194, historian

Read `00-shared.md` first, then panel 178's historian report
(`docs/panel/178-reports/historian.md`) in your copy. Advisory, no veto.

**Verify every precedent by web search, with its source; an unsourced claim
is inadmissible.** 178's report is a starting point to re-check, never a
premise: say for each of its claims whether it still holds and where.

1. How languages build a C struct with long array fields without writing
   every element: C's designated initialisers and the rest zeroed (cite the
   C11 and C23 paragraphs, and what C23's `{}` says of padding), Rust's
   `..Default::default()` and `MaybeUninit`/`zeroed` for FFI structs, Zig's
   `std.mem.zeroes` and `= .{}` with defaults, Go's zero value and cgo,
   Swift's imported C structs' zero initialiser, Odin and Nim's `default`.
2. Zero as a valid value: where it is documented as valid and where not
   (POSIX `PTHREAD_MUTEX_INITIALIZER` against a zeroed mutex on macOS and
   glibc), and what each language promised about it.
3. A whole struct lent to `void *` with a separate count (defect 092): how
   Rust, Zig, Go (cgo) and Swift bound it, if they do.

Report: `<scratchpad>/p194/reports/historian.md`, written as you go, every
claim with its URL and the date you read it.
