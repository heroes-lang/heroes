# Panel 259, the blind reading: the registration. REGISTERED, NOT RUN.

Written 2026-11-25 by the sitting's clerk, **before any session is drawn and before any session runs; none is drawn and
none runs**: no paid run is authorised for this sitting (the author was asked at 06:11 and has not answered), and **the
run waits for the author's yes**. **Repaired** the same morning, before any session, from the critic's first reading
(`docs/panel/259-reports/critic-briefs.md`, F14, F15, F19, F25, its item 2), at the base `3ec775b66`
(`00-shared.md` § 2): the key names its platforms, the decision rules read what they say on both, a *may* is graded,
`task-t2.md` no longer points at q4, the folder's names are settled and design C's key header is registered. A later
coordinator who starts it, on the author's word, runs **this** file's inputs, by hash, with the two arms and the
classification the synthesis adds below. The rows, the key, the decision rules (D1, D2, D2w, D3), the size and the cap
are `docs/panel/259-briefs/llm-ergonomist.md` as it stands at the commit that carries this file. The spec-warden's
predictions are its own (`docs/panel/259-reports/spec-warden.md`, `## Predictions registered before the blind run`),
copied and hashed by the runner as it starts.

**Corrected by the synthesis, 2026-11-25, before any session**: the author authorised the paid run at 09:25 with a
ceiling of **20.00 USD** for the whole run (`llm-ergonomist.md`, the paragraph of that time), so *"no paid run is
authorised"* above is history; the cap below reads 20.00. The two arms, the warden's `E4` change and the *may*
classification are written below by the synthesis (`docs/panel/259-no-large-value-or-lend-is-judged-given-back-by-its-frames-address-what-a-callback-c-left-held-stays-held-to-a-bound-a-spec-sentence-tells-and-held-stands.md`),
and **the run still waits** for the key's rows § What the synthesis added names as owed.

## The inputs, by hash (`shasum -a 256`, 2026-11-25, at `3ec775b66`)

| file | sha256 |
|---|---|
| `arms/spec-now.md`: `spec/heroes-spec.md` at the base `3ec775b66` (the same bytes at `2cf85ea64`) | `c915f0f2d47f35c6b3162d198c27bedfbb3fe2b962e05185a878619356372bee` |
| `arms/spec-adopted.md`: `now` with `docs/panel/259-reports/synthesis-adopted.diff` applied by `patch -p1` (the text `synthesis-adopted.txt`, sha256 `c90282afb26eb50190ca8e133d22f2603c5bbc902603fcc0c0cc5fb902b0c39b`, 390 bytes) | `aaf23b9e7f76472e672a1ce51d6c7efbe00f5e21c2b83d8f5caea44a6af616f3` (34,828 bytes) |
| `arms/spec-rival.md`: `now` with `docs/panel/259-reports/synthesis-rival.diff` applied (the text `synthesis-rival.txt`, sha256 `c7e2341a0a1460c3337fa3daf3a9ef70549afc0b43848f99a5dd66201664570b`, 477 bytes) | `0461f1d3941433d09f41299f13d56361a4d37eec01351bf742d53cc1edcf4ac1` (34,915 bytes) |
| `tasks/task-t1.md` (`blind/task-t1.md`, repaired) | `f71c54471a591832e97f563f1bb089d4ae10faccf0cdea07beeb063a4ef952f7` |
| `tasks/task-t2.md` (`blind/task-t2.md`, repaired) | `29fdbaeee74e2638860fa1dbea40265d5804e9a94395c66b676b1c5b0bb8a18f` |
| `programs/co.h` | `ed909982b316b77d2a8a99d6871d7dc049ad6c82aed642ba602c99b048560c8c` |
| `programs/sw.h` | `e2c84c66eab40ee72cf3c690dd813bc1274678e26b5b30f209571a1daa2ac135` |
| `programs/jump.h` | `64d3ab4492142e99104a3791c4579381c7582467b0f492048c7e7100fc1ef977` |
| `programs/q1.hero` | `e8d2a25511e47007df015e6451db591b8f8588e69633023b31bc86a0920f4cf9` |
| `programs/q2.hero` | `2a8dd2e699e62e71cb72791995c88ac823e9ab47058212d1270dad73e7e8218a` |
| `programs/q3.hero` | `246759162e0e7326fec5c960863caa0dd57213898abb58f9c94b321d6caed8c8` |
| `programs/q4.hero` | `80e968d5964ce39aa89ac161d3b046445ae44d4e04eabd12f97ad93c7aa02002` |
| `programs/q5.hero` | `33d97704951be1ef2f7ae8c5dc56e49d533c475aeae1d77bce5a1ba2fbf70fc1` |
| `programs/q6.hero` | `ce11259e109b2dc55652a1f572cc990a83840a8e5d435ed4d8e1c2b1b2d54f78` |
| `programs/q7.hero` | `ebb1c12946a476bbb2b67434380cfee8eab5770c921412e10427b2024716da28` |

