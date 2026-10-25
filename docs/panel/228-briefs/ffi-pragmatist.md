# Panel 228, the ffi-pragmatist's brief

Read `00-shared.md` in this folder first, and `../228-reports/critic-briefs.md` § 16, whose probe is
the reason you sit. This brief adds what only you are asked.

**Your worktree**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/228-ffi-pragmatist`,
detached at `9a38f1b26`, made by the coordinator; build your own compiler there with `clang -I
runtime seed/heroes.c runtime/runtime.c -o heroes` and, for the tree's own, `./heroes build
selfhost/main.hero -o heroes-next`. Write nothing outside the repository's root, read nothing of
another seat's worktree (the critic's probe is `.claude/worktrees/228-critic/critic228/jsonc/probe2.c`:
copy it, never build there), print no credential, start no paid run.

**The question**: the document `check --json` writes is read by programs, and the one this project
measured a consumer of is a C program over json-c (panel 193, `docs/panel/193-reports/ffi-pragmatist.md`).
For each route of the shared brief, write and compile the consumer a C author would write to learn
*whether the file is finished and, if not, what goes at each hole*, and run it on three answers: the
answer today's compiler gives (no `"holes"`), the answer the route gives, and, for every route that
changes the document, the older answer read by the newer consumer. Say for each: refuses loudly,
reads right, reads wrong in silence, or crashes, with the exit code and the command.

1. **A against D**: the critic's probe says the idiomatic json-c read of an absent `"holes"` faults
   (exit 139) and that D lets a consumer refuse by the number. Reproduce it, and say whether a
   consumer written to panel 193's own advice (check `"schema"` first) is safe under A, and what
   share of the consumers you would expect to write it that way (an inference, say so).
2. **B**: a consumer that reads stdout as one document, against `check --apply --json` and `check
   --json --dump-scopes` on a holed file under B (two documents, or text then a document, on one
   stream).
3. **C**: a consumer that reads `"diagnostics"` to decide *broken or not*, against a hole under C at
   exit 0 and at exit 1.
4. **E**: whether a count alone is what such a consumer needs, or whether it must then parse the
   text report, and what that costs it.
5. **The types**: whether a type as a string the file writes (`"lib.Box"`, `"[i64]"`, `"str?"`) is
   what a C consumer can use, or whether it would need a structure, and what a type the checker does
   not know should be (`?` today, which reads as the fallible suffix).

**Output** as `.claude/agents/ffi-pragmatist.md` asks. **Write
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rechecked-issues/docs/panel/228-reports/ffi-pragmatist.md`
as you go.**
