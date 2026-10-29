# 072 — M-generated-programs: programs nobody wrote, each with the answer it owes, judged by five oracles that cannot lie

## Goal

Row 73 of the chain, opened on 2026-10-28 (`8830e6621`) and built the same day
in the lane `lane-generated-programs`: a generator, written in Heroes, that
composes programs valid by construction and knows what each must print before
the compiler is asked; five oracles over every program; a reducer that takes a
failure down to something a person can read; and a committed sample in the
net. One sitting, panel 231, on the soundness lane and its critic, its
resolution adopted provisionally (https://github.com/heroes-lang/heroes/issues/2243).
**The acceptance**: the milestone page's own words, the author's two hard
shapes, *the C boundary* (structs by value, `@` out-parameters, callbacks,
`cstr`, `owned`) and *depth* (records inside variants inside maps, generics
instantiated across `use` lines, expressions hundreds of levels deep); panel
184's R6 landed in the specification; the batch's gate green on the tree that
became the trunk.

## What surprised

**A program the compiler builds can be read by the model the compiler is.**
The generator computes every answer with the `i64` operators of the compiler
it judges, so a miscompiled `>>` in both is invisible: 36 of 300 failures
under a planted shift with the harness built by the base, 0 with the harness
built by the planted compiler (the compiler-engineer's Q1). The net's step now
builds the harness with the seed's compiler and judges stage 2, and the
contract grid holds every operator against references through other
operators.

**Oracle 1 was written to lie.** *No 134* and *no exit 2* had been overtaken a
month before by panel 184's two designed limits, so the compiler's answer is
read in five, two of them limits recorded rather than defects.

**The generator's yield is a margin, measured by plants.** Nine planted
miscompilations: gate A, the compiler's own tests, sees seven, its constant
evaluator being a second model of the operators; the sample of 128 sees all
nine; and one of the two gate A misses is seen by no hand-written case at all
(measurement 058). Two hunts of 600 seeds against the trunk found nothing,
which is the other half of the same reading.

**The milestone page was wider than the plan.** The C arm passed `i64`s
alone, and the plan had the rest as a later task; the page names structs,
out-parameters, callbacks, strings, handles, maps and generics as the
author's own shapes, so they were built here, as families written whole with
exact models (decision 2248). The floats are dyadic rationals, which a
`double` holds exactly and which print as their exact decimals, so the model
owes nothing to the compiler's floats; the map's keys are sorted by
construction, so it owes nothing to its `sort`.

**A rule the language states, met by its own instrument.** The first struct
draft wrote `&` on two `u8`s and the compiler refused 15 of 17 such programs:
bitwise operators are `i64`'s alone (spec § 7). `var` and `all` and `fold`
were refused as names the same way, each before a program ran.

**An instrument proved live on every run.** The leak oracle reads a header
that drops 200 allocations on every run of the suite and fails as *blind*
when it does not see them, which it did when `leaks` was taken away. Two
malloc fills, 0 and 190, make a byte nobody wrote print `5` and
`-4702111234474983741`.

## What broke and why

- **A byte ceiling held the compiler instead of the program**: under `run`,
  40 of 40 programs read *the runtime did not compile*; every configuration is
  built with `build -o` and only its binary bounded.
- **A checker's whole-tree pass inside a sweep** broke the sweep's own stub
  trees; the emission's compile check moved to a pass of its own.
- **The generator's root was checked by nobody**: `heroes check
  tests/harness/generate.hero` refused a label `main.hero`'s check never saw,
  since the net reaches the modules that make and judge a program and not the
  reducer, the symptom or the hunt; the net's own tests now run that root's
  80 tests, as they run the issue program's and the thesis instrument's.
- **A reduction slid from one planted defect to the other** in 1 of 49, its
  symptom naming the first part of a printed line, `index`, whatever computed
  it; defect 2244, repaired by naming the first wrong part's own computation.
- **macOS's `xargs -I` builds at most 255 bytes**, the second time in two
  milestones: the gate's parallel net did not start until each suite went
  through a one-line runner; and `cache` run beside the compiler's tests read
  red, a shared `build/` rewriting its objects, and was run alone after.

## What landed, and what carried forward

Steps 0 to 20 on 2026-10-28, pushed to `main` at `35da1dd5d` (steps 1 to 16)
and `1d0aeab0a` (17 and 18) with the author's yes, and the close after them:

- the catalogue (step 1, measurement 056) and panel 231, its critic reading
  the briefs first;
- the generator's core, its judge in Heroes and the contract grid (step 2); a
  declared abort held to its place, the reducer holding one symptom, the hunt
  (step 3);
- the corpus's sanitised leg at `-O0` for a program that binds C and
  `emission`'s compile pass (step 4, the first halves of tasks 972 and 1234);
  the C boundary arm with its proved header and oracle 4 at the boundary
  (step 5); the depth dial at the floor (step 6, decision 2246); strings and
  their slices (step 7); fallible functions (step 8); the plant set (step 9,
  measurement 058);
- panel 184's R6 in spec § 9, 10,328 real (step 10); the oracles at the
  boundary as R5 words them, the leak oracle with its live control and two
  malloc fills (step 11, decision 2247; task 972 closed); the C boundary's
  five families and the three of Heroes alone (steps 12 and 13, decision
  2248; task 1234 closed); defect 2244 and the generator's root under the
  net's runner (step 14); the hunt's depth dial past the floor (step 15);
  task 2214's reading, measurement 059 (step 16), and its removal of 17 cases
  on the author's word (step 19); two defects of the generator the close
  found, a prefix sliced through a character and the driver on Windows
  (steps 17 and 18); defect 2249, the compile pass's platform skip (step 20).

**Re-measured at the gate, 2026-10-28**: the full net on stage 2, 33 suites,
10,254 passed and 0 failed; stage 2 and stage 3 the same bytes; the
compiler's own 1,763, the net's own 464; the site's build green. After the
last steps: generated 133 and 0, surface 435, check 693, annotations 1,192,
fixes 1,001, emission 2,118, the net's own 465. The hunt outside the net: 3,200
seeds from 50,001 judged through every oracle, 1,157 of them binding C, 0 defects
of the compiler and 2 of the generator, repaired at steps 17 and 18.
On the Windows box: generated 133 and 0, the eleven parts, the leak control
a skip. The census and panel 187's R2 after the push are not owed: `selfhost/`
moved in the specification's pins alone, and nothing the compiler refuses
changed.

**Carried forward**: decision 2241, the author's two counts of panel 230, on
which the tag waits; task 2245 on M-compatibility-promise, three sentences
of the specification the compiler does not keep; the CI's reading of
`generated` on its four legs and of the leak control on Linux and Darwin
(run 40780595765 and the close's push); R7's Windows measurement on the CI's
own runner; the four cases the reading kept by a citation alone and
`no-size-self`'s twin, for the next debrief.

## The tag, 2026-10-29

**`records/tagged` refused the tag the close placed, and it was right.** Two
`adjacent` defects filed at M-panic-location, 2230 and 2238, reached their
second tag with this one, where the rule counts an aged `adjacent` as
`blocking` (`.claude/rules/verification.md` § Bounded discovery). The tag
was taken back before any push, and the author chose the repair over a
waiver, to close the milestone well.

- **Defect 2230, step 21** (`5350c6b19`): `int_out_of_range` told a program to
  widen a type that was not its own, the library's `exit`'s `u8` or a width a
  header gives an `extern` binding, which a wider binding is refused for at
  build. The note now says whose width it is and offers a number in range
  alone, written after the check rather than carried into it, since every
  frame of the walk holds the checker. `check/walk.hero` stood at its
  ceiling of 1870 lines, and `header_line` moved to `check/callee_text.hero`
  to pay for its lines.
- **Defect 2238, step 22** (`4820bcba7`): a C name a group cannot spell, a
  keyword or a word another language spells, was a line that cannot end,
  *expected a field name*, or the refused word's own lesson, never that the
  name is the header's. 130 probes, ten keywords and sixteen refused words in
  each of the five places a group writes a C name, now read one message
  each, from the lexer, naming the route, and every route ran before a
  message named it. Asked of every keyword the lexer reads, its first
  reading cost `check selfhost/main.hero` 1.6% of its user time, a score of
  owned slots zeroed and released at each call; a filter that reads the line
  in place and holds nothing put it back inside the noise, 7.00 s against
  the trunk's 7.00 s (`b8b991445`).

**What surprised**: the tag's instrument found what the close's gate did not
ask, which is what it was written for, so that the queue cannot only grow.
And the repair of 2238 went where half of it already lived: the lexer reads
where a refused word stands from the text alone, so a keyword in the same
place is told by the same reading, and `parse/`, at 8736 lines of its budget
of 8737, took no line more, `tag_marker`'s message moving to `c_names` to pay
for the parser's silence.

**Re-measured at the tag's gate, 2026-10-29**: the full net on stage 2, 33
suites, 10,217 passed and 0 failed; stage 2, built by the seed's stage 1, and
stage 3 the same 28,596,638 bytes, so the seed stands; the compiler's own
1,765 and the net's own 465; the site's build green.

**The tag's commit**: `b0bfd2690` went out with the author's yes and the two
issues were closed after it, so `records/tagged` read them open at a tag
placed on it, a light tag's time being its commit's (`%(creatordate)`,
`tests/harness/records_github.hero`). The tag goes on the commit that says
so, made after the closes, as M-panic-location's went on the commit after
its defect 2240.
