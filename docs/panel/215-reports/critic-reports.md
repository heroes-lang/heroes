# Panel 215, the completeness critic, pass 2: what the seats' reports leave missing

Written 2026-10-14 from 10:45, after the five reports, in the critic's own copy
`.claude/worktrees/scratch-panel-215/critic2` at `7e5c02048`, its compiler built from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, real 12.65 s, `heroes 0.4.0`, load
5.1). No verdict. Every finding names the command that settles it and what it printed; a sentence
that could not be run says *unrun*. What the copy built is kept in
`docs/panel/215-reports/critic-work/pass2/`; the copy is removed at the end.

## In short, most consequential first

1. **Q3: the paid run's union task fails panel 194's registered prediction, and the failure is the
   specification's** (§ 1). A2 read **1 of 10** correct first try under R1+K; 194's warden registered
   *"nothing (R1) ... becomes my verdict if the landing's reader test fails R1+K's prediction"* (≥ 8
   of 10). The readers had no compiler (`--tools "Read,Write"`, `Bash` disallowed), so defect
   2062's message, to which the ergonomist attributes the misses, reached none of them; three cite
   § 13's *"named or anonymous"*, four rejected the `partial` binding because R1's sentence does not
   say it zeroes undeclared bytes. The compiler's message is **true of C** (`clang`: *"no member
   named 'fd' in 'struct tagged'"*), and r08's `partial` binding is the one R1's own landing tests
   (`ffi-rest-zero-a-partial-record.hero`, *"a named union a `partial` record leaves to C"*), which
   the specification never says. None of the four seats adopting Q3's A cites this number.
2. **Q1: the prototype G leaves two shapes open at every level on x86-64** (§ 2): a record nested in
   a record and an array of records lent with `counted_by`, MemorySanitizer **1/1/1/1/1** and
   **2/1/1/1/2** with the prototype exactly as without it. `offsetof` takes `inner.d` and `a[1].c`
   under `-pedantic`, so a recursive G stays clang-checked arithmetic, which also answers the
   ffi-pragmatist's objection to a dump-fed G.
3. **Contradiction 1: the ffi-pragmatist's G is the vetoed class** (§ 2). Its gap macro prints
   `1432778632` for `1234605616436508552` on a union arm followed by a declared field, as the naive
   G does; it survives the compiler-engineer's `uni.hero` only because it never zeroes tail padding.
4. **Contradiction 2: one mechanism, and the level that matters is `heroes build`'s default** (§ 2,
   § 5). Re-poisoning R7's lend region brings line 29's report back at `-O1` to `-O3`, so the
   compiler-engineer is right and the ffi-pragmatist's inlining reading is refuted (also by its own
   two-module run). `heroes build` and `heroes test` compile at `-O0`: there the same binary prints
   `memcmp 0 -224 0` and then `-116 0 -247` for records `==` calls equal, and a Heroes record hands
   C **7 bytes of the stack's `0xAA`** through its padding. **The Windows box keeps the bytes at
   every level** (sentinel, 0 everywhere; unrun by every seat); x86-64 SysV loses them as values at
   every level in C.
5. **Q2, contradiction 3** (§ 6): the "three spellings" objection to C is measured away (type
   identity against POSIX's and Windows' own typedef names holds on Darwin, glibc and Windows); **B
   with `z_stream` marked refuses the tree's own repair of defect 413** (2 `pinned_copied`, unmarked
   exit 0); an unlisted route, *a `partial` group record is not a value*, moves 14 of 905 files,
   `examples/ctime`'s 30 harmless copies of a `struct tm` among them; Task B's verb can only be written with `@`,
   so its 0 of 10 neither meets nor refutes the spec-warden's condition; and the dispute is
   CLAUDE.md's ranking against design.md §1.12's *"does not suspend Principle 0"*, which the
   sitting should rule in writing.
6. **Contradiction 5: the deletion is the prototype's, and it un-refuses a program** (§ 3):
   `heroes-g fmt` drops `pinned` at exit 0, and the refused `p_mx.hero` (`pinned_copied`) becomes a
   program that builds. B owes `fmt.hero` and `dump.hero` in the checker's commit.
7. **Contradiction 4: the field-read copy is `-O0` machine code only and invisible** (§ 4):
   `bl _memcpy` at `-O0`, one `ldr` at `-O2`; a C thread contending for the lock while Heroes reads
   `c.n` under it: ThreadSanitizer 0 warnings at both levels, `200000 true`. No adopted route makes
   it a value; B's sentence would be false of the emitted C at `-O0`.

## 1. Q3: the paid run's union task fails panel 194's registered prediction, and the misses come from the specification's text, not from defect 2062's message

