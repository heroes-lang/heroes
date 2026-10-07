# Panel 196, completeness critic, second pass (the seats' reports)

Work copy: `<scratchpad>/196-critic/` (the frozen tree `39935f7c`, its compiler
built from the seed at 20:23). The compiler-engineer's five prototypes
(`heroes-s1`, `heroes-s2`, `heroes-s7`, `heroes-s7g`, `heroes-r`) were copied
into it at 23:13 (SHA-256 of `heroes-s7g` checked against the seat's: same);
probes are under `196-critic/p2/`. Started 23:08 by `date`. Nothing in the
repository, any worktree or another seat's copy was edited or run inside; no
paid run; no timing.

Read: my brief, the repaired `00-shared.md`, the reports of the
compiler-engineer (CE), the ffi-pragmatist (FFI), the spec-warden (SW), the
historian (H), the nine blind reports with `scored.md`, the two variant diffs,
and defect 413's issue.

(Written as I go; the summary at the end is the last thing written.)

## 1. The blind seat's two refused programs, run on the prototypes that exist (23:15)

`scored.md` ran the nine programs on the frozen compiler only, though the
blind brief says *"and on a seat's S1 or S2 prototype where one exists"*. Both
prototypes exist. Programs extracted from the reports' first fenced block
(the headers from their `files` sections) into `p2/blind/`:

| program | frozen | `heroes-s1` | `heroes-s2` | `heroes-s7g` |
|---|---|---|---|---|
| t1-A (variant A, S1's sentence) | 1: `lent_shape`, `ffi_type`, `fixed_outside_a_group` x2 | **1**: `lent_shape`, `fixed_outside_a_group` x2 | | 1, the same three |
| t1-B (variant B, S2's sentence) | 1: `expected_extent` | | **0, prints `ba7816bf...15ad`, the right digest** | **0, the right digest** |

So SW's *"one grammar change from plausible"* is now a measurement: the B
reader's program is correct under S2 as built. And the A reader's program is
refused under S1 as built by exactly the three refusals SW predicted would
survive (`lent_shape`: `lent` cannot stand on an `@` parameter today).

## 2. The composition (U + S7 with a guard, S2 its spelling, S5 beside it, S4 for single cells) attacked at the shapes beside it (23:16 to 23:25)

Every row run on this Mac, Apple clang 21, in `p2/`. `heroes-s7g` is the
compiler-engineer's S7 with its 16-byte canary; **it also carries the default
refusal** (`ffi_one_cell` fires on `md_scalar.hero` under `heroes-s7g` and
`heroes-r`, not under `heroes-s7`, measured 23:22), a route that seat objects
to.

1. **S7's buffer handed to a C function that KEEPS it** (`p2/keep/`).
   `setvbuf(stream: ptr, @buf: [i8] counted_by 64, mode: i32, size: u64)`, the
   stream from `fdopen(1, "w")`, then `fputs` through it:
   - installed in `main` (`setvbuf_keep.hero`): `heroes-s7` and `heroes-s7g`
     `run` 0, output intact; **`heroes-s7g run --sanitize` 134, `AddressSanitizer:
     stack-use-after-scope ... in __sfvwrite`**: stdio writes into the
     emitter's dead buffer;
   - installed from a helper whose frame then dies (`setvbuf_frame.hero`):
     **`run` 0, the stream's line ends in `@c\357l\001\0\0\0`, bytes of a dead
     frame**; `--sanitize` 134, `SEGV ... in h_setvbufframe_work`.
   The guard says nothing (no write past N happened). The binding cannot say
   *C does not keep it*: `lent` on an `@` parameter is refused (`lent_shape`,
   *no lend can stand in an `@` position*, measured on t1-A above). Yet § 13
   (line 399 of the frozen spec) already rules this for every other temporary:
   *"A lend lives for its call and no longer: a parameter is taken to keep what
   it is handed unless declared `lent`, and a lend reaches only one so
   declared."* **S7's buffer is a lend that does not obey the lend rule.** No
   seat names this shape.
2. **S7's buffer is a stack local of N, unbounded** (`p2/big/`, `SHA256_Final(@md:
   [u8] counted_by N)` under `heroes-s7g`): N = 1 MiB and 4 MiB `run` 0; **N = 8
   MiB and 16 MiB `run` 139 with no message at all** (0 lines matching `panic`
   or `stack exhausted`; the runtime's *stack exhausted* names recursion, not
   one frame that jumps past its window). The extent is the binding's word, so
   a mistyped constant is a silent crash; a Heroes thread's stack is smaller
   than main's (spawn.c sizes it), so the threshold there is lower (unrun).
3. **What S7 does not reach, measured** (`p2/read_s7.hero`, `p2/rec/`):
   - **`void *` buffers**: `read(fd: i32, @buf: [u8] counted_by 64, n: u64)` and
     `arc4random_buf(@buf: [u8] counted_by N, nbytes: u64)` are refused under
     `heroes-s7g`, `ffi_parameter_type`, *the header's `void *` points at
     `void`, which has no width*. `read`, `recv`, `fread`, `getrandom`,
     `arc4random_buf` all write through `void *`. The compiler-engineer's § 9
     cites `read` as a case where *S7's constant is the wrong form*; it is not
     reachable by S7 at all, so the sibling form both seats discuss needs
     `[u8]` admitted against `void *` too, or it serves `gethostname` alone.
   - **arrays of records**: `poll(@fds: [PollFd] counted_by 2, ...)`: refused
     with two messages for one mistake (`ffi_type` and `counted_by_shape`). And
     no form reaches it today either: a group record `{p: PollFd[2]}` lent whole
     to `struct pollfd *` is `ffi_parameter_type` on the frozen compiler and
     `heroes-s7g`. `poll` with two descriptors, `kevent`, `writev`,
     `epoll_wait` need C written by hand under every route on the table.
4. **Defect 396 with a RECORD cell, reached by no route in the composition**
   (`p2/rec/poll_cell.hero`): `poll(@fds: PollFd, nfds: u32, timeout: i32)`
   lent `@b.p` of a group record `{p: PollFd, after: i64}`, `nfds: 2`,
   `after` built as `{fd 1, events POLLOUT}`: **`run` 0 under the frozen
   compiler, `heroes-s7g` and `heroes-r`**, `after` 17179869185 becomes
   1125917086711809: the kernel wrote `revents` into the neighbour field. The
   default refusal is scalar-only; S4 may not copy a record (the
   ffi-pragmatist's veto, which the compiler-engineer joins); S7 refuses
   records. Today's correct spelling exists (route C, `@fds: PollFd counted_by
   nfds`: `record_lend_extent` at build for 2, measured) and an unmarked one is
   silent. **The census's 509 record-typed `@` lends are this shape's
   population**, and U's sentence (*one element*) is the only thing the
   composition says to them.
5. **The composition closes defect 396's own reproducer only if S4 is in it,
   and S4 is the one route the seats split on**: `md_scalar.hero` under
   `heroes-s7` (S1 + S2 + S7, no refusal, no guard on cells): **`run` 0,
   printing `1 1 1 186`** (23:28), the digest in the frame as on the frozen
   tree; S5 cannot read OpenSSL, so adding it changes nothing here; the compiler-engineer
   approves S4 (canary, no spec sentence), the ffi-pragmatist only in its
   guard-page form, the spec-warden objects to it standalone. The synthesis
   must say which, because it decides whether 396 is closed or only ruled on.

## 3. Defect 413 and what the ruling must say about a record lent to C (23:17 to 23:24)

**No adopted route answers 413.** S7 copies numbers only (records refused,
item 2.3); S4 may not copy a record (the veto); U says one element and nothing
of the address. The ffi-pragmatist's veto says what must not happen (*a record
is never copied*) and no seat says how an element reaches C instead.

**413 is wider than its item**, measured on the frozen compiler with zlib's
`z_stream` (the ffi-pragmatist's binding, `partial` with two fields) and with
libuv 1.53.0 from Homebrew (`uv/version.h`), whose loop keeps every handle's address in a queue
(`p2/ident/`):

| shape | zlib `init / reset / end` | libuv: two `uv_timer_init`, then `uv_print_all_handles` |
|---|---|---|
| a local `@zs` (the ffi-pragmatist's control) | `0 0 0` | two handles, two addresses of `main`'s frame, exit 0: correct |
| a field `@h.z` of a local group record | `0 0 0` (in place) | |
| an element `@zs[0]` (413 as filed) | `0 -2 -2` (the seat's) | **exit 0, the loop holding two addresses of dead stack temporaries** (`0x16bc12370`, `0x16bc122d0`), not the array's |
| **a Heroes function's own `@s: ZStream` parameter**, `init(@zs)` wrapping `deflateInit_`, the caller then using its local | **`0 -2 -2`, at `-O2` and at `-O0`** | **`-O2`: the queue is a cycle, the same dead address printed without end, killed at 60 s (exit 124); `-O0`: exit 139, no message** |
| the same wrapper handed an element, the element then copied to a local | **`0 0 0` at `-O2`, `0 -2 -2` at `-O0` and under `--sanitize`**: whether it works is the optimiser's stack-slot choice | |
| a stream initialised in a function and **returned** | `-2 -2` | |
| a stream **copied** after init, `t = zs` | the copy `-2 -2`, the original `0` | |

**Where the second row comes from** (`heroes build z_wrap.hero --emit-c`):
`h_zwrap_init(struct z_stream_s *ph0_s)` opens with `h0_s = *ph0_s;`, hands C
`&h0_s`, and ends with `*ph0_s = h0_s;`. The emitter writes that copy for every
`@` parameter of every Heroes function: `selfhost/emit/body.hero:294-310`,
*"Copy-in, after every declaration: §4.8's first half."* So **wrapping a C
call in a Heroes function, the ordinary way to structure a program, hands C a
copy**, with no array in sight, and with libuv that is memory corruption at
exit 0 or an endless loop, not zlib's polite -2. Same cause as 413 (§ 9's *copy
in, copy out* lowered literally for a value C knows by address), so by
`.claude/rules/verification.md` § Bounded discovery it widens 413's item rather
than becoming its own; `blocking` either way. Searched `issues/` of the b13
lane (23:30) for `emit/body.hero`, `copy-in`, `Copy-in`, `ph0_`, `libuv`,
`uv_`, `callee`: the hits are other shapes (panel 108's unread cell, defect
011's two arguments of one place, defect 114's zeroed slots), and **the
nearest is an open item of this very milestone**:
`issues/2026-09/25/2026-09-25-0003-m2-m-copies-a-pthread-mutex-t-which-posix-does-not-allow-and.md`
(M-buildable-structs, panel 178's completeness critic: *19 of 50 blind readers
raised copy-in/copy-out of the mutex on their own ... whether a group record can
say it is not a value is the question*). Its premise, *"the emitted C passes
the cell's address (`pthread_mutex_lock(&h0_m)`), so a local cell does not move
between calls"*, holds only in the function that owns the local: through a
wrapper's `@m` the lock is taken on the wrapper's copy (by the `body.hero`
prologue above; the mutex itself unrun tonight). No defect names the wrapper
shape (a negative over that vocabulary).

**What the ruling must say, in the order the measurements force it:**

1. **An `@` argument reaches C as its place's own address, never a copy**: a
   binding, a field, an array's element (the array unshared first, then
   `&data[i]`), and a Heroes function's own `@` parameter, which IS its
   caller's place (the callee works through the pointer). Inside Heroes this is
   indistinguishable from copy-in-copy-out where `@` is exclusive: `f(p: @x, q:
   x)` with `f` writing `p` returns `q.a` 1 and leaves `x.a` 5 (`alias.hero`,
   frozen, 23:25), which a by-pointer lowering keeps, since `q` is copied at the
   call. One case, not a proof; § 9's sentence can stay the semantics and § 13
   says what C receives.
2. **C may keep that address only while the place does not move, and the
   language moves places silently**: a return (`-2 -2`), a copy (`-2`), an
   array's growth or unsharing (unrun here; by construction of a refcounted
   array). The ruling either writes that down as the program's obligation, as §
   13 does for C's other promises, or gives such a record a mark the checker
   holds (route R-pin, § 8). Without one of the two, an in-place element lend
   fixes 413's reproducer and leaves `z_return`.
3. **Any temporary the emitter makes for a call (S7's buffer, S4's guarded
   cell, and today's element temporary until it goes) is a lend, and reaches
   only a parameter declared `lent`**, which means `lent` becomes legal on an
   `@` parameter (`lent_shape` moves). That is § 13 line 399's own rule; item
   2.1 is what breaks without it.

## 4. The guard: a 16-byte canary, or the ffi-pragmatist's guard page? (23:20 to 23:24)

**Neither, as built, is robust enough; the guard page is the one that can be
made so.** Measured both sides' failure shapes, which neither seat ran:

- **The canary is defeated by data equal to its pattern** (`p2/canary/`,
  `heroes-s7g`): `fgets(@s: [i8] counted_by 16, n: i32, stream: ptr)` called
  with `n: 48` on a file of 200 bytes and no newline, the binding's lie the guard
  exists for. File of `0x41`: **`panic: fgets wrote past the 16 elements lent
  to s`, 134**, caught. File of `0xA5`, the canary's own byte (the emitted C
  fills `g[16]` with `0xA5` and compares against it): **no panic; the program
  runs on and prints all four of its lines, then exits 134 with nothing on
  stderr** (the cause of that last abort is unrun; the frame's stack protector
  is the likely one, an inference). `--sanitize` catches both
  (`stack-buffer-overflow in fgets`). The pattern is a constant, so whoever
  controls the bytes C writes (a file, a peer on a socket) controls whether the
  guard sees the overrun. A per-process random pattern would close this one
  miss; it would not close the next.
- **The canary puts N on the stack** (item 2.2): 8 MiB and up is exit 139 with
  no message.
- **The guard page as prototyped (one scratch region per process, mapped once)
  is wrong under threads** (`p2/gpage/gp_threads.c`, the seat's
  `s7_guardpage.c` widened by me, two threads hashing `abc` and `xyz` 200,000
  times each through `final_into`): **26 and 89 wrong digests, exit 0**: the
  two threads share one buffer. Heroes has threads (`hero_thread_spawn_sized`).
- **It cannot hold N above a page**: extent 16,448 on a 16,384-byte page puts
  the buffer 64 bytes before the mapping; the copy-in faults outside the guard
  page, **exit 134 with no name** (the seat's handler `_exit`s silently there;
  mine prints that it could not name it).
- From the seat's own table: where the KERNEL writes (Linux `pipe`, Windows
  `GetKeyboardState`) the guard page makes the call fail (`EFAULT`, error 998)
  and the program goes on at exit 0 with a failed call it may not check;
  nothing is corrupted, but nothing is named either. The canary would abort
  there. So the spec-warden's wording condition, *"a guard page makes C writing
  past them aborts true of any overrun"*, is false as written: it is *aborts or
  the call fails*.

**What the robust form must be, from those rows**: a guarded region **per
thread** (thread-local, created at the thread's start beside its alternate
signal stack, released at its end), **sized to N rounded up to pages** and
grown when a larger N arrives, **one region per `@` buffer of the call**, and
**allocated in stack order** so that a callback into Heroes (§ 13: *a callback
is a parameter*, e.g. `qsort`'s comparator) that makes another such call does
not reuse the outer call's buffer; the fault handler is the runtime's existing
one (`runtime/parts/stack.c`) with a third witness, which the seat already
says. Unbuilt in Heroes; a runtime ABI bump. The cheaper robust alternative,
a fresh guarded mapping per call (`mmap` and `munmap`), is thread-safe,
re-entrant and unbounded at two system calls per call: its cost is unmeasured,
and CLAUDE.md § Precedence ranks robustness above speed. **The canary is the
fallback only if the synthesis bounds N at `check` and accepts the pattern
miss in writing; I would not adopt it as the guard.**

## 5. The blind seat: what *0 of 9 wrote the fault* and *5 of 6 wrote C* license (23:14 to 23:20)

Read from the nine reports, the two variant diffs and the nine folders'
`spec.md` and `headers.txt` under `/tmp/b196-t*` (read only):

- **Six of the nine readers read U's sentence.** `grep -c 'An \`@\` parameter is
  ONE element'` on each folder's `spec.md`: 1 in every A and B folder, 0 in
  every C folder (both variant diffs add it at line 363). So **the
  spec-warden's *"3 of 3 bind `frexp`'s `int *` as an unmarked `@exp: i32` ...
  (3 of 3 tonight without it)"* is false in its parenthesis**: 2 of the 3
  control readers had U, 1 did not. U's effect is not separable from S1's and
  S2's sentences in this run: there is no U-alone arm.
- **No reader saw S7's sentence.** The route every seat now converges on has 0
  reader measurements; the variants were the coordinator's S1 and S2 drafts.
  The spec-warden's S7 prediction (8 of 10 with no C file) is registered for
  the landing, so the sitting adopts S7 on engineering evidence alone. If an
  arm is run before the synthesis (a paid run, the coordinator's and the
  author's to decide; the author approved this sitting's blind `claude -p`
  runs), it should be scored on `heroes-s7`, **not `heroes-s7g`, which carries
  the default refusal and would refuse the `frexp` control** (item 2's note).
- **Task 1 never exercised defect 396's function.** `headers.txt` lists the
  one-shot `SHA256(d, n, md)` beside `SHA256_Init/Update/Final`, and **all
  three readers chose `SHA256`** (t1-A, t1-B by name; t1-C wraps it). Its `md`
  is the same shape (a count beside it counts the input, not `md`), so the
  fault was writable; but the 396 reproducer's `SHA256_Final` with a context
  record was read 0 times.
- **Task 3's excerpt carried Apple's annotation**, `char
  *_LIBC_COUNT(__namelen)`, which spells the count relation in the header text
  the reader saw; all three bound `ptr counted_by <the count>`. glibc says it
  differently, after the parameter list, `__fortified_attr_access
  (__write_only__, 1, 2)` (the historian's verified quote), and OpenSSL says
  nothing at all; whether a reader shown those reaches the same binding is
  unrun. So task 3 measures a reader handed the answer's shape in the
  clearest spelling any header has.
- **0 events in 9 bounds little.** The fault was possible in the 6 buffer
  programs; 0 of 6 is consistent at 95% with a per-program rate up to 39% (0 of
  3 on task 1 alone: up to 63%). It licenses *no reader of these nine wrote a
  one-cell byte lend*; it does not license the spec-warden's use of it against
  S3 widened (*"the fault it catches appeared 0 of 9"*) as a frequency, nor the
  reverse. The registered n = 10 predictions are the right instrument, and
  they should carry an S7 arm and a U-alone arm.
- **5 of 6 wrote C is a fact of the language more than of the readers.** Of the
  four correct buffer programs, four wrote a header; the fifth and sixth
  (t1-A, t1-B) failed to build, one with no header and one with structs only.
  Before the runs my first pass measured that no task-1 or task-3 program has
  a correct one-file answer under any variant, and the repaired brief told the
  readers they may write a header. So the measurement confirms a prediction
  the language made, and adds one thing the language did not: **t1-C, the
  frozen arm, wrote a `static inline` wrapper whose body checks `md_len <
  SHA256_DIGEST_LENGTH` before calling** (the count-and-check BCrypt and
  PostgreSQL ship, the historian's § E); the reader put the guard in C because
  the language had no place for it.
- **What one more run on the prototypes says**: t1-B builds and prints the
  right digest under `heroes-s2` and `heroes-s7g` (§ 1). So under S2 the B
  reader's first try is correct with a header of structs only, and S2's
  literal spelling is what the reader wrote, prompted by the variant's own
  example (`counted_by 32`), which is the spec-warden's condition on the literal
  answered once and prompted, not at n = 10.

## 6. Contradictions between seats, and which side is checkable

1. **S1**: the historian approves (advisory, S1 or S2, D's `ref int[2]`); the
   compiler-engineer and the ffi-pragmatist object; the spec-warden vetoes.
   *Checkable, and checked*: the historian's own text says D's form
   presupposes S0 (a local fixed array), which the compiler-engineer vetoes;
   and the one reader under S1's sentence wrote a program `heroes-s1` refuses
   three times (§ 1). On this tree S1 binds `SHA256_Final` from one file in 0
   ways, which is the compiler-engineer's prediction; the historian's reading
   holds for a language with S0, not this one.
2. **S4 on single cells**: the compiler-engineer approves it as a backend check
   with no spec sentence (canary); the ffi-pragmatist approves only the
   guard-page form; the spec-warden objects standalone. *Checkable in part*:
   the spec-warden's objection is to a SENTENCE (*a program cannot be written
   against sometimes*), the compiler-engineer's approval is of a check with no
   sentence, so the two may both stand; what is not checkable tonight is the
   cell guard itself (no compiler with S4 exists; the C is the seats'). § 2.5:
   without S4, 396's reproducer is open at exit 0.
3. **The sibling count** (`gethostname`'s `namelen` as the buffer's extent):
   the compiler-engineer objects if it is left out and would build it by copy
   (a heap buffer of the run-time count, unbuilt); the ffi-pragmatist objects
   to it by copy (§1.11 level 3, *unbox-in/box-out*) and wants it lent in
   place through S0's `[u8].ptr()`, which the compiler-engineer objects to
   until built (the unshare); the spec-warden approves the wording (`S7s`).
   *Neither form is built*, so the synthesis cannot adopt one on measurement;
   and § 2.3 shows the form's motivating cases (`read`, `recv`) are refused by
   S7 before the extent is asked (`void *`), so whichever is built must also
   say what `[u8]` against `void *` is.
4. **The guard**: the compiler-engineer built the canary and grants the guard
   page *misses neither*; the ffi-pragmatist conditions S7 on the guard page.
   *Checkable, and checked* (§ 4): each misses something the other catches
   as built; the guard page is the one that can be made robust.
5. **S5's mechanism**: the compiler-engineer prices a redeclaration in a unit
   of its own, from the dump's desugared type; the ffi-pragmatist measured that
   a redeclaration in Heroes' C spellings (`int8_t`, `uint64_t`) refuses
   correct bindings (*conflicting types*) and asks for the header's spellings or
   filtered `-ast-print`. *Not a contradiction once written down*: the
   compiler-engineer's source (`desugaredQualType`) is the header's spelling;
   the landing must say which unit and which spelling, and the ffi-pragmatist's
   `gethostname(int8_t a0[64], ...)` is the case that pins it.
6. **The literal spelling `counted_by 32`**: the compiler-engineer built it (the
   grammar moves); the spec-warden objects unless a reader test shows readers
   writing a number; its S7 sentence says *a constant of the group* only.
   *Checkable in part*: t1-B wrote the literal (prompted by the variant's own
   example) and it builds under `heroes-s2` (§ 1). The synthesis must say
   whether `CParam` moves.
7. **The census of live scalar lends**: 34 (shared brief, ffi-pragmatist,
   spec-warden) against 36 (compiler-engineer, adding the Heroes library's two
   bindings held as string literals). *Checked*:
   `selfhost/library_source.hero` lines 153 and 164 hold `hero_file_read_str(path:
   str, @status: i64)` and `hero_str_try_from_cstr(p: cstr, @status: i64)`
   (`sed -n 150,166p`, 23:25). 36 is right; my own first pass repeated 34.

## 7. Claims asserted and not measured with the command that settles them

- **The guard page *"misses neither the first nor any overrun length"*** (the
  compiler-engineer, granting it) and *"a guarded temporary turns every forward
  overrun into a named abort where C writes in user mode ... on all three
  platforms"* (the ffi-pragmatist): measured single-threaded with N under a
  page. Two threads: wrong digests at exit 0; N above a page: an unnamed fault
  (§ 4).
- **The guard's *no emission change where unused***: the `cmp` against the seed
  was run with `heroes-s7` (no guard). `heroes-s7g` carries the default refusal
  and cannot compile the compiler's own source (12 `ffi_one_cell`, the seat's
  table), so its emission of `selfhost/` was never compared. An inference from
  the guard living inside S7's buffer code; the landing's `cmp` settles it.
- **The census row *"S1, S2 and S7 change no tracked file's check outcome"***
  rides in `heroes-r`: measured as *no row differs by anything but
  `ffi_one_cell`*, which is a measurement of the 59 changed rows, and an
  inference for S7 in the 2,681 unchanged ones (S7 admits forms that were
  errors, so a changed row would have been a 1 becoming 0, which the census
  would have shown; it showed 0 the other way). Acceptable, and worth saying.
- **The spec-warden's *"(3 of 3 tonight without it)"***: false, 2 of the 3 read
  U (§ 5).
- **The spec-warden's *"a guard page makes C writing past them aborts true of
  any overrun"***: false where the kernel writes (the ffi-pragmatist's own
  Linux and Windows rows) and as prototyped for threads and large N (§ 4).
- **The compiler-engineer's § 9, `read` as a sibling-count case for S7**: S7
  refuses `read` (`void *`), measured (§ 2.3).
- **Defect 413's item names one lowering** (`ir/inout.hero`); the same copy is
  made by `selfhost/emit/body.hero:294-310` for every Heroes function's `@`
  parameter, measured with zlib and libuv (§ 3). The item is the coordinator's
  re-run of the seat's two programs, which is right as far as it goes.
- **The blind seat's `scored.md` ran the programs on the frozen compiler
  only**; its brief promised the prototypes too. Done now (§ 1).
- **The historian's central CVE**: re-checked, `curl` of
  `https://api.osv.dev/v1/vulns/CVE-2026-41681` (23:21): published
  2026-04-24, *"MdCtxRef::digest_final() writes past caller buffer with no
  length check ... From 0.10.39 to before 0.10.78"*, as the historian quotes.
  The rest of that report I did not re-fetch.
- **The ffi-pragmatist's *"LeakSanitizer: 268,096 bytes ... zlib's deflate
  state"*** is marked an inference by the seat itself; the defect's issue
  carries it as the seat's, not re-run. Fine as written.

## 8. Routes nobody listed

Each one is what would have to be true for a route outside the table to exist,
and each is measured as far as a probe without a build can go.

1. **`lent` on an `@` parameter, and temporaries only for `lent`**: the
   emitter hands a temporary (S7's buffer, a guarded cell, an element copy)
   only to a parameter declared `lent`; everywhere else it hands the place.
   This is § 13 line 399's lend rule extended to `@`; it moves `lent_shape`
   (today *no lend can stand in an `@` position*). Closes § 2.1 (setvbuf) by
   construction and gives § 3's ruling its spelling. Unbuilt; a checker rule
   and a spec clause.
2. **In-place lowering of every `@` record argument**: `&place` for an
   element (after an unshare) and the callee working through `ph0_s` instead of
   copying it into `h0_s` (`emit/body.hero`'s prologue). Closes 413 and the
   wrapper rows of § 3. Unbuilt; the IR and emitter change 413's repair owes
   anyway.
3. **R-pin, a group record that says it is not a value**: copy, return and
   storage in an array refused for it, so `z_return`, `z_copy` and a grown
   `[ZStream]` become compile errors instead of `-2` or a corrupted libuv loop.
   **Not new**: it is the open question of panel 178's completeness critic,
   filed as an M-buildable-structs feature on 2026-09-25 (§ 3). This sitting
   is the one that measured why it matters for the boundary. A language form:
   its own sitting, or this one's question to the author.
4. **S4 in place**: instead of copying a lent local into a guarded temporary,
   the emitter LAYS OUT a local that is lent with `@` inside its own guard
   (`struct { T v; unsigned char g[16]; }`, or against a guard page), so C gets
   the place itself, identity kept. Compatible with the ffi-pragmatist's veto
   (nothing is copied, records included), and it reaches `md_scalar.hero` (a
   local); it does not reach a field (`md_field`, `poll_cell`) or an element.
   Unbuilt, unpriced.
5. **`[u8]` against `void *` for S7** (and for the sibling form): the byte as
   the unit where the header says none, so `read`, `recv`, `fread`,
   `getrandom`, `arc4random_buf` become reachable. Today S7 refuses all of them
   (§ 2.3).
6. **Arrays of records to C** (`poll`, `kevent`, `writev`, `epoll_wait`): S7
   over records for a parameter declared `lent` (a copy is harmless where C
   keeps nothing, which is the veto's own lifting condition, *records the
   emitter can show C never keeps the address of*, met by the binding's word as
   every extent is), or a group record holding `T[N]` lent whole to `T *`.
   Nothing on the table reaches them (§ 2.3).
7. **A ceiling on a stack-held temporary**: refuse at `check` a stated extent
   whose bytes pass a bound, or allocate every emitter buffer off the stack
   (which the per-thread guard region of § 4 does anyway). § 2.2's silent 139
   needs one of them.

## 9. The question the sitting should have asked

**What address does C receive for an `@` argument, and for how long may C use
it?** The sitting asked how many elements; tonight's probes say the extent
ruling is unsafe without this one, because the composition creates new
short-lived addresses. Today, measured, one spelling (`@x`) hands C four
different things:

| what is lent | what C receives | lives for |
|---|---|---|
| a local, a field | the place | the place's life |
| an array's element | a temporary in the caller's frame | the call (413) |
| a Heroes function's own `@` parameter | the callee's copy of the caller's place | the callee's frame (§ 3, unfiled) |
| under S7 / S4 | the emitter's buffer / guarded copy | the call (§ 2.1) |

§ 9 says *copy in, copy out*; § 13 says *a C out-parameter is an `@`
parameter* and *a lend lives for its call ... unless declared `lent`*. Those
three sentences do not say which row a binding gets, and a library that keeps
the address (zlib, libuv, stdio's `setvbuf`, a mutex) is correct only in the
first row. The ruling the measurements support is § 3's three sentences:
the place, never a copy; C keeps it only while the place does not move; a
temporary only for `lent`.

**And a second, smaller**: *which C functions does the composition still
leave to C written by hand?* Measured: every `void *` buffer (`read`, `recv`,
`arc4random_buf`), every array of records (`poll` with two descriptors), every
function that keeps a buffer (`setvbuf`, unsafe rather than unreachable), and
every count-carrying buffer until the sibling form is built. The claim *S7
removes C by hand* is true of typed outputs with a fixed extent (digests,
`pipe`, `uuid_generate`, `ctime_r`, `gethostname` with a constant); the
synthesis should say that much and no more.

## 10. In one paragraph, for the synthesis

The seats' measurements hold where I re-ran them, and the B reader's program
builds and prints the right digest under S2 as built. But the composition, as
the seats converge on it, is not yet the robust one: **S7's buffer is a lend
that ignores § 13's lend rule**, so a C function that keeps it (setvbuf) reads
a dead frame at exit 0, and nothing in the binding can say it does not; **its
16-byte canary misses an overrun whose bytes equal its constant pattern**
(fgets of `0xA5`: no panic, exit 134 with no message) and its stack buffer
crashes silently from 8 MiB; **the guard page as prototyped corrupts under two
threads** (26 and 89 wrong digests at exit 0) and cannot hold N above a page,
so the robust guard is a per-thread, per-buffer, page-sized region, unbuilt;
**396 with a record cell (`poll`) stays silent at exit 0 under every route**;
and **396's own reproducer stays at exit 0 unless S4 lands**, which is the one
route the seats split on. **No route answers defect 413, and 413 is wider than
filed**: a Heroes function's own `@` record parameter hands C a copy
(`emit/body.hero:294-310`), which with libuv is an endless loop at `-O2` and a
silent SIGSEGV at `-O0`, and with zlib `-2` at both levels (handed an element
first, it works or fails by the optimiser's choice). The blind seat's 0 of 9 bounds the fault's rate only below 39%, 6 of
its 9 readers read U, none read S7, and task 1 never used `SHA256_Final`. The
ruling should state what address C receives and for how long (§ 3, § 9),
make `lent` legal on `@` and required for any temporary, and take the guard
page in its per-thread form as S7's condition.

Nothing in the repository, any worktree or another seat's copy was edited; no
paid run; no timing (the 60 s in § 3 is a `timeout` on an endless loop, not a
measurement). Finished 23:31 by `date`.
