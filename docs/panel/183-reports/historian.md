# Panel 183, historian (advisory, no veto)

Written 2026-09-30 by the historian seat, as it went. This seat has no shell:
every precedent below is what a fetched page says, with its URL, and anything
that would need a run is marked *unrun*. Where the completeness critic ran a
compiler locally (`docs/panel/183-reports/completeness-critic-briefs.md` § 1.4
and § 3), that is cited as the critic's run, never as mine. Source code was
read at the branch or tag named in its URL on 2026-09-30; `main`, `master` or
`devel` means that day's head. A sentence marked *reading* is my inference from
a verified source, not a fact any source states. *Heroes of code* is not cited
anywhere below, and no claim rests on it.

## Task 1: where production compilers bound an unclosed opener's reach

### CPython, 3.10 and later: the reach is the file, the report is the opener

- **Where it lands: at the opener.** *What's New In Python 3.10*: "the
  interpreter now includes the location of the unclosed bracket of parentheses
  instead of displaying *SyntaxError: unexpected EOF while parsing* or pointing
  to some incorrect location", with the example `'{' was never closed` under
  line 1 where 3.9 had said `invalid syntax` under line 3; "inspired by previous
  work in the PyPy interpreter"; bpo-42864 (Pablo Galindo) and bpo-40176
  (Batuhan Taskaya). **Verified**:
  https://docs.python.org/3/whatsnew/3.10.html. bpo-42864 was filed 2021-01-08
  and closed *fixed* 2021-01-20, target 3.10. **Verified**:
  https://bugs.python.org/issue42864. The critic's run: 3.9.6 prints `invalid
  syntax`, 3.13.15 and 3.14.7 print `'[' was never closed` at the opener.
- **How far the reach goes: to end of file, and no line ends it.** Brackets live
  in the tokenizer: indentation is processed only `if (!blankline && tok->level
  == 0)`, so no INDENT or DEDENT is made inside brackets; at `EOF` with
  `tok->level` non-zero it returns `ERRORTOKEN`; a closer pops the stack, and a
  closer of the wrong kind is its own tokenizer error, *closing parenthesis '%c'
  does not match opening parenthesis '%c' on line %d*, while one with nothing
  open is *unmatched '%c'*. **Verified** (3.12 branch):
  https://raw.githubusercontent.com/python/cpython/3.12/Parser/tokenizer.c. The
  What's New example is itself a column-0 statement line read inside the `{`.
- **When the opener's report wins over the parser's.** After a parser failure,
  `_PyPegen_tokenize_full_source_to_check_for_errors` ("Tokenize the whole input
  to see if there are any tokenization errors such as mistmatching parentheses")
  runs the tokenizer to the end, and replaces the parser's error with
  *'%c' was never closed* only `if (current_err_line > error_lineno)`: the parser
  failed on a line after the innermost open bracket's line. **Verified**:
  https://raw.githubusercontent.com/python/cpython/3.12/Parser/pegen_errors.c.
  The same test is in 3.10's `_PyPegen_check_tokenizer_errors`, and 3.10's
  `raise_unclosed_parentheses_error` names `parenstack[level-1]`, the innermost
  opener. **Verified**:
  https://raw.githubusercontent.com/python/cpython/3.10/Parser/pegen.c. The
  parser's own error position is "the furthest token that was attempted to be
  matched but failed". **Verified**:
  https://github.com/python/cpython/blob/main/InternalDocs/parser.md.
- ***Reading*, unrun by this seat.** A stray closer of the same kind later in
  the file pops the open bracket, the tokenizer ends at level 0, and the opener
  is never named: question (b)'s shape, unsolved in CPython. That CPython
  reports one SyntaxError and nothing after it is the mechanism (an exception);
  I found no page saying it in those words: *unverified as a sentence*.

### rustc: brackets matched in the lexer, reported at end of file, then it stops

- **Where it lands: at end of file, with the opener labelled.** `eof_err` in the
  token-tree reader: *this file contains an unclosed delimiter* on the EOF span,
  a label *unclosed delimiter* on each open delimiter, at most
  `UNCLOSED_DELIMITER_SHOW_LIMIT = 5`. **Verified**:
  https://raw.githubusercontent.com/rust-lang/rust/master/compiler/rustc_parse/src/lexer/tokentrees.rs.
  The critic's run: rustc 1.90.0 on a missing `}` printed that error at 10:3, the
  end of the file, with the opener labelled.
- **How far the reach goes: to end of file, or to a closer an outer opener
  matches.** The comment: "If the incorrect delimiter matches an earlier opening
  delimiter, then don't consume it (it can be used to close the earlier one).
  Otherwise, consume it. E.g., we try to recover from: `fn foo() { bar(baz( }`".
  Asked directly, the file refers to no keyword and consults indentation only
  through `same_indentation_level`, used to pick which opener to name ("high
  likelihood of these two corresponding"). **Verified**: same URL.
- **Indentation as a label, not a boundary.** PR #104012 (merged 2023-01-28)
  added `report_suspicious_mismatch_block`, whose labels are *this delimiter
  might not be properly closed...* and *...as it matches this but it has
  different indentation*, after a report that an extra `{` at line 1420 was
  reported at line 2605. **Verified**: https://github.com/rust-lang/rust/pull/104012,
  https://raw.githubusercontent.com/rust-lang/rust/master/compiler/rustc_parse/src/lexer/diagnostics.rs.
- **What happened next: rustc stopped recovering past it.** PR #108297, *Exit
  when there are unmatched delims to avoid noisy diagnostics* (chenyukang, from
  a discussion in #104012, merged 2023-03-01, milestone 1.69.0). **Verified**:
  https://github.com/rust-lang/rust/pull/108297. PR #108606 (merged
  2023-03-02) added the tests for #104367 and #105209: "After landing #108297,
  these issues are resolved." **Verified**:
  https://github.com/rust-lang/rust/pull/108606. #104367 (opened 2022-11-13) is
  a five-line program with a mismatched delimiter that printed the
  unclosed-delimiter error, four parse errors and an internal compiler error.
  **Verified**: https://github.com/rust-lang/rust/issues/104367. Today
  `lex_token_trees` returns `Err` when any delimiter is unmatched,
  `new_parser_from_source_file` builds no `Parser` from an `Err` (`?`), and
  `unwrap_or_emit_fatal` emits the errors and raises `FatalError`. **Verified**:
  https://raw.githubusercontent.com/rust-lang/rust/master/compiler/rustc_parse/src/lexer/mod.rs,
  https://raw.githubusercontent.com/rust-lang/rust/master/compiler/rustc_parse/src/lib.rs.
  So in a file with an unmatched delimiter rustc reports no later mistake.

### Go: the line ends the reach, and the report is where the parser stopped

- **The reach is one line, by the semicolon rule.** A semicolon is inserted
  "immediately after a line's final token" if it is an identifier, a literal,
  `break` `continue` `fallthrough` `return`, `++` `--` `)` `]` `}`; the rule
  names no exception inside brackets. **Verified** (go1.27, 2026-05-26):
  https://go.dev/ref/spec#Semicolons. *Reading*: `double(3` at a line's end is
  followed by a `;`, and only a line ending in `,`, an operator or an opener
  carries the reach to the next line.
- **Where it lands: at the point of failure, never at the opener.** The `gc`
  parser (`cmd/compile/internal/syntax`): `list` reports *in %s; possibly
  missing %s or %s* and calls `p.advance(_Rparen, _Rbrack, _Rbrace)`; its test
  data show the form *unexpected name d in argument list; possibly missing comma
  or )*. **Verified**:
  https://raw.githubusercontent.com/golang/go/master/src/cmd/compile/internal/syntax/parser.go,
  https://go.dev/src/cmd/compile/internal/syntax/testdata/issue49205.go. The
  newline form as users meet it: *syntax error: unexpected newline in composite
  literal; possibly missing comma or }*. **Verified**:
  https://github.com/manticoresoftware/manticoresearch-go/issues/5.
- **Statement keywords end the skip inside a function, declaration keywords at
  top level.** "The stopset contains keywords that start a statement. They are
  good synchronization points in case of syntax errors and (usually) shouldn't
  be skipped over": `break const continue defer fallthrough for go goto if
  return select switch type var`; "The stopset is only considered if we are
  inside a function (p.fnest > 0)". At top level, after *non-declaration
  statement outside function body* and *after top level declaration*,
  `p.advance(_Import, _Const, _Type, _Var, _Func)`. **Verified**:
  https://go.dev/src/cmd/compile/internal/syntax/parser.go?m=text and the raw
  file above.
- **`gofmt`'s parser (`go/parser`)**: `atComma` reports *missing ','* plus
  *before newline* when the token is an inserted semicolon, then "insert"s the
  comma and continues; `advance(to)` synchronises on `stmtStart` (the same
  fourteen words) or `declStart` (`import const type var`; `func` is absent and
  no comment says why). **Verified**:
  https://raw.githubusercontent.com/golang/go/master/src/go/parser/parser.go.

### clang: reported where the parser stopped, noted at the opener, skipped to `;`

- **Where it lands: at the token that cannot go on, with a note at the opener.**
  `BalancedDelimiterTracker::diagnoseMissingClose`: `err_expected` at the current
  token, `note_matching` at the opener, then, unless already at a closing
  bracket, `SkipUntil(Close, ..., StopAtSemi | StopBeforeMatch)`. **Verified**:
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang/lib/Parse/Parser.cpp.
  The critic's run (Apple clang 21.0.0): *expected ')'* at the `;` (`a.c`) or at
  the next line's `if` (`b.c`), *note: to match this '('* at the opener, and a
  later function's mistake still reported.
- **How far the reach goes: to the next `;`, or to a closer an outer opener is
  waiting for.** `SkipUntil` skips nested brackets whole; of an unexpected `)`
  `]` `}`: "If there is a LHS token at a higher level, we will assume that this
  matches the unbalanced token and return it. Otherwise, this is a spurious RHS
  token, which we skip." Module boundaries also stop it: "They generally
  indicate a "good" place to pick up parsing again". **Verified**: same URL.
  *Reading, unrun*: in `b.c` the parser reported at `if` and then skipped from
  it to the next `;` at its own depth, so `if` ended the expression but was not
  a place to resume.
- **Start-of-line keywords, at declaration level only.** `SkipMalformedDecl`
  stops at `namespace` (and `inline namespace`) when `Tok.isAtStartOfLine()`:
  "'namespace' at the start of a line is almost certainly a good place to pick
  back up parsing"; the same for `extern`; and in Objective-C, `-` and `+` at
  the start of a line "probably start new method declarations". It skips a `(`
  whole with `SkipUntil(tok::r_paren)`, so *by that code* these words end
  nothing inside an open bracket. **Verified**:
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang/lib/Parse/ParseDecl.cpp.
  The `extern` case is recent: commit b147019f (Alejandro Alvarez Ayllon,
  2025-10-02, PR #161641, *Preserve `externs` following broken declarations*),
  because a malformed declaration before an `extern "C"` block made the parser
  skip whole sections of standard headers. **Verified**:
  https://www.mail-archive.com/cfe-commits@lists.llvm.org/msg610814.html.

### Swift: a start-of-line declaration, then statement, finishes an open list

- **The compiler's own parser, today.** `Parser::parseListItem`
  (`lib/Parse/Parser.cpp`): "If we're in a comma-separated list, the next token
  is at the beginning of a new line and can never start an element, break.",
  guarding `Tok.isAtStartOfLine() && (Tok.is(tok::r_brace) ||
  isStartOfSwiftDecl() || isStartOfStmt(/*preferExpr*/ false))`; the list then
  finishes and `parseMatchingToken` emits the list's error at the current token
  and a note at the opener. **Verified**:
  https://raw.githubusercontent.com/swiftlang/swift/main/lib/Parse/Parser.cpp.
  The texts: *expected ')' in expression list* and *to match this opening '('*.
  **Verified**:
  https://raw.githubusercontent.com/swiftlang/swift/main/include/swift/AST/DiagnosticsParse.def.
  On an error inside the list, `skipListUntilDeclRBrace` skips to the closer, a
  `,`, a `}`, or a token that starts a declaration, except that `var` or `let`
  followed by `:` is taken as an argument label ("Could have encountered
  something like `_ var:` or `let foo:` or `var:`"). **Verified**: same URL.
- **It began as (a) and was widened to (b).** In the `swift-2.2-RELEASE` and
  `swift-3.0-RELEASE` tags the check was declarations only: "If we're in a
  comma-separated list and the next token starts a new declaration at the
  beginning of a new line, skip until the end.", `if (SeparatorK == tok::comma
  && Tok.isAtStartOfLine() && isStartOfDecl() && ...) { skipUntilDeclRBrace(RightK,
  SeparatorK); break; }`, and in the 3.0 tag that skip loops only `while
  (Tok.isNot(T1, T2, ...) && !isStartOfDecl())`, so the list ends at the
  declaration and `parseMatchingToken` reports there. **Verified**:
  https://raw.githubusercontent.com/swiftlang/swift/swift-2.2-RELEASE/lib/Parse/Parser.cpp,
  https://raw.githubusercontent.com/swiftlang/swift/swift-3.0-RELEASE/lib/Parse/Parser.cpp.
  In `swift-3.1-RELEASE` it already reads `Tok.is(tok::r_brace) ||
  isStartOfDecl() || isStartOfStmt()`, and so in 4.0, 4.2 and 5.0. **Verified**:
  https://raw.githubusercontent.com/swiftlang/swift/swift-3.1-RELEASE/lib/Parse/Parser.cpp
  (and the `swift-4.0-RELEASE`, `swift-4.2-RELEASE`, `swift-5.0-RELEASE` files).
  Swift 3.0 was released 2016-09-13 and 3.1 in March 2017. **Verified**:
  https://www.swift.org/blog/swift-3.0-released/,
  https://www.swift.org/blog/swift-3.1-released/. The commit that widened it:
  *unverified* (GitHub's blame did not render).
- **`if` counts as a statement there, and SE-0380 is why that is safe.**
  `isStartOfStmt` returns true for `kw_if` and `kw_switch` whatever `preferExpr`
  says; `preferExpr` reaches only the contextual `then`. **Verified**:
  https://raw.githubusercontent.com/swiftlang/swift/main/lib/Parse/ParseStmt.cpp.
  SE-0380 (*Implemented (Swift 5.9)*) made `if` and `switch` expressions in
  returns, assignments and declarations only: "There are of course many other
  places where an expression can appear, including as a sub-expression, or as
  an argument to a function. This is not being proposed at this time."
  **Verified**:
  https://github.com/swiftlang/swift-evolution/blob/main/proposals/0380-if-switch-expressions.md.
  *Reading*: so a start-of-line `if` inside a call's list is never legal Swift,
  and the break refuses no program. A pitch of 2026-07-30 proposes `if` and
  `switch` "in arbitrary expression positions"; no status shown. **Verified**:
  https://forums.swift.org/t/pitch-allow-if-switch-expressions-in-arbitrary-expression-positions/88643.
- **The Swift-written parser states the principle.** swift-syntax's
  `TokenPrecedence`: "Describes how distinctive a token is for parser recovery.
  When expecting a token, tokens with a lower token precedence may be skipped
  and considered unexpected." Statement keywords ("Keywords that start a new
  statement": `if` `for` `while` `return` `else` `switch` `guard` ...) and
  declaration keywords ("Keywords that start a new declaration") rank above a
  closing bracket, so recovery looking for `)` skips neither. **Verified**:
  https://raw.githubusercontent.com/swiftlang/swift-syntax/main/Sources/SwiftParser/TokenPrecedence.swift.
  Whether the compiler prints this parser's diagnostics today: *unverified*; on
  2022-08-22 Doug Gregor wrote its "eventual goal ... is to replace the C++
  parser within the Swift compiler". **Verified**:
  https://forums.swift.org/t/a-new-swift-parser-for-swiftsyntax/59813.

### GHC: the layout rule does not know brackets, so a column ends the reach

- **The rule as specified.** "Where the start of a lexeme is preceded only by
  white space on the same line, this lexeme is preceded by <n> where n is the
  indentation of the lexeme"; `L (<n>:ts) (m:ms) = ; : (L ts (m:ms)) if m = n`
  and `= } : (L (<n>:ts) ms) if n < m`; and `L (t:ts) (m:ms) = } : (L (t:ts) ms)
  if m /= 0 and parse-error(t)`. **Verified** (Haskell 98 Report § 9.3):
  https://www.haskell.org/onlinereport/syntax-iso.html. The Haskell 2010 Report
  § 10.3 has the same algorithm and Note 5 on `parse-error(t)`; its context stack
  holds a column per implicit block and 0 for an explicit `{`. **Verified**:
  https://www.haskell.org/onlinereport/haskell2010/haskellch10.html (the fetch
  of that page garbled the `m = n` sign, so the equation is cited from the 98
  text). Asked directly, neither page has a sentence exempting a line inside `(`
  or `[`.
- ***Reading*, unrun: the reach ends at the enclosing block's column, whatever
  the word.** A line inside an unclosed `(` that starts at the column of the
  enclosing implicit block, a top-level declaration included, gets a virtual
  `;` (or `}` if further left), which the grammar refuses inside the bracket.
- **Where it lands: at that line, in words about indentation or brackets.**
  GHC's `PsErrParse`: when the offending token's text is empty (a virtual layout
  token) it prints *parse error (possibly incorrect indentation or mismatched
  brackets)*, otherwise *parse error on input '...'*. **Verified**:
  https://raw.githubusercontent.com/ghc/ghc/master/compiler/GHC/Parser/Errors/Ppr.hs.
  Whether GHC reports anything after its first parse error: *unverified*; a
  search snippet of GHC issue #16955 says the parser "immediately stops", and
  the page itself was unreadable (https://gitlab.haskell.org/ghc/ghc/-/issues/16955).

### Elm: every line inside brackets must be indented; the report is where the closer was due

- **The rule.** In Elm 0.19.1 `checkIndent` succeeds only `if col > indent &&
  col > 1`. **Verified**:
  https://raw.githubusercontent.com/elm/compiler/0.19.1/compiler/src/Parse/Space.hs.
  The tuple and list parsers call `Space.checkIndent end E.TupleIndentEnd` and
  `Space.checkIndent end E.ListIndentEnd` after each element, so a next token at
  column 1, the column where "module, import, and top-level function definitions
  must start", fails the bracket, and the error carries `end`, the end of the
  last element: where the closer was due, not the opener and not the next line.
  **Verified**:
  https://raw.githubusercontent.com/elm/compiler/0.19.1/compiler/src/Parse/Expression.hs,
  https://elmprogramming.com/indentation.html.
- **The cost it paid: a closer is refused at a low column too.** Issue #2155
  (2020-12-10, Elm 0.19.1, open): a `)` placed "before or at the same indent
  level of the previous cases" is a syntax error. **Verified**:
  https://github.com/elm/compiler/issues/2155.
- The report's wording for `TupleIndentEnd` and `ListIndentEnd`: *unverified*
  (the fetched `Reporting/Error/Syntax.hs` was truncated before it); whether Elm
  reports more than one syntax error per file: *unverified*.

### Summary of task 1

| compiler | reach of an unclosed opener ends at | report lands at | later mistakes told |
|---|---|---|---|
| CPython 3.10+ | end of file (a same-kind closer pops it) | the opener, if the parse failed on a later line | no (*unverified as a sentence*) |
| rustc | end of file, or a closer an outer opener matches | end of file, opener labelled | no, since 1.69 |
| Go (`gc`, `go/parser`) | the line end, by the semicolon rule | the point of failure | yes, one per line, ten at most |
| clang | the next `;`, or a closer an outer opener awaits | the point of failure, note at the opener | yes |
| Swift | a start-of-line declaration or statement word, or `}` | the point of failure, note at the opener | yes |
| GHC | the enclosing layout column (*reading*) | that line | *unverified* |
| Elm 0.19.1 | column 1 or the enclosing indent, any token | the end of the last element | *unverified* |

## Task 2: keywords as synchronisation points

### The textbook terms

- Stanford CS143 Handout 09 (Summer 2012, written by Maggie Johnson, revised by
  Julie Zelenski; its bibliography opens with Aho, Sethi, Ullman 1986):
  "*Panic-mode* error recovery is a simple technique that just bails out of the
  current construct, looking for a safe symbol at which to restart parsing. The
  parser just discards input tokens until it finds what is called a
  *synchronizing* token", and "A parser should avoid *cascading errors*, which
  is when one error generates a lengthy sequence of spurious error messages."
  **Verified**:
  https://web.stanford.edu/class/archive/cs/cs143/cs143.1128/handouts/090%20Top-Down%20Parsing.pdf
  (pp. 15 and 16).
- The heuristic that is question (b) in textbook form: "We might add keywords
  that begin statements to the synchronizing sets for the non-terminals
  generating expressions." **Verified as that page's sentence** (GeeksforGeeks,
  updated 2025-07-23, no book cited):
  https://www.geeksforgeeks.org/error-recovery-in-predictive-parsing/. Its
  attribution to Aho, Lam, Sethi and Ullman, and the reason a search snippet
  gives (a missing semicolon letting the parser skip the next statement's
  keyword): *unverified*.

### Wirth, in the lineage design.md names

- *Compiler Construction* (Addison-Wesley 1996, ISBN 0-201-40353-6; revised
  edition, Zurich, May 2017), § 7.3: "Virtually without exception, only weak
  symbols are omitted, symbols which are primarily of a syntactic nature, such
  as the comma, semicolon and closing symbols"; "a declaration sequence always
  begins with the symbol CONST, TYPE, VAR, or PROCEDURE, and a structured
  statement always begins with IF, WHILE, REPEAT, CASE, and so on. Such strong
  symbols are therefore never skipped. They serve as synchronization points in
  the text, where parsing can be resumed with a high probability of success";
  "Strong symbols not to be skipped are assigned a high ranking (ordinal
  number)"; "Frequently, follow-up errors are diagnosed, whose indication may be
  omitted, because they are merely consequences of a formerly indicated error";
  and a good compiler is one where "frequently encountered errors are correctly
  diagnosed and subsequently generate no, or few additional, spurious error
  messages". A call's parameter list: `REPEAT expression; ... UNTIL (sym =
  rparen) OR (sym >= semicolon)`, where `semicolon = 52` ranks below `end`
  `else` `elsif` `until` `array` `record` `const` `type` `var` `procedure`
  `begin` `module` `eof`, and `if = 32`, `while = 34`, `repeat = 35` rank below
  it. **Verified** (pp. 32, 34 to 37):
  https://people.inf.ethz.ch/wirth/CompilerConstruction/CompilerConstruction1.pdf.
- Project Oberon's compiler: `ParamList` goes on `WHILE sym <= ORS.comma` and
  ends with `Check(ORS.rparen, ") missing")` (ORP, "N. Wirth 1.7.97 / 8.3.2020");
  the scanner ranks `if` 32 to `for` 37 below `comma` 40, and `semicolon` 52,
  `end` 53, `else` 55 ... `return` 58, `const` 63 ... `module` 69 above it; and
  `Mark` prints only `IF (p > errpos) & (errcnt < 25)`, then sets `errpos := p +
  4` (ORS, "NW 19.9.93 / 15.3.2017"). **Verified**:
  https://people.inf.ethz.ch/wirth/ProjectOberon/Sources/ORP.Mod.txt,
  https://people.inf.ethz.ch/wirth/ProjectOberon/Sources/ORS.Mod.txt.
- *Reading*: in both of Wirth's compilers a parameter list ends at a
  declaration word and at a statement's separator or ender, and does **not**
  end at a statement's first word; Oberon puts a `;` before every statement, so
  the `;` does that work, and an error within four characters of the last one
  is never printed.

### Pascal-P4

- Pemberton and Daniels, *Pascal Implementation: The P4 Compiler and
  Interpreter* (1982, 2002): `fsys` is "containing the synchronising symbols
  for this statement"; its initial value "is all those symbols that can
  uniquely start a declaration (blockbegsys) or a statement (statbegsys)";
  `thensy` is added when an `if`'s expression is parsed and `elsesy` when its
  statement is; without that, one misspelled `then` gives "a hopeless cascade
  of error messages, all because of one error". **Verified**:
  https://homepages.cwi.nl/~steven/pascal/book/2syntax.html. `skip` in P5 (Scott
  A. Moore's extension of P4): "skip input string until relevant symbol found",
  `while not(sy in fsys) and (not eof(input)) do insymbol`. **Verified**:
  https://raw.githubusercontent.com/tangentstorm/pascal/master/p5/pcom.pas. The
  members of `statbegsys` and `blockbegsys`: *unverified* (both fetches were
  truncated before their initialisation).

### Which of task 1's compilers resynchronise at a keyword

- **Yes, at statement keywords**: `gc` inside a function (the stopset);
  `go/parser` (`stmtStart`); Swift's compiler, when the keyword opens a line;
  swift-syntax by precedence. All in task 1.
- **Yes, at declaration keywords**: `gc` and `go/parser` at top level; Swift;
  clang at declaration level (`namespace`, `extern` at the start of a line).
  rust-analyzer, the Rust IDE's parser, which is not rustc:
  `ITEM_RECOVERY_SET` holds `fn struct enum impl trait const async unsafe
  extern static let mod pub crate use macro ;`. **Verified**:
  https://raw.githubusercontent.com/rust-lang/rust-analyzer/master/crates/parser/src/grammar/items.rs.
  Its author's tutorial (matklad, 2023-05-21): "for `ParamList`, `{` is in
  follow, and we do want it to be a part of the recovery set, but `fn` is *not*
  in follow, and yet it is important to recover on it." **Verified**:
  https://matklad.github.io/2023/05/21/resilient-ll-parsing-tutorial.html.
- **At a balancing keyword, in a token filter**: F#'s `LexFilter` pops the head
  context, a bracket's included, when the token is `in` `else` `elif` `done`
  `with` `finally` or a closer and some context further down the stack balances
  it (`not (tokenBalancesHeadContext token stack) && (stack |> suffixExists
  (tokenBalancesHeadContext token))`; debug text "IN/ELSE/ELIF/DONE/RPAREN/
  RBRACE/END/INTERP at %a terminates context at position %a"). **Verified**:
  https://raw.githubusercontent.com/dotnet/fsharp/main/src/Compiler/SyntaxTree/LexFilter.fs.
  So an `else` ends an open `(` only when an enclosing `if` can take it.
- **No keyword**: CPython and rustc (the two that match brackets in the lexer);
  GHC and Elm, which use a column.

### What they measured or wrote about the cost

- **rustc gave up every later mistake to stop the noise**: the motivating
  report of PR #104012 (an extra `{` at line 1420 reported at line 2605),
  #104367 (six diagnostics for five lines, one an internal compiler error), and
  PR #108297's title, *to avoid noisy diagnostics*. All verified in task 1.
  *Reading*: rustc's noise came from a parser running over mis-grouped token
  trees, and its lexer consults no keyword, so it is not evidence against
  keyword synchronisation; it is evidence that a wrong grouping cascades.
- **Go wrote its policy down**: `go/parser` discards an error "reported on the
  same line as the last recorded error" ("discard - likely a spurious error")
  and stops "if there are more than 10 errors"; `gc` keeps "only one syntax
  error per line, no matter what error" and prints *too many errors* at 10
  unless `-e`. **Verified**:
  https://raw.githubusercontent.com/golang/go/master/src/go/parser/parser.go,
  https://raw.githubusercontent.com/golang/go/master/src/cmd/compile/internal/base/print.go.
- **Oberon**: four characters, 25 errors (above).
- **A measurement, of panic mode rather than keywords**: Diekmann and Tratt,
  *Don't Panic! Better, Fewer, Syntax Errors for LR Parsers* (ECOOP 2020; arXiv
  1804.07133 v4, 2020-07-03): on 200,000 syntactically invalid Java files from
  the Blackbox project, panic mode (Holub's algorithm) reported 981,628 error
  locations against CPCT+'s 435,812, "well over twice as many", and "the more
  input that is skipped, the more likely that a cascade of further parsing
  errors ensues". **Verified**: https://arxiv.org/abs/1804.07133,
  https://soft-dev.org/pubs/html/diekmann_tratt__dont_panic/.
- **Not found**: a measurement of false reports caused by resynchronising at a
  keyword too early. Searched for: panic mode, synchronizing tokens, cascading
  errors, spurious errors, the Swift list heuristic's history. Ripley and
  Druseikis (*A statistical analysis of syntax errors*, 1978) is reported by a
  search snippet to count missing `)` as 3.5% of the syntax errors in 589
  Pascal programs: *unverified* (the abstract page refused the fetch).
- **Where the risk was handled by grammar, not measured**: Swift's `if`
  (SE-0380, task 1); Swift's `var:`/`let foo:` labels; and Go's `func`, absent
  from `go/parser`'s `declStart` although a function literal begins with it,
  with no comment saying whether that is the reason (*unverified* as a reason).

## Task 3: indentation-sensitive languages

Which close an open bracket at a line that dedents to a declaration?

- **Python: no.** Inside brackets no INDENT or DEDENT is made and the reach runs
  to end of file; only the report moves to the opener, by the line test
  (task 1).
- **Haskell: in effect, by column rather than by word, and as an error rather
  than a recovery** (task 1, *reading*): the dedented line gets a virtual `;`
  that the grammar refuses inside the bracket, reported as *possibly incorrect
  indentation or mismatched brackets*.
- **F#: yes, as a context closure.** "When a column position becomes an offside
  line, a context is pushed. The closing bracketing tokens ), }, and end
  terminate offside contexts up to and including the context that the
  corresponding opening token introduced" (§ 15.1.5); a `Paren(token)` context
  is pushed for "(, begin, struct, sig, {, [, [|, or quote-op-left" (§ 15.1.6);
  "When a token occurs on or before the offside limit for the current offside
  stack, and a permitted undentation does not apply, enclosing contexts are
  closed until the token is no longer offside", and "If a token is offside and
  a context cannot be closed, then an "undentation" warning or error is issued"
  (§ 15.1.8). **Verified** (*The F# 4.1 Language Specification*, pp. 255 to
  259): https://fsharp.org/specs/language-spec/4.1/FSharpSpec-4.1-latest.pdf.
  The 2006 announcement of the light syntax: "every construct starting at first
  column is implicitly a new declaration", and when a token occurs prior to an
  offside line "enclosing constructs are terminated. This may result in a
  syntax error, e.g. when there are unclosed parentheses." **Verified** (Don
  Syme's blog, 2006-08-23, signed "Don and James for the F# team"):
  https://learn.microsoft.com/en-us/archive/blogs/dsyme/lightweight-syntax-option-in-f-1-1-12-3.
  What `fsc` prints for an unclosed `(` before a column-0 `let`: *unrun*; search
  results show FS0583 *Unmatched '('* beside FS0010, *unverified*.
- **Nim: yes, relative to the enclosing block, not to column 0.** Inside
  brackets `optPar` refuses a new line indented less than the current block:
  `if p.tok.indent < p.currInd: parMessage(p, errInvalidIndentation)`; at or
  beyond it a line goes on freely ("As a rule of thumb, indentation within
  expressions is allowed after operators, an open parenthesis and after
  commas"). **Verified**:
  https://raw.githubusercontent.com/nim-lang/Nim/devel/compiler/parser.nim,
  https://nim-lang.org/docs/tut1.html. *Reading, unrun*: a dedent to column 0
  inside a proc body's bracket is refused at that line, while a bracket opened
  at top level, where the current indentation is 0, is not ended by a column-0
  line at all.
- **Elm: yes, at column 1, whatever the token, closers included** (task 1,
  issue #2155).

*Reading*: Heroes' rule inside brackets is Python's (design.md § 4.15 says so,
"Python's discipline"), and rule (a) is where it departs toward the others. It
is narrower than Elm's and GHC's, which end a bracket at a column whatever the
token: rule (a) asks for column 0 **and** a declaration word, which keeps a
closer at column 0 legal. The shared brief's census counts 33 `}` and 11 `)`
opening column-0 lines inside brackets (the coordinator's measurement, not
mine), which is Elm's #2155 shape.

## Reading

**verdict**: approve (advisory): (a) as landed; (b) only on the condition
below.

**precedents**, each verified above unless marked:

- (a), a declaration line ends an open bracket: Swift's compiler in the 2.2 and
  3.0 tags (3.0 released 2016-09-13), kept and widened since; F#'s offside rule
  and its "every construct starting at first column is implicitly a new
  declaration" (2006); Elm 0.19.1 and, by *reading*, GHC, by column rather than
  word; Wirth's declaration words ranked above `;` (1996, 2017, 2020); `gc` at
  top level; clang's start-of-line `namespace` and `extern` (2025-10-02), at
  declaration level only; matklad's `ParamList` recovering at `fn` (2023).
- Against (a), the reach is the file: CPython 3.10 (report at the opener) and
  rustc, which since 1.69 (2023) stops at an unmatched delimiter and tells no
  later mistake.
- (b), a statement word ends an open bracket: Swift's compiler from the 3.1 tag
  (March 2017) to today, `if` included, safe because SE-0380 (Swift 5.9) keeps
  `if` and `switch` out of argument position; `gc`'s stopset inside functions;
  swift-syntax's precedence; F#'s token filter, for `else` `elif` `in` `done`
  `with` `finally` and only when an enclosing context balances the word; the
  textbook heuristic (its attribution *unverified*).
- Against (b): Wirth's `ParamList` keeps going at `IF` to `FOR`; clang reports
  at `if` and, by *reading* of its source, skips past it to the `;`.

**argument** (120 words or fewer): (a) has precedent, and it held: Swift's
compiler ended an open list at a start-of-line declaration by 2016 and still
does; F#, Elm and GHC end a bracket at a dedent. Asking for a declaration word
as well as column 0 is what spares it Elm's cost, a closer at a low column
refused (#2155). It departs from the ancestry § 4.15 names, Python, which reads
to end of file, so the new sentence should say so. (b) is Swift's 2017
widening, but every precedent does it where the grammar is known: in a parser,
or in F#'s filter only when an enclosing context takes the word. Swift's `if`
is safe only because SE-0380 keeps `if` out of argument position.

**condition**, what would change this reading:

- For (a): a compiler that bounded a bracket at a declaration line and then
  withdrew the rule because it refused legal programs or reported falsely.
  None found; Swift widened its rule instead. Also a program that compiles with
  one of the seven words, `function` before a name included, at column 0 inside
  brackets (the shared brief's census found its 55 only in cases that are wrong
  on purpose; the grammar is the compiler-engineer's).
- For (b): the compiler-engineer's grammar answer. If a program that compiles
  can begin a line inside brackets with `if`, `match` or `else`, no precedent I
  found accepted refusing it: Swift's rule is safe by SE-0380's scope, and F#
  closes at `else` only when an enclosing `if` can take it. Then (b) should keep
  to the words that are statements only, `while`, `for`, `return`, `assert` (the
  critic's brief names `if` and `match` as the two that are expressions), or
  take F#'s condition for `else`. The Swift pitch of 2026-07-30 would put Swift
  itself in that position, and its outcome is precedent worth watching.
- For the diagnostic's placement: clang (a note), Swift (a note) and rustc (a
  label) name the opener when they report at a later point, and CPython reports
  at the opener itself; Go's and GHC's message texts name no opener, and Elm's
  wording is *unverified*. Keeping *citing the opener* in § 4.15's new sentence
  follows the first four.
