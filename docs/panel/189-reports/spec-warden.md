# Panel 189, the spec-warden's report

Opened 2026-10-04 00:13 by `date`; written as I go. My brief is
`docs/panel/189-briefs/spec-warden.md` (the repaired one; the text before the
critic is `spec-warden-before-the-critic.md`), bound by `00-shared.md` and
`00-facts.md`.

## My copy

`<scratchpad>/189-spec-warden/`, made at 00:12 with `git -C <trunk> archive
7d9f2e8f | tar -x`. Seed sha256 begins `3bc3aa8bd2b5ba65`, the compiler built
from it in the copy `958320f39dee0b9e`, both as the brief says. My cases are
`<scratchpad>/189-spec-warden-cases/` (F1's two rows copied there, mine beside
them).

design.md §1.6, reached by grep in my copy (line 255): **10240 tokens, measured
by `claude-opus-5` through `POST /v1/messages/count_tokens`**. The payment rule
(lines 312 to 327): every amendment owes a named removal, measured in the commit
that spends it, or a registered prediction naming (i) the instrument that will
score it and (ii) the milestone at which it is scored, the instrument existing
on the day of registration (metric 3, `heroes mutate`, `heroes measure`, a line
count, a compile, a diagnostic transcript). Lines 379 to 381: every addition
carries §1.0's burden whatever the headroom.

## Run before drafting (00:13 to 00:18, my compiler, `HEROES_RUNTIME` my copy's `runtime/`)

- **F1's two rows I lean on, re-run from copies**: `comment-latin1` (`xxd`:
  `# caf` then `e9`) `check` exit **2**, *error: cannot read `p.hero`*; `used`
  (`geom.hero` holding `e9` in a comment) exit **1**, `unknown_module`, *... and
  that file is not there*. Both hold.
- **What § 1's sentence claims, tested** (*"comments may contain any UTF-8,
  strings any but a raw carriage return or line end"*): a tab in a comment and
  in a string, a raw CR in a comment, a NUL in a comment, a whole file in CRLF:
  each `check` exit **0**. A raw CR in a string: exit 1, `raw_carriage_return`,
  `fix (certain): write it as \r`. And between 00:33 and 00:34 (two `date`
  readings), in a comment and in a string
  each: U+202E (a right-to-left override, Trojan Source's character), U+FEFF
  in the middle of the text, U+2028 (a line separator), and VT and FF in a
  comment: each exit **0**. The sentence, and every draft's *"any
  character"*, is true on every shape I tried.
- **What the spec's word "UTF-8" means to the runtime**: `hero_utf8_valid`
  (`runtime/parts/str.c:248`) refuses a continuation byte or 0xF8 to 0xFF as a
  leader, a truncated sequence, an overlong form, a surrogate and anything past
  U+10FFFF: RFC 3629's UTF-8, read from the code (not run against each shape).
  So the spec's word and the check that would fire are the same set.
- **The two namespaces, enumerated from the tree** (my copy): the codes the
  compiler prints, every `error[...]` and `warning[...]` in `tests/golden/`,
  **187**; the failure codes a program can see, every `fail(code: ...)` in
  `selfhost/library_source.hero` and every `hero_failure_*` name in
  `selfhost/` and `runtime/`, 12 strings, of which 8 are codes
  (`file_not_found`, `not_found`, `not_text`, `null_cstr`, `read_failed`,
  `write_failed`, `missing_key`, `does_not_fit`) and 4 are helper names
  (`hero_failure_eq`, `_hash`, `_release`, `_retain`). **Intersection:
  none.** Widened to every unescaped `code: "..."` literal in `selfhost/`
  (372): one shared word, `does_not_fit`, and there it is the parser's internal
  `fail` (`parse/fixed_length.hero:82`), not a printed diagnostic. So
  **`not_text` would be the first word that is both a code the compiler prints
  and a code a program sees.**
- **What the spec names**: its code spans hold exactly four failure codes,
  `missing_key` (§ 10), `not_found` (§ 11), `null_cstr` and `not_text` (§ 13),
  and of the spec's 124 code-span words **none** is one of the 187 printed
  diagnostic codes; against all 372 code literals of `selfhost/`, one word is
  shared, `x`, a unit test's placeholder code (`diag_render.hero:139`) and a
  field name in the spec. The document names the codes a program handles and never a
  code the compiler prints. `read_file`'s own codes are not named either (§ 11
  gives its type only).

