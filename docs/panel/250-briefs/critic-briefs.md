# Panel 250, the completeness critic's first pass, over the briefs

Read every file in `docs/panel/250-briefs/` of the worktree `.claude/worktrees/p250` (at
`722ae76bf693bebdb7a5cca37052ed92646753c0`), whole, and the author's paragraph that convened the
sitting, which the base does not hold (`git -C /Users/joseph/Temp/heroes/heroes-lang show
b5323a21c:docs/roadmap/milestones/M-core-packages.md`, its last section). **Before any seat starts**,
name every framing fact a brief states that you cannot verify with a command you run (a line number, a
path, a count, a hash, a measurement, a claim about what a document, the compiler, a verb or a ruling
says or does not say), each with the command you ran and what it printed; every route nobody listed
that a reader of feature 2315, task 2312 and decision 2528 (`gh issue view <n> --repo heroes-lang/heroes
--comments`), of panel 235's R2, R6, R7, R12, R15, R19 and R20, of panel 234's R2 and R13, and of panels
241 and 242 would ask about; and every question the sitting should ask and does not.

Things the briefs' author flagged as carried, read rather than run, or unsettled, which you settle or
hand to a seat with the command that would settle them:

1. **The carried figures of panels 241 and 242**: *"16 functions, 186 lines, in 12 modules"*,
   *"the compiler's tree grows by 2,325 lines net"* (`242-*.md:53-56`), *"368 of the placed tree's
   2,334 lines"* (`241-*.md:68`), and 00-shared.md's arithmetic on them (368 against 186). Say whether
   the arithmetic is the right one for R2's ruler as `docs/panel/242-*.md:140-152` writes it, and what
   at this base (core 0.4.0, not 0.2.0) would settle it.
2. **The closure table** (00-shared.md § The numbers): a textual closure by
   `.claude/worktrees/p250-scratch/briefs/reach/closure.sh` and `programs.sh`, checked against the
   compiler's `#line`s for two programs of seventeen. Re-run the scripts, read them for a reading they
   get wrong (a `use ... as`, a `use` inside a `test` block, a package's own bracketed `use`), and say
   whether the 78 and 17,753 stand.
3. **"No blind seat"**: the claim that the question changes no line a program writes, with its two
   exceptions named (F2, the split). Say whether another route listed changes a `use` line, a catalogue,
   a message a reader acts on or a spec sentence, and whether the escape as written would reach the
   author in time.
4. **The reading of panel 235's R2** (*"Go's modules copied whole"*, read by 00-shared.md as Go's module
   rules taken as the base and not a placement rule): read `235-*.md:1-30` and `545-566` and say whether
   that reading is the record's or the coordinator's.
5. **The negative sentences**: *"No file of 0.4.0 is C"*; *"the spec names neither `core` nor the
   placement folder"*; *"`heroes fetch` takes no operand"* (`selfhost/cli/table.hero:195`); *"the
   compiler carries no catalogue at the base"*; and the one left as a question, whether a fetched
   package's `link "x"` can find an archive the package carries (`selfhost/cli/package_units.hero:298`
   read, no more). Run each, or say what would.
6. **The 320 placed files the author was given**, read by 00-shared.md as the `.hero` count of 416
   placed files: re-run `git ls-files examples | grep '/heroes.packages/'` and its two filters.
7. **The measured refusals**: `check` exit 1 `package_unread` and `fetch` exit 1 `sum_mismatch` on a
   pruned copy of `examples/ini`, and `fetch` exit 0 placing 26 files into an empty one
   (`p250-scratch/briefs/partial/examples/*.txt`). Re-run one, with a compiler you build yourself.
8. **The routes**: W, M, G, H for the unit; F1 to F4 for the sum; R2's ruler moved, `core` split, trees
   not committed, and the author's *b* and *c*. Say whether each is a route or a restatement, and
   whether one is missing (for instance: the build compiling only the reached files of a whole tree and
   the ruler reading that; a listing kept in the catalogue rather than placed; a placement keyed by the
   reached set; a package that declares its own grain to the index).
9. **The interface's order**: 2312's goldens in lane b37-pm tonight (10 untracked case folders at 22:30)
   against this sitting's ruling, and the 55 golden case folders that hold a placed tree by hand: say
   whether the briefs hand any seat a premise about which golden moves that no seat is asked to run.
10. **The bounds**: that no brief names a paid run or asks a seat for a run its rules forbid; that every
   program that may not end is bounded with its time and its bytes; that the Windows box is written as
   off and owed to the CI's leg; that each seat's copy and compiler are its own; and that the
   spec-warden's counts are said to be vendored and a lower bound.

Build a compiler at the base in `.claude/worktrees/p250-scratch/critic/` from the seed if a check needs
one (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then stage 2; `sources
2ab7ca0a644692a1`). Work read-only in the repository, in `.claude/worktrees/p250-scratch/briefs/` and in
every lane's worktree; write only under `.claude/worktrees/p250-scratch/critic/` and your report. No paid
run, no `claude -p`, no API call, no `heroes measure --refresh`, no credential (`gh auth status` at most,
never `gh auth token`), no GitHub write, no file outside the repository's root. A program that may not
end runs as a binary under `timeout -s KILL`, its output through `head -c 65536`.

Write your report to `docs/panel/250-reports/critic-briefs.md` in the p250 worktree as you go, and end
it with a numbered list: each item the fact, the command, what it printed, and the repair you propose to
the brief.
