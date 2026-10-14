# Panel 215, the shared brief: what a C record promises when Heroes copies it or passes it by value

Written 2026-10-14 from 08:20 by the coordinator. **Every number, path and count below is followed
by the command that produced it, run while this brief was written** (`.claude/skills/panel/SKILL.md`
§ 2, CL-077); a sentence that says the tree lacks something names what was searched. Read your own
brief after this one.

## The sitting

Convened by the author's answer of 2026-10-14 at about 08:15, meant as *I ratify everything and
accept all your suggestions, proceed*, where the coordinator's suggestion was to convene sittings for
M-buildable-structs' issues 1334 and 1335, *no existing rule decides them*. The coordinator puts the
two into **one sitting**, since both ask what a group record (a C struct a group binds) promises when
the program copies it or passes it by value, and adds the question measurement 044 opened. A **full
panel**: the answer may be a mark in the language and a sentence in the specification (CLAUDE.md § 4).

**Three questions, each with the coordinator's routes** (the coordinator's list and not the option
set; a route nobody listed is worth more than a vote):

**Q1, padding across a call by value** (issue 1334). Panel 178's completeness critic, 2026-09-25: a
`{char c; double d}` record passed by value to a Heroes function and handed to `write()` carries
uninitialised bytes at offset 1 under MemorySanitizer at `-O0` to `-O3` on Linux x86-64, clean on Linux
arm64 (`docs/panel/178-reports/completeness-critic.md` § 1, its probe in
`docs/panel/178-reports/completeness-critic-work/w/pad/`: `copies.hero`, `small.hero`, `small.h`,
`msan.sh`, read). Re-run 2026-10-14 by M-buildable-structs' step 5 on clang 22.1.8 in emulated x86-64:
3 reports at `-O0`, all at the lend after `cd_through`, none at `-O1` to `-O3`
(`docs/roadmap/milestones/M-buildable-structs.md`, the paragraph on 1333 to 1335, read). The
specification promises nothing of padding, and Heroes' own `==` and `hash` walk fields, never bytes
(`.claude/rules/generated-c.md` § Arithmetic and memory). CERT DCL39-C names the class.
Routes: **A** nothing; **B** every construction of a group record zeroes the whole object first, as
`rest: zero` already does (`selfhost/emit/zeroed.hero`); **C** a group record crosses a call by
address; **D** a copy into a zeroed temporary before C receives it; **E** the record's padding named as
a field the program cannot read. **Measured by the critic's pass 1, B does not close it**: the probe
with every construction ending `rest: zero` (three `memset`s in its C) reads the same three reports
at `-O0` on the same line (`n2 = w_cd(@b)`, after `cd_through`): the bytes are lost in the by-value
call, not at the construction. Panel 178's probes no longer build as written (`small.hero` needs `@=`
six times, `copies.hero` two `=` and a `counted_by` rewrite); the critic's working copies are in
`docs/panel/215-reports/critic-work/pad/`.

