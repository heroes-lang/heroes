# Panel 087 — the guard that could not fire, and the four doors behind it

**Convened** 2026-08-19, opening M-separate-compilation, after the baseline net came
back **exit 134**. **Trigger** the semantics of a built-in — `read_file`'s contract
— which is `spec/**` and a failure *class* (CLAUDE.md §4). **Lane** full panel:
five seats, differentiated inputs. **Status** `provisional — author ratification
pending`.

**Spec: unchanged. +0 tokens.** That is the sitting's first result and it was not
the proposal's plan.

## The proposal, verbatim

> `read_file(path: str) -> str?` (spec:179) **aborts** when the file's bytes are
> not valid UTF-8, and the abort is unreachable by any Heroes branch. Make it fail
> instead:
>
> 1. `runtime/hero_os.h:39-41` gains `HERO_OS_NOT_TEXT 3` beside the three.
> 2. `runtime/parts/os.c:114` guards `hero_str_from_bytes(buffer, got)` with the
>    `hero_utf8_valid` the runtime already has.
> 3. `selfhost/library_source.hero:145-153` gains a `fail("file_not_text", …)` arm.
>
> Status quo alternative: keep the abort, and make `tests/harness/suite_records.hero`
> filter paths on a premise about the world.
>
> Spec: one of six options, S0 (nothing) through S5, priced +0 to +22.

## What provoked it

`heroes run tests/harness/main.hero -- ./heroes` was **exit 134** at `a97c8d2`:
twelve suites green, then `panic: hero_str_from_bytes: not well-formed UTF-8` in
`records`, no summary line. A five-line program handling both `.ok` and `.err`
reached **neither arm**. Of 1373 tracked files exactly one is not valid UTF-8 — the
JPEG commit `abaa7ca` added, the first non-text file this repository has ever
tracked.

The finding underneath is `docs/defects/002-the-guard-the-language-took-away.md`:
the archived Rust twin (`archive/bootstrap-rs/heroes-cli/tests/milestones.rs:183`,
`:262`) reads `let Ok(text) = std::fs::read_to_string(file) else { continue };` and
survives this input by construction. The Heroes port transcribed those lines
faithfully to `suite_records.hero:148-150` and `:175-177` — and **the branch could
never be taken**, because the failure it exists for killed the process instead of
arriving as a value.

## The verdict table

| judge | verdict | section | its own finding |
|---|---|---|---|
| compiler-engineer | **accept-with-conditions**, as a **split** | §1.12 both halves | the guard is **5 lines and 3.47 s**, emission byte-identical, **0** emissions re-blessed, net 840/840 — and the *named code* is refused on Principle 0: `heroes mutate`'s `typo-code` row is **25 mutants, 0 killed**, the table's only zero |
| ffi-pragmatist | **accept-with-conditions** | §1.12's *completeness*, §1.11, §4.19 | compiled the class: **four doors, the proposal shuts one**. `args()` on non-UTF-8 argv is 134 with **no error channel to widen**; `.to_str()` on a real `sqlite3_column_text` is 134 — measured against real `sqlite3.h` |
| spec-warden | **accept** the behaviour, **S0 at +0**, **veto on S4** | §1.6, §1.2, §12 | §12 says the compiler has the bug, so the repair is free and a clause would *document* a defect. **Falsified S4 by running a program.** S2 is **false** at +1 token over S3 |
| llm-ergonomist | **accept-with-conditions**, prefers a clause | the thesis, locality | the two variants produce **character-identical programs**; what changes is **attribution**. Found the second door unprompted, from the spec alone |
| historian (advisory) | **accept-with-conditions** | precedent | the pattern held across six languages — **zero of six abort**. But every language that fails ships a **bytes escape hatch beside it**, and PEP 383 is the record of shipping without one |

## The finding that shrank the proposal by two thirds

Three seats, from three different inputs, converged on the same cut — and none of
them was asked for it.

