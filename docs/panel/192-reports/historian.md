# Panel 192, the historian's report

Seat: historian (advisory, no veto; `.claude/agents/historian.md`).
Brief: `docs/panel/192-briefs/historian.md`, with `00-shared.md`,
`00-facts.md` and the critic's first pass
(`docs/panel/192-reports/completeness-critic-briefs.md`), all read whole
before the first search.

**The clock.** This seat's tools are Read, Write, WebSearch and WebFetch; it
has no shell, so it cannot run `date`, and it writes no time of day of its
own. The date on every source, **2026-10-04**, is the session environment's
date, not a `date` reading. **An interruption**: the account's session limit
stopped this seat mid-research, while it was settling Staticcheck's
treatment of the bidirectional controls; the coordinator's message gives the
stop at about 18:30, the report at 14,230 bytes written at 18:24, the limit
reset at 20:50 and the resume at 21:07. Those four times are the
coordinator's readings, not this seat's. Every page cited below was fetched
in this sitting, before or after the stop.

**The rule this report follows.** Every factual claim names a page fetched in
this sitting, with its URL, read 2026-10-04. Pages were read through
WebFetch, which hands the page to a small model that answers a question about
it; so a "quote" below is that model's quotation of the page, and where its
answer was a paraphrase the report says *paraphrased by the fetch*. A claim
resting only on a search engine's summary, not on a fetched page, says
*search summary only*. A claim resting on memory says *recollection,
unverified*. The author's book was not used as a source.

**Cost.** No paid run, no build, no file written in the trunk but this one.
Web searches and fetches only.

## Q1 and Q2: invisible and reordering characters in source

### Trojan Source, toolchain by toolchain

| toolchain | what it did | where | when | level | source, read 2026-10-04 |
|---|---|---|---|---|---|
| **Rust** | two lints, `text_direction_codepoint_in_literal` and `text_direction_codepoint_in_comment` | compiler | Rust 1.56.1, 2021-11-01 | **deny** by default | [S1], [S2] |
| **GCC** | `-Wbidi-chars=[none\|unpaired\|any\|ucn]` | compiler | GCC 12 | warning, default `unpaired` | [S3], [S4] |
| **Clang** | `misc-misleading-bidirectional`, `misc-misleading-identifier`, a homoglyph check | **clang-tidy**, not the compiler | Extra Clang Tools 14.0.0 | a linter check | [S5], [S6], [S7] |
| **Go** | nothing in the toolchain found; proposal #20209 (2017) on hold since 2017-10-09; the third-party `bidichk` linter | linter outside the toolchain | proposal 2017-05-02 | none | [S8], [S9], [S10] |
| **Python** | PEP 672, informational, *does not give any recommendations*; no tokenizer change found | a document | 2021-11-01 | none | [S11] |
| **Java** | JDK-8278542, *javac could produce a warning for suspicious uses of bi-directional Unicode control characters*: **open, unresolved** | none shipped | filed 2021-12-10 | none | [S12] |
| **Swift** | a forum thread; no compiler change found | none | 2021-11-01 | none | [S13] |
| **Zig** | no bidirectional rule found; refuses **every ASCII control but LF** in strings, character literals and comments (see below) | tokenizer | 2024 and 2026 | error | [S14] to [S19] |

What each says, in detail:

- **Rust** [S1] `https://blog.rust-lang.org/2021/11/01/cve-2021-42574/`:
  *"Rust 1.56.1 today, 2021-11-01, with two new deny-by-default lints
  detecting the affected codepoints, respectively in string literals and in
  comments."* The code points: *"U+202A, U+202B, U+202C, U+202D, U+202E,
  U+2066, U+2067, U+2068, U+2069"*. **The list holds no ZWJ (U+200D), no ZWNJ
  (U+200C), no variation selector, no U+2028 or U+2029, and no LRM, RLM or
  ALM (U+200E, U+200F, U+061C)**: nine code points, the embeddings,
  overrides, isolates and their two pops. On a legitimate use: *"we recommend
  replacing them with the related escape sequence. The error messages will
  suggest the right escapes to use."* [S2]
  `https://doc.rust-lang.org/rustc/lints/listing/deny-by-default.html` gives
  both lints *Default Level: Deny*, the same nine code points, and an example
  whose error labels the character as `'\u{202e}'`: **the compiler's own
  output writes the refused character by its escape.**
  It is **a lint, not a lexical rule**: deny by default, so
  `#[allow(...)]` can lower it (the doc's example writes
  `#![deny(text_direction_codepoint_in_literal)]`, which presumes the level
  can be set). That a crate can allow it is the lint machinery's general
  property; that this lint is allowed in practice anywhere is *recollection,
  unverified*.