The two tasks' hashes moved with the repair (the clerk's were `5a144cd1...` and `093e2910...`); the ten programs' are
the clerk's, unchanged.

**The key's inputs, never in a session's folder** (`blind/design-c/`, copied by the repair from
`.claude/worktrees/panel259-scratch/blind-mmap/`, which the sweep takes when idle):

| file | sha256 |
|---|---|
| `design-c/sw.h`: t2's design C, `programs/sw.h` with `mmap`ed stacks and a `PROT_NONE` page below each | `20260c7671f878a9e4d93eee861f39bf96a0706950c8145f86321a1f70320f9b` |
| `design-c/q3m.hero`: `cmp` equal to `programs/q3.hero` | `246759162e0e7326fec5c960863caa0dd57213898abb58f9c94b321d6caed8c8` |

## Every session's folder

**`brief.md`, `task.md`, `spec.md`** and, for t1 and t2, **`programs/`**, nothing else (panel 257's names,
`llm-ergonomist.md` § The arms): `brief.md` is the runner's `brief-session.md`, `task.md` one of the two tasks,
`spec.md` one arm, never named. **A t1 session's `programs/`** holds `co.h`, `sw.h`, `jump.h` and `q1.hero` to
`q7.hero`; **a t2 session's** holds `co.h` and `sw.h` alone, which `task-t2.md` names and nothing else.
`blind/answers.md` and `blind/design-c/` are never in a session's folder.

## The *may* classification, before any session

Written by the synthesis, **before any session**, from each arm's text alone, never from a build: for each arm and each
program, **in** where a sentence of that arm says a class holding the program *may* abort (or may do anything else),
else **out**. A reader's `NOT PROMISED` for a program **in** is **text-right** (`llm-ergonomist.md` § The key); D1 and
D3 never count it, D2w counts it.

| arm | q1 | q2 | q3 | q4 | q5 | q6 | q7 | t2 A | t2 B | t2 C |
|---|---|---|---|---|---|---|---|---|---|---|
| `now` | out | out | out | out | out | out | out | out | out | out |
| `adopted` | out | out | out | out | out | out | out | out | out | out |
| `rival` | out | out | out | out | out | out | out | out | out | out |

