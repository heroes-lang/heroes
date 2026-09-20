# Panel 168 — shared brief

**Soundness lane**, two seats: `compiler-engineer` and `ffi-pragmatist`. The
proposal changes no surface, no diagnostic text and no spec token: it is an
allocation layout inside the runtime, and the mechanism the release uses to
find its own header.

**Every number below was produced by a command run while this brief was being
written, on 2026-09-20, on Darwin arm64.** The command is named beside each.
Nothing here is copied from `docs/`, from panel 167, or from defect 066's entry.
**Re-run anything you intend to rest a verdict on.** The coordinator's own
framing was wrong once already in this session and the correction is recorded
below rather than quietly applied.

## The proposal, in ten lines

Panel 167 clause 1 adopted a **trailing** header for route A's lease
allocation. Build route A on the **leading** header that already ships instead,
and leave design.md §4.19's third reserved case — a buffer C takes ownership of
— to the feature that builds it, with its own spelling that waives `end_lease`
and its own allocation question.

Clause 2 (the two type rules widened from `cstr` to `ptr`), clause 3 (defect 067,
landed at `69853b58`) and the refusals of routes B, C and D are **not** in front
of this sitting. What is in front of it is clause 1's allocation half and
whether route A's field lease is sound without it.

## What the runtime does today, read rather than remembered

`grep -n "hero_alloc_held\|hero_release_held" runtime/parts/*.c` and
`grep -n "HeroHeldHeader" runtime/heroes_runtime.h`:

| fact | where |
|---|---|
| `typedef struct { uint64_t magic; int64_t len; } HeroHeldHeader;` | `runtime/heroes_runtime.h:158` |
| `HERO_HELD_MAGIC` = `0x4845524f48454c44` | `runtime/heroes_runtime.h:160` |
| `hero_str_held` allocates `sizeof(HeroHeldHeader) + s.len + 1` and returns `(char *)(h + 1)` | `runtime/parts/str.c:419-428` |
| `hero_held_release(const char **slot)` finds the header by **subtracting** `sizeof(HeroHeldHeader)` | `runtime/parts/str.c:451-470`, the subtraction at `:461` |
| a bad magic word is **`hero_panic`**, and the message says *"this is a compiler bug, please report it"* | `runtime/parts/str.c:462-466` |
| `hero_alloc_held` / `hero_release_held` keep a counter, `hero_live_held` | `runtime/parts/alloc.c:349-358` |

`wc -l` on the family, today: `check/lending.hero` 386, `check/leasing.hero`
259, `check/lend_extent.hero` 304, `check/lend_types.hero` 138,
`check/lend_decls.hero` 130, `emit/ffi_lend.hero` 169, `runtime/parts/str.c`
470.

`grep -rn` over `examples/ tests/ selfhost/`, `*.hero` only: `.lease()` **28**,
`.cstr()` **126**, `.ptr()` **65**.

## Measurement 1 — the length does not survive to the IR

`./heroes build <a lease program> --emit-c` emits, on two adjacent lines of the
same function:

    t3 = hero_str_held(t2);
    ...
    hero_held_release(&h1_c);

`--dump-ir` on the same program: `$t3: cstr = call builtin lease($t2)`,
`store c <- $t3`, and later `call builtin end_lease(@c)`. **The release
instruction carries the slot and nothing else** — no length, no operand naming
the initialiser.

And with a `while` loop between the lease and the release, `--dump-ir` puts the
lease in **`bb0`** and the release in **`bb3`**. Recovering the initialiser from
the release site is therefore a cross-block reaching-definition walk. Neither the
emitter nor the checker has one, and `selfhost/check/leasing.hero`'s module doc
(lines 25-30) names the **absence** of flow analysis as the reason its rule is
sound: *"A lease cell is recognised by its declaration's INITIALISER being the
lease call, through the resolver — a fact about the cell in hand."*

