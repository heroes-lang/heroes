# Panel 263, the completeness critic's third reading: route (b'') as built, before the synthesis's amendment

Written as it went, 2026-11-27 from about 23:05, a fresh agent, in the detached worktree
`.claude/worktrees/263-critic3` (`git worktree add --detach .claude/worktrees/263-critic3 lane-bpp`, HEAD `2dd6d086f`,
217g), scratch `.claude/worktrees/263-critic3-scratch/`. Read whole, in this order: the brief
(`.claude/worktrees/rebel-scratch/coord/panel263-critic3-brief.md`), lane bpp's report `bpp-build.md`, the sitting on
`main`, my second reading `critic-reports.md`, the ffi-pragmatist's, the spec-warden's and the compiler-engineer's
reports; `runtime/parts/cbstack.c`, `selfhost/emit/thunk_text.hero` and `callback_door.hero` at `2dd6d086f`; lane bpp's
run files under `.claude/worktrees/lane-bpp-scratch/` (read only, nothing built or run there). No sub-agent, no paid
run, no commit, push, merge or GitHub write, no credential printed, no download or `docker pull`, nothing outside the
repository's root, no `pkill`. **Ran** and **read** are kept apart; I give no verdict on any route.

## 0. The ground (ran)

- **(b'')'s compiler**: in my copy, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`real 16.16`), then
  `./heroes build selfhost/main.hero -o heroes-next` (`real 191.00 user 144.07`), **`sources 185600f90a609b3c`**. The
  seed is `main`'s (lane bpp did not regenerate it), so stage 2 is the compiler whose emitter writes (b'')'s thunks and
  doors.
- **The base's compiler**: `git archive 8e818cf9c seed runtime selfhost vendor` into `263-critic3-scratch/base/`, the
  commit lane bpp branched from (`git merge-base main lane-bpp`), the seed's compiler (`real 12.64`) and its stage 2
  (`real 156.86`), **`sources 692ac54673e38215`**. `main` has since moved to `b9b8b83cb`, by step 199r and merges:
  `git diff --stat 8e818cf9c b9b8b83cb -- runtime selfhost seed spec` reads 4 files of the signal guard's
  (`rebind.c`, `rebind_entries.c`, `sigentries.c`, `sigguard.c`), +71 −37, and no `selfhost/`; I took the lane's base,
  so every difference below is (b'')'s alone. Every program is built by `heroes build` with `HEROES_RUNTIME` naming
  that compiler's own `runtime/` (`bin/b4.sh`, the run form's four builds).
- Load 4 to 8 on 8 cores at the start (`uptime`), two other sessions' containers up (`unw`, `sf3`, never touched). **No
  duration below is judged**; costs are instructions retired.

## 1. The ffi-pragmatist's veto: `csh.hero` and the seat's other programs (ran)

**This Mac** (`out/csh-mac.txt`, three runs each, `bin/r1.sh`, `timeout -k 5 20`):

| `csh.hero`, the seat's own | `-O0` | `-O2` | `--sanitize -O2` | `--sanitize -O0` |
|---|---|---|---|---|
| base | `both: 10022002`, 3 of 3 | the same | ASan `stack-buffer-underflow` in the model's `memcpy`, 134, 3 of 3 | the same |
| **(b'')** | **`both: 10022002`, 3 of 3** | **the same** | the base's end, 3 of 3 | the base's end, 3 of 3 |

The route is exercised, not bypassed: lldb's auto-continuing breakpoints on `csh-bpp-O2` count **2** entries through
`hero_cb_switch` (the two coroutines' callbacks, each on a runtime stack of its own) and **6** through
`hero_cb_door_switch` (each coroutine's `needstack` and two yields, made on the library's block), exit 0, the answer
right. **Linux arm64**: § 8.

**The seat's other programs.** Its C-alone programs (`cbs.c`, `cs.c`, `ra.c`, `ofib_main.c`, `py_on.c`, `probe_cost.c`,
`fi.c`, `cdepth.c`) link no runtime of Heroes (`grep -l "heroes_runtime\|hero_"` finds only a comment of `td.h`): (b'')
cannot move them, by construction (read). Its Heroes programs, base against (b''), this Mac:

- `td.hero`, how far below C's `sp` a trivial callback reaches (`out/td-mac.txt`): base **256** (the probe's floor) /
  548 / 400 / 1,544 bytes at `-O2` / `-O0` / `--sanitize -O2` / `--sanitize -O0`; **(b'') 440 / 520 / 488 / 3,248 warm,
  520 / 632 / 904 / 4,480 at the first call** (the first maps the stack). So (b'')'s decision, made on C's stack before
  its switch, reaches deeper below C's `sp` than the base's whole callback at `-O2` and under the sanitizer: the
  measurement under § 2's band.
- `hq.hero` (`qsort` of 1,000,000 through a Heroes comparator): `sorted 1` at both levels, both sides.
- `gt.hero` (no callback; its emission is byte-identical under the two compilers but the `hero_program_sources` line,
  `diff` empty): modes 0, 1, 3 and 4, three runs each at both levels, both sides (`out/gt-mac.txt`): mode 0 the stop's
  words 3 of 3 everywhere; modes 1, 3, 4 `checks failed 0` everywhere (mode 4 about 39,500 to 40,000 switches from the
  handler a run).
- `ofib.hero`, a Heroes job on OpenSSL's default ASYNC fibre (`pkg-config --modversion libcrypto`: 4.1.0;
  `out/ofib-mac.txt`, one run each): the base at exit 0 with the answer right having changed 1,379 (`-O0` n 200),
  54,487 (`-O0` n 1,200), 2,938 (`-O2` n 1,200) and **276,369** (`-O2` n 20,000) bytes below the 32 KiB fibre;
  **(b'') 0 bytes changed and the answer right at n 10, 200 and (`-O2`) 1,200, and `panic: stack exhausted in
  ofib.deep` past them** (told; the header counts only at the job's return, so a told run is uncounted, its recursion
  being on the runtime stack). The runtime stack's floor of 64 KiB gives the job twice the fibre's room, so `-O0` n 200,
  which the base ran by writing below, now fits. Route (o)'s guarded door: told, both sides.

