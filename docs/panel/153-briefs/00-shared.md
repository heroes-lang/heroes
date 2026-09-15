# Panel 153 — shared brief

**Convened** 2026-09-15 on `docs/work/DEFECTS.md` 042 and 043, without asking,
under CLAUDE.md § 4's *ask once per milestone then convene again*: no milestone
is open, and panels 150, 151 and 152 convened under the same clause on the same
day. **Full panel**, five seats plus the completeness critic. Both defects were
filed by panel 152's llm-ergonomist, which read only the specification, and
neither was ruled on: that sitting was convened on defect 037, which closed
today.

## The two questions

**Q1, defect 042.** § 13 cannot bind a struct that is both READ and POINTED AT.
`getaddrinfo` hands back `struct addrinfo **`, which needs a HANDLE (`record AI
tag addrinfo`, no fields); reading `ai_family` off what comes back needs a
record WITH fields; both would carry `tag addrinfo`, and § 13 says *two records
may not name one tag*. And even with both, nothing turns the pointer into the
struct: the language has no dereference and no address-of. **How does a program
read the fields of a struct C hands back by pointer, walk a list of them, and
hand one to C by pointer?** `struct stat` under `lstat`, and every linked list
in every C header, are the same shape.

**Q2, defect 043.** The specification's only FFI example is an acquire-and-release
pair — `sqlite3_open` and `sqlite3_close` — and carries neither `acquires` nor
`consumes`. The seat that filed it predicts **at least 8 in 10** models omit both
on this shape, following the example, and the omission compiles, runs, leaks,
and produces no diagnostic and no abort, because the completeness rule arms only
where some `extern` declares `consumes`. **The example gains
`@out: Db acquires sqlite3_close` and `db: Db consumes`: what pays for it, and
may a section's one worked example be incomplete on the section's own rule?**

## What is true at HEAD, measured 2026-09-15 before this brief was written

- **A handle over `struct addrinfo` compiles and runs**, on Darwin and on Linux
  under `--sanitize`: `tests/golden/run/fixedbugs-getaddrinfo-is-bindable.hero`
  prints `0`. Defect 037 closed today: the compiler supplies `struct` from
  clang's own refusal (*must use 'struct' tag*), never from a probe. So Q1 is no
  longer blocked on the spelling; it is blocked on the FIELDS.
- **A handle has no fields, by definition**: `selfhost/handles.hero`'s
  `is_handle` is *group record, `tag`, no fields, not `partial`*. A group record
  WITH fields and a tag is C's struct **by value** — `examples/ctime/main.hero`'s
  `record Broken tag tm partial`. So `tag` + fields already means one thing.
- **`one_tag_one_type`** (`selfhost/check/decls.hero:285`, 17 lines) refuses two
  group records on one tag, `error[duplicate_tag]`. It was narrowed at panel 145
  for TWO HANDLES over one C type: the compiler refuses a swap between them and
  clang accepts it, so the mutant survives every instrument. **For a handle
  (`T *`) beside a fielded record (`T`) clang does NOT accept the swap** — that
  pair is not the case the rule was written for, and nobody has ruled on it.
- **Nothing dereferences and nothing takes an address.** No operation turns a
  handle or a `ptr` into the struct it points at; `@` is copy-in/copy-out
  (design.md §4.8) and is the only way an address reaches C. `@x: T` against a
  `T *` out-parameter is the shipped shape; `@hints: T` against a
  `const T *` IN-parameter has not been ruled on.
- **`struct addrinfo` on this Mac** (`netdb.h`): `ai_flags` `ai_family`
  `ai_socktype` `ai_protocol` are `int`; `ai_addrlen` is `socklen_t`;
  `ai_canonname` is `char *`; `ai_addr` is `struct sockaddr *`; `ai_next` is
  `struct addrinfo *`. `netdb.h` declares **18** functions returning a struct
  pointer. `getaddrinfo(const char *, const char *, const struct addrinfo
  *hints, struct addrinfo **res) -> int`; `freeaddrinfo(struct addrinfo *)`.
  `struct stat` has 28 lines of fields here.
- **Q2's fence was compiled and run** against real SQLite with both marks:
  opened `:memory:`, closed it, exit 0 (`docs/work/DEFECTS.md` 043).
  `tests/harness/suite_special.hero:330` runs the spec's fence under a `main`
  that opens `:memory:`, prints the code and closes — legal under both marks.
- **The spec today**: **7974** real on `claude-opus-5`, **5989** vendored,
  digest `2e77c4e72ce69512`, ceiling 10240, **2266 free**. Q2 was priced at
  **+7 vendored** on the 5988 base. **The −3 removal panel 152 named — *what
  the header leaves opaque* — is SPENT**: it landed today in defect 037's
  ledger row. Q2 needs its own payment: a named removal or a registered
  prediction naming an instrument that exists.

## Two routes for Q1, listed so a seat can refuse both and name a third

**Route A — fields on a handle.** A handle may carry a field block, read
THROUGH the pointer:

```
record AI tag addrinfo <word>
    ai_family: i32
    ai_next: AI
```

`ai.ai_family` is C's `ai->ai_family`; a field of the handle's own type is how a
list is walked; the fields are read and never written; a read through `nullptr`
aborts. **It needs a word**, because `tag` + fields already means by value; the
llm-ergonomist judges spellings blind.

**Route B — two records, one tag, and a read.** `one_tag_one_type` widens to
*one tag, at most one handle and at most one fielded record*. A built-in copies
the struct out of the handle — `a = ai.read()` is an `AddrInfo` — and
`ai_next: AI` in the fielded record is a pointer field, which a handle is. No
new syntax; one new name in § 11's `Built-ins:` sentence.

**Both routes owe an answer to the same third thing**: handing a record VALUE
to C by pointer, `const struct addrinfo *hints`. `@hints: AddrInfo` is what
exists; whether `@` may stand on an in-parameter is part of the question.

## Rules that bind every seat

Build in a COPY — `cp -r` the tree to your scratch directory, `rm -rf build`,
work there. The seed builds in about 3 s: `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`. **Never rebuild the compiler from `selfhost/`**
in a copy (cold cache, tens of minutes; it will kill you). `HEROES_RUNTIME=<copy>/runtime`
when building a program from elsewhere. `.env` carries `ANTHROPIC_API_KEY`:
`set -a; . ./.env; set +a`. **The working tree is frozen from this brief to the
synthesis**, and you do not write into it.

**A claim enters your report only after the command that settles it has been
run.** Where you could not run it, say so in your own words. A negative claim
names what you searched for. A number carries its unit in the same sentence.
Write your report to the path your own brief names.
