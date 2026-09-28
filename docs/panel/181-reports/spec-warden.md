# Panel 181, spec-warden report: defect 116, a depth-zero line that ends where no token can end it

Written 2026-09-28. Tree: `git archive 0fc98107` in
`/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-181/spec-warden/`, `build/`
removed, compiler built from the seed (`real 3.89`). Ceiling read by grep before
anything else: design.md:255, **10240 tokens on `claude-opus-5` through
`POST /v1/messages/count_tokens`**; payment rule design.md:311-314, unconditional.
Every number below comes from a command run in this sitting in that directory;
where I infer, I say so. The key was sourced from the trunk's `.env` only for
`measure --refresh`; only its length (108) was printed.

## Verdict

- `verdict`: **veto** on route (c), Variants Z and Z+ (Principle 0 unmet).
  **Object** to Variant Y as worded. **Approve** route (a) or (b), with sentence
  W1b paid by the named removal R. Counts are measured, so none of this is
  provisional.
- `section`: design.md §1.6 (budget and payment rule, :255-314), §1.2 (:190-204),
  §1.0 Principle 0 (:112-128), §4.15 (:1939-1948, panel 007 ratified 2026-08-03).
- `spec_token_delta` (measured; before 8999 real, 6672 legacy, 6794 cl100k):
  Y **+77 real** (9076; +56/+55 vendored, over `DELTA_GATE`'s 50); Z **+53** (9052);
  Z+ **+78** (9077; +61/+62, over the gate); W1b **+30** (9029; +18/+19);
  **W1b with R: +20 real (9019; +13/+14 vendored)**, headroom 1221, 1161 after
  the FFI floor's 60.
- `removal`: R, § 1's *"no parentheses around conditions"* (spec line 32):
  **-10 real, -5 vendored**. The compiler does not enforce it: `if (a && b)`,
  `if (a &&` / `b)` at either margin and `while (i <` / `3)` all `check` at exit 0
  and `fmt` strips the parentheses. Under the refusal, a long condition is
  broken inside parentheses, which that clause, read as a prohibition, forbids.
- `needed_for_self_hosting`: **no**, for the continuation (route c) and for the
  sentence. The repair adds no form, so it owes nothing here.
- `argument` (≤120 words): The repair adds no form. §4.15 and panel 007, both
  ratified, already exclude depth-zero continuation, so bringing the compiler to
  them owes Principle 0 nothing, like defect 005's repair. Route (c) is a new
  form. The compiler does not need it, and panel 007's ratified condition (a
  baseline showing models produce the shape) cannot be met: metric 2 has 0 tasks,
  and the tree cannot stand in, because `fmt` joins the shape away. Veto. Y grants
  a false permission: *unless a block opens below it* licenses fragment 2, which
  the compiler itself calls "an indented block". The spec is true without a
  sentence but not unambiguous: the ratified golden's own comment describes the
  Go reading. W1b states the rule exactly.
- `prediction`: with route (a) or (b) landed, run `heroes check` over the 1164
  `.hero` files of `0fc98107` as they stand there. **Exactly one exit changes:
  `tests/golden/surface-fixtures/comments107/margin.hero`, 0 to 1.** Details
  below. Scored at M-agreed-retention's close.
- `condition`: on (c), an author-written metric-2 run showing the shape, plus the
  author re-ratifying panel 007's condition. On Y, strike *unless a block opens
  below it* and the partial list. On W1b, a blind reading showing Variant X
  already answers fragments 1, 2 and 10 at the confidence W1b gives (then R alone).
  On R, a blind reading showing the clause is load-bearing.

## The prices

Each candidate was inserted after spec line 17, wrapped at 80 columns like the
spec, measured with `./heroes measure`, then copied into my copy's
`spec/heroes-spec.md` for `./heroes measure --refresh`. The original was
restored afterwards (sha256 prefix `1b56c89f1b68570b` before and after). The
baseline refresh read 8999, dated 2026-09-28, digest `072437a576dbb74c`, which
is the digest panel 180's ledger row pinned.

