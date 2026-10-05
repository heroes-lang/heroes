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

## Continued 2026-10-05: corrections, Q3, Q4, Q5 and the verdict

**The clock, again.** The author paused all work at 21:26 on 2026-10-04 and
asked to resume at 00:40 on 2026-10-05; both times are the coordinator's
message, not this seat's readings. The text above was committed in the trunk
at `99a67630` as it stood, so from here this seat **only appends**: a
sentence above that is wrong stays where it is and is corrected below, with
the date. The four `(pending)` sections above are filled below, under the
same questions. Pages fetched before the pause were read 2026-10-04; the one
page fetched after the resume, RFC 8259 [S63], was read 2026-10-05. Source
numbers continue from [S46].

**A convention for what follows.** An escape made of a backslash, `u` and
four hex digits is described in words below and never written out, for the
reason correction 1 gives. Braced escapes, `\u{...}`, are written as they
are.

### Corrections to the text above, 2026-10-05

1. **This report holds two raw ZERO WIDTH SPACE characters (U+200B).** In
   the Staticcheck bullet, *"the same written ... : no diagnostic"*, and in
   the Pylint bullet, *"zero-width-space ..."*, the text meant the
   six-character escape: a backslash, `u`, then `200b`. The tool that wrote
   the file decoded that escape, so the committed bytes hold a raw U+200B
   between the quotes and between the backticks: invisible in most
   renderers, and the character this sitting is about, in the report about
   it. Found by this seat reading its own committed file back on 2026-10-05,
   where the two spots read as an empty `""` and empty backticks. Nothing
   else above was decoded: `\x1A`, `\x1B`, `\0`, `\t` and every braced
   `\u{...}` read back as written. The facts stand: Staticcheck accepts the
   escaped spelling of U+200B, and Pylint suggests the escaped spelling.
   **This is one more measured instance of the class, not a precedent**: a
   tool between a writer and a file turned a visible spelling into an
   invisible character in silence, and only a reader who knew what to look
   for found it.
2. **Swift refuses a raw tab and every other raw C0 control inside a
   one-line string.** The table row *"Swift | accepted | accepted |
   refused"*, the bullet quoting Swift's grammar (*"Any Unicode scalar value
   except ..."*), and the sentence *"The tab: refused in a string by Zig
   (since 2024) and accepted by Rust, Go, Swift, ..."* are wrong about
   Swift's compiler, which is stricter than its book's grammar. [S47]
   `https://raw.githubusercontent.com/swiftlang/swift/main/lib/Parse/Lexer.cpp`,
   in `lexCharacter`, which lexes a string literal's characters, as the
   fetch copied it:
   ```
   if ((signed char)(CurPtr[-1]) >= 0) {
     if (isPrintable(CurPtr[-1]) == 0)
       if (!(IsMultilineString && (CurPtr[-1] == '\t')))
         if (EmitDiagnostics)
           diagnose(CharStart, diag::lex_unprintable_ascii_character);
   ```
   The file declares `using clang::isPrintable;`, and clang's predicate
   [S48]
   `https://raw.githubusercontent.com/llvm/llvm-project/main/clang/include/clang/Basic/CharInfo.h`
   is documented as *"Return true if this character is an ASCII printable
   character; that is, a character that should take exactly one column to
   print in a fixed-width terminal"*, its set holding no tab (paraphrased by
   the fetch). [S49]
   `https://raw.githubusercontent.com/swiftlang/swift/main/include/swift/AST/DiagnosticsParse.def`
   makes it an error: `ERROR(lex_unprintable_ascii_character)`,
   *"unprintable ASCII character found in source file"*; and
   `ERROR(lex_nul_character)`, *"nul character embedded in middle of
   file"*. So in Swift a raw tab, ESC or NUL in a one-line string is a
   compile error, and a raw tab alone is allowed in a multi-line (`"""`)
   string. Read from the code, **not run** here. The corrected row: Swift |
   **refused** (allowed in a `"""` string) | **refused** | refused. With it,
   the tab is refused raw in a string by **Zig and Swift**, and by JSON
   (below), and accepted by Rust, Go, Python, JavaScript, C and TOML.
