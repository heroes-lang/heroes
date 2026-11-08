# Panel 239, the historian's brief

Read `00-shared.md` and `proposal.md` beside this file first. You are advisory, with no veto. Every
precedent you cite you read on the web while writing, with its URL and the day read, marked
`verified`; a claim you could not read is marked `unverified` and nothing rests on it. Your worktree
is `.claude/worktrees/239-historian/` if you need the tree; you need no compiler. Your report is the
record the sitting's `## Prior art` is written from, so a line of it says only what the cited page
states (`.claude/rules/records.md` § And the record says whose idea it was).

## What the sitting needs from you

1. **One name, two libraries** (2363): what C itself says (C11 6.9p5, *exactly one external
   definition*, and what a program with two in two libraries is); how the linkers resolve a name two
   archives or two shared libraries define (GNU ld, gold, lld, ld64 and its two-level namespace,
   Microsoft's `link.exe`), what each can be asked to report or refuse (`--trace-symbol`, `-Map`,
   `--warn-backrefs`, gold's `--detect-odr-violations`, ld64's warnings about duplicate libraries,
   `LNK4006` and `LNK2005`), and when each appeared; ELF symbol interposition and why it is allowed
   (the dynamic loader's search order, `-Bsymbolic`, version scripts, protected visibility).
2. **How languages that link C for a user meet it**: Go's cgo (and its internal linker's *duplicate
   symbol* errors), Rust's `#[link]` and `cargo`'s `links` key (which refuses two crates linking one
   native library), Zig's build system, Nim, Swift Package Manager, Haskell's Cabal, and any build
   system that refuses or warns where two linked libraries define one name; dates where the change
   was a correction after a reported crash.
3. **One record, two layouts in two units** (2364): C11 6.2.7 (compatible types across translation
   units), the one-definition rule's checkers for C and C++ (GCC's `-Wodr` under LTO, gold's
   detection, `-fsanitize` checks if any), and how a binding generator that reads headers per module
   (bindgen, cgo, Zig's `@cImport`, Swift's Clang importer and its modules) keeps one C type laid out
   one way across a program; whether any orders other modules' headers inside one unit, or refuses
   where a macro of one header changes another's struct.
4. **Error messages for both**: a compiler, linker or build tool whose diagnostic names two
   libraries defining one symbol, or one type laid out two ways, and what it tells the user to do.

Your report: `docs/panel/239-reports/historian.md`, written as you go, ending as `00-shared.md`
§ Rules says.
