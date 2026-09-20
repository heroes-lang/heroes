# Panel 171 — brief for the llm-ergonomist

**Read `spec/heroes-spec.md` and nothing else.** Not design.md, not the
repository, not the other briefs or reports, not earlier panel files. Your
verdict carries information precisely because your input is the document a
model is given to write Heroes, and nothing more. If you find yourself wanting
repository context, write that wanting down as a finding instead of satisfying
it.

Your report is an **experiment**, not an opinion.

## What has been decided, stated without the repository

The language's author has ruled: **a C function's pointer parameter is assumed
to KEEP what it is handed, unless its declaration says otherwise.** So handing C
a pointer into the program's own bytes — `s.cstr()`, `f.ptr()`, which § 13 calls
a lend — is **refused** at an unmarked parameter. A binding author who knows a
function does not keep writes a word on that parameter, and then the lend is
admitted. A function whose retention is unknown, or decided per call, takes a
copy instead — `x: cstr @ s.lease()` — and the program says when the copy dies.

**Your sitting decides the word and the sentence.** Nothing else.

## The sentence § 13 carries today, which you wrote

*A lend lives for its call and no longer: C keeping the pointer reads bytes the
program may have changed or freed since, and nothing checks it.*

Under the ruling, **the last clause becomes false**: something now checks it.
The sentence must say the default and name the word.

## The blind pair, plus two

Four candidate words, given in no meaningful order and label-stripped. Each
would appear in the grammar as `CParam = [ "@" ] ident ":" Type [ "counted_by"
ident ] [ "<WORD>" ] …` and in prose as: *a parameter marked `<WORD>` is one the
call does not keep past its return, so a lend may reach it; an unmarked `cstr`
or `ptr` parameter is assumed to keep, and takes a lease or a pointer C owns.*

> **W1** `reads`
> **W2** `lent`
> **W3** `transient`
> **W4** `borrows` — **note**: § 13 already uses this word on a RESULT, meaning
> *the call hands back one it keeps*. W4 tests whether a second meaning on a
> parameter is tolerable or a trap.

## Your four tasks

**Task 1 — cold reading.** Before writing anything, for each word: what does a
reader who has never seen it believe it means on a C parameter? One sentence
each. Then read the definition and say whether the belief survives.

**Task 2 — the two bindings.** For each word, using only the document plus the
proposed sentence, write two `extern` declarations and a call to each: (a)
`strlen(s: cstr)`, which does not keep, and (b) `keep_label(s: cstr)`, a
function that keeps the pointer and reads it later. Report first-try results as
counts: did you mark (a) and not (b), and did (b)'s call take a lease?

**Task 3 — the unmarked case, which decides your verdict.** Under the old
sentence a reader believed an unmarked parameter was fine to lend into. Under the
new default it is refused. **For each word, write the sentence you would put in
§ 13 so that a reader learns BOTH facts — the default and the word — in the
fewest words, and say whether any word's meaning makes the default sentence
harder to write.** That is the question no other seat can answer.

**Task 4 — a fifth word, if one is better than all four.** You produced the
sentence this milestone landed. You are the only seat whose input is what a
reader actually gets.

## What to report

A verdict on each word (approve · object · VETO, with the ground); first-try
results as counts; what you had to invent; the § 13 sentence you would land,
priced in your own word count; a falsifiable prediction about a reader's
behaviour with a number.

Write to `docs/panel/171-reports/llm-ergonomist.md`. You hold a veto on
non-local constructs.
