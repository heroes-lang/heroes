# Panel 249, the completeness critic's first pass, over the briefs

Read every file in `docs/panel/249-briefs/` of the worktree `.claude/worktrees/p249` (at
`722ae76bf693bebdb7a5cca37052ed92646753c0`), whole. **Before any seat starts**, name every framing fact a
brief states that you cannot verify with a command you run (a line number, a path, a count, a hash, a
measurement, a claim about what a document, a header, a library, the C library, the spec or the compiler
says or does not say), each with the command you ran and what it printed; **every route nobody listed**
that a reader of defect 2522 and its two comments (`gh issue view 2522 --repo heroes-lang/heroes
--comments`), of panel 247's R3 and R4 (`docs/panel/247-*.md:191-211`), of panel 248's R1
(`docs/panel/248-*.md:177-226`) and of 248's critic's item 20 (`docs/panel/248-reports/critic-reports.md:230-246`,
`:341-345`) would ask about; and **every question the sitting should ask and does not**. You give no
verdict on the routes.

Things the briefs' author flagged as carried, read rather than run, sampled, or unsettled, which you
settle or hand to a seat with the command that would settle them:

1. **The lane.** The sitting is the soundness lane plus the historian, no spec-warden and no blind seat
   (`00-shared.md` § The lane), with an escape that sends a route needing a spec sentence, a new or
   widened diagnostic, new refusal words or a change a binding's author must learn to the full panel at
   the synthesis. Panel 248 was escalated before any seat ran on its critic's item 23
   (`248-reports/critic-briefs.md:603-606`). Say whether question 3 (the class refused or told, and the
   words), D-imports (a new diagnostic by its own description) and `ffi_defined_elsewhere`'s widening under
   A or B' on Linux send this sitting to the full panel **now**, by `SKILL.md:29-34`, or whether the
   escape at the synthesis is enough; and what the full panel's two seats would be given if so.
2. **The census's frame**: the four points (the stack, the mask, the signal numbers, the frames) are the
   coordinator's naming of item 20's question; the route table in § Question 1 is marked *unrun as a
   table*. Say whether a point is missing (what a library's own C observes that none of the four names:
   `errno` across a handler, `SA_RESTART`'s effect on a system call, the order handlers chain in, a signal
   delivered to which thread) and whether a cell asserts more than its citation.
3. **The libraries' list**: the tree's enumeration (`git ls-files '*.hero' | xargs grep -h -o -E '\blink
   "[^"]+"'`, and `pkgconfig`) and the coordinator's charge (bdwgc, Go's `c-archive`, ART, the users of
   SIGURG and of the real-time signals, libuv, OpenSSL, SDL, sqlite). Say what a reader would add, from
   the world and not from memory, and how you asked.
4. **The CI's three reds and candidate C's premise**: read from job 132377462166's log
   (`briefs/ci/job-132377462166.log`, lines 4112 and 4315-4350); the mechanism read by `objdump` of
   **one** case's handler only (`briefs/fp/run.sh`); the emulated image's clang 22.1.8 against the CI's
   21.1.8 (`briefs/ci/job-132377462166.log:1329`); the reproduction of the three cases in the emulated
   image unrun and handed to the compiler-engineer. Disassemble the other two cases' handlers
   (`fixedbugs-2494-a-heroes-atexit-function-...` and `-reaches-through-fork-...`, their `.h`) the same way
   and say whether the same shape holds; and read the CI run at this base (43640139573, its Linux x86-64
   job 132585763737) if it has ended.
5. **R3's two cases at this base**: on Darwin three runs and three storms only (`briefs/r2522/`), where
   R3 asks 20 of 20; in Linux arm64 the coordinator's container (`briefs/linux/census/census.log`). Say
   whether the counts are what the logs print and whether a seat must re-run them at 20.
6. **The facts read from sources**: Go 1.27.3's (the Mac's toolchain) while the Linux image would install
   Go 1.24 (`golang-go 2:1.24~2`): whether the c-archive clause at `runtime/signal_unix.go:178-182` stands
   at 1.24 is unread; bdwgc 8.2.8's tarball while Debian's build flags are unread; Darwin's `sa_tramp`
   field read from the SDK's header and no further.
7. **The census seed**: `nm -u` of the Mac's libraries and `nm -D --undefined-only` of the image's
   (`census.log`): an import is not a behaviour, and an import list misses a raw system call and a call
   through `dlsym`. Re-run one of each.
8. **The routes found unlisted** (`00-shared.md` § Question 2, D, and the searches it names): whether
   D-tramp is a restatement of B' or of 248's I-dylib, whether D-imports is 247's route A under another
   name (247 vetoed a name list, `docs/panel/247-*.md:293-294`), whether syscall user dispatch belongs in
   the sitting at all; and whether a route is still missing (a mark kept by the C library itself, a
   kernel's answer the thunk could ask on Linux that 248 did not try, a refusal at the first install by a
   library rather than at the callback).
9. **The bounds**: that no brief names a paid run or asks a seat for a run its rules forbid; that every
   program that may not end is bounded in time and bytes, the emulated container and a Go archive's
   program included; that the Go environment keeps every write inside the root (re-run the coordinator's
   `briefs/go/` check if you doubt it); that `apt-get` is asked only inside a `--rm` container; that the
   Windows box is written as off and owed to the CI's leg; and that each seat's copy and compiler are its
   own.
10. **Found while writing**: the runtime's remedy (`sigguard.c:87-91`) and spec line 404 differ since
    `cd693eec1`; say whether the briefs state it correctly and whether it bears on question 3.

Build a compiler at the base in `.claude/worktrees/p249-scratch/critic/` from the seed if a check needs
one (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, in a `git -C .claude/worktrees/p249
archive 722ae76bf | tar -x` copy). Work read-only in the repository and in
`.claude/worktrees/p249-scratch/briefs/`; write only under `.claude/worktrees/p249-scratch/critic/` and
your report. Every program that may not end under `timeout -s KILL`, its output through `head -c 65536`;
Docker containers named `critic-249-<n>`, `docker ps` read after each. No paid run, no `claude -p`, no
API call, no credential (`gh auth status` at most, never `gh auth token`), no GitHub write, no file
outside the repository's root.

Write your report to `docs/panel/249-reports/critic-briefs.md` in the p249 worktree (make the folder if
it is absent) as you go, and end it with a numbered list: each item the fact, the command, what it
printed, and the repair you propose to the brief.

**Your second pass**, after the seats, is over their reports and these briefs as repaired:
`docs/panel/249-reports/critic-reports.md`, under the same rules: a route nobody listed, a claim asserted
and not measured with the command that settles it, a contradiction between seats with which side is
checkable, a census row that rests on an import or on memory, and the question the sitting should have
asked and did not.
