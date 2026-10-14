# Panel 215, the spec-warden's drafts for Q3: the union shapes measured, two sentences priced, the re-run's tasks

Written 2026-10-14 from about 11:45, on `215-briefs/draft-q3-spec-warden.md`, after reading
`critic-reports.md` whole and the correction at the end of `llm-ergonomist.md`. My copy is
`.claude/worktrees/scratch-panel-215/sw-build` at `7e5c02048`, its compiler built from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, real 15.01 s, `heroes 0.4.0`).
Offline only: no `--refresh`, no paid run, no GitHub write, no credential. Every number has its
command beside it. What the copy built is kept in `spec-warden-work/q3/`, and the copy is removed at
the end.

design.md §1.6, grepped today: line 253 is the heading, and line 255 reads *"must fit in 10240"*.

## 1. Every union shape the language accepts, built and run on this Mac

Each probe is a program in `build/q3/` of my copy, built with `./heroes build <f>.hero -o <f>` (the
default level, `-O0`) and run. Kept in `spec-warden-work/q3/` (sources, headers, build transcripts).
Two headers: A2's `union.h` byte for byte (`cmp` against `.claude/worktrees/readers/kit/union.h`:
same), wrapped by `dirty.h`, which adds a 4096-byte `0xAA` fill of the stack and a count of nonzero
bytes over `sizeof` the whole object; and `shapes.h`, one struct per shape beside A2's, every
struct 16 bytes on this Mac but `s5` and `s6` (8), by a C `sizeof` compiled here. "Nonzero" counts
every byte of the object after `dirty()`, padding included; the one nonzero byte in each row is
`kind`'s 2 or 1.

