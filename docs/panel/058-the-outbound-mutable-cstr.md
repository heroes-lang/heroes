# 058 — The outbound mutable `cstr`

**Status**: `RATIFIED 2026-08-15` (was `provisional — author ratification pending`).
**Convened** 2026-08-15 by author instruction, from `docs/debrief/DECIDE.md`'s
`.cstr()` item — which panel 057 found while pricing `dirname(3)`, not by looking
for it.
**Lane**: full, five judges, all reporting, four of five building in copies.

**Two options were vetoed twice each, the sitting's own framing was wrong, and the
spec clause it was convened to price turned out to cost six times what it should.**

## The defect

`.cstr()` is zero-copy — `hero_str_cstr` returns `s.ptr`, the interior of a
**refcounted, copy-on-write** buffer (§4.20). So a C function that writes through it
writes into values the program never handed to C:

```
extern "string.h"
    function strtok(s: cstr, sep: cstr) -> cstr

a = "x," + "y"
b = a                      # shares the buffer
t = strtok(s: a.cstr(), sep: ",".cstr())
```
```
before: a=[x,y] b=[x,y]
after:  a=[x y] b=[x y]      # b was never passed to C
```

design.md §4.10's value semantics, broken through the FFI, exit 0, no diagnostic.
And a **literal**'s `.cstr()` points into `__TEXT,__const`: `exit 138, SIGBUS, no
message`.

## The proposal, verbatim

> Four options, to be judged separately:
> **(i)** `-Werror=incompatible-pointer-types-discards-qualifiers` plus a gate that
> makes it exit 1 on the author's line; **(ii)** unshare-or-copy at `.cstr()`;
> **(iii)** a new spelling for a mutable buffer lent to C; **(iv)** document and do
> nothing.
>
> Spec candidates measured: short clause **+32** (3208→3240), full clause **+55**
> (3208→3263). Headroom 888.

## The verdict table

| judge | (i) flag + gate | (ii) unshare/copy | (iii) new spelling | (iv) nothing | spec |
|---|---|---|---|---|---|
| compiler-engineer | **adopt-with-condition** ×3 | **veto** | **veto** | object | — |
| ffi-pragmatist | **adopt-with-condition** ×3, else veto | **veto** | refuse *for now* | **veto** | — |
| spec-warden | (measured it, and set the gating condition) | — | **VETO** | refuse | **adopt a rewrite at +2**; object +32, refuse +55 |
| llm-ergonomist | adopt-with-condition (the readable predicate) | — | — | refuse | variant 3 conditional, variant 2 object |
| historian *(advisory)* | refuse **as written**, adopt if narrowed to the **call site** | adopt-with-condition — *necessary, not sufficient* | adopt-with-condition | **refuse** | — |

**(ii) and (iii) carry two vetoes each. (iv) is refused by every seat. (i) survives,
conditioned by three seats independently.**

## The clause was six times its price, and the expensive version is unsafe

The spec-warden re-ran both candidates and then wrote the sentence they were both
trying to write. Turning *"`s.cstr()` passes a `str` to C"* into

> **"`s.cstr()` lends a `str` to C to read."**

costs **+2 measured** (3208→3210) against +32 and +55. A plainer *"passes a `str` to
C for reading only"* is +3.

**And the full clause is not merely dear, it is wrong**: *"may read … and never
write"* **licenses** a C function that *keeps* the pointer — the use-after-free
design.md:2064 defers to Part 7 item 10. The llm-ergonomist reached the same hole
from the other end, blind, as its hesitation 7: *"a `const char *` that C stores
(strtok, some `setenv`) passes every variant's rule and dangles later."* **Two seats,
no contact, same hole in the most explicit wording.**

Silence is refused too, and on the project's own rule: under CLAUDE.md §12 a
compiler that refuses what the spec permits has the bug (ledger row `3141`).

## (ii) is vetoed twice, and the ownership pass is why

The compiler-engineer's measurement is the one that ends it. `--dump-ir` shows
`slots a: str · $own1: str` — the ownership pass's synthetic slot means **every heap
`str` already has refcount 2** at a call. So *unshare-if-shared* copies at **100% of
call sites**, which is the identical refutation §4.10 already carries for in-place
`push`, and it deletes §4.20's *"single highest-return decision"*.

It does not even fix the unique case: measured, `a` alone still goes `[x,y]` →
`[x y]` with `len` still 3.

The ffi-pragmatist priced it independently and got worse:

```
a.len()=3  strlen=1                    ← the length now lies, silently
AddressSanitizer: heap-buffer-overflow, WRITE of size 41
  allocated by hero_alloc … (20-byte region)