- **GCC** [S3] `https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html`,
  verbatim through the fetch: *"Warn about possibly misleading UTF-8
  bidirectional control characters in comments, string literals, character
  constants, and identifiers. ... There are three levels of warning supported
  by GCC. The default is `-Wbidi-chars=unpaired`, which warns about
  improperly terminated bidi contexts. `-Wbidi-chars=none` turns the warning
  off. `-Wbidi-chars=any` warns about any use of bidirectional control
  characters. By default, this warning does not warn about UCNs."* So **GCC
  distinguishes an unbalanced run, and by default warns on that alone.** It
  is a warning, never an error by default. [S4]
  `https://developers.redhat.com/articles/2022/01/12/prevent-trojan-source-attacks-gcc-12`
  (2022-01-12): implemented by Marek Polacek, diagnostics by David Malcolm,
  *"in trunk for GCC 12"*; it calls *"a tokenization boundary such as a
  comment or string literal"* a bidirectional context (paraphrased by the
  fetch); its example output is
  `/*<U+202E> } <U+2066>if (isAdmin)<U+2069> <U+2066> begin admins only*/`:
  **GCC's diagnostic quotes the source line with each control written as
  `<U+XXXX>`**, the shape defect 244 gives Heroes' diagnostics. Whether GCC 12
  was backported to older branches: *unverified* (GCC's bugzilla, PR 103026,
  refused the fetch behind an anti-bot page).
- **Clang** [S5]
  `https://releases.llvm.org/14.0.0/tools/clang/tools/extra/docs/ReleaseNotes.html`,
  under *New checks*: `misc-misleading-bidirectional`, *"Inspects string
  literal and comments for unterminated bidirectional Unicode characters"*,
  and `misc-misleading-identifier`. [S6]
  `https://clang.llvm.org/extra/clang-tidy/checks/misc/misleading-bidirectional.html`:
  *"Warns about unterminated bidirectional unicode sequence."* [S7]
  `https://blog.llvm.org/posts/2022-01-12-trojan-source/` (Serge Guelton,
  2022-01-12): the checks went into **clang-tidy, not the compiler**, and the
  post names *"a trade-off on parse time"* as the reason (paraphrased by the
  fetch). Search summaries repeatedly say *"gcc and clang compilers added
  warnings"*; every such summary traces to a glossary site
  (unicodefyi.com), and no Clang compiler warning was found in a primary
  source: **a Clang frontend warning is unverified, and the LLVM project's own
  post says the work went to clang-tidy.**
- **Go** [S8] `https://github.com/golang/go/issues/20209`, *"proposal: spec:
  disallow LTR/RTL characters in string literals?"*, opened 2017-05-02 by
  karalabe, **open, labelled Proposal-Hold**; it proposes *"requiring
  developers to use explicit `\x` escape sequences instead"* (paraphrased by
  the fetch). [S9] its comments, through
  `https://api.github.com/repos/golang/go/issues/20209/comments?per_page=100`:
  - bradfitz, 2017-05-02, answering *"String literals must be able to hold
    any byte sequence whatsoever"*: *"Yes, but not in their raw form. Only if
    they're escaped."*
  - robpike, 2017-05-16: *"I don't believe the language is the place to
    solve this, plus it's a very slippery slope"*; and 2017-05-18: *"Maybe
    something, perhaps the language, perhaps vet, should simply forbid
    literal non-printing characters above U+007E."*
  - rsc, 2017-10-09: *"the first step is to add a vet check and only after
    building experience with it think about actual language restrictions (or
    not). Marking this proposal-hold until there is a proposal ... of what a
    vet 'unicode' check would check."*
  - holiman, 2021-11-01, posting the Trojan Source news to the thread.

  No `go vet` check for these characters was found (searched: the issue's
  comments, *go vet bidi*); that none exists is a negative claim on that
  vocabulary, *unverified*. [S10]
  `https://pkg.go.dev/github.com/breml/bidichk@v0.2.6`: `bidichk`, a
  third-party analyser golangci-lint runs, flags the same nine code points as
  Rust. That golangci-lint added it in v1.43.0: *search summary only*.