3. **A formatter that writes an invisible character by its escape does
   exist.** The sentence *"no formatter was found that writes a refused
   character by its escape (searched: the sources above; a negative claim,
   unverified)"* is falsified by elm-format, found after it was written.
   [S60]
   `https://raw.githubusercontent.com/avh4/elm-format/main/elm-format-lib/src/ElmFormat/Render/Box.hs`,
   the helper `fix` inside `formatString`, in part, as the fetch copied it:
   ```
   else if c == '\t' then
       "\\t"
   ...
   else if not $ Char.isPrint c then
       hex c
   else if c == ' ' then
       [c]
   ...
   else if Char.isSpace c then
       hex c
   ```
   where `hex` writes `"\\u{" ++ (printf "%04X" $ Char.ord char) ++ "}"`
   for Elm 0.19. Haskell's `isPrint` [S61]
   `https://hackage-content.haskell.org/package/base-4.22.0.0/docs/Data-Char.html`
   *"Selects printable Unicode characters (letters, numbers, marks,
   punctuation, symbols and spaces)"*, false for the categories
   LineSeparator, ParagraphSeparator, Control, Format, Surrogate,
   PrivateUse and NotAssigned (paraphrased by the fetch). So elm-format,
   in every string it prints, writes a raw tab as `\t`, a control or format
   character (ESC, the bidirectional controls, U+2028, U+2029, ZWJ, ZWNJ,
   U+FEFF, the tag characters) as `\u{...}`, and a space other than U+0020
   (NBSP, U+3000) as `\u{...}`; a variation selector, being a nonspacing
   mark, counts as printable and stays raw. That list is read from the code
   and Haskell's documentation, **not run**; in particular, that it rewrites
   the ZWJ inside an emoji sequence is an inference. Gren's formatter
   library, a port, states the same rule in words [S62]
   `https://packages.gren-lang.org/package/gilramir/gren-format-lib/version/1.1.1/module/Formatter.Logical.LiteralFormat`:
   every other code point is written *"verbatim if it is printable, a
   `\u{HEX}` escape if it is not"*.
4. **"No precedent was found that refuses ZWJ, ZWNJ or a variation selector
   raw in a string"** holds for every compiler and linter found, and needs
   two qualifications found later. Google's Swift style guide [S50]
   `https://google.github.io/swift/`: *"Invisible characters, such as the
   zero width space and other control characters that do not affect the
   graphical representation of a string, are always written as Unicode
   escape sequences"*, and *"Control characters, combining characters, and
   variation selectors that do affect the graphical representation of a
   string are not escaped when they are attached to a character or
   characters that they modify"*; one standing alone is written as an
   escape (paraphrased by the fetch). And elm-format rewrites ZWJ and ZWNJ
   as escapes by its code (correction 3, an inference). A style guide and a
   formatter; still no refusal.
5. **"three of the four ship enabled by default"** (the linters'
   introduction) rests on three named sources and one gap: Staticcheck's
   ST1018 carries no non-default mark [S28], Clippy's correctness group is
   deny by default [S34], ESLint's rule is in its recommended config [S35];
   **whether Pylint enables its E25xx messages by default was not
   fetched**, so the sentence is three verified and one unverified.
6. **"The date on every source, 2026-10-04"** holds for every page cited
   above; [S63], fetched after the resume, was read 2026-10-05.

### Q1 and Q2, continued

- **JSON** [S63] `https://www.rfc-editor.org/rfc/rfc8259` (December 2017),
  read 2026-10-05: *"All Unicode characters may be placed within the
  quotation marks, except for the characters that MUST be escaped:
  quotation mark, reverse solidus, and the control characters (U+0000
  through U+001F)."* JSON refuses a raw tab in a string, as Zig and Swift
  do; TOML does not.
- **Swift warns on NBSP in code**: `WARNING(lex_nonbreaking_space)`,
  *"non-breaking space (U+00A0) used instead of regular space"* [S49].
  Whether it fires inside a string: not read.
- **The class, a policy one can lower or a rule of the grammar.** Rust's
  bidirectional refusal is a deny-by-default lint a crate can lower [S1],
  [S2]; GCC's is a warning with `-Wbidi-chars=none` [S3]; Zig's and Swift's
  control refusals are lexical errors with no switch in the code read
  [S14], [S47]. So precedent has both shapes, split the way the brief's
  question splits them: the bidirectional refusal as a policy (which maps
  onto `check --permissive` dropping it), the control refusal as grammar
  (which maps onto keeping it). Precedent does not decide Heroes' own
  thesis-or-robustness line; it shows the split is not new.

**Routes the precedents suggest that the brief did not list (Q1, Q2):**
- **(i) Rust's list**: refuse in strings and comments only the nine
  explicit bidirectional controls, beside the C0 and C1 controls in a
  string, and leave the Default_Ignorable characters real text needs raw.
  Rust's compiler and GCC shipped exactly that set [S1], [S3].
- **(ii) Staticcheck's context rule**: refuse a format character raw but
  exempt ZWJ after an emoji or a variation selector, the variation
  selectors, and the tag characters [S29], [S30]; Google's Swift guide says
  it in words, raw where it modifies a neighbour and escaped where it
  stands alone [S50]. It needs a check that knows a grapheme's shape, a
  cost for the compiler-engineer to price.
- **(iii) elm-format's route**: no refusal, and the formatter writes every
  non-printable character and every non-ASCII space in a string by its
  escape [S60]. It reaches the editor and the web page once the file is
  formatted, which a diagnostic does not, and it is undone the moment the
  file is edited without the formatter.
- **(iv)** for comments the brief lists the unpaired-run refusal; precedent
  adds that in a line-comment language a run in a comment always ends with
  its line [S37], so *unpaired* there means *a control with no matching pop
  before the line ends*, which a check can decide on one line.

## Q3, filled: how a refused character is written

