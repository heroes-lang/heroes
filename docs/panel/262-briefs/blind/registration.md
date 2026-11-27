# Panel 262, the blind reading: the registration. REGISTERED, NOT RUN, NOT AUTHORISED.

**Amended 2026-11-26, about 21:45, by the author's word** (*ok a 25 dollari*, in conversation): the paid run **is authorised, with a ceiling of 25.00 USD for the whole run**, never more. The ceiling of 25.50 USD proposed below for the 31 sessions is **lowered to 25.00 USD** (the runner stops before a session whose cap plus margin would pass it; the 21-session plan, 17.50 USD at the caps, stays within it). The lines below that say the run is not authorised, or that the ceiling is proposed, are history and stay; this paragraph governs. **Still owed before any session**: the labels' reliability, two independent readers on panel 259's wrong answers (the repair measured kappa 0.02 on W and 0.07 on H against its own keyword rule: `blind/labels-259/`), and the synthesis.

Written 2026-11-26 by the sitting's clerk at the base `aded91388`, **before any session is drawn; none is drawn and
none runs**. **Repaired the same day from 15:46 by the repair**, before any seat or session, from the completeness
critic's first reading (`docs/panel/262-reports/critic-briefs.md`, its C13 to C19 and C24, its repair 3 and 7), and
moved onto the base `87003cb42` through `c18a9a9a3` (`00-shared.md`, the head): every number below that the move or the repair could
change was produced again by a command run by the repair, named beside it. What the repair changed, in one list:
**every wrong verdict is labelled**, a wrong `RUNS` included, and an answer may carry several labels; **five labels
added**, T, I, P, R and S; **the labels tried on panel 259's archived answers**, free (§ The labels, tried); **D2's
program set fixed before any session, the same under every arm**, four programs where it held one; **t1 gains five
programs** (`h9` to `h13`), one of them the first program of this run that can measure the *list read as exhaustive*
reading, L, and **D5** reads it; `h2.h`'s first line no longer quotes `now`; the key's build is named; `n3`'s
condition (and, since defect 2855's repair at the base, `n3` in D1), `h4` against `h6`, the room's sentence on `n2` and `n3`, and the caps, re-written.

**The paid run is not authorised**: the author authorised 20.00 USD for panels 259, 260 and 261 only (the
coordinator's brief to the clerk), so **this run waits for the author's yes, which the coordinator asks for**, with the
size and the ceiling below. A later session that starts it, on the author's word, runs **this** file's inputs, by
hash, after the synthesis has written in § The arms the texts and hashes it adds, and in § The classification its
classification of each program under each arm.

## The inputs, by hash (`shasum -a 256`; the repair, 2026-11-26, about 16:15)

**Every input's full hash is `inputs.sha256` beside this file** (40 lines: the two tasks, the session brief, `key.sh`,
`key-new.sh` and the 35 files of `programs/`; itself `83f01287...db3c`), one line per file under this folder, so that
`shasum -a 256 -c inputs.sha256` run in this folder reads every line (40 of 40 `OK`, the repair; the clerk's first
line named `arms/spec-now.md`, a file that does not exist, and `-c` failed it: the critic's § 1 table); **the `now` arm's hash is
`arms.sha256`**, one line naming `spec/heroes-spec.md`, checked from a copy's root at the base (`shasum -a 256 -c
docs/panel/262-briefs/blind/arms.sha256`). **Repaired by the synthesis, 2026-11-27** (the critic's second reading, K5):
`arms.sha256` now holds **three lines**, the landed spec `9ff7d2a1...2fc3` as `now` and the two arms the synthesis
wrote (§ What the synthesis added), checked from the root of a tree whose spec is `main`'s at `77eb38534` and which
holds the sitting's folder (3 of 3 `OK`, the synthesis); the base's line, `3df14254...d93f`, is history. The ones a
reader of this page needs:

| file | sha256 |
|---|---|
| `now` **since the synthesis**: `spec/heroes-spec.md` at `main` `77eb38534`, panel 261's text landed (526 lines, 36,120 bytes; `git show main:spec/heroes-spec.md \| shasum -a 256`) | `9ff7d2a1bfda4323e54de004b27ef522bd86885a3fa249eb20406fb2a2942fc3` |
| `now` until the synthesis (history): `spec/heroes-spec.md` at `87003cb42`, the same bytes as at `aded91388` (516 lines, 35,246 bytes) | `3df14254998f68eb4a95ffe6f4c52b6772e2b0298a88ed82eee4b436fd12d93f` |
| `task-t1.md` (amended by the repair) | `fdbd7d624585c125452b9fad979808318d5fbeaffc858f7eb4738738d37a0578` |
| `task-t2.md` (the clerk's, unchanged) | `617de0368efb223cfb5907b52d005b1954153f72f89d695eee346f82cca1e00c` |
| `brief-session.md` (the clerk's, unchanged) | `01855da511c5b0da778b0794175646721ec88e767920380f4b853844bc8c5fc8` |
| `programs/prelude.h` (unchanged) | `95d7d7d6618ae288cb943352a9ecc39fa18bb787492569ae8f761f37e8a1f9eb` |

**Where the programs come from** (`grep -n -i 'heroes-lang\|panel\|defect\|runtime\|guard\|lane\|critic\|clerk\|seen to
end\|from inside\|no function of the C library\|handler of C'` of every file in `programs/`, the repair: **one line**,
`esc.h:1`, *"a jump back to that save point from inside the coroutine"*, a claim's two words describing the program's
own jump, not a handler; kept, the same under every arm; nothing else):

- `prelude.h` and `h1.h` to `h6.h` are **panel 257's blind programs** (`docs/panel/257-briefs/blind/programs/`, its
  `p1`, `p4`, `p5`, `p6`, `p7`, `p8`), each `.hero` its `pN.hero` with the header's name changed. **All byte for byte
  but `h2.h`, whose first line the repair changed** (the critic's C19 (ii)): it quoted `now`'s first exit word for word,
  *"a handler leaves by a jump no function of the C library makes"*, which under an arm that drops the phrase carries
  `now`'s vocabulary into it; it now reads *"a handler leaves by an inline jump: `__builtin_setjmp` and
  `__builtin_longjmp` on x86-64, the same jump written in assembly on arm64"*, the mechanism alone, for every arm alike.
  The price is the break with 257's `p4` bytes: a comparison of `h2` with 257's `p4` now compares two comments.
- `h7.h` is the `run` golden `fixedbugs-2706-reused-stacks.h` with its comments taken out, one line of description
  added and a `run_case` calling `rs_case(1, 5, 150, cb)`; `h8.h` is the clerk's (a handler's `siglongjmp` into a live
  coroutine `makecontext` made).
- **`h9`** (the repair's, the critic's C24): the critic's `programs/critic/tl.h` and `other.hero`, run by the critic on
  this Mac and on Linux arm64 at `aded91388`, the header renamed and its first line stripped as `h2.h`'s was. A
  function `spawn` started leaves a handler of C's by the inline jump; the program waits for it; then C calls the
  callback on the program's own thread. **Its Heroes code differs from the others'**: the task says so.
