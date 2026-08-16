# Panel 074 — the word, and the drawer it opens

**Convened** 2026-08-16, M-selfhost-port, as the completion of panel 072. That
sitting fixed the mechanism on two seats and refused to name the word: *"This
sitting is entitled to say the mechanism is this and no other; it is not entitled
to put a word in the language on two seats."* **The two missing seats sat here:
llm-ergonomist and spec-warden.** The mechanism was not reopened.

## What was asked

Panel 072 settled that an `extern` group's `record` carries a **marker naming the
C tag**, because 4 of its 155 measured tags — `stat` among them — collide with a
function in Heroes' single namespace, so a boolean marker cannot work. Open:
**the spelling**, and **the sentence § FFI then owes**.

Ballot: **A** `record FileStat tag stat` · **B** `record FileStat as stat` ·
**C** `record FileStat is "struct stat"` · **D** `record FileStat c_name stat`.

## What the coordinator measured before the briefs went out (2026-08-16)

- The defect reproduces: `record TagOnly` against `struct TagOnly { int32_t a; }`
  is **exit 2, *internal error: compiling the generated C failed*** — clang saying
  `must use 'struct' tag to refer to type 'TagOnly'`, the compiler blaming itself
  for the author's header.
- Over six real header sets by clang's JSON AST: **323 struct tags, 109 tag-only**
  (public names only: 275 / 81). POSIX 110/85, curl 18/15, zlib 17/6, sqlite3
  23/3, SDL3 120/0, raylib 35/0. The platform is tag-only; application libraries
  are not.
- **6 of 328 tags are also a function or an object** — `flock`, `sigaction`,
  `sigvec`, `stat`, `timezone`, `wait`, all POSIX. Panel 072 measured 4 of 155 on
  its own set; this session's wider survey finds two more, and the conclusion is
  unchanged and stronger.
- `struct` and `union` are unavailable as the word: both are in the foreign-word
  registry with a `certain` fix (`crates/heroes/src/lexer/keywords.rs:61,63`).
- Spec base **3440**, ceiling **4096**, headroom **656**. `cargo test` **566
  passing**.

## Verdict table

| judge | verdict | the finding that decides it |
|---|---|---|
| **llm-ergonomist** (spec-only, blind) | **approve `tag`** (conditional) · **object `as`** · **veto `is "…"`** · approve `c_name` second | Wrote the three tasks first. **Task A it could see failing from the page** (`record stat` collides with `function stat`); **Task B it wrote confidently and wrong** — *"Task A fails loudly and in Heroes; Task B fails invisibly and somewhere else."* On `as`: every language spends `as` on introducing the **new** name (`import numpy as np`), so it would have read the swap backwards, and `spec:9` already says *"No aliases"*. On the string: the payload's own grammar is unstated, so `"struct stat *"` is guessable, and *"whether the declared thing is a value or a pointer is decided by characters inside a string that Heroes does not parse"* |
| **spec-warden** | **approve at a folded wording, +22** · object to every appended wording · **veto `is "…"`** on §1.0 | Priced the ballot and **rewrote it**: appending a sentence leaves `spec:217`'s *"the same name"* contradicted four lines above its own example. The fold — *"all its fields, and the same name unless `tag` gives C's"* — is **+26 against a −4 named removal, net +22 (3462)**, twelve tokens cheaper than the cheapest appended form. **And it found the finding of the sitting**: `emit/ffi.rs:127`'s `declaration()` matches Function and Constant and drops `record` through `_ => return None`, so a name this program *did* declare `extern` never reaches exit 1 — CLAUDE.md §7's own rule, unimplemented for records |

**Convergence, from two seats with disjoint inputs**: `tag` wins, the free-form
string is vetoed twice for two different reasons, and **both make the same
condition** — the marker's absence must be a Heroes diagnostic on the author's
line, not an internal error. Neither seat was told the other's brief.

## The disagreement, and it is small

The warden reports **no token preference between `tag` and `as`** (both 3462) and
declines to break the tie: *"§1.1 says tokens break ties only when comprehension
is indifferent; here they are equal, so my seat contributes nothing and the
llm-ergonomist decides alone."* The ergonomist objects to `as` on a measured
prior about how every other language spends that word. **So the tie is broken by
the seat whose brief it is**, which is the panel working as designed.

The ergonomist's second choice is `c_name` — *"safest on direction, weakest on
payload"* — and it says plainly that if the header check is **not** adopted, it
switches its first preference, because then the word alone must carry the whole
burden of preventing a swap.

## Two things the sitting found that nobody asked about

