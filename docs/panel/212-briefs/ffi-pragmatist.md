# Panel 212, the ffi-pragmatist's brief

Read `00-shared.md` first, all of it (revised after the critic's pass); then
`.claude/agents/ffi-pragmatist.md`. You have a veto on ABI breakage and on anything that makes the
founding constraint (design.md §1.11, §4.19: there is no standard library, everything comes from C)
a fiction.

**Your question is the proposal's Q2: how does the verb talk to GitHub?** Make your own worktree and
compiler as the shared brief says. Three routes are on the table, and you may add a fourth if you
can say what makes it exist.

**Route A, spawn `gh`.** `selfhost/cli/process.hero` spawns programs one word at a time and
**`process.run` hard-codes the standard-input path to empty** (`process.hero:96-103`, the critic):
so `gh api --input -` is not reachable, `gh api --input <file>` is (the body in a scratch file,
as `refresh.hero` does). Establish with commands, against a **read** endpoint only
(`gh api repos/heroes-lang/heroes`, `gh api rate_limit`, `gh api -i ...`): what a Heroes program
sees of `gh`'s exit code, output and standard error when `gh` is missing, unauthenticated,
rate-limited or killed; where a secondary rate limit shows (status, headers, `Retry-After`), given
that a spawned `gh` writes to a file; and whether the body of 54,818 bytes passes.

**Route B, libcurl across the C boundary.** Write, and **compile with clang against the system's
curl headers**, the binding a real `extern` group would need for one HTTPS POST with an
`Authorization` header and a JSON body, returning the status and the body: the group, the records,
which parameters are `lent`, `@`, `counted_by`, `consumes`, and what spec §13 lets you write. Show it
compiles, or the clang error and the sentence of §13 that refuses it.

**Route C, spawn `curl`, which the compiler already does.** `selfhost/cli/refresh.hero:81-82`
(`heroes measure --refresh`): the body in a scratch file, the key in neither argv nor a file
(`--variable`, `--expand-header`), curl 8.3.0 or newer. Read it, say what it would take to point it
at `https://api.github.com/repos/.../issues`, and what it does when `curl` is missing or too old
(the local one is 8.7.1).

**Route D, `gh api graphql`** with several aliased mutations in one request: whether the answer's
shape (an `errors` array beside partial `data`) is something `process.hero`'s result can carry back.

Then compare, with what you ran: failure modes (timeout, partial write, redirect, **killed between
the request and the answer, so that GitHub holds the issue and the file does not know**); where the
token comes from (an environment variable, `gh auth token`) and **what shows it to another process**
(for each route, the command: `ps -o args= -p <pid>` while the request runs); the C-boundary surface
each adds; and JSON: `selfhost/cli/json_text.hero` already escapes strings, so say whether any route
needs a second escape.

**You make no request that changes GitHub and run no `gh issue create`.** Reads only. Your report is
`docs/panel/212-reports/ffi-pragmatist.md` in the lane tree.