**The coordinator's recollection, verified.** *"Rust, Swift and JavaScript
beside `\u{...}`"*: all three, with Zig and Elm beside them.
- **Rust**: `\u{7FFF}`, *"24-bit Unicode character code (up to 6 hex
  digits)"*, a Unicode scalar value, so no surrogate and nothing above
  U+10FFFF [S20].
- **Swift**: `\u{n}`, *"Between one and eight hexadecimal digits"* [S22];
  its lexer refuses a surrogate, the noncharacters U+FDD0 to U+FDEF and
  anything above U+10FFFF with *"invalid unicode scalar"* [S47], [S49].
- **JavaScript**: `\u{X}` to `\u{XXXXXX}` up to U+10FFFF, beside the
  four-digit form [S24]. A JavaScript string is UTF-16 code units and may
  hold a lone surrogate: *"Each code unit can be written in a string with
  `\u` followed by exactly four hex digits"* [S54]
  `https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String`.
  Whether the braced form accepts a surrogate: not read.
- **Zig**: `\u{NNNNNN}`, a *"hexadecimal Unicode scalar value UTF-8 encoded
  (1+ digits)"* [S56] `https://ziglang.org/documentation/master/`; above
  U+10FFFF is `invalid_unicode_codepoint` [S57]
  `https://raw.githubusercontent.com/ziglang/zig/master/lib/std/zig/string_literal.zig`.
  `"\u{d800}"` is today the error *"unicode escape does not correspond to a
  valid codepoint"*, and issue #20270 [S55]
  `https://github.com/ziglang/zig/issues/20270` (mnemnion, 2024-06-11, open,
  labelled accepted) proposes allowing it, because `\x` already writes
  arbitrary bytes.
- **Elm**: `\u{XXXX}`, as elm-format writes it [S60].

*"C, Python and Go beside `\xNN`"*: all three have `\x`, and **each means
something different by it**.
- **C**: `\x` takes an *"arbitrary number of hexadecimal digits"* and
  writes one code unit, a byte [S51]
  `https://en.cppreference.com/w/c/language/escape`; a hex digit after it
  extends it, so `"\xff""f"` is the workaround [S25]. C's universal
  character names may **not** name a code point below U+00A0 other than
  `$`, `@` and the backquote, nor a surrogate, nor (C23) one above U+10FFFF
  [S51]: in C, ESC is `\x1b` or `\033`, never a universal character name.
- **Python**: `\xhh`, *"Unlike in Standard C, exactly two hex digits are
  required"*; in a `str` it is *"a Unicode character with the given
  value"*, in `bytes` a byte [S23]. Python's `\x` cannot write half a
  character.
- **Go**: exactly two hex digits, and *"The three-digit octal (`\nnn`) and
  two-digit hexadecimal (`\xnn`) escapes represent individual bytes of the
  resulting string"* [S21]. Go's `\x80` writes half a character.

The other byte escapes:
- **Rust** limits `\x` in a `str` and a `char` to *"7-bit character code
  (exactly 2 hex digits, up to 0x7F)"*; a byte string takes it to 0xFF
  [S20]. **The brief's recollection that Rust refuses `\x80` and above in a
  string is verified by the reference.** Rust's error message for it was not
  fetched (the search for it was refused when the author paused the
  session).
- **Zig**'s `\xNN` is a *"hexadecimal 8-bit byte value (2 digits)"*, and
  non-UTF-8 bytes may be embedded with it [S56]: half a character.
- **JavaScript**'s `\xXX` is a code unit, U+0000 to U+00FF [S24]: a
  character.
- **TOML 1.1.0**'s `\xHH`: *"Any Unicode character may be escaped with the
  `\xHH`, ... forms. The escape codes must be Unicode scalar values."*
  [S27]. The newest precedent (2025-12-18) chose the character, not the
  byte, and added `\e` for ESC beside it.

**Languages with no byte escape**:
- **Swift**: `\0`, `\\`, `\t`, `\n`, `\r`, `\"`, `\'` and `\u{n}`, nothing
  else [S22]; ESC is `\u{1B}`.
- **Java**: no hexadecimal escape; octal `\0` to `\377`, which is U+0000 to
  U+00FF, a character [S52]
  `https://docs.oracle.com/javase/specs/jls/se21/html/jls-3.html`. Its
  Unicode escapes (`\u` and four hex digits) are translated before lexing,
  so one naming a line feed ends the line inside a literal, and *"use the
  escape sequence `'\n'`"* instead [S52].
- **Kotlin**: `\t`, `\b`, `\n`, `\r`, `\'`, `\"`, `\\`, `\$`, and `\u` with
  four hex digits; neither `\x` nor octal is documented [S53]
  `https://kotlinlang.org/docs/characters.html`.
None of the three can write half a character.

