# Panel 153 — brief for the historian

Read `docs/panel/153-briefs/00-shared.md` first. You have no file write; your
report is your final message, verbatim, and the coordinator writes it to
`docs/panel/153-reports/historian.md`.

## What you judge

Precedent, advisory, no veto. **Every claim of date, version, syntax or design
is verified by web search and carries its source**; unsourced precedent is
inadmissible, and this seat has been the most hallucination-prone in the panel.

## The question, in the language's own terms

A small language with no standard library binds C headers. A struct C hands
back **by pointer** — `struct addrinfo **` from `getaddrinfo`, `struct stat *`
filled by `lstat`, every linked list — must have its fields READ and its list
WALKED, and a struct value must sometimes be handed to C **by pointer**
(`const struct addrinfo *hints`). The language has value semantics, no
dereference, no address-of; a HANDLE is an opaque pointer type; `@` on a
parameter is copy-in/copy-out. Two routes are on the table:

- **A**: the handle type may declare fields, read through the pointer (Oberon's
  `p.f` for `p^.f`?), with a word marking it;
- **B**: two types over one C struct — the pointer and the value — and a
  built-in that copies the struct out of the pointer.

## What to find, with sources

1. **Reading fields through a pointer without an explicit dereference**: Oberon
   and Oberon-07 (`POINTER TO RECORD`, implicit dereference in field selection —
   the Report's exact wording and section), Modula-2, Nim (`ptr T` and `.`
   auto-dereference; `p[]`), Zig (`*T` and field access; `?*T` for nullable),
   Go's cgo (`*C.struct_addrinfo`, `.ai_family` through the pointer). For each:
   is the dereference implicit for FIELDS, and what happens on nil/NULL (a trap,
   a panic, undefined behaviour)?
2. **Two types over one C struct**: how Nim, Zig, cgo and Rust's bindgen spell
   the value and the pointer of one C struct, and whether any of them forbid
   having both.
3. **Handing a value to C by pointer without address-of**: languages with
   `var`/`inout` parameters used against `const T *` (Pascal, Ada `in out` with
   `by-reference` types, Nim `var`). Did any of them ship copy-in/copy-out and
   later change it? Sources.
4. **The one worked example in a reference**: does any language Report
   (Algol 60, Pascal, Oberon, Go spec, Zig's language reference) keep a worked
   example that is INCOMPLETE on its own section's rule, or did one ever have to
   be corrected because readers copied it? Q2 rests on this.
5. **NULL reads**: how Oberon's trap, Go's nil-pointer panic and Zig's optional
   pointers each price the check, if any source states it.

## What your report must carry

Each finding with its source URL and the date or version it is true of; a plain
*not found* where the search came up empty, naming what you searched for; your
verdict on A versus B by precedent alone; and **one prediction the panel can
score** — for example, a count of how many of the systems above dereference
implicitly for field access — with the instrument that scores it (a URL to read,
a command to run).
