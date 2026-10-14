# Panel 215, the llm-ergonomist's experiment (a paid run, by the author's budget)

**The author's budget, 2026-10-14 about 08:20, meant as: *you can run paid tests, I give you ten
dollars*.** This sitting spends at most 7 USD of it here, every run bounded by `--max-budget-usd`
and its spend read from its own JSON output. The seat is the experiment: ten fresh `claude -p`
sessions, one reader each, run by the coordinator, never a subagent and never inside the project's
context (`.claude/skills/panel/SKILL.md` § 2's command, with `--safe-mode`, `--restricted`,
`--strict-mcp-config`, Read and Write only, and `GIT_CEILING_DIRECTORIES` set so no git repository
is found above the reader's folder; the folder lives under `.claude/worktrees/blind-215/`, inside the
repository's root as CLAUDE.md § Hard stops asks, and ignored by git).

**Corrected after the critic's pass 1** (its report, `215-reports/critic-briefs.md` § 3 and § 4): the
tasks are split so that each measures one thing, and the readers' blindness rests on measured means.

**Each reader runs once**, with `--bare` (no CLAUDE.md discovery, no auto-memory, no hooks: `claude
--help`, read), `--restricted`, `--strict-mcp-config`, `--tools "Read,Write"`, `--allowedTools
"Read,Write"`, `--max-budget-usd 0.7`, authenticated by `ANTHROPIC_API_KEY` from the trunk's `.env`
(its length printed, never the key), from its own folder `.claude/worktrees/readers/r<NN>/` in which
`git init` was run first, so the session's git status is that empty repository's and not the project's
(panel 209's measured remedy). The folder holds `heroes-spec.md` (the specification at `7e5c02048`),
`sockpath.h`, `union.h`, `counter.h`, `tasks.md` and `brief.md`, which names no project, no sitting
and no expected answer, and says the helpers are defined in the headers.

**Task A1, a path into a fixed text field** (panel 194's R1 with K, and issue 1333's route T
condition): `struct sock_path { unsigned short family; char path[108]; }` and `static inline int
sp_send(const struct sock_path *p)`, returning `strnlen(p->path, 108)`; build one with family 1 and
path `/tmp/heroes.sock`, every other byte zero, and print what `sp_send` returns (16).

**Task A2, a record holding a union** (panel 194's *one task holding a union*): `struct tagged { int
kind; union { int fd; void *handle; } via; }` and `static inline int tg_kind(const struct tagged *t)`;
build one with kind 2 and the union zero, and print what `tg_kind` returns (2).

**Task B, a C type that must not be copied** (Q2): `struct counter { pthread_mutex_t lock; int64_t n;
}` with `counter_init`, `counter_lock`, `counter_unlock`, `counter_value`, each `static inline`;
write a function that increments the counter under its lock, call it three times from `main`, print
the value (3).

**Scored by the coordinator**, each program built with the sitting's compiler and run: first try
correct (builds, prints the line). A1: the path written correctly; `char` bound `i8` or `u8` (panel
194's objection condition: `u8` by 3 or more of 10 turns R1 to object; route T returns if writing the
path fails first try in at least half). A2: whether it builds, and how the union was bound. B: **read,
not run**, since a program that copies the mutex also prints 3: a copy is a `=` of the record, a
by-value parameter or a return by value after `counter_init`; an `@` parameter is passed by address and
is not a copy. The ten transcripts, programs and JSON outputs (with `total_cost_usd`) go to
`docs/panel/215-reports/llm-ergonomist-work/`, the report to `docs/panel/215-reports/llm-ergonomist.md`.
**Panel 194's three arms** (L, R1 honest, R1 honest with K) are not rebuilt: today's specification is
the third arm, landed, and the report says the other two stay unscored.
