# Panel 181, completeness critic

Written 2026-09-28. No verdict. Tree: `git archive 0fc98107` in
`/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-181/critic/`, `build/`
removed, compiler built from the seed (`real 3.79`). `git diff --stat dfcac362
0fc98107` touches only three files under `docs/`, so the brief's compiler
(`dfcac362`) and the seats' tree are the same compiler. Every number below comes
from a command run in this directory; where a sentence infers, it says so.
Nothing was built, run or edited in the trunk or in another seat's directory;
their files were read, and the ones I ran were copied here first.

My copies of three prototypes, each the tree plus the compiler-engineer's changed
files only, built from `selfhost/` with an `-O2` seed: `ra/` (route (a) final,
from `a4/`: `layout`, `lexer`, `state`), `rb/` (route (b): `cursor`, `parse`),
`rp2/` (P2: `grammar_expr`, `layout`).

## 1. What I re-ran, and whether it held

**The seventeen programs of `00-shared.md`**, extracted verbatim from its
appendix by script into `probe/s/`, each through `check`, `run` and `fmt`.
**Every row holds**, check exit, first diagnostic, output and `fmt` answer alike:
s01 6, s02 2 with `fmt` printing `y = xs.len(` / `)`, s03 and s04 refused *found
an indented block*, s05 *found `+`*, s06 prints 2, s07 6, s08 5, s09 5 with `fmt`
one line, s10 6, s11 true, s12 3, **s13 prints 6 and `fmt` exits 2**, s14 5, s15
3, s16 *found the end of the block*, s17 `constant_body`. One cell the table left
blank: s17's `fmt` exits 0 (a checker error, not a parse error).

**The census of 21 hits in 10 files, by two methods of my own.**

- *Textual, never running the Heroes lexer* (`census/textcensus.py`): my own
  tokeniser from the spec's words (§ 0, § 1, § 2, § 7), its own bracket depth,
  interpolation holes and margin, and number validation from § 2. It reads **21
  hits in 10 files, the same 21 lines** as the brief: 16 after a malformed token,
  3 after `.`, 2 after `/`. It took three repairs of my own scanner to get there
  (an interpolation hole that closed into a second string, `07.5` wrongly refused
  as a leading zero, a hole left open at a line end), each found by a hit in a
  file the lexer census did not name, and each is commented in the script.
- *Token stream* (`census/tokcensus.py` over `heroes lex --dump-tokens` of all
  1164 files; `heroes lex` without the flag prints no tokens for the 22 files
  with a lexer error, so a census on it would miss 16 of the 21). Validated
  first on my probes: it fires on all 18 same-margin probes and on none of the
  deeper, shallower or leading-operator ones. **21 hits in 10 files**, by kind
  `error` 16, `dot` 3, `slash` 2, the brief's classification file by file.

Beyond the brief's count, from the same two instruments:

| where the line left open goes | token census | textual census |
|---|---|---|
| same margin | 21 in 10 files | 21 in 10 files |
| one deeper (an INDENT) | 1235 in 271: `=>` 824, `else` 404, `error` 3, `:` 2, `+` 1, `,` 1 | 1234 in 270; the one missing is `null` at `check/fixedbugs-variant-case-name-refused.hero:30`, which the lexer makes an `error` token |
| shallower (a DEDENT) | 7 dedents on 5 lines: `|` at `check/success-clause-placement.hero:19`, and 6 `error` dedents, all at end of file | 1, the `|` |
| a postfix `(` `[` `?` `::` heading the line after a block's DEDENT | **0 in 1164**; 5 of 5 on my probes (§ 2.1) | not counted |

The deeper row matches the compiler-engineer's INDENT census exactly. Nobody
counted the shallower row; it holds one real line, a refusal already.