- **Python** [S11] `https://peps.python.org/pep-0672/`, *Status: Active,
  Type: Informational, Created: 01-Nov-2021*: *"This document does not give
  any recommendations and solutions."* *"The possible issues generally can't
  be solved in Python itself without excessive restrictions of the language.
  They should be solved in code editors and review tools (such as diff
  displays), by enforcing project-specific policies, and by raising awareness
  of individual programmers."* Of bidirectional marks: *"Python only allows
  them in strings and comments."*
- **Java** [S12] `https://bugs.openjdk.org/browse/JDK-8278542`, Enhancement,
  **Open, Unresolved**, created 2021-12-10, assignee Jan Lahoda, affects 18 and
  19. Nearly four years on, javac warns about nothing here by this record.
- **Swift** [S13]
  `https://forums.swift.org/t/is-swift-vulnerable-to-trojan-source-attacks/53205`,
  opened 2021-11-01: a quick check *"suggest[s] that it is"* vulnerable; a
  core-team member (Xiaodi Wu) notes UAX #31 mitigations *"discussed in the
  past here but never made it through the Swift Evolution process"*; a user
  suggests a SwiftLint custom rule. No compiler change found in the thread.
- **Zig** has no bidirectional rule that this search found, but it is the
  sharpest precedent on **raw control characters**, below.

**The attack's authors' own recommendation** [S42] `https://trojansource.codes/`:
for compilers, *"throw errors or warnings for unterminated bidirectional
control characters in comments or string literals"*; for language
specifications, *"formally disallow unterminated bidirectional control
characters in comments and string literals"*; for editors and repositories,
*"make bidirectional control characters and mixed-script confusable
characters perceptible with visual symbols or warnings."* The paper [S43]
`https://arxiv.org/abs/2111.00169`: Nicholas Boucher and Ross Anderson, v1
2021-10-30, v2 2023-03-08, *"To appear in the 32nd USENIX Security
Symposium"*. **The authors asked for a refusal of the UNTERMINATED run, not
of the character.**

**Where the attack's own examples open the run** (a fact for a language
whose comments run to the end of the line and whose strings are one line,
which is Heroes, spec `:31` and `:35-36`):
- [S44] `https://raw.githubusercontent.com/nickboucher/trojan-source/main/Python/commenting-out.py`,
  line 4, as the fetch rendered it: `if access_level != 'none<U+202E><U+2066><U+2067>': # Check if admin <U+2069><U+2066>' and access_level != 'user`.
  **The run opens inside a one-line string**; the `#` comment holds only the
  closing PDI and an LRI.
- [S45] `https://raw.githubusercontent.com/nickboucher/trojan-source/main/Python/early-return.py`,
  line 4: `''' Subtract funds from bank account then <U+2067><U+200B>''' ;return`:
  an RLI and a **ZERO WIDTH SPACE (U+200B, a Default_Ignorable code point)**
  inside a string written on one line.
- The directory listing ([S44]'s parent,
  `https://github.com/nickboucher/trojan-source/tree/main/Python`) holds
  `commenting-out.py`, `early-return.py`, `homoglyph-function.py` and
  `invisible-function.py`; the last *"Throws Syntax Error"* on Python 3.9.5
  (paraphrased by the fetch).
- UAX #9 [S37] `https://www.unicode.org/reports/tr9/` (the header as the
  fetch read it: Unicode 18.0.0, revision 52, 2026-09-01): rule X8, *"All
  explicit directional embeddings, overrides and isolates are completely
  terminated at the end of each paragraph"*; *"Paragraphs are divided by the
  Paragraph Separator or appropriate Newline Function (for guidelines on the
  handling of CR, LF, and CRLF, see ...)"*; U+2029 is class B and U+2028 is
  class WS. So under the algorithm a run cannot outlive its line where a line
  feed ends a paragraph, which is what an editor drawing one line per
  paragraph does; that every editor and web view does so is *unverified*.

So in both Python examples the reordering run opens inside a string and acts
on the rest of the same line. Neither opens it in a comment. That is two
files, not a proof that a comment alone cannot carry an attack; it is
evidence about where the published attacks put the opening character.

### Raw control characters inside a string literal

