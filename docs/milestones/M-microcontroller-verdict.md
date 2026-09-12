# M-microcontroller-verdict — whether a program can run on a microcontroller, and what it would cost


**Scheduled 2026-09-10 by author decision**, out of the session that asked
whether supporting a microcontroller such as the ESP32, or a program under an
RTOS, would be worth a step, since the language compiles without a garbage
collector. The record was silent on the question, measured: zero hits for
microcontroller, RTOS, bare metal, freestanding, newlib or ILP32 in design.md,
`spec/`, `docs/` and the site in that sense. So the question was put to the
compiler and the runtime first, and the row is written on what came back:
`docs/measurements/026-the-two-facts-a-32-bit-target-refuses.md`.

**What it delivers.** A ruling and not a target, on `M-cleanup-verdict`'s
precedent: a milestone named for the verdict can close with a refusal, and one
named for the device would claim what the sitting exists to decide. The sitting
is the full lane, because a new class of machine touches the runtime's layout
(§1.12), the tool surface (§10) and a refusal in the log. It is handed seven
questions, each with its measurement beside it:

1. **Whether `DESIGN-LOG.md:539`'s refusal reaches a machine the compiler cannot
   run on.** Cross-compilation was *considered and not entered* on 2026-09-03
   because *the three platforms are measured on real machines by rule*. A board
   on the author's desk is a real machine and the program is measured there; but
   the compiler is 5,666,664 bytes on this Mac, so it is the program and never
   the compiler that crosses. The refusal's ground argues for the board and its
   letter, *never a `--target` flag* (M-arm-platform), against it. The sitting
   rules which.
2. **The 64-bit lock-free reference count at 32 bits.** `runtime/heroes_runtime.h:122`
   fails its `_Static_assert` under `--target=riscv32-unknown-elf` and under
   `arm-none-eabi`: a string literal's block is `static const` and the counter
   inside it must be read without a lock. On a microcontroller that block is the
   flash, so the header's own argument is stronger there; a narrower counter
   changes the layout `HERO_STR_STATIC` lays out, and the ABI stamp with it.
3. **Who delivers `spec:167` on a task stack.** `runtime/parts/stack.c` has a
   POSIX branch on `sigaltstack`, `mmap` and `dladdr` and a Windows branch on
   `SetThreadStackGuarantee`; FreeRTOS has neither, and a task's stack is small.
4. **The toolchain.** ESP-IDF ships GCC and Espressif ships a clang of its own;
   `heroes build` spells `clang` (`selfhost/cli/clang_floor.hero`,
   `selfhost/cli/compile.hero`), and `.claude/rules/generated-c.md` promises C11
   that clang type-checks. Whether the emitted C compiles under GCC is unrun.
5. **`main` against `app_main`.** The emitted `main(argc, argv)` calls
   `hero_args_set` and `hero_runtime_check_leaks`, and a device has no `argv`.
   Panel 114 R2's route, C beside the program, is the candidate.
6. **`spec:228` at 32 bits.** `size_t` is `u32` there, so a program bound to the
   device's SDK compiles for that chip alone, and M-core-packages' question (v)
   multiplies by every function the SDK declares.
7. **The instrument.** A board over serial, and Espressif's QEMU fork for a CI
   leg, which Homebrew's QEMU 11.1.1 is not: its `xtensa` and `riscv32` machine
   lists carry no `esp32`.

**Decided ahead, by the author on 2026-09-10 on recommendation**: the RISC-V
chips first (ESP32-C3, C6), because upstream clang and GCC carry RISC-V and Apple
clang already type-checks for `riscv32-unknown-elf`, while Xtensa lives only in
Espressif's fork; and ESP-IDF over FreeRTOS rather than bare metal, because
design.md Part 2's *not a systems language* is met to the letter when the
registers are Espressif's C, and every `str`, `[T]` and `{K: V}` needs `malloc`.

**What warrants it, said carefully.** Not Principle 0: a platform is not a form,
and the thesis effect on firmware is unrun, since metrics 2 and 4 have never run
anywhere (M-thesis-harness). What fits is the founding constraint, §1.11: the
device's SDK is C, and a language with no standard library has nothing to port.
§ What production-ready means row 3 names *the machines it runs on*, and this row
asks that table a question rather than joining it as an owner.

**The measuring session ran the same evening**, 2026-09-10, on ESP-IDF v6.1
installed through `eim`
(`docs/measurements/027-behind-the-first-refusal-two-files-and-one-symbol.md`).
Against each question above: **2** is confirmed at the type level and again at
the link, where `__atomic_load_8` is the one undefined symbol of a gc-sectioned
hello, and sharpened, since ESP-IDF's substitute is a single global spinlock in
a critical section (`components/esp_libc/src/stdatomic.c:20-22` of the v6.1
tree) and not a write to the literal, so what the refusal costs and what it
protects are two sentences; **3** narrows to two files, `runtime/parts/stack.c`
with 27 errors behind three headers newlib lacks (`dlfcn.h`, `sys/mman.h`,
`ucontext.h`) and one line at `runtime/parts/spawn.c:128`, nothing else in 6047
lines asking the device for what it does not have; **4** is answered both ways,
GCC 15.2 compiles the emitted C with 0 errors and 6 `-Wunknown-pragmas` for the
emitter's clang-only diagnostic block, and Espressif's clang 21.1.3, above the
floor of 18, with 0 errors; **7** stays open, QEMU having been excluded by the
non-interactive install. The RISC-V object needs 83 outside symbols, 15 of them
libgcc's because ESP32-C3 has no FPU and no 64-bit registers, and the hello with
its runtime and newlib is 42,351 bytes at `-O2`, read through a labelled probe
stub. Nothing ran on a board.