**The coordinator's key** (`k01` to `k10`, `c01` to `c03`, `p2_guido`, copied to
`probe/key/`): **every row holds**, parse, check and output. One note does not
reproduce from the kept files: *"a first run without the prints gave
`unused_binding` on eight of ten"*; with the print lines stripped I get seven of
ten (`k01 k03 k04 k05 k07 k09 k10`), the other three being parse errors. The
first-run files are not kept, so this is unsettled rather than wrong.

**The two scorings.** The ergonomist's prediction: falsified on 1, 4, 5 and 10,
as the key says. The historian's P2: `bad_operand` at 6:9, held.

**The historian's P1, which the seat stood on and could not run: I ran it.** Nim
2.2.12 is installed here. `let y = a +` over `1` at the column of `let`, inside a
proc: compiles, prints 6. `let n = xs.` over `len` at that column: *Error:
invalid indentation* at (4, 3). Both one level deeper: 6 and 2. `if a and` over
`b:` at the same column: compiles, prints 2. **P1 holds**: Nim's parser admits
the same column after a binary operator, and its FAQ's rule is enforced only
after `.`.

**Other precedents the historian derived, now run** (`probe/langs/`): Python
3.14.7, `y = a +` over `1`: *SyntaxError* at the same column and one deeper.
Go 1.27.1, `y := a +` over `1`: prints 6; `y := a` over `+ 1`: *"+1 is not used"*.
Ruby 4.0.7: a trailing `+` continues (6); a LEADING `+ 1` at the same column runs
as a second statement and is dropped, printing 5 at exit 0. Swift 6.4, which the
historian left unverified: a trailing `+` prints 6, and a leading `+ 1` at the
same column ALSO joins, printing 6. JavaScript: `return` over `a + 1` returns
`undefined`.

**Prices.** Vendored rows of W1b, W1b+R, Y, Z and R, and the real rows of W1b
(9029), W1b+R (9019), Y (9076) and R (8989) against today's 8999, all as the
spec-warden's table; the compiler-engineer's route (c) sentence 9067 (+68,
vendored +52) and its clause 9024 (+25, +18), as its § 8. Each variant was
copied into my copy's spec for `measure --refresh` and the spec restored
(sha256 prefix `1b56c89f1b68570b` before and after); only the key's length (108)
was printed.

**The spec-warden's prediction, pre-scored on three prototypes.** My `check`
exits over the 1164 files are identical to its `price/exits.tsv` (635 zero, 529
one). Under `ra/`, `rb/` and `rp2/` **exactly one exit changes in each**,
`surface-fixtures/comments107/margin.hero`, 0 to 1. It holds for all three, which
also means it cannot tell the routes apart: the ratified golden, `shape116` and
the sixteen error-token goldens change their diagnostics or not and keep exit 1.

**Route (a) on my build**: `check` 152/1 (the ratified golden), `annotations`
197/2, `surface` 314/1 (`suite_surface.hero:380`), as the compiler-engineer's
table. Not run by me: `canonical`, `fixes`, `grammar`, `run`, `layout`, units.
Not run by anybody: `emission`, `determinism` and `corpus`, which
`.claude/rules/verification.md` names for a widened refusal. The exit diff above
covers their compile precondition (no other file stops compiling); that it covers
the suites themselves is an inference.

## 2. What is missing

### 2.1 A member of the class that no route closes: a postfix after a block

Defect 005 (`docs/records/done/2026-09-03-0541-...`, fixed 2026-08-28) was a
continuation at bracket depth zero across a DEDENT: `return match c` / arms /
`+ "x"` printed `bx`. Its repair, `ends_the_expression`, guards `binary`'s
climbing and nothing else. The postfix loop has the same hole:

```
f = if c
    double
else
    triple
(5)
print(f)
```

