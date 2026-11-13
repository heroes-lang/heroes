# Panel 247, the completeness critic's first pass, over the briefs

Read every file in `docs/panel/247-briefs/` of the worktree `.claude/worktrees/p247`, whole. Before
any seat starts, name every framing fact a brief states that you cannot verify with a command you run
(a line number, a path, a count, a hash, a price, a claim about what a document, the spec, a log or the
compiler says or does not say), each with the command you ran and what it printed; every route nobody
listed that a reader of defect 2494 (`gh issue view 2494 --repo heroes-lang/heroes`), of panel 246's
R4 (`docs/panel/246-*.md:301-326`) and of the lane's material under
`.claude/worktrees/b34-scratch/b34-signals/` would ask about; and every question the sitting should ask
and does not.

Things the briefs' author flagged as inferred, read rather than run, or unsettled, which you settle or
hand to a seat with the command that would settle them:

1. **The storm's `exit -9`**: `r2494/before-storm.log` reads `exit -9` three times, and the driver
   (`r2494/storm.py`) prints `NOT ended 30 s after the storm` when the SIGKILL is its own; read the
   driver and the log, and say whose SIGKILL it was, or that it cannot be told from them.
2. **What the prototypes are**: 00-shared.md says each is the runtime at `62ce17563` with one change
   (protoC: `thread.c` +56 lines and one line of `os.c`; protoD: `alloc.c` +31), measured by `diff`
   against `git -C .claude/worktrees/b34-round archive 62ce17563 runtime`. Re-run it.
3. **The prices and the hashes**: `spec-warden.md`'s table and `llm-ergonomist.md`'s three arm hashes,
   rebuilt from the briefs' own words (not from `p247-scratch/price/`) and measured with your own
   stage 2's `measure` from your tree's root; and the twelve prepared session folders under
   `.claude/worktrees/p247-scratch/247-llm-ergonomist/` (read, never run: each holds `brief.md`,
   `task.md` and `heroes-spec.md`, and the arm's hash must match).
4. **Panel 246 and a program's own handler**: 00-shared.md says R4 describes the layer's mechanism and
   does not rule on a program's own Heroes handler, after a grep that printed lines 102, 152 and 305.
   Read the sitting whole and its two critic reports (`docs/panel/246-reports/critic-*.md`) and say
   whether a ruling stands that this sitting would contradict.
5. **The three tracked programs** 00-shared.md names as handing C a Heroes handler, and what each
   does today: run them with your compiler (`./heroes-next run tests/harness/main.hero -- ./heroes-next
   run <substring>`), and search the tree for any other program that hands a Heroes function to a C
   function that installs a handler (`signal`, `sigset`, `bsd_signal`, `sigaction` through a
   program's own header, `SetConsoleCtrlHandler`).
6. **Route F** (C's detection with E's delivery) is the coordinator's addition and nobody built it;
   say whether it is a route or a restatement of E, and whether a route that defers to a safe point on
   the interrupted thread itself (neither a runtime thread nor a refusal) is missing from the list.
7. **The briefs' bounds**: the blind seat's twelve sessions at a 0.40 USD cap and a runner bound of
   3.60 USD inside the 5 USD ceiling awaiting the author's yes; the storm runs each seat makes (time,
   bytes, processes); and whether any brief asks a seat for a run its rules forbid.

Build a compiler at the base (`ae5e2cedd`, 00-shared.md § The base, `sources 7cc70d136b590d46`) in
`.claude/worktrees/p247-scratch/critic/` from the seed if a check needs one. Work read-only in the
repository and in the lane's folder; write only under `.claude/worktrees/p247-scratch/critic/` and
your report. No paid run, no `claude -p`, no API call, no credential (`gh auth status` at most, never
`gh auth token`), no GitHub write, no file outside the repository's root, one heavy command at a time
(a round gate and two lanes are building on this Mac).

Write your report to `docs/panel/247-reports/critic-briefs.md` in the p247 worktree as you go, and
end it with a numbered list: each item the fact, the command, what it printed, and the repair you
propose to the brief.
