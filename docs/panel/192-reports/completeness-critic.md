# Panel 192, the completeness critic, second pass: over the reports

Started 2026-10-05 03:31 (from `date`). Written as I go. My role is the one
`/panel` gives it: name what is missing, never a verdict on the proposal. My
first pass, over the briefs, is `completeness-critic-briefs.md` beside this
file; the repaired briefs absorbed it (F10 to F12 are its findings).

Read: the repaired briefs (`00-facts.md`, `00-shared.md`, the five seats',
`blind/`), and the reports `compiler-engineer.md`, `ffi-pragmatist.md`,
`spec-warden.md`, `historian.md`, `llm-ergonomist.md` and
`llm-ergonomist-{a,r,b,c}.md`, as they stood when I read each (times below).

**My copies**: `<scratchpad>/192-critic/` (the first pass's, its compiler
sha1 `8084f018f5387536`, the base's) and `<scratchpad>/192-critic-p2/`, made
for this pass: `base/` a fresh `git archive 4c3524fb`, `blind/` the twenty
scored programs copied out, `ce/` a copy of the compiler-engineer's final
compiler and runtime (`heroes-ce`, sha1 `744c3197848b79d3`, the one its
section 15 names). Nothing was built or run inside a seat's copy.

**First, two sentences of my own first pass that the seats measured false**:
- *"`[1].validated_bytes().must()` keeps the cache key's bytes on today's
  seed"* (my section 4, question 7). I had run `[27]` through a typed
  binding only. An array literal as a receiver is `[i64]`, so it is
  `bad_operand` (the compiler-engineer's section 3, 21:07:55; the
  spec-warden's section 3, 21:09). The route needs `soh: [u8] = [1]`.
- *"After Q1, a program's NUL comes from `read_file`"*, which I sent on as a
  question. The ffi-pragmatist measured a third door: a binding's own C
  returning `str` through `hero_str_from_bytes` with a length (its *Where a
  program's `str` gets a NUL*), and the literal, as I had said. It also
  found the harness holding NULs in a `str` by design (`shell.hero:30-32`,
  `:608`, `:626`).

## 1. Claims asserted and not measured

- **"Route (d) keeps one spelling per character"** (the compiler-engineer's
  Q3 verdict; the spec-warden's Q3 at 00:44, *one spelling kept*). Run on
  the compiler-engineer's own compiler at 03:36:57
  (`192-critic-p2/ce/sp/`), `print("a<X>b")` with each X:
  - `\u{1b}`, `\u{1B}`, `\u{01b}`, `\u{001b}`, `\u{0001b}` and
    `\u{00001b}` all check at exit 0;
  - `\u{41}`, `\u{e9}`, `\u{200b}`, `\u{0}`, `\u{d800}`, `\u{110000}`,
    `\u{}`, `\u{1b` and `\x1b` are refused.

  That is ten spellings of ESC (five digit counts, two cases), and the
  compiler-engineer's section 7 says `fmt` keeps each as written. Its section
  7 names the gap (*"One gap"*) and leaves `fmt`'s canonical digits owed and
  unbuilt. The two verdicts state the property the built route lacks. Either
  the canonical digits land with the escape, or the claim narrows to *one
  spelling per character up to the escape's digits*.
- **The spec-warden's P7, pre-run** (it is registered for the landing gate;
  run here on the compiler-engineer's built route, 03:37:26 to 03:38:05,
  `192-critic-p2/ce/p7/p7.py`, output `p7.txt`). Each of F1's 55 string
  files went through `check`, then `check --apply`, the applied program
  through `check`, `build` and a run, its output compared with the
  literal's bytes:
  - 46 are `invisible_character`, one is `raw_tab` and one is
    `raw_carriage_return`. All 48 apply, check at exit 0 and print exactly
    the literal's bytes;
  - U+0000 is `nul_in_string`, and the applied program is still refused,
    as P7's corrected form (00:45) says;
  - 6 are accepted as written: NBSP, U+00AD, U+200B, U+200C, U+200D and
    U+FEFF mid-string.

  So P7 would pass on this tree today. The six are the residual the
  compiler-engineer names; P7 does not cover them.
- **"A program on it passes in every arm"** was my first pass's worry, and
  the run shows it did not happen: no B or C program used `[u8]`. My re-score
  (03:33:04 to 03:33:20, `192-critic-p2/blind/rescore.py`, the scorer's own
  functions over copies, my compiler) reads 20 passes, each exit 0 with
  nothing on stderr. Each used its arm's spelling. **The scorer never reads
  a program's exit code** (`192-blind-score.py:150-154` reads `stdout`
  only), so a program that printed the bytes and then panicked would pass.
  None did, by my run.
- **"The full net reads the base's counts"** (the ffi-pragmatist's Q4
  prediction, for rBE). It ran the compiler's own tests, the net's own tests
  and the examples, not the net's emission forms.
  - **Measured.** The `emit` form over an rBE tree, built and judged by
    rBE's own seed-built compiler (03:52:42 to 03:53:06,
    `192-critic-p2/rbe/net-emit.txt`), reads **1 passed, 7 failed**.
  - **Control.** The base tree with the base's compiler reads 8 passed, 0
    failed (03:53:33 to 03:53:57).
  - **The cause is not the lend.** Every emitted program carries a
    `_Static_assert` probe for each of the prelude's extern declarations,
    and rBE changes the prelude's: `hero_file_read_str`,
    `hero_file_write_str` and `HERO_OS_BAD_NAME` replace the probes of
    `hero_file_read` and `hero_file_write`. That moves 28 to 142 lines in
    each case.
  - **The lend adds to it.** `grep -rl 'hero_str_cstr(' tests/` names 37
    blessed emissions (36 under `tests/emission/`, one under
    `tests/golden/emit/`), which `hero_str_lend` moves too.

  **The `emission` suite over the same rBE tree** (03:54:07 to 03:57:08,
  `192-critic-p2/rbe/net-emission.txt`) reads **397 passed, 362 failed**.
  Every one of the 362 first differs at the prelude's probe lines
  (`hero_file_read`, `hero_file_write`), by golden tree:
  - `run` 270;
  - `examples` 55;
  - `ir` 22;
  - `fixedbugs` 8;
  - `emit` 7.

  So rBE owes a re-blessing of 362 emitted goldens, read by hand, which its
  cost table (*"the emitter: the same one line"*) leaves out. Any route that
  changes the prelude's extern group owes the same, route (e) alone
  included. The base's control count is in section 7.
- **The spec-warden's s2-d, the escape sentence inside its recommended
  rec4-r1** (its draft, `192-spec-warden/w/drafts/out/rec4-r1.md:47-50`,
  read only). It reads: *"`\n` `\t` `\r` `\\` `\"` in a string, and
  `\u{...}`, a code point in hex, for one it holds no other way (`\u{1b}` is
  ESC); `\'` instead of `\"` in a character literal"*.
  - **It drops the *"not 0 or a surrogate"* that arm B's and s2-b-string's
    words carry.** A string holds a NUL or a surrogate *no other way*, so by
    the sentence `\u{0}` and `\u{d800}` are legal. The built route refuses
    both: `nul_in_string` and `unknown_escape`, my run at 03:36:57. By
    CLAUDE.md § 12 the compiler would then be the one in the wrong.
  - **It leaves the character literal as ambiguous** as the spec-warden
    found arm B's words, by the same *`\'` instead of `\"`*. The built route
    refuses `'\u{1b}'`.
  - **It says nothing of the digits' case or leading zeros** (section 1's
    ten spellings).

  Unpriced: s2-d with those three repaired.
