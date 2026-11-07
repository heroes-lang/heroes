# Panel 235, the llm-ergonomist's brief: the blind seat

Added 2026-11-07 on the completeness critic's first pass
(`docs/panel/235-reports/critic-briefs.md`, its item 19): the reader test
measures what models write and gives no verdict, so the seat that holds the
veto on non-local constructs (`.claude/agents/llm-ergonomist.md`) had nobody
in it. `use <json>/parse` means what another file, the catalogue, says; this
seat judges whether that is a construct whose meaning cannot be determined
from its line.

**How it runs**, as `/panel` § 2 requires of this seat since 2026-09-30: one
fresh `claude -p` session, never a subagent, in a folder of its own that
holds only its inputs, with `--restricted --safe-mode --strict-mcp-config
--bare`, the tools `Read` and `Write` and no other, `claude-opus-5-5` (or
`claude-sonnet-5-5` where the `claude` command refuses that model, which the
runner prints), `--max-budget-usd 2.00`, its output capped at 32,000 tokens a
call, charged to the reader test's ledger as its own kind with a stop of 3.00
(the cap plus one capped Opus 5.5 call, `blind/README.md` § The budget). The folder is
`build/b235-seat/` in `235-base` (git ignores `/build`), assembled by
`blind235.py seat`; it stands inside the repository's root, as the reader
test's sessions do, because CLAUDE.md § Hard stops writes no file outside it,
and `--bare` and `--safe-mode` keep the trunk's and the lane's `CLAUDE.md`
from the session, which its `context` answer checks.

**Its inputs**, and nothing else: today's specification unamended
(`spec/heroes-spec.md` at `235-base`), the three competing sections of the
reader test's U question as `variant-1.md` to `variant-3.md`, label-stripped,
in the order `blind/seat/variants.tsv` records (drawn by
`random.Random(235).shuffle`), the reader test's T2 and T3 as `task-1.md` and
`task-2.md` with T3's own module, and `blind/seat/brief.md`, the seat's method
and output structure without the project's names. It is never given
design.md, the repository, the spec's token counts or the author's position.

**Its report** is copied by the coordinator to
`docs/panel/235-reports/llm-ergonomist.md`, under a header saying how it was
run (the command, the model, the cost the CLI reported, the folder's sha256
list); a `context` answer of yes voids it until it is re-run from a clean
folder, which the seat's stop of 3.00 does not admit after a first session:
the coordinator raises it only within the author's 25 USD, and the synthesis
says so.

**Its row in the synthesis** is its `verdict` per variant, the veto on a
non-local construct included; its `prediction` is scored against the reader
test's pooled pass@1 for the same three arms.
