# Panel 167 — spec-warden report: the lend has no lifetime rule

Defect 066. Every number below was run in this sitting, in a copy of the tree at
M-declared-extents step 6, against `claude-opus-5` through
`POST /v1/messages/count_tokens` via `./heroes measure spec/heroes-spec.md --refresh`.
Nothing here is an estimate; nothing here is *unrun*.

## The budget veto is NOT engaged, and that is said first

Ceiling grepped from `docs/design/design.md:255` at the start of this sitting:
**10240**, measured by `claude-opus-5` through `count_tokens`. Not taken from the
brief.

Baseline re-measured twice, before and after every draft: **8154 real, digest
`249990ca1b6b2f66`** — identical to the brief's. With §4.19's FFI floor of 60
mortgaged, **2026 net free**. The most expensive route on the table (B, +60)
lands at 8214, leaving 1966. **No route on this ballot comes within 1900 tokens
of the ceiling.** The budget decides nothing here and I will not let it pretend
to. What decides is Principle 0 and §1.12.

## The measured cost of every route

Each draft was applied to `spec/heroes-spec.md` in the copy, refreshed, and
reverted; the revert was diffed clean and the baseline digest re-read each time.

| route | draft | after | delta | digest |
|---|---|---|---|---|
| — | baseline | 8154 | — | `249990ca1b6b2f66` |
| **A** | field lease | 8176 | **+22** | `091c91cf5b32d798` |
| **B** | `keeps` on `CParam` + production row | 8214 | **+60** | `5cee0dc5438abd08` |
| **C** | the hole, standalone sentence | 8186 | **+32** | `6496e1ce60e2c11d` |
| **C-joined** | the hole, joined to the lease sentence | 8174 | **+20** | `11cd98f01889b9df` |
| **C-absorbing** | joined, syntactic rule made a consequence | **8167** | **+13** | `44fa44bb11d45b03` |
| **D** | withdraw the field lend | 8074 | **−80** | `4f5b16af5e9fdd16` |

The drafts, verbatim as measured.

**A**, replacing `spec/heroes-spec.md:370-374`:

> `x: cstr @ s.lease()` is a COPY of the bytes that C may read for as long as the
> program says, `p: ptr @ f.lease()` the same for a field's, and `end_lease(@x)`
> frees either and empties the cell. […unchanged tail…]

**B**, inserted after line 369, plus the production:

> A `ptr` or `cstr` parameter marked `keeps` says C retains the address past the
> call: a lend is refused there and only a lease may be passed.

    CParam = [ "@" ] ident ":" Type [ "counted_by" ident ] [ "keeps" ] [ "owned" ident ]

**C**, inserted after line 369:

> A lent address is valid until the call returns and no longer; a C function that
> keeps one is on its own.

**C-absorbing**, replacing lines 370-374 — the form I recommend:

> `x: cstr @ s.lease()` is a COPY of the bytes that C may read for as long as the
> program says, and `end_lease(@x)` frees it and empties the cell. **A lend is over
> when the call returns, so** a lend and a lease name stand only as an argument of
> a call, nothing else writes a lease's cell, and a lease nobody ends, like a
> handle nobody consumes, aborts when `main` returns, saying how many.

**D**: deletion of lines 367-369, the `f.ptr()` lend and its write-back clause.

## The question that is mine alone, and its premise is false

*§ 13 already carries the `cstr` lease's lifetime sentence — does the field
direction need its own, or does the existing one generalise?*

**Neither. There is nothing there to generalise, because § 13 has no lend
lifetime rule for EITHER direction.** Lines 370-374 are the **lease's** rule: a
lease may be read *"for as long as the program says"*. The only sentence naming a
lend is syntactic — *"A lend and a lease name stand only as an argument of a
call"* — and that constrains where the NAME may appear, not how long the ADDRESS
lives. Read the section for the word and it is absent.

So the brief's framing — a field-direction hole beside a covered `cstr`
direction — is wrong, and I ran the shape beside it rather than reasoning about
it (CLAUDE.md § RUN IT, CL-061).

### `s.cstr()` has the identical hole, measured today

