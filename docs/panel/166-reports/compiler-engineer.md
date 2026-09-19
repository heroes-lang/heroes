# Panel 166 — compiler-engineer

Every number below was run on 2026-09-19 in a scratchpad copy of the tree at
`3702e3d2`, with the compiler built from the seed in that copy:
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, **4.055 s total,
3.72 s user**. Nothing here was measured with the repository's own gitignored
binary and nothing was measured in `archive/bootstrap-rs/`.

Line counts are in `tests/harness/suite_layout.hero`'s unit (`code_lines`,
`suite_layout.hero:525-543`: every non-blank line outside a `test` block),
reproduced in awk and cross-checked against the `DECIDED` table — my replica
reads `selfhost/emit/gate.hero` 365 against its decided 365 and
`selfhost/check/builtins.hero` 378 against its decided 378, which is what says
the replica is right.

---

## The measurement the four routes are priced against

| file | layout lines | ceiling | headroom |
|---|---|---|---|
| `selfhost/check/lend_types.hero` | 127 | 300 (§11) | **173** |
| `selfhost/emit/field_lend.hero` | 100 | 300 | 200 |
| `selfhost/check/lending.hero` | 250 | 300 | 50 |
| `selfhost/lend_errors.hero` | 167 | 300 | 133 |
| `selfhost/parse/members.hero` | 263 | 300 | 37 |
| `selfhost/check/contextual.hero` | 168 | 300 | 132 |
| `selfhost/emit/builtins.hero` | 244 | 300 | 56 |
| `selfhost/emit/extern_field.hero` | 286 | 300 | 14 |
| `selfhost/emit/extern_probe.hero` | 296 | 300 | **4** |
| `selfhost/check/builtins.hero` | 378 | 378 (`suite_layout.hero:417`) | **0** |
| `selfhost/emit/gate.hero` | 365 | 365 (`suite_layout.hero:421`) | **0** |
| `selfhost/ir/flatten.hero` | 1139 | 1150 (`suite_layout.hero:412`) | **11** |

Two of these the brief did not have and both are load-bearing:
`emit/extern_probe.hero` has **four** lines of room, and `ir/flatten.hero` has
**eleven**. `emit/gate.hero` is at zero as the brief says — and **no route on
this table needs a row there**, because the gate refuses `cstr` as a container
element and not one of A–E introduces a type.

## Principle 0, once, for all four routes

**`needed_for_self_hosting`: no.** The compiler's own `extern` groups are four —
`selfhost/cli/io.hero:34`, `selfhost/cli/process.hero:37`,
`selfhost/cli/process.hero:49`, `selfhost/emit/literal.hero:41` — and **not one
of them declares a `ptr` parameter**. `.ptr()` occurs in `selfhost/` only inside
`lend_errors.hero`'s message text and `check/lending.hero`'s test strings. So
`.ptr()` is not on the closure list; it entered on design.md §1.12's
completeness clause (*"any C library must be bindable"*), and every route here
is judged on §1.12 and Part 11, never on compiler-need.

---

## Route A — refuse `.ptr()` from an immutable binding