For the `str` lease the length is not a constant at any rate: it is `$t2`'s
run-time `.len`, and `$t2` is dead by the release.

## Measurement 2 — the decisive one, and it is not about finding the header

A C model of both layouts is committed beside this brief as
**`docs/panel/168-briefs/layouts.c`**, 94 lines by `wc -l`: two allocators, two
releases, and a `c_destructor` that does what `sqlite3_free` does. It is here
rather than in a scratchpad because a session-specific path goes away with the
session, which is the rule panel 147 gave this directory its home for.

The release in the model **panics exactly as the runtime does** on a bad magic
word, because a first version of the model returned quietly and that made the
comparison flatter one side. The trailing release was handed the **correct**
length.

`clang -std=gnu11 -Wall -Wextra -Wpedantic -Werror`, five runs each:

| sequence | plain, Darwin arm64 | `-fsanitize=address,undefined` |
|---|---|---|
| **A** leading, C frees, then `end_lease` | **exit 133**, five of five, **and no message at all** | `bad-free`, named at `c_destructor` — C's own line |
| **B** trailing, C frees, then `end_lease` | **exit 134**, five of five, with the runtime's panic saying *"this is a compiler bug, please report it"* | `heap-use-after-free`, named inside the release |
| **C** leading, ordinary case | exit 0, correct bytes | clean |
| **D** trailing, ordinary case, correct length | exit 0, correct bytes | clean |

**What this settles.** The trailing header does not make the give-away case
work: it aborts too. With the correct length in hand it still reads freed memory,
because C has already freed the block — so neither of panel 167's two recorded
candidates (a pointer-keyed side table, or the emitter passing the length)
addresses the failure. They address only the ordinary case, which the leading
header already gets right, identically.

**A correction the coordinator owes this sitting.** The first framing of this
measurement, written before the model was aligned with the runtime, said the
leading header gives a *loud abort* and the trailing one a *silent exit 0*. Both
halves are false. A is **silent** on Darwin — exit 133, no message, not even from
malloc, with `MallocErrorAbort=1` making no difference. B is **loud and wrong**,
naming the compiler for a fault that is the program's.

## Measurement 3 — what the compiler already refuses, run rather than asserted

`free(p: cstr)` declared in a group is refused before any of this is reachable:

    error[ffi_writable_parameter]: `p` of `free` is declared `cstr`, and the
    header says `void *` — C does not promise to leave it alone

exit 1. So the give-away case is reached only through a `const`-qualified
parameter, `sqlite3_bind_blob`'s `const void *` being the one panel 167 used.

And panel 167's clause-2 premise is **re-run and stands**: a file holding
`record Box` with field `p: ptr` and `function give() -> ptr` is `check` **exit
0** today. Neither type rule exists for `ptr`.

## What this sitting is asked

1. Is the proposal right — leading header for route A, and §4.19's third case
   left to the feature that builds it?
2. Is route A's field lease **sound** on the leading header, or does something
   in it need the allocation base that only a trailing header gives?
3. Is clause 2's widening still what makes route A sound, given measurement 3?
4. **Is there a route nobody has listed?** Panel 167 recorded two candidates and
   both answer a question that turns out not to be the one that fails. Say what
   would have to be true for a third to exist, and search for it.
5. The give-away case ends in an abort under **both** layouts and under neither
   does the program learn what it did wrong. Is that a defect to file, and
   against what?

## Working rules for this sitting

- **Build in a copy.** `cp -r` the tree to your own scratchpad, then
  `rm -rf target build` in the copy. A judge's measurement is worth having and
  its working tree is not.
- **The cheap route to a compiler**:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, measured at
  **3.80 s real** today. Rebuilding from `selfhost/` takes roughly twenty
  minutes and will kill you on the watchdog.
- Never `archive/bootstrap-rs/`: nothing builds it.
- Write your report to `docs/panel/168-reports/<your seat>.md`.