`check` 0, prints **10**; `--dump-ast` reads `bind f = if …(5)`. With `[0]`
(`q2`) it prints 7, with a lone `?` (`q7`) 11. **`fmt` exits 2, *this is a
compiler bug*, on all three, today and under `ra/`, `rb/` and `rp2/`**
(measured), which is exactly how defect 005 was found. No priced route touches
it, because the separator there is a DEDENT, not a missing terminator; both
census instruments are blind to it by construction, and my own count of the
shape is 0 in 1164 files. By defect 118's own sentence (*a verb that says this
is a compiler bug is one*) it is a defect, and it is not 118's arm shape, so
"118 closes with 116" is true of 118's two members and false of the class. A
reader sees `f` bound to a function and a stray `(5)`; the compiler runs one
statement. That is the ergonomist's question *"reads as two statements and runs
as one"*, answered yes, today, by a shape outside the ten fragments.

### 2.2 The reverse shape at depth zero: a `certain` fix that changes the program

The PEP 8 and Black break, before the operator, at depth zero:

```
total = base
- fee
print(total)
```

is refused with `discarded_value`, whose fix is tagged **`certain`**; `check
--apply` writes `_ = - fee`, which compiles and prints **100** where 93 was
meant. `y = a` / `- 1` prints 5; `return base` / `- fee` returns 100
(`probe/lead/` m1 to m3). Unchanged under every route (`ra/`, `rb/`, `rp2/`,
measured). The ergonomist's fragment 7 is this shape; the key recorded
`discarded_value` and nobody applied the fix. It falsifies
`.claude/rules/diagnostics-and-goldens.md`'s *"none violates the rule"* (read at
23 sites, 2026-09-08) and is design.md §4.17's inverse case, a fix that changes
meaning. Panel 180 ruled this break inside brackets only (R2, R3); spec § 0's
own sentence, *"Where a NEWLINE separates without a `,`, the next line may not
begin with a `-` that does not touch its operand"*, reads as reaching depth zero
(the ergonomist read it so), while design.md's R2 bullet (`:1959`) limits it to
a literal's elements. Three artifacts, three answers. `[0]` leading a line after
`y = xs` gets the same `certain` `_ =` and compiles (m4).

### 2.3 Route (a)'s `certain` fix on Guido's shape

