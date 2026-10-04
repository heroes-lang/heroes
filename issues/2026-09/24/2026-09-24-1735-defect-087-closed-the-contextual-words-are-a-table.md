---
kind: defect
area: cli
milestone: none
filed: 2026-09-23
commit: 3692af263d72a0f02492eea6bc93488ce4927201
github: none
---


# Defect 087 closed: the contextual words are a table the parser reads and the command prints

2026-09-24, M-agreed-retention step 6, in lane `7fe2cc48`, merged `bd33f30c`. Found
by panel 176's compiler-engineer (its § 9), reproduced by the coordinator
before filing.

- [x] **087 — `heroes grammar` says the language has six contextual words, and the parser reads eleven** | the sentence lists `as`, `link`, `package`, `tag`, `partial`, `owned`, and `parse/members.hero` and `parse/tails.hero` also read `acquires`, `borrows`, `consumes`, `counted_by` and `lent` | **closed 2026-09-24**, M-agreed-retention step 6 (`7fe2cc48`, merged `bd33f30c`) | `selfhost/cli/grammar.hero` · `selfhost/parse/members.hero:115`

    **Origin:** panel 176's compiler-engineer, 2026-09-23 (its § 9); reproduced
    by the coordinator the same day: `./heroes grammar` prints *"the six
    contextual words — `as`, `link`, `package`, `tag`, `partial`, `owned` —
    which are ordinary identifiers everywhere except the one position each is
    read in"*, and `grep` over the two parser files finds the other five read
    as markers.

    **Why it is a defect.** A tool that prints the language's own grammar states
    a count that is false by five, and panel 176's resolution would make it
    false by seven. A literal list of the words the parser owns is a premise
    that expires in silence; the seat priced deriving it from one table at
    about twelve lines.

## The repair

`heroes grammar` said *the six contextual words* and the parser read eleven,
because the words were literals at eleven sites in four parser files and the
command had no table to print: a count kept by hand. Now
`selfhost/keywords.hero` declares one constant per word, `WORD_AS` to
`WORD_BORROWS`, and lists them in `contextual()` in declaration order; the
parser reads the constants at every site (`parse/members.hero`,
`parse/tails.hero`, `parse/use_line.hero`, `parse/group.hero`), and `heroes
grammar` prints the table between the keywords and the operators. The sentence
that counted six is gone; what the command still cannot check — a production
against the function that reads it — is said in its place.

`tests/harness/suite_grammar.hero` holds the three facts a comment could not:
every `constant WORD_` declaration is printed, in order; every one is read by a
file under `selfhost/parse/`; and no file there spells one of the words as a
literal. Mutation-tested: a literal `"tag"` put back in `tails.hero` turns the
check red naming `tag`; `WORD_LENT` dropped from `contextual()` turns it red
after the compiler is rebuilt (the list is what the binary prints, so the
mutation is visible only through a rebuild, as for the keywords).

## The measurements

In the lane, with the compiler rebuilt from the edited `selfhost/` (74.68 s
user): the compiler's own tests 676/676 (one added), the net's own tests
167/167 (two added), `grammar` 9/0, `canonical` 2/0, `layout` 2/0, `order`
3/0, `records` 24/0. The seed regenerated and the fixpoint holds: the seed the
new compiler emits and the one the compiler built from that seed emits are
byte-identical (26 214 619 → 26 245 946 bytes).

**One stale count remains, and it is not this defect's to change**:
design.md:2668, a Part 6 row, says *the six contextual words §4.19 already
has*. Parts 1-11 change by panel; noted for the next sitting that touches that
row.

**And the merge commit's body says one thing too many.** `bd33f30c` reads
*selfhost/, seed/ and tests/ are byte-identical to the lane's*; selfhost/ and
seed/ are (`git diff`, zero lines), and tests/ differs by the four files defect
086's golden added to the trunk after the lane was cut — its `.hero`, `.h`,
`.expected` and blessed emission — which the lane never had. Corrected here,
2026-09-24, since a commit body is not rewritten.