**A NUL by escape.** No general-purpose string literal found refuses one:
Rust has `\0` [S20], Swift `\0` [S22], JavaScript `\0` [S24], Go `\x00` and
`\000` [S21], Python `\x00` [S23], C `\0` [S25], Java `\0` [S52]. **The one
refusal found is Rust's C-string literal**: in `c"..."`, *"`\0` and `\u{0}`
are not permitted (C strings are implicitly null-terminated)"*, and a raw
NUL may not stand in one (paraphrased by the fetch) [S20]. Rust 1.77.0
(2024-03-21) [S64] `https://blog.rust-lang.org/2024/03/21/Rust-1.77.0/`
gives them as *"a nul-byte terminated string in memory of type
`&'static CStr`"*, with *"lack of interior nul byte"* checked at compile
time. So design.md `:1016-1018`'s reason for freezing the escapes, a NUL
that truncates every C call, has precedent only in a literal whose type is
the C string. Heroes' `str` is both the string and the C string
(design.md `:547`, F3); Rust split the two into two types and refused the
NUL only in the second.

**One spelling.**
- **A raw character refused because an escape exists**: Zig refuses a raw
  tab in a string, `\t` being the spelling [S14], [S18]; Swift in a
  one-line string [S47]; JSON every raw control, tab included [S63]; Rust in
  a character literal [S20]. The reason Zig gives is rendering, not
  spelling [S16]; no source fetched gives *one spelling* as the reason.
- **An escape refused because the character could stand raw**: **no
  compiler found does it.** The nearest are a style rule and an open
  proposal. Google's Swift guide: *"For any character that has a special
  escape sequence (`\t`, `\n`, `\r`, `\"`, `\'`, `\\`, and `\0`), that
  sequence is used rather than the equivalent Unicode (e.g., `\u{000a}`)
  escape sequence"* [S50]. JDK-8271171 [S46]
  `https://bugs.openjdk.org/browse/JDK-8271171`, John Rose's *"obnoxious
  backslash puzzlers should be linted with -Xlint:unicode-escapes"*, would
  flag Unicode escapes of printable ASCII; status New, unresolved
  (paraphrased by the fetch).
- **What formatters do**: Prettier declines: *"Prettier maintains the way
  your string is escaped"*, an emoji is not rewritten as its escapes *"and
  vice versa"* [S58] `https://prettier.io/docs/rationale`. Black normalizes
  inside the escape only: *"Another area where Python allows multiple ways
  to format a string is escape sequences. ... Black normalizes such escape
  sequences to lowercase, but uses uppercase for `\N` named character
  escapes"* [S59]
  `https://black.readthedocs.io/en/stable/the_black_code_style/current_style.html`.
  elm-format writes the invisible ones as escapes [S60] (correction 3). A
  formatter that rewrites an escape back to its raw character: none found
  (searched: the sources above).

**The message a writer meets.** Every precedent that refuses names the
spelling in its message, and none steers to a raw character: Rust, *"The
error messages will suggest the right escapes to use"* [S1]; Staticcheck,
*"consider using the %q escape sequence instead"* [S29]; Pylint names the
escape per character, ESC's being `\x1B` [S31]; Clippy, *"consider replacing
the string with"* the escaped form [S33]. Swift's unknown escape reads
*"invalid escape sequence in literal"* [S49], with no suggestion in its
definition. Today's `unknown_escape` (F10), *a string holds its characters
as themselves*, is the one message found that points the other way.

**What these say about the routes (Q3):**
- **(a′) a `[u8]` built at run time**: no precedent was searched for; every
  language above writes ESC in a literal.
- **(b) `\u{...}` for every code point but 0 and a surrogate**: spelled so
  by Rust, Swift, Zig, JavaScript and Elm; a surrogate is refused by Rust,
  Swift, Zig (today) and Go's four- and eight-digit forms [S21]; U+0000 is
  refused there by none but Rust's C-string literal.
- **(c) `\xNN` as bytes** writes half a character in C, Go and Zig. Two
  routes the brief did not list: **(c′) `\xNN` limited to U+0001 to
  U+007F**, Rust's choice for its strings; and **(c″) `\xNN` as a code
  point U+0001 to U+00FF**, TOML 1.1's, Python's and JavaScript's reading.
  Neither can write half a character.