`y = a +` over `show(n: a)` (the historian's P2, `show` returning nothing). Under
`ra/`: one `continuation_outside_brackets` with a **`certain`** join; applied, it
writes `y = a + show(n: a)`, which is **`bad_operand`, exit 1** (measured). The
compiler-engineer's map has no shape whose second line stands alone, which is why
its "81 of 81" holds; its own condition (*"one measured shape where its `certain`
fix, applied, fails to compile"*) is met by this one. The historian named the
class (*`certain` only where the next line cannot stand alone*); rule 3 of route
(a) catches a binding or a mutation on the new line and not a `()` call. Today
the same program costs one diagnostic, pointing at the `+`.

### 2.4 The error-token glue reaches the plausible mistakes, not only bad literals

The compiler-engineer's finding holds (`a = 0X10` over `b = 1 +* 2` reports one
of two mistakes; with a statement between, both). It also holds across a BLANK
line (e2), and for the foreign words the lexer makes `error` tokens: `p: ptr =
null` or `x = None` over `y = 1 +* 2` reports `reserved_word` alone (e4, e5).
`reserved_word` is the thesis's showcase for an imported habit, so this is the
likelier trigger. Measured: `ra/` and `rp2/` report both mistakes; `rb/` does
not.

### 2.5 Routes nobody priced

- **Same column or deeper (occam 2.1, Koka, CoffeeScript, and Nim's parser per
  P1 above)**: the historian's *"package with a written record"* for a 007-bis.
  Route (c) was priced as deeper-only with the same margin refused, which no
  precedent the historian found does.
- **Admit, and let `fmt` print it away**: the tree already treats `if (a && b)`
  this way (c01 to c03 compile, `fmt` strips the parentheses, spec § 1 says *"no
  parentheses around conditions"*). The price would be 118 plus the `xs.len(` /
  `)` output. It contradicts §4.15 as ratified, so it is the author's to open; it
  is listed because the option set was drawn without it.

## 3. Framing facts taken on trust

- **`00-shared.md`: "the 61-kind match."** 69: `is_line_ender` names 17 kinds
  `true` and 52 `false` (`awk` over `layout.hero:42-54`), and `variant TokenKind`
  has 69 cases. The 61 is `layout.hero:12`'s own comment, a premise that expired
  in the source.
- **`00-shared.md` § What the sitting decides, 3: "the `error` token at a line's
  end (19 hits)"**, against *"16 of the 21"* above it. My census: 16 at the same
  margin plus 3 before an INDENT is 19; 6 more stand before end-of-file dedents.
  The brief says neither.
- **`00-shared.md`: "the compiler implements the clause panel 007 rejected".**
  It implements panel 007's RATIFIED mechanism: item 1, Go's ender list, and item
  2, *"terminator insertion unchanged everywhere"* (`007:56-71`), with an INDENT
  only for a deeper line. Their consequence at the same margin is the
  continuation. The ratified *"broken inside parentheses or not at all"* has no
  mechanism behind it in either text (design.md:1932-1948 carries both bullets),
  and the ratified golden's comment spells the consequence out. So a refusal is
  the ratified rule, as every seat says, but it also amends a ratified mechanism
  sentence (P and P2 amend *"unchanged everywhere"* itself). The spec-warden's
  *"owes Principle 0 nothing"* rests on the brief's framing.
- **`00-shared.md`: the cascade "not measured here".** The measured answer is the
  opposite of a cascade: a hidden diagnostic (§ 2.4).
- **Defect 116's origin: "searched ... for *depth zero* ... and found only panel
  007's deferral and panel 180's reports".** `grep -rli "depth zero"` over the
  same three places returns 7 files, among them defect 005's record and the log
  entry of 2026-08-28. Defect 005 is the class's precedent and is in no brief;
  the spec-warden found it.
- **The coordinator's late fact (a), `check/depth-zero-continuation.hero`:
  holds.** Ratified 2026-08-04 (`f7c9d68b`, *"The five adversarial cases lose
  their UNVERIFIED marker"*). Its comment reads *"A line ending in `+` gets no
  terminator, so the next line is read as part of the same expression"*, which is
  the same-margin reading and true today; its body tests only the deeper line
  (exit 1, `expected_expression` at 7:1); its first claim, *"there is NO
  continuation at bracket depth zero"*, is false at the same margin (s01). It
  guarded one margin of three, and the net has certified the same margin since
  (`suite_surface.hero:380` expects `fmt` at exit 0 on `margin.hero`).
- **The coordinator's late fact (b), `M-thesis-harness.md:141`: false as
  measured, on four counts.** (1) *"the form is refused and now enforced"*: s01,
  k01, k04, k05, k10, b2, b3 and k1 compile. (2) `design.md:1800-1804` is not the
  rule (§4.15's bullet is `:1939-1948`); the item's own re-verification of
  2026-09-10 moved the pointer and did not re-run the claim. (3)
  `ends_the_expression` refuses one shape, an operator leading the line after a
  control form's block, and its postfix neighbour compiles (§ 2.1). (4) *"panel
  095, ratified"*: panel 095 is the blank-line sitting; `grep -n -i continu` finds
  only a note that its historian could not open panel 007. The repair was defect
  005's `/decide` answer `5a` of 2026-08-28, in the session that ratified 095 as
  `1a` (`docs/records/log/2026-08-28-0003-...`). It owes a dated correction.
- **`00-shared.md`'s other citations hold**: design.md:1939-1948, panel 007 at 91
  lines and its items 1 to 4, spec lines 5 to 17, `layout.hero` 205 lines with
  `is_line_ender:40`, `line_start:61`, `:82`, `maybe_terminator:131`,
  `scan.hero:57-62`, `lexer.hero:135, :145`, 1164 files, 272, 32, 66, zero hits
  outside `tests/golden/`, 8999, 6672, 6794, headroom 1241, floor 60.

## 4. Contradictions, and which side the world takes

- **`is_thesis_rule`: compiler-engineer "yes" (measured on `a5/`) against
  spec-warden "no, under (a) as briefed" (inferred).** Both are right about
  different (a)s: the engineer measured that (a) as worded and P2 leave `check
  --permissive` at exit 1, which is the warden's reasoning. What neither weighs:
  panel 180 left its neighbour `line_end_before_continuation` off the list
  (`diag.hero:90`), and with this code on it the control arm would compile the
  continuation §4.15 says the language never had. That is the author's.
- **The spec owes nothing (engineer) against W1b (warden).** Measured: the
  ergonomist's blind X column (`R R A R R R R R A R`) matches `ra/`, `rb/` and
  `rp2/` 10 of 10, and today's compiler 6 of 10. A careful reader of today's spec
  already reads the refusal; W1b's case rests on a less careful reader, unmeasured.
- **Spec-warden's R against CLAUDE.md § 12.** § 12 says the compiler has the bug;
  R deletes the spec clause the compiler ignores (c01 to c03, `fmt` strips them).
  design.md:1976 reads *"`if x > 3`, not `if (x > 3)`"*. R reverses § 12's default
  direction, and the synthesis should say so rather than inherit it.
- **The ergonomist's "no variant compiles a program that means something other
  than it reads"** held on its fragments; today § 2.1 is one that reads as two
  statements and runs as one, and § 2.2 is one that `--apply` writes.
- **The historian's "not found: a silent misreading from a same-column
  continuation in an indentation-sensitive language"**: Heroes' own defect 005
  printed `bx` at exit 0 (across a DEDENT, not at the same margin), on record
  since 2026-08-28; Ruby's leading `+ 1` above is a second, in a language without
  significant indentation.
- **Engineer: defect 118 closes with 116 under (a).** For its two members, yes
  (s13 exit 1 under `ra/`). For the formatter's exit 2 on a depth-zero read-on,
  no (§ 2.1, all four compilers).
- **The ergonomist's standing prediction** names *"the next harness run"*, metric
  2, which has 0 tasks (`harness/tasks/README.md:13`): the unscoreable shape of
  panel 007's pair, which the spec-warden declined to renew.
- **Spec § 0's keep-list against the compiler, inside brackets.** The spec says a
  line there keeps its NEWLINE after *"a name or keyword other than `function` and
  `fail`"*; `is_line_ender` answers `false` for 13 more (`constant record variant
  match if else for while in test assert extern use`). Measured: `print(xs.len(),
  if` over `true)` plants no terminator after `if` (refused as `missing_body`).
  Harmless to acceptance, but any depth-zero sentence keyed on *"a token that
  cannot end a line"* (Y, Z) has two sets in the tree to mean.

## 5. The question the sitting did not ask

**Is defect 116 the class, or one entry of it?** The class is *the parser reads on
across a line end at bracket depth zero*, and enumerated from the world it has at
least five entries today: after a non-ender (116), after `=>` (`arm()`'s
`skip_terminators`, the engineer's), after an `error` token (the glue, § 2.4),
after a control form's block through a postfix (§ 2.1, open under every route),
and the reverse direction, the leading `-` whose `certain` fix rewrites the
program (§ 2.2). Every seat priced the first entry, and the briefs listed only its
neighbours in the lexer. What was not asked: what single rule makes every
depth-zero line end end its statement unless a production writes a block there,
and which one site enforces it, so that the next member is refused by
construction rather than found by `fmt` exiting 2, as 005, 107, 118 and § 2.1
each were.

## Files

`census/textcensus.py`, `census/tokcensus.py`, `census/*.out`;
`exits/{base,ra,rb,rp2}.tsv`; `probe/s/` (the seventeen), `probe/key/`,
`probe/sw/`, `probe/post/` (§ 2.1), `probe/lead/` (§ 2.2), `probe/err/` (§ 2.4),
`probe/ra/`, `probe/ra-apply/`, `probe/langs/`; `price/`; `ra-suites.log`.
