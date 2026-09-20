# Panel 168 — compiler-engineer

- `verdict`: **approve** the proposal (leading header for route A; §4.19's third
  case left to the feature that builds it) — **and VETO the trailing header** if
  this sitting keeps panel 167 clause 1's allocation half. The veto is on
  soundness, not on price.
- `section`: **design.md §1.12** ("A Heroes program must not segfault and must
  not corrupt memory … it is a tie-break … which of two admitted shapes wins",
  `docs/design/design.md:571-593`), with **§1.1/§1.7** on the ceiling.
- `implementation_cost`: the proposal is **0 lines in every pass**. The
  alternative it displaces cannot be paid at all without raising two ratchets
  that measure at exactly their ceiling. Figures below, each with its command.
- `needed_for_self_hosting`: **no**. Neither layout is on the closure list;
  run rather than asserted: `grep -rn "\.lease()\|end_lease(" --include='*.hero' selfhost`
  returns **two hits and both are comments** (`selfhost/check/walk.hero:1349`,
  `selfhost/check/lending.hero:135`) once the four lease modules are excluded.
  The compiler does not lease.
- `argument` (≤120 words): The trailing header does not make the give-away case
  work; it makes the runtime the party that reads freed memory. At 8 bytes it
  panics and blames the compiler for the program's fault. At 65 536 and
  1 048 576 bytes the magic word **survives the free** on this machine, three
  runs of three, so the check passes and the release frees the block a second
  time — the corruption class §1.12 forbids, created by the layout that was
  adopted to prevent an abort. The leading header makes C's own `free` the
  faulting call, caught by the allocator at the fault, and ASan names the
  program's C line. And it costs nothing: the runtime already ships it.

---

## The measurements I ran

Every number below was produced in this session, on Darwin arm64, 2026-09-20,
in a copy of the tree at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e64edfa2-e1eb-4a90-b26e-8635f02430e5/scratchpad/work/tree`,
built with
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` — **4.29 real,
4.06 user, 0.10 sys** by `/usr/bin/time -p` (the brief's 3.80 is the same
command on an idler machine; the ratio is sane so the number stands).

### M1 — the release instruction carries the slot and nothing else. Confirmed.

`./heroes build <straight lease> --dump-ir`:

    $t3: cstr = call builtin lease($t2)
    store c <- $t3
    ...
    call builtin end_lease(@c)

`./heroes build <straight lease> --emit-c -o straight.c` then
`grep -n "hero_str_held\|hero_held_release" straight.c`:

    123:    t3 = hero_str_held(t2);
    135:    hero_held_release(&h1_c);

**In my own words**: `end_lease` is a one-operand instruction whose operand is
the *cell*. There is no length, no type, and no edge back to the initialiser.
Anything the release needs beyond the pointer must be recovered by a pass that
does not exist.

With a `while` between the lease and the release, same command: the lease is in
**`bb0`** and the release in **`bb3`** (`bb1` test, `bb2` body). The recovery is
a cross-block reaching-definition walk. For the `str` lease the length is not a
constant at all — it is `$t2`'s run-time `.len`, and `$t2` is dead at `bb3`.

### M2 — the decisive one, re-run, and then attacked at the shapes beside it

`clang -std=gnu11 -Wall -Wextra -Wpedantic -Werror docs/panel/168-briefs/layouts.c -o layouts`,
then five runs of each case:

| case | plain | ASan/UBSan |
|---|---|---|
| A leading, C frees, then `end_lease` | **exit 133** ×5, **stderr empty** | `bad-free`, `layouts.c:61 in c_destructor` |
| B trailing, C frees, then `end_lease` | **exit 134** ×5, the runtime's panic blaming the compiler | `heap-use-after-free`, `layouts.c:56 in trail_release_n` |
| C leading, ordinary | exit 0 ×5 | clean |
| D trailing, ordinary, correct length | exit 0 ×5 | clean |

The brief reproduces exactly. **What the brief did not do is vary the size**,
and that is where the trailing header stops being merely useless and becomes
unsound. `scratchpad/m2/sizes.c` runs the same give-away at
n ∈ {0,1,7,8,15,16,100,1000,4096,65536,1048576} and *reports* rather than aborts
what the release's magic check would decide. Three runs, identical:

    n=4096     magic survives the free: no   -> release would panic
    n=65536    magic survives the free: YES  -> release would DOUBLE FREE
    n=1048576  magic survives the free: YES  -> release would DOUBLE FREE

Run for real at n=65536 (`scratchpad/m2/big.c`, five runs): **no `panic:` line is
printed** — the magic check passes — and the process dies inside the second
`free`, exit 134, with nothing on stderr.