The **compiler-engineer** was asked whether edit 2 could land alone. It could, and
the measurement is the sitting's centre: guard `os.c`, rebuild **from the seed
only** in 3.47 s, and the reproducer goes from `panic … 134` to `err read_failed`
exit 0, `heroes check <jpeg>` from **134** to `error: cannot read` exit **2**, the
`records` suite to 6/6, the whole net to **840 passed, 0 failed** — with the
emitted C for a hello-world **byte-identical** and **zero** files under
`tests/emission/` re-blessed. The mechanism is `runtime_object`'s cache key
(`selfhost/cli_toolchain.hero:186`), which hashes `runtime_text` over
`runtime/*.c runtime/*.h runtime/parts/*.c`. **The robustness win is fully
separable from the code-naming win.**

The **spec-warden** was asked which of six spec options to buy and answered
**none**. §12 decides it: `spec:179` already types `read_file` fallible, so the
document is already right and the runtime is what lies. Paying 11–21 tokens to
*describe* a defect inverts §12; fix `os.c` and the reader's inference becomes
**sound at +0**. It also found no payment available — `tests/harness/tasks/` does
not exist, so a reader-rate prediction is inadmissible under panel 046 R1, and it
declined to be the fourth sitting to reach for panel 051's held −8.

The **llm-ergonomist**, which saw only the spec, produced the reason the silence
was dangerous rather than merely thin: the spec announces every abort with the
word "abort" in **7 lines / 8 occurrences** (`:64`, `:142`, `:148` ×2, `:155`,
`:157`, `:172`, `:229`), so the silence at `read_file` reads as a *guarantee*. Its
programs under the two variants were **character-identical**, because no defence
is expressible either way; what the clause bought was not better code but correct
**blame**. Its own words: under the status quo a programmer "wrote T1 with no
hesitation at all … Confidently. Wrongly."

## The veto, and it killed the option another seat preferred

The ergonomist's counter-proposal was **S4** — one sentence in § Failure declaring
the abort list closed — on the argument that it fixes the *class* where a per-line
clause fixes the *witness*.

**The spec-warden vetoed it and falsified it with a running program**, which the
coordinator then reproduced independently:

```
function make<K>(k: K) -> {K: i64}      # keyed at f64 by the call
    m: {K: i64} @ {}
    m[k] @ 1
```
`heroes check` exit **0** · `heroes run` → `panic: a map key that is not equal to
itself (nan)`, exit **134**. A legal program, no compiler bug, and an abort the
spec never names — already on the books at `docs/panel/084` and `SCHEDULED.md`.
Eleven more reachable non-compiler-bug aborts among the runtime's **49 distinct
messages / 63 sites** go unnamed: `out of memory`, `array too large`,
`f64 render overflow`, `cannot create the C locale for rendering`, and the rest.

So S4 would have been **false on the day it shipped**, and under §12 the compiler
would have acquired a dozen bugs by fiat. CLAUDE.md §9's obligation — a claim gets
a test that makes it fire — is **not constructible** for it: you can pin the panic
count at 63, but that tests the count, not the sentence. **S4 does not fix the
class; it asserts the class is fixed.** The honest class fix names ~12 aborts and
is a different sitting.

The same seat killed **S2** on a point that is not pedantry: *"never abort"* is a
universal negative, and `read_file` calls `hero_alloc`, which panics
`out of memory` (`alloc.c:51`). The false wording costs **one token more** than the
true one (S2 +12, S3 +11).

## The four doors — and the proposal shuts one

The ffi-pragmatist was asked CLAUDE.md §1's fourth obligation: attack the repair at
the shapes *next to* the one that provoked it. It compiled all of them.

