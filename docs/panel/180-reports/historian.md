# Panel 180, historian

(Written to disk by the coordinator from the seat's final message, verbatim;
the seat has no write tool.)

- **`verdict`**: **approve route (b)**, advisory. I also object to **route (c) as it is worded**.

- **`precedents`**

  Method. Every quotation below came through the fetch tool, which returns page text through a summarising model. The one exception is ES3: I read those lines myself from the PDF page images (pp. 21-24). "Derived" means I read it off the cited rule or source and did not run it. I did not use *Heroes of code* as a route for this sitting.

  **A. The languages, one entry each.** For each: what it allows inside brackets before a closer, before an operator and before a comma.

  - **Go** (verified)
    - Rule 1 in the spec: *"a semicolon is automatically inserted into the token stream immediately after a line's final token if that token is"* an identifier; a literal; `break` `continue` `fallthrough` `return`; or `++ -- ) ] }`.
    - Rule 2: *"a semicolon may be omitted before a closing ")" or "}"."*
    - Source: https://go.dev/ref/spec, header *"Language version go1.27 (May 26, 2026)"*.
    - Older wording: *"at the end of a non-blank line if the line's final token is"*, already listing `]`. It is in weekly.2009-12-22 (https://raw.githubusercontent.com/golang/go/weekly.2009-12-22/doc/go_spec.html) and still in the go1.4.2 copy (https://stuff.mit.edu/afs/sipb/project/golang/arch/go1.4.2-linux-amd64/doc/go_spec.html). When the wording changed is unverified.
    - History: Rob Pike's proposal is dated Dec 10, 2009 by https://golang.design/history/ and Dec 9 by the thread; the difference is unresolved. It landed in release.2009-12-22, where Russ Cox wrote *"semicolons are now implied between statement-ending tokens and newline characters"* (https://groups.google.com/g/golang-nuts/c/CimhWZj-EYY).
    - The scanner keeps an `nlsemi` flag and, per the fetched source, tracks no bracket depth (https://go.googlesource.com/go/+/refs/heads/master/src/cmd/compile/internal/syntax/scanner.go).
    - **Before a closer:** refused without a trailing comma. The test file https://go.dev/test/syntax/composite.go (2012) expects *"need trailing comma before newline in composite literal"*. The 2009 proposal already named the hazard: *"a semicolon is mistakenly inserted after the last element of a multi-line list if the closing parenthesis or brace is on a separate line"*. Its remedy was trailing commas in parameter and argument lists (https://groups.google.com/g/golang-nuts/c/XuMrWI0Q8uk).
    - **Before an operator:** refused (derived). Brian Stuart objected in the same thread on 2009-12-10: *"Most mathematical typesetting I've seen puts the operator at the beginning of the line"*.
    - **Before a comma:** refused. Comma-first style fails (golang-nuts, 2010-04-26, https://groups.google.com/g/golang-nuts/c/InoEeRjuorQ/m/AXLxkjH46XMJ).
    - Rule 2 lets a programmer drop a written `;` before `)` or `}`. It does **not** let an inserted one through before an argument list's `)`. The Arguments production, `"(" [ ... [ "," ] ] ")"`, has no `;` in it.
  - **Python** (verified)
    - *"Expressions in parentheses, square brackets or curly braces can be split over more than one physical line without using backslashes."* and *"There is no NEWLINE token between implicit continuation lines."* (https://docs.python.org/3/reference/lexical_analysis.html, 3.14.7).
    - The same text is in the 1.5.2 reference (https://docs.python.org/release/1.5.2p2/ref/implicit-joining.html). 1.5.2 was released 13 April 1999 (https://www.python.org/download/releases/1.5/). Anything earlier is unverified.
    - Allows a break anywhere inside brackets.
    - PEP 8 recommends breaking **before** binary operators for new code (https://peps.python.org/pep-0008/). Guido van Rossum announced that change on 2016-04-15 (https://mail.python.org/pipermail/python-dev/2016-April/144205.html). Black does the same: *"will break a line before a binary operator"* (https://black.readthedocs.io/en/stable/the_black_code_style/current_style.html).
  - **JavaScript** (verified)
    - ES3 §7.9.1, December 1999: a semicolon is inserted before an *offending token* separated by a LineTerminator. It is *"never inserted automatically if the semicolon would then be parsed as an empty statement or ... in the header of a for statement"*.
    - The restricted productions are postfix `++`/`--`, `continue`, `break`, `return` and `throw`.
    - §7.9.2 gives `a = b + c` / `(d + e).print()` as *not* transformed (https://www.ecma-international.org/wp-content/uploads/ECMA-262_3rd_edition_december_1999.pdf).
    - The current list adds `yield`, `=>`, `async` and `using`, plus a do-while special case. That comes from MDN, a secondary source (https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar). The fetch tool could not render tc39.es. When the do-while clause entered, and why, is unverified.
    - Inside brackets a break is allowed anywhere, except that the restricted productions apply at every depth. This is a third rule, driven by the parser.
  - **Kotlin** (verified)
    - Lexer grammar: `LPAREN: '(' -> pushMode(Inside);`, `LSQUARE: '[' -> pushMode(Inside);`, `Inside_NL: NL -> channel(HIDDEN);`, and `{` pushes the default mode again (https://raw.githubusercontent.com/Kotlin/kotlin-spec/release/grammar/src/main/antlr/KotlinLexer.g4).
    - The page grammar ("Version 1.9-rfc+0.1") writes `{NL}` into each production: `'(' {NL} expression {NL} ')'`, and `additiveOperator {NL}` with the newline only *after* the operator (https://kotlinlang.org/spec/syntax-and-grammar.html). My fetch found no "Inside", "pushMode" or "HIDDEN" on that page, so whether a page reader is ever told newlines vanish inside `(` `[` is a question.
    - The compiler calls `disableNewlines()` in the parenthesised expression, the argument list and indexing. Outside brackets only `DOT, SAFE_ACCESS, COLON, AS_KEYWORD, AS_SAFE, ELVIS, ANDAND, OROR` may start a continuation line, so **not `+` or `-`** (https://raw.githubusercontent.com/JetBrains/kotlin/a2e835385431d7c09053c26dc5b960efd5620c56/compiler/psi/src/org/jetbrains/kotlin/parsing/KotlinExpressionParsing.java). It follows that at statement level `a` / `- b` is two statements (derived).
    - Inside `(` `[`: Python's rule.
  - **Scala** (verified)
    - 2.13 §1.2: a newline becomes `nl` only if the token before it *can terminate* a statement and the token after it *can begin* one. `, . ; : = => ... [ ) ] }` cannot begin one. Newlines are *"disabled"* between matching `( )` and `[ ]` (https://www.scala-lang.org/files/archive/spec/2.13/01-lexical-syntax.html).
    - Scala 3 adds leading infix operators, which require whitespace after the operator (https://docs.scala-lang.org/scala3/reference/changed-features/operators.html). The version that introduced them is unverified.
    - Inside `( [`: anything goes. Inside `{ }`: a two-sided test.
  - **Swift** (verified)
    - *"Whitespace ... distinguish[es] between prefix, postfix, and infix operators, but is otherwise ignored"*, and line feed counts as whitespace (https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/ReferenceManual/LexicalStructure.md).
    - *"If a period appears at the beginning of a line, it's understood as part of an explicit member expression, not as an implicit member expression."* (Expressions.md in the same repository).
    - In the compiler, `isFollowingLParen` is *"an l_paren token that does not start a new line"* (https://raw.githubusercontent.com/swiftlang/swift/main/include/swift/Parse/Token.h). The reference does not say how a line break ends a statement (Statements.md, as fetched): a question.
    - The rule ignores bracket depth.
  - **Odin** (source read, behaviour unrun)
    - There is no spec, and the overview and FAQ state no newline rule.
    - The designer, Ginger Bill, on 2026-02-19: *"Odin's rules, which are very similar to Python's, are to ignore newline-based 'semicolons' within brackets"* (https://www.gingerbill.org/article/2026/02/19/choosing-a-language-based-on-syntax/).
    - Source: `advance_token` skips a `;` whose text is `"\n"` when `p.expr_level > 0`. But `parse_call_expr` sets `p.expr_level = 0`. `expect_closing_token_of_field_list` accepts a newline before the closer without complaint (https://raw.githubusercontent.com/odin-lang/Odin/master/core/odin/parser/parser.odin). The C++ parser has `ignore_newlines(f) { return f->expr_level > 0; }` (https://raw.githubusercontent.com/odin-lang/Odin/master/src/parser.cpp).
    - `-strict-style` *"Errs on missing trailing commas followed by a newline"* (https://github.com/odin-lang/Odin/wiki/Compiler-Flags).
    - Derived: a group or index follows Python's rule; call arguments follow Go's rule plus a newline allowed before the closer. **This is Heroes' mechanism and Heroes' irregularity.**
  - **Nim** (partly verified)
    - The tutorial, version 2.2.12: *"As a rule of thumb, indentation within expressions is allowed after operators, an open parenthesis and after commas."* (https://nim-lang.org/docs/tut1.html). My search of the manual for continuation wording found none.
    - The grammar has `OP1 optInd` and `optPar ']'` (https://raw.githubusercontent.com/nim-lang/Nim/devel/doc/grammar.txt).
    - `parseOperators` carries the comment *"the operator itself must not start on a new line"* and has no depth guard (https://raw.githubusercontent.com/nim-lang/Nim/devel/compiler/parser.nim). Whether the lexer marks a token as starting a line when it is inside brackets is unverified.
  - **V** (verified): *"When a function signature spans multiple lines, commas between parameters are optional"* (https://raw.githubusercontent.com/vlang/v/master/doc/docs.md). I found no rule about a break before an operator.
  - **CoffeeScript** (verified): *"When each property is listed on its own line, the commas are optional."* (https://coffeescript.org/). How it treats a line that starts with an operator: unverified.

  **B. Where the spec said one thing and the implementation did another** (all verified unless marked)
  - **kotlin-spec #40, opened 2019-12-15.** The spec's ANTLR grammar rejected a newline inside a `${ }` hole that the compiler accepted. The issue is closed; how it was repaired is unverified (https://github.com/Kotlin/kotlin-spec/issues/40).
  - **ANTLR grammars-v4 #584, 2017-03-29.** A third-party Go grammar made semicolons optional everywhere, which is looser than gc. It was repaired by implementing the insertion rule (https://github.com/antlr/grammars-v4/issues/584).
  - **Effective Go** lists `break continue fallthrough return ++ -- ) }`, without **`]`**, while the spec has listed `]` since at least 2009-12-22. It still differs today (https://go.dev/doc/effective_go). I found no issue about it.
  - **Go's error messages** had to learn to name the newline:
    - #1006, 2010-08-07: users were shown `found ';'` for a line break.
    - #3008, 2012-02-13, filed by rsc, asked for *"missing ',' before newline"*.
    - Griesemer's change of 2022-03-31 introduced *"possibly missing comma or )"*.
    - Links: https://github.com/golang/go/issues/1006, https://github.com/golang/go/issues/3008, https://groups.google.com/g/golang-checkins/c/wymQREfFRsU.
  - **Heroes itself.** design.md §4.15 (`/Users/joseph/Temp/heroes/heroes-lang/docs/design/design.md:1932-1938`) records an earlier copy of Go's token list that *"dropped"* entries. Defect 104 is the second time this rule has drifted in transcription.

  **C. What the precedents say about the three routes**
  - **(a) The spec lists the exceptions.** No spec I surveyed lists newline exceptions in prose. Kotlin writes the newline into each production, and its own spec repository shows exactly that form drifting away from the compiler (#40). Heroes has no instrument that compares a production to the parser (`/Users/joseph/Temp/heroes/heroes-lang/.claude/rules/spec-shape.md`).
  - **(b) The compiler is made uniform.** Go is uniform. It paid in trailing commas and in more than a decade of error-message repair, but never with a silent misreading. Odin is non-uniform in the same way Heroes is today.
  - **(c) Python's rule inside brackets.** This is the majority rule: Python, Kotlin, Scala, JavaScript, and Swift in effect. But none of those languages uses a newline to separate elements inside brackets, and Heroes does (`Sep = "," | NEWLINE`, spec § 7). Precedents answer a line-initial `-` or `.` explicitly: Swift with a whitespace rule and a sentence about `.`, Scala 3 with whitespace, Kotlin with a whitelist of operators, Go by refusing.
  - **(d) A route none of the briefs names:** Scala's two-sided test. The lexer plants no terminator before a token that cannot begin an element. I have not priced it.

  **D. The shortest wording.** Go spends **zero words** on brackets: its one lexical rule ignores depth, and the `[ "," ]` in each production carries the rest. Every spec I found that uses a last-token rule enumerates its tokens (Go, Scala). The Heroes spec states no line-ender list at all: I read it in full, and the list is only in design.md §4.15. So a Go-style statement of the rule has to pay for that list. No spec states the shape Heroes actually implements, Go's terminators plus an allowance before closers; Odin, the one language that does the same, has no spec.

  **E. Registered predictions**
  - **P1, scoreable today.** In a seat directory, `heroes run` on this program:
    ```
    function main()
        a = 5
        b = 3
        xs = [a
            - b]
        print(xs.len())
    ```
    I predict it exits 0 with no diagnostic and prints `2`, a silent two-element reading. Python, JavaScript, Kotlin, Scala and Swift continue the expression here (derived from their rules), and Go refuses it. P1 is falsified by a non-zero exit or by `1`.
  - **P2, scoreable if a Go or Odin toolchain exists; unrun.**
    - Go refuses `x := (1` / `+ 2)`. Falsifier: a depth clause added to https://go.dev/ref/spec § Semicolons.
    - The brief's own sample prediction ("no language using Go's rule accepts a break before a binary operator inside parentheses") is predicted **false for Odin**: `odin check` should accept `x := (1` / `+ 2)` and reject `f(1` / `+ 2)`. The source lines cited in A are what would falsify it.

- **`argument`**: Go's rule is safe in Go because no expression bracket accepts a semicolon: a planted terminator inside one is always a loud syntax error, paid for in trailing commas and diagnostic repair. Heroes kept the depth-blind terminator and made NEWLINE a separator inside `[ ]` and `{ }`, a combination none of the surveyed specifications states. That, not the spec sentence, is the departure no ruling covers: `[a` / `- b]` is predicted to read silently as two elements, where Python, JavaScript, Kotlin, Scala and Swift continue the expression and Go refuses it. Route (b) fits precedent: one uniform rule, stated with its line-ender list as Go's spec does, and an explicit answer for a line-initial `-` after a NEWLINE separator.

- **`condition`**: I would change my reading if any of these turned up:
  1. P1 fails, meaning the shape is refused or read as one element. The argument would then shrink to the spec's missing line-ender list.
  2. A page showing CoffeeScript, V, or another language with newline-separated elements reading a line-initial `- b` as a new element for years with no defect filed. That would make the silent reading a precedent rather than a departure.
  3. A ruling in `docs/panel/` or `docs/records/log/`, under wording the brief's grep did not search for, that weighed terminators ignoring depth plus NEWLINE separators against the leading-`-` shape. The departure would then be deliberate.

  Running Odin on the two P2 shapes would confirm the Odin reading or withdraw it.

Files: `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/180-briefs/historian.md`, `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/180-briefs/00-shared.md`, `/Users/joseph/Temp/heroes/heroes-lang/spec/heroes-spec.md` (lines 9-12, 198-207), `/Users/joseph/Temp/heroes/heroes-lang/docs/design/design.md` (lines 1932-1948).

Sources: all URLs are cited inline above. Additional ones consulted: [Go 101 line-break rules](https://go101.org/article/line-break-rules.html), [go issue 20511](https://github.com/golang/go/issues/20511), [go issue 35263](https://github.com/golang/go/issues/35263), [issue49205.go](https://go.dev/src/cmd/compile/internal/syntax/testdata/issue49205.go), [interglacial ES3 mirror](https://interglacial.com/javascript_spec/a-7.html), [Nim Whitespace FAQ](https://github.com/nim-lang/Nim/wiki/Whitespace-FAQ/4efb0026304c269d7806a5d85f9a6e397f7c3b24), [V issue 22021](https://github.com/vlang/v/issues/22021), [Scala 3 indentation](https://docs.scala-lang.org/scala3/reference/other-new-features/indentation.html), [Odin overview](https://odin-lang.org/docs/overview/), [Odin FAQ](https://odin-lang.org/docs/faq/), [Sonar on parsing Kotlin](https://www.sonarsource.com/blog/parsing-kotlin/).

## Scored by the coordinator, 2026-09-27, before the synthesis

- P1: `heroes run` on the program above, the trunk's compiler at `29ed5601`: exit 0, prints `2`. **Held.** Three shapes beside it, run the same way, read silently too: `[base * qty` / `- discount]` prints 2, `[a` / `(b)]` prints 2, `[a` / `!b]` prints 2.
- P2: unrun (no Go or Odin toolchain was used).
