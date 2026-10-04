- [x] **042 — spec § 13 cannot bind a struct that is both READ and POINTED AT, which is the shape C uses most** | the fielded record and the handle record would both need `tag addrinfo`, and two records may not name one tag, so `getaddrinfo` had no writable binding at all | **repaired 2026-09-15**, panel 153 R1, ratification pending | `selfhost/check/decls.hero`'s `one_tag_one_type` · `selfhost/emit/ffi_tag.hero`'s `record_by_tag` · `selfhost/handles.hero`

    **Origin:** found 2026-09-15 by **panel 152's llm-ergonomist**, reading only
    the specification, while answering a question about something else. It called
    it *"a language finding rather than an ergonomics one, and it is worth more
    than the qualifier question"*.

    **THE DEFECT WAS SMALLER THAN IT LOOKED, and three of its claims were
    falsified by measurement before anything was built.** Each was run:

    | the entry says | measured 2026-09-15 |
    |---|---|
    | *`struct stat` under `lstat` is the same shape* | **false**: `record FileStat tag stat partial` with `@buf` builds, runs, prints a file's size. The caller owns that struct |
    | *passing the fielded record where C wants `const struct addrinfo *` needs an address-of the language does not have* | **false**: `@hints: Hints` against `const struct addrinfo *` builds and runs. `@` is copy-in/copy-out and the emitter passes the address |
    | *the three ways out are all closed* | **one was open**: a fielded record naming the struct through a TYPEDEF sits beside a handle legally today, because the rule only sees records that carry a `tag` |

    What was left after those is one rule and one missing operation.

    **THE REPAIR: one tag carries one of each KIND.** `one_tag_one_type` admits
    at most one handle and at most one record with fields per tag, keyed on the C
    type each spells rather than on the tag alone. Its premise was measured in C
    alone, under the compiler's own flags:

    | two Heroes types over one C struct | the swap in C | clang |
    |---|---|---|
    | two HANDLES, `struct s *` twice | a type where its own type belongs | **silent** — the premise holds, the pair stays refused |
    | two RECORDS WITH FIELDS, `struct s` twice | the same | **silent** — the premise holds, the pair stays refused |
    | a HANDLE and a RECORD, `struct s *` and `struct s` | either direction | **two errors** — the premise fails, and this is the pair `getaddrinfo` needs |

    Heroes answers `type_mismatch` on that swap first, so both instruments catch
    it and neither is blind. **The rule keeps everything it was written for.**

    **And the diagnostic path it rested on is repaired in the same change, which
    panel 153's completeness critic caught before it landed.**
    `selfhost/handles.hero` had given the lookup as a REASON for the old rule —
    *"`record_by_tag` answers with the FIRST record carrying the tag, so a
    message about the second would carry the first's caret"* — and the first
    prototype removed the rule and left the lookup. The critic built that shape
    and got exactly the predicted wrong caret, on a path no suite watches because
    both arms exit 1. The lookup is now asked which KIND a message is about:
    every clang message this file reads names `struct <tag>`, which is the record
    with fields. Witness:
    `tests/golden/fixedbugs/fixedbugs-a-tag-with-two-kinds-names-the-right-record.hero`,
    whose caret lands on the offending record's own tag.

    **Measured after.** `getaddrinfo` binds whole against `netdb.h` and nothing
    else — the handle for `struct addrinfo **res`, the record with fields for
    `const struct addrinfo *hints`, both marks read — and builds, runs and exits
    0, plain and under `--sanitize`. Under the shipped compiler the same file is
    `error[duplicate_tag]`, exit 1. The case is
    `tests/golden/run/fixedbugs-a-tag-names-a-handle-and-a-record.hero`; the two
    pairs that stay refused are in
    `tests/golden/check/ffi-handle-refusals.hero`, one of each kind.

    **WHAT THIS DOES NOT CLOSE, said plainly rather than folded in.** A program
    still cannot turn a handle into the struct behind it with anything the
    LANGUAGE offers. What panel 153's completeness critic measured is that it
    does not need to: three lines of a header of the author's own — a typedef and
    a `static inline` returning the struct by value — and the shipped compiler
    reads `ai_family` and walks `ai_next` to the end, output byte-identical to
    the equivalent C program, every field still verified by clang against the
    header. Twenty headers of the project's own already ship under test. **The
    one cost of that route is filed as defect 045**: the null read segfaults at
    exit 139, which §1.12 forbids by name. **Whether the language should do it
    instead of the header is queued as `panel 153` in `docs/work/DECIDE.md`**,
    with that route on the ballot, because the sitting's own option set was short
    by one and a resolution chosen from an incomplete set cannot be the most
    complete one.