| shape (header) | the binding that builds | what it prints | the wrong spelling, and what it meets |
|---|---|---|---|
| **A2's own: a named member of an untagged union**, `struct tagged { int kind; union { int fd; void *handle; } via; }` | `record Tagged tag tagged partial` naming `kind`, built `Tagged(kind: 2, rest: zero)` (r08's binding) | `2 0 1 of 16`: `tg_kind` 2, `via` 0 nonzero bytes, the whole object 1 | `fd: i32`, `handle: ptr` flat (r01's program verbatim): **`ffi_unknown_field`** twice, *"`union.h` declares no `fd` in `struct tagged`"*, its note *"correct the spelling, or name the header whose `struct tagged` has a member `fd`"*. `kind` alone, not `partial`: **`ffi_incomplete_record`**, *"`Tagged` does not name `via`"*. `via: i64`: **`ffi_field_type`**, *"`Tagged.via` is `union (unnamed union at ./union.h:3:5)`"*. `via.fd: i32`: **`expected_field_type`**, a parse error at the `.`. **None of the four notes names `partial`** |
| the same, **through a header macro** in a header of the program's own (`#include "union.h"`, `#define fd via.fd`, `#define handle via.handle`) | `record Tagged tag tagged` naming `kind`, `fd`, `handle`: the nine readers' program, unchanged | `2`, `0`, then `Tagged(kind: 3, fd: 7)` prints `3 7` | (as above, without the macro) |
| the same, **a typedef of the member's type** in a header of the program's own (`typedef __typeof__(((struct tagged *)0)->via) VIA;`) | `record VIA` (`fd`, `handle`) and `record Tagged tag tagged` with `via: VIA` | `2 1` | a route nobody listed; it builds, and it rests on GNU's `__typeof__`, so no reader is taught it |
| **an anonymous C11 union**, `struct s1 { int kind; union { int fd; void *handle; }; }` | `record S1 tag s1` naming `kind`, `fd`, `handle`; `S1(kind: 2, rest: zero)` and `S1(kind: 2, fd: 7)` | `0 1` (`fd` 0, one nonzero byte), then `7 7` | both arms in one construction: **`ffi_union_field`**, *"names `fd` and `handle`, which are the same bytes"*. Neither arm and no `rest: zero`: **`missing_fields`**, *"leaves out `fd` and `handle`"*. `via: i64` on a `partial` record: **`ffi_unknown_field`** (*"declares no `via` in `struct s1`"*) |
| the same bound **`partial`**, `kind` alone, `rest: zero` | builds | `1` | |
| **a named member of a TAGGED union**, `struct s3 { int kind; union v3 { ... } via; }` | `record V tag v3`: refused. With `typedef union v3 V3;` in a header of the program's own: `record V3` (`fd`, `handle`) and `record S3 tag s3` with `via: V3` | `0 1`, then `S3(kind: 2, via: V3(fd: 7))` prints `7 7` | `record V tag v3`: **`ffi_tag_is_a_union`**, *"declares `v3` as a `union`, not a struct"*, its note naming the typedef route and `ptr` (as `tests/golden/unsupported/fixedbugs-157-*` pin) |
| the same bound **`partial`**, `kind` alone, `rest: zero` | builds | `1` | |
| **a typedef'd union**, `typedef union { int fd; void *handle; } VU;`, alone and as `struct s4 { int kind; VU via; }` | `record VU` (`fd`, `handle`) and `record S4 tag s4` with `via: VU`; built `S4(kind: 2, rest: zero)`, `S4(kind: 2, via: VU(rest: zero))`, `S4(kind: 2, via: VU(fd: 7))`, `VU(fd: 9)` | `0 1`, `0 1`, `7 7`, `9 true` | both arms of `VU`: **`ffi_union_field`**, *"`shapes.h` declares `VU` a `union`"*. `fd` flat on `S4`: **`ffi_unknown_field`** |
| **an anonymous struct inside a union**, `struct s5 { int kind; union { struct { int16_t lo; int16_t hi; }; int32_t i; }; }` | `record S5 tag s5` naming `kind`, `lo`, `hi`, `i`; built naming `lo` and `hi`, or `i`, or nothing with `rest: zero`, or `lo` alone with `rest: zero` | `0 1`; `196610` for `lo: 2, hi: 3`; `2 3` read back from `i: 196610`; `2` for `lo: 2, rest: zero` (`hi` zero) | `lo` alone without `rest: zero`: **`missing_fields`**, *"leaves out `hi`"*. `lo`, `hi` and `i`: **`ffi_union_field`**, *"names `lo` and `i`, and `hi` and `i`"* |
| beside it, **a named struct inside an anonymous union**, `union { struct { int16_t lo, hi; } parts; int32_t i; }` | `record S6 tag s6` naming `kind`, `i` (the struct `parts` shares every byte with `i`) | `3` (`parts.hi` of 196610), then `0 1` with `rest: zero` | |

**What the table says about the specification's two sentences.**

- **A field is a name C reads as `s.name` on the struct.** That is what clang is asked
  (`_Static_assert(... ((struct tagged *)0)->kind ...)` in the emitted C) and what decides every row:
  an anonymous union's or struct's members pass; `via.fd` does not; a header macro makes `s.fd` mean
  `s.via.fd` and passes. The specification never says it. Its union sentence says *"A field in a C
  union, named or anonymous"*, and panel 186 meant by *named* a union whose members a header macro
  reaches (`struct sigaction`, `struct in6_addr`; `186-a-field-in-a-c-union-*.md:85-88`), which the
  sentence does not say either. Three readers quote it as licence to write `fd` (r03, r06, r07), and six
  say the specification does not tell how a named union's members are written (r02, r05, r06, r08,
  r09, r10; `grep -l -i -E "doesn't say how|does not say how|unclear whether a named|no way to write
  .via|no syntax|dotted"`, each hit read).
- **A union with a type name is a record of its own and a field's type**; one with only a tag needs a
  typedef (the message says where); one with no name at all, A2's, is reachable only through a macro
  or GNU's `__typeof__`, so the binding to teach is `partial`.
- **`rest: zero` zeroes the whole object, `partial` or not.** The emitted C of A2's `partial`
  binding is `t2 = (struct tagged){0};` then `__builtin_memset(&t2, 0, sizeof t2);`
  (`heroes build s2a_partial.hero --emit-c`, lines 177 and 179), `sizeof` of C's struct, not of the
  field list. Every `partial` row prints one nonzero byte of 16 after a dirtied frame. The promise four
  readers looked for (r02, r03, r06, r07) is kept and not written.
- **No message for A2's shape names `partial`** (four wrong spellings, four codes, above). That is a
  diagnostic's (design.md §4.17), beside defect 2062, and the re-run gives no reader a compiler, so it
  does not move the score. I file nothing (no GitHub write); the coordinator may.

## 2. The two sentences, drafted