- **`h10`** (the repair's): `fixedbugs-2707-two-stacks.h` at `order 1, how 0` (a handler on an alternate stack leaves
  by `siglongjmp` to a buffer a `makecontext` coroutine saved; the coroutine calls the function twelve frames down),
  its comments taken out, its `how 1` branch and its parameters removed, `cb(7)` for `cb(90)`.
- **`h11`** (the repair's): `fixedbugs-2702-a-handler-left-by-setcontext.h` at `onstack 0` (the handler, **on the
  coroutine's own stack**, leaves by `setcontext` to the context the coroutine took; the coroutine calls the function
  twelve frames down), the same cuts.
- **`h12`** (the repair's): a coroutine of `sw.h` (an assembly switch, a `malloc` stack, never given to `makecontext`)
  saves a buffer with `sigsetjmp` and yields; a handler leaves by `siglongjmp` into it; the coroutine calls the
  function and leaves for good by `longjmp` to the runner. **By `now`'s letter it is none of the five exits** (the
  handler is left by `siglongjmp`, a function of the C library, onto a stack never given to `makecontext`), and it
  **aborts** (§ The key): the program on which a reader of the list as exhaustive, L, answers `RUNS` and is wrong. It
  includes `sw.h`, so t1's folder holds `sw.h` too.
- **`h13`** (the repair's): the critic's `programs/critic/rw.h` and `watched.hero`, renamed, the header's first line
  without its name: the program counts SIGUSR1 with `hero_signal_watch` (`hero_os.h`), C raises it and then calls the
  callback. The handler is the remedy's own (the critic's C24 (ii)).
- `co.h` and `sw.h` are **panel 259's registered blind inputs byte for byte** (`docs/panel/259-reports/evidence.tar.xz`,
  `docs/panel/259-briefs/blind/programs/`); `esc.h`, `th.h` and `n1.hero` to `n4.hero` are the clerk's (`n2` and `n3`
  are panel 259's compiler-engineer's `escape_heap` and `escape_carved` on `sw.h`).