```

**And the historian supplied the reason both measurements were bound to come out
that way**, from two languages with Heroes' exact shape. CPython licenses a foreign
write on *provenance* — *"unless the object was just created using
`PyBytes_FromStringAndSize(NULL, size)`"* — and Erlang on the allocator — *"only
mutable after a call to `enif_alloc_binary`"*. Neither licenses it on **uniqueness**.
Ruby licenses on uniqueness, and Ruby's strings are mutable. Heroes is the Erlang
case, so unsharing converts aliasing into *silent single-value mutation of an
immutable* — which is what both seats measured.

## (iii) is vetoed twice, and it is a Part 7 row read out loud

The spec-warden: a mutable-buffer spelling **is design.md Part 7 item 10 verbatim** —
*"a C-width vocabulary (`c_int`, `const`)"* — which CLAUDE.md §13 bars before the
fixpoint. The compiler-engineer priced it anyway: `Ty::Cstr`/`Prim::Cstr` occur in
**13 non-test modules** across `resolve/`, `types/` and `emit/`, and a refcounted
buffer additionally needs `descriptors.rs`, `own.rs`, a producer, a consumer and a
`heroes mutate` operator — **400–600 lines**, checker + lowering + backend +
ownership. design.md **Part 5**'s test (does it need the type checker *and* the
lowering *and* the backend?) is failed by (i) and passed by (iii), which is exactly
backwards from what should enter.

**The historian dissents and it is recorded rather than smoothed.** Four ecosystems
converged on a separate type independently — JNI's non-const array API, .NET's
`byte[]`/`StringBuilder` over `string`, OCaml's `bytes`, Rust's `CString` with no
`as_mut_ptr` — and OCaml is the receipt for *when*: 4.02 (2014) opt-in to 4.06 (2017)
default, a three-year staged migration with a per-file compatibility flag. Heroes has
no deployed corpus and would pay none of that. **The disagreement is not about the
destination but about the clock**: the historian prices doing it later, §13 forbids
doing it now, and both are right about their own question.

## (iv) is refused by every seat

The ffi-pragmatist measured the shim route the option implies:
`const char *hero_strtok(const char *s, …) { return strtok((char *)s, …); }` compiles
with **0 lines of diagnostic** under this project's `FLAGS` (`-Wcast-qual` is not in
the set), defect intact. And `heroes` has **no shim-compilation surface** (§4.19
records it undecided, panel 036), so "write a shim" means a Makefile — CLAUDE.md §10
and §1.12's stated failure mode in one act.

The historian: the industry ran this experiment and withdrew it.
`-fwritable-strings` was deprecated in GCC 3.4 and **removed in GCC 4.0** — *"Use
named character arrays when you need a writable string."*

## Whether refusing `char *` closes real doors — the sitting's sharpest disagreement, resolved by measurement

The historian refused (i) **as written** on documented precedent: GCC built exactly
this signal and left it out of `-Wall` — *"only if you have been very careful about
using `const` … Otherwise, it will just be a nuisance; this is why we did not make
`-Wall` request these warnings"* — with two named false positives, POSIX `execv`
(whose RATIONALE says the strings are not modified *and* that ISO C cannot express
it) and glibc `iconv`.

**Both named counterexamples are out of the flag's reach, and the ffi-pragmatist
compiled the proof.** `execv` takes `char *const argv[]` and `iconv` takes `char
**inbuf` — both are `char **`, which Heroes already reaches as `ptr`, and the flag
cannot see them. `e7_execv.c` links and execs clean **with the flag live**.

Then the population, enumerated by `clang -Xclang -ast-dump` over **17 headers**
rather than by eye:

| | plain `char *` params | callee **writes** | reads only | takes ownership |
|---|---|---|---|---|
| libc (string/stdlib/unistd/stdio/time/pwd/libgen) | 88 | 73 | 4 | 1 |
| sqlite3 · curl · raylib · zlib · png | 12 | 10 | 0 | 2 |
| SDL2 · ncurses · openssl · readline · libxml2 | 86 | 76 | 14 | 6 |
| **total** | **186** | **159 (85%)** | **18 (10%)** | **9 (5%)** |

**85% genuinely write.** The legacy const-incorrect class is 10% and is nameable:
ncurses' five terminfo entry points, seven in OpenSSL, `cget*`, `rl_expand_prompt`.

**And the sentence that reframes the whole option: the flag does not delete a
function, it deletes one *spelling*.** The correct spelling — `ptr` with a buffer C
owns — compiles today. Measured under the flag, all at exit 0: `getcwd` via
`malloc`+`ptr`; `strtok` with `s: ptr` and `a` intact; `putenv` (where the copy is
mandatory anyway, since it retains); and **ncurses `tigetnum(capname: ptr)` → `80`,
linked against the real libncurses, zero shim files**.

Corroborated from the other seat: the spec-warden built **13 of 13 examples at exit
0** under the flag — curl, raylib, sdl, sqlite included, so no rung was skipped for a
missing library — with **zero diagnostics of the refused class**. The 12 warnings
`examples/sqlite/` does produce are `-Wincompatible-pointer-types`, the documented
`ptr` out-parameter exemption `corpus.rs:382` *requires* to be present. The corpus is
green because it looks and finds nothing, not because it does not look.

## The blocking sub-problem: §4.19's own verification trips the flag

Three seats found it and two characterised it precisely.

The ffi-pragmatist's reproduction is the worst case: **a program that declares
`strtok` and never calls it still fails.** §4.19's verification synthesises
`strtok((const char *)0, …)` inside the `_Static_assert`, and that assert is emitted
with **no `#line`** — so a location-gated recovery misses the *first* of three errors
and the whole compile stays exit **2**, *"internal error: compiling the generated C
failed"*. CLAUDE.md §7 violated outright, for a declaration the author wrote.

