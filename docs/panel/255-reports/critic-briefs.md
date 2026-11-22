# Panel 255, the completeness critic's first reading: the briefs, before any seat

Written 2026-11-22 as I went, by the completeness critic (no seat, no verdict on the route). Everything
below was run in my copy, `.claude/worktrees/p255-scratch/critic/` (written `C/` below), a detached
worktree at `a75179641` (`git rev-parse HEAD`: `a7517964102a395ce8719ab2a30ed7dbb8d91aa7`;
`git status --short | wc -l`: 0 before I wrote `C/p255/`), `TMPDIR=C/p255/tmp`, unless a line says
otherwise. Read whole: `00-shared.md`, `2679-question.md`, `compiler-engineer.md`, `ffi-pragmatist.md`,
`critic.md` and every file of `repro/`. My files: `C/p255/` (`probe/`, `shape7/`, `musl/`, `repro/`,
`linux.sh`, `linux-run2.log`).

## Findings

### 1. The brief's "carried, and not on disk": the OUTPUTS are on disk, and `ijstale.c` is in the tree

`00-shared.md` § 2 says lane-core's files are not on disk and every measurement of the question's table
and of option C's paragraph is *carried, unreproduced*. Re-run, the negative is a fact about the
search's names, not about the disk:

- `ls .claude/worktrees/lane-core-scratch/2679` answers *No such file or directory* and `git branch
  --list '*core*'` prints nothing: confirmed. `find .claude/worktrees/*-scratch` for `probe_mangle*`,
  `ijstale*`, `linux-x86-probe.log`: none, confirmed (the only `linux-arm64.log` it finds is the
  coordinator's own, `p255-scratch/coord/linux-arm64.log`).
- But `find .claude/worktrees/rebel-scratch -maxdepth 3 -iname '*2679*'` names
  **`.claude/worktrees/rebel-scratch/logs/lane-core-2679.log`** (7,519 bytes, `ls -la`: 14:37 today),
  lane-core's own transcript of `repro.sh` on Linux arm64, `probe_mangle.c` on Linux x86-64 and
  `mac/run.sh` on this Mac. It holds the probe's printed lines: arm64 (Debian GLIBC 2.41-12+deb13u3,
  clang 22.1.8) *"slot 13 key agrees across two frames (sps 0xfffff8eeacf0 0xfffff8ee9cc0), 1 slot(s) of
  22 agree"* then *"slot 10 ... slot 13 ... (sps 0xfffff59a4e90 0xfffff59a4e70), 2 slot(s) of 22
  agree"*; x86-64 *"slot 1 ... slot 6 ... (sps 0x7ffffffc7928 0x7ffffffc6908), 2 slot(s) of 8"* then
  *"slot 6 ... 1 slot(s) of 8"*; and the shapes' runs, the same frames as the coordinator's table.
- What the log does NOT hold: the transformation. Every line says only *"key agrees"*; *"by `xor` on
  arm64"* and *"`ror 17` then `xor` on x86-64"* are the question's words with no output behind them.
  And the second slot that agreed on arm64 did so for two stack pointers **0x20 apart**
  (`0xfffff59a4e90`, `0xfffff59a4e70`): two calibration frames close together are exactly what lets a
  wrong slot agree. Q1 should be told so (calibrate at frames far apart, and say how far).
- `ijstale.c` is in the tracked tree: `git ls-files docs/panel/251-reports/` lists
  `evidence.tar.xz`, and `tar -tJf` of it names `251-compiler-engineer/q1/ship/ij/ijstale.c` beside
  `ijv.c` (three ways a handler jumps WITHIN itself, a nested signal among them), `ijo.c`
  (`SA_ONSTACK`) and `ijn.c` (the control), all built *"-O2 -fomit-frame-pointer"* or
  *"-fno-omit-frame-pointer"* by their own headers (`C/p255/ev251/`, extracted). These are the shapes
  of the compiler-engineer's own 251 condition (*"the stale and `memcpy`-within shapes ... the
  `memcpy`-out one"*, `251-reports/compiler-engineer.md:587-590`, read). The golden cases run at `-O0`,
  `-O2` and both sanitized (`tests/harness/suite_run.hero:1-35`), but `ijv.c`'s third way, a SIGUSR2
  handler that `siglongjmp`s back into the still-running SIGUSR1 handler, has no golden twin
  (`grep -l SIGUSR2` over the 2522, 2558 and 2571 files: none), and it is the shape where a decoded
  stack pointer must keep the outer handler's entry. **No seat's brief points at the archive.**
