# LEARN — what the author might want to understand

Read by **`/learn`**, and by nothing else. **Never convened by the assistant**:
not at a milestone close, not when this file grows, not as a suggestion at the
end of a step. It exists when the author asks for it.

Every item asks **what is true** — how a pass works, why a representation was
chosen, what a failure's cause was, what a golden guards against. **None of them
blocks anything, and that is why this file does not live with the work**: an
explanation left unread costs nothing until it is wanted, and filing it beside
the decisions the compiler is waiting on made two opposite clocks share one
shelf, where the urgent kind always wins.

**A question here is a note, not a question yet.** `/learn` turns it into one:
where we are, why the piece exists, the code pasted rather than cited, what the
alternatives were, then one question with lettered options.

**One notation, and a ticked item STAYS here** — this list is also its own
record, which is what makes it the one live list `records/lists` does not read
for `- [x]`. The count under the banner is the OPEN items, so it says what is
still unanswered. Measured 2026-08-26 and true today: a `- [x]` has never
existed in this file and `git log --grep='^learn:'` returns zero commits. That
is the contract working as written rather than a backlog, and the count is not a
debt.

**The shape**, since 2026-09-07 and the same as the other four lists: one line
per item, `- [ ] **<origin>** | <the question>`, then the body indented four
spaces. Nothing lives outside the two banners.

*******************************************************************************
**OPEN: 342**

- [ ] **M-cstr-lifetime, walkthrough** | Read `selfhost/check/lending.hero` top to bottom and say, before looking at the tests, which of its three clauses would catch each of five programs | `selfhost/check/lending.hero` · `tests/golden/check/fixedbugs-a-lend-*.hero` | three clauses that look like one rule are three different kinds of rule, and telling them apart is the whole lesson

    **Origin:** M-cstr-lifetime close, 2026-09-09.
    **The five programs**, and each is one of the golden cases or a near miss:
    `return ("a" + n.to_str()).cstr()` · `return pass(c: s.cstr())` where
    `pass` answers `cstr` · `record H` with a `c: cstr` field · `c = s.cstr()`
    then two extern calls · `strlen(s: "legal".cstr())`. For each, name which
    clause fires, or say none.
    **The question after:** one of those five is refused by TWO clauses at
    once, and one is sound today and refused anyway. Say which, and say what
    the second one buys.

- [ ] **M-cstr-lifetime, golden ratification** | Four cases were written and marked nothing; read them and decide whether each pins what it claims to pin | `tests/golden/check/fixedbugs-a-lend-*.hero` and their `.expected` | a golden that pins the wrong thing is worse than none, and the only way to know is to read the case against the rule

    **Origin:** M-cstr-lifetime close, 2026-09-09. The four are the returned
    lend, the two-hop laundering, the record field and the bound lend.
    **The question after:** one of the four provokes two diagnostics and its
    annotations say so. Work out why the OTHER three provoke exactly one each,
    and then say which clause would have to change for that count to move.

- [ ] **M-cstr-lifetime, mutation drill** | Break the rule four ways in `selfhost/check/lending.hero` and predict which suite goes red for each, before running any | `selfhost/check/lending.hero` · `.claude/rules/verification.md` § What gates what | the map exists so that "which suites" stops being a guess, and this is the drill that proves you can read it

    **Origin:** M-cstr-lifetime close, 2026-09-09.
    **The four mutations:** delete the early exit; change `argument_of` to
    record only a call's args and not a UFCS receiver; drop the `is_extern`
    guard in `no_cstr_out_of_heroes`; and make `no_cstr_in_a_record` ignore
    `rec.header`. For each, name the suite that catches it and the case that
    fires.
    **The question after:** exactly one of the four is caught by NO suite in
    the tree today. Say which, and what case would have to exist.

- [ ] **M-cstr-lifetime, exit quiz** | Six questions with one answer each, and five of the six were got wrong by somebody during this milestone | `docs/journal/040-cstr-lifetime.md` · `docs/panel/122-the-lend-was-two-defects.md` | every question here is a place a measurement replaced a belief, and four of them replaced the coordinator's

    **Origin:** M-cstr-lifetime close, 2026-09-09.
    **The six:** (a) how long does the slot holding a lent `str` live, exactly?
    (b) why is a rule about the TYPE `cstr` wrong when a rule about the
    POSITION of `.cstr()` is right? (c) what does `§4.19` prescribe for calling
    an extern in another module, and what does it NOT prescribe? (d) why can no
    position rule close defect 024? (e) which of the two 2026-09-08 numbers
    about the corpus was wrong, and by how much? (f) why was this milestone
    left untagged?
    **The question after:** answer (f) in one sentence that does not mention
    defect 024, and say whether your sentence is still true.

- [ ] **M-interpolation-verdict step 1** | Write `x = "a" + b + c` in a file and predict what `heroes parse --dump-ast` prints for that line, out of four candidate shapes, before running it | `heroes parse <file> --dump-ast` · `spec/heroes-spec.md` § Operators | the answer is why the dump can be used as a measuring instrument, and the wrong answers are why raw text cannot

    **Origin:** M-interpolation-verdict step 1, 2026-09-08, the step that used
    the dump to count 2536 holes in 1461 concatenations.
    **The four candidates:** (a) `x = "a" + b + c`, unchanged; (b)
    `bind x = ("a" + b + c)`; (c) `bind x = (("a" + b) + c)`; (d)
    `bind x = ("a" + (b + c))`. Exactly one is right, and which one it is
    settles two things a text search has to guess: where the operator's
    precedence put the grouping, and whether a chain broken across three source
    lines comes back as one.
    **The question after:** `+` on `str` is left-associative here. Name one
    program whose PRINTED result would differ if it were right-associative, and
    one where it could not.

- [ ] **M-interpolation-verdict step 2** | Four candidate rules for what may stand inside `{...}`. Rank them by what they cost the SPEC, before reading the number | `docs/measurements/022-the-narrow-rule-costs-more-than-the-wide-one.md` · `design.md` §1.2 | the ranking most readers write down is upside down, and seeing why is seeing what §1.2 actually prices

    **Origin:** M-interpolation-verdict step 2, 2026-09-08.
    **The four:** only a bare name · a name and its `.field` steps · any
    postfix run, a call or an index included · any expression at all. Rank them
    cheapest-first in spec tokens, with the opening sentence, the example and
    the escape rule held identical in all four so only the rule sentence moves.
    **The question after:** the ROADMAP's entry calls the bare name *"the
    cheapest rule to write and to lex"*, and it is right about the lexer. Say
    in one sentence why that does not make it the cheapest rule, and which of
    the two costs §1.2's formula counts.

- [ ] **M-interpolation-verdict step 2** | 312 string literals in this tree hold a brace and 24 of them look exactly like a hole. Work out, from the spec, whether those 24 would break loudly or quietly | `examples/template/main.hero:145-235` · `spec/heroes-spec.md` § Bindings, § Files and layout | the difference between a loud and a quiet migration is the whole of §1.12, and here it is decided by three rules a reader can check

    **Origin:** M-interpolation-verdict step 2, 2026-09-08.
    **The setup:** all 24 are in `examples/template/main.hero`, which
    implements `{key}` substitution by hand, and they read `"Hello, {name}."`,
    `"{name} the {role}"`, `"{anything}"`. Suppose `{n}` became a hole naming a
    binding. Say whether each of the 24 would still compile.
    **The question after:** the answer rests on three separate rules of the
    language, not one. Name them, and then name the one thing that would have
    to be true of that file for the answer to flip to quiet.

- [ ] **M-discard-refusal step 3** | Three repairs of one refused line print three different answers. Predict them before you look, then say which of the three a `certain` fix could ever have been | `docs/panel/118` R3 · `selfhost/value_errors.hero` `discarded_failure` | this is the whole argument for shipping no automatic repair, and it is two commands long

    **Origin:** M-discard-refusal step 3, 2026-09-08. The program is four lines:
    `risky` fails on a negative, `run()` returns `i64?` and contains the
    refused line, `main` prints `run().default(1)`. Write the three repairs —
    `_ = risky(0 - 1)?`, `_ = risky(0 - 1).is_err()`, `_ = risky(0 - 1).must()`
    — and predict each printed answer. Then run them.
    **The question after:** one of the three preserves the program's meaning
    exactly. It is also the one that preserves the bug. Say what that means for
    what `heroes check --apply` is allowed to do.

- [ ] **M-discard-refusal step 3** | A wall with a door beside it: work out, from the spec alone, how to defeat the rule on a KNOWN fallible in five characters | `spec/heroes-spec.md` § Bindings · `docs/panel/118` § Amendments | the sitting adopted a refusal that closed nothing, and the fact that killed it is one program long

    **Origin:** M-discard-refusal step 3, 2026-09-08 — the amendment that
    withdrew R1's third position. Read the clause at `spec:99` and then answer:
    if `_ = e` refuses a `T?`, what is `_ = [e]`? Then say why the spec spends
    tokens naming that hole instead of closing it, and what would have to be
    true for closing it to be possible without reading another file.

- [ ] **M-discard-refusal step 2** | Two repairs for one clang warning: one deletes the assignment, one keeps it and says the silence is deliberate. Both work on the shape that provoked them. Say which shapes separate them | `selfhost/emit/body.hero` · `selfhost/emit/unread.hero` `may_lose_its_destination` | the first was built, measured clean, and thrown away, and the reason is a runtime call carrying a check

    **Origin:** M-discard-refusal step 2, 2026-09-08. `_ = a == b` on two
    integers, on two `str`, and on two arrays. For each, say what the emitter
    prints and whether dropping the assignment would delete anything worth
    keeping. `runtime/parts/array.c`'s `hero_array_require` and
    `selfhost/emit/structural.hero`'s deliberate `hero_panic` are the answer's
    two halves.

- [ ] **M-discard-refusal close** | The mutation table's second column, and what it measures that the first does not | `docs/journal/038-discard-refusal.md` · `heroes mutate --operator drop-question --survivors` | 236 killed is the headline and 193 is the number that says what the thesis buys

    **Origin:** M-discard-refusal close, 2026-09-08. Run it. The `check` column
    says 236 of 236; the `--permissive` column says 193. Say in one sentence
    what the 43 are, and then say why `--permissive` exists at all — design.md
    Part 11 calls it a control arm, and this is the first table in this project
    where the arm's own number is worth reading on its own.

- [ ] **M-ir-lowering.1** | First IR. `x = 2 + 3 * 4` becomes how many instructions — 3, 4, or 5? And which of them proves the precedence table, given the tree is gone by then?

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/tests/scalars.rs (an_expression_becomes_a_line_per_operation)
    **Why it matters:** flattening is the whole job of the pass, and it is what makes the C emitter a printer

- [ ] **M-ir-lowering.1** | A `for` loop is four blocks and a `while` is three. Which block does `continue` jump to in each, and what exactly happens to the program if a `for`'s `continue` lands on the test?

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/control.rs (for_loop) · tests/golden/ir/adversarial-continue-steps.expected
    **Why it matters:** the wrong answer compiles, type-checks, passes every other test, and hangs

- [ ] **M-ir-lowering.1** | `a && b` produces two blocks and no `and` instruction. Say why in one sentence — and then say which spec line forces it

    **Where to look:** spec/heroes-spec.md § Operators · ir/control.rs (short_circuit)
    **Why it matters:** a construct that evaluates one side conditionally IS control flow, and calling it an operator hides that

- [ ] **M-ir-lowering.1** | `function step(@r: Reader, ...) -> int?` with a `?`, an early `return` and a tail: how many `copyout` instructions does the dump show, and in which blocks?

    **Where to look:** tests/golden/ir/adversarial-try-copies-out.expected · ir/verify.rs (check_copy_out)
    **Why it matters:** §4.8's "copy-out happens always" is the sentence panel 000 called the place this project would stall

- [ ] **M-ir-lowering.1** | Four instructions: `add`, `add!`, `lt`, `index!`. Which can stop the program, and what makes `add` and `add!` different — the operation or the operands?

    **Where to look:** ir/print_inst.rs (aborts) · spec lines 126–128
    **Why it matters:** the reader who misses this writes a pass that assumes one exit per block