1. **`partial`'s size is unstated, and the reader must guess it.** The ergonomist
   could not write Task B without assuming that a `partial` record still has the
   header type's size — *"if that assumption is wrong my program corrupts the
   stack, and the specification gave me no way to find out."* Measured by the
   coordinator after the verdict came in: a `record Big partial` naming one `i32`
   of a 128-byte struct, passed as `@b: Big` to a C function that writes 124
   bytes, comes back **complete and correct** (checksum 21204 = 124 × 0xAB, exit
   0) — the emitter declares the local with the header's own C type. **The
   behaviour is right and the document is silent.** Filed.
2. **Three hesitation points block a POSIX binding even after the word lands**,
   all from the blind seat: whether a `partial` record can be *constructed* at
   all (every out-parameter binding needs it — `stat`, `gettimeofday`, `dirent`,
   `addrinfo`); whether extern record fields are matched **by name or by
   declaration order** (if by order, a transposed pair is a silently different
   program); and how a reader resolves `mode_t` to a width and a sign, which the
   spec demands and the header does not give. Filed as their own items.

## The resolution — provisional, author ratification pending

1. **The word is `tag`**, in the modifier slot beside `partial`, spelled
   `record FileStat tag stat partial`. The free-form string is **vetoed by both
   seats**; `as` is objected to by the seat that owns the question; `c_name` is
   the runner-up and returns if condition 3 below is not met.
2. **The spec sentence is the warden's fold, not an appended clause** — it
   repairs `spec:217`'s *"the same name"*, which the marker falsifies, instead of
   standing beside it. **+22 net, 3440 → 3462**, against a −4 named removal, with
   the ledger row written into `measure/gate.rs`.
3. **`emit/ffi.rs`'s `declaration()` gains its record arm in the same commit.**
   Both seats made it a condition, from opposite directions: the warden because
   the §1.2 payment is otherwise unbanked, the ergonomist because a missing
   marker must fail *in Heroes*. Without it the sitting's approval lapses.
4. **Rider 1 of panel 072 lands with it** (`DECIDE.md:344`): the field
   assertion's marker must carry the **declaration's** name rather than the C
   type's, because `struct stat` is two words and `emit/ffi_record.rs:46` splits
   on whitespace. It is untestable until today and mandatory from today.
5. **`emit/typedefs.rs` (291 lines) is split along a named seam** with the change,
   per CLAUDE.md §11.
6. **`struct stat` is the acceptance test**, as panel 072 required: 18 fields,
   four nested tag-only `struct timespec`, `@buf` as an `@` parameter, exit 0.
7. **The marker is NOT extended to `union`** — panel 072's ffi-pragmatist veto
   stands until panel 073 closes.

**What a veto at ratification would compel**: the word is one token in the
grammar and one sentence in the spec; reverting is a revert. The
`declaration()` repair at (3) stands regardless — it is CLAUDE.md §7's existing
rule, and the exit-2 it fixes is a defect under any spelling.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| llm-ergonomist | direction swaps (the Heroes name written into the C slot) at N≥20 first tries: `as` ≥25%, `tag` ≤5%, `c_name` ≤2%; marker omitted entirely ≤10% with a sentence that keys on the visible `struct timeval` text, ≥30% with one that keys on the tag/typedef distinction as a property of C | M-ffi-ladder's POSIX rung |
| llm-ergonomist | Task B (`gettimeofday`) goes from 0% to ≥70% first-try; **Task A stays under 50% whatever the word**, and the residue is the out-parameter and field-order questions, not the marker | same |
| spec-warden | the clause lands at exactly **3462**; at the next rung `find examples -name "*.h"` is still 0, ≥1 real `tag` binding exists in the corpus, and **`SPEC_TOKENS` takes no second tag-related amendment** — the third conjunct written to fail, because 072's live union veto makes a top-up plausible | M-ffi-ladder's next rung |

## Conditions on the record

- **ergonomist, approve → object** on `tag` if first-try measurement shows readers
  writing `tag struct stat` (payload confusion) above 10%; its first preference
  becomes `c_name` if the header check is not adopted.
- **ergonomist, veto on the string lifts** only if the payload is restricted to
  `struct <identifier>` and checked against the header — *"at which point it is
  candidate A wearing quotation marks and three extra tokens."*
- **warden, approve → object** if the marker lands without `declaration()`'s
  record arm, or on any appended (non-folded) wording.
- **warden's veto on the string stands** unless a binding on the closure list or
  a Part 11 metric needs a C spelling that is not `struct <identifier>`.
- **Neither seat is compiler-need**: `selfhost/` has **no `extern` group at all**
  (the six hits are the lexer's own keyword string), and the closure list's only
  group binds `hero_os.h`, which this project writes and which declares no
  struct. The marker pays as a **§12 repair** of a false spec sentence, and the
  ledger row says so.

## Author's verdict

*Pending.*
