# Panel 192: a `str` holds any UTF-8, the C boundary refuses a NUL, and a string refuses what its reader cannot see

2026-10-05, written from 15:48 by the clock (`date`). The **full panel**:
- `compiler-engineer`, `ffi-pragmatist`, `spec-warden` and `historian`;
- `llm-ergonomist` as a blind experiment run by the coordinator;
- the completeness critic, over the briefs first and over the reports after.

Convened by the author's *3a* of 2026-10-04 for defects 245, 251 and 283, with
panel 191's Q-c and Q-i, the blind seat capped at 5 USD.

**The tree.** The seats worked from `git archive 4c3524fb`, batch 10's closing
commit. The briefs are `docs/panel/192-briefs/`, repaired from 18:08 on
2026-10-04 after the critic's first pass and before any seat was launched;
each keeps the text the critic read as `<name>-before-the-critic.md`. The
reports are `docs/panel/192-reports/`, each append-only since it was first
committed at `99a67630`.

**Interruptions.**
- The account's session limit stopped every seat twice, at about 18:30 and at
  about 01:30. Each was resumed from its own report.
- The author paused all work from 21:26 to 00:35.
- **The Windows box was unreachable from 21:06 on 2026-10-04 to the writing
  of this file**: Tailscale read it offline, last seen 15 hours before 15:40.
  So Q5 was not run on it (§ What Q5's absence costs).

**Faults of process**, each recorded by the seat or the critic:
- **The critic's own first pass was falsified twice by the seats.**
  `[1].validated_bytes()` needs a typed binding, and a binding's own C is a
  third door for a NUL.
- **The tool that writes files turned a written backslash escape into a raw
  ZERO WIDTH SPACE** three times: twice in the historian's report and once in
  the critic's, each found by a scan and corrected underneath. Filed as an
  improvement (R12).
- **Seats wrote times before reading the clock.** Each is corrected
  underneath in its report.
- **A brief's negative sentence was false.** The coordinator's F4 said no door
  asks a `str` for a NUL, while `run.c:76` does; the critic found it, and the
  repaired brief says so.

## The proposal

The coordinator's starting point (`00-shared.md` § The proposal to judge),
written to be refuted:
- a string refuses panel 188's list of invisible characters;
- a comment refuses what reorders a line;
- a refused character stays writable by a spelling that shows it;
- no `str` lends a NUL to C without being told, and no runtime door opens a
  file the program did not name;
- `args()` keeps its sentence on Windows.

**Seven questions**:
- **Q1**: a string;
- **Q2**: a comment;
- **Q3**: writing what is refused;
- **Q4**: a NUL at the C boundary, and the runtime's doors;
- **Q5**: `args()` on Windows;
- **Q6**: the specification;
- **Q7**: the shapes beside.

## The verdict table

| seat | verdict | cost | prediction | condition |
|---|---|---|---|---|
| compiler-engineer | **object** to the proposal's Q1 list; **approve**, built and run on the whole net, a refusal of the Cc controls, the twelve Bidi_Control and U+2028/U+2029, in strings and comments (the tab spared in comments); **approve** route (d), `\u{hex}` only for what a string refuses raw; **object** to (b) and (c); no veto | +520 lines in the layout unit (78,278 to 78,798); `check selfhost/main.hero` +0.6% instructions; the seed's landing order needs two generations | at its batch's gate, `check selfhost/main.hero` retires at most 1% more instructions and the compiler grows at most 600 lines | a measured program hiding a meaning behind a zero-width space; a right-to-left comment that needs a raw mark; a blind run where `\u{...}` writers fail |
| ffi-pragmatist | **approve** rBE: one bit per `str` in the header's magic word, the lend and the lease aborting on it, `read_file` and `write_file` taking the `str` and answering a failure; **object** to (a), (c) and (d) as briefed; no veto. Q5 unrun (the box) | about 5 instructions a lend; `check selfhost/main.hero` +0.71%; `HERO_RUNTIME_ABI` 26 to 27 on the robust reading | all 20 binding folders build and run unchanged on three platforms; the 29 door probes touch nothing on the box | a correct gated program that lends a NUL with an explicit length; the bit above 2% on a real program; Q1 and Q2 refusing a NUL in source with the harness reading its `-z` listings as bytes, which would make (d) the better route |
| spec-warden | **object** to the Default_Ignorable list; **approve** refusing raw controls, U+2028/9 and the bidirectional controls; **approve** (d) for Q3, its veto lifted on the blind programs' token counts; (d) for Q4; **veto** rewriting `:324-325` to describe Windows | rec4-r1, +12 tokens over r1's removal (-22); every delta a vendored lower bound | P1: the real delta 1.0 to 1.5 times the vendored one; P7: every refused string character but U+0000 fixed by `check --apply`, the fixed program printing the same bytes | a program that must hold a NUL in a `str` moves its Q4 from (d) to (a) or (b) |
| historian (advisory) | Q1 **object** to the proposal as written, **approve** its control half, the tab included; Q2 **approve**; Q3 **approve** `\u{...}`, **object** to `\x` as bytes; Q4 **approve** part 4, **object** to an abort as a door's answer; Q5 **approve** the lossless WTF-8 route | 148 pages fetched, 34 searches, no paid run | (none registered) | a compiler that refused ZWJ, ZWNJ or a variation selector raw for years; a runtime that aborts on a NUL at a door by design and kept it |
| llm-ergonomist (blind, run by the coordinator) | 20 of 20 programs right, five an arm, each by its own arm's route | 3.9035 USD of the author's 5 | A registered *at most 1 of 5*, **read 5 of 5, false**: today's spec already writes ESC, through C | (registered rule) B and C both at the ceiling: their choice rests on other grounds |

## What the sitting measured

**What a string and a comment hold today** (F1, the coordinator, reproduced by
the critic):
- of 55 code points written raw in a string, `check` refuses one, CR; in a
  comment, none;
- each of the 54 builds and prints its literal's bytes, the NUL included.

**Which list, measured on real text.** Panel 188's predicate refuses ZWJ,
ZWNJ, the variation selectors and the tag characters. Under the
compiler-engineer's route, `print` of 👨‍💻 (ZWJ), Persian with ZWNJ, ⚠️
(VS16) and a subdivision flag each prints exactly its bytes. Panel 188's list
refuses all four, and none of the five other compilers the seat ran refuses
any of them. The historian found no compiler or linter that refuses them where
they join text, and UTS #55 calls the joiners *necessary*.

**The implicit bidirectional marks**, LRM, RLM and ALM, measured by the
critic with FriBidi inside a string and a comment:
- `"user<RLM> 5 > 3"` draws `"user3 < 5 <RLM>"`;
- `# a<ALM> > 3, b` draws `a3 < <ALM>, b`;
- none moved code outside its string or comment; RLO does.

**ESC is writable today three ways.** The blind arm A's five programs wrote it
through C's `putchar` (`extern "stdio.h"`), R's five through `[27]` with
`validated_bytes()`, and a raw byte would check. Three of five A sessions and
four of five R sessions named the raw byte and turned it down as invisible.
**So writability is not the reason for an escape**: every arm reached the
target in one turn. The reason the sitting found is the refusal's fix. A
`certain` fix needs a spelling inside the literal, panel 066's precedent for
CR, and the run-time route cannot stand in a constant's body or a `match`
pattern (the compiler-engineer, run).

**A NUL, door by door** (the ffi-pragmatist, 29 probes, this Mac and Linux
arm64 alike):
- on the base, 28 doors acted on the truncated name;
- 19 of them changed, made, read, listed or ran something the program never
  named: `remove` deleted another file, `dir_remove_tree` a whole other
  directory, `run_go` ran another program;
- under rBE nothing is touched: the two program-facing doors answer a
  failure, the other 26 stop at the lend, and `run_arg` keeps its panic.

**Where a NUL comes from**: a raw NUL in a literal, `read_file`, and a
binding's own C returning bytes with a length. **The compiler and the harness
hold NULs in `str`s on purpose**:
- the compiler reads a `.hero` source that holds one through
  `hero_str_from_bytes`;
- the harness reads `git ls-files -z` into a `str` (`shell.hero:30-32`,
  `:608`, `:626`).

Under route (d), which refuses a NUL when a `str` is built, the compiler
aborts at 134 on such a source, and the net's own tests go from 1 to 2 failed.

**The prices, in instructions retired** (`/usr/bin/time -l`, this Mac):
- **a scan at every lend**: about 0.44 instructions a byte, +1,080% on a 1 MiB
  string lent 1,000 times;
- **rBE**: about 5 instructions a lend;
- **the composite of the compiler-engineer's route and rBE**, built by the
  critic in one tree, fixpoint by `cmp`, 1,220 own tests passed: `check
  selfhost/main.hero` **+1.23%** (the two alone +0.56% and +0.66%).

**rBE moves the prelude every emitted program carries**: its extern probes for
`hero_file_read` and `hero_file_write` become those of the `str` doors. So
`emission` over an rBE tree reads 397 passed and 362 failed (the base, the
control: 759 and 0). Each of the 362 differs first at those probe lines, which
re-blessing owes, read by hand (the critic).

**One spelling, measured as built.** The compiler-engineer's (d) accepts ESC
in ten spellings, `\u{1b}` to `\u{00001b}` in two cases, and its `fmt` keeps
each as written. The canonical digits are owed, unbuilt (the critic).

**The spec, priced on copies** (the spec-warden, vendored lower bounds):
- the base is 7,117 vendored and 9,392 real, with 848 of headroom;
- the dearest draft is +65;
- `:35-36` is true of today's compiler (71 probes), and must change under any
  refusal.

## Disagreements, unsmoothed

1. **May a `str` hold a NUL?** This is the question the sitting should have
   asked first, and the seats answered it three ways without saying so (the
   critic's section 4):
   - **"No"**: the spec-warden's s4-d, *immutable UTF-8 string with no zero
     byte*.
   - **"A string cannot hold one"**: the compiler-engineer's `nul_in_string`
     message, said of every string.
   - **"Any UTF-8, NUL included; the lend refuses it"**: rBE.

   **The measurements side with the third.** The compiler and the harness
   hold one by design, and that meets the spec-warden's own condition for
   leaving (d), a condition it did not answer. R1 rules.
2. **The class of the new refusals.**
   - The compiler-engineer keeps them under `check --permissive`, robustness
     for the reader, as panel 066 kept CR.
   - The spec-warden, and `diag.hero:94-97`'s written test, read a rule a
     meaningful program breaks as a thesis rule.
   - The critic: the test sides with the spec-warden, and the
     compiler-engineer's is a third category the test does not name.

   R4 rules, and amends the test.
3. **What a comment refuses.**
   - The compiler-engineer: every control but the tab, and all twelve
     bidirectional controls.
   - The historian and the spec-warden: the nine explicit ones, U+2028 and
     U+2029.
   - UTS #55 says *should not prohibit*.

   R3 rules robust, and records the narrower list.
4. **An abort at a door.**
   - The historian objects: every runtime fetched answers a failure, and Go
     deprecated its panicking forms.
   - Under rBE, of the 29 probes, two doors answer a failure, 26 stop at
     the lend and `run_arg` panics.

   R1 keeps the abort where the door is a binding and the failure where the
   door is the program's, `read_file` and `write_file`. Whether outside input
   can reach a lend with a NUL in this repository's programs is unmeasured.
5. **One spelling against one that shows.** Under (d), ZWSP, ZWJ, ZWNJ, NBSP,
   VS16, a soft hyphen, a word joiner and a mid-string BOM have one legal
   spelling, the invisible one, and the message for `\u{200c}` points the
   writer at it (the critic). R5 rules, and names the alternative.

## The resolution: `provisional — author ratification pending`

**R1. The invariant: a `str` holds any UTF-8, U+0000 included, and the C
boundary refuses a NUL** (rBE, the ffi-pragmatist's):
- every `str` carries one bit, in the low bit of its header's existing magic
  word, saying whether it holds a NUL, so the layout does not change; every
  constructor that takes bytes with a length sets it, and concatenation
  carries it;
- `.cstr()` and `.lease()` abort naming the byte where the bit is set, the
  precedent of `hero_run_arg` (`run.c:76`) and of a null `cstr`;
- `read_file` and `write_file` take the `str` and answer a failure the program
  reads for a path holding a NUL, never another file;
- `HERO_RUNTIME_ABI` moves from 26 to 27, so that no compiler and runtime
  pairing runs with the guard silently absent (the seat measured both
  pairings);
- the 362 emissions rBE moves are re-blessed, and each is read to differ only
  at the prelude's probe lines and the lend.

**The conservative alternative, recorded**: route (d), no `str` holds a NUL,
refused where one is built (+4 spec tokens). It owes the compiler's source read
answering a NUL with a diagnostic, and the harness reading its `-z` listings as
`[u8]`. Nobody built it.

**R2. A string literal refuses written raw**:
- every Cc but the line end: C0, DEL and C1;
  - the tab, with a `certain` fix to `\t`;
  - CR keeps `raw_carriage_return`;
  - the NUL, with no spelling;
- the twelve Bidi_Control: the nine explicit embeddings, overrides and
  isolates, and LRM, RLM and ALM;
- U+2028 and U+2029.

Not the rest of Default_Ignorable: ZWJ, ZWNJ, the variation selectors, the
tag characters, ZWSP, a soft hyphen, NBSP, a word joiner and a mid-string BOM
stay legal raw, because real text needs them. **The NUL's message names no
invariant R1 denies**: it says a NUL written raw is invisible and no escape
writes one, and that a `str` which must hold one is built from bytes at run
time.

**R3. A comment refuses the same set but the tab.** Controls reach the
terminal of anyone who reads the file (Zig refuses them), and the implicit
marks reorder a comment's own text, measured. **The conservative alternative,
recorded**: the nine explicit bidirectional controls, U+2028 and U+2029 (the
historian and the spec-warden; UTS #55).

**R4. The class: robustness for the person reading.** The refusals of R2 and
R3 stay under `check --permissive`, as panel 066's CR does, because they guard
what a reader sees rather than what the thesis adds. `diag.hero`'s test comment
gains that third category in the same change, so the code and its comment
agree. **The conservative alternative, recorded**: thesis rules, dropped by
`--permissive`, the written test's present reading.

**R5. Writing a refused character: route (d).**
- `\u{hex}` in a string, only for a character R2 refuses raw. Never for 0 or
  a surrogate. Never for one a letter escape writes: `\u{9}` takes a
  `certain` fix to `\t`.
- **One spelling, by refusal**: lowercase hex, no leading zero. Any other
  digit spelling is refused with a `certain` fix to the canonical one, and
  `fmt` prints it so. The ten spellings the built route accepts are closed.
- Not in a character literal, and not in a group head's string.
- A character a string holds raw is written raw: `\u{e9}` is refused with a
  `guess` fix to `é`. This keeps design.md `:994-995`'s one spelling, at the
  cost Disagreement 5 names.
- `unknown_escape` points `\x1b`, `\033`, `\e` and `\u001b` at `\u{1b}`. Its
  fix stays a `guess`, because `"bin\x86"` reads as a code.
- **The landing order**, measured: generation A with `compile.hero:83` raw,
  built by the base; generation B with `:83` as `\u{1}`; the seed from B, its
  fixpoint by `cmp`.
- **The conservative alternative, recorded**: no new escape, route (a′)
  alone, a few words at `:384` (rec3-r1, -1 token), with the refusal's fix
  then a `guess`.

**R6. `args()` on Windows: the wide `argv` converted losslessly to WTF-8**, so
that the existing check refuses a lone surrogate: `args()` aborts and
`args_checked()` answers `not_text`, as on this Mac and Linux arm64 today
(measured there). `:324-325` keeps its words, at 0 tokens. **This rests on the
Mac and Linux runs and on precedent**: Go 1.21, Zig and Rust's `OsString`, and
Go's earlier U+FFFD replacement, which hung `os.RemoveAll`. It is owed a build
and a run on the box, on this base, before it lands, composed with defect
238's `c-dirwide`. Filed so that it is tracked (R12).

**R7. One runtime printer for both causes.** `failure.c`'s three lines write a
message by its length, so a NUL no longer cuts it, and write its control
characters by their code, so an ESC no longer clears the reader's screen.
`heroes test`'s output does the same for a test's title. These are Q7's shapes
beside: filed with this sitting (R12), and landed with it.

**R8. The specification**: the spec-warden's rec4-r1, amended three ways:
- R1's invariant replaces s4-d: `:64` keeps *UTF-8*, and `:382` says the lend
  and the lease abort on a NUL;
- the two doors' failure codes are named, since `read_failed` and
  `write_failed` are in no sentence today (`grep`, the ffi-pragmatist);
- s2-d takes the critic's three repairs: never 0 or a surrogate, not in a
  character literal, and lowercase digits with no leading zero.

It is priced on a copy at the landing, paid by r1's removal (-22). The
predictions are P7 and P1, scored at the landing round's gate and at the next
`m-*` tag at the latest.

**R9. design.md.**
- The freeze at `:1016-1018` is opened by this sitting for `\u{...}` within
  R5's bounds; `\0`, `\xNN` and octal stay frozen.
- `:994-995`'s one spelling holds by R5.
- `:547` and `:2629`, *always NUL-terminated so `.cstr()` is free*, hold: the
  lend reads one bit.
- Two sentences are owed: R1's invariant, and R4's class.

**R10. Refused**:
- **(b)** `\u{...}` for every code point: two spellings of one character;
- **(c)** `\xNN`: bytes, so half a character, and `01` to `7f` writes none of
  the real-text characters;
- **(e)** named constants, and **(e′)** `\e`, a letter for one character of
  many;
- **panel 188's Default_Ignorable list in strings**: it refuses real text;
- **Q4's (a)**: a scan per lend, +1,080% on long lends;
- **Q4's (c)**: a fallible lend, 137 lends and 21 leases in gated code;
- **Q4's (e) alone**: it leaves every binding's `.cstr()` truncating.

**R11. The costs, restated over the composite.** At the landing round's gate,
`check selfhost/main.hero` retires at most 1.5% more instructions than its base
(the composite measured +1.23%), and the compiler grows by at most 600 lines in
the layout unit for R2 to R5, beside rBE's runtime. The compiler-engineer's 1%
was registered for its route alone and is restated here, not scored.
Robustness ranks above speed (CLAUDE.md § Precedence).

**R12. Filed with this sitting**:
- Q-c, `args()` on Windows (R6), `blocking`: the spec's sentence is false on
  one platform;
- `[97, 0, 98].validated_bytes()` returning a one-byte `str` with no word,
  `blocking` under R1, a wrong value;
- the runtime's printer and `heroes test` writing a message's controls raw
  (R7), `adjacent`;
- no instrument scanning the repository's documents for raw invisible
  characters, `improvement`, the write-tool hazard measured three times.

**R13. The landing**: the next batch, on this provisional resolution, as 227,
231 and 238 landed on theirs.
- **What it holds**: defects 245, 251 and 283, 291 (Q7's diagnostic quoting
  a line) and R12's four.
- **245 closes only after the platform legs** have run its cases: rBE is a
  `runtime/` change at the C boundary, and its Windows leg (F4, the shapes,
  the 29 door probes) is the box's.
- **What it owes**:
  - P7 and P1;
  - R11's two bounds;
  - the 362 re-blessed emissions, read;
  - the rule's own cases, each red on the base first.

## What Q5's absence costs

- **Every Windows fact here is carried** from panel 191 at `7f4c0cc5`. No
  seat ran the WTF-8 conversion on Windows.
- **`:324-325` stays false on Windows** until R6 is built and run. Lane
  b11-windows's `args()` still reads the narrow `argv`, so under its manifest
  a lone surrogate arrives as U+FFFD.
- **Defect 245 waits on the box** through R1's Windows leg, not only through
  Q5.

## Predictions to score

- **The compiler-engineer's**, restated by R11 over the composite.
- **The ffi-pragmatist's**:
  - all 20 binding folders build and run unchanged on three platforms;
  - the 29 door probes touch nothing on the box;
  - the full net reads the base's counts once the 362 emissions are
    re-blessed. The critic measured that unblessed it does not.
- **The spec-warden's**: P1 and P7. P7 was pre-run by the critic on the
  compiler-engineer's route: 48 refusals applied and printed the literal's
  bytes, and U+0000 stayed refused.
- **The blind seat's** *A at most 1 of 5* is scored now: **false**, 5 of 5.

## The critic's passes

**First, over the briefs** (`completeness-critic-briefs.md`): it found
- F4's false sentence and Q4's half-unrun premise;
- the run-time route for ESC;
- route (d), the NUL in the existing scan, the doors that take the `str`,
  and the WTF-8 conversion;
- the scoring rule's two notations and the arms' two variables.

The briefs were repaired before any seat ran.

**Second, over the reports** (`completeness-critic.md`). This synthesis
answers it so:
- the unmeasured claims (one spelling, rBE's net, s2-d): R5, R1's
  re-blessing and R8;
- the disagreements: § Disagreements, ruled by R1, R3, R4 and R5;
- the routes nobody built: the composite, which it built; (d) whole, recorded
  as R1's alternative; R7's one printer;
- the invariant question: R1;
- Q5's absence: R6, R12, R13 and the section above;
- Q-c untracked: R12.

## Author's verdict

**RATIFIED 2026-10-05**, on the author's answer between 16:12 and 16:14 by
the clock read before and after it, meant as: *I ratify both panels, and yes
to the cost of the optional paid run: do it*. **Recorded as a reading**,
CLAUDE.md § 4's default; not `by delegation`.

**What the yes settles**:
- R1 to R13 as the resolution above states them, the robust route at each
  disagreement, over R1's (d), R3's narrower list, R4's thesis class and R5's
  (a′);
- the fifth blind arm funded, five sessions within the 1.0965 USD left in the
  5 USD cap, run before the landing.

The author added, meant as: *right now I am not really paying for those
sessions*. The costs this sitting gives are the CLI's own report,
`total_cost_usd`, and say nothing of a bill; the cap stands as the bound the
sitting named.

**What it does not settle**: R6's run on the Windows box, owed whatever the
answer, and what only the landing measures. The landing is defects 245 and
283 and the items R12 filed, in the batch after batch 11.

## The fifth blind arm, run 2026-10-05

Funded by the author's ratification of that day and run by the coordinator
from 16:48:01 to 16:51:35 by the clock: five sessions, 0.9317 USD by the
CLI's report, 0.1648 left of the 5 USD cap. Its specification is the base's
with route (d)'s words as R8 gives them, s1-mix, s2-d with the critic's three
repairs, and r1 (`192-briefs/blind/spec-d.diff`); R1's sentences at `:64` and
`:382` and the two failure codes are not in it, since the task reaches none of
them. Its task needs U+00E9 and U+200B, both of which a string holds raw under
R2, so R5 refuses `\u{e9}` and `\u{200b}`. Its brief, scorer and run are
`192-briefs/blind/brief-d.md`, `score-d.py` and `run-d.sh`; the five reports
are `192-reports/llm-ergonomist-d.md`.

- **4 of 5 pass** under R5 as the landing will apply it, each printing
  `63 61 66 c3 a9 e2 80 8b 21 0a` at exit 0. All five write `é` raw. Four
  write U+200B raw, each saying in its `choice_points` that it chose to; d2
  writes `\u{200b}`, which R5 refuses with a `guess` fix to the raw character.
- **Against what the scorer registered before the first session**: *`é` raw
  in at least 4 of 5*, read 5 of 5, true; *`\u{200b}` in at least 2 of 5*,
  read 1 of 5, false; *at most 3 of 5 pass*, read 4, false. R5's restriction
  cost one writer in five on this task, and the mistake that one made is a
  compile error under R5.
- **What all five named**: whether s1-mix's *raw control character* covers
  U+200B, a format character (Cf) and not a control (Cc). Four read it as Cc;
  d2 read it the wider way. R5 rests on which characters a string refuses
  raw, and s1-mix leaves that class to the reader, so R8's wording, priced at
  the landing, is where it is answered: the class by name, as R2 has it, or a
  sentence saying a format character is held raw. Under R2 as ratified, d2's
  reading is the wrong one; under s1-mix's words alone the text allows it.
- **Context**: each session answers that only `brief.md` and `spec.md` were
  read, beside the harness's own system prompt and the account's email,
  attached by the CLI, as arms A, R, B and C answered on 2026-10-04; no
  project rule, contract or memory.
