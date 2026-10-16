# Panel 217, the completeness critic, pass 2: the seats' reports checked

Written 2026-10-16 from about 23:28, after every seat had reported. Read: the five seats' reports,
the arms' outputs under `llm-ergonomist-work/`, and the briefs as repaired at 22:05 and 22:20.
`<sp>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad`.

**My tree and compilers.** I re-created my copy `<sp>/217-critic/` from `f57af53da` (`git worktree
add --detach`, `rm -rf build`) and built three compilers there:

- `heroes` from the seed (`real 10.58 user 10.03 sys 0.18`);
- `heroes-r2` from it;
- `heroes-f1f-mine`: my own build of the compiler-engineer's last prototype, from a copy of
  `<sp>/217-ce-probes/f1f-prototype.patch` (sha256 `835578efa8ce…`, the one its report names)
  applied by `git apply`, then `./heroes-r2 build selfhost/main.hero -o heroes-f1f-mine` (`real
  36.52 user 34.56 sys 0.99`; sha256 `4e20d0f7f434…`, not the seat's `fe0128507a63…`, a different
  build of the same patch).

Every probe below is in `<sp>/217-critic-work/pass2/`. The seats' files were copied there before
anything ran. No paid run, no API call, no credential printed.

**In short.** The F1 that every approving seat stood behind, the compiler-engineer's final
prototype, **fails the ffi-pragmatist's veto condition (c)**, and nobody ran it there. A record lent
read-only at a `...` position of a header that writes through it builds, and an immutable `=`
binding prints `103` at exit 0 (§ 2). A `strchr`-shaped C function that strips the `const` lets the
same binding be written, printing `42`. That door is already open today for `cstr lent` (`Xello
Xello`, § 2). On the coordinator's question: condition (c) and a private copy at `...` both leave an
overflowing C writer able to corrupt the heap, measured under ASan. They differ in whether a wrongly
declared writer is told or silenced. A third route, clang's format attribute, is unpriced (§ 1).
**Arm F1 measured one effect nobody reported**: 3 of 10 readers moved `lent`'s first meaning off
the C functions that write (§ 3).

## 1. Condition (c) against a private copy at a `...` position: what each measures

The ffi-pragmatist's condition (c) refuses a read-only lend at a position past the header's
prototype, *"`@` (C may write) and a lease (a private copy) stay open there"*. The alternative it
did not price hands C a private copy of the bytes at that position. **Today's lease is exactly
such a copy**, so its behaviour can be measured now with `heroes-r2`. The reproducer is the
ffi-pragmatist's `va.h` (`va_scribble(int32_t k, ...)` writes `'X'` through the `char *` it reads),
plus three headers of mine (`<sp>/217-critic-work/pass2/alt/`):

- `vr.h`, `va_len`: reads the string as `%s` does;
- `vs.h`, `va_scan`: copies five bytes into it, as `sscanf` would;
- `stdio.h`'s own `printf`.

| program (`alt/<name>/p.hero`) | what is lent at `...` | build | prints |
|---|---|---|---|
| the ffi seat's `va2` (reproduced) | `a.cstr()`, `a` and `b` sharing one string | 0 | `1 Xello Xello`: both names changed |
| the ffi seat's `va` (reproduced) | `"hello".cstr()`, a literal | 0 | **exit 138, SIGBUS** |
| `lease_shared` | a lease of `a`, to `s: cstr` | 0 | `1 hello hello`: C wrote into the copy |
| `lease_literal` | a lease of `"hello"` | 0 | `1 hello`: no fault |
| `lease_to_lent` | a lease, to `s: cstr lent` | 0 | `1 hello` |
| `reader_cstr` / `reader_lease` | a reader, lent / leased | 0 / 0 | `6 6` / `6` |
| `printf_cstr` / `printf_lease` | `printf(fmt:, s:)` from `stdio.h` | 0 / 0 | `[hello]` and `8`, both |
| `writer_cstr` | `va_scan`, `a.cstr()`, 5 bytes into `"hello"` | 0 | `1 found found`: both names changed |
| `writer_lease` | the same through a lease | 0 | `1 hello hello`: the write is lost, silently |
| `writer_short_lease` | `"hi"` leased, 5 bytes written into 3 | 0 | `1 hi`, exit 0; **under `--sanitize` exit 134, `AddressSanitizer: heap-buffer-overflow … WRITE of size 5 … in va_scan`** |

What this measures, each a command's:

- **(c) keeps a printf-style binding of a string buildable.** It refuses `a.cstr()` at `...`, and
  the lease it leaves open builds and prints (`printf_lease`, `reader_lease`). What (c) costs a
  program is three lines per call (lease, call, `end_lease`) and the copy itself. In the corpus it
  costs nothing: the ffi seat's `all2.tsv` holds 3 parameters past a prototype (`awk -F'\t'
  '$9==""'`), curl's `value: cstr` passed a lease and two `i64`s, and none is `cstr lent`. That is a
  reading of their table; I did not re-run their census.
- **The copy's run-time price** (`alt/bench/`, `-O2`, 2,000,000 calls of `va_len`, the warm round,
  `real` within 0.01 s of `user + sys`). The first round is discarded by the ratio rule: `real
  0.20` against `user 0.01`.
  - 16 bytes: `.cstr()` 0.01 s user, the lease 0.05 s, about 20 ns per call;
  - 4 KiB: 0.18 against 0.31 s, about 65 ns per call.

  A copy the compiler emits could cost less than a lease, which also keeps the books `main`'s end
  checks. *Unbuilt.*
- **Neither closes the class.** A C writer that writes past the bytes it was handed overflows the
  private copy (`writer_short_lease`, ASan above). The lease that (c) keeps open is that same copy.
  So **(c) as written repairs the in-place write and the literal's fault, not the overflow**, and the
  ffi seat's *"the repair is one rule for both"* holds for in-place writes only. The private copy
  repairs the same two things, and no more.
- **They differ on a writer declared `lent` by mistake** (`sscanf`'s shape). (c) refuses it at
  exit 1, so the mistake is told. The private copy builds it, and the write is lost with no word
  (`writer_lease`: the program's string unchanged, the scan's result gone). By CLAUDE.md's opening
  line, *every plausible mistake is a compile error*, (c) is the one that tells.
- **For F1's records the copy is not available.** A private copy of a `Counter` at `...` is R2's
  refused copy of a lock. For a lock-holding record, (c)'s refusal is the only one of the two that
  applies. For a copyable record a copy would work. *Unbuilt either way.*
- **A third route, unpriced: ask the header what `...` is for.** clang's AST, which the pointee
  check already reads as `-ast-dump=json`, carries a format attribute:
  - on this Mac: `printf` `FormatAttr printf 1 2` (implicit and from `_printf.h`), `sscanf`
    `FormatAttr scanf 2 3`;
  - in `heroes-linux-arm64:latest` (glibc's `stdio.h` writes no attribute): `Implicit printf 1 2`
    and `Inherited printf 1 2`, `Implicit scanf 2 3` and `Inherited scanf 2 3`;
  - command: `clang -Xclang -ast-dump -fsyntax-only fmtattr.c`, on both.

  So a `cstr lent` at a printf-family `...` could stand, and every other `...` be refused as (c)
  says. Caveats, unmeasured: `%n` writes an `int *`, and a format that is not a literal says
  nothing of which argument is `%s`. *Unbuilt.*

## 2. Contradictions between seats, and which side I could check

- **The compiler-engineer approves F1 at `heroes-f1f`; the ffi-pragmatist vetoes F1 without (c).
  Checked: the prototype does not have (c).** Probe `f1probes/va_record_lent`, a header whose
  `va_bump(int32_t k, ...)` adds 100 through the `struct counter *` it reads, bound `va_bump(k: i32,
  c: Counter lent)`, built with `heroes-f1f-mine`:
  - `peek(c: Counter lent)` reads 3, calls `va_bump(1, c)`, reads again: **`3103`**, exit 0;
  - `va_record_lent_direct`, an immutable `s = Counter(n: 3, rest: zero)` lent to it, then
    `print(s.n)`: **`103`**, exit 0.

  This is the ffi seat's own prediction (*"accepts `va_bump(k: i32, c: Counter lent)` … and the
  program prints `103` at exit 0"*). Nobody ran it: the ffi seat wrote before the prototype existed
  (its § 9), and the compiler-engineer's report never names a variadic header. The same prototype
  refuses a writable header at a fixed position (`writable_header`, `ffi_writable_parameter`), so
  the `const` question is asked everywhere but past the prototype.
- **Condition (a), *a probe whose parameter carries the `const`*, against what the prototype
  emits. Checked: it does not.** `heroes-f1f-mine build key-f1.hero --emit-c` gives:
  - line 153: `static void hero_ffi_probe_h_keyf1_counter_value(struct counter * a0)`;
  - line 214: `int64_t h_keyf1_doubled(struct counter *ph0_c)`.

  The prototype crosses typed (never `void *`), with no `const`, and puts the `const` question in a
  separate assertion (the compiler-engineer's `heroes-f1d`, `const_assertion`). That assertion
  refuses `w11_writable` and my `writable_header`. Whether that meets (a), which the ffi seat made a
  veto line, is the ffi seat's to say; it has not seen the prototype.
- **"With the three refusals a lent parameter is observationally a copy taken at the call"**
  (compiler-engineer, its §4.10 claim) and **"with aliasing refused, a read-only lend is observably
  a copy, so the unmarked call keeps locality"** (spec-warden's argument). **Checked: false**, on
  three measured programs:
  - the compiler-engineer's own `keptpt_lent` prints `11` where `keptpt_value` prints `10`
    (reproduced with `heroes-f1f-mine`): C writing through an address it was allowed to keep;
  - my `va_record_lent`: `3103`;
  - my `c_launder` below: `42`.

  The compiler-engineer files the first under *"a sentence for the spec-warden"*. The arm's draft
  carries no such sentence. `grep -n -i -E 'lock|mutex|kept|keep'` over it hits the word *block*,
  two unrelated *keeps* (lines 11 and 317), and § 13's existing sentences on what C keeps (lines
  361, 416, 438 and 440). None says a read-only lend can see a write C makes through a kept address.
- **C handing back a pointer into a read-only lend.** The compiler-engineer answered my pass-1
  question 5 by reading: *"`lent` on a C parameter already means C keeps nothing … F1 changes
  nothing there"*. **Measured** (`f1probes/c_launder`): `pt_field(const struct pt *p)` returns
  `(int64_t *)&p->x`, bound `pt_field(p: Pt lent) -> ptr`, and `poke(int64_t *q)` writes 42, bound
  `poke(q: ptr)`. Then `s = Pt(x: 3)`, `poke(pt_field(s))`, `print(s.x)` prints **`42`** at exit 0
  under `heroes-f1f-mine`: an immutable binding changed with no `@` anywhere. `heroes-r2` refuses
  the same file (`lent_shape`), so F1 opens this for records. **For strings it is open today**:
  `cstr_launder`, `my_chr(const char *s)` returning `(char *)s`, then `pokec(char *q)` writing
  `'X'`, prints **`Xello Xello`** with the trunk's seed compiler and with `heroes-r2`, at exit 0. I
  searched the issues (`gh issue list --search`) for *strchr*, *launder*, *returns a pointer into*,
  *ptr result lent*, *const away* and *pointer into what it was lent*. Issue 1292 (panel 171) rules
  that the specification *"promises nothing about a pointer C hands back"*, which may or may not
  cover a write through it. So whether this is a defect or a ruled cost is **a question for the
  coordinator, not a premise**.
- **U1, a new word.** The spec-warden objects (*"no binding in the tree is shown to need that
  separation"*); the historian approves it as F1's equal; the ffi-pragmatist calls it *"better at
  the boundary"* (`sqlite3_create_module`, a reading, unrun). **New evidence from arm F1** (§ 3):
  F1's second meaning displaced the first in 3 of 10 readers. It is checkable, and nobody read it.
- **U3, by type**: the historian approves it as a complement (D, Ada); the ffi-pragmatist and the
  spec-warden object. Nobody built it, so it cannot be checked tonight.
- **The draft's aliasing clause is narrower than the compiler.** The draft says *"which no other `@`
  argument of the call may reach"* (`heroes-spec-f1.md` line 275). The prototype also refuses an
  `@` nested inside another argument (`nested_lent`, `aliased_mutable_arguments`, reproduced with
  `heroes-f1f-mine`), which is not an argument *of the call*. Text and compiler disagree on a
  refusal. A landing owes the words.
- **p1, `@p: Pt lent` on a Heroes function**: the spec-warden found `heroes-f1` built it. **Checked:
  `heroes-f1f` refuses it** (`lent_shape`: *"`@` hands the caller's place to change it and `lent`
  hands it to read, so write one"*), so p1 is reconciled at `f1f`. Its `never_rebound` still says
  *"its copy-out hands every caller back the value it passed"* beside a note naming F1.
- **Arm R2's judge.** The brief says *the compiler at `f57af53da`'s source*. The arm's report says
  *"built from `selfhost/` at `f57af53da` plus R1's uncommitted step"*. **Checked: no difference.**
  My `heroes-r2` at `f57af53da` alone builds all ten arm R2 programs at exit 0, each printing `6 4
  5`, and refuses arm F1's r03 alike.

## 3. What arm F1 measured that nobody reported: `lent`'s first meaning moved

**First meaning, *C does not keep it*, on the three C functions that write**: counted by `grep -cE
'function counter_(init|lock|unlock)\(@c: Counter lent\)'` on each program.

- Panel 215's re-run: **30 of 30** declarations carry `lent`.
- Arm R2: **30 of 30**.
- Arm F1: **21 of 30**. r02, r07 and r10 write `@c: Counter` on all three.

r02's note says why in its own words: *"`struct counter *` is an `@` parameter, and `const struct
counter *` is `lent` without `@` (a pointer to const)"*. So for that reader `lent` now means
*read-only*, and a function that writes lost it. The arm's report lists the three as *"three `@c:
Counter`"* under the spec-warden's column and says nothing more. Nothing breaks: `@c: Counter`
without `lent` says *C may keep it*, which the compiler accepts. What it measures is that the word's
second meaning displaced its first in 3 of 10 readers who read the form once. That bears on U1
directly, and on the historian's condition 4.

**The arm's other counts, checked**:

- signatures: `grep -E '^function doubled'` gives `doubled(@c: Counter)` 10 of 10 in arm R2 and
  `doubled(c: Counter lent)` 10 of 10 in arm F1, every call unmarked;
- `counter_value(c: Counter lent)` 10 of 10 in arm F1;
- builds with my own compilers: arm R2 10 of 10 printing `6 4 5`; arm F1 9 of 10, r03 refused
  `ffi_lock_copied` at `c @ Counter(n: 0, rest: zero)`. That store is refused by R2's rule, and the
  specification the reader read says nothing of locks (`grep -n -i -E 'lock|mutex'` over the draft
  hits only the word *block*);
- the nine arm F1 programs built with `--sanitize` run at exit 0 with no sanitizer report;
- costs: the sum of `total_cost_usd` is 1.4139 (R2) and 1.7700 (F1), every session's `modelUsage`
  naming `claude-opus-5-5`;
- `context: yes`: 7 and 3 in arm R2, 10 in arm F1 (`grep -l`).

## 4. Claims asserted and not measured

- **"Observationally a copy"** (compiler-engineer) and **"observably a copy"** (spec-warden):
  measured false, § 2.
- **"F1 changes nothing there"** for a pointer C hands back (compiler-engineer): measured, § 2
  (`42`).
- **"(c) … repairs the `cstr` door that is open today"** (ffi-pragmatist): true for the in-place
  write and the literal's fault; the lease it leaves open overflows (§ 1).
- **"It pays where at least 6% to 24% of sessions need it"** (spec-warden): its own words say *a
  share nobody has measured*.
- **The cold `--emit-c` +1.8%** (compiler-engineer): its own words say *unsettled tonight*.
- **"The compiler's own tests, 1680 passed"**: run with `heroes-f1b` and `heroes-f1d`, not the
  `heroes-f1f` it approves; `run` 525 and 0 with `heroes-f1c`. The report argues that `run` with
  `f1c` speaks for `f1f` (*"differ only on a parameter written `lent` without `@` on a type other
  than `cstr` or `ptr`"*). For the compiler's own tests that is an inference.
- **The arm F1 scores as reach** (historian's prediction, the spec-warden's (b)): scored on task
  words that say *"without changing the counter"*, the control task my pass 1 asked for (A2) was not
  run, and the historian's own report says a 7 or more *"still owes the critic's control task"*.

## 5. Forms nobody built

- **(c) itself**, in any compiler: the ffi seat's rule, and the F1 resolution's veto line (§ 2).
- **The private copy at `...`**: simulated by a lease above, not built.
- **The format-attribute route** (§ 1): unpriced.
- **U1** (a new word), **U2** (a mark at the call), **U3** (by type), **U4** (a lend method to
  C), **U5** (an `@` only read admitted), **F2**: none built, each argued from code or precedent.
- **The IR slot carrying the mode**: the compiler-engineer recommends it for a landing (*"both holes
  I found in my own prototype were a consumer that could not see the mode"*). Unbuilt. The two holes
  above (`va_record_lent`, `c_launder`) are two more of that kind.
- **F0's repair**: R2's note and `never_rebound` made to agree without a form. The compiler-engineer
  names it as F0's condition; unbuilt.
- **A second try under F1**: no reader refused under F1 saw F1's message (my pass-1 question 6). r03
  is the one arm F1 program that would have needed it.

## 6. What the resolution would rest on that nobody ran

- **(c) on the prototype**: now run, and it fails (§ 2). As it stands, F1 meets the ffi seat's veto,
  and every approving verdict (compiler-engineer, spec-warden, historian) was written about a
  compiler that does not refuse it.
- **The control arm**: the task without *"without changing the counter"*. By arm F1's cost it is
  about 1.77 USD, and the arms have spent 3.1839 of the evening's 10 (the arm report's figure,
  checked against the `run.json` files).
- **A specification sentence** for C writing through a kept address (`keptpt_lent`), for the
  variadic position, and for a pointer C hands back. None is drafted or priced, and the landing's
  10,246 real holds none of them.
- **The ffi seat's check of the prototype's crossing against its own condition (a)** (§ 2); the
  Windows box (unrun twice by the ffi seat); a still machine for the compiler's cold `--emit-c`; the
  compiler's own tests and the full `check` and `emission` forms with `heroes-f1f`.
- **The `cstr` laundering class** (`Xello Xello` today): whether issue 1292 (panel 171)'s *"promises
  nothing about a pointer C hands back"* covers it is a question for the coordinator to settle
  before F1 extends it to records.
