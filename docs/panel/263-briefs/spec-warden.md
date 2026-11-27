# Panel 263, the spec-warden's brief

Added 2026-11-27 at about 10:35, when the coordinator widened the sitting; **repaired the same day from about 11:00 by
the coordinator's repair, on the critic's first reading** (`00-shared.md`'s head; the critic's items 7, 9 and 10, its §§ 7
and 9), and re-based on **`f9f42fcc4`**. Read `00-shared.md` first, whole: § 1 the question, § 2 the defect as run (the
damage to the defect's own program, § 2.2), § 3.4 the spec's sentences, **§ 3.9 the secondary question, step 199j's
stop**, § 4 the routes, § 7 which of them move a sentence; then the critic's first reading
(`docs/panel/263-reports/critic-briefs.md`, beside these briefs under `.claude/worktrees/panel263/`). **Your agent file is
your copy's** (`.claude/agents/spec-warden.md`); design.md §1.2 and §1.6 are reached by `grep -n` (§1.6 is
`docs/design.md:255`, its ceiling **16384** tokens *"measured by `claude-opus-5` through `POST
/v1/messages/count_tokens`"*, `:257-258`, re-read at `f9f42fcc4`). Under the author's amendment of 2026-11-22 (CLAUDE.md
§ Precedence: *"the most robust solution now always wins, a new form included and at the cost of the spec's size"*),
**the budget and Principle 0 are costs to be measured and written, never a veto against the most robust route**; your
veto on the soundness of a text, a sentence that promises what a program run on a platform it claims falsifies, stands.

**Your copy**: `.claude/worktrees/263-spec-warden`, a detached worktree at **`f9f42fcc4`** (`00-shared.md` § 8, with its
never-touch list), scratch `.claude/worktrees/263-spec-warden-scratch/`, `TMPDIR` there; build the seed's compiler **and
stage 2** there (the seed's compiler gives no real row, below). **Your texts are yours**: each beside your report as
`docs/panel/263-reports/spec-w*.txt`, with its diff `spec-w*.diff` against `spec/heroes-spec.md` at the base. **No paid
run but the one request below, no credential printed, no GitHub write, nothing outside the repository's root, no
`pkill`, no commit.** Report as you go to `docs/panel/263-reports/spec-warden.md` in your copy, copied to the same path
under `.claude/worktrees/panel263/` when you finish or stop.

## The counts before (re-run by the repair at `f9f42fcc4`, each with its command)

- **The file**: 526 lines, 36,316 bytes (`wc -lc spec/heroes-spec.md`), sha256 `736b534c852e39af…efd3` (`shasum -a 256`):
  the spec did not move from `a246032bf` (`git diff --stat a246032bf f9f42fcc4 -- spec`: empty).
- **Vendored, measured**: `heroes measure spec/heroes-spec.md` with the seed's compiler at `f9f42fcc4`, **no
  `--refresh`**: **claude-legacy 10,374, cl100k_base 10,563**. `SPEC_TOKENS` (`tests/harness/suite_spec.hero:114-115`)
  and the ledger's last row (`docs/measurements/010-spec-budget-ledger.md:263`) read 10563: the same.
- **Real, read from a pin, not a count of today** (the critic's item 10, § 7): the seed's compiler prints `STALE: the
  recorded count is for 24de425e6c28e69b and this file is 25b307910fc3ea19` and no verdict (its pin predates step 208b,
  `750d2540d`, which moved the spec and the pin after the seed's refresh at 207a, `43df12cbf`). **The tree's pin**:
  `selfhost/measure/pinned.hero:55-64` and `suite_spec.hero:175-176`, `:227`: **13,849 on `claude-opus-5`, taken
  2026-11-26, digest `25b307910fc3ea19`**. **The critic's stage 2 printed that `real` row from the pin, no request**, and
  **the instrument's headroom line**: the FFI floor *"mortgages 60 of it (panel 030 R3), so what is measured against the
  ceiling is 13909"*, **headroom 2,475** (`./heroes-next measure spec/heroes-spec.md`, critic § 7; `FFI_FLOOR` 60 read
  at `selfhost/measure/judged.hero:86-87`, the line at `:188`). The clerk's *"2,535"* left the floor out. The repair did
  not build stage 2: run it, and say if it prints otherwise.
- **The sentences each route or the stop touches**, cut by a script and counted offline (`heroes measure <file>`, vendored
  rows only; the cuts are `ground/parts/`; re-run at `f9f42fcc4`, the same):

| sentence | where | bytes | claude-legacy | cl100k_base |
|---|---|---|---|---|
| § 9's *"A source nested up to 2,000 deep … running out of stack aborts."* (**panel 232's R4**, below) | `spec/heroes-spec.md:340-341` | 129 | 33 | 35 |
| § 13's held-memory sentence, panel 259's (*"Where C leaves a callback's frames …"*) | `:438` | 391 | 90 | 91 |
| § 13's handler sentence, panel 257's (*"A callback called after C left a handler of C's …"*) | `:438` | 347 | 88 | 88 |
| the whole of line 438 | `:438` | 1,264 | 348 | 350 |
| **panel 262's adopted text**, which replaces 257's when lane land262 lands it | `docs/panel/262-reports/synthesis/spec-adopted.txt` | 1,234 | 291 | 291 |
| its **thread clause** (*"and a callback C calls on a thread of its own aborts, saying it ran on a thread this program did not start."*) | the same | 108 | 27 | 27 |
| **panel 258's stop sentence** inside it (*"Where C jumps out of a signal handler whose signal interrupted this runtime's allocator … "*), the shape 199j's stop would take | the same | 272 | 61 | 60 |

**§ 9's sentence has a ratification of its own** (the critic's item 10): panel 232's R4, *"§ 9, K1: running out of stack
aborts. It lands with defect 2253 repaired on Darwin and on Linux"*, its reasoning naming this very class, *"a frame past
the guard may write rather than die"* (`docs/panel/232-*.md:244-251`, K1 `:171-174`), landed at `641b43017` (`git log
--oneline -S"running out of stack aborts" -- spec/heroes-spec.md`). A text that moves it answers to 232's reasoning too.

**The real count is yours, and one, and it covers both questions** (the coordinator's message of 2026-11-27 about 10:57,
answering the critic's item 7: *"the coordinator's call is yes, one `--refresh` pricing the main text and 199j's
sentence together"*):
`./heroes-next measure spec/heroes-spec.md --refresh`, one request to the count-tokens endpoint, of the spec with **the
text you would adopt for the main question and the text you would adopt for 199j's stop applied together** to the real
path, restored after (`cp spec/heroes-spec.md spec.orig` and its `shasum -a 256`, the texts applied, `set -a; .
/Users/joseph/Temp/heroes/heroes-lang/.env; set +a`, `${#ANTHROPIC_API_KEY}` printed alone and never the value,
`--refresh`, then `cp spec.orig spec/heroes-spec.md` and the `shasum` compared before any other command). Where either
answer is *no sentence*, the request prices the other alone. **Every other text's price is written *a lower bound,
vendored only*, in those words** (`.claude/rules/spec-shape.md` § How a change to the document is made: *a vendored
delta is not a price*).

## What you judge

1. **§ 9's *"running out of stack aborts"*, route by route** (`00-shared.md` § 2: false today on a `malloc`'d coroutine
   stack, the defect's own program returning at exit 0 with megabytes written, § 2.2; § 7): under (b) is it true as
   written on every stack a callback is entered on, the thunk's own frames on C's stack included (§ 4 (b))? Under (a),
   (a4), (c), (e), (n), which reach only stacks the runtime saw made, write the sentence that says what happens on one it
   never saw **and on a stack in the image's own data** (shape A, § 2.5), or say why none is owed; under (l), (q) or (m)
   alone, which leave the writes, whether any true sentence exists short of saying the stack may be overrun. Hold each
   text to `00-shared.md` § 5's shapes as the seats run them, and to panel 232's R4.
2. **§ 13's held-memory sentence** (panel 259's): under (b) a runtime stack held for each callback suspended or
   abandoned, under (h) one per context until C frees a block the runtime does not see: none of the three things the
   sentence names (*"a value they held of more than 4,096 bytes, or holding a `partial` record, and a buffer they lent
   to C"*). Write the text that counts it against the same 1 GiB, or say how it is bounded with no word a reader reads,
   and which the seats' runs support.
3. **A new abort**, route (f) (and (e) where it refuses): its sentence, its home (`.claude/rules/spec-shape.md` § Where a
   rule lives: the definition of *abort* is § 6's and no section lists every abort; *merging beats appending*), and the
   correct programs it would refuse that run today (the ffi-pragmatist counts them).
4. **Step 199j's stop, the secondary question** (`00-shared.md` § 3.9): a stop no sentence states, which is no arm of
   `hero_signal_refuse` and no callback's abort. **Write the sentence, or the reason none is owed**, its price (within the
   one request above), **its home** (§ 13, beside panel 258's stop sentence in panel 262's adopted text, whose shape it
   would take), and **its instrument**: none reads these words today (`tests/harness/spec_refusals.hero` reads the
   refusal's arms, `00-shared.md` § 3.9), so say what would hold the sentence to the code. Read it against § 13's handler
   sentence and panel 262's R1, and against *"C alone dies too"* (the step's commit): is a stop of a program whose C would
   have died anyway a promise the spec makes or a fact it leaves to the runtime? **PARKED is R1's first sentence's, not
   yours** (`00-shared.md` § 3.9).
5. **Panel 262's thread clause** (ratified, not yet on `main`: `lane-land262` at `f9f42fcc4`, nothing of its own, 11:03):
   its words are the false message this defect prints on the program's own thread (`00-shared.md` § 2.1, § 2.4). Is any
   clause of the adopted text falsified by `reproducer/malloc.hero`'s runs, read clause by clause, *"these words"*
   included? Say what its landing owes this defect, if anything.
6. **design.md's refusal of coroutines**: `docs/design.md:3611` (*"`Coroutines` … refused on **cost alone**"*), `:3642`
   (the Part 6 row, *"Coroutines | require stack switching (`ucontext` or stack copying) — a step change in
   complexity"*) and `:4184-4188` (*"Threads, not green threads, not coroutines"*), found by `grep -n -i
   "coroutine\|makecontext" docs/design.md`. Route (b), and (h), make the runtime switch to or hand out stacks of its own:
   does that owe design.md a line, and which? (CLAUDE.md § 12: a refusal row names the fact that would make it wrong.)
7. **The instrument and the landing**: `tests/harness/spec_refusals.hero`, **225 lines at `f9f42fcc4`** (222 at the
   clerk's base; lane sig-family's merge `d8ab3094e` added the net's assert of the arm PARKED, `CLAIMS` `:32` and
   `SENTENCE_OPENS` `:39` unmoved; `wc -l`, `git diff --stat a246032bf f9f42fcc4 -- tests/harness`), finds panel 257's
   sentence and asks its claims; lane land262 will change it again. For each text, the instrument's change first
   (*instruments first*), the `spec` suite in your copy with the text applied (`heroes-next run tests/harness/main.hero
   -- heroes-next spec`), the net's own tests, the four pins and the ledger's row. **And the site**: `spec/heroes-spec.md`
   is in `.github/workflows/deploy-site.yml`'s `paths:` (`:73`), so a landed sentence publishes the site at its push,
   asked of the author (CLAUDE.md § Hard stops).
8. **Principle 0 and the payment rule**, as costs: per text, the named removal or the registered prediction design.md
   §1.6 asks, written; never a veto against the most robust route.

## Predictions, registered only if a text moves

The llm-ergonomist's blind reading is registered only if the adopted route, or 199j's stop, moves a sentence of the spec,
and its paid run is the author's to authorise (`00-shared.md`'s head). **If any text of yours is one you would adopt,
your report holds the heading `## Predictions registered before the blind run`**, with, per text, what a reader of the
spec alone would predict for `reproducer/malloc.hero` (at the default `n` and sized by `CO_N` to return, `00-shared.md`
§ 2.2), `canary.hero` at each `CO_N`, `static.hero`, OpenSSL's default ASYNC fibre with a recursing job, **and each of
the seven `fixedbugs-2871-*` cases** (*aborts*, *runs*, *may abort*, and in which words), a number and a range; **before
any blind session starts**. If no text moves, write that heading with *none: no sentence moves* under it.

End with the verdict structure of your agent file, then one line per numbered item above: the text you would adopt (its
count, real or *a lower bound, vendored only*), and the one measurement that would change your mind.