**Written after a restart.** The session restarted at about 12:30 (the coordinator's message). Every
file of the copy survived, and the sections below were written from the copy's files, re-read after
the restart.

Every draft is an exact edit of the spec at `7e5c02048` made by
`spec-warden-work/q3/drafts_q3.py`, which asserts that each anchor occurs once. It replaces § 13
from *"elements as the type says."* to *"counting as one."* with one reflow rule: 80 columns, never a
break inside a code span. Reflowing the paragraph unchanged reads **+0 / +0** (`base-rewrap`), so
every delta below is the words alone. Two choices shape all of them:

- **The example names nothing the tasks name.** My first union drafts said `via.fd`, which is A2's
  own member. A specification that names the task's member teaches the re-run its answer, so every
  draft uses `s.u.n`.
- **The sentence leans on the example, not on the word *named*.** A2's `via` is a named member whose
  union type has no name. A reader can call that union *anonymous*, and then *"an anonymous union's
  members are fields"* sends them back to `fd`, the miss itself (r04: *"the union has no tag"*; r07:
  *"its type has no name"*). *"Where C reads `s.u.n`, `u` is the field"* decides A2 without that word.

**R1 moved after `partial` and joined to it.** R1 now follows the `partial` sentence. That puts
*"A bit-field is none of these"* right after the list of field kinds it refers to, and lets R1 name
`partial`, which is what four readers looked for.

### The recommended text, `m-g` (+58 / +58)

> elements as the type says. A bit-field is none of these: leave it to `partial`.
> `record Font partial` names only some, and its size stays C's, not the field
> list's. End a construction with `rest: zero` and every field sharing no byte
> with one it names is zero, those `partial` leaves out included; only a group's
> record has it. A field is a name C reads as `s.name`: an anonymous union's or
> struct's member, or a header macro's. Where C reads `s.u.n`, `u` is the field,
> typed by the record of its union's typedef, or left to `partial`. A union's
> members share its bytes: a record names one or more of each union, reads any,
> and is built naming exactly one, or none when it ends with `rest: zero`, an
> anonymous struct's fields counting as one.

Each clause answers a row of § 1 or a reader's note:

| clause | what it answers | measured where |
|---|---|---|
| *those `partial` leaves out included* | r02, r03, r06, r07: *"unclear whether `rest: zero` zeroes bytes the record doesn't declare"* | `s2a_partial`, `s1_partial`, `s3_partial`: one nonzero byte of 16; the emitted `memset` of `sizeof` |
| *A field is a name C reads as `s.name`* | r02, r05, r06, r08, r09, r10: *how is a named union's member written* | the emitted `_Static_assert` on `((struct tagged *)0)->kind`; `w1_flat`'s `ffi_unknown_field`, which clang states the same way |
| *an anonymous union's or struct's member* | shapes 1, 5, 6 | `s1_anon`, `s5_anon_struct`, `s6_named_struct` |
| *or a header macro's* | panel 186's *named*, libc's `s6_addr` and `sa_handler` | `s2b_macro`; `fixedbugs-151-arms-a-macro-reaches-*` |
| *Where C reads `s.u.n`, `u` is the field, typed by the record of its union's typedef* | shapes 3 and 4; glibc's `struct epoll_event` holds `epoll_data_t data`, a typedef'd union (`sys/epoll.h:77-88` in `heroes-linux-arm64`, read, not built) | `s4_typedef`, `s3_own_typedef` |
| *or left to `partial`* | A2 | `s2a_partial`, r08 |
| *A union's members share its bytes: ...* | the opening shortened from *"A field in a C union, named or anonymous, shares its bytes with the union's other members"*; from *"a record names one or more of each union"* on, panel 186 R7's words unchanged, which `ffi_built.hero:216`'s note quotes | unchanged |

What it keeps: *"every field sharing no byte with one it names is zero"*, which `zeroed.hero:17`
and two goldens' comments quote. What it drops: *"named or anonymous"*, which nothing in
`selfhost/`, `tests/harness/`, `site/src/`, `editors/` or `.claude/` quotes (`git grep`; the only
hits are the spec and the ledger's row 6928).

**Held to the instruments before it was priced.** I applied `m-g-at` (the text above plus § 9's
rewrite) to my copy's `spec/heroes-spec.md` and ran the suites the spec row of `verification.md`
names, then restored the copy (`af7fc4d6a85fc786` again):

- `spec`: **27 passed, 4 failed**. The four are `budget`, `spendable`, `real` and `ledger`, the pins
  that move with any change to the text and are moved in the landing's commit (their own messages
  say so). `named`, `rejected`, `inventory`, `shape` and `anchors` pass.
