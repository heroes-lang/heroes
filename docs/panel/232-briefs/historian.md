# Panel 232, the historian's brief

Read `00-shared.md` first. Your report is
`docs/panel/232-reports/historian.md`, written as you go. **Every precedent
you name is verified by a web search you ran, with its URL and the words the
page itself says**; one you cannot open is not cited, and a precedent is
marked `verified` only where you read the page this session
(`.claude/rules/records.md` § And the record says whose idea it was: a
decision's `## Prior art` cites only what the historian marked verified and
the session re-read). Say what the page states, never what Heroes did about
it. No credential printed, no paid run.

What the sitting needs from history, each a question and none a premise:

1. **The promise and its mechanism.** Read *Go 1 and the Future of Go
   Programs* whole and list what it promises and EVERY exclusion it makes,
   in its words; the critic counted eight, among them specification errors
   and the use of package `unsafe` (F27), which the coordinator's first
   brief had left out. Then Rust 1.0's *stability without stagnation* and
   its editions, and Nim 1.0's promise. Which of them promises one way only,
   and which lets a program declare the version it was written for.
2. **Evaluation order.** Which languages fix the order of an expression's
   parts, in which positions, and in what words. Go's specification does
   NOT fix it everywhere: the critic read `[]int{a, f()}` *may be [1, 2] or
   [2, 2]* there (F29). Java's and C#'s rules; C's unspecified order and
   what C++17 changed and why. And **an assignment's two phases**: whether a
   language specifies that a write evaluates its place's operands, then the
   value, then performs the write (the critic names Go's assignment, F4,
   unverified), the order this compiler keeps.
3. **A shift past the sign bit.** What Java, Go, Rust (debug and release),
   Swift and C# do with `x << n` that loses a set bit or changes the sign,
   and whether any of them traps; and `>>` on a negative.
4. **Diagnostic codes as a promise.** rustc's `E0xxx` codes, TypeScript's
   numbered diagnostics, C#'s `CSxxxx`: whether any language promises a
   code's stability, in what words.
5. **No exceptions, so a stop and a restart.** Erlang's *let it crash* and
   its supervisors, Go's `panic` across goroutines, Rust's `panic=abort`:
   what each says a long-running program does.

For each answer: the page, its words, and `verified` or not.
