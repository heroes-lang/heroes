# Panel 148 — ffi-pragmatist

Read `00-shared.md` first. You judge the founding constraint (design.md §1.11,
§4.19). **Veto on ABI breakage.** You are the seat that compiles.

## What only you are asked

1. **Write both spellings against real headers and compile them.** sqlite3,
   curl and libm are on this Mac; `examples/ledger/db/sqlite.hero` is a 370-line
   real binding with 17 functions.
2. **The question that decides C, and it is yours.** Option C infers the
   obligation from the group declaring a consumer. **Go and read real headers**
   and report how many libraries declare, in one header, BOTH a releaser for an
   opaque type AND a function handing back a borrowed instance of that same
   type. sqlite3 does — `sqlite3_next_stmt`, `sqlite3_db_handle`. Survey more:
   curl, libxml2, cairo, harfbuzz, freetype, OpenSSL, libuv, SDL. **A count, not
   an impression**, and name every header you opened.
3. **Under A, what does a binding author have to write that they do not today?**
   Count it on the shipped SQLite binding: how many of its 17 functions gain a
   word, and does any signature become ambiguous.
4. **Does either option break a binding that compiles today?** Compile
   `examples/ledger/` and `examples/curl/` under each and say.
5. **ABI**: anything that changes how a handle is passed or returned is a veto.

## Build discipline

In a copy, `rm -rf target build`. Seed builds in ~3.4 s; never rebuild from
`selfhost/`. For hand-written C you need no Heroes compiler at all.
