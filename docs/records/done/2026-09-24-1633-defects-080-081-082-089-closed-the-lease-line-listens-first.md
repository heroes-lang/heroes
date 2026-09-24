# Defects 080, 081, 082 and 089 closed: the lease line listens first, says under, hears every trap, and leaves an ignored signal alone

2026-09-24, M-agreed-retention step 4, landing panel 175's resolution item 3
(route E) in a lane, `8a18290a`, merged as `79b55be5`. 080, 081 and 082 were
found by panel 175's seats in panel 173's lease line two days after it shipped;
089 was found at this landing, at the shape beside 082. One handler in two arms
closes the four.

- [x] **080 — the crash handler's lease line names the Heroes function that called the caller, not the one that called C** | the frame walk skips the direct caller when the C function that died was a noreturn call in an optimised library, so the line says `in e.main` for a death inside `e.run` | **closed 2026-09-24**, M-agreed-retention step 4, route E (`8a18290a`, merged `79b55be5`) | `runtime/parts/os.c:249` · `runtime/parts/stack.c`

    **Origin:** panel 175's ffi-pragmatist, 2026-09-23, prototyping route E;
    reproduced by the coordinator the same day before filing.

    **The reproducer.** A shared C library built `-O2 -fomit-frame-pointer`
    whose `lib_assert(x)` fails `assert(x == 42)`, called with a lease live from
    a Heroes function `run` that `main` calls:

        function run(what: str)
            if what == "assert_lease"
                y = "payload"
                d: cstr @ y.lease()
                lib_assert(x: 7)
                end_lease(@d)

    `run` 134, three of three, and the line reads *the process died with 1
    lease(s) still live, **in e.main***: on Darwin arm64 and on Linux arm64. The
    lease and the call are in `e.run`. On Linux x86-64 the walk finds no frame
    and the line names none, which is not false.

    **Why it is a defect.** Panel 173 R1: no path prints a sentence measured
    false. *In* claims the frame that called C, and a noreturn call does not save
    the return address the walk reads. The seat measured *under* true in every
    case it ran, because the named function IS on the stack.

- [x] **081 — on Linux x86-64 a C trap with a lease live dies at 132 with nothing on stderr** | the lease handler installs SIGTRAP and SIGABRT, and `__builtin_trap` on x86-64 is `ud2`, which is SIGILL | **closed 2026-09-24**, M-agreed-retention step 4, route E (`8a18290a`, merged `79b55be5`) | `runtime/parts/os.c:289`

    **Origin:** panel 175's ffi-pragmatist, 2026-09-23; reproduced by the
    coordinator the same day before filing.

    **The reproducer** is 080's library and program, case `trap_lease`: a lease
    live, then `lib_trap()`, which is `__builtin_trap()`. **Linux x86-64: 132,
    zero bytes, three of three.** Linux arm64, the same program: 133 and the
    runtime's lease line, 264 bytes. Darwin, measured by the seat: 133 and the
    line.

    **Why it is a defect.** Defect 070 closed on the report that names the live
    leases when C kills the process; on one of the four platforms, for one of
    the ways C kills it, the report is not there. The seat measured the line
    printing with SIGILL added, 265 bytes. Darwin x86-64 is UNRUN, since there is
    no Intel Mac here, and libmalloc's own trap would be `ud2` there too.

- [x] **082 — the lease line says the process died, and then the handler it found lets the process live and exit 0** | the line is written before the previous disposition is called, so a C library that recovers from its own signal leaves a false sentence above a successful run | **closed 2026-09-24**, M-agreed-retention step 4, route E (`8a18290a`, merged `79b55be5`) | `runtime/parts/os.c:249` · `:264`

    **Origin:** panel 175's compiler-engineer, 2026-09-23, prototyping route E,
    who reported it and did not file it; reproduced by the coordinator the same
    day before filing.

    **The reproducer.** A header whose constructor installs, before `main`, a
    SIGABRT handler that `siglongjmp`s back, and a function that `abort()`s
    inside a `sigsetjmp`:

        extern "e.h"
            function e_try_abort() -> i32

        function main()
            x = "payload"
            c: cstr @ x.lease()
            print("recovered: ", e_try_abort())
            end_lease(@c)

    With `E_CTOR=1`: **exit 0**, stdout `recovered: 1`, and 277 bytes on stderr
    beginning *panic: the process died with 1 lease(s) still live*, three of
    three on Darwin arm64, Linux arm64 and Linux x86-64.

    **Why it is a defect.** Panel 173 R1, *no path prints a sentence measured
    false*, and panel 173 R2 kept the chaining that makes this path reachable.
    The seat measured its E2 ordering — say the line only after the previous
    disposition has been called and has not returned control — at 0 bytes here
    and unchanged everywhere else.

