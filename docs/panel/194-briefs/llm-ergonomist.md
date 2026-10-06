# Panel 194, llm-ergonomist (the blind seat): how it runs

Rewritten after the critic's first pass, which found the first design unsound:
every session saw all three variants, so a reader's answer under one was
coloured by the others; one reader per task judged itself and nobody compiled
the programs; task 2 could not tell the variants apart because `connect` with
`sockaddr_un` is refused at `build`; and `claude -p` would have loaded the
coordinator's own settings (language, model, effort).

**The seat runs as fresh sessions outside the repository**
(`.claude/skills/panel/SKILL.md`), set up by the coordinator at the freeze:

- **Variants, one per session**, label-free in the reader's brief (the
  sentence simply stands in § 13 or does not): **L** no sentence; **M** R1's
  sentence with the union clause the spec-warden prices; **C** the sentence
  saying C's `char` is `i8`, alone. Three variants so each sentence's effect is
  measured apart.
- **Tasks, the same three**: t1 calls `uname` and prints the system name and
  the hardware type (Darwin's `utsname`, five `char[256]`); t2 connects a Unix
  stream socket to `/tmp/demo.sock` through **a shim header** declaring
  `int connect_un(int fd, const struct sockaddr_un *a, unsigned int len);`
  (the ffi-pragmatist's ruling that a shim is allowed); t3 locks and unlocks a
  mutex three times around a counter.
- **One reader per task and variant: nine sessions.** Each folder,
  `/tmp/b194-<task>-<variant>/`, holds only `spec.md` (the frozen spec, the
  variant's sentence in place), `headers.txt` (the C definitions as this
  Mac's SDK writes them, no comment that hints at validity), the shim where
  the task needs it, and `brief.md` (the task; write the program; list every
  choice point and what the other choice would produce; `context`: whether
  anything outside the folder reached you).
- **The command**, from the folder: `claude -p "Read brief.md in this
  directory and follow it exactly. Your inputs are the files in this
  directory only. Write report.md here. Answer in English." --restricted
  --safe-mode --strict-mcp-config --model claude-opus-5-5 --tools "Read,Write"
  --allowedTools "Read,Write" --disallowedTools
  "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage"
  --max-budget-usd 2 --output-format json > run.json 2> run.err`.
  **The bound: nine sessions, at most 2 USD each, 18 USD in all**, under the
  author's approval of 2026-10-06; no session beyond the nine.
- **Judged by compiling, never by stated belief** (the author's notes): the
  critic's second pass compiles and runs every program, L and C with the
  frozen compiler, M with the compiler-engineer's R1 prototype, on this Mac;
  a program is *correct* when it compiles and prints what the task asks, and
  *silently wrong* when it compiles and does something else. The synthesis
  reports per variant: correct first try, refused with a message that names
  the repair, silently wrong.
- The coordinator copies each `report.md` into `docs/panel/194-reports/blind/`
  with a header saying how it was run, and each folder's inputs into
  `docs/panel/194-briefs/blind/`.