## The baseline, measured in my copy (between 00:18 and 00:20)

`./heroes measure spec/heroes-spec.md`: `claude-legacy` 6990, `cl100k_base`
7117, real 9392 (`claude-opus-5`, 2026-10-03). Headroom 848, of which the FFI
floor mortgages 60: **788 free** on the binding instrument. No draft below
comes within 750 of the ceiling, so **no budget veto is in play**; every
verdict here is on the payment rule and §1.0's burden.

## The drafts and their prices (00:20 to 00:25)

Each draft is a copy of the frozen spec with one wording swapped in, every
anchor asserted to occur exactly once, wrapped by a function that reproduces
the document's own wrapping of the two bullets it touches (asserted), so a
delta measures words and not a re-wrap. Priced by `./heroes measure <copy>`
from my copy's root. **Every delta is vendored, so a lower bound, in those
words** (`.claude/rules/spec-shape.md`); the real one is a `--refresh` at a
landing, which no seat runs here. On this document's recent prose rows the
reader's instrument read **1.16 to 1.42 times** the `cl100k` delta (ledger
rows 6126, 6159, 6212, 6344, 6693, 6838: +48/+37, +47/+33, +54/+40, +82/+62,
+37/+32, +61/+44) and 1.0 on a five-token cut (row 6923): so +5 here is about
+6 to +7 real, **an inference, not a measurement**. Scripts and every copy:
`<scratchpad>/189-spec-warden/drafts/` (`make_drafts.py`, `price.py`,
`suites.sh`, `out/`).

The base is § 1's bullet, *"Syntax is ASCII-only; comments may contain any
UTF-8, strings any but a raw carriage return or line end: a string is one
line."* `r1` is panel 188's candidate removal, still in § 13 at `7d9f2e8f`:
*"A package answering with anything this compiler does not pass on is refused,
naming what it said."*

| draft | route | the text | legacy | cl100k |
|---|---|---|---|---|
| u0 | panel 055's standard: no sentence | the spec unchanged (`cmp` against the archive's) | 0 | 0 |
| **u1** | 181: the rule, merged | *A file is UTF-8 and its syntax ASCII-only; comments may contain any character, strings any but ...* | **+5** | **+5** |
| u2 | 181, merged | *A file is UTF-8, its syntax ASCII-only; ...* | +5 | +5 |
| u3 | 181, merged | *Syntax is ASCII-only and the whole file UTF-8; comments may contain any character, ...* | +6 | +7 |
| **u3b** | 181, merged | *Syntax is ASCII-only, the file UTF-8; comments may contain any character, ...* | **+4** | **+4** |
| u7 | 181, in the first bullet | *One file is one module, in UTF-8; the file you compile holds ...* | +5 | +5 |
| u8 | 181, both bullets | u7, and the ASCII bullet's *any UTF-8* becomes *any character* | +3 | +3 |
| **u5** | 181, the rule and its padded shape | *A file is UTF-8 with no byte-order mark, and its syntax ASCII-only; comments may contain any character, ...* | **+13** | **+14** |
| u5b | 181 | *A file is UTF-8, without a byte-order mark, and ...* | +14 | +15 |
| u4b | the consequence, merged | *A byte that is not UTF-8 is a compile error, in a comment too; syntax is ASCII-only, comments may contain any character, ...* | +17 | +18 |
| u4 | the consequence, appended | the base, then *A byte that is not UTF-8 is a compile error, in a comment too.* | +19 | +20 |
| u6 | appended | the base, then *A file that is not UTF-8 is a compile error.* | +14 | +15 |
| **r1** | removal | § 13's package-refusal sentence deleted | **-22** | **-22** |
| u1+r1 | 181, paid | | -17 | -17 |
| **u3b+r1** | 181, paid | | **-18** | **-18** |
| u8+r1 | 181, paid | | -19 | -19 |
| **u5+r1** | 181 with the BOM, paid | | **-9** | **-8** |
| u4b+r1 | consequence, paid | | -5 | -4 |
| u6+r1 | appended, paid | | -8 | -7 |