- **(d) an escape only for what a literal refuses raw**: no compiler
  precedent; a style rule (Google's Swift guide) and an open proposal
  (JDK-8271171).
- **(e) a named constant**: not searched; but **(e′) a letter escape for
  ESC**, `\e`, is TOML 1.1's new answer to exactly F10's ESC [S27].
- **(f) `fmt` writing a refused character by its escape**: elm-format [S60].

## Q4, filled: a NUL lent to C or to the operating system

| runtime | where a NUL is refused | how | what the program reads | source |
|---|---|---|---|---|
| Rust `CString::new` | converting to the C-string type | a scan | `Err(NulError)`, with the position | [S65] |
| Rust path functions | at the door | copy to a 384-byte stack buffer or allocate, then `CStr::from_bytes_with_nul` | `io::Error`, `InvalidInput`, *"file name contained an unexpected NUL byte"* | [S66] |
| Rust `c"..."` | at compile time | the literal may not hold one | a compile error | [S20], [S64] |
| Go `ByteSliceFromString`, `BytePtrFromString` | the door's conversion | `bytealg.IndexByteString(s, 0) != -1` | `EINVAL` | [S67] |
| Go `StringByteSlice`, `StringBytePtr` | the same | the same | **a panic**; both deprecated | [S67] |
| Go on Windows, `UTF16FromString` | the same | the same | `EINVAL` | [S68] |
| Python path functions | the door's converter | `PyUnicode_FSConverter` | `ValueError: embedded null byte`; since 3.8 `os.path.exists` and kin answer `False` | [S72], [S69] to [S71] |
| Python `PyUnicode_AsWideCharString` | the lend, **only when the caller takes no length** | not read | `ValueError` | [S72] |
| Python `PyUnicode_AsUTF8` | nowhere | the docs warn of truncation | nothing | [S72] |
| Java NIO, `UnixPath` | the path's construction | inside the pass that collapses slashes | `InvalidPathException`, *"Nul character not allowed"* | [S74] |
| Java `java.io.File` | the operation | not read | the operation fails (paraphrased) | [S73] |
| .NET since Core 2.1 | `Path.GetFullPath` | `path.Contains('\0')` | `ArgumentException` | [S75], [S76] |
| Zig `std.os.realpath` (2024) | at the door | an assertion | **a panic** with safety on | [S78], [S79] |
| Perl since 5.20 | the system call | not read | a `syscalls` warning | [S82] |

- **Rust** [S65] `https://doc.rust-lang.org/std/ffi/struct.CString.html`:
  *"This function will return an error if the supplied bytes contain an
  internal 0 byte. The `NulError` returned will contain the bytes as well as
  the position of the nul byte."*; `from_vec_unchecked` is `unsafe`, *"The
  caller must ensure `v` contains no nul bytes in its contents."* [S66]
  `https://raw.githubusercontent.com/rust-lang/rust/master/library/std/src/sys/helpers/small_c_string.rs`:
  `io::const_error!(io::ErrorKind::InvalidInput, "file name contained an
  unexpected NUL byte")`; `MAX_STACK_ALLOCATION` 384 (32 on ESP-IDF); a
  shorter path is copied to the stack with a NUL appended and checked with
  `CStr::from_bytes_with_nul`, a longer one goes through `CString::new`
  (paraphrased by the fetch). **Rust pays one copy and one scan at every path
  door** because its `str` carries no trailing NUL; Heroes' `str` does
  (design.md `:547`, F3), so the copy is Rust's cost and the scan alone would
  be Heroes'.
- **Go** [S67]
  `https://raw.githubusercontent.com/golang/go/master/src/syscall/syscall.go`:
  `ByteSliceFromString` returns `(nil, EINVAL)` on a NUL; the deprecated
  `StringByteSlice` and `StringBytePtr`, *"If s contains a NUL byte this
  function panics instead of returning an error"*. **Go deprecated the
  panicking form for the one that returns an error.** That `os.Open` reaches
  `BytePtrFromString`: not read.
- **Python**: [S69]
  `https://mail.python.org/pipermail/python-list/2018-May/884134.html`
  (Marko Rauhamaa, 2018-05-31) shows `os.path.exists("\0")` raising
  `ValueError: embedded null byte` and says it *"can even be a security
  issue"*; [S70]
  `https://github.com/python/cpython/commit/0185f34ddcf07b78feb6ac666fbfd4615d26b028`,
  *"bpo-33721: Make some os.path functions and pathlib.Path methods be
  tolerant to invalid paths"*, makes them *"return False instead of raising
  ValueError"*; [S71] `https://docs.python.org/3/library/os.path.html`,
  *"Changed in version 3.8"*. [S72]
  `https://docs.python.org/3/c-api/unicode.html`: `PyUnicode_FSConverter`,
  *"Embedded null bytes are not allowed in the result."*;
  `PyUnicode_AsWideCharString`, *"Note that the resulting `wchar_t` string
  might contain null characters, which would cause the string to be
  truncated when used with most C functions. If size is NULL and the
  wchar_t* string contains null characters a ValueError is raised."*;
  `PyUnicode_AsUTF8`, *"This function does not have any special behavior for
  null characters embedded within unicode. As a result, strings containing
  null characters will remain in the returned string, which some C functions
  might interpret as the end of the string, leading to truncation. If
  truncation is an issue, it is recommended to use PyUnicode_AsUTF8AndSize()
  instead."* **This is the split the brief's route (a) needs and could not
  place**: a lend that hands over the length may carry a NUL, a lend that
  hands over a bare C string refuses it. Python made the split by the shape
  of the call, which is where Heroes' four internal callers that pass a
  length (F4) and `.cstr()` already differ.
- **Java** [S73] `https://bugs.openjdk.org/browse/JDK-8003992`, *"File and
  other classes in java.io do not handle embedded nulls properly"*, P3,
  created 2012-11-26, resolved 2013-05-06, fix version 8, backported to
  7u40 and 7u45 (paraphrased by the fetch). [S74]
  `https://raw.githubusercontent.com/openjdk/jdk/master/src/java.base/unix/classes/sun/nio/fs/UnixPath.java`:
  `checkNotNul` throws `new InvalidPathException(input, "Nul character not
  allowed")`, called inside `normalizeAndCheck`'s one pass over the path.
  **Java put the NUL test inside a scan it already made**, which is route
  (d)'s argument, at the construction of a path rather than of a string.
- **.NET** [S75] `https://learn.microsoft.com/en-us/dotnet/core/compatibility/2.1`:
  from .NET Core 2.1 the path APIs *"no longer check for invalid path
  characters or throw an exception if an invalid character is found"*,
  because *"Aggressive validation of path characters blocks some
  cross-platform scenarios. This change was introduced so that .NET does
  not try to replicate or predict the outcome of operating system API
  calls."* Yet [S76]
  `https://raw.githubusercontent.com/dotnet/runtime/main/src/libraries/System.Private.CoreLib/src/System/IO/Path.Unix.cs`,
  `GetFullPath`: `if (path.Contains('\0')) throw new
  ArgumentException(SR.Argument_NullCharInPath, nameof(path));`. **When .NET
  dropped every other path-character check, the NUL check stayed.** Whether
  every door reaches it: not read; [S77]
  `https://learn.microsoft.com/en-us/dotnet/api/system.io.file.open` lists
  the invalid-character exception for versions *"older than 2.1"* only.
- **Zig** [S78] `https://github.com/ghostty-org/ghostty/pull/1511` (opened
  2024-02-12, merged 2024-02-13): fuzzing Ghostty's terminal found that
  *"The `std.os.realpath` function asserts that input paths contain no null
  bytes, triggering panics when debug safety is enabled"* (paraphrased by the
  fetch), and Ghostty turned it into an error in its own code; [S79]
  `https://github.com/ghostty-org/ghostty/commit/6a3b676779d5eb17d2fb2896ffcd295c6be911f3`,
  the comment: *"std.os.realpath *asserts* that the path does not have
  internal nulls instead of erroring."* **An abort at the door, reachable
  from outside input, was found by a fuzzer and guarded by the caller.**
  That Zig's `toPosixPath` holds the same assertion: *search summary only*.
