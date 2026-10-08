# Panel 199, llm-ergonomist (blind seat): the coordinator's scoring of the six readings

Written by the coordinator at 10:43 on 2026-10-08 (`date`). The six sessions
(`llm-ergonomist-r1-n.md`, `-r2-n.md`, `-r3-m.md` for variant A, today's compiler
output; `-r1-m.md`, `-r2-m.md`, `-r3-n.md` for variant B, the compiler-engineer's
prototype) ran as fresh `claude -p` sessions, `claude-opus-5-5`, from 09:54 and
10:41, and cost 1.0042 USD together by their `run.json` (6 USD approved, 7
sessions at 0.85 USD allowed; the seventh, `r2-o`, was not run, see the
prediction file). Scored as the brief says: each answer's program applied by the
coordinator in a copy, `check`, then `build` at `-O0` and at `-O2` with the
frozen tree's compiler built from its seed, each binary under `timeout 10`
through `head -c 4096` (`<scratchpad>/readings-score/score.sh`); correct when
`check` is 0 and both binaries print the key's line at exit 0 (2, 120 and 3).

| folder | task | variant | check | `-O0` prints | `-O2` prints | cost, USD |
|---|---|---|---|---|---|---|
| `r1-n` | 1 (`p1`) | A | exit 0, (silent) | `2` | `2` | 0.1750 |
| `r2-n` | 2 (`p2`) | A | exit 0, (silent) | `120` | `120` | 0.1674 |
| `r3-m` | 3 (`p3`) | A | exit 0, (silent) | `3` | `3` | 0.1717 |
| `r1-m` | 1 (`p1`) | B | exit 0, (silent) | `2` | `2` | 0.1742 |
| `r2-m` | 2 (`p2`) | B | exit 0, (silent) | `120` | `120` | 0.1615 |
| `r3-n` | 3 (`p3`) | B | exit 0, (silent) | `3` | `3` | 0.1543 |

**Every reading is correct**: six of six, at both levels. Variant A, the
status quo (clang's warning in C words and the panic, or the silent hang),
scored three of three, so variant B had no room above it: **this experiment
shows B is not worse than A, and cannot show it is better**.

**Against the coordinator's prediction** (`llm-ergonomist-prediction.md`, written
before the B sessions started): clause 1 held (all three B programs repaired
and correct); clause 2 held (each B reader named the error, and on `p1` and `p3`
the UFCS note, as what told it what to change; each A reader had named the clang
warning, the panic or the hang); clause 3's first half held (the `p2` readers of
both variants weighed `n <= 1` against `n == 0` in their `choice_points`) and its
second half was falsified in part: **`r2-m`'s `confidence` hesitates on the
message's meaning**, whether the checker accepts as *a path that returns before
the call* a base case inside an `if` with no `else`. It expected so, and it was
right (the program checks), but the note's wording is what it was least sure of,
which a reading of variant A on `p2` could not have been (A printed no
wording). Clause 4 held: every `context` named the folder's files and the
harness's environment information, with the account's email address among it,
and no project rule, contract or memory.

**What this does and does not say about the sitting's question.** It measures
what a reader does once a mistake is made and shown. The question the sitting
turns on, whether a model writes the self-call in the first place (the critic's
question 7), is asked by no reading here, and the reading of `p2` is the
program's own missing base case, not a self-call through UFCS. Two further
limits: the six readers saw the same `spec.md` (the spec-warden's drafts were
not tried, one change per variant), and the A transcripts for `p1` and `p3`
carry clang's warning, so A is the status quo including a warning the
spec-warden measured false on a correct program (`hidden2.hero`, defect 507's
sibling, `spec-warden.md`).
