# Panel 222, the shared brief: how a gate knows a compiler built from a lagging seed is stale

Written 2026-10-17 from 12:40 by the coordinator, on the trunk at `b2c81211d`. Convened on task
2087 (https://github.com/heroes-lang/heroes/issues/2087), M-bounded-seed's part 2, the rhythm,
whose own text says it is a step that designs its answer first. Read first:
`docs/roadmap/milestones/M-bounded-seed.md` (part 2, and the decisions of 2026-10-15 and
2026-10-17) and panel 219's file, ratified 2026-10-17,
`docs/panel/219-the-seed-is-the-compilers-emission-with-a-re-anchoring-line-joined-to-its-anchor-and-a-file-name-written-where-it-changes-for-every-program.md`,
with its critic's § 4 (`docs/panel/219-reports/critic.md`). **Seats**: compiler-engineer
(backend, tool surface, ceiling), historian (how other self-hosting compilers tell a stale stage),
then a completeness critic.

## The question

The policy the author decided on 2026-10-14: the compiler is always built in two stages, clang on
the seed and then `heroes build selfhost/main.hero` with what clang made; the seed is refreshed
only when that two-stage build goes red, and at each release. Today the seed is regenerated at
every batch's close. **What tells a gate, a hook and the CI that a compiler was built from
sources older than the tree's, when its file is newer than every source?** And what moves with
it: `.github/workflows/ci.yml` (four legs), `.claude/hooks/guard_bash.py` and
`.claude/hooks/trees.py` (the stale-compiler check, which compares modification times),
`.claude/rules/verification.md` § The batch, `seed/README.md` (its one-line bootstrap, printed on
34 lines of 24 files per panel 219's compiler-engineer, the site's start page among them), and
`docs/roadmap/verify.md`.

## What is known (every number a command's, in the files cited)

- `HERO_RUNTIME_ABI` moved 30 times, 8 in the 30 days to 2026-10-17 (`git log -G'define
  HERO_RUNTIME_ABI' -- runtime/heroes_runtime.h`, panel 219's critic), and the seed carries a
  `_Static_assert` on it, so a seed older than the runtime's interface stops at clang's first
  lines; refreshed only when red, about 8 refreshes a month at September's pace. A frozen runtime
  beside the seed was priced there (33 files, 687,335 bytes, a third stage of about 70 s) and not
  built.
- The stale check today: a harness run or a commit is refused where the named compiler's
  modification time is older than `seed/heroes.c` or the newest file under `selfhost/` or
  `runtime/` (`.claude/rules/verification.md` § A suite is the last judge, layer 1).
- Panel 219's compiler-engineer measured a two-stage build cold at `real 70.58` on this Mac and a
  build-only bootstrap root; the critic, history per regeneration under s4 at 60% of s0's.

**Unrun, and questions rather than premises**: the routes. Among those the coordinator can name
(the list is the coordinator's, from the task's text; a route not here is the most valuable thing a
seat can bring): (a) the compiler carries a digest of the sources it was built from, printed by an
existing verb (`--version`), and the hook compares it with the tree's; (b) the seed carries the
digest of the sources it was emitted from, and a stage-1 compiler is never allowed to judge, by
rule and by a check of how it was built; (c) the gate always builds in two stages and the hook
only checks the stage-2 artefact; (d) nothing, the policy written and the hook left as it is.
For each: what it costs (compiler lines, a surface word, CI minutes per leg, the bootstrap line
strangers type), what it refuses and lets through, and how it fails when it fails.

## What each seat is asked

Rule on the question with numbers, built far enough to be real: the compiler-engineer adopt /
object / veto per route on the ceiling and the tool surface (`.claude/rules/cli-surface.md`'s
stopping rule); the historian approve / object, every precedent sourced and dated, *verified* or
*unverified*.

## Rules

- **Build in your own copy**: a detached worktree under
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/222-<seat>` from `b2c81211d`, or your
  scratchpad folder; `rm -rf build`, your compiler from the seed. Never the trunk's `./heroes`, its
  tracked files, or the lanes `lane-land215` and `lane-s4`. Remove a worktree you added.
- **Write your report as you go** to `docs/panel/222-reports/<seat>.md` in the trunk, the one file
  you write there; every number names its command; what you could not run says *unrun*.
- No paid run, no credential printed, nothing outside the repository's root but your scratchpad and
  the Linux containers; no GitHub write, no push, no CI run of your own.
