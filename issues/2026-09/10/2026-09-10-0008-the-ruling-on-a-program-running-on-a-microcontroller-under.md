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