**Why here.** After M-deployable-binary, whose question, *what does the machine
that RUNS a program need*, is this row's question with the answer *a C library
the runtime was not written for*; and after M-arm-platform, which is the
rehearsal for adding a platform under `.claude/rules/platforms.md`. Before
M-install-channels, which would otherwise ship for a class of machine nobody has
ruled on. The cheaper position, a note and no row, was declined by the author
(CL-040).

**What it does not deliver**: a `--target` flag, a fifth CI leg, a board, a
standard library for the device (§1.11), a form in the language (panel 114 R1),
or code. What it may become is the sitting's to say.

*******************************************************************************
**OPEN: 1**

- [ ] **M-microcontroller-verdict** | the ruling on a program running on a microcontroller under an RTOS, RISC-V first, and the two runtime facts a 32-bit build refuses today | `runtime/heroes_runtime.h:122` · `runtime/parts/stack.c` · `runtime/parts/spawn.c:128` · `DESIGN-LOG.md:539` · `docs/measurements/026-the-two-facts-a-32-bit-target-refuses.md` · `docs/measurements/027-behind-the-first-refusal-two-files-and-one-symbol.md` · `.claude/rules/platforms.md`

    **Origin:** author question 2026-09-10, whether supporting a microcontroller
    such as the ESP32, or a program under an RTOS, would be worth a step, since
    the language compiles without a garbage collector. The row, its shape (a
    verdict, not a target), its position and the chip family were the
    assistant's recommendations, and the author accepted all of them the same
    day. No board exists yet; the author is ordering one.

    **What the record said, read before anything was proposed**: nothing about a
    device, measured (zero hits for microcontroller, RTOS, bare metal,
    freestanding, newlib or ILP32 in design.md, `spec/`, `docs/` and the site in
    that sense); Part 2's *not a systems language*; `DESIGN-LOG.md:539`'s
    *cross-compilation considered and not entered, the three platforms are
    measured on real machines by rule*, restated at M-arm-platform as *never a
    `--target` flag*; panel 049's veto of a platform axis, and panel 114's R1 (no
    conditional compilation) and R2 (a header the program ships beside itself);
    `spec:228`, which at 32 bits makes `size_t` a `u32`.

    **What was measured on this Mac, 2026-09-10**, in the file named above: the
    runtime asks the C library for **73** functions, fifteen of them `pthread`,
    eight for processes, seven for signals and the stack guard; under
    `--target=riscv32-unknown-elf` and `arm-none-eabi` it refuses at
    `heroes_runtime.h:122`, a 64-bit atomic that must be lock-free because a
    string literal's block is read-only, and asks for `locale.h` and `math.h`,
    which a freestanding probe has no copy of; the smallest gallery program emits
    189 lines with `main(argc, argv)` and three includes; `stack.c` delivers
    `spec:167` through two branches, POSIX and Windows, and neither exists on
    FreeRTOS; a hello at `-O2` is 89,160 bytes stripped here and links libSystem
    alone. No Espressif toolchain is installed, Homebrew's QEMU knows no `esp32`
    machine, and Apple clang has no RISC-V backend, so everything about newlib,
    GCC and the device itself is **unrun**.

    **Decided ahead** (author, 2026-09-10, on recommendation): RISC-V chips first
    (ESP32-C3, C6), because upstream clang and GCC carry the target and Apple
    clang already type-checks for it, while Xtensa lives only in Espressif's
    fork; ESP-IDF over FreeRTOS rather than bare metal, because Part 2 is met to
    the letter when the registers are Espressif's C, and every `str`, `[T]` and
    `{K: V}` needs `malloc`.

    **The measuring session ran the same evening, 2026-09-10, with ESP-IDF v6.1
    installed through `eim`**
    (`docs/measurements/027-behind-the-first-refusal-two-files-and-one-symbol.md`).
    What it found: with the lock-free assertion neutralised in a scratch copy
    and three headers newlib lacks (`dlfcn.h`, `sys/mman.h`, `ucontext.h`)
    stubbed empty, **every remaining error is in `runtime/parts/stack.c`**, 27
    of them, all the stack guard's POSIX surface, plus one line at
    `runtime/parts/spawn.c:128` (`pthread_getattr_np`); nothing else in the
    runtime's 6047 lines asks the device for something it does not have. The
    emitted C of the smallest program compiles under GCC 15.2 with **0 errors**
    and 6 `-Wunknown-pragmas` warnings for the emitter's clang-only diagnostic
    block, and under Espressif's clang **21.1.3**, above the floor of 18, with 0
    errors. Linked against newlib with `--gc-sections`, as ESP-IDF itself links,
    the hello is undefined on **one symbol**, `__atomic_load_8`: the 64-bit
    reference count, which ESP-IDF supplies through a single global spinlock in
    a critical section (`components/esp_libc/src/stdatomic.c:20-22`), a lock
    that is a global and not a write to the literal. The RISC-V object needs 83
    outside symbols, 15 of them libgcc's soft-float and 64-bit helpers because
    ESP32-C3 has neither an FPU nor 64-bit registers. **Still unrun**: anything
    on a board or under Espressif's QEMU, which the non-interactive install
    excluded. The seven questions the sitting is handed are in the ROADMAP's
    own section for this row, with what 027 answered marked against each.

    **What it may not become by this row alone**: a `--target` flag
    (`DESIGN-LOG.md:539`), a standard library for the device (§1.11), a form in
    the language (panel 114 R1).

*******************************************************************************
