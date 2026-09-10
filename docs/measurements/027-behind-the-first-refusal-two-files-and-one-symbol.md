# 027 — Behind the first refusal: two files, one symbol, and a program that compiles

`026` ended with a list of what it could not say, because no Espressif toolchain
existed on this Mac. The toolchain arrived the same evening, 2026-09-10, and this
file is the measuring session `M-microcontroller-verdict`'s row asked for before
its sitting opens. Every number here was produced in that session on this Mac.
**Nothing ran on a device or in an emulator**, and the two places where a probe
stub stands in for the world are marked as such.

## How the toolchain got here

The author had installed the Espressif Installation Manager, `brew install
--cask espressif/eim/eim-gui` (0.19.0), which installs the installer and not
ESP-IDF. The installation itself was run from this session:

```
$ eim install --non-interactive true --target esp32c3,esp32c6 \
      --path ~/.espressif --do-not-track true --cleanup true
INFO - Non-interactive mode: selecting all required features by default
INFO - Non-interactive mode: selecting all tools by default (QEMU excluded)
INFO - Cloning repository ... path: ~/.espressif/v6.1/esp-idf, reference: Tag("v6.1"), shallow: true
WARN - Attempt 1 failed: "Failed to fetch: Failed to consume the pack sent by the remote"
WARN - Attempt 2 failed ...
WARN - gix-based clone of ESP-IDF failed ...; falling back to system git
...
INFO - Now you can start using IDF tools
eim exit: 0
```

