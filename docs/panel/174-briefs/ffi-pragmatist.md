# Panel 174 — brief: ffi-pragmatist

Read `00-shared.md` in this directory first. It carries the measurement and the
five routes. This brief carries only what is yours.

## Your seat

design.md §1.11 and §4.19: there is no standard library, everything comes from
C, and this is where the language's guarantees stop. **You write the C and you
compile it**, or you say why you cannot. You have a veto on ABI breakage.

## The question is a Win32 question

Everything this sitting turns on is behaviour of `CreateFileA`,
`CreateProcessA`, `TerminateProcess` and handle inheritance. **The author's
Windows box is ON**: `ssh win`, the repository at `/c/w/heroes`, clang 23.1.1,
and two probes from tonight already sit at `/c/w/shareprobe.c` and
`/c/w/windowprobe.c` with their binaries beside them. Read them; they are the
shape of the measurement this seat is asked to extend.

**The box is shared.** Before you run anything: `tasklist | grep -icE
"heroes.exe|main.exe"` must be 0, and you kill nothing you did not start.
`/c/w/cleanup.ps1` lists and kills only `heroes` and `main`. Two of tonight's
measurements were spoiled by a second process on that machine, so this is not
etiquette.

## What this seat is asked, and each answer is a program you ran

1. **Does the trap in route A happen?** Open a file the wide way, spawn a child
   that inherits the handle and keeps writing, let the parent reopen the same
   path with `CREATE_ALWAYS`, and see whether the child's later bytes appear in
   what the parent then reads. This is the measurement that decides whether
   widening alone is sound. **It is currently unrun and it is the sitting's
   sharpest question.**

2. **What does `TerminateProcess` do to grandchildren?** The shared brief
   asserts it kills one process only and marks the assertion as Microsoft's
   documented behaviour rather than a measurement. **Measure it**: a C parent
   that spawns a child that spawns a grandchild that sleeps, then
   `TerminateProcess` the child and look for the grandchild. Cite where you
   read the documented behaviour too.

3. **Does a Job Object close it?** Write route C's Windows half: 
   `CreateJobObject`, `SetInformationJobObject` with
   `JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE`, `AssignProcessToJobObject`,
   `TerminateJobObject`. Compile it and show the grandchild dying. Say what it
   costs per spawn — a job object per call, or one for the whole process?

4. **Does `PROC_THREAD_ATTRIBUTE_HANDLE_LIST` (route D) reach the recorded
   holder?** The holder in `tests/harness/shell.hero:414-417` is a `clang.exe`
   spawned by a `heroes.exe`, i.e. a GRANDCHILD. Say whether limiting the
   direct child's inherited handles stops the grandchild from receiving them,
   and how you established it.

5. **The POSIX half must not regress.** Whatever you propose, the `#else` arm
   at `runtime/parts/run.c:465` serves Darwin and both Linux legs, and CL-055
   makes a program declaring an `extern` run its Linux leg under `--sanitize`.
   Compile and run your C on this Mac and in
   `docker run --rm -v "$PWD":/src:ro heroes-linux` before you claim it is
   sound.

## Build in a copy

`cp -r` the tree to your scratchpad and `rm -rf target build` there. A copied
`build/` leaves paths pointing at the real repository, which has silently
measured the wrong tree before. The working tree at
`/Users/joseph/Temp/heroes/heroes-lang` is frozen for the duration of this
sitting.

## What would make your verdict wrong

Name it, and say which command would falsify it. A negative claim — *Windows
cannot do X* — rests on what you searched for, so write what you searched and
where, or write the weaker sentence.
