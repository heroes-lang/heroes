# One half had to be able to refuse, and the other had to never

2026-09-16. Panel 157, soundness lane, at M-check-completeness. Defect 048 was
filed as an artifact bug — `heroes build --emit-c` writes C that clang refuses
for any `record <Name> tag <name>` binding — and the sitting widened it before
repairing it.

## What was actually wrong was one line's position

`cli/compile.hero` wrote the C and returned before `cli/produce.hero` ran at all,
so the artifact was the **first** emission: before the pointee check, which is a
refusal, and before the tag round, which learns a spelling from clang's own
words. Both halves of the defect are that early return.

## The half nobody had asked about

`--emit-c` skipped the pointee check too. A binding declaring `@n: i32` against
`static inline void fill(uint64_t *n)` emitted at exit 0, **compiled clean under
the real flag list — 0 errors, 0 warnings** — and wrote `0xAAAAAAAA` over the
`guard: i64 @ 123456` in the adjacent stack slot. That is design.md §1.12
verbatim, and the mechanism is bitter: the emitted `fill((void *)&h0_n)` carries
a cast that exists so a **correct** binding compiles clean under
`-Werror=incompatible-pointer-types`, and the same cast launders a **wrong** one
past the only flag that would have seen it.

Five of the 25 `fixedbugs/` cases the build refuses were silent that way, already
committed.

## So the bar moved, and the sitting says which sentence moved it

*Emit C that clang accepts* is **provably insufficient** — those five satisfy it.
`--emit-c` owes **the artifact of a build that passed**. What settles that is not
prose: the flag is how `seed/heroes.c` is made and CI re-emits and compares it on
every push, so it is load-bearing for the bootstrap. The compiler seat also
corrected the brief: design.md:717's *"stop at the C and read it"* is ambiguous
between an output and a dump, and the unambiguous sentence is
`.claude/rules/cli-surface.md`'s *"an output, not a dump"*.

## The decision: two shapes, and they are opposites

**The pointee check is a REFUSAL** on the `--emit-c` path. Its *no* is what stops
the corruption, so it must be able to say it.

**The tag round becomes an ADVISORY probe**: ask clang `-fsyntax-only`, harvest
the tags it requests, **discard the verdict**, re-emit. It must never be able to
say no, because fourteen `fixedbugs/` cases exist precisely to have C where a
build has none, and a refusal would delete them. A probe that can only ADD a
spelling cannot turn an emission into a failure, and on a machine without the
header it learns nothing and the output is yesterday's.

Treating the two alike would have broken whichever one it was not written for.
That is the whole of what the sitting bought.

## Why clang and not a rule, measured on four headers

`struct X *` fails on a typedef of an ANONYMOUS struct; the bare word fails on a
tag-only header; a typedef shim at the top of the artifact fails on the anonymous
shape too, with *typedef redefinition with different types* — it is always-qualify
wearing a hat. **No fixed spelling is correct.** The only clang-free route left
is a C declaration parser inside Heroes, which the compiler seat's veto is aimed
at, held and not cast.

## What the suite learned, and it maintains itself

`suite_emission.hero`'s stated premise — *"`--emit-c` never calls clang"* — is now
false and carries its correction under its date; its substance survives, because
the tag half is advisory. A `fixedbugs/` case refused at exit 1 is a **pass**, on
the condition that **nothing is blessed for it**. So retiring the four blessed
files is what records the decision, and the decision cannot drift from the
instrument. A list of names would have been a premise about the world and would
have expired in silence.

## What is owed and was not built

Panel 157's R4: build every case both ways, compare exit code and stdout, and
**link** rather than merely compile. Of 240 blessed emissions, 28 fail
`-fsyntax-only` and 3 fail with *must use 'struct' tag* while green in the suite,
because a byte comparison cannot see that the bytes do not compile. It is the
only instrument that would have found this class, and it is filed in
`docs/work/milestones/M-package-manager.md` with its price and with the question
its builder must measure first: 240 programs built twice with a link each is a
different order of cost from reading a blessed file.
