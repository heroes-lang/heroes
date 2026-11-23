# Panel 257, the spec-warden's report

Written as I go, from 2026-11-23 11:10 by this Mac's clock (`date`). My copy:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/spec-warden/` at
`828567a30a4018e74605378c3572249b2bed6d85` (`git log -1 --format=%H`; `git status --short` printed nothing).
Seed built there (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 12.5 s real, `./heroes --version`
printed `heroes 0.6.0` / `sources 05186f2b38e77a97`, the critic's own key, so the seed is the same bytes); stage 2
built there (`./heroes build selfhost/main.hero -o heroes-next`). A number in this file is the command's beside it;
a duration is indicative (the Mac is shared: `uptime` read load averages 8.31, 18.19, 45.39 at 11:10 and 24.35,
18.41, 40.74 at 11:13). No paid run, no `claude -p`; the only API use is `--refresh` counts, listed below with
their number, and the key's LENGTH only was ever printed. Nothing outward, nothing committed, no file outside the
repository root.

## 0. Status of this file (updated as I go)

- [x] the brief, the shared brief, the critic's reading, the arms, my agent file, `spec-shape.md`, design.md §1.2
  and §1.6 (`sed -n 192,215p` and `255,300p docs/design.md`, read, not recalled)
- [x] stage 2, the before count re-read (section 1)
- [x] the arms' vendored counts re-read (section 1)
- [x] my own texts, vendored counts and the real counts: **all four `--refresh` spent** (section 3: `w16` +126, `f3`
  +58, `f1b` +111, **final `w26` +138**); **sections 4 to 8 were first written on my first-pass text `w16`
  (`spec-w1.diff`) and have been brought to the final text `w26` (`spec-w7.diff`); section 9 says what changed it** (a
  record is not rewritten: the first pass stays, with its real count)
- [x] the `spec` suite on my text: first-pass text (section 3b) and **final text (section 3d)**: 31 passed, 4 failed
  both times, the same four
- [x] predictions registered (`## Predictions registered before the blind run`, near the end)
- [x] the verdict structure and one line per numbered question (the last two sections)
- reading order: 1, 2, 3, 3b, 3c, 3d, 4 to 8, 9, 10, then 11, the predictions and the verdict (the sections were appended
  as the work went; the numbers are the work's order, not the argument's)

## 1. What I re-read, by command (the facts the brief carries)

- **Before**: `./heroes-next measure spec/heroes-spec.md` in my copy (stage 2, `./heroes-next --version` printed
  `sources 56db985736468b40`, the critic's key too): claude-legacy **9,585**, cl100k_base **9,761**, vendored
  maximum 9,761, **real 12,809** (`claude-opus-5`, 2026-11-22), headroom **3,575** against 16,384, the FFI floor
  mortgaging 60 (12,869 measured against the ceiling). `wc -l -c spec/heroes-spec.md`: 516 lines, 33,272 bytes;
  `shasum -a 256` begins `15804f3fb52a62c208bc59ef`. **Identical to the brief's; nothing differs.**
- **The arms' vendored counts** (each arm applied to a copy with `patch -p1`, `./heroes measure <copy>`; the seed
  compiler's `measure` and stage 2's agree to the token on all of them): f1 9,678 / **9,854** (+93), f1b 9,670 /
  **9,846** (+85), f2 9,672 / **9,848** (+87), f3 9,629 / **9,805** (+44). **All four equal the brief's.**
- **Design.md §1.6's payment rule, read by `sed -n 300,345p docs/design.md`**: *"Every amendment owes a named
  removal or a registered prediction naming an instrument that exists today"*; a prediction is **admissible as
  payment only if it names the instrument that will score it and the milestone at which it is scored, and that
  instrument exists on the day of registration** (panel 046: `heroes measure`, a compile, a diagnostic transcript...);
  *"A prediction naming an instrument nobody has built is still registered as an observation; it simply pays
  nothing."* §1.2 (`sed -n 192,215p`): *real cost = program tokens x (1 + rewrite rate)*; a correction round-trip
  costs 500 to 2000 tokens.
- **A stray of mine**: my first stage 2 build wrote its log to the harness scratchpad
  (`/private/tmp/claude-501/.../scratchpad/sw-stage2.log`), a file outside the repository's root, against
  CLAUDE.md § Hard stops; it read 0 bytes when I listed it at 11:11 (content at removal unread: the build's `time` line may have landed after), and I removed it (`rm -f`; `ls | grep -c sw-` printed 0).
  Everything since is under my copy's `build/` (removed at the end, `rm -rf build`).

## 2. My count predictions, registered at 11:27 (`date` printed `Mon Nov 23 11:27:49 CET 2026` just after writing this; I first wrote 11:40 by mistake and corrected it) BEFORE any `--refresh` was run

**Calibration, read before writing them**: panel 256's warden measured six real counts and wrote that *"the ratio
real / vendored on a marginal sentence ran 1.23 to 1.41 against the whole document's 1.31"* (`git show
rebel-round:docs/panel/256-reports/spec-warden.md`, section 2: 1.29, 1.29, 1.23, 1.41, and 1.27, 1.29 on its two
larger drafts). I use 1.29 and a range of 1.18 to 1.40 times the vendored (cl100k_base) delta.

| id | text | vendored delta (measured below) | predicted real delta | range |
|---|---|---|---|---|
| C1 | `w16`, my first-pass text | +90 | **+116** | +106 to +126 |
| C2 | `f3`, the bare arm | +44 | **+57** | +52 to +62 |
| C3 | `f1b`, the corrected class arm | +85 | **+110** | +100 to +119 |
| C4 | `w19`, my core text (kept in reserve for a revision of C1's text) | +75 | **+97** | +88 to +105 |
| C5 | f1, f2, the fold, the merge, the scope words | +93, +87, +85, +89, +94/+95 | estimated by the ratio only: **a lower bound, vendored only** | |

## 3. The real counts (`--refresh`, at most four; each one count request, never a `claude -p`)

**Protocol, as the brief says, run in ONE shell command each time**: `cp spec/heroes-spec.md build/spec.orig`
(sha256 `15804f3fb52a62c208bc59efce76aa0e55a48435f879471bff968bd4f213c427`), the candidate copied over
`spec/heroes-spec.md`, `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a`, `${#ANTHROPIC_API_KEY}`
printed alone (**108**, never the value), `./heroes-next measure spec/heroes-spec.md --refresh > build/refresh-N-<text>.txt`,
then `cp build/spec.orig spec/heroes-spec.md` and the `shasum -a 256` compared, and `unset ANTHROPIC_API_KEY`.

| # | text | vendored max (delta) | **real** | real delta vs 12,809 | real / vendored | predicted (section 2) | digest (the refresh's own `SPEC_DIGEST`) |
|---|---|---|---|---|---|---|---|
| 1 | `w16` = `spec-w1.diff`, my FIRST-pass text (superseded by row 4) | 9,851 (+90) | **12,935** | **+126** | 1.40 | +116 (106 to 126): **inside, at its top edge** | `535dedd42ad42225` |
| 2 | `f3`, the coordinator's bare arm | 9,805 (+44) | **12,867** | **+58** | 1.32 | +57 (52 to 62): **hit** | `7f2425a636c8eef1` |
| 3 | `f1b`, the coordinator's corrected class arm | 9,846 (+85) | **12,920** | **+111** | 1.31 | +110 (100 to 119): **hit** | `08a061b310e82ec9` |
| 4 | **`w26` = `spec-w7.diff`, my FINAL text** (section 9) | 9,862 (+101) | **12,947** | **+138** | 1.37 | +134 (122 to 146): **inside** | `5d451c5c7bf639da` |

After refresh 1 the restored file's sha256 was `15804f3fb52a62c208bc59efce76aa0e55a48435f879471bff968bd4f213c427`,
equal to the original's (printed in the same command).

## 4. Item 1: every arm's text, sentence by sentence (what each says, in the words the document already uses)

The document's own words, by command on the frozen spec: *abort* is defined once, line 202 (*"An abort ends the
program at once, saying why; no `T?` carries one."*); *callback* and *signal handler* are line 427's; `grep -o -i
'this runtime\|the runtime' spec/heroes-spec.md | sort | uniq -c` printed `1 the runtime` (line 383, about threads);
`grep -o -i -w -E 'windows|linux|macos|darwin'` printed **0** matches; `C's` occurs once (line 371, *"C's `strtod`"*).
The document never says *this runtime*, *held*, *seen*, `setjmp`, `longjmp` or `swapcontext` before line 428
(`grep -n -i -E 'setjmp|longjmp|jmp_buf|getcontext|swapcontext|signal handler|handler|save point'` prints lines 427
and 428 only: the brief's own command, re-run on the frozen file).

**Is *held running* / *not seen to end* a rule a reader can apply to a program they have not run? No, as the arms
word it; yes only through the exits the arm lists.** *Seen* is the runtime's act, and the reader cannot ask the runtime
what it saw. **What *"this runtime sees it return"* promises, and to whom**: nothing a reader can use: it promises no
property of the reader's C, only that *whatever the runtime does not see* is told by an abort. A class defined by the
runtime's own perception is true by construction (the hedge is load-bearing: panel 255's `fp3` and `as_0` and the
unseen saves of R3 are refused *because* the runtime does not follow them) and so tells the reader nothing until a
list of followed exits is attached, and every arm attaches a closed one. **The passive *has not been seen to end*
(f3) drops the observer and keeps the problem.**

| arm | what its sentence does | a way a reader can take it two ways, or a clause a running program falsifies |
|---|---|---|
| **f1** | defines *ends, for the program* by what *this runtime sees* (a return, a `longjmp`, `siglongjmp` or `_longjmp`), then *held running* for anything else, then the abort | (1) *"or jumped within is held running"* is an elliptical participle with no object: it reads as a way of *leaving* (it is the one way that does not). (2) *"a switch of context never switched back"* is **false of `setcontext` out of a handler**: panel 255's `b_0` and `b_1` (handler left by `setcontext` to a context `getcontext` took before the signal) are a switch of context never switched back and **run** (exit 0) on Darwin C4r (`docs/panel/255-reports/ffi-pragmatist-r2.md` lines 199, 302, 582) and on every Linux column including *C, 2679 alone* (`compiler-engineer-r2.md` lines 261, 262). *Never* is also a claim about the future the runtime cannot see: a `swapcontext` is refused because it **saves** the handler's own context, which may be resumed (`compiler-engineer-r2.md` lines 73 to 79, rule 6; the pragmatist's *"the coroutine may come back to the handler through `uc_link`"*), whether or not the program ever switches back. (3) a handler that has not left at all (p6, and `exit()` reaching an `atexit` function, the 2494 golden) is neither *ends* nor *left any other way*, so *held running* does not name it: the reader must infer. (4) `{return, longjmp, siglongjmp, _longjmp}` is a closed list of followed exits and `b_0`/`b_1` falsify it. |
| **f1b** | f1 without the context parenthetical | **dropping the words does not drop the claim**: *"left any other way"* still includes a `swapcontext` away, so f1b says p7 aborts, and p7 **runs** on Linux C2679 (engineer's table, p7, 3 of 3 at four settings), exactly as f1; and it still says `b_*` abort. It is not *"only what is true on both builds"*. |
| **f2** | promises *runs* for a `longjmp`-family exit, *"wherever the buffer lives"*, then the class | the positive promise is the strongest claim of the four and the one a running program falsifies on **route A** builds (Darwin x86-64, glibc's other architectures, a future C library: panel 255 R1, `## What conservative would have been`) and on `as_0`/`fp3` (a `siglongjmp` out whose target lies on a coroutine's stack: refused S on C4r, `ffi-pragmatist-r2.md` lines 283, 284 of panel 255). *"a callback ... runs"* also promises a callback the program's own checks may still abort. *"wherever the buffer lives"* is a clause true only on a decode-verified build: the brief's item 4 says such a clause says so or is cut. |
| **f3** | one rule, no cause named: *called while a handler of C's has not been seen to end, by return or by `longjmp`... out of it, aborts, saying so* | (1) **no thread**: the runtime keeps the entry per thread (`runtime/parts/sigguard.c:473, 477, 479`, `pthread_getspecific`; `runtime/parts/rebind.c:124-127`), so a callback called on another thread while the first thread's handler stands is **not** refused, and f3 says it is; the engineer's item 4 is the running program (pending in her report). (2) *"saying so"* has two antecedents (that it aborts? that the handler had not been seen to end?). (3) the same closed list, falsified by `b_0`/`b_1`. (4) it names **no refused program**, so it does not do what the author's *1a/2a* ask (*tell the reader which correct C programs are refused*): it states a rule and leaves the reader to derive the inline jump and the context switch. It is also the one arm that **subsumes line 427's *called by one*** (a handler that calls the callback has not ended), which is a point for it. |

**The findings that made me change a clause** (the brief: *"your report names which finding made you change a
clause"*), in the order they reached me:

1. **The critic's B22/B23** (p10 and p3 run on both adopted builds; the engineer's table now confirms 3 of 3 at `-O0`,
   `-O2`, plain and `--sanitize` on Linux) removed *"wherever the buffer lives"*, *"refilled"* and the *"save points"*
   wording from my text altogether: the sentence says nothing of buffers or memory, only *out of it*.
2. **Panel 255's tables, read by me** (`b_0`/`b_1`, `fp3`/`as_0`, `ap_*`; the 257 pragmatist's cells did not exist yet)
   changed the context clause from *"a switch of context never switched back"* to *"a `swapcontext` away from it"*, added
   `setcontext` to the exits that end a handler and added *"a jump onto another stack given to `makecontext`"*.
3. **The engineer's depth sweep (her § 4(a); section 9 below) is the one that changed the sentence's verb**: *"aborts"*
   became *"may abort"*, and *"from deeper in the stack than the handler was"* entered. **It falsified the first clause of
   every arm and of my first pass.** My final text is `w26` (`spec-w7.diff`), not `w16`.

## 5. Item 2: the closed list, and the running program that falsifies each clause

`spec-shape.md` (*"What was refused"*): **a closed list of aborts** is refused, panel 087's warden having *"vetoed a
closed list and falsified it with a running program"*. **Every arm has the same shape and it is a closed list on the
other side**: the abort is a class (*not seen to end*, *held running*) whose complement is a **closed list of followed
exits**, `return` and `longjmp`, `siglongjmp`, `_longjmp` (f2 states the list as what *runs*, f1 and f1b and f3 as what
*ends*). The abort sites stay open; **the list of what runs is closed, and a closed list of what runs is falsified
the same way**: by a running program whose exit is followed and unlisted.

| clause | open or closed | the running program that falsifies it (or that would) |
|---|---|---|
| the followed exits `{return, longjmp, siglongjmp, _longjmp}` (f1, f1b, f2, f3) | **closed** | **`b_0`, `b_1`** (`setcontext` out of a handler to a `getcontext` made before the signal): exit 0 on Darwin C4r (pragmatist, panel 255 r2, lines 199, 302, 582) and on Linux today and C2679 (engineer, panel 255 r2, lines 261, 262); both arms read *"any other exit"* or *"seen to end ... by return or by"* and predict an abort. **Measured by panel 255's seats, in this repository; the 257 seats' own cells for it are pending** (the engineer's brief item 3 lists no `b`; I name it as the next program the sitting should run). |
| *"a switch of context never switched back"* (f1, f2) | closed to one verb | **`b_0`, `b_1`** again (a switch never switched back that runs); and the converse: a `swapcontext` the program *does* switch back from later is still refused at a callback called while the handler is switched away (`g_*` rows, panel 255) |
| *"sees `longjmp`... take the thread out of it"* (f1, f1b) | hedged by *sees* | survives `fp3`/`as_0` (not seen, so held) and the unseen saves of R3 (`apu_*`, `cv`) **only because it is circular**: whatever the runtime does not follow is refused, said so |
| *"runs, wherever the buffer lives"* (f2) | positive promise | route-A builds (panel 255 R1: Darwin x86-64, glibc's four other architectures, a future C library: a heap, thread-local or late-loaded buffer is refused there); **`as_0`, `fp3`** on C4r (a `siglongjmp` out, refused S, *"a cost"*, R4); the unseen saves (`apu_*`, `cv`) |
| *"a callback called while ... aborts"* with no thread (f3) | open, **false across threads** | a callback called on **another thread** while the first thread's handler stands (the engineer's item 4, C11): the entry is per thread (`sigguard.c:473-479`), so it runs. *Pending in the engineer's report; source-only reading here (`runtime/parts/rebind.c:124-127` reads `pthread_getspecific`)* |
| *"jumped within"* (f1, f1b, f2) | redundant | none, and it is **not a way of leaving**: a handler that jumps within itself and calls the program's function (p8, the two `jumps-within-itself` goldens) is *called by the handler*, which line 427 already refuses; naming it as a cause adds a second home for the same rule |
| C5, a C++ exception (the S words' parenthetical, repeated by every arm that lists causes) | named in the runtime's words only | **none exists**: `grep -l -E 'throw|__cxa|_Unwind'` over the goldens prints nothing (the brief's own command), and `g++` is clang on both platforms; **no arm of mine names it**, because a clause the sitting can neither run nor falsify is a clause that cannot be priced |
| C12, `exit()` inside a handler reaching an `atexit` function | covered only by a class that says *not ended* | the 2494 golden refuses it in the S words though the handler never left; f1, f1b and f2 (*left ... any other way*) do not describe it; f3 and my text do (*a handler that has not ended*) |
| a handler the runtime never saw begin (installed by a raw system call, by a `dlsym`/`dlvsym`-found `sigaction`) and left by an inline jump | any arm | **runs**: the entry never stood (`sigguard.c` header, *WHAT IT DOES NOT SEE*; R3's residues). True of every arm, including mine: the runtime's gap, not a clause. |
| Windows | any arm | the runtime has no probe there: `sigguard.c:506` (`#else` of `#if !defined(_WIN32)`), `508` (*"Windows: no probe and no walk (panel 247's R10)"*), `sigjump.c:69` (`#if !defined(_WIN32) && !defined(__APPLE__)`): **a callback in a handler runs on the CI's `windows-latest` leg**. Same for line 427 as it stands. |

**A class against a list of causes: which survives?** **Neither survives a falsifying program if it promises an abort**:
the engineer's `p4s`, `p4a`, `p4x`, `b1`, `b2` run in the very class (*left any other way*, *held running*, *not seen to
end*) that every arm says aborts. What survives is a class that **(a) is applied to the program's own act, not to the
runtime's perception** (*a handler ends by a return or by a jump out of it; left any other way ...*), **(b) lists the
exits that end a handler by the calls the runtime was measured to follow** (`longjmp`, `siglongjmp`, `_longjmp` by the
twelve `a-callback-after-*` goldens and the engineer's cells, `setcontext` by `b_*`), and **(c) promises only a *may*
for the exits it does not follow**, listing them as *examples* (*left any other way (by an inline jump, ...)*), never
*only*. **A list of causes (an inline jump, a swapcontext, a jump onto a `makecontext` stack) is the part that is
falsified**, so the final text lists them only after *any other way*, each one measured on Darwin by panel 255 (G, `as_0`,
`fp3`) and, for the inline jump, on both platforms in this sitting.

## 6. Item 3: where it goes, what it costs the runtime, and whether it can merge

**I agree with the brief: before `No function of the program is a signal handler`**, line 428 keeping its present end
(no full stop). Two reasons, one measured by the critic and one mine.

1. **The test that holds the tail** (`selfhost/emit/callback_thunk.hero:354-375`, read: it locates the first
   `No function of the program is a signal handler` and takes the line to its `\n`, then compares to the string literals of
   `#define HERO_SIGNAL_REMEDY`): a sentence placed before that phrase leaves it untouched; my sentence contains
   neither that phrase nor a newline, and the critic's B1 measured an arm placed the same way (the right-hand side
   ends `... since its last call`, the test's result unchanged). **That test is already red at the frozen tree**
   (the mailbox clause of `c0476dff3`, the critic's B2): I did not run it and read no arm's result into it.
2. **`HERO_SIGNAL_REMEDY` is *what to write instead*** (`runtime/parts/sigguard.c:99-109`, the comment above the
   `#define`: *"What to write instead, the last words of every refusal below"*). A rule about how C may leave a
   handler is not a remedy for a Heroes program.

**The alternative the critic listed, priced** (route 4: the sentence appended after *"its last call"* and so held by
that test to the constant the runtime prints):

| what | measured | command |
|---|---|---|
| vendored tokens | **+91** (against +90 placed before: the `. ` costs 1) | `./heroes-next measure` on `build/w/v/w16/append` = `spec-w6.diff` applied |
| real tokens | **not priced by `--refresh`: a lower bound, vendored only** (+91) | |
| goldens whose `.expected` end in the tail and so would change | **22** (`grep -l 'No function of the program is a signal handler' tests/golden/run/*.expected | wc -l`) | by kind: 17 S or U words (`grep -l 'seen to end'`), 3 D (`ran as a signal handler`), 1 allocator (`used this runtime's allocator`), 1 lend (`record of the buffers lent`) |
| refusal sites that print it | **3 files, 3 uses of the constant**: `sigguard.c:453`, `alloc.c:620`, `lend.c:194` (`grep -c 'HERO_SIGNAL_REMEDY,'`) | |
| bytes added to every one of those 22 messages | **399** for the final text (`wc -c spec-w7.txt`; 350 for the first pass) plus the stop | |
| refusals that would then end with a rule *about a jump out of a handler* that has nothing to do with them | **5 of the 22** (the 3 D, the allocator's, the lend's) | |
| refusals where the sentence would say *"... aborts, saying the handler was not seen to end"* in the message that has just said so | **17 of 22** | |
| memory | none: `hero_signal_stop` takes *"no buffer"* (`sigguard.c:137-139`, the comment above it: parts written one after another) | |
| new test | none (the 2533 test already holds the equality) | |

**So the alternative is cheap to build (one constant, 22 `.expected`) and wrong to print**: it buys the equality test
for free and spends it on 5 refusals about something else and 17 that repeat themselves. **Its price is the only
reason to choose it, and it is not a reason the reader gets anything from.**

**Merging beats appending: measured, and the merge loses.** `spec-shape.md`: *"a rule that joins a sentence already
there costs less than a new sentence"*. Two merges, both written and applied (`spec-w4.diff`, `spec-w3.diff`):

| text | vendored (cl100k_base) delta | |
|---|---|---|
| my sentence appended before the key (`spec-w1.diff`) | **+90** (real **+126**, measured) | |
| **merged into line 427's first clause** (`spec-w4.diff`: *"... nor called by one, nor by C on a thread where one of its signal handlers has not ended, which aborts the program, saying ...; its parameters follow the same rule and `()` is `void`"*) | **+89** | **one token cheaper, and a 70-word first clause that separates *its parameters follow the same rule* from the rule it refers to**: the merge can save only the words *If C calls a callback on a thread* it must restate |
| **folded**: my sentence before the key, and line 427's `, nor called by one` taken out because the sentence's first clause says it (`spec-w3.diff`) | **+85** | the one *named removal* I can offer: **5 vendored tokens** (about 7 real at the 1.40 this text measured: an estimate, a lower bound vendored only). It costs the plain statement of the commonest misuse (the handler calls my callback) that panels 247 to 256 and the runtime's K words rest on, so **I do not recommend it**; I report it because §1.6's rule asks for a named removal and this is the only one that removes nothing true. |

**The same three questions on the FINAL text** (`w26`; the first-pass table above is kept as the record):

| text | file | vendored cl100k_base (delta) | real |
|---|---|---|---|
| `w26` before `No function of the program is a signal handler` | `spec-w7.diff` | **+101** | **+138, measured** (12,947) |
| `w26` merged into line 427's first clause | `spec-w9.diff` | **+94** (7 fewer) | not priced by `--refresh`: a lower bound, vendored only |
| `w26` folded (`, nor called by one` out) | `spec-w8.diff` | +96 (5 fewer) | not priced: a lower bound, vendored only |
| `w26` appended at line 428's end, the **REMEDY** route | `spec-w10.diff` | +102 (the `. `) | not priced: a lower bound, vendored only |
| my core `w25` (no `setcontext`, no `makecontext` jump) | `spec-w11.diff` | +88 | not priced |
| **`w27`, true on both builds as they stand** (*left by an inline jump* alone) | `spec-w12.diff` | **+76** | not priced |

The merge now saves **7** vendored tokens and is still a loss: *"... nor called by one, and may abort, saying the handler
was not seen to end, if it is called later on a thread whose handler of C's was left other than by ..."* is one 90-word
first clause that puts *its parameters follow the same rule* (now a rule with a *may* in it) after a parenthesis.
**The fold no longer removes anything the sentence says**: the final text does not restate *called by one* at all, so
`, nor called by one` stays, and `spec-w8.diff` is the removal I name only because §1.6 asks for one.

**Merging is a loss here because the new sentence is a rule about a different event** (a callback called after a
handler's exit) from line 427's clause (a callback that is a handler, or is called by one).

## 7. Item 4: scope (Windows, route A) and what the scoping words cost

**Windows**: the CI's fourth leg is `windows-latest` (`.github/workflows/ci.yml`, `grep -n`: line 366 `Windows x86-64`,
and the other three legs `macos-26`, `ubuntu-26.04`, `ubuntu-26.04-arm`, lines 368 to 372). The runtime has **no probe
there** (`sigguard.c:506-523`, `sigjump.c:69`, quoted in section 5). Unrun by me (no Windows machine in this sitting).
**The frozen spec names no operating system anywhere** (0 matches of `windows|linux|macos|darwin`), and line 427's
*"nor called by one"* is already **knowingly Windows-false**.

| form | vendored delta | |
|---|---|---|
| unscoped (`spec-w1.diff`) | +90 | true of the language the lanes will have made; false on the `windows-latest` leg until a probe exists |
| prefix **`On Linux and macOS, `** (`spec-w5.diff`) | **+95** cl100k_base / +96 claude-legacy (+5 / +6 over unscoped) | true on all four legs; the **first operating-system name in the spec** |
| suffix **`(not on Windows)`** (`build/w/v/w16s2`, not issued as a diff) | +94 (+4) | same truth, one clause nearer the abort |
| real | **not priced by `--refresh`: lower bounds, vendored only**; at the 1.40 this text measured, about +7 real | |

**My answer: unscoped, knowingly Windows-false, with the gap named in the synthesis, and the scoped form priced above
for the coordinator who chooses the other way.** Reasons: (a) CLAUDE.md § 12, *spec beats compiler: the compiler has
the bug*: the Windows runtime owes the probe (panel 247 R10 left it as *"the box's question, unrun"*); writing the gap
into the language puts a platform name in a document that has none and obliges a spec edit, a panel and a re-measure
the day the probe lands; (b) line 427 is already unscoped and false in the same way, so a scoped second sentence
would say *Windows runs it* beside a first one that says *never*; (c) a reader on Windows who trusts the spec is
only ever refused *more*, never less, than the runtime does. **What I did not find**: an open issue for the Windows
probe (`gh issue list --repo heroes-lang/heroes --state all --search 'Windows signal handler probe callback
in:title,body' --limit 12`: four results, 2494, 2519, 2517, 2563, all CLOSED, none about a Windows probe; a search
by those words only, not a proof). **A condition on the landing**: file it.

**For the FINAL text the question is moot**: `w26` says the callback *may* abort, which is **not falsified on the
`windows-latest` leg** (no callback is ever refused there, so a *may* never fires and is never false), and it names no
platform. **Scoping it anyway costs** `(on Linux and macOS)` as a suffix **+106 against +101 (+5 vendored)**, not priced by
`--refresh` (a lower bound, vendored only). **I would not scope it**: the question that remains is line 427's, which is
the runtime lane's and the coordinator's to file.

**Route A**: my text has **no clause that holds only on a decode-verified build** (no *"wherever the buffer lives"*,
no *"whatever memory"*): its positive clause is a list of calls, and on the route-A builds panel 255 names (Darwin
x86-64, glibc's four other architectures, a future C library) a `longjmp` through a heap buffer is refused though
the sentence says a `longjmp` out of the handler ends it; **none of those is a CI leg** (the four legs above), and I
say so rather than scope them. **The adopted Linux build on the CI's glibc 2.43 is the engineer's, pending**; the
sentence's positive clause does not depend on it (a plain `longjmp`'s record is what the twelve goldens pin).

## 8. Item 5: Principle 0, §1.2 and the payment; the instruments that hold the sentence's nouns

**What it adds**: no form, no keyword, no production, no check the compiler needs: it is a sentence of §13 about
an event the runtime already refuses. **Its Part 11 effect**: none measured. **Why it is in the sitting**: the
author's *1a/2a* of 2026-11-23 (panel 255's `## Author's verdict`, the recording session's *"meant as"* gloss) and
the rule of 2026-11-22 (`691531908`: *the most robust solution always wins, ... the spec's size included*; the cost
is measured and written, never a veto). Principle 0 (CLAUDE.md § 2) governs what *enters the language*; this enters
nothing, so **its burden is not the operative rule here and I do not claim it met**: the operative rules are the
author's order and the robustness precedence (rank 3 over rank 4): a correct program refused with words the spec
never explains is the thing R4 itself names.

**§1.2 (real cost = program tokens x (1 + rewrite rate))**: the sentence costs **+138 real tokens in every
session that reads the spec** (measured, refresh 4, the final text; +126 for the first pass). It lowers the rewrite rate **only of a program that binds a C
library whose signal handler leaves by something other than a return or a `longjmp`**. One correction round-trip is
500 to 2000 tokens (§1.2). The break-even is therefore `P(such a program) x (drop in its rewrite rate) x (500 to
2000) = 138`, i.e. `P x drop` between **0.07 and 0.28** (138 over 500 to 2000): with a drop of at most 1 (no rewrite at all), **at least 7% to
28% of the sessions that read the spec would have to meet such a library**. I know of no measure that says they do,
and I think they do not. **So the sentence does not pay on §1.2's arithmetic** and I do not pretend it does: **it is
justified by the author's order and by robustness, not by a measured reduction of the rewrite rate.**

**The payment, by §1.6's own rule** (quoted in section 1: *a named removal measured in the commit that spends it,
or a registered prediction naming an instrument that exists on the day of registration and the milestone that
scores it; otherwise it pays nothing*):

| payment | what it is worth |
|---|---|
| a named removal | **5 vendored tokens** at most (`, nor called by one`, section 6), which I do not recommend taking; I name **no other removal that removes nothing true**, and a search of the spec for a sentence this one makes redundant found none (`grep -n -i -E 'handler|callback'` prints lines 427, 428 and the `callback` mentions of §13's other paragraphs; none states a rule the new sentence repeats) |
| a prediction on **the rewrite rate** (D1 to D3, the blind run) | **pays nothing today**: the instrument is panel 256's runner adapted (`llm-ergonomist.md`), it exists as a registered, **unrun** instrument and no paid run is authorized; §1.6: *"A prediction naming an instrument nobody has built is still registered as an observation; it simply pays nothing."* (the runner exists, but its use is not authorized: I treat it as not available) |
| a prediction on **the sentence's truth** at an instrument that exists | **payable**: `./heroes run tests/harness/main.hero -- <compiler> run fixedbugs-2522` is a compile and a diagnostic transcript, scored at the runtime lane's landing: **23 passed, 2 failed on both adopted builds before the landing, 25 passed, 0 failed after** (the engineer measured 23 / 2 on C2679, section 1 of her report; the 2 are R2's `.expected`), and each clause of my sentence (the final text `w26`) maps to a golden that exists or is owed: return and `longjmp` family (the twelve `a-callback-after-*`), inline jump (`...a-correct-inline-jump-out-of-a-handler-is-refused-in-words-never-false`), a jump within (`...jumps-within-itself-...`, two goldens), `exit()` (`fixedbugs-2494-...-atexit-...`), `swapcontext` (`g1_0`, `g1_1`, `g2`, `g3`), `setcontext` out (`b_0`, `b_1`), a jump onto a `makecontext` stack (`as_0`, `fp3`): **no `run` golden holds any of the three** (`grep -l -E 'swapcontext|setcontext|makecontext' tests/golden/run/*` printed nothing; the one file that matches `getcontext` is `fixedbugs-571-a-deprecated-libc-function-...`, unrelated). **Three clauses of my sentence have no golden, and two more are the engineer's programs and no golden: the depth clause (`p4d`: runs at 4,608 bytes below `run_case`'s frame, S from 5,120, and `p4s`: runs) and the thread clause (`b1`, `b2`: run).** That is new work the landing owes (R5 names *R3's union, when it lands, as run goldens*), and the sentence should not land without it; **a *may* clause is held only by the goldens that show both outcomes** (a shallow callback that runs and a deep one that aborts), which is the shape the engineer's `p4s` and `p4d` already have. |

**Which instrument holds the sentence's nouns (*not seen to end*) to the words the runtime prints? None.**
Searched: `grep -rn -F 'seen to end' selfhost tests/harness runtime --include='*.hero' --include='*.c' --include='*.h'`
printed **two lines, `runtime/parts/sigguard.c:443` and `:449`** (the S and U words), and
`grep -l -F 'seen to end' tests/golden/run/*.expected | wc -l` printed **17** (the runtime's side: those goldens go red
if the runtime re-words the phrase). **No test reads the spec's side** (the files that read `spec/heroes-spec.md`:
`tests/harness/spec_text.hero`, `suite_grammar.hero`, `suite_records.hero`, `suite_spec.hero`, `suite_surface.hero`,
`suite_special.hero`, `thesis_*.hero`, `fix_names.hero`, `package_judge.hero`, and in `selfhost/` `cli/measure.hero`,
`cli/table.hero`, `cli/grammar.hero`, `cli/file_operand.hero`, `measure/{pieces,pinned,judged}.hero`, and
`emit/callback_thunk.hero`, of which only the last reads §13 and only from `No function of the program is a signal
handler` to the end of its line). **The lane `lane-p255-impl` is re-wording the very words** (the critic's reading:
its uncommitted `sigguard.c` diff touches *record*, *seen to end* and *REMEDY*), and the spec's tail has drifted from the
runtime's constant **twice in a week** (batch 36's `cd693eec1`; `c0476dff3`'s mailbox clause, red today). **A
sentence that quotes the runtime's noun and is held by nothing is the shape defect 2533 cost.**

**A new check, priced as the work it is** (a draft, **unrun**): one compiler test beside the 2533 one, in
`selfhost/emit/callback_thunk.hero` (375 lines today, `wc -l`, already past `module-shape.md`'s ~300: the lane may have
to put it in a module of its own): read the spec, assert it holds `saying the handler was not seen to end`; read
`runtime/parts/sigguard.c`, join the string literals of `hero_signal_refuse`'s S arm as the 2533 test joins
`HERO_SIGNAL_REMEDY`'s (`pieces.join`), assert the joined text holds `not been seen to end`. **About 20 lines of
Heroes, one test, gated by `heroes test selfhost/main.hero` (the critic's narrowed run of the 2533 test took 2:07
real at load ~20: indicative)**; it does not hold the K words (*called while handling a signal*), which print only
where the route did not see the handler begin, so my sentence's *"saying the handler was not seen to end"* is exact for
every refusal the route sees begin and **inexact for a K-only refusal** (a handler no wrapper or trampoline saw, caught
by the exact test or the walk): the sentence promises S words there and the runtime prints K's.

## 9. The finding that changed my text: the abort after an unseen exit is a stack-depth test (the engineer, section 4(a))

**Read at 11:38 in `257-reports/compiler-engineer.md` § 4**, measured by her on Linux arm64, **3 of 3 in each of four
builds, today's runtime and C2679 alike** (so the adopted build does not change it):

| program (hers) | what it does | outcome |
|---|---|---|
| `p4n` | p4's inline jump out of the handler, then no callback | exit 0 |
| `p4a` | the same, then a Heroes `atexit` function at `main`'s natural end (spec line 427's own example) | **runs** |
| `p4s` | p4 with the callback called from `run_case`'s own frame, no padding | **runs** (`answer ran for 7`, `result 70`) |
| `p4d`, `PAD` = 0, 1024, 2048, 3072, 4096, 4608 bytes deeper | | **runs** |
| `p4d`, `PAD` = 5120, 6144, 8192, 16384, 65536 | | **134 S** |
| `p4x` | the inline jump, then a second signal whose handler returns at `run_case`'s depth, then p4's deep callback | **runs**; p4 itself aborts |
| `b1`, `b2` | a callback called on **another thread** while the first thread's handler runs / has been left by the inline jump | **runs** |

Her reading of the mechanism (`siginstall.c:150-160, 212-221`): the thunk's frame is compared with the dead handler's
wrapper frame and an entry is kept only where the wrapper's address is above the frame; *"'Held running' is therefore
not a state the handler is in until something ends it: it is a record of a stack address."* **This falsifies the
first clause of every arm, and of my first pass: `a callback called while a handler has not ended aborts` is false
of the natural program** (the one that jumps out of the handler to where it was and calls its callback from there).
Panel 255's R4 had the words already (*"`ub_*` (a callback **above** a handler left by an inline jump)"*), and no arm
carried them.

**What I changed** (the brief: *"your report names which finding made you change a clause"*): the abort is no longer
promised. For a handler that has **not** left (it calls the callback itself, or through `exit` or `qsort`), line 427's
*"nor called by one"* already says everything and the runtime's refusal is depth-independent (the callee is below
the wrapper by construction), so **my sentence no longer restates it**; for a handler that has left **by an exit the
runtime does not follow**, the consequence is *"a callback called later on its thread from deeper in the stack than the
handler was **may** abort, saying the handler was not seen to end"*: **true of every row she measured** (shallow runs,
deep aborts, a later handler clears, another thread runs), and **true on the Windows leg too** (where the runtime has no
probe and no callback is ever refused: *may* is not falsified there), so **the Windows scope words are no longer
needed for this sentence**, and the clause line 427 is already false on Windows for is not mine. **Her p4 and p4d are
the first program that falsified an arm's *clause* in this panel (the critic's p10 and p3 falsified f-arms' *predecessors*).**

**The cost of the honest form**: a *may* is a weaker sentence and tells the reader less; I think it is the only one the
runtime can keep, and **the runtime lane owes a decision the sitting should name**: either make the abort unconditional
(it cannot know a shallow callback is called inside a handler that was left by a jump it never saw, so it would have to
refuse every callback after an entry whose end it never saw, which is the *"refuse more"* conservative route) or accept
that after an exit it does not follow the language's refusal is best-effort, and say so (this sentence). **Neither is mine
to choose; the sentence says the second because it is what the runtime does.**

**Vendored counts of the depth-aware forms, from the saved original** (`build/spec.orig`; **a first run of these four
read +193, +183, +178 and +188 because my `mk.py` read `spec/heroes-spec.md` while the background `spec` suite
job had my first candidate swapped over it, so each included `w16`'s +90; I found it by the arithmetic and re-ran from
`build/spec.orig`, sha256 `15804f3f...`, the numbers below**):

| text | what it is | vendored cl100k_base (delta) |
|---|---|---|
| `w23` | the exits that end a handler; the three that do not; *"a callback called on its thread after that, from deeper in the stack than it was, may abort, saying the handler was not seen to end"* | 9,864 (**+103**) |
| `w24` | `w23` without the depth | 9,854 (+93) |
| `w25` | `w23` without `setcontext` and without the `makecontext` jump | 9,849 (+88) |
| `w23` folded (`, nor called by one` removed) | | 9,859 (+98) |

## 3b. The `spec` suite on my first-pass text (item 5: *"confirm on your text"*)

`./heroes-next run tests/harness/main.hero -- ./heroes-next spec` in my copy, `spec/heroes-spec.md` swapped for
`w16` (`spec-w1.diff` applied) by a background job and **restored by the same job** (its last line, `shasum -a 256`,
read `15804f3fb52a62c208bc59efce76aa0e55a48435f879471bff968bd4f213c427`, the original's; the log is `build/spec-suite-w1.log`
of my copy). Result: **`harness: 31 passed, 4 failed`, the four being `spec/budget`, `spec/spendable`, `spec/real`,
`spec/ledger`**: **the critic's four, the pins the landing moves** (`spec/spendable`: *"`measure` does not say 'is 12869
and the check goes red at 16384'"*; `spec/ledger`: *"the ledger's newest row says 9761 and the compiler measures
9851"*), and **none of `named`, `rejected`, `inventory`, `anchors`, `offered`, `shape`**: the code spans `setcontext`,
`swapcontext`, `makecontext`, `_longjmp`, `siglongjmp` and `longjmp` are admissible. **Duration**: about 6 minutes at a
load of 50 to 130 (indicative, in no verdict). **To re-run on the final text** (section 9 changed the sentence; the
code spans of the final text are the same names).

## 3c. The fourth count: my final text `w26` (`spec-w7.diff`), prediction registered at 11:44 (`date` printed `11:44:30`; I first wrote 11:47 and corrected it) before the request

The three counts of section 3 read real / vendored **1.40, 1.32, 1.31** (mean 1.34). `w26`'s vendored delta is **+101**
(section 9's table, from `build/spec.orig`), so **C6: real delta +134, range +122 to +146**, i.e. a real count of
12,943 (12,931 to 12,955).

**Result of refresh 4 (about 11:44 by `date`; the file was written a minute later and I first wrote 11:46)**: `./heroes-next measure spec/heroes-spec.md --refresh` printed
`SPEC_REAL_TOKENS` **12947**, `SPEC_REAL_MODEL` `claude-opus-5`, `SPEC_REAL_TAKEN` `2026-11-23`, `SPEC_DIGEST`
`5d451c5c7bf639da`; the restored file's sha256 was `15804f3fb52a62c208bc59efce76aa0e55a48435f879471bff968bd4f213c427`
(the original's, printed in the same command), key length printed **108**, never the value. **All four counts are
spent**; the files of the four requests are `build/refresh-{1-w16,2-f3,3-f1b,4-w26}.txt` of my copy (removed with
`build/` at the end; each holds the `measure` output only).

**The budget of the final text, against §1.6's 16,384**: before **12,809**; after **12,947**, **+138**; headroom
**16,384 - 12,947 = 3,437**; with the FFI floor's 60 (`measure`'s own line, panel 030 R3) the number measured against
the ceiling is **13,007**, leaving **3,377**. **Under the rule of 2026-11-22 (`691531908`) this is a cost to be
written, not a veto, and it is far from the ceiling.** The other three real counts, for the coordinator's ranking:
`f3` **+58**, `f1b` **+111**, my first pass `w16` **+126**. **Not priced by `--refresh`: lower bounds, vendored only**:
`f1` +93, `f2` +87, the fold of `w26` +96, the merge of `w26` into line 427's first clause +94, the REMEDY-placement of
`w26` +102, the scope suffix `(on Linux and macOS)` +106 (+5 over `w26`), my core `w25` +88; at this panel's 1.31 to
1.40 these would read about 1.3 to 1.4 times as much, an inference.

## 10. Every clause of my final text against the cells the engineer ran (Linux arm64) and the tables of panel 255 (Darwin)

Read at about 11:45 in `257-reports/compiler-engineer.md` §§ 4 to 6 (her cells, 3 of 3, `-O0`, plain and `--sanitize`,
today's runtime and C2679). **The ffi-pragmatist's own 257 cells appeared at 11:50, after I wrote this table**
(`257-reports/ffi-pragmatist.md` §§ 1 and 2: Darwin today and C4r, 3 runs a cell, plain, `-O2` and `--sanitize`): **I
have updated the Darwin cells below that her table covers and marked them *257*; the ones it does not cover (`b_0`/`b_1`
and any depth sweep on Darwin) stay *panel 255* or *unmeasured***.

| clause of `w26` (`spec-w7`) | Darwin C4r | Linux C2679 (adopted) | the program(s) |
|---|---|---|---|
| *ends by its return* | holds (critic's key: p5) | holds (engineer: p5) | p5 |
| *or by a `longjmp`, `siglongjmp` or `_longjmp` out of it* | holds (critic: p1, p2, p3, p9, p10 on C4r) | **holds** (engineer: p1, p2, p3, p9, p10, `x_*`, `jo_*`, the twelve goldens) | p1, p2, p3, p9, p10 |
| *or by a `setcontext` out of it* | holds (**panel 255**, pragmatist: `b_0`, `b_1` run on C4r; **absent from her 257 table**) | holds (panel 255, engineer: `b_0`, `b_1` run on C2679) | `b_0`, `b_1`: **not re-run in 257 by either seat** |
| *left by an inline jump ... is still taken to be running* | **holds, *257***: p4 and the golden `134 S` on C4r (her § 1) | holds | p4 (S, both) |
| *a callback called later ... from deeper in the stack than the handler was* | **unmeasured**: no depth sweep exists on Darwin (I did not read `rebind.c`'s prune rule; panel 255's G rows all call 64 KiB deeper: `r2new.h:416`, *"The runner then calls the program 64 KiB deeper"*), so the clause is *a sufficient condition* there, never a claim that a shallower callback runs | **measured**: runs at 4,608 bytes, S from 5,120 (the engineer's `p4d`); `p4s` runs | `p4d`, `p4s` |
| *... may abort* | consistent with every row (a *may* is not falsified by a program that runs) | the same | `p4x`, `p4a`, `p4s`, `b1`, `b2` run; `p4`, `p4b`, `p4d`>=5120 abort |
| *on its thread* | source only (`rebind.c:124-127`) | **measured**: `b1`, `b2` run | `b1`, `b2` |
| *... by a `swapcontext` away from it* | **holds, *257***: `p7`, `g1_0`, `g1_1`, `g2`, `g2_1`, `g3`, `nn`, `sched`, `watchdog_b` all **134 S** on C4r, 3 of 3 plain and `--sanitize` (her § 1); **run today** | **FALSE until defect 2702**: p7 and 9 of the 11 other G programs **run**; only `g_3_0`, `g3` are refused | p7, `g_1_0`..`g_3_1` |
| *... or a jump onto another stack given to `makecontext`* | **holds, *257***: `fp3`, `as_0` **134 S** on C4r, plain and `--sanitize`, 3 of 3 (her § 1, row C8); run today | **FALSE until defect 2702**: `fp3`, `as_0`, `cs_1` **run** | `fp3`, `as_0` |
| *saying the handler was not seen to end* | S words | S words (the K words print where no route saw the handler begin) | p4, p6, p8: the engineer's first-line copies |

**Two clauses of my final text wait for defect 2702 on Linux** (`swapcontext`, the `makecontext` jump), exactly the
brief's *"the clause waits for which defect"*, and the engineer's own line says the same of the arms. **A text true on
both builds as they stand is `w27`** (`spec-w12.diff`): *"left by an inline jump"* alone, vendored **+76**
(real **not priced: a lower bound, vendored only**), true of every cell above, and **silent on R4's context
refusals**, so it does not do what the author's *1a/2a* ask for `g1_0`, `g1_1`, `g2`, `fp3` and `as_0`. **My
recommendation: adopt `w26`, and have the landing sequence it with defect 2702** (the sentence enters the spec only
through the runtime lane's landing, § 7 of the shared brief): if 2702 has not landed when the sentence does, land
`w27` and add the two clauses at 2702's landing. **§12 of CLAUDE.md** (*spec beats compiler*) is what makes the first
order right: R4 ratified *G refused on both platforms*, so the Linux build, not the sentence, is the one that is
behind.

## Predictions registered before the blind run

**Registered 11:47 by this Mac's `date`, 2026-11-23. The run is NOT RUN in this sitting (no paid run is authorized)**;
these are predictions, not results, copied and hashed by the runner when it starts. I read `257-briefs/llm-ergonomist.md`
(the arms `now`, `adopted`, `rival`; D1 to D3), `blind/task-t1.md`, `task-t2.md`, `answers.md` and the seven programs'
files; no session folder exists. **Calibration, read before writing them**: panel 256's warden wrote, and its blind seat
confirmed (`git show rebel-round:docs/panel/256-reports/llm-ergonomist-blind.md`, *"Every count of right sessions the
warden missed, it missed low"*; panel 254 the same), that Opus 5.5 readers do better than a warden predicts. **So I
predict 5 of 5 wherever the text states the row's rule in words a reader meets, and below that only where the text is
ambiguous or where the key and the text disagree.** The key is `answers.md`'s *adopted* column (*the landed build's own
run, on each platform*); **p7 splits across platforms until defect 2702 lands** (Linux C2679 runs it, 3 of 3, the
engineer's table), so D1 **rests on p4 alone** while it does, as `llm-ergonomist.md` says. Every cell is
*point (range)*, 5 sessions per cell. **Three leaks I name so a reader of the result does not credit the sentence for
them**: (1) `task-t2.md`'s own question 3 presents the abort words and says *"for the design you think aborts"*, so a
`now` reader may infer that B is the one (the message's *"in a way this runtime does not follow"*); I give `now` a wide
range on t2 B for that. (2) `programs/prelude.h` says the call is made *"from a frame 64 KiB deeper ... no signal
running"*, a depth cue that helps a reader of `w26` (*"from deeper in the stack than the handler was"*) and no other
arm. (3) the engineer's depth sweep (section 9) means that **p4's key, ABORTS, is true because p4 calls 64 KiB deeper**;
a reader of an arm that promises *aborts* unconditionally is right on p4 for a reason that is false of `p4s`.

**t1: right count of 5 on each of p1 to p7** (p6 stays 5 (4 to 5) under every text: line 427's *"nor called by one"* is in
every arm):

| text | p1 | p2 | p3 | p4 | p5 | p6 | p7 (while it splits: out of D1) |
|---|---|---|---|---|---|---|---|
| `now` | 5 (4-5) | 5 (4-5) | 4 (3-5) | **1 (0-2)** | 5 (5-5) | 5 (4-5) | **1 (0-2)** |
| `arm-f1` | 5 (4-5) | 5 (4-5) | 4 (2-5) | 5 (4-5) | 5 (5-5) | 5 (4-5) | 5 (4-5) |
| `arm-f1b` | 5 (4-5) | 5 (4-5) | 4 (2-5) | 5 (4-5) | 5 (5-5) | 5 (4-5) | 4 (2-5) |
| `arm-f2` | 5 (4-5) | 5 (4-5) | 5 (4-5) | 5 (4-5) | 5 (5-5) | 5 (4-5) | 5 (4-5) |
| `arm-f3` | 5 (4-5) | 5 (4-5) | 4 (3-5) | 4 (2-5) | 5 (5-5) | 5 (4-5) | 3 (2-5) |
| **`w26`** (`spec-w7`, mine) | 5 (4-5) | 5 (4-5) | 5 (4-5) | 4 (3-5) | 5 (5-5) | 5 (4-5) | 4 (3-5) |
| `w27` (`spec-w12`, the true-on-both fallback) | 5 (4-5) | 5 (4-5) | 5 (4-5) | 4 (3-5) | 5 (5-5) | 5 (4-5) | 1 (0-3) (silent on `swapcontext`) |

*Why the cells are where they are*: p3 is lower under the arms that say *"where this runtime sees"* because a reader may
doubt that a copied buffer is *seen*; `w26` and `f2` say nothing of buffers (*out of it*, *wherever the buffer lives*).
`w26`'s p4 and p7 are 4 and not 5 because **it says *may***: a reader must decide between ABORTS and RUNS for a
sentence that promises neither, and I expect one of five to say RUNS. `f3`'s p4 and p7 are lowest because it names no
refused program. `now` p4 and p7 are 1 because nothing in the frozen spec says a jump out of a handler matters.

**t2: right count of 5 on question 1 (does the callback run) for A, B, C, and on question 3 (whose mistake, for the
design the reader thinks aborts; key *neither*)**:

| text | Q1 A (runs) | Q1 B (aborts) | Q1 C (runs) | Q3 *neither* | Q3 *the library's C is wrong* |
|---|---|---|---|---|---|
| `now` | 5 (4-5) | **3 (1-5)** | 5 (4-5) | 4 (3-5) | 1 (0-2) |
| `arm-f1` / `arm-f2` | 5 (4-5) | 5 (4-5) | 5 (4-5) | 4 (3-5) | 1 (0-2) |
| `arm-f1b` | 5 (4-5) | 4 (2-5) | 5 (4-5) | 4 (3-5) | 1 (0-2) |
| `arm-f3` | 5 (4-5) | 4 (2-5) | 5 (4-5) | 4 (3-5) | 1 (0-2) |
| **`w26`** | 5 (4-5) | 5 (4-5) | 5 (4-5) | 4 (3-5) | 1 (0-1) |
| `w27` | 5 (4-5) | 3 (1-5) | 5 (4-5) | 4 (3-5) | 1 (0-2) |

**The decision rules** (`llm-ergonomist.md`): **D1** (`adopted` p4, and p7 where it agrees, each at least 4 of 5, and
`now` at most 2 of 5 on one of them): holds for `f1`, `f2` (about 85%); for `f1b` and `w26` on p4 alone (about 65%, the
*4 (3-5)* is the boundary); for `f3` it is a coin flip (about 50%: its p4 is 4 (2-5), its p7 3 (2-5)); `now`'s side holds
(p4 = 1). **D2** (p1, p2, p3, p5 fall by no more than 1 against `now`): holds for every text (about 85%; the risk is p3
under `f1`, `f1b` at 2 or 3). **D3** (at least 4 of 5 say *neither* for B, at most 1 say *the library's C is wrong*):
holds for every text including `now` (about 65%: the **message's** wording, equal in every arm, does the work, so a
sentence can move D3 very little; **if D3 fails it fails for all arms alike and is no argument for or against a text**).
**Spend**: the 30 graded sessions about **7.2 USD** (mean 0.24, range 0.18 to 0.32; 5.5 to 9.5 in all), the verdict
session about 0.3 to 0.5; **the verdict runs** (it needs the 30 at 10.90 USD or less: about 92%). The sentence's own
tokens move the spend by under 0.01 USD (a 138-token difference over 30 sessions).

**Which text I expect the synthesis to adopt**: `w26` as `adopted` (or a coordinator's repair of `f1b` that carries its
depth clause), with `f1b` or `w27` as `rival`; **which one I would adopt**: `w26`, sequenced with defect 2702, `w27` if the
sentence must land before it.

**Count predictions, scored** (section 3): C1 `w16` +116 (106 to 126), counted **+126** (top edge); C2 `f3` +57 (52 to
62), counted **+58**; C3 `f1b` +110 (100 to 119), counted **+111**; C6 `w26` +134 (122 to 146), counted **+138**. **All four
inside their ranges; the ratio real / vendored was 1.40, 1.32, 1.31, 1.37** against panel 256's 1.23 to 1.41.

## 3d. The `spec` suite on my FINAL text `w26` (`spec-w7.diff`)

The same command and the same swap-and-restore job as section 3b, started 11:44:57, finished before 11:48:12
(`build/spec-suite-w7.log` of my copy): **`harness: 31 passed, 4 failed`, the four `spec/budget`, `spec/spendable`,
`spec/real`, `spec/ledger`** (`spec/ledger`: *"the ledger's newest row says 9761 and the compiler measures 9863"*), none
of `named`, `rejected`, `inventory`, `anchors`, `offered`, `shape`; the last line of the log, the restored file's
`shasum -a 256`, equals the original's, and `cmp spec/heroes-spec.md build/spec.orig` printed nothing afterwards. **The
four pins are the landing's** (`SPEC_TOKENS`, `REAL_TOKENS`, the ledger row of `docs/measurements/010-spec-budget-ledger.md`,
per `spec-shape.md` § How a change to the document is made): at the landing `SPEC_REAL_TOKENS` is **12947**, the digest
`5d451c5c7bf639da`, **if the landed spec is `spec-w7.diff` applied to `828567a30`'s**; any other text reads other
numbers. **The seat did not run the compiler's own tests** (`byte for byte (defect 2533)` is red at the frozen tree, the
mailbox clause): unrun, and not this text's.

## 11. Facts of the briefs I re-ran, and which were wrong

*Right (each by its command)*: the before count and the arms' vendored counts (section 1); `sed -n 428p | tail -c 12 | od -c`
ends `s t   c a l l \n` (no full stop); the brief's own `grep -n -i -E 'setjmp|longjmp|jmp_buf|getcontext|swapcontext|signal
handler|handler|fingerprint|save point'` prints lines 427 and 428 only; `sed -n 202p` is the abort definition; `the runtime`
occurs once; the four arms' diffs apply (`patch --dry-run`, then applied: each differs by one line); the `spec` suite reads
31 passed, 4 failed on both of my texts, the critic's four; **22** goldens carry the tail (`grep -l`).
*Wrong, or not what it says*:
1. **Shared brief § 5.1, on f1b**: *"only what is true on both of panel 255's adopted builds"*. **False**: f1b's *"left any
   other way"* includes a `swapcontext` away, and p7 runs on Linux C2679 (the engineer, 3 of 3 at four settings); dropping
   the parenthetical dropped the words and not the claim (the engineer says the same of f1b and f3).
2. **Shared brief § 5.1, the paragraph after the arms**: *"the class the runtime acts on is **a handler not seen to end**,
   not a list of causes"*. **Incomplete in a way that matters**: the runtime acts on a **stack-address test** after an exit
   it does not see (the thunk's frame against the dead wrapper's, `siginstall.c:212-221`): a callback called from the raise
   site runs and one called 5 KiB deeper aborts (the engineer's `p4s`, `p4d`), so *not seen to end* is the runtime's word
   for a record, not a state, and the class aborts only part of the time.
3. **Shared brief § 1 / § 3 on the arms' placement**: right, and measured by the critic; I add that the placement
   before `No function of the program is a signal handler` also leaves the sentence's nouns outside the only test that
   reads that line (section 8).
4. **`00-shared.md` § 6, C7 *"S on both"*** (the critic's finding, confirmed by the engineer): p7 runs on Linux C2679.

## Verdict (the structure of `.claude/agents/spec-warden.md`)

- `verdict`: **approve, of my repaired text `w26` (`spec-w7.diff`) only; object to each of f1, f1b, f2 and f3 as drafted**
  (each says an abort a running program falsifies); **no veto** (§1.6's 16,384 is far, and the rule of 2026-11-22
  makes the cost a thing to be written); **provisional on the blind reading** (registered, not run), as the sitting says.
- `section`: design.md **§1.6** (the 16,384 ceiling, measured by `claude-opus-5`; the payment rule as panel 046 amended
  it) and **§1.2** (real cost = program tokens x (1 + rewrite rate)); CLAUDE.md § 2 (Principle 0) and § 12 (*spec beats
  compiler*); `.claude/rules/spec-shape.md` (*no closed list of aborts*, *merging beats appending*).
- `spec_token_delta`: **measured**: before **12,809** real (9,585 claude-legacy, 9,761 cl100k_base); after **12,947**
  real (9,686 / 9,862): **+138** real, **+101** vendored; headroom 3,437 against 16,384, **3,377 after the FFI floor's 60**.
  The other measured counts: `f3` +58, `f1b` +111, my first pass `w16` +126. **Lower bounds, vendored only**: f1 +93, f2
  +87, `w26` merged +94, folded +96, REMEDY-placed +102, scoped +106, `w27` +76.
- `removal`: **`, nor called by one`, 5 vendored tokens (`spec-w8.diff`), which I do not recommend taking**; nothing else
  removes only what is false. **The rest is unpaid under §1.6** (the rewrite-rate prediction names a runner that exists
  and is not authorized: *it simply pays nothing*), and stands on the author's order and the rule of 2026-11-22.
- `needed_for_self_hosting`: **no**.
- `argument`: Every arm promises an abort that a running program falsifies: after an inline jump a callback called from the raise site runs, one called about 5 KiB deeper aborts (the engineer's p4s, p4d), another thread's runs, a later handler clears it; and `setcontext` out of a handler (b_0, b_1) runs though the arms' context clause forbids it. My text lists the exits the runtime follows, names the three it does not, and promises only *may abort*, so no running program falsifies it. It costs +138 real tokens and pays nothing under §1.6 beyond the author's order; two clauses wait for defect 2702 on Linux; nothing holds its nouns to the runtime's words, so a check is owed.
- `prediction`: **if the landed spec is `spec-w7.diff` applied to `828567a30`'s, `heroes measure spec/heroes-spec.md --refresh`
  reads exactly 12947 (digest `5d451c5c7bf639da`)**; **in the blind run, `w26` p4 right 4 of 5 (3 to 5), `now` p4 right 1
  of 5 (0 to 2)**, D2 holds, D3 holds for every arm alike; **at the landing `run fixedbugs-2522` reads 25 passed, 0 failed
  with goldens added that show `p4s` running, `p4d` at 5120 aborting and `b_0` running**.
- `condition`: (1) the pragmatist's Darwin depth sweep: if a G, `fp3` or `as_0` refusal holds at every depth, *may* could
  become *aborts* for those; (2) `b_0`/`b_1` refused on either build in a 257 re-run takes `setcontext` out of the list;
  (3) **defect 2702 open at the landing: land `w27` (`spec-w12.diff`, +76 vendored), add the two clauses at 2702's landing**;
  (4) a blind run failing D2 withdraws the sentence; (5) the runtime lane making the abort unconditional turns *may*
  into *aborts*; (6) no check holding the nouns: I would still approve, and list it as owed.

## One line per question of the brief (the text I would adopt, its real count; the one measurement that would change my mind)

1. **Every arm's text**: adopt **`w26`**, real **12,947 (+138)**; f1, f1b, f2, f3 are each falsified as drafted (`p4s` runs;
   `b_0`/`b_1` run; f1b's *"only what is true on both"* is false). *Would change my mind*: the pragmatist's Darwin
   cells showing every refusal in the class holds at every depth, which would let *may* become *aborts* for Darwin only.
2. **The closed list**: every arm closes the list of followed exits and is falsified by `b_0`/`b_1` (and its abort by
   `p4s`, `p4a`, `p4x`, `b1`, `b2`); mine lists the followed calls by measurement and promises only *may*. *Would change my
   mind*: a correct program that leaves a handler by a listed call and is refused (route A, or a save the runtime never saw).
3. **Where it goes and what it costs the runtime**: **before `No function of the program is a signal handler`**; the
   REMEDY route is **+102 vendored, 22 `.expected`, 399 bytes on every signal refusal, 5 of the 22 about something else and 17
   repeating themselves**; the merge saves 7 vendored and loses the rule's shape. *Would change my mind*: the runtime lane
   deciding the equality test is worth more than 5 irrelevant refusals.
4. **Scope**: **unscoped**; *may* is not false on Windows; scoping costs +5 vendored (`(on Linux and macOS)`, +106). *Would
   change my mind*: a Windows run showing the runtime refuses a callback in a handler (then line 427 is true there too).
5. **Principle 0 and the payment**: no form enters, Principle 0 is not the operative rule; the sentence **does not pay on
   §1.2's arithmetic** (it would need 7% to 28% of sessions to meet such a library) and **pays 5 tokens by removal at most**;
   **no instrument holds its nouns** (`grep -rn -F 'seen to end'` finds `sigguard.c:443, 449` and 17 goldens, no test of the
   spec's side); a check is **about 20 lines of Heroes, unrun**. *Would change my mind*: the author running the blind run
   and D1 failing for every text (then withdraw), or the coordinator adding the check (then the observation becomes a payment).

**Files**: this report; `spec-w1..w12.txt` and `.diff` (w1 to w6 the first pass, **w7 the adopted text**, w8 the fold, w9
the merge, w10 the REMEDY route, w11 the core, w12 the fallback true on both builds); `spec-w-evidence.txt` (verbatim
measure outputs and the files' sha256). **My copy**: `build/` removed (`rm -rf build`), `spec/heroes-spec.md` equal to
the frozen file (`cmp` against the saved original printed nothing before it was removed); the two compilers stay in the
copy's root, ignored by git.

**What the pragmatist's table adds that no arm and no clause of mine reaches** (her § 1, her words): **`rr_2` runs a Heroes
function inside a resumed handler on C4r** (exit 0, `answer ran for 82`, 3 of 3 both modes), *"the one row of this table where
C4r runs a program inside a signal"*; it is a jump back into a handler that had already been left, **undefined in C**, so it
is no correct program and no sentence of the spec owes it; I list it so the synthesis does not read the sentence's *may*
as covering it. Her libpng, `probe` (a SIGSEGV handler that `siglongjmp`s out after a hardware fault) and `lazy` (a handler
that returns) cells all **run** on C4r in three modes (her § 3a, § 3b), **which is the positive clause of my text, on
real C**: a handler left by `siglongjmp` after a hardware fault, and one that returns, are *"true of both"* in her words.
