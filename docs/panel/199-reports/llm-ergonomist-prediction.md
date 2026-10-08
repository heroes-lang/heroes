# Panel 199, llm-ergonomist (blind seat): the coordinator's prediction before the B readings

Written by the coordinator at 10:41 on 2026-10-08 (`date`), after the three A
readings (`llm-ergonomist-r1-n.md`, `-r2-n.md`, `-r3-m.md`) and their scoring
by the coordinator (each program: `check` 0, and at `-O0` and `-O2` the key's
line, 2, 120 and 3, at exit 0), and before any B session starts, so the B
readings score a prediction and not a story.

**What the B sessions read.** The folders `r1-m` (`p1.hero`), `r2-m`
(`p2.hero`) and `r3-n` (`p3.hero`) hold, as `output.txt`, the transcript the
compiler-engineer's prototype printed for the program, the files `B-p1.txt`,
`B-p2.txt` and `B-p3.txt` of `<scratchpad>/199-compiler-engineer/` (md5 5724ce25,
51af7c29 and a41d4280), copied unchanged: `check` exit 1 with
`error[endless_recursion]` and its notes. The spec is unchanged (the
spec-warden's *variant B's spec stays unchanged*: one change per variant).
`r2-o`, the second reading of question 2, is not run: under the reading where a
hidden abort ends a path the prototype refuses nothing in `p2.hero`, so its
transcript is the A one (`B-p2-R2.txt`, byte for byte `r2-n`'s), already read.

**What I expect.**

1. All three B programs are repaired and correct (`check` 0, the key's line at
   both levels). The A readers were 3 of 3, so B has no room above A: **this
   experiment can show that B is not worse, and cannot show that it is better**.
   It measures what a reader does once the mistake is made and shown, and the
   question the sitting turns on, whether a model makes the mistake at all (the
   critic's question 7), is not asked by any reading.
2. The difference, if there is one, sits in `confidence`: where the A readers
   named the clang warning and the panic or the silent hang as what told them
   what to change, I expect the B readers to name the error's headline, and the
   one-line note about UFCS (`x.f(y)` is `f(x, y)`) on `p1` and `p3`, which `p2`
   does not have.
3. `choice_points` of the B readers on `p2` should mention the base case's
   threshold (`n <= 1` or `n == 0`) as the A reader of `p2` did; none should
   mention the message's wording as a hesitation.
4. `context`: the folder's own files and the harness's environment information
   (working directory, platform, date, the account's email), no project rule.

**What would falsify it.** A B reading that does not reach the key at both
levels, or one whose `confidence` names no part of `output.txt`, or a `context`
that names a project rule: the first two would be a finding against the
prototype's message, the third voids that reading until it is re-run.

The spec-warden registered its own predictions (`spec-warden.md`, P-A and P-B);
this one is the coordinator's and is not a substitute for either.
