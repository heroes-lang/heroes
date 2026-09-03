# 104 — deep recursion stops with a word: the guard-page handler

**Convened 2026-09-03** by M-robustness-guards step 4 (`/decide` of the same day;
author instruction *"ratifica tutto"* and the four criteria: robust over cheap,
consolidation over shortcuts, history over the token count, every platform and
never a silenced error). **Soundness lane**: compiler-engineer and ffi-pragmatist
only — no surface, no spec token, no diagnostic class; a runtime panic like every
other. What the lane gave up: the llm-ergonomist's reading (the spec says nothing
about the stack and nothing changes there), the spec-warden's count (zero tokens
move), the historian's sourced precedent — the seats cite Rust's
`std::sys::pal::unix::stack_overflow` and its Windows twin, read by them in the
sitting, and nothing else is claimed as precedent here. **Status** `provisional
— author ratification pending`.

## The proposal, verbatim (from the brief)

**(a)** A guard-page handler in the runtime, installed at the emitted `main`'s
entry: POSIX — `sigaltstack` with a dedicated alternate stack and `sigaction` on
`SIGSEGV` and `SIGBUS` with `SA_ONSTACK | SA_SIGINFO`; the handler tests `si_addr`
against the main thread's stack bounds and, when the fault is in the guard
region, writes `panic: stack exhausted in <module.function>` and exits 134 the
way every other panic does; otherwise it restores the default disposition and
returns so the fault re-raises as before. Symbolisation: `backtrace()` +
`dladdr()` on POSIX, the mangled name un-mangled through the mangler's inverse;
Windows — `AddVectoredExceptionHandler` on `EXCEPTION_STACK_OVERFLOW`.
**(b)** the fallback: a depth counter in every emitted function's prologue,
portable and deterministic, one increment and one compare per call.

## What was measured for the brief

A five-line program, `down(n)` returning `1 + down(n - 1)`: on the Mac (Apple
clang 21, 8 MB main stack) 50,000 frames exit 0 and 100,000 **exit 139** with
nothing on stderr; in the Linux image (clang 22) 100,000 and 500,000 exit 139;
on the Windows box (clang 22, `/STACK:67108864`) 100,000 exit 0 and 1,000,000
**exit 127** with nothing on stderr — the number `hero_run_go` also uses for
*program not found*. Under `--sanitize` the Mac and Linux print
`AddressSanitizer: stack-overflow … d1000000.hero:2 in h_d1000000_down`, exit 134.
`grep` for any signal handling in `runtime/` and `selfhost/`: nothing.
`hero_panic` (`runtime/parts/panic.c:21-25`) flushes stdout, prints
`panic: %s`, aborts. The emitted `main` calls `hero_args_set(argc, argv)` first
(`selfhost/emit/decls.hero:227`).

## The verdict table