| call site | can a Heroes program survive bad input? |
|---|---|
| `os.c:114` `hero_file_read` | **no** → this repair fixes it |
| `os.c:62` `hero_args_at` → `args()` | **no, and unfixable by this shape.** `./argv $'\xff\xfe'` → 134. `args() -> [str]` has **no error channel** |
| `str.c:266` `hero_str_from_cstr` = every `.to_str()` | **no** — measured against real `sqlite3.h` |
| `text.c:87` `hero_str_chars` | safe **only because** `from_bytes` aborts. **The abort is load-bearing** |

The `sqlite3` case is §4.19's own ladder step 3: a real `extern "sqlite3.h" link
"sqlite3"`, `:memory:`, `insert … cast(x'fffe' as text)`, `sqlite3_column_text`,
**null-checked exactly as spec:229 instructs**, then `.to_str()` → `panic … 134`.
**spec:229 names `nullptr` as the reason, so a binding that obeys the spec to the
letter still dies.** The ergonomist reached the same conclusion from the document
alone, which is two independent seats on one sentence.

And the reason it cannot be repaired with an `extern`: the seat wrote
`bool hero_cstr_is_text(const char *p)` and bound it, and the emitter wraps **every**
`cstr` argument in `hero_cstr_nonnull`, so the emitted call is
`hero_cstr_is_text(hero_cstr_nonnull(t4))` — null → `panic`, 134. **A null-safety
predicate is categorically unbindable**, which is a §1.12 *completeness* failure in
§1.12's own words. The fix must be emitter-side composition, exactly like
`read_file`.

## Where the seats disagreed, unsmoothed

**Two disagreements, both between seats that compiled, neither resolved here.**

**1. Should the wrapper name `file_not_text`?**
- **ffi-pragmatist: yes** (its condition 3). `library_source.hero:143-145`'s own
  comment says codes are distinguished *"because a caller almost always wants
  to"*, and a JPEG and a failing disk are not the same program state. Absence is
  caught loudly: a wrapper naming an undeclared constant is
  `error[ffi_unknown_name]` at exit **1**, measured, on the author's own line.
- **compiler-engineer: no.** +2 `_Static_assert`s in **every program in the
  language** (measured: 10 asserts → 12), **142** emissions re-blessed, and the
  code lands in the one contract nothing checks (`typo-code`: 25 mutants, **0**
  killed). It found **no reader** anywhere in the tree —
  `selfhost/cli_input.hero:26` collapses every read failure into `unreadable`.
  Its condition for reversing: **a named reader in the same commit**.
- It also refused the cheap `status == 3` literal, for a reason of record rather
  than taste: `library_source.hero:129-134` records that **M-header-constants**
  moved those numbers into the header and that *"the header is the only place they
  are written"*. A bare `3` reverses a landed milestone in the one file whose
  comment names it.

**2. Should `spec:229` be amended in the same commit?**
- **ffi-pragmatist: yes** (its condition 1) — it is a measured falsehood in effect:
  obeying it does not save you.
- **spec-warden: no, and it ruled on this in advance.** Its S5 finding: extending
  `:229` *"turns a true floor into a two-path enumeration, row 3405's exact
  anti-pattern, for +11"*, and panel 059 ratified the floor. The sentence is
  **true and incomplete**, not false.

**3. The historian's objection, which is right in general and wrong for the
provoking case.** Every language that fails ships a bytes reader beside it; Heroes
has none — measured by the coordinator: the spec's only mention of bytes is `str`
being "indexed and measured in bytes" (`:51`), and the emitter's built-in list has
no byte reader. So `fail(…)` hands a program a branch it can *observe* but not
*act on*. **But its own falsifier is "a real program that does something useful
other than reporting and exiting" — and `suite_records.hero`'s `continue` is
exactly that.** It skips the photograph and finishes the walk. The general
objection stands; the provoking case is its counter-example.

## The resolution adopted — the most conservative one

**Landed now (this commit's siblings):**

1. `runtime/parts/os.c` — the pre-check before `hero_str_from_bytes`, releasing the
   buffer, setting `HERO_OS_NOT_TEXT`, returning the empty str. The abort in
   `hero_str_from_bytes` is **untouched**, because `hero_str_chars` depends on it.
2. `runtime/hero_os.h` — `#define HERO_OS_NOT_TEXT 3`, with the comment saying why
   it exists while nothing reads it.