- **verdict**: object
- **section**: design.md §4.19 (*"the wrapper §4.19 prescribes as the route out
  of a module"*, quoted by `selfhost/check/lending.hero:39-41`); design.md §1.12
  for what it fails to close.
- **implementation_cost**: **~33 lines, all frontend, none in lowering, the
  descriptor pass, the ownership pass or the backend.**
  `selfhost/check/lending.hero` (250/300) +~16: a `root_name_of` beside
  `rooted_at_a_name` (lines 247-253) returning the root expr id, and the
  mutability question in the sweep at line 231. The data is already in hand —
  `lent_only_into_c` takes `r: resolved.Resolved` (line 201), `r.uses[root]`
  answers `.local(at:)`, and `resolved.Resolved.locals[at].mutable` is the bool
  (`selfhost/resolved.hero:93` and `:119`). Nothing is threaded.
  `selfhost/lend_errors.hero` (167/300) +17, one diagnostic in the shape of
  `field_lend_needs_a_place` at lines 160-175. Plus a golden in
  `tests/golden/check/` with its `#~` annotation.
- **argument**: cheap and sound and aimed at the wrong half. It closes 065 and
  leaves 063 — the memory corruption, rank 3 in CLAUDE.md § Precedence —
  untouched. And the over-refusal is not hypothetical: I ran it.
  `function name_sum(s: Sl) -> i64` returning `sum_n(p: s.name.ptr(), n: 8)`
  runs today, **exit 0, prints 576**. A by-value parameter is `mutable: false`
  (`selfhost/resolve/binding.hero:20-23` via `resolve/decls.hero:74`), so route
  A refuses it — and that is the read-only wrapper §4.19 prescribes, the exact
  ground on which panel 122 dropped the extern discriminator. It also refuses
  `check/lending.hero:304` and `:309`, two of the four tests panel 164 shipped.
  Route A says *the binding may be written*; the question is *does C write*, and
  the language cannot see that from the binding.
- **condition**: I withdraw the objection if the refusal is narrowed to the
  parameter the lend reaches rather than the binding it comes from — which is
  route B's question, not A's.

## Route B — implement panel 164's resolution 2

- **verdict**: **veto**, on soundness.
- **section**: design.md §4.19 line 2192 — *"a C out-parameter is an `@`
  parameter, which §4.8 already compiles to a pointer"*; design.md §1.5
  (declaration/use asymmetry).
- **implementation_cost**: larger than every other route, and it is not a type
  rule. `@` on a `ptr` parameter is a new parameter **mode**, so `ast.Param`
  gains a third state and `resolve/`, `check/walk.hero`'s `marker_mismatch`,
  `ir/flatten.hero`'s argument lowering (11 lines of headroom),
  `emit/extern_probe.hero` (4 lines of headroom) and the call emitter all branch
  on it. The instrument for the floor is the last comparable landing:
  `acquires` at `7e3bb986` touched twelve `selfhost/` modules for **+160 lines**
  and it did **not** change an argument's lowering. B is strictly above that.
- **argument**: the syntax is occupied. `heroes build --emit-c` on
  `function sl_fill(@p: ptr, n: i64)` with `sl_fill(@q, n: 0)` emits
  `(void)sl_fill((void *)&h0_q, t2);` and the probe
  `static void hero_ffi_probe_h_atptr2_sl_fill(void * * a0, int64_t a1) { (void)(sl_fill)((void *)a0, a1); }`.
  Today `@p: ptr` means **C writes a new pointer back into my variable**, a
  `void **` out-parameter. Resolution 2 asks the same spelling to mean *C writes
  through the pointer into the bytes it points at*, the two told apart only by
  whether the argument happens to be `@field` or `@name`. That is why panel 164's
  *"only the type rule is missing"* never landed: it is not a missing type rule,
  it is a second meaning for a mark that has one.
  And clang cannot catch the confusion: `emit/extern_probe.hero:181-195`
  (`probe_arguments`) pushes `(void *)aN` for every opaque or numeric `@`
  parameter, deliberately and with its reason written at lines 172-180. So the
  `void **` reaches a `void *` with no diagnostic — I built and ran that program,
  **exit 0**.
- **condition**: the veto lifts if the write direction gets a spelling of its
  own — a distinct mark on the parameter, not `@` overloaded. That is route C's
  grammar, not resolution 2's.

## Route C — the counted parameter

- **verdict**: approve, conditionally. It is the only route that touches 063.
- **section**: design.md Part 5 (it adds nothing to the seven constructs);
  design.md §4.19 lines 2333-2336, *"a binding annotation vocabulary will
  eventually be needed... Reserve a keyword"*; design.md §1.12, *"the compiler
  and the runtime check rather than assume, and they check at the boundary even
  where the value is known to be good."*
- **core or sugar**: **neither — it is a declaration-site mark.** It adds no
  construct to the type checker AND the lowering AND the backend. The type
  checker reads it, the lowering drops it, the emitted C is byte-identical. That
  is the shape `owned`, `consumes`, `acquires` and `borrows` already have
  (`selfhost/parse/members.hero:110-147`), and Part 5's cost argument does not
  reach it.
- **implementation_cost**, by place:
  - **grammar, +~13**: `selfhost/parse/members.hero` (263/300, 37 of room), a
    `counted_by_marker` copied line for line from `acquires_marker` at lines
    117-131 — the one existing marker that already carries a NAME. Plus one
    field on `ast.Param` and one at the construction, `members.hero:184`.
  - **checker, +~22**: `selfhost/check/lend_types.hero` (127/300, **173 of
    room**). **The instrument exists and is proven by a twin**:
    `selfhost/check/walk.hero:317-338`'s `fixed_bound_check` already reads
    `.fixed x`'s `x.length` off the type table, reads the literal with
    `contextual.literal_value` (`selfhost/check/contextual.hero:140-175`), and
    pushes `ffi_errors.fixed_index_out_of_range`. Route C's check is those same
    twenty lines with a different operand.
  - **emitter, +0.** The brief prices C in "the grammar, the checker, and
    `emit/extern_probe.hero`". `emit/extern_probe.hero` needs **nothing**: the
    mark changes no C signature and no probe argument. Its four lines of room
    are not spent.
  - **the carrier the brief does not name.** Measured from `7e3bb986`, the
    commit that landed `acquires`: `print/fmt.hero` +13, `print/dump.hero` +25,
    `parse/tails.hero` +5, `resolve/qualified.hero` +6, `resolve/state.hero` +3,
    `resolve/types.hero` +3, `ast.hero` +14, `measure/pinned.hero` +6 (and that
    one needs `.env`, per `.claude/rules/records.md` § Working in lanes), plus
    `spec/heroes-spec.md` +13 and `suite_spec.hero`. A new contextual word also
    owes `.claude/rules/diagnostics-and-goldens.md` § A new surface form: the
    formatter round trip, the dump row, `heroes mutate`, `heroes measure`,
    `editors/vscode/syntaxes/heroes.tmLanguage.json` and
    `site/src/lib/highlight.ts`.
  - **total: ~210 lines**, against `acquires`'s measured 160 of carrier.
- **argument, and this is the condition**: `counted_by n` naming **one** sibling
  does not express this repository's own corpus. The brief's 66/18 reproduce
  exactly, and both carry duplicates — deduplicated they are **46 distinct
  declarations and 12 with a length-looking sibling**. I read the twelve, which
  the brief asked a seat to do. At most six are a genuine (pointer, extent)
  pair. Two of the six that are not state the extent as a **product**:
  `fwrite(p: ptr, size: u64, n: u64, f: ptr)`
  (`tests/golden/check/owned-freer-must-be-declarable.hero:35`) and
  `qsort(base: ptr, n: u64, size: u64, cmp: ptr)`
  (`tests/golden/check/fixedbugs-owned-asks-the-result-too.hero:41`). One states
  an extent belonging to a `cstr` and not to the `ptr` beside it:
  `sqlite3_prepare_v2(db: ptr, sql: cstr, n: i32, @stmt: ptr, @tail: cstr)`
  (`tests/golden/fixedbugs/ffi-out-parameter-guard.hero:36`). A mark that cannot
  say `n * size` re-opens 063 on the two commonest functions in C, at exit 0.
  Two further things the resolution must say in words, because leaving either
  silent is how panel 165's sentence became unimplementable:
  1. **The absence of the mark must be a refusal, not silence.** `.ptr()` may
     stand only as the argument of a parameter that declares its extent.
     Otherwise 063 survives wherever nobody wrote the mark, and §1.12's *check
     rather than assume* is not met.
  2. **A variable extent is unknown at check time and must be refused, not
     admitted.** `contextual.literal_value` fails on `.name`
     (`contextual.hero:170-175`). Admitting it silently is the defect wearing a
     mark.
  And it cannot be delegated to clang: `__attribute__((counted_by(n)))` on a
  function parameter is `error: use of undeclared identifier 'n'` on this box
  (Apple clang 21.0.0). `-fbounds-safety` with `__counted_by(n)` does compile
  here, but adopting it changes every emitted TU and the flag list
  (`selfhost/cli/flags.hero`, CLAUDE.md §7) and is a different sitting.
- **condition**: I move to object if the resolution adopts `counted_by <name>`
  with no expression form and no default refusal. Then it is a mark that
  documents 063 rather than closing it.

## Route D — `len()` on a fixed field

- **verdict**: approve as ergonomics; **veto if it is offered as a repair for
  063 or 065.**
- **section**: design.md §1.12's *"It does not suspend Principle 0"* — a form
  enters because the compiler needs it or because it serves the thesis, and *"it
  would be safer"* is not a ticket. D removes an incentive and checks nothing.
- **implementation_cost**, **~15 lines**, and smaller than the brief thinks:
  - **checker: net zero.** `selfhost/check/builtins.hero:184-193` is at its
    decided 378 and measures exactly 378. The edit moves the token `.fixed`
    from the `false` arm (line 187) to the `true` arm (line 186). **The file
    does not grow.** The brief's *"`builtins` is at its decided ceiling too...
    where would it go?"* has an answer: nowhere. It fits where it already is.
  - **lowering, +~8, and this is the tight file**: `selfhost/ir/flatten.hero` at
    1139 against a decided 1150, **eleven lines of room**. The length must come
    from `.fixed x`'s `x.length` as a literal.
  - **emitter, +~4**: `selfhost/emit/builtins.hero:92-100` (244/300) currently
    answers `hero_str_len` for `.fixed`. `hero_str_len(HeroStr s)`
    (`runtime/parts/str.c:173`) takes a `HeroStr` by value, so a `signed
    char[8]` argument is a clang type error — CLAUDE.md §7 makes that exit 2,
    *the compiler is wrong*. A forgotten half fails **loudly**, which is the
    right failure and still a failure.
  - plus a `spec § 11` row and a `measure/pinned.hero` refresh.
  - **What D does not owe**: no new surface form, so
    `.claude/rules/diagnostics-and-goldens.md` § A new surface form does not
    apply — no formatter round trip, no dump row, no tmLanguage, no
    `highlight.ts`. That is D's real advantage over C and E, and it is why D is
    the cheapest thing on this table by an order.
- **argument**: worth having, and it is not a repair. `s.name.len()` makes the
  honest call honest and makes the dishonest one no harder to write. Adopting D
  alone and closing the sitting would leave a program that corrupts memory at
  exit 0 with a nicer way to spell the number it gets right.
- **condition**: I withdraw the veto the moment D is adopted *beside* a route
  that refuses the overstated extent, rather than instead of one.

---

## Which the compiler can actually carry

**C and D together.** D is ~15 lines and lands in three modules, two of which
have room. C is ~210 lines and lands in a dozen, none of which needs a raised
ceiling — and critically it adds **nothing** to the core: no construct in Part
5's seven, no IR instruction, no descriptor, no ownership rule, no emitted-C
change. A one-person compiler at Pascal-P4 scale carries a declaration-site mark
without noticing. It does not carry B, which is a parameter mode.

**A is carriable and does not answer the question asked.** **B is vetoed.**

## What the briefs got wrong

1. **"both at exit 0"** (shared brief, line 5). The shared brief's own 063
   program does **not** exit 0 here. Its exact shape — `signed char name[8];
   int32_t id;`, `sl_fill(p: s.name.ptr(), n: 64)` — prints `7` and then
   **exits 133**. The silent case is the *small* overshoot: with `int64_t id`
   and `n: 16` it prints `7` then **4702111234474983745** (`0x4141414141414141`)
   at **exit 0**. The read direction at `n: 4096` returns **228618** at exit 0,
   and under `./heroes run --sanitize` it is the stack-buffer-overflow the brief
   quotes, verbatim (`[32, 48) 'h0_t'`, offset 48). The defect is real and its
   loud/silent split is the reverse of what the brief shows: **a big lie traps,
   a small lie corrupts in silence** — and off-by-a-little is the realistic
   binding mistake. A resolution written against the trapping case will aim at
   the wrong half.
2. **The denominator carries duplicates.** 66 and 18 reproduce exactly. Sorted
   unique: **46 distinct declarations, 12 with a length-looking sibling.** Read,
   at most **6** are a genuine (pointer, extent) pair, and two of the six that
   are not state the extent as a product of two siblings.
3. **Route C's emitter cost is zero**, not `emit/extern_probe.hero`. The
   compiler-engineer brief's three places are the wrong three: the places C
   actually lands are the grammar, the checker, and **eight carrier modules**
   the brief does not name (`print/fmt.hero`, `print/dump.hero`,
   `parse/tails.hero`, `resolve/{qualified,state,types}.hero`, `ast.hero`,
   `measure/pinned.hero`), measured from `7e3bb986`.
4. **Route D's ceiling question has an answer**: the checker edit is net zero
   lines and needs no room at all. The file with no room is not
   `check/builtins.hero`, it is `selfhost/ir/flatten.hero` at 1139 against 1150.
5. **`emit/gate.hero` does not bite this sitting.** It is at 365/365 as the
   brief says, and no route A–E needs a row there: the gate refuses `cstr` as a
   container element and not one of these routes introduces a type.

## Prediction

**If route C is adopted, its landing commit's `git show --stat` will show zero
lines changed under `selfhost/emit/` and at least nine modified files under
`selfhost/`** — against the brief's three places. Checkable at the close of the
milestone that lands panel 166's resolution, by `git show --stat <tag>` with
`seed/heroes.c` excluded. If the commit touches `selfhost/emit/` at all, or adds
a `DECIDED` row for `emit/extern_probe.hero`, I am wrong that the counted
parameter is frontend-only and the panel should re-price it as a backend change.
