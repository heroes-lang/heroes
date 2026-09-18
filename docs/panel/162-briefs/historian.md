# Panel 162 — historian

Read `00-shared.md` first. This file is your input only. You have **no veto**;
your seat is advisory, and **unsourced precedent is inadmissible**. Every date,
name, version and claim must carry a URL you fetched in this session. This role
is the most hallucination-prone in the panel and the rule exists because of that.

Today is **2026-09-18**. Your knowledge may be stale; check what changed in the
last two years.

## The question

A language with no standard library binds C directly. A C struct field is
`char sysname[256]` — a fixed run of bytes that usually but not always holds
NUL-terminated text. **How does a language that owns its own string type let a
program read that field as a string, and what did each choice cost?**

The second half, and it is equally real: **how does such a language let a program
BUILD a struct with a 256-element array field**, when its own rule is that every
binding is initialised and an array literal states every element?

## What to verify, each with a source

1. **Rust.** `std::ffi::CStr::from_bytes_until_nul`, `CStr::from_ptr`, and
   `str::from_utf8` — find when each stabilised and what it returns on invalid
   input. More importantly: find the **discussion** about reading a fixed
   `[c_char; N]` out of a `libc` struct, which is a known ergonomic sore point.
   `libc::utsname` is the canonical example; find what real code does and whether
   the standard library ever offered a shortcut. Also `Default` for large arrays:
   Rust had a hard limit at 32 elements for years and const generics changed it —
   find when, and what the workaround was before.

2. **Zig.** `std.mem.sliceTo`, `std.mem.span`, and how a `[256]u8` field is read
   as a string. Zig has no separate string type, which makes it the interesting
   negative case: find what it does INSTEAD, and whether that is available to a
   language that does have one. Also `std.mem.zeroes` and `= undefined` for
   building a struct to pass to an out-parameter, which is exactly wall two.

3. **Go.** `unix.Utsname` is the same struct this sitting is about. Find what
   type its fields have today and what a program writes to get a string out of
   one. Go changed those fields from `[65]int8` to `[65]byte` in 2017 (panel
   161 sourced that); find whether the conversion to `string` is a one-liner now
   and what it looks like.

4. **Ada, `Interfaces.C`.** `To_Ada` and `To_C` over `char_array`, standardised
   1995. Find the signatures: what do they do about a run that is not
   terminated, and do they take a length? Ada answered both of this sitting's
   walls thirty years ago and its answer is admissible precedent.

5. **The refusal case.** Find a language that binds C and **refuses** to convert
   a byte array to its string type, leaving the program to read elements. If you
   find one, what happened to it. If you cannot, say so in those words and name
   what you searched for.

6. **The validity question.** Every language above decides what happens when the
   bytes are not valid text: a fallible result, a lossy conversion, an
   unchecked one, or a panic. **Tabulate who chose what**, because this sitting
   has to choose too and `spec § 3` says Heroes' `str` is immutable UTF-8.

## What is NOT wanted

A history of character encodings. What is wanted is what the languages that bind
C did at this exact boundary, and what each paid.

## Deliver

Verdict (advisory) · every claim with the URL you fetched · what each precedent
COST the language that took it · a falsifiable prediction with its milestone ·
and, explicitly, anything in `00-shared.md` you found to be misstated.
