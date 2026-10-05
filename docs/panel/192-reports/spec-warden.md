# Panel 192, the spec-warden: Q6, and Principle 0 for every sentence a route owes

Started 2026-10-04 18:16 (from `date`). Written as I go; a section marked
*in progress* is not finished.

**The ceiling, by grep, at the start of the sitting**: design.md §1.6
(`:255`) says *must fit in 10240 tokens, measured by `claude-opus-5` through
`POST /v1/messages/count_tokens`*. The payment rule (`:316-327`): a named
removal, measured in the commit that spends it, or a registered prediction
naming (i) the instrument that scores it and (ii) the milestone at which it is
scored, the instrument existing on the day of registration.

**My copy**: `<scratchpad>/192-spec-warden/`, from `git -C <trunk> archive
4c3524fb | tar -x`, its compiler built from the seed at 18:17:09 to 18:17:13:
`shasum -a 1 heroes` reads `8084f018f53875362048cc0230d23207976d8b00`,
`shasum -a 256 seed/heroes.c` begins `c79ffd5ad005c301`, `wc -c` reads
35,206,983. All three match `00-facts.md`'s base.

**The base count** (`./heroes measure spec/heroes-spec.md` from my copy's
root, 18:17:18): claude-legacy 6990, cl100k_base 7117, maximum 7117, real
9392 (claude-opus-5, pinned 2026-10-03, the binding number), headroom 848,
FFI floor 60. Same as F8. Every delta below is a **vendored-table delta, a
lower bound, in those words** (`.claude/rules/spec-shape.md`): `--refresh` was
not run, by the brief.

## 1. Is any sentence of `:33-36` or `:47-48` false of the compiler today?

**No**, on 71 probes of my own (18:20:48 to 18:20:49,
`<copy>/w/f1s/gen.py`, output `w/f1s/table.txt`), one claim per program,
each through `./heroes check --brief`, beside F1's 110 which the coordinator
and the critic ran:
- *Syntax is ASCII-only*: `é` as a name, NBSP and U+200B between tokens are
  `unexpected_character`, exit 1.
- *a tab is a compile error* (`:33-34`, the indentation bullet): a tab as
  indentation is `tab_in_indentation`, a tab between tokens `tab_in_line`;
  a tab in a comment or a string checks at exit 0, which `:35-36` permits.
- *comments may contain any UTF-8, strings any but a raw carriage return or
  line end*: 13 more code points (U+200C, U+200D, U+FE0F, U+E0001, U+E0067,
  U+3000, U+2000, U+202F, U+FFF9, U+1F600, U+FFFF, U+10FFFF, U+FDD0) check
  at exit 0 in both. A raw CR is `raw_carriage_return` in an f-string's
  text, a `test` title and a `match` pattern as well as a string; a raw ESC
  and a raw NUL check at exit 0 in each.
- *Six escapes, and no others*: the five in a string and the five in a
  character literal check; `\'` in a string or an f-string and `\"` in a
  character literal are `escape_not_needed`; `\x1b`, `\0`, `\{` and `\x`
  in a test title are `unknown_escape`.

**Where the spec is silent and the compiler decides** (not false, unstated):
- a byte that is not UTF-8, in a comment or a string: `not_text`, exit 1
  (*comments may contain any UTF-8* promises nothing about it);
- **what a character literal may hold**: a raw ESC, tab or NUL checks at
  exit 0, a raw CR is `raw_carriage_return`, `é` and U+202E are
  `char_literal`, whose message states design.md §4.3's rule (`:996`, *one
  ASCII character or one escape*) each time it fires. Panel 055's standard
  already governs that rule: the message, not the document.