**The compiler-engineer found the repair, and found that the obvious one is wrong.**
Setting `zero_of`'s `Ty::Cstr` to `"char *"` silences `strtok` and then makes a
*correct* `const char **` out-parameter binding exit 2 — built and reproduced both
ways. The right repair is to spell a `cstr` zero as the **bare null pointer constant
`0`** (C11 6.3.2.3p3: assignable to any pointer type at any qualification), not as a
cast. **It is a fix even without the flag**: `(const char *)0` is why the emitter
warns about its own `_Static_assert` today.

## Resolution — provisional, author ratification pending

**(i) is adopted, and it does not land until all of its conditions are met.** Three
seats set conditions independently and one states that without them its verdict is
`veto`; the spec-warden's is the same rule from the spec's side — *landing the flag
without the diagnostic class trades a silent corruption for an unactionable exit 2,
which is a net §1.2 loss.* The conditions, merged:

1. **The assertion zero becomes the bare `0`**, not a cast — and this lands first,
   on its own merits, because the emitter warns about its own generated code today.
2. **A fifth exit-1 class** under panel 048's `declaration()` narrowing, covering the
   `_Static_assert` and not only the probe — which requires emitting the `extern`'s
   `#line` before each assertion, as `extern_probe.rs` already does for probes.
3. **The diagnostic names the repair, and the repair is not "add `const`"**: it is
   *"`strtok` writes through this parameter — declare it `ptr` and give it a buffer C
   owns"* (§4.17). It must read the offending parameter's **declared type from the
   value** rather than assert what it is (CLAUDE.md §11).
