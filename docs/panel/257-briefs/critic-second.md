# Panel 257, the completeness critic: SECOND READING (the reports, before the synthesis)

Written 2026-11-23 at 16:20 (`date`) by the sitting's coordinator. You are a **fresh agent**; read `critic.md`
(its *Second reading* section is your method), `00-shared.md`, `resume.md` (what changed after the briefs: the
landed runtime at `523d35537`, the lane's new words, defect 2702), then **every report**: `257-reports/spec-warden.md`
(its § 9, § 10 and the verdict), `compiler-engineer.md` with `ce-clauses.md`, `ffi-pragmatist.md` with
`ffi-clauses.md`, `spec-w*.txt`, and your own first reading `critic-briefs.md` (a fresh agent reads it as a
colleague's: re-run what you doubt). Your reports: `257-reports/critic-reports.md`, written as you go. Your copies
(both made by the coordinator, clean, with compilers from your first reading's sibling, or rebuild from the seed):
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/critic/` (`828567a30`, the frozen tree plus
panel 255's two routes by `HEROES_RUNTIME`) and `.../p257-scratch/critic-landed/` (`523d35537`, the landed runtime).
**You hold both platforms** (this Mac; the container `heroes-linux-arm64:latest`, Debian clang 22.1.8, glibc 2.41).
The rules of `00-shared.md` § 2 bind you (no paid run, no credential, nothing outward, **no file outside the
repository root: the first reading wrote a log into `/tmp`, do not**, containers named `p257-critic-<n>` and
removed, programs as binaries under `timeout`, **both streams** through `head -c`).

## What the reports say, in short (verify, do not take)

The seats found the first issue's arms (and the warden's first pass) **false in specific places**, and converged
on *"may abort"*: **Linux** (the engineer) refuses an unseen exit **by a stack-depth test** (a callback from the
raise site runs, one about 5 KiB deeper aborts); **Darwin** (the ffi-pragmatist) refuses **at every depth**
(`fp3` at `-O2` excepted); `setcontext` out of a handler onto the thread's own stack runs on both; a one-way
`setcontext` onto a `makecontext` stack (`sc1`) is refused on Darwin; a jump between two stacks the program made is
refused on Linux by mapping order (`cs2` ORDER=1, `c_1`, `cs_1`); `p7` (`swapcontext` away) runs on Linux and
aborts on Darwin; `sched` and a Boost.Context fiber, which switch back, are refused on Darwin; route A (forced) refuses
jumps through heap, TLS and late-loaded buffers; **K words** (*"called it while handling a signal"*) are printed
for `as_1`, not S. **The engineer reports that the landed tree builds no Heroes program in either Linux image**
(`runtime/platform/hero_ui_print_gtk.c:609`, `-Werror=conditional-uninitialized`, a UI file) and ran its landing check
on a copy with one initializer: **check that, and say whether it is so on `rebel-round`'s head** (`git -C
/Users/joseph/Temp/heroes/heroes-lang log --oneline -1 rebel-round`; it is the coordinator's to report).

## The candidates, and what you do with them

The coordinator wrote two candidate texts **after** reading the reports, `spec-x1.txt` and `spec-x2.txt`
(`257-reports/`, each goes before `No function of the program is a signal handler`; their diffs apply:
`patch --dry-run -p1 < spec-xN.diff`), **to be attacked, not adopted**: x1 states what runs and what *may abort*, x2
the refusal side first. Beside them stand the warden's `w26` (`spec-w7.txt`) and `w27` (`spec-w12.txt`), and each
seat's repaired clause (`ce-clauses.md` R-1 to R-8; `ffi-clauses.md` § 3).

1. **Run, on both platforms and both builds (panel 255's routes and the landed runtime), the programs that decide**:
   from the engineer's `programs/compiler-engineer/` on **Darwin** and from the ffi-pragmatist's
   `programs/ffi-pragmatist/` in the **container**: at least `cs2` (both orders), `sc1`, `fp3`, `p4d` at the pads
   0, 1024, 4096, 6144, 16384, `p7`, `b_0`/`b_1`, `as_1`, `sched` (a green-thread scheduler on `swapcontext`), a
   watchdog A/B/C. **Where a program does not run on the other platform, say why** (a Darwin-only header,
   `_XOPEN_SOURCE`). The question is **C10 measured**: for each clause of `x1`, `x2`, `w26`, `w27` is there a
   program that falsifies it on either platform, on either build?
2. **Your own shapes, at least three, beside the candidates** (CL-061), chosen to falsify `x1` and `x2`: for
   instance a handler on an alternate stack that leaves to the thread's own stack **on a thread the program
   started**; a `setcontext` to a context `getcontext` took in a *different function's* frame; a nested handler (a
   second signal while the first runs) left by a jump; a `longjmp` out of a handler into a frame that has since
   been **popped and reused** (undefined in C: say which of your shapes are defined).
3. **The words.** Read the landed `.expected` words and `sigguard.c`'s constants (S, U, K, D, SW). For each clause
   of `x1` and `x2` that says *what the program is told*, is the sentence true of the words **printed** by
   the builds, on both platforms (the first stderr line of each refusal you ran)? The lane's note says it did **not**
   argue that S's words are true of Darwin `fp3` and `as_0`: **is a reader of those words misled, and does the
   sentence need to say so?**
4. **Scope**: Windows (unrun, no probe), route A (forced, simulated by the ffi-pragmatist), x86-64 (unrun). Which
   clause of `x1` or `x2` is false there, and does the candidate say so?
5. **Contradictions between the seats**, which you check (depth: Linux test, Darwin none; `setcontext`; the words),
   and **what the sitting should have asked and did not**.

End with `## What the synthesis must settle`, most important first, and say **which of `x1`, `x2`, `w26`, `w27` you
would not print in the spec, and the program that stops you**. You give no verdict on an arm; you name the
program. **No paid run is yours to start; no credential is yours to print.** Write as you go; finish within a
bounded effort (the first reading took 53 minutes; aim for no more).