```
function stash(n: u64)
    s = "H".repeat(n)
    k_register_s(s: s.cstr())     # C parks the address in a static

function main()
    stash(n: 40)
    print(noise())                # 10
    print(k_read_later_s())       # 0    — the honest answer is 72
```

```
$ ./heroes run keeps.hero              ->  10 / 0,  exit 0, no diagnostic
$ ./heroes run keeps.hero --sanitize
==16319==ERROR: AddressSanitizer: heap-use-after-free on address 0x606000000270
    freed by thread T0 here: ... in h_keeps_main keeps.hero:11
```

The field probe from the shared brief reproduces beside it, `stack-use-after-scope`,
also exit 0. **Two directions, one defect.** Defect 066 is a LEND defect written
down as a FIELD defect — CL-078's shape exactly, and the shapes beside it are
where what it actually is became visible.

**The corpus reverses with it.** The brief counts the field lend at 33
occurrences, zero in `examples/`. The `cstr` lend is **59 occurrences across
`tests/golden` and `examples`, 10 of them in four shipped examples**. The half
nobody put on the ballot is the half the corpus actually uses.

**Answer to my question, then:** the field direction must NOT get its own
sentence. One clause quantified over *a lend* binds both directions, and it
joins a sentence already present — my seat's panel 122 finding, and here the
cheapest draft is also the only complete one. §1.2 and §1.12 agree for once.

## Verdicts, per route

### Route A — the field lease: **object**, on §1.0 and a measurement

**Ground: the lease already exists for `cstr`, and the `cstr` lend still reaches
use-after-free today.** That is not an argument, it is the probe above. A lease
is a form the program MAY choose; it refuses nothing. Land route A and
`keep.hero` still compiles, still runs, still exits 0, still prints the wrong
number — because nothing stops the author writing `.ptr()`. **Route A closes
zero of the two reproductions**, and the shared brief's *"closes 066 by
construction"* is falsified by the construction's own twin, in the language since
2026-09-09. +22 tokens for a second way to be right when the first way to be
wrong is untouched.

### Route B — the binding-author's mark: **veto**, on an already-ratified measurement

**design.md:2344-2347 refuses this route by name**, and it was measured rather
than argued: *"no declaration-site mark can express retention at all:
`sqlite3.h:4888` puts the decision in the **fifth argument** of one declaration,
`curl_easy_setopt` in its second, and 0 of 71 `cstr` parameters in this tree are
decidable from a header."* Retention is a property of the CALL — of a sibling
argument's VALUE — not of the parameter. `keeps` on `text:` cannot know which
call passed `SQLITE_STATIC`.

**And it breaks the shipped corpus, measured**: `examples/ledger/db/sqlite.hero:342`
passes a lend, `text: text.cstr()`, with `destructor: SQLITE_TRANSIENT`. That call
is CORRECT and `keeps` would refuse it, along with the four call sites in
`examples/ledger/main.hero` routed through it. **Write the mark and the corpus
stops compiling; omit it and nothing is checked** — verbatim the shape panel 147
used to refuse the type-keyed releaser at design.md:2665, one position over on
the same axis. Route B is that refused row with the mark moved from the type to
the parameter, and the parameter is no better informed than the type was.

Cost beyond the +60: `keeps` is a keyword, so `grammar/keywords` binds it to
`selfhost/keywords.hero`, and a new refusal triggers every golden tree, not the
`selfhost/**` row (`.claude/rules/verification.md`).

### Route C — write the hole down: **approve**, in the absorbing form, at +13

**Panel 166's veto of this shape does not transfer, and the difference is
mechanical.** There, the header's `const` was a real instrument and writing the
hole down would have declined to use it. Here panel 124 measured that no
instrument exists — 0 of 71 — and A and B are shown above to check nothing while
appearing to. §1.12 says the boundary is where the guarantees end and **"a
guarantee that ends quietly is not one."** Today it ends quietly: exit 0, no
diagnostic, wrong answer. Route C makes it end loudly in the one document that is
the prompt. That is the minimum §1.12 demands when there is nothing to check
with, and it is not a substitute for a mechanism — it is the honest record that
none exists.

The absorbing form at **+13** is the cheapest and the best: it makes *a lend name
stands only as an argument of a call* a CONSEQUENCE of the lifetime rule rather
than a second free-standing assertion, which is §1.7's subtraction rather than an
addition wearing a sentence.

