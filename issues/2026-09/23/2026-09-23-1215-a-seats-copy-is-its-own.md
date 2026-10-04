---
kind: task
area: process
milestone: M-agreed-retention
filed: 2026-09-20
commit: 64c9265411b6de2175b6235646f023035da991ae
github: none
---

# A seat's copy is its own

2026-09-23, M-agreed-retention step 2. The milestone's fourth item, closed by
the author's instruction, which is the only thing CLAUDE.md § 4 lets amend a
skill.

- [x] **M-agreed-retention** | `/panel`'s working rules do not say that a seat's tree copy is its own, and two seats shared one scratchpad in one sitting | **closed 2026-09-23**, author instruction | `.claude/skills/panel/SKILL.md` § Procedure, step 3 · `.claude/rules/verification.md`

    **Origin:** panel 170's completeness critic, 2026-09-20. **Filed rather than
    fixed, because CLAUDE.md § 4 says the skills are amended by author
    instruction and not by a panel.**

    Two seats shared one scratchpad and one rebuilt the other's compiler
    underneath it, which produced an emission divergence a seat reported as a
    question and the critic then traced to a stale binary. Independently, the
    compiler-engineer found its own first copy six commits behind and re-ran
    everything. **Twice in one sitting**, and it is
    `.claude/rules/verification.md` § *The compiler that judges is a build
    artifact* arriving inside a sitting rather than in a gate.

## What closed it

**The author's answer, 2026-09-23**, to the recommendation put at the
milestone's panel gate: *each seat that builds works in a directory of its own,
copied from HEAD, with its own compiler rebuilt from the seed inside it and
never shared.* The answer was yes, as recommended.

**What landed** is one paragraph in `.claude/skills/panel/SKILL.md`, step 3,
after the two that already said *build in a copy* and *the copy is not
sufficient on its own*: every brief names `<scratchpad>/<NNN>-<seat>/`, copied
from the frozen tree once `git status` reads clean and `git log -1` reads the
HEAD the briefs name; the seat builds its own compiler there from the seed; and
no seat reads, builds or runs inside another's.

**The two facts it rests on were re-read in the sitting's own reports before the
paragraph was written**, rather than carried from this item's body:
`docs/panel/170-reports/compiler-engineer.md:9` (*its `.git` read `7267db07`
(step 10), six commits behind*) and
`docs/panel/170-reports/completeness-critic.md:255-259` (*`scratchpad/tree/` is
the compiler-engineer's copy … Two seats shared one scratchpad path*).

**And the question step 1 left open is answered by the same report.** Step 1
wrote that whether a subagent's scratchpad is the coordinator's was not
measured. The critic's line 226 says it: *the scratchpad this session shares
with it*. So one location really was every seat's, and a directory per seat is
the smallest rule that makes two seats unable to meet.