- **"Arm R's example teaches a refused receiver"** was measured (the
  spec-warden, 21:09) and then read against the run: 0 of 5 R sessions
  copied it. The spec-warden corrected itself at 00:44. Nothing missing;
  noted because the synthesis may quote the first sentence alone.
- **The program-token metric that lifted the escape veto** (the
  spec-warden, section 10: B 25, C 23, R 51.4, A 136.8 tokens a program) is
  a vendored count, a lower bound by the spec-warden's own section 3, not
  the reader's tokeniser. The ratio may survive `--refresh`; that is an
  inference, unrun.
- **The `claude-haiku-4-5-20251001` beside `claude-opus-5-5`** in every
  blind session's `modelUsage` (`llm-ergonomist.md`, *The models*) is
  recorded and not explained. Whether any of a session's reading or
  writing went through it is unmeasured: a question, small, for the record.

## 2. Where the seats disagree, and which side holds

**The class of a refusal.**
- **The compiler-engineer keeps every new refusal under `check
  --permissive`**: strings, comments, character literals. It cites panel
  066's CR and Zig's and Rust's refusals *"without the thesis"* (its
  section 6 and Q1 verdict).
- **The spec-warden reads the list's own test the other way** (its section
  6): *"reads a raw ESC as a thesis rule"*, and calls it *"the
  compiler-engineer's call"*.