**This is the veto ground.** `runtime/parts/str.c:447-450` already states the
rule the trailing header breaks: *"freed memory owes nobody its contents."* The
leading header never reads the block after C frees it, because C's `free` on
`base + 16` is rejected by the allocator before the release runs. The trailing
header hands C the allocation base, so C's `free` succeeds, and the release then
consults a word inside a block somebody else may already own. Whether that word
reads as `HEROHELD` is the allocator's business. When it does, the runtime frees
a block it does not own. §1.12, and CLAUDE.md § Precedence rank 3.

### M3 — the two refusals, re-run, with one correction to the brief

`./heroes check` on a file with `record Box { p: ptr }` and
`function give(b: Box) -> ptr`: **exit 0**. Neither rule exists for `ptr`. The
`cstr` twin gets both, `error[cstr_in_a_record]` and
`error[cstr_out_of_heroes]`. Clause 2's premise stands.

**Correction the sitting needs.** The brief says `free(p: cstr)` against a
`void *` header "is refused before any of this is reachable … exit 1". Measured:
`./heroes check` on that file is **exit 0**. `error[ffi_writable_parameter]`
fires at **`./heroes build`**, exit 1. The conclusion survives; the stage does
not. A sitting that writes "the checker refuses it" would be wrong, and the gap
matters because `check` is what an editor runs.

### M4/M5 — the give-away case is reachable today, without route A

This is the finding I would put first if the report were ordered by weight.

A C function name is **not** a `ptr` (`error[type_mismatch]: expected 'ptr',
found '(function(ptr) -> ())'`) and a `cstr` does not convert to `ptr`
(`expected 'ptr', found 'cstr'`), so `sqlite3_bind_blob(..., sqlite3_free)` in
that literal spelling is unwritable. **Two spellings beside it are not.** An
`extern constant MY_FREE: ptr` bound to a macro that casts `free` is `check`
exit 0. And the destructor argument is not needed at all: a C function that
frees the `const char *` it is handed is the whole give-away.

    extern "giveaway.h"
        function eat(s: cstr)

    function main()
        x = "payload"
        c: cstr @ x.lease()
        eat(s: c)
        end_lease(@c)

with `static void eat(const char *s) { free((void *)(uintptr_t)s); }`:
`./heroes check` **exit 0**, `./heroes build` **exit 0**, and ten runs give
**133 133 133 133 133 133 133 133 134 134** — nondeterministic, **stderr empty
every time**. Under `./heroes build --sanitize`:

    ERROR: AddressSanitizer: attempting free on address which was not malloc()-ed
    SUMMARY: AddressSanitizer: bad-free giveaway.h:5 in eat

So the case panel 167 changed the allocation layout to fix is **already
expressible with the shipped `cstr` lease**, needs no route A, no field lease and
no `ptr`, and under the layout that ships today the sanitizer names **the
program's own C line** as the faulting party. Under the trailing header the same
sequence would name `hero_held_release`. Panel 167's ffi seat's ground for the
trailing header is therefore false twice over: it does not fix the case, and the
case does not wait on route A.

### The ceilings, re-measured in the instrument's own unit