- [x] **089 — a C library that ignores SIGABRT and then raises it is killed by the runtime, under a false lease line** | panel 173's lease handler takes SIGTRAP, SIGABRT and SIGILL whatever disposition it finds, so a signal the program set to be ignored reaches the handler, which speaks and re-raises it at its default | **closed 2026-09-24**, M-agreed-retention step 4, route E (`8a18290a`, merged `79b55be5`) | `runtime/parts/os.c` (`hero_lease_crash_install`)

    **Origin:** the coordinator, 2026-09-23, at panel 175's landing, attacking
    the Windows arm being written at the shape beside the recovering handler of
    defect 082: a handler that recovers was covered, a disposition that ignores
    was not.

    **The reproducer.** A C constructor sets SIGABRT and SIGILL to `SIG_IGN`; a
    C function raises both and returns 1; the program holds a lease across the
    call and prints what it returned. The right answer is `ignored: 1` and exit
    0. **On the trunk at `a747e5a2`: 134, three of three, on Darwin arm64, Linux
    x86-64 and Linux arm64**, with *the process died with 1 lease(s) still
    live* (300 bytes on Darwin and Linux arm64, 254 on Linux x86-64). Windows
    unrun on the trunk, where no SIGABRT handler is installed, so the shape is a
    question there rather than a premise.

    **SIGTRAP is not in the reproducer, and why.** Under Rosetta, the Linux
    x86-64 leg on this Mac, a C program with no Heroes in it that ignores
    SIGTRAP and raises it dies at 133, where the same program exits 0 on Linux
    arm64 (measured 2026-09-24); SIGABRT and SIGILL are honoured there. A case
    raising SIGTRAP would be red on that leg for the emulator's reason.

    **Why it is a defect.** The handler's own comment says it never changes what
    the process does, and here it turns a program that goes on into a death, and
    names a cause for it. The repair is in route E's lane: a signal found
    ignored is put back and not taken, on the POSIX arm and on Windows' `signal`.


## The repair

**POSIX** (`runtime/parts/os.c`): the handler calls the disposition it found
FIRST and speaks only if control comes back (082); names only a Heroes frame,
and says *under* it (080); takes SIGILL beside SIGTRAP and SIGABRT (081); speaks
with no lease live, naming the signal and who raised it from `siginfo_t` (the
milestone's first item); puts back a signal it finds ignored instead of taking
it (089). Under the sanitiser it stays installed, and ASan's death callback
silences it once ASan has reported. `runtime/parts/stack.c`'s frame helpers
moved above the ASan switch so the handler can use them there.

**Windows**: the heap-corruption arm gained its no-lease line (0xC0000374). The
box's first run read 134 and 2, the two deaths the arm did not hear: a C
`abort()`, which the UCRT turns into `raise(SIGABRT)` and then a fail-fast no
exception handler sees — now a `signal` handler that calls the one it found,
speaks, and ends in `abort()` at `SIG_DFL`, the status the process always had;
and a C trap, `ud2`, a continuable exception a `__try` can recover — now the
filter of last resort, after the one it found. The second run read 135 and 2:
the coordinator's own SIGABRT line wrapped inside the sentence the golden asks
for, and Windows under ASan had never been run, its arm compiled out. Both
repaired and measured on the box.

**The harness**: `expectation.hero`'s silent ending refuses a word on stderr,
which 082's shape had passed because only the exit code was read; and
`suite_runtime.hero`'s threads check reads a function-POINTER object, which its
own comment said it could not and which the Windows arm is the first of — its
declarator is now asked, not the row, and a mutation that drops the object's
`SHARED_BY_DECISION` entry turns the check red naming `os.c:258`.

## The measurements

After the last edit, in the lane: **Darwin arm64** run 143/0, lines 144/0,
runtime 8/0, determinism 173/0, warnings 204/0, canonical 2/0, emission 498/0,
the net's own tests 165/165, the compiler's 675/675; **Linux arm64 and x86-64**
(docker, the latter under Rosetta) run 139/0; **Windows x86-64** (the box) run
137/0. The five new goldens were also run one by one, plain and `--sanitize`,
on every leg. The last edit touched only the Windows arm, and the runtime
preprocessed for POSIX is byte-identical before and after it, plain and ASan
(`clang -E -P`, `cmp`), which is why the Linux runs stand for it.

**What the runtime before the merge (`13fcd61d`) does with each new golden**,
Darwin arm64, three of three:

| golden | old runtime |
|---|---|
| `fixedbugs-a-c-handler-that-recovers-leaves-no-line` | exit 0 with 206 bytes of a false line on stderr — red under the silent ending |
| `fixedbugs-an-ignored-signal-stays-ignored` | 134 under a false lease line; 134 on both Linux legs too |
| `fixedbugs-c-aborts-with-no-lease-live-and-the-runtime-says-it-did-not` | 134, zero bytes |
| `fixedbugs-c-gives-a-pointer-back-twice-and-the-runtime-says-so` | 133, zero bytes |
| `fixedbugs-c-traps-with-a-lease-live-on-every-architecture` | 133 with the line — green on Darwin, where 081 never was; 132 and nothing on Linux x86-64 |

**080 has no golden that pins the frame name, and the reason is measured**: the
Heroes frame the walk can find differs between the configurations of one case —
`lib.run` on Darwin, `lib.main` on Linux x86-64, none under the sanitiser, where
the stack bounds are never measured. Its evidence is the battery on three
platforms, where every *under* named a function on the stack.

**SIGTRAP is not in 089's golden**: under Rosetta a C program with no Heroes in
it that ignores SIGTRAP and raises it dies at 133, where Linux arm64 and Darwin
exit 0 (measured 2026-09-24). The runtime puts SIGTRAP back the same way.
