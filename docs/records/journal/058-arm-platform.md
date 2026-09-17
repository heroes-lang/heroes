# 058 — M-arm-platform: the fourth real machine, and C's third `char`

## Goal

arm64 Linux as a measured platform: an image beside the x86-64 one, the seed
built from C alone there, the compiler's own tests and the harness's own
passing, and a fourth CI matrix entry — so the leg is a judge and not a hunting
instrument. And one registered prediction to score: **plain `char` is unsigned
on the ARM ABI and signed on x86-64**, which either finds a divergence in the
corpus's 20 `extern` programs of 55 or shows the FFI's width rules are stronger
than three legs could prove.

The machine was the deliverable. What it found in its first hour was the
milestone.

## What surprised

**A prediction can be right about the fact and wrong about the cause, and the
cause is what tells you where to look next.** The row said *unsigned on the ARM
ABI*. The divergence is real — Debian arm64 reads `CHAR_MIN 0`, `CHAR_MAX 255` —
but this Mac is an arm64 machine and reads `-128 … 127`. AAPCS64's Table 3 does
say `char` is an unsigned byte, and one line under it says *"a platform ABI may
specify a different combination of primitive variants but we discourage this"*;
Apple's own arm64 document says *"the `char` type is a signed type"*. So the
fact belongs to the **platform's ABI**, and a fourth leg chosen for its
architecture alone could have been an arm64 Darwin and shown nothing at all.
What made this leg informative is the pair *arm64 + Linux*, which nobody argued
for by that name when the row was scheduled.

**And the search location was wrong too.** The prediction said the corpus's 20
`extern` programs were where it would show. `corpus` reads 53 passed, 0 failed
on the new leg — none of them binds a plain-`char` member. It showed in a golden
case written thirteen months of work earlier, whose own comment carries the
premise the leg falsified: *"`char` is signed here"*, measured on one machine,
true on three legs of four.

**An instrument can measure the wrong thing while running correctly.** The first
AST walk over nine headers returned *0 plain-`char` array fields*. It tested
`qualType.startswith('char [')` and clang writes `char[14]` with no space. The
corrected walk returns 23. That number then went into a shared brief under the
heading *"it is arrays, not scalars"* — and a seat's wider walk found four
scalars in `elf_prpsinfo`. **The count was right about its nine headers; the
sentence over it was a claim about the world.** Three of five seats rested on it
without re-measuring.

**Five judges differentiated by input still share one blind spot, and the
instrument for that is a sixth reader who gives no verdict.** Panel 161's
completeness critic found that two seats had priced two different quantities
under one name — the spec-warden's `+21 real` is the price of a *sentence* and
the compiler-engineer's 60-to-90 lines is the price of the *change* — and that
the ffi-pragmatist had falsified its own scoping by its own method, hand-editing
the emitted C and then listing four sites to move, none of which was the code it
had just hand-edited. It also ran the one probe the historian had handed to
"whichever seat owns the C boundary" and that no seat picked up.

**The cheapest answer and the most robust one were the same, and that is worth
distrusting until it is measured.** `spec § 13` tells an author to declare a
field at *the header's own width and sign*. C's plain `char` has no sign the
header states, so the rule asks for something that cannot exist. Six routes
tried to describe the divergence — a ninth integer type, a new word inside
`extern`, accepting either spelling, refusing the type, mandating `u8`, splitting
the rule by position. The seventh **removes** it: one string, `-fsigned-char`,
so the sign is the same on every leg and § 13's sentence becomes true to the
letter rather than being weakened. No spec token. No new vocabulary. The Linux
kernel took the mirror flag in 6.2 for the reason Torvalds gave, that making the
language rules stricter to avoid differences is a good thing.

**A defect filed apart can turn out to have no repair of its own.** 058 and 059
were separated because their repairs looked different — a language question and
a table row. 059 closed with **no edit**: `c_spellings.hero:59` was never wrong
code, only a sentence that was false on one leg, and 058's flag makes it true on
four.

## What broke and why

- **`tests/golden/run/ffi-a-char-array-member.hero` did not compile on the new
  leg**, and took `run`, `determinism` and `emission` with it — 1825 passed, 3
  failed, every other suite at zero. Cause: the case binds `char name[4]` as
  `i8[4]`, and `emit/extern_field.hero:154` compares the header's sign with the
  declared one, correctly, on a leg where they differ. Fix: the flag. It now
  runs there and prints `68` and `68 7`, the same as everywhere else.

- **On arm64, the `ffi_parameter_type` note said *"Declare it `i8`"* about the
  `i8` it had just refused.** Cause: `c_spellings.hero:59` tables `char -> i8`
  on every target, in a file whose own header comment records having learned,
  one type over with `long`, that a target question is not a constant to be
  tabled. Fix: the same flag, which makes the table's answer true rather than
  changing it.

- **`selfhost/main.hero`'s own tests went 654 passed, 1 failed** the moment the
  flag landed, on `"the array is the fact: fifteen flags, gnu11 named first"`.
  That is the instrument working: the flag list's length is pinned by a test, so
  a sixteenth string cannot arrive in silence. The test now asserts sixteen and
  asserts the new flag **by name**, because a count alone would go green on any
  sixteenth string.

- **The formatter moved a comment out of an array literal.** A block written
  beside `"-fsigned-char"` inside `flags()` came back below the function,
  orphaned. Not a defect filed — it relocates rather than drops, and the file's
  own convention puts each flag's reason in the block above `flags()` — but it
  is written down because the next session to comment a list element will meet
  it.

- **A header record with a `const` member makes the compiler blame itself**:
  `internal error: compiling the generated C failed` at exit 2 for an `extern`
  the author wrote. Found by panel 161's ffi-pragmatist beside the `char`
  question, reproduced on this Mac before filing, and filed as defect 060.

- **Two numbers in `docs/ROADMAP.md` were behind.** § The chain's opening
  sentence said rows 1–57 were done over a table whose row 58 had closed that
  morning — found at the OPENING of row 59 rather than at that close, the second
  time that sentence has been wrong. And § Where we are said the net's own tests
  were 156 where both machines read 157; the test that made the difference
  entered in `1029ee3c`, the commit before this milestone's first.