- **Most headers include `prelude.h`; four do not** (`grep -L prelude.h programs/h*.h`: `h2.h`, `h7.h`, `h9.h`,
  `h13.h`; the critic's C19 (i)), and `task-t1.md` says so.
- Every `.hero` is canonical (`heroes-next fmt <file> | cmp - <file>`, the repair's first stage 2, `sources
  24c248d27fa1f712` at `c18a9a9a3`: `h9` to `h13`, 5 of 5; the clerk's 12 of 12 at `aded91388`, their bytes
  unchanged).

## The arms

| label | the spec the session reads |
|---|---|
| `now` | `spec/heroes-spec.md` at `87003cb42`, hash above (the same bytes as at `aded91388`: `git diff --stat aded91388 87003cb42 -- spec`, empty); **since the synthesis, the landed spec at `main` `77eb38534`, `9ff7d2a1...2fc3`** (below) |
| `adopted` | `now` with **the resolution's text** applied: written, with its diff and sha256, by the synthesis, **before any session**: `docs/panel/262-reports/synthesis/spec-adopted.md`, `180f31da...1a79` |
| `rival` | `now` with **the second-ranked text** the synthesis names (the one that fixes the other misreading, where the adopted fixes one): the spec-warden's `spec-w1`, `docs/panel/262-reports/synthesis/spec-rival.md`, `7c57d49f...6ad9` |

**Three arms**, since the question is two opposite misreadings and at least two more readings (`00-shared.md` § 2
question 1): a text that cures 259's widening may invite 257's narrowing, and a second text is how the run tells which.
**If the author authorises less**, the run is `now` and `adopted` alone (21 sessions), and every rule below is read on
those two. If panel 261's text has landed on § 13 by the run's day, every arm is the landed spec with the candidate
re-applied, re-hashed here before any session (the handler sentence is byte-equal in both, `00-shared.md` § 3.1).
**It has, after the base** (step 203f; at `main` `0bde00d84` the spec is 526 lines, sha256 `9ff7d2a1...2fc3`, the
handler sentence byte for byte on line 438: `00-shared.md`, the head): **unless the coordinator names the base's spec
for the run, `now` is the landed spec**, its hash written here before any session, and the key is re-run under the
build of the run's day as § The key already owes.

## The key (the plain build's, every cell three runs of three; the build named by `--version`)

**This Mac** (Darwin arm64, Apple clang 21.0.0), **the repair's stage 2 at `87003cb42`, `sources
6ce3f9e116d22e3a`**: `h1` to `h13` and `n1` to `n4` by `sh blind/key.sh <compiler> <out>` (20:53 to 20:54 by the
runner's clock), every cell one distinct output of three: **`key-mac-repair/`**, 102 outputs and `summary.txt`. **Every
output equals the repair's first pass at `c18a9a9a3` but `n3`'s six, which now run** (`diff` per file after `key.sh`'s
normalisation of pid, addresses and `(N)`: 6 of 102 differ; step 199d repaired defect 2855). **The first pass**
(`key-mac-repair-c18a9a9a3/`, stage 2 `sources 24c248d27fa1f712`): `h1` to `h8` and `n1` to `n4` by `key.sh`, 15:56 to
15:57, **all 72 outputs byte-equal to the clerk's `key-mac/` at `aded91388`** after the same normalisation (steps 199b,
199c and 200a moved no cell); `h2` again with its new first line and `h9` to `h13` by `blind/key-new.sh` (the clerk's
loop over those programs alone; `h13`'s cells by the same loop typed by hand, before the script named it). `key-mac/`
keeps the clerk's 72 at `aded91388`. **Linux arm64** (`heroes-linux-arm64:latest`, image id `338a1044ac90`, Debian 13, glibc 2.41,
Debian clang 22.1.8): § The key on Linux arm64, below.

| program | what C does | `now`'s class | plain (this Mac) | `--sanitize` (this Mac) |
|---|---|---|---|---|
| `h1` | a handler leaves by `siglongjmp`, the buffer static | silent | `RUNS`: `answer ran for 7`, `result 70` | `RUNS` |
| `h2` | a handler leaves by an inline jump written in assembly | may (exit 1) | `ABORTS`, STANDING's words (*"... had not been seen to end: either C called it from inside that handler, or ..."*), exit 134 | the same |
| `h3` | the handler returns, then the program calls | silent | `RUNS` | `RUNS` |
| `h4` | the handler itself calls the program's function | line 427 | `ABORTS`, STANDING's words, exit 134 | the same |
| `h5` | the handler switches context back to before the signal by `swapcontext`, never resumed | may (exit 2) | `ABORTS`, SWITCHED's words (*"... had switched to another context ..."*), exit 134 | the same |
| `h6` | the handler saves a buffer, jumps back into itself by `siglongjmp`, then calls the function | may (exit 5) and line 427 | `ABORTS`, STANDING's words, exit 134 | the same |
| `h7` | a `siglongjmp` out of a handler onto later frames in memory a returned `makecontext` task once used | may (exit 3) | `RUNS` | `RUNS` |
| `h8` | a handler's `siglongjmp` into a live `makecontext` coroutine, which calls the function | may (exit 3) | `ABORTS`, ONSTACK's words (*"... onto a stack a context switch had entered and this runtime has not seen end"*), exit 134 | the same, after ASan's warning |
| **`h9`** | a function `spawn` started leaves a handler of C's by the inline jump; then C calls the function on the program's own thread | may (exit 1, by its letter: no *"on its thread"*) | `RUNS`: `left on the thread: 1`, `answer ran for 7`, `result 70` | `RUNS` |
| **`h10`** | a handler on an alternate stack leaves by `siglongjmp` into a `makecontext` coroutine, which calls the function | may (exit 3) | `RUNS` | `RUNS` |
| **`h11`** | a handler on the coroutine's own stack leaves by `setcontext` to the coroutine's context, which calls the function | may (exit 3) | `RUNS` | `RUNS` |
| **`h12`** | a handler leaves by `siglongjmp` into a live coroutine of an assembly switch, never given to `makecontext`, which calls the function | **silent** (none of the five exits) | **`ABORTS`, STANDING's words**, exit 134 | the same, after ASan's warning |
| **`h13`** | the program counts SIGUSR1 with `hero_signal_watch`; C raises it, then calls the function | silent | `RUNS`: `watch 0`, `answer ran for 7`, `result 70`, `counted 1` | `RUNS` |
| `n1` | a `makecontext` coroutine whose body is the callback, yields, is resumed | silent | `RUNS`: `answer ran for 7`, `result 70` | `RUNS` |
| `n2` | an assembly-switched coroutine on `malloc`'s stack leaves for good by `longjmp` to its host; then a callback | silent (the room's sentence by letter, its abort not reached) | `RUNS`: `left: -1`, `answer ran for 7`, `result 70` | `RUNS`, after ASan's warning |
| `n3` | the same on stacks carved from an array in the host's frame | silent (the same) | **`RUNS`**: `left: -1`, `answer ran for 7`, `result 70` (at `aded91388` and `c18a9a9a3`: `ABORTS` after `left: -1` in REENTERED's words, defect 2855) | `RUNS` |
| `n4` | a thread C made calls the callback; no handler | silent (line 383's handle sentence the nearest) | `ABORTS`, the thread's words (*"... ran on a thread this program did not start"*), exit 134 | the same |

**`n3` was a defect's program**: defect 2855, a program that installs no handler refused in a handler's words, at
`aded91388` and `c18a9a9a3`. **The rule registered** (the critic's C17; the clerk's *"out of every gate unless its key
is one verdict on both platforms"* admitted it by the letter where its key was a defect's false message, which no
gate may reward): **`n3` is graded only once its key is what a text promises**, 2855's repair landed and the key re-run
on both graded platforms under that build. **That condition holds at the base**: step 199d (`77fade16c`, merged at
`c54db505f`, *"a coroutine leaving for good to its host runs"*) repaired 2855 (its issue open until the batch's gate,
`gh issue view 2855`), and `n3` **runs** on this Mac and on Linux arm64, plain and `--sanitize`, three of three (§ The
key on Linux arm64). **So `n3` joins D1**, a program with no handler that runs, which a reader of the sentence as
covering every switch answers `NOT PROMISED`; on the run's day its key is re-run with the rest, and should it split, it
leaves D1 by the rule below.

**The key's build** (the critic's C18): **the key is the plain build's**; **a program whose plain and `--sanitize`
builds give different verdicts has no key**, as a program whose two graded platforms differ has none, and is out of
every rule that reads it. `n1` is defect 2852's shape (a `makecontext` fiber on Darwin that calls a callback; 2852 open
and unrepaired at the base: `gh issue view 2852`; the probe `mj_carved` still refused under `--sanitize`,
`00-shared.md` § 3.4) and runs under `--sanitize` today by the layout's luck: **`n1 --sanitize` is re-run
on Darwin on the run's day** under that day's runtime.

**Owed before any session, and the run waits for them**: **this Mac's rows again and the Linux arm64 rows**, plain and
`--sanitize`, three runs each, both under the build of the run's day (`main` may have moved: the lane sig-family has 2852
and 2853 left, `00-shared.md` § 3.5), by `key.sh` (its loop takes `h1` to `h13` and `n1` to `n4`). **A
program whose two graded platforms give different verdicts has no key** and is out of every rule that reads it. The
x86-64 image is recorded, never graded; **neither arm64 platform runs route A** (`00-shared.md` § 3.3), so no key
here says anything of Darwin x86-64 or of glibc's other architectures.

### The key on Linux arm64

**Run by the repair at the base**, one container `p262-repair` (`heroes-linux-arm64:latest`, `338a1044ac90`), over a
`git archive ee2aef6d9` (the base's compiler, runtime and case bytes) with this folder added: the seed `sources
1e1eb23a1927d35b`, stage 2 **`sources 6ce3f9e116d22e3a`**, the same keys as this Mac's; `sh blind/key.sh ./heroes-next
L-key` over `h1` to `h13` and `n1` to `n4`: **every one of the 34 cells is one distinct output of three, and every
cell's first run equals this Mac's** after ASan's warning lines and the shell's `Aborted` are set aside and pid,
addresses and `(N)` normalised (`diff` per file: 0 of 34 differ; `n3` runs here too). So **no program of t1 or t2
splits between the graded platforms at the base**, plain or `--sanitize`, and every rule has its key. Outputs:
`../ground/linux-arm64/L-key/`, `L-key.txt`, `L-env.txt`, `L-build-*.txt`.

**The first pass, at `c18a9a9a3`**, one container `p262-repair` (`docker run --rm --pull never`, `heroes-linux-arm64:latest`,
image id `338a1044ac90`; Debian 13, `ldd (Debian GLIBC 2.41-12+deb13u3) 2.41`, Debian clang 22.1.8), over a `git
archive c18a9a9a3` with this folder added: the seed `sources 05186f2b38e77a97`, stage 2 **`sources 24c248d27fa1f712`**,
the same keys as this Mac's; `sh blind/key.sh ./heroes-next L-key` over `h1` to `h12` and `n1` to `n4` (`h13` was
written after the container's tree was copied: **its Linux row is the critic's `watched.hero`, the same program under
another header name**, run in the same container, below). **Every one of the 32 cells is one distinct output of
three, and every cell's first run equals this Mac's** after ASan's warning lines and the shell's `Aborted` are set aside
and pid, addresses and `(N)` normalised (`diff` per file: 0 of 32 differ); `n3` aborted there in REENTERED's words, as
on this Mac. Outputs: `../ground/linux-arm64-c18a9a9a3/`.

## The grades

- **build-right**: the reader's verdict is the key's; `RUNS` with the key's lines (t1's are fixed by the task; t2's
  are graded line by line); an `ABORTS` is build-right where the key aborts, its sentence graded apart (below). A
  reader giving the Mac and Linux each a verdict is build-right where both are the key's.
- **words**: an abort sentence names **a handler of C's not seen to end, or a call from inside one** (either claim of
  the sentence, in the reader's words) for `h2`, `h4`, `h5`, `h6`, `h8` and `h12`; **a thread C made** for `n4`.
  Graded by the cause it names, never by its wording.
- **text-right**: the reader's `NOT PROMISED` for a program the arm's classification (below) puts inside a *may*.
  `NOT PROMISED` is never build-right.
- **wrong**: every answer that is neither build-right nor text-right: a `NOT PROMISED` outside a *may*, an `ABORTS` on
  a program the key runs, **a `RUNS` on a program the key aborts**, a `RUNS` with other lines.
- **the reading** (repaired: the critic's C13, C14, C16 and C24): **every wrong answer is labelled**, by the reason
  that decided it: **the choice point the reader names as deciding that program's verdict, or its answer block's own
  reason**, never every topic its `choice_points` discuss (§ The labels, tried: a rule that labels every topic agreed
  with a reader at a kappa near 0). **An answer carries every label its reason supports**, so no precedence between
  two is needed:
  - **W**, the sentence's *may* covers this switch (259's widening);
  - **N**, the sentence's *may* means *aborts* (257's narrowing);
  - **L**, the list read as exhaustive, so an exit it does not name is promised (2848's reading); also a `RUNS` that
    reads silence as a promise;
  - **H**, *handler* read as any frame of C's, or as the program's callback;
  - **T**, the sentence read as reaching a callback on another thread than the one whose handler was left (`now` has no
    *"on its thread"*);
  - **I**, the pronoun: *"a jump that lands within it"* read as *within the stack once given to `makecontext`*, not
    within the handler;
  - **P**, *may* read as a platform split;
  - **R**, the room's abort (the sentence before the handler sentence) read as reached by this program;
  - **S**, § 9's *"running out of stack aborts"* read onto a stack C switched to (a stack of `malloc`, `mmap` or a
    carved array): **reported, never counted as a misreading until a seat has run it** (the critic's C28: whether a
    callback that recurses past a `malloc` stack aborts or corrupts memory is unrun, and 259's `t2-h-2` raised it as a
    reason);
  - **O**, other, quoted.

  Two graders label the same answers apart; their disagreements are reported, never resolved by a third.

## The labels, tried on panel 259's archived answers (free; the repair, 2026-11-26 from 15:52)

**What was run.** `docs/panel/259-reports/llm-ergonomist-blind-answers.tar.xz`, unpacked in the repair's scratch
(`.claude/worktrees/panel262-repair-scratch/a259/`), never in the tree; its 30 graded sessions' `result.md`, graded by
its own `score/grades-t1.tsv` and `grades-t2.tsv` against its key. **The units**: every answer this registration
calls wrong, **48** in 13 sessions (`labels-259/units.txt`): 45 `NOT PROMISED` (t1 q1 to q7 and t2 designs A to C) on
programs whose key printed or aborted outside a *may* (259's *may* table put every one **out**, its report's § t1), 2
`RUNS` on q4 under `now` (`t1-h-3`, `t1-h-5`), whose key was a compiler defect's wrong value, and 1 *yes* on t2's design
A under `now` (`t2-h-2`), the same defect's; `t1-h-1`'s q5 (key: an abort) among the 45. **Grader A**, the repair, read each of the
13 sessions' `choice_points` whole and each unit's answer block (`labels-259/graderA.tsv`). **Grader B**, a rule,
`labels-259/graderB.py`, written and hashed (`graderB.registered`, 15:52:07, `9b21c83c...fac2`) **before grader A read
any answer whole**, labels a unit by regular expressions over its session's `choice_points` and the answer lines
naming the program; its contamination: the repair had read 259's report's summary of the readers' reasons first.
**Two readers apart were not available**: the repair's brief forbids it a sub-agent, so the second grader is a rule,
and **two readers apart stay owed** before the paid run, free (the critic's second reading or the synthesis's writer).

**What it gave** (`labels-259/agree.txt`, `python3 -I agree.py graderA.tsv graderB.tsv`):

| label | grader A | grader B | both | agreement | kappa |
|---|---|---|---|---|---|
| W | 42 | 3 | 3 | 0.19 | 0.02 |
| H | 30 | 8 | 6 | 0.46 | 0.07 |
| S | 7 | 48 | 7 | 0.15 | 0.00 |
| I | 2 | 7 | 1 | 0.85 | 0.17 |
| R | 0 | 13 | 0 | 0.73 | 0.00 |
| P | 0 | 7 | 0 | 0.85 | 0.00 |
| O | 4 | 0 | 0 | 0.92 | 0.00 |
| N, L, T | 0 | 0 | 0 | 1.00 | — |

Units with the same set of labels: **2 of 48**; with at least one label shared: 17 of 48.

**What it says**, each a reading of the two files, the numbers the command's:

1. **A label is a reader's, not a rule's.** A rule over `choice_points` labels every topic a reader discusses (S on
   all 48: every session weighed § 9's stack somewhere; R on 13), and misses the one that decided the verdict (W on 3
   of grader A's 42). Hence the grade above: the label is the reason that decided the answer.
2. **W and H are one finding in 259's data, as the critic said (C14)**: every one of grader A's 30 H units carries W
   too, and the other 12 W units are two sessions (`t1-h-1`, `t1-h-4`) that apply the clause to switches without saying
   what *handler* means. With several labels per answer, the two no longer need a precedence; under the clerk's single
   label 30 of 48 units would have been split between W and H by whichever the grader chose.
3. **The pronoun reading I is in the archive, measured**: 2 of 5 `now` t1 readers name the choice and both take *"it"*
   for the stack (`t1-h-1` choice point 4, *"'it' is the stack once given to `makecontext`. That puts q6's `longjmp`
   under the clause"*; `t1-h-4` choice point 2). No other session of the 30 names it (`grep -n -i -E '"it" is|"it"
   (refers|means)|which part of the rule "within it"'`).
4. **S is a reason readers give** (7 units, 3 sessions: `t2-h-1`, `t2-h-2`, `t2-h-3`), and grader A gave it beside H
   and W on five of them; under the clerk's labels these had no name but O.
5. **O is where the key was a defect's** (3 of 4 units: a reader's *runs* where the key was a compiler defect's wrong
   value), and `t1-h-1`'s q5, whose `NOT PROMISED` rests on where records live. **A wrong verdict against a defect's key
   is no misreading of the text**; the rule for `n3` above is the same rule.
6. **N, L, T and P have no instance**: no 259 program could show them (none keyed `ABORTS` inside a *may*, none outside
   the list, none on two threads), so this trial says nothing of their agreement. **L and T are measured by this run's
   `h12` and `h9` alone.**

## The classification, arm by arm (the clerk's for `now`, from the text alone, amended by the repair; the synthesis confirms or amends it, and writes `adopted`'s and `rival`'s, before any session)

| program | `now`, read from spec § 13 lines 427 and 428 alone |
|---|---|
| `h1`, `h3` | **silent**: none of the five exits; no promise either way |
| `h2` | **may**: *"a jump no function of the C library makes (written in assembly, say)"* |
| `h4` | **line 427**: *"never a result or a signal handler, nor called by one"*: the call is the one the line forbids; **no sentence of `now` says what such a call does** (the handler sentence's trigger is a callback called *after* C left a handler, and `h4`'s handler is never left; only its words, *"or that C called it from inside one"*, name the case). **So `h4` has no text-right under `now`**, and stays in D4, reported (the critic's C16) |
| `h5` | **may**: *"a `swapcontext` away from it"* |
| `h6` | **may**: *"a jump that lands within it"*, **and line 427's case too**: the handler calls the function itself after its jump (the critic's C16). It stays in D3 where `h4` does not because the fourth exit's letter gives it a sentence under `now` and nothing gives `h4` one |
| `h7` | **may**: *"a jump ... onto a stack once given to `makecontext`"* (**the key runs it**) |
| `h8` | **may**: the same clause (the key aborts it) |
| `h9` | **may**: the first exit by its letter, the sentence naming no thread (**the key runs it**; T) |
| `h10` | **may**: *"a jump ... onto a stack once given to `makecontext`"* (**the key runs it**) |
| `h11` | **may**: *"a `setcontext` onto a stack once given to `makecontext`"* (**the key runs it**) |
| `h12` | **silent**: none of the five exits (the jump is `siglongjmp`; the stack was never given to `makecontext`); **the key aborts it** (L's program) |
| `h13` | **silent**: the handler is the program's own count, which returns; a reader who takes it for *"a handler of C's"* reads it into the sentence (the critic's C24 (ii)) |
| `n1` | **silent**: no handler of C's (259's widening reads it as *may*) |
| `n2`, `n3` | **silent** on the handler sentence; **the room's sentence applies by its letter** (*"Where C leaves a callback's frames without their returning (by a jump ...)"*: `leave`'s frames are left by `esc_leave`'s `longjmp`), its abort not reached (no value over 4,096 bytes, no lend, one callback). A reader's `ABORTS` or `NOT PROMISED` resting on the room's sentence is labelled R, not W (the critic's C16) |
| `n4` | **silent**: no sentence names a callback on a thread C made; line 383's *"reaching C through it on a thread that does not hold it aborts, naming the call"* (a handle's) is the nearest sentence a reader may borrow |

