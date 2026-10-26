# Panel 230, the historian's brief

Read `00-shared.md` beside this file first. You are advisory, with no veto: you say who reached each
route first, what it cost them, and whether anyone reversed it. **Every precedent is verified with a
web search and the page re-read**, marked `verified` (you read the page and it says so),
`secondary` (a page about it says so) or `unverified`; only `verified` reaches the sitting's prior
art (`.claude/rules/records.md` § And the record says whose idea it was). Each line says what the
page itself states and nothing of what Heroes would do.

**Start from what this project's historians already sourced**, re-reading each page rather than
copying it: panel 159's (`docs/panel/159-reports/historian.md`, Go's refusal of a result from `main`
at `:134`) and panel 117's (its verdict table in `docs/panel/117-...md`, JEP 306, N1426, P2186R2, and
its prediction that a renderer reachable only from `assert` grows a second printer), and panel 131's
correction of the Zig precedent (`std.testing.expectEqual` against `expectEqualDeep`).

Your report is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panic-location/docs/panel/230-reports/historian.md`,
**written as you go, from your first finding**. No file outside the repository's root, no credential
printed, no paid run.

**The candidates below are questions, from the coordinator's memory; none is a premise.**

1. **A place passed, not computed (L1)**: Rust's `#[track_caller]` and `core::panic::Location`
   (which RFC, which release, what it costs at a call); Swift's `#file`/`#line`/`#function` default
   arguments on `fatalError` and `precondition`, and `#fileID` (which proposal, and why it moved from
   a path: the builder's path in a shipped binary is this sitting's finding 6); Zig's `@src()`; C's
   `assert` and what C99 added (`__func__`); GHC's `HasCallStack`; OCaml's `Assert_failure` with file,
   line and column. Which print a column, and in what form.
2. **A place found at the stop (L2, L4) or resolved later (L5)**: Go's runtime traceback from its own
   line table (what it prints, its exit status); Rust's backtrace and what it needs at run time;
   symbolication off-line (`atos`, `addr2line`, Breakpad or Crashpad's symbol files): who prints an
   address and resolves it elsewhere, and what that cost the reader.
3. **Both sides of a failed comparison (S1, S3)**: Rust's `assert_eq!` (`left`/`right`, `Debug`);
   pytest's assertion rewriting; Zig's `expectEqual` and `expectEqualDeep`; a test failure that
   prints a diff, and its bounds on a large value; **how a string side is written** (escaped, quoted)
   so the value cannot pass for a line of the report.
4. **A map shown in a fixed order**: whether Go's `fmt` prints maps sorted by key, since which
   release, with what reason; anyone ordering a display by the rendered text.
5. **`main` that fails (M1, M2, M4, M6)**: Rust's `fn main() -> Result<(), E>` (the `Termination`
   trait: which RFC, which release, what it prints, which status) and `std::process::ExitCode`; Zig's
   `pub fn main() !void` (what it prints, the status, the error return trace); Swift's top-level
   `throws`; Go's refusal (159's); C's `int main`; whether any reversed or amended the shape.
6. **A way to write on stderr (M5)**: Rust's `eprintln!`, Go's `fmt.Fprintln(os.Stderr, ...)`,
   Python's `print(..., file=sys.stderr)`: when each arrived, and whether a language added one after
   shipping without it.
7. **The statuses**: Rust's panic status (101?), Go's (2?), `abort()`'s 134 on POSIX shells, what
   Windows reports (`0xC0000409`, `__fastfail`); **a status past 255** (POSIX's `exit` and `_exit`
   keep the low eight bits; what Rust's `process::exit` and Go's `os.Exit` document about it, on
   POSIX and on Windows); `sysexits.h`; whether a language specification states what an abort exits
   with.
8. **Bounds on a rendering**: a debugger's or a test framework's limits on a large or deep value.

**Your output**: one section per question, each precedent a line with its link, its status, and what
the page states; then `prediction` (a route someone reversed, and why, or none found with what was
searched), and `condition`.