- **Perl** [S82] `https://perldoc.perl.org/perl5200delta`: *"Embedded \0
  characters in pathnames or other system call arguments produce a warning
  as of 5.20. The parts after the \0 were formerly ignored by system
  calls."* Whether the call now also fails: not in the text fetched,
  *unverified*.

**What a truncated path caused.**
- **The name**, Phrack 55, article 07, 1999-09-09, rain.forest.puppy,
  *"Perl CGI Problems"* [S81] `https://insecure.org/news/P55-07.txt`:
  *"Perl allows NUL characters in its variables as data. Unlike C, NUL is
  not a string delimiter."*; `$database="rfp\0.db"` opens `rfp`
  (paraphrased by the fetch); the term *poison NULL byte* credited to Olaf
  Kirch's Bugtraq post. That is defect 245's and Q-i's shape, a language
  string holding a NUL and a C door stopping at it, fifteen years before
  Perl warned [S82].
- **CVE-2006-7243** [S80]
  `https://services.nvd.nist.gov/rest/json/cves/2.0?cveId=CVE-2006-7243`,
  published 2011-01-18, CWE-20: *"PHP before 5.3.4 accepts the \0 character
  in a pathname, which might allow context-dependent attackers to bypass
  intended access restrictions by placing a safe file extension after this
  character, as demonstrated by .php\0.jpg at the end of the argument to the
  file_exists function."*
- Other CVEs of the class: not searched beyond these two.

**What these say about the routes (Q4):**
- **Proposal part 4** (no `str` lends a NUL unasked; a door answers a
  failure, never another file): every runtime fetched refuses the NUL
  somewhere before the system call, Perl before 5.20 excepted.
- **The answer**: Rust, Go, Python, Java and .NET answer **a failure the
  program reads**; Go deprecated its panicking forms for it; the one abort
  found, Zig's assertion, became a fuzzer's crash a caller had to guard.
  `hero_run_arg`'s panic (F4) is Zig's shape, not the others'.
- **(a) a scan at the lend**: Rust's path doors scan and copy per call
  [S66]; Python scans only the lend that takes no length [S72], which is the
  precedent for leaving the four callers that pass a length free.
- **(b) a fact kept per string**: not found among these.
- **(c) a fallible lend**: Rust's `CString::new` [S65], Go's
  `ByteSliceFromString` [S67].
- **(d) refused at construction**: **no general string type found refuses a
  NUL**; Rust, Go, Python, Java, JavaScript and Swift strings each hold one
  through the escapes above. Java's NIO refuses it at the construction of a
  *path*, inside a scan it already makes [S74].
- **(e) doors that take the string and answer a failure**: what Rust's,
  Go's, Python's, Java's and .NET's doors do.
