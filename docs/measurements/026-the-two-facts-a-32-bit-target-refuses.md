# 026 — Two facts a 32-bit target refuses, measured before any row was written

The author asked on 2026-09-10 whether supporting a microcontroller such as the
ESP32, or a program under an RTOS, would be worth a step, since the language
compiles without a garbage collector. The record was silent on the question
(§ What the record says, below), so before anything was recommended the question
was put to the compiler and to the runtime on this Mac. Every number here was
produced in that session and nowhere else. The two other platforms were not
probed. No Espressif toolchain exists on this machine, so what ESP-IDF's own C
library answers is **unrun**, and this file says so wherever it would otherwise
be inferred.

The row this seeded is `M-microcontroller-verdict`, `docs/ROADMAP.md` § The
chain, and the decisions the author took on it the same day are its
`DESIGN-LOG.md` line.

## What is on this Mac

```
$ clang --version | head -1
Apple clang version 21.0.0 (clang-2100.1.1.101)

$ clang --print-targets | grep -iE "riscv|xtensa|arm|thumb"
    arm  arm64  arm64_32  armeb  thumb  thumbeb          # no riscv, no xtensa

$ for t in xtensa-esp32-elf-gcc xtensa-esp-elf-gcc riscv32-esp-elf-gcc \
           riscv32-esp-elf-clang idf.py esptool.py arm-none-eabi-gcc; do command -v $t; done
(nothing: none of the seven is installed)

$ ls -d ~/.espressif ~/esp ~/esp-idf /opt/esp*
(none)

$ qemu-system-riscv32 --version | head -1
QEMU emulator version 11.1.1                              # Homebrew, upstream

$ qemu-system-xtensa -machine help
kc705 kc705-nommu lx200 lx200-nommu lx60 lx60-nommu ml605 ml605-nommu none sim virt

$ qemu-system-riscv32 -machine help
amd-microblaze-v-generic none opentitan sifive_e sifive_u spike virt
```

So: a QEMU that knows no `esp32` and no `esp32c3` machine, because those live in
Espressif's fork and not upstream; and a clang that can **type-check** for a
RISC-V target but cannot emit code for one, which is what made the probe below
possible without a cross-compiler.

## What the runtime asks of the C library

The instrument is the host assembler output, so that nothing is written: every
`bl`/`b` target and every `@GOTPAGE` data reference that is not a label defined
in the same unit.

```
$ clang -O1 -S -o - -I runtime runtime/runtime.c \
  | awk '/^_[A-Za-z0-9_]+:/{s=$1; sub(":","",s); def[s]=1}
         /^\t(bl|b)\t_/{calls[$2]=1}
         /@GOTPAGE/{match($0,/_[A-Za-z0-9_]+@GOTPAGE/); d=substr($0,RSTART,RLENGTH); sub("@GOTPAGE","",d); data[d]=1}
         END{for(c in calls) if(!(c in def)) print "call", c; for(d in data) if(!(d in def)) print "data", d}' \
  | grep -v "hero_" | sort
```

**73 external functions and 4 data symbols**, grouped here by hand from the
sorted list:

| group | count | names |
|---|---|---|
| processes | 8 | `fork` `execvp` `waitpid` `pipe` `dup2` `kill` `getpid` `_exit` |
| signals, the stack guard, the loader | 7 | `sigaction` `sigaltstack` `mmap` `mprotect` `munmap` `dladdr` `sysconf` |
| threads | 15 | `pthread_attr_destroy` `pthread_attr_getstacksize` `pthread_attr_init` `pthread_attr_setstacksize` `pthread_create` `pthread_get_stackaddr_np` `pthread_get_stacksize_np` `pthread_getspecific` `pthread_join` `pthread_key_create` `pthread_mutex_lock` `pthread_mutex_unlock` `pthread_once` `pthread_self` `pthread_setspecific` |
| files and directories | 20 | `open` `read` `write` `close` `fopen` `fclose` `fread` `fwrite` `fseek` `ftell` `fflush` `setvbuf` `stat` `mkdir` `remove` `rename` `opendir` `readdir` `closedir` `fcntl` |
| text and output | 15 | `printf` `fprintf` `fputs` `putchar` `snprintf` `strtod` `strlen` `strchr` `strpbrk` `memcmp` `memcpy` `bzero` `newlocale` `uselocale` `freelocale` |
| memory, stopping, sleeping | 5 | `malloc` `free` `abort` `exit` `nanosleep` |
| inserted by the compiler | 3 | `__error` `__memcpy_chk` `__stack_chk_fail` |
| data | 4 | `__stderrp` `__stdoutp` `__stack_chk_guard` `__chkstk_darwin` |

