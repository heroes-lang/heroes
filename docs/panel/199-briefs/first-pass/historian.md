# Panel 199, historian

Read `00-shared.md` first, whole. You have web search: every precedent cited with its source and date, or marked unverified.

1. How other compilers tell a function whose every path calls itself: rustc's `unconditional_recursion` (warn by default? since which version?), Swift's *all paths through this function will call itself*, clang's and GCC's `-Winfinite-recursion` (GCC since 12?), Go's `vet` (a check?), Kotlin, C#, Java (javac?), Haskell (GHC: no?), Zig. Error or warning, on by default or not, and what each misses (mutual recursion, generics, calls through a value).
2. Any record of a language that made it an ERROR, and why; any that removed it, and why.
3. The UFCS shape: a method named like a built-in calling the built-in on its receiver, in D (UFCS) or Nim (method call syntax): known pitfalls, documented.

Advisory, no veto. Write what was searched where you found nothing.