**The defect's own shapes, the route's claim** (`out/canary-helper-mac.txt`, counting canary): canary kind 4 at `-O2`
n 9,000 / 10,000 / 100,000 and `-O0` 1,500 / 2,000, kind 6 at both levels, `helper.hero` at n 10,000 / 30,000: **(b'')
0 bytes below, told `stack exhausted in <m.f>`, 134, every run**, where the base wrote 24,022 to 112,508 bytes at exit 0,
hung, or ended in the false thread message; sanitized, (b'') ASan's `stack-overflow` with 0 counted, the base 26,045
to 232,206 bytes at exit 0. **A change from (b) and (b')** (read from `cbstack.c`'s `HERO_CB_SIZE_*`, measured here): the
runtime stack is sized like the stack C entered on, 256 KiB for the canary, so `-O2` n 9,000 to 100,000 is told where (b)
and (b') returned the right answer on 8 MiB; no callback has less room than the base gave it, and none more.

**So the seat's own veto ground does not stand under (b'') on this Mac**, at both levels, and nothing of its other
programs moved. Its condition's second half, *"the unbounded alternation"*, is § 2.

## 2. The refusal band: what the thunk leaves on C's stack, and whether it closes (ran, prototypes in my scratch)

**The band, reproduced** (`bin/depth.sh`, lane bpp's bisection, the deepest alternation n right with 0 bytes below a
256 KiB `malloc` stack; `out/depth-mac.txt`):

| | `-O2` | `-O0` | `--sanitize -O2` | `--sanitize -O0` |
|---|---|---|---|---|
| base | 4,093 | 908 | 3,270 | 774 |
| **(b'')** | **3,262** | 1,165 | **2,014** | **700** |

Past its depth (b'') is refused with 0 bytes, told; the base writes from n 4,094 at exit 0 (16 bytes at 4,094, 6,797
at 4,200). **And a second band nobody measured, the single callback**: `bin/leave.sh` bisects `CO_LEAVE`, the header's
own measure of the room it leaves on C's stack when C calls the callback (`co_entry` burns its stack down to that many
bytes and 64 more above the lowest, so each figure is the room less about 64; the same instrument on every side;
`out/leave-mac.txt`):

| smallest room right with 0 bytes below | `-O2`, n 0 | `-O2`, n 1 (one C call inside) | `-O0`, n 0 | `-O0`, n 1 |
|---|---|---|---|---|
| base | **103** | 167 | 471 | 759 |
| **(b'')** | **1,047** | 1,127 | 1,095 | 1,319 |

So **any callback C calls with about a kilobyte or less left of a known range, and more than the base needs, is refused
under (b'') where the base runs it right** (`CO_LEAVE` 103 to 1,047 at `-O2`, 471 to 1,095 at `-O0`): the reserve's 1 KiB is the cause, and it is the ffi-pragmatist's
veto class (a correct program refused), wider than the alternation's.

**What the 80 bytes a level are** (`otool -tV` of the `-O2` object of `alt`'s emission): the thunk `h_0cb_alt_step`
`sub sp, sp, #0x20` (**32**: its frame record and the 16-byte record of the argument and the result), the door's runner
`hero_ccr_h_alt_c_again` `stp x29, x30, [sp, #-0x10]!` (**16**), and C's own `c_again_` (**32**). The base's 64 is
`c_again_`'s 32 and its thunk's 32 with `alt.step` inlined into it. The runtime's own frames on C's stack beyond the
base's: the record (16) and the runner (16). The decision (`hero_cb_enter` and its callees) is transient, but needs
440 bytes below the thunk warm at `-O2` (`td`), which is what the 1 KiB reserve holds.

**Prototypes** (a copy of (b'')'s runtime, `263-critic3-scratch/rt-p/`, and hand-edited emissions of `alt.hero`,
`o/proto/`, built with the compiler's own clang words, `bin/hb2.sh`; a control, (b'')'s emission unedited on `rt-p`,
reads 3,262 as (b'') does):

- **P1, the thunk keeps no record on C's stack**: its one integer argument travels to the runtime's entry
  (`hero_cb_enter1`) in a register and the result comes back in `x0`, which `hero_cb_switch` already leaves as the
  runner left it. **64 bytes a level; deepest 4,078** (refused past it with 0 bytes, 1,008 left).
- **P1 with a smaller reserve** (`-DHERO_CB_RESERVE_BYTES`): 512 bytes, deepest **4,086**, 0 bytes at the refusal;
  **256 writes 40 bytes below at the refusal** (its own path, `hero_cb_reserve_refused`'s `stack_t` and
  `sigaltstack`); 128 writes 40 at a run that succeeds. So with the decision on C's stack the reserve cannot go under
  about 300 bytes at `-O2`, and the band cannot close.
- **P1c, the door's runner a tail branch into C** (`ldr x0, [x0, #8]; b _c_again_`; a hand change for this one
  signature, calling the header's `c_again_` because `c_again` is a `static inline` wrapper clang inlines; a general
  door would load the argument registers from its record by the C ABI and branch, unbuilt): **48 bytes a level;
  deepest 5,438**, past every n the base runs right, told past it with 0 bytes.
- **P4, the switch made before the decision** (`hero_cb_enter_s`, about 40 lines of AArch64 assembly in `rt-p`: it
  writes nothing on C's stack, moves to a decision stack, decides there with enter1's body, then switches to the
  chosen runtime stack with a frame record linking to the thunk's; **one decision stack for the process, Darwin
  syntax, plain builds, no CFI on the stub**: a prototype for this measurement, not a landing), with P1 and a reserve
  of 16 bytes (`out/p4-mac.txt`):