- **A group head's string** refuses a raw ESC (`unshowable_name`) and a NUL
  (`unwritable_name`), against the letter of *strings any but a raw carriage
  return*. It is a refusal of the header's NAME, on its value: `extern
  "a\tb.h"`, an escape, is refused the same way. Panel 188 R10 ruled it has
  no sentence. So `:35-36` is about a string's spelling, and a refusal on a
  value is another rule.

**Found beside, already filed**: the `char_literal` message writes U+202E
raw, twice in the full form (headline and excerpt) and once under
`--brief` (`xxd`: `e2 80 ae`); a `mixed_arithmetic` excerpt over
`print("a<U+202E>b" + 1)` does the same. Defect 291 holds this shape
(*in an excerpt and a message*), so nothing new is filed.

## 2. The strongest reason proposal part 1 is wrong, as written

Proposal part 1 gives a string literal panel 188's predicate, which refuses
Unicode's whole Default_Ignorable set. That set holds the variation
selectors and ZWJ that ordinary emoji are written with. Run at 18:24 over
the base's group head (`w/f1s/cases/head-*.hero`), the predicate the
proposal would apply:

| text | code points | group head today | `print("<text> low disk")` today |
|---|---|---|---|
| WARNING SIGN, fully qualified | U+26A0 U+FE0F | `unshowable_name`, *the invisible character U+FE0F* | exit 0 |
| RED HEART, fully qualified | U+2764 U+FE0F | `unshowable_name` | exit 0 |
| HEAVY CHECK MARK, fully qualified | U+2714 U+FE0F | `unshowable_name` | exit 0 |
| family: man, woman, girl | U+1F468 U+200D U+1F469 U+200D U+1F467 | `unshowable_name` | exit 0 |
| flag of Scotland | U+1F3F4, five tag characters, U+E007F | `unshowable_name` | exit 0 |
| Persian *mi-khaham* | holds U+200C | `unshowable_name` | exit 0 |
| ROCKET | U+1F680 | exit 0 | exit 0 |

So the proposal as written refuses `print("<U+26A0 U+FE0F> low disk")`, a correct
program that runs today: `.claude/rules/verification.md` § Bounded
discovery calls *a correct program refused* `blocking`. Under design.md
§1.2 it is the worst kind of rule: the writer cannot see the U+FE0F it
wrote, since it arrives inside the emoji, so **no sentence of the spec can
lower the rewrite rate it causes**. Stating *drawn as nothing* in `:35-36`
does not tell a model that the warning sign it writes holds such a character. What pays for a
refusal here is the list, not the document: a variation selector, ZWJ and
a tag character after a base character they modify are real text.
**Unrun**: how often a model writes the fully-qualified warning sign; a question for
a blind run, not a premise. That the compiler's own tests would not see it
is measured: F6's scan finds no U+FE0F, U+200D or U+E0001 in the tracked
corpus.

*Stopped by the account's session limit at about 18:30, resumed at 21:07:53
(`date`); the coordinator's message gives the open-milestone fact: no
milestone is open, so "the next milestone tag" is the next `m-*` tag, placed
only over zero `blocking` and zero `systemic` items. Everything below was run
from 21:07 on, in my copy, unchanged since 18:30.*

## 3. The drafts and their prices (Q6)

`<copy>/w/drafts/price.py` applies each draft to the base spec as exact
replacements, each anchor asserted to match once, writes the result to
`w/drafts/out/<name>.md` and runs `./heroes measure` on it from my copy's
root (21:08:03 to 21:08:12). **Every delta is vendored, a lower bound, in
those words**: legacy, cl100k and their maximum against the base's 6990,
7117 and 7117. The real instrument reads the base 32% above the vendored
maximum (9392 against 7117), so a real delta is likely larger than the one
written here; that is an inference, not a measurement. The three blind arms'
sentences reproduce the ergonomist's brief exactly: R +20, B +35, C +30.

### Site 1, `:35-36`: what a string and a comment hold

Base: *Syntax is ASCII-only; comments may contain any UTF-8, strings any but
a raw carriage return or line end: a string is one line.*

| draft | route | the words that change | dL | dC | dMax |
|---|---|---|---|---|---|
| s1-ctl | Q1: C0, DEL, C1 | *strings any but a raw control character: a string is one line* | -3 | -3 | **-3** |
| s1-ctl-sep | Q1: and U+2028, U+2029 | *... a raw control character, U+2028 or U+2029* | +6 | +7 | +7 |
| s1-ctl-sep-bidi | Q1: and the bidirectional controls | *... a raw control character, bidirectional control, U+2028 or U+2029* | +10 | +13 | +13 |
| s1-188 | Q1: panel 188's list | *... a raw control character, U+2028, U+2029 or a character drawn as nothing* | +13 | +15 | +15 |
| s1-unseen | Q1: `UNSEEN` | *... U+2028, U+2029, a space other than ` ` or a character drawn as nothing* | +20 | +22 | +22 |
| s1-q2 | Q2 alone | *comments may contain any UTF-8 but a bidirectional control, U+2028 or U+2029, strings any but a raw carriage return or line end* | +14 | +17 | +17 |
| s1-q1q2-merged | Q1 controls, separators, bidi; Q2 bidi, separators | *comments and strings may contain any UTF-8 but a bidirectional control, U+2028 or U+2029, and a string no raw control character* | +13 | +16 | +16 |
| s1-q1-188-q2 | Q1 188's list; Q2 | the same, *... and a string no raw control character or character drawn as nothing* | +18 | +21 | +21 |
| s1-055 | **the message standard**: true under every route | *Syntax is ASCII-only, but comments and strings may hold UTF-8: a string is one line.* | -9 | -9 | **-9** |
| s1-055-any | the same, Q2 none | *comments may contain any UTF-8, strings UTF-8 too* | -5 | -5 | -5 |
| r-cr | the CR clause alone removed | *comments and strings may contain any UTF-8: a string is one line* | -9 | -9 | -9 |

### Site 2, `:47-48`: an escape

| draft | route | the words | dL | dC | dMax |
|---|---|---|---|---|---|
| s2-b-arm | (b), arm B verbatim | *Seven escapes ... and `\u{...}`, a code point in hex that is not 0 or a surrogate (`\u{e9}` is `é`), in a string, `\'` instead of `\"` in a character literal* | +33 | +35 | +35 |
| s2-b-string | (b), a string only | *... and, in a string only, `\u{...}`, ... (`\u{1b}` is ESC)* | +33 | +35 | +35 |
| s2-b-ascii | (b), a character literal's `\u{...}` below `\u{80}` | *... where `\u{...}` stays below `\u{80}`* | +45 | +47 | +47 |
| s2-c-arm | (c), arm C verbatim | *... and `\xNN`, a byte in hex from `01` to `7f` (`\x41` is `A`)* | +29 | +30 | +30 |
| s2-d | (d) | *... and `\u{...}`, a code point in hex, for one it holds no other way (`\u{1b}` is ESC)* | +32 | +33 | +33 |
| s2-e | (e), a named constant | *`ESC` (the escape character, a `str`)* in the `Built-ins:` sentence | +13 | +13 | +13 |

Arm B's sentence, read as written, opens `\u{...}` in a character literal
too, by *`\'` instead of `\"`*: `'\u{e9}'` would be 233, which fits a `u8`,
so `s[i] == '\u{e9}'` checks and never matches the UTF-8 `é` (`c3 a9`). A
route (b) owes s2-b-string or s2-b-ascii, not arm B's words.

### Site 3, `:384`: a `[u8]` answers `validated_bytes()` (route (a′))

| draft | the words | dL | dC | dMax |
|---|---|---|---|---|
| s3-r-arm | arm R verbatim: *... for a field of bytes or a `[u8]` (`[104, 105]` gives `hi`)* | +19 | +20 | +20 |
| s3-r-bare | *... for a field of bytes or a `[u8]`*, no example | +7 | +7 | **+7** |
| s3-r-esc | *... or a `[u8]` (`[27]` gives ESC)* | +15 | +15 | +15 |
| s3-r-ptr | s3-r-bare and, at `:48`, *any other character is a `[u8]`'s `validated_bytes()` (section 13)* | +29 | +31 | +31 |

### Site 4: what `.cstr()` does with a NUL, and what a door answers

| draft | route | the words | dL | dC | dMax |
|---|---|---|---|---|---|
| s4-a | Q4 (a) or (b) | *`s.cstr()` lends a `str` to C, aborting if it holds a zero byte;* | +9 | +9 | +9 |
| s4-c | Q4 (c) | *... as a `cstr?`, failing `zero_byte` if it holds one;* | +18 | +18 | +18 |
| s4-d | Q4 (d) | the type row: *immutable UTF-8 string with no zero byte, indexed and measured in bytes* | +4 | +4 | **+4** |
| s4-d-rf | Q4 (d), read_file's failure said | s4-d and *(a file that is not UTF-8 or holds a zero byte fails `not_text`)* | +25 | +25 | +25 |
| s4-e | Q4 (e), today's truth said | *`s.cstr()` lends a `str` to C, which reads it to its first zero byte;* | +9 | +9 | +9 |
| s4-door | a door's failure said | *`write_file(...) -> ()?` (a path holding a zero byte fails)* | +9 | +9 | +9 |

### Site 5, `:324-325`: `args()`

| draft | route | the words | dL | dC | dMax |
|---|---|---|---|---|---|
| (none) | Q5 by WTF-8: the existing check refuses | unchanged | 0 | 0 | 0 |
| s5-text | platform-neutral | *one that is not text aborts* | -2 | -2 | -2 |
| s5-win | the sentence says Windows' substitution | *... aborts, but Windows hands a lone surrogate over as U+FFFD* | +15 | +15 | +15 |

### Removals measured

| draft | what | dL | dC | dMax |
|---|---|---|---|---|
| r1 | panel 188's r1: *A package answering with anything this compiler does not pass on is refused, naming what it said.* | -22 | -22 | -22 |
| r-cr / s1-055 | the raw carriage return's clause | -9 | -9 | -9 |

