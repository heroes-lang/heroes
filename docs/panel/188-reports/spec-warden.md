# Panel 188, the spec-warden's report

Written as I went, 2026-10-03 from 16:29 to 17:15 by `date`. My copy is
`<scratchpad>/188-spec-warden/`, `git archive 826ddc2f`; its seed's sha256
begins `2c809845ed0f7ba8` and the compiler built from it `afc05be6b2c50184`,
both as the shared brief says. **My cost**: no paid run, no `--refresh`, no
container; local builds and runs only, one process at a time. I did not read
the other seats' reports (they reached the directory at 16:57), so this
verdict is independent of them.

## The ceiling, by grep

design.md §1.6, line 255 today: **10240 tokens measured by `claude-opus-5`
through `POST /v1/messages/count_tokens`**. The payment rule (same section,
lines 312 to 327): every amendment owes a named removal or a registered
prediction naming an instrument that exists on the day of registration and
the milestone at which it is scored; and (lines 379 to 381) every addition
still carries §1.0's burden of proof, compiler-need or a measured thesis
effect, whatever the headroom.

## The baseline, measured in my copy

`./heroes measure spec/heroes-spec.md` at 16:29: `claude-legacy` 6990,
`cl100k_base` 7117, real 9392 (`claude-opus-5`, the pinned figure recorded
2026-10-03). Headroom 848, of which the FFI floor mortgages 60 (panel 030 R3):
**788 free** on the binding instrument. Same numbers as F3. No draft below
comes within 750 of the ceiling, so no budget veto is in play: every verdict
here is on the payment rule and §1.0's burden.

## Run in my copy before drafting (16:35 to 16:50)

- `extern "/usr/include/stdio.h"`: `check` exit 1, `machine_locked_path`,
  whose message states the whole rule and three repairs (`CPATH`, a
  `package`, a relative `sub/foo.h`). This is the shape F4's precedent
  (`aab44f9b`) leaned on: *"the sentence is §1.4 redundancy the compiler pays
  back loudly"*, panel 089's condition.
- F6 re-run, `HEROES_RUNTIME` at my copy's `runtime/`: `extern "<stdio.h>"`
  `check` 0, `build` **2**, *internal error: compiling the generated C failed*,
  clang's `#include <<stdio.h>>`. **F6's unrun cell, answered on this Mac**:
  `extern "stdio.h>"` builds at exit 0 and prints `hi`, but clang prints
  `warning: extra tokens at end of #include directive [-Wextra-tokens]`
  **twice** (once for `build/pointee-*/check-*.c`, once for
  `build/tu-*/p.c`), pointing at the generated C and never at the program's
  line. So the malformed name is not silent; it is told in clang's voice.
- **The candidate removal, run**: a hand-written `.pc` answering
  `Cflags: -I... -pthread` under `PKG_CONFIG_PATH`, and `extern "sevenpk.h"
  package "sevenpk"`: `check` 0, `build` exit 1, `error[ffi_package]: the
  package `sevenpk` answered with `-pthread`, which this compiler does not
  pass on`, then a note naming the whole allow-list and the reason (Go's
  CVE-2018-6574) and a note *name the library directly with `link` if you
  need it*. The message states the entire rule and its repair: panel 089's
  condition, met exactly as `aab44f9b` met it for the absolute path.
- **The tree, counted** (`grep -rhoE '^[[:space:]]*extern "[^"]*"'` over the
  tracked `.hero` files of my archive, my own `cases/` and `drafts/`
  excluded): **648** group heads, **250** distinct header names, **0** holding
  `<` or `>`, none empty, every one made of letters, digits, `_`, `-`, `.`
  and `/` only; one absolute, `tests/golden/check/ffi-a-group-head-names-not-locates.hero`,
  the `machine_locked` golden, already exit 1. The only bracketed spellings
  anywhere in `.hero` and `.md` files are two comments using `"<header>"` as a
  placeholder (`selfhost/emit/clang_place.hero:142`,
  `tests/harness/suite_records.hero:2604`) and defect 216's own item. F6
  quotes the critic's **832** lines; my method counts declarations at a
  line's start and reads 648, and I have not reconciled the two. Both say
  zero declarations put brackets in the name.
- `include` is a word the lexer refuses with a fix (`selfhost/keywords.hero`
  line 309), and `spec/rejected` reads every identifier inside a code span.