| id | text | legacy | cl100k | real | delta real |
|---|---|---|---|---|---|
| X | nothing added | 6672 | 6794 | 8999 | 0 |
| Y | ergonomist's Variant Y | 6728 | 6849 | 9076 | +77 |
| Z | ergonomist's Variant Z | 6711 | 6834 | 9052 | +53 |
| Z+ | Z + *; at its own margin, or after any other token that cannot end a line, it is an error.* | 6733 | 6856 | 9077 | +78 |
| W1 | *Outside them no line goes on below, so a long expression breaks inside parentheses.* | 6690 | 6813 | 9027 | +28 |
| **W1b** | ***Outside brackets no line goes on below, so a long expression breaks inside parentheses.*** | 6690 | 6813 | 9029 | +30 |
| W2 | *Outside them no line goes on below: one that ends with an operator, `.`, `,`, `:`, `=`, `@` or `->` is an error, so a long expression breaks inside parentheses.* | 6720 | 6841 | 9064 | +65 |
| T | edit in place: line 15 becomes *...breaks after an operator inside brackets, and outside them nowhere.* | 6680 | 6802 | 9014 | +15 |
| R | removal: *No braces, no semicolons, no parentheses around conditions.* becomes *No braces, no semicolons.* | 6667 | 6789 | 8989 | -10 |
| W1+R | | 6685 | 6808 | 9017 | +18 |
| **W1b+R** | | 6685 | 6808 | 9019 | **+20** |
| T+R | | 6675 | 6797 | 9004 | +5 |
| Y+R | | 6723 | 6844 | 9066 | +67 |

No candidate threatens the ceiling. `DELTA_GATE` is 50 vendored
(`tests/harness/suite_spec.hero:181`): Y and Z+ cross it and would each need a
payment named in the commit body; Y+R sits at the edge (51 legacy, 50 cl100k).
The real/vendored ratio of W1b is 30/19, or 1.58, inside the 1.2 to 1.7 band that
panel 180's warden predicted and saw held.

**Why W1b and not the cheaper T.** Price is not the reason, following the author's
instruction of 2026-09-12 (robust over cheap). T rewrites the sentence panel 180
landed the day before, and it can be misparsed as "an operator [that is] inside
brackets". W1 says *them*, whose antecedent is two sentences back. W1b uses
*goes on below*, the spec's own phrase from the bracket sentence it contrasts
with, so a reader holds one concept. It enumerates nothing, so it cannot leave a
token out. The enumerations in Y and W2 omit `::`, `|` and every keyword; `for x
in` / `xs` at the same margin compiles today and prints 1 and 2 (probe `k1`).

## 1. Whether the spec owes a sentence

**Under route (a) or (b) the spec is TRUE without an addition, on its literal
reading.** Line 9 says NEWLINE comes from the indentation. The last-token rule is
stated for *"there"* (inside brackets) only. `Sum = Product { ( "+" | "-" )
Product }` cannot take a NEWLINE after `+`. So `y = a +` NEWLINE `1` matches no
production, and the compiler, not the spec, is wrong (CLAUDE.md §12).

**But the spec is not unambiguous, and the record proves it.** *"A line there
keeps its NEWLINE when it ends with..."* is Go's rule, and in Go that rule holds
at every depth. A reader who takes *there* as "also inside brackets" rather than
"only inside brackets" reads depth-zero continuation, which is exactly what the
compiler's authors built from §4.15's Go sentence. The author-ratified golden
`tests/golden/check/depth-zero-continuation.hero` says in its own comment: *"A
line ending in `+` gets no terminator, so the next line is read as part of the
same expression"*. So the refusal can be inferred, but nothing forces the
inference: a rule a reader can miss, which is the case for stating it. The
llm-ergonomist's blind reading decides which reading a reader takes; this seat
prices it.

**Variant Y states a false permission.** *"is an error unless a block opens below
it"*: in fragment 2 (`y = a +` / a deeper `1`), a block does open below, to the
lexer and in the compiler's own words: s03 is refused with *"expected an
expression, found an indented block"* (reproduced). A reader of Y concludes
fragment 2 is legal, and it is refused under every route. The only depth-zero
line that legally ends in a non-ender before a block is a `match` arm's `=>`, and
a reader already knows that from the `Arm` production.

**Under route (c) the sentence is owed by construction, and Z is not the
package.** Panel 007 item 3 asked for *"the explicit continuator set and the spec
sentence as one package"*. Z names the set but does not say what happens at the
same margin, or after `=`, `:`, `,`, `@`, `->` or `::`. It also leaves open
whether a third line is one level deeper than the first line or than the second.
Z+ closes the first two of those at +78 real.