- [ ] **M-scalars-run** | Name the pass in the verifier's failure, and index the invariants by phase — the practice the record converges on that this project has not taken. LLVM's `-verify-each` exists "for cases where it is suspected that a pass is creating an invalid module but it is not clear which pass is doing it"; rustc bakes the attribution in (`validate_body(tcx, body, format!("after pass {pass_name}"))`), gives MIR a `phase`, panics on an illegal transition, and validates unconditionally at the last phase; Go runs `checkFunc` "between each phase" under `-d=ssa/check/on` and it caught a real ARM bug in `runtime/malloc.go` (golang/go#22499). **Cost: zero payoff today with one pass, total at pass two** — so the moment to take it is when the ownership pass lands, not before (2026-08-19)

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/verify.rs · docs/measurements/002-metric-3.md
    **Why it matters:** retrofitting phase-indexed invariants later means auditing every existing check (Go's own runtime, read 2026-08-19 as precedent)

- [ ] **M-ir-lowering.2** | `g.rows[r].cells[c] @ v` printed as `store g.rows[$t1].0[$t2]` until the fix. Say what the printer had lost, and why only the *second* field name was affected — not the first

    **Where to look:** ir/print_names.rs (field_type) · tests/golden/ir/place-paths.expected
    **Why it matters:** a path is a fold, and dropping the accumulator only shows up from the second step on

- [ ] **M-ir-lowering.2** | The verifier had 257 lines of checks and no test that any of them fired. Fifteen tests now break one invariant each. Pick two and say what the *wrong* IR would have done at M-scalars-run, in C

    **Where to look:** ir/tests/verify.rs · docs/journal/005-lowering.md §3
    **Why it matters:** LLVM keeps a directory for exactly this; a safety net nobody has fallen into is indistinguishable from no net

- [ ] **before M-ir-lowering** | Hand-desugar `for x in xs`, `?`, and one UFCS chain from the appendix calculator — **against the IR text** (panel 019 re-specified this: there is no desugared tree to compare against, so the exercise's answer key is `heroes build --dump-ir`)

    **Where to look:** design.md Part 5 · tests/golden/ir/
    **Why it matters:** the desugarer landed in M-ir-lowering and the dump is the only evidence the table was honoured

- [ ] **panel 018 impl** | The pre-018 shape `MAX = constant: int` now costs exactly ONE diagnostic and the body below it vanishes with it. Which code fires, and what makes the recovery stop before the next declaration?

    **Where to look:** syntax/tests/recovery.rs (the_old_shape_costs_one_diagnostic) · syntax/decl.rs file()
    **Why it matters:** the migration path is itself a diagnostic, and the closed keyword set is the anchor

- [ ] **panel 018 impl** | `record Point` now ends in a token that can end a statement. What did that single fact let recovery DELETE, and what replaced the span arithmetic?

    **Where to look:** syntax/recover.rs recover_to_next_decl · docs/glossary/002 § coda
    **Why it matters:** panel 007's patch died of the inversion — the lesson demonstrated on itself

- [ ] **panel 018 impl** | `for i < 3` gets the `while` fix as a Guess; `for !done` gets it Certain. What distinguishes the two parse states, given the message is identical?

    **Where to look:** syntax/stmt.rs for_stmt
    **Why it matters:** certainty is a property of the parse state, not of the message

- [ ] **panel 018 impl** | `function f():` costs one diagnostic with a certain fix and the block still parses. Where is the colon eaten, and on which constructs does the same helper fire?

    **Where to look:** syntax/stmt.rs eat_python_colon · tests/golden/check/trailing-colon.hero
    **Why it matters:** the blind experiment's top pre-registered slip became machine-repairable

- [ ] **before M-checker-core** | Paper exercise: hand-check five expressions in two columns (⇐ checking / ⇒ synthesis)

    **Where to look:** glossary entry to be born from this session
    **Why it matters:** M-checker-core is the conceptual cliff; the exercise should precede it

- [ ] **M-token-stream.4** | Write `let x = 5` in a .hero file and run `heroes lex` on it: what comes out, and what travels attached to the diagnostic?

    **Where to look:** spec/reserved-words.md · lexer tests (certain_fix_travels_with_the_diagnostic)
    **Why it matters:** the thesis made executable: the likeliest LLM mistake fails loudly with the repair pre-written

- [ ] **M-syntax-tree.1** | `function advance(@l: Lex)` dumps with a result type. Which one, and where did it come from, given the source never wrote it?

    **Where to look:** syntax/decl.rs (the arrow branch), printer/dump.rs
    **Why it matters:** a synthesised node is how later passes stop asking "was it written?"

- [ ] **M-data-declarations** | A `match` used as a VALUE requires every arm to produce a value. Found by the llm-ergonomist in panel 014: the rule is orthogonal to the arm-body spelling, and today `tag = match t` with an arm whose body is `break` (or `v @ v + 1`, which does not even diverge) is silent under **both** readings — `=> assert false` inline and the same statement inside a block arm are equally unguarded

    **Where to look:** docs/panel/014-*.md, archive/bootstrap-rs/heroes/src/syntax/control.rs
    **Why it matters:** it is the only sentence in that panel that converts a silent wrong program into a loud one, and it belongs to the checker, not the grammar

- [ ] **panel? M-syntax-tree.2** | The foreign-word registry reserves words that are plausible *identifiers*: the appendix had a variant case `.var` and could not lex. Found the same class as panel 013, Heroes-side

    **Where to look:** archive/bootstrap-rs/heroes/src/lexer/keywords.rs (foreign_word), design.md appendix (`.var` → `.variable`)
    **Why it matters:** `var case union use include class try const` are all unusable as names today; the registry's price is now measurable

- [ ] **M-syntax-tree.2** | `if` is an expression (§4.7). In the dump of `state = if t.done …`, what appears on the `bind` line, and where do the branches go?

    **Where to look:** archive/bootstrap-rs/heroes/src/printer/bodies.rs (write_valued), tests/bodies.rs (2026-08-19)
    **Why it matters:** one rule seen in two places is the claim; the printer is where it either holds or does not (the bootstrap's test tree, archived 2026-08-19)

- [ ] **M-syntax-tree.3** | `# Just a remark.` + blank line + a declaration. Why is the formatter forbidden from closing that gap?

    **Where to look:** archive/bootstrap-rs/heroes/src/printer/tests.rs (a_blank_line_between_comment_and_declaration_is_preserved), §4.1 (2026-08-19)
    **Why it matters:** in this language whitespace carries meaning twice: indentation, and this (the bootstrap's printer tests, archived 2026-08-19)

- [ ] **M-checker-core** | Classify `break`/`continue`/`return`: they must NOT be typed `()`. RFC 1216 records that exact wrong turn ("some code in the compiler assigns type `()` to diverging expressions because it doesn't have a sensible type to assign to them"); under `()` an `int`-valued `match` with a `return` arm becomes a type error and panel 014 reconvenes

    **Where to look:** design.md §4.7 (the new bullets), docs/panel/014-match-arm-body.md
    **Why it matters:** Rust types them `!`, Kotlin `Nothing` — the historian's decisive row

- [ ] **panel? M-checker-core** | `+` on `str` is unwritable per the spec's operator table, while §4.20 gives the runtime string concatenation and the appendix writes `"…" + name`. Found by the judge that had only the spec — it restructured a whole program to avoid a construct it could not find

    **Where to look:** docs/panel/015-* § Watch list, design.md §4.20
    **Why it matters:** the same class as the missing `\"` (panel 008): the language is INCOMPLETE without it, and nothing says so

- [ ] **M-name-resolution** | Order-free top level. `factor` and `group` call each other, and neither is declared first. How many passes does the resolver make over the declarations, and what exactly is in the table before the first body is walked?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/top.rs (module doc)
    **Why it matters:** this is why the language has no forward declarations — not "does not need", cannot express a use for

- [ ] **M-name-resolution** | Three names in one function: `MAX` (a top-level constant), `x` (a local), `print` (a built-in). Each records a different `Ref` variant. Which one is looked up FIRST, and what would break if the order were reversed?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/exprs.rs (`name`), tests/names.rs (2026-08-19)
    **Why it matters:** the lookup order is what makes "shadowing is an error" and "a built-in's name is taken" consistent instead of contradictory (the bootstrap's test tree, archived 2026-08-19)

- [ ] **M-name-resolution** | `total: int @ 0` then `total @ 1` and nothing else: one diagnostic. Now add `print(total)` at the end: zero. Which counter changed, and why is the *initialiser* not counted as a write?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/scope.rs (`report_unused`), tests/unused.rs (2026-08-19)
    **Why it matters:** if the initialiser counted, the unused rule would be a no-op for every cell in the language (the bootstrap's test tree, archived 2026-08-19)

- [ ] **M-name-resolution** | A file whose last function is `function simplify(e: Expr) -> Expr` / `???` reports no unused bindings anywhere — not even in the function at the top. Which field decides it, and where is it computed (hint: not in the walk)?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/mod.rs (`has_hole`), §4.16
    **Why it matters:** a hole at the bottom of the file suspends a rule at the top of it, so the decision cannot be made while walking

- [ ] **M-name-resolution** | `print(totl)` next to `total = 1` used to produce **two** errors. Which one was the consequence, and what does the resolver now remember in order to stay quiet about it?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/mod.rs (`near_names`, `suggested`), journal 003 §3
    **Why it matters:** the compiler was reporting a mistake it had itself proposed the repair for

- [ ] **panel? M-checker-core** | A `match` over `bool` is treated as countable, so `_` is banned over it — but `true`/`false` are literals, not `.cases`, so an exhaustive `bool` match cannot be written at all today. Found while writing `patterns.rs`

    **Where to look:** archive/bootstrap-rs/heroes/src/types/patterns.rs (`wildcard`, `exhaustive`)
    **Why it matters:** the two rules that hold each other up (§4.7) do not fit the one type that is a two-case variant by construction

- [ ] **panel? M-data-declarations** | `()` inside a container — `[()]`, `{str: ()}`, `()?` — resolves and checks clean. Ten lines to reject, and the engineer's condition 4 in panel 017 says C1 is *not* credited with closing it

    **Where to look:** docs/panel/017-* § Resolution, archive/bootstrap-rs/heroes/src/types/lower.rs
    **Why it matters:** M-value-aggregates's descriptor pass would otherwise be asked for `h_unit_copy` on a zero-size element

- [ ] **panel? M-data-declarations** | §4.9's same-typed-argument rule lands hardest on FFI, where C's numeric APIs cluster same-typed parameters: `pow(base:, exponent:)` and `hypotenuse(a:, b:)` now need labels at every call site. Visible in `examples/gallery/08-ffi.hero`. Does the rule cross the `extern` boundary, or does a bound C signature get an exemption?

    **Where to look:** examples/gallery/08-ffi.hero, design.md §4.9, §4.19
    **Why it matters:** the rule is one of the thesis's headline mechanisms and this is the first place its price is concrete

- [ ] **panel? M-rich-diagnostics** | `to_i64` on a `str` is not offered (parsing can fail and the spec does not say what it returns), and ordering on `str` is not offered either (`a < b` on strings is an error, because a collation is a language decision). Both are rejections, so both are relaxable

    **Where to look:** archive/bootstrap-rs/heroes/src/types/builtins.rs, ops.rs
    **Why it matters:** two absences a model will reach for, chosen rather than overlooked

- [ ] **M-checker-core** | Bidirectional checking, the two modes. `x = []` is an error and `xs: [int] = []` is not — which mode is each, and why can exactly four forms not synthesise?

    **Where to look:** archive/bootstrap-rs/heroes/src/types/mod.rs (the table), expect.rs
    **Why it matters:** §4.5's promise that errors stay local is this table and nothing else

- [ ] **M-checker-core** | `tag = match t` with one arm `=> return 0`: legal, and `tag = match t` with *every* arm a `return`: an error. Which file decides that, and how many places in the compiler know what a diverging branch is?

    **Where to look:** archive/bootstrap-rs/heroes/src/types/join.rs, stmts.rs (`Flow`)
    **Why it matters:** the answer is 1, and a judge's objection was withdrawn because of it

- [ ] **M-data-declarations** | `t == .plus` type-checks and `t = .plus` does not. What does the equality operator hand to the case, and why does every *other* binary operator do the same thing?

    **Where to look:** archive/bootstrap-rs/heroes/src/types/exprs.rs (the Binary branch, `contextual`)
    **Why it matters:** one rule where two would have been the obvious shape

- [ ] **M-rich-diagnostics** | Run `heroes check` on a file with one wrong field label. Which four pieces does the message carry, and which one of them is the file you would otherwise have opened?

    **Where to look:** archive/bootstrap-rs/heroes/src/diagnostics/render.rs, §4.17
    **Why it matters:** the whole §4.17 argument is that a model has no project open

- [ ] **M-rich-diagnostics** | `heroes mutate` prints two columns. Which one is the measurement, and what would a compiler that rejected every program score in each?

    **Where to look:** archive/bootstrap-rs/heroes/src/mutate/mod.rs, docs/measurements/001-metric-3.md
    **Why it matters:** panel 011 made the control arm mandatory before any number could be quoted

- [ ] **panel 020** | The exit-code question is the panel's most interesting disagreement: the historian defended **2** from GCC's `sorry()` and the ergonomist measured **1** by watching its own first action under both. Read GCC's `toplev.c` line the historian quoted (`if (sorrycount) exit (FATAL_EXIT_CODE)`) and say which judge the source actually supports

    **Where to look:** docs/panel/020 § Where the judges disagreed
    **Why it matters:** the same evidence pointed both ways until somebody read it to the end |

- [ ] **M-scalars-run.1** | First C. `print((2 + 3) * 4)` becomes how many lines of C, counting neither declarations nor `#line` directives — and which of them can stop the program?

    **Where to look:** tests/golden/emit/scalars.expected · archive/bootstrap-rs/heroes/src/emit/inst.rs
    **Why it matters:** the emitter is a printer, and reading its output is reading what clang will see

- [ ] **M-scalars-run.1** | Why is `#line 8 "f.hero"` repeated before every instruction on line 8 rather than emitted once? One sentence, and it is about C rather than about Heroes

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/writer.rs (the module doc) · tools/spike/01-first.c:19
    **Why it matters:** the frozen target had this wrong and nobody noticed for two milestones

- [ ] **M-scalars-run.1** | An `if` whose both arms `return` produces a block the emitter does not print at all. Say what would happen in C if it printed the label but not the block, and what would happen if it printed neither the label nor the `goto`

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/decls.rs (reachable)
    **Why it matters:** C is physical, and this is the one place where that matters more than the CFG

- [ ] **M-scalars-run.1** | `bump(@v)` becomes three lines of C in two functions. Name them, and say which one runs on the error side of a `?`

    **Where to look:** emit/decls.rs (prologue) · emit/inst.rs (CopyOut) · design.md §3.1's `@` bullet
    **Why it matters:** §4.8's "copy-out happens always" is now a calling convention, and M-strings-ownership/M-value-aggregates/M-generics-library inherit it

- [ ] **M-scalars-run.2** | The dominance invariant's first wording was rejected by an existing test within the hour. Read both wordings and say which construct falsified the first — then say why that construct is *correct*

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/values.rs (the module doc) · ir/tests/verify.rs
    **Why it matters:** an invariant that is too strong is as wrong as one that is too weak, and only one of the two is loud

- [ ] **M-scalars-run.2** | `heroes build` on a file with a hole exits 1; `heroes check` on the same file exits 0. Both are right. Say what question each command is answering

    **Where to look:** archive/bootstrap-rs/heroes-cli/src/commands/compile.rs · §4.16
    **Why it matters:** the ergonomist measured exit 0 producing a `build && ./artifact` loop that runs yesterday's binary

- [ ] **panel 021** | Reference counting leaks cycles, and Nim needed a whole second collector (ORC) for exactly this. Safe for `str`; false the first time a refcounted type can cycle, which is M-value-aggregates's recursive `variant`

    **Where to look:** docs/panel/021 § Watch list · examples/gallery/11-trees.hero
    **Why it matters:** the milestone that makes it false is the next one

- [ ] **M-strings-ownership.1** | First refcount. `s: str @ "a"` then `s @ s + "b"` produces how many refcount instructions, and which of them is the one that would double-free if the pair were reversed?

    **Where to look:** tests/golden/emit/strings.expected · archive/bootstrap-rs/heroes/src/own.rs (rule 3)
    **Why it matters:** the order of two operations around a store is the whole rule

- [ ] **M-strings-ownership.1** | The pass **moves** an owning temporary into a slot instead of releasing it at the end of its block. Read the two refuted wordings in `ir/phases.rs`'s doc and say what `.must()` does to an expression that made the second one false

    **Where to look:** archive/bootstrap-rs/heroes/src/own.rs (rule 5) · ir/phases.rs
    **Why it matters:** this is panel 019's slots-over-phi decision being cashed, one milestone later

- [ ] **M-strings-ownership.1** | A plain `str` parameter is never decrefed and an `@ str` parameter is never swept. Both are correct and for different reasons. Give each reason in one sentence

    **Where to look:** archive/bootstrap-rs/heroes/src/own.rs (rules 1 and 2) · design.md §3.1's `@` bullet
    **Why it matters:** the convention a judge got wrong on its first compiled program, with ASan as the referee

- [ ] **M-strings-ownership.2** | `print(0.1)` prints `0.1` and `print(1.0)` prints `1.0`. Say what breaks in the golden harness if the second printed `1` — the answer is not about readability

    **Where to look:** tests/golden/run/f64-rendering.hero (the comment) · runtime/runtime.c (hero_f64_render)
    **Why it matters:** it is the one place a type confusion becomes observable in the artifact the harness compares

- [ ] **M-strings-ownership.2** | Three forms of the "every owned slot is released" check failed before one worked, and the third failure was invisible: the check stayed silent on a hand-broken sweep. Say what a store to a counted slot has in common with a sweep, and what separates them

    **Where to look:** ir/phases.rs (released_on_return)
    **Why it matters:** an invariant that cannot fail on a hand-made violation is not an invariant

- [ ] **panel 022** | **CORRECTED 2026-08-12 (panel 023, owed since):** it is *not* 40 bytes for every `T` — `sizeof(Big?)` is **48** for a 40-byte record, because past 32 bytes the payload wins and the union takes the larger side. The claim was measured while the judge was compiling something else, and it is the shape of statement that is easy to write and false at one boundary. Original, kept as the record's own vocabulary: `T?` is **40 bytes for every `T`**, because `Failure` is two `str`s — so every `m[k]` returns through memory rather than in registers, measured on AAPCS64. design.md fixes `T?`'s C representation **nowhere**; M-value-aggregates states it rather than inheriting it. Rust polices the same largest-case tax with two lints, `result_large_err`'s threshold being 128 bytes

    **Where to look:** docs/panel/022 § Watch list · design.md §4.6, §4.20
    **Why it matters:** the cost lands on the return type of every fallible function in the language

- [ ] **M-value-aggregates step 1** | The first **graph algorithm** in this compiler. `types/sized.rs` colours nodes white/grey/black instead of carrying a `visited` flag, and the difference IS the algorithm: grey means "on the current path", which is what makes a back edge a cycle rather than a diamond. Question: for `record Point/record Rect { a: Point, b: Point }`, how many times does the walk enter `Point`, and what would a `visited` flag get wrong that colours get right — and vice versa?

    **Where to look:** archive/bootstrap-rs/heroes/src/types/sized.rs (walk) · docs/panel/023 R9
    **Why it matters:** the historian predicted the first defect here would be a false positive, and this is the mechanism that avoids it

- [ ] **M-value-aggregates step 1** | `Checked::type_order` is empty for a cyclic file, and the first version published a partial order instead. Task: say what a *partial* topological order of a cyclic graph would have done downstream, and why "the emitter never runs on a program with diagnostics" is a weaker guarantee than an empty list

    **Where to look:** archive/bootstrap-rs/heroes/src/types/sized.rs (the tail of `walk`) · the test `a_cycle_leaves_no_order_to_mistake_for_one`
    **Why it matters:** it was a test that found this, one line after the code was written

- [ ] **M-value-aggregates step 1** | Count: the corpus invariant runs over **66** accepted programs and only **14** aggregate declarations. Question: why so few, and which of `heroes mutate`'s ten operators could ever produce a containment cycle? (The answer decides whether `no_size` can appear in Metric 3 at all.)

    **Where to look:** archive/bootstrap-rs/heroes/src/types/tests/sizes.rs · harness/mutations/operators.md
    **Why it matters:** an invariant over a corpus with no instances is a rule nobody is following

- [ ] **M-value-aggregates coverage** | Two rules bit while writing the coverage cases, and both are the language working rather than failing: `_` as a whole ARM over a variant is `wildcard_on_variant` ("name every case, so that adding one breaks this `match`"), and an unread parameter is `unused_binding`. Task: for a variant with five cases where three share a body, write the arm that is legal — and say what `|` costs against what `_` would have cost

    **Where to look:** spec lines 99-103 · tests/golden/run/variant-arms.hero
    **Why it matters:** the rule exists so that adding a case is a compile error, and it is only worth its cost if the legal form is writable

- [ ] **M-value-aggregates step 5** | The first **type-erased dispatch** in this project: the runtime works on an array through `HeroDesc`'s five function pointers, and the emitter's own calls stay typed. Count: for `[Point]` where `Point` holds a `str`, how many functions does one `hero_array_decref` reach, and which of them is the one the compiler generated versus the one it wrote by hand?

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/perfn.rs (descriptors) · runtime/runtime.c (hero_array_decref) · tests/golden/emit/aggregates.expected
    **Why it matters:** it is the one place the "clang type-checks every call" property is deliberately given up, once per type

- [ ] **M-value-aggregates step 5** | `push` **copies, always** — building an array by successive push is O(n²), the same shape as `s + t`. Question: what would have to be true of `xs` for an in-place append to be legal, and why can the IR never know it? (The answer is in what a refcount of 1 means for a value held by a slot.)

    **Where to look:** runtime/heroes_runtime.h (hero_array_push) · design.md §4.10's "known performance consequence"
    **Why it matters:** the choice looks like a performance decision and is a correctness one

- [ ] **M-value-aggregates step 5** | Three defects in one step, each a *missing row* rather than wrong logic: `Ty::Array` absent from the field walk in `perfn.rs` (aborted at `hero_unreachable`), a variant's `hash` prototyped but never defined (undefined symbol at link), and a descriptor emitted after the function that names it (undeclared identifier). Task: say which of the three the "list every arm, never a catch-all" rule caught, and which two it did not — and what would have caught them

    **Where to look:** commits after ce2276a · archive/bootstrap-rs/heroes/src/emit/perfn.rs
    **Why it matters:** the rule has a shape, and knowing its edge is knowing when to add another instrument

- [ ] **M-value-aggregates step 6** | **Copy-on-write, per step.** Count: for `g.rows[0].cells[0] @ 7` where `h = g` shares every level, how many heap blocks exist before the write and how many after — and which of them does `h` still point at? Then say what a single unshare at the primitive would have left `h` pointing at

    **Where to look:** tests/golden/run/adversarial-cow-per-step.hero · runtime/runtime.c (hero_array_unshare, hero_array_set) · docs/panel/022 R2
    **Why it matters:** panel 022 measured the wrong version passing every instrument this project has

- [ ] **M-value-aggregates step 6** | The order is the rule: the value is increfed **before** the outermost unshare. Task: trace `n.children[0] @ n` with the incref moved to *after* the unshare, and say what the stored value would then refer to. (The answer is why `Op::CowCheck` is not an IR instruction — a judge built a three-block cycle out of the hoistable form.)

    **Where to look:** archive/bootstrap-rs/heroes/src/own.rs (the indexed-store arm) · docs/panel/022 R3
    **Why it matters:** it is the one place in the pass where instruction order carries the correctness argument

- [ ] **M-value-aggregates close** | **Walkthrough offer** (optional, author's call): the path of one `g.rows[0].cells[0] @ 7` from source to C, through `sized.rs`'s order, `counted.rs`'s answer, `own.rs`'s incref, `aggregate.rs`'s lvalue walk, and the three runtime primitives

    **Where to look:** docs/journal/008-aggregates.md · tests/golden/run/adversarial-cow-per-step.hero
    **Why it matters:** it is the one path in the compiler where four passes each contribute one line to the same statement

- [ ] **M-value-aggregates close** | **Golden ratification offer**: five adversarial cases marked UNVERIFIED — `no-size-best-friend`, `records-own-strings`, `adversarial-aggregate-overwrite`, `adversarial-recursive-tree`, `adversarial-cow-per-step`

    **Where to look:** tests/golden/check/, tests/golden/run/
    **Why it matters:** each was written to fail before it was written to pass; the ratification is checking that claim

- [ ] **M-value-aggregates close** | **Mutation drill offer**: delete one line from `own.rs` rule 6, or one array arm from `perfn.rs`'s field walk, and predict which instrument fires — the leak counter, ASan, clang, or nothing

    **Where to look:** archive/bootstrap-rs/heroes/src/own.rs · archive/bootstrap-rs/heroes/src/emit/perfn.rs
    **Why it matters:** the milestone's own record says the answer differs per line, and that difference is the lesson

- [ ] **M-value-aggregates close** | **Exit-quiz offer**: six defects, all of them a missing row. For each, say which table was exhaustive (so the compiler caught it) and which walk had a catch-all (so it did not)

    **Where to look:** docs/journal/008-aggregates.md § What broke and why
    **Why it matters:** the rule "list every arm" has an edge, and knowing the edge is knowing when to add an instrument instead

- [ ] **M-optional-map** | **`int?` owns memory**, which is the least obvious consequence in the language: its error side is two `str`s, so every `T?` is reference-counted whatever `T` is. Question: how many heap blocks does `half(8)` allocate when it returns `ok(4)`, and how many does `half(7)` allocate returning `fail("odd", "not divisible")`? (The second answer depends on something about literals.)

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/perfn.rs (option_bodies) · tests/golden/run/fallible.hero
    **Why it matters:** it is where "a scalar is free" stops being true, and the reason is a field nobody looks at

- [ ] **M-optional-map** | The map's seed is **fixed**, and the reason is not security or speed. Task: say what would break, and at which milestone, if the seed were taken from the clock — and why the answer is about `diff` rather than about the map

    **Where to look:** runtime/runtime.c (HERO_MAP_SEED) · docs/panel/006 · ROADMAP M-selfhost-fixpoint
    **Why it matters:** it is the one place where a hash table's usual defaults are wrong for this project specifically

- [ ] **M-optional-map** | The leak that only existed with a computed value: `{1: "one" + "!"}` then `.default(…)` lost one block, `{1: "one"}` lost nothing. Explain the difference in one sentence, then say what it implies for how every counting test in this repo has to be written

    **Where to look:** DESIGN-LOG 2026-08-10 (Op::MapGet) · runtime/runtime.c (HERO_STR_STATIC)
    **Why it matters:** the cheaper spelling of every one of these tests would have passed

- [ ] **panel 024** | The other three costed gaps, none of which had been reported before: a map's **iteration order** (and whether it yields keys or pairs), the **sign of `%`** on negatives (`-7 % 3` is `-1` or `2`?), and **`slice` out of range** (clamp, abort, or `T?`). All three are silent when guessed wrong. Plus two the ergonomist reframed: **`T` → `T?` widening** is unspecified and used constantly, and **the spec never shows how to construct a variant case**

    **Where to look:** docs/panel/024 R5
    **Why it matters:** these are the whole silent-error surface a reader with only the spec could find

- [ ] **panel 025** | **A claim of mine was false and I repeated it to you unverified.** Panel 024's ergonomist said `args: [Expr]` and `lhs: Expr` both compile; I reported that. `lhs: Expr` is `no_size` at exit 1 with two notes and a fix. Task: run it, then say what the difference is between a gap that costs a *retry* and one that costs a *wrong answer* — and which of the two the project's instruments can see

    **Where to look:** archive/bootstrap-rs/heroes/src/types/sized.rs · docs/panel/025 § The reversal
    **Why it matters:** the whole ranking of what to spend spec tokens on depends on that distinction

- [ ] **panel 025** | Two gaps both judges named and neither candidate touched, both loud: **`T` → `T?` widening** (unspecified, used constantly — `return 5` from `-> int?` is never sanctioned) and **`if`-as-an-expression's layout** (the spec says it is one and shows only the statement form; the judge routed around it with `@` cells in every program)

    **Where to look:** docs/panel/025 § Watch list · spec lines 116, 141
    **Why it matters:** they cost retries rather than wrong answers, which is exactly the rank this panel established

- [ ] **M-generics-library step 1** | `abort must` was lowered with an **empty argument list**, so the only message available was "a `.must()` failed" — the one thing the reader already knows. The failure now travels with it. Question: why does the read of the failure need no synthetic slot, when panel 021 had to route `assert`'s counted operands through one? (The answer is one word about *where* the read happens.)

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/fallible.rs (the `must` arm) · archive/bootstrap-rs/heroes/src/ir/asserts.rs · docs/panel/021
    **Why it matters:** it is the same hazard in two constructs, and only one of them has it

- [ ] **panel 026** | **I sold you the decision on an inverted reason.** I said a compiler needs incremental insertion for a symbol table. True, but `{K: V}` + insertion gives neither shadowing nor O(1) scope pop and the map has no delete — so it is the *wrong* structure for one, and `resolve/scope.rs` already chose a vector. Task: read `scope.rs` lines 41, 96, 107 and say what a map would have to gain to serve there

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/scope.rs · docs/panel/026 R5
    **Why it matters:** the funding still stands on other grounds, but not on the one I gave

- [ ] **panel 026** | Measured: `{"a":1,"b":2,"c":3}` iterates `b a c` at cap 8, and the same map with a duplicated first key iterates `b c a` at cap 16 — yet `hero_map_eq` says they are equal. Question: which line of `emit/aggregate.rs` puts the *source text's* duplicate count into the data structure, and what else does that leak?

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/aggregate.rs (build_map) · runtime/runtime.c (hero_map_new) · docs/panel/026
    **Why it matters:** a shipped guarantee was false for a reason nobody had looked at

- [ ] **M-generics-library step 2** | **A defect I wrote and the leak counter's magic word caught.** `hero_map_grown` first did `memcpy` of the live entries plus `free` of the old block *without* dropping — the reasoning being that the references pass to the new block unchanged, so no descriptor should run. Every printed answer was correct and the program panicked at exit with `not a Heroes string block`. Task: say what that reasoning got right, and name the second place it put refcount arithmetic

    **Where to look:** runtime/runtime.c (hero_map_grown) · DESIGN-LOG 2026-08-11
    **Why it matters:** the fix is shorter than the bug and does no arithmetic at all

- [ ] **M-generics-library step 2** | `m[k] @ v` needed **zero new IR forms** — `ir/places.rs` already built the step. Count: how many files changed to make a map assignable, and how many would `set(m, k, v)` have needed? (The panel costed the second at three passes plus `Arg::InOut` on a builtin.)

    **Where to look:** docs/panel/026 R2 · archive/bootstrap-rs/heroes/src/emit/aggregate.rs (the map arm of write_element)
    **Why it matters:** the cheap spelling was already in the IR and the expensive one was in the proposal

- [ ] **M-generics-library spec repair** | **The one document nothing in this project reads.** Symptom: a program written from the committed spec answered `error[unknown_name]: nothing named `has` is in scope`, exit 1, four days after `has` was struck. Task: say which of the three artifacts (spec, design.md, the compiler) was wrong under CLAUDE.md §12's precedence rule — and why the answer is not the one the rule's first sentence gives

    **Where to look:** spec line 128 · docs/panel/026 R7 · archive/bootstrap-rs/heroes/src/resolve/tests/spec.rs
    **Why it matters:** the removal measured −17 and shipped −4, and the missing 13 were a sentence in a different section

- [ ] **M-generics-library spec repair** | Count: 23 built-ins, three spellings the spec uses for a name (`` `join` ``, `range(`, `` `.must()` ``), and one English word that must not count as a mention. Question: which built-in is named in the spec by *only* the third form, and what would a bare word-boundary search have wrongly reported struck?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/tests/spec.rs (`mentions`)
    **Why it matters:** a test over prose is only as good as the form it matches, and the loose version passes while meaning nothing

- [ ] **panel 027** | **Two seats vetoed the same thing and neither could see the other's reason.** One compiled it: C11 6.7.9p21 zero-fills a short initialiser list, so the field is `NULL` everywhere with no diagnostic, and the call is `SEGV` with no type name. The other read the tree: the order it installs is one `<` refuses to compute. Task: say which of the two arguments would still stand if the other were withdrawn — and which one panel 022 had already made

    **Where to look:** docs/panel/027 § Where they converged · docs/panel/022 (null `hash`)
    **Why it matters:** the project's rule is that judges are differentiated by *input*, and this is what it buys when it works

- [ ] **panel 027** | `_Static_assert(HERO_RUNTIME_ABI == N)` was believed to catch a stale runtime. Measured: it catches a **function** change (undefined symbol at link) and **not** a struct-field change — new header, old `runtime.o`, link exit 0, a read four bytes past a 40-byte object, silence. Question: what actually protects the build today, and what would have to be true for the `_Static_assert` to earn the sentence the header's comment gives it?

    **Where to look:** runtime/heroes_runtime.h:10-12 · archive/bootstrap-rs/heroes-cli/src/commands/toolchain.rs (the cache key)
    **Why it matters:** a guard credited with a job it cannot do is worse than no guard

- [ ] **panel 027** | The order the port actually needs is **`sort_by_key` over `span.start`** at four sites (`types/mod.rs:197`, `resolve/mod.rs:195`, `ir/mod.rs:258`, `emit/gate.rs:108`), and **neither** branch of the proposal supplies it — a structural `cmp` would sort `Diagnostic` by *kind*. Task: write the Heroes signature that does serve those four sites, and say which M-generics-library step has to land before it can be written

    **Where to look:** archive/bootstrap-rs/heroes/src/diagnostics/mod.rs:71-83 · design.md §4.12
    **Why it matters:** the closure list has no comparator form, and this is the M-generics-library audit's real question

- [ ] **panel 027** | `to_i64`'s range check, and why reasoning about it fails. Two checks a reviewer would sign off are wrong: `v <= (double)INT64_MAX` **accepts** 2^63 and `v > -9223372036854775809.0` **rejects** `INT64_MIN`. Question: what does `(double)INT64_MAX` round to, and why does the correct check use a hex float and a half-open interval? Then: on arm64 an unchecked cast does not trap — say what `inf` and `NaN` silently become

    **Where to look:** runtime/runtime.c (hero_f64_to_int) · docs/panel/027 R7
    **Why it matters:** 8 of 13 probe values are UB under the raw cast and the hardware hides all of them

- [ ] **panel 027** | **A mandate the compiler never implemented, found by pricing a sentence.** design.md:809 says "Slicing that lands mid-sequence is an error"; `slice("caffè", from: 0, to: 5)` exits **0** with a corrupt byte. Task: say why the error belongs at the slice rather than at `chars`, and what law `chars` can assert over `heroes mutate`'s corpus once it is total

    **Where to look:** design.md:809 · docs/panel/027 R4 · runtime/runtime.c (hero_str_slice)
    **Why it matters:** the ergonomist found the symptom, the warden found the mandate, and neither had the other's input

- [ ] **M-generics-library step 3** | **Two defects the map shipped with, and neither golden could see them.** `n = m` then `m["b"] @ 2` printed `2 2 2` where the spec requires `1 2 -1` — exit 0, ASan clean, leak counter zero. And `m["a"] @ "x" + "y"` leaked one block. Task: say why `run/maps.hero` passed through both, then say what one line in that file would have to change to catch each

    **Where to look:** tests/golden/run/fixedbugs-map-store-aliased.hero · fixedbugs-map-store-leaked.hero · runtime/runtime.c (hero_map_set)
    **Why it matters:** the cheaper spelling of a container test — int values, never aliased — passes while testing neither rule

- [ ] **M-generics-library step 3** | The aliasing defect was found by **rewriting a stale comment**, not by a test: the header still said "READ-ONLY at M-optional-map … there is no `hero_map_unshare` here", which was true one milestone after it stopped being safe. Question: which instrument in this project *should* have caught it, and why did none — given that panel 022 had already measured this exact failure for arrays

    **Where to look:** docs/panel/022 R2 · runtime/heroes_runtime.h (the map paragraph)
    **Why it matters:** a comment that describes the past is a claim nobody re-checks

- [ ] **M-generics-library step 3** | The value is MOVED into a map and the key is COPIED, in the same call. Task: say which one `own.rs` increfed and why, then say what `hero_array_set`'s comment means by "the caller increfed before this call, which is what lets the value be something that lived inside the container the unshare just copied"

    **Where to look:** archive/bootstrap-rs/heroes/src/own.rs (the indexed arm) · runtime/runtime.c (hero_map_set)
    **Why it matters:** two arguments to one function with opposite ownership rules, and the asymmetry is the correct one

- [ ] **M-generics-library step 3** | **A row retiring rewrites its own tests, and that is the design.** Three unit tests used `sort` and `join` to demonstrate "not emitted"; both acquired entry points, so all three went red. Count: how many golden and unit cases changed in this step *because a refusal became a capability* rather than because behaviour changed?

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/tests/gate.rs · tests/golden/unsupported/three-capabilities.hero (rewritten a seventh time)
    **Why it matters:** "a row nobody can make fire is a row nobody can retire" — the cost of that rule, paid

- [ ] **panel 028** | **Three sentences of the spec are jointly impossible.** `range(a, b)` positional · two same-typed parameters mean mandatory labels · `range` is written in Heroes. Task: say which of the three you would have dropped, then read why each of the other two exits was refused — and say what made this invisible for eleven weeks

    **Where to look:** spec lines 81, 113, 152 · docs/panel/028 § The finding
    **Why it matters:** nothing in the project checks the spec against itself, and the compiler could not, because `range` had no implementation to disagree with

- [ ] **panel 028** | **Five of six off-by-one directions are silent**, and the sixth is loud only by accident — it survives because the loop bound is `len` of the thing being indexed, and the hedged form `range(0, len(s) - 1)` loses even that. Task: write the sixth case both ways and say which instrument in this project fires on each

    **Where to look:** docs/panel/028 R2
    **Why it matters:** the language's central claim, tested on the most common loop in programming

- [ ] **panel 028** | **`weak` linkage, refuted by compiling it.** Two translation units, one mangled symbol, different bodies: zero diagnostics under `-Weverything`, exit 0, and one module printed `4` where its own source says `6`. Question: which existing hazard in this project is that the same shape as, and why does `_Static_assert(HERO_RUNTIME_ABI)` not catch it?

    **Where to look:** docs/panel/028 R3b · archive/bootstrap-rs/heroes-cli/src/commands/toolchain.rs
    **Why it matters:** it is the build's documented ghost promoted to a language mechanism

- [ ] **panel 028** | Two judges reached opposite conclusions about the same sentence from different inputs — the warden measured that the tier phrase buys the *compiler* nothing, the ergonomist reported that it buys the *reader* three things. Task: say which object each was measuring, and why the disagreement is the panel design working rather than failing

    **Where to look:** docs/panel/028 § Where they disagreed
    **Why it matters:** the withdrawal condition one judge wrote was met by the other judge's transcript

- [ ] **M-generics-library step 4** | **The library is APPENDED, never prepended, and one line of arithmetic is the whole reason.** Every span is a byte offset into one text and every message renders a line number from it. Question: what would prepending have done to the line number in every diagnostic in every program — and what does appending cost instead, at the one boundary where it shows?

    **Where to look:** archive/bootstrap-rs/heroes/src/library/mod.rs · archive/bootstrap-rs/heroes/src/source/mod.rs (`with_library`)
    **Why it matters:** the cheap choice and the correct one differ by which end

- [ ] **M-generics-library step 4** | `library_at` was `text.len()` for a file with no library, and every end-of-file diagnostic moved a line. Question: which span starts exactly at `text.len()`, and why did the sentinel have to become `u32::MAX` rather than the boundary being made exclusive?

    **Where to look:** archive/bootstrap-rs/heroes/src/source/mod.rs (`Source::new`) · the test `an_empty_variant_says_what_is_missing`
    **Why it matters:** an off-by-one in a sentinel, caught by a test about something else entirely

- [ ] **M-generics-library step 4** | **Three places hand a program back and all three had to learn about the library**: `check --apply`, `fmt`, and `fmt --in-place`. Count: what would each have written into the author's file, once per invocation, without `user_text()`? Then say why the *comments* needed filtering separately from the declarations

    **Where to look:** archive/bootstrap-rs/heroes-cli/src/commands/check.rs · archive/bootstrap-rs/heroes/src/printer/fmt.rs
    **Why it matters:** a formatter that appends a library to the file it formats is a fixpoint that never closes

- [ ] **M-generics-library step 4** | The dumps show the user's program and the emitted C shows the library. Task: say why that asymmetry is right, using CLAUDE.md §10's own sentence about what a `--dump-<stage>` answers — then say what `--emit-c` would look like with seven library functions and no reachability walk

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/builtins.rs (`reachable`) · archive/bootstrap-rs/heroes/src/ir/print.rs
    **Why it matters:** one artifact answers a question about the file, the other has to build a binary

- [ ] **M-generics-library step 4** | **`mutate` moved from 96%/79% to 99%/82% in one step**, and the step added no diagnostic. Question: which mutation operator got easier to catch when seven call sites gained labels, and what does that say about §4.11's rule as a *measurement* rather than as a design preference?

    **Where to look:** harness/mutations/operators.md · docs/measurements/
    **Why it matters:** the label rule was priced as a cost to the writer and it reads here as a gain

- [ ] **M-generics-library step 4** | A parse error at the end of a file used to point at EOF; with the library appended that became "the library's first line", and the pipeline refused the whole compilation as a compiler bug. Task: read `Cursor::here_or` and say why the fallback is the declaration's keyword rather than the previous token

    **Where to look:** archive/bootstrap-rs/heroes/src/syntax/cursor.rs · tests/golden/check/missing-body.hero
    **Why it matters:** the guard was right to fire and the diagnostic was wrong to be there

- [ ] **panel 029** | **Four well-typed programs, one source text, four outputs, zero diagnostics.** `words.fold("", cat)` where `cat` takes two `str`. Task: say why `types/builtins.rs:263`'s rule cannot tell the two orders apart, then say why the language's own defence against argument-order mistakes — mandatory labels on same-typed parameters — cannot fire here

    **Where to look:** archive/bootstrap-rs/heroes/src/types/builtins.rs (the `fold` rule) · docs/panel/029
    **Why it matters:** the protection is switched off exactly where the hazard is, and the reason is structural rather than an oversight

- [ ] **panel 029** | **A live defect, and the generated C said so itself.** `xs: [()] @ []` checks clean, builds, and aborts with `entered unreachable code — this is a compiler bug`; the emitted C reads `hero_unreachable(); /* not an array */`. Task: find the one line of `gate.rs` that lets it through, and say why `[ptr]` needs a *typed* annotation today but will be *inferred* after step 6

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/gate.rs:146 · docs/panel/029 R4b
    **Why it matters:** a comment in generated C nobody reads is a diagnostic nobody receives

- [ ] **panel 029** | A wrong descriptor in an instantiation is invisible to **every** instrument this project has: clang, ASan, UBSan and the leak counter all pass, and `[-0.0] == [0.0]` prints `false`. Question: why is the memory well-formed, and which two of the five descriptor members are the only ones that differ between `int` and `f64`?

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/descriptors.rs · docs/panel/029 R4c
    **Why it matters:** the one place where "it runs clean" carries no information at all

- [ ] **panel 029** | A readable mangled suffix is **not injective**: `pair<int, str_x>` and `pair<int_str, x>` both spell `h_m_pair_int_str_x`. Task: construct the third pair of Heroes declarations that collides, then say what `mangle.rs`'s existing injectivity argument for `module_of` has to do with it

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/mangle.rs · docs/panel/029 R5
    **Why it matters:** the readable name was the obvious choice and it rejects a legal program

- [ ] **panel 029** | **Two judges scored their own predictions honestly and it is worth reading how.** The warden refused a removal it had measured (−15), because panel 013 had bought that form as a condition of approval; and it scored its own budget prediction refuted without an escape hatch — *"exempting repairs of pre-existing defects gives every overrun an escape hatch"*. Task: say what the alternative ruling would have cost the project

    **Where to look:** docs/panel/029 § Two acts of judicial integrity
    **Why it matters:** the panel's value is exactly its willingness to score itself

- [ ] **M-generics-library step 6** | **My own first defect in the pass, and the verifier caught it.** A generic call *inside* a generic body records the template's own parameter — `wrap(x)` in `depth<T>` records `T`, not `str` — so instantiating from it verbatim carried `Ty::Generic` into the copy. The message was `an instruction still has a type parameter in it`. Task: say why the fix is `apply(callee_args, caller_args)` and not "collect from the template instead of the instance"

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/mono.rs (`calls_in`) · archive/bootstrap-rs/heroes/src/ir/phases.rs
    **Why it matters:** the check was written before the pass and fired on its first run, which is the only reason this was five minutes rather than a milestone

- [ ] **M-generics-library step 6** | Count: for `first<T>` used at `[str]`, `[int]` and `[[str]]`, how many C functions does the emitter write, and how many does `first([10, 20])` followed by `first([30, 40])` add? Then say what the four-hex-digit suffix is a hash *of*, and why not of the `TyId`

    **Where to look:** tests/golden/run/generics.hero · archive/bootstrap-rs/heroes/src/emit/mangle.rs (`instance`)
    **Why it matters:** the answer to the second is a fixpoint argument, not a taste one

- [ ] **M-generics-library step 6** | `Checked::counted` is dense over the interner and its own comment says nothing may intern after it. Task: say what `[str]` interned by the pass would have read as without `extend_counted`, and why one instantiation is not enough to test it — `run/generics.hero` uses two on purpose

    **Where to look:** archive/bootstrap-rs/heroes/src/types/counted.rs · archive/bootstrap-rs/heroes/src/types/mod.rs
    **Why it matters:** "a leak that no test can see", in the comment's own words, and the test that sees it

- [ ] **M-generics-library step 6** | `heroes check` exits 0 on a program with polymorphic recursion and `heroes build` exits 1. Question: is that split right, and what would it cost to close it? (The answer is the same shape as every gate refusal, which is why it was not closed here)

    **Where to look:** tests/golden/unsupported/polymorphic-recursion.hero
    **Why it matters:** the first ERROR-kind case in a directory named for `unsupported`

- [ ] **M-generics-library close** | **Walkthrough offer** (optional, author's call): one `map(xs, show)` from source to C — `types/calls.rs` recording the instantiation, `ir/mono.rs` cloning and substituting, `counted.rs` rebuilt, the mangled hash, and the library function it lands in

    **Where to look:** docs/journal/010 · archive/bootstrap-rs/heroes/src/ir/mono.rs
    **Why it matters:** it is the one path where four passes and a hash all have to agree on one name

- [ ] **M-generics-library close** | **Golden ratification offer**: the milestone's adversarial cases — `run/generics.hero` (one generic at `str` and at `int`, the case that proves `counted` was rebuilt), `run/closure-list.hero`, `unsupported/polymorphic-recursion.hero`, `run/abort-slice-splits-a-character.hero`, `check/fixedbugs-builtin-as-value.hero`

    **Where to look:** tests/golden/
    **Why it matters:** each was written to fail before it was written to pass

- [ ] **M-generics-library close** | **Mutation drill offer**: delete the `extend_counted` call in `ir/mono.rs`, or the origin guard in `scope.rs`, and predict which instrument fires — the leak counter, ASan, clang, the phase check, or nothing

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/mono.rs · archive/bootstrap-rs/heroes/src/resolve/scope.rs
    **Why it matters:** the milestone's own record says the answer differs per line

- [ ] **M-generics-library close** | **Exit-quiz offer**: six live defects, five reachable for milestones. For each, say which instrument *could* have caught it earlier and why none did — and which two were found by a judge sent to price something else

    **Where to look:** docs/journal/010 § What broke and why
    **Why it matters:** the pattern is that they were all reachable and none had a program that met them

- [ ] **panel 030** | The spec invites a hallucination class **today**: `extern function read_file(path: str) -> str` is well-formed Heroes, and its near-misses (`puts`, `system`, `getenv`, `atoi` taking a Heroes `str`) are real symbols that link, run, and are silently wrong. Question for M-ffi-ladder: does `str` become forbidden as an `extern` parameter type, and what does that cost?

    **Where to look:** docs/panel/030 § Q3 · design.md §4.19
    **Why it matters:** the FFI's silent-wrong-answer class, reachable before the FFI milestone exists

- [ ] **panel 031** | **The proposal's own example taught an illegal program, for the second time in this record.** `geom.dist2(a, b)` cannot be a qualified call — `dist2(a: Point, b: Point)` has two same-typed parameters, so labels are mandatory — and it *is* legal as UFCS, meaning `dist2(geom, a, b)`. Task: say why the judge that found it wrote the correct form in its own program while predicting ≥30% of readers would copy the wrong one, and what that says about where examples sit relative to rules

    **Where to look:** docs/panel/031 § The finding that decided the wording · spec line 81
    **Why it matters:** panel 023 was found the same way, and the method is writing from the document rather than reading it

- [ ] **panel 031** | **Zero tokens bought two rules.** `use geom` *binds* `geom`: "shadowing is a compile error" and "an unused binding is a compile error" then answer the module-name collision and the unused `use` without another sentence. Question: which other word could have been chosen, and what would each have failed to cover?

    **Where to look:** docs/panel/031 R3 · spec lines 66, 74, 76
    **Why it matters:** the cheapest amendment in the project's record is a verb

- [ ] **panel 031** | Count: **39 `Span {` sites and 271 `&Source` parameters**, of which concatenation changes 0 and a file id inside `Span` changes 39. Task: say why `Span` staying 8 bytes and `Copy` is a Cyclone-rule question and not a performance one, then say what the appended library at M-strings-ownership has to do with N modules at M-module-namespace

    **Where to look:** archive/bootstrap-rs/heroes/src/source/mod.rs · CLAUDE.md §5
    **Why it matters:** the special case built for one turned out to be the general case

- [ ] **panel 031** | **Two ways to break §4.19, both of them optimisations this emitter already performs.** Deduplicating extern prototypes by C name: exit 0, calling through a wrong signature, printing garbage bytes. Pruning an unused module's `#include`: exit 0 printing `6714990092` where the unpruned form is two clang errors. Question: why is one whole-program `.c` enough for §4.19 today, and which of M-separate-compilation's four acceptance rows is the one this measures early?

    **Where to look:** docs/panel/031 R9 · docs/panel/030 R2
    **Why it matters:** the guarantee is the TU's contents, and both repairs are refusals to be clever

- [ ] **panel 031** | **A comment in the compiler is false and the milestone would have inherited it.** `mangle.rs` says "at M-module-namespace a module is a declared name rather than a file stem, so the shape stops being a stem-sanitising question at all" — but `module_of` strips `_`, so modules `geo_m` and `geom` are one component, and clang answers `redefinition of 'h_geom_Point'` on a legal program, which CLAUDE.md §7 makes exit 2. Task: say why the fix is a Heroes diagnostic naming both files rather than a ban on `_` in a module name

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/mangle.rs · docs/panel/031 R10
    **Why it matters:** five of this compiler's own file names carry `_`

- [ ] **panel 031** | **The historian and the ergonomist wanted opposite rules for one collision, and the tie-break was a measured confidence.** A local named after a module: D's two-phase rule (innermost scope wins) against a shadowing error. The ergonomist's confidence in the *silent* reading rose from 50% to 70% under the quoted-path variant. Question: state the project rule that decides this class, and say what it costs when the loud rule is the wrong one

    **Where to look:** docs/panel/031 § The disagreements · CLAUDE.md §1
    **Why it matters:** precedent said nothing, so the tie-break had to come from an experiment

- [ ] **M-module-namespace step 1** | **The spec-against-the-compiler test fired before any golden did.** Amending spec line 6 turned `measure::spec::the_spec_never_uses_a_word_the_compiler_rejects` red with `left: ["use"]`, because `use` was still in the lexer's foreign-word table. Question: which of the two tables in `lexer/keywords.rs` a word lives in decides *what kind of message* it gets — and why did moving one word between them cost exactly one line in each?

    **Where to look:** archive/bootstrap-rs/heroes/src/measure/spec.rs · archive/bootstrap-rs/heroes/src/lexer/keywords.rs
    **Why it matters:** the spec is tested against the compiler, and here it was the spec that moved first

- [ ] **M-module-namespace step 1** | Count: three shapes are refused where a module name belongs — `use "geom"`, `use geom.shapes`, `use shapes/geom`. Exactly one carries a machine-applicable fix. Task: say which, then state the rule that decides it, using `.fixed`'s own contract (CI applies every certain fix and asserts the result checks clean)

    **Where to look:** archive/bootstrap-rs/heroes/src/syntax/decl.rs (`use_decl`, `is_module_name`) · tests/golden/check/use-is-quoted.fixed (2026-08-26)
    **Why it matters:** a `Certain` fix that leaves a second syntax error is worse than no fix (the fixture as it stood 2026-08-26)

- [ ] **M-module-namespace step 1** | **`use` is not in panel 007's ender list, and one recovery path had to learn it.** A bare `use` plants no terminator, so `recover_to_next_decl` ate the whole `function main()` below it — the failure panel 018's keyword-first shape was bought to prevent. Question: why was the repair a line-number comparison in `use_decl` rather than adding `KwUse` to `is_line_ender`, and what would the second have changed about the *language*

    **Where to look:** archive/bootstrap-rs/heroes/src/syntax/decl.rs · archive/bootstrap-rs/heroes/src/lexer/layout.rs (`is_line_ender`) · archive/bootstrap-rs/heroes/src/syntax/recover.rs
    **Why it matters:** the layout rule is a panel path and the parser's recovery is not

- [ ] **M-module-namespace step 1** | **A formatter test found a real defect on its first run**: the blank line between a file's header comment and the first `use` was being deleted, which under §4.1 turns a remark about the file into documentation for the line below. Task: say why the `Decl` arm already had the guard and the `Use` arm did not, then say why the same guard must NOT fire between two `use` lines

    **Where to look:** archive/bootstrap-rs/heroes/src/printer/fmt.rs · archive/bootstrap-rs/heroes/src/printer/tests/modules.rs
    **Why it matters:** adjacency is meaning in this language, so a blank line is not whitespace

- [ ] **M-module-namespace step 2** | **The architecture M-strings-ownership built for one special case turned out to be the general one.** `Source` went from "one text plus a library boundary" to "N files plus a table", and the panel's number is why: 39 `Span {` construction sites and 271 `&Source` parameters, of which concatenation changes **zero**. Task: say what a file id inside `Span` would have cost at each of those two counts, and why `Span::to` is the specific function that makes it worse than arithmetic

    **Where to look:** archive/bootstrap-rs/heroes/src/source/mod.rs · docs/panel/031 R7
    **Why it matters:** the cheap change and the correct change were the same one, and it was measured rather than argued

- [ ] **M-module-namespace step 2** | **Two lexer tests failed on a newline nobody typed.** Normalising every file to end in `\n` moved EOF from `1:6` to `2:1` on a file written without a trailing newline. Question: why does the separator have to be closed *before* the next file rather than after the previous one, and which file in a compilation is the only one that may keep exactly what was written?

    **Where to look:** archive/bootstrap-rs/heroes/src/source/mod.rs (`Source::of`) · archive/bootstrap-rs/heroes/src/lexer/tests/end_to_end.rs
    **Why it matters:** the fix is one line moved, and the test that caught it is about a language with no modules

- [ ] **M-module-namespace step 2** | `module_of` moved from `emit/mangle.rs` to `source/files.rs`. Task: state the layering argument in one sentence, then say what `mangle.rs`'s module doc claimed about M-module-namespace that panel 031 measured false — and why the repair is a diagnostic over a *set* of modules rather than a stricter `module_of`

    **Where to look:** archive/bootstrap-rs/heroes/src/source/files.rs · archive/bootstrap-rs/heroes/src/emit/mangle.rs
    **Why it matters:** a comment that promises a future milestone will fix something is a claim, and this one was wrong

- [ ] **M-module-namespace step 3** | **A diagnostic in a non-root module named the root file and a line nobody could find in it.** `render_line` and `render` each printed `src.name` and `line_col` directly — the same answer while there was one file, a *false* one the moment there were several. Task: say why `render` needs **two** line numbers now, which one is printed and which one slices the text, and why the repair was one function on `Source` rather than two fixed call sites

    **Where to look:** archive/bootstrap-rs/heroes/src/source/mod.rs (`locate`) · archive/bootstrap-rs/heroes/src/diagnostics/render.rs
    **Why it matters:** well-formed and wrong is the failure mode a green suite cannot see

- [ ] **M-module-namespace step 3** | **Discovery re-uses the real lexer rather than a prefix scanner** (panel 031 R8), and one test is the whole argument: `# use geom` and `print("use geom")` name no module. Question: what would the spec rule the compiler-engineer asked for have bought, what would it have cost, and which CLAUDE.md section forbids the reason it would have been bought for?

    **Where to look:** archive/bootstrap-rs/heroes/src/modules/mod.rs (`uses_of`) · docs/panel/031 R8
    **Why it matters:** a second grammar for the same text is two grammars that can disagree

- [ ] **M-module-namespace step 3** | **Discovery diagnoses nothing.** A missing module and a cycle are both reported by `graph.rs` against the *finished* `Source`. Task: say why — in terms of what a `Span` may point into — and then say what makes discovery terminate on a cycle given that it is not the cycle check

    **Where to look:** archive/bootstrap-rs/heroes/src/modules/mod.rs · archive/bootstrap-rs/heroes/src/modules/graph.rs
    **Why it matters:** the walk that terminates and the check that reports are two different things wearing one name

- [ ] **M-module-namespace step 3** | `is_library` split into two questions: **`is_root`** for the dumps and `fmt`, `is_library` for emission and `heroes test`. Task: say what `heroes test` would silently lose if it had taken `is_root`, and quote the CLAUDE.md sentence that decides which of the two each caller wants

    **Where to look:** archive/bootstrap-rs/heroes/src/source/mod.rs (`is_root`) · archive/bootstrap-rs/heroes/src/emit/decls.rs
    **Why it matters:** one predicate had been answering two questions since M-strings-ownership, and only one file made the difference visible

- [ ] **M-module-namespace step 4** | **The compiler-engineer's M-module-namespace prediction is CONFIRMED, and it was specific**: "both `resolve/exprs.rs` (279 today) and `resolve/errors.rs` (286 today) cross 300, forcing at least one split under CLAUDE.md §11". They reached 368 and 418. Task: say what the judge could see in the tree that made the number predictable, then score the other half of its prediction (non-test lines above 23,700) when the milestone closes

    **Where to look:** docs/panel/031 § Predictions to score · archive/bootstrap-rs/heroes/src/resolve/
    **Why it matters:** a panel prediction that lands on a line count is the cheapest kind to score and the rarest to make

- [ ] **M-module-namespace step 4** | **One `top_in` instead of a flat `get` is where "always qualified" lives.** The top-level table's key went from `name` to `(module, name)`. Question: why does an unqualified name fall back to the *library* module and to no other, and what would break if it fell back to every module instead?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/mod.rs (`top_visible`) · docs/panel/031 R5
    **Why it matters:** the library is the one module every file sees without naming it, and that is not an exception to the rule

- [ ] **M-module-namespace step 4** | `Ref::Module` is recorded on the **receiver**, not on the call. Task: say why the check has to happen before the receiver is synthesised, then find the two later passes that read it and say what each would have done with `geom.dist2(a, b)` without it — one of them produced `hero_unreachable(t1, t2)` in real C during this step

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/qualified.rs · archive/bootstrap-rs/heroes/src/types/calls.rs · archive/bootstrap-rs/heroes/src/ir/calls.rs
    **Why it matters:** three tokens with two meanings, decided once because Part 5 erases one of them

- [ ] **M-module-namespace step 4** | **`geom.Point(x: 3, y: 4)` is a construction, not a call**, and routing it through the call path produced C that asked clang to call a record. Question: which existing function in `types/calls.rs` already made that split for unqualified names, and why did the qualified branch have to repeat it rather than reuse the dispatch?

    **Where to look:** archive/bootstrap-rs/heroes/src/types/calls.rs · archive/bootstrap-rs/heroes/src/ir/calls.rs
    **Why it matters:** the qualified form is an ordinary name that says where it lives, so it needs every branch the ordinary one has

- [ ] **M-module-namespace step 4** | Four rules arrived with **no new specification**: an unused `use`, a `use` colliding with a declaration, a module in value position, and one module named twice. Task: for each, name the spec line that already covered it, and say which of the four is the one that needed a *new* message anyway

    **Where to look:** spec lines 74 and 76 · archive/bootstrap-rs/heroes/src/resolve/errors_modules.rs
    **Why it matters:** "binds" was chosen as the verb precisely to make three of these free

- [ ] **M-module-namespace step 5** | **`heroes mutate` has been measuring a compiler nobody runs, since M-generics-library.** The CLI attaches the library (`input::read`); `mutate::fate` built a bare `Source::new`. Making them the same pipeline moved the score from 97%/81% to **96%/79%**. Task: say which resolution path differs when the library is present — the hint is that `xs.map(f)` reaches `Ref::Top` in one and `Ref::Builtin` in the other — and then say why the LOWER number is the one to publish

    **Where to look:** archive/bootstrap-rs/heroes/src/mutate/mod.rs · archive/bootstrap-rs/heroes-cli/src/input.rs
    **Why it matters:** the control arm and the real arm have to be the same compiler, or the metric measures the harness

- [ ] **M-module-namespace step 5** | **Two modules, two `Point`s, two C structs.** `h_two_Point` and `h_geom_Point` in one translation unit, from `record Point` in each file. Question: what did the emitter take the module from before this step, and construct the two-module program that would have produced `error: redefinition` — then say why no existing golden caught it

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/ctype.rs · archive/bootstrap-rs/heroes/src/emit/decls.rs
    **Why it matters:** every single-file program has one module and it is the root's, so the whole corpus was blind to this by construction

- [ ] **M-module-namespace step 5** | The emitter's `module: &str` parameter was threaded through eight functions and is now derived at each site from the declaration's own span. Task: name the two things the ROOT module still names, and say why neither of them is a declaration

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/mod.rs · archive/bootstrap-rs/heroes/src/emit/writer.rs
    **Why it matters:** a parameter that is always the same value is a parameter that is about to be wrong

- [ ] **panel 032** | **The only silent wrong program in this project's panel record.** Under "a directory is a module", `use syntax` means one thing while `syntax.hero` is absent and another once it exists — so *adding a file* rebinds every call in every importer, and both versions type-check. Task: say why the ergonomist could build it from the wording alone, and which earlier panel's falsified form the wording repeats

    **Where to look:** docs/panel/032 § C is dead · DESIGN-LOG 2026-08-10 (panel 023)
    **Why it matters:** a rule in a subordinate clause is the highest-risk text in the document, and this is the second time

- [ ] **panel 032** | **Two namespaces, one check, written over the wrong one.** What you type before the dot and what reaches the linker have different collision sets: last-part components collide 22-way on `mod` over the port's 169 paths, whole-path concatenation collides zero times. Question: which of the two does `module_names_collide` compare today, and construct the legal program it lets through

    **Where to look:** archive/bootstrap-rs/heroes/src/modules/graph.rs · archive/bootstrap-rs/heroes/src/source/files.rs
    **Why it matters:** the answer is a program that exits 2 saying the compiler is wrong

- [ ] **panel 032** | Four seats vetoed C from four inputs and none was asked about the others' ground: a silent rebind, a header set chosen by directory membership, a deleted "always qualified", and TypeScript's four-year retreat from the same model. Task: say what that convergence is worth as evidence, and what it would have taken for a single seat's veto to carry the same weight

    **Where to look:** docs/panel/032 § The verdict table
    **Why it matters:** the panel's design is differentiated inputs, and this is the case that shows what they buy

- [ ] **M-module-namespace defects** | **Two namespaces wearing one name, and the emitter was reading the wrong one.** `FileEntry.module` is what the author types before the dot; `FileEntry.component` is what reaches the linker. Until panel 032 the emitter took the first. Task: say why `module_of` had *always* sanitised and the bug still existed, and why panel 031 R10's diagnostic — written for this exact pair of names — did not fire

    **Where to look:** archive/bootstrap-rs/heroes/src/source/files.rs · archive/bootstrap-rs/heroes/src/modules/tests.rs (`fixedbugs_two_modules_whose_concatenations_collide…`)
    **Why it matters:** the check was correct and was comparing the wrong two strings

- [ ] **M-module-namespace defects** | **`at_source` and `at_library` were the right split for two files and the wrong one for four.** In the split calculator the emitted `#line` reached 410 while `main.hero` is 78 lines. Task: say what M-ffi-ladder loses when §4.19's guarantee is delivered to a file that does not contain the declaration, then count how many callers `Source::locate` has now and what each was doing before it existed

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/writer.rs · archive/bootstrap-rs/heroes/src/source/mod.rs
    **Why it matters:** the third caller in one milestone to need the same three values, and the first two were found by running rather than by testing

- [ ] **M-module-namespace defects** | **The deletion panel 030 promised and panel 031 withdrew happened anyway, for a different reason.** `writer::LIBRARY_FILE` and `Writer.source` became dead when the two `#line` entry points collapsed into one. Question: what does that say about *why* the library stopped being a special case — the file table, or the rule?

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/writer.rs · docs/panel/031 § The disagreements
    **Why it matters:** a prediction can be right about the outcome and wrong about the mechanism

- [ ] **M-module-namespace close** | **Walkthrough offer** (optional, author's call): one `geom.dist2(a: p, b: q)` from source to C — the parser building a `Method` node, `resolve/qualified.rs` recording `Ref::Module` on the *receiver*, the checker refusing to prepend it, the IR lowering it as a plain call, and `h_geom_dist2` coming out of a component the file table owns

    **Where to look:** docs/journal/011 · archive/bootstrap-rs/heroes/src/resolve/qualified.rs
    **Why it matters:** it is the one path where the same three tokens have two meanings and four passes have to agree which

- [ ] **M-module-namespace close** | **Golden ratification offer**: the milestone's adversarial cases — `check/use-is-quoted.hero`, `check/use-has-a-path.hero`, and the two `fixedbugs` cases (`modules::tests::fixedbugs_two_modules_whose_concatenations_collide…`, `surface::fixedbugs_every_module_names_itself_in_the_emitted_c`)

    **Where to look:** tests/golden/check/ · archive/bootstrap-rs/heroes/src/modules/tests.rs
    **Why it matters:** the two fixedbugs cases were written from a judge's compiled evidence rather than from a hypothesis

- [ ] **M-module-namespace close** | **Mutation drill offer**: delete `FileEntry.component` and mangle with `module` again, or make `at_span` use `line_col`, and predict which instrument fires — clang, the leak counter, a golden, a surface test, or nothing. The milestone's own record says the answer differs per line, and for one of the two the answer was *nothing* for a whole day

    **Where to look:** archive/bootstrap-rs/heroes/src/source/files.rs · archive/bootstrap-rs/heroes/src/emit/writer.rs
    **Why it matters:** both were live in shipped code and neither had a test until a judge compiled them

- [ ] **M-module-namespace close** | **Exit-quiz offer**: five defects. For each, say which instrument *could* have caught it earlier and why none did — and which three were found by running rather than by testing

    **Where to look:** docs/journal/011 § What broke and why
    **Why it matters:** the pattern this time is that the tests were right and were asking about one file

- [ ] **panel 033** | The direction disagreement is unresolved **by design**: the historian has the whole ancestry for default-private (Wirth's Modula-2 → Oberon arc, Oberon/Nim/Go/Zig/Erlang, Rust RFC 0001 and Swift SE-0117 both reversing *towards* it, none reversing away), and three judges have the counts against it (256 `private` vs 355 `export`; a binding module is 100% public by construction — 20/20 libm lines, 287/287 SQLite). Question: which of those two is evidence about *Heroes*, and which is evidence about languages that had ecosystems?

    **Where to look:** docs/panel/033-visibility.md § The direction split
    **Why it matters:** this is the first panel where ancestry and measurement point opposite ways and neither is wrong

- [ ] **panel 033** | Drill: the ergonomist's predicted v-2 failure is **"export the verb, forget the noun"** — you export `tokenize` because it is what you call, and forget `Token` because the other file only *names* it. Say which pass would report it, what the message must contain to be fixable without opening a second file (CLAUDE.md §8), and why the same slip cannot happen under v-1

    **Where to look:** docs/panel/033-visibility.md § The direction split
    **Why it matters:** it is the one mistake in the session that a judge made for real before catching it

- [ ] **panel 034** | **The experiment R4 owes, at M-program-corpus**: convert `examples/`'s error codes to `constant`s and re-run `heroes mutate`. The ergonomist predicts codes-mismatch ≤1/20 and false positives 0/20. Question before running it: what does `typo-code` even *mean* once there are no literals left at the comparison — does its row go to zero sites, and is that a pass or a hole in the instrument?

    **Where to look:** docs/measurements/004-error-codes.md · harness/mutations/operators.md
    **Why it matters:** an instrument that reports success because it lost its sites is the failure mode measurement 004 was written to avoid

- [ ] **panel 034** | Drill: the diagnostic is **root-dependent** — the same file errors rooted at `lex` and is clean rooted at `main`, because `modules::load` loads the root plus what it names. Say which other compiler answers already depend on the root, and which of them are defensible. One of them was a shipped miscompilation until this session

    **Where to look:** docs/panel/034 § Part 3 · archive/bootstrap-rs/heroes/src/modules/mod.rs
    **Why it matters:** root-dependence is not hypothetical in this compiler and the panel found it twice in one day

- [ ] **panel 034** | Drill: why can the proposed diagnostic not protect the **self-hosted** compiler? The answer is in `archive/bootstrap-rs/heroes/src/diagnostics/mod.rs:74` and §1.0's error-accumulation idiom, and it says something about which of this project's instruments measure `examples/` rather than the thing being built

    **Where to look:** docs/panel/034 § Part 3 · design.md §1.0
    **Why it matters:** the harness protects the corpus, and the corpus is not the compiler

- [ ] **sweep 001** | **Twenty defects, three shapes, none found by reading** — the recursive hunt the author asked for after two panels in a row found a defect in code they were only skimming. Seven fixed, thirteen open with reproducers. Question worth answering before the fixes land: which of the three shapes could have been caught by an instrument that already exists, and which needed one nobody had built?

    **Where to look:** defect 001 (docs/work/DONE.md)-the-post-m8a-sweep.md
    **Why it matters:** the pattern is the finding; the individual defects are its evidence

- [ ] **sweep 001** | Drill: `emit/descriptors.rs::generated()` answers "which types did the program mention inside a container" and its three callers ask "which types does the function I am about to write name". Say why that is **the same defect as D3** one level up, and what the general form of the question is

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/descriptors.rs:75 · defect 001 (docs/work/DONE.md) § Shape C
    **Why it matters:** one repair retires three of the nine emitter defects, and it is the repair D3 already got

- [ ] **sweep 001** | `Resolved::module_declaring` picks the **alphabetically first** module of N: it names one the file cannot see, attaches a fix that produces `wrong_arity` if followed, and cascades a false `unused_binding` telling the author to delete the `use` line that was the real fix. It had exactly one possible answer when there was one module. Question: how many other lookups in the resolver had one possible answer at M-ffi-ladder and now pick by sort order?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/mod.rs:211 · defect 001 (docs/work/DONE.md) N9
    **Why it matters:** the only defect in the sweep that is neither a widened scope nor a hand-built position

- [ ] **naming** | **Walkthrough offer: why renaming is not renumbering.** Panel 030 R7 refused renumbering on RFC/PEP/LLVM precedent, was ratified, and was then amended the same week to abolish numbers entirely. Reconstruct the distinction without rereading: what failure mode does R7's precedent actually protect against, why does a rename into a disjoint namespace not have it, and what would have to be true for the amendment to be wrong

    **Where to look:** docs/panel/030-the-build-order-revised.md § Amendment to R7 · CLAUDE.md §14
    **Why it matters:** the whole argument turns on *silent* versus *loud* failure, which is the same distinction §8 and the emit gate are built on

- [ ] **panel 036** | **The two silent routes, as a retrieval exercise.** Before reading the panel file: a decoy `sqlite3.h` sits beside the generated `.c` in `build/<hash>/`. Say what `#include "sqlite3.h"` does, what `-Weverything` says about it, and why `_Static_assert(HERO_RUNTIME_ABI == 10)` — which saved the *runtime* from exactly this in panel 020 — cannot be written for a foreign header. Then: which of these six wrong bindings compile clean under include-only emission, and which fire? (`-> f64` on `sqlite3_step` · `-> int` on `sqrt` · `(col: f64)` where the header says `int` · `ptr` where the header says `const char *` · `-> int` on `sqlite3_errmsg` · a typo'd name)

    **Where to look:** docs/panel/036 § Two silent-wrong-answer routes
    **Why it matters:** four of the six compile clean, and the answer is why §4.19's sentence needed a mechanism rather than a promise

- [ ] **panel 036** | Say what `_Generic((sqlite3_open((const char*)0,(void*)0)), int:1, default:0)` costs at runtime, and why the call inside it does not happen. The answer is one clause of C11 6.5.1.1p3 — find it before reading the panel

    **Where to look:** docs/panel/036 · C11 6.5.1.1p3
    **Why it matters:** it is the whole reason the mechanism is free, and a reader who assumes the call runs will refuse the design for the wrong reason

- [ ] **M-ffi-ladder** | **The ender list, reopened by a literal rather than by a rule.** `nullptr` was added to the language and missed in `is_line_ender`, so `p: ptr @ nullptr` planted no terminator and the *next* line was swallowed as a continuation — the error landed on an innocent statement, which is panel 007's own trap. Task: say why a value-producing keyword is the class that keeps being forgotten, and what `every_value_keyword_ends_a_line` can and cannot catch (it fires only for a spelling somebody remembered to add to *it*)

    **Where to look:** archive/bootstrap-rs/heroes/src/lexer/layout.rs · lexer/tests/adversarial.rs
    **Why it matters:** a list whose completeness is a premise, and the third time this project has paid for one

- [ ] **M-ffi-ladder** | **A stale comment found a live defect.** `emit/decls.rs` said "no other type crosses the boundary: `ffi_type` refuses them in the checker" — and nothing did. `extern function weird(n: int) -> [int]` reached clang as `call to undeclared function` plus `incompatible integer to pointer conversion`, exit 2, the compiler blaming itself. Task: say which half of that comment was true when it was written, and why the premise's death was silent

    **Where to look:** archive/bootstrap-rs/heroes/src/types/decls.rs (`ffi_signature`) · tests/golden/check/ffi-type.hero
    **Why it matters:** CLAUDE.md §11's own example, found by reading the comment rather than the code

- [ ] **M-ffi-ladder** | `s.cstr()` is an `Op::Cast`, not a call, and the IR has carried `CastKind::StrToCstr` with **no producer** since M-ir-lowering. Say why the conversion is a cast rather than a built-in call — the answer is one line of `ir/inst.rs`'s own table — and then say what makes the borrow free, and what makes it good only for the duration of the call

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/calls.rs · emit/inst.rs · design.md §4.20
    **Why it matters:** the highest-return decision in the string design, collected three milestones later

- [ ] **M-ffi-ladder** | **`_ = f(x)` emitted `-Wunused-but-set-variable` on every program that used it**, which is the ordinary way to ignore a C status code. The fix skips the temporary for a discarded **call** and for nothing else. Task: say what breaks if the same rule is applied to `_ = xs[9]` — the answer is why the restriction is not tidiness

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/inst.rs · emit/decls.rs (`discarded_call`)
    **Why it matters:** a pure op may vanish and an aborting one may not, and the difference is the whole of §4.3's abort rule

- [ ] **panel 036 / M-ffi-ladder** | **The spec-warden's prediction, met to the letter, in the milestone it was made in.** It said ≥30% of programs writing a file would fail first-try on `-> ()?` alone, because `ok(())` is a parse error, `ok()` is `wrong_arity` and falling off the end is `missing_return`. Measured before writing a line of the library: all three refuse, so **no function in the language could construct a `()?`**. Task: say why the arity of `ok` is now derived from the payload rather than fixed at 1, and what the alternative spellings would each have cost

    **Where to look:** archive/bootstrap-rs/heroes/src/types/construct.rs · docs/panel/036 § Predictions
    **Why it matters:** a hole in the type system that only appeared when a signature needed it, predicted by a judge who never wrote one

- [ ] **M-ffi-ladder / emit** | **Every translation unit now carries two option structs and eight per-type functions it may never use**, because `read_file -> str?` and `write_file -> ()?` are the first library functions with a `T?` in their signature and `Names::with_options` walks the interned arena. Deriving the set from the emitted functions was tried and **reverted**: a walk over slots, results and instruction types misses a `T?` reached through a declared record's field, and a missing name is a hard error found by the mutant corpus rather than by any case somebody wrote. The honest filter needs the declaration graph as well as the IR

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/ctype.rs (`with_options`) · emit/mod.rs
    **Why it matters:** the cost of putting `T?`-returning functions in the library, paid by every program in the language

- [ ] **M-ffi-ladder close** | **Score the remaining panel 036 predictions.** Already scored: the compiler-engineer's `decl.rs` split (correct, 479 lines), the spec-warden's exact token count (correct on its own draft), the spec-warden's `-> ()?` failure (correct, and no spelling worked at all), the ffi-pragmatist's `cstr`→`str` need (correct, `to_str` gained a row). Still open at M-program-corpus and the Part 11 harness: the four llm-ergonomist first-try rates, and the historian's `exit`/flow-analysis prediction — which did **not** fire, because `hero_exit` is `_Noreturn` and the Heroes wrapper never returns either

    **Where to look:** docs/panel/036 § Predictions to score
    **Why it matters:** five judges, five different failure surfaces, and the two that mattered were both found by compiling

- [ ] **author question 2026-08-12 / panel 018** | **Where did the `=` at top level go, and which rule pays for the indented value?** Asked while reading `examples/curl/main.hero`: why is a `constant`'s value on the next line instead of after an `=`. Before rereading: say which *other* declaration would have to change if `constant MAX: int = 64` were legal, name the rule that already gives the indented body its value (it is not a rule about constants), and say what a single-line form would cost a `constant` whose body is three statements. Then read panel 018's option B and the two seats that rejected it — one on a compiler count, one on a measured first-try rate

    **Where to look:** docs/panel/018-top-level-declaration-shape.md § The candidate + :81-82 · design.md §4.2:737-762, §4.7:1086 · Part 8 wart 2
    **Why it matters:** the answer is that it was never a decision about `constant`, and the three-lines-for-one-number cost is on the record as accepted — which is the shape of most "why is this strange" questions in this language

- [ ] **panel 039 / §4.12** | **The subtraction that is the only admissible argument about comptime.** §4.12 calls it "the most counter-intuitive finding of the review: generics make the compiler *smaller*" — seven container functions that would each have been a hand-written case in the type checker became 149 lines of Heroes in `library/source.hero`. The machinery that bought that is `ir/mono.rs` (393 lines) plus `types/generics.rs` (140). Before rereading: say which of the seven (`map`, `filter`, `fold`, `find`, `sort`, `sum`, `any`) would have been the most expensive as compiler magic and why; then say what the arithmetic looks like for a compile-time evaluator, whose analogues in this tree are `own.rs` (309 lines, 30 `Op::` sites) and `emit/inst.rs` (445, 32) — and which side of the subtraction it lands on

    **Where to look:** design.md §4.12:1421-1440 · archive/bootstrap-rs/heroes/src/ir/mono.rs · archive/bootstrap-rs/heroes/src/types/generics.rs · archive/bootstrap-rs/heroes/src/library/source.hero
    **Why it matters:** a feature is only cheap if it *deletes* something, and this is the one place in the record where that was measured rather than asserted

- [ ] **panel 039** | **Termination, and why this compiler refuses a knob two other languages ship.** `ir/mono.rs:24-33` refuses polymorphic recursion **structurally** rather than with a depth limit, citing MLton total for twenty-five years because SML bans it, undecidability (Henglein 1993; Kfoury–Tiuryn–Urzyczyn 1993), and Rust as the warning — "accepts at type-check and blows up at codegen… late and unattributable". Zig's comptime ships `@setEvalBranchQuota` over a default of 1000 backward branches; Nim's VM ships an iteration limit. Task: say what a structural argument buys that a quota cannot, and then say what `constant A: int` / `B` and `constant B: int` / `A` proves about whether this language already has the problem it refused

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/mono.rs:24-33 · docs/panel/039-comptime-and-part-6.md § Appended 2026-09-04
    **Why it matters:** the strongest argument in the comptime question is not a cost, it is a contradiction — and the contradiction is already half-present

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-ir-lowering close | Milestone debrief offers, all optional: walkthrough (one function from source to dump, following the four blocks of a `for` and the copy-out chain) · ratify the 5 adversarial IR goldens (continue-steps, try-copies-out, diverging-arms, nested-shortcircuit, for-evaluates-once) · mutation drill (change `continue_to: step` to `continue_to: test` in control.rs and predict which tests fail — the answer is interesting) · exit-quiz (hand-lower `for x in xs` or `e?` on paper, then diff against `--dump-ir`)

    **Where to look:** /learn
    **Why it matters:** the author's call, when and how much

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-syntax-tree close | Milestone debrief offers, all optional: walkthrough (tokens → tree → text, the two renderings and why both exist) · ratify the 5 adversarial cases (`fn` both positions, depth-zero continuation, braces as a block, signature without `:`, trailing comma) · mutation drill (break one line of `expr.rs`'s precedence table and predict which test fails) · exit-quiz (re-implement `wrapped` in fmt_expr.rs, or the three-line-shapes lookahead, on a throwaway branch)

    **Where to look:** /learn
    **Why it matters:** the author's call, when and how much

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-name-resolution close | Milestone debrief offers, all optional: walkthrough (`heroes check --dump-scopes` on the appendix: the alphabetical table, the nesting, the read/write counts) · ratify the 5 adversarial cases (one tier · the `@`-typo · the four unused shapes · the hole's reach · shadowing with sibling loops as the control) · mutation drill (make `report_unused` count writes as reads and predict which of the 164 tests fail) · exit-quiz (re-implement the `nearest` candidate rule, or the sequential shadow check, on a throwaway branch)

    **Where to look:** /learn
    **Why it matters:** the author's call, when and how much

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-typed-frontend close | Milestone debrief offers, all optional: walkthrough (a mismatch from `heroes check` end to end: which pass produced it, what `Want` was in play, why the message names the *other* end) · ratify the 5 adversarial cases (mixed arithmetic · non-exhaustive and the `_` ban · no implicit `T?` · swapped arguments · the jump-and-unit pair) · mutation drill (make `join.rs` treat a diverging branch as a value and predict which of the 259 tests fail) · exit-quiz (re-implement the `Want` enum's three cases, or the same-typed-argument rule, on a throwaway branch)

    **Where to look:** /learn
    **Why it matters:** the author's call, when and how much

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-scalars-run close | Milestone debrief offers, all optional: walkthrough (one function from `.hero` through the IR to the C to the binary, following the `#line` directives and the copy-out) · ratify the 5 adversarial `run/` cases (overflow-aborts, division-edges, short-circuit, copy-out-edges, join-slot) · mutation drill (delete the `preds.is_empty()` test in `emit/decls.rs::reachable` and predict which of the 55 goldens fail — the answer is interesting) · exit-quiz (hand-emit the C for a `while` loop, then diff against `--emit-c`)

    **Where to look:** /learn
    **Why it matters:** the author's call, when and how much

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-strings-ownership close | Milestone debrief offers, all optional: walkthrough (one `str` from literal to heap block to sweep, following `--dump-ir` then `--emit-c`) · ratify the 2 new adversarial cases (str-copy-out, str-self-assign) · mutation drill (swap the incref and decref around a store in `own.rs` and predict which of the 68 goldens fail — and which configuration catches it) · exit-quiz (hand-write the refcount instructions for `out @ out + to_str(i)` inside a loop)

    **Where to look:** /learn
    **Why it matters:** the author's call, when and how much

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-ffi-ladder close | Milestone debrief offers, all optional: **walkthrough** (one `extern` from the group's head line to the emitted `#include`, the `_Static_assert`, the unmangled call and the `-l` flag — four artifacts from one declaration) · **ratify the 5 adversarial cases** (`check/ffi-type` · `fixedbugs/ffi-return-type` · `run/ffi-libm` · `run/ffi-cstr` · `run/edges-file-args-exit`) · **mutation drill** (change `HERO_RET_INT`'s `+(c)` back to `(c)` and predict which of the two acceptance programs fails — the answer is the milestone's best finding) · **exit-quiz** (write the `extern` group for three `curl` functions from `curl.h` alone, then compile it)

    **Where to look:** /learn
    **Why it matters:** the author's call, when and how much

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-syntax-tree.1 | A `#` comment, then a blank line, then a declaration: does the comment become documentation — and which two comparisons decide it? **Answered in the code, so this is a reading and not a decision**: `take_docs` breaks on `line + 1 != wanted || col != decl_col`, so a blank line ends the run and the comment documents nothing

    **Where to look:** syntax/cursor.rs take_docs:206-232 (§4.1)
    **Why it matters:** Go's adjacency rule is two integer comparisons, not a parser mode

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | panel 029 | The ownership pass emitted `store total <- $t1` with no incref into a generic body, and `incref $t2` on the `[A]` beside it. Question: why did the array get counted and `B` not — and what would have happened at `B = str` if monomorphisation had run second? **Answered where it is load-bearing rather than in prose**: `Phase::Mono`'s doc comment says the order is forced, because `is_refcounted` answers `false` for `Ty::Generic` — right for `T = int`, a leak for `T = str` — so an ownership pass running first "does not decline to decide, it decides wrongly", and `released_on_return` cannot catch it because it asks the same predicate

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/mod.rs:205-213 (`Phase::Mono`) · types/counted.rs · docs/panel/029 R1
    **Why it matters:** "it cannot decide yet" and "it decided wrongly" call for the same fix and are not the same finding

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-literal-bases step 1 | Two decoders became one, and the milestone could not have landed otherwise. Open `archive/bootstrap-rs/heroes/src/types/exprs.rs:41` and `archive/bootstrap-rs/heroes/src/ir/exprs.rs:206` at the commit before `944ee94`, where each calls `parse::<i64>()` on the same slice. Question: with one base, what did drift between them cost — name the observable symptom. With four, what does it cost instead? Then say why the *diagnostic* had to move into `lexer/digits.rs` too, and not only the decoder

    **Where to look:** archive/bootstrap-rs/heroes/src/lexer/digits.rs · `git show 944ee94^:archive/bootstrap-rs/heroes/src/ir/exprs.rs` (2026-08-12, a revision rather than a file on disk)
    **Why it matters:** the answer is the difference between a duplicated sentence and a wrong number

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-literal-bases step 1 | Case is settled in two different places on purpose. `0X10` is a compile error; `0xFF` is not, and `fmt` rewrites it to `0xff`. Question: name the fact about C headers that makes refusing uppercase *digits* wrong, and the fact about the three prefixes that makes refusing uppercase *prefixes* right. Then: how many languages enforce hex digit case in their lexer, and what kind of tool is the only precedent?

    **Where to look:** archive/bootstrap-rs/heroes/src/lexer/number.rs (the `0X` arm) · archive/bootstrap-rs/heroes/src/lexer/digits.rs (`canonical_int`) · docs/panel/041 § historian
    **Why it matters:** §4.15 wants one spelling and the obvious reading of it would have refused the paste the notation exists for

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-literal-bases step 1 | `leading_zero` carries **two** fixes and neither is `certain`, while `base_prefix_case` carries one that is. Question: state the rule that decides which, in one sentence, without using the word "confidence". Then say what would have gone wrong had `0700`'s fix been tagged `certain` — be specific about what `heroes check --in-place` would have done to a file

    **Where to look:** archive/bootstrap-rs/heroes/src/lexer/number.rs · tests/golden/check/leading-zero.hero · CLAUDE.md §8
    **Why it matters:** a machine-applicable fix that picks one of two readings is the defect wearing a repair's clothes

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-literal-bases step 1 | Count, then explain: run `heroes mutate` over a directory holding `constant MASK: int` / `0xff` and `constant WIDE: int` / `0b1010`. How many `typo-digit` mutants, and how many would the pre-milestone `neighbouring_digit` have produced? Then say which *character* decided the difference, and why nothing in the test suite would have gone red

    **Where to look:** archive/bootstrap-rs/heroes/src/mutate/edits.rs (`neighbouring_digit`) · docs/journal/014-literal-bases.md § What surprised
    **Why it matters:** this is CLAUDE.md §11's expired premise caught one milestone before it expired, which is the only time the fix is cheap

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-sized-integers step 2 | The shape of an enum decided whether a ruling was true. Open `archive/bootstrap-rs/heroes/src/emit/ops.rs` at the line `let integral = matches!(operands, Ty::Int(_));` and read the comment above it. Question: with one `Ty` variant per width instead, what does that line evaluate to for a `u8`, what C comes out, and what does the program print? Then: how many rustc errors did each shape produce, and why is the difference not a matter of taste

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/ops.rs · archive/bootstrap-rs/heroes/src/types/table.rs (`IntKind`) · docs/journal/015 § What surprised
    **Why it matters:** a promise about runtime behaviour turned out to depend on a choice nobody would look at for it

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-sized-integers step 8 | `fit_i8(-129)` was accepted, and the five adversarial cases found it. Read the repaired test construction in `emit/ops.rs` (the `from_low < low` / `from_high > high` pair). Question: state, in one sentence, what the ORIGINAL version tested and why it was right for `fit_u8` and wrong for `fit_i8`. Then: why is a bound *omitted* rather than emitted as a tautology — name the compiler flag that decides it

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/ops.rs (the `fit_` arm) · tests/golden/run/sized-integers.hero § "a narrowing can fail and says so"
    **Why it matters:** a silent truncation inside the function built to prevent silent truncations

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-sized-integers step 3 | Three blanket renames crossed a language boundary in one session: `: int` into English prose, `"int"` into a Rust table key, and Heroes' `int` into C's inside `_Generic`. Question: for each, name the test that caught it and say how long it would have survived without that test. Then the general one: this repository holds four languages in overlapping files — what property would a rename tool need to be safe here, and does `\bint\b` have it

    **Where to look:** docs/journal/015 § What broke and why · git show 3280ffe · git show 04fc16a
    **Why it matters:** the method is sound and the tool was not, which is a distinction worth being able to make

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-program-corpus | **Walkthrough**: the six defects the corpus found, in the order it found them, with the program that met each one on screen. Three of them are one class — a narrowing that asks the world instead of the value — and the exercise is to be shown the *comment* beside each and decide whether you would have agreed with it

    **Where to look:** archive/bootstrap-rs/heroes/src/{syntax/members.rs, types/contextual.rs, resolve/exprs.rs, emit/inst.rs, printer/fmt_expr.rs, types/calls.rs} · docs/journal/016
    **Why it matters:** the class this project keeps writing rules about, caught three times in one milestone by programs rather than by reading

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-program-corpus | **Golden ratification**: `tests/golden/check/fixedbugs-variant-case-name-refused.hero`, `fixedbugs-at-marker-on-a-builtin.hero` and `tests/golden/run/fixedbugs-unary-minus-is-not-a-literal.hero` — three cases named after defects, each carrying symptom, cause and date. Read the control in each, which is the half that makes the fix safe to assert

    **Where to look:** tests/golden/
    **Why it matters:** §9's `fixedbugs` rule, three instances in one milestone

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-program-corpus | **Mutation drill**: `typo-code` went from 88 mutants at 0% to 3 at 0% and `typo-ident` gained 127 at 100%. Before reading `docs/measurements/006`, predict which way the *headline* moved and by how much — then say whether the headline moving that way is evidence of anything

    **Where to look:** docs/measurements/006 · archive/bootstrap-rs/heroes/src/mutate/
    **Why it matters:** panel 011 forbids pooling the headline, and this is the run where the headline is most tempting

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-program-corpus | **Exit quiz**, four closed questions: (a) `n + left & 1` — what does it parse as, and is that the same as C? (b) `mask & bit != 0` — same two questions. (c) A `[str]` in a `for` loop accumulated with `push`: what is its complexity, and what does the spec say about it? (d) Which of the nine corpus programs never calls `exit`, and what does that buy on Darwin arm64?

    **Where to look:** examples/logs/mask.hero · examples/adventure/ · spec § Strings, arrays, maps
    **Why it matters:** two of these were the corpus's own failing assertions, and the language was right both times

- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-program-corpus | **The blind experiment**, read whole: one `llm-ergonomist` given `spec/heroes-spec.md` and nothing else wrote three programs and answered four questions. It reported six things nobody asked about, including one that turned out to be a compiler defect. The exercise is to read its four answers *before* the measurement's conclusions and write down which of the six you would have acted on

    **Where to look:** docs/measurements/007
    **Why it matters:** the cheapest reader this project has, and its by-products are free

- [ ] **M-struct-passing close — the offers** | M-struct-passing, walkthrough | **Why does emitting *nothing* make the mechanism sound?** Read `archive/bootstrap-rs/heroes/src/emit/types.rs`'s first arm — eleven lines that return early for a group's `record` — then `emit/extern_record.rs`. The question to answer is not what the code does, it is what property the early return buys that no assertion could have bought

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/types.rs · emit/extern_record.rs
    **Why it matters:** it is the one design decision in this milestone, and it is a deletion

- [ ] **M-struct-passing close — the offers** | M-struct-passing, golden ratification | **The five adversarial cases**, marked `# UNVERIFIED — pending debrief` until read: `tests/golden/check/ffi-record-field.hero` (six refusals, six `#~`), `tests/golden/run/f32-the-narrow-float.hero` (built on the three values that separate the widths), `tests/golden/run/fixedbugs-a-narrow-float-converted-to-ok-zero.hero`, plus `examples/raylib/main.hero`'s two tests

    **Where to look:** tests/golden/
    **Why it matters:** §9 says the marker stays until the author reads them

- [ ] **M-struct-passing close — the offers** | M-struct-passing, mutation drill | **Three catch-all arms broke on one day.** Given `emit/convert.rs` before the fix — an arm for `Ty::Float(FloatKind::F64)` and a `_ => return` — predict what `to_i64` on an `f32` produces, and at what exit code, *before* reading the answer. Then say which of the project's flags could have caught it and why none did

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/convert.rs · docs/journal/018 § What broke and why
    **Why it matters:** the answer turns on panel 021's zero-initialiser being a defence, not an oversight

- [ ] **M-struct-passing close — the offers** | M-struct-passing, exit quiz | Four questions with numbers as answers: how many rustc errors did `Ty::F64` → `Ty::Float(FloatKind)` produce, and what does that number *mean*; why does the field probe take the field's **address**; why does `HERO_RET_F32` accept one C type where `HERO_RET_F64` accepts three; and which of `{int32_t; float}` / `{float; float}` against `{int32_t; int32_t}` actually breaks on arm64

    **Where to look:** docs/panel/060 · docs/journal/018
    **Why it matters:** the last one is the question the panel's own brief got wrong

- [ ] **M-struct-passing close — the offers** | M-selfhost-probe step 1 (panel 065) | The double-emit determinism test emits the same program twice with one binary and diffs the two files. Which of these regressions does it catch? (a) the emitter starts ordering `_desc` definitions by insertion instead of TyId, deterministically; (b) the emitted C mentions the output path; (c) a map with a random seed enters the emitter. One of the three passes it silently

    **Where to look:** archive/bootstrap-rs/heroes-cli/tests/golden.rs (the two tests at :520 and :555) · docs/panel/065
    **Why it matters:** the difference between *stability* and *order* is what the port's 13 sort obligations rest on, and the sitting found two orders no artifact pinned

- [ ] **M-struct-passing close — the offers** | M-selfhost-probe step 1 (panel 065) | At the fixpoint, A (Rust) builds B, B emits C.c, and `diff B.c C.c` must be empty. The port forgets one `sort` on a map walk that reaches the emitted C. Which comparison fails — B.c vs C.c, or a golden `.expected` — and why can a Heroes-vs-Heroes generation *never* show it?

    **Where to look:** docs/panel/065 § the paragraph the port author will rely on
    **Why it matters:** it decides where to look on the day the fixpoint breaks, and the answer is not "everywhere": sorted order vs fixed-seed hash order are two deterministic functions of the same data

- [ ] **M-struct-passing close — the offers** | M-selfhost-probe close | **The milestone's /learn offers, all optional**: (a) walkthrough of `selfhost/` — eleven files that mirror the Rust lexer one idea per file, readable in one sitting; (b) golden ratification — the 5 cases marked `# UNVERIFIED — pending debrief` (4 in selfhost/lexer.hero, 1 in selfhost/number.hero) plus the two order-pinning goldens from panel 065; (c) mutation drill on the ported lexer (break one scanner, watch which of the 55 tests names it); (d) exit-quiz on measurement 009's twelve gaps — which three would you have predicted, and which one has a real cost

    **Where to look:** selfhost/ · docs/measurements/009-selfhost-readiness.md · docs/journal/020-selfhost-probe.md
    **Why it matters:** the author's call, never convened by the assistant

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the declaration knot (parse_decl.hero) | **The group is flattened in the parser and nowhere else.** `extern "m.h" link "m"` with 3 signatures under it: how many `ast.Decl` values does `file` push, and what do all 3 carry that an ordinary declaration's `header` field answers with `fail`?

    **Where to look:** selfhost/parse/decl.hero § The extern group · docs/panel/036
    **Why it matters:** a declaration's index is identity for every later pass — the flattening is why no pass after the parser knows the word "group"

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the declaration knot | **Four Rust files became one Heroes module and the language left no choice.** Name the two edges that close the ring (which function calls back into which), and name one module that stayed OUTSIDE the knot and why it could

    **Where to look:** selfhost/parse/decl.hero's module map · CLAUDE.md §11 (the threshold yields)
    **Why it matters:** the same measurement decided grammar_expr.hero, so the rule is now two-for-two: a recursive-descent grammar's knots are module-shaped, not file-shaped

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the parser's front door (parse.hero) | `parse` concatenates diagnostics lexer-first, then parser — never sorted across stages. The test feeds a tab on line 2 and `junk` on line 3: which diagnostic comes out FIRST, and what would interleaving by position have put first instead?

    **Where to look:** selfhost/parse.hero, the second test
    **Why it matters:** a shape error and the word error that caused it read better together — the order is a design decision, not an accident of the pipeline

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the resolver's messages (resolve_errors.hero) | **One candidate is a certain fix, three are prose.** `unknown_name("totl", ...)` with `["total"]` in scope attaches a machine-applicable rename; with three candidates it attaches none. What is the §4.17 line that decides, and which OTHER rule in the same file hands over a certain fix even when three names are one edit away?

    **Where to look:** selfhost/resolve/errors.hero § Did you mean
    **Why it matters:** a tool that picks one of three is guessing, and only certain fixes may be applied — the case-only rule is the one exception and the reason is that the letters are already right

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the resolver's messages | The edit budget is 1 below 4 characters and 2 at 4 or more. Predict: does `nearest("fo", ["bar"])` offer anything? Does `nearest("total", ["wholly"])`? Then check the two asserts that answer

    **Where to look:** selfhost/resolve/errors.hero, the third test
    **Why it matters:** a did-you-mean that reaches too far teaches the wrong name — the budget is the difference between a repair and a trap

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, DEFECT drill (emitter) | **Hypothesise before reading the fix.** Raw symptom: two brand-new resolver tests die with `panic: entered unreachable code`, exit 134; the 10-line witness is `locals[0].reads @ locals[0].reads + 1` on a `[Local]` inside a record. Question A: the READ on the right side works — why does only the WRITE abort? Question B: the first repair (a plain C assignment) passed every functional test and still shipped a bug — which resource test catches it, and what does `hero_array_set` do that a bare `=` does not? Answers in the golden named below, but hypothesise first

    **Where to look:** tests/golden/run/fixedbugs-a-field-stored-behind-an-index.hero · docs/debrief/QUEUE.md (the 2026-08-17 DEFECT entry)
    **Why it matters:** the defect class is CLAUDE.md §11's expired premise, IN THE EMITTER'S OWN PROSE — and the witness was the port meeting a shape no golden had ever written

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the resolver complete | **The driver's order is the design.** resolve() runs: hole scan, collect, per-declaration walk, unused_uses, report_unused, cycles, sort. Two questions with one answer each: why must cycles run LAST (what is incomplete before every body is walked), and why does the hole scan run FIRST as a flat pass over the arena instead of inside the walk?

    **Where to look:** selfhost/resolve.hero, the driver's comments · resolve/mod.rs
    **Why it matters:** each position in that order fixes a defect the record can name — the cycle check reads a table only the walk completes, and a hole inside a construct the walk gives up on must still suspend the sweep

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the resolver complete | **Count the knots.** The resolver crossed as 10 Heroes modules from 15 Rust files. Which three Rust file-pairs/triples became single modules because Heroes refuses cycles, and which one pair was merged for the OPPOSITE reason (one concern, no cycle forcing it)?

    **Where to look:** selfhost/resolve/walk.hero, resolve_cycles.hero, resolve_state.hero module docs
    **Why it matters:** the module rule decided four boundaries in one pass's port — the language is shaping its own compiler

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the bidirectional knot (check_walk.hero) | **The first program the ported checker refused was the spec's own example.** `dist2(Point(...), Point(...))` at spec:186 — why does §Functions' rule fire on it, what are the two certain fixes, and why does the RUST compiler agree byte for byte? Then read the DECIDE item on why the repair waits for a panel

    **Where to look:** selfhost/check/walk.hero's first test · docs/debrief/DECIDE.md's 2026-08-17 item
    **Why it matters:** a port that reproduces the original's refusals down to the fix text is measured fidelity — and a spec whose example its own rule refuses is the exact class of silent inconsistency the project exists to catch

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the bidirectional knot | **Thirteen errors for 1,335 first-draft lines, twelve of them one class.** What is declaration_in_arm, why is `_ = 0` refused as an arm body while `fall_through()` — a ()-returning call — is legal, and which CLAUDE.md §11 rule does the loud refusal serve?

    **Where to look:** selfhost/check/walk.hero § fall_through · the 09ac46a commit body
    **Why it matters:** the error list of the knot's first compile is the thesis measured on its own compiler

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the checker complete (checker.hero) | **The driver's order is seven positions, each fixing a named defect.** Why do sizes run FIRST (what would a later answer be worth after a containment cycle), why do map_keys and the fixed sweep run AFTER decls (which table do they read, and what happens when it is empty), and why is counted LAST (what does a TyId past the table's end read as)?

    **Where to look:** selfhost/checker.hero's comments · types/mod.rs::check
    **Why it matters:** the order is not style — every position is a fixed defect, and shuffling two of them reintroduces one

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the checker complete | **The whole pipeline is now four calls.** parse -> resolve -> check, in Heroes, on Heroes source. Take the dist2 program from checker.hero's first test, predict what expr_types holds for the `dx*dx + dy*dy` node, then print it

    **Where to look:** selfhost/checker.hero, the checked() helper
    **Why it matters:** the pipeline that will compile the compiler is the one you can now step through by hand

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the IR state (ir_build.hero) | **Three invariants are made hard to break, not checked later.** Find where each lives: a temporary assigned once, a block ending exactly once, nothing emitted after a terminator. Which ONE of the three silently drops work instead of refusing loudly, and why is that the right polarity here?

    **Where to look:** selfhost/ir/build.hero: fresh_value, terminate, push_inst
    **Why it matters:** the builder's shape IS the verifier's checklist — each hard-to-break rule is one check the verifier recomputes

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the IR state | **Value id 0 is () in every function, defined by no instruction.** What breaks at the C level if a call to print HAD a destination? (The answer is one clang error, and raylib made it matter.)

    **Where to look:** selfhost/ir/build.hero::begin, ir.hero's call op
    **Why it matters:** a void call with a fake destination is the kind of plausible-looking IR that C refuses outright

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the lowering skeleton (ir_lower.hero) | **Flattening is the whole job.** `return a + b` becomes exactly three instructions and a terminator. Write them out by hand (what does each load produce, what does the add name), then check against the first test

    **Where to look:** selfhost/ir/lower.hero, first test
    **Why it matters:** if you can predict the instruction list, you understand why the C emitter is a printer and not a second compiler

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the lowering skeleton | **A negative literal is folded at lowering, not negated at runtime.** What goes wrong with `-128` against an i8 if the minus stays an instruction? (The answer is a clang warning class and one unreachable value.)

    **Where to look:** ir_lower.hero::fold_negative
    **Why it matters:** the two passes must agree on WHERE the sign lives, or the range check and the emitted C check different numbers

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, control flow lowered (ir_lower.hero) | **A for loop is four blocks and a while three — and the fourth is the interesting one.** Why does `continue` inside a for land on the STEP block instead of the test? What would `for i in xs { if skip(i) { continue } ... }` do if it landed on the test?

    **Where to look:** ir_lower.hero::for_loop, the continue test
    **Why it matters:** an infinite loop from one wrong jump target — the step block IS the difference between the two loop kinds

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, control flow lowered | **&& is not an instruction anywhere in the IR.** Find the branch polarity in short_circuit: which edge evaluates the right side for &&, and which for \|\|? Then say why `f() && g()` MUST be lowered this way (§4.14)

    **Where to look:** ir_lower.hero::short_circuit, the polarity test
    **Why it matters:** if && were an instruction, g() would run when f() is false — visible side effects in the wrong world

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, fallibles lowered (ir_lower.hero) | **Only two of the five T? forms need an edge.** Sort them: ok/fail, .is_err(), .must(), .default(v), e? — which lower to straight-line instructions and which open blocks? Then say why is_err is the cheap one

    **Where to look:** ir_lower.hero, the T? section
    **Why it matters:** the cost model of error handling IS this table: a tag comparison is free, a branch is not, and ? is a whole exit edge

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, fallibles lowered | **`f()?` must not call f twice — find the guarantee.** Where does the subject go before the tag is read, and what would `hold` NOT existing break in `next_token()?`

    **Where to look:** ir_lower.hero::hold
    **Why it matters:** one synthetic slot is the difference between reading a value twice and running its side effects twice

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, calls lowered (ir_lower.hero) | **The dot is asked in panel 079's order: the field first.** Trace `s.g(50)` where BOTH exist — a field g holding a function value AND a free function g. Which lowers, and what did the OLD order print? (Two numbers, one wrong, exit 0.)

    **Where to look:** ir_lower.hero::method, the panel-079 comment
    **Why it matters:** the same three tokens with two meanings is exactly the ambiguity class Heroes exists to kill — and the compiler itself carried one for a milestone

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, calls lowered | **Why does Callee have four cases instead of a name?** Say what breaks if the emitter re-derives the linkage for a Heroes function named `open`

    **Where to look:** ir_lower.hero's section doc, ir.hero::Callee
    **Why it matters:** clang says nothing, the program prints 7 instead of a file descriptor — the one bug class with no diagnostic anywhere

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, assert lowered (ir_lower.hero) | **A str side of an assert rides a $assert slot; an i64 side does not.** Why is the block edge the dangerous place for a counted value, and which pass makes it so?

    **Where to look:** ir_lower.hero::lower_assert's comment, the two assert tests
    **Why it matters:** the ownership pass releases an owning temporary at the end of its defining block — a block-crossing read is a use-after-free the type system cannot see

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, match lowered (ir_lower.hero) | **There is no match in the IR — find the two shapes it becomes.** Lower by hand: `match shape` on a 3-case variant, and `match n` on 0/1/_. How many blocks each, and why does only ONE of the two get a default-less dense switch?

    **Where to look:** ir_lower.hero's match section, the two match tests
    **Why it matters:** exhaustiveness is what buys the dense switch — the checker's proof becomes the emitter's freedom

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the knot closed | **The subject of a match goes in a $s slot before anything reads it.** Same rule in hold() for T? and in the for loop's holder. Name the shared invariant in one sentence

    **Where to look:** ir_lower.hero: lower_match, hold, for_loop
    **Why it matters:** "evaluate once, read many" is the difference between semantics and side-effect bugs in every one of these three forms

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the def-use table (ir_uses.hero) | **One table says what an instruction reads — find its second customer.** The verifier is one; who else walks definitions and uses, and what class of bug do TWO tables buy you?

    **Where to look:** ir_uses.hero's module doc
    **Why it matters:** "a refcount bug that reproduces once a week" is the price of two answers to one question

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, dominance (ir_values.hero) | **The first version of the invariant was WRONG and the assert lowering proved it.** State both versions — "read only in the defining block" vs "definition dominates every use" — and name the two blocks of an assert that tell them apart

    **Where to look:** ir_values.hero's module doc
    **Why it matters:** the weaker, TRUE statement is the one worth asserting: a lesson about invariants, not about IRs

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the verifier (ir_verify.hero) | **GHC's lesson, inverted: the tree was never the point.** What did Core actually buy GHC, and which single decision of panel 019 is this module the other half of?

    **Where to look:** ir_verify.hero's module doc, panel 019 point 1
    **Why it matters:** "buy the check, not the machinery" is the project's whole IR strategy in five words

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the verifier | **The copy-out check asks WHICH, not HOW MANY.** The sneaky test copies s0 out twice and s1 never — two writes for two parameters. Say why counting passes it and identity catches it, and find the other check in this codebase that made the same mistake

    **Where to look:** ir_verify.hero::check_copy_out, sweep 001 audit S8
    **Why it matters:** a count is a proxy, and a proxy that agrees with the real check on every easy case is how defects survive review

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, monomorphisation (ir_mono.hero) | **Termination is structural, not a depth limit.** State the exact test that refuses f<T> calling f<[T]> but allows f<i64> calling f<i64>, and name what Rust does instead (and why that is the warning)

    **Where to look:** ir_mono.hero::recursive, the grow test
    **Why it matters:** undecidable inference (Henglein 1993) means the compiler must refuse the SHAPE — a limit would make the error message a function of compiler internals

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, monomorphisation | **The queue substitutes the callee's arguments through the CALLER's bindings.** Trace outer<i64> -> inner(x): what does the template record at that call site, and what does the copy's call_instances hold? (Panel 084's defect is the wrong answer.)

    **Where to look:** ir_mono.hero::run and substituted_calls
    **Why it matters:** a span that exists once per copy cannot key a program-wide table — the answer must live on the copy

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the ownership pass (ir_own.hero) | **Six rules, six compiled counterexamples.** Pick rule 5 (an owning temporary MOVES into a slot in its defining block) and explain what `name + " scored " + got.must().to_str()` breaks in the rejected alternative

    **Where to look:** ir_own.hero's module doc
    **Why it matters:** the uniform reading — every value reaching a store is borrowed — is what makes the other five rules one-liners

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the ownership pass | **The whole chain now runs: lower -> mono -> own -> verify at owned, on strings, loops, records and a generic.** What did the first real run of the owned checks find, and in WHICH module was the defect?

    **Where to look:** ir_own tests, ir_phases.hero's counter comment
    **Why it matters:** a check wired but never exercised is a check that may itself be wrong — the pass and its verifier debugged each other

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the mangler (emit_mangle.hero) | **The typehash was designed for this exact module, four days early.** Name the three FNV-1a ingredients Heroes cannot express, and say what replaced each one in the polynomial hash

    **Where to look:** emit_mangle.hero's doc, tests/golden/run/premise-mangler-hash-in-heroes.hero
    **Why it matters:** a premise written as a test that fires when it dies — and today the premise was cashed, green on the first hash

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the mangler | **Why is the module component sanitised to [A-Za-z0-9]?** Give the two module names from THIS compiler that collide without it

    **Where to look:** emit_mangle.hero's doc
    **Why it matters:** injectivity by narrowing the component, not widening the separator — Nim could do the opposite only because its identifiers forbid __

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, C types and names (emit_ctype.hero) | **The unit rule has two instances — name both.** What C error does `void t0;` produce, and why can a T[N] temporary not exist either? What exists instead, in each case?

    **Where to look:** emit_ctype.hero's doc, c_type's unit and fixed arms
    **Why it matters:** the two arms where the answer is "no declaration at all" are the emitter's most surprising rule, and both were measured before the emitter existed

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, C types and names | **Why does an invented name carry a leading digit?** Show the collision `h_m_opt0` had, and say why `0opt0` cannot have one — by construction, not by check

    **Where to look:** emit_ctype.hero's doc
    **Why it matters:** reserving vocabulary via the lexer's own refusal is a zero-cost invariant: there is nothing to collide with and no diagnostic to write

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, structural eq/hash (emit_structural.hero) | **eq and hash are one invariant, and the day they disagreed a map lost a key it held.** Which three eq rows were missing until 2026-08-12, why did hash already cover them, and what did {Box: i64} do?

    **Where to look:** emit_structural.hero's doc
    **Why it matters:** "silent on insert, loud on lookup" is the failure signature of every eq/hash drift — knowing it saves an afternoon

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, structural eq/hash | **Why does a fixed array member unroll instead of comparing pointers or memcmp?** Two wrong shortcuts, two different silent failures — name both

    **Where to look:** emit_structural.hero::field_eq_expr's fixed arm
    **Why it matters:** a->f == b->f compiles clean and compares ADDRESSES; memcmp reads padding — both lie without a diagnostic

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, per-type functions complete (emit_perfn.hero) | **Why do a variant's case payloads come BEFORE the variant itself, twice?** Name the two orderings (typedefs, function bodies) and what breaks in C if either flips

    **Where to look:** emit_perfn.hero's doc, the payload-before-outer test
    **Why it matters:** the outer eq calls the inner ones — C wants everything declared before use, and the checker's type_order only solved half of it

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, per-type functions complete | **A T?'s retain does NOT go through the descriptor's copy — why?** What two things would going through it cost?

    **Where to look:** emit_perfn.hero::one_reference's comment
    **Why it matters:** a descriptor has copy(dst,src) and no retain-in-place: the wrong reuse means copying a value onto itself and casting away a const

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, operators (emit_operator.hero) | **% is guarded like / but reports differently — why was "integer overflow" for % a FALSE message?** And what does INT64_MIN % -1 do on arm64 versus x86 with no guard?

    **Where to look:** emit_operator.hero's div/rem arm, panel 035
    **Why it matters:** same guard, different truth: the remainder is 0 and overflows nothing — the quotient C computes on the way is what has no int64

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, operators | **1 << 63 must be INT64_MIN, not a trap — how does the emitted C make a UB-free left shift reach the sign bit?**

    **Where to look:** emit_operator.hero's shift arm
    **Why it matters:** the unsigned round-trip is what lets a mask set name its own top flag while staying defined for every input

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, containers (emit_container.hero) | **One unshare per array step, each at its own level — walk g.rows[0].cells[0] @ 7 by hand and count the unshares.** Then say what "one at the primitive" would alias

    **Where to look:** emit_container.hero's doc, panel 022
    **Why it matters:** the nested-store aliasing bug reported success on every instrument — only the counted walk shows why the rule is per-step

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, containers | **This morning's bootstrap fixedbug is now pinned in the port's own test.** What are the exact three lines xs[i].f @ v emits, and which of the three was missing in each of the two broken versions?

    **Where to look:** emit_container.hero, the fixedbug test; tests/golden/run/fixedbugs-a-field-stored-behind-an-index
    **Why it matters:** the defect found by the port, fixed in the bootstrap, and now guarded in BOTH compilers — the full circle

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, calls as C (emit_ops.hero) | **Four callee kinds, four C spellings — and only ONE goes through unmangled.** Say which, why, and what the cstr guard wraps around its arguments (and which parameter kind is deliberately excluded)

    **Where to look:** emit_ops.hero, the extern test
    **Why it matters:** the linkage recorded in the IR pays off here: the emitter never re-derives who is being called

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, calls as C | **print is a compiler form, not a function.** Why does hero_print_end own the newline rather than the emitter? (The answer is a header and a collision surface.)

    **Where to look:** emit_ops.hero::print_call's doc
    **Why it matters:** one #include in every generated unit is a cost paid on every FFI program forever

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, whole functions print (emit_body.hero) | **Why does the prologue exist, in one C rule?** And name the ONE exception to "nothing is initialised here" and what it buys

    **Where to look:** emit_body.hero's doc, the prologue
    **Why it matters:** goto may not jump over an initialisation — the whole hoisting scheme falls out of that sentence, and the refcounted {0} trades one clang check for an unconditional exit sweep

- [ ] **M-struct-passing close — the offers** | M-selfhost-port, whole functions print | **An unreachable join is OMITTED, label and all — why is unlabelled not enough?**

    **Where to look:** emit_body.hero's doc, the bb1 test
    **Why it matters:** C is physical: an unlabelled block falls through from the one above it, and the correct program that produces one is any if whose both arms return

- [ ] **M-struct-passing close — the offers** | M-selfhost-port (ffi_record crossing) | Two rows about one struct: clang echoes `t.hero:2:12` for a probe under a `#line` — is that 2 the line in the whole concatenated text or the line within t.hero, and which one does `locate()` answer?

    **Where to look:** selfhost/emit/ffi_record.hero record_at_line, selfhost/source.hero line_of vs file_line_of
    **Why it matters:** the bootstrap compared the wrong denominator and every single-file test hid it — the port found it by having to choose between two functions whose names state the difference

- [ ] **M-struct-passing close — the offers** | M-selfhost-port (the differential) | The port and the bootstrap disagreed on six inputs about ONE thing: how wide the caret is under `sort(ps)   # a note`. One of them underlined the comment too. Which one was right, and what in the bootstrap's own source says so before any measurement?

    **Where to look:** archive/bootstrap-rs/heroes/src/syntax/cursor.rs::previous_significant_span's doc, and expr.rs's two call sites
    **Why it matters:** a repair applied to one shape and not its neighbour is CLAUDE.md §1's fourth rule, and the port is what made it visible after five days

- [ ] **M-struct-passing close — the offers** | M-selfhost-port (the counter class) | Six sites in the port skipped a hand-kept index because a `continue` in a match arm is a loop jump. Given `for decl in decls` with `index @ index + 1` at the bottom, what does the language offer as a no-op arm — and why is `_ = index` not it?

    **Where to look:** selfhost/check/sized.hero::collect, selfhost/ir/verify.hero, docs/debrief/DECIDE.md's item (2026-08-26)
    **Why it matters:** the class cost a false `no_size`, every C type name off by one, and a verifier that refused every program with a `.must()` (`docs/debrief/` was re-cut into `docs/work/` on 2026-08-26)

- [ ] **M-struct-passing close — the offers** | M-selfhost-port (the streams) | The port's `check` output looked reordered against the bootstrap's when both were captured with `2>&1`, and byte-identical when captured apart. What makes the merged order differ, and why is it NOT a contract violation?

    **Where to look:** selfhost/cli/io.hero, CLAUDE.md §10's contract line
    **Why it matters:** telling a real difference from an artefact of the instrument is the whole job of a differential

- [ ] **M-struct-passing close — the offers** | M-harness-port | The harness runs the compiler through `system()` and reads two files back. Open `tests/harness/shell.hero` and answer: why is the command wrapped in `( … )` before the redirection? What does `printf a; printf b > out` put in `out` without it?

    **Where to look:** tests/harness/shell.hero, its `run` function (2026-08-26)
    **Why it matters:** a shell redirection binds to one command, not to a list — and the harness lost half its output to this before its own tests caught it (the harness as it stood 2026-08-26)

- [ ] **M-struct-passing close — the offers** | M-harness-port | `tests/golden/check/x.hero` carries `#~ <code>` comments AND `x.expected` records the same diagnostics. Why is that redundancy the one check a regenerator cannot fake? (2026-08-26)

    **Where to look:** tests/harness/suite_annotations.hero · CLAUDE.md §9
    **Why it matters:** 181 pinned diagnostics rest on it, and it is the reason `UPDATE_GOLDEN` can be forbidden rather than merely discouraged (the fixture as it stood 2026-08-26)

- [ ] **M-struct-passing close — the offers** | M-harness-port | `heroes run tests/harness/main.hero -- ./build/heroes-seed` runs the net through the self-hosted compiler. Name the four configurations that were run, and say which one is "life after the archive"

    **Where to look:** docs/journal/023-harness-port.md
    **Why it matters:** the milestone exists so that the archive is a move rather than an amputation, and only one of the four proves it

- [ ] **M-struct-passing close — the offers** | M-harness-port | The `layout` check holds 17 files to the length they measure today rather than to §11's 300. Why is a ceiling-per-file the honest answer here, and what would a bare 300 have done on its first run?

    **Where to look:** tests/harness/suite_layout.hero
    **Why it matters:** a rule that cannot be obeyed is not a standard, and the language itself forbids splitting a mutually recursive knot

- [ ] **M-struct-passing close — the offers** | M-bootstrap-archive (the emission oracle) | `tests/emission/` holds 142 files of generated C, 5.4 MB. A manifest of 142 **hashes** would catch exactly the same changes for 142 lines. Say what the hash cannot do, and why that answer changes the day the bootstrap is archived rather than today

    **Where to look:** tests/harness/suite_emission.hero's module doc
    **Why it matters:** an oracle is only as useful as the failure it can explain, and after the archive there is no second compiler to ask

- [ ] **M-struct-passing close — the offers** | M-bootstrap-archive (the emission oracle) | Four guards keep this suite from quietly stopping: a floor, a collision check, an orphan sweep, a skip floor. Pick the ORPHAN sweep and say which mistake it catches that the other three cannot

    **Where to look:** tests/harness/suite_emission.hero::orphans
    **Why it matters:** a renamed case leaves bytes nobody compares, and nothing else in the repository would notice

- [ ] **M-struct-passing close — the offers** | M-bootstrap-archive (`measure`, the tokeniser) | Heroes cannot build a `str` out of arbitrary bytes, so the port could not decode the BPE table the way the Rust does. What does it do instead, and why is the answer sound rather than a trick?

    **Where to look:** selfhost/measure/bpe.hero's module doc, `spelled`
    **Why it matters:** the two implementations agree to the token on the real spec — 3440 and 3512 — and that agreement is the whole proof

- [ ] **M-struct-passing close — the offers** | M-bootstrap-archive (`measure`, the pre-tokeniser) | The port reads a "letter" as `A-Z a-z` where the Rust asks Unicode. That is a premise about the INPUT, not about the language. Where does its falsifier live, and what exactly makes it fire?

    **Where to look:** selfhost/measure/pieces.hero's claim · tests/harness/suite_spec.hero::inventory
    **Why it matters:** CLAUDE.md §11: a premise about the world expires in silence unless something is watching it

- [ ] **M-struct-passing close — the offers** | M-bootstrap-archive (`mutate`, the two arms) | Every mutant is judged twice — `check`, and `check --permissive`. On the gallery the numbers are 512 (96%) and 414 (78%). Say what the SECOND column is for, and what a language scoring 100% in both would have proved

    **Where to look:** selfhost/mutate/score.hero::fate · panel 011
    **Why it matters:** a language that rejects everything maximises the kill rate and proves nothing, which is why the control arm is mandatory

- [ ] **M-struct-passing close — the offers** | M-bootstrap-archive (`mutate`, the language's own help) | The thirteen operators each match on every `ExprKind` case by name, because `_` is forbidden on a variant. Say what that costs in lines, and what it buys the day a fourteenth expression form is added

    **Where to look:** selfhost/mutate/edits.hero · spec:120
    **Why it matters:** the Rust has a `_` in the same place, and that is where the lexer's `nullptr` defect came from

- [ ] **M-struct-passing close — the offers** | measurement 012 (the writer, and the 46×) | The compiler spent 43% of its own build re-copying text it had already written: `out @ out + line`, once per emitted line, copies everything so far every time. The repair keeps 128 lines in a small array, glues them into one string, and glues the strings once at the end. Say why the same trick cannot fix the lexer's token array — what does `join` give a `[str]` that nothing gives a `[Token]`?

    **Where to look:** selfhost/emit/writer.hero (the module doc and CHUNK_LINES) · selfhost/source.hero (from_files, the loader's twin of the same defect) · docs/measurements/012 · design.md §4.10
    **Why it matters:** the language's commonest line is quadratic, 56% of the build fell to two fixes that needed no permission, and most of the rest waits on a decision only the author can make (DECIDE R2)

- [ ] **M-struct-passing close — the offers** | measurement 013 (the place store, and the borrow that was safe by accident) | For eleven milestones, reading an array into a temporary and then calling a function that mutates that same array through `@` was safe — but nobody had made it safe: the ownership pass parked every accumulator in a synthetic slot, so its refcount was 2 and the header under the borrow could not die. The place store made refcounts honest, and the very first named case (`place-store-c5`, case 5) turned the accident into a crash. Say why "the count is 2" and "someone holds a reference on purpose" are different facts, and which one a correctness argument may rest on

    **Where to look:** selfhost/ir/place_store.hero (load_survives_write and the module doc) · selfhost/ir/own.hero (the .load arm) · docs/measurements/013
    **Why it matters:** CLAUDE.md §11's expiring-premise rule, measured in the wild: the premise ("rule 5 keeps it alive") was never written anywhere, which is why nothing fired when it died

- [ ] **M-struct-passing close — the offers** | M-separate-compilation step 1 (the order nobody promised) | A map in Heroes hands back its keys in NO order, and the compiler walks maps to decide what to write into the C. Once, unsorted, that produced 20.8 MB of C whose LINES matched the old compiler's exactly and whose SEQUENCE did not — 3,330 lines out of 724,197, and it was the last thing standing between the port and the fixpoint. Say why sorting the keys fixes it, and then the harder half: why the SAME missing sort is harmless in `emit.hero:153`, where the walk drains a worklist. What is different about what leaves that loop?

    **Where to look:** selfhost/emit/ctype.hero:236 (the mark and its story) · selfhost/emit.hero:153 (`ORDER: none`) · design.md §4.9
    **Why it matters:** the same construct is a defect in one place and correct in the other, and the only thing that tells them apart is what somebody can observe

- [ ] **M-struct-passing close — the offers** | M-separate-compilation step 1 (a convention that could not be checked) | The rule was written down and greppable for six milestones, and the grep returned 10 hits of which 4 were prose quoting the rule rather than obeying it. Now `# ORDER:` counts only as the first word of a comment, and a check reads it. Say what the check must search to decide whether a walk is covered — why a fixed window of lines above the walk is the wrong answer, and what the right scope is

    **Where to look:** tests/harness/suite_order.hero (`marked_above` and its cases) · selfhost/resolve/cycles.hero:177 (a mark 25 lines above its sort)
    **Why it matters:** a convention nothing executes decays into prose, which is CLAUDE.md §11's expiring premise with a comment on top

- [ ] **M-struct-passing close — the offers** | panel 091 (the assertion that is a seatbelt) | The compiler writes `_Static_assert` lines and tiny `hero_ffi_probe_*` functions into the generated C so that clang checks your `extern` declarations against the real header. A sitting asked whether to stop writing them for declarations you never call, and the answer was no — because one of them turned out to be the ONLY thing standing between a program and four destroyed bytes of memory: `getline`'s `size_t` out-parameter declared `i32`, never called. Say why the ASSERT and the PROBE ask different questions, and which of the two saw that one

    **Where to look:** selfhost/emit/extern_assert.hero · selfhost/emit/extern_probe.hero · docs/panel/091 § What the ffi-pragmatist compiled
    **Why it matters:** the two look like the same thing in the emitted C and are not: one asks what the header RETURNS, the other what it ACCEPTS

- [ ] **M-struct-passing close — the offers** | panel 091 (a saving that costs nothing because you were going to get it anyway) | A program that prints `1` carries 27 lines of C verifying a library it never touches, and under one `.c` per module 138 of 155 modules would carry them. The obvious fix — emit them only where they are reached — was refused, and the duplication dies anyway. Say why: what is it about the LIBRARY being one module, plus one `.c` per module, that makes the multiplier disappear with no code written?

    **Where to look:** docs/panel/091 § The resolution · selfhost/source.hero:109-110 · design.md:786
    **Why it matters:** the cheapest change is the one the architecture was already going to make, and spotting it is worth more than building the clever version

- [ ] **M-struct-passing close — the offers** | panel 092 (the macro that hid a check) | For months, `extern "string.h" function memset(s: ptr, c: i32, n: i32)` — where C says `size_t` — compiled at exit 0, while the same mistake against a hand-written header was exit 1. The emitted probe line was **byte-identical** in both cases. Say what was different, and why `-fno-builtin` did not help. Then the useful half: the compiler emits a tiny never-called function per `extern` just so clang will type-check the arguments — say why that function has to exist at all, when the real call site is right there in the same file

    **Where to look:** selfhost/emit/extern_probe.hero (the module doc, then the parenthesis) · docs/panel/092 § The cause, wrong twice before it was right
    **Why it matters:** the check existed, the diagnostic was good, and one macro expansion made it invisible on exactly the functions everybody binds

- [ ] **M-struct-passing close — the offers** | panel 092 (two declarations of one enum, both correct) | Fixing `examples/curl` needed `constant CURLOPT_URL: i32` and `function curl_easy_setopt(..., option: u32, ...)` — the same C enum declared two different ways in the same file, and the header says both. Say why, and what it means for `.to_u32().must()` appearing at the call

    **Where to look:** examples/curl/main.hero (the call and its comment) · spec:204
    **Why it matters:** the FFI's rules are C's rules, and C distinguishes an enumeration constant from an enumeration type

- [ ] **M-struct-passing close — the offers** | M-separate-compilation step 4 (a check that is right in one scope and wrong in every other) | The four conversion errors that catch a wrong float or narrow-integer binding live in a `#pragma clang diagnostic` block around the probes, and can NEVER join `-Werror=shorten-64-to-32` in `flags()`. Why? (a) clang refuses them under gnu11 (b) enabled globally they fire on 19 of 146 blessed emissions of correct programs — `s[i]` landing in a `u8`, an `f32` literal — because generated code converts on purpose (c) they would slow every build

    **Where to look:** selfhost/emit/extern_probe.hero::PROBE_ERRORS · the step-4 commit body
    **Why it matters:** the probe's arguments are exactly the declared types, so inside the block ANY implicit conversion is a wrong binding — the same diagnostic is 100% signal in one scope and 13% noise in the other

- [ ] **M-struct-passing close — the offers** | M-separate-compilation step 4 (the exact conversion that still loses) | `pow(x: f32, y: f64)` printed a fifth-digit-wrong answer at exit 0, yet the conversion f32→double at the call is EXACT — not one bit changes. Where was the precision lost, and why does this diagnostic's sentence ("the precision the header expects is lost before the call") differ from the integer one ("C would convert the value in silence")?

    **Where to look:** tests/golden/fixedbugs/ffi-float-parameters.hero · selfhost/emit/ffi_narrowed.hero::PROMOTED
    **Why it matters:** the defect is upstream of the conversion clang sees, which is why -Wconversion — the flag that watches value-changing conversions — is measured blind to it

- [ ] **M-struct-passing close — the offers** | M-separate-compilation step 4 (what stays silent, and why it must) | After step 4 the spec says two parameter mismatches still build: what C converts exactly (`i16` against int) and what a pointer points at. For each: is it unchecked by CHOICE or undetectable by clang? One of the two could be closed tomorrow if Heroes grew one feature — which feature, and which hole?

    **Where to look:** spec:205-207 · docs/panel/092 § What is still not checked
    **Why it matters:** a hole the document names is a contract; the pointer half waits on a typed pointer, and knowing that is knowing the FFI's roadmap

- [ ] **M-struct-passing close — the offers** | M-separate-compilation step 5 (one gate, three doors) | `bind.system(x)` is now refused — and so are `_ = bind.system` (no call!) and `"p".cstr().hero_file_read(@st)`. Why must the VALUE form be refused if nothing is called on that line? What would C need at the point where the address is taken?

    **Where to look:** selfhost/resolve/qualified.hero (the function_decl arm) · tests/golden/surface-fixtures/externroute/wrong.hero
    **Why it matters:** a rule that gates only the spelling it was named after is a sieve — "callable" undercounts how a name crosses a boundary

- [ ] **M-struct-passing close — the offers** | M-separate-compilation step 5 (the library is a module too) | `hero_args_count()` from your program was accepted at exit 0 until today, and `args()` still is. What is the difference between the two, seen from the C that gets emitted — and why do the library's CONSTANTS (`HERO_OS_OK`) stay reachable while its extern functions do not?

    **Where to look:** selfhost/resolve/walk.hero (the is_library gate) · tests/golden/check/extern-across-modules-library.hero
    **Why it matters:** the wrapper is not bureaucracy: it is the place the checks live, and the constant's accessor is compiler-written on both sides

- [ ] **M-separate-compilation step 7 (the cache that could not see a header)** | Before this step, this program printed `1` while `conf.h` on disk said `#define CONF_LIMIT 2`, at exit 0. The compiler was not confused about the value — it never asked. Given that `constant CONF_LIMIT: i64` in an `extern` group emits as `int64_t h_bind_CONF_LIMIT(void) { return CONF_LIMIT; }`, say why the compiler's cache key — which contains the whole emitted C, character for character — cannot see the difference between a header saying 1 and the same header saying 2

    **Where to look:** selfhost/cli/deps.hero (the module header) · selfhost/cli/units.hero:75 (the key)
    **Why it matters:** the cache key is only as good as what it can see, and this is the one thing it structurally could not

- [ ] **M-separate-compilation step 7 (what a compiler asks the compiler)** | The repair does not add the header to the key. It asks clang, on every compile, "which files did you open?" (`-MD`), records each one's content digest beside the object, and re-checks all of them before reusing it. Three choices were available: (a) hash the header names the `extern` groups write down, (b) run the preprocessor first so the key can contain the answer, (c) what was done. Say what (a) misses, and what (b) costs — the numbers are in the module header

    **Where to look:** selfhost/cli/deps.hero (WHY CLANG'S OWN LISTING, WHY BESIDE THE OBJECT) · docs/panel/093 R4
    **Why it matters:** the sitting named the goal and not the mechanism, and the mechanism was where the whole cost was

- [ ] **M-separate-compilation step 7 (the cheapest way to be correct is to be useless)** | `tests/harness/suite_cache.hero` pins three things, and the second and third exist only because of the first. If a build system only had to pass "a header edit is not invisible", one line of code would do it: never reuse anything. Say what each of the other two cases forbids, and why "only `bind.o` moved" is a statement about acceptance row 1 rather than about the cache

    **Where to look:** tests/harness/suite_cache.hero · docs/panel/033 (acceptance row 1)
    **Why it matters:** a test that only checks the loud direction lets the quiet failure through

- [ ] **M-separate-compilation step 7 (157 answers to one question)** | `runtime_text` reads the runtime's 14 parts, concatenates them and hashes 162 KB. It was called once per translation unit — 157 times per build of the compiler, each with a shell `cat` — for an answer that cannot change during a build. It is now a field of `Toolchain`, computed once. Given this compiler's measured hashing speed of ~1.2 MB/s, work out how much of a per-module build that was, and say why the same shape did NOT show up on the fused path

    **Where to look:** selfhost/cli/toolchain.hero (the `runtime_key` field) · selfhost/cli/verbs.hero (the memo threaded through the loop)
    **Why it matters:** a loop-invariant computation is the oldest optimisation there is, and it hid inside a function whose name made it look free

- [ ] **M-separate-compilation step 7 (the language chose the seam)** | The runtime's object had the same header blindness, so `cli_toolchain` needed to call `cli_deps.fresh`. That did not compile: `cli_deps` hashes files, `digest` lived in `cli_toolchain`, and Heroes refuses module cycles — `module_cycle` fires on the `use` edge whatever it carries. The fix was to move `digest`/`hex8` into `selfhost/cli/digest.hero`. Say what the compiler would have had to do to ACCEPT the cycle, in terms of what it emits — and why `selfhost/cli/digest.hero` is a better file than the one it left, independently of the cycle

    **Where to look:** selfhost/cli/digest.hero (the module header) · selfhost/cli/toolchain.hero · docs/panel/031 R6 (the cycle refusal as "the reversible direction")
    **Why it matters:** a rule that forbids something usually forbids the wrong thing too, and this one pointed at a seam that was already there

- [ ] **M-separate-compilation step 9 (the question that was the wrong question)** | `cursor.take_docs` decides whether a comment documents the declaration under it. It used to ask `line_col(text, offset)` for a LINE NUMBER, which counts newlines from byte 0. It now asks two other things and never learns a line number at all. The whole repair is that substitution, and it took `heroes check` on the compiler's own source from 88 s to 28. **The question**: what are the two things it asks instead, and why is neither of them expensive?

    **Where to look:** selfhost/text_lines.hero (`newlines_between`, `column_of`, `lines_above`) · selfhost/cursor.hero (`take_docs`) · docs/journal/025
    **Why it matters:** it is the clearest case in the project of a cost that came from asking for more than you need, and the general rule is one sentence long

- [ ] **M-separate-compilation step 9 (one shape, four places)** | `out.uses @ out.uses.push(v)` and `uses @ uses.push(v)` compile to different C: the first copies the whole array, the second grows it in place. The difference is that `out.uses` is a FIELD of a record and `uses` is a bare name. Four places in the compiler had the first form in a hot loop — the resolver's two arenas, the lexer's token array, and the scope table — and fixing all four took the check from 28 s to about 8. **The question**: why can the in-place store only fire on a bare name? What would it have to prove to fire on a field?

    **Where to look:** selfhost/resolve/state.hero (`new_resolver`, `declare`) · selfhost/state.hero (`push_token`) · design.md §4.10 · DESIGN-LOG 2026-08-26
    **Why it matters:** this is copy-on-write seen from the inside, and the answer is about aliasing rather than about optimisation

- [ ] **M-separate-compilation step 7 (a key that cannot see its input)** | An `extern constant` emits as `return CONF_LIMIT;` — the NAME, not the value. So editing the header changes what the program prints and changes nothing in the generated C. A cache key built from the generated C is blind to that edit **by construction**, and the measured symptom was a program printing `1` at exit 0 with the header saying `2`. **The question**: three repairs were weighed and one landed. Given that hashing the header names an `extern` writes down misses the file one level below, and preprocessing first costs a second clang pass over 157 units, what is left?

    **Where to look:** selfhost/cli/deps.hero · tests/harness/suite_cache.hero · DESIGN-LOG 2026-08-26
    **Why it matters:** the answer is a habit worth having: when you cannot compute a fact, ask the tool that already knows it

- [ ] **M-separate-compilation close (an invariant that cannot see its subject)** | `suite_annotations` is described in this project as its one unforgeable check: a regenerator can rewrite an expectation file but cannot invent a `#~` mark in the program. It sweeps two directories. Measured at the close: 19 marks live outside them, 15 of those in `tests/golden/fixedbugs/`, which is the directory that exists for defects that already shipped once. **The question**: what property of the suite's MECHANISM limits it to two directories, and which of the three ways out keeps the word *unforgeable* true?

    **Where to look:** tests/harness/suite_annotations.hero:42-46 · docs/work/DECIDE.md · CLAUDE.md §9
    **Why it matters:** the interesting part is that the suite's own failure message describes the state it is in, about a floor it cannot see past

- [ ] **M-argv-execution** | walkthrough offer: `runtime/parts/run.c` end to end — why Windows has no argv, what `win_quote` defends, where the watchdog lives

    **Where to look:** runtime/parts/run.c
    **Why it matters:** the compiler's whole contact with the OS is this one file now

- [ ] **M-argv-execution** | golden ratification offer: the 5 adversarial cases of the milestone

    **Where to look:** tests/golden/
    **Why it matters:** §9's marker is still on them

- [ ] **M-argv-execution** | exit-quiz offer: which failure was POSIX-invisible and why — the stack, the CRLF, the depfile backslash, the pause utility

    **Where to look:** docs/journal/026-argv-execution.md
    **Why it matters:** four defects no Mac could ever show

- [ ] **M-package-layout step 2** | Three vocabularies now describe one module: the PATH the machine opens (`geom/point.hero`), the MODULE that identifies it in a compilation (`geom/point`), the COMPONENT that reaches C (`geompoint`) and the BINDING a call writes (`point`). Given a file `syntax/decl.hero` used from the root, say which of the four each of these produces: `module_names.stem_of`, `module_names.component_of`, `module_names.binding_of`, and the `module` field of its `FileEntry`

    **Where to look:** selfhost/module/names.hero (the module doc names all four) · selfhost/source.hero:132
    **Why it matters:** reading a value from the wrong vocabulary is a defect class rather than a slip: panel 032 D1 and two more compiled shapes at panel 099

- [ ] **M-package-layout step 2** | The compiler refuses two modules whose LAST parts match, anywhere in one program, and separately refuses two whose C COMPONENTS match. Say which of these three pairs trips which check, and which trips neither: (a) `syntax/decl` + `ir/decl`, (b) `print_inst` + `printinst`, (c) `syntax/decl` + `syntax/expr`

    **Where to look:** selfhost/module/paths.hero (`last_parts_collide`) · selfhost/modules.hero (`collisions`)
    **Why it matters:** one check protects the program's names, the other protects the linker, and each is blind to what the other sees

- [ ] **M-package-layout step 2** | `use a/b` and `use a / b` produce the SAME five tokens from the lexer, and the compiler reads them as two different programs. Say what distinguishes them, and then say which of the compiler's two readers of a use line would get it wrong if that rule lived in only one of them

    **Where to look:** selfhost/parse/use_line.hero (the cursor reader) · selfhost/module/paths.hero (`path_at`, the token reader, and the test that makes them agree)
    **Why it matters:** panel 031 R8 refused a second grammar for one text, and this is that refusal one level down — the same rule in two places, with a test that fires when they diverge

- [ ] **M-package-layout step 2** | A `use` line whose last token is punctuation closes with NO terminator, because `use` is not in panel 007's ender list. Say what the first shape of `module_path_climbs` did to the line BELOW it because of that, and say why the dot refusal — older than both new ones — had the same bug and never showed it

    **Where to look:** tests/golden/check/fixedbugs-use-refusal-eats-the-next-line.hero · selfhost/parse/use_line.hero (`skip_rest_of_line`)
    **Why it matters:** the golden written to pin a new refusal found a defect in it within minutes, which is what §9's annotation-plus-snapshot redundancy is for

- [ ] **M-package-layout** | walkthrough offer: the three sittings of one day, in order — where a path starts (099), what happens when two paths end in the same word (100), and whether a path should bind its last part at all (101). The third refused a rule four seats approved

    **Where to look:** docs/panel/099 · 100 · 101 · docs/journal/027-package-layout.md
    **Why it matters:** the only day this project held three sittings on one clause, and the corpus priced the answer twice

- [ ] **M-package-layout** | golden ratification offer: the milestone's adversarial cases — `use-has-a-path.hero` (four refusals, one of whose comments was measured false), `use-as-renames-it.hero`, and `fixedbugs-use-refusal-eats-the-next-line.hero`

    **Where to look:** tests/golden/check/
    **Why it matters:** §9's marker is still on them

- [ ] **M-package-layout** | exit-quiz offer: six consumers of `as` were shipped one at a time. Name them in the order they were found, and say which one made the other five invisible

    **Where to look:** CLAUDE.md §9 · selfhost/cli/syntax_cmds.hero (the guard's own test)
    **Why it matters:** one omission, two consumers, and the second was the instrument watching the first

- [ ] **M-package-layout** | mutation drill offer: `heroes mutate` over the module-resolution path, now that a `use` line carries two spans (the path and the binding) rather than one

    **Where to look:** selfhost/parse/use_line.hero · selfhost/resolve/top.hero
    **Why it matters:** a form the mutator cannot re-print is a form it silently declines to mutate

- [ ] **M-robustness-guards step 1** | `add(@bs[0], "y")` on a cell now compiles to a READ of the element into a temporary, the call with the temporary's address, and a STORE back through `bs[0]` after the call. Count the instructions `ir/inout.hero` adds to `main` for that one line by reading `--dump-ir`, and say which of the three would be missing if the index expression were lowered twice

    **Where to look:** selfhost/ir/inout.hero (`argument`, `element_value`, `write_back`) · `heroes build tests/golden/run/inout-through-paths-of-a-cell.hero --dump-ir`
    **Why it matters:** the index is evaluated once, and `pick` printing once in that golden is the proof

- [ ] **M-robustness-guards step 1** | before the repair, `heroes check` accepted seven of eight `@`-on-an-immutable shapes and the eighth, `add(@m["k"], "x")`, was refused as `type_mismatch`. Which single fact about the language made one rule cover all seven, and why did the map case never need it?

    **Where to look:** spec:79-82 and :88 · design.md §4.10 (every place has exactly one root) · selfhost/resolve/writes.hero (`inout_root`)
    **Why it matters:** a rule that reads the root instead of the shape is one rule, not eight

- [ ] **M-robustness-guards step 1** | the emitter's comment said an index step *"cannot appear yet … and the walk stops rather than producing something that compiles"*. It compiled. Name the C type the callee actually received for `@bs[0]` and the flag on panel 047's list that would have refused the unit at exit 2

    **Where to look:** selfhost/emit/aggregate.hero (`place`) · docs/work/DONE.md (the step-1 item, exit 138)
    **Why it matters:** a premise about the world expired in silence while its argument stayed valid (CLAUDE.md §11)

- [ ] **M-robustness-guards step 4** | `runtime/parts/stack.c` asks TWO questions before it says *stack exhausted*: where the faulting address is, and where the stack pointer is. A wild store 8 KiB under the stack answers the first one yes. Say what it answers to the second, and which of the two witnesses alone would have made the handler lie

    **Where to look:** runtime/parts/stack.c (the handler; the two-witness comment) · docs/panel/104
    **Why it matters:** a guard that fires on the wrong fault turns a memory bug into a false diagnosis, which is worse than a bare crash

- [ ] **M-robustness-guards step 4** | `down(100000)` at `-O2` prints `100000`; `down(1000000)` at `-O2` panics. Which of these two sentences about clang follows from the pair — *it turned the recursion into a loop* or *it made each frame smaller* — and what single extra run would tell them apart?

    **Where to look:** tests/golden/surface-fixtures/deep/main.hero (its comment) · docs/journal/031 § What surprised
    **Why it matters:** one measurement at one depth was written down as a property of the optimiser and was wrong

- [ ] **M-robustness-guards step 4** | the handler runs on an ALTERNATE stack (`sigaltstack`) and Windows reserves one with `SetThreadStackGuarantee`. Say in one sentence why a handler for *the stack is full* cannot run on the stack, and where the name `main.down` in the message comes from when the stack it would walk is the one that overflowed

    **Where to look:** runtime/parts/stack.c (`hero_stack_guard_install`, the frame walk)
    **Why it matters:** the message names the function; that name has to come from somewhere the fault did not destroy

- [ ] **M-robustness-guards step 5** | `HERO_RUNTIME_ABI` went 18 → 19 in THREE files and the edits had to land in an order. Name the three, say which one is the seed binary's and why bumping the header first would have left the repository with no compiler that can compile anything — then say what `seed/heroes.c`'s eighth line does the day someone forgets

    **Where to look:** runtime/heroes_runtime.h:37 · selfhost/emit/decls.hero (the `_Static_assert` line) · seed/heroes.c:8 · seed/README.md § When this file must be regenerated
    **Why it matters:** the stamp is the one guard that fires at compile time rather than at run time, and it fires against the compiler you are holding

- [ ] **M-robustness-guards step 5** | the harness's scratch directory is now `build/harness-<pid>`. Say what two harnesses started at the same second wrote into one `build/harness` before, and why the emission BLESSING (`UPDATE_EMISSION=1`) was the one place where that turned into a file committed as truth — then say what `is_a_capture` checks that a byte comparison could not

    **Where to look:** tests/harness/main.hero (`scratch`) · tests/harness/suite_emission.hero (`is_a_capture`, `judge`)
    **Why it matters:** the blessed traces have no other source of truth: the compiler that produced them is archived

- [ ] **M-corpus-depth step 3** | `heroes fmt` DELETED the parentheses from `1 << (DEPTH - depth + MIN_DEPTH)` in `examples/binarytrees/main.hero`, and the program still prints the published numbers. Say which of the two is true — *`+` binds tighter than `<<` in Heroes* or *the formatter normalises redundant parentheses wherever it finds them* — then say what the same line means in C, where the answer is the other way round

    **Where to look:** spec § Operators (the precedence list, strongest first) · examples/palette/ (the corpus's other program whose lines stand as `heroes fmt` produced them, for `&` against a comparison)
    **Why it matters:** a canonical form that changes a line's meaning would make every diff untrustworthy (design.md §4.15), so which parentheses it may drop is a language fact and not a style

- [ ] **M-corpus-depth step 3** | `record Node` with `left: Node` and `right: Node` is `error[no_size]`, and `examples/binarytrees/` holds its two halves in a `[Tree]` instead. Say what the compiler would have to know to give that record a size, and why `[T]` answers it when a second `Node` field does not — then say which ONE line of the spec is the whole rule

    **Where to look:** spec § Types (the last bullet) · examples/binarytrees/main.hero (the header, which quotes the diagnostic) · examples/json/value.hero (a variant recursive through both `[T]` and `{K: V}`)
    **Why it matters:** every value is an independent copy, and this is the one place where that guarantee is visible as a refusal rather than as a convenience

- [ ] **panel 105** | **Where does the type of `wanted(what: "a")` come from?** Open `selfhost/check/walk.hero` at `user_call` and read the two lines that call `check_generics.bind` and `check_generics.bind_missing`. In `x: i64? = wanted(what: "a")`, which of the two binds `A`, and what value does `expected` hold when `wanted` is called as `print(wanted(what: "a"))`? Choose: (a) `bind`, from the argument `"a"` · (b) `bind_missing`, from `i64?` · (c) neither, and that is the error

    **Where to look:** selfhost/check/walk.hero (`user_call`), selfhost/check/generics.hero (`bind_missing`)
    **Why it matters:** the whole of panel 105 is one word — *else* — and this is where it lives

- [ ] **panel 105** | **Why does `bind_missing` never push a diagnostic?** Write `n: i64 = wanted(what: "a")` in a scratch file and run `heroes check` on it. Count the diagnostics. Then read `bind_missing`'s doc comment and say which OTHER function reported the one you saw

    **Where to look:** selfhost/check/generics.hero (`bind_missing`), selfhost/check/walk.hero (`compare`)
    **Why it matters:** one mistake, one message — §4.5's promise, kept by having exactly one owner per message

- [ ] **panel 105** | **What does the verifier refuse that the checker accepted?** Run `heroes build --dump-ir` on `tests/golden/run/fixedbugs-an-empty-result-bound-by-its-context.hero` and count the `function empty_of` lines. Then predict: if the checker had left `A` unbound, how many would there be, and which check in `selfhost/ir/phases.hero` would fire first — `no_generic_survives` or `no_error_survives`?

    **Where to look:** selfhost/ir/phases.hero, the golden
    **Why it matters:** the difference between a type parameter that survived and a type nobody resolved is the difference between two bugs with the same symptom

- [ ] **M-isolated-threads.0** | **How many shapes of allocation does the runtime have, and which one would lie?** Open `runtime/parts/alloc.c` and count the entry points below `hero_malloc_raw`. Three of them hand memory out. Then answer: if `hero_eq_queue` in `parts/array.c` had been given `hero_alloc` instead of `hero_grow_kept`, what exactly would every program that compares two deep arrays print at exit — nothing, a panic naming a heap block, or a panic naming a scratch buffer?

    **Where to look:** runtime/parts/alloc.c · runtime/parts/array.c:186-215 · design.md:2210-2222
    **Why it matters:** the leak gate is the only leak instrument this platform has, and an instrument that cries wolf is one people switch off

- [ ] **M-isolated-threads.0** | **A nested `hero_array_eq` answers `true` without comparing anything. Why is that not a bug?** Read `hero_array_eq` in `runtime/parts/array.c` from `if (hero_eq_running)` to the end, and say which one of these makes it sound: that `true` is the identity of `&&`, that the outermost call drains the queue before it returns, or that both are needed. Then say what `hero_eq_len = base` on the way out is for, given the answer is already known by then

    **Where to look:** runtime/parts/array.c (the comment above `HeroEqWork`) · docs/panel/076
    **Why it matters:** this is how a language with no recursion limit compares a value of unbounded depth, and it is the state that becomes per-thread the day Part 7.13 lands

- [ ] **M-c-callbacks.0** | **Why did handing a Heroes function to C need no trampoline, when three months of planning assumed it would?** Open `selfhost/emit/ctype.hero` at the `.function_ty` arm of `c_type`, and beside it look at what the compiler actually emits for `function worker(arg: ptr) -> ptr` — one line of `--emit-c` output. Then say which of these is the reason: that the emitter learned to write a function pointer in this milestone, that a Heroes function value carries no captured environment so it is one address and C's function pointer is one address, or that `ptr` and a function pointer are the same size on this machine

    **Where to look:** selfhost/emit/ctype.hero · docs/measurements/017 · docs/panel/111
    **Why it matters:** the deferral rested on cfront's fate and on the belief that a C11 backend could not express a thread's entry point, and the belief was never tested until it was cheap

- [ ] **M-c-callbacks.1** | **Three things stood between a correct `atexit` binding and a working program, and only one of them was in the checker. Name the other two.** The symptoms, in order, were `error[ffi_type]` at the declaration, then `unknown type name 'h_0fn_294870dd'` at exit 2, then nothing at all. Read `selfhost/emit/unit.hero` from the comment beginning *"A PROBE names types too"* down to the `extern_probes` call, and say what each of the two repairs is protecting: a list that had never included externs, an emission ORDER, both, or neither

    **Where to look:** selfhost/emit/unit.hero · selfhost/emit/decls.hero · tests/golden/fixedbugs/ffi-callback-typedef-reaches-the-probe.hero
    **Why it matters:** two defects that had been latent since the probe was written, invisible because until this milestone every type crossing the boundary spelled itself without a table

- [ ] **M-c-callbacks.2** | **The thread guard is emitted into your program, not built into the runtime. Read the four measurements and say why.** `docs/measurements/018` raced four runtimes on the compiler's own test suite: at every str/array/map through a thread-local, the same through `pthread_self()`, at the allocator alone, and a control. One of the four numbers is three times what the brief predicted. Then say which fact about C makes the emitted placement free for a program that binds no callback: that the compiler knows which functions it hands out, that C cannot call an address nobody gave it, or that both are the same fact said twice

    **Where to look:** docs/measurements/018 · selfhost/emit/callback_guard.hero · runtime/parts/thread.c
    **Why it matters:** the answer that won was not among the three that were priced, and it was found by asking where a foreign thread ENTERS rather than where the corruption shows

- [ ] **M-c-callbacks.3** | **`hero_thread_is_home` is never set to false, on any thread, anywhere. Why is the guard still correct?** Read `runtime/parts/thread.c` — it is short — and say which one is doing the work: that C11 zero-initialises a thread-local in each new thread, that `hero_args_set` runs before any Heroes code, or that a worker thread never calls `hero_thread_claim`. Then say what would break if the flag were an ordinary global instead

    **Where to look:** runtime/parts/thread.c · runtime/parts/os.c
    **Why it matters:** a guard whose false case is never written is the cheapest kind there is, and the whole of its correctness is in a sentence of the C standard

- [ ] **M-c-callbacks** | walkthrough offer: the day in six steps, in order — the permission and the guard as one commit (because the guard protects exactly what the permission admits), the two `/decide` answers applied in the session that asked, a commit with **no code at all** that is only the record of what Windows and Linux answered, the spec sentence landing at the bound a seat had predicted before it was written, panel 112 refusing the word, and defect 013 repaired by changing the question

    **Where to look:** docs/journal/033-c-callbacks.md · docs/panel/111, 112
    **Why it matters:** the shape worth seeing is that the feature was 32 lines and the day was everything standing next to them

- [ ] **M-c-callbacks** | golden ratification offer: the milestone's adversarial cases — `check/ffi-callback-position.hero` (the accepted form plus the three refused positions, each `#~` annotated), `run/ffi-callback-c-calls-back.hero` (the permission end to end, and the guard's QUIET direction), and the four `fixedbugs/` cases named after the defects that provoked them

    **Where to look:** tests/golden/check/ · tests/golden/run/ · tests/golden/fixedbugs/
    **Why it matters:** §9's marker is still on them, and one of the four has no `.expected` on purpose — say why before reading its comment

- [ ] **M-c-callbacks** | exit-quiz offer: the same program, `atexit` handed `nullptr`, does three different things on three machines. Name them, then say what that proves about where a compile-time refusal could ever have lived

    **Where to look:** docs/journal/033-c-callbacks.md § What surprised · tests/golden/fixedbugs/ffi-a-null-function-pointer-says-so.hero
    **Why it matters:** the answer is that NULL-tolerance is a property of the C library, not of the type and not even of the C function

- [ ] **M-c-callbacks** | mutation drill offer: `heroes mutate` over the callback path, now that a function type may stand in an `extern` parameter and nowhere else — the interesting mutants are the ones that move a function type from a parameter into a result, an `@` out-parameter or an `extern constant`, which are the three refusals one argument covers

    **Where to look:** selfhost/check/ffi.hero · selfhost/emit/callback_guard.hero
    **Why it matters:** a position rule has three ways to be wrong and the sitting only named two of them

- [ ] **M-isolated-threads step 3** | `hero_str_decref` used to be two lines — subtract one, then ask whether the answer is zero — and is now one call whose answer is the count BEFORE the subtraction, tested against 1. Two threads, both dropping the last two references. Say what the OLD pair could print that the new one cannot, and why counting the block twice is worse than not freeing it

    **Where to look:** runtime/parts/str.c `hero_str_incref`/`hero_str_decref` · runtime/heroes_runtime.h § the reference count
    **Why it matters:** it is the whole reason an atomic read-modify-write exists, in the smallest program that shows it

- [ ] **M-isolated-threads step 3** | Going UP the count asks for the weakest order there is and going DOWN asks for a stronger one. Read the two paragraphs in `str.c` and say, in your own words, what the thread that frees the block has to be able to SEE, and why the thread that merely takes a new reference has to see nothing

    **Where to look:** runtime/parts/str.c, the comment above `hero_str_incref`
    **Why it matters:** the asymmetry looks arbitrary and is the one thing in this file that cannot be guessed

- [ ] **M-isolated-threads step 3** | `cow.c:32` is `if (a->refcount == 1) return;` and step 3 did NOT repair it, on purpose. Two threads reach that line holding the same array. Walk what each one does and say what the program ends up printing — then say why making the READ atomic does not help

    **Where to look:** runtime/parts/cow.c, the comment above the test · docs/panel/111 § The runtime corrupts memory
    **Why it matters:** it is the difference between a race an atomic closes and one that needs a protocol, and the second kind is what is left of this milestone

- [ ] **M-isolated-threads step 3** | The leak gate's counter could have been made per-thread — it is faster and the race goes away. Step 2 measured why that is worse. A worker thread allocates a `str` and never gives it back: say what the program does under each of the two counters, and which of the two answers a person would rather get

    **Where to look:** runtime/parts/alloc.c, the comment above the two counters · DESIGN-LOG 2026-09-05 (step 2)
    **Why it matters:** the cheap repair and the right one differ by what the instrument can still SEE, which is a shape that recurs

- [ ] **M-isolated-threads step 4** | `parts/alloc.c` could have stored this thread's kept buffer in a `_Thread_local` pointer, which is two words, and stores it in a `pthread_key_t` instead, which is twenty lines. Read panel 111's transcript of `worker: live=3` and `destructor: live=0` at the SAME address, then say in your own words what a destructor can and cannot see

    **Where to look:** runtime/parts/alloc.c § the kept buffer · docs/panel/111 § Point 3 is broken twice over
    **Why it matters:** it is a fact about the machine that no amount of reading the C standard would have given, and both seats found it by running it

- [ ] **M-isolated-threads step 4** | `f64.c`'s locale is repaired with a compare-and-exchange and NOT by making it `_Thread_local`, which would have been one word. Two threads, both printing a number. Walk what each does under each of the two repairs, and say how much memory a program with a thousand short-lived threads loses under each

    **Where to look:** runtime/parts/f64.c § the one the sweep found
    **Why it matters:** the shorter edit is the wrong one here, and the reason is a shape that recurs: per-race and per-thread are different denominators

- [ ] **M-isolated-threads step 4** | The check written at step 3 went red on FOUR objects that step 4's own repair had just introduced, and the author had not noticed writing them. Open `runtime/parts/alloc.c`, find the key, the slot and the two once-guards, and say for each one whether you would have called it shared mutable state — then read the allow-list entry that says why each is safe anyway

    **Where to look:** tests/harness/suite_runtime.hero § Rule 3 · runtime/parts/alloc.c
    **Why it matters:** it is the difference between a list somebody wrote and a list taken from the world, and it is the thing CLAUDE.md §1's newest rule is about

- [ ] **M-declared-freer step 5** | A marked `@` cell does NOT hand C the address of the binding you wrote. Open `ir/inout.hero`'s `argument` and read the six lines under the mark test, then say which of these is what C is given: (a) `&problem`, the author's own `str?`; (b) `&$cell0`, a hidden `cstr` the compiler made; (c) a copy of the string. Then say what would go wrong with the one you did not pick

    **Where to look:** selfhost/ir/inout.hero § argument · tests/golden/ir/owned-cell.expected
    **Why it matters:** the whole design is that two different cells exist, and every other question about the feature follows from seeing that

- [ ] **M-declared-freer step 5** | The release for a marked `@` cell is paid by `call_or_release` and NOT by `emit_call`, which pays the ordinary write-backs. `sqlite3_exec` returns an `i32` and carries no mark on its result. Say what would have been skipped if the cell release had been hung off the result mark's branch, and name the program it would have been skipped for

    **Where to look:** selfhost/ir/owned_release.hero § call_or_release, release_cells
    **Why it matters:** the two marks are independent, and the milestone's own witness is the case that separates them

- [ ] **M-declared-freer step 5** | Before the call, the compiler stores `nullptr` into the hidden cell. Delete that store in your head and answer three things: what the C local holds instead; what `free` is then handed; and why running the program — at `-O0`, at `-O2` and under `-fsanitize=address,undefined` — did not notice, on the day it was measured

    **Where to look:** selfhost/ir/inout.hero § argument · tests/golden/ir/owned-cell.hero · docs/panel/116
    **Why it matters:** a guard no run can falsify is the kind that gets deleted by somebody reading a green test

- [ ] **M-declared-freer step 5** | Three ways of writing `owned` wrongly were `internal error` at exit 2, and the probe could catch none of them. Read `check/freer.hero`'s `refusal_for_mark` and say, for each of `owned bogus_name`, `owned my_heroes_function` and `owned two_parameter_extern`, which check catches it and why a probe structurally could not

    **Where to look:** selfhost/check/freer.hero § mark_refusals, refusal_for_mark · selfhost/emit/extern_probe.hero
    **Why it matters:** a probe is emitted per `extern`, so a name nobody declares has no probe — the shape of the instrument decides what it can never see

- [ ] **M-closures-verdict step 1** | `hero_desc_func` in `runtime/parts/desc.c` declares a size, a copy function and a drop function. Read those three fields and answer: how many bytes does a function value occupy, what does its copy function do, and what does its drop function do. Then say what each of the three would have to become if a function value could carry two captured `i64`s

    **Where to look:** runtime/parts/desc.c § hero_desc_func · runtime/heroes_runtime.h § HeroFn
    **Why it matters:** the descriptor is the one place the runtime states what a value IS, and a panel refused a feature because that statement and the feature disagreed by 24 bytes

- [ ] **M-closures-verdict step 1** | `--dump-ir` on a whole program prints `funcref` once for every place a function is used as a value. Run it on `examples/calculator/main.hero` and count them. Then run it on `selfhost/main.hero` and count them, and explain why the second number is not the compiler's total

    **Where to look:** selfhost/ir/print.hero § the .func_ref arm · selfhost/ir/emissions.hero
    **Why it matters:** the second number is 1 and the compiler has 190 modules — the reason is separate compilation, and mistaking it for a total is how a measurement goes wrong

- [ ] **M-closures-verdict step 1** | Three of the compiler's four production function values are in `selfhost/measure/pieces.hero`. Read `run_of` and the three predicates it is handed, and say what a single function taking a `kind` parameter buys over three near-identical loops. Then say which of the three predicates a closure would delete, and why the answer is none

    **Where to look:** selfhost/measure/pieces.hero § run_of, is_letter, is_digit, is_other
    **Why it matters:** §1.7's test is subtraction, and a feature that deletes nothing has to earn its place some other way

- [ ] **M-closures-verdict step 1** | `heroes check` refuses a comparison of two `str` with a message naming two types, while `ops.hero` accepts ten. Write the four-line program that provokes it, then find the three places one string is passed to `value_errors.bad_operand`, and say which of the three fires for `-"x"`

    **Where to look:** selfhost/check/ops.hero:81-90, :131, :168 · docs/work/DEFECTS.md 018
    **Why it matters:** errors are a deliverable here, and this one tells a reader holding a `u8` to convert a value that needed no conversion

- [ ] **M-closures-verdict close — walkthrough** | The route a capturing closure would have taken through the compiler, opened one file at a time: where the parser would put it and why a module of its own is `error[module_cycle]`, what `.func_ref` becomes in the IR, and the three tables that would each need a new case. Read `selfhost/ir/emissions.hero`'s `.func_ref` arm and `selfhost/emit/ctype.hero`'s comment, then say which of the two would have to change first

    **Where to look:** selfhost/ir/emissions.hero · selfhost/emit/ctype.hero · selfhost/grammar_expr.hero's knot map
    **Why it matters:** the refusal rests on a count of places, and the walk is what makes the count mean something rather than being a number

- [ ] **M-closures-verdict close — golden ratification** | Four cases were written this milestone and none is marked UNVERIFIED, because all four are repairs of measured defects rather than adversarial cases. Read them and rule on whether each one's `#~` annotations cover every diagnostic the program provokes — that is the half a regenerator cannot fake, and the last case needed two codes on one line before it did

    **Where to look:** tests/golden/check/fixedbugs-a-comparison-names-every-numeric-width.hero and the other three
    **Why it matters:** the annotation is the only part of a golden a machine cannot invent, so its correctness is the author's to confirm

- [ ] **M-closures-verdict close — mutation drill** | Reverse the four repairs one at a time, in your head, and say for each which suite goes red and which stays green. Three of the four were invisible to all twenty-three suites before their cases existed, and the fourth had a test pinning the wrong answer — find that one without looking it up

    **Where to look:** selfhost/check/ops.hero · selfhost/value_errors.hero · selfhost/resolve/names.hero · selfhost/check/render.hero
    **Why it matters:** a wrong message is not a wrong answer, and no instrument here was watching that class

- [ ] **M-closures-verdict close — exit quiz** | Four closed questions. (1) Why can a signature not state whether `filter` keeps or discards what its function accepts? (2) The compiler passes a function as a value at four production sites — name what all four have in common that makes closures useless to them. (3) A `total: i64 @ 0` captured by copy: say which of a read and a write a checker can refuse, and why the other cannot. (4) The free spec budget is 70 and not 71 — say which character in `suite_spec.hero` decides that

    **Where to look:** docs/panel/119-the-warning-that-does-not-fit.md · docs/panel/120-the-signature-cannot-say-which-half-it-keeps.md · docs/measurements/020-four-sites-in-fifty-five-thousand-lines.md · tests/harness/suite_spec.hero:204
    **Why it matters:** each of the four is a place this milestone's own coordinator or a seat got it wrong first

*******************************************************************************