- The emitter writes every group's header as `"#include <" + header + ">"`
  (`selfhost/emit/c_text.hero:113`), and its own test at line 197 writes
  `h??).h` as `#include <h?\` over `?).h>` (defect 207's splice): **the
  name and C's spelling of it are not the same text**, which decides between
  two of the wordings below.
- The compiler's own groups name `hero_os.h`, `heroes_runtime.h` and
  `stdlib.h` only: nothing in this sitting is a self-hosting need.

## The drafts and their prices (16:45 to 17:05)

Each draft replaces § 13's *"A group names its header, and `link` a library
when the symbols need one."* in a copy of the frozen spec (merging, per
`.claude/rules/spec-shape.md`), unwrapped unless it says otherwise; priced by
`./heroes measure <copy>` from my copy's root. **Every delta below is
vendored, so a lower bound, in those words**; the real one is the
coordinator's `--refresh` at a landing. On this document's last § 13 landing
and its last round the reader's instrument read 1.17 to 1.24 times the cl100k
delta (ledger rows 6928 and 7117), so +10 here is about +12 to +14 real: an
inference, not a measurement. Scripts and every copy:
`<scratchpad>/188-spec-warden/drafts/` (`make_drafts.py`, `make_drafts2.py`,
`make_drafts3.py`, `price.py`, `out/`).

| draft | route | the header sentence becomes | legacy | cl100k |
|---|---|---|---|---|
| **a6_without** | (1a) | *A group names its header without the `<` `>` C puts around it, and `link` ...* | +11 | +10 |
| a1_between | (1a) | *A group names its header as C writes it between `<` and `>`, and `link` ...* | +11 | +10 |
| a3_list | (1a) | *A group names its header, never empty and holding no `>` or line end, and `link` ...* | +11 | +11 |
| a7_name | (1a) | *A group names its header, the name between C's `<` and `>`, and `link` ...* | +12 | +11 |
| a5_incl | (1a) | *... as `#include <...>` holds it, without the brackets, ...* | +13 | +13 |
| a2_example | (1a) | *A group names its header, `stdio.h` and never `<stdio.h>`, and `link` ...* | +14 | +13 |
| a6s | (1a) and `\` | a6 plus *`/` between directories* | +17 | +15 |
| a4_both | (1a) | *... as C writes it between `<` and `>`, so never empty and with no `>` or line end, ...* | +23 | +22 |
| abs_only | F4 restored | *... when the symbols need one, neither by an absolute path.* | +6 | +6 |
| a6abs | (1a) and F4 | a6 plus *, neither by an absolute path* | +17 | +16 |
| a1abs | (1a) and F4 | a1 plus the same | +17 | +16 |
| b3_bare | (1b) | *... as C writes it between `<` and `>`, with no `'` `\` `"` `//` or `/*`, ...* | +26 | +25 |
| b2_after | (1b) | the old sentence kept, *The name is written as C writes it between ..., with no ...* after it | +32 | +30 |
| b1_merged | (1b) | b3 plus *which C leaves undefined there* | +32 | +31 |
| c1_set | (1c) | *... by a relative path of letters, digits, `_` `-` `.` `+` and `/`, ...* | +21 | +20 |
| c3_posix | (1c) | *... by a relative path of letters, digits, `_` `-` and `.`, `/` between directories, ...* | +21 | +20 |
| c2_set_c | (1c) | a1 plus c1's set | +32 | +30 |
| g1_noesc | (1g) | *A group names its header, in a string with no escape, and `link` ...* | +7 | +7 |
| **r1_remove** | removal | *A package answering with anything this compiler does not pass on is refused, naming what it said.* deleted | **-22** | **-22** |
| **a6+r1_wrap** | (1a) paid | a6 and r1, the paragraph re-wrapped (text below) | **-12** | **-13** |
| a6+r1 | (1a) paid | a6 and r1, unwrapped | -11 | -12 |
| a1+r1 | (1a) paid | | -11 | -12 |
| a7+r1 | (1a) paid | | -10 | -11 |
| a2+r1 | (1a) paid | | -8 | -9 |
| a6abs+r1 | (1a), F4, paid | | -5 | -6 |
| a1abs+r1 | (1a), F4, paid | | -5 | -6 |
| b3+r1 | (1b) paid | | +4 | +3 |
| c3+r1 | (1c) paid | | -1 | -2 |
| g1+r1 | (1g) paid | | -15 | -15 |

Re-wrapping moved the count by one, as panel 181 found.