## 2. What it displaces

**R, and it is a repair as well as a payment.** The precedent is ledger row 3512
(`docs/measurements/010`), where `spec:177`'s *"None of these names may be
redeclared"* was spent because the sentence was stricter than the language. Here
too the clause is stricter than the language: parenthesised conditions compile
(probes `p1` to `p4`, exit 0, `fmt` strips them), and under the refusal they are
the only way to break a long condition. design.md §4.15's *"No parens around
conditions... Zero information, two tokens saved"* describes `fmt`'s canonical
output, which R does not change. The residual cost is inferred rather than
measured: a C-trained model may write the two parentheses and `fmt` deletes them;
that costs program tokens, but no rewrite. The llm-ergonomist's task 2(ii) (a
long `&&` condition) is the reading that could show R is load-bearing, and it is
my condition on R.

Under route (c) I found nothing in the spec that Z or Z+ makes redundant. R pays
10 of its 78 tokens. A prediction naming metric 2 pays nothing (panel 046 R1).

## 3. Under §1.2

**The strongest case against the repair is a rewrite-rate argument, and I state
it.** Today s01 compiles and means what its writer meant (prints 6; the first
line alone can never be a valid statement, so the joined reading is the only
one). The repair turns that program into a compile error, which costs one round
trip (500 to 2000 tokens) every time a model writes the shape. How often models
write it is **unmeasured**: metric 2 has never run (`harness/tasks/README.md`:
*"Status: 0 tasks"*), and design.md:376 says the formula stays unaudited until
it does.

**Why it does not carry.** First, the shape the compiler admits is the one that
looks like two statements, while the one a Python or Nim reader reaches for, the
deeper line (s03, s04), is refused already, with a message about a block. Today
the rewrite rate is paid on the natural shape and waived on the odd one. Second,
the status quo is not a stable language: `fmt` rewrites s02 into `y = xs.len(` /
`)` and accuses itself on s13 (defect 118). Third, W1b moves the cost from a
round trip to the prompt: at +30 real tokens, it pays if it prevents one round
trip in every 17 to 67 programs (500/30 to 2000/30). That is arithmetic on an
unmeasured frequency, not a measurement.

## 4. Principle 0, and the instrument panel 007 meant

**The repair owes neither a Part 11 effect nor a compiler need.** §1.0 binds what
*enters* (design.md:2679). The depth-zero refusal was ratified on 2026-08-03 and
never left. The precedent is exact: defect 005 (`docs/records/done/2026-09-03-0541-...`)
was the dedent-side shape of the same rule, repaired in `grammar_expr.hero`'s
`ends_the_expression` as *"a compiler made to obey a ruling it already had"*, with
no spec token. What this sitting adds that 005 did not is a possible **new
diagnostic class**, which is CLAUDE.md §4's trigger, but a diagnostic is not a form.

