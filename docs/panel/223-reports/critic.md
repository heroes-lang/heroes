# Panel 223, completeness critic, second pass: what the two reports leave missing or contradicted

Read 2026-10-17 from 18:34, the two seats' reports as they stood then
(`docs/panel/223-reports/compiler-engineer.md`, 341 lines, and `ffi-pragmatist.md`, 273, by `wc -l`)
and the repaired briefs. No verdict is given here. My copy is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/223-critic/w/` (git-ignored: `git
check-ignore -v` names `.gitignore:35`), holding two C files, `combo.c` and `oracle.c`, and
`flags.txt`, the words of `flags()` in `selfhost/cli/flags.hero` at `3c478500b`. On the Windows box,
`C:/w/223-critic/` only. I read no file in a seat's directory. No compiler was built; nothing below is
a run of the emitter.

**The compile line used throughout**, called *the flags* below: `clang -target <t> $(cat flags.txt)
-Werror <level>`, so any clang warning is a refusal, as for the ffi-pragmatist.

## 1. The mask's source: the facts agree, the rule they are judged by does not

Each seat's measurement holds when I run it:

| claim | seat | my command | result |
|---|---|---|---|
| `m.b = -1` and `m.b = ~m.b` are refused | compiler-engineer (§ 4, row 3) | the flags, `-fsyntax-only`, one spelling per file, `struct bf` | both `[-Werror,-Wsign-conversion]` |
| `m.F--` passes | ffi-pragmatist (§ 4) | the same | `m.b--` and `m.b -= 1`: no diagnostic |
| `m.F--` passes over many kinds, every CI target | ffi-pragmatist (nine kinds, this Mac only) | `combo.c`: `m.F--` over `int s:3`, `_Bool f:1`, `unsigned char uc:2`, `unsigned long long w:33`, an unsigned and a signed `enum :2`, a bit-field of an anonymous struct, two in a union arm, a `volatile` one; the flags at `-O0` and `-O2` on `x86_64-unknown-linux-gnu`, `aarch64-unknown-linux-gnu`, `arm64-apple-macos14`, `x86_64-pc-windows-msvc` | **0 diagnostics** in all 8 cells |
| `m.F--` sets exactly the named bits | ffi-pragmatist (an oracle over 3 records) | `oracle.c`: each bit set alone in a zeroed object, every named bit-field read, against `m.F--`'s bytes, over the record above | **0 bytes differ**: this Mac (24 bytes), `heroes-linux-arm64:latest` and `heroes-linux:latest` (24), the box, clang 23.1.1, MSVC layout (48) |
| on `powerpc64` the dump's column is not the memory's bits | ffi-pragmatist (§ 4) | `clang -target powerpc64-unknown-linux-gnu`: the dump, and `-S -emit-llvm` of `const struct bf mb = {.b = 7}, mc = {.c = 15}` | the dump prints `b 1:0-2`, `c 1:5-8`; clang stores `mb`'s byte 1 as `-32` (`0xe0`) and `mc`'s bytes 1 and 2 as `7` and `-128` (`0x80`): holds |
| the compiler-engineer's assertion fails there and nowhere on the legs | compiler-engineer (§ 3) | `_Static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, ...)` through `-fsyntax-only` | `static assertion failed` on `powerpc64` (its `__BYTE_ORDER__` is `__ORDER_BIG_ENDIAN__` by `-dM -E`); no output on `x86_64-pc-windows-msvc` and `aarch64-unknown-linux-gnu` |

**So the conflict is not over a fact.** The dump is wrong on a big-endian target (the ffi-pragmatist),
and the compiler-engineer's route refuses to build there instead of masking wrongly. What divides them
is a rule, `.claude/rules/module-shape.md` § A narrowing asks the value: *where a premise is
unavoidable*, a falsifiable claim and a test that fires. The assertion is that test. Whether the
premise is unavoidable is the checkable side, and it is avoidable. Two things are now settled by
measurement.

- **The compiler-engineer's stated price for the clang-bits source is falsified.** Their row 3 objects
  that *each field type needs its own spelling of all-ones*. `m.F--` is one spelling, with 0
  diagnostics over nine kinds and four targets, and equal to the oracle on four legs (above).
- **The remaining price is names in emitted C, and the landed code already pays it for whole
  members.** The landed code spells each member a dump names under `padding.guarded`, a
  `push_macro`/`#undef`/`pop_macro` around the one line that spells it (`selfhost/emit/padding.hero:248-261`).
  It writes a constructor before `main` (`selfhost/emit/padding_tables.hero:84-96`). A `m.F--` line
  through the same guard inside that constructor is the same mechanism. **Neither seat built it**, so
  its line count is unrun.

**Missing from both on this point**:

- **A `const` named bit-field.** The dump prints the qualifier (`1:0-2 | const unsigned int x`,
  measured). Under `m.F--`, clang refuses the line (the ffi-pragmatist's condition 2): a correct
  binding's build would exit 2. Under the compiler-engineer's dump route, the atomic `and` writes the
  byte that holds it. That is the ffi-pragmatist's 6.7.3p6 concern, and the compiler-engineer's
  conditions do not name it. The census reads 0 such bit-fields in two C libraries' headers (a floor,
  the ffi-pragmatist's). Each route owes the same guard: leave such a byte unwritten.
- **What a big-endian refusal means.** Under the dump route, a correct program with such a record
  fails to compile on a big-endian target. `.claude/rules/generated-c.md` § Flags makes a clang
  failure exit 2, *the compiler is wrong*. No CI leg is big-endian; the synthesis should say whether
  that is a platform the language promises.

## 2. Atomic or volatile: they are not alternatives, and the plain form's support rests on a premise the tree contradicts

- **The two are compatible, measured.** `combo.c`'s `clear` is `__atomic_fetch_and(b + at[i], keep[at[i]],
  __ATOMIC_RELAXED)` through `volatile unsigned char *b`. It meets the ffi-pragmatist's condition 3
  and the compiler-engineer's atomic. It compiles with **0 diagnostics** under the flags at `-O0` and
  `-O2` on the four targets. It keeps `b = 5`, `c = 10` and leaves bytes 1 and 2 `45 01` on this Mac,
  the bytes the compiler-engineer's route printed (their § 3c). At `-O0`, the level `heroes build`
  uses, the instruction is a `lock cmpxchgb` loop on both x86-64 targets, `ldclrb` on Darwin and an
  `ldaxrb` exclusive loop on `aarch64-unknown-linux-gnu` (`clang -S` of `combo.c`, Apple clang). The
  compiler-engineer's `lock andb` is the `-O2` form. All of these are atomic.
- **The ffi-pragmatist's *Heroes has no thread of its own* rests on a spec grep alone** (their § 4).
  The tree contradicts it. A Heroes program binds `hero_thread_spawn` through `extern "hero_os.h"`
  (`runtime/hero_os.h:414`; `selfhost/cli/link.hero:289-291` does it), `examples/threads/main.hero`
  is cited there, and `tests/golden/run/` holds `fixedbugs-396-two-threads-lend-at-once` and three more
  thread cases (`ls tests/golden/run/ | grep -i thread`). The writer of a bit-field is C in any case,
  since a Heroes program cannot name one (spec line 383). What the race needs is a C function that
  writes a bit-field through an address it kept, called from a thread while the record is lent again.
- **Which side is checkable.** The compiler-engineer *ran* a program the landed compiler accepts
  (`race.hero` over `race.h`, their § 2c) and counted lost bit-field values under the plain form on
  two machines. I did not re-run it. The ffi-pragmatist's single-thread test of 200,000 patterns
  cannot see a race by its construction. Their *no such library among the 32 records* is a reading of
  headers, which cannot show what a library does at run time; they say so (*a question, not a
  premise*). Their own turning condition, *a C library that keeps a Heroes record's address and
  writes a bit-field ... from its own thread*, is what `race.h` is. **The synthesis must say whether a
  library written for the probe counts.** Panel 215 judged by probe headers written for it (`pn.h`).
  Where that condition turns them, they go to B-run at the stores, and B-run is a plain write-back,
  the mechanism the compiler-engineer vetoed on that same measurement.
- **Unrun by either**: the race program under the combined clear (atomic through `volatile`), and
  under masks from `m.F--`. The masks are byte-equal to the dump's on the four legs (the
  compiler-engineer's § 2b, the ffi-pragmatist's § 3, mine above), so the second changes nothing a
  run could see there.
- **What judges the masks.** MemorySanitizer cannot judge an atomic route's masks. The
  compiler-engineer measured that it marks every byte an atomic read-modify-write touches as written
  (their § 3b). The ffi-pragmatist's MemorySanitizer table ran the plain `&=`. For the combined route,
  the mask's judges are the value probe (their `vp.hero`) and an oracle like `oracle.c`, not MSan.
- **Found by one seat, absent from the other.** The ffi-pragmatist's `volatile` bit-field on Linux
  arm64 is stored 32 bits wide (their § 1 table), so R1 writes inside C's access unit there too. And
  on MSVC `curl_hstsentry`'s gaps `(17,3)` lie inside the 4-byte unit of `includeSubDomains`. The
  compiler-engineer's § 2a lists `struct bf` and `struct gap` only. Both seats agree R1 can lose its
  own zero there and never a member's value.

## 3. Can atomic `and` with masks from `m.F--` be built by about 20:30?

**The C half is measured to build. The compiler half is unbuilt by anyone, so this is an estimate.**

Measured: the emitted C the combination needs compiles warning-free under the project's flags on all
four CI targets at both levels. Its masks equal a no-write oracle on all four legs (§ 1). Its clear
keeps the named bits (§ 2).

Read, not run: the change from the compiler-engineer's built route is narrow:

- `membership.encoded` carries each named bit-field's path in its `n` entry (the dump already gives
  `m.path`), and `Held` keeps those paths and adds them to `bare` so the guard covers them;
- `padding_tables.tables` writes, inside `hero_pad_init`, one block per record with bits,
  `{ T m; __builtin_memset(&m, 0, sizeof m); m.<path>--; ...; <masks> = hero_pad_masks(...(unsigned char *)&m...); }`,
  each line through `padding.guarded`, in place of the bit table, its constants and the
  `_Static_assert`;
- the runtime's `hero_pad_masks` reads a byte image of `m` instead of `lo`/`hi` pairs, and
  `hero_pad_clear` takes `volatile unsigned char *`;
- a bit-field whose dump type text begins `const` gets no line and its byte no mask.

**What it costs in time, from the seats' own numbers.** Stage 2 is 74.37 s (shared brief). The
compiler-engineer's verification of their route was the compiler's tests (1,720), `emission` with the
610 re-emitted (36 s), `determinism`, `emit`, the value probe here and on the box, and MSan in the
container with its two-stage build. Whether that fits before 20:30 depends on the seat that already
holds the built route making the swap. Their prototype has the tables, the constructor and the clear
in place; anyone else starts from the lane. **An inference, and it is unrun.**

**Owed by any landing, whichever mask source** (the compiler-engineer's § 6, unrun there): `wholes`,
`descriptors`, `lldb`, `runtime` (the runtime moves, so every emission's key line moves: 609 of 610
identical but that line, their § 3e), `layout` itself (they used the hook's mirror), the seed and its
fixpoint, the full net. `padding.hero` stands at its 300-line ceiling as built and needs a split.

## 4. Missing, and the side that can check it

1. **The compiler-engineer's prediction is tied to its mask source.** It reads *0 lines outside
   condition 3's classes*, and the classes name *bit tables* and *one `_Static_assert` per unit*. A
   landing with `m.F--` moves the same one emission
   (`tests/emission/run-fixedbugs-156-records-beside-bit-fields-build.c`) with other lines (guarded
   `m.F--` lines in the constructor) and falsifies the prediction as written. The synthesis restates
   the classes to match the adopted source. Checkable at the landing by their own instrument.
2. **The two drafts of `.claude/rules/generated-c.md`'s sentence each lack what the other found.**
   The compiler-engineer's (their § 4, condition 4) has the atomic, the 2112 store and the
   little-endian assertion. The ffi-pragmatist's condition 4 has the newer-library residue (a
   library's new bit in an older header's unheld bits zeroed before each lend; `tcp_info`, their § 4),
   the `volatile`-on-arm64 and MSVC writes inside a unit by R1, and the race residue. Neither has the
   `const` exception.
3. **Both seats say no route that writes inside a bit-field's memory location rests on the C
   standard.** The compiler-engineer's strongest objection (§ 4) and the ffi-pragmatist's 5.1.2.4p25
   (§ 4) agree: an atomic beside C's plain write is still a data race by C11's definition, so the
   route rests on each ISA's byte-level atomicity, measured on three. **The question the sitting did
   not ask**: which sources can put a dirty bit in a byte a named bit-field holds, counted. That is
   the compiler-engineer's own turning condition for *at the source* (*a census showing those two
   sources cannot carry a bit-field record*), the one listed route that needs no atomic. Neither seat
   ran it.
4. **Linux x86-64 is thin.** The race and the value probe ran on the box (MSVC x86-64), not on Linux
   x86-64 (compiler-engineer § 6). The ffi-pragmatist's single-thread table ran there, emulated.
   MemorySanitizer on Linux x86-64 for the route: unrun by both.
5. **Per-lend cost of the combined form at `-O0` on x86-64**: the `lock cmpxchgb` loop above, not
   `lock andb`. The compiler-engineer's box timing (about +8 ns, by wall clock, a weaker reading they
   name) was of a build at `heroes build`'s level, so it may already include it. Whether it did is not
   stated.
6. **Agreed and checkable, recorded so the synthesis need not reopen them**: D is outside this lane
   (both); B over every byte draws one veto and one objection, both resting on writing members' bytes;
   one mechanism at the lend and at 2112's store (both approve); C left open stops nothing counting
   (both, and the shared brief).
