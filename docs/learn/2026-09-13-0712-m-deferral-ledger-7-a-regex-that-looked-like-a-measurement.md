- [ ] **M-deferral-ledger 7** | A brief told five judges that the compiler contains 1457 variant constructions. Three of them tried to reproduce it and got 678, 1350 and 626. Find the regex that produced 1457, say what it actually counts, and then get the real number the way the critic did — from the compiler rather than from the text.

    **Where to look:** the correction in
    `docs/panel/142-there-is-no-asymmetry-to-restore-because-there-is-no-destructuring-anywhere.md`
    § Three corrections; the match-arm count of 994, which IS exact; and
    `heroes parse <file> --dump-ast`, which is where a count of a syntactic form
    should come from.

    **Why it matters:** the number was wrong in a direction nobody could see —
    it was too big, so it made the change look more expensive and the argument more
    comfortable. One judge then restated it as a fact inside an argument about
    whether the language needs the feature at all. A grep over source text answers a
    question about text; a question about a language's forms has to be asked of the
    thing that parses them, and this project owns that thing.
