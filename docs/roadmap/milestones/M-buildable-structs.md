# M-buildable-structs — the structs a program can build

**Opened 2026-10-14, at row 66, by the author's instruction**, meant as: *when
you have finished, start the next step*, given once M-typed-inspection had
closed; in its own lane, `lane-buildable-structs`, from `fab354f7`.

**What the opening found, which this page had not said**: panel 194's R1 with
K had landed already, under this milestone's name, while its row stayed
`scheduled`. `838d2719` (step 1, `rest: zero`, the bit in `Checked`, the
whole-object zero `emit/zeroed.hero` writes), `a975e3d6` (step 2, the
`ffi_field_type` note naming `i8`) and `54c5f618` (step 3, § 13's two
sentences), all of 2026-10-06, reached the trunk through lane b13-bs's merge
into batch 13's round (`3e2a5035`). Panel 196's spec-warden measured the two
sentences at 9,583 real, panel 194's spec-warden's prediction (9,583 ± 3)
scored a hit, and the round's `--refresh` priced the merged document at 9,831
(`5c782a8f`, 2026-10-07; the ledger's row 7467). Defects 187, 397, 401 and
549 have changed the form's own modules since (`git log` over them, read
2026-10-14). What the milestone still owes is the open `feature` and `task` issues of its
GitHub milestone, 1332 to 1337, the first of which is the landing that already
happened, and the walk every tool that re-prints a program owes a new form,
which the landing of 2026-10-06 did not finish: neither highlighter paints
`rest: zero`, and `heroes grammar` lists fourteen contextual words and not
these two, though the parser reads them in one position (both read
2026-10-14).

**Measured 2026-10-14, the compiler-engineer's prediction for R1's landing**
(panel 194, *200 to 330 `selfhost/` insertions, no DECIDED row moved but
possibly `ast.hero`, a cold `--emit-c` within 0.5% of its parent*), on
`838d2719` against its parent: **393 insertions**, by `git diff --numstat
-- selfhost` with the 63 lines `expr_kinds.hero` took from `ast.hero`
left out, so **above the range**. The overrun is the sitting's own amendment,
which no seat built before predicting: the whole-object zero
(`emit/zeroed.hero`, 133) and its call (`emit/construct.hero`, +25); the rest
reads 235, beside the seat's own build at 241. No DECIDED row moved (`ast.hero`
517 of 550). The cold `--emit-c` of the parent's tree, three rounds
interleaved, each in a fresh directory holding `selfhost/` and `runtime/`:
the parent's compiler 1,084.43, 1,084.92 and 1,084.49 billion instructions
retired, R1's 1,085.08, 1,084.53 and 1,084.52, **+0.01%** on the means; CPU
time, user plus sys, 98.97 s against 99.44 s on the means (+0.47%, R1's third
run the slowest of the six at a load of 1.89), real within 3.3% of user plus
sys in every run, on this Mac's 8 cores at a load of 1.65 to 2.22 with other
lanes working; the emitted C identical, 1,272,814 lines. The emission half
**held**, the insertion half **missed**, high.

**The walk, finished 2026-10-14.** `rest` and `zero` are contextual words, by
`selfhost/keywords.hero`'s own definition (read in one position, a name
everywhere else): two `WORD_` constants read at the five sites that spelled
them as literals, so `heroes grammar` prints sixteen; both highlighters paint
them where `parse/rest_words.hero` reads them, which vscode-textmate's own
engine and the site's tokenizer agree on, line for line, over eighteen sample
lines; a round trip of the formatter and a line of the AST dump are unit
tests. `keywords.hero` went to 312 lines past §11's 300, so its two tables
became two modules, the foreign words now `selfhost/foreign_words.hero`.

**The union under MemorySanitizer, run again 2026-10-14** on the lane's
compiler in `heroes-linux-arm64` (Debian clang 22.1.8): the C
`run/ffi-rest-zero-a-union-none-of-whose-members-is-named` emits is clean at
`-O0` to `-O3`, printing its `.expected`, and a control that copies heap bytes
nobody wrote over each construction, in place of its whole write and its
`memset`, is reported at every level, two reports each. The Windows box, which
panel 194 named beside Linux, is unrun.

**Scheduled 2026-09-18** out of panel 164, which recorded *a real five-field
`utsname` needs 1280 literal zeros*. **This file opened 2026-09-25**, with
panel 178, while the milestone's row stays `scheduled`: one `**OPEN**` row at a
time is what `site/src/lib/chain.ts` accepts, and M-agreed-retention holds it.

**What the milestones before it left standing.** Panel 163 found there was no
wall for a struct C fills: a function that returns the record, and `partial` to
declare only what is read. What it did not do is make a struct with long arrays
writable in the program's own construction, and it left `[0; 256]` unpriced.

**Why it is a milestone and not a note.** The repository was the wrong sample:
its longest fixed array is 8, so nothing here ever hit the wall. The headers
M-core-packages will bind are the right one, and panel 178's census counts 42
public structs on Darwin, 29 on Linux arm64 and 23 on Linux x86-64 with an
array longer than 8, in three kinds: bytes the program never touches, buffers C
fills, and bytes the program writes. The same struct differs by platform
(`utsname` is five of `[256]` on Darwin and six of `[65]` on Linux), and an
all-zero value is not valid for every type on every platform (a zeroed
`pthread_mutex_t` locks on Linux and is `EINVAL` on Darwin). **Why here, before
M-core-packages**: the argument M-readable-bytes made for its own position, a
binding written before the wall moves is a binding rewritten after it.

**What panel 178 decided, provisionally**: `rest: zero` on a group record's
construction, zero admitted as bytes and never as a claim, the header's own
initialiser as the way C says which value is valid, C's `char` spelled once,
and defects 091 to 094 filed. What the landing owes and what the sitting left
open are the open `feature` and `task` issues of its GitHub milestone.

**This page moved 2026-10-06** from `docs/work/milestones/`, where panel 178's
branch wrote it on 2026-09-25, when that branch was merged: the page keeps its
reasoning, and on 2026-10-06 its six items became files of `issues/2026-09/25/`
(`.claude/rules/records.md` § A live list), each as the page held it, and on
2026-10-12 GitHub issues.
