# Panel 248, the completeness critic's first pass, over the briefs

Read every file in `docs/panel/248-briefs/` of the worktree `.claude/worktrees/p248` (at
`df7612ad3dc62dcc6d90bccc650adf6b9736078f`), whole. Before any seat starts, name every framing fact
a brief states that you cannot verify with a command you run (a line number, a path, a count, a
hash, a measurement, a claim about what a document, a header, the C library, the spec or the
compiler says or does not say), each with the command you ran and what it printed; every route
nobody listed that a reader of defects 2522, 2502 and 2490 (`gh issue view <n> --repo
heroes-lang/heroes`), of panel 247's R3 and R4 and of panel 246's R4 would ask about; and every
question the sitting should ask and does not.

Things the briefs' author flagged as carried, read rather than run, remembered or unsettled, which
you settle or hand to a seat with the command that would settle them:

1. **Q1's storms** (defect 2522): 3 of 3 `exit -9` on this Mac with SIGKILL reports in the locale
   lock, 3 of 3 exit 0 in Linux arm64 through about two million signals, run with
   `.claude/worktrees/p248-scratch/briefs/tools/storm.py` and `r2522/`. Read the driver and the logs
   and say whose SIGKILL it is and whether the counts are what the logs print.
2. **The installers' lists**: Darwin's (the SDK's `.tbd` stubs and header greps, five that take a
   handler plus `__sigaction`), glibc's (`nm -D`, fifteen names, nine taking a handler), the
   runtime's own 27 installer call lines under `runtime/parts` and `runtime/platform`, and the
   address sanitizer's own `signal`/`sigaction` (dyld substitutions on Darwin, weak symbols on
   Linux). Re-run each and say whether a name that installs a handler is missing (`sigvec`,
   `sigset`, `bsd_signal`, `sysv_signal`, `ssignal`, `__sysv_signal`, `pthread_sigmask` is not one).
3. **Q1's routes**: R4's corrected interposition, and four the briefs' author added, all unbuilt: W
   (`--wrap` at link time), U (a climb by unwind tables instead of frame records), M (the thread's
   signal mask compared), K (the residue kept, which panel 247's R3 forbids). Say whether each is a
   route or a restatement, whether `--wrap` exists on Apple's linker and on lld-link at all (run
   `ld -help`/`ld64.lld --help`/`lld-link /?` where present, or say it cannot be run here), and
   whether a route is missing (for instance a handler the runtime installs for every signal a
   program's C may catch, or refusing the thunk's address to an installer by its section).
4. **Q2's measurement** (defect 2502): 100 waiters 273.0 to 274.8 M instructions and 10,338 to
   10,472 involuntary switches against 21.7 to 22.0 M and 222 to 236 for 100 sleepers, with the
   lane's `waiters.c` over the base's layer; and the descriptor limits (`launchctl limit maxfiles`
   256, `ulimit -n` 1048576, `kern.maxfilesperproc` 92160). Re-run one of each and say whether a
   wake descriptor per waiting socket fits under the limit a program meets by default.
5. **Q3** (defect 2490): the three `directory` cases and the one fetch case, nine of whose eleven
   command lines start with `cd`; whether `chdir` is on POSIX's async-signal-safe list (IEEE Std
   1003.1-2024, XSH 2.4.3: read it and quote it, or say it cannot be read); and what
   `CreateProcessW` does with a relative or missing `lpCurrentDirectory` (Microsoft's page: read and
   quote it, or say so).
6. **Q4 and Q5**: the thirteen doors of `runtime/hero_net.h`; that `<core>/net` binds no
   `hero_net_stop_asked`; that no timeout or deadline door exists in `net`, `net/http` or the header;
   `HERO_SPAWN_SLOTS` 256 at `runtime/parts/spawn.c:105`; and `examples/server/main.hero`'s `served`.
   Say whether a server holding 256 quiet connections stops accepting, as a question a seat settles
   with a bounded run (the binary under `timeout`, its output through `head -c`, `pgrep` after), never
   by reading alone.
7. **The lane**: whether Q4's and Q5's faces in `<core>/net` and `net/http`, a package's new
   functions, stay in the soundness lane or need the full panel, by `.claude/skills/panel/SKILL.md`'s
   rule and spec § 13's sentence naming the runtime's doors (hg3).
8. **The briefs' bounds**: that no brief names a paid run or asks a seat for a run its rules forbid;
   that every program that may not end is bounded; that the Windows box is written as off and owed to
   the CI's leg; and that each seat's copy and compiler are its own.

Build a compiler at the base in `.claude/worktrees/p248-scratch/critic/` from the seed if a check
needs one (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`). Work read-only in the
repository and in `.claude/worktrees/p248-scratch/briefs/`; write only under
`.claude/worktrees/p248-scratch/critic/` and your report. No paid run, no `claude -p`, no API call, no
credential (`gh auth status` at most, never `gh auth token`), no GitHub write, no file outside the
repository's root.

Write your report to `docs/panel/248-reports/critic-briefs.md` in the p248 worktree as you go, and
end it with a numbered list: each item the fact, the command, what it printed, and the repair you
propose to the brief.
