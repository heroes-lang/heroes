# Panel 188, the historian's brief

Read `00-shared.md` and `00-facts.md` in this directory first. You are the
precedent seat (your charter, `.claude/agents/historian.md`): every claim you
make is verified by a web search and cited with its URL and the date you read
it; an unsourced precedent is inadmissible.

1. **C's header-name rule.** Find the text of C11 6.4.7 (Header names) in a
   committee draft (N1570 is the usual C11 one), quote the paragraph on which
   characters are undefined between `<` and `>`, give its number, and say
   whether C17 (N2310) or C23 (N3096 or the final draft) changed it. The
   shared brief's recollection is a question until you answer it.
2. **What compilers document.** How GCC, clang and MSVC document a header
   name holding `\`, `"` or a line end, and whether MSVC reads `\` as a
   separator.
3. **What other front ends do with the name a binding includes.** Zig's
   `@cInclude`, Go's cgo preamble, Rust's `bindgen` and the `cc` crate, Nim's
   `header` pragma, Swift's module maps, D's ImportC, Python's cffi: does any
   validate the name before handing it to C, and with what message.

Write your report to `docs/panel/188-reports/historian.md` as you go: per
question, what you found, its source and date, what it means for Q1 to Q4, and
a falsifiable prediction.