### Composites, merged where they can be

| draft | parts | dL | dC | dMax |
|---|---|---|---|---|
| proposal-as-written | s1-q1-188-q2, s2-b-string, s4-a | +60 | +65 | **+65** |
| route-a1-ctl | s1-ctl, s3-r-bare | +4 | +4 | +4 |
| route-a1-ctl-esc | s1-ctl, s3-r-esc | +12 | +12 | +12 |
| route-a1-ctlsepbidi-q2 | s1-q1q2-merged, s3-r-bare | +20 | +23 | +23 |
| route-b-ctl | s1-ctl, s2-b-string | +30 | +32 | +32 |
| route-c-ctl | s1-ctl, s2-c-arm | +26 | +27 | +27 |
| route-d-ctl | s1-ctl, s2-d | +29 | +30 | +30 |
| rec (the first round's guess, superseded by rec2 below) | s1-q1q2-merged, s3-r-bare, s4-d | +24 | +27 | +27 |
| rec-r1 | rec, paid by r1 | +2 | +5 | +5 |
| rec-055 | s1-055, s3-r-bare, s4-d | +2 | +2 | +2 |

**No draft comes near the ceiling**: the dearest, the proposal as written,
is +65 vendored against 848 of real headroom (788 once the FFI floor's 60 is
taken). No veto on the budget is in reach; my verdicts below rest on §1.2,
§1.6's payment rule and Principle 0.

### Second round (21:12 to 21:16): wordings that survive the list, and an example that checks

Two faults in the first round, each measured:
- **Every *any but X* wording is exhaustive**: it says everything outside X
  is allowed, so it turns false the day the list moves (a Unicode table
  refreshed, Q7) or the day Q2 refuses something in a comment.
- **Arm R's example does not check if copied.** Run at 21:09 on the base
  (`<copy>/w/a1/`): `print([104, 105].validated_bytes().must())` is
  `error[bad_operand]: validated_bytes takes a run of bytes, i8[N], u8[N] or
  [u8], found [i64]`, with **no fix line**; so is `b = [27]` then
  `b.validated_bytes()`. `hi: [u8] = [104, 105]` then
  `hi.validated_bytes().must()` checks, builds and prints `hi`, and the
  typed ESC program prints the target bytes `1b 5b 33 31 6d 45 52 52 4f 52
  1b 5b 30 6d 0a`. The compiler-engineer measured the same at 21:07:55 for
  `[1]`. So *`[104, 105]` gives `hi`* teaches a receiver that is refused.

| draft | the words | dL | dC | dMax |
|---|---|---|---|---|
| **s1-mix** | *Syntax is ASCII-only, but comments and strings may hold UTF-8; a string holds no raw control character and is one line.* | -3 | -3 | **-3** |
| s1-mix-any | *comments may hold any UTF-8 and strings UTF-8 too, but no raw control character: a string is one line* | +1 | +1 | +1 |
| s1-ctl-bidi9 | exhaustive: *... a raw control character, U+2028, U+2029 or a bidirectional embedding, override or isolate* | +15 | +18 | +18 |
| s3-decl | *... or a `[u8]` declared so* | +10 | +10 | +10 |
| **s3-typed-esc** | *... or a `[u8]` (after `b: [u8] = [27]`, `b.validated_bytes()` holds ESC)* | +31 | +31 | +31 |
| s3-typed-hi | arm R's example repaired: *(after `b: [u8] = [104, 105]`, `b.validated_bytes()` holds `hi`)* | +35 | +36 | +36 |

| composite | parts | dMax |
|---|---|---|
| rec2-bare | s1-mix, s3-r-bare, s4-d | +8 |
| rec2-bare-r1 | the same, paid by r1 | -14 |
| rec2-decl | s1-mix, s3-decl, s4-d | +11 |
| rec2-decl-r1 | the same, paid by r1 | -11 |
| rec2-esc | s1-mix, s3-r-esc, s4-d (only if a literal receiver checks) | +16 |
| rec2-esc-r1 | the same, paid by r1 | -6 |
| **rec2** | s1-mix, s3-typed-esc, s4-d | **+32** |
| **rec2-r1** | the same, paid by r1 | **+10** |
| rec2-typed-r1-s5 | rec2-r1 and s5-text | +8 |
| rec2-a | s1-mix, s3-typed-esc, s4-a (the lend aborts instead of (d)) | +37 |
| rec2-b-esc | s1-mix, s2-b-string, s4-d (an escape instead of (a′)) | +36 |

**The document's own instruments over the drafts** (21:12:52 to 21:14:47):
`./heroes run tests/harness/main.hero -- ./heroes spec` with each draft
copied over my copy's `spec/heroes-spec.md` in turn, then the base restored
(`cmp` against `git show 4c3524fb:spec/heroes-spec.md`: equal). The base
reads `spec: 21 passed, 0 failed`. Each of rec2-bare, rec2, rec-055,
s2-b-string, s2-c-arm, s2-d, proposal-as-written and s3-r-esc reads `17
passed, 4 failed`, and the four are `budget`, `spendable`, `real` and
`ledger`, the record checks every changed document fails until its count is
written. `shape`, `anchors`, `offered`, `named`, `rejected`, `inventory`,
`colouring` and the rest pass on all eight.

**And an instrument no escape route has**: nothing holds `:47-48` to the
compiler. `spec/colouring` holds the editor's TextMate grammar against
`escape_text`'s arms (`tests/harness/suite_spec.hero:1184` to `:1206`), and
`compiler_escapes` (`:1343`) reads single-letter arms, which a braced
`\u{...}` does not fit (the critic's section 5). So s2-b-string passes all
17 against a compiler that refuses `\u{1b}`: a spec sentence about an
escape is unwatched today. *Instruments first* (`.claude/rules/spec-shape.md`
§ How a change to the document is made) puts that work before any of (b),
(c) or (d) lands, and (a′) owes none.

**Calibration of vendored against real**, from the ledger's five newest rows
(`docs/measurements/010-spec-budget-ledger.md:209` to `:213`), cl100k delta
to `claude-opus-5` delta: +44 to +61 (1.39), -14 to -20 (1.43), +104 to +129
(1.24), -5 to -5 (1.0), +194 to +228 (1.18). Every one lies in 1.0 to 1.5
times; that is the band my token prediction uses (section 8).

## 4. Is a sentence owed at all? The standard each route meets

**The three precedents, read** (the brief's item 2):
- **Panel 055** (`aab44f9b`, 2026-09-05): *Neither may be an absolute path.*
  left the spec because the message states the rule each time it fires.
- **Panel 181** (2026-09-28, the warden): *a rule a reader can miss* is
  stated; the author's instruction that night (`345f167b`) took the longer
  text.
- **Panel 188 R10** reconciled them by its warden's design.md §1.4 test: *a
  refusal of what the writer writes, in a shape a reader can plausibly reach
  from the document, is forced by the document; a refusal of a shape nobody
  writes, or of what the machine answers, has its home in the message*; and
  then it went with the message because a blind measurement (P3, 0 of 13)
  showed the error did not occur, and §1.6 (`:379-381`) binds an addition
  whatever pays for it.

**`:35-36` is owed under any refusal, by a different rule**: it says *any*.
A refusal of a raw character in a string or a comment that `:35-36` permits
makes the compiler wrong by CLAUDE.md § 12 (*spec beats compiler*). So the
sentence must change; the standards decide only what it then says.

| route | owed? | the standard it meets | the cheapest true words |
|---|---|---|---|
| Q1, the controls (tab, ESC, every C0 but LF, DEL, C1) in a string | **yes, stated** | **181's**: today's `:35-36` itself leads a reader to a raw ESC (*strings any but a raw carriage return*), and F10's `unknown_escape` points the same way; a raw tab is a second spelling of `\t` a writer types | s1-mix, **-3** |
| Q1, U+2028, U+2029, the bidirectional controls in a string | the refusal, not a sentence | **055's**: nobody writes them on purpose (the compiler-engineer's lexer count: 1 in a string in 2,014 files, case 216's group head) | s1-mix stays true: 0 |
| Q1, Default_Ignorable's emoji characters | **neither** | no sentence can help: the writer does not see U+FE0F (section 2) | the list, not the document |
| Q2, a comment | the refusal, not a sentence | **055's**: 0 tracked comments hold any class (the compiler-engineer's lexer); the writer is an attacker, the reader a person | s1-mix drops *any*: 0 more |
| Q3, a spelling | **yes** | §1.2: a program needing ESC cannot be written from the spec alone without it, and today's message points elsewhere (F10) | s3-r-bare +7 to s3-typed-esc +31 |
| Q4 (d) | the type row | the invariant's one home, where `str` is defined; a reader cannot avoid what `read_file` meets in data, so its failure is **055's** | s4-d, +4 |
| Q4 (a), (b) | yes | spec-shape: *each site that aborts says so beside its operation* | s4-a, +9 |
| Q4 (c) | yes, and every lend changes | | s4-c, +18, and every lend in code: about 163 by the critic's approximate lexer, 28 of them in `docs/panel/` and `archive/` |
| Q4 (e) alone | it would state the defect | it describes 245 rather than repairing it | s4-e, +9 |
| the doors, a failure the program reads | no | **055's**: `read_file` and `write_file` are already `T?` | 0 |
| the doors, an abort | yes | a reader cannot see the prelude's own `.cstr()` | +9 at `read_file` |
| Q5 by WTF-8 or a strict conversion | no | `:324-325` becomes true on Windows | 0 (s5-text, -2, optional) |
| Q5 by changing the sentence | **vetoed below** | | s5-win, +15 |

**Answer to *is any sentence false of the compiler today*** (section 1):
no. The question is what becomes false: `:35-36` under any Q1 or Q2
refusal, and `:324-325` on Windows today (F9, carried, unrun here).

## 5. What design.md owes, route by route

- **The freeze, §4.3 `:1016-1018`**, names a panel as the way to open an
  escape. (b), (c), (d) and (f) owe its amendment by this sitting, saying
  which escape opens and why the NUL ground does not bite (the escape
  excludes 0, or Q4 (d) makes the invariant hold whatever writes the
  bytes). **(a′) and (e) leave it untouched.**
- **One spelling, §4.3 `:994-995`, and §4.15 `:2029`**: (a′) keeps both,
  since a refused character has no literal spelling and the run-time route
  is not a literal; refusing a raw tab **restores** one spelling of `\t`.
  (b) breaks both (`\u{e9}` against `é`, `\u{9}` against `\t`) unless (d)'s
  restriction or (f)'s formatter carries the canonical form; (c) breaks
  them for every ASCII character (`\x41` against `A`).
- **The character literal, §4.3 `:996`** (*one ASCII character or one
  escape*): under (b) `'\u{e9}'` is *one escape* and 233, which fits a `u8`,
  so `s[i] == '\u{e9}'` checks and never matches UTF-8's `c3 a9`. (b) owes
  `:996` the ASCII bound and the spec s2-b-string (+35) or s2-b-ascii
  (+47), never arm B's words, which open it by *`\'` instead of `\"`*.
- **§4.20's free lend (`:547`, `:2629`)**:
  - **(a) does not survive**: one scan per lend; *free with zero copies*
    becomes *zero copies, one scan*, and `:1016-1018`'s rationale loses its
    ground, since an interior NUL would abort at the lend instead of
    truncating in silence.
  - **(b) survives as O(1) at the lend**, at the price of a fact in the
    header `:2629` describes (*refcount and an 8-byte magic word*), kept
    by every constructor; design.md owes the field.
  - **(d) survives verbatim**: design.md owes the invariant (*no `str`
    holds a zero byte*) beside *Always NUL-terminated*, and `:1016-1018`
    restated as *an escape that can write 0 would break it*.
  - (c) and (e) keep the lend free and move the cost to every use site
    (c) or leave the truncation (e).

## 6. The class, as the spec sees it

**The spec states no thesis rule.** `grep -n -i -E
'thesis|permissive|warning|warn'` over `spec/heroes-spec.md` prints nothing;
the document says *compile error* or *refused* alike for a thesis rule
(`_` on a variant, `:227`) and for one without which a program has no
meaning (a tab, `:33-34`). **That is right and must stay so**: design.md
Part 11's control arm runs *the same model, same spec size* against `check`
and `check --permissive` (`:4019-4023`), so a spec that marked which rules
the arm drops would tell both arms which rules are optional. **Each answer
to Q1's and Q2's class costs the spec 0 tokens**; the cost lands in
`diag.hero`'s `is_thesis_rule` list and its comment (`:94-152`). Its own
test (*a rule the THESIS adds, as opposed to a rule without which the
program has no meaning*) reads a raw ESC as a thesis rule (`print` carries
it, so the program has a meaning), which is the compiler-engineer's call,
not a spec question. design.md Part 11's parenthesis lists five examples
against 22 codes in the list, so it owes nothing either.

## 7. Verdicts, one per question, in the seat's form

Every delta is vendored, a lower bound; so every verdict is **provisional**
on the real count, which only `--refresh` gives and which no seat runs.

### Q1. A string

- `verdict`: **object** to proposal part 1's list as written; **approve** a
  refusal of the raw control characters (every C0 but the line end that
  ends the string, DEL, C1), with U+2028, U+2029 and the bidirectional
  embeddings, overrides and isolates beside them. Provisional.
- `section`: design.md §1.2; §1.4; CLAUDE.md § 12 for `:35-36`.
- `spec_token_delta`: s1-mix 7117 to 7114, **-3**; the proposal's s1-q1-188-q2
  +21; s1-unseen +22.
- `removal`: s1-mix is a removal: *any* and the CR clause leave.
- `needed_for_self_hosting`: no for the refusal; and it lands only after
  `compile.hero:83` stops holding two raw U+0001 (F5).
- `argument`: The predicate the proposal borrows refuses U+FE0F, U+200D,
  U+200C and the tag characters, which the warning sign, the red heart, a
  ZWJ family, the flag of Scotland and Persian text are written with; all
  six check today in a `print` and are refused in a group head (section 2).
  The writer cannot see U+FE0F, so no sentence lowers that rewrite rate.
  The controls are the other case: a reader writes a raw tab or ESC, today's
  *any* invites it, and s1-mix states the class in three fewer tokens.
- `prediction`: P4 (section 8), an observation.
- `condition`: a list that spares the variation selectors, ZWJ, ZWNJ and
  the tag characters turns the objection into approval, and both of the
  historian's fetched precedents spare them (Rust's nine code points, Zig's
  controls). P4 at 0 of 5 would not turn it: a correct program refused is
  `blocking` whatever its frequency, and section 2 measured six.

### Q2. A comment

- `verdict`: **approve** a refusal of the bidirectional controls, U+2028 and
  U+2029 in a comment, at **0 tokens** beyond s1-mix; **object** to stating
  it (s1-q2, +17).
- `section`: design.md §1.4; panel 188 R10.
- `spec_token_delta`: 0 beyond s1-mix (s1-mix drops *any*, which a comment
  refusal would falsify).
- `removal`: none owed.
- `needed_for_self_hosting`: no.
- `argument`: No tracked comment holds any of these characters (the
  compiler-engineer's count with the compiler's own lexer). The writer is an
  attacker and the victim a person reading in an editor; a model writing from
  the spec never reaches them, so a sentence spends redundancy where no error
  occurs, as R10 ruled for a header. Whether the refusal is a thesis rule
  costs the spec nothing (section 6). A warning or a shown `<U+202E>` also
  costs 0 tokens; that neither reaches an editor is not a spec question.
- `prediction`: the landing round's census moves 0 tracked files by the
  comment refusal (the census, scored at that gate, by the next `m-*` tag).
- `condition`: a comment needing LRM, RLM or ALM in right-to-left text, in
  `examples/` or the closure list, would ask the list to spare those three;
  s1-mix stays true either way.

### Q3. Writing what is refused

- `verdict`: **approve (a′)**, the sentence s3-typed-esc (+31), or s3-r-esc
  (+15) if the compiler gives an array literal receiver of
  `validated_bytes()` the type `[u8]`. **Object** to arm R's words as the
  landed text. **Veto, on Principle 0, an escape's sentence (b), (c), (d) or
  a constant (e) on this record**, lifted by the condition below.
- `section`: CLAUDE.md § 2 (Principle 0); design.md §1.6 `:379-381`; §1.2;
  §4.3 `:994-996`, `:1016-1018`.
- `spec_token_delta`: s3-r-bare +7, s3-decl +10, s3-r-esc +15, s3-r-arm +20,
  s3-typed-esc +31; s2-e +13, s2-c-arm +30, s2-d +33, s2-b-string +35,
  s2-b-ascii +47.
- `removal`: r1, -22 (section 8).
- `needed_for_self_hosting`: **yes for (a′)**: the compiler's own tests use
  it 13 times in 9 files (`grep`, all in `## Tests` or `test` blocks), and
  its working code needs it once Q1 refuses `compile.hero:83`'s U+0001 (the
  compiler-engineer: `soh: [u8] = [1]` checks). No for every escape, since
  (a′) already writes every byte the compiler needs, on today's seed.