- `grammar` 9, `fixes` 996, `special` 10, `unseen` 3, **each 0 failed**.

### The cheaper tiers, priced, for the conservative record (CL-040)

- **`m-c2` (+35 / +35).** It keeps the rule and drops the routes: *"A field is a name C reads as
  `s.name`, an anonymous union's or struct's member or a header macro's, never `s.u.n`."* The reader
  must find `partial` or the typedef's record alone. It carries R1 as *"every field of C's struct
  ... those `partial` leaves out included"*, 3 to 4 tokens dearer than `m-g`'s R1.
- **`m-anon` (+5 / +5), the floor.** It repairs R1 as `m-g` does and cuts *"named or anonymous"* to
  *"A field in an anonymous C union shares its bytes ..."*. Nothing in it is false. It stops the
  misreading three readers made, and it says nothing of typedef'd unions or a macro's member, which
  today's *named* covered in panel 186's sense.
- **R1 alone**, under the union sentence as it is: `r1-short-only` **+8**, `r1-b` (*"declared or
  not"*) **+9**, `r1-a` **+12**, `r1-c` (the promise carried by the `partial` sentence) **+12 / +11**.
  These answer the four readers' `partial` notes and leave the *named or anonymous* misreading in
  place.

## 3. Prices, the band, and which removal pays

`./heroes measure` on each draft in my copy, against **7519 / 7655** (the full list of 38 is
`spec-warden-work/q3/measure_q3.txt`). The **real** column is a band and not a reading. It is the
larger vendored delta times the tree's own ratios, the same as in my first report: ×1.00 (the
floor), ×1.31 (this document's own ratio, 10048 / 7655) and ×1.60 (the ledger's top). **The real
count needs a `--refresh`, which is the coordinator's.** Spendable: **132** (10240 − 60 − 10048).

| draft | what it is | legacy | cl100k | real band (×1.00 / 1.31 / 1.60) | sha256 |
|---|---|---|---|---|---|
| **`m-g`** | the recommended text | +58 | +58 | **≥58 / 76 / 93** | `6ba55dc5452e8af6` |
| **`m-g-at`** | `m-g` with § 9's rewrite | **+49** | **+48** | **≥49 / 64 / 78** | `ac4ff3e263629118` |
| `m-g-nogroup` | `m-g` without *"only a group's record has it"* | +50 | +51 | ≥51 / 67 / 82 | `1a8eafa220f5d8a2` |
| `m-g-nogroup-at` | both removals | +41 | +41 | ≥41 / 54 / 66 | `b83228885f670d24` |
| `m-k` | `m-g` with *"typed by its union's typedef"*, 5 tokens shorter | +53 | +53 | ≥53 / 69 / 85 | `f98267ff7c8332b8` |
| `m-g-nomacro` | `m-g` without *"or a header macro's"* | +53 | +54 | ≥54 / 71 / 86 | `308cbc217bf02197` |
| `m-c2` | the rule, no routes | +35 | +35 | ≥35 / 46 / 56 | `bd58c1e447cbfe0c` |
| `m-anon` | the floor | +5 | +5 | ≥5 / 7 / 8 | `20064a56f98eb6a6` |
| `r1-short-only` | R1 alone, merged | +8 | +8 | ≥8 / 10 / 13 | `40712f82a82ac6ca` |
| `at-alone` | § 9's rewrite alone | −9 | −10 | returns ≈10 / 13 / 16 | `a046db95a2a66cdc` |

**No veto.** The dearest recommended composition, `m-g`, reaches **10141** at the top of the band,
under the **10180** where the check goes red. `m-k` saves 5 by dropping *"the record of"*, the words
that tell a reader a union's typedef is bound as a record. I keep them (CLAUDE.md § Precedence,
2026-09-28: the most robust route, at the spec's tokens too).

**Which removal pays.**

