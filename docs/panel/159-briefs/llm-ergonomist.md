# Panel 159 — llm-ergonomist brief

**Read only `spec/heroes-spec.md` and this file.** Not design.md, not the
repository, not the other briefs, not the shared brief. Your seat found these
three, and you are being asked the same way: from the document alone.

**You are not told what the compiler does.** That is deliberate. If you infer it,
say you inferred it.

## Four tasks. Write the programs out

1. **Sort a list of records by a key, with a tie-break.** Two people share a
   score; the earlier one must come first. Write it. Then state, from the
   document, whether your program prints what you intended — and say how you
   know.
2. **Swap two elements of an array in place.** Write it from the document. If
   you find yourself building something around the problem, say what and why.
3. **A program that reads a file and reports failure.** `read_file` gives a
   `str?`. Write `main`. Then say whether `?` is available to you there, and
   quote the sentence that decides it.
4. For each of the three, say in one line **what you assumed** and **where the
   document left you to assume it**.

## Then, and only then

For each of the three, propose the shortest sentence that would have saved you,
and say which existing sentence it should MERGE INTO rather than stand beside —
the document's rule is one rule, one home, and merging is measured cheaper than
appending.

Rank the three by what they cost a reader who guesses wrong. Be concrete about
the failure: does the program not compile, or does it compile and do something
else?

## What your verdict is asked to answer

- Which of the three is the dangerous one, and why is it dangerous rather than
  merely annoying?
- Is any of the three better left unstated — is there a reading on which the
  document's silence is a ruling rather than an omission?
- Your proposed sentences, exactly as you would write them.
- You hold a veto on non-local constructs. Say plainly whether you cast it.

Your verdict owes a falsifiable prediction and the condition under which you
would change it. Write to `docs/panel/159-reports/llm-ergonomist.md`.
