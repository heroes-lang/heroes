# M-online-compiler — the compiler, reached without installing it


**Scheduled, no warrant** (author instruction 2026-09-06, *"maybe it makes sense
to add a step to the roadmap that says: let us make a compiler that runs inside a
browser, so whoever wants to try the language can try it quickly on the site,
compiling to wasm perhaps"*, and in the same minute the fallback, *"or if that
cannot be done, a remote compiler, but with safeguards so it does not become a
way to break into a machine or to burn resources forever"*). Installing Heroes is
a `git clone` and one clang line (M-install-channels); this row is for the
visitor who will not run even that.

**The refusal this row half-reverses, and the half of it that still stands.**
`DESIGN-LOG.md:539` refused *a web playground* on 2026-09-03, and
`docs/work/DONE.md:2415` records it among five candidates given a recorded
refusal *so the candidate is not proposed again as new*. The reason given was
*Part 2 and Part 9: wasm breaks the FFI premise*, and both cited passages —
`design.md:590` and `design.md:2869` — are about **a Heroes program targeting the
web**, as is `design.md:520`'s *native compilation rather than wasm* among the
decisions that are not revisited. All three rest on one fact: wasm cannot call
native C libraries, so under §1.11 a Heroes program compiled to wasm has nothing
to call. **The compiler is a different program** and the argument does not reach
it. Where it still bites is the second half — whatever runs the **visitor's**
program must reach the headers that program names.

**The number that makes this hard, measured 2026-09-06.** Of **56** programs
under `examples/`, **20** declare an `extern`: `hero_os.h` (10), `math.h` (3),
`stdio.h` (2), `sqlite3.h` (2), `time.h`, `raylib.h`, `curl/curl.h`,
`SDL3/SDL.h`, with two linking `raylib` and two `sdl3`. §1.11 is why — there is
no standard library, so the programs that show what the language is *for* are
exactly the ones that open a window, a socket or a database. **A playground that
refuses `extern` demonstrates a language that does not exist; one that allows it
on a public server is a remote shell**, and `extern "stdlib.h" { function
system(cmd: cstr) -> i32 }` is one line the compiler is right to accept. The
deliverable is therefore a **decision about what a stranger's program may name**,
and then whichever engine enforces it.

**The two engines, in the author's own order of preference.**

- **In the browser.** `seed/heroes.c` is **733,838** lines of C and would be
  compiled to wasm with `runtime/`; the visitor's program is then checked,
  formatted, dumped and emitted as C entirely on their own machine, at zero
  attack surface and zero running cost, inside a site that stays static. What it
  cannot do is **run**: `runtime/parts/run.c` reaches the toolchain through
  `execvp` and `CreateProcess`, and a browser has neither. Running there too
  needs clang itself hosted in wasm, and would still reach only the **36**
  programs that name no header.
- **Remotely.** The real compiler, the real clang and the real libraries, so all
  56 run, `sqlite` and `curl` included. The price is the sandbox the author
  named: one container per request off the image M-install-channels already
  builds, no network, a read-only tree, ceilings on wall clock, CPU, memory and
  output size, and the allow-list above. It also puts a service behind a site
  that is static today (Astro on Cloudflare Pages).

**No language change is owed, whichever engine wins** (panel 114; panel 036 as
corrected by 114's spec-warden, which found the record had been reading a
**deferral** of `compile` as a veto). 114 ruled that platform variation lives in
the header a program ships, with the `#ifdef` inside — one `.hero` source, exit 0
on macOS and Windows, `--emit-c` carrying **zero** platform words. A browser is a
fourth platform under that ruling, so the mechanism for its arm exists already at
zero spec tokens and this milestone opens no sitting to obtain one.

**The stack is the browser route's first hard number, and it was measured for
another platform** (M-thread-stacks, on the author's Windows box, 2026-09-06 at
`bcf6c41a`). A recursive-descent compiler in wasm gets a fixed slice of linear
memory chosen once at link time, which is structurally the Windows main thread's
1 MB rather than a stack that grows. That box now builds the seed with **the
contract's plain line**, no `/STACK` flag, and takes `build selfhost/lexer.hero
--dump-ir` and `build selfhost/main.hero --emit-c` to exit 0 — the first being
the module that in September died at **exit 127 with both streams empty**. So the
compiler compiling itself fits under a megabyte, and the failure mode when it
does not is silence, which is the one a browser would also give. CI now
**asserts** the module case on the Windows leg rather than reporting it
(`.github/workflows/ci.yml`); the whole-compiler case was measured and is not
asserted.

**And the spawn question is a build error there rather than a silence.** Read
2026-09-06, correcting the note that carried the number above, which had it that
the new floor goes inert on such a target. `runtime/parts/spawn.c:142-148` gates
`hero_spawn_floor = 0` on **`_WIN32` alone** and not on the absence of an OS to
ask, so a third platform takes the `#else`, and `hero_spawn_stack_of_self()` at
`:120-134` splits again on `__APPLE__` — landing a wasm build in the glibc arm,
on `pthread_getattr_np`. **There is no third arm**, so such a build either
compiles to a real query or stops at that line; its author states the two-arm
shape is deliberate on §11's loud-fallback rule, `_ => hero_unreachable()`
beating `_ => false` at M-value-aggregates, so a platform arriving there should stop the build
until somebody decides that platform's answer. The two readings are opposite
risks for this row: a silent skip is a defect found in a browser months later, a
build failure is an arm somebody writes on purpose. Whether a wasm libc provides
that function is untested here.

**What is not measured, written as a question rather than as a premise**
(CLAUDE.md §1). Apple clang 21.0.0 on the author's Mac has **no WebAssembly
target compiled in** — `clang --target=wasm32 -c` answers *No available targets
are compatible with triple "wasm32"*, measured 2026-09-06 — and neither `emcc`
nor `wasm-ld` nor a wasi-sdk is installed, so the browser route needs a **second
toolchain**, against M-install-channels' rule that every channel builds from the
seed with the one clang line. Whether the seed compiles under one is untested.
Whether a wasm-hosted clang is a real option is untested. What the languages
closest to this one actually ship is unverified and belongs to the historian, not
to the convener.

**Its opening convenes a panel** (CLAUDE.md §4 — this is the tool surface, and
what a stranger's program may name is language-facing). Three questions for it:
which engine; whether a wasm build of `heroes` is a second binary under §10 or
the same program for another target; and whether the allow-list is a property of
the playground or a `heroes` flag, which is §10's stopping rule asked about a
capability with no other caller.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