| judge | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **(a) APPROVE-WITH-CONDITIONS · (b) REJECT** | design.md §1.7/Part 5, §1.12, §1.1 | (a) new `runtime/parts/stack.c`, 227 prototype lines, **156 essential**; `runtime.c` +1, `os.c` +1; **0 files under `selfhost/`**; **ABI unchanged at 18** because the install hides inside `hero_args_set`. Linux: `_POSIX_C_SOURCE` → `_GNU_SOURCE` (`pthread_getattr_np` undeclared otherwise, measured) and `-rdynamic` on the Linux link line (without it glibc `dladdr` names nothing; binary 134,640 → 142,832 B). (b) per 10^8 calls on this Mac: `-O0` plain 0.50 s, counter **0.81 s (+55–60%)**; `-O2` plain 0.000 s, counter **0.81 s** — the counter forbids the recursion-to-loop transformation | at close: `git diff --stat` shows **0 files under `selfhost/emit/`**, `wc -l runtime/parts/stack.c` ≤ 200, `HERO_RUNTIME_ABI` moved only by step 5 (18 → 19, never 20), and the net's new `-O0` deep-recursion check prints `panic: stack exhausted in` at exit 134 on **both** the macOS and Linux CI legs | (1) **two witnesses**: `si_addr` in the guard region AND `sp` within 4 KiB of the low bound — on `si_addr` alone a wild store 8 KiB under the stack was reported *stack exhausted*, a false abort; (2) defer when a handler already exists (ASan's report is richer); (3) Linux's two lines, the `runtime.c:49-80` decision rewritten with its reason; (4) Windows measured on the box before the commit; (5) the frame walk stays (`w.hero` faulted inside `__vfprintf` and only the walk named `w.build`); (6) a `fixedbugs` golden at `-O0` 100k (exit 134 + the line) and a shallow one (exit 0); (7) (b) returns only as a frame-address probe on a platform whose VEH cannot name the frame, never as a depth counter |
| ffi-pragmatist | **(a) APPROVE-WITH-CONDITIONS · (b) REJECT** | design.md §1.11, §1.12, §4.19 | no ABI ground for a veto: nothing changes how a value crosses. Handler compiled on the Mac and in the Linux image: overflow → `panic: stack exhausted`, exit 134; NULL and wild stores re-raise, 139. `SIGSTKSZ` is 131,072 here and **8,192** on glibc 2.41. Real SDL2 and SDL3 (`/opt/homebrew`) install no SIGSEGV/SIGBUS handler after `SDL_Init` | a Heroes program binding **SDL3**, calling `SDL_Init` and recursing to death on the main thread, prints `panic: stack exhausted in <module.function>` and exits 134 under (a) with no shim, here and in the Linux container; falsified the day `SDL_Init` installs a SIGSEGV/SIGBUS handler | (1) **save and chain, never reset**: the brief's *restores the default disposition and returns* is a compiled defect — a library that installed its own handler in a constructor loses its write-barrier and dies **138**; saving `oact` and chaining gives 134 with the library recovering (five builds measured); (2) under `--sanitize` do not install (`__has_feature(address_sanitizer)`), ASan keeps its report; (3) threads: a C library's own thread overflowing dies 138 today and **132** with the handler (no per-thread alt stack) — a live hole to record, with a test; (4) keep SIGBUS; (5) size the alt stack in bytes (≥ 64 KiB, `mmap`ed, `PROT_NONE` below), never in `SIGSTKSZ` units; (6) Windows not verified: `AddVectoredExceptionHandler(0, h)` + `EXCEPTION_STACK_OVERFLOW` + `EXCEPTION_CONTINUE_SEARCH`, `SetThreadStackGuarantee` before any overflow, a run on the box before the commit; (7) the site sentence may shrink only to *the main thread on POSIX* until (3) and (6) are measured closed |

## Disagreements, stated plainly

**One, and it is about what happens when a handler is already installed.** The
engineer's condition 2 says the runtime never installs over a handler it did not
put there. The pragmatist's condition 1 says save the previous handler and CHAIN
it for a fault that is not on the guard page, because a library that installed
its own handler in a constructor (before `main`) recovers under chaining and dies
under the brief's *restore default* — and would go unguarded under *never
install over*. Both seats agree on the sanitizer: ASan's handler is already
there before `main`, its report names the `.hero` line, and ours must yield to
it. The resolution below takes the pragmatist's form for libraries and the
engineer's for the sanitizer, because the two cases are told apart at compile
time (`__has_feature(address_sanitizer)`) and the pragmatist compiled the
chaining case both ways.

**Both seats reject (b), for the same reason from two sides.** The engineer
measured its price (+55–60% at `-O0`, and at `-O2` it costs 0.81 s where the
plain recursion costs 0.000 s, because a counter with a side effect blocks the
transformation that turns the recursion into a loop) and named its premise
about the world (one `MAX` is wrong at Windows' 64 MB and for large frames,
CLAUDE.md §11). The pragmatist showed that where (a) is blind — a library's own
thread, a library that overwrites the handler after `main` — (b) is blind too,
because those frames are C. It covers nothing (a) lacks.

## Resolution — `provisional — author ratification pending`

**(a) lands in its complete form, with every condition of both seats; (b) does
not land**, and returns only as a frame-address probe on a platform whose
exception mechanism cannot name the frame, never as a depth counter.

What lands, in the runtime and nowhere else:

1. `runtime/parts/stack.c`, one part, ≤ 200 lines: a 256 KiB `mmap`ed alternate
   stack with a `PROT_NONE` guard page below it (never `SIGSTKSZ`, which is
   8,192 on glibc 2.41); `sigaction` on **`SIGSEGV` and `SIGBUS`** with
   `SA_ONSTACK | SA_SIGINFO`, installed from `hero_args_set` — so the emitted
   `main` is unchanged and **`HERO_RUNTIME_ABI` does not move for this step**.