- Two routes the brief did not list: **(f) a yes-or-no door answers rather
  than fails**: Python 3.8's `exists` returns `False` for a path holding a
  NUL [S70]. **(g) a separate C-string type or literal, the NUL refused when
  a string becomes one** (Rust's `CString` and `c"..."`) [S20], [S65],
  which Heroes' `cstr`, *promising a zero* (spec `:386`, F2), is the start
  of.

## Q5, filled: an argument that is not Unicode on Windows

- **Rust** [S83] `https://doc.rust-lang.org/std/env/fn.args.html`: *"The
  returned iterator will panic during iteration if any argument to the
  process is not valid Unicode. If this is not desired, use the `args_os`
  function instead."* [S84] `https://doc.rust-lang.org/std/env/fn.args_os.html`:
  *"Note that the returned iterator will not check if the arguments to the
  process are valid Unicode."* [S85]
  `https://doc.rust-lang.org/std/ffi/struct.OsString.html`: on Windows
  *"strings are often arbitrary sequences of non-zero 16-bit values,
  interpreted as UTF-16 when it is valid to do so"*, and `OsString` holds them
  *"as a sequence of 8-bit values, encoded in a less-strict variant of
  UTF-8"*; `into_string` converts *"if it contains valid Unicode data"*, and
  *"On failure, ownership of the original `OsString` is returned."* So
  **Rust's `args()` is spec `:324-325`'s sentence kept on Windows**, and
  `args_os()` with `into_string()` is `args_checked()`'s shape. That a lone
  surrogate on Windows reaches the panic: the documentation's general
  sentence, not run.
- **WTF-8** [S86] `https://wtf-8.codeberg.page/` (moved from
  `https://simonsapin.github.io/wtf-8/`): *"The WTF-8 encoding"*, edited by
  Simon Sapin, last updated 2022-02-23 by the page; *"WTF-8 (Wobbly
  Transformation Format − 8-bit) is a superset of UTF-8 that encodes
  surrogate code points if they are not in a pair."* A lone U+D800 is
  `ED A0 80`, and a pair must be written as its supplementary code point;
  its users named are Rust's standard library for OS strings on Windows,
  Scheme 48 and Racket (paraphrased by the fetch). And: *"WTF-8 must not be
  used to represent text in a file format or for transmission over the
  Internet"*, *"any WTF-8 data must be converted to a Unicode encoding at
  the system's boundary."*
- **Go**: [S87]
  `https://raw.githubusercontent.com/golang/go/master/src/os/exec_windows.go`
  builds `os.Args` from `windows.UTF16PtrToString(syscall.GetCommandLine())`;
  [S88]
  `https://raw.githubusercontent.com/golang/go/master/src/internal/syscall/windows/syscall_windows.go`:
  that helper calls `syscall.UTF16ToString`; [S68]
  `https://raw.githubusercontent.com/golang/go/master/src/syscall/syscall_windows.go`:
  *"Unpaired surrogates are decoded using WTF-8 instead of UTF-8
  encoding."* [S90] `https://go.dev/doc/go1.21`: *"On Windows the syscall
  package now supports working with files whose names, stored as UTF-16,
  can't be represented as valid UTF-8. The UTF16ToString and
  UTF16FromString functions now convert between UTF-16 data and WTF-8
  strings. This is backward compatible as WTF-8 is a superset of the UTF-8
  format that was used in earlier releases."* [S89]
  `https://go.dev/src/syscall/wtf8_windows.go?m=text`: copyright 2023;
  *"the conversion never fails and is lossless"*; it points to
  go.dev/issues/59971. **What the earlier route cost** [S91]
  `https://github.com/golang/go/issues/59971` (TBBle, 2023-05-04), *"os:
  RemoveAll hangs on Windows when the directory tree contains files with
  non-UTF8-representable filenames"*: names were read with the surrogate
  replaced, the replaced name did not exist, and `RemoveAll` looped for ever
  (paraphrased by the fetch); [S92] `https://pkg.go.dev/unicode/utf16`:
  *"If the pair is not a valid UTF-16 surrogate pair, DecodeRune returns the
  Unicode replacement code point U+FFFD."* So Go's `os.Args` on Windows has
  held a lone surrogate as its WTF-8 bytes since 1.21, and Go's strings,
  being bytes, carry it with no check; before 1.21 it held U+FFFD, an
  inference from [S90]'s *"UTF-8 format that was used in earlier
  releases"* and [S92].
- **Python** [S93] `https://docs.python.org/3/library/sys.html`: *"On Unix:
  Command line arguments are passed by bytes from OS. Python decodes them
  with filesystem encoding and 'surrogateescape' error handler."* The page
  has no Windows note. [S94] `https://peps.python.org/pep-0529/` (Steve
  Dower, Python 3.6, final): paths *"will be transcoded from utf-16-le into
  utf-8 using surrogatepass (Windows does not validate surrogate pairs, so
  it is possible to have invalid surrogates in filenames)"*; it does not
  discuss `sys.argv`. **What `sys.argv` holds on Windows for a lone
  surrogate: no page fetched says, *unverified*** (recollection: the `str`
  keeps it as a lone surrogate code point). Python never aborts on an
  argument: on Unix an undecodable byte becomes a code point U+DC80 to
  U+DCFF, *"undecodable bytes are replaced by a Unicode character U+DCxx on
  decoding"* [S95] `https://docs.python.org/3/library/os.html`, and the
  failure moves to whoever encodes it strictly.