| | alternation deepest, `-O2` | room, n 0 / n 1, `-O2` | alternation deepest, `-O0` | room, n 0 / n 1, `-O0` |
|---|---|---|---|---|
| base | 4,093 | 103 / 167 | 908 | 471 / 759 |
| (b'') as built | 3,262 | 1,047 / 1,127 | 1,165 | 1,095 / 1,319 |
| **P4** (P1, reserve 16) | **4,094** | **23 / 71** | **1,259** | **55 / 263** |
| **P4 with P1c's runner** | **5,459** | 23 / 55 | **1,819** | 55 / 199 |

lldb counts 11 entries of `hero_cb_p4_on_r` and 10 door switches at n 10 (`alt-p4-r16-O2`): the route runs.
**So on this Mac, at both plain levels, the band closes, on both of its shapes**, with two changes: the thunk's
arguments and result in registers (P1), and the switch before the decision so the reserve holds only the thunk's own
frame (P4). The runner's tail branch is not needed to close it and buys 1,365 levels more at `-O2`. **Its cost**:
`cost_cb -O2` P4 with the cached page size (§ 6) **+255 instructions a callback** against (b'')'s +310 (counters,
best of five), the stub's second switch +18 over P1's. **What it leaves unbuilt**: a decision stack per thread (the
stub reads a global; a landing reads it from the thread pointer, `TPIDRRO_EL0` here and `TPIDR_EL0` on Linux, without a
call), CFI on the stub's two switches, ASan's fibre annotations on them, more than one integer argument (eight in
registers and a pointer to those C passed on the stack), the x86-64 and Windows stubs, and **the sanitized levels**,
where the band is widest (2,014 against 3,270 at `--sanitize -O2`): P4 there is unrun, and the runner's frame under the
sanitizer is 48 bytes (`stp x22, x21, [sp, #-0x30]!`) because it brackets the C call with the fibre annotations; a door
switch that calls them itself on C's side would leave none, unbuilt. **Linux**: P4 given an ELF twin of its stub closes
the band there too (§ 8).

## 3. Shape D and the warden's veto (ran; the sentences counted offline)

**Shape D, this Mac** (`dshape.hero`, `out/dshape-mac.txt`, `out/dshape-O2-100k-mac.txt`):

| bytes below C's 256 KiB `malloc` stack | `-O0` n 2,500 | n 4,000 | n 6,000 | `-O2` n 6,000 | `-O2` n 100,000 (60 s bound) |
|---|---|---|---|---|---|
| base | 7,952, exit 0 | **81,424, exit 0** | 114,656, then the false *"this runtime's record of the buffers lent to C was entered again ..."*, 134 | 0, exit 0, 3 of 3 | hang 2 of 3; once 139,200 then *"stack exhausted in c_deep"* |
| (b'') | 7,952, exit 0 | **81,424, exit 0** | 114,656, the same false message | 0, exit 0, 3 of 3 | hang 3 of 3 |

The same bytes to the byte on both sides at every n that returns. Lane bpp's (b'') run at `-O2` n 100,000 ended in the
false lend-record message after 139,200 bytes; mine hung three times; the base did both: **the end is the run's
layout's, on both sides**, and so is the false message, which the base gives at `-O0` n 6,000 too.

**The sentence that says C's frames are C's**, in § 9's last bullet (`spec/heroes-spec.md:340-341`), the warden's w2
family's place, written by me (`263-critic3-scratch/spec/c1.txt`):

> A source nested up to 2,000 deep compiles on every platform, and none is refused for its depth; running out of
> stack aborts, and C that a callback calls runs on the stack C called the callback on, where running out is C's.

and a fuller one that says what C's running out does (`spec/c2.txt`): *"..., where C's own frames may run past it and
write over what lies below, as C alone would."* **Counted** (ran: each applied to a copy of `main`'s spec, `736b534c…`,
`./heroes-next measure`, (b'')'s stage 2, offline, no request; `w1` applied by the warden's own diff, digest
`f372f420b838493f`, its 10,380 / 10,569 reproduced):

