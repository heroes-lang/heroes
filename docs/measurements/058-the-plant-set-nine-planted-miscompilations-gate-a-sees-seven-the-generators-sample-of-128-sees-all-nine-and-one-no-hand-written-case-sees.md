# 058: the plant set: nine planted miscompilations, gate A sees seven, the generator's sample of 128 sees all nine, and the two gate A misses are seen, one by a single hand-written case and one by none

Measured 2026-10-28 at M-generated-programs, by panel 231's R3 (decision 2243): *the milestone's
yield is measured as a plant set: one planted miscompilation per emitter rule and operand class,
each built into a compiler of its own, each scored against the `run` form and against the generator
built by stage 1*, with the critic's last pass adding gate A, the compiler's own tests, which saw
both of the sitting's plants (`docs/panel/231-programs-nobody-wrote-the-generator-its-oracles-its-reducer-and-what-it-leaves-in-the-net.md`,
§ Added after the critic's last pass). On the trunk at `5b3285a1f`, its stage 2 `sources
5e0ca4c634086a7c`; every plant built from that tree by stage 2 in a throwaway worktree
(`.claude/worktrees/plant-b2`) and reverted at once, `git status` clean but for untracked binaries.

## 1. The plants

Each is one line added to `selfhost/emit/operator.hero`, the rule that writes one operator's C, for
one class of operands, the shape a wrong optimisation takes; each was probed alive on a program of
its own, the operands passed through a parameter so no fold sees them, and each moved its own line
and no other:

| plant | the miscompilation | probe, trunk against plant |
|---|---|---|
| E1 | `+` of two negatives whose sum is below -2^40 gives the sum plus 1 | `-2199023255552 + -5`: -2199023255557, -2199023255556 |
| E2 | `-` with a left operand past 2^32 and a negative right flips bit 0 | `8589934592 - -1`: 8589934593, 8589934592 |
| E3 | `*` of two negatives gives the product minus 1 | `-3 * -4`: 12, 11 |
| E4 | `/` rounds toward minus infinity, not toward zero | `-7 / 2`: -3, -4 |
| E5 | `%` takes the divisor's sign for a negative dividend | `-7 % 3`: -1, 2 |
| E6 | `>>` is logical for a negative operand shifted 32 or more (the critic's B2) | `-8 >> 40`: -1, 16777215 |
| E7 | `&` with a negative right operand sets bit 1 | `6 & -3`: 4, 6 |
| E8 | `^` of two operands past 2^62 flips bit 0 | 6, 7 |
| E9 | `<<` by 48 or more shifts by the count less 48 | `1 << 50`: 1125899906842624, 4 |

## 2. Who sees which

- **Gate A**, `heroes-E<k> test selfhost/main.hero`, the compiler's own 1,763 tests.
- **The generator**, the `generated` suite's sample: seeds 1 to 128 at 30 per cent declared, made by
  the generator built by stage 1 (`./heroes`, `sources 638149f67ad4d782`) and judged by the planted
  compiler through every oracle; the count is the programs wrong in a configuration
  (`WRONG-O0` and the others), read before oracle 4, which a defect of the judge's own spoiled on the
  first reading (it named the runtime under the working directory alone; repaired the same hour,
  `runtime_of`, and E1's sample judged again: the same 9, every other program passing).
- **The `run` form**, for the two plants gate A misses: the 599 cases of `tests/golden/run/`, each
  built at `-O0` by the trunk's compiler and by the plant and run, a case counted where its stdout or
  its exit moved.

| plant | gate A, tests failed | generator, of 128 | `run` form, of 599 |
|---|---|---|---|
| E1 | **0** | **9** | **0** of 597 built |
| E2 | 6 | 10 | not run, gate A sees it |
| E3 | 2 | 16 | not run |
| E4 | 2 | 28 | not run |
| E5 | 2 | 24 | not run |
| E6 | 1 | 15 | 1, by the critic (`fixedbugs-577-...`) |
| E7 | **0** | **6** | 1 of 597 built (`bitwise`) |
| E8 | 1 | 2 | not run |
| E9 | 1 | 15 | not run |

## 3. What it says

**One plant is seen by no instrument written by hand.** E1, `+` of two negatives whose sum is
below -2^40, passes the compiler's 1,763 tests and moves none of the 597 `run` cases this Mac builds
(the 2 others, `fixedbugs-437-...` and `fixedbugs-589-...`, need `sys/prctl.h` and built for no
compiler here); the generator's sample shows it in 9 of 128 programs. E7, `&` with a negative right
operand, moves one hand-written case, `run/bitwise`, and 6 of the sample's 128. That is the
milestone's yield measured the way the sitting asked: the generator's margin over the net, on a plant
the net misses, and the first such plant measured.

**Gate A sees seven, and often by accident**: E3, E5 and E8 fail tests of a cache and of hashing
(`defect 357`, `defect 443`), whose code uses the operator, not a test of the operator; E6 and E9 one
test of the checker's constant evaluator (`check/const_values.hero:369`). A plant whose operand class
the compiler's own code never meets, two negatives summing below -2^40, `&` by a negative, is seen by
no test of the compiler.

**The generator's sample sees all nine, in 2 to 28 of 128 programs**, each in every one of the seeds
whose program reaches that operator with that class of operand; its weakest is E8 (2 of 128), two
operands past 2^62 being rare under the constants mix, the gap an operand domain would close
(panel 231's R3).

## 4. What it does not settle

Nine plants are one class of defect, an emitter's operator rule; the C boundary, strings and the
fallible forms were not planted. The `run` form was run for the two plants gate A misses and at
`-O0` alone. The generator's sample was judged by the generator built by stage 1, which the CI's net
step now runs; a sample judged by a harness built by the planted compiler would be blind to every
plant here (panel 231's Q1).
