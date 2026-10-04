---
kind: decision
area: none
milestone: none
filed: 2026-10-03
commit: 8637cccd3ebdb880abb5d68e8834b103611ea0b0
github: none
---

# Panel 184's R5 landed: every command runs on a thread whose stack the compiler chooses

2026-10-03 at 11:30 by the clock, lane depth, defect 169, at `ffaf9f4d`.
Panel 184's R5
(`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md`,
ratified 2026-10-01) landed as the engineer's prototype priced it, with a
refusal answered rather than panicked. Before the first edit, on the lane's
compiler built from the seed of `02e507bc`, panel 184's blind task 3
(`docs/panel/184-briefs/blind/task3a.hero`, 300 additions) stopped `check` at
134, on this Mac and on Linux arm64, and `check selfhost/main.hero` under
`ulimit -s 512` did the same on both.

**What landed.** `selfhost/main.hero` starts one thread of 268,435,456 bytes
(`STACK_BYTES`) through the runtime's new `hero_thread_spawn_sized` and runs
the whole command on it, `main` exiting with its answer. A thread the
operating system will not start is exit 2 with the system's own number, never
a fallback to `main`'s stack, which would keep the compiler running while it
broke the floor in silence. `runtime/parts/spawn.c` takes the size, never
below panel 115's floor, asks `pthread_attr_setstacksize` for it and treats
its refusal as a refusal before any thread exists (a refused size used to
start the thread on the default, seen only by the check on the new thread);
on Windows the size is a reservation, `STACK_SIZE_PARAM_IS_A_RESERVATION`,
unrun there with the box offline. The guard that names the frame where a
stack ran out is claimed on the thread as on every thread the runtime starts,
so past the floor the abort is `panic: stack exhausted`, exit 134, as before.

**Beside it, in the same file**: under `ulimit -s unlimited` glibc reports the
main thread's size as all the room down to the mapping below it,
93,823,035,207,680 bytes in the Linux arm64 container, and panel 115's floor
asked every thread for that: `examples/threads` stopped at its first spawn,
*panic: the operating system refused to start a thread*, with `spawn.c` as it
stood at `02e507bc`. An unlimited limit now asks no floor (a thread asked for
nothing gets glibc's own, 2 MiB there); the example and the compiler both run
under it.

**Measured on this Mac** (arm64, `ulimit -s` 8176, the lane's compiler at
`-O0`, `scratchpad/lane-depth/r5mac/`, 25 shapes: panel 184's thirteen and
twelve beside them): every shape checks and formats at 1,000 and 2,000, the
never-closed parenthesis exit 1 as it must; at 5,000 nested calls (`check`
and `build`) and a record literal through an array field abort, and nothing
else, the two checking at 3,000 and aborting at 3,500 when bisected; at
10,000 every chain aborts in the checker and `fmt` still formats every shape
but the record literal. `build` aborts at 2,000 in no shape;
seven do not finish within 150 s there, in the IR verifier and not on the
stack (below). Task 3a checks and prints 45150; `check selfhost/main.hero`
under `ulimit -s 512` is 0 where it was 134.

**Measured on Linux arm64** (the `heroes-linux-arm64` container, Debian
clang 22.1.8, `ulimit -s` 8192, the same shapes at 1,000, 2,000 and 5,000):
every shape checks and formats at 1,000 and 2,000; at 5,000 the nested calls
and the record literal abort and nothing else, checking at 3,000 and aborting
at 3,500 as on this Mac; `build` at 2,000 aborts in no shape, eight past
150 s; the four cases build and print what they expect; task 3a checks and
prints 45150; `ulimit -s 512` 0 where it was 134.

**What R6 can say, and what it cannot yet.** N = 2,000 holds on this Mac and
on Linux arm64 for `check` and `fmt` in every shape, and for `build` where
the build finishes; the first abort on both is between 3,000 and 3,500, so
the margin is 1.5 at its thinnest, and doubling `STACK_BYTES` would double it
at the cost of address space. The Windows box is owed (offline on the day),
so the sentence is not written in the spec: its draft and offline price are
the lane's report's.

**What it found beside it.** The IR verifier takes time cubic in a function's
size, hidden until today behind the abort at a few hundred:
`irphases.released_on_return` in the owned temporaries of the block a
function returns from (a list nested 250 deep builds in 3.49 s of user time,
500 in 24.54 s), and `irvalues.dominators` in its blocks (`match` nested 250
deep 9.47 s, 500 69.28 s; an `&&` chain of 2,000 about 30 s), on a busy
machine. A correct program at the floor can take minutes to build. Reported
for an item of its own; the lane's cases keep to shapes that build in
seconds.