**Route (c) owes Principle 0 in full and cannot pay it.** The compiler does not
need it. The coordinator's instrument found zero depth-zero continuations
outside `tests/golden/` (I did not rerun that instrument), and every
parenthesised break I tried compiles. There is no measured Part 11 effect.
Panel 007's condition names *"the measurement baseline"*, which is metric 2
(panel 007's own § Predictions: *"scored against the baseline"*;
`docs/work/milestones/M-thesis-harness.md:127-162`, *"nothing else can score
them"*). **It has 0 tasks and has never run.** `grep` of `docs/measurements/`
for *depth 0*, *007-bis*, *illegal break* and *baseline* finds no run of it.

**The tree cannot stand in for the baseline.** `fmt` joins s01 and a break across
a blank line into one line (exit 0), and wraps a break with a comment in
parentheses. The `canonical` suite holds the tree to `fmt`'s output, so a
same-margin break a model wrote into the tree has been erased. Zero hits is
therefore no evidence either way. (This is an inference from `fmt`'s measured
output and the suite's stated job; I did not audit the git history.) An argument
from Part 1 is §1.0's other branch, but using it to admit (c) would override a
condition the author ratified, which is the author's decision to make and not
this panel's.

**Two record facts the synthesis should carry.** (i) The M-thesis-harness item
says *"the form is refused and now enforced"*. s01, b2 (a blank line between),
b3 (a pattern broken after `|` at the arm margin, prints 1) and k1 (`for x in` /
`xs`) all compile at exit 0 today, so that sentence is false as measured and owes
a dated correction underneath. (ii) If W1b lands, that item's open question,
*"whether the spec owes the ~28-word layout sentence"*, is answered on the refusal
side, and only Nim's rule stays deferred. My predecessor's panel 007 prediction
(zero illegal breaks in the baseline) named an instrument that does not exist. I
do not renew it (panel 046 R1).

## 5. The diagnostic class

**No list of all codes exists.** The one hand-maintained list is
`is_thesis_rule`, `selfhost/diag.hero:92-116`, and `annotations/thesis`
(`tests/harness/suite_annotations.hero:169-200`) requires each code on it to have
an annotated witness. A code not on that list joins no list:
`tab_in_indentation` is pushed at `selfhost/layout.hero:77` and has no golden
anywhere. (Searched: the codes of panel 180, `indentation_jump`,
`tab_in_indentation` and `expected_expression` across the tree, and
`all_codes|known_codes|CODES|every code` in `selfhost/` and `tests/harness/`.)

**What a new code owes, from panel 180's precedent** (`line_end_before_continuation`
is in 36 files): the push site; a `tests/golden/check/` case with its `#~`
annotation (CLAUDE.md §9); a hand-written `.expected` (no `UPDATE_GOLDEN` in
`check/`); a `.fixed` if the fix is `certain` (the `fixes` suite); `surface` rows
whose exit is derived from the spec sentence, as panel 180 R5 did with
`brackets180/`; the `comments107/margin.hero` header, which describes the shape
as read on; a design.md §4.15 bullet naming the code (:1961, :1968 precedent);
and, because it widens a refusal, the `check`, `run`, `emission`, `determinism`
and `corpus` suites as well (`.claude/rules/verification.md`). For the `Fix`, one
measured fact: on the commented shape `fmt` already prints the parenthesised form.

**`is_thesis_rule` depends on the route.** This is an inference, not run. Panel
180's criterion (`diag.hero:85-91`) makes a code a thesis rule when, without it,
the shape still has a meaning. Under route (a) as briefed, the lexer reports AND
plants the terminator. Dropping the code under `--permissive` would leave the
terminator in place, the parser would refuse anyway, so the code is not a thesis
rule. Under route (b), if the parser reports and reads on, dropping the code
leaves Go's meaning, so it is a thesis rule and needs a witness. No
`heroes mutate` operator breaks a line (`grep -niE "line|break|newline|join"
harness/mutations/operators.md` returns nothing), so metric 3 cannot see the
choice today. The compiler-engineer should settle it on a prototype.

## Prediction, in full

With the compiler that lands route (a) or (b), built from its seed, run `heroes
check` over the 1164 `.hero` files that `0fc98107` has outside `archive/`,
`build/` and `site/node_modules/`, each file's content as at `0fc98107`.
**Exactly one exit differs from today's:
`tests/golden/surface-fixtures/comments107/margin.hero`, 0 to 1.** Today's exits
are 635 zero, 529 one, none two, recorded in `price/exits.tsv` in my directory
(sha256 prefix `edcb78bf10c1fd0d`). Of the ten files carrying the coordinator's
21 hits, nine already exit 1. The prediction is falsified by a second changed
exit, which would mean the shape lives where the coordinator's instrument did
not look (it checked the same margin only), or by any file reaching exit 2. It
names an instrument that exists today and the milestone at which it is scored,
so it is admissible under panel 046 R1. It measures Principle 0's answer (the
corpus needs no continuation) and §1.2's cost of the repair on existing programs
(zero rewrites).

## Not run by this seat

- The coordinator's instrumented lexer (the 21 hits). I reproduced the file count
  (1164) and the exits, not the hits.
- Any prototype of routes (a), (b) or (c); the `is_thesis_rule` consequence
  above is reasoned from the brief's description of route (a).
- Defect 118 under each route. That is the compiler-engineer's question, and I
  have no measurement of it.
- Whether a model produces the shape. No instrument for that exists today.

## Files

- Probes: `probe/` (`p1` to `p4` parenthesised conditions; `s01`, `s03`; `b1`
  comment, `b2` blank line, `b3` pattern `|`, `k1` `for x in`).
- Priced variants: `price/*.md`; candidate texts `price/cands.txt`; exit baseline
  `price/exits.tsv`, `price/files.txt`.