3. **No spec change** (S0, +0). No library change. No new Heroes-visible
   `constant`, therefore **no emission churn and no re-blessing**.

Measured by the coordinator after landing it: seed rebuild **3.29 s**; the
reproducer `fail: read_failed`, exit **0**; `heroes check <jpeg>`
`error: cannot read`, exit **2**; `records` **6 passed, 0 failed**; hello-world's
emitted C **byte-identical**, 156 lines, `diff` silent.

**Why `read_failed` and not `file_not_text`:** it is the resolution that changes
the least Heroes-visible surface while making the net green, which is what
CLAUDE.md §4 asks a provisional default to be. **Its named cost, stated rather
than smoothed:** the message is `could not read <path>` for a file that read
perfectly well, which is mildly dishonest, and the ffi-pragmatist is right that a
caller usually wants to tell those apart. The runtime already distinguishes them;
only the wrapper collapses them. **One `constant` line reverses this** the day the
author says so.

**No ABI bump.** `HERO_RUNTIME_ABI` stays 14, on both compiling seats' reasoning
and one finding that settles it: the only instrument asserting the two ABI numbers
agree is `archive/bootstrap-rs/heroes-cli/tests/seed.rs:65`, in the tree CLAUDE.md
§5 says nothing builds. Bumping would move the guarantee off a live cache key onto
a dead Rust test. Skew is caught better anyway — `error[ffi_unknown_name]` at exit
1, measured.

**The status quo alternative is refused**, and not by the coordinator: filtering
harness paths is CLAUDE.md §11's forbidden shape verbatim — *"a narrowing asks the
value, never the world"*. "No file the record walk reads is binary" is a premise
about the world, and `abaa7ca` falsified it. Two independent measurements agree the
available filters answer a different question: git's own `-text` heuristic names
**three** tracked files, two of them valid-UTF-8 `.hero` fixtures with raw carriage
returns, and macOS `iconv` false-positives three `.rs` files.

## What a veto would compel

The ffi-pragmatist's veto, not cast: **any version that weakens the
`hero_str_from_bytes` abort itself.** `text.c:87` takes one byte off an invalid
sequence and depends on that abort for the language-wide well-formedness
invariant. The caller-side pre-check is the correct shape and must stay the shape.

The compiler-engineer's veto, not cast: a `[u8]`/`bytes` return, a second value out
of `read_file`, a `str` allowed to hold non-UTF-8 (`spec:51` reaches the checker,
the emitter and every runtime entry point), or a second fallible composition form
in the emitter.

## Predictions to score

| # | seat | prediction | scored at |
|---|---|---|---|
| 1 | spec-warden | `heroes measure` prints `maximum 3512` — this sitting spends **zero** spec tokens | M-separate-compilation close |
| 2 | spec-warden | `./heroes run tests/harness/main.hero -- ./heroes` completes with a summary line, exit 0 or 1, **never 134** | M-separate-compilation close |
| 3 | spec-warden | `grep -rl 'file_not_text' --include=*.hero .` returns **≤2** files; if **>3** must name it, the silence cost something real and S3 is re-argued | M-selfhost-fixpoint |
| 4 | compiler-engineer | with the guard alone, the net stays exit 0 and **zero** files under `tests/emission/` are re-blessed on this change's account (`git log --follow`) | M-separate-compilation close |
| 5 | compiler-engineer | if the named code lands, **zero** sites read it — no `e.code == "file_not_text"` outside a golden written to justify it — while `typo-code` stays at **0% killed** | M-separate-compilation close |
| 6 | ffi-pragmatist | the sqlite3 TEXT case and `./argv $'\xff\xfe'` **both still exit 134**; both must exit 0 for the class to be closed. Reproducers left at `panel087-ffi2/work/{sqlite,argv}.hero` | M-ffi-ladder close |
| 7 | llm-ergonomist | given the panic and the spec, models blaming the *compiler* rather than their own program: **≤3/10** under silence, **≥9/10** under a clause | when a metric-2 arm exists |
| 8 | llm-ergonomist | given T3, **≥3/10** programs branch on an **invented** `read_file` code (`"not_found"`, `"io_error"`) — silent failure. If it holds, the missing-codes gap outranks this sitting | when a metric-2 arm exists |
| 9 | historian | the first milestone whose harness must read a non-`.hero` artifact byte-for-byte **queues a bytes reader**; falsified if a real program does something useful with the `fail` arm | M-ffi-ladder |

