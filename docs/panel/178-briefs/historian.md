# Panel 178 — historian

Read `00-shared.md` first. You judge precedent, advisory, no veto. **Every
date, version, line count and quotation is verified by web search and cited
with its URL**; an unsourced precedent is inadmissible, and *I did not find it*
is a finding. You have no write tool: return your report as your final message,
and the coordinator writes it to `docs/panel/178-reports/historian.md`
unchanged.

## What to find

1. **How languages that bind C build a C struct without naming every field**,
   and whether the zero is written at the site or implied:
   - Swift: the initialiser the Clang importer gives an imported C struct, and
     what it sets each field to;
   - Zig: `std.mem.zeroes`, and whether it refuses any type;
   - Rust: `..Default::default()` struct update syntax, `std::mem::zeroed`
     (its safety section), and the `bytemuck::Zeroable` trait (what it
     promises and who asserts it);
   - Go: zero values, and what `cgo` gives a C struct declared with `var`;
   - C23: `= {}`, and what it says of padding;
   - Odin, D, Nim or Ada, where one of them has a distinct answer.
2. **Padding**: what C11 says of padding bytes after an initialiser that names
   some members (§6.7.9) and after a store to a member (§6.2.6.1): quote the
   paragraphs. The coordinator's recollection of §6.2.6.1p6 is unverified.
3. **Who declares that all-zero is a valid value**: a marker the author asserts
   once on a type (Rust's `Zeroable`, anything else you find), versus assumed
   everywhere (Go), versus refused. And whether any system states the claim per
   platform, since measurement 7 found the pthread mutex valid at zero on Linux
   and not on Darwin.
4. **A string into a fixed byte array**: what `strlcpy`, Rust's
   `CString`/`copy_from_slice`, Zig or Go offer to put a path into
   `sockaddr_un.sun_path`, and what each does when it does not fit.

Give each finding with its source, and say which route of the shared brief's
table it bears on.