**Merging beats appending, measured again**: the rule merged costs +3 to +5
(u1, u3b, u8), the rule appended as a consequence +15 to +20 (u4, u6).

**The suites that read the spec, on the drafts** (00:22 to 00:25; the draft
copied over my copy's `spec/heroes-spec.md`, the three suites run one at a
time, the base restored and proved by `cmp` after each): the base reads
`spec` 21 passed, 0 failed. u1, u1+r1, u3b+r1, u8+r1 and u5+r1 each read
`spec` **17 and 4**, the four being `budget`, `spendable`, `real` and
`ledger` (read from each output's `FAIL` lines), the family any change turns
red until its pins move; `inventory`, `named`, `rejected`, `shape`, `anchors`
and `offered` pass on all five (u5's *byte-order* is ASCII). `special` 10 and
0 and `grammar` 9 and 0 on all five.

**`r1` re-verified by running it** (between 00:20 and 00:22, my compiler): a hand-written
`sevenpk.pc` answering `Cflags: -I<dir> -pthread`, `extern "sevenpk.h" package
"sevenpk"`: `check` exit 0, `build` exit **1**, `error[ffi_package]`, *the
package `sevenpk` answered with `-pthread`, which this compiler does not pass
on*, a note stating the whole allow-list and its reason (Go's
CVE-2018-6574) and a note *name the library directly with `link` if you need
it*. The message states the entire rule and its repair: panel 089's condition
(*"§1.4 redundancy the compiler pays back loudly"*), met as `aab44f9b` met it.

## The routes Q1 lists to be refused, priced (between 00:26 and 00:31)

The same method. These are the sentences each route would **owe** the spec,
because each makes § 1's *"comments may contain any UTF-8"* or design.md
§1.10's *"Strings and comments are full UTF-8"* untrue as written.

| draft | route | the text | legacy | cl100k |
|---|---|---|---|---|
| c1 | a bad byte accepted in a comment, refused in a string | *comments may contain any bytes, strings any UTF-8 but a raw carriage return ...* | +1 | +1 |
| e1 | an encoding declaration (PEP 263's route) | the base, then *A first line `# coding: latin-1` names another encoding.* | +16 | +17 |
| e2 | transcoding with no declaration | the base, then *A file that is not UTF-8 is read as Latin-1.* | +16 | +17 |

Run for e2 (in the same interval, Python's codecs): the byte 0x80 is `€` in Windows-1252 and
U+0080, a control character, in Latin-1; a UTF-16 file with its mark read as
Latin-1 begins `ÿþx` with a NUL after every letter. So a fallback turns a
Windows-1252 string into **a different string with no error**, the
*"different but valid"* program design.md §1.4 exists to prevent.

## The state, from both sides: what `read_file` calls `not_text` (between 00:26 and 00:31)

Q1 asks whether the compiler's `not_text` and the program's are one state. The
program's half can be measured today, so I measured it: eighteen files under
`<scratchpad>/189-spec-warden-cases/q1-set/`, each read by a Heroes program of
seven lines (`../reader/main.hero`, `read_file` and `e.code`, built by my
compiler), and the same files through today's `heroes check`.

| case | the bytes | `read_file` | `check` today |
|---|---|---|---|
| a to f | F1's six: Latin-1 in a comment and in a string, 0xFF, a lone 0x80, a cut 0xC3 at the end, 0xE9 first | `not_text` | 2, *cannot read* |
| g | Windows-1252 `€` (0x80) in a string | `not_text` | 2, *cannot read* |
| h | overlong `C0 AF` | `not_text` | 2, *cannot read* |
| i | a surrogate, `ED A0 80` | `not_text` | 2, *cannot read* |
| j | past U+10FFFF, `F4 90 80 80` | `not_text` | 2, *cannot read* |
| p, q | UTF-16 LE and BE, with their mark | `not_text` | 2, *cannot read* |
| k | a valid U+FFFD in a string | ok | 0 |
| l | a valid U+FFFF in a string | ok | 0 |
| m | a NUL in a string | ok | 0 |
| n | a UTF-8 byte-order mark | ok | **1**, `unexpected_character`, *the invisible character U+FEFF is not part of the language's syntax*, 1:1, no fix, **and a second message** for the same mistake, `unexpected_block` at 2:1 |
| o | `café`, valid | ok | 0 |
| **r** | **UTF-16 LE, no mark, ASCII text** | **ok** | **1**, **30 messages**: 29 `unexpected_character`, *the control character U+0000 ...*, one per NUL, and one `expected_declaration` |

Two things here the briefs do not carry:

- **Row r is well-formed UTF-8.** UTF-16 without its mark over ASCII text is
  each letter followed by 0x00, and 0x00 is UTF-8. So it is not `not_text` to
  a program, it will not be `not_text` to the compiler under the proposal's
  own definition (*the first byte that is not UTF-8*), and today it costs the
  author thirty messages, none naming UTF-16. F8's *"UTF-16, with a
  byte-order mark or without, is cannot read"* holds for the critic's case
  (0xE9 at line 2) and not for an ASCII one. Not 227's cause; a shape beside,
  for Q6 and Q2.
- **Row n's message never says "byte-order mark"** and offers no fix, and a
  second diagnostic follows it. Not 227's cause either (the critic's table
  says so), but it matters to Q5 below: it is where a writer lands who obeys
  a sentence saying "UTF-8" with a tool whose UTF-8 writes a mark.

**`heroes measure` on a file that is not UTF-8** (in the same interval): a copy of the spec
whose first `·` is saved as the Latin-1 byte 0xB7 reads *error: cannot read*,
exit 2, **no count printed**. Today the budget's instrument never counts a text
the reader would not receive.

## Q5. Is a sentence owed? (written from 00:33)

**The two standards** (my brief; panel 188 R10 weighed the same two). Panel
055's, carried by `aab44f9b` (2026-09-05): *"Neither may be an absolute path."*
left the spec because the message *"states the entire rule"*, which that
commit calls *"§1.4 redundancy the compiler pays back loudly, which is panel
089's shape"*; panel 089's own words are a removal *"redundant under §1.4
because the compiler is loud in both directions"*. Panel 181's, its
spec-warden: *"the refusal can be inferred, but nothing forces the inference:
a rule a reader can miss, which is the case for stating it"*, taken under the
author's instruction of 2026-09-28 (`345f167b`, *the most robust and solid
route, even at the cost of the spec's tokens*), and still paid by a removal.

**The test my seat wrote at panel 188 to reconcile them**, from design.md §1.4
(line 235, *"redundancy is spent deliberately, where errors actually occur,
and nowhere else"*): *a refusal of what the writer writes, in a shape a reader
can plausibly reach from the document, is forced by the document; a refusal of
a shape nobody writes, or of what the machine answers, has its home in the
message.* **R10 did not adopt that test as such** (by it, a6 was owed): it kept
F4's standard on two grounds of its own, that the error §1.4 would spend
redundancy on did not occur on today's document (measured, 0 of 13 sessions
bracketed a header's name) and that §1.0's burden binds an addition whatever
pays for it. The author ratified R10 yesterday, as a reading. Both of R10's
grounds are applied below, and my seat's test beside them.

**Applied here**:

1. **The rule is already in the document**, by two clauses and a type: § 1
   *"Syntax is ASCII-only; comments may contain any UTF-8, strings any but ..."*
   and § 3 *"`str` | immutable UTF-8 string"*; design.md §1.10 (line 460):
   *"Strings and comments are full UTF-8"*. Every byte of a valid file is ASCII
   or inside a comment or a string, so a file is UTF-8 by derivation. What the
   document does not say is what a file that is not gets, and the spec's shape
   is to name no compiler code at all (above: 0 of 187).
2. **Nothing in the document leads a reader to another encoding.** A file that
   is not UTF-8 is what a TOOL writes: a code page, an editor's setting, a
   shell's default. That is the machine's answer, the message's half of the
   test. The same holds for every row of my table: none of a to j, p or q is a
   shape a reader composes from the text.
3. **§1.0's burden binds a sentence whatever pays for it** (design.md lines
   379 to 381, *"every addition still carries §1.0's burden of proof ...
   compiler-need or a measured thesis effect"*), which is R10's second
   ground. Nothing measures a thesis effect: 0 of 1,910 tracked `.hero` files
   are outside UTF-8 (F8), no generation run exists, and the blind seat's arms
   measure the repair after a message, not the first file written. Compiler
   need: none, the compiler's own sources being UTF-8. **R10's first ground is
   unmeasured here**: it rested on a run (0 of 13 sessions bracketed the
   name), and this sitting's counterpart, whether a writer leaves a file that
   is not UTF-8 on today's document, is P4 below, unrun. The 1,910 files say
   little about it, because the tool that wrote them writes UTF-8. So my
   verdict rests on §1.0's burden and my seat's test, not on a measured
   absence of the error, and P4 is what could overturn it.
4. **A sentence cannot be both cheap and complete, measured on the drafts.**
   u3b (+4, *"the file UTF-8"*) makes the file's encoding something the reader
   decides, and the option beside "UTF-8" in a Windows tool is UTF-8 with a
   byte-order mark (recalled, unrun by me: the ffi-pragmatist's item 1
   measures what PowerShell's `Out-File`, `>`, `Set-Content` and Notepad
   write). That file is refused by row n's message, which never says
   "byte-order mark", offers no fix and adds a second message. So a sentence
   that sends a reader toward UTF-8 creates a reachable mistake it must then
   state: the complete sentence is u5, **+13 and +14**, more than three times
   u3b. And row r, UTF-16 with no mark over ASCII text, is well-formed UTF-8:
   no sentence about UTF-8 reaches it at all. Each of these shapes is told
   best by a message that fires exactly when it occurs and names the bytes
   found.
5. **Panel 089's condition is unmet today and met only by the repair.** Today
   the compiler is loud and false: *cannot read* at exit 2 for a file it read,
   *not there* for a module that is there. The message is the rule's home only
   if it states the rule: that a `.hero` file is UTF-8, which byte at which
   line and column is not, and the repair.

**Verdict on Q5: no sentence (u0), 0 spec tokens, on F4's and R10's
standard**, on the condition in 5. **The sitting's other reading, recorded for
the author**: if the synthesis takes panel 181's standard under the
instruction of 2026-09-28, the sentence is **u5 paid by r1, -9 legacy and -8
cl100k** (lower bounds), the rule with its padded shape, re-verified removal
and all:

    - A file is UTF-8 with no byte-order mark, and its syntax ASCII-only; comments
      may contain any character, strings any but a raw carriage return or line end:
      a string is one line.

(byte for byte as priced, `drafts/out/u5+r1.md`, whose other change is § 13's
*"asks the system where its headers and libraries are and what else it
needs."* ending the package paragraph), and **u3b+r1, -18 and -18**, only if
the ffi-pragmatist measures that no Windows writer's UTF-8 option writes a
mark:

    - Syntax is ASCII-only, the file UTF-8; comments may contain any character,
      strings any but a raw carriage return or line end: a string is one line.

Not u4, u4b or u6: stating the consequence (*"is a compile error"*) costs +15 to +20 where the rule merged
costs +3 to +5, and the spec states its other byte rules (*"Syntax is
ASCII-only"*) as rules, leaving the refusal to the compiler.

## Q1 from my seat: one word for one state, or two meanings

**One state, measured from the program's side.** The compiler is a Heroes
program and reads a source file with its own `read_file`, which already
answers `not_text` (`selfhost/library_source.hero:217`, *"the bytes of " +
path + " are not UTF-8"*) for exactly the twelve files of my table that are not
UTF-8 (a to j, p, q) and reads the six that are (k to o, r). A diagnostic
`not_text` passes on the code the compiler was handed, for the same bytes; the
program's other two producers (`validated`, `validated_bytes`) name the same
state for bytes from C.

**It is a decision, not a convention, and design.md does not cover it.** The
two namespaces share no word today (my enumeration above, 187 printed codes
against 8 program codes), so `not_text` would be the first. design.md §4.6
(line 1178, *"Errors carry a stable code plus a human message"*) governs a
program's codes and §4.17 a diagnostic's content; **neither section says
whether a diagnostic may take a program's code**, so I say so rather than
stand on a section.

**What the spec says of either.** Of the program's `not_text`, one sentence,
§ 13 (*"`f.validated_bytes()` does the same ..., and either fails
`not_text`"*), for bytes from C, the state left to the word itself; § 11 gives
`read_file` its type and no code. Of a compiler code, nothing, by the
document's shape: none of the 187 is in it. So the shared word costs the spec
**0 tokens and asks no sentence**. From the spec's own sentence, `not_text` is
also the right half: § 1's bullet has an ASCII half (syntax) and a UTF-8 half
(comments and strings); `unexpected_character`'s *"not part of the language's
syntax"* names the first, and a byte that is not UTF-8 inside a comment breaks
the second.

**Where it would become two meanings, concretely**: if the compiler's
`not_text` fires on any file `read_file` reads, a valid U+FFFD marked as
replaced (F8's warning), a byte-order mark, a NUL, or row r, the compiler's
word means *"not UTF-8, or something else"* and the program's means *"not
UTF-8"*. P2 below is that condition as a measurement.

**The class, from the spec's side** (Q1's last part, a seat's reading): not a
thesis rule. The refusal follows from what § 1 and § 3 say a file and a `str`
hold, like `unexpected_character` and `raw_carriage_return`, which
`is_thesis_rule` (`selfhost/diag.hero`, 19 codes) leaves out; Part 11's
control arm disables *"the thesis-bearing checks"* (design.md line 3993), and a
control arm that dropped this one would compile U+FFFD where the author's byte
was: a different program than written.

## The refused routes, held to a feature's standard (CLAUDE.md § 12)

| route | verdict | spec tokens (lower bounds) | what would make the refusal wrong |
|---|---|---|---|
| c1, a bad byte accepted in a comment | **object**, not on budget: it makes design.md §1.10 (*"Strings and comments are full UTF-8"*, Part 1, a panel's) false, and its cost lands in every writer, which must then carry bytes no `str` holds (F8) | +1 and +1 | a measured population of writers whose only bad bytes are in comments, for whom one re-save costs more than that path |
| e1, an encoding declaration | **veto**: §1.0's burden unmet for a new form (no compiler need, no measured effect), and one program in several byte spellings, against §4.15's *"exactly one correct way to write any program"* (line 2029), the ground Part 6's row on style-insensitive identifiers already stands on | +16 and +17 | a measured writer population that cannot produce UTF-8 at all |
| e2, a Latin-1 fallback | **veto**: §1.4, run above: Windows-1252's `€` becomes U+0080 with no error, a different valid program; and §1.0's burden for the rule it adds | +16 and +17 | none on tokens; only every non-UTF-8 file of a measured population being Latin-1 exactly, which rows g, p and q already refute |

## My own instrument under the proposal

`heroes measure` is one of the readers (F2, `cli/measure.hero:44`) and the
instrument §1.6's number lives in. Today it prints no count for a file that is
not UTF-8 (run above). **Condition**: under any route that reads with a
replacement (Q3), `measure` prints no count and no digest for such a file and
`--refresh` sends nothing, so the binding number can never be pinned for a
document the reader would not receive. A count of a lossy text is not a
measurement of the document, as a count without its model id and date is not
one either (`spec/real`'s own words).

## The payment each draft owes (design.md §1.6, lines 312 to 327)

| draft | adds | payment owed | what pays |
|---|---|---|---|
| **u0, recommended** | nothing | **none**: no amendment | the spec does not move |
| u5+r1, the other reading | the rule with its padded shape, +13 and +14 | a named removal, measured in the commit that spends it | **r1**, -22 and -22, re-verified by running (above); and §1.0's burden still unmet, which is why it is not my recommendation |
| u3b+r1 | the rule, +4 and +4 | the same | r1 |
| u4, u4b, u6 | the consequence, +15 to +20 | the same | r1 covers it; not recommended (the spec states byte rules as rules) |

**No prediction pays for a sentence here.** §1.6 admits a prediction as payment
only if it names an instrument that exists today and the milestone at which it
is scored. What a sentence would change is the first file a writer saves, and
the instrument that measures that is a generation run (P4 below), which is
not on §1.6's list. So a sentence, if adopted, is paid by r1 or not at all.

## Predictions

No milestone is open (`docs/ROADMAP.md`: row 63, M-buildable-structs,
`scheduled`; batch 8, 27 defects in four lanes). So each prediction below is
**scored at the round's gate that lands defect 227** (batch 8's, or the first
after this sitting's ratification) and is marked `lapsed` and re-decided if
unscored by **the next `m-*` tag** (design.md §1.6, line 329).

- **P1 (registered; `heroes measure`)**: `heroes measure
  spec/heroes-spec.md` reads `claude-legacy` 6990, `cl100k_base` 7117 and the
  real pin 9392 on the parent of 227's first commit and on its last: **0
  tokens**. Falsified by any sentence about encodings landing with 227
  without a sitting's text.
- **P2 (registered; a compile, `heroes check`, with the seven-line reader
  `read_file` as the second judge)**: over the eighteen files
  `<scratchpad>/189-spec-warden-cases/make_q1_set.py <dir>` writes (verified
  byte for byte against the set measured above, and Python's strict UTF-8
  decoder agreeing with `read_file` on all eighteen), the compiler that lands
  227 tells `error[not_text]` at exit 1 on **exactly the twelve** `read_file`
  answers `not_text` (a to j, p, q), and on **none** of the six it reads: k,
  l, m and o at exit 0, n and r at exit 1 with no `not_text` among their
  messages. Falsified by any file outside its row; a failure is the *two
  meanings* of Q1 above, and my approval of the shared word goes with it.
- **P3 (registered, only if the synthesis takes the other reading;
  `heroes measure --refresh` at its landing)**: u5+r1 reads **between -18 and
  -1** against 9392 on `claude-opus-5`; u3b+r1 **between -28 and -14**.
  Basis: the vendored deltas and the ledger's ratios above, so the ranges are
  an inference and the score is the measurement.
- **P4 (an observation, paying nothing until run)**: ten fresh sessions, each
  given today's spec and the Windows box's PowerShell, asked to write and
  `check` a program whose comment holds `é`, leave a file that is not UTF-8 in
  **at most 1**. A paid run, the coordinator's to authorise; unrun, and its
  size is ten short sessions. It is the measurement that would make a
  sentence owed (below).

## The condition that would change my verdict

- **A sentence becomes owed (u5+r1)** if: the adopted message does not state
  the rule and its repair (panel 089's condition fails, so the message is not
  the rule's home); or P4, run, reads 2 or more of 10 (a measured effect, so
  §1.0's burden is met), under either standard; or a later change to the spec
  rewrites § 1's UTF-8 clause or § 3's `str` row, so the derivation in Q5's
  point 1 no longer holds (then the sentence travels in that same commit).
- **u3b instead of u5** within the other reading if the ffi-pragmatist
  measures that no Windows writer's UTF-8 option writes a mark.
- **The shared word `not_text`**: I withdraw approval and object if P2 fails;
  then a distinct code, or the compiler's set repaired to `read_file`'s.
- **`measure`**: I object to any route under which `heroes measure` counts,
  or `--refresh` sends, a text with replaced bytes.
- **The vetoes on e1 and e2** lift only on a measured population of writers
  who cannot produce UTF-8 at all.

## Byproducts for the coordinator, filed by nobody here

- **Row n**: a UTF-8 byte-order mark at a file's start is told *the invisible
  character U+FEFF is not part of the language's syntax*, never *byte-order
  mark*, with no fix and a second message (`unexpected_block` at 2:1).
  `adjacent` by its class (a second message for one mistake; a true message
  less exact than it could be); not 227's cause. `grep` of `docs/panel/` and
  `docs/records/log/`: no sitting has ruled on a mark at a file's start (panel
  188 names U+FEFF only inside a header's string), and `docs/work/DEFECTS.md`
  holds no item for it.
- **Row r**: UTF-16 LE with no mark over ASCII text is well-formed UTF-8, so
  `not_text` never sees it; `check` exits 1 with **30 messages** (29
  `unexpected_character` for U+0000 and one `expected_declaration`), none
  naming UTF-16. `adjacent`; not 227's cause, a shape for Q2 and Q6. F8's
  *"UTF-16, with a byte-order mark or without, is cannot read"* holds for the
  critic's case (0xE9 at line 2) and not for an ASCII one.
- **r1 stays removable on its own** by the standard R10 kept (its message
  states the whole rule, re-verified above). Not this sitting's question;
  noted so the other reading, if taken, spends it once and a later sitting
  knows whether it is still there.
- U+202E and the other bidirectional controls are accepted in comments and
  strings (exit 0), as § 1 says they may be; whether that is wanted is not
  this sitting's question (panel 188 refused them in a header's string only).

## What I ran, what I did not, and my cost

Ran, in my copy and my cases folder only: one compiler built from the seed,
one seven-line program built and run, `heroes check` over 35 hand-made programs,
`heroes measure` over the spec and 28 copies of it, and the suites that read the
spec, `spec` six times and `special` and `grammar` five times each, one
harness process at a time, the base restored and proved by `cmp` after each
draft. Not run: any paid session, `heroes measure --refresh`, any container,
the Windows box (what PowerShell and Notepad write is the ffi-pragmatist's item
1, and Q5's point 4 leans on it unrun), the historian's precedents (that Go,
Rust and Zig take no encoding declaration is my recollection, design.md §1.10
naming only their choice of UTF-8). No timing. Nothing removed. I did not read
the other seats' reports; the critic's first pass over the briefs I read as an
input.

**A fault of mine, corrected before this report was finished**: seven times in
it were first written without a `date` reading beside them (the baseline, the
`r1` run, the refused routes, the codec run, the state table, the `measure`
run and the run of the shapes beside § 1's sentence); at 00:34 each was
replaced by the interval two readings bound. And one sentence said R10
*applied* my seat's test; it did not, and the paragraph now says what R10
did.

## The structured verdict

- `verdict`: **approve** the refusal at **0 spec tokens**, no sentence (u0),
  not provisional (the document does not change); **approve** `not_text` as
  one word for one state, on P2; **object** to c1; **veto** e1 and e2. The
  other reading (u5+r1) is priced provisional: vendored lower bounds only.
- `section`: design.md §1.6 (the payment rule, lines 312 to 327; §1.0's
  burden for every addition, lines 379 to 381), §1.4 (line 235), §1.10 (line
  460), §4.15 (line 2029). On Q1's shared word **design.md does not cover
  it**: §4.6 (line 1178) governs a program's codes and §4.17 a diagnostic's
  content, neither its name.
- `spec_token_delta`: u0, 6990 and 7117 vendored and 9392 real before and
  after: **0**. The other reading, u5+r1: 6990 to 6981 legacy and 7117 to
  7109 cl100k, **-9 and -8, lower bounds**; real unrun.
- `removal`: none owed by u0. For the other reading, r1 (§ 13's
  package-refusal sentence), -22 and -22, verified by running.
- `needed_for_self_hosting`: no.
- `argument`: The refusal is a diagnostic, not a form: zero spec tokens,
  repairing a false message at exit 2, which robustness and truth rank above
  tokens. The rule is in the document (§ 1, § 3; design.md §1.10) and nothing
  in it leads a reader to another encoding: a tool writes one, which is the
  message's to tell. As at R10, §1.0's burden binds a sentence whatever pays,
  and no effect is measured. A sentence cannot be both cheap and complete:
  *the file UTF-8* (+4) makes the encoding the reader's choice, whose
  neighbour, a byte-order mark, meets a message that never names one;
  complete costs +14. `not_text` is one state: the compiler's own
  `read_file` already says it, for exactly these files.
- `prediction`: P1, 227's landing moves the spec by 0 tokens; P2, `not_text`
  fires on exactly the twelve files `read_file` refuses and none of the six
  it reads; both scored at the round's gate that lands 227, lapsing at the
  next `m-*` tag.
- `condition`: the message not stating the rule, or P4 at 2 or more of 10
  (u5+r1 owed); P2 failing (the shared word withdrawn); `measure` counting a
  replaced text (objection).