- `argument`: (a′) exists, writes every byte but NUL, keeps one spelling per
  character, and needs no seed, no freeze amendment and no instrument. An
  escape's increment over it needs a measured Part 11 effect, and this
  sitting's arm R cannot supply the comparison as run: its example,
  `[104, 105]`, is `bad_operand` with no fix if copied as the receiver. An
  escape also owes §4.3's freeze, one spelling, the character literal's
  ASCII bound (`'\u{e9}'` is 233 and never matches `c3 a9`) and an
  instrument holding `:47-48` to the compiler, which none does today.
- `prediction`: P3 (section 8), scored by this sitting's blind run.
- `condition`: the veto lifts for (b) or (c) if its arm beats R by the
  brief's rule (5 against at most 1) **and** R's failures are mostly the
  route not found (`unknown_escape` on `\x1b` and its kin) rather than the
  example's trap (`bad_operand`); or if a funded arm on s3-typed-esc scores
  at most 3 of 5. A B win lifts it for (d) too, whose `\u{1b}` is B's
  spelling of ESC; (e) was in no arm and stays vetoed.

### Q4. A NUL at the C boundary, and the doors

- `verdict`: **approve (d)** on my axis (s4-d, +4); (a) or (b) at +9 if (d)
  falls on capability; **object** to (c) and to (e) alone.
