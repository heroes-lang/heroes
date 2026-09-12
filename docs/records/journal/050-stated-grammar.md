# 050 — M-stated-grammar: every form stated once, where the prose that governs it is

## Goal

The author asked for a specification of the syntax in a super formal shape,
beside the prose one, explicitly without regard to the token count. The obvious
reading was a file: `spec/grammar.md`, EBNF, outside §1.6's budget on
`spec/reserved-words.md`'s precedent, kept honest by checks.

That reading was refused on 2026-08-12 as an appendix, and the refusal's reason
had two halves with different fates: *"spec tokens"* dissolves with a separate
file, and *"a reader would then have to reconcile with the prose"* does not.
Panel 133 was convened on the second half.

What was delivered instead is a split by derivability. The productions are
authored **inside** `spec/heroes-spec.md`, each beside the prose of the
construct it governs, in Wirth's notation; `heroes grammar` prints the half that
is already data in the compiler — the twenty-one keywords and the nine binding
powers, each operator with its `ast.BinaryOp` name beside its spelling — and
prints the productions collected, reading them from the document at run time
without writing a file.

## What surprised

**A sitting's own premise can be false, and the seat that reads least is the one
that finds it.** design.md Part 7 item 16 said variant construction was
unspecified and that a program written from the spec alone could not build one.
`spec § 9` had stated it since the previous day, commit `d245239a`, panel 126's
re-shaping. The llm-ergonomist, given only the specification and forbidden the
repository, wrote `.circle(r: 3)` correctly and cited the clause before it was
allowed to open anything else. Nobody had noticed the paragraph go stale, and
nothing could have: no instrument reads a design.md sentence against the spec.

**Five different inputs falsifying the same half is a stronger result than five
agreements.** The seat that never saw the compiler found four wrong productions.
The seat that compiles bindings found the productions narrower than the language
in one place and wider in another, and predicted that the guarding check would
pass green over the wide one forever. The seat that costs implementations
measured 82 hand-written exception rows to judge 74 productions. The seat that
guards the budget found 1322 of 2903 tokens were hand-written prose carrying
five language rules the prompt did not have. The seat that checks precedent
found every free-standing grammar in the survey archived, including one with this
proposal's exact mechanism. None of them was told what the others were doing.

**Two records were reading a source backwards, and both were found by being sent
to verify a citation rather than to form an opinion.** Panel 024 had cited
Aycock et al. as evidence that a grammar buys nothing; the paper's finding about
parallel examples is about *translation*, and on the tasks nearest to writing a
language its best arm was a compact formal description **plus** examples. And
Wirth, whose reports this project claims descent from, shipped the grammar
**twice** in both Oberon and Pascal, inline and again collected — so *"the prose
plus a compiler diagnostic beats a second description"* was a departure from him
rather than an application of him.

**A tokeniser's ratio is not a property of a document.** The productions cost
+1134 vendored and **+1815** on the reader's own instrument, a ratio of 1.60
where the whole document sits at 1.26, because a grammar is dense punctuation
and the real tokeniser splits it finer. An estimate from the document's own
ratio would have been 384 tokens low. The vendored figure is a lower bound and
saying so is not a formality.

**The instruments caught the author of this milestone twice, outward-facing both
times.** `README.md` promised the whole language in fewer than 300 lines and the
spec is 376. The site's claims checker refused the build twice: six sentences
named a 6K ceiling where the tree derives 8K from the suite's constant, and the
start pages said "eleven verbs" without naming `grammar`. Neither was noticed by
a person.

## What broke and why

**`heroes measure design.md` exited 1 and declared a breach of §1.6, citing the
section that says the compiler has no budget.** The default branch weighed the
vendored maximum against `CEILING`, which had been a `claude-opus-5` real number
since 2026-09-09 — the wrong ceiling and the wrong scale at once. The quieter
half was worse: a file with no ceiling printed "Headroom: 3241", a number a later
session could quote as permission to spend. A path no ceiling covers now prints
its counts, says so, and exits 0. **The test that should have caught it asserted
the defect**: both its assertions used `path: "x"`, so an arbitrary path being
judged at all was the thing under test.

**A comment claimed a reach the check did not have.** `grammar/powers` compares
the source read as text against the command that compiled it — both spellings of
the same match — so `.percent => .add`, panel 067's defect, makes both sides
agree and the check passes. It is closed one level down, by a unit test
asserting that `%`'s operator spells `%` again; falsified by making exactly that
edit, which turned the unit test red and left the suite green. The comment was
rewritten to say which half it holds.

**Slicing a byte off the specification aborts on its own em dash.** The
collected view's first loop walked bytes and the language stopped it with
"string slice splits a character" — the same rule `tests/harness/spec_text.hero`
already carried, learned again rather than read.

**An extractor of one's own is an extractor nothing exercises.** The command's
first production reader demanded a non-space in the fifth column and silently
dropped every continuation line, printing three productions truncated and
complete-looking. The suite had its own correct reader and passed over it. The
suite now reads what the command prints, which is what makes the command's
extractor the only one — and immediately found two checks overrunning their
sections, because `3  ==  eq` in the precedence table parses as a production
named `3`.

**Two files passed §11's threshold and both split on real seams rather than
taking a ratified exception.** `cli/measure.hero` gave up the reader's own count
and `--refresh` to `measure/pinned.hero`: counting a file on tables we carry and
asking the network what the real tokeniser said are different questions, with
different inputs and cadences. `grammar_expr.hero` gave up the operator tables
to `operators.hero`: the knot is for climbing a stream of operators, not for
saying what they are, and `op_text` had been sitting under that file's test
heading while being the only spelling table in the compiler.