Two of the `pthread` names carry `_np` and are Darwin's; the Linux branch of
`runtime/parts/stack.c` reads the bounds through `pthread_getattr_np` instead,
which this Mac's assembler output cannot show. The list is what one platform's
build of the runtime asks for, and it is the runtime as one unit: a program that
spawns no thread and reads no directory still links the whole file.

The runtime's own size, for the record: `runtime/runtime.c`,
`runtime/heroes_runtime.h`, `runtime/hero_os.h` and the twenty files under
`runtime/parts/` are **6047 lines** together (`wc -l`), and the platform
conditionals in them count **48 lines** (`grep -c "_WIN32\|__APPLE__\|__linux__"`,
per file: `runtime.c` 3, `parts/f64.c` 5, `parts/alloc.c` 4, `parts/os.c` 1,
`parts/dir.c` 3, `parts/spawn.c` 10, `parts/fs.c` 8, `parts/run.c` 7,
`parts/stack.c` 7).

## The probe: the runtime and one emitted program under a 32-bit target

`-fsyntax-only` writes nothing; `-ffreestanding` with no sysroot means the only
headers found are clang's own (`stdint.h`, `stddef.h`, `stdbool.h`, which are the
three `heroes_runtime.h` includes), so a *file not found* below names a hosted
header the code asks for and says nothing about whether a device's C library has
it.

```
$ clang -fsyntax-only --target=riscv32-unknown-elf -ffreestanding -I runtime runtime/runtime.c
runtime/heroes_runtime.h:122:16: error: static assertion failed due to requirement
  '__atomic_always_lock_free(sizeof(_Atomic(long long)), 0)':
  a 64-bit atomic needs a lock here, and a str literal's block is read-only
runtime/runtime.c:92:10: fatal error: 'locale.h' file not found

$ clang -fsyntax-only --target=arm-none-eabi -ffreestanding -I runtime runtime/runtime.c
(the same two lines)
```

The program is the smallest in the gallery, `examples/gallery/00-first.hero`,
which prints `(2 + 3) * 4`. The compiler used was built from the seed into the
scratchpad, because `./heroes` in the root had been removed by a peer session
between two commands.

```
$ ./heroes build examples/gallery/00-first.hero --emit-c > first.c
$ wc -l < first.c
189
$ grep -n "#include" first.c
2:#include "heroes_runtime.h"
3:#include <math.h>
4:#include <hero_os.h>
5:#include <heroes_runtime.h>
$ grep -n -A5 "^int main" first.c
184:int main(int argc, char **argv) {
185-    hero_args_set(argc, argv);
186-    h_00first_main();
187-    hero_runtime_check_leaks();
188-    return 0;
189-}

$ clang -fsyntax-only --target=riscv32-unknown-elf -ffreestanding -I runtime -x c first.c
heroes_runtime.h:122:16: error: static assertion failed ... (as above)
first.c:3:10: fatal error: 'math.h' file not found
2 errors generated.

$ clang -fsyntax-only -I runtime -x c first.c        # host, arm64, the control
(0 errors)
```

**The two facts, then.** First, the assertion at `heroes_runtime.h:122` is
about the target's word size and not about the probe: a string's reference
count is a 64-bit atomic, the header asserts it is lock-free because a literal's
block is `static const` and may live in read-only memory, and on a 32-bit
machine a 64-bit atomic is not lock-free. The header's own comment says why a
lock is not an answer; on a microcontroller the read-only memory is the flash,
so the argument is stronger there and not weaker. Second, every emitted unit
includes `<math.h>` and the runtime includes `<locale.h>`, `<float.h>` and
`<math.h>` (`runtime/runtime.c:92-94`): a hosted C library is assumed, which an
RTOS with newlib supplies and bare metal does not.

