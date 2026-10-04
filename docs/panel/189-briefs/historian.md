# Panel 189, the historian's brief

Read `00-shared.md` and `00-facts.md` in this directory first; they bind you.
You judge precedent (your charter, `.claude/agents/historian.md`), and every
claim you bring is verified by a source you read, with its URL and the date
you read it; a claim you could not read says so. You run no command and no
paid session; web search and fetch are yours.

## What to find

1. **How compilers and interpreters tell a source file that is not valid in
   its encoding**: at least Go (gc and `go/scanner`), Rust (rustc), Python
   (and PEP 263), Swift, Zig, Java (javac), C# (Roslyn), clang and GCC (a C
   source's input charset), Nim, and any language you find whose front end is
   documented on this: the code or the words, whether it reports one
   occurrence per file or each one, whether it keeps reading past the first,
   whether it names the byte, the line and column, a likely encoding, and
   whether any of them applies a fix. Quote the message where you can.
   Include the routes that are not a refusal: an encoding declaration or a
   transcoding (Python's PEP 263, Ruby's magic comment, any other), and a
   front end that accepts a bad byte inside a comment; and UTF-16 sources.
2. **The name**: whether a precedent names this state by the bytes
   (*invalid UTF-8*), by the encoding (*not UTF-8*), by the file (*not a text
   file*), and which name a reader of the message needs (Q1).
3. **What else carries bytes that are not text into a compiler**: argv, the
   environment, file names (Q6), and what the precedents do with each.

Report: per question, the precedents with their sources; your verdict
(advisory, you have no veto); a falsifiable prediction; the condition that
would change it. Write `docs/panel/189-reports/historian.md` as you go.
