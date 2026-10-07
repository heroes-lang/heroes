# Panel 196, llm-ergonomist: the blind seat, run by the coordinator

Run as fresh `claude -p` sessions outside the repository, never as a subagent
(`.claude/skills/panel/SKILL.md` § 2, the 2026-09-30 and 2026-10-01
instructions), with the command of panel 194's `blind-run.sh`: `--restricted
--safe-mode --strict-mcp-config --model claude-opus-5-5 --tools "Read,Write"
--allowedTools "Read,Write"`, the long `--disallowedTools`, **`--max-budget-usd
2` per session, nine sessions, three at a time**; each folder
`/tmp/b196-t<task>-<variant>/` holds `brief.md`, `spec.md` and `headers.txt`
alone, with no `CLAUDE.md` in or above it. The author approved the blind seat's
`claude -p` runs on 2026-10-06 (*fine to do the blind reader with claude -p, I
am not paying for it for now*). The folders' briefs are copied into
`docs/panel/196-briefs/blind/` with the sitting.

**The question the seat answers**: given only the specification, does a reader
bind a pointer C writes as an array as ONE cell (defect 396's program), and
does a route's sentence change what it writes?

**Variants, label-stripped** (each reader sees one `spec.md` and is not told
another exists):

- **C**, the frozen spec as it is (`spec/heroes-spec.md` at `39935f7c`);
- **A**, S1's sentence added to § 13 after the out-parameter example;
- **B**, S2's sentence: `counted_by` taking a constant or a number.

The two sentences are the coordinator's drafts, written into
`docs/panel/196-briefs/blind/variant-A.diff` and `variant-B.diff` before any
session starts; the spec-warden's own drafts may word them differently and the
synthesis says which wording a reader saw.

**Tasks** (each a short paragraph in `brief.md`, with `headers.txt` holding the
C declarations the task needs, copied from the real headers and naming them):

1. print SHA-256 of `abc` as 64 lowercase hex digits through OpenSSL's
   libcrypto (`SHA256_Init`, `SHA256_Update`, `SHA256_Final`,
   `SHA256_DIGEST_LENGTH`, `SHA256_CTX`); the fault's own shape;
2. print the binary exponent `frexp` gives for 8.0 (`double frexp(double x, int
   *exp);`); a correct single-cell out-parameter, the control a refusal must
   not break;
3. print this machine's host name through `gethostname(char *name, size_t
   namelen)`; a buffer whose count C does take.

**Repaired after the critic's first pass**: no task 1 or 3 has a correct
one-file answer under any variant, so **a reader may write C header files in
its folder** and puts each under a `files` heading of its report; task 3 is
not a clean control (its most natural wrong binding is the fault itself, with
a count present), so task 2 is the control; and every program is judged by
`build` and `run`, never `check` alone, since `check` runs no header probe.

**What is scored, from each `report.md`** (its headings `program`,
`files`, `choice_points`, `confidence`, `context`): whether the program binds a
buffer as one `@` cell; whether it would `build` and `run` on the frozen tree's compiler
(the coordinator runs every program, in its folder copy, on the frozen
compiler and on a seat's S1 or S2 prototype where one exists, and records the
exit codes); what the reader says it was least sure of; and whether any project
rule reached its context (a yes voids that reading).