The unit is `suite_layout.hero`'s `code_lines` (`tests/harness/suite_layout.hero:526-560`):
non-blank lines outside a `test` block. I replicated it in awk and **validated
the replica against four independent facts** — it returns `selfhost/ast.hero`
524, `selfhost/print/fmt.hero` 1174, `selfhost/parse/members.hero` 298 (panel
167's three ratchet numbers, to the line) and `selfhost/check/builtins.hero` 378
(exactly its `DECIDED` entry). `./heroes run tests/harness/main.hero -- ./heroes layout`
in the copy: **layout: 2 passed, 0 failed.**

| file | code lines | ceiling | headroom |
|---|---|---|---|
| `selfhost/ir.hero` | **310** | 310 (`DECIDED`) | **0** |
| `selfhost/emit/inst.hero` | **350** | 350 (`DECIDED`) | **0** |
| `selfhost/emit/ctype.hero` | **395** | 395 (`DECIDED`) | **0** |
| `selfhost/ir/print.hero` | 466 | 470 | 4 |
| `selfhost/ir/flatten.hero` | 1139 | 1150 | 11 |
| `selfhost/check/lending.hero` | **293** | 300 | **7** |
| `selfhost/lend_errors.hero` | **277** | 300 | **23** |
| `selfhost/emit/builtins.hero` | 244 | 300 | 56 |
| `selfhost/check/leasing.hero` | 202 | 300 | 98 |
| `selfhost/check/lend_extent.hero` | 216 | 300 | 84 |
| `selfhost/check/lend_types.hero` | 127 | 300 | 173 |
| `selfhost/check/lend_decls.hero` | 116 | 300 | 184 |
| `selfhost/emit/ffi_lend.hero` | 104 | 300 | 196 |

**Does the proposal touch a ratcheted file? No — it touches nothing.** The
leading header is `runtime/parts/str.c:419-428` and `:451-470` as they stand;
`wc -l runtime/parts/str.c` = 470, `runtime/heroes_runtime.h` = 602, and the
whole compiler-side surface of the feature is **two lines**,
`selfhost/emit/builtins.hero:140` and `:143` (`grep -rn "hero_str_held\|hero_held_release" runtime selfhost`
returns 20 hits, 18 of them in the runtime, 2 in `selfhost/`).

**Does the alternative? Yes, and at zero headroom.** Every route that makes a
trailing header findable needs the release to carry something the IR does not
have (M1). A new operand or a new IR slot field lands in `selfhost/ir.hero`,
which measures **310 against a ceiling of 310**, and reaches
`selfhost/emit/inst.hero` at **350 of 350**. Two files at zero, one at four, one
at eleven. `tests/harness/suite_layout.hero:483-491`: *"a file already over the
ceiling may not grow further."* That is the §1.1/§1.7 half of the veto.

### What route A's field lease costs on the leading header

Walking the four things that could need the allocation base:

1. **Finding the header at release.** Subtraction, `str.c:461`, unchanged. Zero
   lines, and — this is the part that decides the cost — **one** release
   function for both the `str` lease and the field lease. Under a trailing
   header the two have different padding, so `end_lease` stops being
   monomorphic and `emit/builtins.hero:143` stops being one line.
2. **Alignment.** Measured with a program linked against the real runtime:
   `sizeof(HeroHeldHeader)` = **16**, `_Alignof` = **8**,
   `_Alignof(max_align_t)` = **8**; six `malloc` returns all have
   `base % 16 == 0` and `(base + 16) % 16 == 0`. Sixteen is a multiple of
   `max_align_t`'s alignment, so the body is as aligned as the allocation base.
   A field lease of `i64[8]` or `f64[8]` needs nothing.
3. **The leak counter.** `hero_live_held`, `runtime/parts/alloc.c:349-358`,
   gated at `:200`. It is incremented at the allocation and decremented at the
   release under **both** layouts identically; in the give-away case the program
   aborts before exit under both, so the counter never speaks. No dependence on
   the base.
4. **The length.** `grep -rn "HeroHeldHeader" runtime selfhost` shows **nothing
   reads `h->len`** — it is written at `str.c:423` and never consulted. A field
   lease under the leading header therefore needs no new header machinery at
   all; under a trailing header the `pad()` formula must be computed identically
   in two functions, an invariant with no instrument.

**So: sound, and cheap.** My estimate for route A's field lease on the leading
header, against the files above:

| where | lines | headroom after |
|---|---|---|
| `runtime/parts/str.c` — a `hero_bytes_held(const void *, int64_t)` | ~10 | n/a |
| `runtime/heroes_runtime.h` — its declaration | 1 | n/a |
| `selfhost/emit/builtins.hero` — dispatch `lease` on the receiver, the shape `to_str` already uses at `:154` | ~4 | 52 |
| `selfhost/check/lend_types.hero` — the `ptr` arm of `lease`/`end_lease` | ~6 | 167 |
| `selfhost/check/lend_decls.hero` — clause 2's two widenings | ~2 | 182 |
| `selfhost/lend_errors.hero` — the `ptr` wording of two diagnostics | ~20 | **3** |
| `selfhost/parse/`, `selfhost/ast.hero`, `selfhost/print/`, `selfhost/ir*`, `selfhost/emit/inst.hero` | **0** | unchanged |

Forty lines, four compiler files, no ratchet touched. Pascal-P4 scale is not in
danger. **The tight one is `lend_errors.hero` at 277 of 300**, and nobody has
named it: two new diagnostics with message, two notes and a fix are ~20-30 lines
there, so clause 2 either reuses codes whose names lie (`cstr_in_a_record` firing
on a `ptr` field) or pays a split.

## The route nobody listed

**Two, and the second is the one that matters.**

**(a) Priced and refused: the hidden sibling local.** `h1_c__len` beside the cell
needs *which slots are lease cells* to reach the emitter. `is_lease_cell`
(`selfhost/check/leasing.hero:62-67`) **cannot be copied by the emitter**: it
reads `r.locals[local].value`, the resolver's record of a declaration's
initialiser — an AST-level fact that the IR does not carry (M1: the slot line is
`slots x: str · c: cstr`, a name and a type). Carrying it costs a field in
`selfhost/ir.hero`, **310 of 310**. And it buys nothing: M2 shows it does not fix
the give-away, and nothing reads `h->len` today. Worth having for no other reason
either — I looked for one and found none.

**(b) The route nobody listed, and it is not a mechanism.** For a third
candidate to exist, the *release* would have to decide, without reading the
block, whether the block is still the program's. Nothing in a pointer can say
that, and the runtime's own bookkeeping cannot either: a side table keyed by the
pointer (`hero_handle_slot`, `runtime/parts/alloc.c:368`, already shipping) would
answer *yes, mine* — C's `free` does not remove the entry — and the release would
free it again. **Every listed candidate tries to make the release smarter, and
the release is not the problem: the problem is that the program is obliged to
release at all.** The only thing that removes the obligation is a spelling that
waives `end_lease` — which is §4.19's third reserved case,
`docs/design/design.md:2332` ("a buffer that C takes ownership of"), and which
the proposal correctly leaves to the feature that builds it. So the answer to
question 4 is: **no third route exists at the allocation layer, and that is
provable rather than unsearched.**

## Question 5 — is the silent abort a defect, and against what

**Yes, and not against the layout.** The give-away program compiles at exit 0 and
dies with an empty stderr and an exit code that is not even stable (133 ×8,
134 ×2 in ten runs, measured). design.md §4.17 asks a diagnostic to carry
everything needed to fix the program; this carries nothing. File it against the
**language**, as: *a `cstr` lease may be handed to a C function that frees it,
`check` exit 0, `build` exit 0, and the program dies with no message.* Not
against the header layout, because both layouts die, and the leading one at least
tells the truth under `--sanitize`. The repair is §4.19's third case, or a rule
about the call, and neither is this sitting's.

**A second thing to file, smaller**: `runtime/parts/str.c:462-466` says *"this is
a compiler bug, please report it"* on a bad magic word. Under a trailing header
that message would be the **normal** outcome of a correct compiler and an
incorrect program (M2 case B). Under the leading header it stays rare and honest.
One more reason the layouts are not interchangeable.

## Prediction, with an instrument that exists today

At the **M-declared-extents close**, with route A's field lease landed on the
leading header:

1. `./heroes run tests/harness/main.hero -- ./heroes layout` is **green with no
   new row in `DECIDED`**, and `selfhost/ir.hero` still measures **310**,
   `selfhost/emit/inst.hero` **350**, `selfhost/emit/ctype.hero` **395** in
   `code_lines`. Zero lines land in `selfhost/parse/`, `selfhost/ast.hero`,
   `selfhost/print/` or `selfhost/ir/`.
2. The whole feature adds **≤ 40 code lines** across **≤ 5** files under
   `selfhost/`, `selfhost/lend_errors.hero` being the one that needs a split or a
   raised ceiling if it is not.
3. The counter-prediction, which is what makes this falsifiable rather than
   flattering: **if the trailing header is retained instead, `selfhost/ir.hero`
   exceeds 310 or `DECIDED` gains a raised ceiling.** There is no third outcome,
   because M1 measured that the release instruction has nowhere to put a length.

Scored by re-running the `layout` suite and the `code_lines` measurement above.

## What I could not settle, and why

- **Three platforms.** Everything here is Darwin arm64. The 65 536-byte magic
  survival is allocator behaviour; glibc's `mmap` threshold is 128 KiB by default
  so the 1 MiB case is likely the same there and the 64 KiB one may not be.
  **Unrun on Linux and Windows.** It does not change the verdict — one platform
  where the check silently passes is enough for §1.12 — but the sitting should
  not write *"at 64 KiB everywhere"*.
- **Route A's field lease is not built**, so my claim that it needs a
  `hero_bytes_held` distinct from `hero_str_held` is read from
  `hero_str_held(HeroStr s)`'s signature, not run. It is a reading, and I say so.
- **Clause 2's blast radius is measured but not run against the suites.**
  `grep -rn "^ \+[a-z_]\+: ptr$" --include='*.hero' examples tests selfhost`
  returns **5** record fields typed `ptr`, and `grep -rn -- "-> ptr"` returns
  **23** results across 17 files. At least one is a bare record outside any
  group: `tests/golden/unsupported/fixedbugs-pointer-reached-through-a-field.hero:32-33`,
  `record Slot` / `p: ptr`, which `./heroes check` passes at **exit 0** today and
  which clause 2's widening would **refuse**. Per
  `.claude/rules/verification.md`, a change to what the checker refuses is judged
  by every golden tree, not by the `selfhost/**` row. That cost is unpriced and
  it belongs to clause 2, which is not in front of this sitting — named here so
  it is not rediscovered.
- **Whether `heroes fmt` returns a field-lease program byte-identical** — panel
  167's engineer prediction — cannot be run until the form exists.
