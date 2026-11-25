# Panel 259, the spec-warden's brief

Read `00-shared.md` first, whole (its § 4.4 on what the specification says and where a sentence can go, and § 6 on the
routes, bear on you most), then the critic's first reading (`docs/panel/259-reports/critic-briefs.md`, written at the
first base `2cf85ea64`; the briefs were repaired from its items 1 to 9 and re-frozen at the base `3ec775b66`,
`00-shared.md` § 2; the spec is the same bytes at both). **Your agent file is the base's**
(`.claude/agents/spec-warden.md` of your copy); design.md §1.2 and §1.6 are reached by `grep -n`, never from memory.
Under the author's amendment of 2026-11-22 (CLAUDE.md § Precedence: *"the most robust solution now always wins, a new
form included and at the cost of the spec's size"*) the budget is **a cost to be measured and written, never a veto**.
Your copy is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/259-spec-warden`, a worktree of `3ec775b66`; build
its compiler and its stage 2 there (§ 3 of the shared brief). Your own repaired texts go beside your report as
`docs/panel/259-reports/spec-w*.txt` with their diffs `spec-w*.diff`.

## The counts

- **Before**, read by the repair's stage 2 at this base (`./heroes-next measure spec/heroes-spec.md` from its tree's
  root, no `--refresh`, 07:46; the clerk's at `2cf85ea64` read the same): claude-legacy **9,877**, cl100k_base
  **10,054** (the vendored maximum), **real 13,185** (`claude-opus-5`, the pin of 2026-11-23), headroom **3,199**
  against **16,384**, the FFI floor mortgaging 60 (13,245 against the ceiling). The file: 516 lines, 34,437 bytes,
  sha256 `c915f0f2d47f35c6b3162d198c27bedfbb3fe2b962e05185a878619356372bee`. **Re-read them yourself** and say if any
  differs.
- **The candidate sentences**: `arms/arm-r.txt` (a told refusal), `arms/arm-t.txt` (a switch the binding tells, with a
  runtime entry point whose name, `hero_stack_entered`, is the clerk's placeholder: `grep -rn hero_stack_entered
  runtime selfhost spec` prints nothing), `arms/arm-p.txt` (a promise). Each has a diff, `arms/arm-*.diff`, that
  puts it **before** `No function of the program is a signal handler` on line 428 and applies to the base
  (`patch --dry-run -p1 < arms/arm-<x>.diff` printed `patching file 'spec/heroes-spec.md'`, three of three, the repair
  at this base; each applied copy holds its `.txt` once, `grep -c -F -f`). **They are drafts by the clerk; every clause
  of each is a claim to be tested and repaired, and none is ranked.** The critic's F18 marks clauses the runs already
  decide.
- **The vendored counts**, measured by the critic at `2cf85ea64` and again by the repair at this base, the same
  numbers (`./heroes-next measure <applied copy>`, offline, **from the tree's root**):

  | arm | bytes | claude-legacy | cl100k_base | vendored delta (max) |
  |---|---|---|---|---|
  | base | 34,437 | 9,877 | 10,054 | |
  | `arm-p` | 34,575 | 9,909 | 10,086 | +32 |
  | `arm-r` | 34,704 | 9,945 | 10,123 | +69 |
  | `arm-t` | 34,723 | 9,954 | 10,131 | +77 |

  **Run `measure` from your copy's root**: it reads `vendor/tokenizers/` from the directory it runs in, so run from
  anywhere else it prints *"error: cannot read `vendor/tokenizers/claude-legacy.json`"* and exits 2 (the repair, 07:47,
  run from inside an applied copy); and for any file but the pinned spec it prints the two vendored rows and **no
  `real` row**. Measure your own texts the same way. **A vendored delta is not a price**
  (`.claude/rules/spec-shape.md` § How a change to the document is made): **the real count** needs `./heroes-next
  measure spec/heroes-spec.md --refresh`, one request to the count-tokens endpoint each, **only where the coordinator's
  prompt authorises it, and then at most four**: `cp spec/heroes-spec.md spec.orig` and its `shasum -a 256`, apply the
  text, `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a` (print `${#ANTHROPIC_API_KEY}` alone, never the
  value), `--refresh`, then restore with `cp spec.orig spec/heroes-spec.md` and compare the `shasum` before any other
  command. Without that authorisation every count is written **a lower bound, vendored only**, in those words.

## What you judge