So the version is **ESP-IDF v6.1**, chosen by the installer's default and not by
this session; QEMU is **not** installed, because non-interactive mode excludes
it, and `eim list-tools` refuses to answer outside a terminal (*"Failed to
select: IO error: not a terminal"*), so how to add it is unrun. On disk: **7.1
GB** under `~/.espressif`, of which the ESP-IDF tree is 749 MB.

```
$ riscv32-esp-elf-gcc --version | head -1
riscv32-esp-elf-gcc (crosstool-NG esp-15.2.0_20251204) 15.2.0
$ clang --version | head -1                      # ~/.espressif/tools/esp-clang/esp-21.1.3_20260408/esp-clang/bin/clang
Espressif clang version 21.1.3 (https://github.com/espressif/llvm-project.git esp-21.1.3_20260408)
$ clang --print-targets | grep -iE "riscv|xtensa"
riscv32 - 32-bit RISC-V; riscv64 - 64-bit RISC-V; xtensa - Xtensa 32
```

Espressif's clang is **21**, above this project's floor of 18
(`selfhost/cli/clang_floor.hero`), which answers half of the row's question 4
before any C is compiled.

## Method

Every command below targets ESP32-C3's ISA, `-march=rv32imc_zicsr_zifencei
-mabi=ilp32`, which is also the newlib multilib the toolchain selects
(`.../riscv32-esp-elf/lib/rv32imc_zicsr_zifencei/ilp32/libc.a`). The runtime is
compiled as one unit, `runtime/runtime.c`, as every Heroes program links it. The
program is `examples/gallery/00-first.hero`, emitted with `--emit-c` in `026`.

Three things stand in for the world, each in the scratchpad and never in the
tree:

1. **a copy of `runtime/`** in which the `_Static_assert` at
   `runtime/heroes_runtime.h:122` reads `1 || __atomic_always_lock_free(...)`,
   so that the compiler can say what lies behind the first refusal;
2. **empty stub headers** for the includes newlib lacks, added one at a time
   until no *No such file* remained, so that the compiler can name the
   declarations those headers were supposed to carry;
3. later, **two probe stubs**, one a `pthread_getattr_np` that returns failure
   and one a non-atomic `__atomic_load_8`, so that an object and then a link
   could exist for `nm` and `size` to read. Neither is an answer; both are
   labelled in the section that uses them.

## 1. The runtime against the toolchain's newlib: the same two refusals, then one

```
$ riscv32-esp-elf-gcc -march=rv32imc_zicsr_zifencei -mabi=ilp32 -std=gnu11 -fsyntax-only -I runtime runtime/runtime.c
runtime/heroes_runtime.h:122:1: error: static assertion failed: "a 64-bit atomic needs a lock here, and a str literal's block is read-only"
runtime/parts/stack.c:102:10: fatal error: dlfcn.h: No such file or directory
```

With the assertion neutralised in the scratch copy, the only line left is the
`dlfcn.h` one. A fatal include error hides everything behind it, so the stubs:

```
stubbed <dlfcn.h>
stubbed <sys/mman.h>
stubbed <ucontext.h>
--- errors now, by file ---
  27 parts/stack.c
```

**Three headers newlib does not have, and every error behind them is in one
file.** The 27, by what they are:

| what | lines in `runtime/parts/stack.c` |
|---|---|
| `pthread_getattr_np` undeclared | 155 |
| `ucontext_t` unknown | 209 |
| `Dl_info` unknown, `dladdr` undeclared, `dli_sname` (5 uses) | 238 to 249 |
| `SA_SIGINFO` undeclared, `struct sigaction` has no `sa_sigaction` | 261, 262, 417, 418 |
| `siginfo_t` has no `si_addr` | 276 |
| `mmap`, `mprotect`, `munmap` undeclared; `PROT_READ` `PROT_WRITE` `PROT_NONE` `MAP_PRIVATE` `MAP_ANONYMOUS` `MAP_FAILED` undeclared | 368 to 376 |
| `SA_ONSTACK` undeclared | 418 |

Espressif's clang, `--target=riscv32-esp-elf --sysroot=$(riscv32-esp-elf-gcc
-print-sysroot)`, reports the same class in the same file and stops at its
default limit of 20 errors.

So the runtime's whole POSIX surface that the device lacks is the stack guard:
`sigaltstack`'s alternate stack, `sigaction` with `SA_SIGINFO`, the fault
address in `siginfo_t`, the `ucontext_t` the handler reads, `mmap`'s guard page
and `dladdr`'s symbol name. `026` predicted this file; it did not predict that
it would be the **only** one at the type level.

## 2. A second file, one line

Compiling to an object with the guard yielded through its own macro
(`-DHERO_STACK_GUARD_YIELDS_TO_ASAN=1`, the branch `runtime/parts/stack.c:86-94`
already keeps for the sanitizer) found one more:

```
parts/spawn.c:128:9: error: implicit declaration of function 'pthread_getattr_np'
```

`runtime/parts/spawn.c:120-134` measures the calling thread's stack size for a
spawned thread, `pthread_get_stacksize_np` on Darwin and `pthread_getattr_np`
elsewhere. ESP-IDF's pthread has neither. That is the second and last file.

## 3. The emitted program compiles, under both compilers

```
$ riscv32-esp-elf-gcc ... -std=gnu11 -Wall -fsyntax-only -I stubs -I runtime-probe -x c first.c
errors: 0   warnings: 6
00first.c:60: warning: ignoring '#pragma clang diagnostic' [-Wunknown-pragmas]     (and :61 :62 :63 :64 :78)

$ clang --target=riscv32-esp-elf ... -fsyntax-only -x c first.c
errors: 0
```

The six warnings are the emitter's `#pragma clang diagnostic push` / `error
"-Wdouble-promotion"` / `"-Wimplicit-float-conversion"` / `"-Wfloat-conversion"`
/ `"-Wimplicit-int-conversion"` / `pop` block, which GCC does not know and
skips. Under GCC those four promotions therefore stop being errors in the
emitted unit, which is a fact about the guard and not about the program. **The
other half of question 4 is answered**: the emitted C11 compiles under GCC 15
with zero errors, and under Espressif's clang 21 with zero errors and zero
warnings.

## 4. What the RISC-V runtime object needs from outside

With the `pthread_getattr_np` probe stub in place the runtime compiles to an
object, and `nm -u` gives the target's own list, **83 symbols**, replacing the
73 that `026` read off this Mac's assembler output:

- **15 from libgcc**, because ESP32-C3 has no FPU and no 64-bit registers:
  `__divdi3` `__eqdf2` `__eqsf2` `__extendsfdf2` `__fixdfdi` `__floatdidf`
  `__floatdisf` `__gedf2` `__gtdf2` `__gtsf2` `__ltdf2` `__ltsf2` `__nedf2`
  `__nesf2` `__truncdfsf2`;
- **5 atomic libcalls**, because `rv32imc` has no atomic instructions at all
  (no `A` extension) and no 64-bit atomics on any RV32: `__atomic_load_8`
  `__atomic_store_8` `__atomic_fetch_add_8` `__atomic_fetch_sub_8`
  `__atomic_compare_exchange_4`;
- **63 from newlib and POSIX**: `__errno` `__getreent` `_exit` `abort` `close`
  `closedir` `dup2` `execvp` `exit` `fclose` `fcntl` `fflush` `fopen` `fork`
  `fprintf` `fputs` `fread` `free` `freelocale` `fseek` `ftell` `fwrite`
  `getpid` `kill` `malloc` `memcmp` `memcpy` `memset` `mkdir` `nanosleep`
  `newlocale` `open` `opendir` `pipe` `printf` `pthread_attr_destroy`
  `pthread_attr_getstacksize` `pthread_attr_init` `pthread_attr_setstacksize`
  `pthread_create` `pthread_getspecific` `pthread_join` `pthread_key_create`
  `pthread_mutex_lock` `pthread_mutex_unlock` `pthread_once` `pthread_self`
  `pthread_setspecific` `putchar` `read` `readdir` `remove` `rename` `setvbuf`
  `snprintf` `stat` `strlen` `strpbrk` `strtod` `sysconf` `uselocale` `waitpid`
  `write`.

## 5. The link: one symbol

Against the toolchain's newlib with `--specs=nosys.specs`, which is not
ESP-IDF's C library but the nearest thing a link on this Mac can reach:

```
$ riscv32-esp-elf-gcc ... -Wl,--gc-sections --specs=nosys.specs first-rv32.o runtime-rv32.o -o first-rv32.elf
   3 undefined reference to `__atomic_load_8'
```

**One symbol.** Everything the runtime asks for and newlib does not have is in
functions the hello program never calls, and `--gc-sections` drops them. ESP-IDF
builds that way itself: `tools/cmake/build.cmake:178` adds
`-ffunction-sections` and `tools/cmakev2/project.cmake:399` links with
`-Wl,--gc-sections`.

Without `--gc-sections`, the dropped functions would have needed:

```
  28 `__atomic_fetch_sub_8'    18 `__atomic_fetch_add_8'    12 `__atomic_load_8'    3 `__atomic_store_8'
   4 `waitpid'    3 `dup2'    2 `sysconf'    1 `pipe'    1 `nanosleep'    1 `execvp'    1 `__atomic_compare_exchange_4'
```

`waitpid` `dup2` `pipe` `execvp` are `runtime/parts/spawn.c` and
`runtime/parts/run.c`, the process-running half this compiler uses and a device
program never reaches.

## 6. What ESP-IDF adds, and how it supplies the 64-bit atomics

The 68 of `026`'s 73 names that are not Darwin-only or compiler-inserted, checked
against the toolchain's `libc.a` with `nm --defined-only`: **41 defined**. The 27
not defined there, checked against `~/.espressif/v6.1/esp-idf/components` for a
definition-shaped line in a `.c` file outside tests:

| supplied by ESP-IDF | where |
|---|---|
| the 13 `pthread_*` | `components/pthread/pthread.c`, `pthread_local_storage.c`, `pthread_cond_var.c` |
| `mkdir` | `components/vfs/vfs_calls.c` |
| `_exit` | `components/esp_libc/src/syscalls.c` |
| `sysconf` | `components/esp_libc/src/sysconf.c` |

| supplied by nobody | mentioned in `.c` files under `components/`, tests excluded |
|---|---|
| `execvp` `waitpid` `sigaltstack` `mprotect` `dladdr` | 0 files |
| `pipe` `dup2` `sysconf`-shaped `nanosleep` | 1, 1, 3 files, none a definition |
| `mmap` `munmap` | 3, 2 files, none a definition |
| `sigaction` | 9 files, none a definition |

And the atomics. ESP-IDF's answer to `__atomic_load_8` on this chip is
`components/esp_libc/src/stdatomic.c:20-22`, verbatim:

```
// Only need to implement 64-bit atomics here. Use a single global portMUX_TYPE spinlock
// to emulate the atomics.
static portMUX_TYPE s_atomic_lock = portMUX_INITIALIZER_UNLOCKED;
```

with `_ATOMIC_ENTER_CRITICAL()` around each operation (`:92`, `:100`). So on the
device every touch of a string's reference count would take one global spinlock
inside a critical section. **That is the lock `heroes_runtime.h:122` refuses**,
and the header's stated reason, a write to a read-only page, is not what this
implementation does: the lock is a global, not a word inside the literal. What
the refusal costs and what it protects are therefore two different sentences,
and the sitting's question 2 is sharper for it.

## 7. A size, with a probe stub

To let `size` read a number the link was completed with a **non-atomic
`__atomic_load_8` stub**, scratchpad only, never for a device:

```
   text	   data	    bss	    dec	    hex	filename
  41418	    372	    561	  42351	   a56f	first-rv32.elf
```

**42,351 bytes** for the program, the runtime and the newlib it pulls, at `-O2`
with `--gc-sections`, on `rv32imc`. Not an image: FreeRTOS, ESP-IDF's libc and
the bootloader are not in it. Against the same program's 89,160 bytes stripped
on arm64 macOS (`026`).

## What this answers, by the row's questions

- **Question 2** (the 64-bit lock-free reference count): confirmed at the type
  level and again at the link, where it is the only undefined symbol of a
  gc-sectioned hello; ESP-IDF's substitute is a global spinlock in a critical
  section, not a write to the literal.
- **Question 3** (who delivers `spec:167`): `runtime/parts/stack.c` is the only
  file whose POSIX surface the device lacks, 27 errors enumerated above, plus one
  line at `runtime/parts/spawn.c:128`. Nothing else in 6047 lines asks the
  device for something it does not have.
- **Question 4** (the toolchain): GCC 15.2 compiles the emitted C with 0 errors
  and 6 `-Wunknown-pragmas` warnings for the clang-only diagnostic block;
  Espressif's clang 21.1.3 compiles it with 0 errors and clears the floor of
  18.
- **Question 7** (the instrument): QEMU was excluded by the non-interactive
  install and `eim list-tools` needs a terminal, so the emulator is still
  unrun.

## What this file does not say

Nothing ran on an ESP32-C3 or under Espressif's QEMU. The link was against the
toolchain's newlib with `nosys`, not ESP-IDF's `esp_libc` under FreeRTOS, so
`sysconf`, `_exit`, `mkdir` and the `pthread_*` names, which ESP-IDF supplies
and `nosys` does not, appear undefined only in the no-`gc-sections` run. The
size carries a probe stub. And `nanosleep` was not found defined under
`components/esp_libc/src` by a grep for `nanosleep(`; a definition elsewhere or
under another name is a question, not an absence.