- **Zig** [S96] `https://github.com/ziglang/zig/pull/19005` (squeek502,
  merged 2024-02-25): *"Windows paths now use WTF-16 <-> WTF-8 conversion
  everywhere, which is lossless"*; before, converting ill-formed UTF-16
  would *"either fail or invoke illegal behavior"*; a new `InvalidWtf8`
  error for a user's input that is not WTF-8 (paraphrased by the fetch).
  That the same holds for command-line arguments: *search summary only*.

**What these say about the routes (Q5):**
- **The trunk's narrow door (`x?y`) and panel 191's UTF-8 manifest
  (`x<U+FFFD>y`)** are both lossy conversions: Go's route before 1.21,
  whose cost is issue #59971 [S91]. For `args()` the loss is quieter than
  for a directory walk, but it is the same route.
- **The strict conversion (error 1113)** is Zig's other old half, *fail*
  [S96]: the program cannot see the argument at all.
- **The wide `argv` converted losslessly to WTF-8, then the existing UTF-8
  check**: Go 1.21 [S90], Zig [S96] and Rust's `OsString` [S85] convert so;
  Rust then panics in `args()` and hands back the original in
  `into_string()` [S83], [S85]. That keeps `:324-325` true and gives
  `args_checked()` its `not_text` through code that exists (F9). WTF-8's
  own rule [S86], never let it out as text, holds because Heroes' `str`
  door already refuses those bytes (F9, on this Mac).
- A route the brief did not list: **`args_checked()`'s failure carries the
  argument's bytes**, as Rust's `into_string` returns the original
  `OsString` on failure [S85], so a program can still name a file whose name
  is not Unicode. Heroes' `[str?]` has no place for them today; this is a
  question for the ffi-pragmatist and the spec-warden, not a finding.

## Verdict, argument, condition (filled)

**`verdict`** (advisory, per question):
- **Q1: object** to proposal part 1 as written, because it refuses ZWJ,
  ZWNJ, the variation selectors and the tag characters raw, which no
  compiler or linter found does; **approve** its control half (C0, C1, the
  nine bidirectional controls, U+2028, U+2029), the tab included.
- **Q2: approve** a refusal of the nine bidirectional controls, U+2028 and
  U+2029 in comments.
- **Q3: approve** a spelling that shows the character; `\u{...}` has the
  widest precedent; **object** to `\xNN` as bytes.
- **Q4: approve** proposal part 4; **object** to an abort as a door's
  answer.
- **Q5: approve** keeping `:324-325` through a lossless WTF-8 conversion;
  **object** to the replacing conversion as the lasting route.

**`precedents`**: the sections above, each with its URL; every one
verified by a page fetched in this sitting unless marked *unverified*,
*search summary only* or *paraphrased by the fetch*.

**`argument`** (120 words or fewer): Precedent splits by character class.
Raw controls in a string are refused by Zig, Swift's one-line strings, JSON
and TOML, and flagged with the escape as the fix by Staticcheck, Pylint and
Clippy; the nine bidirectional controls are refused by Rust's compiler in
strings and comments since 1.56.1. No compiler or linter found refuses
ZWJ, ZWNJ, a variation selector or a flag's tag characters where they join
text: Staticcheck exempts them by name, and Unicode calls the joiners
necessary. At the C boundary every mainstream runtime fetched answers a NUL
with a failure the program reads; the one abort found became a fuzzer's
crash. On Windows, Go 1.21, Zig and Rust convert losslessly to WTF-8; Go's
lossy route hung `RemoveAll`.

**`condition`**, what would change the reading:
- **Q1**: a compiler or linter that refuses ZWJ, ZWNJ or a variation
  selector raw in a string and kept it for years without an exemption would
  move me to approve part 1 whole; Zig or Swift reverting their raw-tab
  refusal would move me against refusing the tab.
- **Q2**: Rust lowering its comment lint, or a published attack carried by
  a line comment alone, which the two examples read here do not show.
- **Q3**: a compiler that refuses an escape for a character the literal
  could hold raw (it would give route (d) precedent); a record of
  elm-format's rewriting of ZWJ being reverted because it broke emoji in
  source (it would warn against route (f) over format characters).
- **Q4**: a runtime that aborts on a NUL at a door by design and kept it
  undeprecated, or a defect caused by a door answering a failure instead of
  truncating.
- **Q5**: a defect class from Go's or Zig's WTF-8 conversion, WTF-8 bytes
  leaking into files above all; Python's own Windows behaviour, unverified
  here, would not change it.

**Cost of this seat**, counted by hand from its own transcript, so
approximate to a few: about 148 page fetches and 34 web searches, failures
included (a session limit, an anti-bot page, two 404s, a refused connection,
one search refused when the author paused the session). No paid run, no
build, no file written but this report.