4. **The refusal is written into design.md Part 6 or Part 8 with a named falsifier**
   (CLAUDE.md §12 — a refusal carries a feature's burden of proof).
5. **A golden for the read-only 10%** (`putenv` or `tigetstr`) recording that the
   copy is the accepted cost, carrying the falsifiable claim *18 of 186 pay it*; if
   that fraction is ever measured above ~25% on a header a real program binds, reopen.

**The spec gains the +2 rewrite** — *"`s.cstr()` lends a `str` to C to read"* — and
**not** the +32 or +55 clause, the second of which licenses the dangle.

**(ii) is refused**, two vetoes, on a measurement rather than an argument: the `$own`
slot makes the copy unconditional and it does not fix the unique case.

**(iii) is refused for now**, two vetoes, as Part 7 item 10 under §13 — with the
historian's dissent on the record and the condition that reopens it stated by two
seats: a program **the closure list or §4.19's ladder needs** that binds a `char *`
parameter. Today the closure list binds two outbound `cstr`, both `const char *`
(`library/source.hero:122,131`).

**(iv) is refused** by every seat.

**Principle 0 admits none of (i)–(iii)**, and (i) enters the way panel 053's
`hero_cstr_nonnull` did: through §1.12, as a backend obligation of §7's existing
class. Measured cost of (i): **92 lines, 6 files, all in `emit/` plus one flag** —
zero in `lexer/`, `syntax/`, `resolve/`, `types/`, `ir/`, `own.rs`, `descriptors.rs`.
design.md Part 5's test — checker **and** lowering **and** backend — is failed by
(i) and passed by (iii), which is the right way round.

## Findings that are not about the ballot

- **The read direction is the bigger ergonomic hole, and no option here touches
  it.** The llm-ergonomist, blind on the spec, could not write `getcwd` in **any** of
  the three variants — not on the write rule, but because **the spec documents no
  `cstr` → `str` at all**. It holds C's answer and cannot print it; `print(got)` may
  print an address, since the spec's `==` line says a `cstr` compares as one. Verified
  after the sitting: `got.to_str()` **works** and prints the directory. This is panel
  048's own finding — *"the language is wider than its specification at the FFI
  boundary"* — with half of it still unrepaired. Queued.
- **The lifetime hole is untouched by every option.** A `const char *` that C
  **stores** passes all three variants' rules and dangles later. Reached
  independently by the llm-ergonomist (hesitation 7) and the spec-warden (the full
  clause *licenses* it). §4.19:2064 defers it to Part 7 item 10.
- **A buffer whose length Heroes knows is a §1.12 hole this sitting does not
  close** (ffi-pragmatist). `malloc`+`ptr` gives back no length, so `snprintf`'s
  truncation and `readlink`'s no-NUL return have no correct spelling. This is the
  condition under which (iii) returns.
- **A separate live defect, found while measuring and independent of the ballot**:
  `guard_cstr_arguments` (`emit/ops.rs:205-232`, panel 053's guard) narrows on the
  declared type `Ty::Cstr` and ignores `SlotKind::Param { mutable: true }`, so an
  `@cstr` out-parameter is emitted as `hero_cstr_nonnull(&h0_e)` — **a vacuous
  guard**, since it checks the address of a local, which is never null. Reproduced on
  unmodified `main` at exit 0. It compiles only because
  `-Wincompatible-pointer-types` is not in `FLAGS` as `-Werror=`. ~3 lines. Queued
  regardless of this panel.
- **`-Wcast-qual` is not in `FLAGS`**, which is why the shim route is silent. Not
  proposed here; recorded because it is the thing that makes (iv) invisible.

## Predictions to score

| judge | prediction | scored at |
|---|---|---|
| compiler-engineer | Adopted as measured, (i) stays inside `emit/` + `flags.rs` at ≤100 lines. At M-struct-passing close, `git diff --stat m-binding-fidelity..m-struct-passing` shows **zero** lines in `own.rs`, `owning.rs`, `types/table.rs` attributable to this question, and `ffi_mutable_parameter` has **zero** fires across `examples/` and `tests/golden/run/`. If any of the three files moves for this, (ii) or (iii) was smuggled in | M-struct-passing |
| ffi-pragmatist | A full ncurses binding — **all 28 plain-`char *` entry points, including the five read-only ones** — compiles, links and runs under the flag with **zero `.c` shim files**, by spelling that parameter `ptr`. `tigetnum` already returns 80. If any one of the 28 needs a shim, the flag is wrong and the verdict is withdrawn | M-ffi-ladder rung 5, or any time |
| spec-warden | `the_corpus_generates_c_that_compiles_without_a_single_warning` (`corpus.rs:382`) is green at M-ffi-ladder rung 5 with the flag live, and **0 of the corpus's ≥20 `extern` groups** declares a `char *` parameter (today 0 of 20; 41/1134 of the bindable population). ≥1 falsifies the refusal | M-ffi-ladder rung 5 |
| llm-ergonomist | On ≥20 binding tasks whose C function writes through a non-const `char *`, first-try **silent** failures — compiling programs that corrupt a `str` or no-op — are **≥60% with no rule and ≤5% with the `char *` rule: a delta of ≥55 points**. Second, non-competing: **T2-shaped tasks (C fills a caller buffer) succeed ≤10% in every variant**, because they die on `cstr` → `str` | first Part 11 run with a model harness |
| historian | If (i) is adopted as *declared* rather than at the call site, among the first 50 real C declarations Heroes binds at least one **non-writing** function is refused — `execv`/`execvp`/`execve`, glibc `iconv`, `getopt`. **Partly pre-falsified**: the ffi-pragmatist showed all three are `char **`, outside the flag's reach | M-ffi-ladder rung 5 |

## Process notes

- **Four of five judges built in copies with `target/` removed**; the fifth (historian)
  read no repository file, which is its brief. No judge reported the
  `CARGO_MANIFEST_DIR` symptom. The working tree was frozen from the briefs going out
  to this synthesis — the second observance of both halves of the `/panel` amendment.
- **The coordinator's framing was wrong twice and both are recorded.** The sitting was
  convened on the write direction, and the blind seat reports the **read** direction is
  the larger hole. The two spec clauses put on the ballot were priced at +32 and +55,
  and the sentence that says the same thing costs **+2** — the ballot's own numbers
  were the expensive ones.
- **A judge's named counterexamples were falsified by another judge inside the
  sitting**, which is the differentiated-input design working: the historian's `execv`
  and `iconv` are `char **`, and only the seat that compiles could see it.

## Ratification — 2026-08-15

**RATIFIED as it stands** (author instruction, `/decide`: *"rattifica tutto"*, given
after reading the synthesis — the fourth blanket ratification of the day and recorded
as that rather than as five individual reviews). **(i)** is adopted with all five
conditions binding; **(ii)** and **(iii)** stay refused on their two vetoes each;
**(iv)** stays refused; the spec takes the **+2** rewrite and not the +32 or +55
clause.

**This ratification converts a resolution into a work order, and that is the
difference from the day's other three.** 057 ratified a refusal — nothing was owed
afterwards. Here the yes authorises the five conditions, and until they are met the
compiler goes on behaving exactly as it does today: **the silent corruption is
reachable in a ratified language**, which is a state worth naming rather than
letting the tick imply otherwise. The conditions are not decoration on an adopted
option; three seats made them the price of their yes, and one said plainly that
without them its verdict is `veto`.

**The order the conditions must be worked in is fixed by one of them.** Condition 1
— the assertion zero becomes the bare `0` rather than a cast — is a repair **on its
own merits, today, with or without the flag**, because the emitter currently warns
about its own generated `_Static_assert`. It must land first and separately: the
obvious alternative (spelling the zero `(char *)0`) was built and **reproduced a
regression** on a correct `const char **` out-parameter binding, so the wrong repair
here is not hypothetical.

**What a blanket yes cannot settle**, and here it is unusually large: four of the
five predictions name `M-struct-passing`, `M-ffi-ladder` rung 5, or a model harness
that does not exist. Panel 046's R2 — *re-decided, never renewed* — governs them
unchanged. **The one to watch is the ffi-pragmatist's**, because it is checkable at
any time and its author offered it as a withdrawal condition: all 28 of ncurses'
plain-`char *` entry points compile and run under the flag with zero shim files. One
failure and the refusal is wrong.

**Three things the yes deliberately does not close**, each queued in its own right:
the **read** direction (the spec documents no `cstr` → `str`, and the blind seat
could not write `getcwd` in any variant); the **lifetime** hole (a `const char *` C
stores passes every option's rule and dangles later — §4.19:2064 defers it); and a
**buffer whose length Heroes knows**, which is the stated condition under which
option (iii) returns rather than a wish.

**On the historian's dissent**: ratifying the refusal of (iii) does not ratify the
claim that a separate mutable-buffer type is wrong. Four ecosystems reached it
independently and OCaml's took three years to land; §13 forbids it *now*, and the
sitting recorded the disagreement as being about the clock. If Part 7 item 10 is ever
taken up, this dissent is the argument that starts it and it should not be
rediscovered.
