# Panel 077 — the builtin that was already there

**Convened** 2026-08-16, M-selfhost-port, on `docs/debrief/DECIDE.md:349`, `:350`
and `:351` — one subject: what `partial` promises, and the union shape that still
answers wrongly. Full lane, five seats.

**This sitting refuted its own brief, and the brief was the coordinator's.** It
asserted that C cannot detect a union with one declared member, having checked
`__is_union` (C++ only), `sizeof` equality and the positional probe. **Two seats
that compile found the answer independently, in a builtin this emitter already
calls on line 10 of every generated FFI unit:**

```c
_Static_assert(__builtin_classify_type(*(UDef *)0) != 13, "…");
```

**A union classifies as 13, a struct as 12.** Verified by the coordinator after
both verdicts arrived; it fires on a **one-member** union, which no size
arithmetic can see.

## What was already broken, and how much of it the brief did not know

Measured against the compiler as it stood **after panel 073 landed this morning**:

| shape | verdict today | why |
|---|---|---|
| `union { i; f; }`, two fields declared | exit 1 `ffi_union_field` | panel 073's `sizeof(T) >= Σ sizeof(field)` |
| **`union { i; f; char pad[128]; }`, two fields declared** | **exit 0, prints 1065353216** | `128 >= 8` **passes** — and **`SDL_Event` is a padded union** |
| union, **one** field declared, not `partial` | exit 0, prints `5 true` | the predicate needs two fields |
| union as a **map key** | exit 0 | `extern_union.rs::used` walks `Construct` and `Eq`/`Ne` only |
| **`record U tag utag` over `union utag`** | **exit 2, internal error** | `typedefs.rs:145` spells `struct` unconditionally |
| `record S2 { a: i32 }` over a **two-field struct** | **exit 0, prints `1 true`** | clang exempts a bare `{0}` from `-Wmissing-field-initializers` |

The last row is the one nobody put on the ballot and it has **no union in it**.
The ffi-pragmatist produced its sharpest instance: `record FileStat tag stat`
naming `st_size` and **not** `partial` says **two different files — different
inodes, different mtimes — are equal**, at exit 0.

## Verdict table

| judge | verdict | the finding that decides it |
|---|---|---|
| **compiler-engineer** | approve 1 narrowed (size **+12**, construction **−13**) · object 2 · **veto 3** · approve 4 with two conditions | Found `__builtin_classify_type` and prototyped it: **+110 lines, 3 files, zero golden churn, 568/569** (one test needs splitting by case). Vetoes the marker route because a marker is a **spelling** mechanism and item C is *the author who did not write it*. And ruled the architecture question the next sitting inherits: union-ness **is not in the `.hero` source**, so moving this into the checker means **putting a C front end in the checker** — measured, `heroes check` would then need the target machine's headers, turning a verb whose job is *is this program wrong* into one that can exit 2 |
| **ffi-pragmatist** | approve 1 (+28) · object 2 · **approve 3, premise refuted** · object 4 · **the `{0}` fix is the sitting** | Same builtin, found independently. Measured option 4's blast radius over five real libraries by clang AST sweep: **12 of 382 named structs (3.1 %)** are unions or contain one; **SQLite 0, raylib 0** — §4.19's rungs 3 and 5 untouched. Attacked the size four ways (fields at the end, 16-byte alignment, by-value round trip, flexible array member) — **all four carry C's size**; the FAM case is caught by ASan naming the `.hero` line. And found that `char d_name[1024]` is **unbindable by any spelling**, so `partial` is load-bearing for `dirent` rather than a convenience |
| **spec-warden** | object 1, 2, 3 · **veto 4** · approve its own fifth | **Vetoed option 4 on a measurement**: `__is_union` is C++-only and a 2-member union is indistinguishable from a 1-member struct under the existing probe — so the trigger *as the brief spelled it* cannot fire. Found the `{0}` exemption and priced its repair at **two characters and +0 spec tokens**. Ruled the two silences apart: a wrong guess about **construction** is exit 1, a wrong guess about **size** is a correct program. And established a fact that binds every future sitting: **metric 2 has zero tasks**, so no prediction about a reader is admissible payment |
| **llm-ergonomist** (spec-only, blind, contamination disclosed) | approve 1 · **object 2** · object 3 | *"The document already answers the size question, and answers it wrong."* § Types says a record's extent is its fields; § FFI says it is C's struct; `partial` is the one construct where they diverge. **The size cannot be learned by trying** — no `sizeof`, no addresses — *"and `partial` is precisely the licence for clang's field check to stop firing, so the one instrument that could notice is the one the feature disables."* Its cost is **avoidance**: not a wrong program, a binding never written |
| **historian** (advisory) | approve 1 and 4-broad · object 3 | **Every FFI that let the field list decide the size shipped a wrong number**: Nim's `dirent` **272 vs 1**, Rust's `libc` `struct stat` **136 vs 120**; Go's zero-sized incomplete type is [#40507](https://github.com/golang/go/issues/40507), open since 2020. Every FFI that got it right **documents the rule** — cgo's padding sentence, Zig's *"cannot be stored"* — and Heroes is the only one that says nothing. **And it predicted a mechanism this sitting can have for free**: C11 6.7.2.3#1a (DR 251) makes `struct T` against a header's `union T` a **constraint violation**, so panel 074's `tag` marker already closes every **tagged** union. Verified by the coordinator — clang refuses it and suggests the word |

