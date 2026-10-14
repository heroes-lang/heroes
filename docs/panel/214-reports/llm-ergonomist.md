# Panel 214: the llm-ergonomist's report

**Inputs read.** I read nine files and nothing else: `brief.md`, `heroes-spec.md` and `sample-1.md` to `sample-7.md`, all in
`.claude/worktrees/scratch-panel-214/llm-ergonomist/`. The proposal did not come to me as a spec diff. It came as six
pairs of messages plus one message on its own. Below, a *structured* message is one headed `error[ffi_header_refused]`
and a *raw* message is a clang transcript headed `internal error`. I do not assume which of the two is the proposal.

**What the numbers are.** The confidence percentages and the predictions are my own judgement. None of them was run.

## verdict

**approve.** Five amendments follow, and none of them blocks:

- (a) a `fix` line that names the prefixes a rename must avoid;
- (b) a `fix` line for sample 2;
- (c) when clang suggests a name that belongs to the runtime, the message says the header cannot rely on it;
- (d) sample 3's second note, rewritten in one clause;
- (e) for the compiler's own errors (sample 7), the message names the Heroes construct.

## experiment

**Method.** For each sample I read one message alone and wrote the first edit I would make. Then I read the other
message alone and did the same. Then I compared the two.

### Sample 1: `notes.h` redefines `hero_str_len` (X raw, Y structured)

- **What is wrong.** Line 2 of `notes.h` defines a `static` `hero_str_len`. The Heroes runtime declares a non-static
  `hero_str_len(HeroStr)` earlier in the same unit. Raw X's second error, at line 3 (`int` passed to `HeroStr`), is a
  knock-on effect of the first.
- **Whose mistake.** The repair has to be made in the header, but the header is legal C. C does not reserve the name
  `hero_str_len`. The name is taken only because the runtime declares it in the header's unit, and section 13 of the
  spec never says that. So the edit belongs in the header, and the cause lies in how the compiler builds the unit and in
  what the spec leaves unsaid.
- **How sure.** 70% from raw X that the edit is mine to make, because its first words say "internal error" while its body
  points at the header. 90% from structured Y.
- **My edit (the same under both messages):**
  ```c
  #include <stdint.h>
  static inline int notes_len(int x) { return x; }
  static inline int eight(void) { return notes_len(8); }
  ```
- **Which message lets me act alone: Y.** X carries the same facts, the runtime's signature included. But its first
  words contradict its body, and it adds the knock-on error.

### Sample 2: `counts.h` redefines `int64_t` (X structured, Y raw)

- **What is wrong.** Line 1 of `counts.h`, `typedef long int64_t;`, clashes with the unit's own `int64_t`, which is
  `long long` on this machine.
- **Whose mistake.** The header's.
- **How sure.** 90% from X. 75% from Y, whose first words point at the compiler.
- **My edit:**
  ```c
  #include <stdint.h>
  static inline int64_t seven(void) { return 7; }
  ```
- **Which message lets me act alone: X.** Both carry enough. X also says whose `int64_t` came first.
- **Where I hesitated.** Neither message has a `fix` line, and both quote `('long' vs 'long long')`. That invites
  `typedef long long int64_t;`. C11 allows the same typedef twice, so that edit compiles here. It would fail where
  clang's `int64_t` is `long`, which I believe is LP64 Linux (from my knowledge of clang's predefined macros; unrun), and
  the original header would compile there. The failure is loud, but it comes on the next platform, after a first try
  that passed. A `fix (guess)` saying "delete the typedef and include `<stdint.h>`" would close that route.

### Sample 3: `gauge.h` declares `int fabs;` (X raw, Y structured)

- **What is wrong.** `int fabs;` at file scope has external linkage. C11 7.1.3 always reserves that name, and clang
  predeclares it as a builtin.
- **Whose mistake.** The header's.
- **How sure.** 50% that I diagnose it correctly from raw X. 90% from structured Y.
- **The first edit raw X invites.** X's notes say "included multiple times" and "consider using #ifdef guards or #pragma
  once", so the likely first edit is
  `#pragma once` above `int fabs;`. It fails again, loudly, and one round is lost.
- **My edit from Y.** Delete line 1, since nothing in the group uses it:
  `static inline int eight(void) { return 8; }`.
- **Which message lets me act alone: Y.** Its second note is the one sentence in all seven samples that I had to read
  three times. It explains clang notes that Y never shows. "An include guard or `#pragma once` would not change this"
  says the same thing in one clause.
