# Panel 249, the completeness critic's second pass, over the reports

*Written by the coordinator on 2026-11-15 at about 02:40, after the three seats had finished and before
this pass ran; no second-pass brief was written with the others, and this one is the omission repaired.*

Read every file in `docs/panel/249-briefs/` and `docs/panel/249-reports/` of the worktree
`.claude/worktrees/p249`, whole (the sitting's base is `722ae76bf693bebdb7a5cca37052ed92646753c0`, stage 2
`sources 2ab7ca0a644692a1`): the three seats' reports (compiler-engineer, ffi-pragmatist, historian) and
your own first pass (`critic-briefs.md`). You give no verdict. Name what is MISSING.

**The author's answer that frames the synthesis** (2026-11-15, about 02:03, on the coordinator's
question put the evening before with its recommendation, *c*, confirmed in these words: *I confirm your
recommendations*): the sitting is split. The synthesis rules **question 1 (the census)** and **question 2
(the candidates)** now; **question 3** (which class is refused or told, and in which words) and the
blind seat run **only if no candidate holds** on a platform. So say, platform by platform (Darwin, Linux
arm64, Linux x86-64, and Windows, which has no POSIX signal), whether a candidate holds by the reports'
own measurements, which, and on what cases.

1. **A route nobody listed, or listed and nobody built**, and for each route a seat recommends: was it
   built in that seat's copy and run on the cases it named (`SKILL.md` 3c)? The reports name D-rebind
   (the ffi-pragmatist's, unlisted in the briefs), D-tramp, A with plain `SIGRTMAX`, A with glibc's
   `__libc_allocate_rtsig(0)`, A with SIGEMT on Darwin, B', B'' (the compiler-engineer's variant, the
   depth confirmed by frame records), C (`-fno-omit-frame-pointer`), I-onstack, I-mask, D-imports and
   route K (the author's change to R3). A recommendation over an unbuilt route is named as such.
2. **The contradiction the synthesis cannot adopt without settling**: the compiler-engineer holds that
   D-rebind, which resets its depth to zero at every jump, should miss a handler that jumps *within
   itself* and then calls a Heroes function back, refused 3 of 3 by B' and B''; its test program is
   `injump/` in `.claude/worktrees/p249-scratch/249-compiler-engineer/`, unrun against D-rebind because
   D-rebind's build stands in the ffi-pragmatist's folder (`c/rebind/rebind.c` there). **Run it**: copy
   `injump/` and the ffi-pragmatist's rebind sources into a tree of your own under
   `.claude/worktrees/p249-scratch/critic/`, build both as the ffi-pragmatist's report says D-rebind is
   built, run it on Darwin as many times as the reports ran theirs, and say what it prints. Beside it the
   same trade on Linux: the ffi-pragmatist approves B' only if `setcontext`, `swapcontext` and the jumps
   reset its depth, and the compiler-engineer's correct programs that call back after a handler left by
   a jump are what that reset answers; does the reset open the in-handler-jump miss there too?
3. **A claim asserted and not measured with the command that settles it**: above all, for each
   candidate a seat approves, R3's two cases refused and the census's libraries' correct programs
   passing, on each platform the seat names; the ffi-pragmatist's prediction that Debian's SDL3 takes
   signal 36, never 64 (the historian reads SDL's source as taking the first number from `SIGRTMIN + 2`
   whose action is the default, so a marker that is only a blocked bit could look free to it); the
   compiler-engineer's C measurements (0.6 to 0.7 % of code at `-O2` on x86-64, and the `run` suite whole
   unrun); B''s `runtime` suite reading one floor outgrown (137 against 109).
4. **The escape**: whether a candidate the synthesis would adopt needs a spec sentence, a new or widened
   diagnostic, new refusal words or a change a binding's author must learn, which `00-shared.md` § The
   lane sends to the full panel at the synthesis; the ffi-pragmatist says D-rebind needs none of the four
   and A and B' each need a widened diagnostic. Check that reading against what each route's refusal
   prints.
5. **Route K**: both building seats say no route closes defect 2522 under R3 as written, every one
   leaving a library that installs past the C library's functions (a raw `rt_sigaction`, a `dlsym`
   lookup, an unblocking handler). State R3's sentence as it stands (`docs/panel/247-*.md`), the change
   the seats would ask the author for, in a sentence the author can answer, and the residue each
   candidate leaves under it; the synthesis puts that change to the author as a decision, with a
   recommendation.
6. **Defects found beside the question**: the runtime's null arm killing a Go archive's recovered nil
   dereference (both building seats) is filed as defect 2558; name any other the reports hold that is not
   yet filed, with its reproducer's path.
7. **The question the sitting should have asked and did not**, with who could run it and how.

Build a compiler at the base in `.claude/worktrees/p249-scratch/critic/` from the seed if a check needs one
(a `git archive` of `722ae76bf`); to rebuild a seat's route, copy its files into a tree of your own,
never build or run inside a seat's folder. Work read-only everywhere else; write only under
`.claude/worktrees/p249-scratch/critic/` and your report. No paid run, no `claude -p`, no API call, no
`heroes measure --refresh`, no credential (`gh auth status` at most, never `gh auth token`), no GitHub
write, no file outside the repository's root (a redirection to `/tmp` is one). A suite's or the harness's
output goes whole into a file and is read from it, never through `head` or `tail`. A program that may not
end runs as a binary under `timeout -s KILL`, its output through `head -c 65536`; a Docker run with
`--rm`, `docker ps` read after it. A Go command runs with `GOTELEMETRY=off` and `GOFLAGS=-modcacherw`,
its caches under your folder (`GOCACHE`, `GOMODCACHE`, `GOPATH`), since the first pass's `go` wrote three
telemetry files into the home folder, deleted on the author's answer. The Windows box is on tonight but
is not this sitting's: a Windows question is owed to the CI's leg.

Write your report to `docs/panel/249-reports/critic-reports.md` in the p249 worktree as you go, and end it
with a numbered list: each item what is missing or contradicted, the command you ran and what it printed
(or the command that would settle it, unrun), and what the synthesis must not adopt without it.