- `section`: CLAUDE.md § Precedence (robustness, rank 3, above Principle 0
  and tokens); design.md §4.20 (`:547`, `:2629`); §1.5.
- `spec_token_delta`: s4-d +4; s4-a +9; s4-e +9; s4-c +18; a door's failure
  0, a door's abort +9.
- `removal`: inside the composite, paid by r1.
- `needed_for_self_hosting`: not the question: robustness ranks above it.
- `argument`: (d) costs a reader nothing at the lend or the door: no `str`
  holds a zero byte, `.cstr()` stays free with design.md unchanged, and
  every door is safe by construction, enforced at the ffi-pragmatist's
  closed set of constructors (a literal, and `hero_str_from_bytes` with a
  stated length). Its price, `read_file` failing on NUL-separated data, is a
  failure through a `str?` the program already reads. (c) puts `?` or
  `.must()` at about 163 lends in code, §1.5 inverted. (e) alone writes
  defect 245 down instead of repairing it.
- `prediction`: s4-d reads +4 to +6 real at the landing (P1's band).
- `condition`: a program on the closure list or in `examples/` that must
  hold a NUL in a `str` (the ffi-pragmatist's to find) moves me to (a) or
  (b) with s4-a.

### Q5. `args()` on Windows

- `verdict`: **approve** making `:324-325` true on Windows, at **0 tokens**
  (s5-text, -2, optional); **veto s5-win**.
- `section`: CLAUDE.md § 12; design.md §1.6 `:379-381` (§1.0's burden).
- `spec_token_delta`: 0, or -2; s5-win +15.
- `removal`: none owed.
- `needed_for_self_hosting`: no.
- `argument`: A conversion that keeps a lone surrogate as its WTF-8 bytes
  lets the existing check refuse it: on this Mac `args()` over `78 ed a0 80
  79` aborts and `args_checked()` answers `not_text` (F9, the critic's run),
  so the sentence becomes true with no word, composing with panel 191's
  manifest. s5-win would write into the one trusted document that a Windows
  program reads `x<U+FFFD>y` for an argument it never received: a wrong
  value stated as a rule, with neither compiler need nor thesis effect.
- `prediction`: `:324-325` is unchanged at the landing commit (`git diff`).
- `condition`: none lifts the veto on s5-win; a Windows run showing no
  conversion reaches the check would leave Q5 open, not the sentence owed.

### Q6. The specification as a whole

- `verdict`: **approve rec2-r1**: s1-mix, s3-typed-esc, s4-d, paid by r1,
  **+10** (7117 to 7127), provisional; **robust**, per CLAUDE.md § 4. The
  conservative reading, recorded for the author: rec2-decl-r1, **-11**, with
  *a `[u8]` declared so* in place of the typed example. **Object** to the
  proposal as written: +65, and its list refuses correct programs.
- `section`: design.md §1.6 and its payment rule (`:316-327`).
- `spec_token_delta`: rec2-r1 +10; rec2 +32 without r1; rec2-esc-r1 -6.
- `removal`: r1, *A package answering with anything this compiler does not
  pass on is refused, naming what it said.*, -22; its message states the
  whole rule and the repair, run 21:19 (`error[ffi_package]`, section 8).
- `needed_for_self_hosting`: yes for s3's sentence, no for the rest.
- `argument`: Each part has its own reason: s1-mix keeps `:35-36` true under
  every list and every Unicode refresh, states the class a reader writes,
  and costs -3; s3-typed-esc states what the compiler already needs with
  an example that compiles if copied; s4-d is robustness's one row. The
  cheaper (a′) wordings save 21 to 24 tokens and leave the reader to find
  that `[27]` is `[i64]`; on CLAUDE.md's instruction of 2026-09-28 the
  spec's own tokens buy the robust route.
- `prediction`: P1 and P2 (section 8).
- `condition`: section 9.

## 8. The payment each draft owes, and the predictions

design.md §1.6 (`:316-327`): a named removal, measured in the commit that
spends it, or a registered prediction naming an instrument that exists today
**and** the milestone at which it is scored. No milestone is open, so *the
next `m-*` tag* is the latest scoring point every prediction below names
(the coordinator's message of 21:07).

**The removal I name: r1**, § 13's *A package answering with anything this
compiler does not pass on is refused, naming what it said.*, -22 on both
tables. Its premise run, not read (21:19:06, `<copy>/w/pkg/`): a `.pc` file
answering `-pthread` gives `error[ffi_package]: the package `badpkg`
answered with `-pthread`, which this compiler does not pass on`, a note
listing every flag accepted and why (*Go's CVE-2018-6574*), and a note
*name the library directly with `link` if you need it*. The message states
the whole rule and the repair each time it fires, and no sentence lets a
writer avoid what a package file answers: panel 055's standard, the one
`aab44f9b` applied, and panel 188's warden's §1.4 test (*what the machine
answers has its home in the message*). Panel 188 R10 recorded it as the
sitting's other reading and adopted neither it nor its a6.

| draft | dMax | owes | paid by |
|---|---|---|---|
| s1-mix, s1-ctl | -3 | nothing: a removal | itself |
| s1-055, r-cr | -9 | nothing: a removal | itself |
| s5-text | -2 | nothing: a removal | itself |
| r1 | -22 | its own reason | the run above |
| s1-ctl-sep, s1-ctl-sep-bidi, s1-188, s1-unseen, s1-q2, s1-q1q2-merged, s1-q1-188-q2, s1-ctl-bidi9 | +7 to +22 | a payment, and none is due: they state what nobody writes | **objected to** (section 4) |
| s3-r-bare, s3-decl, s3-r-esc, s3-r-arm, s3-typed-esc | +7 to +31 | a payment | r1; for s3-typed-esc the +10 r1 leaves, P2 |
| s4-d | +4 | a payment | r1, inside the composite |
| s4-a, s4-door | +9 each | a payment | r1 |
| s4-e alone | +9 | a payment | **objected to**: it writes defect 245 down |
| s4-c | +18 | a payment, and Principle 0 | **objected to** |
| s2-b-string, s2-b-ascii, s2-c-arm, s2-d, s2-e | +13 to +47 | **Principle 0 first** (section 7, Q3), then a payment | vetoed on this record; if lifted, r1 and a prediction on the arm that lifted it |
| s5-win | +15 | | **vetoed** |
| **rec2-r1** (the recommendation) | **+10** | the +10 r1 does not cover | **P2** |
| rec2-decl-r1 (the conservative reading) | -11 | nothing | r1 |
| if the escape veto lifts: alt-b-r1 / alt-c-r1 | +14 / +9 | the remainder | a prediction on the arm that lifted it |
| if (d) falls: alt-a-r1 | +15 | the remainder | P2 |

### The predictions

- **P1, tokens.** For whichever composite lands, measured alone on a copy at
  the landing as `.claude/rules/spec-shape.md` orders, the `claude-opus-5`
  delta lies in **1.0 to 1.5 times its vendored maximum delta**, rounded
  outward, sign kept: rec2-r1 **+10 to +15**; rec2 +32 to +48; rec2-decl-r1
  -11 to -17; r1 alone -22 to -33; s1-mix -3 to -5; s4-d +4 to +6. The band
  holds all five calibration points of section 3. **Instrument**: `heroes
  measure spec/heroes-spec.md --refresh` (exists). **Scored**: at the commit
  that lands this sitting's spec change, at the latest the next `m-*` tag.
  Falsified by a real delta outside the band.
- **P2, the rewrite rate, the payment for rec2-r1's +10.** With s3-typed-esc
  in the spec, **at least 4 of 5** fresh sessions of this sitting's blind
  protocol (`blind/brief.md` unchanged, `<scratchpad>/192-blind-score.py`)
  write a program that passes, by the `[u8]` route. **Instrument**: the
  protocol and its scorer, which exist and ran in this sitting.
  **Scored**: a run at the landing round, at the latest the next `m-*` tag;
  not run by then, it is `lapsed` and the +10 is re-argued under the
  removal branch (§1.6 `:329-331`). It is a paid run of five sessions, about
  0.80 to 0.90 USD at panel 189's 0.16 to 0.18 each, so the author's to
  fund. Falsified at 3 of 5 or fewer.
- **P3, this sitting's arm R, registered before I read its results** (none
  were in the trunk at 21:21). Every arm-R program that fails, fails at
  `check`, with `bad_operand` on an array literal or an unannotated binding
  as the receiver (the example's trap) or `unknown_escape` (the route not
  found), and none checks and prints the wrong bytes; and if R scores below
  4 of 5, `bad_operand` is the commoner of the two. **Instrument**: the
  scorer's record of each spelling and `heroes check` on each failing
  `c.hero`. **Scored**: at this sitting's synthesis. It pays for nothing; it
  decides how B and C are read against R (section 7, Q3).
- **P4, an observation, pays nothing.** Under proposal part 1's list, at
  least 3 of 5 fresh sessions asked to print a status line opening with a
  warning sign write U+26A0 U+FE0F in the literal, so the program is
  refused. The instrument exists (the blind protocol with a new brief); it
  is unfunded, the cap's remainder after 20 sessions at 0.24 being at most
  0.20 USD, one session.

## 9. For the synthesis: what the drafts depend on, and what I did not run

**What s1-mix's truth depends on.** *A string holds no raw control
character* is true only if Q1's refusal reaches every string the lexer
makes: a plain string (`literals.string`), an f-string's text
(`lex_interp.scan_piece`, a separate reader by the compiler-engineer's
section 2), a `test` title, a `match` pattern and a group head's string. One
left out makes `:35-36` false again. It never names a Unicode table, so a
refreshed `DEFAULT_IGNORABLE` (Q7) cannot falsify the document; every
exhaustive draft could.

**A character literal costs the spec 0 tokens either way**: `:46` claims
nothing about its content beyond three examples, and the `char_literal`
message states design.md §4.3 `:996`'s rule each time it fires (run 18:20).

**A route nobody listed, for Q3**: the context rule applied to
`validated_bytes()`'s receiver. Spec `:42` says a literal takes the type its
context asks for; whether a receiver that takes `i8[N]`, `u8[N]` or `[u8]`
asks for `[u8]` is the compiler-engineer's ruling, not mine. Adopted, it
makes `[27].validated_bytes()` and arm R's own example check, and s3-r-esc
(+15, rec2-esc-r1 -6) the robust wording. Refused, the typed example stays
owed. Either way `bad_operand` today carries no fix at all.

**The message a writer meets second** costs no token and is §1.2's second
reading: under (a′), `unknown_escape` must stop saying *a string holds its
characters as themselves*, which points at the refused raw character (F10),
and point at the run-time route instead.

**Instruments first, for any escape**: a check holding `:47-48` to
`escape_text`'s arms, which does not exist (section 3).

**When each sentence may land** (the spec never runs ahead of the compiler,
CLAUDE.md § 12): s3-typed-esc is true today and may land first; r1 removes
a true sentence and may land any time; s1-mix becomes true only in the
commit that refuses the raw controls in every string, so it lands there,
after `compile.hero:83` changes; s4-d lands in the commit that makes the
constructors refuse a zero byte. Each is measured alone first, then
merged, then `--refresh`, as spec-shape orders.

**What I did not run**:
- `heroes measure --refresh`, or any paid run: every delta here is a
  vendored lower bound, and P1 is the bridge to the real one.
- The blind experiment's results: not in the trunk when P3 was written; I
  read no other seat's copy.
- Any compiler change: s1-mix and s4-d are priced as sentences, against the
  base compiler's behaviour; their truth after landing is the builders'.
- Windows: F9 is the critic's run on this Mac and panel 191's report.
- Section 2's premise about models, how often one writes U+FE0F for a
  warning sign: unrun, P4.

**What would change my verdicts**, in one place:
- **The escape veto (Q3)** lifts if B or C beats R by the brief's rule and
  R's failures are mostly `unknown_escape` rather than `bad_operand` (P3),
  or a funded arm on s3-typed-esc scores 3 of 5 or fewer.
- **The objection to the list (Q1)**: the list sparing the variation
  selectors, ZWJ, ZWNJ and the tag characters turns it to approve; it
  cannot be turned by a sentence.
- **Q4**: a program that must hold a NUL in a `str` moves me from (d) to
  (a) or (b), alt-a-r1 at +15.
- **s5-win**: nothing; the 0-token route stands.
- **The payment**: if the synthesis declines to touch § 13, the whole of
  rec2 (+32), or of the conservative rec2-decl (+11), is owed by a
  prediction of P2's shape over the wording that lands; P1 is a calibration
  of the price and pays for nothing.

Finished at 21:22 (`date` read before this last write).

## 10. After the blind run (appended 2026-10-05, from 00:39 by `date`)

The sitting paused at 21:26 and this report was committed at `99a67630`; it
is appended to from here, every correction written underneath with its time.
Resumed 00:39:34. My copy is unchanged: `shasum -a 1 heroes` begins
`8084f018f5387536`, and its `spec/heroes-spec.md` equals the backup of
`4c3524fb`'s (`cmp`, 00:43:32).

### What the run measured, read and counted in this session

From `docs/panel/192-reports/llm-ergonomist.md` and its four arm files:
**20 of 20 pass**, every arm 5 of 5, each by its own route: A through C's
`putchar`, R by a `[u8]` and `validated_bytes()`, B by `\u{1b}`, C by
`\x1b`. The registered *A at most 1 of 5* is false.

Counted by me at 00:40 to 00:42, over the arm files in the trunk:
- **How R spelled it**: all five wrote a typed binding, `esc_bytes: [u8] =
  [27]` or `bytes: [u8] = [27]`, and each `reading` cites spec `:42`'s
  context rule (*a literal takes the type its context asks for ...
  otherwise `i64`*) for the annotation. None wrote `[27].validated_bytes()`.
- **Who reached the raw byte from `:35-36`**: sessions naming a raw 0x1B
  inside a string literal as an option (an `awk` over each session for
  *raw* beside *0x1b*, *ESC* or *27*): **A 3 of 5** (a2, a3, a4), **R 4 of 5**
  (r2, r3, r4, r5x), B 0, C 0. Every one turned it down as invisible or
  fragile. The ergonomist's *the five A sessions' `choice_points` name the
  raw byte* is three by this count: a1 and a5 read the escape list as
  closing every way to ESC in a literal.
- **Each program's own tokens**, Part 11's metric 1: the twenty `c.hero`
  extracted from the arm files into `<copy>/w/blind-programs/` and each run
  through `./heroes measure` (vendored maximum, lower bounds):

  | arm | tokens per program | mean | non-blank lines |
  |---|---|---|---|
  | A | 116 to 157 | 136.8 | 8 to 14 |
  | R | 46 to 66 | 51.4 | 4 to 5 |
  | B | 25 each | 25 | 2 |
  | C | 23 each | 23 | 2 |

  B's and C's check at exit 1 on the base, whose lexer has neither escape,
  as the scorer's substitution expects; A's and R's at exit 0. With the
  rewrite rate at 0 in every arm, §1.2's real cost of a program that writes
  ESC is its tokens: an escape halves it again after (a′) quarters it.

### Corrections, underneath what they correct

- **Section 3, second round** (*So `[104, 105]` gives `hi` teaches a
  receiver that is refused*), corrected 00:44: the example copied as a
  receiver is refused (measured 21:09), and **no reader copied it**: 0 of
  5, each deriving the annotation from `:42`. The hazard is real in the
  compiler and did not occur in five readings.
- **Section 7, Q3 and Q6** (*s3-typed-esc*, *rec2-r1*), withdrawn 00:44:
  arm R's own words (s3-r-arm, +20) scored 5 of 5 with no trap, so the 11
  tokens the typed example adds bought nothing measured. Measurement beats
  opinion (CLAUDE.md § 12), mine included.
- **P3** (section 8), scored 00:44: **vacuous**. No R program failed, so it
  decided nothing; the premise under it, that the example's trap would
  bite, is contradicted 0 of 5.
- **The veto on an escape** (section 7, Q3), withdrawn for (d) at 00:44.
  **My registered condition was not met** (B 5, R 5), and I say so plainly.
  It is withdrawn because it named the wrong instrument: Principle 0 asks
  for *a measured Part 11 effect*, and Part 11's metric 1 measured one,
  25 tokens against 51.4 at an equal first-try rate, where design.md §1.1
  makes tokens decide (*tokens win only when comprehension is
  indifferent*). And design.md §4.3's own panel 066 paragraph (`:1003-1007`)
  is the thesis argument: *the escape is what made the repair writable*; a
  refused raw ESC gets a `certain` fix only if a spelling exists that holds
  the same bytes, which (a′) cannot give in place. That framing error is
  mine. **Kept**: an objection, not a veto, to (b) unrestricted and to (c),
  on one spelling (§4.3 `:994-995`, §4.15 `:2029`): `\u{e9}` against `é`,
  `\x41` against `A`. **The veto on (e) stands**: no arm measured it, and a
  constant per character is an open list.
- **Section 2's** *Unrun: how often a model writes the fully-qualified
  warning sign*: still unrun; the run measured ESC alone.
- **"The next `m-*` tag"**: M-issue-files is the open row (83 of the
  chain), and the newest `m-*` tag is still `m-agreed-retention` (`git
  tag`, 00:40), so the next tag is that milestone's or a later one's.

### The verdicts as they stand at 00:44

- **Q1**: unchanged, and **strengthened**: s1-mix states the class that 7
  of the 10 readers without an escape reached from `:35-36` today.
- **Q2**, **Q4**, **Q5**: unchanged.
- **Q3**: **approve (d)**, s2-d (+33), as the robust route: one spelling
  kept, NUL unwritable, the refusal's fix `certain`, a refused character
  above ASCII checked at compile time instead of hand-encoded into a `[u8]`
  and found at run time. **The conservative route, recorded for the
  author**: (a′) in arm R's measured words, s3-r-arm (+20), on today's seed
  and with no compiler change. Under (d) no (a′) sentence is owed: it stays
  true, and the spec's silence on it is not a falsehood. (d) owes before it
  lands what section 3 and the compiler-engineer name: the instrument
  holding `:47-48` to `escape_text` (none exists), and
  `lex_interp.piece_text`'s premise *no escape's second byte is a brace*,
  which a braced escape breaks (the compiler-engineer's section 2).
- **Q6**: **approve rec4-r1**: s1-mix, s2-d, s4-d, paid by r1, **+12**
  vendored (7117 to 7129), provisional. **Conservative**: rec3-r1, s1-mix,
  s3-r-arm, s4-d and r1, **-1**, fully paid by the removal.

| composite | parts | dL | dC | dMax |
|---|---|---|---|---|
| rec3 | s1-mix, s3-r-arm, s4-d | +20 | +21 | +21 |
| rec3-r1 | the same, paid by r1 | -2 | -1 | **-1** |
| rec4 | s1-mix, s2-d, s4-d | +33 | +34 | +34 |
| rec4-r1 | the same, paid by r1 | +11 | +12 | **+12** |
| rec4-b-r1 | s1-mix, s2-b-string, s4-d, r1 | +12 | +14 | +14 |

Priced 00:43:32 to 00:43:33, `<copy>/w/drafts/price5.py`, lower bounds.

### The payment, as it stands

- **rec3-r1** owes nothing: r1's -22 covers it.
- **rec4-r1** owes +12 past r1, paid by **P7**, registered here:
  **at the landing round's gate, every one of F1's string code points the
  landed refusal refuses (`<scratchpad>/192-facts/cp<hex>-string.hero`) is
  repaired by `heroes check --apply` to an escape, and the applied program
  checks at exit 0, builds, and prints its literal's bytes unchanged**, 100
  per cent, compared byte for byte as the critic's F1 build run did.
  **Instrument**: F1's files, `check --apply`, `build`, `cmp`, all existing.
  **Scored**: at the landing round's gate, at the latest the next `m-*` tag.
  Falsified by one refused code point whose fix is not `certain`, or whose
  applied program fails, or prints other bytes. It is the thesis effect
  itself, a refusal whose repair a machine applies, and it costs no run.
- **P1** gains two rows: rec4-r1 **+12 to +18** real, rec3-r1 **-1 to -2**.
- **P6, optional and within the cap**: an arm D, the base spec with s2-d's
  one sentence, five sessions, at least 4 of 5 passing by `\u{1b}`. B's and
  C's sessions cost 0.1663 to 0.1789 USD each, so five cost at most about
  0.90 of the 1.0965 USD the author's 5 leaves after 3.9035. It would
  measure (d)'s own words, which no arm read; it is a paid run, so the
  coordinator's and the author's to decide, not mine to start.

### What would change these verdicts now

- **(d) to the conservative (a′)**: the builders measuring that a braced
  escape cannot be made safe in an f-string's piece scanner, or P6 run and
  scoring 3 of 5 or fewer.
- **(b) or (c) over (d)**: a measured case where (d)'s restriction refuses a
  program a reader writes from its sentence more often than once in five.
- Everything else as section 9 says.

Appended through 00:46 (`date` read before this write).

*Two corrections to the lines just above, 00:45 by `date`*: the closing
*Appended through 00:46* is a time I did not read; `date` read 00:44:13
before that write and 00:44:49 after it. And panel 066's paragraph is
design.md `:999-1008` at the base (`grep -n`, 00:44:57), not `:1003-1007`;
its last sentence, *the escape is what made the repair writable*, is
`:1008`.

*A correction to P7, 00:45 by `date`*: as written it covers U+0000, whose
raw form the refusal refuses and for which (d) gives no spelling by design
(`\u{0}` is refused, the NUL stays unwritable), so its fix cannot be
`certain`. **P7 reads**: every refused code point of F1's strings **but
U+0000** is repaired by `check --apply` as above; **and U+0000's refusal
carries no `certain` fix** and names the NUL. Falsified by either half.