- **A question, not a finding.** X names its unit `efabs.c` while the program is `gauge.hero`. Samples 5 and 6 do the same
  (`estd.c`, `jtypo.c`). Either the samples were made that way, or a raw message names a unit the reader cannot map back
  to the program.

### Sample 4: `tally.h` calls an undeclared function (X structured, Y raw)

- **What is wrong.** Line 1 calls `hero_str_le`, which nothing declares.
- **Whose mistake.** The header's.
- **How sure.** 90% from X. 60% from Y.
- **The first edit raw Y invites.** An undefined symbol in a program with an `extern` group reads as a missing library,
  and §13 says to "`link` a library when the symbols need one". So the likely first edit is
  `extern "tally.h" link "tally"`, which fails loudly. The only pointer to the header is the object file's name,
  `tally-...o`.
- **The first edit structured X invites.** Taking clang's suggestion gives `return (int)hero_str_len(8);`, which fails
  loudly (`int` passed to `HeroStr`). X calls `hero_str_len` "a name of the Heroes runtime's own" and stops there. It
  never says whether a header may call it.
- **My edit:** `static inline int eight(void) { return 8; }`. If `hero_str_le` was meant as the header's own helper, the
  header has to define it.
- **Which message lets me act alone: X.**

### Sample 5: `ruler.h` defines `sqrt` (X raw, Y structured)

- **What is wrong.** Line 1 of `ruler.h` defines a static `sqrt(int)`. The unit includes `<math.h>` after the groups'
  headers, and `<math.h>`'s `double sqrt(double)` conflicts with it.
- **Whose mistake.** The repair has to be in the header. But the clause Y itself cites, C11 7.1.3, reserves a file-scope
  `sqrt` only where `<math.h>` is included. The header does not include it; the unit the compiler builds does, after the
  header, and the spec does not say so. The header is right C inside a unit it did not choose.
- **How sure.** 90% that the header is the only place to repair. 60% on the attribution above.
- **The first reading raw X invites.** X places the error in the SDK's `math.h:467`, and the header appears only as the
  "previous definition". The likely reading is a toolchain fault, or a bug report prompted by "internal error". I would
  edit the header in 65% of tries.
- **My edit.** Delete line 1, which is unused. Renaming it to `ruler_sqrt` also works.
- **Which message lets me act alone: Y.**
- **The next case over, unrun.** Raw X shows the group's header at line 20 of the unit and `heroes_standard.h` at line 45,
  so part of the runtime is read *after* a header. A header's `#define` would reach that part. Suppose a header pairs
  `#define fabs my_fabs` with a `static` `my_fabs` of the same signature. `<math.h>`'s `extern` declaration after a
  `static` one takes internal linkage (C11 6.2.2), so I expect it to compile and to change the runtime's float behaviour
  without any error. If that holds, it is the case in this class that fails silently. I did not run it.

### Sample 6: `gadget.h` uses an unknown type (X structured, Y raw)

- **What is wrong.** Line 1 uses `HeroSt`, a type nothing declares.
- **Whose mistake.** The header's.
- **How sure.** 90% under both messages.
- **The first edit both messages invite.** Raw Y even prints the replacement under the caret:
  `static inline int eight(void) { HeroStr s; (void)s; return 8; }`. This **compiles and prints 8**. The header now
  compiles only inside a Heroes unit, using a runtime type the spec never names, so its meaning lies outside both the
  header and its group. Nothing fails today. It breaks the day the runtime renames the type.
- **My edit.** Delete the unused local: `static inline int eight(void) { return 8; }`.
- **Which message lets me act alone: X, narrowly.** It says whose `HeroStr` is. Neither message says not to use it.

### Sample 7: the compiler's own error (one message only)

- **What is wrong.** There are two declarations of `h_partsone_failure_9c07dcd`, with parameter types
  `h_0opt_731f54b6` and `h_0opt_ca5589c`, and both are placed at `parts/one.hero:7`. My reading: `one.failure<T>` is
  instantiated twice in one unit, once at `T = one.Held` (from `one.made()`) and once at `T = Held`, the program's own
  record (from `mine()`). The two `T?` types got two C names, but the two instantiations got one.
