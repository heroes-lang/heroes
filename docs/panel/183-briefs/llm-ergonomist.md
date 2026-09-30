# Panel 183, brief for the llm-ergonomist

Your input is **the files of your own directory and nothing else**:
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/183-llm-ergonomist/`.
It holds `spec.md` (a copy of the language's specification, which you read in
full first), `task1.hero` and two outputs, `task1-output-P.txt` and
`task1-output-Q.txt`. A second program and its outputs come in a second
reading, when both of them exist. Read no
other file anywhere: not the repository, not a design document, not the other
seats' briefs, not a compiler. Your seat judges the objective, what a model
writing this language from its specification predicts and writes, and it has a
veto on a non-local construct. A copy of this brief is in your directory as
`brief.md`.

## The question

The program holds mistakes. Each output is what a compiler for this language
prints on it, both run; nothing tells you which is today's. The sitting
decides how a compiler should read a program past a mistake of a certain kind,
and where it should report it; which kind is for you to find.

## The tasks

1. **Predict, before you open any output.** For `task1.hero`, list every
   mistake you see, and for each one where you expect the compiler
   to report it (line and column), with the words of the specification you
   relied on. Say what, if anything, the specification tells you about the
   kind of mistake you judge the first to be.
2. **Fix from each output.** Take each of the two outputs in turn and write the program you would submit after reading that output
   alone (you may read the program; you may not use the other output or your
   prediction). Then count, for each output: the mistakes your fixed program
   still has, and the turns you expect it to take to reach a program that
   compiles.
3. **Compare.** Which output did you fix in fewer turns, and
   which message, if any, sent you to the wrong place or to the wrong fix?
4. **Your context.** Say whether any project rule, contract, memory or file
   other than your directory's reached your context while you worked. A yes
   voids this reading until it is re-run from a clean copy.

Write your report to
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/183-llm-ergonomist/report.md`
AS YOU GO. English, no em dashes. The coordinator copies it into the sitting's
records.