- **The test, `diag.hero:94-97`**: a thesis rule is one *"the THESIS adds,
  as opposed to a rule without which the program has no meaning"*, and
  `--permissive` drops exactly those. A program holding a raw ESC, a raw tab
  or a raw RLO has a meaning (F1 at `build`: each prints its literal's
  bytes). By the test as written, each refusal is a thesis rule. Panel
  066's CR already departs from the test in the same way.
- **The historian** finds both shapes in precedent: the bidirectional
  refusal as a policy one can lower, the control refusal as grammar.

**Which side holds**: the written test sides with the spec-warden's
reading. The compiler-engineer's choice is a third category, *robustness
for the person reading*, which the shared brief offered for Q2 and which the
test does not name. Not measured by anyone: what Part 11's control arm
loses if these refusals stay in it. Either the test's comment gains the
third clause, so `--permissive` and the code agree, or the refusals join the
list.

**Q2, a comment.** Three lists:

| seat | in a comment it refuses |
|---|---|
| compiler-engineer | every Cc but the tab (ESC, NUL, a lone CR, DEL, C1), the twelve Bidi_Control, U+2028, U+2029 |
| spec-warden | *the bidirectional controls, U+2028 and U+2029* |
| historian | the nine explicit bidirectional controls, U+2028, U+2029 |

Two differences:
- **Controls in a comment**: the compiler-engineer alone refuses them. Zig
  does (the historian's [S14], the compiler-engineer's matrix). The other
  two seats are silent on them rather than against.
- **The three implicit marks** (LRM, RLM, ALM). The compiler-engineer's
  FriBidi evidence for them, `if a <RLM>><RLM> b` drawn `a < b` (its
  section 12), sits in code, which the lexer already refuses as
  `unexpected_character`. **Run here inside a literal** (03:51:06,
  `192-critic-p2/bidi/`, `fribidi --ltr --nopad --nobreak`):
  - `"a <RLM>><RLM> b"` draws `"a <RLM><<RLM> b"`;
  - `"user<RLM> 5 > 3"` draws `"user3 < 5 <RLM>"`;
  - in a comment, `# a <RLM>><RLM> b` draws `a <RLM><<RLM> b`, and
    `# a<ALM> > 3, b` draws `a3 < <ALM>, b`;
  - none moved code outside its string or comment; RLO did, as the
    compiler-engineer measured.

  So an implicit mark changes what a string's or a comment's own text
  appears to say. That is the compiler-engineer's ground, now measured where
  the refusal acts. Against it stands the scope of the harm (inside the
  literal only) and UTS #55's *"should not prohibit"* (the historian's
  [S36]).

**Q3's spelling.**

| seat | approves | objects to |
|---|---|---|
| compiler-engineer | (d) | (b), (c) |
| spec-warden, from 00:44 | (d), its veto lifted on Part 11's metric 1 | (b), (c), with (a′) as the conservative route |
| historian | *a spelling that shows the character; `\u{...}` has the widest precedent* | `\xNN` as bytes |

The historian notes (d) has no compiler precedent, only a style guide.
What holds and what is not measured:
- **(d) against (b)**: the blind run cannot decide it. Every B program used
  `\u{1b}`, which (d) also admits. Run here, the composite compiler checks
  all five B programs as written, no substitution, and they print the
  target bytes (03:50:30, `192-critic-p2/blind/route-*`).
