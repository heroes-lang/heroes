---
paths:
  - "selfhost/**"
  - "tests/**"
  - "examples/**"
  - "spec/**"
  - "docs/**"
  - ".claude/**"
---

# Which suites judge a change, and what may run beside them

Home of CLAUDE.md § Verification's two additions of 2026-09-09 (author
instruction). What each rule cost to learn is in `docs/records/contract/case-law.md`,
cited as `CL-NNN`. § Verification keeps the rules; this file keeps the map and
the reasoning, because a map is long and a rule is short.

## The map is not written here. The command that produces it is

**A list of which suite reads which directory is a premise about the world and
it expires in silence** (`.claude/rules/module-shape.md`). So the map below is
today's ANSWER and the command above it is the instrument. Re-run the command
rather than trusting the table:

```sh
# the twenty suite names the net registers, which are what `-- <compiler> <name>` takes
grep -oE '"[a-z_]+"' tests/harness/main.hero | sort -u

# what each suite file walks, from its own constants
for f in tests/harness/suite_*.hero; do
  printf "%-14s " "$(basename $f .hero | sed 's/^suite_//')"
  grep -ohE '"(tests/golden/[a-z-]+|examples|selfhost|spec/[a-z-]+\.md|docs/[a-z/]+)"' "$f" \
    | sort -u | tr '\n' ' '; echo
done
```

**`golden` is not one of the twenty**, measured 2026-09-09, and finding out
why corrected the map: `suite_golden.hero` runs as **four FORMS**, and their
names are the selectors — `check`, `ir`, `emit`, `unsupported`
(`tests/harness/main.hero:135-140`). So `-- <compiler> golden` selects nothing
and prints no line, and a suite name that selects nothing is a green run that
tested nothing. That is the failure this paragraph exists to prevent, and it
happened here first.

## What gates what, measured 2026-09-09

| touched | the suites that judge it |
|---|---|
| `selfhost/**` | `canonical` `layout` `order` `records`, **plus the compiler's own tests** |
| `tests/golden/check/**` | **`check`** `annotations` `canonical` `fixes` |
| `tests/golden/fixedbugs/**` | `annotations` `canonical` **`emission`** |
| `tests/golden/unsupported/**` | **`unsupported`** `annotations` `canonical` |
| `tests/golden/run/**` | `canonical` `determinism` `lines` `run` `warnings` |
| `tests/golden/emit/**` | **`emit`** `canonical` `determinism` `warnings` |
| `tests/golden/ir/**` | **`ir`** `canonical` `determinism` |
| `tests/golden/surface-fixtures/**` | `annotations` `fixes` |
| `examples/**` | `canonical` `corpus` `emission` `warnings` |
| `spec/heroes-spec.md` | `spec` `special` |
| `docs/**`, `DESIGN-LOG.md`, `CLAUDE.md`, `.claude/**` | `records` |
| `tests/harness/**` | **the net's own tests**, `heroes test tests/harness/main.hero` |

**`emission` on `tests/golden/fixedbugs/` is the row that cost this file.**
That suite's own comment states its premise: *"those cases are wrong FFI
bindings, and the thing that refuses them is clang, at build time. `--emit-c`
never calls clang, so all fourteen emit — measured."* On 2026-09-09 four cases
entered that directory which the CHECKER refuses, so they emitted nothing and
the suite went red — correctly. **The repair was to move the cases, not to
loosen the suite**: a defect the checker refuses belongs in
`tests/golden/check/` under a `fixedbugs-` prefix, which is where the milestone
before had already put four of its own, and which `emission` excludes by
design. A premise written down is a premise that fails loudly; this one did its
job.

## What may run beside a gate, and what may not

**Free: a pass-or-fail gate.** While a suite is deciding green or red, other
work may proceed — a subagent, a panel's judges, a second worktree — because
nothing another process does changes whether an assertion holds.

**Forbidden: anything while a clock runs.** CL-025 is unchanged and it is the
half that bites: time before and after with `/usr/bin/time -p`, and while a
clock runs the machine stays still. Paid for again on 2026-09-08, when a run
read `real 1870.49` against `user 65.37` — the ratio says it was waiting, so
the number was discarded and the honest re-run read 67 s. **The rule is
therefore: parallel work is free on correctness and forbidden on duration.**

**Forbidden: editing what the running suite reads.** A suite reading the tree
owns the tree until it exits (CL-025). `records` reads `CLAUDE.md`, `.claude/**`
and all of `docs/`, so amending the contract while the net runs is the panel
056 story with the coordinator in the judge's chair. Two routes out, and both
are better than waiting idle:

- **draft in the scratchpad** and apply when the suite exits — no conflict is
  possible, because nothing in the tree moves;
- **detach a git worktree** and work there. A worktree is a separate checkout
  with its own index, which is why it is safer than a second session in this
  one: CL-041 and CL-070 are both about a shared index carrying away another
  session's work, and a worktree has no such index to share.

That second route became a way of working on 2026-09-12, and its rules are
`.claude/rules/records.md` § Working in lanes: one milestone, one file, one
worktree. What made it possible is that the records stopped being monoliths —
a lane now writes files nobody else is writing — and what still binds is this
section's own line: parallel work is free on correctness and forbidden on
duration.

## And the order that makes this worth doing

CLAUDE.md § Verification already asks for it and CL-063 is its case: **the named
suites gate a sub-step, one at a time, and the full net runs once before a
push.** The value of the map above is that "the named suites" stops being a
judgement call. Read the map, run those, and keep the thirteen-minute net for
the push — where its cost buys something, because it is the last thing between
the work and a public branch.
