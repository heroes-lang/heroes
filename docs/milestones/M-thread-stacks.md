# M-thread-stacks — every thread's stack, and what happens when it runs out *(closed 2026-09-06)*


The story is [journal 035](journal/035-thread-stacks.md). What a later milestone
has to honour is here, and nothing else.

**Panel 115 refused three things permanently, each with the measurement that
produced it and a return condition that is its only amendment path.** One stack
size on all three platforms is refused: 8 MiB is glibc's default imported, and on
Windows it is an eightfold **cut** from the 64 MiB `selfhost/cli/flags.hero`
links for a measured reason. A Windows arm passing `dwStackSize` is refused while
`STACK_SIZE_PARAM_IS_A_RESERVATION` is absent, because without it the number sets
the **commit** and not the reserve — libuv ships that defect today, and
`runtime/parts/spawn.c` makes the same call. And a stack-size parameter on
`hero_thread_spawn` is refused at a price both compiling seats measured:
`HERO_RUNTIME_ABI` 21→22, 209 emission goldens, two sites in `seed/heroes.c` and
ten hand-written `extern` groups in `examples/`, for a form no program asked for.
It returns if a program is shown that must choose its own stack.

**What the floor is, so a later reader does not re-derive it.** A thread this
runtime starts is given at least the stack of the thread that ran `main`. It is a
fact about the machine in hand rather than a number, so nothing ages and nothing
reaches the spec. On glibc it is inert; on Windows it does not run. It raises one
platform and lowers none, and the delivered size is verified **after the fact**
because macOS and glibc refuse opposite things and neither refuses a terabyte.

**What a later thread milestone inherits.** `hero_spawn_stack_of_self()` has two
arms and no third, deliberately: a new platform is a compile error at that line
until somebody decides its answer, which is §11's loud-fallback rule rather than
an oversight. And `runtime/parts/thread.c`'s isolation refusal — not this
milestone's guard — is what stops a Heroes callback on a thread a C library made;
narrowing `selfhost/emit/callback_guard.hero`'s set reopens a silent exit 132.

**Still open and re-homed to M-core-packages**: `cow.c`'s `if (refcount == 1)` is
a test and then a mutate, and two sittings have now failed to race it.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