- **What would decide it**: whether writers spell a character (d) refuses
  as an escape. That is `\u{e9}` for `é`, or `\u{200b}` for a ZWSP, which
  (d) refuses and whose only legal spelling is then the invisible one
  (section 4). No arm's task needed one. The spec-warden's P6 (an arm D on
  s2-d's own words) is unfunded and would not test that either, since its
  task is ESC.
- **(d)'s *one spelling* does not hold as built** (section 1).

**Q4's route.**
- **The ffi-pragmatist recommends rBE** and objects to (d) *"as briefed"*,
  on two measured breakages. The compiler aborts (134) on a `.hero` file
  holding a NUL, because it reads its sources through `hero_str_from_bytes`.
  And the net's own tests go red at defect 317's case, because the harness
  reads `git ls-files -z` into a `str` by design (`shell.hero:30-32`, `:608`,
  `:626`).
- **The spec-warden approves (d)** (s4-d, +4), its condition *"a program
  that must hold a NUL in a `str` moves me from (d) to (a) or (b)"*. It
  keeps the verdict *"unchanged"* at 00:44. Its section 10 read the blind
  reports; it cites none of the ffi-pragmatist's measurements of 00:40.

**Which side holds**: the measurements. The compiler, a program on the
closure list, holds a NUL in a `str` while reading such a source, and the
harness holds NULs by design: the spec-warden's own condition, met by
measurement it has not answered. Its Q6 recommendation, rec4-r1, carries
s4-d (*"immutable UTF-8 string with no zero byte"*), which is false under
rBE. The ffi-pragmatist's own condition names what would turn it: (d)
becomes the better route if Q1 and Q2 refuse a NUL in a source **and** the
harness reads its `-z` listings as bytes. That is a route nobody built
(section 3).

**An abort at a door.**
- **The historian objects to an abort as a door's answer.** Every runtime
  it fetched answers a failure, Go deprecated its panicking forms, and
  Zig's assertion became a fuzzer's crash (its Q4).
- **Under rBE, 25 of 27 doors abort at the lend** (134). The
  ffi-pragmatist calls `hero_run_arg`'s panic the precedent *"for the doors
  that are bindings"*.

Not measured: whether any door of this repository's programs can be reached
with a NUL from outside input. That is the Ghostty shape the historian cites,
and it would turn the judgement into a fact. Nor has anyone said whether a
panic on outside input is a *crash* in `.claude/rules/verification.md`'s
`blocking` list.

**One sentence contradicts another seat's route, measured.** The
compiler-engineer's `nul_in_string` reads *"C reads a string only to its
first NUL, so a string cannot hold one"*. Under rBE a `str` holds one, and
only the lend refuses it. On my composite of the two (section 3), `check
nulc.hero` prints that sentence, while `rf.hero` built by the same compiler
prints `3` for a `str` holding a NUL (03:48:59, `192-critic-p2/comp-f4/`).
True of a literal's spelling, false of a `str`; its wording waits on the
invariant (section 4).

## 3. Routes nobody built

**The composite the synthesis is likely to adopt, built here.** The
compiler-engineer's route (Q1, Q2, Q3 (d), Q7) and the ffi-pragmatist's
rBE (Q4) touch disjoint files. The compiler-engineer's are the lexer side of
`selfhost/` (15 files), the tests, the grammar and the site. rBE's are
`runtime/` (`hero_os.h`, `heroes_runtime.h`, `parts/failure.c`, `os.c`,
`str.c`, `text.c`), `selfhost/emit/inst.hero` and
`selfhost/library_source.hero` (`diff -rq` against a fresh base, 03:36).

Overlaid on `git archive 4c3524fb` in `192-critic-p2/comp/`, built in the
compiler-engineer's landing order:
- **generation A**, with `compile.hero:83` raw, built by the base's compiler
  (03:39:03 to 03:40:28);
- **generation B**, with `:83` as `\u{1}`, built by A (03:40:34 to 03:41:32);
- **B's seed**, built by clang alone with nothing on stderr, whose compiler
  emits the same seed: **`cmp` silent**, 35,493,146 bytes, sha256
  `330f4f60a2530671` (03:44:09);
- **its own tests**: 1,220, all passed (03:45:37);
- **F4**:
  - `nulc` refused at `check` (`nul_in_string`);
  - `rf` prints `3`, then stops at the lend (134);
  - `qi-read` reads `true`, `<failed>`;
  - `qi-write` reads `true`, and writes no `out-a`;
  - an `assert` prints `61 00 62` whole;
- **the blind programs as written**: A's, R's and B's fifteen check and
  print the target bytes; C's five are `unknown_escape`, now pointing at
  `\u{1b}` with two `guess` fixes.

**Its cost on the compiler checking itself**, instructions retired by
`/usr/bin/time -l` (03:47:47 to 03:48:33), `check selfhost/main.hero` over
one tree all four accept: the compiler-engineer's `192-ce-meas`, copied, with
`:83` by route (a′). Every compiler was seed-built by clang, two runs each:

| compiler | mean, millions | against the base |
|---|---|---|
| base (`8084f018f5387536`) | 81,324.8 | |
| compiler-engineer's route (`744c3197848b79d3`) | 81,780.5 | **+0.56%** |
| rBE, its seed built here (`f6e3606a76b9816b`, 35,213,202 bytes, 37 `hero_str_lend(`, as its seat counted) | 81,859.6 | **+0.66%** |
| the composite (`f01de4cf82c771a5`) | 82,323.3 | **+1.23%** |

The costs add. The compiler-engineer registered *"at most 1% more
instructions than on that batch's base"* for its route alone. If both land
in one batch, that prediction is falsified by the other seat's route, so the
synthesis restates it over the composite. The ffi-pragmatist's own
condition, *"the fact measured above 2% on a real program"*, is not reached.

**Not run on the composite**: the net (beyond `emit` and `emission` on rBE
alone, sections 1 and 7), Linux arm64 and Windows.

**Other routes no seat built**:
- **(d) whole, for Q4**: rD, plus the compiler's source read answering a
  NUL with a diagnostic, plus the harness reading `-z` listings as `[u8]`.
  The ffi-pragmatist names it as what would make (d) better; the
  spec-warden approves (d) without it.
- **rBE's robust reading**, `HERO_RUNTIME_ABI` moved to 27, together with
  the seed's order of landing. The rBE copy holds 26
  (`heroes_runtime.h:58`, `grep`), the conservative reading; with 26 the
  base compiler over rBE's runtime builds with the guard silently absent
  (the ffi-pragmatist's *HERO_RUNTIME_ABI*).
- **rBE beside lane b11-windows**:
  - **merged in text only**. `git merge-file` of rBE's `hero_os.h` and
    `os.c` against the lane's (`d1f857d4`) over `4c3524fb` reads 0
    conflicts each (03:51, `192-critic-p2/merge/`);
  - **its new doors delegate**. `hero_file_read_str` and
    `hero_file_write_str` call the existing `hero_file_read` and
    `hero_file_write` after their `memchr`, so they would take the lane's
    wide door on Windows;
  - **unbuilt and unrun**. The lane also changes `fs.c`, `dir.c`,
    `replace.c`, `codepage.c` and `runtime.c`.
- **One runtime printer for both causes.** rBE rewrites `failure.c`'s
  three lines to write by length, for the NUL. The compiler-engineer files
  apart, `adjacent`, the same three lines writing ESC raw (its Q7, F11).
  One change could write a message by length and its controls by code.
  Neither seat built it, and the filing should name rBE's edit.
- **Q1 with (a′) as its only spelling**, the spec-warden's conservative
  route: its refusal's message and fix, and `compile.hero:83` in two more
  lines in a file at 300 of 300. Priced in the spec (rec3-r1, -1), never
  built in the compiler.
- **Routes the historian lists and no seat weighed**:
  - (e′), `\e` for ESC, TOML 1.1's answer;
  - (c″), `\xNN` as a code point U+0001 to U+00FF, Python's and
    JavaScript's reading, which writes no half character;
  - for Q4, (f) a yes-or-no door answering `false` (Python 3.8's
    `exists`), and (g) the NUL refused where a `str` becomes a `cstr`,
    which is rBE's lend in other words;
  - for Q5, `args_checked()`'s failure carrying the argument's bytes.

## 4. The question the sitting should have asked

**What invariant does a `str` carry: may it hold a NUL?** The sitting asked
for routes (Q4's (a) to (e)) before the invariant they serve. The seats came
back holding three different answers without saying so:
- **"No `str` holds a zero byte"**: the spec-warden's s4-d, the type row
  *"immutable UTF-8 string with no zero byte"*, inside its recommended
  rec4-r1.
- **"A string cannot hold one"**: the compiler-engineer's `nul_in_string`
  message, said of every string, and its `\u{0}` refusal on the same
  ground.
- **"A `str` holds any UTF-8, NUL included; the lend refuses it"**: the
  ffi-pragmatist's rBE. It keeps `print`, `write_file`, SQLite's length
  binding and the harness's `-z` listings exact.

Everything that follows depends on which of them holds:
- the spec's type row, `:64`, against an abort clause at `:382`;
- design.md `:547` and `:2629` (*Always NUL-terminated*), and whether the
  freeze's ground at `:1016-1018` still reads as written;
- the wording of every message that names the NUL;
- whether `read_file` refuses a file holding one.

The historian's route (g) is the same question in Rust's shape: one string
type, or a string type and a C-string type with the NUL refused between
them. design.md has a sentence for neither answer (the ffi-pragmatist's
*"design.md does not cover a NUL that arrives without an escape"*).

**And when *one spelling* and *a spelling that shows it* collide, which
wins?** Shared proposal part 3 says a refused character stays writable by a
spelling that shows it, and design.md `:994-995` says one spelling. For a
character a string holds as itself, route (d) keeps one spelling at the cost
of a visible one, measured on the
compiler-engineer's compiler (03:36:57 and 03:57:14, `192-critic-p2/ce/sp/`):
- `\u{200b}`, `\u{200c}`, `\u{200d}`, `\u{2060}`, `\u{a0}`, `\u{ad}`,
  `\u{fe0f}` and `\u{feff}` are each `escape_not_needed`;
- `\u{9}` takes a `certain` fix to `\t`;
- `\u{200e}` (LRM, which a string refuses raw) is accepted.

The message for ZWNJ reads *"`\u{200c}` writes the invisible character
U+200C, which a string holds as itself: `\u{…}` writes only a character a
string refuses written as itself"*. Its fix reads *"fix (guess): write the
character itself, which no screen shows"*, and `check --apply` leaves the
file as it was.

So the only legal spelling of a ZWSP, a soft hyphen, a word joiner, ZWJ,
ZWNJ, NBSP, VS16 or a mid-string BOM is the invisible one, and the message
points the writer at it. That is the shape F10 named in today's
`unknown_escape`, now chosen on purpose for the residual.

The residual, the six F1 code points the route accepts (section 1), has only
an invisible spelling. Nobody asked whether a writer who wants a ZWSP
visible in the source must be refused. Route (b) would answer *no*, at the
price of two spellings; a (d) widened to the Default_Ignorable residual
would answer it too.

## 5. What Q5's absence costs the synthesis

The box was unreachable from 21:06 on 2026-10-04 (the coordinator's
message); I did not use it.
- **Every Windows fact in the sitting is carried.** F9 is panel 191's, at
  `7f4c0cc5`, not this base: `x?y` narrow, `x<U+FFFD>y` under the
  manifest, error 1113 under a strict conversion. No seat ran the WTF-8
  conversion on Windows. On this Mac and on Linux arm64 the ffi-pragmatist
  measured that the WTF-8 bytes abort `args()` and give `args_checked()`
  `not_text`. That Windows gives those same bytes is an inference from
  panel 191's compiler-engineer row (*`\ud800` kept*).
- **`:324-325` stays false on Windows when batch 11 lands.** The
  ffi-pragmatist read lane b11-windows (`d5133e26`, `dc7eed87`):
  `args()` still reads the narrow `hero_argv`. Under the lane's manifest,
  `x<D800>y` arrives as `x<U+FFFD>y`, valid text naming another file: Q-i's
  shape on the way in. The spec-warden's Q5 rests on route 4 making the
  sentence true *"at 0 tokens"*. Until route 4 is built and run, the
  sentence is false on one platform. Its veto of s5-win keeps the spec from
  saying so.
- **Nothing tracks Q-c once this sitting closes.** Defect 238's body holds
  *a directory named with a lone surrogate* (panel 191's R2), not `args()`'s.
  By my grep of the open issues (`surrogate`, `Q-c`, `324-325`, `D800`,
  `panel 192`), no other item holds it. A negative claim on that
  vocabulary. The synthesis either files it or carries it in 238's item.
- **Defect 245 cannot close at the landing round's gate.** rBE is a change
  in `runtime/`, and `.claude/rules/verification.md` § The batch closes a
  C-boundary defect only after the push's platform legs have run its cases.
  rBE's Windows leg (F4, the 14 shapes, the 29 door probes) is owed to the
  box. So the tag waits on the box through 245, not only through Q5.
- **What Windows changes in Q4 is unrun.** Panel 191's ffi-pragmatist says
  `hero_win_wide` (length -1) stops at an interior NUL. That rBE's lend
  check runs before every wide door, so none is reached with one, is the
  ffi-pragmatist's reading of the code, not a run.
- **The conversions' own choice rests on reading.** The strict conversion
  refuses at start, which `args_checked()` cannot report. WTF-8 lets the
  existing check refuse. The ffi-pragmatist's *route 4* and the historian's
  precedent (Go 1.21, Zig, Rust's `OsString`) agree, and neither is a
  Windows run on this base.

## 6. The blind experiment, as run

Checked: the twenty folders scored, each `c.hero` rebuilt by me and re-scored
with the scorer's own functions (section 1). Every pass reproduces, with the
spelling the coordinator recorded.

- **It cannot separate the arms.** Twenty of twenty pass, at the ceiling in
  every arm. The registered rule's *"B and C both at 4 or 5"* branch fired,
  which says the choice between them rests on other grounds. What it did
  measure: each arm writes ESC by its own sentence, and the program's size
  (the spec-warden's metric 1, vendored).
- **A route nobody listed, my first pass included, carried arm A.** All five
  A sessions wrote ESC through `putchar` from `extern "stdio.h"`, and the
  registered *"at most 1 of 5"* is false. Writability today has three
  routes: the FFI, `[u8]` with `validated_bytes()`, and a raw byte. The
  sessions turned down the raw byte as invisible: 3 of 5 A sessions and 4
  of 5 R sessions name it and refuse it (the spec-warden's count, 00:40).
- **The task could not reach the question that divides the seats.** ESC is
  refused raw under every route, so every route's escape admits it. A task
  needing a character (d) refuses as an escape (`é` by code, a visible
  ZWSP) is what separates (b) from (d). So is a task needing ZWJ in an
  emoji, which separates Q1's lists. Section 2 lists the questions no arm
  could answer.
- **The remainder could buy it.** 5 - 3.9035 = 1.0965 USD; B's and C's
  sessions cost 0.1663 to 0.1789. The spec-warden's P6 (an arm on s2-d's
  words, five sessions, about 0.90) fits, but its task is ESC, so it would
  measure the wording and not the restriction. The same five sessions on a
  task that needs a refused-as-escape character would test (d)'s
  restriction itself. A paid run, the author's to fund.
- **The channel hazard my first pass named was measured, outside the
  experiment, and it reached this report too.** The historian's correction 1: its own Write calls turned a
  written escape (a backslash, `u`, then `200b`) into a raw U+200B in its committed report, twice. In the
  blind run no `c.hero` holds a raw ESC (the scorer's spelling column, and my
  re-score). So no session wrote a four-digit JSON-style escape that the
  channel could have decoded into one; the hazard did not bite there. **It
  bit this file**: the sentence above, as I first wrote it, quoted the
  historian's four-digit escape, and the tool that wrote it left a raw
  U+200B between two backquotes. A scan of every character above ASCII
  found it at 04:00, and a script replaced it with words. A third measured
  instance, after the historian's two.
- **The scorer** does not read a program's exit code (section 1). Its
  description gained a label after `a1` and before the rest, which the
  coordinator records; the pass rule was not touched.

## 7. What I re-ran, and what I did not

**Re-ran**, each in `192-critic-p2/` and named above:
- the twenty blind programs, scored again;
- the ten spellings of ESC and nine refused ones on the compiler-engineer's
  compiler;
- P7 over F1's 55 strings;
- the composite (generations A and B, the seed, its fixpoint, its 1,220
  tests, F4, the blind programs as written);
- rBE alone, seed-built (35,213,202 bytes, 37 `hero_str_lend(`);
- instructions retired by `check selfhost/main.hero`, four compilers, two
  runs each;
- `emit` over rBE and over the base;
- FriBidi over the implicit marks in a string and a comment;
- a textual merge of rBE with lane b11-windows.

**The `emission` suite**: over the rBE tree with rBE's compiler, 397 passed,
362 failed (03:54:07 to 03:57:08). Over the base tree with the base's
compiler, the control, **759 passed, 0 failed** (03:57:41 to 04:00:44,
`192-critic-p2/base/net-emission-base.txt`). So every one of the 362 is
rBE's.

**Did not run**:
- the Windows box and Docker;
- any paid run, or `measure --refresh`;
- the net over the composite beyond `emit` and `emission` (no `records`,
  which needs `.git`; no `check`, `fixes`, `run`, `wholes`, `descriptors`
  or `determinism` over it);
- rBE's door probes and shapes, which I read and did not repeat;
- the compiler-engineer's census and net, read and not repeated.

Each seat's copy was read only; every build and run above is in my own
folders.

Finished at 04:01 (from `date`, read at 04:00:52 before this last write).
