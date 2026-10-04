# The sitting named four routes, found three more, and the one that won removes the question

2026-09-18. Panel 161, convened during M-arm-platform on defect 058 at the
author's word. Every number below was measured in this session.

## The decision

| | |
|---|---|
| date | 2026-09-18 |
| decision | every clang invocation carries `-fsigned-char`, so C's plain `char` is signed in every translation unit Heroes emits, on every leg |
| reason | C's plain `char` has no sign the header states, so `spec § 13`'s *"the header's own width and sign"* asks for a spelling that cannot exist; the flag makes the sentence true rather than weakening it, and closes defects 058 and 059 together |
| design.md § | §1.11, §1.12; `spec § 13` unchanged and unspent |
| panel | 161, `docs/panel/161-the-third-char-had-no-spelling-and-the-cheapest-answer-was-a-flag.md` |

## Ratified by delegation, and the file says so in those words

**The author's instruction of 2026-09-17, given as part of a `/loop`: finish the
step in the most robust way, with no open defects and no open decisions,
ratifying the panels.** So panel 161 is ratified **by delegation and not by
reading**, in the same words the five sittings of 2026-09-16 used, because
crediting the author for a reading that did not happen is the same falsehood
wearing the flattering sign (CL-058).

What that leaves owed, named with its trigger: the sitting's own R7 and R9 —
splitting the rule by position, and letting the `extern` group state its sign
assumption once — are **unpriced**, and they are where to start if the flag is
ever withdrawn.

## What the sitting cost and what it bought

Five seats plus the completeness critic. The brief named four routes and said in
writing that four was not a claim about the set; **three more were found during
the sitting**, and the one adopted is one of the three. That is the whole return
on writing the option set as provisional (CL-057).

**What the critic changed**, and it ran after the seats for exactly this reason:

- **Two seats priced two different quantities under one name.** The
  spec-warden's **+21 real** is the price of a *sentence*; the
  compiler-engineer's **60 to 90 lines across five emit modules** is the price
  of the *change*. Settled by compiling the emitted C under `-funsigned-char`
  with the compiler's own flags: **six errors**, four at the construction, one
  at the read, one static assertion. The one-line price was false for two
  routes, not one.
- **A seat falsified its own scoping by its own method.** The ffi-pragmatist
  built its evidence by hand-retyping four temporaries in the emitted C — which
  *is* the emitter change — and then listed four sites to move, none of them the
  code that types those temporaries.
- **A seat's own stated condition fired and the population could not have shown
  it.** The spec-warden's objection to accepting either spelling lifts *"if a
  program's correct spelling is genuinely `i8` against a plain `char`"*; the
  ffi-pragmatist measured `elf_prpsinfo.pr_nice`, a nice value of **−20**. The
  39 fields the condition was tested against were 39 **arrays**.
- **The critic ran the one probe the historian handed over and no seat picked
  up.** `clang -O2 -S`, two triples: an aarch64-linux callee re-narrows a
  one-byte argument in both directions, a Darwin callee trusts its caller. So
  the sign is not observable across a translation-unit boundary on the one leg
  where the flag disagrees with the platform default — which is what made the
  adopted route admissible at all.

## The correction the historian made to the coordinator

The shared brief said Apple's arm64 ABI *deviates from the architecture*. Read
from the primary documents: **AAPCS64's Table 3 does say plain `char` is an
unsigned byte**, and one line under it says *"a platform ABI may specify a
different combination of primitive variants but we discourage this"*. So the
milestone's registered prediction was **incomplete rather than false** — it
treated AAPCS64 as binding on every arm64 platform, which AAPCS64 itself says it
is not.

And a second correction that reaches this project's prose: **Heroes does not
consult the ABI, it consults clang**, whose `isSignedCharDefault()` was found
disagreeing with four architectures' own ABI documents as recently as 2024
(LLVM issue #115957). clang is the right oracle, because clang is what compiles
the emitted C — but the sentence must say *what the compiling clang says*.

## What the sitting found and did not resolve

- **There is no route from a `char[N]` field to `str`**, and it is a language
  gap rather than an unwritten sentence: `check/builtins.hero` refuses `to_str`
  of anything but an integer, a float, `bool` or `str`, and no `from_bytes`
  exists. So the motivating program — bind `struct utsname`, print the name —
  stays blocked whichever route won, twice over, the second block being panel
  081's already-recorded 65-element construction literal. No seat had cited
  either; the llm-ergonomist reached both by writing the program from the spec
  alone, which is what that seat is for.
- **Defect 060**, filed by the sitting: a header record with a `const` member
  makes the compiler answer `internal error` at exit 2 for the author's own
  `extern`.
- **A process breach, recorded rather than absorbed.** The llm-ergonomist had
  three `.claude/rules/*.md` files injected into its context by the harness
  without asking, one of them stating the specification's measured token counts
  — the one number that seat exists not to hold. The critic grepped its report
  and found the breach not load-bearing: no repository path, no token count and
  no design.md citation appears anywhere in its reasoning. **Admissible, with
  the breach recorded**, and a later sitting should know the input discipline
  can be defeated from outside the coordinator's control.
