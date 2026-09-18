# Panel 164 — historian

Read `00-shared.md` first. Advisory, no veto. **Unsourced precedent is
inadmissible**: every claim carries a URL fetched this session. Today is
2026-09-18.

## The question

A language with value semantics binds C. A struct field is `char name[16]`. C
lets it decay to `char *` at every call, which is both the idiom every POSIX
program uses and the origin of a well-documented class of defects when the field
holds no terminator. **How do languages that bind C let a fixed byte field reach
a C pointer parameter, and what did each choice cost?**

## What to verify, in this order

1. **Rust.** A `[c_char; 16]` field in a `libc` struct does not coerce to
   `*const c_char`. Find what a program writes to pass it — `.as_ptr()`, and
   what `CStr::from_bytes_until_nul` (stable 1.69) was added FOR. Find the
   discussion that motivated it: the claim that a fixed array is not a C string
   until a terminator is found is exactly this sitting's route 3.

2. **Zig.** A `[16]u8` field and a `[*:0]const u8` parameter: Zig's type system
   distinguishes a sentinel-terminated pointer from a plain one. Find the
   documentation, and whether `std.mem.sliceTo` or a cast is what programs
   write. Zig encoded the terminator in the TYPE; find what that cost and what
   it bought.

3. **Go.** `unix.ByteSliceToString` and `&uts.Sysname[0]`: how does cgo let a
   `[65]byte` field reach a `*C.char`? Find whether Go requires an explicit
   `unsafe.Pointer` and why.

4. **The decay itself.** C's array-to-pointer decay is the rule every route here
   either adopts or refuses. Find one authoritative account of what it has cost
   — CERT's `ARR` rules or MISRA's — so the sitting weighs route 1 against a
   sourced bill and not a feeling.

## Deliver

Verdict (advisory) · every claim with its URL · what each precedent cost · a
falsifiable prediction · anything in `00-shared.md` you found misstated.
