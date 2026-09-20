# Panel 169 — brief for the ffi-pragmatist

Read `docs/panel/169-briefs/00-shared.md` first.

## Your axis

design.md §1.11: there is no standard library, everything comes from C. You hold
the veto on ABI breakage, and you are the only seat that writes and compiles the
binding a route implies.

## Your own record in this milestone

At panel 167 you supplied the ground for a trailing header. At panel 168 you
**withdrew that prediction on your own measurement**, having asked whether
`sqlite3_free` is `free` and found that it need not be —
`sqlite3_config(SQLITE_CONFIG_MALLOC)` replaced gives `134 134 134 133 133`.
That is the standard this sitting holds you to and it is a compliment.

Your panel 168 evidence is committed: `docs/panel/168-briefs/bind_blob.c`,
`allocator.c`, `failpath.c`, `fieldlease.c`. Reuse them.

## The part only you do

**R1, the handle route, is a binding question before it is a compiler
question.** The shared brief measures that `borrows`/`acquires`/`consumes` work
on a handle and are refused on a `cstr`, and that the live set aborts at exit.
Write the bindings and say whether the route is liveable:

1. **Bind a real retaining API through a handle.** `sqlite3_bind_text` /
   `sqlite3_bind_blob` with `SQLITE_STATIC` is the case: the library keeps the
   pointer until the statement is reset or finalized. Can the buffer be a
   handle the program acquires and consumes, with the live set counting it? What
   does the author write, and how much worse is it than `s.lease()`?
2. **`record Block tag void`** — `unread_mark`'s own note says this is C's
   `void *`. Compile it. Does it reach a `const void *` parameter? Does
   `acquires`/`consumes` read on it? If it does, the language may already have
   the answer and nobody has written the binding.
3. **Count what R4 would cost now.** Panel 167 refused withdrawing the field lend
   on your veto: 28 `const void *` in `sqlite3.h` and 7 in `raylib.h` unbindable
   without shims. Your own panel 168 ruler read **37** in `sqlite3.h`. Re-count,
   and then ask the question that was never asked: **how many of those
   declarations does a program actually reach through a LEND, as opposed to
   through a handle or a C-owned buffer?** The veto was about bindability, and
   bindability through a handle is not the same measurement.
4. **069.** A destructor callback is how every real library expresses retention,
   and Heroes cannot pass one. Judge whether 069's repair is a precondition for
   R1 and for §4.19's third case, or independent of both.

## The questions

1. Which route closes 066 in a binding you would actually write?
2. Does R2 (the caller-side rule) break any binding in this repository?
   `examples/ledger/db/sqlite.hero` is the one with a real library behind it.
3. **Is there a route nobody listed?** You reframed the last sitting once. Try
   again.
4. Does anything here break ABI?
5. **Register a falsifiable prediction** with an instrument that exists today.

## Working rules

- `cp -r` the tree to your scratchpad, then `rm -rf target build` in the copy.
- A compiler in 3.80 s:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. Never rebuild
  from `selfhost/`.
- Every number from a command run in this session, named. A negative claim is run
  or it is a question naming what you searched.
- Report to `docs/panel/169-reports/ffi-pragmatist.md`. Veto on ABI breakage.