- **Whose mistake.** The compiler's. The program is valid:
  - §1 writes a module's declarations qualified (`one.Held`), so two records called `Held` in two files do not clash in
    Heroes.
  - §9 infers `T` from the argument.

  Its opening words, "internal error", are true here, and in no other sample.
- **How sure.** 85%.
- **My edit.** A workaround, and a guess:
  ```
  record Shelved
      b: str

  function mine() -> Shelved?
      return fail(code: "x", msg: "mine")
  ```
- **Where I hesitated.** The message never says `Held`. To see "two records with one name" I had to decode mangled
  names. Two plausible wrong first edits, both loud:
  - adding a type annotation, such as `m: one.Held? = one.made()`;
  - splitting the `print` into two lines.

  Both leave two instantiations in one unit.
- **The lines it shows are not Heroes.** The source shown under `shelf.hero:13:37` and `parts/one.hero:7:23` is C, not
  those Heroes lines. Line 7 of `parts/one.hero` is `.err e => e.msg`; `failure`'s signature is on line 4.
- **Can I act without opening another file?** No. The message ends by pointing at `build/.../shelf.c`.
- **What it should say.** "compiler defect: `one.failure` instantiated at `one.Held` and at `Held`, both from
  shelf.hero:13, received one C name; workaround: give one record another name."

### Across all samples: what a message must carry so that I know whose mistake it is

1. **Opening words that are true.** Among the raw messages, "internal error" opens samples 1 to 6, which are header
   faults, and also sample 7, which is the compiler's own. So it tells the reader nothing. The structured messages give a
   header fault its own named class and keep `internal error` for the compiler. This has the largest effect of anything
   here, because it decides whether the reader edits or reports.
2. **The failing line, quoted from the file that owns it**, plus the program location that brought that file in.
3. **The other side of a clash: who owns it and when it enters the unit.** The runtime comes before the header (1, 2),
   clang's builtins appear at first use (3), and `<math.h>` comes after (5). This is what separates "your header is wrong
   C" (2, 3, 4, 6) from "your header is right C in a unit you did not build" (1, 5). Whose mistake it is gets decided
   there.
4. **The edit, tagged `guess` or `certain`, and for a rename, the names that are taken.** In these samples the runtime
   and the generated C use `hero_`, `Hero`, `h_` and `HERO_`. That list comes from seven samples, and I cannot know
   whether it is complete.
5. **For a suggestion passed on from clang: whose name it is, and whether a header may use it** (4, 6).
6. **For the compiler's own error: the construct in Heroes names, every site, and a workaround.** C names come after, if
   at all.
7. **One error, not the errors it causes** (raw sample 1).

## hesitation_points

Each place where the spec left me a choice, the choice I made, and what the other choice produces.

1. **Whether a header may use the runtime's names (`HeroStr`, `hero_str_len`).** §13 is silent and never says the
   runtime shares the header's unit. I chose no: the header declares its own. The other choice compiles silently in
   sample 6 (`HeroStr`) and fails loudly in sample 4 (`hero_str_len(8)`).
2. **Which names a header must avoid.** §13 says only that headers are read in the order of their groups, with
   `_GNU_SOURCE` first. It says nothing of the runtime or of `<math.h>`. I prefixed names with the header's name. A
   `hero_` name fails loudly if it is taken and is a hidden risk if it is not.
