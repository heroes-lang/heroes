# Panel 258, the shared brief: a C jump over a callback's frames, and what those frames held

**Lane: soundness** (`compiler-engineer` and `ffi-pragmatist` only). The proposal changes no surface and no spec token as
the coordinator reads it; **each seat says in its report whether (c) below needs a sentence of `spec/heroes-spec.md`**, and
a yes sends the sitting to the full panel, which this lane gives up. Convened 2026-11-24 by the author's answer *2c*
(`docs/roadmap/milestones/M-rebel-library.md`, the section of that day; decision issue 2751).

Every number, path and count below was produced by a command the coordinator ran on 2026-11-24 between 00:19 and 00:28 on
this Mac (macOS arm64, Apple clang), and the command is named with it. A seat that finds one false says so first in its
report. **No seat or critic starts a paid run, a `claude -p` session, an API call or a cloud run; none prints a
credential** (a seat that must know whether `gh` is authenticated runs `gh auth status`, never `gh auth token`).

## The proposal, in ten lines

1. A callback is a Heroes function C calls through a thunk (`selfhost/emit/callback_thunk.hero`). C may `longjmp` (or
   `siglongjmp`, `_longjmp`, `__longjmp_chk`) over the thunk's frame and every Heroes frame below it, as libpng's
   `png_error` does; a `setcontext` or a coroutine never resumed abandons frames the same way.
2. The frames abandoned held refcounted values (strings, arrays, maps, records). The jump runs no code, so **nobody
   releases them, for the rest of the process**.
3. Defect 2704's repair (`runtime/parts/callback.c`, steps 120a and 120c) counts callbacks entered and not left per thread;
   a program ending with a count above zero is **not audited at exit** for a missing decref (the leak gate,
   `hero_runtime_check_leaks`, and on Linux `--sanitize` LeakSanitizer through `__lsan_is_turned_off`). That removed a
   false *compiler bug* abort of a correct program; it left the leak whole.
