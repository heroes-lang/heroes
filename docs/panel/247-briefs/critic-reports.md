# Panel 247, the completeness critic's second pass, over the reports

Read every report in `docs/panel/247-reports/` of the worktree `.claude/worktrees/p247`, whole: the
historian's, the spec-warden's, the compiler-engineer's, the ffi-pragmatist's and the blind seat's
(`llm-ergonomist.md`, assembled by the coordinator from its runner's grading), and your own first pass
(`critic-briefs.md`), with the briefs they answer (`docs/panel/247-briefs/`). You give no verdict.
Name what is MISSING: a route nobody listed, a claim a report asserts and did not settle with the
command that would, a contradiction between seats with which side is checkable and the command that
checks it, and the question the sitting should have asked and did not. Settle what you can with
commands of your own, in your folder.

The tensions the coordinator reads in the reports, which you check rather than accept:

1. **Refuse or defer.** The compiler-engineer and the ffi-pragmatist approve C (the refusal), the
   ffi-pragmatist in a repaired form (a per-thread depth kept by the runtime's own wrapping of
   `sigaction`, `signal` and `sigset`, and `SS_ONSTACK`, the walk optional), and both object to F
   and S; the historian approves a deferring route on precedent; the spec-warden vetoes E's and S's
   sentences on Principle 0; and the blind seat measured arm r (hrr, C with its remedy) 8 of 8 under
   C, arm s (hs) 8 of 8 under F (provisional, no safe-point runtime built) and arm e 4 of 8 under F,
   against arm x's 1. Does the blind data pay the ticket the spec-warden said E and S lacked, and
   for which route? What does each route do to the programs that work today (`quiet`, `t3x1`, the
   2496 case) and to a program's own C handler?
2. **The remedy hrr names.** Arm r's readers followed its remedy, a handler in C in a header the
   program ships, 8 of 8 right. The spec-warden measured that remedy failing silently when the header
   is bound by two modules (`swp6`, a `static` flag copied per unit), and the ffi-pragmatist measured
   that the class is the C library's (`snprintf` in a plain C handler killed 4 of 4 on Darwin), so
   *a C handler* is safe only when it does what POSIX allows. Is hrr's remedy sound as worded, and
   what would G, the runtime's own door that counts signals (built by the ffi-pragmatist, 71 lines),
   change in it? A sentence naming G was never measured on readers: say what that leaves the sitting.
3. **The ffi-pragmatist's repaired C.** Re-run its decisive cases from its folder's logs or its
   commands (the `fownlocale` and `fliblocale` storms, the `--sanitize` cases, the `qsort`
   comparator), say whether the repair holds as reported, and what of glibc's nine installers it
   covers.
4. **The blind seat's own findings**: the `sigemptyset` macro (filed by the coordinator as defect
   2511), `record sigset_t partial` with no fields refused `empty_record` with a fix that does not fit,
   a handler lending a buffer to `write` re-entering the lend tracking with a false message under
   today's runtime, and C's refusal printed twice when a second signal lands. Which belong to 2494's
   landing, and which are defects of their own?
5. **R4's reach.** Panel 246's R4, ratified, says no Heroes code runs in a handler. Is any route the
   reports approve in conflict with that sentence, or with spec section 13's callback sentence?

Work read-only in the repository and in the seats' folders; write only under
`.claude/worktrees/p247-scratch/critic2/` (set `TMPDIR` inside it) and return your report as your
final message (the harness may refuse a subagent a report file of its own; the coordinator writes it
to `docs/panel/247-reports/critic-reports.md`). No paid run, no `claude -p`, no API call, no credential
(`gh auth status` at most), no GitHub write, no file outside the repository's root, one heavy command
at a time (three lanes build on this Mac). A program that may not end runs as a built binary under
`timeout -s KILL`, its output bounded. Every number with the command that produced it.

End with a numbered list: each item the fact, the command, what it printed, and what the synthesis
owes it.