| text | claude-legacy | cl100k_base |
|---|---|---|
| the base | 10,374 | 10,563 |
| **c1** | **10,397 (+23)** | **10,586 (+23)** |
| c1 with w1 (the route's whole price in § 9 and § 13) | 10,403 (+29) | 10,592 (+29) |
| c2 | 10,411 (+37) | 10,600 (+37) |
| c2 with w1 | 10,417 (+43) | 10,606 (+43) |

Real tokens: unmeasured (a paid request, not this reading's); vendored counts are a lower bound by the warden's own
reading of them.

**What a reader of § 9 then predicts for shape D**: `enter` calls `c_walk`, a C function of its header, which recurses;
under c1 it runs on the coroutine's `malloc`'d block and its running out is C's, so the reader predicts no told end
and no promise, and under c2 that it *may write over what lies below and run on, as C alone would*, which is the run:
81,424 bytes at exit 0, and the ffi-pragmatist's C-alone OpenSSL job (its § 1) the same in kind. Under § 9 as it stands
a reader predicts *aborts*, which neither side does. **Does the warden's veto lift?** Its letter is *"any route that
leaves a callback's stack unguarded while § 9 stays unmoved"*: under (b'') every frame the callback's Heroes code makes
is on a guarded runtime stack (0 bytes, § 1), what is unguarded is C's block under C's own frames, and c1 moves § 9; so
**its letter is answered by c1**. Its ground is the warden's own sentence under (b), *"What C's own frames do on a stack
of C's is C's, as any C overrun is: § 13 promises an abort of C's reach only past a lend (`counted_by`)"*, which is
c1's content; its objection to w3, *"a confession of §1.12"*, is to a sentence saying the program may write over memory
it does not own, which c2 says of C's own frames alone and c1 does not say. Whether the seat takes c1, c2 or neither
is the seat's to give; I give no verdict. **One thing c1 leaves the veto to read**: on a stack no wrapper saw
(`known` false in `hero_cb_enter`), the entry checks no reserve, so the thunk and the decision put up to 440 bytes
(`-O2`, warm) to 4,480 (`--sanitize -O0`, first call) of the runtime's own frames on C's block unchecked (`td`, § 1;
`cbstack.c`, read): bounded, which the warden's condition asked, but the runtime's and not C's. P4 (§ 2) leaves the
thunk's own frame there and nothing of the runtime's.

**Shape D at `-O2` n 100,000, the language's or C's?** The writes are C's: `c_deep`'s own frames on C's own block,
identical in bytes under both builds at every n that returns, and made as C alone makes them. The ends (a hang, the
false lend-record message, or a told end) are the runtime reading its own thread-local words those frames overwrote,
the shape the sitting drafted as `d-p263-guard-return-address` and `d-p263-static-hang`, both `blocking`, the base's
today. **So (b'') adds nothing there and removes nothing**: routes (l) and (q) are owed by those drafts whatever route
lands, as the sitting said, and not as a condition of (b'') against the base; under c1 a false message in shape D is
still a §4.17 false message, so the drafts keep their class. Whether (l) and (q) would make it told is an inference
(they move the words, not the writes, the compiler-engineer's § 8), unrun.

## 4. The four signal cases: the room's count (ran, this Mac)

The four cases copied from `tests/golden/run/` with their three headers into `p/cases/`, built by (b'')'s compiler
against `rt-p`, whose `hero_frame_exhausted` (`parts/frame.c`) first prints the runtime stacks mapped and their bytes
against the room (counted where `hero_cb_map` adds them and `hero_cb_unmap_retired` gives them back), at `-O0` and
`-O2`; the base by the base's compiler (`out/cases4-mac.txt`). Every (b'') run ends at the room's abort, as the base's:

| case | the golden's count (the base, ran) | (b'') | runtime stacks counted at the abort | their bytes | what they are |
|---|---|---|---|---|---|
| `fixedbugs-2752-nested-callbacks-…-either-level-…` | 2,047 | **2,031** | 1 | 8,519,680 | the stack the callbacks stand on, sized as the main thread's 8 MiB, with two guards of 64 KiB and the record's page |
| `fixedbugs-2752-the-room-taken-at-three-depths-…-3000-times-…` | 963 | **956** | 1 | 8,519,680 | the same |
| `fixedbugs-a-c-longjmp-over-a-callback-inside-a-coroutine-…` | 2,047 | **2,045** | 3 | 1,228,800 | three of 409,600: a 256 KiB coroutine's size and 144 KiB of guards and page |
| `fixedbugs-a-coroutine-left-for-good-holding-a-large-value-…` | 2,046 | **1,149** | 1,149 | 470,630,400 | one per callback left for good in a coroutine, the stacks w1 names |

The fourth reads 1,149 with (b'')'s own runtime too (`o/cases/c4-bpp-O0`); lane bpp's 1,236 was read at 217f, before
217g's guard above each stack took a held stack from 344,064 to 409,600 bytes. **So it is both, and they are apart:**

- **The accounting moves the count, by design in one case and by the prototype's choices in three.** In the fourth,
  every stack counted is one w1 names (*"a stack this runtime gave them stay held ... counted ... against 1 GiB"*):
  1,149 is w1's count, and the golden's 2,046 is the base's. In the other three the stacks counted are **live** (the
  one the callbacks stand on, or a coroutine's run's in use and spare), which w1's *"counted with the live ones"* can
  be read to cover; and in all four **a stack's two 64 KiB guards and its record's page, 144 KiB of no access, count
  as room**, which is address space and no memory, and which no sentence states (36% of each held stack in the fourth).
- **The words omit the stacks in all four.** The abort comes from `frame.c`'s *"the room large values take off the stack is
  full"*, which names no stack, while stacks hold 0.8% (the first two), 0.1% (the third) and **43.8%** (the fourth) of
  the room. `cbstack.c`'s own words name them (*"the room large values, buffers lent to C and the stacks this runtime
  gives callbacks C calls take is full"*) only where a stack's own mapping is the request that overflows. That is the
  warden's third finding (*"under w1 they name no stack, so a program stopped by its held stacks would be told a cause
  it does not have"*), now measured.

**Which is right**: the counts are the runtime's and § 13 promises none of them (*"saying how many callbacks have not
returned"*), so the goldens are re-pinned at a landing under w1's rule, the rule written in each case's header; the
words are owed the stacks wherever any stack is counted, which is a §4.17 matter and `blocking` by § Bounded discovery
if it landed as built; and whether guards count is a choice the landing states (counting them bounds address space,
not counting them bounds memory). **Not a defect of the stacks' release**: the third case's three stacks are what a
coroutine's run maps, not one per jump (2,045 of its 2,047 callbacks fit).

## 5. The 368 bytes the reserve's own stop writes at `--sanitize -O0` in Linux: the writer (ran, Linux arm64)

Reproduced at `2dd6d086f`: `alt-bpp-sO0` at n 1,000, 2,000, 3,000 and 100,000, **`at the stop 368 (lowest 1072)`**,
*"C called back alt.step with 3840 bytes left ... less than the 4096 this runtime keeps there"*, every run
(`out/lx-run.txt`); on this Mac the same build is refused with 0 bytes (3,872 left). The whole output in Linux opens
with ASan's own *"WARNING: ASan is ignoring requested __asan_handle_no_return: stack type: default top:
0xffffd5cde000; bottom 0xffffbd7be000; size: 0x000018520000"*. **The writer, by a hardware watchpoint** (lldb in the
image, ASLR left on since the container refuses `personality`; a breakpoint at the header's `co_entry`, then
`watchpoint set expression -w write -s 8 -- (char *)co_block + 262144 - 1072`, the lowest byte the count names;
`out/lx-368-watch.txt`, transcribed from the terminal, the command with it): the first write there is

```
frame #0: __asan::TryAsanInitFromRtl() + 8
frame #1: ___interceptor_sigaltstack + 40
frame #2: hero_cb_reserve_refused(who="alt.step", left=3840) at cbstack.c:535:9
frame #3: hero_cb_enter(...) at cbstack.c:659:47
frame #4: h_0cb_alt_step(h0_n=267) at alt.c:260:5
```

with `sp = 0xffff80dbf3d0`, exactly `lo − 1072`. **So the writer is the reserve's own refusal path, on C's stack**:
`hero_cb_reserve_refused`'s frame (its `stack_t` and refusal record, with the sanitizer's redzones at `-O0`) and its
`sigaltstack(NULL, &alt)` query, which on Linux goes through ASan's interceptor (`___interceptor_sigaltstack`), whose
callee's frame record lands 1,072 bytes below the stack; on top of `hero_cb_enter`'s own `-O0` frame, about 4,900
bytes from the decision's frame down, past a reserve of 4,096. Darwin's `sigaltstack` is not intercepted, and the same
build there writes nothing. The ASan warning is the instrumented `hero_cb_enter` calling `__asan_handle_no_return`
before the `_Noreturn` refusal, with ASan's idea of the stack spanning the thread's and the coroutine's (a cosmetic
line under the sanitizer, printed before the told end). **It is P4's case**: a refusal made on the decision stack
writes nothing on C's (§ 2); short of P4, the refusal must not call anything on C's stack before its switch (the
alternate stack's bounds read once per thread, not asked at the refusal), or the sanitizer's reserve must hold about
5 KiB.

## 6. The costs (ran, this Mac, instructions retired, `bin/costrun.sh`: six rounds alternated, the first discarded,
the best of five; `out/cost1`, `out/cost2`)