## The guard that turns deep recursion into the promised abort

`spec:167` promises that recursion too deep aborts. `runtime/parts/stack.c` is
492 lines and delivers it in two branches: under `!defined(_WIN32)` it includes
`signal.h`, `pthread.h`, `dlfcn.h` and `sys/mman.h` and uses `sigaltstack`,
`sigaction`, `mmap` with a `PROT_NONE` page and `dladdr` for the function's
name; the Windows branch (`runtime/parts/stack.c:424-492`) uses
`SetThreadStackGuarantee` and `AddVectoredExceptionHandler`; under the sanitizer
the guard yields (`runtime/parts/stack.c:86-94`). Neither branch exists on FreeRTOS, and a task's stack there
is small. Who delivers `spec:167` on a device is a question for the sitting and
is not answered here.

## A hello binary on this machine

An arm64 macOS number, recorded because it is the only one available, and it
says nothing about a device's flash.

```
$ ./heroes build examples/gallery/00-first.hero -O2 -o first-O2
$ size first-O2
__TEXT   __DATA   __OBJC   others       dec          hex
49152    16384    0        4295032832   4295098368   100020000
$ strip first-O2s && ls -l first-O2s        # a copy
89160 bytes
$ otool -L first-O2 | tail -n +2
/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1356.0.0)
```

The compiler itself was **5,666,664 bytes** at 10:53 this morning (`ls -l
heroes`, before a peer session removed it). That the compiler cannot run on the
device is therefore a fact about this binary; a device's flash size is the
vendor's figure and was not measured here.

## What the record says

Searched 2026-09-10, case-insensitive, for `embedded`, `microcontroller`, `MCU`,
`ESP32`, `Arduino`, `RTOS`, `bare metal`, `freestanding`, `newlib`, `ILP32`,
`32-bit target`:

- `design.md`, `spec/heroes-spec.md`, `site/src`: **zero** hits.
- `docs/`: nine files carry the word *embedded*, every one in the sense of an
  embedded library or an embedded NUL
  (`docs/panel/120-the-signature-cannot-say-which-half-it-keeps.md:96`,
  `docs/panel/083-the-pointer-that-could-not-be-read.md:47`), none in the sense
  of a device.

What the record does say, read before anything was proposed:

- `design.md` Part 2, non-goals: *"Not a systems language. This is a high-level
  language with an excellent C FFI. Low-level work is delegated to C. The
  runtime itself will be written in C."*
- `design.md:745`: *"The compile toolchain is clang only (`tcc` is not usable on
  Apple Silicon; `zig cc` remains interesting for future cross-compilation)."*
- `DESIGN-LOG.md:539`, 2026-09-03: *"Considered and not entered, each with the
  rule that refused it: … cross-compilation (the three platforms are measured
  on real machines by rule)"*. The `M-arm-platform` row of `docs/work/SCHEDULED.md`
  restates it as *"What it may not become: a `--target` flag."*
- `docs/panel/049-the-platform-axis.md`: the platform axis inside the language
  refused with a veto; `docs/panel/114-the-question-was-not-which-platform.md`
  R1: no conditional-compilation form enters; R2: a program may ship a header
  beside itself, where `#ifdef` already exists.
- `spec:228`: a parameter and a field are declared at the header's own width
  and sign. On a 32-bit target `size_t` is `u32`.

## What this file does not say

Whether ESP-IDF's newlib answers the 73 names; whether FreeRTOS's own stack
check can stand where `stack.c`'s two branches stand; whether the emitted C
compiles under GCC, which is what ESP-IDF ships, against `.claude/rules/generated-c.md`'s
*clang type-checks it*; and anything about the Xtensa chips. Each is a question
for the row's measuring session, on a toolchain this Mac does not have yet.
