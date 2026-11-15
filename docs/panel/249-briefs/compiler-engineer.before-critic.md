# Panel 249, compiler-engineer

Read `00-shared.md` first, whole: the base (`722ae76bf`, `sources 2ab7ca0a644692a1`), the rules (**your
binaries are named `ce-...`, your containers `ce-249-<n>`**, every program that may not end has its
bounds there, Go only with its writes moved into your folder), the platforms (**the Windows box is
off**; the emulated x86-64 image is not the CI's leg), the lane and **the escape**, and the three
questions with every fact this brief leans on. You judge from the compiler's and the runtime's side, with
a veto on soundness. Your folder is `.claude/worktrees/p249-scratch/249-compiler-engineer/`, your tree
`tree/` in it, your compiler built there from the seed in two stages. Your report is
`docs/panel/249-reports/compiler-engineer.md` in `.claude/worktrees/p249`, in your definition's output
structure (`.claude/agents/compiler-engineer.md`, its line 36), one section per question, written as you
go. **The route you recommend you build in your tree and run on the cases you name**; a route that does
not build is reported as not built, never adopted (`/panel` 3c).

**Your share of the building**, so the two soundness seats do not build the same thing twice: **you
build candidate C** (frame records in every unit the compiler builds) **and candidate B'** (a wrapper
handler with a per-thread depth) into the runtime; the ffi-pragmatist builds candidate A (the owned
marker) and D-tramp, and runs the census. Each of you prices the others' from your own side, and reads
the other's report before your verdict if it is there.

## Where the code is (`wc -l` and `grep -n` at the base; 00-shared.md has the rest)

- `runtime/parts/sigguard.c`, 364: the remedy `HERO_SIGNAL_REMEDY` (87-91), the walk's bound (156), the
  probes through the C library's own `sigaction` on SIGURG (175-186), `hero_signal_learn` (196), the
  thunk's three questions in `hero_callback_entered` (316-345: the exact test 320, the claimed thread 324,
  `SS_ONSTACK` 332, the walk 338), the Windows arm (347-364). `runtime/parts/sigdoor.c`, 137.
  `runtime/parts/stack.c`, 1213: the alternate stack every claimed thread holds (994-1009), the guard
  yielding to ASan (96-100, 567). `runtime/parts/alloc.c`, 1548: D's guard, its comment from 489, its
  refusal through `hero_signal_stop` with the same remedy (555-557). `runtime/parts/thread.c`, 144.
  `runtime/runtime.c`, 315: the parts' order (`stack.c` 154, `sigguard.c` 161, `sigdoor.c` 166, `alloc.c`
  181).
