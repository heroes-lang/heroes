# Panel 257, the resumed seats (compiler-engineer and ffi-pragmatist)

Written 2026-11-23 at 14:35 (`date`), by the sitting's coordinator, after the API's five-hour limit stopped both
seats mid-report (their reports end where they stopped, below). **A seat is relaunched from ITS OWN report file**,
never from the other seat's findings: read your brief (`compiler-engineer.md` or `ffi-pragmatist.md`), `00-shared.md`,
your own report in `257-reports/`, and this file; **do not read the other seat's report** (it keeps your two
tables independent, and the critic's second reading is what crosses them). Everything in your brief stands unless
this file amends it. Your copy and your compilers are where you left them (`build/` may be gone: rebuild from the seed).

## What is new since the briefs (each by its command, run at 14:3x)

1. **The runtime lane has landed**: `rebel-round` read `b9129d41b` and contains `523d35537` *"merge lane-p255-impl,
   panel 255's route C on both platforms (step 116a, defect 2679)"* (`git log --oneline -8 rebel-round`). The spec
   is untouched at line 428 (`git show <rev>:spec/heroes-spec.md | sed -n 428p | md5` equal at `828567a30` and
   `rebel-round`: `8703e262...`; the two lines of the spec that moved are panel 256's mailbox `number`).
   **Sources of truth for the landed build**: `runtime/parts/sigjump.c`, `sigjump_decode.c`, `sigjump_probe.c`,
   `sigjump_record.c`, `sigjump_save.c`, `rebind.c` with `rebind_entries.c`, `rebind_image.c`, `rebind_install.c`,
   `rebind_jump.c`, `rebind_record.c`, `sigguard.c`, and `tests/golden/run/fixedbugs-2522-*.expected`,
   `fixedbugs-2679-*` (23 files of 2679 and 2702, `ls | grep -c`). **A copy of that tree is yours**:
   `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/<seat>-landed/` (a detached worktree at
   `523d35537`, made by the coordinator, clean): build its compiler there from its seed (`clang -I runtime seed/heroes.c
   runtime/runtime.c -o heroes`; `git diff --stat 828567a30 rebel-round -- seed` printed nothing, so the seed is the same
   file) and `rm -rf build` after any copy. **Panel 255's two routes are no longer the build; this is.**
2. **The words.** The lane's final words of the NEW refusal (`HERO_SIGNAL_SWITCHED`, `sigguard.c:440`; Darwin's G forms
   `g1_0`, `g1_1`, `g2`, `g3`), **as the coordinator was told them (re-read them in the source and the `.expected`,
   never copy these)**: *"was called while a signal handler of C's that this runtime saw begin had switched to another
   context (`swapcontext`, or `setcontext` into a context `makecontext` made) and had not been seen to switch back or
   to end: either C called it on that other context, or C abandoned the handler by a switch this runtime cannot tell
   from one that comes back. A handler not seen to end is held to be running, ..."*. The EXISTING words are unchanged
   (20 `.expected` carry them): Linux `g_3_0` and Darwin `fp3` and `as_0` keep *"had not been seen to end"* with its
   second alternative *"C left the handler in a way this runtime does not follow"*. **The lane did NOT argue that those
   words are true of Darwin `fp3` and `as_0`: read whether they are** (what does a reader who is told them conclude,
   and is it so?).
3. **Defect 2702 (R3)** is not built; a lane for it is opening. **The sentence must not promise its refusals as final**:
   every clause that rests on the context rows is *"where the landed runtime refuses it, until 2702 says otherwise"*,
   and the report says which clause that is.
4. **The spec-warden has reported.** You may read **its text files only**, `257-reports/spec-w7.txt` (the text it
   adopts, *w26*) and `spec-w12.txt` (its fallback *w27*, true on both builds by its reading), not its report:
   w26 is *"A handler of C's ends by its return or by a `longjmp`, `siglongjmp`, `_longjmp` or `setcontext` out of it;
   left any other way (by an inline jump, a `swapcontext` away from it or a jump onto another stack given to
   `makecontext`) it is still taken to be running, and a callback called later on its thread from deeper in the stack
   than the handler was may abort, saying the handler was not seen to end."* **From now on the clauses you test are
   w26's and w27's** (item 8 of the engineer's brief and item 9 of the ffi-pragmatist's), beside the first issue's.
   The warden's reading, which you check and do not take: the abort after an unseen exit is **a stack-depth test**
   (the engineer measured it on Linux: a callback from the raise site runs, one about 5 KiB deeper aborts), so *"aborts"*
   became *"may abort"*; `setcontext` out of a handler (`b_0`, `b_1`) **runs** on every build and is therefore on the
   list of exits the runtime follows.

## What each seat still owes (your report says what is done; do these, in this order)

**Both**: (a) the **landing check** below; (b) your **verdict** in your agent file's structure and **one line per
numbered question of your brief** (the answer, and the one measurement that would change it); (c) **copy every program
you ran, with its script, into `257-reports/programs/<seat>/`** (the engineer's folder is EMPTY: her programs are in her
work folder; the critic runs them from there on the other platform); (d) `docker ps -a | grep p257` empty at the end.

**compiler-engineer**: your brief's item 8 (each clause of w26, w27 and the first issue's arms, true / false /
untested, with the cell), item 9 (`ubuntu-bins.sh` with `VARIANT=C2679`, the accessor's reading beside every row),
item 10 (C12), item 11 (unrun), then the landing check. Your report's sections 0 to 7, 5b, 4b and 9a are done.

**ffi-pragmatist**: **first, the Darwin depth sweep the warden's condition names**: the inline jump (`p4`), the G forms
(`g1_0`, `g1_1`, `g2`, `g3`, `p7`), `fp3` and `as_0`, each with the callback called at **0, 1024, 4096, 6144 and 16384
bytes deeper than the handler** (the engineer's `p4d` has a `PAD` parameter on Linux; write yours from the shapes
you have), under C4r and today: **does any refusal hold at every depth, or is it a stack-depth test on Darwin too?**
Then items 5 (the inline jump's spelling), 6 (the words; with the lane's new ones), 7 (`fp3` and `as_0`: what class is
that, and are the existing words true of it), 8 (bindability), 9 (each clause), 10 (C12), 11 (unrun), then the
landing check. Your report's sections 0 to 4 are done.

## The landing check (new, both seats)

In `<seat>-landed` (item 1): re-run **the decisive rows against the landed runtime, on your platform, plain and
`--sanitize`, three runs a cell**: `p1` to `p10`; `gcopy`; the C4 inline jump and the C5 program; the C7 G forms
(Darwin: `g1_0`, `g1_1`, `g2`, `g3`; Linux: `g_3_0`, `p7`), `b_0` and `b_1`; **the depth sweep** (the engineer's `p4d`;
the ffi-pragmatist's own); C12; and the accessor that says which route decides a jump (the lane's golden
`fixedbugs-2679-the-runtime-says-which-route-decides-a-jump`). Put the result in a section `## Against the landed
runtime (523d35537)` with the table **beside your panel-255-routes cells**: a row that differs is a finding. **Say, for
each clause of w26 and w27, whether it is true of the landed runtime on your platform.** The first line of stderr
of every refusal, verbatim.
