# Panel 149 — report of the compiler-engineer

Everything below was run in a copy; compiler built from the seed in 3.14 s. The
seed carries panel 148 R5. **No selfhost rebuild was attempted, so every price
below is a written, line-counted, UNCOMPILED patch** — said at each one rather
than implied otherwise.

## The finding that reframes the sitting: THE WALK ALREADY EXISTS

**`selfhost/check/map_keys.hero:103-160` is `reaches_handle` + `inside_handle`,
58 lines**: a reachability walk over group-record fields, fixed-array elements,
variant payloads, arrays, maps and fallibles, **returning a dotted path**. It
ships, it has three tests, and `tests/golden/check/ffi-handle-refusals.expected:3`
prints its answer: `` `Conn` cannot be a map key, because `Conn.handle` is a
handle ``.

**So R1 is not a new walk and R2 is not a new message.** R1 is a second caller,
and the sitting's real question is what the walk RESTS on.

## A defect found while measuring the walk's termination

`map_keys.hero:104` reads `if depth > 16`. Bisected this session with generated
chains of group records ending in a handle, used as a map key:

| chain of group records | `heroes check` |
|---|---|
| 2 … 16 | exit 1, ``error[handle_map_key]: … because `R0.s` is a handle`` |
| **17, 18, 19, 20** | **exit 0, no diagnostic** |

**A handle as a map key at seventeen levels is accepted in silence.**
`selfhost/handles.hero:139-160` calls that refusal *"a hole nothing can close"*;
the depth bound reopens it. It is a premise about the world in exactly the sense
`.claude/rules/module-shape.md` refuses, and the comment justifying it is true
for the FLOAT twin and false for the handle twin, because the runtime guard
behind the handle rule is an address, not a check. **Defect-grade, independent of
the sitting, and load-bearing on it**: reusing this walk as it stands hands
`unmarked_handle_producer` the same hole.

## R1 — adopt with condition

**Section**: design.md §1.7 (core plus elaboration). R1 adds no construct — no
lexer token, no AST node, no IR node, no emitter change. §1.7's own test, *must
be implemented in the type checker AND the lowering AND the backend*, is failed
in all three places, so this is the cheapest shape a rule can have.

| file | now | after | delta |
|---|---|---|---|
| `selfhost/check/reaches.hero` (new) | 0 | **99** | +99 |
| `selfhost/check/map_keys.hero` | 269 | ~216 (the walk moves out) | −53 |
| `selfhost/check/acquiring.hero` | 145 | ~165 | +20 |

**Net +66 lines, three files, one new 99-line module**, under the ~300-line
threshold. Nothing in `selfhost/emit/`, the lexer or the descriptors moves.

**The home, and why the other two are wrong.** A new `check/reaches.hero`:

- **`handles.hero` is wrong on its dependencies, measured**: every function in it
  takes `ast.Decl` or loose pieces and it imports no table, while a reachability
  walk needs `state.Checker` and `check/table`. Twelve modules `use handles` and
  three are the parser, the mutator and five emitters. **Dragging the checker's
  interned table into `handles.hero` drags it into the parser.**
- **`check/ffi.hero` is wrong on size and role**: 384 lines already, and its
  question is *may this cross*, asked of one type, not *what does it reach*.
- **`check/acquiring.hero` is wrong** because the walk now has two callers, and
  CLAUDE.md's opening rule is that each rule is written in exactly one place.
  The FFI half has paid for the alternative once: `check/table.hero:415-418`
  records `c_type_of` and `c_type` disagreeing about a group's record for a whole
  milestone, in silence.

**needed_for_self_hosting: no.** `grep -rn "record [A-Za-z0-9_]* tag " selfhost`
returns nothing — this compiler declares no handle. R1 enters under Principle 0's
second limb, design.md §1.12 robustness.

## R2 — adopt the FULL path, and it is free

**Zero new lines for the path, +4 to make it the full path.**
`map_keys.hero:142-160`'s `inside_handle` already builds `owner + "." + field`.
The only decision is the shape, and `map_keys.hero:161-169` has already reasoned
about it for the float twin: deepest owner wins there because *"a map key is
repaired by changing ONE declaration"*. **That reason inverts here.** The repair
for `unmarked_handle_producer` is on the PRODUCER's declaration; the path is not
the repair site, it is the evidence that the producer reaches a handle at all.