**Q2, a record C forbids to copy** (issue 1335). POSIX does not allow a `pthread_mutex_t` to be
copied; `m2 = m` in Heroes copies it. Measured 2026-10-14 by step 5, on this Mac and on glibc: a copy
of a locked mutex reads locked (`trylock` 16), unlocks and locks again at 0, the original then unlocks
at 0, each destroys at 0, at exit 0: two locks where the program wrote one, and no message. The
emitted C passes a cell's address to C (`pthread_mutex_lock(&h0_m)`), so a local cell does not move
between calls (issue 1335's body, read). Routes: **A** nothing; **A2** the record bound `partial` with its mutex field undeclared, which today's
language already allows (the critic's pass 1: the copy still copies its bytes); **B** a mark on a group record saying
it is not a value, `=` and a by-value parameter refused for it; **C** the same decided from the
header (a struct holding a type C marks, by name or by attribute); **D** a check at run time.

**Q3, R1's payment.** **Corrected after the critic's pass 1**: panel 178 is superseded (its file says
*SUPERSEDED 2026-10-06*) and R1 was ratified by **panel 194**, whose spec-warden called 178's binding
count the wrong instrument and moved R1's payment to a reader test at the landing, unscored since
2026-10-06, with R1's objection condition: `u8` written for a C `char` by 3 or more of 10 readers under
R1 with K, or a nonzero byte under MemorySanitizer after `rest: zero`. So the paid experiment below
scores Q3. For the record, panel 178's spec-warden had made R1 turn to *object* if every long-array
construction stood in two bindings or fewer; measurement 044
(`docs/measurements/044-the-census-structs-m-core-packages-constructs-are-sockaddr-and-sha256-ctx-by-one-route-in-one-binding-or-two.md`,
read) counts `sockaddr` (`net`) and `SHA256_CTX` (`crypto/sha256`, by OpenSSL's low-level route) and
`fd_set` (`net`, only through `select`): **one binding or two**. R1 landed on 2026-10-06
(`838d27198`, `a975e3d6a`, `54c5f6187`, on `main`: `git merge-base --is-ancestor`), its sentence in § 13
at line 383 (`grep -n "rest: zero" spec/heroes-spec.md`). Routes: **A** R1 stands, judged on panel 194's conditions by the reader test; **B** R1 withdrawn with
its two sentences (lines 383 and 389) and K's with them or kept; **C** R1 stands and its sentence is
shortened; **D** panel 178's condition declared void, superseded by 194's. (`a975e3d6a` is K0, the
`char`-is-`i8` note, not R1; R1 is `838d27198` and its sentence `54c5f6187`.)

**What the sitting must rule**: each question's answer, the specification's tokens each costs, what a
program written today would meet, and what M-buildable-structs owes before it closes.

## The tree, frozen

- The sitting's tree: worktree `.claude/worktrees/lane-panel-215`, branch `lane-panel-215`, at
  `7e5c02048` (`git log --oneline -1`): the trunk `ecc1534aa` (v0.4.0) with panel 214's landing,
  defects 2056 and 2059 and M-buildable-structs' steps 1 to 6 merged, its full net running.
- **Your copy**: `git worktree add --detach .claude/worktrees/scratch-panel-215/<your seat> 7e5c02048`
  from the repository's root, then inside it `clang -I runtime seed/heroes.c runtime/runtime.c -o
  heroes` and `mkdir -p build`. Read, build and run only there; write your report into the sitting's
  tree at `.claude/worktrees/lane-panel-215/docs/panel/215-reports/<your seat>.md`, as you go.
  Rebuilding the compiler from `selfhost/`: `./heroes build selfhost/main.hero -o build/heroes-next`,
  about a minute.
- **The platforms**: `heroes-linux-arm64:latest` and `heroes-linux` (x86-64, emulated) in Docker
  (`docs/platforms/linux/LINUX-MACHINE.md`); the Windows box is on (`ssh win`, Git Bash, `/c/w`, clang
  23.1.1; keep the ssh call in the foreground; clean your files there at the end).
- **No paid run** but the one this sitting names (the llm-ergonomist's, run by the coordinator), **no
  credential printed** (never `gh auth token`), **no GitHub write**, **no push**, no `git add -A`, no
  `git commit -a`, no `git stash`, **nothing written outside the repository's root** (never the session
  scratchpad). Never pipe a suite into `head` or `tail`. A program that may not end is built and its
  binary run under `timeout`, its output bounded (`.claude/rules/verification.md` § A run that may not
  end).

## The evidence, measured

- **The specification**: `./heroes measure spec/heroes-spec.md` in the sitting's tree: 7519 legacy,
  7655 cl100k, real 10,048 (pinned 2026-10-13), against 10240; 132 real left once the FFI floor's 60
  is held back. A sentence's real cost needs a `--refresh`, which no seat starts.
- **Group records today**: `grep -rl '^extern' examples --include='*.hero' | wc -l` reads 21 (the
  critic's pass 1; unquoted, zsh prints 0), and the records they bind are the population a mark would
  meet; count them in your copy and say the command.
- **The census of programs with an `extern`**: 905 outside `docs/` on this tree (`git grep -l -E
  '^[[:space:]]*extern ' 7e5c02048 -- '*.hero' ':!docs' | wc -l`, the critic's pass 1; 878 on panel
  214's tree).

## What every report holds

The structure your agent definition gives (`.claude/agents/<seat>.md`), plus: for each question, the
route you would adopt and why, the route you refuse and the program or fact that refuses it; every
number with its command; a **prediction** naming the instrument that scores it and when; and what you
could not run.