**What the seats rest on.** Four seats adopt Q3's A (R1 stands). The spec-warden's report was last
written at 08:43 and the paid run's report at 09:35 (`ls -lT 215-reports/*.md`), and the warden's
cites none of the run's results; the compiler-engineer's and the ffi-pragmatist's cite none either
(`grep -n -i "A2\|1 of 10" compiler-engineer.md ffi-pragmatist.md historian.md`: no reader
score; the historian's, last written 09:37, scores Q3 on `u8` alone). The
llm-ergonomist approves R1 because *"the one task most readers failed failed on the union's
message"* (`llm-ergonomist.md:68-70`), and defect 2062 is filed on that reading.

**What panel 194 registered, read in its file.** `docs/panel/194-reports/spec-warden.md`, the
verdict table: R1's prediction is *"under R1+K ≥ 8 of 10 correct first try and ≤ 2 of 10 write
`u8`"*. The synthesis's version adds *"one task holding a union"*
(`docs/panel/194-the-rest-is-zero-*.md`, § Predictions to score). And the row that decides what
follows a miss: **`nothing` (R1) | object | ... | *"becomes my verdict if the landing's reader test
fails R1+K's prediction"***. So a miss of the first-try half under R1+K is the event 194's warden
registered as turning their verdict to withdrawal.

**What the run read on that arm.** A2, the task holding a union, under today's specification (R1
with K): **1 of 10** correct first try (`llm-ergonomist-work/scores.txt`). The ergonomist's own
words: *"on the task holding a union the prediction's first half fails"*.

**The attribution to the message cannot be right for a first try.** The readers had Read and Write
only and no compiler (`run_one.sh`, `--tools "Read,Write"`); each wrote its program once, and the
coordinator built it afterwards. **No reader saw defect 2062's message before writing.** What they
had was the specification, and their notes say which sentences decided:

- **The union sentence, § 13 lines 387-390** (*"A field in a C union, named or anonymous, shares its
  bytes ... a record names one or more of each union"*, landed by panel 186's R6, `git log -S'named
  or anonymous' -- spec/heroes-spec.md`: `b7c510c56`). Readers r03, r06 and r07 quote *"named or
  anonymous"* as the reason they wrote `fd` and `handle` on the record
  (`grep -n -i "named or anonymous" llm-ergonomist-work/r*/notes.md`).
- **R1's own sentence, line 383** (*"every field sharing no byte with one it names is zero"*).
  Four readers rejected the `partial` binding, the one that builds, because that sentence does not
  say it reaches bytes the record does not declare: r02 *"it's unclear whether `rest: zero` zeroes
  bytes the record doesn't declare"*, r03 *"which the spec doesn't say"*, r06 *"leaves unclear
  whether `rest: zero` covers the unnamed bytes"*, r07 *"unclear whether `rest: zero` zeroes the
  bytes that aren't listed"* (`grep -i -E "partial|rest: zero" r0*/notes.md`, read). The compiler
  does zero them (`zeroed.hero` emits `(T){0}` and a `memset` of `sizeof`), so the promise the
  readers looked for is kept and not written.

**What "named" means in the tree, measured.** The goldens bind a named union two ways, and the
specification states neither:

- **left to C by a `partial` record ended with `rest: zero`**, which is **R1's own golden**:
  `tests/golden/run/ffi-rest-zero.h:30-32`, *"a named union a `partial` record leaves to C"*,
  `typedef struct { int32_t kind; union { char c; double d; } u; } SP;`, bound `record SP partial`
  with `kind` alone (`ffi-rest-zero-a-partial-record.hero:7-8`), its `sp_union` reading the union
  zero. That is A2's shape exactly, and the binding reader r08 wrote, the one that built;
- **reached through a header macro** (`tests/golden/run/fixedbugs-151-unions.h`: `#define
  addr_bytes addr_u.a8`, *"the way libc's `s6_addr` and `sa_handler` are reached"*).

So R1's landing knew A2's shape and tested it; its sentence says *"every field sharing no byte with
one it names"*, which four readers could not read as covering a member the record does not
declare. The macro route, in the copy (`build/c2/a2/`, kept as `pass2/a2/`):

| header | reader r01's `a2.hero` (flattened `fd`, `handle`) |
|---|---|
| the experiment's `union.h` | `error[ffi_unknown_field]: union.h declares no fd in struct tagged`, exit 1 |
| the same with `#define fd via.fd` and `#define handle via.handle` after the struct | `wrote r01`, prints `2`, exit 0 |
| plain C, `t.fd = 1` on the experiment's header (`clang -fsyntax-only`) | `error: no member named 'fd' in 'struct tagged'` |

So the compiler's message is **true of C** (`struct tagged` has no member `fd`), and what misled
nine readers is the specification promising a named union's members as fields without saying that
it takes the header's macro, and not saying that `partial` with `rest: zero` is the binding R1's
own golden uses. **Defect 2062 is written as its finder saw it** (CL-078): its title
calls `via` *"an anonymous union"* (it is a named member of an untagged union), and its class rests
on *"a false message ... the header declares the member"*, which clang contradicts. Where it lives
is the specification's union sentence and R1's silence about `partial`, and a repaired note alone
would leave the next first try where this one was.

**The question the sitting did not ask**: does A2's 1 of 10 fire panel 194's *"nothing becomes my
verdict"* row? The run is not 194's test as registered (one arm, not `utsname`), which every seat
says; but its R1+K arm's first-try half is an absolute threshold that this run scores on 194's own
*"one task holding a union"*, and it missed it by seven readers. Three readings are open, and the
sitting picks one in writing: (a) the row fires and R1 is re-argued under the removal branch, as
the 215 spec-warden's own § 4 lays out for a lapse; (b) the miss is charged to panel 186's union
sentence and not to R1, which four readers' notes contradict for the `partial` half; (c) the test is
re-run as registered after the specification's union sentence says where a named union's members
come from. None of the four seats that adopted Q3's A cites this number.

**A route nobody listed for Q3 follows from it**: **R1's sentence says it reaches the whole
object** (*"... and every byte it does not name is zero"*, or *"the rest of C's struct is zero"*),
since the compiler already writes `memset(&t, 0, sizeof t)`; and the union sentence names the macro
route. Neither is priced: the spec-warden's drafts (`drafts.py`) shorten R1 and never widen it.
*Unrun*: the token price of either sentence, and whether readers then pick `partial`.

## 2. Q1: route G as adopted leaves two shapes open at every level, and the ffi-pragmatist's G is the vetoed one

**Contradiction 1, checked: yes, the ffi-pragmatist's G is the class the compiler-engineer vetoed,
and its probe could not tell them apart.** The ffi-pragmatist's G is `HERO_ZERO_GAPS_CD`
(`ffi-pragmatist-work/q1/routes/common.pl`): a `memset` from the end of one declared field to the
`offsetof` of the next, written for `struct cd`, whose two declared fields are all its members, so
naive and sound coincide there (*"G was written and run for the full record `struct cd` alone"*,
its § What I could not run). Built on the same probes in the copy (`build/padc/run_uni.sh`: each
emitted unit patched by `patch_g.py` with the three forms, `clang -O2`, this Mac):

| probe | today | compiler-engineer's G, vouched (prototype) | naive G (spans = declared fields) | ffi-pragmatist's gap macro, generalised to each pair of declared fields |
|---|---|---|---|---|
| `uni.hero`: `{int32_t kind; union {int32_t fd; void *h;};}`, bound `kind`, `fd` (the compiler-engineer's) | `1234605616436508552` | `1234605616436508552` (writes nothing) | **`1432778632`** | `1234605616436508552` |
| `unimid.hero`: the same union **followed by** `int32_t tail`, bound `kind`, `fd`, `tail` (the critic's) | `1234605616436508552` | `1234605616436508552` (writes nothing) | **`1432778632`** | **`1432778632`** |

The gap macro survives `uni.hero` only because it never zeroes **tail** padding: on `struct tp {
double d; char c; }` its one gap is `offsetof(c) - (offsetof(d) + 8)` = 0 bytes, so it writes
nothing where x86-64 loses bytes 9 to 15 (*arithmetic of the macro, read; the prototype's zeroing of
that shape is measured below*). Completed with the tail, it is the naive form on both probes. So
the two seats' adoptions of "G" differ on exactly the half that makes G sound: the
compiler-engineer's needs the header's membership (the screen's vouching today, the dump's rows for
the rest), and the ffi-pragmatist's own condition turns it to object to that half (*"so its bytes
would have to be written from the dump unverified; then C (by address)"*), the route the
compiler-engineer vetoes by `cret.hero`.

**What reconciles them, measured, and listed by neither**: the dump names every C member, and
each one can still be written as clang-checked `offsetof`. In the copy (`build/padc/offs.c`,
`clang -std=c11 -Wall -Wextra -pedantic`, no warning): `offsetof(struct s, h)` through an
anonymous union prints `8+8`; `offsetof(struct ocd, inner.d)` (a nested member) `8`;
`offsetof(struct arr, a[1].c)` (an element of an array member) `16`. So **G with the header's
membership, every span written as `offsetof`/`sizeof` of a C member the dump names, recursing into
member records and array elements**, is checked by clang as the ffi-pragmatist asks and sound as
the compiler-engineer asks. Only an unnamed bit-field run stays outside `offsetof` (a record holding
one is `partial`, spec line 385), and leaving its storage unit alone writes no member's byte.

**Two shapes beside G that no seat built, and the prototype leaves both open at every level.**
`build/padc/pc.hero` over `pc.h`: each record crosses a Heroes call by value, then is lent from a
place no zeroed lend region holds, as `fieldlend.hero` does, each lend handing every byte to
`write(2)` on `/dev/null`. Run by `build/padc/msan_c2.sh` in `heroes-linux` (x86-64 emulated,
Debian clang 22.1.8), each compiler built there (today's from the seed, the prototype from its own
emitted C, `build/g2c/heroes.c`); output `build/padc/msan-x86-pc.txt`:

| line | shape | today `-O0` / `-O1` / `-O2` / `-O3` / `-O2 -fno-inline` | prototype G |
|---|---|---|---|
| 42 | `struct tp { double d; char c; }`, **tail** padding, a field lend | 1 / 1 / 1 / 1 / 1 | **0 at all five** |
| 48 | `struct cd`, a field lend (the compiler-engineer's own shape) | 1 / 1 / 1 / 1 / 1 | **0 at all five** |
| 45 | `struct ocd { struct cd inner; }`, a **record nested in a record**, a field lend | 1 / 1 / 1 / 1 / 1 | **1 / 1 / 1 / 1 / 1** |
| 50 | `[Cd] counted_by 2 lent`, an **array of records**, elements pushed from by-value calls | 2 / 1 / 1 / 1 / 2 | **2 / 1 / 1 / 1 / 2** |

Why, read in the prototype's emitted C (`heroes-g build pc.hero --emit-c`): `struct ocd`'s only span
is `{offsetof(struct ocd, inner), sizeof(...->inner)}` = the whole 16 bytes, so `hero_pad_zero`
writes nothing (the vouched screen passes it: its one field is its one member); and the array lend
goes through `hero_lend_take`, which copies the elements in with their bytes, with no
`hero_pad_zero` line at all (`spans_of` answers *"not a record"* for `.array`). **The
compiler-engineer's prediction** (*"0 reports for `fieldlend.hero`, `cret.hero` and `rzpad.hero`"*)
**names none of these shapes, and its argument's "closes every shape measured on x86-64" is true
of the shapes measured.** A record nested in a group record is a shape the tree already binds, padded on both levels
(`tests/golden/run/ffi-rest-zero-a-nested-group-record.hero`: `struct outer { int32_t k; SA inner;
}` with `SA` holding padding and an anonymous union, `ffi-rest-zero.h:19-28`, read); a nested
record loses bytes in a by-value crossing only when the whole is 16 bytes or less, as `struct ocd`
is. The array lend is spec § 13's own form for *"the group's records"*. Both are the "nested" and
"padded inside an element" shapes CLAUDE.md § RUN IT names. The recursion above (`inner.d`,
`a[1].c`) is what closes them; *unbuilt*.

**Contradiction 2, checked: one mechanism, and the ffi-pragmatist's explanation of the clean
levels is refuted, twice by the seats' own files and once directly.**

- Both seats measure the same loss: x86-64 SysV passes `{char; double}` as `{ i8, double }`, so
  bytes 1 to 7 never travel (the ffi-pragmatist's `abi-lowering.txt`; the historian's
  `GetINTEGERTypeAtOffset` quote). Their *"every level"* readings both come from places outside a
  lend region: a Heroes record's field (compiler-engineer's `fieldlend.hero`), an array element
  and a field copied into a lent local (ffi-pragmatist's S1, S2), and its S0 at `-O2`, a local lent with
  `counted_by` (*whether that lend goes through a zeroed region, unread*).
- The ffi-pragmatist's § 1.2 says the critic's clean `-O1` to `-O3` is clang inlining
  `cd_through` within one module. Refuted by its own `q1/two-module-routeA-B.txt` (two modules, no
  inlining possible: 5 reports at `-O0`, **0** above) and by the compiler-engineer's `-fno-inline`
  rows (`msan-ce-x86.txt`: `small.hero -O1/-O2/-O3 -fno-inline`, 0 each).
- **Directly** (`build/mech/mech.sh` in `heroes-linux`, output `build/mech/mech-x86.txt`): the
  runtime's `hero_lend_local` re-poisoned for MemorySanitizer right after its `memset(local, 0,
  size)`, the critic's `small.hero` unchanged:

  | runtime | `-O0` | `-O1` | `-O2` | `-O3` | `-O2 -fno-inline` |
  |---|---|---|---|---|---|
  | today's | 3 (lines 29, 38) | 0 | 0 | 0 | 0 |
  | the region re-poisoned | 3 (lines 29, 38) | **2 (line 29)** | **2 (line 29)** | **2 (line 29)** | 3 (lines 29, 38) |

  (Reports counted by `WARNING: MemorySanitizer`, and the `.hero` lines found in the four frames
  after each; the rest of each report's frames are in `small.h` and the runtime.)

  Line 27 (`a` built in place and lent, the control) stays clean under the poisoned runtime. So
  the by-value call loses the bytes at every level, and **R7's zeroed region is what hands C zeros
  above `-O0`**: the compiler-engineer's reading, not the ffi-pragmatist's.

**Which programs read a wrong VALUE, not only a report.** Re-run here, the ffi-pragmatist's
`main.hero` (`build/mech/main/`) built plain, no sanitizer, each binary run three times:

| level | run 1 | run 2 | run 3 |
|---|---|---|---|
| `-O0` | `memcmp 0 -224 0`, `sha true false true` | `memcmp -116 0 -247`, `sha false true false` | `memcmp 0 -224 0`, `sha true false true` |
| `-O2` | `memcmp 0 0 0`, `sha true true true` | the same | the same |

`eq true true true` in all six. So at `-O0` the **same binary** prints different answers from one
run to the next for records `==` calls equal, at exit 0. That level is not a corner:
**`heroes build` and `heroes test` compile at `-O0` by default**, and `heroes run` at `-O2`
(`./heroes help`: *"-O0 compile at -O0 (the default here)"* under `build` and `test`, *"-O2 ...
(the default here)"* under `run`). The compiler-engineer's `-O2 ... where `heroes run` compiles`
is right of `run` and silent about `build`; the critic's pass 1 called `-O0` the level where the
bytes are lost as if it were the rarer one. At `-O2` the value read the same three times; whether
a dirtier stack makes `-O2` print a different value is *unrun*. The other wrong values in this
sitting are a route's, not today's: the naive G's `1432778632` (the compiler-engineer's) and the
gap macro's on `unimid.hero` (above).

## 3. Contradiction 5: the formatter's deletion is the prototype's, and it turns a refused copy of a locked mutex into a program that builds

**Checked in the copy** (`build/fmtp/`, the compiler-engineer's `pin/p_mx.hero` and `mx.h`, the
prototype compiler `build/heroes-g` built from `prototype.diff` applied to `7e5c02048`, real 111.6
s):

| command | result |
|---|---|
| `heroes-g fmt p_mx.hero > out.hero` | exit 0; `diff`: `record Box tag box partial pinned` became `record Box tag box partial` |
| `heroes-g parse p_mx.hero --dump-ast` | `extern "mx.h" record Box tag box partial`: the dump does not print the word either |
| `heroes-g build p_mx.hero` | `error[pinned_copied]: a pinned record is not a value ...`, at `p_mx.hero:15:16` |
| `heroes-g build out.hero` | **`wrote b`**: the formatted program, which copies a locked mutex, builds |

**Whose it is**: the prototype's. It added the word to the parser and the AST
(`parse/records.hero`, `ast.hero`) and to neither `print/fmt.hero` nor `print/dump.hero` (`grep -n
pinned selfhost/print/*.hero`: no hit but an unrelated comment). Today's formatter keeps every word
the tree uses: the `canonical` suite holds every tracked `.hero` to `fmt(file) == file`, which is
how the compiler-engineer found the seven kept programs not canonical.

**Why today's guard did not stop it**, the known class: the guard's *"same TREE"* check is
`dump(text) == dump(fmt(text))` (`selfhost/print/guard.hero`), and the dump is a renderer kept in
step with the AST by hand, so a field the dump does not print is a field the guard cannot see.
`.claude/rules/diagnostics-and-goldens.md:103-106` already says so (*"it agreed with itself while
the formatter deleted a word"*, CL-036). **What the measurement adds** is the consequence for
this sitting: for route B, the deletion is not cosmetic. A refusal that lives in one word is undone
by the one tool design.md §4.15 makes every diff pass through, at exit 0, and the output is the
wrong program the refusal exists to stop. So B's landing owes `fmt.hero` and `dump.hero` the word
**in the same commit as the checker**, a golden carrying it (so `canonical` reads it), and the rest
of CL-036's walk; the compiler-engineer's ≤ 450-line prediction names `print/` and does not price
that walk. Route C, which has no word, has nothing a formatter can drop.

## 4. Contradiction 4: the whole-record copy on a field read is real at `-O0` only, invisible to ThreadSanitizer, and no seat built it

**Read in the emitted C** (`build/race/`, the critic's `taskB/b.hero`, `heroes build --emit-c`):
inside `bump(@c)`, `c.n @ c.n + 1` is `t2 = (*ph0_c); t3 = t2.n; ... (*ph0_c).n = t5;`. The
**write** goes back in place, one field; only the **read** copies the whole `struct counter`, mutex
included, after `counter_lock` and while the lock is held.

**What the machine does with it** (`clang -S` on that C, this Mac, `build/race/b-O0.s`,
`b-O2.s`): at `-O0`, `bl _memcpy` of the whole record after `_counter_lock`; at `-O2`, one `ldr x8,
[x19, #64]`, the field alone. So the copy exists at the level `heroes build` uses by default (§ 2)
and not at `heroes run`'s.

**A second thread, built** (no seat did: the spec-warden's *"a question, unrun"*, the
ffi-pragmatist's *"a marked record's field should be read in place"*, the compiler-engineer's B
prototype *"0 lines in the emitter"*). Heroes threads take one `i64` (`hero_thread_spawn(body:
(function(i64) -> i64), arg: i64)`, `tests/golden/run/fixedbugs-382-*`), so two Heroes threads
cannot share a record's place; a C thread the header starts can, through `@`, which spec § 13
allows while the place does not move. `build/race/race.hero` over `race.h`: a C thread locks,
increments and unlocks `c` 200,000 times while Heroes reads `c.n` 100,000 times, **every read under
the lock** (a correct program). Its `peek` emits `t2 = (*ph0_c); t3 = t2.n;`. Under
ThreadSanitizer in `heroes-linux-arm64` (`build/race/tsan.sh`, output `tsan-arm64.txt`):

| level | exit | printed | TSan warnings |
|---|---|---|---|
| `-O0` | 0 | `200000 true` | **0** |
| `-O2` | 0 | `200000 true` | **0** |

So no value moved and no instrument here reports it. In C11's terms a plain read of the mutex's
bytes while another thread's `pthread_mutex_lock` writes them atomically is a data race
(§5.1.2.4p25): *a reading of the standard, not a measurement*; ThreadSanitizer models a lock as an
atomic read of its address and does not instrument libc's own stores, which is why it is silent
(*an inference from its design, unread in its source this session*).

**Does an adopted route make it matter?** Not as a value, by every run here. As a promise, yes:
route B's sentence (*"is a place and never a value"*, the spec-warden's `q2-b-mark`) is false of
the emitted C at `-O0` for every field read through `@`, and so is H's guarantee if it says the
emitter reads no member it was not asked for. The repair is the emitter's (a field read in place,
`(*ph0_c).n`), unpriced by every seat, and owed by B before its sentence, by CLAUDE.md § 12 (*spec
beats compiler*).

## 5. Q1 by value, with no sanitizer, on all four legs: the Windows box was unrun by every seat

The compiler-engineer and the ffi-pragmatist both left Windows *unrun* (no MemorySanitizer target
there, measured by the compiler-engineer) and inferred from Microsoft's convention that it keeps
the bytes; the compiler-engineer listed *"any sentinel-value probe without MemorySanitizer"* as not
run. A sentinel needs no sanitizer: the caller zeroes the record whole, fills the stack below with
`0xAA`, passes it by value to another translation unit and takes it back by value into a slot it
also filled with `0xAA`; each side counts the padding bytes that are not the caller's zeros
(`build/sentinel/s_main.c`, `s_through.c`, `sentinel.sh`, pure C, no Heroes):

| leg | `-O0` callee / caller | `-O1` | `-O2` | `-O3` |
|---|---|---|---|---|
| this Mac, Darwin arm64, Apple clang 21.0.0 | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| `heroes-linux-arm64`, Debian clang 22.1.8 | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| **`heroes-linux`, x86-64**, Debian clang 22.1.8 | **7 / 0** | **7 / 7** | **0 / 7** | **0 / 7** |
| **the Windows box**, MINGW64 shell, clang 23.1.1 (`ssh win`, `/c/w/critic215s`, removed after: `ls /c/w \| grep -c critic215` reads 0) | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |

So **Windows x64 keeps the bytes at every level, measured**, and the class is SysV x86-64's alone
on this project's legs (macOS x86-64 shares that convention and is unrun, no such machine here). On
x86-64 the loss is a **value** at every level, `-O2` included, with no sanitizer: the stack's
`0xAA` arrives in the record's padding on one side of the call or the other.

**In a Heroes program** (`build/padc/dv.hero` over `dv.h`, run by `dv.sh` in `heroes-linux`, three
runs each; output `build/padc/dv-x86.txt`): `main` calls a C `dirty()` that fills the stack with
`0xAA`, then `work()`, whose `a` is built with `rest: zero`, crosses `cd_through` by value into a
Heroes record's field, and is lent to a C function counting nonzero padding bytes:

| compiler | `heroes build -O0` (the default) | `heroes build -O2` |
|---|---|---|
| today | `true 0 7`, three times | `true 0 0`, three times |
| the compiler-engineer's prototype G | `true 0 0` | `true 0 0` |

`x.h == a` is `true` and C reads **7 nonzero bytes, the stack's own**, out of the copy, at the level
a plain `heroes build` uses: CERT DCL39-C's leak, by value, deterministic once the stack holds
something. The first version of this probe, with the work in `main`, read `true 0 0` at both levels
(`main`'s frame sat on fresh zero pages above what `dirty()` touched), which is why a clean reading
proves nothing without a dirtied frame. A Heroes program showing a nonzero value at `-O2` stays
*unrun* (C alone shows it, above).

## 6. Q2, contradiction 3: what B and C each rest on, one of C's objections measured away, and the evidence that would decide it not gathered

**What each side rests on, read in the reports.**

| seat | route | rests on |
|---|---|---|
| spec-warden | C, no sentence (0 tokens) | Principle 0: a word is a form and needs a thesis effect, a refusal adds none (panel 010); B *"protects only bindings whose author already knew"*; turns to B if Task B reads ≥ 3 of 10 copying **and** C cannot decide without a list the code cannot keep |
| compiler-engineer | B | C is *"a per-libc name list"* (Darwin's dump spells the mutex `struct _opaque_pthread_mutex_t`, glibc's `pthread_mutex_t`); header facts reach `build`, not `check`; C needs a dump the screen exists to avoid |
| ffi-pragmatist | B, C as a note | no header marks a type non-copyable (grepped); the dump spells each lock three ways; a list is blind to `z_stream` and `TAILQ_HEAD` |
| historian | B, widened to moves and arrays; C seeded from POSIX §2.9.9 | Ada 83's `limited`, GNAT's `pthread_mutex_t`, C++ `= delete`, Swift `~Copyable`; Zig's refusal the counter-precedent |
| llm-ergonomist | object to a mark *"on this evidence alone"* | Task B: 0 of 10 copied |

**Measured: the "three spellings" objection belongs to reading the dump's printed text, not to
route C.** Given a member's name (which the dump gives), clang answers by **type identity** against
the standard's own typedef, one spelling per type: `__builtin_types_compatible_p(__typeof__(((T
*)0)->M), pthread_mutex_t)`. `build/q2c/ident.c`, four `_Static_assert`s (the lock is a
`pthread_mutex_t`; `n` is not; a nested `inner.lock` found by its path; a mutex is not a
`pthread_cond_t`): all hold on this Mac (`clang -std=gnu11 -Wall -fsyntax-only`) and in
`heroes-linux-arm64` (glibc); `build/q2c/identw.c` with `CRITICAL_SECTION` and `SRWLOCK` holds on
the Windows box (clang 23.1.1; its folder removed). What stays of C's objections: it needs a member
name for every lock, and **a lock can only sit in a `partial` record or in a record bound after a
lock's own typedef** (no field type names it, spec § 13; the compiler-engineer's `empty_record` on a
fieldless one; `ffi-a-typedef-the-note-names-binds-by-its-own-name` binds glibc's union by
`__size`), so C dumps every `partial` record, today dumped only *"when the program builds it from
two or more fields"* (`emit/layout_screen.hero`). That population is **37 `partial` record lines in
35 files** outside `docs/`, one in `examples/` (`git grep -h -E '^[[:space:]]+record
[A-Za-z_0-9]+.* partial' 7e5c02048 -- '*.hero' ':!docs' | wc -l`; the compiler-engineer counted the
same 37). And it is told at `build`, as every other header fact is (`ffi_unknown_field`, the
critic's pass-1 `v2.hero`). The list itself (POSIX §2.9.9's set, C11 `mtx_t` and `cnd_t`, the
Windows locks) stays a premise about the world, and blind to `z_stream` as B is blind to it unless
its binding's author knew.

**A route nobody listed, built and censused: a `partial` group record is not a value.** No new
word, decided at `check`, and it reaches the shape every reader of Task B wrote (all ten bound
`record Counter tag counter partial`, `grep -h "record .*counter" r*/b.hero`) and the brief's route
A2 (the lock left undeclared). The compiler-engineer's prototype with `rec.pinned || rec.partial` in
both its tests (`build/heroes-p`, rebuilt from the prototype, real 69 s), `heroes check` over the
905 files with an `extern` (the brief's command, its `7e5c02048:` prefix stripped; the first run
read every file as missing, exit 2, and is discarded), eight at a time, real 45.0 s: today **732
exit 0, 173 exit 1** (the compiler-engineer's figures), the variant **720 and 185**; **14 files
move**. It refuses the critic's unmarked `copy.hero` (10 refusals) and keeps `b.hero` and
`race.hero`. What its 14 show is the cost of any rule that refuses copies by record and not by
use:

- `examples/ctime/main.hero`, **30 refusals**, every one a copy of a `partial` `struct tm` with
  no lock in it: read-only helpers taking it by value (`function year_of(b: Broken) -> i64`, lines
  69 to 89) and their arguments, a constructor helper's return (`return ok(Broken(...))`, line 87)
  and the bindings of its result (`landing = made(...).must()`, lines 100 to 164): harmless
  copies;
- `tests/golden/run/fixedbugs-413-zlib-knows-a-stream-by-its-address.hero:36` and
  `fixedbugs-413-libuv-keeps-every-handles-address.hero:39`: `zs: [ZStream] @= [ZStream(rest:
  zero), ...]`, records **whose address C keeps, held in an array on purpose** and lent element by
  element, which panel 196's R1 made correct (the golden's header: `0 -2 -2` until R1). **Measured
  with B itself**: the golden copied to `build/z413/z.hero` with `pinned` added to `record ZStream
  tag z_stream_s partial`, `heroes-g check` gives 2 `error[pinned_copied]` at line 36, where the
  unmarked golden exits 0. So **a `z_stream` marked under B refuses the tree's own repair of defect
  413**, and leaving it unmarked leaves the
  array's growth, which spec § 13 says moves the place, unrefused. No seat priced this: the
  historian asks for arrays refused, the spec-warden's draft refuses *"holding it in an array"*,
  and the compiler-engineer's census could not see it because no record in the tree carries the
  mark (its *"0 of 905"* measures the absence of marks, not B's reach).

So the variant is too broad as a rule (ctime), and its census is the only measurement in this
sitting of what refusing copies of C-held records would cost real programs.

**The evidence that would decide B against C was not gathered.** Task B's verb (*"increments the
counter's `n` while holding its lock"*) is one only `@` can write; no reader in A1 or B wrote a
helper that returns a record (`grep -E '^function' r*/b.hero r*/a1.hero`: every B helper that
takes the counter takes `@c`, and no A1 file defines a function returning a record). The shapes that copy a lock were not tasked: a
helper that builds and initialises a counter and returns it (the critic's pass-1 `copy.hero`), a
read-only helper taking it by value (which `ctime` shows programs write), a counter kept in a
Heroes record or an array. The ergonomist lists *"a task where a copy of the mutex is the natural
code"* as not run. So the spec-warden's condition (≥ 3 of 10 copying) was scored on a task that
cannot produce the copy, and **0 of 10 neither meets it nor refutes B's thesis effect**. The same
missing task would measure the other side: how often a correct program (a read-only helper) is
refused by B or C, the rewrite turn §1.2 charges.

**And the ranking the dispute is really about.** CLAUDE.md § Precedence puts robustness (3) above
Principle 0 (4); design.md §1.12 says *"It does not suspend Principle 0 ... What §1.12 decides is
the shape of a form already admitted, and which of two admitted shapes wins"* (lines 594-597, read).
The compiler-engineer and the ffi-pragmatist adopt a word on robustness; the spec-warden refuses
the word on Principle 0 and admits the refusal because it adds no form. Both readings are in the
tree; the sitting should say which governs a new word, in writing, since the answer decides Q2
before any measurement does.

## 7. Routes nobody listed, in one place

- **Q1, G with the header's membership written as clang-checked `offsetof`, recursing** (§ 2):
  every C member the dump names, nested members by designator (`inner.d`), elements of array
  members (`a[1].c`), and the elements of an array lend zeroed in the region `hero_lend_take`
  fills. It answers the ffi-pragmatist's *"written from the dump unverified"* and closes the two
  shapes the prototype leaves open. *Unbuilt*; its line count is outside the compiler-engineer's
  ≤ 250 prediction, which priced a non-recursive G.
- **Q1, the historian's F: refuse a padded record lent to C as bytes** (`void *`), zerocopy's and
  bytemuck's rule. Listed by the historian alone and priced by nobody: what it refuses today is
  every binding the ffi-pragmatist wrote for `write`, `send`, `memcmp` and `SHA256_Update` (`@buf:
  rec.Cd counted_by n lent` against `const void *`), so it is a refusal of correct programs unless
  the record spells its padding (route E, which § 13's *"a group's record is the header's struct"*
  refuses). *Unmeasured*.
- **Q2, C decided by type identity** (§ 6): the dump's member names, and clang asked
  `__builtin_types_compatible_p` against POSIX's, C11's and Windows' own typedef names. Measured on
  three platforms; costs a dump of 37 `partial` records in 35 files; told at `build`.
- **Q2, a `partial` group record is not a value** (§ 6): built and censused, 14 of 905 files move,
  too broad as a rule (`examples/ctime`'s 30 harmless copies of a `struct tm`).
- **Q3, R1's sentence says it reaches the whole object, and the union sentence names the header's
  macro route** (§ 1): what four readers looked for and nine readers lacked. *Unpriced* by the
  spec-warden's drafts, which only shorten R1.

## 8. Claims still asserted and not measured, after this pass

- **The compiler-engineer's ≤ 250-line prediction for G** covers the shapes it built; the nested
  and array shapes are outside it, measured open (§ 2).
- **The compiler-engineer's ≤ 450-line prediction for B** names `print/` and does not price
  CL-036's walk (the dump, `mutate`, the probe's reader, the editor grammar, the site's
  highlighter), which § 3 shows is not optional for a word whose loss turns a refusal into a build.
- **"0 of 905 move" under B** (compiler-engineer) measures that no record in the tree carries the
  mark, not what B refuses once records are marked; the variant census and the marked defect-413
  golden (§ 6) are the only readings of B's reach.
- **The ffi-pragmatist's `-O0`-only attribution to inlining** is refuted (§ 2); its *"1334 shows
  at every level across modules"* is right, and for the reason the compiler-engineer gives (places
  outside a lend region), not because modules are separate units.
- **The llm-ergonomist's attribution of A2's misses to defect 2062's message** cannot hold for a
  first try (no reader saw a compiler), and defect 2062's *"false message"* is contradicted by clang
  (§ 1).
- **The spec-warden's `q2-at-place` premise, *every `@` lowered by address***, which it ran for a
  local, a field and an element: run here for a generic call, by address
  (`h_atp_grow_242d26d(&h1_ps, t4)`, `pass2/atp/atp.c`); a method call with `@` is refused by the
  language (`error[ufcs_on_mutable]`), a function type cannot carry `@` (`error[expected_type]`),
  and a map lookup is `P?`, not a place (`error[type_mismatch]`). So it holds on every shape the
  language has. *Unrun*: across two modules.
- **The spec-warden's real-token bands** stay bands (no `--refresh`); unchanged by this pass.
- **macOS x86-64**: both seats infer it loses the bytes as Linux x86-64 does; *unrun*, no such
  machine.

## 9. The questions the sitting should have asked and did not

1. **Does the paid run's A2 (1 of 10 first try, R1+K, a union task) fire panel 194's *"nothing
   becomes my verdict"* row**, and is the miss R1's, panel 186's union sentence's, or the task's
   (§ 1)? No seat that adopted Q3's A cites the number.
2. **Which governs a new word, CLAUDE.md's ranking (robustness above Principle 0) or design.md
   §1.12's *"does not suspend Principle 0"*** (§ 6)? Q2's B against C is that question.
3. **What does a reader write when copying the lock is the natural code** (a constructor helper,
   a read-only helper, a counter kept in a record or an array)? The only instrument that scores
   B's thesis effect, and C's and B's false refusals, was not tasked (§ 6).
4. **May a record whose address C keeps live in a `[T]`?** The tree says yes on purpose (defect
   413's goldens) and spec § 13 says growth moves it; B as prototyped says no (§ 6).
5. **Which level does the class bite at?** `heroes build` and `heroes test` default to `-O0`,
   where the bytes reach C as values today (§ 2, § 5); the sitting read `-O0` as the corner and
   `-O2` as the real case.
6. **What G owes before its sentence or its rule lands**: the recursion and the array lend (§ 2),
   since H's guarantee in `generated-c.md` would be false of the prototype on two measured shapes.

## 10. Not run by the critic

Any paid reader run (forbidden; § 6's task and § 1's re-run are the coordinator's to weigh); the
recursive G; G or any route on the Windows box beyond the sentinel (no MemorySanitizer there);
macOS x86-64; a Heroes program showing a nonzero padding value at `-O2` (C alone shows it); the
type-identity route as a compiler change (only its C question was asked, on three platforms); the
full net with any prototype; real token counts (`--refresh`, not the critic's).
ThreadSanitizer's own source was not read; why it is silent (§ 4) is an inference from its design.

**Where the files are.** Every `build/<dir>/` path above named a folder of the critic's copy; each
is kept as `docs/panel/215-reports/critic-work/pass2/<dir>/` (`padc`, `mech`, `race`, `sentinel`,
`q2c`, `fmtp`, `z413`, `atp`, `census`, `a2`), sources, scripts and outputs, no binary. Not kept:
the two compilers (`build/heroes-g`, the compiler-engineer's `prototype.diff` with `padding.hero`
and `pinned.hero` applied to `7e5c02048` and built by `heroes build selfhost/main.hero`; and
`build/heroes-p`, the same with `pass2/census/partial-variant.diff`), and the prototype's emitted
C used in the containers (`build/g2c/heroes.c`, 58,767,644 bytes, `heroes-g build selfhost/main.hero
--emit-c`), each remade by the command given. No file of the critic's was written outside the
repository's root (clang's own intermediate objects, which `clang a.c b.c -o x` puts in the
system's temporary directory and removes, aside; the containers' `/tmp` is the container's); the
Windows box's two folders, `/c/w/critic215` and `/c/w/critic215s`, were removed by the same command
that used them.
