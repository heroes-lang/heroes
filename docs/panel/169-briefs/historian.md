# Panel 169 — brief for the historian

Read `docs/panel/169-briefs/00-shared.md` first. You are advisory and hold no
veto. **You must verify every date, count and claim by web search**, because this
seat is the most hallucination-prone in the panel and unsourced precedent is
inadmissible.

## What you established at panel 167, and it is being used

Your finding there: **no language in the survey enforces foreign retention
statically. Not one.** Ten ecosystems. The static enforcement that exists — Ada's
accessibility levels, Rust's lifetimes, Swift's `@escaping` and `~Escapable`,
Hylo's second-class references, Cyclone's regions — is always on the **caller's**
side and stops at the foreign boundary by construction. cgo's designer wrote it
down in 2015. Java is restricting JNI. Cyclone is dead and D's DIP 1000 is seven
years in preview.

**That finding is now load-bearing in a way it was not at panel 167, and this
sitting needs you to say whether it is being used correctly.**

## The question that is yours alone

Panel 168's completeness critic found that **defect 068's corrupting action is a
Heroes statement**, not a C one. In its reproducer the wrong answer is produced
by an assignment two lines below the lend, in the same function, to the very
binding whose field's address was lent. **No C code participates in the
corruption; C only observes it.**

So 068 was classified with 066 as *foreign retention* — the class you proved
nobody enforces — and filed as unsolvable on that ground. **But it is
caller-side, which is where your own survey says static enforcement exists
everywhere.**

1. **Is that reading of your survey correct?** Name the languages whose rule
   would refuse the C program above, and say what they call it. Rust's borrow
   checker on a `&mut` while a raw pointer is live; Swift's exclusivity
   enforcement; Ada's accessibility; C++'s lifetime profile and the clang
   `-Wdangling` family; Hylo's second-class references. Which of these is
   **caller-side and cheap**, and which needs a whole ownership system?
2. **What is the smallest such rule that ever shipped?** The candidate on the
   ballot is: *a binding whose field's address has been lent in this function is
   not re-assigned in this function*. That is an over-approximation with no flow
   analysis. Has any shipping language shipped something that small, and what did
   it over-refuse in practice?
3. **The handle route.** Heroes already has `acquires`/`consumes`/`borrows` on an
   opaque handle with a run-time live set that aborts at exit. The proposal is
   that a retaining C function take a handle rather than a lent pointer. **Who
   else did this?** Candidates to verify rather than assume: Ada's limited types,
   Java's `Arena`/`MemorySegment` scopes in the FFM API (JEP 442/454) and what
   their `close()` does, .NET's `SafeHandle`, OCaml's custom blocks, Zig's
   allocator-passing, Python's capsules. **Which of them enforces at compile time
   and which at run time**, and did any move from a pointer to a handle
   specifically because a pointer could not carry the rule?
4. **The FFM precedent is the sharpest one and is yours to check.** Java replaced
   JNI with an API where a foreign pointer is scoped to an `Arena` and use after
   close is an exception. Is that the handle route under another name? What did
   it cost the ecosystem, and what did it refuse to do?

## What to report

- each claim with its source and its date, and say where a search returned
  nothing;
- which of the four routes has the best precedent and which has none;
- **the negative you are best placed to give**: has any language tried the
  caller-side rule for this exact shape and **withdrawn** it? That is the finding
  that would change the sitting.
- a falsifiable prediction.

Write your report to `docs/panel/169-reports/historian.md`. The coordinator will
copy it beside the others, because your seat has no write tool in some
configurations — if you can write the file, do.