2. **Two witnesses**: the fault is stack exhaustion only when `si_addr` lies in
   the guard region below the main thread's stack AND the faulting `sp` is
   within 4 KiB of that bound. Anything else is not ours.
3. **Save and chain**: the previous disposition is saved at install; a fault that
   is not ours is handed to it (`sa_sigaction` under `SA_SIGINFO`, else
   `sa_handler`), and only a saved `SIG_DFL`/`SIG_IGN` is reset to default before
   returning. A library that installs after `main` overwrites us and the program
   is as it was today — recorded as the boundary's residual hole, with the
   pragmatist's `e2_after_liboverwrite` as its witness.
4. **Under `--sanitize` nothing is installed** (`#if !__has_feature(address_sanitizer)`):
   ASan's `stack-overflow` report, which names the `.hero` line, stays.
5. **The message names the function**: a bounded, monotone frame walk from the
   fault's `pc` through the stack range, `dladdr` on each frame until a `h_`
   symbol is found, un-mangled to `module.function` — `panic: stack exhausted in
   d100000.down`, exit 134 through the same path as every other panic. Linux
   needs `_GNU_SOURCE` in `runtime.c` (the decision at `:49-80` rewritten with
   this reason) and `-rdynamic` on the Linux link line in `flags.hero`.
6. **Windows** gets its arm — `AddVectoredExceptionHandler(0, …)`,
   `EXCEPTION_STACK_OVERFLOW`, `SetThreadStackGuarantee` at entry,
   `EXCEPTION_CONTINUE_SEARCH` for everything else — and the arm is **measured on
   the box before the commit**, never read. Its silent 127 becomes 134 with the
   word, or the step does not close.
7. **Threads are a recorded hole**, not a fix: a C library's own thread that
   overflows dies 138 today and 132 with the handler, because the alternate stack
   is per-thread. M-isolated-threads installs one per runtime-created thread; a
   library's thread stays the boundary's. The site's *two holes* sentence shrinks
   only to *the main thread on POSIX and Windows* when all three platforms print
   the word, and says so.
8. **Goldens**: a `fixedbugs-` case at `-O0`, 100,000 frames, stderr line and exit
   134; a shallow case at exit 0; both in the net so the engineer's prediction is
   scored on the macOS and Linux CI legs.

## What a veto would compel

A veto of (a) leaves the language with a goal (§1.12) its showcase program
falsifies in five lines, and the site's *two holes* copy as the permanent answer.
A veto of the rejection of (b) compels a per-call cost on every program and a
number about the world in the emitter, measured to block `-O2`'s recursion
optimisation; the seats' conditions name the one shape (b) may return in.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | the landing commit's `git diff --stat` shows 0 files under `selfhost/emit/`; `wc -l runtime/parts/stack.c` ≤ 200; `HERO_RUNTIME_ABI` moves only for step 5 (18 → 19, never 20); the net's `-O0` deep-recursion golden prints `panic: stack exhausted in` at exit 134 on both the macOS and Linux CI legs — a nameless line on Linux falsifies the `-rdynamic` half | M-robustness-guards close |
| ffi-pragmatist | an `extern "SDL3/SDL.h" link "SDL3"` program that calls `SDL_Init` and recurses to death prints the panic line and exits 134, no shim, on the Mac and in the Linux image; falsified the day `SDL_Init` installs a SIGSEGV/SIGBUS handler | the first SDL3 program in `examples/` |

## Corrections to the brief, which the coordinator wrote

- *"restores the default disposition and returns"* was a compiled defect, not a
  design: it kills a constructor-installed library's write-barrier (exit 138,
  measured five ways). The resolution says chain.
- The brief sized the alternate stack in `SIGSTKSZ` units by implication;
  `SIGSTKSZ` is 8,192 on glibc 2.41, which is a second overflow inside the
  first if the handler symbolises. Bytes, `mmap`ed.
- The brief asked whether `dladdr` names the function *as linked today*; on the
  Mac it does (`-g` on every build, nothing strips); on Linux it does not
  without `-rdynamic`. The brief did not know that the Linux arm changes the
  link line.
- Windows 100,000 frames is exit 0 because of the 64 MB reserve ratified this
  morning, not because Windows is safe: 1,000,000 is exit 127 and silent.

## Author's verdict

**Pending.** The item that asks for it is open in `docs/work/DECIDE.md` and names
this sitting as `panel 104`; work proceeds on the provisional default (CLAUDE.md
§4 — a panel never blocks).