| language | raw tab in a string | other raw C0 controls | raw CR | source, read 2026-10-04 |
|---|---|---|---|---|
| **Zig** (master) | **refused** | **refused** (0x01 to 0x1F but LF, and 0x7F) | refused | [S14] tokenizer |
| **Rust** | accepted | accepted | **refused** | [S20] |
| **Go** | accepted | accepted (NUL may be refused anywhere in source) | accepted in an interpreted string by the spec's wording | [S21] |
| **Swift** | accepted | accepted | refused | [S22] |
| **Python** | accepted | accepted; **NUL refused anywhere in source** | refused (a newline) | [S23] |
| **JavaScript** | accepted | accepted | refused | [S24] |
| **C** | accepted | which controls the source character set holds is the implementation's: *recollection, unverified* | ends the line | [S25] |
| **TOML** (a data format) | **accepted** | **refused**, in strings and in comments | refused | [S26], [S27] |

- **Zig** [S14]
  `https://raw.githubusercontent.com/ziglang/zig/master/lib/std/zig/tokenizer.zig`:
  the `string_literal` and `char_literal` states both send
  `0x01...0x09, 0x0b...0x1f, 0x7f` to `.invalid`, so **a raw tab, a raw ESC
  and every other C0 control but LF are refused inside a string and a
  character literal**; the comment and doc-comment states send
  `0x01...0x09, 0x0b...0x0c, 0x0e...0x1f, 0x7f` to `.invalid` (CR handled
  apart), and the file's tests include `testTokenize("//\t", &.{.invalid})`.
  How it got there, in order:
  - [S15] `https://github.com/ziglang/zig/commit/7fbe9e7d60fa17e49177f52df922bbf295ecc619.patch`
    (Josh Wolfe, 2021-03-03, *"update docs and grammar to allow CRLF line
    endings (#8063)"*): **before** it, the reference said control characters
    were never allowed *"(Note that Windows line endings (CRLF) are not
    allowed, and hard tabs are not allowed.)"*, anywhere, comments
    included; **after** it, tab and a CR before LF were allowed.
  - [S16] `https://github.com/ziglang/zig-spec/issues/38` (andrewrk,
    2021-07-08, open), *"grammar clarifications regarding tabs and carriage
    returns"*: in comments *"Any TAB is rejected by the grammar since it is
    ambiguous how it should be rendered"*; *"TAB are rejected by the grammar
    inside multi-line string literals"*; a tab as whitespace between tokens
    is accepted and `zig fmt` rewrites it.
  - [S17] `https://github.com/ziglang/zig/pull/20885` (andrewrk, merged
    2024-08-01), *"std.zig.tokenizer: simplification and spec conformance"*,
    labelled breaking; *"hard tabs are now illegal"* where the spec says so
    (paraphrased by the fetch).
  - [S18] `https://ziggit.dev/t/the-latest-zig-distribution-doesnt-allow-tab-chars-in-comments/6481`
    (2024-10-22, Zig 0.14.0-dev): users meet *"Comment contains invalid
    byte: '\t'"*, and a reply says tabs in string literals are refused too,
    with `\t` as the spelling. A forum, so secondary; [S14] is the primary
    for strings.
  - [S19] `https://github.com/ziglang/zig/commit/9296ec14e9efe901b397cf8560f75cacf867f170.patch`
    (Andrew Kelley, dated 2026-08-20, *"langref: rewrite Source Encoding
    section"*): *"Some code points are never allowed, even in Comments:
    ASCII control characters, except for U+000a (LF): U+0000...U+0009,
    U+000b...U+0001f, U+007f. Non-ASCII Unicode line endings: U+0085 (NEL),
    U+2028 (LS), U+2029 (PS). Byte order marks: U+FEFF (BOM)."* and
    *"tooling such as zig fmt provides convenience functionality to convert
    invalid source encodings to valid source encodings, for instance by
    stripping byte order marks and carriage returns."* Whether this text is
    in a tagged release yet: *unverified*.

  So Zig went **refuse, allow (2021), refuse again (2024 in the tokenizer,
  2026 in the reference)**, and the reason it gives is the one Heroes'
  defect 251 gives: *ambiguous how it should be rendered*. Zig refuses
  U+2028 and U+2029 anywhere in the file, comments included.
- **Rust** [S20] `https://doc.rust-lang.org/reference/tokens.html`: a string
  literal holds *"Any Unicode characters except U+0022, U+005C, and U+000D"*
  (paraphrased by the fetch); **CR is not permitted in any string literal**; a
  **character** literal refuses a raw LF, CR **and TAB**. So Rust refuses a
  raw tab in a character literal and accepts it in a string: the split
  Heroes would have if it refused the tab in one literal and not the other.
- **Go** [S21] `https://go.dev/ref/spec`: *"Within the quotes, any character
  may appear except newline and unescaped double quote."* *"Implementation
  restriction: For compatibility with other tools, a compiler may disallow
  the NUL character (U+0000) in the source text."*
- **Swift** [S22]
  `https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/ReferenceManual/LexicalStructure.md`:
  *quoted-text-item*, *"Any Unicode scalar value except `"`, `\`, U+000A, or
  U+000D"*.
- **Python** [S23] `https://docs.python.org/3/reference/lexical_analysis.html`:
  a short string holds any source character except the backslash, newline or
  its quote (paraphrased by the fetch), and *source_character: "any Unicode
  code point, except NUL"*.
- **JavaScript** [S24]
  `https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar`:
  any code point but `\`, CR, LF and the opening quote, and *"U+2028 ... and
  U+2029 ... are now allowed in string literals since ES2019"* (paraphrased
  by the fetch).
- **C** [S25] `https://en.cppreference.com/w/c/language/string_literal`: an
  *s-char* is *"a multibyte character from the source character set
  (excluding (`"`, `\`, and newline)"*; and *"A string literal is not
  necessarily a string; if a string literal has embedded null characters, it
  represents an array which contains more than one string"*, with
  `"abc\0def"`, `strlen` 3, array size 8.
- **TOML 1.0.0** [S26] `https://toml.io/en/v1.0.0`: a basic string, *"Any
  Unicode character may be used except those that must be escaped: quotation
  mark, backslash, and the control characters other than tab (U+0000 to
  U+0008, U+000A to U+001F, U+007F)."*; a literal string, *"Control
  characters other than tab are not permitted in a literal string."*;
  comments, *"Control characters other than tab (U+0000 to U+0008, U+000A to
  U+001F, U+007F) are not permitted in comments."* **TOML 1.1.0** [S27]
  `https://toml.io/en/v1.1.0` (published 2025-12-18 by the page) keeps the
  rule and adds `\e` and `\xHH` (Q3 below).

### Linters that refuse a raw invisible or control character in a string, and offer the escape

These are the precedents nearest proposal part 1 with part 3's spelling as
the fix. None is a compiler; three of the four ship enabled by default.

- **Staticcheck ST1018**, *"Avoid zero-width and control characters in
  string literals"*, available since 2019.2, with no *non-default* mark [S28]
  `https://staticcheck.dev/docs/checks/`. Its source [S29]
  `https://raw.githubusercontent.com/dominikh/go-tools/master/stylecheck/st1018/st1018.go`
  (paraphrased by the fetch; it declined to copy the code whole): it flags a
  format character (category Cf) and a control character (Cc) other than
  `\n`, `\t` and `\r`, raw string literals included; it exempts the Arabic
  number signs (U+0600 to U+0605, U+0890, U+0891, U+08E2), the variation
  selectors, the tag characters of a flag (U+E0020 to U+E007F), and ZWJ
  under the comment *"Allow zero-width joiner in emoji, including those that
  use variation selectors"*; its message, *"string literal contains the
  Unicode [format/control] character %U, consider using the %q escape
  sequence instead"* (the bracket is the fetch's). Its test data [S30]
  `https://raw.githubusercontent.com/dominikh/go-tools/master/stylecheck/st1018/testdata/go1.0/CheckInvisibleCharacters/CheckInvisibleCharacters.go`,
  case by case as the fetch reported it:
  - a raw **tab** in a string: no diagnostic;
  - a raw U+200B: *Unicode format character U+200B*; the same written
    `"​"`: no diagnostic;
  - U+0007: *Unicode control character U+0007*;
  - an emoji ZWJ sequence, a variation selector with ZWJ, and a subdivision
    flag's tag characters: no diagnostic.

  Two things the fetches did not settle. **ZWNJ (U+200C)** is not among the
  exemptions the fetch found, so by the code's shape it would be flagged as
  a Cf character, Persian text included: an inference, *unverified* by a
  test. **The bidirectional controls**: the first fetch listed them among the
  exemptions, the second quoted a comment, *"Bidirectional formatting
  characters. At best they will render confusingly, at worst they're used to
  cause confusion"*, and said they fall to the flagged default; the test data
  holds no bidirectional case. **Unverified either way.**
- **Pylint** [S31]
  `https://raw.githubusercontent.com/pylint-dev/pylint/main/pylint/checkers/unicode.py`
  and [S32]
  `https://pylint.readthedocs.io/en/stable/_sources/whatsnew/2/2.13/summary.rst.txt`
  (2.13.0, 2022-03-24): `bidirectional-unicode` (E2502) and six
  `invalid-character-*` messages, E2510 to E2515, each naming the escape to
  write instead: backspace `\b`, carriage-return `\r`, sub `\x1A`, **esc
  `\x1B`**, nul `\0`, zero-width-space `​`. ESC's help: *"Commonly
  initiates escape codes which allow arbitrary control of the terminal."* The
  module's docstring cites PEP 672. Its bidirectional list is the nine plus
  U+200F (paraphrased by the fetch).
- **Clippy** [S33]
  `https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/unicode.rs`:
  `invisible_characters`, since 1.49.0, group **correctness**, which Clippy's
  README [S34] `https://github.com/rust-lang/rust-clippy` gives the default
  level **deny**; it checks U+200B, U+00AD and U+2060 and suggests *"consider
  replacing the string with"* the escaped form. `non_ascii_literal` is in
  **restriction** (allow by default) and suggests `"\u{20ac}"` for `"€"`.
- **ESLint `no-irregular-whitespace`** [S35]
  `https://eslint.org/docs/latest/rules/no-irregular-whitespace`: in the
  recommended config, since v0.9.0; it lists VT, FF, NEL, NBSP, U+1680,
  U+180E, U+2000 to U+200B, U+2028, U+2029, U+202F, U+205F, U+3000 and
  U+FEFF, which is close to what the compiler's `UNSEEN` adds over
  `DEFAULT_IGNORABLE` (F6) and holds **no ZWNJ and no ZWJ**. Its option
  *"`skipStrings`: true (default) allows any whitespace characters in string
  literals"*: **by default this rule does not look inside a string**. The page
  marks no default on the other four `skip*` options.

### Unicode's own guidance

- **UTS #55** [S36] `https://www.unicode.org/reports/tr55/`: Version 2,
  Revision 5, 2024-01-29, editors Robin Leroy and Mark Davis.
  - **§5.1.6, *Directional Formatting Characters***: *"Implementations should
    not prohibit the use of the directional formatting characters; they are
    useful in ensuring the correct display of bidirectional text, as
    illustrated in this document. However, in order to avoid disruption when
    the code is displayed as plain text, it may be useful to warn when the
    effect of the explicit directional formatting character extends across
    atoms."*
  - §3.2: *"It is further recommended that languages allow the ignorable
    format controls between atoms, as defined in Section 4.1, Bidirectional
    Ordering, to the extent possible, even if the atom boundary occurs within
    a single lexical element."*
  - §4.2: *"It is recommended that editors also provide an option to make
    visible any default ignorable code points (that is, code points with the
    Default_Ignorable_Code_Point property)."*
  - §4.2.1: ZWNJ, ZWJ and the variation selectors *"can occur within a word,
    and in particular within an identifier"*; it suggests drawing each
    without removing its effect (paraphrased by the fetch).
  - §5.1.3: allow U+200C and U+200D in identifiers, because *"these
    characters are necessary in the orthographies of some major languages."*

  **So Unicode's own standard says not to prohibit the bidirectional
  controls, to warn where their effect crosses atoms, and to show the default
  ignorables in an editor.** It is written for a language with right-to-left
  identifiers and literals; Heroes' syntax is ASCII (spec `:35`), so its
  argument that a program needs these controls to fix its own display
  applies to Heroes' strings and comments only.
- **UAX #9** [S37], above: the nine explicit controls (LRE, RLE, PDF, LRO,
  RLO, LRI, RLI, FSI, PDI) and the three implicit marks (LRM U+200E, RLM
  U+200F, ALM U+061C). Rust's nine and GCC's set are the explicit ones; the
  marks are refused by none of the compilers found.

### Showing rather than refusing

- **VS Code 1.62** (October 2021) [S38]
  `https://code.visualstudio.com/updates/v1_62`: *"To address CVE-2021-42574,
  VS Code now renders Unicode directional formatting characters by
  default."* *"The setting `editor.renderControlCharacters` is now `true` by
  default."*
- **VS Code 1.63** (November 2021) [S39]
  `https://code.visualstudio.com/updates/v1_63`: *"All uncommon invisible
  characters in source code are now highlighted by default"*; ambiguous
  characters likewise; settings `editor.unicodeHighlight.invisibleCharacters`,
  `.ambiguousCharacters`, `.nonBasicASCII` and `.includeComments`.
- **GitHub** [S40]
  `https://github.blog/changelog/2021-10-31-warning-about-bidirectional-unicode-text/`,
  2021-10-31: a warning on a file holding bidirectional text, which may be
  *"interpreted or compiled differently than it appears in a user
  interface"*, and a pointer to VS Code, *"which highlights the characters by
  default."* Whether GitHub's own view draws the characters inline: the
  changelog does not say (fetched twice).
- **A compiler writing the character visibly in its own output**: GCC [S41]
  `https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html`,
  `-fdiagnostics-escape-format=[unicode|bytes]`, default `unicode`: *"Unicode
  characters that are not printable ASCII in the form '<U+XXXX>', and bytes
  that do not correspond to a Unicode character validly-encoded in UTF-8 ...
  in the form '<XX>'"*, example `before<U+03C0><BF>after`; and Rust's lint
  output labels the character `'\u{202e}'` [S2]. **Both show it inside a
  diagnostic about it, and neither rewrites the file.** The nearest to a
  formatter rewriting the source is `zig fmt` stripping a BOM and a CR [S19];
  no formatter was found that writes a refused character by its escape
  (searched: the sources above; a negative claim, *unverified*).
- **Who chose showing over refusing, and said so**: Python, *"They should be
  solved in code editors and review tools"* [S11]; Go's rsc, a tool first
  and the language later [S9]; Unicode's UTS #55, *"should not prohibit"*
  [S36]; Clang, into clang-tidy for parse time [S7].

### What these say about the routes (Q1 and Q2)

- **Proposal part 1, a string refuses what a reader cannot see**: Zig refuses
  every raw control but LF in a string, tab included, and gives defect 251's
  reason [S14] to [S19]; TOML refuses every control but tab [S26]; Staticcheck,
  Pylint and Clippy flag raw control and format characters in strings and
  each names the escape as the fix [S28] to [S34]. **Every one of these keeps
  the joiners that real text needs, or does not touch them**: Staticcheck
  exempts emoji ZWJ, variation selectors and flag tags by name [S29], [S30];
  Rust and GCC cover the nine explicit bidirectional controls only [S1],
  [S3]; ESLint's list omits ZWJ and ZWNJ [S35]; UTS #55 says ZWJ and ZWNJ are
  *"necessary"* [S36]. **No precedent was found that refuses ZWJ, ZWNJ or a
  variation selector raw in a string.** Panel 188's `DEFAULT_IGNORABLE`
  refuses all three; that has no precedent this search found.
- **The tab**: refused in a string by Zig (since 2024) and accepted by Rust,
  Go, Swift, Python, JavaScript, C, TOML and Staticcheck. Refused by Rust in
  a character literal [S20]. Zig's reason is rendering, not one spelling
  [S16].
- **Proposal part 2, a comment refuses what reorders a line**: Rust refuses
  the nine in comments [S1]; GCC warns on an unpaired run in a comment
  [S3]; clang-tidy checks comments [S5]; Zig refuses controls, tab
  included, in comments [S14]. Against: UTS #55 §5.1.6 [S36], PEP 672
  [S11]. The published Python attacks open their run in a string [S44],
  [S45].
- **The unbalanced-run route** is GCC's default, clang-tidy's whole check
  and the attack authors' own request [S3], [S5], [S42]. **Rust, the one
  compiler that errors, refuses every occurrence, balanced or not** [S1].
- **Showing it**: VS Code and GitHub show it where Trojan Source bites, in
  the editor and the web view [S38] to [S40]; GCC and Rust show it in their
  diagnostics only [S41], [S2]. A tool-only rendering, which is what Heroes'
  defects 244 and 290 do, reaches neither the editor nor the web page.

(Q3 below.)

## Q3: how a refused character is written

(pending)

## Q4: a NUL lent to C or to the operating system

(pending: Rust's `CString::new`, its `c"..."` literals and its path functions
were fetched before the stop and are written here next.)

## Q5: an argument that is not Unicode on Windows

(pending)

## Verdict, argument, condition

(pending)