Written by the synthesis from the three texts alone. The one *may* of `now` is line 428's handler sentence, whose class
is *a callback called after C left a handler of C's*; no program and no design holds a handler (`grep -n -i -E
'signal|sigaction|sigaltstack|raise'` over `programs/` prints nothing). The adopted sentence says *aborts* and *stays
held*, never *may*; the rival is `w1` (no *may*) before it. So no reader's `NOT PROMISED` is text-right in this run, and
D2w reads every `NOT PROMISED` as width the text did not ask for.

## The cap (`llm-ergonomist.md` § The size, the spend and the cap)

**31 sessions**; 0.60 USD a graded session, 1.20 the verdict, margin 0.10, a ceiling of **20.00 USD**, the author's
(2026-11-25, 09:25, recorded in `llm-ergonomist.md`; the clerk registered 15.00 and *"none given"*, corrected here by the
synthesis before any session); the runner stops before any session where `spent + cap + margin` would pass 20.00, and
the verdict runs only if the 30 spent at most 20.00 − 1.20 − 0.10 = **18.70 USD**. The price it rests on is panel 257's
measured run, 31 sessions for 6.297764 USD (carried); this run's estimate, 6.3 to 8.5 USD, is **an inference, unrun**.

## What the synthesis adds before any session

The two arms' texts and hashes in the first table; which of `arm-r`, `arm-t`, `arm-p` or a seat's text each is; the
*may* classification above; and, where the adopted resolution has been built by then, its run of q1 to q7 and of t2's
three designs (C with `design-c/sw.h`), three of three, **on this Mac and in the Linux arm64 image** (and in the x86-64
image, recorded), as the key, replacing every **UNRUN** of `answers.md`. **Where it has not been built, or where the
landed build's q4 or design A gives the two graded platforms different verdicts, the run waits**: a reading graded
against a key nobody ran, or against a program that is two programs, is not a measurement.

## What the synthesis added, 2026-11-25, before any session

**The two arms.** `adopted` is the resolution's R2 sentence (the compiler-engineer's +61 draft with the critic's three
false clauses repaired, the synthesis's wording): `now` with `docs/panel/259-reports/synthesis-adopted.txt` put after
`atexit(f: (function() -> ())) -> i32`. on line 428, before the handler sentence. `rival`, the second-ranked text, is
the spec-warden's `w1` (`docs/panel/259-reports/spec-w1.txt`, byte for byte, `cmp`) before the same sentence
(`synthesis-rival.txt`): true under the prototype of the adopted route for every shape run, and already promised by § 3,
so it measures whether the promise said beside the price moves a reader. Neither is `arm-r`, `arm-t` or `arm-p`. Counts
(`./heroes-next measure`, from a tree's root, no `--refresh`): `adopted` cl100k_base 10,144, `rival` 10,162, base 10,054:
**+90 and +108, a lower bound, vendored only** (the texts as the coordinator amended them after 17:30, a buffer lent to C under the bound; the draft before read +92 and +110). Both read clean by `spec/refusal-words` and keep line 428's tail
byte-equal (`cmp`).

**The warden's `E4` change, registered** (`spec-warden.md`, its predictions' last paragraph; the critic's § F1 measured
that it also ends q4's split). The script is `docs/panel/259-reports/blind-e4/to-e4.pl` (after `e5`'s definition,
`e5(` becomes `e4(` and a field path five deep four deep); its outputs for q1, q2, q3, q4 and q6 are beside it, q3's and
q4's byte-equal to the warden's `q3e4` and `q4e4` (`cmp`); q7 holds `E2` and the script leaves it as it is (`cmp`);
`task-t2.md`'s *"a record of 262,144 bytes"* reads *32,768* (one line, `diff`); `task-t1.md` stays, its words true at
`E4`. **q5 stays at `E5`, by the synthesis**: at `E4` it no longer reaches the room's bound (the base and the adopted
route's prototype `rt-e` alike print `left on fresh stacks, 5000 times`, exit 0, 3 of 3 each, this Mac), which would
leave no program of t1 where the adopted sentence's abort can be read; at `E5` it aborts under both, 3 of 3 each. q5 at
`E4` and 40,000 iterations was started and stopped unfinished, the machine swapping past three minutes: **unrun**. Its
confound (a 256 KiB record on a 256 KiB coroutine stack, so a reader predicting a stack overflow answers `ABORTS`)
stands for q5 alone, said here.

| file, replacing the registered one of the same name in a session's folder | sha256 |
|---|---|
| `programs/q1.hero` ← `docs/panel/259-reports/blind-e4/q1.hero` | `2dd061aeee342fd956d8eb5dec4d349496a6d36195c5f1e1f2234d79c545d1ce` |
| `programs/q2.hero` ← `blind-e4/q2.hero` | `3da8917ef51efcbcc745534ec4ee08ef8f1f909ee555eb9a7c84cb54ae8fe41e` |
| `programs/q3.hero` ← `blind-e4/q3.hero` | `180fac477656effc92111441fa3534a9563708f032241c8b181692143ddd3791` |
| `programs/q4.hero` ← `blind-e4/q4.hero` | `5b9351effc2758f217c73f46d1ef98652d95cb063449135c2fe80d4e3758ea58` |
| `programs/q6.hero` ← `blind-e4/q6.hero` | `aad2c4d0b4fbcaeb37ffd99a32d597e89299ca4600e137b75853a7254bbb318c` |
| `tasks/task-t2.md` ← `blind-e4/task-t2.md` | `28c1309c762b2cb462568ad775e417543fb528b35bec5d0c37569a971d6be097` |
| the key's design C program ← `blind-e4/design-c-q3m.hero` (`cmp` equal to `blind-e4/q3.hero`), beside the registered `design-c/sw.h` | `180fac477656effc92111441fa3534a9563708f032241c8b181692143ddd3791` |
| the script, `blind-e4/to-e4.pl` (never in a session's folder) | `e027a215c5f8b969fc547eef9dbcc6722acf6745946cadcdd3b300a066b18ed3` |

`co.h`, `sw.h`, `jump.h`, `q5.hero`, `q7.hero` and `task-t1.md` keep the hashes of the first table.

**The key, as far as it is run** (this Mac, at `3ec775b66`, the synthesis's stage 2 built from that base's seed,
`sources 117362877b136f67`, in `.claude/worktrees/panel259-scratch/synth/`, three runs each, the three outputs alike):

| | q1 | q2 | q3 | q4 | q5 (`E5`) | q6 | q7 | t2 A | t2 B | t2 C |
|---|---|---|---|---|---|---|---|---|---|---|
| `now`, this Mac | `PRINTS` | `PRINTS` | `PRINTS` | **`WRONG`**: `body holds 11` twice, then *compiler bug*, 3 of 3 | `ABORTS` | `PRINTS` | `PRINTS` | a wrong value | yes | yes |
| `now`, Linux arm64 | **unrun** at `E4` | **unrun** | `PRINTS` (the critic's `q3e4`, § F1) | **`WRONG`**, the Mac's lines (the critic's `q4e4`) | `ABORTS` (`answers.md`) | **unrun** at `E4` | `PRINTS` (`answers.md`) | a wrong value | yes | **unrun** at `E4` |
| `adopted`, this Mac, under `rt-e` (R1's E built; its lend bound and R2's words unbuilt, neither reached here: no extern of q1 to q7 takes an `@` or `lent` parameter, read) | `PRINTS` | `PRINTS` | `PRINTS` | **`PRINTS`**: `body holds 7` three times, `carved: 7` | `ABORTS` (2,047 callbacks not returned at the abort, the gate's count) | `PRINTS` | `PRINTS` | yes | yes | yes |
| `adopted`, Linux arm64 | **unrun** | | | | | | | | | |

At `E5` this Mac's base gave q4 `body holds 11` in two runs and `body holds 7` in one, each then *compiler bug*: at the
registered size the program was not even one verdict here. At `E5` q6 **aborts** under `rt-e` (*stack exhausted in
q6.e5*, 3 of 3) where it prints at the base, which would have moved it out of D2's premise; at `E4` it prints under
both. **Owed before any session, and the run waits for them**: the Linux arm64 rows above at these inputs, under `now`
and under `rt-e` or the landed build, three runs each; a program whose two graded platforms then differ under an arm has
no key under it (`llm-ergonomist.md` § The key). The landed build's words for q5 are not graded (an `ABORTS` is graded
by whether it names the switch or the stack, never by wording).