**What route C does NOT do, and the sitting must write this down:** it does not
make a program safe. `examples/ledger/db/sqlite.hero:19-23` already carries this
rule as an eleven-line COMMENT — the hazard is today prevented by prose, not by a
compile error, in the project whose thesis is that every plausible mistake is a
compile error. Route C promotes that comment to the spec. It does not close
defect 066; it states it.

### Route D — withdraw the field lend: **object**, on §1.12's completeness clause and on the count

Measured at **−80 real**, not the brief's −44, because the write-back clause goes
with it. Already refused at panel 166 as route F, and the new count makes it
worse: withdrawing the field lend leaves **59 `cstr` occurrences, 10 in shipped
examples**, with the identical hole. It pays 80 tokens to close the smaller half
of a defect and advertises a completeness §1.12 forbids surrendering.

## The route nobody listed, which the brief asked for

**E — refuse the lend at the call site that cannot be checked, by requiring the
retention fact where it actually lives: the argument.** Panel 124's measurement
says the fact is in argument five's VALUE. The language already has a form that
reads a sibling argument by name — `counted_by n`, landed at panel 165, which
names the sibling that gives the extent. A `retained_by d` on a `ptr`/`cstr`
parameter, naming the sibling that decides retention, is the same mechanism on
the same axis, and unlike `keeps` it can distinguish `SQLITE_STATIC` from
`SQLITE_TRANSIENT` because it asks the value, not the world (CLAUDE.md §11).

**I do not rank E and I refuse to price it**, because nobody has drafted the rule
that decides which VALUES of `d` mean retention, and whether that is decidable at
all for `curl_easy_setopt`. That is the same refusal I made at panel 166 and the
brief asks me to repeat it where it applies. E is named so the sitting can send
it somewhere, not so it can be adopted today.

## The ledger row I would write

    167 | defect 066 | lend lifetime | C-absorbing | +13 real (8154 -> 8167,
        digest 44fa44bb11d45b03) | payment: registered prediction below |
        A objected (+22, closes 0 of 2 reproductions) | B VETOED (+60,
        design.md:2344 measured no declaration-site mark can express retention;
        refuses examples/ledger/db/sqlite.hero:342) | D objected (-80, closes
        the smaller half) | defect 066 stays OPEN and is RESTATED as a LEND
        defect binding both directions, 59 cstr occurrences and 33 field

## Registered falsifiable prediction

Payment for C-absorbing's +13, under design.md:316-318 — both instruments exist
today and I ran both in this sitting.

1. **`./heroes measure spec/heroes-spec.md --refresh`** — the ratified wording
   lands at **8167 real, +13**. Above **+32** it has stopped being the joined
   form and owes a fresh named removal.
2. **`./heroes run <case> --sanitize`** — scored at the M-declared-extents close.
   Routes A, B and C **each close zero of the two reproductions**: after any of
   them lands alone, both `keep.hero` (`.ptr()`) and `keeps.hero` (`.cstr()`)
   still compile, still exit 0, and still fire `stack-use-after-scope` and
   `heap-use-after-free` respectively. Land any one of them and produce a
   compile error on either program without route E, and this prediction is false
   and my objection to A was wrong.

## What would change each verdict

- **A** — a compiled demonstration that the field lease REFUSES the lend at a
  retaining position rather than offering an alternative to it. Then it is a
  mechanism and I approve it on §1.12.
- **B** — a rule, derivable from header text alone, separating a retaining
  parameter from a non-retaining one with a measured false-positive rate of zero
  across the 71. Panel 124 measured that no such signal exists; produce one and
  this becomes a cost argument, exactly as design.md:2665 says of its own row.
- **C** — a mechanism arriving from any route. C is the honest record of a hole,
  and the day the hole closes the sentence is a named removal, not a legacy.
- **D** — a measurement that the `cstr` lend is decidable while the field lend is
  not, which would make the two halves genuinely different. My probe says they
  are the same defect.
- **The budget veto** — engages at a measured 10180 real with the FFI floor
  mortgaged. Today's worst route leaves 1966 net free, so nothing on this ballot
  approaches it.
