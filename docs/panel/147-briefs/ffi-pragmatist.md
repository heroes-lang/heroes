# Panel 147 — ffi-pragmatist

Read `00-shared.md` first. This brief adds what only you are asked for.

You judge the **founding constraint** (design.md §1.11, §4.19): there is no
standard library, so everything a program touches comes from C. You have a veto
on ABI breakage. **You are the seat that compiles.**

## Why this sitting is yours in particular

Panel 137's ffi seat is the reason this milestone's second item exists: it
measured, while pricing something else, that a `sqlite3_stmt *` advanced by a
loop is a **third** release obligation and that neither `owned` nor `lease`
reaches it. This sitting is that finding coming up for judgment.

## What to write and compile — real C, real libraries

Available on this Mac and already used by the corpus: `sqlite3` (3.51.0),
`curl`, `libm`. `examples/ledger/db/sqlite.hero` is a **370-line real binding**
with 17 functions, and `examples/curl/main.hero` is a second.

1. **Write the C that each route implies**, and compile it. Route A means the
   compiler emits a call to a named releaser at scope exit; Route B means it
   emits one at each exit edge of a block. Write both by hand as the C a
   compiler would have to produce, against real `sqlite3.h`, and compile them.
   Say which one C makes awkward and why.
2. **The question that decides Route A, and only you can answer it**: is a
   release function's signature **derivable from the header** for the libraries
   that matter? `sqlite3_finalize(sqlite3_stmt*)` returns `int`;
   `sqlite3_close(sqlite3*)` returns `int`; `curl_easy_cleanup(CURL*)` returns
   `void`. **Survey more than these three** — go and read real headers — and
   report how many take exactly one handle and nothing else. A releaser taking
   two arguments, or a flag, or returning something a program must check, is the
   shape that breaks Route A, and if it is common Route A is not what it looks
   like.
3. **The double-release and the escape.** Under Route A, what happens when a
   handle is released at scope exit AND the program also calls the releaser? The
   route says the compiler refuses your own call — check whether that is even
   writable given that `sqlite3_finalize` is an ordinary declared function a
   binding calls from a wrapper. `examples/ledger/db/sqlite.hero`'s `finalized`
   is exactly such a wrapper. **Would Route A make the shipped binding
   uncompilable?** Compile it and find out; that is a number, not an opinion.
4. **The counter-case.** `curl_easy_cleanup(handle: Curl consumes)` is already
   marked `consumes`. Does Route A make `consumes` redundant, complementary, or
   contradictory? Compile something that has both.
5. **ABI.** Anything in either route that changes how a handle is passed to or
   returned from C is a veto. Check, do not assume.

## Build discipline

**In a copy.** `cp -r` the tree to your scratch and `rm -rf target build` after.
The seed builds in ~3.4 s: `clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`. Do not rebuild from `selfhost/` — it is ~20 minutes and will kill you.

For hand-written C you do not need the Heroes compiler at all; `clang -lsqlite3`
is enough and is faster.

## What your report must contain

The C you compiled, the command, and its output. A verdict with a measured cost.
A falsifiable prediction. A negative claim — *"no library does X"* — must name
which headers you actually read.