| program | base | (b'') as built | per unit |
|---|---|---|---|
| `cost_cb -O2`, 20,000,000 callbacks | 3,363,603,772 | 9,566,101,460 | **+310.1 a callback** (+184.4%) |
| `cost_cb -O0` | 8,427,595,950 | 20,372,801,425 | +597.3 (+141.7%) |
| `cost_qsort -O2`, 1,000,000 `i64` | 3,933,186,241 | 10,390,324,373 | +164.2% |
| `coro_cb -O2`, 20,000,000 callbacks from a coroutine's `malloc`'d stack | 30,597,247,168 | 36,923,250,015 | +316.3 a callback (+20.7%) |
| `ccost -O2`, 20,000,000 C calls inside one callback | 2,623,802,069 | 4,803,256,839 | **+109.0 a C call** (+83.1%) |
| `ccout -O2`, 20,000,000 C calls outside every callback | 2,617,208,344 | 2,811,770,247 | **+9.7 a call** (+7.4%) |
| `cost_calls -O2`, Heroes calls inside one callback | 242,552,161 | 241,987,737 | −0.2% |
| `cost_rec -O2`, panel 104's recursion | 122,202,728 | 122,213,047 | 0.0% |

Lane bpp's +310 a callback and +109 a C call stand to within an instruction; its +8.7 a call outside every callback
reads +9.7 here. **Where
the 310 go** (`lldbp/count.py`, lldb stepping one callback, the 1,000th, from the thunk's first instruction to its
return, by function): base **156**; (b'') **468**: `hero_cb_enter` 94, `_tlv_get_addr` 64 (the base's 24: five more
thread-local reads, one a variable a function on Darwin), **`sysconf` 46 and `getpagesize` 22, the page size asked at
every callback** (`hero_cb_size_wanted`), `hero_cb_on_r` 32, `pthread_getspecific` 18 (the base's 12: the entry asks
the thread's key the question asks again), `hero_cb_size_wanted` 12, `hero_cb_take` 12, `hero_cb_switch` 12, the
runner's own 17, `hero_cb_c_range` 8, `hero_cb_give` 8.

**The obvious savings, prototyped** (`cost_cb -O2`, hand builds on `rt-p`; controls: the base's emission on the base's
runtime 3,363,517,668, (b'')'s on `rt-p` 9,564,539,358):

| variant | instructions | a callback over the base |
|---|---|---|
| the page size read once (`HERO_CB_FAST`, a static) | 8,224,098,603 | **+243.0** |
| P1, no record on C's stack | 9,444,723,813 | +304.1 |
| both | 8,103,944,567 | **+237.0** (lldb: 396 against 156, +240) |
| P4 with the page size read once | 8,464,815,919 (its round's base 3,362,613,651) | +255.1 (the stub's second switch, +18) |

**What an optimised (b'') would cost**: measured, +237 (+243 with one line); what is left is the entry's own 94 (a
decision that repeats half of the question's: the thread's key and the four signal-return compares, which
`hero_callback_asked` asks again on the runtime's side), five more thread-local reads (40), and the switch with the
stack's take and give (44). **By arithmetic, unrun**: one decision for both and one thread-local read carried in the
entry would take about 100 more, to about +130 to +150, beside (b')'s +133 and under (b)'s +284; the switch
itself (`hero_cb_switch`'s 12 instructions a callback by lldb's count, and the stub's 27 more under P4) is the floor no
prototype here went under. **Per C call** the door's
+109 inside a callback and +9.7 outside every one are untouched by these prototypes (the door reads `hero_cb_current`,
a thread-local, at every bound call of a program that hands C a function: Darwin's `_tlv_get_addr`). **Linux**: § 8.

## 7. Panel 258's R2 and (b'')'s switch (read; the run in § 7.1)

**Read** (`git show 64e15c644`, 214d's body; `git show eba5b297d:runtime/parts/unwind.c` and `rebind_jump.c`, lane
unwinder's head, which puts 214a back over `main` with 214b to 214d): 214d makes **the stop** find the target's frame
by identity, *"the one standing at the jump's stack pointer exactly, or ... at the walk's place for it ... Never `CFA >=
target`"* (`unwind.c:643-651`), the compiler-engineer's condition's second half. But three checks before it keep the
unwind on **one stack**:

- `hero_rebind_unwinds` (`rebind_jump.c:326-337`) asks it only *"from the thread's own stack to a target on it"*;
- `hero_unwind_chained` (`unwind.c:742-758`) walks the frame records from the hook to the target and answers 0, no
  unwind, at the first record whose saved frame pointer is not above it and below the stack's top;
- `hero_unwind_walked` (`unwind.c:509-531`) stops (`walk->stopped`) at any frame below the target outside `[lo, hi)`,
  the thread's own stack, and still reads `sp > walk->target` as *passed*.

Under (b'') a C jump over callbacks is made from C running inside a door's call, on C's stack, and the record chain
from it climbs C's frames, then the door's switch record **on the runtime stack** (`hero_cb_door_switch` hands C an
`x29` naming its record there), then the Heroes frames there, then `hero_cb_switch`'s record, back to the thunk on C's
stack. Wherever the runtime stack lies, that record's address is not between the frame below it and the thread
stack's top, or is outside `[lo, hi)`: **so by reading, 214d unwinds nothing across (b'')'s switches**, and every
callback (b'') does not run in place now runs on a runtime stack. The jump then goes as before 214a, and what the
jumped-over Heroes frames held stays held, counted, as § 13's held-memory sentence says, and is no fault. The CFI the
compiler-engineer asked is there (lane bpp's `bt-mac.txt`: the unwinder crosses both switches to `main`); what 214d's
landing does not do is walk across it.

### 7.1 The run: both lanes merged in my scratch (ran, this Mac)

`git merge-tree --write-tree lane-bpp lane-unwinder` (no commit, no ref; tree `b667dafba`) reads three conflicts,
`selfhost/emit/callback_thunk.hero`, `ctype.hero` and `guarded_names.hero`; I took its tree into
`263-critic3-scratch/mrg/` and resolved them by hand there: the thunk's text stays (b'')'s (`emit/thunk_text.hero`),
with 214d's mark (`unwound_frame.thunk_mark`) as the thunk's first line on C's stack; `Names` keeps both fields,
`doors` and `unwound`; the guarded names are the union. `cbstack.c` there takes `-DHERO_CB_NO_SWITCH` to build the
same runtime with (b'')'s switch off, so one compiler and one emission give both sides. Seed compiler, then stage 2
(`real 234.04`), **`sources 3e2c574b2046b392`**. My second reading's `uwx.hero` (214a's golden, its 1,000 rounds run
on the thread's own stack, a static array's, or a `malloc`'d block's, the runtime's live count printed after the exit
audit) and 214a's golden itself, from `lane-unwinder`, each emitted once and built twice (`out/r2-merged-mac.txt`):

| live blocks at exit, 1,000 rounds | thread's own stack | static array | `malloc`'d block | 214a's golden |
|---|---|---|---|---|
| 214d, (b'')'s switch off, `-O0` and `-O2` | **0** | 11,000 | 11,000 | its line, exit 0 |
| **214d with (b'')'s switch, `-O0` and `-O2`** | **11,000** | 11,000 | 11,000 | its line, exit 0 |

**So with (b'') R2 releases nothing, on the one stack where 214d releases everything**, and nothing faults: the jump
goes as before 214a and the audit forgives. 214a's golden does not see it (its `.expected` is the line and the exit).
**What a landing of both owes**: 214d's three one-stack checks taught (b'')'s stacks, crossing at a door's switch
record and at an entry's by their identity (`HeroCbDoor.rec_at`, the entry's record) and bounding each frame by the
stack it stands on (`hero_cb_bounds_of`), never by an order of addresses (its walk's `sp > target` is one); the
thunk's mark placed where that walk reads it (I put it on C's side; the runner on the runtime's is the other choice,
unrun); a case that counts what is released (`uwx`'s live count) beside 214a's golden, which cannot fail on this; and
the cost of both, 214d's +43 a callback (its commit body) on (b'')'s, paid today for nothing. **Linux**: unrun; 214d's
Linux hook asks the same thread's-own-stack rule (`sigjump_save.c:212-221`, `hero_jmp_unwinds`, read) and walks with
the same `unwind.c`.

## 8. Linux arm64 (ran in `heroes-linux-arm64:latest`, one container `c263c`, `--rm --pull never`, removed at the end)

Both compilers built in the image from `git archive` of the two commits (`bin/lx-build.sh`; stage 2 `real 4m50` each):
**(b'') `sources 185600f90a609b3c`, the base `692ac54673e38215`**, the Mac's two. Every program built there by
`heroes build` (`bin/lx-run.sh`, `out/lx-run.txt`):

- **`csh.hero`**: base and (b'') alike, 3 of 3 each: `both: 10022002` at `-O0`, `-O2` and `--sanitize -O2`; `task
  stack overflow`, 134, at `--sanitize -O0` on **both** sides (the model's `needstack(1024)` under ASan's `-O0`
  frames, the base's own end; lane bpp's 217e run had (b'') alone end so at `--sanitize -O2`, gone since 217f).
  **The veto's ground does not stand on Linux either.**
- **The band**: the alternation's deepest right n, base / (b''): `-O2` **4,094 / 3,262**, `-O0` 961 / 1,254,
  `--sanitize -O2` **3,272 / 2,302**, `--sanitize -O0` **813 / 732**; the single callback's smallest `CO_LEAVE`
  (`bin/leave.sh`; the header leaves about 64 bytes more than it is told), base / (b''): `-O2` **23 or under / 1,095** (n 0)
  and 87 / 1,175 (n 1), `-O0` 423 / 1,095 and 695 / 1,303. **The prototypes there** (`rt-p` built in the image, P4's
  stub given an ELF twin of its assembly, `out/lx-p4.txt`): P1 **4,077**, P1c **5,437**; **P4 (reserve 16) 4,095 at
  `-O2`, 1,364 at `-O0`, smallest room 23 or under (the instrument's floor) at `-O2` n 0 and n 1, 55 and 247 at
  `-O0`**; P4 with P1c's runner 5,460 and 2,047. **So the band closes on Linux too, at both plain levels, with P1 and
  P4**; the sanitized levels unrun under P4 on either platform.
- **The defect's shapes**: canary kinds 4 and 6 and `helper.hero`: (b'') **0 bytes, told, every run**; the base 19,461
  to 196,544 bytes, at exit 0 or told after, and canary 6 at `-O2` n 100,000 **exit 135, nothing said**.
- **Shape D**: the same bytes on both sides (`-O0` n 4,000 **41,957 at exit 0**; n 6,000 told after 68,242; `-O2`
  n 100,000 told after 139,200, 3 of 3 each).
- **`td.hero`**: base 256 / 548 / 304 / 1,536; (b'') 328 / 420 / 336 / 3,112 warm, 392 / 552 / 800 / 4,336 first
  (`-O2` / `-O0` / `--sanitize -O2` / `--sanitize -O0`).
- **OpenSSL 3.5.7's default fibre** (`ofib.hero`, `out/lx-ofib.txt`): the base **exit 139 at `-O0` n 200 and exit
  135 at `-O2` n 1,200, nothing said**; (b'') the answer right at n 200 (both levels) and 1,200 (`-O2`), told past;
  the 200 bytes changed below at n 10 on both sides are the library's own heap traffic (the ffi-pragmatist's § 16).
- **Costs**: no instruction counter in the image, so lldb's count of one callback (`lldbp/count.py`, `out/lx-count.txt`):
  base **180**, (b'') **457** (+277: `hero_cb_enter` 94, `__sysconf` and `__getpagesize` 66, `__pthread_getspecific`
  36 against the base's 18, `hero_cb_on_r` 29, the switch 12), P1 with the page size read once **384** (+204), P4 so
  **396** (+216). The program's own clock, best of five of six alternated, the container's load about 3: base 10.08 ns a
  callback, (b'') **25.88** (+15.8 ns), P1 so 25.98, P4 so 24.21: this clock does not resolve the prototypes' 70
  instructions, and the count is the judge.
- **Not run by me on Linux**: the four signal cases (lane bpp's run reads them failing the same way); R2 merged (§ 7.1);
  `gt.hero`.

## What the synthesis's amendment must settle

Most important first; each with the command that settled it or would. Every path under `263-critic3-scratch/`.

1. **The refusal band, the ffi-pragmatist's veto class, stands on (b'') as built and closes with two changes I
   prototyped.** As built it refuses correct programs the base runs right in two shapes: the alternation (`-O2`
   3,263 to 4,093 on this Mac, to 4,094 in Linux; `--sanitize -O2` 2,015 to 3,270 and 2,303 to 3,272; `--sanitize -O0`
   701 to 774 and 733 to 813), and **any single callback entered with less than the reserve left of a known range**
   (`CO_LEAVE` 103 to 1,047 here at `-O2`, 23 or under to 1,095 in Linux), which nobody had measured. **P1** (the
   thunk's arguments and result in registers, no record on C's stack: 64 bytes a level) and **P4** (the switch made
   before the decision, the reserve 16 bytes) close both shapes at `-O2` and `-O0` on both platforms: the alternation
   4,094 / 4,095 against the base's 4,093 / 4,094, the single callback 23 (or under) against 103 / 23. Settled by
   `bin/depth.sh o/proto/alt-p4-r16-O2`, `bin/leave.sh o/proto/alt-p4-r16-O2 0` and `… 1`, and `out/lx-p4.txt`. **Owed
   before it is a route**: a decision stack per thread read without a call, CFI on the stub's two switches, ASan's
   fibre annotations on them, arguments beyond one integer (eight in registers, a pointer to C's stack-passed ones),
   a nested decision (a handler's callback while one is being decided) made in place, and **the sanitized levels
   measured under P4**, where the band is widest and P4 is unrun; the x86-64 and Windows stubs.
2. **Panel 258's R2 releases nothing across (b'')'s switch** (§ 7.1): with both lanes merged, a C jump over callbacks
   on the thread's own stack leaves **11,000** blocks live where 214d alone leaves **0**; no fault, and 214a's golden
   cannot see it. The compiler-engineer's condition (2) is met in its letter (CFI, a stop by identity) and not in its
   purpose: 214d's three one-stack checks must be taught (b'')'s stacks, crossing by the door's and the entry's
   records' identity. Settled by `mrg/heroes-next` building `p/uw/uwx.hero` with and without `-DHERO_CB_NO_SWITCH`
   (`out/r2-merged-mac.txt`); a landing of both is gated by `uwx`'s live count, never by 214a's golden alone.
3. **§ 9's sentence**: under (b'') as built § 9 is false for C's own frames under a callback (shape D: the base's bytes
   exactly, 81,424 here and 41,957 in Linux at exit 0), so the warden's veto stands on it as built. **c1** (*"..., and
   C that a callback calls runs on the stack C called the callback on, where running out is C's."*) **+23 vendored on
   both counters, +29 with w1**; c2, which says what C's running out does, +37 and +43. Its letter answers the veto;
   its acceptance is the seat's; real tokens unmeasured (one paid request). Settled by `./heroes-next measure` of
   `spec/c1-w1.md`, then the warden's one `--refresh`. A sentence moving § 9 adds c1's text to the blind reading's arms.
4. **The room's abort**: its words (`frame.c`) name no stack while (b'')'s stacks hold up to 43.8% of the room, a
   §4.17 false cause the warden foresaw; the four signal cases' counts move by w1's rule (1,149 for the coroutine left
   for good, from 2,046) and by two choices no sentence states (the live stack counted, and its 144 KiB of guards and
   page counted as room). Settled by `o/cases/c<1-4>-bppi-O0` (`rt-p`'s instrument) and `o/cases/c4-bpp-O0`; the
   goldens re-pinned with the rule in their headers.
5. **The 368 bytes in Linux at `--sanitize -O0`** are the reserve's own refusal on C's stack: `hero_cb_reserve_refused`
   and its `sigaltstack` query through ASan's interceptor (a watchpoint, § 5). The compiler-engineer's veto class
   (bytes below a stack in a shape the route claims) **stands for that one build** as built; P4 moves the refusal off
   C's stack, and under the sanitizer that is unrun. Settled by `out/lx-368-watch.txt`'s command in the image.
6. **The costs**: +310 instructions a callback as built (Linux +277 by lldb's count, +15.8 ns by its clock), +109 a C
   call inside a callback, +9.7 outside every one; **+243 with the page size read once (one line), +237 with P1 too,
   +255 with P4**, under (b)'s +284 and over (b')'s +133. An optimised (b'') near (b')'s by merging the entry's
   decision with the question and carrying one thread-local in the entry is arithmetic, unrun. Settled by
   `bin/costrun.sh` over `o/pc/cb-*-O2` and `lldbp/count.py`.
7. **The runtime stack's size**: (b'') gives a callback the room C's stack had (the coroutine's 256 KiB, the thread's
   8 MiB, a floor of 64 KiB), so the canary at `-O2` n 9,000 to 100,000, which (b) and (b') returned right on 8 MiB,
   is told, and OpenSSL's 32 KiB fibre gets 64 KiB; a choice for the amendment to state, not a refusal against the
   base, which wrote below there. Settled by `out/canary-helper-mac.txt`, `out/ofib-mac.txt`, `out/lx-ofib.txt`.
8. **Shape D is the base's under (b'')**, its false lend-record message and its hangs the base's too: the drafts
   `d-p263-guard-return-address` and `d-p263-static-hang` and their routes (l) and (q) stand whatever lands, and are
   no condition of (b'') against the base. Settled by `out/dshape-mac.txt` and `out/lx-run.txt`.
9. **Unrun by me**: P4 under the sanitizer; the four signal cases in Linux; `gt.hero` in Linux; Windows, Linux
   x86-64, an Intel Mac, musl; a real copy-stack library (libco shared) and a conservative collector under (b'').

**Each seat's veto, read on the runs**:

| seat | as cast | on (b'') as built | with a named change I prototyped |
|---|---|---|---|
| **ffi-pragmatist** | veto of (b) as stated (`csh.hero`); its condition: `csh` and the unbounded alternation; its class: a correct program refused | **`csh` lifted** (3 of 3 at `-O0` and `-O2` on both platforms, the sanitized ends the base's; `td`, `hq`, `gt`, `ofib` unmoved or better); **its class standing** on the refusal band, two shapes (item 1) | **lifted by P1 and P4** at both plain levels on both platforms; the sanitized levels unrun |
| **compiler-engineer** | veto of (c1); approval of (b) with seven conditions, *"any one red turns me to (c2)"* | conditions (1) doors (my `check` of `sum13000` / `sum14000`: the base's 0 / 134) and (4), (5), (6) built (the last three lane bpp's, read; (6)'s words owed, item 4); **(2) unmet in purpose** (item 2); (3) the four cases are the room's count and words (item 4); **(7) unmet: +310 over (b)'s +284**; its veto class **standing at Linux `--sanitize -O0`** (368 bytes, item 5) | (7) **met** with the page size read once (+243) and P1 (+237); (2) unbuilt (214d's checks taught the stacks); the 368 bytes unrun under P4 |
| **spec-warden** | veto of any route leaving a callback's stack unguarded with § 9 unmoved | **standing**: § 9 unmoved, C's frames under a callback write below (shape D), and on a stack no wrapper saw the entry's own frames are unchecked | **its letter answered by c1** (+23 vendored, +29 with w1), its acceptance the seat's; P4 takes the runtime's frames off an unseen stack |
| historian (advisory) | none | (b'') is the shape it verified for OCaml 5 (C on the system stack, nested callbacks on the current fibre) | none |

## Files beside this report

`263-critic3-scratch/`: `bin/` (`b4.sh`, `r1.sh`, `depth.sh` and `hb2.sh` after lane bpp's, `leave.sh`, `costrun.sh`,
`lx-build.sh`, `lx-run.sh`, `lx-368.sh`), `rt-p/` (the prototypes: P1, P4 with its ELF twin, `HERO_CB_RESERVE_BYTES`,
`HERO_CB_FAST`, the room's instrument), `mrg/` (both lanes merged, three conflicts resolved by hand), `spec/` (`c1`,
`c2`, `w1` texts), `lldbp/count.py`, `p/` (the programs, copied from the seats', lane bpp's and `lane-unwinder`'s),
`o/proto/` and `o/pc/` (the hand-edited emissions `alt-p1.c`, `alt-p1c.c`, `alt-p4.c`, `alt-p4r.c`, `cb-p1.c`,
`cb-p4.c`), and `out/` (every run file named above). Builds removed at the end; the texts kept, and copied under
`programs/critic/third/` beside this report.