- **§ 9's `@` rewrite still applies, and now it has a reading.** The anchor *"Semantics: copy in,
  copy out (copy-out always happens, including on early return and `?`)"* occurs once at
  `7e5c02048` (the script asserts it), and the rewrite returns **−9 / −10**. **All ten B readers**
  cite copy-in/copy-out as a risk to the lock (`grep -l -i -E "copy.in.*copy.out|copy-in/copy-out|copies
  in and copies out" r*/notes.md`: 10 files). The emitted C passes the place itself:
  - in one module, a local, a field and an element (my first report);
  - **across two modules**, the critic's open question, measured here: `build/q3/twomod/`,
    `use lib`, emits `h_lib_bump(&h0_c)`, `h_lib_bump(&h1_h.f_left)` and the element's own slot,
    and prints `1 11 21 1`. It is one emitted unit holding both modules.

  It changes no program here: all ten readers wrote `@c`. What it changes is the reason a reader has
  to prefer `@` to a copy, so it matters to the third task (§ 4). **What it owes**: the
  compiler-engineer's yes, and four messages that say *copy-out* (`resolve/errors.hero:202`,
  `resolve/writes.hero:307` and `:311`, `resolve/written_nowhere.hero:79` and `:96`). None of them
  is false, since the meaning is one inside Heroes (`ir/inout.hero:11-15`), but they would speak
  another vocabulary than the spec.
- ***"only a group's record has it"* returns −7 / −8.** It prevents only a loud error
  (`rest_outside_a_group`), so panel 009's allocation rule allows the cut. No task of the re-run can
  measure what the cut costs, since every task builds a group's record. **I hold it in reserve**, not
  in the recommended text.
- **Neither covers the addition.** `m-g-at` is still **+49 / +48** after § 9's removal. The rest is
  paid by the registered prediction in § 5, scored by the re-run, an instrument that exists today
  (`llm-ergonomist-work/run_one.sh`, and `spec-warden-work/q3/kit/judge.sh`).

## 4. The reader task for the re-run, with its answer key

All of it is in `spec-warden-work/q3/kit/`:

- `brief.md`: 215's brief word for word, the file names now `a2.hero`, `u.hero` and `c.hero`;
- `tasks.md`;
- `union.h` and `counter.h`, `cmp`-equal to the kit's;
- `key/a2.hero`, `key/u.hero` and `key/c.hero`, each built and run here;
- `judge.sh`.

**The specification the readers get.** `python3 -I
docs/panel/215-reports/spec-warden-work/q3/drafts_q3.py spec/heroes-spec.md <dir>` at `7e5c02048`
writes `<dir>/m-g-at.md` (sha256 `ac4ff3e263629118`). Without the § 9 rewrite it is `<dir>/m-g.md`
(`6ba55dc5452e8af6`). **I recommend `m-g-at`.** Task C asks whether a reader copies a lock, and under
§ 9's *copy in, copy out* a reader has no reason to prefer `@` to a copy. The old text would score
§ 9's sentence as much as the language. A2 and U pass `@` only to C, which § 13 already says
receives the place, so they read the same under either text.

**The three tasks** (`tasks.md`):

- **A2**: 215's words and `union.h`, byte for byte, so the score compares with 1 of 10.
- **U**: panel 178's Task 1 word for word (`178-briefs/llm-ergonomist.md`), the task panel 194
  registered (*"178's Darwin `utsname` task"*, `194-reports/spec-warden.md`, the verdict table). The
  only words changed are the kit's opening sentence, which now says that `u.hero` uses the system's
  `sys/utsname.h`. The SDK's header on this Mac declares the five `char[_SYS_NAMELEN]`, and
  `_SYS_NAMELEN` is 256 (`sys/utsname.h:72-79`), as the task quotes.
- **C**, new, the critic's § 6 question. The words describe behaviour and never prescribe a copy:
  *"a function that sets up a new counter for its caller"* (a return, or a fill in place), *"a
  function that gives a counter's value doubled ... without changing the counter"* (a by-value
  parameter, or `@`), and two counters kept *"in a record of your own, `Stats`"*. Expected output:
  `6`, `4`, `5`.

**The answer key**, each line measured at `7e5c02048` on this Mac:

| task | correct first try | the key | what to count beside it | the wrong spellings, measured |
|---|---|---|---|---|
| A2 | builds against `union.h` unchanged, prints `2`, exit 0. A header of the reader's own counts only if it includes `union.h` unchanged and adds only macros or a typedef | `record Tagged tag tagged partial` naming `kind`, `Tagged(kind: 2, rest: zero)` (`key/a2.hero`). Also correct: the macro route and the `__typeof__` route (§ 1) | the binding: `partial`, flat, typed or other (the judge's `form`); whether the notes cite the new sentences | flat `fd`, `handle`: `ffi_unknown_field`; `kind` alone without `partial`: `ffi_incomplete_record`; `via: i64`: `ffi_field_type`; `via.fd`: `expected_field_type` |
| U | builds against the system's `sys/utsname.h`, prints `Darwin`, exit 0 | five `i8[256]`, `Utsname(rest: zero)`, `uname(@u)`, `u.sysname.validated_bytes().must()` (`key/u.hero`). Also correct: `partial` naming `sysname` alone (`u_partial.hero`, prints `Darwin`) | **`u8`** on any of the five fields, panel 194's ≤ 2 of 10 (the judge's `u8`) | `u8[256]`: `ffi_field_type` five times, each with K's note *"where the header writes `char`, the field is `i8`"*; a short literal `[0]`: `fixed_array_length` |
| C | builds against `counter.h` unchanged, prints `6`, `4`, `5`, exit 0 | `record Counter tag counter partial` naming `n`; `Stats` built from `Counter(n: 0, rest: zero)` twice; `set_up(@s.first)`, `bump(@s.first)`, `doubled(@s.first)`: every lend by address (`h_ckey_set_up(&h0_s.f_first)`), no initialised lock ever copied (`key/c.hero`) | **a copy of an initialised lock**: after `counter_init` ran on it, a `Counter` (or a value holding one) is returned, passed to a parameter without `@`, bound by `=` or `@=` from a name, field or element, or put by value into a record or array. A construction `Counter(...)` before the init is no copy. **And B's or C's false refusal**: a program that copies a lock record only before the init, which B and C as drafted would refuse | the copying program `c_copy.hero` (a constructor returning the lock, a by-value reader, `Stats` built from returned values) **builds at exit 0 and prints `6 4 5`, with no message**, its C `struct counter h_ccopy_new_counter(void)` and `h_ccopy_doubled(struct counter h0_c)` |

**The judge**: `sh judge.sh <heroes> <reader-folder> <kit-folder>` prints one TSV row per reader. Run
here on the key: `a2=ok form=partial u=ok u8=no c=ok ret=-;byval=-`. On r01's A2, `u_u8.hero` and
`c_copy.hero`: `a2=build:ffi_unknown_field form=flat u=build:ffi_field_type u8=yes c=ok
ret=h_c_new_counter,;byval=h_c_doubled,`. Each binary runs under `timeout 20`, its output cut at
4096 bytes (`verification.md` § A run that may not end), since a copied locked mutex can block. **A
copy by `=` leaves no signature in the C, so a person reads every `c.hero`.** The judge's `ret` and
`byval` are hints, never the score.

**Found while building the key, for Q2.** My first key passed `@s` and `@s.first` to one call, and
`aliased_mutable_arguments` refused it, as it should. Its first `fix (guess)` reads *"pass a copy
for one of them: declare a second cell holding the same value, pass `@` that cell, and write it
back after the call"*. For a record that holds a lock, that guess is a copy of the lock. A reader
with a compiler would be told to write the copy that issue 1335 measured. Under route B or C the
guess would itself be refused. It is the compiler-engineer's, and I file nothing.

**What the re-run does not score.** Panel 194 registered three arms: L, *R1 honest* and *R1 honest
with K*. This re-run is one arm, the repaired *R1 with K*. Of 194's thresholds it scores the
first-try half on its union task and the `u8` half on its `utsname` task. The L and *R1 honest* arms
stay owed, about twenty more readers (near 4 USD at the first run's 0.17 to 0.22 a reader), a paid
run for the author.

## 5. The verdict, in the seat's structure

- `verdict`: **approve `m-g-at`** as the text the re-run reads, provisional on its score, with
  `m-g` if the sitting does not adopt § 9's rewrite. **Object** to the union sentence as it stands
  (*named or anonymous*) and to R1's sentence as it stands, each failing the readers its own way
  (§ 1). **No veto.** **Provisional**: every real figure is a band.
- `section`: design.md §1.6 (line 255, 10240; the payment rule), §1.2, §4.17 (beside: no message
  names `partial`), §4.19; CLAUDE.md § 12 (spec beats compiler: the compiler zeroes the whole object,
  and the spec must say so); panel 009's allocation rule.
- `spec_token_delta`: **`m-g-at` +49 legacy / +48 cl100k** (7519 → 7568, 7655 → 7703), real ≥49,
  about 64, ≤78 (10097 to 10126). **`m-g` +58 / +58**, real ≥58 to 93 (10106 to 10141).
  `m-anon` +5 / +5. All measured offline; the real count is the coordinator's `--refresh`.
- `removal`: **§ 9's *copy in, copy out* (−9 / −10)**, a truth repair that 10 of 10 B readers'
  notes ask for. In reserve: *"only a group's record has it"* (−7 / −8). Neither covers +58, and the
  rest is paid by the prediction below.
- `needed_for_self_hosting`: **no** (`selfhost/` binds no union of C's).
- `argument`: Nine readers missed A2 on the specification's words, not on a message they never saw.
  *"Named or anonymous"* licensed the flattened `fd`, and R1 hid that `rest: zero` zeroes what
  `partial` leaves out, which the compiler does (`memset` of `sizeof`). One rule decides every union
  shape I built: a field is what C reads as `s.name`, and where C reads `s.u.n`, `u` is the field or
  `partial` keeps it. The example is neutral, so the re-run is not taught its answer. +49 after § 9's
  removal, within budget at the band's top. (90 words)
- `prediction`:
  1. **Tokens.** `m-g-at` landed on `7e5c02048` reads 7568 / 7703 offline, and `--refresh` reads real
     between **10097 and 10126**. `m-g` reads 7577 / 7713, real 10106 to 10141.
  2. **The re-run on `m-g-at`**, at least 10 fresh readers, every program built by `judge.sh`:
     - **A2 ≥ 8 of 10 correct first try** (194's threshold; it read 1 of 10);
     - **U ≥ 8 of 10** correct first try, and **`u8` ≤ 2 of 10** (194's prediction, now scored);
     - **C ≥ 8 of 10** print `6 4 5`, and **≥ 3 of 10 copy an initialised lock** (my condition's
       first half, scored for the first time on a task that can produce the copy).

     Scored at the re-run, inside M-buildable-structs.
- `condition`:
  - **A2 ≤ 7 of 10.** If the misses' notes cite the new words, I turn to object to `m-g`'s union
    clauses and fall back to `m-anon` (+5). If they cite R1's zero promise, panel 194's *"nothing
    becomes my verdict"* row fires, and R1 is re-argued under the removal branch.
  - **`u8` ≥ 3 of 10 on U**: object to R1 + K, as 194 registered.
  - **C ≤ 2 of 10 copying**: my Q2 stance stands (a refusal decided from the header, no word), and a
    word has no measured thesis effect. **≥ 3 of 10**, together with the ffi-pragmatist's showing
    that C cannot decide without a list the code cannot keep: I turn to B, as my first report
    registered.
  - **Veto** if a `--refresh` of the adopted text reads above **10180** with no named removal.

## 6. What I could not run

- **The real count.** Every real figure is a band; `--refresh` is the coordinator's.
- **The re-run itself.** It is a paid run, the coordinator's.
- **Any shape on Linux or the Windows box.** Every row of § 1 ran on this Mac at the default
  `-O0`. The padding bytes the nonzero counts include are zero here and promised by no sentence:
  that is Q1, and no draft above promises a byte.
- **B's and C's prototypes on Task C's programs.** The critic's `heroes-g` and `heroes-p` were not
  kept, so the false-refusal count is scored by reading.
- **`@` across two modules emitted as two units.** `--emit-c` gave one unit holding both modules.
- **Kept**: the probe sources, headers and build transcripts (`spec-warden-work/q3/a2`, `shapes`,
  `tasks` and `twomod`), the 38 measurements (`measure_q3.txt`), the five suite outputs, the drafts
  script and the kit. No binary is kept. The copy is removed at the end.
- **One slip of mine, undone.** To check that the key programs were canonical, I wrote
  `heroes fmt`'s output for them into the session scratchpad, outside the repository's root, which
  the shared brief forbids. The three files were removed at once, and nothing else of mine was
  written there. `key/c.hero` was not canonical (three blank lines after its loops). It was formatted
  in place with `heroes fmt --in-place` and judged again: `a2=ok form=partial u=ok u8=no c=ok
  ret=-;byval=-`.
