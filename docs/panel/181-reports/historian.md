*Written out by the coordinator from the seat's final message on 2026-09-28 at
01:56: the historian has no file-writing tool by its definition. The text below
is the seat's, unchanged.*

I have what I need. Reading spec § 5 settled the premise P2 depends on: a line that computes a value must be bound or discarded with `_ =`, and only a `()` line stands alone.

# Panel 181, historian

- **`verdict`**: **approve refusal**, advisory, by route (a) or (b), with precedent leaning to (a). I **object** to two things:
  - ratifying what the compiler does today (it continues at the same margin and refuses the deeper line);
  - recording route (c) as "Nim's precedent". Nim's written rule and Nim's parser disagree (B.4).

- **`precedents`**

  **Method.** Most quotations came through the fetch tool, which returns page text through a summarising model. Two I read myself from page images: the F# 4.1 specification (pp. 255-259) and the occam 2.1 manual (pp. 3-4).
  - **verified**: quoted at the cited page.
  - **derived**: I applied the quoted rule to the shape and did not run it.
  - **unverified**: I could not confirm it.

  I have no toolchain for any other language, so nothing outside Heroes was run. I did not use *Heroes of code* as a route for this sitting.

  **A. A line end outside brackets always ends the statement**
  1. **Python** (verified)
     - Language Reference 3.14.7: *"The end of a logical line is represented by the token NEWLINE. Statements cannot cross logical line boundaries except where NEWLINE is allowed by the syntax"*. Also: *"Expressions in parentheses, square brackets or curly braces can be split over more than one physical line without using backslashes."*, *"The indentation of the continuation lines is not important."* and *"Implicitly continued lines can carry comments."* (https://docs.python.org/3/reference/lexical_analysis.html).
     - The implicit-joining sentence is on the 1.5.2 page too (https://docs.python.org/release/1.5.2p2/ref/implicit-joining.html).
     - What `y = a +` does at depth zero (derived): a syntax error at every column, same margin and deeper alike. The exact message text is unverified; I found it only in secondary sources.
     - PEP 8 (created 05-Jul-2001): *"Long lines can be broken over multiple lines by wrapping expressions in parentheses. These should be used in preference to using a backslash"* (https://peps.python.org/pep-0008/).
     - Breaking before a binary operator became the suggestion for new code on 2016-04-15 (Guido van Rossum, https://mail.python.org/pipermail/python-dev/2016-April/144205.html). That advice presumes brackets, because outside them Python offers only the backslash (derived).
  2. **PEP 3125, "Remove Backslash Continuation"** (verified). Jim J. Jewett, created 29-Apr-2007, status Rejected (https://peps.python.org/pep-3125/, source https://raw.githubusercontent.com/python/peps/main/peps/pep-3125.rst). **This is the closest precedent to this sitting.**
     - Its *Alternate Proposals*: *"let any unfinished expression signify a line continuation, possibly in conjunction with increased indentation"*.
     - Objections on record:
       - *"expression continuation should not be confused with opening a new suite"*. This is panel 007's objection.
       - The obvious implementation *"would require allowing INDENT or DEDENT tokens anywhere"*.
     - Andrew Koenig, 2007-05-02, proposed Stu Feldman's EFL rule: *"if the last token in a line is one that lexically cannot be the last token in a statement, then the next line is considered a continuation"*. He placed it *"at a very low lexical level--even before the decision is made to turn a newline followed by spaces into an INDENT or DEDENT token"*, with *"the space after the newline ... just space"* (https://mail.python.org/pipermail/python-3000/2007-May/007237.html).
     - Guido replied the same day: *"I am worried that (as no indent is required on the next line) it will accidentally introduce legal interpretations for certain common (?) typos, e.g. `x = y+  # Used to be y+1, the 1 got dropped` / `f(x)`"* (https://mail.python.org/pipermail/python-3000/2007-May/007244.html).
     - The PEP's last word on this: *"Requiring that the continuation be indented more than the initial line would add both safety and complexity."*
     - Rejection notice: *"There wasn't enough support in favor, the feature to be removed isn't all that harmful, and there are some use cases that would become harder."*
     - Python today still joins lines only inside brackets or after a backslash (A.1).
     - **Heroes today implements half of Koenig's proposal** (derived from the shared brief's measurements). It skips the terminator after a non-ender, but still emits INDENT and DEDENT after one. So it keeps exactly the half Guido objected to: no indent required.

  **B. The column rule: an aligned line is a new item, whatever the last token**
  3. **Haskell 2010 Report** (verified)
     - §2.7: *"For each subsequent line, if it contains only whitespace or is indented more, then the previous item is continued (nothing is inserted); if it is indented the same amount, then a new item begins (a semicolon is inserted)"* (https://www.haskell.org/onlinereport/haskell2010/haskellch2.html).
     - §10.3: *"L (<n>: ts) (m : ms) = ; : (L ts (m : ms)) if m = n"* (https://www.haskell.org/onlinereport/haskell2010/haskellch10.html).
     - The shape (derived): the same column gives `a + ; 1`, a parse error, and parse-error(t) inserts only `}`, which does not rescue it. One column deeper continues.
  4. **F# 4.1 specification** (verified, page images; https://fsharp.org/specs/language-spec/4.1/FSharpSpec-4.1-latest.pdf)
     - §15.1.4: a `let`'s `=` *"introduces an offside line at the column of the first non-whitespace token after the = token"*.
     - §15.1.6: a SeqBlock is pushed *"Immediately after an infix token is encountered"*.
     - §15.1.8: a token *"directly on the offside line of a SeqBlock on the second or subsequent lines"* inserts `$sep`, and *"If a token is offside and a context cannot be closed, then an 'undentation' warning or error is issued"*.
     - §15.1.9's infix exceptions are for LEADING infix tokens: *"may be offside by the size of the token plus one"* and *"may align precisely with the offside line of the SeqBlock"*.
     - The shape (derived): `let y = a +` / `1` at the column of `let` is offside.
     - The specification's own front page says *"Discrepancies may exist between this specification and the 4.1 implementation."*
  5. **Elm** (message verified, Elm version unverified)
     - elm/error-message-catalog #255, 2018-03-11: a `}` at column 1 inside a declaration is refused with *"I need whitespace, but got stuck on what looks like a new declaration. You are either missing some stuff in the declaration above or just need to add some spaces here:"* (https://github.com/elm/error-message-catalog/issues/255).
     - It is the one diagnostic I found that names both readings of the line end.

  **C. The last-token rule without significant indentation (column irrelevant)**
  6. **Go** (verified)
     - Spec, *"Language version go1.27 (May 26, 2026)"*, § Semicolons: a semicolon is inserted after a line's final token only if it is an identifier, a literal, `break` `continue` `fallthrough` `return`, or `++ -- ) ] }` (https://go.dev/ref/spec).
     - So `a +` / `b` continues at any column (derived).
     - The line end is named in diagnostics: the scanner sets `s.lit = "newline"` on an inserted `_Semi`, and `syntaxErrorAt` prints `tok = p.lit` for it (https://raw.githubusercontent.com/golang/go/master/src/cmd/compile/internal/syntax/scanner.go, .../parser.go).
  7. **EFL** (partly verified). Koenig's description is in A.2. The fetch tool extracted this from Feldman's manual (PostScript, dated by the tool August 12, 1987; I could not read the file myself, so the wording is unverified): *"A statement is continued if the last token on a line is an operator, comma, left brace, or left parenthesis."* (https://stuff.mit.edu/afs/sipb/user/daveg/Info/Links/doc/unix.manual.progsupp2/06.efl/efl.PS).
  8. **JavaScript** (verified), ECMAScript §12.10 (https://read262.jedfox.com/ecmascript-language-lexical-grammar/automatic-semicolon-insertion/)
     - `a = b + c` / `(d + e).print()` *"is not transformed by automatic semicolon insertion, because the parenthesized expression that begins the second line can be interpreted as an argument list for a function call"*. This is the specified case of a reader seeing two statements where the language reads one.
     - The reverse case: `return` / `a + b` becomes `return; a + b;`.
     - Neither transfers to Heroes (derived). Heroes decides by the LAST token, as Go does, so a line-initial `(` after an ender is already separated.
  9. **Kotlin** (verified)
     - The grammar puts `{NL}` only after `additiveOperator`, and on both sides of `&&` and `||` (https://kotlinlang.org/spec/syntax-and-grammar.html).
     - The compiler lets only `DOT, SAFE_ACCESS, COLON, AS_KEYWORD, AS_SAFE, ELVIS, ANDAND, OROR` lead a line (https://raw.githubusercontent.com/JetBrains/kotlin/a2e835385431d7c09053c26dc5b960efd5620c56/compiler/psi/src/org/jetbrains/kotlin/parsing/KotlinExpressionParsing.java).
  10. **Ruby** (verified)
      - *"One expression might be split into several lines when each line can be unambiguously identified as 'incomplete' without the next one."*, with `x =` / `1 +` / `2` as the example (https://raw.githubusercontent.com/ruby/ruby/ruby_4_0/doc/syntax/layout.rdoc).
      - Ruby 4.0.0 (25 Dec 2025, Feature #20925): *"Logical binary operators (||, &&, and and or) at the beginning of a line continue the previous line, like fluent dot"* (https://www.ruby-lang.org/en/news/2025/12/25/ruby-4-0-0-released/). That is a move toward more continuation.
  11. **Swift** (partly verified): whitespace on both sides makes an operator infix (https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/ReferenceManual/LexicalStructure.md). How a line break ends a statement: unverified.

  **D. Significant indentation plus a last-token rule (the family closest to Heroes)**
  12. **occam 2.1 Reference Manual** (verified, page image; SGS-THOMSON, May 12, 1995; *"First published 1988 ... as the occam 2 Reference Manual"*; https://homepages.inf.ed.ac.uk/stark/ipp/manuals/occam-2-1.pdf)
      - p. 3: *"the indentation of each statement forms an intrinsic part of the syntax"*.
      - p. 4, *Continuation lines*: *"A long statement may be broken immediately after one of the following: an operator ..., a comma, a semi-colon, assignment `:=`, one of the keywords FROM, FOR, IS, RETYPES or RESHAPES. A statement can be broken over several lines providing the continuation is indented at least as much as the first line of the statement."*
      - So: an explicit continuator set, and the same column or deeper.
      - Whether the 1988 edition has the same text: unverified.
  13. **Koka** (verified; https://raw.githubusercontent.com/koka-lang/koka/master/doc/spec/spec.kk.md)
      - *"If the indentation is equal to the layout indentation, and the first lexeme on the line is not an expression continuation, a semicolon is inserted"*.
      - *"for long expressions and declarations, indented or aligned lines do not get braced or semicolons if"*. One of the two listed conditions: *"The previous line ends with a clear expression or declaration end continuation token, namely an operator (including `.`), an open brace ..., or `,`."*
      - So: the same column or deeper.
  14. **CoffeeScript** (verified)
      - `UNFINISHED`, commented *"Tokens that, when appearing at the end of a line, suppress a following TERMINATOR/INDENT token"* (https://raw.githubusercontent.com/jashkenas/coffeescript/main/src/rewriter.coffee).
      - `lineToken` calls `suppressNewlines()` both when the next line is at the same indent and when it is deeper (https://raw.githubusercontent.com/jashkenas/coffeescript/main/src/lexer.coffee).
      - **Issue #995**, opened 2010-12-29 (https://github.com/jashkenas/coffeescript/issues/995; dates from api.github.com): `if a and b or` / `    c and d` / `    doIt()` *"won't compile"*, because the body must sit at a different indentation from the continuation.
      - jashkenas, 2011-04-23: *"Yes, this behavior should be documented, but I'm afraid it's correct."*
      - jashkenas, 2013-03-05: *"I don't know of a good way to get rid of the in/outdebt hacks"*.
      - Closed 2017-04-26 without a fix: *"it doesn't appear to be affecting enough people"*.
      - **This is panel 007's block-header objection, lived for six years.**
  15. **Scala** (verified, with the Scala 3 behaviour derived)
      - 2.13 §1.2: a newline becomes `nl` only if the token before it *"can terminate a statement"*. The spec's own example continues `x < 0 ||` / `x > 10` at the same column, and *"With an additional newline character, the same code is interpreted as two expressions"* (https://scala-lang.org/files/archive/spec/2.13/01-lexical-syntax.html).
      - Scala 3 emits `<indent>` only *"if an indentation region can start at the current position"* (https://docs.scala-lang.org/scala3/reference/other-new-features/indentation.html).
      - So after a trailing operator, both the same column and a deeper one continue (derived).
      - Significant indentation and leading infix operators arrived together in Dotty 0.18.1-RC1, 2019-08-30, PR #7024 (https://nightly.scala-lang.org/blog/2019/08/30/18th-dotty-milestone-release.html).
  16. **Nim** (documentation verified; parser read at source, behaviour unrun)
      - Tutorial 2.2.12: *"As a rule of thumb, indentation within expressions is allowed after operators, an open parenthesis and after commas."* (https://nim-lang.org/docs/tut1.html).
      - Whitespace FAQ (last edited 2018-03-28): *"The only rule is that the continuing line must be indented at least one level above the first line."* (https://github.com/nim-lang/Nim/wiki/Whitespace-FAQ).
      - The grammar comments agree: `#| plusExpr = mulExpr (OP8 optInd mulExpr)*` and `#| optInd = COMMENT? IND{>}?`.
      - **But the code does not.** `parseOperators` calls `optPar(p)`, and `optPar` errors only `if p.tok.indent < p.currInd`. The grammar says `optPar = (IND{>} | IND{=})?`. This holds on `devel`, on `version-2-2` and at `v1.0.0` (https://raw.githubusercontent.com/nim-lang/Nim/version-2-2/compiler/parser.nim).
      - `dotExpr` does call `optInd`, whose `skipInd` demands `p.tok.indent > p.currInd`.
      - Derived: after a binary operator Nim admits the same column, and after `.` only deeper. **Nim's written rule is route (c); Nim's parser is not.**
      - Nim also closes a block header with `:` (`ifStmt = 'if' expr ':' stmt`, https://nim-lang.org/docs/manual.html), so a deeper line after a continued condition can never be taken for the block's opener. Heroes has no such token.

  **E. Changes of direction, and incidents**
  - **Toward more continuation**: Scala (2019, leading infix) and Ruby (2025, leading logical operators).
  - **Considered and not adopted**: Python (2007).
  - **Not found**: a language that accepted a same-column continuation and later refused it, or a report of a silent misreading from a same-column continuation in an indentation-sensitive language. I searched Nim and CoffeeScript issues and general search on *same indentation*, *invalid indentation*, *continuation*, *trailing operator*. This is a question, not a finding.
  - The real incident I found (CoffeeScript #995) is a real program refused, not a misreading. The misreading cases are specified (ECMAScript) or hypothetical (Guido).
  - **Heroes itself**
    - Panel 007, ratified 2026-08-03 (`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/007-terminator-enders.md`), deferred "Nim's rule". Its premise, that Nim's rule is one rule, is what B.4 above complicates.
    - Panel 180's R3 (`docs/design/design.md:1967-1975`, provisional) already refuses, inside brackets, *"a token that would have gone on with the line above ... with the line end named as the cause"*.

- **What the precedents say about the routes**
  - **(a) Refuse in the lexer, planting the terminator.** This is Python's architecture: NEWLINE is a tokenizer product outside brackets, and statements cannot cross it. In the last-token languages I read, the line-end decision also sits at the lexical level: Go's scanner (`nlsemi`), CoffeeScript's lexer, and Koka's layout pass. Koenig argued that level is the easy one. Naming the line end in the message is Go's practice and Heroes' own R3. Reporting it FROM the lexer is a Heroes choice (design.md §4.17), not a precedent question.
  - **(b) Refuse in the parser, from line positions.** This is Nim's architecture: tokens carry `indent`, and the parser checks them (*"the operator itself must not start on a new line"*). Kotlin (`newlineBeforeCurrentToken()`) and Swift (`isFollowingLParen`) do the same. It works. But Nim is also the example of a written rule and a parser check drifting apart, and (b) would put a second copy of the rule beside the lexer's last-token rule.
  - **(c) Nim's rule, one level deeper, same margin refused.** Written by Nim's FAQ and weighed by PEP 3125 (*"safety and complexity"*). Not enforced by Nim's parser (unrun). CoffeeScript shows the cost in a language where, as in Heroes, indentation alone opens a block: #995 is `s06`/`s15` one level down. Every indentation-sensitive language I found with trailing-operator continuation admits the same column: occam, Koka, CoffeeScript, Scala 3, and Nim's parser. If 007-bis ever sits, the package with a written record is occam's: an explicit set, the same column or deeper, and the statement's margin taken from its first line.
  - **The Fix.** Two repairs have precedent:
    - parentheses, PEP 8's prescription, which keeps the author's break;
    - joining the lines, which keeps the author's statement.
    Guido's example is a third reading, a dropped operand, where neither fix is the program meant. So precedent supports `certain` only where the next line cannot stand alone. In Heroes that excludes a `()` call line, which *"stands alone"* (spec § 5, read) (derived).
  - **The shapes beside it**
    - **Blank line**: Scala's two newlines always separate, even after a trailing operator. That is precedent for the `use` glued across a blank line.
    - **Shallower line (s16)**: Haskell closes the block, and Scala 3 suppresses `<outdent>` only after `then else do catch finally yield match`, not after an operator (derived). Refusing it therefore has precedent.
    - **Comment between the lines**: Python allows comments on implicitly continued lines and forbids them after a backslash.
    - **The `error` token at a line end**: I found nothing sourced to add.

- **The shortest wording**
  - **For refusal**, the shape is Python's: *"The end of a logical line is represented by the token NEWLINE."* The Heroes spec already says *"NEWLINE ends a statement"* and *"Inside `(` `[` `{` a NEWLINE never ends a statement"*, but its last-token rule says *"A line there"*, meaning inside brackets. What is missing is the depth-zero clause. A precedent-shaped candidate is *"Outside brackets every line ends with a NEWLINE."* Unpriced: that belongs to the spec-warden.
  - **For admission**, occam's two sentences, the list plus *"indented at least as much as the first line of the statement"*, or Nim's FAQ sentence for deeper-only. Ruby's single sentence (*"unambiguously identified as 'incomplete'"*) is the shortest of all and names no set, which is the soundness failure panel 007 found.

- **`argument`**: Python refuses a depth-zero break at every column; in 2007 it weighed admission and declined (PEP 3125), Guido objecting to the same-margin case, "as no indent is required on the next line". Every indentation-sensitive language I found that continues after a trailing operator (occam, Koka, CoffeeScript, Scala 3, Nim's parser; read, not run) admits the same column and a deeper one. Heroes today admits the same margin and refuses deeper: I found that nowhere. Guido's hazard may not transfer (spec § 5 refuses a value line standing alone; P2 scores it), so precedent does not forbid admission; the package it offers is occam's (explicit set, same column or deeper), 007-bis's question. Until then, refusal is the ratified rule.

- **`condition`**: any of these would change my reading.
  1. A language reference that documents what the compiler does today (same column admitted, deeper refused) with years of use. Today's behaviour would then be a precedent rather than an accident.
  2. `nim check` refusing a same-column operand after `+` (P1 falsified). Route (c) would then be a precedent that held, not only one that was written.
  3. A report of occam, Koka, CoffeeScript or Scala 3 programs silently misread through a same-column continuation. That would strengthen refusal on precedent. If P2 holds, the case against the same margin rests on the ratified rule and Principle 0 (zero hits in `selfhost/`, per the shared brief), not on Python's history.
  4. Not a precedent, but the ratified trigger: panel 007's baseline showing that models produce the break.

- **Predictions**
  - **P2, scoreable today with the trunk's compiler; unrun by me.** This is Guido's 2007 typo translated:
    ```
    function show(n: i64)
        print(n)

    function main()
        a: i64 = 5
        y = a +
        show(n: a)
        print(y)
    ```
    I predict `heroes check` exits 1 with a type diagnostic on `a + show(n: a)`, meaning the hazard that shaped Python's choice is loud in Heroes. Exit 0 falsifies it.
  - **P1, the one I stand on.** Nim 2.2 accepts `let y = a +` / `1` with `1` at the column of `let` inside a proc body. It refuses `let n = xs.` / `len` at that column with *invalid indentation*. In other words, Nim's parser admits after an operator what its FAQ says it refuses, and enforces the FAQ only after `.`. It is falsified by `nim check` refusing the first program. The URL that would falsify it is `parseOperators` calling `optInd` instead of `optPar`: https://raw.githubusercontent.com/nim-lang/Nim/version-2-2/compiler/parser.nim

Files read:
- `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/181-briefs/historian.md`
- `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/181-briefs/00-shared.md`
- `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/181-briefs/compiler-engineer.md`
- `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/007-terminator-enders.md`
- `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/180-reports/historian.md`
- `/Users/joseph/Temp/heroes/heroes-lang/docs/design/design.md` (lines 1911-1982)
- `/Users/joseph/Temp/heroes/heroes-lang/spec/heroes-spec.md` (lines 1-218)
- `/Users/joseph/Temp/heroes/heroes-lang/docs/work/DEFECTS.md` (items 116, 118)

Sources: every URL used is cited inline above.
