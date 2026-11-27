# Panel 262, the spec-warden's brief

Read `00-shared.md` first, whole (its head: the repair and the move; § 1 the proposal, § 2 the questions, § 3.1 the
sentence and its neighbours, § 3.2 how it has been read, § 3.3 the words, § 3.5 the defects, § 3.6 the record, § 3.7 the
counts, § 4 the candidates), then the critic's first reading (`docs/panel/262-reports/critic-briefs.md`, C1 to C28), on
which these briefs were repaired before any seat starts. **Your agent file is the base's** (`.claude/agents/spec-warden.md`
of your copy); design.md §1.2 and §1.6 are reached by `grep -n`. Under the author's amendment of 2026-11-22 (CLAUDE.md
§ Precedence: *"the most robust solution now always wins, a new form included and at the cost of the spec's size"*),
**the budget and Principle 0 are costs to be measured and written, never a veto**; your veto on the soundness of a text
(a sentence that says what the runtime does not do, on a platform it claims) stands.

**Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/262-spec-warden`, a detached worktree at the
base **`87003cb42`**; build its compiler from the seed and stage 2 there (`00-shared.md` § 6), **`sources
6ce3f9e116d22e3a`**. **The texts are yours**: write each beside your report as `docs/panel/262-reports/spec-w*.txt`,
with its diff `spec-w*.diff` against the base's `spec/heroes-spec.md`. **No paid run but the `--refresh` requests below
where authorised, no credential printed, `gh` reads only, nothing outside the repository's root (`TMPDIR` inside your
copy), no `pkill`, no commit.**

## The counts

- **Before**, read by the repair at the base (`00-shared.md` § 3.7, stage 2's `heroes-next measure
  spec/heroes-spec.md` from the tree's root, no `--refresh`, byte-equal to the clerk's at `aded91388`: the spec did not
  move): claude-legacy **10,067**, cl100k_base **10,245**, **real 13,439** (`claude-opus-5`, the pin of 2026-11-26),
  headroom **2,945** against **16,384**, the FFI floor mortgaging 60 (13,499). The file: 516 lines, 35,246 bytes,
  sha256 `3df14254...d93f`. **The seed printed `STALE` at the clerk's base and no longer does** (the seed refreshed by step 207a, `00-shared.md` § 3.7): judge with your stage 2 all the same. **The sentence alone**: 88 on both
  vendored tables (`ground/parts/handler-sentence.txt`, 347 bytes); the room's sentence beside it 90 and 91; lines 427
  and 428 together 371 and 373 (the clerk's and the critic's, the same). **Re-read them** and say if any differs.
- **After the base, panel 261's text landed** (`00-shared.md`, the head: step 203f; at `main` `0bde00d84` the spec is
  526 lines, 36,120 bytes, sha256 `9ff7d2a1...2fc3`, the handler sentence byte for byte on line 438, `suite_spec.hero`
  pinning `REAL_TOKENS` 13776 and `SPEC_TOKENS` 10507, read and not measured by the repair; `ground/spec-at-0bde00d84.md`).
  **Measure `before` on the spec your copy holds and say which**; if your copy is the base's, measure the landed one
  too (`git show main:spec/heroes-spec.md` into a file, `heroes-next measure <file>`, vendored rows only).
- **A candidate's price**: `measure` of a file other than the pinned spec prints the two vendored rows and no `real`
  row, and **a vendored delta is not a price** (`.claude/rules/spec-shape.md` § How a change to the document is made).
  The real count needs `./heroes-next measure spec/heroes-spec.md --refresh`, one request to the count-tokens endpoint
  each, **only where the coordinator's prompt authorises it, and then at most four**: `cp spec/heroes-spec.md
  spec.orig` and its `shasum -a 256`, apply the text, `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a`
  (print `${#ANTHROPIC_API_KEY}` alone, never the value), `--refresh`, then restore with `cp spec.orig
  spec/heroes-spec.md` and compare the `shasum` before any other command. Without that authorisation every count is
  written **a lower bound, vendored only**, in those words.

## What you judge

1. **The texts, one per candidate of `00-shared.md` § 4**, each written and measured (none is drafted for you):
   **(a)** two sentences by kind, a signal handler of C's apart from a coroutine switch with no handler; **(b)** the
   sentence restated by the property the runtime tests (`00-shared.md` § 3.3: a handler of C's seen to begin and not
   seen to end on the callback's thread, or a delivery's own frame beneath it), with what *seen to end* is for a
   reader; **(c)** the sentence as it stands plus one clause (for example *signal* before *handler*, *"on its
   thread"*, or a clause saying a switch with no handler running is none of the five); **(d)** the routes nobody
   listed, **(d1) to (d9)** (`00-shared.md` § 4: the critic's C26 added (d6) merge, (d7) line 427's home, (d8) abort at
   the exit, (d9) a told route), with what would have to be true for each to exist. **For each, say the platform,
   route and thread on which every clause is true, and the defect whose landing it waits for**, or promise nothing
   positive (the critic's C2): the graded platforms are arm64 Darwin and Linux on route C; **route A is glibc's other
   architectures and Darwin x86-64** (`rebind_decode.c:314-315`), where the drafted `d-lane-sig-family-1` refuses
   correct programs (`00-shared.md` § 3.5); musl and FreeBSD are systems the runtime was never built on (§ 5). Ask the
   compiler-engineer's runs, not the clerk's names, which clause a program falsifies.
2. **`x1` is false as written** (`00-shared.md` question 3; the critic's C4, the repair's first item): both of panel
   257's R2 conditions are met (2706 closed; the CI's two Linux legs passed the route case at `4a5e585a9`, an inference
   from the log), and at the base on route C `fixedbugs-2705` and the own-stack coroutine
   (`2702-a-handler-jumps-into-a-coroutine-on-the-own-stack-...`) are refused in ONSTACK's words, **any asynchronous
   handler `siglongjmp`ing out of code inside the runtime's allocator stops the program at the jump** (panel 258's R1,
   `sigguard.c:175-212`, words of its own, no spec sentence), on route A 2706's two programs and `b_0`, `cs_0` are
   refused, and a callback on another thread runs (the key's `h9`). **A text that reuses x1 or any run-promise is
   offered only held to these falsifiers**: scoped to a signal that interrupted C and not the runtime, to the thread,
   to the route, or saying what happens there.
3. **The word *may*** (`00-shared.md` § 2 question 4): the spec's only *may abort* by the phrase, against § 6's single
   definition of an abort. **Not the only conditional abort** (the critic's C6): § 9's *"running out of stack aborts"*
   (line 331, no depth), lines 424-425's *"C reaching past them aborts, or fails the call where the kernel writes"*,
   line 487's handle *"aborts, ... unless C has since handed its address out again or a million other lives have
   ended"* (a stated exception, *unless*), and the room's bound (line 428): the spec's own precedents for wording a
   conditional abort, against which *may* is weighed. Panel 257 kept it on two measurements that may have moved (2702
   closed; Linux's depth test unrun at this base). Where a clause can say *aborts* or *runs* by a fact a reader sees in
   their program, say which; where it cannot, say what *may* is, in the spec's own words, so that neither 257's nor
   259's reader reads it otherwise, **nor a reader who takes *may* for a platform split** (P).
4. **The word *handler*** (`00-shared.md` § 2 question 5): lines 427 and 428 and the remedy use *signal handler* and
   *handler of C's*. Write the text that leaves no reader taking a handler for any frame of C's (in 259's archive every
   H reader is also a W reader: `blind/registration.md` § The labels, tried), **nor for the remedy's own counting
   handler** (`hero_signal_watch`, the key's `h13`), and that says whether *"a signal handler of C's"* is the program's C
   or the process (every plain-built program runs the runtime's own SIGSEGV handler, `stack.c:1269-1282`).
5. **The pronoun and the thread** (`00-shared.md` question 1, I and T): *"a jump that lands within it"*, whose nearest
   antecedent is *a stack* (2 of 5 of 259's `now` t1 readers read it so); and the sentence's lack of *"on its
   thread"*. Each text says which.
6. **The instrument and the placement** (`00-shared.md` § 2 question 6): every text stands **before** *"No function
   of the program is a signal handler"* (the compiler's test of defect 2533, `selfhost/emit/callback_thunk.hero:400-413`,
   holds the line's tail byte for byte). `tests/harness/spec_refusals.hero` finds the sentence by `SENTENCE_OPENS`
   (`:39-40`), ends it at the first `". "` (`:68`) and asks both `CLAIMS` (`:32-36`) of it, and **its test reads the
   real spec** (`:213-222`, the net's own tests): **for each text, the instrument's change first**
   (`.claude/rules/spec-shape.md`: *instruments first*), written as a diff and run (`heroes-next run
   tests/harness/main.hero -- heroes-next spec` in your copy with the text applied, and `heroes-next test
   tests/harness/main.hero`; the `spec` suite at the base reads 36 and 0, `ground/suite-spec-repair.txt`, the repair's, as the clerk's did; the four
   pins and the ledger move with any text, the rest must not). Does a split sentence need the instrument to read two,
   and which claim does each half keep? **Three arms' words stand in no golden**, and **two more stops carry neither
   claim** (panel 258's jump refusal, 0 goldens; guard D, 2: `00-shared.md` question 6): say whether that matters to a
   claim the text makes.
7. **What a reader of the whole paragraph meets** (`00-shared.md` § 2 question 9, widened by the repair): the room's
   sentence immediately before the handler sentence, § 9's *"running out of stack aborts"* (which may be no abort on a
   `malloc` coroutine stack: unrun, the critic's C28), line 427's *"never ... called by one"*, the thread a callback
   may be called on (no sentence names `hero_thread_refuse`'s abort: `grep -n -i 'thread C\|C made'` finds nothing but
   line 383), **panel 258's jump refusal and guard D**. Merge where you can (`.claude/rules/spec-shape.md` § Where a
   rule lives: *merging beats appending*; (d6)); one home per rule; no closed list of aborts (panel 087, the same file).
8. **Panel 261's text on the same lines** (`00-shared.md` § 3.1 and its head): **landed after the base** by step 203f,
   in words that differ from its adopted diff (`ground/spec-261.md` against `ground/spec-at-0bde00d84.md`: lines 438
   and 439), keeping the handler sentence byte for byte. Write each diff against the spec `main` holds when you write it
   and check it applies to the base's too (`patch --dry-run` on each), saying which you priced.
9. **The record's sentences a text falsifies or supersedes**: panel 257's R1 and R2 (`docs/panel/257-*.md:113-151`),
   the runtime's eight arms (`runtime/parts/sigguard.c:598-644` at the base) where a claim moves, the module docs that
   quote the sentence (`sigguard.c:557-567`, REENTERED's comment), Rebel's `os.ledger:454` (the remedy quoted, `00-shared.md`
   § 3.8), and **design.md**, which holds none of the guard's rules **but refuses coroutines in the language**
   (`docs/design.md:3597`, `:3628`, `:4170-4176`; the critic's C10): say whether a text owes design.md a line.
10. **What the landing owes** (`00-shared.md` question 13; the critic's C5, C27): per text, the goldens whose words
    move **by form** (90 `run` cases hold a claim at the base; the `check` case `package-a-file-source-in-no-repository`
    holds *from inside* and is no refusal: a `grep -rl` that rewrites it is wrong), the net's own tests, the ledger, and
    **the site's build and its publication** (the site serves the spec verbatim; `deploy-site.yml`'s `paths:` hold the
    spec, `suite_spec.hero`, `selfhost/**` and `runtime/**`; `claims.ts` holds the README's *"fewer than 600 lines"*:
    say each text's line count).

## Predictions registered before the blind run

**Your report must hold the heading `## Predictions registered before the blind run`**, with your predictions under
it, **before any blind session starts** (the runner copies your report, hashed, as it starts). **The run is not
authorised yet** (`llm-ergonomist.md`): predict anyway, so the registration is whole when the author answers. Predict
for each of your own texts (each `spec-w*.txt`, named by its candidate): under it as `adopted`, and under `now`, **each
decision rule's count of `blind/registration.md` as repaired** (D1 over `h1`, `h3`, `h13`, `n1`, `n2`, `n3`; **D2 over its
fixed set `h7`, `h9`, `h10`, `h11`**; D3 over `h2`, `h5`, `h6`, `h8`; **D5 over `h12`**; D4's labels, W, N, L, H, T, I,
P, R, S, O, several per answer), a number and a range, and the spend (the registration's caps and its price inference).
An arm is read against your text whose diff is byte for byte the arm's; else *unpredicted*. Say which of your texts you
expect adopted, and which you would make `rival`.

End with the verdict structure of your agent file, then one line per numbered item above: the text you would adopt
(its count, real or *a lower bound, vendored only*), and the one measurement that would change your mind.