**No golden moves**: `tests/golden/check/ffi-handle-refusals.expected:3` is
`Conn.handle`, depth one, where full path and deepest owner coincide.

## R3 — refuse with veto on two of the three candidates

**Veto on *allow the mark to repeat, once per reachable handle type* and on
*name the field in the mark*.** Per-FIELD marks add a construct the parser, the
AST, the checker and the emitter must all handle, which is design.md §1.7's
definition of core, verbatim; everything else in this sitting is erased in the
frontend.

**The measurement that kills *refuse more than one handle TYPE*.** One `extern`,
two `@` parameters, both `Slot`, both `acquires slot_close`; the program releases
each with its own call. **One handle type.** Every mark written and correct.
`heroes check` exit 0, `heroes build` clean, **run exit 134**. The emitted C is
the proof: the call, then **one** `hero_handle_acquired();`, then two
`hero_handle_consumed();`. Root cause: **`selfhost/emit/ops.hero:162` tests a
BOOLEAN**, `handles.hands_a_handle_over`. The mirror program aborts 134 the other
way. **So a rule keyed on the number of handle TYPES does not catch the shape
that breaks, and the breakage needs no record at all. The axis is the number of
HANDLES.**

**The measurement that kills the other two.** Two byte-identical Heroes programs
(`record Four` holding `a: Slot[4]`) against two headers; a C probe measured one
filling 4 and the other filling 2. **The number of handles a `Slot[4]` carries is
chosen by C at runtime**, so no mark the author can write and no constant the
compiler can read is right. A third, where C filled two and the program released
exactly those two with one call each, **aborts 134**. Under a mark-repeats or
name-the-field scheme with N taken from the type it would abort too, saying two
never given back. **The fixed-array route is a refusal, not a count.**

**The diamond, which is why a visited set is not enough on its own.**
`Outer{p: Pair, q: Pair}`, `Pair{s: Slot}`, marked once, both handles released:
checks clean, **aborts 134**. A set-valued walk answers `{Slot}` — one type, one
declaration, one mark — and the program needs two.

**Cost of the adopted resolution** (written, uncompiled): refusing a mark whose
type reaches N ≠ 1 handles is **+25 lines** on top of R1's module, where N is a
memoised sum over `c.out.type_order`, which `checker.hero:47` fills and
`check/sized.hero:44-48` documents as **empty exactly when there is a cycle**.

**The emitter defect, which must land regardless of this sitting**:
`selfhost/handles.hero:67-93` 27 → 36 lines, `selfhost/emit/ops.hero:158-166`
9 → 13. **+13 lines, two files, exactly two call sites in the whole tree**,
measured by grep. **Golden-neutral**: no declaration anywhere in `examples/`,
`tests/` or `selfhost/` carries two `acquires` or two `consumes`.

**Price of the vetoed alternative.** Parser 0, AST 0, checker 0 — per-POSITION
repetition already ships (`selfhost/ast.hero:373` and `:463`). The cost is
entirely per-FIELD: a field inside a group record has no declaration position to
hang a mark on, so it needs new surface syntax, a new AST field, a checker rule
and an emitter loop — **and it still gets `Slot[4]` wrong.** A core construct
bought for a case it does not fix.

## The termination argument, stated so a later reader can check it

**The interned type table is acyclic by construction** and the walk over
`.fixed`, `.array`, `.fallible`, `.map` terminates without any guard: a structure
is interned after its components, so a component's id is smaller than its
container's. `check/ffi.hero:71-75` already writes this down for
`crosses_the_boundary`.

**The declaration graph is NOT acyclic, and it is user-writable.** Three programs
measured, all parsing, all passing `ffi_field`, all refused only by `sized`: a
self-referential group record, a mutual pair, and a fixed-array cycle. **And
`acquiring` runs anyway**: a program holding the cycle AND a direct unmarked
producer prints **both** diagnostics. `checker.hero:46-50` pushes `sized`'s
diagnostics and does not return; `:74` then calls `bindings_say_which`.

**Is a visited set sufficient? Yes for termination, and there is no second cycle
shape.** Every recursive edge lands either inside the type table or on
`decls[n.decl]` through `.named`, and the group vocabulary admits only two
recursive field forms, both gated. A set keyed on the declaration index enters
each declaration once.