## The decision rules, registered before any session

- **D1, no widening** (259's finding): under `adopted`, for each of `h1`, `h3`, `h13`, `n1`, `n2` and `n3` (`n3` while its
  key runs on both graded platforms on the run's day, § The key), at least **4 of
  5** readers are build-right (`RUNS` with the lines), and at most **1 of 5** answers `NOT PROMISED`. Under `now` the
  counts are reported (259's run predicts 2 to 3 `NOT PROMISED` each for programs of this kind). Failing **reopens the
  text**: it still reads as covering switches it does not name.
- **D2, no narrowing** (257's finding), **its set fixed here, the same under every arm** (the critic's C15): `h7`, `h9`,
  `h10` and `h11`, every program the key runs on both graded platforms that `now` puts inside a *may*. Under `adopted`,
  for each of the four, at most **1 of 5** readers answer `ABORTS`. Failing **reopens the text**: a *may* is read as
  *will*, and a correct program predicted refused. **A text that names none of the four's exits does not pass D2 by
  silence**: the set does not depend on the arm's classification. (`h11` runs on route C only; route A refuses it in
  UNFOLLOWED's words, the drafted defect `d-lane-sig-family-1`, `00-shared.md` § 3.5; neither graded platform is
  route A.) **Not added**: 2848's `getcontext`/`setcontext` jump on the own stack then a jump out of a handler (runs),
  which the critic's C15 named: by `now`'s letter it is none of the five exits (no stack once given to `makecontext`,
  no assembly, no `swapcontext`), so it is a D1 program, not a D2 one, and `h1` already holds its class.
- **D3, the aborts and their words**: under `adopted`, for each of `h2`, `h5`, `h6` and `h8` (keyed `ABORTS` on both
  platforms, once the Linux rows exist), at least **4 of 5** readers are build-right or text-right, and every `ABORTS`
  among them names a handler not seen to end or a call from inside one (**words**). Failing **amends the text's
  words or classes** before it lands.
- **D4, reported**: `h4` (line 427's case: under `now` no text-right; under each other arm the synthesis says what is
  text-right for it before any session) and `n4` (the thread) under each arm, verdicts and words; `n3` against the build
  of the run's day where it splits or aborts there; **every label per arm and task, each grader's**, and the two graders' agreement per label.
- **D5, the list not read as exhaustive** (2848's reading, the critic's C13; added by the repair): under `adopted`,
  `h12` (keyed `ABORTS` on both graded platforms) at most **1 of 5** `RUNS`; under `now` the count and the L labels are
  reported. Failing **amends the text**: a list still reads as closed, and an exit it does not name as promised.
- **What the run does to the resolution**: D1 or D2 failing for `adopted` and holding for `rival` moves the resolution
  to `rival`; failing for both reopens the sitting on the text; D3 or D5 failing amends it. **The spec-warden's
  predictions** (its report, `## Predictions registered before the blind run`) are copied and hashed as the run starts,
  and read against an arm whose diff is byte for byte one of the warden's texts; else *unpredicted*.

## The size, the spend and the cap (the author's to set; nothing is authorised)

- **Size**: **31 `claude -p` sessions**: t1 and t2, three arms, five each, and one verdict session reading the 30
  reports (its folder built after the 30, holding the arms' specs, the diffs, the tasks and the reports, never a key).
  **The two-arm variant**: 21 sessions (20 graded and the verdict).
- **The inputs**: t1 **23,864 bytes** (`cat prelude.h sw.h h?.h h??.h h?.hero h??.hero | wc -c`, 28 files: thirteen
  programs, `prelude.h` and `sw.h`), where the clerk's eight were 10,757 (`h2.h`'s new first line takes 27 bytes off
  them: 10,730 by the same command over `h[1-8]`); t2 **9,522** (`cat co.h sw.h esc.h th.h n?.hero | wc -c`,
  unchanged); beside a spec of 35,246 bytes.
- **The cap per session** (repaired: thirteen programs where the clerk's caps were set for eight): **1.00 USD** each t1
  session, **0.60 USD** each t2 session, **1.50 USD** the verdict, margin **0.10**; the runner stops before any session
  where `spent + cap + margin` would pass the ceiling; at the caps the 31 cannot pass 15 × 1.00 + 15 × 0.60 + 1.50 =
  **25.50 USD**, the 21 cannot pass 10 × 1.00 + 10 × 0.60 + 1.50 = **17.50 USD**.
- **The ceiling proposed**: **25.50 USD** for the 31 (or **17.50** for the 21), the caps' own bound, so that no session
  is cut by the ceiling before the caps; **the author's to change**.
- **The price it rests on, an inference, unrun**: panel 257's t1 sessions cost 0.16 to 0.24 on 6,485 bytes of t1
  input and seven programs; panel 259's 0.34 to 0.47 (mean 0.43) on 17,929 bytes and seven programs, its t2 0.19 to
  0.26, its verdict 0.93 (`259-reports/llm-ergonomist-blind.md`, § The spend, read by the repair). Scaled from 259's t1
  by bytes (23,864 / 17,929) or by programs (13 / 7), a t1 session here is about **0.57 to 0.80**; a t2 session about
  0.25 to 0.35: **about 13 to 18 USD for the 31, 9 to 13 for the 21**.

## The area, the runner and every session

- **The folder**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/262-llm-ergonomist/`, **drawn at the
  synthesis and not before**: the top of an empty git repository of its own (`git init`, `.gitignore` `*`, local user
  `reader` <`reader@invalid`>, no commit), inside this repository's root and ignored by it, with `home/` and `tmp/`
  and one folder per session holding only `brief.md` (this folder's `brief-session.md`), `task.md`, `spec.md` (one
  arm, never named) and `programs/` (**t1's 28 files** or t2's eight).
- **The runner**: panel 259's, as adapted for its run (`259-briefs/blind/registration-runner.md`, its diffs against
  panel 256's committed bytes) and as panel 261's repair registered it (`261-briefs/blind/registration-runner.md`),
  **adapted again by whoever starts this run, every change written as a `diff -u` into a
  `registration-runner.md` beside this file before any session**: the folders' contents above, the two tasks, the
  key's per-platform rows, the grades, the labels and D5. The clerk and the repair wrote no runner.
- **Every session**: the command of `.claude/skills/panel/SKILL.md` § Procedure (`claude -p ... --restricted --safe-mode
  --strict-mcp-config --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools ... --max-budget-usd <cap>
  --output-format json`), with `--bare` and `--model claude-opus-5-5` as panels 257's and 259's runs took them, `<
  /dev/null`, `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area, **after the free preflight** (a key the API
  refuses, cost 0, its scan clean), counted a success only where `subtype` is `success`, `is_error` false and
  `terminal_reason` `completed`; the scan of each transcript; every folder's files read again by hash after the run.
  `claude --version` printed **`2.1.323 (Claude Code)`** on this Mac (the clerk, about 14:50; *carried* by the repair,
  not re-run), the version panel 259's run used. **Whether `--tools "Read,Write"` under `--bare --restricted
  --safe-mode` yields a Write tool is panel 259's open question** (31 of 31 sessions there could not write `report.md`,
  `259-reports/llm-ergonomist-blind.md:88-92`): the session brief's last line keeps the fallback.
- **Never started by a seat, by the critic, by the clerk, by the repair or by the synthesis's writer without the
  author's word.**

## What the synthesis added, 2026-11-27, before any session

Written by the sitting's synthesis
(`docs/panel/262-a-signal-handler-of-cs-ends-at-its-return-or-a-followed-jump-back-and-a-callback-on-its-thread-before-then-may-abort.md`),
**before any session is drawn; none is drawn and none runs.** Every hash is `shasum -a 256` of the file named, run by
the synthesis about 02:15; every count is `heroes measure` of the file with the seed's compiler built from `main`'s seed
at `77eb38534` (`sources 1e1eb23a1927d35b`, the base's seed, unmoved) from that commit's `vendor/`, no `--refresh`:
**a lower bound, vendored only**. The synthesis's scratch: `.claude/worktrees/panel262-scratch/synth/`.

**The arms, three as registered, each the landed spec with one text** (the rule of § The arms for a landed 261):

| arm | the spec the session reads | sha256 | its diff over the landed spec (sha256, bytes) | cl100k_base | claude-legacy |
|---|---|---|---|---|---|
| `now` | `spec/heroes-spec.md` at `main` `77eb38534` (526 lines, 36,120 bytes), panel 261's text landed by step 203f, the handler sentence byte for byte on line 438 | `9ff7d2a1bfda4323e54de004b27ef522bd86885a3fa249eb20406fb2a2942fc3` | — | 10,507 | 10,318 |
| `adopted` | `docs/panel/262-reports/synthesis/spec-adopted.md`: the landed spec with `synthesis/spec-adopted.txt` (`20ae64da520a38d8ad23a28776add2a3929da60e109e2b661f9407b98c6d1e6e`, the resolution's R1) in the handler sentence's place; `patch -p1` of the diff over the landed spec gives it (`cmp`) | `180f31da671e2a5a0be2211d1340b36a9f1d2c68d918aab8f06034c52f391a79` | `synthesis/spec-adopted.diff`, `909fb9085d92c8cd08e6f1b7fe8de672945fc84bfe8c94b238cb59879a90a665`, 3,930 | **10,710 (+203)** | 10,521 (+203) |
| `rival` | `docs/panel/262-reports/synthesis/spec-rival.md`: the spec-warden's `spec-w1` (`synthesis/spec-rival.txt` and `262-reports/spec-w1.txt` the same bytes; `spec-rival.diff` and `spec-w1.diff` `cmp` equal; the hash is the warden's `main-w1.md`'s, `7c57d49f6d48bb7c`) | `7c57d49f6d48bb7c4ecedfa71a1fad87b93d50ac3e6c9da65111b53ade0b6ad9` | `synthesis/spec-rival.diff`, `6bb97f71aee879e749cc562d3ba1aaafcfbbba4032a917d237b3178d022e059d`, 3,315 | 10,571 (+64) | 10,381 (+63) |

Each arm is 526 lines (`wc -l`). **`rival` is the text that fixes the other misreading**: `adopted` promises a run
after a followed jump and puts every `setcontext` in its *may*; `rival` keeps the old list with `h7`, `h10` and `h11`
inside its *may*, so D2 separates the narrowing as the warden designed it.

**How the warden's predictions are read**: `rival`'s diff is `spec-w1.diff` byte for byte, so the warden's `spec-w1`
column is read for it as registered; **`adopted` is unpredicted by the warden** (no warden text's diff is its own: it
is `spec-w2` with five changes, the synthesis's R1), and the warden's `spec-w2` column is read against no arm. The
warden's report is read at `d490152b740a8fe0dd2c0064db5b00574c7bf3765cb9c4aa2a166fcbfc5e5c88`
(`docs/panel/262-reports/spec-warden.md`).

**The key on `main`'s tip, this Mac** (the synthesis, 02:10 to 02:11, `sh blind/key.sh` with the seed's compiler and
`77eb38534`'s runtime, `panel262-scratch/synth/key-main.txt`): 34 cells, each one distinct output of three, every
verdict the one § The key gives at the base; `n1 --sanitize` runs. **It is not the run's day's**: the key is re-run
on both graded platforms under the build of the run's day before any session, as § The key owes.

**The classification, arm by arm** (the critic's K7 settled: under `adopted` the end of a handler by a jump is **a
promise with a stated exception**, *"unless this runtime cannot tell that the jump left it"*, which no graded
platform's key meets; `now`'s column is the clerk's and the repair's, confirmed):

| program | `now` | `adopted` | `rival` (`spec-w1`) |
|---|---|---|---|
| `h1` | silent | **promised**: ended by `siglongjmp` back into the call the signal interrupted | silent |
| `h2` | may (exit 1) | may: *"a jump no function of the C library makes"* | may (the same words) |
| `h3` | silent | **promised**: ended by its return | silent |
| `h4` | line 427, no text-right | may: *"from inside it"*; `NOT PROMISED` text-right | line 427, no text-right (its sentence opens *"after C left"*) |
| `h5` | may (exit 2) | may: *"a `swapcontext`"* | may |
| `h6` | may (exit 5) and line 427 | may: *"from inside it"*, and *"C jumping back into its frames starts it again"* | may: *"a jump into the handler's own frames"* |
| `h7` | may (exit 3) | **promised**: ended by `siglongjmp` back into the call the signal interrupted | may: *"onto a stack once given to `makecontext`"* |
| `h8` | may (exit 3) | may: *"a jump into a coroutine the signal did not interrupt"* | may (both clauses) |
| `h9` | may (no *"on its thread"*) | **promised**: the callback's thread is one where no signal handler of C's has run | **promised** by its second sentence (the handler was not on the callback's thread) |
| `h10` | may (exit 3) | **promised**: ended by `siglongjmp` back into the coroutine the signal interrupted | may: *"onto a stack once given to `makecontext`"* |
| `h11` | may (exit 4) | may: *"a `setcontext`"* | may: *"a `setcontext` onto a stack once given to `makecontext`"* |
| `h12` | silent (L) | may: *"a jump into a coroutine the signal did not interrupt"* | may (the same words) |
| `h13` | silent | **promised**: *"counting a signal with `hero_signal_watch` runs none"* | **promised** (the same words) |
| `n1` | silent | **promised**: no signal handler of C's | **promised** |
| `n2`, `n3` | silent; the room's sentence by its letter | **promised**; the room's sentence by its letter, its abort not reached | **promised**; the same |
| `n4` | silent | **aborts**, *"saying it ran on a thread this program did not start"* | silent |

**What is text-right, and what is wrong, where the classes moved**: under `adopted`, a `NOT PROMISED` on a promised
program is wrong, and one that names the stated exception as its reason is labelled **P** and counted as `NOT PROMISED`
by D1 and D2 all the same (the text's job is that a reader predicts the run); `h4`'s `NOT PROMISED` is text-right and
its `ABORTS` build-right with the words; `n4`'s `ABORTS` is build-right with the thread's words and its `NOT PROMISED`
wrong. Under `rival`, `h4` and `n4` are as under `now`. **The decision rules' sets do not move** (D1 `h1`, `h3`,
`h13`, `n1` to `n3`; D2 `h7`, `h9`, `h10`, `h11`; D3 `h2`, `h5`, `h6`, `h8`; D5 `h12`; D4 `h4`, `n4`).

**The refusal list: what an `ABORTS` may name and be right in its words**, by program, the same under every arm, since
the key's words do not move with the text: `h2`, `h4`, `h5`, `h6`, `h8`, `h12`: **a signal handler of C's not seen to
end, or a call from inside one** (either claim, in the reader's words); `n4`: **a thread the program did not start**
(or C's own thread). Any other cause named for one of them (the room's bound, § 9's stack, panel 258's stop at a jump
out of the allocator, guard D's re-entrance, line 383's handle on another thread) is wrong in its words and labelled
by its reason (R, S, O). No program of t1 or t2 is keyed in panel 258's or guard D's words, so neither is graded as a
build; a reader who names either for a program that runs is labelled O and quoted.

**The labels' rule, fixed before any session** (the critic's second reading, § 1: two readers of panel 259's 48 wrong
answers agree at kappa 0.90 on W, 0.85 on O, 1.00 on I, **0.50 on H and 0.45 on S**, and N, L, T, P and R have no
instance there, so their agreement is unmeasured until this run's own answers are read): § The grades' labels stand,
with three rules added.

1. **H** is given wherever the unit's answer block, or a choice point it cites as deciding the verdict, names the thing
   C left as a callback, a frame of C's or any frame, **in any words, a gloss in brackets included** (*"a callback
   ("handler")"*), or reads *handler* as anything but a signal handler.
2. **S** is given **only where the unit's own answer block offers § 9's stack abort as a ground for that program's
   verdict**; never from a list of reasons the reader gives once for every program or design alike. (The rule of §
   The grades: the label is the reason that decided this answer.)
3. **L's second clause**, *"also a `RUNS` that reads silence as a promise"*, is given **only where the key aborts by
   the runtime's design** (an arm of the guard, the thread's words, the room's bound); a `RUNS` against a key that is a
   defect's (a false message, a wrong value, a refusal a `blocking` defect names) is graded **O**, as `n3` was.

**The two readers** (the critic's K6): **two fresh agent sessions**, started by the coordinator **after the paid run**,
outside the area, each with no other context: each is handed, per wrong unit, the session's `result.md` under a random
id, the program's files, the reader's verdict and that it is wrong, and § The grades with the three rules above;
**never the arm's name or its spec, the key's outputs, the decision rules' results or the other reader's labels**.
Each writes its labels to a file and hashes it before the agreement is computed (`python3 -I
labels-259/agree.py <A> <B>`); disagreements are reported per label and per arm, never resolved by a third, and D4
keeps both readers' labels apart. They are not `claude -p` sessions of the run and spend nothing of its ceiling. **The
synthesis does not run them.**

**The ceiling: 25.00 USD for the whole run** (the author's word of 2026-11-26 about 21:45, at the head of this file),
lowering the registered plan's 25.50. **What it buys, at the registered caps** (1.00 each t1 session, 0.60 each t2,
1.50 the verdict, margin 0.10; the runner stops before a session where `spent + cap + margin` would pass 25.00, and the
verdict session runs last): **all 30 graded sessions in every case** (at their caps they spend 24.00; the 30th starts
at 23.40 + 0.60 + 0.10 = 24.10); **the verdict session whenever the thirty spent 23.40 or less**. By the price this file
infers from panel 259's (t1 about 0.57 to 0.80, t2 about 0.25 to 0.35, an inference, unrun) the thirty spend about 12
to 17 USD and the verdict fits; only if every session reached its cap would the verdict session be left out, and the
decision rules, which read the thirty, would be graded all the same.

## Amended before any session, 2026-11-27, by the coordinator

**The coordinator's decision, 03:26**: a process choice inside the run the author authorised (*ok a 25 dollars*,
this file's head), the same size and the same ceiling of 25.00 USD, recorded here so that **the author can turn it**.
Written by the run's session before any session is drawn or run, every number below a command's, run that session
(its notes: `.claude/worktrees/blind262-scratch/notes.md`). Nothing above this section is changed; where it and a
line above disagree, this section governs.

**What moved, found by the run's session at 03:18, before its first step** (the brief asked it to verify `main`'s
spec and stop if it had moved):

- **Step 208b, `750d2540d`** (2026-11-27 03:15:31 +0100, panel 260's amendment after its blind reading): `main`'s
  spec moved on **one line, 411**, § 13's first paragraph (`hero_os.h` and `hero_compiler.h` *"the runtime's own
  interfaces and none of the layer's"*, *"so a group over `hero_os.h` or `hero_compiler.h` says no `layer` unless
  its library calls the layer"*). `git show main:spec/heroes-spec.md | shasum -a 256` read **`736b534c...efd3`**
  (526 lines, 36,316 bytes), not the `9ff7d2a1...2fc3` registered above, which is `77eb38534`'s (re-hashed). The
  handler sentence, line 438, is not in the diff (lines 430 to 445 the same bytes).
- **Step 208a, `b626d4c05`** (2026-11-26 23:53:35 +0100, merged at `f1d7da1ba`, 2026-11-27 02:01:42): *"a group over
  the runtime's own `hero_os.h` that says `layer` is refused"*. **`h13.hero`'s first line is `extern "hero_os.h"
  layer`**, so the stage 2 of `main` at `d42929b67` (`sources 7c7c978a4dfeddf3`) refused it on this Mac, plain and
  `--sanitize`: *BUILD FAILED*, `error[ffi_layer_unreached]` at `h13.hero:1:20`, *"`hero_os.h` is the runtime's own
  header, none of the platform layer's ... take the word out"*, `fix (certain): take `layer` out`, where § The key
  says `h13` runs. Its other 16 programs gave the registered verdicts, 96 of 96 outputs equal to `key-mac-repair/`
  after `key.sh`'s normalisation.

**The four rules, before any session:**

1. **`now` is pinned to `main`'s spec at `9b88d3e84`** (`main`'s head at 03:26, 2026-11-27 03:21:27 +0100):
   sha256 **`736b534c852e39aff9bdde0b7e6008a836b25f74b4110b017cce145bd178efd3`** (`git show
   9b88d3e84:spec/heroes-spec.md | shasum -a 256`, verified), and **the run reads that commit's spec whatever `main`
   does during it**: its copy is `arms-9b88d3e84/spec-now.md` beside this file. **`adopted` and `rival` are the
   synthesis's two diffs applied to it** by `patch -p1` (no fuzz, no offset), each `cmp` equal to the run's session's
   probe: `arms-9b88d3e84/spec-adopted.md` **`d66dfd307384d3fa7b28cc72e757cc8d9e34680f260a92eaa5732c5870267b17`**
   (526 lines, 37,203 bytes; `synthesis/spec-adopted.diff`, `909fb908...a665`) and `arms-9b88d3e84/spec-rival.md`
   **`ecfb9931f987e29b25d72c15edb52796ff940cd4a3936b49070704c250214d7c`** (526 lines, 36,588 bytes;
   `synthesis/spec-rival.diff`, `6bb97f71...059d`). Each differs from the synthesis's arm above on line 411 alone, and
   `now` from `77eb38534`'s spec on line 411 alone (`diff`). The three lines are appended to `arms.sha256` below the
   old three, which stay (from this worktree's root, `shasum -a 256 -c` reads the new three `OK`; the first old line
   names `spec/heroes-spec.md` and fails here, as it did before, this worktree holding the base's spec). **The
   warden's predictions**: `rival`'s diff over the pinned spec is still `spec-w1.diff` byte for byte, so the warden's
   `spec-w1` column is read for `rival` as registered, its *"landed spec (`main`, sha256 `9ff7d2a1...2fc3`)"* read as
   the landed spec of the run's day, which differs from that one on line 411 alone; `adopted` stays unpredicted.
2. **`h13.hero` loses the word `layer` and nothing else**, the certain fix the compiler names: its first line
   `extern "hero_os.h"`, 487 bytes, sha256 **`06be86280a074048acbfff1880468051d8096b36d1dff33a3a436b25b533b42e`**,
   canonical (`heroes-next fmt h13.hero | cmp - h13.hero`, the pinned build) and checked (`heroes-next check`, exit
   0). The registered bytes stay beside it as `programs/h13.hero.registered` (`50e88d4a...02da`, 493 bytes). Two
   lines are appended to `inputs.sha256` below the old 40: the new `programs/h13.hero` and `programs/h13.hero.registered`;
   `shasum -a 256 -c inputs.sha256` reads 41 `OK` and one `FAILED`, the old `programs/h13.hero` line, history by
   design. `h13.hero.registered` is never copied into a session's folder. **`h13` stays in D1 with its registered
   class, `promised` under `adopted` and `rival`** (*"counting a signal with `hero_signal_watch` runs none"*),
   **because the new key runs it** (rule 3). t1's input is now **23,858 bytes** (`cat prelude.h sw.h h?.h h??.h h?.hero
   h??.hero | wc -c`, 28 files); t2's 9,522, unchanged.
3. **The key is re-run on both graded platforms under the pinned commit's build**, `9b88d3e84`'s stage 2, `sources
   7c7c978a4dfeddf3`: built from that commit's seed (`sources 1e1eb23a1927d35b`) in a `git archive 9b88d3e84` (the
   run's scratch, `tree/`), the same key as `d42929b67`'s stage 2 (both built and read; `git diff d42929b67 9b88d3e84`
   touches `rebel/` and `site/` alone). `sh blind/key.sh` over `h1` to `h13` and `n1` to `n4`, plain and `--sanitize`,
   three runs each:
   - **this Mac** (03:30:49 to 03:31:42): every one of the 34 cells one distinct output of three, **102 of 102
     outputs equal to `key-mac-repair/`** after `key.sh`'s normalisation, ASan's warning lines set aside: **no cell
     moved**; `h13` prints `watch 0`, `answer ran for 7`, `result 70`, `counted 1`, exit 0, plain and `--sanitize`;
   - **Linux arm64** (03:32:09 to 03:37:23), one container `blind262` (`docker run --rm --pull never`,
     `heroes-linux-arm64:latest`, image `338a1044ac90`; `aarch64`, Debian clang 22.1.8, `ldd (Debian GLIBC
     2.41-12+deb13u3) 2.41`), over `git archive 9b88d3e84 selfhost runtime seed vendor spec` with this folder's
     `key.sh` and `programs/` added (36 files, each its hash above, the last line per path): the seed `sources
     1e1eb23a1927d35b`, stage 2 **`sources 7c7c978a4dfeddf3`, this Mac's**; every one of the 34 cells one distinct
     output of three, and **every cell's first run equal to this Mac's** after ASan's warning lines and the shell's
     `Aborted` are set aside and pid, addresses and `(N)` normalised (`diff` per file: 0 of 34 differ). The
     container is removed (`docker ps -a`: none named `blind262`).
   So **no program of t1 or t2 splits between the graded platforms or between the plain and `--sanitize` builds**,
   and every rule has its key as § The key gives it; `n1 --sanitize` runs on Darwin under the day's runtime; `n3`
   runs on both, so it stays in D1. Outputs: `.claude/worktrees/blind262-scratch/key-mac-9b88/` and `linux/L-key/`.
4. **`claude --version` on the day: `2.1.326 (Claude Code)`** (read 03:23, 2026-11-27), where this file carried
   `2.1.323` (§ The area, the runner and every session). Whether its `--bare --restricted --safe-mode` session is
   offered a `Write` tool is asked of the free preflight's transcript before any paid session (the runner's
   registration says how), and the run stops if it is.