4. **The leak, measured** (`/usr/bin/time -l`, `docs/panel/258-briefs/leak/main_1k.hero` and `docs/panel/258-briefs/leak/main_100k.hero`, a callback that builds a
   string of about 1 KB and an array of 50 numbers and then calls C, which `longjmp`s over it): 1,000 jumps read
   5,963,776 and 6,012,928 bytes maximum resident (two builds), 100,000 jumps read **between 307,429,376 and 352,927,744
   over eight runs, six of them 352.9 MB** (one 338,821,120, one 307,429,376: macOS compresses resident pages, so the
   low readings are the machine's, and the files differ only by blank lines the formatter added), so about 3.5 KB a jump; the program is correct
   and exits 0 (`docs/panel/258-briefs/leak/time_1k.txt`, `docs/panel/258-briefs/leak/time_100k.txt`). A server that decodes untrusted images through `libpng`
   loses that on every corrupt image.
5. **Proposal (c), to be judged**: the compiler gives each function that can run inside a callback a *frame record*, the
   addresses and kinds of its refcounted slots, linked on a thread-local chain that the thunk starts; where the runtime
   sees a jump it walks the chain from the jumping frame down to the target's stack pointer and **releases what those
   frames held**, then pops the records; the audit is on again for such a program.
6. The three answers the author was put: (a) keep the audit off, the leak whole (the status quo and **the interim**);
   (b) audit by subtraction, which leaves the leak whole and needs the knowledge that would release the blocks, so it is
   dominated by (c) and is not a candidate; (c) as above. The author chose to keep (a) as the interim and to convene
   this sitting on (c).
7. **A hazard the coordinator names and does not resolve**: a jump may be made from inside a signal handler (defect
   2704's second golden: a handler that `siglongjmp`s out of the function the program gave C), where `free` and an
   allocator's locks are not async-signal-safe; and the abandoned frames' stack memory is reused by the code that lands.
   So *where* and *when* the release runs, and whether the hook may only copy the slots' values out and release later at
   a safe point, are the sitting's questions, not details.
8. Where the runtime cannot see a jump (Windows; hand-written assembly; `__builtin_longjmp`; a C++ exception through C
   frames, none of it checked by the coordinator) the interim rule stays.
9. What the sitting returns: a verdict per seat, **the most robust resolution** with the condition a veto would compel,
   what the conservative one would have been, and a prediction each seat can be scored on.
10. The resolution is provisional until the author reads it; work proceeds on the interim.

## What the coordinator read, each with its command

- `runtime/parts/callback.c` is 86 lines (`wc -l`); `hero_callbacks_inside` is a `_Thread_local int64_t` at line 61;
  `hero_callback_left` at line 63; `__lsan_is_turned_off` at the end of the file.
- **The jump hook exists on Linux and every non-Darwin POSIX system**: `runtime/parts/sigjump_save.c` guards its whole
  body `#if !defined(_WIN32) && !defined(__APPLE__)` (line 25, `grep -n '^#if'`), and `longjmp`, `_longjmp`, `siglongjmp`
  and `__longjmp_chk` there each call `hero_jmp_find()`, then `hero_jmp_jump(env)`, then the real function
  (`sed -n 126,140p`); `hero_jmp_jump` decodes the target's stack pointer (`hero_jmp_sp_of`) **before** the jump, so the
  abandoned frames are still on the stack when it runs. Darwin's route is `runtime/parts/rebind_jump.c` (its module doc
  names it route D-rebind). No file of `runtime/platform/*win*.c` holds `longjmp` (`grep -l -i longjmp`), which is a
  question about Windows, not a premise.
- The thunk's first line is `hero_callback_entered("<name>", __builtin_return_address(0), __builtin_frame_address(0))`
  and its line after the call is `hero_callback_left()` (`docs/panel/258-briefs/leak/main_1k.emitted.c`, lines 212 to 218, from
  `heroes build main_1k.hero --emit-c` with the tree's stage 2 compiler at `9ef40517f`); the runtime therefore knows the
  thunk's frame address and its return address. `hero_callback_entered` has two definitions in
  `runtime/parts/sigguard.c`, lines 573 and 636.
- **A frame's owned values are C locals declared and zeroed in the prologue and released one by one before the return**:
  in `docs/panel/258-briefs/leak/main_1k.emitted.c` line 189 the callback's body declares `HeroStr h1_word = {0}; HeroArrayHeader * h2_nums =
  {0}; ...` and its `own` slots `h6_own6`, `h10_own10`, `h11_own11`, and lines 199 to 203 call
  `hero_str_release_at(&h1_word); hero_array_release_at(&h2_nums); ...`. The prologue of that function calls
  `hero_thread_guard("main1k.body")` (`runtime/parts/thread.c:141`) and **registers none of its slots**: that is the
  fact (c) changes. The rules page `.claude/rules/generated-c.md` says a refcounted slot is zero-initialised so cleanup is
  unconditional, which is what makes a slot's state "owned or zero" at a call site, to be checked by a seat, not assumed.
- The cost of the interim, from defect 2704's closing text (issue 2704 on GitHub, measured by its lane on
  a shared Mac): 100 million calls of a trivial callback at `-O2`, six runs each, 0.67 to 0.69 s before and 0.83 to
  0.85 s after, about +1.6 ns a callback (two thread-local stores). A frame record on every function entry has not been
  measured by anyone.
- Evidence programs and goldens that exist: `tests/golden/run/fixedbugs-2704-*` (three cases: a `longjmp` over a
  callback holding a `str`, a handler that `siglongjmp`s out of the function the program gave C, the same on a thread the
  program started) with `fixedbugs-2704-longjmp.h` and `fixedbugs-2704-handler.h`; `libpng` 1.6.59 is installed on this
  Mac (`pkg-config --modversion libpng`); panel 257's `pngread` is in `docs/panel/257-reports/evidence.tar.xz`.
- This Mac holds itself awake (`pmset -g assertions`: Amphetamine, PreventUserIdleSystemSleep 1); it is loaded by about
  ten other agents, so **a build is slow and a timing is noise unless the ratio `real` to `user` plus `sys` says
  otherwise** (`.claude/rules/verification.md` § The compiler that judges, § What may run beside a gate).

## How a seat works

- **Your own copy, one seat's**: `git worktree add --detach .claude/worktrees/258-<seat> <the base commit the
  coordinator's prompt names>` from `/Users/joseph/Temp/heroes/heroes-lang`; build your own compiler there from the seed
  (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`), then stage 2 (`./heroes build selfhost/main.hero -o
  heroes-next`) if you prototype in `selfhost/`; **never use the trunk's compiler or another seat's**, never read,
  build or run inside another seat's folder. `rm -rf build` after any copy. Linux arm64 (glibc) is the container
  `heroes-linux-arm64:latest` (`docs/platforms/linux/LINUX-MACHINE.md`); remove any container you start.
- **Write your report as you go** to `docs/panel/258-reports/<your-seat>.md` of your own copy, and copy it to the same
  path of `/Users/joseph/Temp/heroes/heroes-lang` when you finish or when you stop for a limit, so a killed seat leaves what
  it had. **Never commit and never edit the trunk's tracked files**: the coordinator commits the sitting.
- **Contract**: your verdict (approve, approve with a condition, object, veto), the section it rests on, the cost it
  names, **one falsifiable prediction** with the milestone or the measurement at which it is checkable, and every place the
  coordinator's brief was false or silent. Say what you ran and what you read, apart. A seat's veto is a refusal, never a
  price.
- Never `pkill` by name; stop a process you started by its PID.
