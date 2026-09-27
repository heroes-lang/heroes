# Panel 180, spec-warden report

Written 2026-09-27 from `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/spec-warden/`:
the trunk archived at `29ed5601`, the compiler built there from the seed. Every number below
was run in this directory unless it says *unrun* or names someone else's measurement.
design.md §1.6 was read by grep at the start of the sitting (`grep -n 10240
docs/design/design.md`, line 255): the ceiling is **10240**, measured by `claude-opus-5`
through `count_tokens`, and the payment rule is unconditional.

- `verdict`: **object, provisional.** I object to Q and to R as worded, and to route (c).
  Provisional because every variant was priced on the vendored instrument, which is a
  lower bound. The `real` row is unrun for all of them: `measure` prints STALE and I did
  not run `--refresh`. I approve one of two wordings, depending on the route: **RbComma**
  under route (b), or **GQ** under route (a). Both are defined below.
- `section`: design.md §1.6 (the payment rule, unconditional), §1.2, §1.4 (a different
  but valid program), §4.15 (terminators everywhere, brackets included). Also CLAUDE.md
  § 12 (spec beats compiler) and § Precedence (take the robust resolution).
- `spec_token_delta`: today's text is **6693** vendored maximum (cl100k), 6568 legacy,
  and the cached `real` is **8861**. Measured deltas on the vendored maximum: P 0, **Q +65**,
  **R +47**, Q made true +95, **RbComma +39** (+23 with the § 10 removal), **GQ +66** (+50
  with it). The worst case, +95, sits against 1319 real tokens free after the FFI floor.
  Estimated, unrun: the worst case on `real` is about +152.
- `removal`: § 10's clause *"A container literal separates elements by newline across
  lines and by comma on one"*, measured at **-16** on its own. Under either true rule it
  follows from `Sep`, and it describes the form `heroes fmt` prints: w01 parses commas
  across lines, and `fmt` rewrites them as one element per line. Nothing reads the
  clause (grep of `tests`, `selfhost`, `site/src`, `.claude`). The removal holds only if
  the ergonomist's fragment-10 score does not drop without it. Nothing else has to come
  out for the budget.
- `needed_for_self_hosting`: **no.** `selfhost/` holds no line starting with `,` (grep),
  and it parses today, so it uses no refused shape.
- `argument`: Neither rule-stating candidate is true as written. Q predicts 7 of my 35
  probes wrong: a signature's and an extern's leading `,` parse, and five type-closer
  probes it calls legal are refused. Q and R omit `true`, `false` and `nullptr`, which end
  a line. R as worded refuses a NEWLINE before every `,`, a shape `heroes fmt` itself
  prints (six lines for `nextvalue.hero`), so route (b) under R breaks the formatter.
  Route (c) creates a silent pair: `[a` / `- b]` is two elements today, `(a` / `- b)` would
  subtract (§1.4). Adopt RbComma (+39) under route (b), or GQ (+66) if the compiler stays.
