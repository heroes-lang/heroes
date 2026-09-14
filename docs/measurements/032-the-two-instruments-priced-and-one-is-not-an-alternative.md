# 032 — The two instruments priced, and one of them is not an alternative

Date: 2026-09-14 · M-marked-acquisition step 2 · **the pricing panel 147 could
not take, because the form it was convened on did not parse.**

## Why this file exists

Panel 147 R4 admitted a form and refused to choose its instrument, for a reason
it stated: it had no price. `released` produced
`error[expected_extern_signature]`, so **every Route A finding at that sitting
was hand-inserted C or a reading of four draft lines**. The milestone's first
item says the choice must be **compiled**.

Two instruments were on the ladder:

- **A**, a `consumes`-shaped mark on the **acquiring** call;
- **B**, **escape refusal**, whose machinery ships twice already as
  `cstr_escapes` and `lease_escapes`.

Both were priced. They did not turn out to be alternatives.

## 1. Instrument B is refused by the same measurement that refused Route A

No build was needed, and that is itself the finding. **Escape refusal asks: may
a handle leave the scope that acquired it?** Counted from the tree:

```sh
grep -rn --include='*.hero' -E "^function .*-> *(Db|Statement|Held|Slot|Conn|Curl|Chunk|Thing)\??( *)$" examples/ tests/
```

**Exactly two Heroes functions in the whole tree let a handle escape**, and both
are in the reference SQLite binding:

| function | what it returns | call sites |
|---|---|---|
| `opened() -> Db?` | a record holding a `CDb` acquired inside it | **7** |
| `prepared(db, sql) -> Statement?` | a record holding a `CStmt` acquired inside it | **6** |

A blunt escape refusal refuses **2 of 2** — **100%** of the handle-producing
wrappers in the corpus, and with them the 13 call sites that depend on them.
That is the **same** failure that killed Route A at panel 147, arrived at from
the other direction: the warden measured that 22 of 23 leaking paths acquire
inside those two wrappers, and this measures that those two wrappers are exactly
what an escape rule forbids.

**So B is not an alternative to A. B needs A.** An escape can only be permitted
where the obligation **propagates to the caller**, and saying that a function
hands its caller an obligation is precisely what A's mark is for. The ladder the
sitting drew has one rung, not two, and the other is a clause of it.

## 2. Instrument A's surface, built and measured

Built in a copy of the tree, mirroring `consumes` exactly — a contextual word,
read in one position, on both the parameter and the result, as `owned` already
rides both.

**+25 code lines across 7 files**, in `suite_layout.hero`'s own unit:

| file | before | after | |
|---|---|---|---|
| `selfhost/ast.hero` | 503 | 508 | **+5** |
| `selfhost/parse/members.hero` | 230 | 238 | **+8** |
| `selfhost/print/fmt.hero` | 1153 | 1160 | **+7** |
| `selfhost/parse/tails.hero` | 282 | 285 | **+3** |
| `selfhost/resolve/qualified.hero` | 173 | 175 | **+2** |
| `selfhost/resolve/state.hero` | 325 | 325 | 0 |
| `selfhost/resolve/types.hero` | 189 | 189 | 0 |

It compiles, it parses, and a program using it runs.

**The compiler enumerated the work itself**, one build at a time, exactly as
panel 132 relied on: `error[missing_fields]: FunctionDecl is built with every
field, named: … acquires_result: — all of them, always`, at **6 constructor
sites across 5 files**. No site can be forgotten, because the build stops at
each one.

**Two files land over their ceilings**, and they are the two the sitting
predicted: `ast.hero` 505 → **508**, `print/fmt.hero` 1156 → **1160**.

## 3. CL-036 reproduced live, inside the prototype, within minutes

The mark was added to the AST and the parser and **not** to the formatter. Then:

```
    input:   function sqlite3_open(filename: cstr, @out: Db acquires) -> i64
    heroes fmt: function sqlite3_open(filename: cstr, @out: Db) -> i64
```

**The word was gone. Exit 0, no diagnostic.** That is
`.claude/rules/diagnostics-and-goldens.md`'s sentence — *a tool that has not
learned a new form does not error, it drops it* — happening rather than being
cited, and it cost six lines to close. It is recorded because the rule is
usually quoted from a past defect; here it is a fresh one, produced on demand.

## 4. A prediction scored

Panel 147's compiler-engineer predicted: *"Both routes must touch `ast.hero`
(2 lines) and `fmt.hero` (3) — the same two design.md:3518 named at ceiling
when `consumes` was priced."*

**Measured: `ast.hero` +5 and `fmt.hero` +7.** The prediction is **right in kind
and low by about half in degree** — both files, both at their ceiling, both
needing a move. Scored as HELD on the claim that mattered (which files) and
UNDER on the number.

**And one prediction is NOT scored, for a better reason than failure.** Panel
147's completeness critic predicted *escape refusal prices under 120 code
lines*, on the evidence of `ffi_sweep` at 93 and `consuming` at 114. Its line
count was never taken, because § 1 dissolved the question it would have
answered: escape refusal is not a competing instrument whose price could win,
it is a clause of the other one. The prediction is marked **moot — the
alternative it priced stopped being one**, which is not the same as lapsed and
is recorded so nobody scores it later as a miss.

## 5. What is NOT priced yet, said plainly

**The surface is not the instrument.** What makes the mark do anything is the
counter: an increment emitted at the acquiring call, a decrement at the
`consumes`-marked release, and a fourth exit check in
`runtime/parts/alloc.c` beside the three that are there. That half is
**unbuilt**, so its number is **unrun**, and no sentence here should be read as
pricing it.

What the built half already settles is the shape of the rest: the checker needs
**no flow analysis**, because the two ends are both written by the binding
author — `acquires` on the producer, `consumes` on the releaser, which
`examples/curl/main.hero` already carries. `check/leasing.hero:29` and
`check/consuming.hero:22` both state the checker has no flow analysis, and this
design does not ask for any.

## What this measurement does not do

It does not choose between a compile error and a loud exit — that is the
milestone's second item, and the counter above is the loud-exit half of it. It
does not decide the spelling; `acquires` is the placeholder the prototype used
so the surface could be built, and panel 147 left the word open.
