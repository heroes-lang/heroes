# Panel 153 — brief for the llm-ergonomist

You judge the objective: whether a model reading ONLY the specification writes
correct programs. **You receive the specification's § 13 and nothing else** —
no design document, no repository, no other sitting — and your verdict is an
experiment, not an opinion. Your report is your final message, verbatim; the
coordinator writes it to `docs/panel/153-reports/llm-ergonomist.md`. You have a
veto on a non-local construct.

## Three tasks, each performed and each written down

**Task 1 — the omission (blind A/B).** Below are two versions of the section's
worked example, labelled only X and Y. For each, read § 13 with THAT example in
place of the fence it carries, then write from that reading the binding a
program needs to open a SQLite database at `":memory:"` and close it, and the
`main` that does so. Do it as ten independent attempts per version, each from
the section alone. **Count, per version, how many of the ten attempts omit a
mark the section's own rules require** (`acquires` on the out-parameter that
begins a handle's life, `consumes` on the parameter that ends it). Report both
counts as `N of 10`, and say which version is the section's current fence if you
can tell, and how.

```
X:
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    record Db tag sqlite3
    function sqlite3_open(path: cstr, @out: Db acquires sqlite3_close) -> i64
    function sqlite3_close(db: Db consumes) -> i64

Y:
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    record Db tag sqlite3
    function sqlite3_open(path: cstr, @out: Db) -> i64
    function sqlite3_close(db: Db) -> i64
```

**Task 2 — reading through a pointer (blind A/B).** C's `getaddrinfo` hands
back a pointer to a `struct addrinfo` whose fields are `ai_family: int`,
`ai_next: struct addrinfo *`, and the list is walked through `ai_next` and freed
with `freeaddrinfo`. Two candidate additions to § 13 follow, labelled P and Q.
Under EACH, write the `extern` group, and a `main` that resolves `"127.0.0.1"`,
prints every `ai_family` in the list, and frees it. Then say, for each: how many
new words a reader has to hold; where a reader would go wrong (name the line);
whether the construct is LOCAL — decidable from the line it is on and the
declarations it names, with no other file open — and which of the two you would
adopt, and why, in the section's own terms.

```
P (fields on a handle):
    A handle may list fields, read through the pointer: `record AI tag addrinfo`
    followed by `ai_family: i32` and `ai_next: AI` makes `ai.ai_family` C's
    `ai->ai_family`, a field of the handle's own type is how a list is walked,
    the fields are read and never written, and a read through `nullptr` aborts.
    [a marker word after the tag distinguishes it from a struct by value —
    propose one, or say none is needed and why]

Q (two records and a read):
    One tag may name one handle and one record with fields: `record AI tag
    addrinfo` is the pointer and `record AddrInfo tag addrinfo` with `ai_family:
    i32` and `ai_next: AI` is what it points at, and `ai.read()` copies that out
    as an `AddrInfo`; reading through `nullptr` aborts.
```

**Task 3 — `lstat`.** `int lstat(const char *path, struct stat *buf)` fills a
struct the caller provides, and `st_size` is read afterwards. Using § 13 as it
stands, write the binding and a `main` that prints a file's size. Say whether
§ 13 as written lets you, and if it does, whether P or Q adds anything for this
shape.

## What your report must carry

The programs you wrote, verbatim; the two counts of Task 1 as `N of 10` each;
your P-versus-Q verdict with the failure you predict for each in the form *"K
in 10 models will …"*; your veto, if any, on locality; and **one prediction the
panel can score**, with the instrument.

## The section, verbatim from `spec/heroes-spec.md` as of 2026-09-15

## 13. FFI
Anything beyond this document — sockets, maths, JSON, databases — comes from C
libraries. A group names its header, and `link` a library when the symbols need one. clang
checks every result type, constant and record field against that header, and a result may be
wider than C's. A **parameter** and a **field** are declared at the header's own
width and sign — `i32` where C says int, `u64` where it says `size_t` — and one that
disagrees is refused, except a parameter C converts exactly (`i16` against int)
and what a `ptr` points at. A C out-parameter is an `@` parameter, and what it
points at is held to the same width and sign — `@n: u64` where it says
`size_t *`:
```
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    record Db tag sqlite3
    function sqlite3_open(path: cstr, @out: Db) -> i64
    function sqlite3_close(db: Db) -> i64
```
A callback is a **parameter**, never a result; its parameters follow the same rule
and `()` is `void`: `atexit(f: (function() -> ()))`.

A group's `record` is the header's struct: all its fields, and the same name
unless the header writes it after the word struct, which `tag` gives:
`record FileStat tag stat partial`. A
field is a number, `bool`, `ptr`, `cstr`, another record of the group, or a fixed
array of one: `i32[4]`, never a `[T]`; build one with `[a, b, c, d]`.
`record Font partial` names only some, and then comparing it and using it as a
map key are compile errors — for it and for any value holding it. Its size stays
C's, not the field list's. One with a `tag` and no fields is a **handle**, C's
pointer to that type: `record Db tag sqlite3` is `sqlite3 *`.
`nullptr` is its null and `==` compares the address; a map key is an error. A
parameter declared with it takes no other handle, and two records may not name
one tag.
A group's `constant` has no body: the header holds the value.

`s.cstr()` lends a `str` to C; outside a group nothing answers `cstr` and no
record holds one. `c.validated()` copies one back as a `str?`, and a null one
fails `null_cstr`.
`x: cstr @ s.lease()` is a COPY of the bytes that C may read for as long as the
program says, and `end_lease(@x)` frees it and empties the cell. A lend and a
lease name stand only as an argument of a call, nothing else writes a lease's
cell, and a lease nobody ends, like a handle nobody consumes, aborts when
`main` returns, saying how many.
`owned sqlite3_free` after a `cstr` result or a `char **` out-parameter: the
compiler frees that string with that function, hands it over as a `str?` (the
`@` cell is only written), and refuses your own call of it. Unmarked pointers
are never freed. `consumes` after a parameter says the call ends that value's
life, so passing one the function borrowed is an error: mark the parameter `@`
and the value does not survive the call. `acquires sqlite3_finalize` after a result
or `@` out-parameter reaching a handle says the call begins that handle's life and names
the one that ends it, which the program owes it. The owing is counted, so a
handle consumed twice hides one never consumed. `borrows` says the call hands
back one it keeps, and where any `extern` consumes a handle type every call handing
one back says which it is.
A group may name a **package** instead of a library: `extern "raylib.h" package "raylib"`
asks the system where its headers and libraries are and what else it needs. A
package answering with anything this compiler does not pass on is refused,
naming what it said.

    Extern = "extern" string [ ( "link" | "package" ) string ] NEWLINE
             INDENT { Member } DEDENT .
    Member = "function" ident "(" [ CParam { "," CParam } ] ")"
               [ "->" Type [ "owned" ident ] [ "acquires" ident | "borrows" ] ] NEWLINE
           | "constant" ident ":" Type NEWLINE
           | "record" ident [ "tag" ident ] [ "partial" ] ( Fields | NEWLINE ) .
    CParam = [ "@" ] ident ":" Type [ "owned" ident ]
             [ "consumes" | "acquires" ident | "borrows" ] .
