# Panel 177 — ffi-pragmatist

Read `00-shared.md` first. Your directory is `<scratchpad>/177-ffi-pragmatist/`,
HEAD `521c5e02`; build the compiler there from the seed. You judge the founding
constraint (design.md §1.11, §4.19: everything comes from C), with veto on ABI
breakage. Real libraries are on this machine under `/opt/homebrew/opt/<name>`:
`openssl@3`, `json-c`, `sqlite` and `raylib` are there and **cJSON is not**
(`ls -d`, 2026-09-24), which is why `cj.h` in this directory is a miniature of
it. Say which you use and their versions.

## What to settle, by compiling real C

1. **Route R against real libraries.** R aborts a handle that reaches a
   NON-consuming parameter unless it is in the live set, and a handle a
   `borrows` call hands back is never in the set. In the libraries you bind,
   find the borrowed handles of a type some function consumes — json-c's
   `json_object_object_get` handing back a child the parent owns, SQLite's `sqlite3_next_stmt` and `sqlite3_db_handle`,
   OpenSSL's `SSL_get_rbio` and `SSL_get_SSL_CTX` are candidates, not a census
   — and how often a correct program hands one to a non-consuming call. Write
   and run the programs; report how many correct ones R as stated would abort.
2. **Route P and the copy it misses.** P poisons the binding a consuming call
   was handed, and a copy made before the call keeps the address. How often do
   the real idioms of these libraries copy a handle into a second binding or a
   record field before releasing it? Show the shapes, run them.
3. **Route S and C's structs.** S makes a handle two words. Which real headers
   hand out handles INSIDE a struct the binding declares as a group record
   (raylib's `Font.texture` is one; `examples/` has raylib bindings), and what
   would S do to them? Would S change a single byte C sees in any binding you
   build? If yes, that is your veto's territory: say so.
4. **Question 2's success values, re-run.** Panel 176's critic listed json-c 0,
   OpenSSL `add0`/`set0` 1, cJSON `true`, `SSL_set0_rbio` `void` — carried, not
   re-run for this brief. Read them from the installed headers or man pages
   (cJSON's is not installed, so its row is unrun unless you can reach its
   header) and say which transfer only on success, which always, and which
   document neither.
5. **The two placements of the success clause.** Write the json-c
   `json_object_object_add` (a function, `json_object.h:403`, json-c 0.19) and
   one OpenSSL `add0` binding under both — **pick an `add0` the header declares
   as a function**, since `SSL_CTX_add0_chain_cert` is a macro (`ssl.h:1441`,
   OpenSSL 3.6.4), and say which: on the parameter, `val: Json transfers json_object_put on 0`;
   on the result, `-> i32 when 0`. Which can a binding author write from the
   header's own comment without reading the source? Which fails loudly when
   written wrong? These spellings do not compile anywhere; write them as the
   binding would read and say what C each implies.
6. **`retains` on a parameter.** Bind `X509_up_ref` (a status result,
   `x509.h:865`) and `json_object_get` (a handle result, `json_object.h:160`) as
   panel 176 adopted them, and write the C each implies at the call.

## Say for every claim which of these it is

Compiled and run, on which legs; or argued, and why it could not be compiled.
