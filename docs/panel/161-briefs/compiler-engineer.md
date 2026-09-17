# Panel 161 — compiler-engineer

Read `00-shared.md` in this directory first. It carries the measured fact, the
four named routes and the constraints. This file is your input only.

## What you are asked

**Price each route in the live compiler, and say which is core and which is
sugar.** You have a veto on soundness.

## Where to look, with line counts measured 2026-09-17 by `wc -l`

| file | lines | what it holds |
|---|---|---|
| `selfhost/emit/extern_field.hero` | 363 | the field assertion; line 154 is the array-element sign row, line 261 `unsignedness_of` |
| `selfhost/emit/assert_spelling.hero` | 279 | `C_INTEGER_TYPES` (twelve rows), `CSTR_SPELLINGS`, `HERO_RET_*` |
| `selfhost/emit/c_spellings.hero` | 188 | the C-type → Heroes-type table; line 59 is defect 059 |
| `selfhost/widths.hero` | 213 | `IntKind`, `int_signed`, `int_bits`, `int_c_type` |

Also reachable and worth grepping before you conclude: `selfhost/emit/ffi_field.hero`,
`ffi_narrowed.hero`, `extern_record.hero`, `extern_assert.hero`, `selfhost/ffi_errors.hero`,
`selfhost/cli/header_types.hero`.

## The questions, in order of what they decide

1. **Route 1 (a ninth integer type) — what does it actually touch?** `IntKind`
   is a variant; count the `match` sites over it that would go non-exhaustive,
   because Heroes refuses an inexhaustive match on a variant, so the compiler
   itself tells you the blast radius. Report the count and the command that
   produced it. Say whether a ninth kind whose sign is the TARGET's is even
   representable in `widths.hero`, whose `int_signed` returns `bool` with no
   target in hand.

2. **Route 2 (a spelling legal only inside `extern`)** — the contextual words
   already in that position are `tag`, `partial`, `owned`, `link`, `package`,
   `consumes`, `acquires`, `borrows`. Find where they are lexed and parsed and
   say what a ninth costs there. **Then answer the sub-question that decides the
   route**: a field bound this way has to be READ into some Heroes type. Is
   there a representation that keeps the read total and sound — and what does
   `t.name[0]` yield?

3. **Route 3 (accept `i8` or `u8` against plain `char`)** — this is a one-line
   change at `extern_field.hero:154` and the corresponding scalar path. Measure
   it, then say what it costs in soundness: name a concrete program where a
   value round-trips wrong, and compile it if you can.

4. **Route 4 (refuse)** — what the diagnostic would have to say, and whether
   `ffi_field_type` can carry it.

5. **The fifth route nobody has listed.** This is the highest-value thing you
   can return. In particular: is there an answer that changes **what is
   asserted** rather than **what is written** — the `_Generic` rows at line 154
   are the compiler's own choice, not C's — and would such an answer be sound?

## What the answer must not break

`tests/golden/run/ffi-a-char-array-member.hero` exists because `char[N]` was
unbindable at any of the eight widths, and its comment names four things the
repair must not relax, all measured in August:

    u8[4]  against char[4]  -> refused, `char` is signed here
    i16[4] against char[4]  -> refused, wrong width
    i8[8]  against char[4]  -> refused, wrong length
    i8[2]  against char[4]  -> refused, wrong length

**The first of those four is the premise this sitting is about** — *"`char` is
signed here"* was measured on one machine. The other three must stay refused
under any resolution you propose, and you are asked to say so having run them
rather than having read them.

## How to work

`cp -r` the tree to your scratchpad, `rm -rf build`, work there. The seed builds
in a few seconds: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
**Do not rebuild from `selfhost/`** — ~20 minutes, and it has killed seats on
the watchdog. **Never `archive/bootstrap-rs/`**: nothing builds it and a
measurement there is of a compiler that does not ship.

If you want the arm64 machine, it is `heroes-linux-arm64` in Docker and the run
line is in `docs/ref/environment/linux/LINUX-MACHINE.md`. A sign fact needs it;
a line count does not.

## Deliver

Verdict · the design.md section it rests on · cost measured in files and lines
touched, with the command · a falsifiable prediction and the milestone at which
it is checkable · any veto, stated as a refusal rather than a price.
