# M-stated-grammar — every form stated once, where the prose that governs it is

**Scheduled and opened 2026-09-12 by author decision**, the same day panel 133
sat. The author asked for a specification of the syntax in a super formal shape,
beside the prose one and without regard to the token count, then raised §1.6's
ceiling from 6144 to **8192** to make room for it, and gave a standing
instruction the contract now carries in § Precedence: take the most robust and
production-ready resolution, never the easiest, the compromise, or the cheapest
in tokens.

**What it delivers.** The productions of Heroes, in Wirth's notation, **inside**
`spec/heroes-spec.md`, each beside the prose of the construct it governs; and
`heroes grammar`, a subcommand that prints from the compiler's own tables the
half of a grammar that is already data — the twenty-one keywords, the token
spellings, and the nine binding powers with each operator's `ast.BinaryOp` name.

**What warrants it, with the instrument**: **§1.1**, and the warrant is panel
133's finding rather than a preference. Five seats with five different inputs
falsified the same half of the proposal that came in, the hand-written
productions in a separate file, and none defended it. The historian's survey
found every free-standing grammar archived — Rust's removed in 2019 and its
working group archived 2024-04-08, and `ziglang/zig-spec`, this proposal feature
for feature, archived 2025-11-23 three years after its checker had correctly
reported six divergences nobody repaired. What is still alive is Wirth's own
arrangement, which both the Oberon and the Pascal reports use: the grammar
beside the prose, and printed again collected.

**The reasoning a later milestone has to honour.** *The productions are not the
half that can be hand-written in a file of their own.* The seats proved it on a
draft written from the parser the same day: four wrong productions found by the
seat that never saw the compiler, a multi-line `extern` parameter list the
compiler accepts at exit 0 and the draft could not derive, and `[ Generics ]`
derivable and dead, which the guarding check would have passed green over
forever because `parse/tails.hero` reads generics before the extern branch. Each
of the four checks that milestone proposed would have been green over at least
one of those.

*And the raise relaxes nothing.* design.md §1.6's payment rule is unconditional
at every level, with no threshold to be under, so a block of productions owes a
named removal or a registered prediction exactly as a sentence does.

**What it does not deliver, so that nothing pretends otherwise.** No grammar
file of its own on disk: a file with a home nobody is obliged to open is where
the historian found the rot lands, so the collected view is printed and never
written. No exception table for the production check either — each block names
the parser function that reads it, the way a section names its anchor, because
the engineer measured that the alternative is 82 hand-written rows to judge 74
productions and that a name-only row is a mute button.

*******************************************************************************
**OPEN: 2**

- [ ] **M-stated-grammar** | the Part 11 experiment that scores what the productions bought, and the instrument that runs it | `design.md` Part 11 · `docs/panel/133-the-half-that-can-be-derived-and-the-half-that-cannot.md` § Predictions to score

    **Origin:** §1.6's payment rule, 2026-09-12. The productions are paid for by
    a registered prediction, and the robust reading of *"an instrument that
    exists today"* is to build the one that scores it in the same milestone
    rather than to lean on one that merely exists.

    The prediction is the llm-ergonomist's: spec plus grammar beats the spec
    alone by **at most +1 task in 10**, and the silent-divergence delta is
    **exactly 0**. Its own seat expects the grammar to buy nothing measurable
    here, which is what makes it worth registering.

- [ ] **M-stated-grammar** | whether `heroes grammar` should also print the productions it cannot derive, once they are in the spec | `selfhost/cli/` · `spec/heroes-spec.md`

    **Origin:** panel 133 R2, 2026-09-12, left deliberately unbuilt.

    R2 adopted the collected view as generated, and this milestone prints only
    the derivable half. Extracting the productions from the spec to print them
    beside it is the Rust Reference's `mdbook-spec` arrangement and is the
    obvious next step; it is not taken here because nothing yet needs it, and a
    second reader of the spec's text is a second thing that can disagree with it.

*******************************************************************************
