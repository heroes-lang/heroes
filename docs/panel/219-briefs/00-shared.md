# Panel 219, the shared brief: the seed's emission profile, without the `#line` directives nobody reads

Written 2026-10-17 from 03:10 by the coordinator, on the trunk at `cbf724983`, whose seed was
regenerated at `bf6a9c4eb`. Convened on issue 2086 (https://github.com/heroes-lang/heroes/issues/2086),
a task of M-bounded-seed, by the author's answer that night accepting the coordinator's
recommendation, *convene the sitting* (the milestone's panel gate). Read first:
`docs/roadmap/milestones/M-bounded-seed.md` (its part 1, *the shape*) and
`docs/measurements/045-the-seed-is-seventy-percent-of-the-history-and-a-rewrite-that-keeps-it-at-the-releases-clones-in-seventy-one-mebibytes.md`.
**Seats**: compiler-engineer (the backend, the tool surface, the ceiling), historian (how other
self-hosting compilers keep their bootstrap), then a completeness critic.

## The question

The seed, `seed/heroes.c`, is the compiler's own emitted C that a clean checkout builds with one
clang line; nobody steps through it in a debugger, and half of it is `#line` directives. **Should
the seed be written in an emission profile of its own, and which: without `#line`, with the line
number alone (C11 § 6.10.4), with a bootstrap root that leaves out `mutate`, `probe` and `measure`,
or another shape; and how is that profile asked for, given that CLAUDE.md § 10 refuses `--no-line`
as a flag?** The milestone's target is a number: a seed under 10 MB.

## What was measured while this brief was written (every number is a command's)

This Mac, the seed at `bf6a9c4eb`:

| shape | bytes | `xz -9` | how |
|---|---|---|---|
| the seed as written | 51,514,006 (1,391,162 lines) | 3,983,808 | `wc -c`, `wc -l` |
| its `#line` lines | 621,943 lines, 24,710,575 bytes (48.0%) | — | `grep -c '^#line '`, `grep '^#line ' \| wc -c` |
| without any `#line` | 26,803,431 | 2,504,280 | `grep -v '^#line '` |
| `#line N` with no file name | 32,868,681 | 3,512,328 | `sed -E 's/^(#line [0-9]+) "[^"]*"$/\1/'` |

`clang -I runtime -c` on the seed as written: `real 7.42 user 7.29`; without `#line`: `real 7.26
user 7.16` (load 2.0, `real` within 0.15 s of `user`, so neither run waited). So `#line` costs bytes
and history, not compile time. Neither shape alone reaches 10 MB; the bootstrap root without
`mutate`, `probe` and `measure` is unmeasured.

**Unrun, and questions rather than premises**: the seed's size per module, and what a bootstrap
root without those three costs and saves; whether the compiler a bootstrap-root seed builds can
build `selfhost/main.hero` whole (the two-stage rhythm of issue 2087 needs it to); what a seed
without `#line` costs a crash in the seed-built compiler (its stack trace then names C lines);
how the profile is asked for under § 10's stopping rule (a subcommand, an input the compiler reads,
the regeneration's own command line), and what `seed/README.md`, the CI's fixpoint step and the
hook that judges a stale compiler then say.

## What each seat is asked

Rule on the question with numbers: each shape's size (bytes and `xz -9`), whether it builds and
passes the fixpoint, what it costs and saves, how it is asked for, and your verdict per shape
(compiler-engineer: adopt / object / veto on the ceiling and the tool surface; historian: approve /
object, every precedent sourced and dated, *verified* or *unverified*). CLAUDE.md § Precedence
ranks robustness above token cost and size.

## The rules every seat keeps

- **Build in your own copy**, a detached worktree under
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/219-<seat>` made with
  `git -C /Users/joseph/Temp/heroes/heroes-lang worktree add --detach <path> cbf724983`, or your
  scratchpad folder `<scratchpad>/219-<seat>/`; then `rm -rf build` and your compiler from the
  seed. Never the trunk's `./heroes`, never the trunk's tracked files, never the lane
  `lane-land215`. Remove a worktree you added when you finish.
- **Write your report as you go**, to `docs/panel/219-reports/<seat>.md`, the one tracked file you
  write. Every number names its command; what you could not run says *unrun*.
- **No paid run**; **no credential printed**; **a program that may not end** bounded in time and
  bytes; **nothing outside the repository's root** but your scratchpad and the Linux containers.