3. **How a header gets `int64_t` (sample 2).** I chose `#include <stdint.h>`. The other choice,
   `typedef long long int64_t;`, passes here and fails on LP64 Linux (unrun; from clang's predefined macros).
4. **Whether to rename or delete an unused declaration that clashes (samples 3, 5).** I deleted it. A rename works if the
   new name is free, and fails loudly if it is another library name such as `abs`.
5. **Whether to follow clang's did-you-mean (samples 4, 6).** I did not. Following it fails loudly in sample 4 and
   compiles silently in sample 6.
6. **Two `Held` records in two modules (sample 7).** §1 makes this legal, so I kept the program and called the rename a
   workaround. A model that believes it caused the error rewrites a valid program.
7. **The result's width.** §13 says "a result may be wider than C's", so `eight()` could be `-> i32` or `-> i64` for
   `int`. I chose `i32`, the header's width. Both are legal and neither changes anything here.
8. **Wording in samples 1 to 6.** "repair the header, or name the one that declares this group's C" took me a second
   reading. I took "the one" to mean a header, so: point `extern` at a header that does declare these functions.
9. **Wording in structured 1 and 2.** They end clang's note with "previous declaration is here" (sample 2: "previous
   definition is here"), and "here" points at nothing the message shows.

## argument

Every structured message beats its raw counterpart on what decides a first repair: whose file to edit. The raw ones say
"internal error" for a header's fault, place the error in the SDK's math.h (5), advise an include guard that cannot help
(3), or surface as a link failure inviting a `link` clause (4). The structured ones quote the header line, name the other
side and why it shares the unit, and leave "internal error" to the compiler's own (7). Residual: clang's did-you-mean is
relayed toward runtime names (4, 6), and in 6 following it compiles; no rename says which prefixes are taken; sample 3's
second note is unreadable; sample 7 speaks only C.

## prediction

These are unrun. They can be checked when the harness runs these seven samples as repair tasks: show one message, take
the model's first edit, rebuild.

- **P1, samples 1 to 6.** The first edit touches only the header, and the rebuild compiles. Structured messages: at least
  85% of tries. Raw messages: at most 60%. That is a gain of at least 25 points.
- **P2, sample 3, raw.** At least 30% of first edits add an include guard or `#pragma once`. Structured: at most 5%.
- **P3, sample 4, raw.** At least 20% of first edits add or change `link`. Structured: at most 5%.
- **P4, sample 6.** Under both messages, at least 40% of first edits adopt `HeroStr` and compile, and the two rates are
  within 10 points of each other. With a note saying the runtime's names are not the header's to use: at most 10%.
- **P5, sample 4, structured.** At least 15% of first edits write `hero_str_len(8)`, which fails loudly.
- **P6, sample 7.** At most 50% produce a compiling program with the first edit. At least 25% make an edit that keeps both
  instantiations (an annotation, or a split `print`).
- **Silent errors.** None in samples 1 to 5 and 7, under either message. Sample 6 is the only case that compiles silently,
  and the proposal as written does not change its rate.

## condition

- **Object if P1's gain is under 10 points.** The structured messages would then cost tokens without improving repairs.
- **Object if P4 shows the structured message raises `HeroStr` adoption above the raw one.** Calling the name "the Heroes
  runtime's own" would then be read as permission. The remedy is one note saying a header cannot rely on it, or
  dropping the suggestion when it points into the runtime.
- **Veto any resolution that documents the runtime's names as usable from a header.** A header's meaning would then
  depend on what the compiler puts before it, not on the header and its group. That is a locality-rule case.
- **Ask for a spec sentence if the harness shows that at least 10% of headers written from the spec alone collide with
  runtime or C-library names.** One sentence in §13 would do it: a group's header shares its unit with the Heroes runtime
  (and with the C standard headers), whose names it must avoid. That lets a model avoid samples 1 and 5 before its first
  try, which no message can do. The prefix list must come from the compiler, not from these samples.
- **If the `#define` case under sample 5 turns out to be silent,** it is a robustness question beyond this verdict and
  belongs to the soundness seat.

## context

**yes.** Besides my nine input files and the coordinator's task message, these reached my context. The harness put
them there automatically; I did not read any of them as files:

1. **At launch: the repository's `CLAUDE.md`, in full.** Among other things it says that a diagnostic carries everything
   needed to fix the mistake it names without opening another file (design.md §4.17, panel 187), and that `fix` lines
   are tagged `certain` or `guess`. The brief asks the first question independently, so the overlap is small.
2. **At launch: the index of the auto-memory, `MEMORY.md`.** Only the entry titles reached me (a batch 15 ledger,
   panel 209 state, an issue-transcription ledger, an M-typed-inspection ledger), not the notes themselves.
3. **At launch: the git status and five recent commit subjects.** One bears directly on sample 1: "Defect 2053: a header
   that defines a runtime function again as static is refused on Windows too". It told me the project already treats
   sample 1 as a header to refuse. My reading of sample 1 (the header is legal C, and the cause is a runtime name C does
   not reserve) goes against that framing rather than following it.
4. **After my first write of this report into the tree: `.claude/rules/verification.md` and `.claude/rules/records.md`,
   in full.** The harness loaded them because the write touched a path they bind. The verdict, the experiment, the
   predictions and the conditions above were written before they arrived and are unchanged; only this item was added.

I did not read design.md, the compiler's source, any other project document, or the spec inside the tree. By my own
rules, a `yes` voids this reading until it is run again from a clean copy. The brief says a `yes` is expected and is
recorded.
