# Panel 257, compiler-engineer (Linux arm64)

Read `00-shared.md` first, whole, then `docs/panel/257-reports/critic-briefs.md` (its repairs bear on you).
**Your agent file is the frozen commit's** (`.claude/agents/compiler-engineer.md` in your copy); its veto is on
soundness and yields to the most robust resolution. Your copy is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/compiler-engineer/`. Your report:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p257/docs/panel/257-reports/compiler-engineer.md`,
written as you go.

## Your job: the truth of the sentence against the Linux build, and the words it prints

The sentence is the spec's answer to *"which correct C programs does a jump out of a handler refuse, and what
is the program told"* and must be true on Linux and on Darwin. **You hold Linux**: the container
`heroes-linux-arm64:latest` (Debian clang 22.1.8, glibc 2.41, aarch64), today's runtime and the **adopted
Linux build** (`routes/C2679/sigjump.c`, § 4 of the shared brief, applied over `runtime/parts/` of a copy). The
ffi-pragmatist holds Darwin. Panel 255's Linux scripts (`linux.sh`, `r2/leg2.sh`) are your models: a Linux
compiler is built **inside the container** from the seed and stage 2 (a Mac binary does not run there), a
program built with `HEROES_RUNTIME=<the runtime copy in the container's view>`, each run under `timeout 20`,
both streams through `head -c 100000` (`2>&1 | head -c 100000 > o.txt`, the exit from `PIPESTATUS`), three runs a cell, plain and `--sanitize`. Mount your copy into the container
(`docker run --rm -v <copy>:/w ...`); **name every container `p257-ce-<n>` and `docker rm -f` it when done**.

1. **The table of § 6**, C1 to C9, on Linux: **today** and **the adopted build**, each cell *runs / S / U / K /
   D / other* with the **first line of stderr** copied, the program and the command beside it. C10 is the
   critic's to read across your table and the ffi-pragmatist's, so **keep your programs where it can run them
   on the other platform**: copy each program you ran, with the script that built and ran it, into
   `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p257/docs/panel/257-reports/programs/compiler-engineer/`
   (headers, `.hero` files, `run.sh`), never a binary.
2. **The ten programs** `p1` to `p10` (`257-briefs/blind/programs/`, `answers.md`; p1 to p7 are the registered
   reading's, **p8 (a jump that lands within the handler), p9 (the first of 40 live saves) and p10 (the handler
   refills a buffer and leaves by it) are the critic's**) on Linux, today and adopted, plain, `-O0` and `-O2`, and
   `--sanitize`. **The critic's own cells** (`critic-briefs.md`, *the registered key, run on both platforms*:
   Linux C2679 RUNS p1, p2, p3, p9, p10, **p7**, ABORTS (S) p4, p6, p8) are a datum to **re-measure, not to copy**; a
   cell of yours that differs from it is a finding.
3. **C5, the C++ exception**: none exists. Write the smallest program in which a C++ exception is thrown inside
   a signal handler's dynamic extent and caught outside it (`clang++` is in the container; its `g++` is clang and
   there is no GCC), say
   whether it is **correct C++** (what flag or rule makes it so: `-fnon-call-exceptions`, a synchronous
   `raise`, only what you can run), and run it. If you cannot make one that is correct, say so: then *"a C++
   exception"* in the words is a claim nobody can test.
4. **What *"a callback called while ..."* means (C11, and the first issue's *"the next callback"*).** (a) After an inline jump, a program that **never calls another
   callback**: does it exit 0? (b) A callback called **on another thread** while the first thread's handler is
   held running (`HERO_SIG_JUMPS` and the entries are per thread, `runtime/parts/sigguard.c`): does it run? (c)
   A callback called **from a second signal handler** after the first was left by a jump. Each is a shape that
   tests whether *"the next callback aborts"* is true, or only *"the next callback of that thread"*.
5. **C6, the refill** (§ 3, *what a refill is*): does a correct program that really LEFT the handler through a
   buffer the handler refilled get refused under the adopted build, or only the program whose jump lands inside
   it? Build both and run them. **If a program that leaves the handler is refused, name it; if none is, say
   that none was found and what you tried.**
6. **C7, G** (`g_3_0` and the forms of panel 255's `compiler-engineer-r2.md` § 3.4): which G forms does the
   adopted Linux build refuse, which does it run? **Which clause of an arm is false on Linux until defect 2702
   lands** (a context switched away from and never back)?
7. **The words.** For every refusal you ran: the first line of stderr under today's and the adopted build. Does
   *"saying the handler was not seen to end"* describe it? Where a refusal's words say *"holds no record of"*
   (U) under the adopted build, that is a finding against R2's promise.
8. **Each arm's clauses** (`arms/arm-f*.diff` (the second issue; the first, `arms/v1/`, the critic measured)): mark each clause **true on Linux** (cell and program), **false**
   (the program that falsifies it), or **untested** (why). A clause you find false: write the repaired clause in
   `257-reports/ce-clauses.md` and run the repaired claim.
9. **The CI's Linux library** (shared brief § 4): run `C2679` through `compiler-engineer/p255/r2/ubuntu-bins.sh` with
   `VARIANT=C2679` and `ubuntu-run.sh` (arm64, glibc 2.43, `ubuntu:26.04`), and **put the accessor's reading
   (`decoding: N`) beside every row of your table**: *"whatever memory"* holds only where it reads 3.
10. **C12** (`exit()` inside a handler reaching an `atexit` function, line 427's *called by one*): the 2494 golden's
    words under today's and the adopted build.
11. **Unrun, and said so**: Windows (no probe in the runtime, shared brief § 4) and Linux x86-64 (`ubuntu-26.04`) have no image here; do not build one. Say which clause
   or program rests on x86-64 (the golden's `__builtin_setjmp` path compiles only there, § 3).

End with the verdict structure of your agent file, then one line per numbered question: the answer, and the
one measurement that would change it. **A route that does not build is not adopted**: say first whether the adopted Linux build built and what `./heroes-next run fixedbugs-2522` read on it. **Its pass is 23 passed, 2 failed**, exactly `fixedbugs-2522-a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused` and `fixedbugs-2522-a-correct-jump-out-of-a-handler-through-a-copied-buffer-is-refused-in-words-that-say-so`, the two goldens R2 moves (panel 255's own gates, `compiler-engineer-r2.md:382` and `ffi-pragmatist-r2.md:521` and `686-687`, the critic's B10); 25 and 0 would be a build that did not move them. **The compiler's own test `byte for byte (defect 2533)` is already RED at the frozen tree** (shared brief § 1): do not read it as yours. Or say that you did not run it, and why.