## Queued, with the reproducers

Each of these left the sitting as an item rather than a change. **The commit does
not read as "the class is fixed"**, which was the ffi-pragmatist's condition 2.

- **`file_not_text` as its own code** — the two compiling seats disagreed; the
  author's call. The engineer's reversal condition is a named reader in the same
  commit (`cli_input.hero:24-28`) plus a golden that fires the arm.
- **`spec:229` and the second door** — two seats found it independently; the warden
  refused the wording on precedent. Needs a wording that is a floor, not an
  enumeration.
- **`args()` has no error channel** — the one door of the four that no status code
  can reach.
- **A null-safety predicate is unbindable** — `hero_cstr_nonnull` wraps every `cstr`
  argument, so the whole class of "ask C whether this pointer is safe" is closed.
  §1.12 completeness.
- **~12 unnamed reachable aborts** — the honest version of S4, as its own sitting.
- **A bytes reader** — the historian's objection; Principle 0 has no compiler-need
  for it today, measured.
- **`spec:14` is false**: *"strings and comments may contain any UTF-8"* against
  `error[raw_carriage_return]` at exit 1 for U+000D inside a string. Found by the
  warden while hunting a removal; a §12 repair at ~±0 tokens.
- **`suite_determinism.hero:13-14` names an instrument that does not exist**:
  *"`seed/heroes.c` is checked by `cmp` against what the seed-built compiler
  emits"*. Two independent greps found no such check in `tests/harness/` or
  `.github/workflows/`. The seed can go stale in silence. CLAUDE.md §11's exact
  shape.
- **Extern-group asserts are not pruned by reachability.** A hello-world's C is 156
  lines and **13 of them (8.3%)** are `_Static_assert`s and probes for a library
  `extern` group the program never touches. The emitter prunes the library's
  *Heroes* functions (`emit.hero:104-111` says why) and not its FFI half. Under
  M-separate-compilation those 13 lines land in all **153** translation units.

## Process notes

**Four of five seats died on a watchdog on the first attempt** — the coordinator's
briefing error, not theirs: they were told to rebuild the self-hosted compiler,
which takes **~16 minutes** in one command. The relaunched briefs carried a
3-minute-per-command rule and the cheaper route the coordinator had found
meanwhile (guarding `os.c` needs no compiler rebuild at all, because the runtime is
compiled per build from source). **Two seats then independently confirmed that
route**, which is the finding that shrank the repair from a language change to five
lines of C.

The panel skill still briefs the compiler-engineer with pointers into
`crates/heroes/src/`, archived at M-bootstrap-archive. Both compiling seats were
redirected to `selfhost/` + `runtime/` in the brief. `DECIDE.md` already carries
the item for CLAUDE.md §4's own stale path; the skill needs the same correction.

**The freeze held.** The working tree was untouched from the first brief until this
file was written, which is the rule that binds the seat that convened the sitting
hardest (panel 056's process note).

## Author's verdict

*(pending — `docs/debrief/DECIDE.md`)*