- `prediction`: this is the payment. The coordinator's `heroes measure
  spec/heroes-spec.md --refresh` on the adopted wording will read a `real` delta between
  **1.2 and 1.7 times** the vendored delta measured here: RbComma +47 to +66, RbComma-s10
  +28 to +39, GQ +79 to +112, GQ-s10 +60 to +85, Q made true +114 to +162. A reading
  outside that band falsifies it. The instrument exists today, and it is scored at the
  synthesis, no later than M-agreed-retention's close.
- `condition`: I approve **RbComma(-s10)** if the compiler-engineer's route (b) prototype
  passes the compiler's own tests plus `surface`, `canonical`, `check`, `annotations` and
  `grammar`, and `heroes parse` on my 35 probes agrees with the sentence on every file. I
  approve **GQ(-s10)** if route (b) is priced unsound. I would **veto** if route (c) were
  adopted without a measured Part 11 effect, or if a `real` reading put the spec plus the
  floor past 10240. That second case would need a delta over 1319, about 14 times the
  largest vendored delta.

## 1. The price of each wording

For each variant I put the text at `spec/heroes-spec.md` in place of lines 11-12, ran
`./heroes measure spec/heroes-spec.md`, then restored the file. The digest
`f5ede81a6ebf8de6` was checked after every run. My wrapper (textwrap, width 80) reproduces
today's lines 11-12 byte for byte, so the wrapping adds nothing to any delta. cl100k is the
maximum in every row. Each variant exits 1 with STALE, which is the correct refusal to judge
without a `real` reading.

| variant | what it is | legacy | cl100k (max) | delta | true of today's compiler on the 35 probes? |
|---|---|---|---|---|---|
| P | today | 6568 | 6693 | 0 | no (defect 104) |
| **Q** | as given | 6636 | 6758 | **+65** | **no: 7 wrong** (t04 t18 t06 u02 t09 u05 u07); 9 if "a literal" excludes `true`/`nullptr` (t01 t02) |
| **R** | as given | 6617 | 6740 | **+47** | describes route (b); it also refuses 6 shapes accepted today |
| Qtrue | Q corrected to the measured compiler (names `true` `false` `nullptr`; `,`/`)` of `Args`, `Params`, `Member`; `)` group, `]` array, `}` map) | 6667 | 6788 | +95 | yes, checked by hand |
| Rtrue | R plus `true`, `false`, `nullptr` | 6627 | 6749 | +56 | route (b) |
| Rcomma | Rtrue plus "or a `,`" | 6632 | 6753 | +60 | route (b), widening only |
| Rshort | Rtrue without the first clause | 6604 | 6728 | +35 | route (b) |
| **RbComma** | Rshort plus "or a `,`" (text below) | 6609 | 6732 | **+39** | route (b), widening only |
| RbComma2 | the same with "and none otherwise" | 6611 | 6734 | +41 | route (b), widening only |
| **GQ** | short sentence, with Q's exceptions written into 6 productions | 6635 | 6759 | **+66** | **yes**, checked by hand |
| GR | short sentence, with `[ NEWLINE ]` at 13 closers | 6652 | 6777 | +84 | route (b) |
| s10only | the § 10 clause removed, nothing else | 6552 | 6677 | -16 | |
| RbComma-s10 | | 6593 | 6716 | +23 | |
| GQ-s10 | | 6619 | 6743 | +50 | |

The components: GQ's productions alone cost +38, GR's alone +56, and the short sentence
alone +28. The short sentence alone is **false**: without production edits it refuses
fragment 10, §4.9's canonical shape.

RbComma, the wording I back under route (b):

> Inside `(` `[` `{` a NEWLINE never ends a statement, and a line there carries one only
> when it ends with a name, a literal, `true`, `false`, `nullptr`, `?`, `???` or a closing
> bracket; it stands only before a closing bracket or a `,`, or where a production writes it.

**Estimated `real` cost, unrun.** Today's document reads 1.32 real per vendored token
(8861/6693). The grammar block measured 1.60 (the commit body of `e497646a`, not re-run).
On that band: RbComma about +52 to +62, GQ +87 to +106, Qtrue +125 to +152. These are
estimates. spec-shape.md says so in its own words: *a vendored delta is not a price.*

**§1.2's arithmetic.** Break-even is the probability that a session makes one break
inside brackets that today costs a round trip (500 to 2000 tokens). Taking the upper estimates above
(+62, +106, +152), it comes to 3.1-12% for RbComma, 5.3-21% for GQ and 7.6-30% for Qtrue. How often
a model breaks a line before an operator inside brackets is **unmeasured**. Python's PEP 8
style prior, which recommends exactly that break, is a question and not a premise. The
price does not decide whether to repair: a false sentence in the one document a reader
trusts is a defect at any price.

## 2. Whether the grammar owes a change too

**Route (b): the sentence alone carries it, and should.** The uniform rule is one lexical
fact about every bracket. Its home is § 0, where spec-shape.md says the NEWLINE discipline
is stated once. Writing it into the productions costs more: GR +84 against RbComma +39.
The one interaction a reader has to resolve is a NEWLINE that could be either `Sep` or the
NEWLINE before a closer, as in fragment 10. RbComma's "before a closing bracket" covers it.

**Route (a): a production should say it.** The exceptions are facts about individual
productions, and spec-shape.md's one-home rule puts each one in the production that
governs it: `Args`, `Params`, `Member` get `{ [ NEWLINE ] "," X } [ NEWLINE ] ")"`, and the
group, array and map get `[ NEWLINE ]` before their closer. That is **GQ at +66, 29
cheaper than the sentence-only Qtrue (+95).** It also leaves no production name inside §
0's prose.

`grammar` passed 9 of 9 on the pristine text, on GQ and on GR (run in this directory; the
pristine run took 30.9 s real against 29.5 s user). It judges the productions against each other and never
against the parser, so it is green on today's false sentence too.

## 3. Principle 0's burden

- **Repairing the sentence owes nothing under Principle 0.** Its tokens still owe the
  payment rule, which is unconditional under §1.6. The payment is the § 10 removal plus
  the registered prediction.
- **Route (b)'s widening is a repair.** It lets a NEWLINE stand only before `)`, `]`, `}`
  or `,`, tokens that cannot begin an expression. So no program's meaning can change and
  no plausible mistake stops being an error. It makes the compiler true to the part of its
  own spec (P: *"between any two tokens"*) that is safe to make true. Under CLAUDE.md § 12
  that is the compiler's bug, and it owes neither a Part 11 effect nor a compiler need.
  Under RbComma, 10 probes change from refused to accepted, and none the other way.
- **R's narrowing is not a repair.** Refusing a NEWLINE before a call's, a signature's or
  an extern's `,` removes programs that parse today (t03 t04 t08 t11 t18 x03), and
  `heroes fmt` prints that shape: `,` on a line of its own, 6 times in its output for
  `tests/golden/surface-fixtures/comments101/nextvalue.hero`, a fixture that parses at exit
  0. It catches no measured class of mistake, so its burden is unmet.
- **Route (c) is not a repair either.** v01 ran: `[a` / `- b]` is two elements (it prints
  `2`), and v02 `(a` / `- b)` is `expected_group_close`. Route (c) would make v02 a
  subtraction, which is a silent pair, the thing §1.4 exists to prevent. It is a different
  question from §4.15's deferral at depth zero, and design.md is silent on it (the shared
  brief's grep; I did not re-run it). It would owe at least the evidence §4.15 asks of its
  sibling.

## 4. Where the rule's instrument lives

**No suite checks the § 0 sentence against the compiler.**
- A grep of `suite_spec`, `suite_grammar` and `suite_special` for `NEWLINE`, *never ends a
  statement* and *between any two tokens* finds only `grammar`'s list of lexical classes.
- Across the whole tree, the sentence appears only in the spec, `.claude/rules/spec-shape.md`,
  panel 179's files and DEFECTS.md.
- `e497646a`'s body says ExternParams, `Params`, `Args` and `TypeArgs` "inherit" the
  rule, and names one run: the multi-line extern signature. t09, u01 and u05 show that `TypeArgs` refuses a
  break before its `)` and its `,`. One shape was run, and the neighbouring shapes were not
  (CL-061).

**What would have caught it on 2026-09-12:** one `surface` row per clause, of the form
`heroes parse <fixture>` with the exit code the sentence predicts. A row breaking before
`+` inside a group, expecting exit 0 as P says, is red on the first day. `probe.Probe` rows
exist today. Panel 179's `heroes probe` bracket-break family is how this generalises, but
it has **not landed**: it is not in `heroes --help`.

**The sitting should add both of these:**
- a fixture directory of these 35 shapes, each carrying its expected outcome and citing
  § 0, run as `surface` rows;
- a requirement, when the probe lands, that every discard of the bracket-break family is
  a break § 0 refuses.

## Adjacent shapes, found and not priced

- A continuation line inside brackets may start at any column. t16, at 7 spaces, parses,
  while § 1 says *"exactly 4 spaces per level"*. The spec is silent here, and the error
  runs the harmless way: a reader over-constrains and is never refused.
- Blank lines inside brackets were accepted in all four probes (y01 to y04).

## Files

- Probes, with 35 `.hero` files: `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/spec-warden/probes/`
- Every priced spec variant: `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/spec-warden/variants/`
  (`pristine.md` is today's text)
- `spec/heroes-spec.md` in this directory is restored to the pristine digest. `--refresh` was
  never run.