## The disagreement, and how it resolves

The warden **vetoed option 4** and two compiling seats **approved it** — and they
are not contradicting each other. The warden vetoed the trigger *as the brief
spelled it* (a member count, or `sizeof` equality), which genuinely cannot fire;
the other two replaced the trigger with one that can. **The veto's own condition
is met by the thing that answers it**: *"I flip on option 3 the day clang gains a
C-mode union predicate"* — it had one all along, and the seat was looking for the
C++ spelling.

The historian's contradiction stands on the record and is not resolved by
measurement: **Go has shipped `==` over C-union bytes for ~15 years with no
public bug report**. Refusing it goes further than Go, and the justification has
to be §12's robustness tie-break rather than precedent.

## The resolution — provisional, author ratification pending

1. **The predicate becomes `__builtin_classify_type(*(T *)0) != 13`**, replacing
   panel 073's `sizeof(T) >= Σ sizeof(field)`, which is **incomplete for a padded
   union and blind at one declared field** — and `SDL_Event` is a padded union,
   so the flagship case was still printing a false number three hours after the
   sitting that repaired it.
2. **It carries a premise-death control** (CLAUDE.md §11), emitted with it: two
   assertions stating that this clang still answers 13 for a union and 12 for a
   struct. clang answered 12 for unions before 2019; a toolchain that regresses
   would turn every union check into one that silently passes, and the control
   makes that a **build failure** instead.
3. **The trigger gains the map key.** `hash` was reachable where `==` was
   refused, which is worse than refusing neither.
4. **The tagged-union exit 2 is repaired**: the emitter must spell `union T`
   where the header says union, and a mismatch becomes a diagnostic on the
   author's line rather than an internal error. C11 6.7.2.3#1a gives this for
   free — clang refuses it and even names the right word.
5. **`extern_complete.rs` emits `(0)` instead of `0`**, closing a hole with no
   union in it: a one-field record over any multi-field struct silently claimed
   completeness. Two characters, +0 spec tokens, measured to leave unions alone
   and to raise no false positive on a real one-member struct.
6. **`extern_union.rs`'s marker carries the declaration's name**, not `c_type` —
   panel 072 rider 1's defect, reintroduced in a file written the same morning
   one file over from where it was fixed.
7. **The spec says the size and not the construction** (+12…+14 measured):
   construction's wrong guesses are all exit 1 with the rule in the message, so
   panel 035 R4 applies; the size has no diagnostic **because there is no
   mistake** — the reader who guesses wrong gets a correct program, and the cost
   is a binding never written. Two blind seats refused to write one.
8. **Option 3 (require a marker over a union) is refused** on the
   compiler-engineer's veto: a marker is a spelling mechanism and item C is the
   author who omitted it.

**What a veto at ratification would compel**: the predicate and the probe are
emitter strings; reverting is a revert, and the spec sentence is one line.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| compiler-engineer | `grep -ri union crates/heroes/src/types/` yields **zero** hits about a *C* union (4 today, all Heroes' own), `extern_union.rs` ≤ 240 lines, `ffi_record.rs` ≤ 300. If a C-union rule lands in `types/`, union-ness was frontend-knowable and the §1.7 argument was wrong | M-ffi-ladder |
| ffi-pragmatist | **zero** SQLite and raylib declarations need `partial` added, and **exactly one** binding in the project does — `SDL_Event` — needing one word and emitting byte-identical output. And the first FFI defect report after this lands is about `char`, not unions | M-ffi-ladder |
| spec-warden | `heroes measure` reads the funded number, a golden naming one field of a multi-field C struct moves **exit 0 → exit 1**, and **no existing golden turns red** — the three SDL3 read programs and the one-member union stay green | M-selfhost-port close |
| llm-ergonomist | with the size sentence, *"how many bytes is this?"* goes from **≥60 % answering the field list** to **≤5 %**; Task A's first-try correctness is **unchanged**, which is the measurement that most tempts the do-nothing option and is the wrong one | M-program-corpus |
| historian | at M-ffi-ladder a `grep` finds **zero** programs comparing two `extern` records with `==` or using one as a map key — so a broad refusal costs nothing real, exactly as Swift's does | M-ffi-ladder |

## Conditions on the record

- **compiler-engineer**: flips to object if `__builtin_classify_type` answers 12
  for a union on any CI platform — *and the control in (2) turns that into a
  build failure rather than a silence, so the evidence arrives on its own*.
  Option 3's veto downgrades if a marker that also carries union-ness repairs the
  tagged-union exit 2 in **≤ 40 lines**.
- **ffi-pragmatist**: **vetoes option 4 if it is adopted instead of making the
  declaration honest** — refusing `==` while the record is still spelled complete
  keeps §4.19's promise false in the source text. Withdraws the `(0)`
  recommendation on one false positive against a real header (measured: none in
  569 tests).
- **spec-warden**: flips on the size sentence the day metric 2 has ≥1 task, which
  is what would make a comprehension prediction admissible payment.
- **historian**: withdraws support for a broad `==` refusal if the ladder corpus
  shows ≥1 legitimate use, or if a shipped FFI is found that refuses equality on
  C aggregates and documented regret.

## Author's verdict

*Pending.*