**Is it sufficient for the COUNT? No**, and that is the sharp half. The diamond
re-enters `Pair` legitimately: the visited set suppresses the second visit and
the count comes back one when the truth is two. **Termination and counting want
opposite things from the same set.**

**The cheap alternative, priced and rejected.** `acquiring` could skip itself
whenever `c.out.type_order` is empty: three lines, no set. It loses because it
makes this rule's termination depend on another pass's output — change `sized` to
report a cycle without emptying the order and non-termination comes back in
silence. CLAUDE.md § Precedence asks for the most robust resolution, never the
cheapest.

## What breaks

**Baseline, this session**: `./heroes test selfhost/main.hero` → **646 tests, all
passed, exit 0**, `real 62.48 / user 58.34 / sys 2.10`. **The "after" is unrun.**

**`examples/`: nothing breaks. Read, not guessed.** Every extern group in the
four handle-bearing programs was enumerated. **No group record in any shipped
example holds a handle field.** The three handle-in-a-record sites that exist are
all ORDINARY Heroes records declared outside a group, and an extern cannot return
one: `check/ffi.hero:80` answers false for a record with no header. **Zero
examples newly need a mark.**

**Exactly one shipped program newly errors**:
`tests/golden/run/abort-handle-given-back-unmarked.hero`. **`runtime/parts/alloc.c:181-184`
predicted this in writing**: *"If a later rule catches one of them at compile
time, its case goes red and this sentence is what must change."* So R1's price
includes rewriting the negative-count message and moving that case to
`tests/golden/check/` under a `fixedbugs-` prefix.

**`tests/golden/run/fixedbugs-a-handle-in-a-record-field.hero` survives R1 and
does NOT survive a vocabulary-level refusal.** Its group declares no `consumes`,
so `consumed_types` is empty and `bindings_say_which` returns at
`acquiring.hero:107-108`. **This is the concrete reason not to refuse the
handle-in-a-field SHAPE outright**: it would delete a shipped capability to fix a
counting problem. Refuse the MARK where N ≠ 1, not the shape.

**Suites that must gate this**: `.claude/rules/verification.md`'s 2026-09-14
addition, written at the previous milestone's close for this exact rule — a
change to what the checker REFUSES is judged by `check` `run` `emission`
`determinism` `corpus`, not by the `selfhost/**` row.

## Incidental, outside the sitting's question

A `-Wconditional-uninitialized` warning in the seat's own header produced
`internal error: compiling the generated C failed` at **exit 2** — the compiler
blaming itself for C the binding author wrote. `.claude/rules/c-boundary.md`
names four classes that should be exit 1 and a warning in the author's own header
is not among them. Reported, not pursued.

## Prediction

**If R1 lands by calling `map_keys.reaches_handle` as it stands — depth bound
kept, no visited set — then a chain of seventeen group records ending in a
handle, returned by an unmarked `extern` in a group that also declares a
`consumes`, will `heroes check` at exit 0 with no `unmarked_handle_producer`**,
exactly as the map-key rule does today at seventeen and refuses at sixteen.
Settled by generating the two chains. **The map-key half of this experiment was
already run and behaved as predicted**, which is why it is a condition rather
than an objection.

## Conditions

- **R1 to plain adopt**: the walk lands in ONE module both rules call, the
  `depth > 16` bound dies in that commit, and `check/acquiring.hero` gains `test`
  blocks firing at depth one, at depth two, on a diamond and on each of the three
  cycles. It carries **zero `test` blocks** today against `map_keys`'s three and
  `ffi.hero`'s four; **widening an untested 145-line rule is how the sixteen-deep
  hole survived.**
- **R1 to object**: a measurement showing the moved walk costs more than 100
  lines net, or that `map_keys`'s three tests cannot be kept green across the
  move.
- **R3 veto lifted**: a named C function in a real header returning a struct BY
  VALUE carrying two or more pointers the caller must release. None found in this
  repository's corpus; `getaddrinfo` is not one. **But the mark still cannot
  carry the count for `Slot[4]`**, which the two-header experiment settles
  independently of any header.
- **The emitter fix (+13 lines) is not conditional on anything.** Those are
  correct programs that abort at 134 today.
