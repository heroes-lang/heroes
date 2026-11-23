# Panel 257, ffi-pragmatist (Darwin, and real C)

Read `00-shared.md` first, whole, then `docs/panel/257-reports/critic-briefs.md` (its repairs bear on you).
**Your agent file is the frozen commit's** (`.claude/agents/ffi-pragmatist.md` in your copy); its veto is on ABI
breakage and on *any C library must be bindable* (design.md §1.12, §4.19, by grep), and yields to the most
robust resolution. Your copy is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/ffi-pragmatist/`. Your report:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p257/docs/panel/257-reports/ffi-pragmatist.md`,
written as you go.

## Your job: the truth of the sentence against real C, and against Darwin's build

You hold **this Mac** (Darwin arm64, Apple clang 21.0.0): today's runtime and the **adopted Darwin build**,
C4r (`rebind.c` and `sigguard.c`, § 4 of the shared brief, over `runtime/parts/`). The compiler-engineer holds
Linux. Build as the model in § 4: your stage 2, `HEROES_RUNTIME=<copy>`, plain and `--sanitize`, `timeout 20`,
both streams through `head -c 100000` (`2>&1 | head -c 100000 > o.txt`, the exit from `PIPESTATUS`), three runs a cell. **Copy each program you ran, with its script, into**
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p257/docs/panel/257-reports/programs/ffi-pragmatist/`
(headers, `.hero`, `run.sh`, never a binary), so the critic can run it on Linux.

1. **The table of § 6**, C1 to C9, on Darwin, **today** and **C4r**, the first line of stderr of each cell. On
   Darwin `rr_*`, `nn` and the A″ forms are today's residues (panel 255); say which row of yours they are.
2. **The ten programs** `p1` to `p10` (`257-briefs/blind/programs/`, `answers.md`; p8, p9 and p10 are the critic's) on
   Darwin, today and C4r, plain, `-O0`, `-O2` and `--sanitize`. **p7 (G)**: today RUNS on this Mac and C4r
   refuses it (the critic's cells, `critic-briefs.md`, a datum to **re-measure, not copy**): confirm.
3. **Real C, the binding the sentence implies.** For each of these write the C header and the Heroes binding, build
   and run it, and say what a reader who followed the sentence would expect and what happens: (a) **libpng
   1.6.59's error path** (`png_error` is a `longjmp` that is **not** inside a signal handler: a control, the
   2679 case; `pngdemo` and `good.png` in the archive, `ffi-pragmatist/p255/r2/sh/`); (b) **a `SIGSEGV` or
   `SIGBUS` recovery handler** that `siglongjmp`s out (the golden `fixedbugs-a-c-handler-that-recovers-leaves-no-line`
   in `tests/golden/run`, read it); (c) **a watchdog** with the three handlers of the blind reading's t2 (A
   `siglongjmp`, B `swapcontext`, C a flag), each a real C library **driven by a timer signal** (`setitimer` or
   `alarm`, a long computation interrupted, not `raise`) and a Heroes program binding it, **kept in
   `257-reports/programs/ffi-pragmatist/` as `watchdog-a`, `-b`, `-c`**: they are the key of t2, which has none
   (`answers.md`), so run them on this Mac today and under C4r and say what each prints; (d) **a
   preemptive green-thread scheduler** (`SIGALRM`, the handler `swapcontext`s to the next thread), the real-world G.
4. **"Though C is correct."** Several arms end *"though C is correct"*. For **each program the arms say is
   refused**, say **what C and POSIX say of it**: defined, implementation-defined, or undefined, and where. You
   have `man` pages on this Mac (`man -P cat sigsetjmp`, `swapcontext`, `sigaction`, `longjmp`); **a clause of a
   standard you did not read is written *recalled*, never quoted**. A program the standard leaves undefined
   (a jump back into a handler already left, `rr_*`; a `swapcontext` in a handler, which POSIX marks not
   async-signal-safe, if the man page says so) is one the sentence must not call correct. **Write the clause the
   arms should say instead.**
5. **The inline jump**: clang refuses `__builtin_longjmp` on arm64 (§ 3). Confirm on this Mac (the error text),
   and say what a reader who reads *"an inline `__builtin_longjmp`"* in the spec writes on arm64. Give the
   class's wording that is true on both (the golden's own: *"a jump no function of the C library makes"*; your
   own).
6. **The words.** For every refusal on Darwin, today and C4r: the first line of stderr. Does *"saying the handler
   was not seen to end"* describe it? A *"holds no record of"* under C4r is a finding against R2's promise.
7. **C8, `fp3` and `as_0`** (C4r refuses both in S words, today runs both): what real C program is that class,
   in one line, in words a reader of the spec could understand. If the class cannot be said in a spec sentence,
   say that, and what it costs to leave it out.
8. **Bindability** (§1.12): is there a C library a Heroes program binds today that **stops working** under C4r
   because of a refusal the sentence names? Name it, or say what you searched (`find /usr/lib /opt/homebrew -name '*.dylib'`
   is not a search: say the libraries you tested).
9. **Each arm's clauses** (`arms/arm-f*.diff`, the second issue; the first, `arms/v1/`, the critic measured): mark each **true on Darwin** (cell, program), **false** (the falsifying program) or
   **untested**; a clause you find false, write the repaired clause in `257-reports/ffi-clauses.md` and run it.
10. **C12** (`exit()` inside a handler reaching an `atexit` function): the 2494 golden's words today and under C4r.
11. **Unrun**: the CI's `macos-26`, Darwin x86-64 and Windows (no probe in the runtime, shared brief § 4); say which
    rows rest on any.

End with the verdict structure of your agent file, then one line per numbered question: the answer, and the one
measurement that would change it. **A route that does not build is not adopted**: say first whether the adopted Darwin C4r build built and what `./heroes-next run fixedbugs-2522` read on it. **Its pass is 23 passed, 2 failed**, exactly `fixedbugs-2522-a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused` and `fixedbugs-2522-a-correct-jump-out-of-a-handler-through-a-copied-buffer-is-refused-in-words-that-say-so`, the two goldens R2 moves (panel 255's own gates, `compiler-engineer-r2.md:382` and `ffi-pragmatist-r2.md:521` and `686-687`, the critic's B10); 25 and 0 would be a build that did not move them. **The compiler's own test `byte for byte (defect 2533)` is already RED at the frozen tree** (shared brief § 1): do not read it as yours. Or say that you did not run it, and why.
