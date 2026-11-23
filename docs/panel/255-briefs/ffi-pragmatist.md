# Panel 255, ffi-pragmatist

Read `00-shared.md` first, whole (repaired after the critic's first reading; read
`docs/panel/255-reports/critic-briefs.md` findings 1 to 10 too), then `2679-question.md`, panel 251's
§ R1 (`docs/panel/251-a-handlers-jump-is-judged-by-a-validated-record-a-thread-left-running-ends-main-as-the-programs-mistake-and-a-whole-value-crossing-waits.md`)
and your own report of that sitting, `docs/panel/251-reports/ffi-pragmatist.md`, lines 72 to 77 above
all: you recorded that Darwin's `setjmp` mangles the saved stack pointer (*"`eor` with a cookie, then
`autdb`, read in `dyld_info -disassemble` of libsystem_platform"*) and chose the entry over the frame,
partly because *"a record of the entry stays right across two stacks ... where comparing addresses would
not"*. You judge the founding constraint (design.md §1.11, §4.19: everything comes from C): what each
route does to the C real libraries run, and **whether it is sound**, compiled and run. Your veto is on
soundness and on ABI breakage; a cost is reported.

**Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p255-scratch/ffi-pragmatist/`, a
detached worktree at `a75179641`. Build there as `00-shared.md` § 7 says; your work goes under an
untracked `p255/` folder in it; keep each route's runtime as files there (`p255/T/rebind.c`, ...) and copy
them into `runtime/parts/` before a build, restoring the tree's own with `git show HEAD:<path>` when done.
**Your report**:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p255/docs/panel/255-reports/ffi-pragmatist.md`,
written as you go. **Your platform is Darwin, this Mac** (`rebind.c`), real libraries on it, and the
binding's C. The compiler-engineer holds Linux's `sigjump.c` and libpng there; you never read its folder.

Work in this order; what you cannot reach is said *unrun* in your report. The Mac's baseline of today's
runtime is the critic's (`00-shared.md` § 4: 2571 4/0, 2558 5/0, 2522 25/0); add your own run of
`fixedbugs-a-c-handler-that-recovers-leaves-no-line`, the handler filters § 4 lists and the `runtime`
suite under today's runtime first, each count line whole from a file.

## Q1. Can Darwin's saved stack pointer be decoded at a jump? (decides whether C reaches Darwin)

The instructions of `_setjmp`, `setjmp`, `sigsetjmp`, `_longjmp` and `siglongjmp` in libsystem_platform on
this Mac (macOS 27.2.1; the CI's leg is `macos-26`); which slot holds the stack pointer and by what
transformation; whether a program Heroes builds runs as `arm64` or `arm64e` (`file`, `lipo -archs`); and a
probe that saves at two far-apart known stack pointers and asks whether one cookie, with or without
stripping a signature (`xpacd`), recovers each, on two threads and after `fork`. Say what would have to be
true for the decode to be a premise verified before `main` and failing loud.

## Q2. Build route T on Darwin

`00-shared.md` § 5: the save writes an identifier into a span Darwin's saves never write (the critic
measured `[104,112)` and `[180,184)`); a jump reads that span of its target only and takes the record it
names, the entry Darwin's record already holds. Settle (a) to (f) of § 5 by runs where a run can: whether
`_longjmp` or `siglongjmp` read or check the span (disassembly, and a run with the span poisoned); a
partial copy; a stale identifier; Darwin's `ucontext_t` (has it a span `getcontext` never writes, or does
a context keep today's record?); a buffer filled by a save never seen. Say, as a soundness seat, whether
the runtime writing into memory the program handed the C library is sound, with the reason.

## Q3. Build route A on Darwin, the conservative arm; and C if Q1's decode holds

A: a fingerprint read only of a pending buffer on the thread's stack, its alternate stack, or the writable
segments of images loaded before `main` (from the add-image callback `rebind.c:721` registers); the
sanitizer shape (critic 8): on Darwin a buffer held by a non-saving frame moves off the stack only where
`ASAN_OPTIONS` asks `detect_stack_use_after_return=1`; run it. C on Darwin only if Q1 says the decode holds,
across two stacks as your 251 report's sentence asks. **Every Darwin route removes the read at a jump
outside every handler** (shapes 7 and 8, `rebind.c:425`): say how yours does.

## Q4. The same inputs under every route you built (today, T, A, C where built), on this Mac

The eight shapes (`repro/run.sh`), the three sets, the recovers case, the handler filters, the `runtime`
suite; `ijstale.c`, `ijv.c` (its nested SIGUSR2 shape), `ijo.c` and `ijn.c` from
`docs/panel/251-reports/evidence.tar.xz`. Name every verdict that moves against the baseline and its
direction: the refill case must stay refused; the copied-buffer case (ruling B's known cost) as it lands.
`swapcontext` notes `from` and reads it before it is filled (`rebind.c:465` and 466, `00-shared.md` § 4):
a context case refused may be refused for that cause; tell the two apart.

## Q5. Real libraries on Darwin

- **libpng**, the issue's origin (Homebrew's 1.6.59, `pkg-config --modversion libpng`, the critic's): a
  Heroes program decoding a PNG through libpng and one taking its error path (a truncated file), under
  each route.
- **A correct jump OUT of a handler through a buffer A cannot prove lives**: on the heap, `_Thread_local`,
  in a library `dlopen`ed after `main`, under each route. And how common: the libraries on this Mac (or the
  Linux images, read only) that keep a `sigjmp_buf`/`jmp_buf` in thread-local storage or on the heap
  (`nm`, headers, sources), or the search's words and that it found none.
- **A coroutine shape**: a context filled by `getcontext`/`swapcontext` on a stack the program then frees
  (Darwin's `ucontext` needs `_XOPEN_SOURCE`), under each route.
- Lua is on neither platform (`pkg-config --list-all`, the critic's): `lua_pcall` is unrun unless you
  build one from source inside your copy.

## Q6. The binding's C, and routes B, D, E, F in assembly

Does any route change what a Heroes binding writes, the C a header must hold, or what
`ffi_defined_elsewhere` tells a program library defining one of the ten names? Compile one binding under
each route to show it. From the libraries' side: D (releases told) against libpng's custom allocator
(`png_create_read_struct_2`), an arena and ASan's `free` interceptor; E (a read that cannot fault) against
ASan's `write` interceptor and a system call per pending pair. Build one only where it could be more
robust than T or C.

## Your report

A table per route on this Mac: the shapes, the case sets with their count lines, `ij*.c`, the real-library
runs, exit and verdict; your answers to `00-shared.md` § 6's two questions (a verdict that depends on the
platform; a sentence owed to the spec); your verdict per route (approve, object, veto, with the soundness
or ABI reason for a veto); **one falsifiable prediction** with the moment it becomes checkable (the lane
that lands the route, the CI's `macos-26` leg after the push); and everything unrun, said so.