1. **Is a sentence owed at all, and by which route?** R1 owed none (panel 258's sitting). Which route of § 6 changes
   what a reader of the spec can predict about a program (`blind/programs/q4.hero`, the defect's shape written for a
   reader, is the case; today it prints a wrong value then aborts on this Mac and ends by SIGSEGV, nothing printed, in
   the Linux arm64 image at this base and in the x86-64 one at `2cf85ea64`, `blind/answers.md`), and which changes only
   the runtime? A route that owes none says why a reader loses nothing.
2. **Each draft's text.** Does each sentence say one thing, with the document's words (*abort*, § 6 line 202,
   *"An abort ends the program at once, saying why"*; *callback*; *handler*; *seen*, which line 428 already uses:
   *"saying the handler was not seen to end"*)? Can a reader apply **"inside the stack of the thread that called
   C"** to a program they have not run (`below`, `00-shared.md` § 5: an array in the extern call's own frame, every
   coroutine frame inside the open call), or **"code of its own"** (on Linux `unseen_carved` switches by the runtime's
   own `swapcontext` and is refused, § 5), or **"followed"** (a `makecontext` reached by `dlsym(RTLD_NEXT, ...)` is
   not, on Linux, § 4.2)? A sentence a reader can take two ways is a defect of the text: name it, repair it in your own
   `spec-w*.txt`, measure it. **A *may* is graded now** (`llm-ergonomist.md` § The key and D2w): a text that says a
   class *may* abort leaves its programs `NOT PROMISED` to a reader, cannot pass D1 for q4, and fails D2w where its
   class holds a program that keeps its value (q7, 512-byte records on q4's switch, prints on every platform): say for
   each text whether q7 lies in its class.
3. **The closed list.** `.claude/rules/spec-shape.md:57-60`: *"no section lists every abort"*, a closed list vetoed
   and falsified by a running program at panel 087. `arm-r` names what is followed (`malloc`, `mmap`,
   `makecontext`): is that a closed list, and **which running program falsifies it** (the seats' tables: a stack from
   a library's own allocator, a stack copied into a shared region, a thread's stack the program made with
   `pthread_attr_setstack`, `below`'s stack in the extern call's own frame, a `makecontext` reached through
   `RTLD_NEXT`)? A class against a list of causes: which survives the next program?
4. **Where it goes, and merging.** Before `No function of the program is a signal handler` (the compiler's own test
   at `selfhost/emit/callback_thunk.hero:408` holds the rest of line 428 equal to `HERO_SIGNAL_REMEDY`, shared brief
   § 4.4), or § 9 beside line 331 (*"running out of stack aborts"*), or § 13 beside lines 465 to 466 (*"A lend lives
   for its call"*). **Merging beats appending** (`spec-shape.md:67-69`): can the sentence MERGE into line 428's handler
   sentence, which already names `swapcontext` and `makecontext`, or into line 427's *"A callback is a
   parameter"*? Write the merged text and measure it.
5. **Principle 0, Part 6 and the payment.** design.md Part 6 refuses *"Coroutines"* as a form of the language, *"on
   cost alone"* (`grep -n -i coroutine docs/design.md`, lines 3593 and 3624); every coroutine here is a C library's,
   reached by a callback. Does a sentence that names C's switches contradict that row, or state a fact of the C
   boundary (§1.11, §1.12)? `arm-t` adds a form (a function of the runtime's header a binding calls): what does
   design.md §1.6's payment rule ask of it, a named removal or a registered prediction naming **an instrument that
   exists** (grep the `spec`, `unseen` and `records` suites), and which instrument would hold the sentence's nouns to
   the words the runtime prints, as `callback_thunk.hero:408` holds line 428's tail and
   `tests/harness/spec_refusals.hero` holds each arm of `hero_signal_refuse` to line 428's two claims? That pin moved
   with step 179a's `HELD` arm (shared brief § 1 item 5), which this sitting ratifies or refuses: do its words keep
   one of line 428's claims, and does a sentence of this sitting change what they must say?
6. **Scope.** The arms say nothing of Windows, where the runtime reads the thread block (`stack_lane.c:89-100`) and
   has no jump hook (shared brief § 4.3), nor of the Linux handles (§ 4.2, a door the `dlsym-make` lane is moving now):
   true there, knowingly false there, or scoped, and what the scoping words cost.

## Predictions registered before the blind run

**Your report must hold the heading `## Predictions registered before the blind run`**, with your predictions under it,
**before any blind session starts** (the runner copies your report, hashed, as it starts). **The run is NOT RUN in
this sitting** and is registered: read `llm-ergonomist.md` (the arms `now`, `adopted`, `rival`, the key and its two
graded platforms, build-right and text-right, the rows, D1, D2, D2w and D3) and every file of `blind/`; never a
session's folder (none exists). **Predict per candidate text** (`arm-r`, `arm-t`, `arm-p` and your own), each a number
and a range: for t1, the build-right count of 5 on each of q1 to q7 under `now` and under that text, and the `NOT
PROMISED` count on q1, q2, q3, q6 and q7; for t2, the build-right count of 5 on each question; whether D1, D2, D2w and
D3 hold; the spend. Say which text you expect to be adopted.

End with the verdict structure of your agent file, then one line per numbered question: the text you would adopt (its
count, real or *a lower bound, vendored only*), and the one measurement that would change your mind.
