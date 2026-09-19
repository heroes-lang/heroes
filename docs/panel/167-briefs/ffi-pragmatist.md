# Panel 167 — brief for the ffi-pragmatist

Read `00-shared.md` first. Compile, do not opine: every number you report comes
from a command you ran.

**Build in a COPY**, as the shared brief says: `cp -r`, `rm -rf archive build
target heroes`, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.

## What this seat is asked

**Write and compile the C that each route implies, and say which real bindings
each one makes possible or impossible.** The one that matters is the retaining
API, and this repository ships one: `sqlite3_bind_text` with `SQLITE_STATIC`,
`examples/ledger/db/sqlite.hero:107`. Bind it under each route and report what
happens.

**Three questions, each answerable only by running something:**

1. **Is retention ever visible to the toolchain?** Route H worked because clang
   reads `const` from the header. Is there anything a header states, or any
   flag, that says *this parameter is retained*? `-fbounds-safety`'s attribute
   family, `__attribute__((lifetimebound))`, the SDK's annotations — check them
   and report what your clang actually does with each, with the output.
2. **Can a copy be made to cost nothing where C does not retain?** A field lease
   copies always. Measure the copy for an 8-byte and a 4096-byte field against
   today's lend, and say whether the difference is one a binding author would
   notice.
3. **What does a real registration API need that a lend cannot give?** `iovec`,
   `setvbuf`, `sqlite3_bind_text`, and any other you find: is the bytes' owner
   always the program, or does some API want C to own them?

## What would change your verdict

Name it, and name what you searched. A sentence like *no real library does X* is
a claim about the option set; write what you grepped and where.