- `diff rebel-scratch/coord/2679-question.md lane-p255/docs/panel/255-briefs/2679-question.md`: no
  output, the copy is verbatim.

### 2. Darwin's arm reads given-back memory at EVERY jump, handler or not: a seventh shape, measured

The coordinator saw it (*"Darwin's `hero_rebind_jump` calls `hero_rebind_settle` at `rebind.c:425`
before it asks whether any handler is live"*) and built shapes 5 and 6, which jump INSIDE a handler.
Read: `rebind.c:424-427` settles first and returns at `now == NULL` after; `sigjump.c:186-189` returns at
`own->depth == 0` before it reads. So on Darwin a plain `longjmp` with no handler anywhere, libpng's own
`png_error` path, is a read site; on Linux it is not. I wrote it (`C/p255/shape7/shape7.h`: a static
buffer saved, a heap or mapped buffer saved and given back, then `longjmp` to the static one, no
handler) and ran it on this Mac with my stage 1 compiler (`run7.sh`, `mac7.log`):

| shape 7 | Mac plain | Mac `--sanitize` |
|---|---|---|
| `plain_heap` | exit 0 | **134**, heap-use-after-free, `#0 hero_rebind_fingerprint rebind.c:334`, `#1 hero_rebind_settle rebind.c:371`, `#2 hero_rebind_jump rebind.c:425`, `#3 hero_rebind_longjmp rebind.c:478` |
| `plain_mmap` | **139, SIGSEGV** | 134, SEGV, the same four frames |

On Linux arm64 (`heroes-linux-arm64:latest`, stage 1 from the seed in the container, `C/p255/linux7.sh`,
`C/p255/shape7/linux7.log`) all four runs are **exit 0**, printing `plain_heap: 1` and `plain_mmap: 1`,
plain and `--sanitize`: the arms differ, as the code says. Consequences for the briefs: the six
shapes do not hold the one shape that separates the two arms; a Darwin route that only repairs the
handler path would leave this; and route C on Darwin would need its jump to read nothing at depth 0
(Linux's order), which neither brief asks.

### 3. The platforms the resolution reaches: glibc only, six architectures by the code, two measured

- **musl does not compile the runtime at all.** In `postgres:16.15-alpine` (local, `arm64`, musl's
  `/lib/ld-musl-aarch64.so.1`) after `apk add clang musl-dev linux-headers` (Alpine clang 22.1.3),
  `clang -I runtime -c runtime/runtime.c` exits 1 with three errors, all `runtime/parts/siglookup.c`:
  `87:28` undeclared `dlvsym`, `173:9` undeclared `dladdr1`, `173:47` undeclared `RTLD_DL_LINKMAP`; the
  seed build exits 1 on the same three (`C/p255/musl/musl.log`). And `siglookup.c:87` asks for
  `dlsym` at version `"GLIBC_2.34"`. So the question's *"musl (unmangled, the same slots by its source)
  is unrun here"* and § 7's *"a route's behaviour on an unknown C library is judged by what it does when
  its premise fails there"* describe a platform no route reaches; the non-Darwin POSIX arm
  (*"every POSIX system but Darwin"*, `sigjump.c:12`) is glibc's in fact. Seats should not spend on it,
  and the synthesis should not count it as a premise failure handled.
- **But glibc on six architectures**: `siglookup.c:68-79` gives a first `dlsym` version for
  `__x86_64__`, `__aarch64__` / `__powerpc64__`, `__riscv`, `__arm__`, `__s390x__`. Route C's
  calibration *"verifies a known slot per C library and architecture"*; the seats will know two. On the
  other four the question's own rule makes *"every jump inside a handler keep every entry"*: every correct
  jump out of a handler that today's fingerprint follows would be refused there (by reading; unrun, no
  such machine here; whether Docker's emulation can run `linux/riscv64` was not tried). The option set
  has no line for them (finding 9).
- **The CI's Linux is not the containers' glibc.** `ci.yml:373`: the legs are `ubuntu-26.04` (x86-64)
  and `ubuntu-26.04-arm`, `macos-26`, `windows-latest`; § 7 says the Linux leg is `ubuntu-latest`. Both
  local Linux images print `ldd (Debian GLIBC 2.41-12+deb13u3) 2.41` (my run; the `gtk4-ui` image
  `deb13u4`). The only Ubuntu image here is `ubuntu:24.04` (`docker image inspect`: `arm64`). So C's
  premise is measured on Debian's glibc 2.41 and first meets Ubuntu 26.04's on the CI after the push,
  unless a seat pulls `ubuntu:26.04` (native arm64 here) and runs the probe there (unrun).
- **Darwin**: this Mac is macOS 27.2.1, build 26B131 (`sw_vers`); the CI's Darwin leg is `macos-26`.
  The decode the ffi-pragmatist measures is this OS's `libsystem_platform`; panel 251's R1 already lists
  *"the run on `macos-26` that answers the historian's TPRO question"* as owed (`sed -n 222,238p` of the
  251 file). And `rebind.c` has a Darwin **x86-64** arm (`#elif defined(__x86_64__)`) with no CI
  leg and no instrument here: `clang -arch x86_64` builds, `arch -x86_64 ./untouched-x86` answers *"Bad
  CPU type in executable"* (no Rosetta). Darwin x86-64 is judged by its premise's failure only, and the
  briefs do not say so. (`grep -n '__x86_64__' rebind.c`: 517, the `makecontext` wrapper's x86-64 arm.)
- **Windows**: `sigguard.c:174-180` (*"none on Windows"*), so the defect and every route stop there.
  True, and unsaid in the briefs; one line would close the question for the reader.

### 4. A route nobody listed: a token in a span no save point writes, read only at the jump

What would have to be true for a route that reads no given-back memory, needs no record of a fill, and
reaches Darwin without decoding a signed stack pointer: the save point writes an identifier into a span
of the buffer the C library's save never writes (the buffer lives at that moment: the caller is handing
it to the C library), and a jump reads that span of its TARGET only (it lives: the C library reads it
next), taking the record the identifier names. A `memcpy` of the whole buffer carries its source's
identifier, so the refill case names the handler's own save and stays refused, and the copied-buffer
case names its source and would run, as under C. **Its first premise measured** by
`C/p255/probe/untouched.c` (fill with `0xA5`, then `0x5A`, save, read which bytes changed; plain clang,
so the C library's own saves):

| C library | buffer | written by `setjmp` / `_setjmp` / `sigsetjmp(,0)` / `sigsetjmp(,1)` | never written by any of the four |
|---|---|---|---|
| Darwin arm64, this Mac, `-O0` and `-O2` | `jmp_buf` 192, `sigjmp_buf` 196 | `[0,104) [112,180) [188,192)` / `[0,104) [112,176)` / `[0,104) [112,176) [184,188)` / `[0,104) [112,180) [184,192)` | `[104,112)`, `[180,184)` |
| glibc 2.41 arm64, `heroes-linux-arm64:latest`, `-O0` and `-O2` | 312 and 312 | `[0,96) [104,180)` for three, `+[184,192)` for `sigsetjmp(,1)` | `[96,104)`, `[180,184)`, `[192,312)` |

The rest is unmeasured and is what a seat would have to settle: (a) that no jump of the C library reads
or checks that span (Darwin's `_longjmp` disassembly; glibc's `__longjmp_chk`); (b) that every copy a
program makes copies the whole buffer, since a partial copy leaves an old identifier over new
registers, the dangerous side; (c) that an identifier cannot collide with a stale one (a generation per
thread); (d) contexts (`ucontext_t`'s spare fields); (e) above all, whether the runtime may WRITE into
memory the program handed the C library, a soundness question for both seats' vetoes. I do not argue
for it; it is in no brief and it answers the one thing C cannot on Darwin.

### 5. Two more routes the set does not name

- **The composition by architecture**, *C where the calibration verifies, A where it does not*: the
  question pairs C with A by platform (*"C on Linux, A on Darwin"*), never by calibration outcome, and
  its fallback for a C that does not verify is the told refusal of every jump out of a handler, which is
  below A. On the four unmeasured glibc architectures of finding 3 that difference is the whole
  outcome. Whether the fallback is A or the refusal is a question the sitting should answer.
- **F in assembly.** § 5 carries lane-core's *"not available"*. What would have to be true: a save point
  that is not a tail call yet leaves the caller's frame as the C library must see it. On arm64 a `bl`
  writes only `x30` and leaves `sp` as the caller left it; on x86-64 a `call` moves `rsp` by 8. The
  difficulty is where the caller's return address and the registers the stub uses live across a later
  jump, which the C library restores from the buffer and nowhere else. Q6 asks the compiler-engineer to
  judge F "from the code"; the brief should name this shape so the judgement is about it, not about a C
  `musttail`.

### 6. Coverage: what neither seat builds or runs, per route and platform

By reading the two briefs' questions (CE the compiler-engineer, FP the ffi-pragmatist):

| route | Linux arm64 | Linux x86-64 | Darwin arm64 | other |
|---|---|---|---|---|
| today | CE: shapes, cases | — | FP: shapes, cases | — |
| C | CE: shapes, cases; FP: own C, libpng | CE: probe and six shapes only; **no case** | FP, if the decode holds | glibc on 4 more architectures: nobody (finding 3) |
| A | CE: shapes, cases, Q3 | **nobody** | FP: shapes, cases | — |
| token, composition, F-in-assembly | nobody | nobody | nobody | — |

And inside the briefs:

- **libpng under A is run by nobody.** FP Q3's first bullet asks libpng *"under today's runtime and a C
  of your own on Linux"*; A, the conservative arm and the proposed Darwin route, is not named. libpng
  is present on both (`pkg-config --modversion libpng`: 1.6.59 on this Mac; 1.6.48 in
  `heroes-linux-arm64:gtk4-ui`, with `libjpeg`).
- **FP Q3's second bullet is unbuildable as written**: *"run it under A and under C on both platforms
  you can"*, while Q2 gives the ffi-pragmatist A on Darwin, C on Linux, and no seat may read another's
  folder. Either it builds an A of its own on Linux (and says so) or the bullet narrows.
- **Lua is present nowhere**: FP Q5's *"Lua's `lua_pcall`, where present"*; `pkg-config --list-all`
  names no Lua on this Mac, and the `gtk4-ui` image's `pkg-config` and `/usr/lib/aarch64-linux-gnu/`
  name none. Say it is absent, or say what to install.
- **The x86-64 cases under C**: C's x86-64 decode is the less simple of the two (the question's `ror
  17`), and the first time any 2522/2558/2571 case runs under it would be the CI's leg after the push.

### 7. Cases and suites the routes move that no seat is told to run

Both briefs name `fixedbugs-2522`, `-2558`, `-2571` (34 cases: `ls | wc -l` 25, 5, 4, confirmed).
Outside them, by `git grep -l -E '(setjmp|longjmp|getcontext|swapcontext|setcontext|makecontext|jmp_buf)'`
and `grep -l -E 'sigaction|signal\('` over `tests/golden/run/*.h`:

- `tests/golden/run/fixedbugs-a-c-handler-that-recovers-leaves-no-line` (its header's lines 19, 22, 36,
  47, 51: a handler that `longjmp`s / `siglongjmp`s out of `abort` and a trap through a static
  `recover_jump`): the canonical correct jump out of a handler, through every route's changed jump.
- 18 more headers installing handlers (`fixedbugs-2494-*` nine, `2257`, `2583` two, `2496`, `2503`,
  `an-ignored-signal-stays-ignored`, `the-platform-layer-stop`), which go through the wrapper whose
  entries every route prunes.
- The `runtime` suite holds a census of the runtime's statics, each with its reason
  (`tests/harness/suite_runtime.hero:383` names `rebind.c`'s `hero_rebind_records`): a route that adds
  a calibration key, an image-segment table or an identifier generation, or removes the record, moves it.
- `runtime/parts/siglookup.c:134-145`, the `dlsym` answers for the same ten names: CE Q2's *"what
  removing them moves"* points at `selfhost/cli/supplied*.hero` only (12 files, `ls`).

### 8. CE Q3's sanitizer question, as worded, measures the shape that never moves

Q3 asks whether *"a local `jmp_buf` of a function"* lives on ASan's fake stack under `--sanitize`. The
runtime sets no ASan option (`git grep` of `asan_default_options|detect_stack_use_after_return` in
`runtime/` and `selfhost/`: none; `selfhost/cli/symbolized.hero:25` keeps a user's `ASAN_OPTIONS`).
Measured with `C/p255/probe/fakestack.c` (the buffer local to the function that calls `setjmp`) and
`fakestack2.c` (the buffer held by a caller that does not save; a callee saves into it):

| | saving function's local | held by a non-saving frame |
|---|---|---|
| Linux arm64, Debian clang 22.1.8, default options | ON the thread's stack | **OFF the thread's stack** (`0xfbff8d9f0020`) |
| the same, `detect_stack_use_after_return=0` | | ON |
| the same, `=1` | ON | |
| Darwin, Apple clang 21.0.0, default | ON | ON |
| Darwin, `detect_stack_use_after_return=1` | ON | **OFF** (`0x103561020`) |

So under A on Linux, a correct jump out of a handler through a buffer a caller holds and a callee
saves into has no record **under `--sanitize` alone, by default**; on Darwin, only where the user's
`ASAN_OPTIONS` asks for use-after-return. Q3 should name the second shape; the ffi-pragmatist's Darwin
A is not asked it at all.

### 9. The question the sitting should ask: may one program's verdict depend on the platform?

Under the recommendation (*C on Linux, A on Darwin*), the copied-buffer case runs on Linux and is refused
on Darwin, and a heap or thread-local buffer jumped out of a handler is followed on Linux and refused on
Darwin. The run harness has one `.expected` per case (`ls tests/golden/run | sed -E 's/^[^.]*//' | sort
| uniq -c`: 920 `.hero`, 920 `.expected`, no per-platform expectation) and skip lists for Windows,
`elsewhere` and `darwin_alone` only (`grep -n '^function ' tests/harness/platform_skips.hero`), no list
that skips a case on Darwin. So the split needs a harness change, and before that a ruling: a program
whose verdict differs by platform. `grep -n -i -E 'every platform|all (three )?platforms|portab'
docs/design.md` found no rule either way (a question, not a premise). Neither brief asks it.

### 10. Two readings of the code a seat should have before it builds

- **`hero_sig_on_own` lumps the alternate stack and any third stack** (`siginstall.c:144`): `here` and
  `there` are only *own* or *not own*, so `hero_sig_prune` (`siginstall.c:150-160`) compares an entry on
  the alternate stack with a target on a `makecontext` stack by address, as one stack (`there == here
  && w <= at`). CE Q2's third-stack case should hold both orders of those two addresses; under C the
  decoded stack pointer feeds this comparison directly. By reading; unrun.
- **`swapcontext` fingerprints its `from` before the C library fills it**, on both arms: Linux
  `sigjump.c:278-279` notes `oucp` then `hero_jmp_leave(ucp)` reads every pending pair but `ucp`'s, so
  `oucp` is read pre-fill (inside a handler only, depth > 0); Darwin `rebind.c:465-466` notes `from`
  then `hero_rebind_jump` settles it pre-fill at every swap. A later jump back to that context inside a
  handler then reads a mismatch and is refused. By reading; unrun. CE's third-stack case under today's
  runtime and FP's coroutine shape may be refused for this cause, not the one they test: the seats
  should be told to tell the two apart.

### 11. Facts re-run and confirmed (carried by no seat as a premise beyond these)

- `spec/heroes-spec.md` grep: lines 427 and 428, the rule only. Panel 251 § R1 (`grep -n '^### R1'`:
  222) says *"No spec token"*. Its prior art names *"ThreadSanitizer's `setjmp` records
  (`tsan_interceptors_posix.cpp`, `tsan_rtl_amd64.S`)"* (`docs/panel/251-*.md:291`), not
  `ExtractLongJmpSp` by name, which stays the question's word.
- Issue 2679 (`gh issue view 2679 --repo heroes-lang/heroes --json ...`): OPEN, `kind:defect`,
  `class:blocking`, `area:runtime`, created 2026-11-22T12:07:20Z, 0 comments.
- Line numbers, all as § 4 says: `sigjump.c` 283 lines, 69, 135, 138, 144, 148, 156, 185, 187, 189,
  212-238, 243-281, 10-67, 43-46, 56; `sigguard.c:317` (`HERO_SIG_JUMPS 32`), 320-340; `siginstall.c:150`;
  `rebind.c` 727 lines, 52-53, 301, 304, 318, 330, 334, 363, 371, 377-378, 424-425, 721. The ten names:
  `sigjump.c:24-27`.
- The twelve `a-callback-after-*` cases extern `fixedbugs-2522-a-jump-out-of-a-handler-then-a-correct-callback.h`,
  which holds `static sigjmp_buf depth_sig_env` (:20) and `static jmp_buf depth_env` (:21). The one
  header naming `malloc|_Thread_local|__thread|dlopen|mmap` is `...-past-the-runtimes-names.h`.
- **A small count correction**: the grep of § 4 prints 7 static buffers and 5 non-static matches, not
  4: `sigjmp_buf env` and `jmp_buf env` are function PARAMETERS
  (`fixedbugs-2522-a-handler-of-cs-jumps-and-its-deep-callback.h:34, 36`), so 3 locals (`:47, 50, 53`)
  and 2 parameters. The conclusion (static or stack) stands.
- `fixedbugs-2522-a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused` and the copied-buffer
  case exist, with their `.expected`.
- Stage 1 in my copy: `heroes 0.6.0`, `sources 05186f2b38e77a97`; the hook refused my
  `./heroes run tests/harness/main.hero -- ./heroes run ...` naming the tree's key `61416c0c33ecc5a9`;
  `heroes doctor` from `C/p255/repro/`: `ok runtime C/runtime`. `/usr/bin/time` is absent in
  `heroes-linux-arm64:latest` (*"No such file or directory"*), so a seat timing inside it uses the
  shell's `time`.
- `.github/workflows/`: `grep -rn -i 'musl\|alpine'` prints nothing; the `only` input's four legs.
- The six shapes, re-run with my stage 1 compiler: **this Mac and Linux arm64 both match § 3's table
  cell for cell** (exits, first sanitizer line, the two frames; `C/p255/repro/mac.log`,
  `C/p255/linux-run2.log`); `sizeof(jmp_buf)` and `sizeof(sigjmp_buf)` 312 on Linux arm64. § 3's
  *"not built"* for `context` on Darwin means built, printing `context: -2` at exit 0.

### 12. Today's verdicts of the three case sets are stated for one set on one platform

§ 9 gives `fixedbugs-2571` on this Mac (4 passed, 0 failed). Nothing states today's count for
`fixedbugs-2522` or `-2558` on either platform, or any set on Linux; a seat naming *"a case whose
verdict moves"* needs the baseline its route moved from, read on the same platform. My Mac baseline,
stage 2 in my copy (`./heroes build selfhost/main.hero -o heroes-next`, exit 0, `real 209.90`, `user
151.63`, `sys 43.43`, beside other lanes' work; `--version` then `sources 61416c0c33ecc5a9`, the
tree's key), the three sets in sequence into `C/p255/h2571.log`, `h2558.log`, `h2522.log`:

| set (this Mac, today's runtime) | count line, read whole from the file | exit |
|---|---|---|
| `fixedbugs-2571` | `run (only fixedbugs-2571): 4 passed, 0 failed` | 0 |
| `fixedbugs-2558` | `run (only fixedbugs-2558): 5 passed, 0 failed` | 0 |
| `fixedbugs-2522` | `run (only fixedbugs-2522): 25 passed, 0 failed` | 0 |

(zsh's `time`: 2571 `99.01s user 19.37s system ... 2:11.34 total`; 2558 `37.58s user 12.43s system ...
59.854 total`; 2522 `198.23s user 89.59s system ... 5:51.92 total`, beside other lanes' work, indicative
only.) Linux arm64's baseline stays unrun by me; the compiler-engineer's container run of today's
runtime is the one that gives it, and the brief should say it is the baseline.

### 13. The contract amended today, and the lane chosen on the spec's silence

`CLAUDE.md:106-109` in this tree (commit `691531908`, *"Contract: the most robust solution always wins,
a new form and the spec's size included (author, 2026-11-22)"*): *"the most robust solution now always
wins, a new form included and at the cost of the spec's size: Principle 0, the spec's budget (design.md
§1.6) and the spec-warden's vetoes yield to it"*. `00-shared.md` § 6 cites rank 3 and § 4 and not this.
The lane was chosen because the spec states no mechanism (§ 1), which holds for C. But A adds correct
programs refused (heap, thread-local and late-image buffers; finding 8's sanitizer-only one), and the
token route of finding 4 a partial-copy residue: whether either owes the spec a sentence telling a
program's author is a spec question, and the soundness lane has no seat that writes or prices one,
while the amendment removed the budget reason not to. The sitting should say whether any route it may
adopt owes the spec words; if one does, the lane is short a seat for it.

## What the coordinator must repair before any seat starts

1. **Widen the option set and give each new route to a seat** (findings 4, 5): the token in a span no
   save writes, read only at the target of a jump (its first premise measured on both C libraries; it
   is the one route besides C that reads no given-back memory, and the only one that may reach Darwin
   without decoding a signed stack pointer; its soundness question is the runtime WRITING into the
   program's buffer); the composition *C where the calibration verifies, A where it does not*, against
   C's stated fallback of refusing every jump out of a handler; and F written in assembly (arm64's `bl`
   leaves `sp` alone), so Q6's judgement of F is about that shape.
2. **Add shape 7 to `repro/` and to both seats' lists** (finding 2): a give-back, then a jump OUTSIDE
   every handler, which on Darwin faults (139 plain on the mapped page, heap-use-after-free under
   `--sanitize`, through `hero_rebind_jump rebind.c:425`) and on Linux is clean (exit 0, four runs).
   Tell the ffi-pragmatist that a Darwin route must remove the read at depth 0 too.
3. **Correct § 2's "not on disk" and point both seats at what is** (finding 1):
   `.claude/worktrees/rebel-scratch/logs/lane-core-2679.log` (the slot outputs; the `xor` and `ror 17`
   are NOT in it; a wrong slot agreed for frames 0x20 apart, so Q1 calibrates far apart), and
   `docs/panel/251-reports/evidence.tar.xz`'s `251-compiler-engineer/q1/ship/ij/` (`ijstale.c`, `ijv.c`
   with its nested-signal way that no golden case holds, `ijo.c`, `ijn.c`).
4. **Correct the platform frame** (finding 3): the non-Darwin arm is glibc's (musl does not compile the
   runtime, `siglookup.c:87` and `:173`, measured), so drop musl as a premise-failure case; the CI's
   Linux legs are `ubuntu-26.04` and `ubuntu-26.04-arm`, not `ubuntu-latest` and not Debian's glibc
   2.41, and its Darwin leg `macos-26`, not this Mac's 27.2.1; name what C's fallback gives the four
   other glibc architectures `siglookup.c:68-79` names, and Darwin x86-64's arm (no instrument here);
   say Windows has no record (`sigguard.c:174-180`). Decide whether a seat runs C's probe on
   `ubuntu:26.04` before the push or the CI's legs are its first judge, and say which.
5. **Close the coverage holes of finding 6**: libpng under A (both platforms; present on both); the
   ffi-pragmatist's *"under A and under C on both platforms"* made buildable (its own Linux A, or
   narrowed); A on x86-64 and C's 34 cases on x86-64, or said to be the CI's first run; Lua said
   absent on this Mac and in the images.
6. **Put the platform-split question to the sitting** (finding 9): under *C on Linux, A on Darwin* one
   correct program runs on one and is refused on the other, and the run harness has one `.expected` per
   case and no list that skips a case on Darwin; the sitting rules on the split before it adopts it.
7. **Say whether any route owes the spec a sentence** (finding 13), under today's amendment of
   § Precedence; if one does, the soundness lane is short the seat that writes and prices it.
8. **Re-word the compiler-engineer's Q3 and give the ffi-pragmatist its Darwin twin** (finding 8): the
   buffer that moves to ASan's fake stack is one held by a frame that does not save (OFF by default on
   Linux arm64), never the saving function's local (ON even at `detect_stack_use_after_return=1`); on
   Darwin it moves only under the user's `ASAN_OPTIONS`.
9. **Add to both run lists** (finding 7): `fixedbugs-a-c-handler-that-recovers-leaves-no-line`, the
   cases of the 18 other handler headers, the `runtime` suite (its census of statics), and, for the
   compiler-engineer's Q2, `siglookup.c:134-145`'s answers for the ten names.
10. **Hand the compiler-engineer the two code readings of finding 10** (`hero_sig_on_own` lumps the
    alternate stack with any third stack; `swapcontext` fingerprints `from` before the fill on both
    arms), so its third-stack case and the ffi-pragmatist's coroutine shape tell their causes apart.
11. **State today's verdicts of the three case sets per platform** (finding 12) as the baseline a
    "moved" is read against; mine for this Mac are in finding 12.
12. **The small corrections of finding 11**: 3 locals and 2 parameters, not 4 locals; *"not built"*
    means built and printing `context: -2`; R1's prior art names TSan's `setjmp` records, not
    `ExtractLongJmpSp`; `/usr/bin/time` is absent in `heroes-linux-arm64:latest`.
