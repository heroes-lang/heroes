# 073 — M-compatibility-promise: what `1.0.0` will promise, said before it binds, and the holes its sentences found when they were run

## Goal

Row 74 of the chain, opened on 2026-10-29 by the author's instruction and built the same day in the
lane `lane-compat-promise`: the paragraph `1.0.0` rests on, the suite that will make a broken promise
red, and the sentences of the specification the promise needs true before it binds. Two rulings of
the author's, both of the opening day, framed it: **the promise starts at `v1.0.0`**, the `0.x`
series promising nothing and its compatibility suite only telling (*report it, saying that the error
is a known case, like the one of `@=`, and so all is well*); and **before `1.0.0` no form enters the
language to keep an older program compiling** (CLAUDE.md § 2, *an absolute rule*). One sitting on the
specification, panel 232, the full panel with the blind seat within 12 USD, its resolution adopted
provisionally (decision 2262).

## What surprised

**A sentence that only says what the compiler already does is a claim, and running it is what finds
the compiler's holes.** The sitting was asked for sentences, and its seats wrote programs around
each one: the shapes beside a sentence found nine defects in one evening, six of them `blocking`,
and the lanes that repaired them found four more beside those. A promise drafted from reading would
have promised, on its first day, a silent segfault (a frame larger than the stack guard's window), a
use-after-free reached by a program with no C of its own (binding the runtime's own header), and an
abort a program could turn into exit 0 (a Heroes function bound as libc's `SIGABRT` handler).

**A sugar the specification calls one call was two code paths.** `x.f(y)` is `f(x, y)` by § 9, and
the compiler checked the two spellings by name but lowered them by shape: the direct `must(x)` fell
into a path that never asked whether it was a branch, and the dotted `n.ok()` into one that refused
it everywhere its own message said to put it. An equivalence the document states is written once in
the compiler or it diverges in silence; the repair made one function lower both, the IR of the two
spellings then identical byte for byte.

**`left to right as written` was false on a row of the language's own table.** `.default(v)` is a
branch (design.md Part 5), so its fallback runs only for an error, while a reader holding § 9's
sugar and a sentence saying every part runs left to right predicts a call. The order of evaluation
took five clauses in their homes, not one line.

**What the blind seat measured was mostly a model's habit.** Of 38 answers the registered rule
counted wrong, 25 held the right lines followed by prose (*the spec does not say ... I assumed*),
Sonnet writing such prose after correct lines in 19 of 40 answers and Haiku in 6; every one of task
W's 20 answers predicted the program. The one sentence that moved every reader was `exit`'s literal
refused: 0 of 10 predicted it without the sentence, 10 of 10 with it.

**The promise's instrument measured another verb.** The paragraph said *compiles*; the suite asked
`check`, which reads no C header: all 371 cases of the `unsupported` form pass `check` and are
refused by the build, and 21 of the corpus's 69 programs bind C.

**The thesis and a `1.x` promise pull opposite ways.** Thirteen thesis rules joined in the 52 days
from `v0.1.0`, six of them filed as defect repairs, and a built-in's name is taken everywhere: a
`1.x` that keeps refusing new mistakes keeps breaking `1.0` programs. The sitting adopted the strong
reading provisionally and put the other to the author.

**An old spelling kept for a release served nothing.** The eight place-less runtime entry points
waited for a release whose seed would know their twins, and the release's own seed cannot compile
against today's runtime at all: the author's rule against a deprecated alias reached the runtime's C
names, and they went the same day.

## What broke and why

**In the language, nineteen defects, each repaired by this milestone's batch**, found by
running the sitting's sentences and the shapes beside them:

- **The stack** (2253): a frame larger than what is left of the stack died at 139 with nothing
  said. Darwin's probe faulted the guard while `sp` stood megabytes above it, so neither of the
  handler's witnesses held; Linux's clang moved `sp` past the guard in one instruction, probing
  nothing; and at `-O2` clang inlined the program's `main` into C's before the guard was
  installed. A third witness (the probe routine's `pc`), `-fstack-clash-protection` on Linux, and a
  `main` kept out of line.
- **Two spellings of one call** (2254, 2272): the dotted and the direct spelling of `must`,
  `default`, `is_err` and `ok` were lowered by shape and checked by name; one function now lowers
  both. The note after an `unsupported` refusal named 24 of the 41 built-ins (2273).
- **The C boundary** (2255, 2256, 2257, 2258, 2259, task 2237): a block C recycled for another type
  was stopped as a stale copy, its mark cleared only on three routes; a program with no C of its
  own reached a use-after-free by binding the runtime's own header, under any group that read it;
  a handler bound from Heroes ran after the runtime's stop and ended at 0, since `abort()` honoured
  whatever disposition the program set; `==` on a dead handle named no place; a runtime of another
  release was told `internal error`; and eight entry points kept for a release no seed called.
- **Numbers** (2271, 2276): the `f32` ladder read its round trip back through a double and printed
  one float's text for its neighbour, and an `f32` literal was rounded twice, through `atof` and a
  narrowing, landing on the far neighbour at nine decimals of 7,584,028,237 checked exhaustively.
- **Messages and instruments** (2242, 2251, 2252, 2260, 2261, 2270, 2211, 2212): a note asking for
  `@` where `@` is refused, a width the program cannot widen offered, a digest no check read, a
  `.must()` named that the program never wrote, a swapped pair of labels told as four mistakes, a
  false example in design.md, fences of the specification compiled by nothing, and a float tie no
  golden printed.

**In the process.** A seat and then every lane stopped on an account's five-hour limit, three times,
and resumed from their own reports with nothing lost, once only after the night had passed. The
coordinator read a `pmset` listing cut by `head -5` as Amphetamine's absence, and an archive filter
by name dropped 276 text files that reading the archive back found; `bsdtar` honours one `-T`. A
critic and a lane wrote temporary files to `/tmp` and removed them; a fetch tool left three PDFs
outside the repository, removed with eight folders of rendered pages on the author's yes. Two peer sessions' notices expired unread.

## What landed, and what carried forward

Steps 0 to 9 on 2026-10-29 and 30 in the lane `lane-compat-promise`, the batch's lanes merged into
it, and the close after them, pushed to `main` with the author's standing yes of that night:

- the opening at row 74, the promise starting at `v1.0.0` (step 0); `exit` taking an `i64`, a
  literal status outside 0 to 255 refused where written and a computed one aborting at its call
  (step 1, task 2250, panel 230's fallback `11abort`); measurement 060, the checker's rules by churn
  (step 2, task 2213); the compat suite, judging from `v1.0.0` and telling before it, its known
  changes named (step 3, task 1066's instrument), then building what it judges and reading the
  thesis references (panel 232's R10); CLAUDE.md § 2's rule against a form kept for an older
  program;
- panel 232 (steps 4 to 8): its briefs repaired on the critic's 34 findings before any seat, the
  blind seat registered before its first session, the synthesis provisional (decision 2262), its
  resolution on the milestone's page and in `.claude/rules/records.md` § Release tags, design.md
  § 4.17's codes never renamed in a `1.x`, and the author's reading of 2026-10-29 ratifying R1 to
  R12 with R12 turned to a map's insertion order (feature 2274);
- task 2237: the eight runtime entry points without a place removed beside their `_at` twins,
  `HERO_RUNTIME_ABI` 31 to 32, and the seed refreshed across it by `abi-put-back`
  (cd47d9b85, 276,991 lines and 28,876,131 bytes, rebuilt by the seed before it);
- the batch: defects 2211, 2212, 2242, 2251 to 2261, 2270 to 2273 and 2276, nineteen, each with its
  `fixedbugs-` cases, and feature 2274, a map's keys in the order they were first written, with
  the debugger's formatter and three examples saying that order;
- step 9 (641b43017): the sitting's ten sentences and § 10's map order in the specification,
  10,343 to 10,514 real (+171), 7890 to 8019 on the vendored ranks.

**Re-measured at the batch's gate, 2026-10-30**: the seed's stage 1 building stage 2 and stage 2
building stage 3, the two emitting the same bytes; the compiler's own 1,786 and the net's own 482;
the full net on stage 2, 34 suites, `compat` the new one, 10,545 passed and 0 failed once the
closing commit repaired what its parallel pass and its re-run alone found: `records` 25 and 3 (a
sentence label read as a numbered milestone, a placeholder path in a comment, panel 231's
ratification written without the comma its check reads) and `unsupported` 370 and 1 (defect 2117's
header declaring `main` on a line past 4096 bytes, its message moved by defect 2253 as the three
cases re-read at 801603b2a were, and missed by that re-reading's filter); the floors it told raised,
`lines` 498 to 632 and `full` 37 to 53; the emissions of the two cases this Mac skips blessed on
Linux arm64, every other file of the image's run the same bytes as this Mac's; the site's build
green. `heroes check selfhost/main.hero`, timed on the batch's tree with the machine still, seven
alternated pairs and the first discarded, every run exit 0: 6.89 s on the trunk's compiler and 7.03
s on the batch's, +2.0%, the spread of one binary 0.05 s; a batch of robustness repairs lands its
cost reported (CLAUDE.md § Precedence), and task 2279 finds which commit pays it.

**Carried forward**: defects 2275, 2277 and 2278, `adjacent`, filed beside the batch and left; the
CI's four legs on the close's push, Windows among them for 2253 and 2257, run on no Windows machine
before it; the census and panel 187's R2 after the push, the batch having changed what the compiler
refuses; task 2279 on M-latency-ceiling, the 2.0% `heroes check` lost; the paragraph's publication
in `README.md` at M-publication-gate.

## The tag, 2026-10-30

The close (29715956f) went out with the author's standing yes of the night before: 62 commits, 48
of them touching a path the site's build reads, the site built green over the trunk first. The
push landed at 12:03, 77 minutes after that build ended; what held it is not known to this record.
The nineteen defects, feature 2274 and tasks 2237, 2250, 2213, 2245 and 1066 were closed by 12:06,
before the CI's net could read them open: `records` read 27 and 1 over the close commit, its
`homes` finding row 74 done and six of its items standing, and 28 and 0 once they were closed. The
34 open features and tasks two tags had passed were re-checked against the close commit and each
given a dated comment by 12:10; one (810) had a premise overtaken by panel 232's paragraph, what it
asks untouched, and none closed. The tag goes on the commit that says so, made after the closes,
as M-generated-programs' went, over a list with no `blocking` or `systemic` defect (2275, 2277 and
2278 `adjacent`, at their first tag) and no decision open.