- `selfhost/emit/callback_thunk.hero`, 343: the thunk's first line (146, `hero_callback_entered("...",
  __builtin_return_address(0), __builtin_frame_address(0))`) and `hero_signal_learn` in
  `hero_callback_of` (216). `selfhost/emit/callback_guard.hero`, 207.
- `selfhost/cli/flags.hero`, 584: `flags()` (133-154, 19 words, none a frame word), `frame_words()`
  (198-202, a word asked of the machine), `level_words` (214), the comment saying a unit's compile, the
  runtime's object and the link read one list (204-213). `selfhost/cli/link_flags.hero`, 215: `-rdynamic`
  on every POSIX link (its comment from 70, so an executable's own definition of a C library name is
  exported to a shared library, which 248's compiler-engineer measured, `248-reports/compiler-engineer.md:192-204`,
  carried).
- `selfhost/cli/supplied.hero`, 357, and the `supplied_*.hero` modules beside it (`ls selfhost/cli | grep
  -c supplied`: 12 files): panel 239's member `ffi_defined_elsewhere` (`.claude/rules/c-boundary.md:186-221`).
- `tests/harness/suite_run.hero`, 740: each `run` case judged at `-O0` (221) and `-O2` (228), and under
  `--sanitize` (238). `tests/golden/run/`: 795 `.hero` cases (`ls | grep -c -E '\.hero$'`), 14 of them
  `fixedbugs-2494-` (`ls tests/golden/run | grep -c -E '^fixedbugs-2494-.*\.hero$'`).
- 248's route I-onstack as you built it then: `briefs/inputs/p248-ce/siginstall.c` (250 lines,
  `df1390978d6811c9`), the probes' change in your 248 report (`248-reports/compiler-engineer.md:56-76`,
  carried); I-mask's Linux runtime part, `briefs/inputs/p248-fp/siginstall_mask.c` (236) and
  `sigguard_mask.c` (374).

## Question 2, candidate C: frame records in every unit the compiler builds

1. **Reproduce the CI's three reds in the emulated x86-64 image, at the base** (00-shared.md § The
   platforms; `heroes-linux:latest`, no `--platform` flag): seed, stage 2, then the `run` form narrowed
   to each of the three cases, `fixedbugs-2494-a-heroes-function-a-c-handler-calls-is-refused`,
   `-a-heroes-atexit-function-a-c-handler-reaches-through-exit-is-refused`,
   `-a-heroes-function-a-c-handler-reaches-through-fork-is-refused`, one at a time; then all fourteen
   `fixedbugs-2494`. The coordinator's emulated stage 2 is recorded in `briefs/linux/base-amd64.log`;
   bound your container at `timeout -s KILL 5400` and say the wall it took (emulated, at the load you
   read, not a measurement). Say first whether the image reproduces the CI's 803-and-3 shape on those
   three at `-O2` and passes them at `-O0`, as the CI's log reads (`briefs/ci/job-132377462166.log:4315-4350`);
   if it does not, the emulated image is not this question's instrument, and you say so.
2. **Build the word**: `-fno-omit-frame-pointer` reaching every unit, the runtime's object and the link,
   in the place `flags.hero` would carry it (in `flags()` for every machine, or asked of the machine as
   `frame_words()` asks its word, and why), stage 2 rebuilt, `--version`'s `sources` line said. Run the
   three cases and the fourteen in the emulated image, and the fourteen and the `callback`, `signal` and
   `handler` groups on this Mac and in Linux arm64 (in Linux arm64 the coordinator's `objdump` read the
   case handler keeping its record at `-O2` without the word, 00-shared.md § Question 2, C; on this Mac
   that disassembly is unread: read it). Which suites the change owes:
   `flags.hero`'s row of `.claude/rules/verification.md` names `run` whole and `cache` alone, because every
   unit's and the runtime's key moves; run them on this Mac or say which you could not and why.
3. **What moves and what it costs**: `cmp` of `--emit-c` for one 2494 case with and without the word,
   the line `hero_program_sources` taken out of each (`.claude/rules/verification.md` § The formatter's
   probe says how), which should print nothing since the word is clang's and not the emitter's (say what
   it printed); and the price on x86-64, where the word changes code: the size of the text of one program
   and of the compiler (`size`, or `objdump -d | wc -l`), and instructions where a counter exists (the
   Linux image has no `/usr/bin/time`, as 248's container 2 found, `248-reports/compiler-engineer.md:546-547`,
   carried; say what you counted instead). On this Mac and in Linux arm64 say whether the word changes a
   single instruction of `qsortcost` (panel 247's, `briefs/inputs/ev247/247-compiler-engineer/cases/`,
   built as `ce-qsortcost`), by `objdump -d` compared, before you count instructions there.
4. **What it leaves**: R3's two cases, whose library is built apart `-fomit-frame-pointer` (the word
   reaches none of it: run them under the word on both POSIX legs and say so with the exits); a library
   that keeps frame records but calls back deeper than `HERO_SIGNAL_WALK` frames; Windows, where there is
   no walk (247's R10). **Say whether C alone closes 2522**, in R3's terms: it closes the tree's three
   cases on x86-64 at most, and R3's two cases are its residue.

## Question 2, candidate B': a wrapper handler with a per-thread depth

Build it in your tree's runtime, Linux arm64 first (by name, as 248's I-onstack and I-mask were), the
probes kept on the C library's own `sigaction`:

1. The runtime's definitions of the installers (glibc's eight linkable handler takers and the compat
   `sigvec`, `sigignore` and `siginterrupt` mirrored, as 248's brief listed them, `248-briefs/00-shared.md:395-417`,
   carried) install **a wrapper of the runtime's** as the kernel's handler and keep the program's own in
   a table, so `sigaction(sig, NULL, &old)` answers the program's handler; the wrapper keeps the
   program's flags' meaning (`SA_SIGINFO`, `SA_RESETHAND`, `SA_NODEFER`, `SA_RESTART`) and its mask, and
   around the program's handler keeps a per-thread depth the thunk asks.
2. **The objection you wrote at 248** (`248-reports/compiler-engineer.md:493-500`, carried): a depth a
   `siglongjmp` out of the handler leaves raised refuses every later callback of a correct program.
   Build and run **the question 00-shared.md poses**: a depth entry holding the wrapper's stack pointer at
   entry, compared with the thunk's frame, so a handler left by `siglongjmp`, by glibc's plain `longjmp`
   (which gives no mask back, `248-reports/ffi-pragmatist.md:399-407`, carried) or by `_longjmp` on
   Darwin is told apart from one still running; on the thread's own stack and on the alternate stack.
3. **Run**: R3's two cases 20 of 20 each, armed runs counted (`ce-flib-nofp`, `ce-fliblocale`, the
   library built as 00-shared.md says), the shared-library form, a `dlopen`ed library, the 2494 group
   (which two cases move, and to which words), `--sanitize` at `-O0` and `-O2` (does the runtime's strong
   definition stand in front of ASan's weak interceptor, as 248 found of I-onstack, and is ASan's own
   report kept), and the rows of 248's `rows.c` (`briefs/inputs/p248-fp/rows.c`) that apply.
4. **The census's point (4)**: the wrapper adds a frame between the delivery and the library's handler.
   Run bdwgc's `gcreal.c` under it (`briefs/inputs/p248-fp/`, built as 248's `phases.sh` builds it), and
   any census probe the ffi-pragmatist hands you; say what a handler that walks its own stack (a crash
   reporter's `backtrace`) sees.
5. **Darwin**: a wrapper defined in the executable misses a two-level dylib's installer (248's rows,
   `248-reports/ffi-pragmatist.md:130-136`, carried). Say what Darwin's arm of B' is (the definitions in
   the executable and the dylib residue told, or the trampoline of D-tramp, which the ffi-pragmatist
   builds), and build the executable's arm far enough to run R3's two static cases on this Mac.
6. **Cost**: `qsortcost` instructions at the base and under B', three runs each (`/usr/bin/time -l` on
   this Mac, instructions retired), and what one callback pays for the depth question.

## Question 2, candidate A and D-tramp, from the runtime's side

Read the ffi-pragmatist's report as it lands. Say, by reading and where you can by building: what an
owned marker chosen at run time (glibc's `SIGRTMIN` is a function, `__libc_current_sigrtmin`, 00-shared.md)
does to the runtime's own **27 install lines and 18 mask lines** (00-shared.md lists them), R2's poller
thread born with every signal blocked (`hero_net.c:1115-1122`), the stop's handler, `run.c`'s forwarding,
the stack guard, and `sigguard.c:139` and `panic.c:54` inside the refusal and panic paths; and whether
the probe should keep borrowing SIGURG once the marker is another signal, given a Go archive installs a
SIGURG handler of its own at constructor time (00-shared.md, Go 1.27.3 `runtime/signal_unix.go:178-182`).

## Question 1, the runtime's half of the census

For each candidate (A, B', C, D-tramp) and for the two 248 refused (I-onstack, I-mask with SIGURG): what
the runtime itself changes in what a program's own C observes on the four points of 00-shared.md, read
from the runtime's code and run where a row is in doubt; and **what a Heroes program that binds a
census library is told today** at the base (build one that links libuv, one that links a Go archive, and
one that defines `sigaction` in its library, and read what `selfhost/cli/supplied.hero` tells each:
248 measured the last, `248-reports/compiler-engineer.md:162-177`, carried).

## Question 3, from the compiler's side

1. **D-imports** (00-shared.md § Question 2, D): what reading a bound library's imports at build would
   take in the member's machinery (`supplied.hero` asks the link who supplies a name; does any step read a
   library's undefined names, and on which linkers), what it would tell, and **how many programs of the
   tree it would refuse or tell**: those that hand C a function and bind a library whose objects import an
   installer (the census seed's libuv, libcrypto, SDL3 on this Mac), counted with the command you name.
   It is a new diagnostic: say so in those words (the escape).
2. **The words**: what a refusal at run time could say that R1's does not, and what spec line 404 and
   `HERO_SIGNAL_REMEDY` would then say (00-shared.md § Found while writing: the two differ since
   `cd693eec1`). A change of words is the escape's.
3. **Whether any answer short of a route closes 2522 under R3** (*"Never a known cost"*), or needs the
   author's change to R3 (route K), said in those words.

## What your verdict owes

For each candidate: built or not, its lines per file, what moves (the compiler's own tests where
`selfhost/` moved, the `run` cases above, the `runtime` suite where `runtime/` moved, `run` whole and
`cache` alone where `flags.hero` moved, the emissions where `selfhost/emit/` moved), what it refuses or
changes that runs today, its cost in instructions or in the count you could take, every crash report its
storms left (armed runs alone counted), what it closes on Darwin (a refusal with R1's words before the
callback's first C call) and on Linux (no Heroes code inside the signal), and its residue; whether it
needs a spec sentence, a new or widened diagnostic, or a change a binding's author must learn (the
escape); and a veto, if you cast one, names the program that corrupts memory or dies in silence under
the route vetoed, built and run.