**The suites that read the spec, on the drafts, run in my copy** (the draft
swapped into `spec/heroes-spec.md`, the base restored after, `cmp` clean;
`./heroes run tests/harness/main.hero -- ./heroes <suite>`, each output read
whole): the base reads `spec` 21 passed, 0 failed. a1, a1+r1, a2, a6, a7,
b1, c1, g1, a6+r1_wrap and a6abs+r1 each read `spec` 17 and 4, the four
being the budget family (`budget`, `spendable`, `real`, `ledger`), which any
change turns red until its pins move. **a5 reads 16 and 5: `spec/rejected`
goes red on `include`.** So any seat's sentence that writes C's directive in
backticks is refused by the document's own instrument. `special` 10 and 0
and `grammar` 9 and 0 on a6+r1_wrap, a6abs+r1 and a7. `'`, `\` and `"` in
code spans (b1) pass `named`, `rejected` and `inventory`.

## Q4. Is a sentence owed? The two precedents, and what reconciles them

**F4's precedent** (2026-09-05, `aab44f9b`): *Neither may be an absolute
path.* left the spec as the named removal paying for panel 111's callback
sentence, because the message states the rule: *"§1.4 redundancy the
compiler pays back loudly"*, panel 089's condition.

**A later precedent of this seat pulls the other way.** Panel 181
(2026-09-28), the spec-warden: *"the refusal can be inferred, but nothing
forces the inference: a rule a reader can miss, which is the case for
stating it"*. The author's instruction that night (`345f167b`, meant as
*always choose the most robust and solid route, even at the cost of the
spec's tokens*) took the longer text, and its commit names panel 181's
sentence as its first application. That instruction, and the one of
2026-09-12, both postdate F4's removal.

**design.md §1.4 reconciles them** (line 235): *redundancy is spent
deliberately, where errors actually occur, and nowhere else*. Stated as a
test for this sitting: **a refusal of what the writer writes, in a shape a
reader can plausibly reach from the document, is forced by the document; a
refusal of a shape nobody writes, or of what the machine answers, has its
home in the message.** Applied:

- **F6's `extern "<stdio.h>"`** is C's own directive carried into the
  string: the one member of the class a reader who knows C plausibly
  reaches, because *"A group names its header"* never says which spelling
  the string takes. Panel 055's blind reader showed that from that very
  sentence (`docs/panel/055-where-a-header-is.md`, lines 32 to 34: it
  *"never says the string is a C include spelling rather than a filesystem
  path"*). **Document.** Whether models write it at all is unmeasured: 0 of
  648 in the tree, and the A/B measures the repair after a message, not the
  first try (the critic, in the llm-ergonomist's brief). So this rests on an
  argument from Part 1 (§1.3, §1.9: a reader carries C's spelling of the same
  thing), which §1.0 admits if the panel accepts it, and not on a
  measurement. P3 below is the measurement that would settle it.
- `>` elsewhere, a line end, a NUL, the empty name: nobody writes them, and
  a6 forces them anyway for a reader who knows C (a name C puts between `<`
  and `>` cannot hold `>` or a line end and is not empty). **Message.**
- **(1b)'s** `'`, `\`, `"`, `//`, `/*`: 0 of 648 tracked names hold one;
  F1's names were made for the test. **Message.**
- **The escapes** are not a rule at all: § 2 already defines a string's
  value (six escapes) and the `Extern` production takes the lexer's
  `string`. The compiler writing the source spelling is the compiler's bug
  (CLAUDE.md § 12). **No sentence.**
- **r1's package refusal** is what a `.pc` file answers, which no sentence
  lets a writer avoid, and its message states the whole rule and the repair
  (run above). **Message**, so r1 is removable by the same test that owes a6.
- **The absolute path** is written by the writer, and panel 055's blind
  reader reached for it from the same sentence. **Document**, by this test,
  which is the consequence the synthesis has to face: if it adopts the test,
  F4's removal is reversed (abs_only +6 and +6; a6abs+r1 -5 and -6); if it
  declines, the record should say that F4's standard governs absolute paths
  and panel 181's governs brackets.

## Verdict per route

| route | verdict | spec tokens (vendored, lower bounds) | what it owes |
|---|---|---|---|
| (1a) the refusal | **approve** | 0 | nothing on my scale: a diagnostic is not a form (panel 181's warden; §1.0 binds what enters), and it closes a `blocking` class (exit 2, a malformed name accepted), robustness and truth ranking above tokens (CLAUDE.md § Precedence, 3 over 4) |
| (1a)'s sentence | **approve a6, paid by r1**, provisional on the real count | a6+r1_wrap -12 legacy, -13 cl100k | r1, a named removal measured in the commit that spends it; P1 registered beside it |
| the absolute path | approve abs with a6 if the synthesis adopts the test above; otherwise record which standard governs which | a6abs+r1 -5 and -6 | r1 covers it |
| (1b) | the refusal: no objection on budget at 0 tokens, its home the message; **object** (not a budget ground) that it turns away names clang builds today (F1: `a'b.h`, `d//b.h`, `e/*b.h` print 7) for 0 measured writers, which is the historian's C text and the unrun Windows box to settle. **Veto any (1b) sentence** (b1, b2, b3) | b3 +26 and +25; b3+r1 still +4 and +3 | §1.0's burden unmet: redundancy spent where errors do not occur (§1.4) |
| (1c) | **veto** | c1 or c3 +21 and +20, c2 +32 and +30 | §1.0's burden unmet for its increment over (1a), and it cannot be made consistent: a real header outside the set (one holding `+`, say) is a shape a reader writes, so the refusal cannot live in the message alone; and its sentence writes into the one trusted document a premise about which marks headers use, measured on this Mac at most (the tree: letters, digits, `_ - . /`; the machine's survey is the ffi-pragmatist's; Linux and Windows unrun) |
| (1d) | approve as a complement at 0 tokens; **object** as the whole answer | 0 | it leaves `extern "stdio.h>"` accepted (exit 0, clang's warning in clang's voice, measured above) and F6's shape a `build` failure, where the thesis wants it at `check`, before any clang run |
| (1f) | **approve, owed under every route** | 0 | nothing: it is the spec today (§ 2; CLAUDE.md § 12) |
| (1g) | **object** | g1 +7 and +7 if stated | an exception to § 2's escapes at one string position, which the document would then owe to stay derivable, to buy nothing (1f) with (1a) does not; unstated, it refuses a string § 2 calls valid |
| (1h) | **approve (1a) with (1f)**, with a6+r1 (robust) or no sentence (conservative, F4's standard, 0 tokens) | as above | as above |
| (1i) | no budget ground; if it lands, **no sentence**, because it makes a6 false (C would no longer put `<` `>` around the name) | 0 | its soundness (panel 036 R2's decoy, F5) is unrun by me and not my seat's |
| (1e) | the spec-side routes nobody listed: a positive statement of the spelling (a6) instead of a list of refusals; payment by a removal verified by running (r1); the test above and its consequence for the absolute path | | |

**Against the wordings I do not recommend.** a1 says *as C writes it*, which
is false for any name whose Heroes string needs an escape (`a"b.h` is
`"a\"b.h"` here and `<a"b.h>` in C) and for a trigraph name, which the
emitter splits across two lines (`c_text.hero:197`); a6 names the brackets
and leaves the spelling to § 2, so it stays true under (1f). a3 enumerates
the refusals and never names the mistake a reader makes. a2 spends three
more tokens on one header's example. a4 is a1 and a3 together. a5 cannot
land (`spec/rejected`). a7 is an acceptable second to a6, one token dearer.

**The recommended text**, re-wrapped as priced (a6+r1_wrap):

    libraries. A group names its header without the `<` `>` C puts around it, and `link` a
    library when the symbols need one. clang checks every result type, constant and record
    field against that header, and a result may be wider than C's.

and, in the package paragraph, *asks the system where its headers and
libraries are and what else it needs.* ends the paragraph, the sentence after
it deleted. With the absolute path (a6abs+r1): *..., and `link` a library
when the symbols need one, neither by an absolute path.* Nothing outside the
spec reads r1's words (`grep`: only panel 153's dated brief quotes them).

## The payment, and the instruments I lean on

- **A named removal over a prediction.** design.md §1.6 (lines 316 to 338):
  a named removal is measured in the commit that spends it, while a
  prediction is collected later or lapses and is re-argued. r1 is measured
  (-22 and -22) and verified by running.
- **I lean on `heroes measure --refresh` and the batch's census of `check`**
  (`.claude/rules/verification.md` § The batch, step 3), both existing today.
  Not this sitting's A/B: it measures the repair after a message, not whether
  a reader writes the brackets. Not `heroes mutate`: its sixteen operators
  (`selfhost/mutate/ops.hero`) hold none on a header, so a prediction on it
  pays nothing (§1.6). Not a golden: it scores the refusal, which the
  sentence does not cause, and the goldens are owed anyway by
  `.claude/rules/diagnostics-and-goldens.md`.

## Predictions

- **P1 (registered, scored at the landing by `heroes measure
  spec/heroes-spec.md --refresh`)**: a6+r1 re-wrapped reads between **-20 and
  -6** against 9392 on `claude-opus-5`; with the absolute-path clause
  (a6abs+r1), between **-14 and +2**. Falsified outside those ranges.
- **P2 (registered, scored at the round's gate by the census)**: with (1a)
  and (1f) landed, alone or with (1b) or a set of letters, digits and
  `_ - . /`, the census of `check` over the tracked `.hero` files, trunk
  against the round's compiler, changes the exit of **0** files besides the
  sitting's new cases. Basis: the 648 names above.
- **P3 (an observation, paying nothing until run)**: ten fresh sessions
  given the status-quo spec and asked to bind `sqrt` from `math.h` write the
  brackets inside the string in **at most 1**; with a6, in 0. A `claude -p`
  generation run, the coordinator's to authorise; unrun.

## The condition that would change my verdict

- **a6 is not owed**, and I accept no sentence rather than veto it, if the
  synthesis keeps F4's standard and says so; **a6 is owed under either
  standard** if P3's run finds 2 or more of ten bracketed on the status quo;
  **a6 must not land** if (1i) does.
- **r1 stops being a removal** if anyone shows a first-try program written
  differently because of it; a6 would then be paid by P1 as a registered
  prediction, admissible and weaker.
- **The veto on (1c) lifts** if its set is a standard's rather than the
  world's (POSIX's portable filename character set is recalled as letters,
  digits, `.`, `_` and `-`, which is exactly what the tree's 648 names use
  with `/`: unverified, the historian's) **and** a writer of a character
  outside it is measured. **The veto on a (1b) sentence lifts** if a writer
  of one of its five is measured, in the tree, a generation run or a corpus.

## Byproducts for the coordinator, filed by nobody here

- F6's unrun cell, answered above: `extern "stdio.h>"` draws clang's
  `-Wextra-tokens` twice, in clang's voice, at the generated C.
- `ffi_package` points at the group's first member (`p.hero:2:5`), not at
  the `package` string on line 1: a true message less exact than it could
  be, `adjacent` by its class if filed.

## The structured verdict

- `verdict`: **approve** (1a) and (1f) at 0 spec tokens; **approve** a6
  merged, paid by r1, **provisional** (the real delta is unrun; vendored lower
  bounds only); **object** to (1b), to (1d) alone and to (1g); **veto** (1c)
  and any (1b) sentence.
- `section`: design.md §1.6 (the payment rule, lines 312 to 327, and §1.0's
  burden for every addition, lines 379 to 381), §1.4 (line 235), §1.2;
  CLAUDE.md § 12 for (1f).
- `spec_token_delta`: real 9392 before (pinned); a6+r1 re-wrapped, cl100k
  7117 to 7104 and legacy 6990 to 6978, **-13 and -12, lower bounds**; real
  after unrun.
- `removal`: r1, § 13's package-refusal sentence, -22 on both vendored
  tables, verified by running.
- `needed_for_self_hosting`: no.
- `argument`: The refusal is a diagnostic, not a form, and closes a blocking
  class at zero spec tokens; (1f) is already § 2. Two precedents meet: F4
  removed a rule its message states; panel 181, later and under the author's
  2026-09-28 instruction, stated a rule a reader can miss. §1.4 reconciles
  them: redundancy goes where errors occur. Of this class only C's own
  `<stdio.h>` is a mistake a reader plausibly makes, because § 13 never says
  which spelling the string takes. a6 says it for ten vendored tokens and
  forces (1a)'s refusals; r1, a refusal of what the machine answers, stated
  whole by its message, pays twice over. (1b)'s and (1c)'s sentences spend
  redundancy where nobody errs.
- `prediction`: P1, a6+r1 re-wrapped reads -20 to -6 real at its landing's
  `--refresh`; P2, the census changes 0 tracked files' exit.
- `condition`: F4's standard kept by the synthesis (a6 not owed), P3 at 2 or
  more of ten (a6 owed either way), (1i) adopted (a6 false), or r1 shown to
  change a first-try program.
