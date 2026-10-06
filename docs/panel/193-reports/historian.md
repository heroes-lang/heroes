# Panel 193, the historian

Written 2026-10-05 from the briefs, as the work went, then stopped by the
account's session limit; resumed 2026-10-06 after 01:30 on the coordinator's
word, the file re-read first. Advisory, no veto. Every precedent below cites
the page it rests on and the day it was read; a claim no page confirmed is
marked **unverified**. No tree was built and no paid run made: my input is the
web. Where the completeness critic ran a tool locally
(`completeness-critic-briefs.md` 1.24), I say whether the documentation agrees
with the run; the run is theirs, the page is mine.

**A warning about my own instrument, found while working.** WebFetch answers
through a small model, and on the SARIF standard it returned three
"quotations" that the standard does not contain: that a consumer "SHALL
assume" `utf16CodeUnits` when `columnKind` is absent (given twice, once
attributed to the invocation object), that §3.57.2 orders the replacements so
each starts at or after the previous one's end, and that a consumer "SHALL
apply all" of a fix's changes. I then read the standard's own pages (the PDF,
saved by the fetch, pages 47, 57-58, 118, 122-127 and 182-187) and none of the
three is there. Every SARIF sentence below is from those pages. **A seat that
cites SARIF from a summary should re-read the page**; the plausible version is
the wrong one, and in one case it supplies a default the standard refuses to
give. The other quotations below came back through the same model; where one
carries weight I read it by a second route (a source file, a doxygen page, a
PDF page) and say so.

## 1. rustc, `--error-format=json`

Source: *JSON Output*, the rustc book,
<https://doc.rust-lang.org/nightly/rustc/json.html>, read 2026-10-05 (twice,
the quotations agreeing).

- **Fields of a span** (quoted from the page's schema comments):
  `byte_start`, "The byte offset where the span starts (0-based, inclusive)";
  `byte_end`, "(0-based, exclusive)"; `line_start` and `line_end`, 1-based,
  inclusive; `column_start`, "The first character offset of the line_start
  (1-based, inclusive)"; `column_end`, "The last character offset of the
  line_end (1-based, exclusive)". So **two units side by side**: offsets in
  bytes, columns in characters. **verified**.
- **The fix** lives on a span, not beside it: `suggested_replacement`, "An
  optional string of a suggested replacement for this span to solve the issue.
  Tools may try to replace the contents of the span with this text", and
  `suggestion_applicability`, one of `MachineApplicable`, `MaybeIncorrect`,
  `HasPlaceholders`, `Unspecified`, with "This suggestion should be
  automatically applied" said of the first alone. **verified**.
- **The file** is per span, `file_name`, with the page's warning that "this
  path may not exist" (a span in the standard library). **verified**.
- **The replaced text**: each span carries a `text` array, "the entire lines of
  text where the span is located", with `highlight_start` and `highlight_end`
  as 1-based character offsets. So rustc ships the text around the place, the
  whole line, not only the replaced bytes. **verified**.
- **The rendered text rides along**: `rendered`, "Optional string of the
  rendered version of the diagnostic as displayed by rustc". So rustc's JSON
  says at least what its text says, by construction. **verified**.
- **Versioning**: the page shows **no version field**. Its stability sentence,
  quoted: "When parsing, care should be taken to be forwards-compatible with
  future changes to the format. Optional values may be `null`. New fields may
  be added. Enumerated fields like "level" or "suggestion_applicability" may
  add new values." So rustc's policy is **added fields under no version at
  all**, consumers told to tolerate them. **verified**.
- **The local run agrees**: the critic's rustc 1.90.0 printed `byte_start` 37,
  `byte_end` 41 and `column_start` 32 where bytes give 38 and UTF-16 units 33
  (1.24), so rustc's "character" is the code point, Heroes' `col` unit.

### 1a. rustfix and `cargo fix`, the applier of those fixes

This is the closest precedent to `check --apply`: a separate consumer applying
a compiler's JSON fixes.

- **It applies by byte offset and carries line and column as metadata.**
  rustfix's `span_to_snippet` builds `range: (span.byte_start as
  usize)..(span.byte_end as usize)` and copies `line_start`/`column_start`
  into a `LineRange` beside it. Source:
  <https://raw.githubusercontent.com/rust-lang/cargo/master/crates/rustfix/src/lib.rs>,
  read 2026-10-05. Its replacement container's ranges are "inclusive of the
  start, exclusive of the end", byte indices into the original data. Source:
  <https://raw.githubusercontent.com/rust-lang/cargo/master/crates/rustfix/src/replace.rs>,
  read 2026-10-05. **verified**.
- **One fix, several edits**: a child diagnostic's spans become one `Solution`
  with `replacements: Vec<Replacement>` (same `lib.rs`). rustc carries a
  multipart suggestion as several spans of one `help` child, each with its own
  `suggested_replacement`. **verified** (structure read in rustfix's source;
  the rustc book does not say in words that a child's spans go together,
  which I searched for on that page and did not find).
- **It iterates, and the reason is overlap.** cargo issue #5813, opened by
  alexcrichton on 2018-07-26, "cargo-fix: Overlapping suggestions should be
  discarded and cargo-fix rerun",
  <https://github.com/rust-lang/cargo/issues/5813>, read 2026-10-05; closed by
  PR #5842, "fix: Iteratively apply suggestions from the compiler", opened
  2018-07-31, merged 2018-08-01, a maximum of **4** rounds overridable by
  `CARGO_FIX_MAX_RETRIES`, stopping when a round applies nothing,
  <https://github.com/rust-lang/cargo/pull/5842>, read 2026-10-05. The
  module comment of `fix.rs` today gives the example `::foo::<::Bar>();`,
  "The spans for these two suggestions are overlapping and its difficult in
  the compiler to **not** have overlapping spans here", and "In theory we
  probably need an infinite number of times to apply fixes, but we're not
  gonna sit around waiting for that"; `unwrap_or(4)` is the default. Source:
  <https://android.googlesource.com/toolchain/rustc/+/HEAD/src/tools/cargo/src/cargo/ops/fix.rs>
  (Android's mirror of the rustc tree; the GitHub path returned 404 to me),
  read 2026-10-05. **verified**. Heroes' `settle` with `ROUNDS` = 64 is the
  same design with a larger bound.
- **Twins were applied twice until 2024.** rust-lang/rust#123304, ehuss,
  2024-03-31, "unsafe_op_in_unsafe_fn suggestion overlaps when used in a
  macro": a macro invoked twice made one suggestion appear twice at one place,
  and, quoted, "This causes `cargo fix` to apply both suggestions which ends up
  with: `pub unsafe fn $x() { unsafe { unsafe { let _ =
  String::new().as_mut_vec(); }}}`", which then fires `unused_unsafe`.
  <https://github.com/rust-lang/rust/issues/123304>, read 2026-10-05. Repaired
  by cargo PR #13728, weihanglo, opened 2024-04-09, merged 2024-04-10, "fix(cargo-fix):
  dont apply same suggestion twice": "if any of the machine applicable fixes
  (`solution`) in a diagnostic is a duplicate, then `cargo fix` should only
  apply the entire suggestion once".
  <https://github.com/rust-lang/cargo/pull/13728>, read 2026-10-05.
  **verified**. Heroes' "an identical twin is written once" is this repair.
- **Half-applied multipart fixes broke the syntax until 2024.** cargo issue
  #14699, blyxyas, 2024-10-16, "[rustfix] Two suggestions applying to the same
  span cause syntax errors" (`cargo clippy --fix` on an `impl Into`),
  <https://github.com/rust-lang/cargo/issues/14699>; repaired by PR #14747,
  saites, opened 2024-10-30, merged 2024-11-02, "Add transactional semantics
  to `rustfix`": before it, a solution's replacements were applied piecemeal
  and a conflict left some applied; after it, all replacements of a solution
  apply or none do (the PR's words: "enabling `rustfix::CodeFix` to apply
  `Suggestion`s as atomic units and rollback partially-applied changes when
  they conflict with existing ones"),
  <https://github.com/rust-lang/cargo/pull/14747>; then PR #14782, saites,
  2024-11-05, gave `Error::AlreadyReplaced` an `is_identical` field so a caller
  skips an identical replacement and refuses a different one,
  <https://github.com/rust-lang/cargo/pull/14782>. All read 2026-10-05.
  **verified**. Heroes' `certain.hero:15-18` records the same two failure
  shapes (an abort, and `print(total(xs.must())rint(0)`), found in-house.

## 2. clang

Source: *Clang Compiler User's Manual*,
<https://clang.llvm.org/docs/UsersManual.html>, read 2026-10-05 (twice, the
quotations agreeing).

- **`-fdiagnostics-parseable-fixits`** prints `fix-it:"t.cpp":{7:25-7:29}:"Gamma"`:
  "The range printed is a half-open range, so in this example the characters
  at column 25 up to but not including column 29 on line 7 in t.cpp should be
  replaced with the string "Gamma". Either the range or the replacement string
  may be empty (representing strict insertions and strict erasures,
  respectively)." And, the unit: **"The printed column numbers count bytes
  from the beginning of the line; take care if your source contains multibyte
  characters."** Line and byte column, start and end, no offset, no replaced
  text. **verified**; the critic's Apple clang 21 run agrees (a byte column,
  1.24).
- **Why bytes**: the LLVM thread "Fixits with multibyte chars", started by
  Jordan Rose on 2012-07-16 after clang crashed printing a fix-it holding a
  three-byte UTF-8 character ("is a three-byte UTF-8 character three columns
  or one column?"); Eli Friedman answered that "Machine-parsable 'columns' are
  not the same as columns in the terminal" and argued for "one 'column' per
  Unicode code point"; Jordan Rose kept byte offsets, noting "normal column
  numbers (-fshow-column) also use byte offsets, not character counts", and
  fixed the crash in r160319.
  <https://discourse.llvm.org/t/fixits-with-multibyte-chars/23837>, read
  2026-10-05. **verified** as the fetch quoted it. The unit was argued, and
  chosen; it was not an accident.
- **No JSON format**: the manual lists `-fdiagnostics-format=` values
  `clang`, `msvc`, `vi` and `sarif`. **verified** on that page.
- **SARIF**: "Emit diagnostics as a SARIF JSON document. SARIF diagnostics are
  written to standard error." and "The SARIF diagnostic format is currently
  unstable." **verified**. Like Heroes' `check --json`, the document goes to
  stderr.
- **clang's SARIF carries no fixes**: `clang::SarifResult`'s public members are
  `setIndex`, `setRuleId`, `setDiagnosticMessage`, `setHostedViewerURI`,
  `addLocations`, `addRelatedLocations`, `setThreadFlows`,
  `setDiagnosticLevel`, `addPartialFingerprint` and `create`, none naming a
  fix or a replacement, by two routes: the doxygen class page
  <https://clang.llvm.org/doxygen/classclang_1_1SarifResult.html> and the
  header <https://raw.githubusercontent.com/llvm/llvm-project/main/clang/include/clang/Basic/Sarif.h>,
  both read 2026-10-05. `Sarif.cpp` writes `{"columnKind",
  "unicodeCodePoints"}` and converts byte columns in `adjustColumnPos`
  (<https://raw.githubusercontent.com/llvm/llvm-project/main/clang/lib/Basic/Sarif.cpp>,
  read 2026-10-05, through the fetch's summary only). **verified**; the
  critic's Apple clang 21 run (no `fixes`, `columnKind` `unicodeCodePoints`)
  agrees.
- **clang's tooling applies by bytes**: `clang::tooling::Replacement` "Creates
  a replacement of the range [Offset, Offset+Length) in FilePath with
  ReplacementText", Offset being "The byte offset of the start of the range in
  the file" and Length "The length of the range in bytes".
  <https://clang.llvm.org/doxygen/classclang_1_1tooling_1_1Replacement.html>,
  read 2026-10-06. clang-tidy's `--export-fixes`: "YAML file to store
  suggested fixes in. The stored fixes can be applied to the input source code
  with clang-apply-replacements." <https://clang.llvm.org/extra/clang-tidy/>,
  read 2026-10-05. **verified**; that the exported YAML carries exactly these
  `Replacement` fields is **unverified** (the page does not show the YAML).
- **So clang uses three framings for one place**: byte columns in the
  parseable fix-its and the text, code points in SARIF, byte offsets with a
  file per edit in its tooling. **verified** (the statements above).

## 3. gcc

- **`-fdiagnostics-parseable-fixits`**: "The location is expressed as a
  half-open range, expressed as a count of bytes, starting at byte 1 for the
  initial column." Example `fix-it:"test.c":{45:3-45:21}:"gtk_widget_show_all"`.
  <https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html>,
  read 2026-10-05. **verified**.
- **The JSON format, while it lived** (GCC 13.2 manual,
  <https://gcc.gnu.org/onlinedocs/gcc-13.2.0/gcc/Diagnostic-Message-Formatting-Options.html>,
  read 2026-10-05): "The emitted JSON consists of a top-level JSON array
  containing JSON objects representing the diagnostics", so there is no object
  in which a version could stand. A location has "a `caret` position and
  optional `start` and `finish` positions"; a position has `file`, `line` and
  three columns: "`display-column` counts display columns, accounting for tabs
  and multibyte characters", "`byte-column` counts raw bytes", and "`column`
  is equal to one of the previous two, as dictated by the
  -fdiagnostics-column-unit option". Fix-its: "a `fixits` array, consisting of
  half-open intervals", each "replacing the text from `start` up to but not
  including `next` with `string`'s value. Deletions are expressed via an empty
  value for `string`, insertions by having `start` equal `next`". **verified**.
- **The display column is not a character count**: the current manual says
  the default unit 'display' "considers the number of display columns
  occupied by each character. This may be larger than the number of bytes ...
  in the case of tab characters, or it may be smaller, in the case of
  multibyte characters", with U+1F642 occupying "two display columns". Same
  page as above, read 2026-10-05. **verified**. So gcc's default `column`
  equals neither Heroes' character `col` nor bytes on a line holding an emoji
  or a tab.
- **The unit of an existing field changed under no version, in GCC 11.**
  GCC 11 release notes: "In previous releases of GCC, the "column numbers"
  emitted in diagnostics were actually a count of bytes from the start of the
  source line." "In GCC 11 the column numbers default to being column numbers,
  respecting multi-column characters." "The output of
  -fdiagnostics-format=json has been extended to supply both byte counts and
  column numbers for all source locations."
  <https://gcc.gnu.org/gcc-11/changes.html>, read 2026-10-05. The GCC 11.1.0
  manual then defines JSON's `column` as following -fdiagnostics-column-unit,
  whose default is 'display'
  (<https://gcc.gnu.org/onlinedocs/gcc-11.1.0/gcc/Diagnostic-Message-Formatting-Options.html>,
  read 2026-10-05). **verified**: by those two pages, a consumer reading
  `column` got bytes before 11 and display columns from 11, in a format with
  no version. Whether any consumer broke on it: **no record found**. Searched
  2026-10-06 for gcc 11, `-fdiagnostics-format=json`, `column`, display
  column and a broken parser; the results were gcc's own pages and a summary
  that said it "could" break parsers, which is a guess and not a record. A
  question, not a premise.
- **Deprecated, then removed.** GCC 15: "The `json` format for
  -fdiagnostics-format= is deprecated and may be removed in a future release.
  Users seeking machine-readable diagnostics from GCC should use SARIF."
  <https://gcc.gnu.org/gcc-15/changes.html>. GCC 16: "The so-called "`json`"
  format for -fdiagnostics-format= has been removed in this release", and "In
  GCC's SARIF output, `fix` objects now contain `description` properties in
  many cases." <https://gcc.gnu.org/gcc-16/changes.html>. Both read
  2026-10-05. **verified**.
- **gcc's SARIF**: added in GCC 13 ("GCC can now emit its diagnostics using
  SARIF", `sarif-stderr` and `sarif-file`, <https://gcc.gnu.org/gcc-13/changes.html>,
  read 2026-10-05); "Fix-it hints are printed to text sinks, and are emitted
  by SARIF sinks as `fix` objects (see SARIF 2.1.0 §3.55 fix object)"
  (*Fix-it hints*, libgdiagnostics documentation,
  <https://gcc.gnu.org/onlinedocs/libgdiagnostics/topics/fix-it-hints.html>,
  read 2026-10-05). GCC 15 added "Experimental SARIF 2.2 output ... via
  -fdiagnostics-add-output=sarif:version=2.2-prerelease" (GCC 15 changes,
  above). **verified**. Which `columnKind` gcc's SARIF declares:
  **unverified** (the source file was too long for the fetch, and the GCC wiki
  refused the automated read).

## 4. The Language Server Protocol, 3.17

Sources: *Language Server Protocol Specification - 3.17*,
<https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/>,
read 2026-10-05; and, because that page is long enough to truncate the fetch,
the specification's own source files under
<https://raw.githubusercontent.com/microsoft/language-server-protocol/gh-pages/_specifications/lsp/3.17/>:
`types/textDocuments.md`, `types/position.md`, `types/textEdit.md`,
`types/textEditArray.md`, `types/textDocumentEdit.md` and
`general/initialize.md`, all read 2026-10-05.

- **`Position`** is `{line, character}`, both zero-based; `character`:
  "Character offset on a line in a document (zero-based). The meaning of this
  offset is determined by the negotiated `PositionEncodingKind`. If the
  character value is greater than the line length it defaults back to the
  line length." **`Range`** is `{start, end}`, "The end position is
  exclusive." **`TextEdit`** is `{range, newText}`: "To insert text into a
  document create a range where start === end", and "For delete operations
  use an empty string". **No offset field anywhere.** **verified**.
- **The default unit and the negotiation** (`textDocuments.md`): "Prior to
  3.17 the offsets were always based on a UTF-16 string representation. So in
  a string of the form `a𐐀b` the character offset of the character `a` is 0,
  the character offset of `𐐀` is 1 and the character offset of b is 3 since
  `𐐀` is represented using two code units in UTF-16." "Since 3.17 clients and
  servers can agree on a different string encoding representation (e.g.
  UTF-8)." "To stay backwards compatible the only mandatory encoding is UTF-16
  represented via the string `utf-16`." The client lists
  `general.positionEncodings` ("Client and server have to agree on the same
  position encoding to ensure that offsets (e.g. character position in a line)
  are interpreted the same on both side") and the server answers
  `positionEncoding` ("If the client didn't provide any position encodings
  the only valid value that a server can return is 'utf-16'. If omitted it
  defaults to 'utf-16'.") (`initialize.md`). The kinds (`position.md`):
  `utf-8`, "Character offsets count UTF-8 code units (e.g bytes)"; `utf-16`,
  "This is the default and must always be supported by servers"; `utf-32`,
  "these are the same as Unicode code points, so this `PositionEncodingKind`
  may also be used for an encoding-agnostic representation of character
  offsets". **verified**. One further sentence of the capability's comment,
  that conversion "is best done where the file is read which is usually on
  the server side", came only through the summary of the long page.
- **Edits in one array** (`textEditArray.md`): "Complex text manipulations
  are described with an array of `TextEdit`'s or `AnnotatedTextEdit`'s,
  representing a single change to the document." "All text edits ranges refer
  to positions in the document they are computed on." "They therefore move a
  document from state S1 to S2 without describing any intermediate state."
  "Text edits ranges must never overlap, that means no part of the original
  document must be manipulated by more than one edit." "If multiple inserts
  have the same position, the order in the array defines the order in which
  the inserted strings appear in the resulting text." **verified**.
- **A stale document is refused by version** (`textDocumentEdit.md`): "The
  text document is referred to as a `OptionalVersionedTextDocumentIdentifier`
  to allow clients to check the text document version before an edit is
  applied." "A `TextDocumentEdit` describes all changes on a version Si and
  after they are applied move the document to version Si+1." **verified**.
  This is the protocol's answer to the critic's stale-file route.
- **Line endings** (`textDocuments.md`): "the protocol specifies the following
  end-of-line sequences: '\n', '\r\n' and '\r'." "Positions are line end
  character agnostic." **verified**.
- **How the default came to cost**: LSP issue #376, MaskRay, 2018-01-13,
  "Change character units from UTF-16 code unit to Unicode codepoint", closed
  under milestone 3.17,
  <https://github.com/microsoft/language-server-protocol/issues/376>, read
  2026-10-05. **verified** (title, author, date, milestone). 3.17.0's
  change-log heading reads `3.17.0 (05/10/2022)` according to a search
  index's match on the specification's source file, 2026-10-06; I could not
  read that heading myself (the page truncated the fetch, and the protocol's
  home page names only 3.18): **unverified on the page**. clangd shipped its
  own `offsetEncoding` extension because, quoted, "LSP specifies that offsets
  within lines are in UTF-16 code units (for `Position`s and also
  delta-encoded document updates)", and today: "This extension has been
  deprecated with `clangd-21` in favor of the `positionEncoding` introduced in
  LSP 3.17. It'll go away with `clangd-23`." <https://clangd.llvm.org/extensions>,
  read 2026-10-05. **verified**.

## 5. SARIF 2.1.0 (with Errata 01)

Source: *Static Analysis Results Interchange Format (SARIF) Version 2.1.0 Plus
Errata 01*, OASIS Standard incorporating Approved Errata, 28 August 2023, 229
pages, <https://docs.oasis-open.org/sarif/sarif/v2.1.0/sarif-v2.1.0.pdf>,
read 2026-10-05 page by page (page numbers are the PDF's). Version 2.1.0
itself was an OASIS Standard on 27 March 2020 (its page 2). Schema:
<https://docs.oasis-open.org/sarif/sarif/v2.1.0/errata01/os/schemas/sarif-schema-2.1.0.json>,
read 2026-10-05.

- **The shape** (pp. 118, 182-185): a `result` "MAY contain a property named
  `fixes`" (§3.27.30); a `fix` "represents a proposed fix ... It specifies a
  set of artifacts to modify. For each artifact, it specifies regions to
  remove, and provides new content to insert" (§3.55.1), with
  `artifactChanges`, "an array of one or more unique `artifactChange` objects
  each of which describes the changes to a single artifact that are necessary
  to effect the fix", whose elements "SHALL refer to distinct artifacts"
  (§3.55.3); each `artifactChange` has `artifactLocation` and `replacements`,
  "an array of one or more `replacement` objects" (§3.56). **One fix, many
  edits, across files.** **verified**.
- **How a fix's edits combine** (p. 185, §3.57.1): "If a single
  `artifactChange` object specifies more than one replacement, then the effect
  of the replacements SHALL be as if they were performed in the order they
  appear in the `replacements` array. The `deletedRegion` property of each
  `replacement` object SHALL specify the location of the replacement in the
  unmodified artifact." Its example walks it: the second replacement "removes
  3 bytes starting at offset 20 *with respect to the unmodified file*".
  **verified**. Whether replacements may overlap, whether a consumer must
  apply every replacement of a fix or may apply some, and how several fixes of
  one result relate: **not found** on pages 118 and 182-187, which I read
  whole; a question, not a premise.
- **The replacement** (pp. 185-187): `deletedRegion` "SHALL" be present
  (§3.57.3), `insertedContent` "MAY" (§3.57.4); a zero-length region is an
  insertion point, and the consumer "SHALL NOT remove any content". "When
  performing a replacement in a text artifact, the SARIF producer SHOULD
  specify a text replacement rather than a binary replacement. This allows the
  SARIF producer to specify the region without regard to whether the artifact
  starts with a byte order mark (BOM)." Inserted text "SHALL be transcoded
  from UTF-8 ... to the encoding of the target artifact", with the note: "This
  implies that a text fix cannot be safely applied unless the target
  artifact's encoding is known." **verified**.
- **The region's fields and units** (pp. 122-127, §3.30): a text region by
  `startLine`, `startColumn`, `endLine`, `endColumn` (1-based; "A text region
  does not include the character specified by `endColumn`"), or by
  `charOffset` ("the zero-based character offset of the first character in
  the region from the beginning of the artifact") and `charLength`; a binary
  region by `byteOffset` ("zero-based byte offset") and `byteLength`. "Column
  numbers are expressed in the measurement unit specified by
  `theRun.columnKind`". If both a text and a binary region are given they
  "SHALL specify the identical range of bytes", and the two sets "SHALL be
  treated independently ... the value of a text-related property SHALL NOT be
  inferred from the value of any set of binary-related properties". "The
  values of text properties SHALL NOT depend on the presence or absence of a
  byte order mark (BOM)". A region may carry a `snippet`, which "allows a
  SARIF viewer to present the contents of the region even if the artifact
  from which it was taken is not available" and "can be used to improve
  result matching" (§3.30.13). **verified**.
- **The unit is declared, never defaulted** (pp. 57-58, §3.14.27): "If a
  SARIF producer processes text artifacts and `theRun.results` is non-empty,
  the `run` object SHALL contain a property named `columnKind` ...";
  `utf16CodeUnits` ("a surrogate pair is considered to occupy two columns") or
  `unicodeCodePoints` ("even a character that is represented in UTF-16 by a
  surrogate pair is considered to occupy one column"). And: "If a SARIF
  consumer uses a column measurement unit other than that specified by
  `columnKind`, and if the consumer is required to interact with the
  artifact's contents ..., the consumer SHALL recompute column numbers in its
  (the consumer's) native measurement unit." **There is no byte column
  kind**; bytes exist only as `byteOffset`/`byteLength`. **verified**.
- **A byte column was asked for**: sarif-spec issue #466, haya14busa,
  2020-07-20, "UTF8 bytes count support as columnKind?", arguing that with
  bytes a tool "can just replace the content of the byte in the specified
  range", closed, labelled 2.2,
  <https://github.com/oasis-tcs/sarif-spec/issues/466>, read 2026-10-05.
  **verified**; whether SARIF 2.2 adopted it: **unverified**.
- **Line endings matter to an applier** (p. 57, §3.14.26): `newlineSequences`
  defaults to `[ "\r\n", "\n" ]`, and its note says "a tool that applies fixes
  (see §3.55), especially one that applies them automatically, can use this
  property to ensure that it inserts and removes content on the correct
  lines." **verified**.
- **Versioning** (p. 47, §3.13.2): "A `sarifLog` object SHALL contain a
  property named `version` ... This string SHALL have the value "2.1.0"", and
  it "SHOULD appear first", the note saying "This will make it easier for
  parsers to handle multiple versions of the SARIF format if new versions are
  defined in the future." **verified**.

## 6. Other appliers the brief did not list

The list in the brief is five; these four came from the searches above, so
the set is my enumeration and not the world's. Each answers a question the
sitting asks.

- **Go's analysis framework** (`golang.org/x/tools/go/analysis`, v0.51.0,
  <https://pkg.go.dev/golang.org/x/tools/go/analysis>, read 2026-10-05). A
  `TextEdit` "represents the replacement of the code between Pos and End with
  the new text. Each TextEdit should apply to a single file." A
  `SuggestedFix`'s "TextEdits must not overlap, nor contain edits for other
  packages. Edits need not be totally ordered, but the order determines how
  insertions at the same point will be applied." **How fixes combine, stated
  in so many words**: a diagnostic's `SuggestedFixes` "Each one represents an
  alternative strategy, and should have a distinct and descriptive message;
  at most one may be applied", and "Fixes for different diagnostics should be
  treated as independent changes to the same baseline file state, analogous
  to a git commit with the same parent. Combining fixes requires resolving any
  conflicts that arise, analogous to a git merge. Any conflicts that remain
  may be dealt with, depending on the tool, by discarding fixes, consulting
  the user, or aborting the operation." **verified**. Its JSON (`-json`)
  gained the fixes on 2022-09-30, Lasse Folger, "x/tools/go/analysis: extend
  json output by SuggestedFixes": "The edits are encoded as replacements for
  ranges defined by 0 based byte start and end indicies into the original
  file", the struct comment "Start and End are zero-based half-open indices
  into the original byte sequence of the file", and "A JSONSuggestedFix
  describes an edit that should be applied as a whole or not at all"; the
  commit "exports the structs that are used for JSON encoding and thus
  documents the JSON schema", with no version field.
  <https://groups.google.com/g/golang-checkins/c/fsmEQ9UfCfk>, read
  2026-10-05. **verified** as the fetch quoted it.
- **ESLint** (<https://eslint.org/docs/latest/extend/custom-rules>, read
  2026-10-05): "A `range` is a two-item array containing character indices
  inside the source code. The first item is the start of the range
  (inclusive) and the second item is the end of the range (exclusive)."
  "After applying fixes, ESLint will run all the enabled rules again on the
  fixed code, potentially applying more fixes. This process will repeat up to
  10 times, or until no more fixable problems are found." "if two fixes want
  to modify characters 0 through 5, only one is applied." A rule's `fix()`
  may return several fixing objects, which "must not overlap". **verified**.
  The message carries one auto-applied `fix` and, apart, `suggestions`, "the
  pair of a description and an EditInfo object", an `EditInfo` being `range`,
  "The pair of 0-based indices in source code text to remove", and `text`
  (<https://eslint.org/docs/latest/integrate/nodejs-api>, read 2026-10-05).
  And **the applier's own answer in JSON**: `--fix-dry-run` "has the same
  effect as `--fix` with the difference that the fixes are not saved to the
  file system. Because the default formatter does not output the fixed code,
  you'll have to use another formatter (e.g. `--format json`) to get the
  fixes" (<https://eslint.org/docs/latest/use/command-line-interface>, read
  2026-10-05), the result's `output` being "The modified source code text.
  This property is undefined if any fixable messages didn't exist" (Node.js
  API page, read 2026-10-06). **verified**. That ESLint's "character indices"
  are UTF-16 code units, JavaScript's string indices, is my **inference**;
  the pages do not say it.
- **clang's tooling**: byte offset and length per replacement, a file per
  replacement (§ 2 above). **verified**.
- **Motoko's `mops check --fix`**, which "applies edits by byte offset" (§ 8,
  the failure it met).

## 7. Who moved a version for an added field

| format | version in the answer | an added field, and what it cost |
|---|---|---|
| rustc JSON | none | `$message_type`, rust-lang/rust PR #115691, jsgf, opened 2023-09-09, merged 2023-11-21 ("Currently the json-formatted outputs have no way to unambiguously determine which kind of message is being output"), <https://github.com/rust-lang/rust/pull/115691>, read 2026-10-05; cargo had to adapt first: PR #13016, dtolnay, 2023-11-20, "Handle $message_type in JSON diagnostics", which "Unblocks rust-lang/rust#115691", two cargo tests that compared the JSON exactly (`doc::doc_message_format`, `metabuild::metabuild_failed_build_json`) failing until the field was made optional for compilers before 1.76, <https://github.com/rust-lang/cargo/pull/13016>, read 2026-10-05. **verified** |
| gcc JSON | none (a top-level array) | GCC 11 added `byte-column` and `display-column` and changed `column`'s default unit; deprecated in GCC 15, removed in GCC 16 (§ 3). **verified** |
| Go analysis JSON | none | the fixes themselves, 2022-09-30 (§ 6). **verified** |
| clang | the parseable fix-its are text with no version; its SARIF carries SARIF's `"2.1.0"` and is "currently unstable" (§ 2) | none found. |
| LSP | the protocol's version, 3.17 | `positionEncoding` entered as a negotiated capability whose absence means the old behaviour: "To stay backwards compatible the only mandatory encoding is UTF-16" (§ 4). **verified** |
| SARIF | `version` SHALL be `"2.1.0"` | new properties wait for a new version of the standard; gcc emits a draft 2.2 under `version=2.2-prerelease` (§ 3, § 5). **verified** |
| `cargo metadata` | `"version": 1`, "The version of the schema for this metadata structure. This will be changed if incompatible changes are ever made." | by stated policy, fields are added under 1: "New fields will be added when needed. Reserving this helps Cargo evolve without bumping the format version too often." `--format-version`: "Currently `1` is the only possible value." <https://doc.rust-lang.org/cargo/commands/cargo-metadata.html>, read 2026-10-05. **verified**. **This is Heroes' shape**: a schema number of 1 with additive change under it. |
| rustdoc JSON | `format_version`, 61 in `rustdoc-types` 0.61.0: "This integer is incremented with every breaking change to the API, and is returned along with the JSON blob as Crate::format_version. Consuming code should assert that this value matches the format version(s) that it supports." <https://docs.rs/rustdoc-types/latest/rustdoc_types/constant.FORMAT_VERSION.html>, read 2026-10-06 | **the one that bumps for an added field**: its maintainer, Alona Enraght-Moony, 2023-12-31, states the metaformat as "a field called `format_version` as the root of the JSON object, that is incremented on every change", counts 13 format changes in 2022 and 5 in 2023, one of them PR #119246, "Add `is_object_safe` field to `Trait`", <https://alona.page/posts/rustdoc-json-2023/>, read 2026-10-05. Its recorded cost: rust-lang/rust#94591, aDotInTheVoid, 2022-03-04, "If merge A and merge B both try to increase the version from 10 to 11, git will helpfully merge both of them, resulting in the version 11 having both changes", <https://github.com/rust-lang/rust/issues/94591>, read 2026-10-05. **verified** as the fetch quoted them. |

So of the formats read, **one moves its version for an added field
(rustdoc JSON); `cargo metadata` keeps 1 and adds; rustc, gcc and Go have no
version and add**; LSP and SARIF version the whole protocol. The two costs on
record cut both ways: an added field broke exact-comparison readers (cargo's
two tests, 2023), and a version bumped by parallel branches merged silently
(rustdoc, 2022), which is the shape of Heroes' lanes merging into one trunk.
The third record is gcc 11: the meaning of an existing field changed with no
version to say so.

## 8. Recorded failures caused by a column's unit, and the shape beside it

**The unit of a column, read in the wrong unit:**

- **An applier misplaced a server's edits.** vim-lsp issue #1687,
  teruteru128, 2026-08-06, open: with clangd 20's `textDocument/formatting`
  on a C file holding emoji, the identifier `targetbits` came out split as
  `targe` and `bits` on two lines, because "vim-lsp counts each surrogate pair
  as 1 UTF-16 code unit instead of 2 when translating LSP offsets to buffer
  positions". <https://github.com/prabirshrestha/vim-lsp/issues/1687>, read
  2026-10-05. **verified**. The closest record to the brief's question: a tool
  applying another tool's edits wrongly because of a column's unit.
- **A server read the protocol's unit as bytes.** LLVM review D46035,
  sammccall, 2018-04-24, "[clangd] Fix unicode handling, using UTF-16 where
  LSP requires it": the conversions "pretended that Position.character was
  UTF-8 bytes, which is only true for ASCII lines"; committed 2018-04-27 as
  rL331029. <https://reviews.llvm.org/D46035>, read 2026-10-05. **verified**.
- **A client sent code points as UTF-16 units.** emacs-lsp/lsp-mode issue
  #2080, Vtec234, 2020-08-14, "Incorrect handling of multibyte UTF-16
  encodings": after a lemon emoji (U+1F34B, two UTF-16 units) the client sent
  `"character":1` where 2 was owed, because "Emacs counts code points".
  Closed. <https://github.com/emacs-lsp/lsp-mode/issues/2080>, read
  2026-10-05. **verified**.
- **A consumer read code points as bytes.** rust-lang/rust PR #119033,
  Zalathar, opened 2023-12-17, merged 2024-01-09, "coverage: `llvm-cov`
  expects column numbers to be bytes, not code points": given code points,
  llvm-cov would "slice strings in the wrong places, producing mangled output
  or fatal errors". <https://github.com/rust-lang/rust/pull/119033>, read
  2026-10-05. Its report, taiki-e/cargo-llvm-cov issue #275, Dushistov,
  2023-05-05, "html output break utf-8": the HTML split the two bytes of `Я`
  around a tag. <https://github.com/taiki-e/cargo-llvm-cov/issues/275>, read
  2026-10-05. **verified**.
- **The producer crashed on its own unit.** The LLVM thread of 2012 (§ 2):
  clang crashed printing a fix-it holding a multibyte character. **verified**.

**The shape beside it: an offset taken in the wrong frame.** This is the one
Heroes is nearest, since a `Span` indexes one text of every file read,
concatenated (the shared brief; critic 1.15).

- **rustc, 2019.** rust-lang/rust issue #65029, Rantanen, 2019-10-02,
  "libsyntax JSON output bytes counts ignore CRLF normalization": rustc
  normalised CRLF to LF early, so "the byte_start/byte_end values cannot be
  used to index the actual bytes of the original file, if the original file
  content was affected by the CRLF normalization", the issue saying this is
  "most evident in rustfix on Windows".
  <https://github.com/rust-lang/rust/issues/65029>, read 2026-10-05. Repaired
  by PR #65074, Rantanen, opened 2019-10-03, merged 2019-10-25: "Track the
  changes made during normalization in the `SourceFile` and use this
  information to correct the `start_byte` and `end_byte` fields in the JSON
  output", covering BOM removal as well.
  <https://github.com/rust-lang/rust/pull/65074>, read 2026-10-05.
  **verified**.
- **Motoko, last month.** caffeinelabs/motoko PR #6393, Kamirus, merged
  2026-09-24, "fix: diagnostic byte offsets and region text with CRLF or
  lone-CR line endings": `byte_start` and `byte_end` were "one byte early on
  every line after a `\r\n`", and `mops check --fix`, which "applies edits by
  byte offset", "garbled the file". The repair: "a plain byte scan: `\r`,
  `\n` and `\r\n` each count as one line break, as in the lexer".
  <https://github.com/caffeinelabs/motoko/pull/6393>, read 2026-10-05.
  **verified** as the fetch quoted it.

**What none of these is**: a record of an applier failing on the pair rustc
ships today and Heroes would, character columns with byte offsets beside
them. My searches, each named in this file, surfaced the llvm-cov case against
rustc's character columns (a consumer that wanted bytes) and #65029 against
its byte offsets, repaired in 2019, and nothing later against either. A
negative resting on searches not aimed at it, so a question.

## 9. What the precedent says of each route on the table

- **The place, as offsets and line and column, start and end.** Every format
  read gives an end: rustc (`byte_end`, `column_end`), clang (`{l:c-l:c}`),
  gcc (`next`), Go (`End`), LSP (`range.end`), SARIF (`endColumn`,
  `charLength`, `byteLength`), ESLint (`range[1]`), clang's `Replacement`
  (`Length`). **None places a fix by its start alone**; the rendered half's
  start-only form is for a reader, and a deletion cannot be applied from it.
  The compiler toolchains hand their appliers **bytes into the file as
  stored** (rustfix, Go's JSON, clang's tooling, mops); the editor protocols
  apply by line and a negotiated or declared column (LSP, SARIF). rustc
  carries both, and its column unit is Heroes' `col` unit.
- **Each fix carries the text it replaces.** Precedented three ways: rustc's
  `text` (whole lines), SARIF's region `snippet`, "to improve result
  matching", and LSP's document version, "to allow clients to check the text
  document version before an edit is applied". Heroes has no document
  version, so the text is the guard available. Supported.
- **One fix of several edits.** Unanimous among the precedents: rustc's
  multipart suggestion, Go's `SuggestedFix`, SARIF's `replacements`, LSP's
  edit array, ESLint's several fixing objects. rustfix learned in 2024 that
  such a fix must apply whole or not at all (cargo #14699, PR #14747). Whether
  Heroes needs it now is Principle 0's question, not mine; if it comes, it
  changes what `fixes` holds, and by `cargo metadata`'s own rule that is the
  incompatible change that moves the number.
- **The answer declares its column unit.** SARIF makes it a SHALL with no
  default; LSP negotiates and defaults; rustc, clang and gcc state it only in
  prose, and gcc changed it in GCC 11 with nothing in the answer to say so.
  The LSP failures of § 8 all come from a unit fixed in prose and miscounted
  by one side. Supported, and SARIF's vocabulary already names Heroes' unit:
  `unicodeCodePoints`.
- **`check --apply --json` returns what `--apply` writes.** The multi-round
  appliers (cargo fix, 4 rounds; ESLint, 10 passes; Heroes, 64) exist because
  one answer's places do not compose in general (cargo's `::foo::<::Bar>`
  comment). ESLint answers the dry-run in JSON with the **fixed text**
  (`output`), not with edits. I found no applier that answers with the
  composed edits of its rounds; that is a question, not a premise. Panel 016's
  "worst silent failure" (critic 2.7) has no precedent in its favour that I
  found: ESLint documents the pairing of fix and JSON.
- **Nothing.** The precedent for "nothing" is clang's and gcc's parseable
  fix-its, a place in text for tools to scrape; but they print an end, and
  Heroes' rendered half does not, so the two invocations do not compose to an
  applicable fix. Not supported by the precedent read.

## 10. Verdict, route, cost, prediction

**Verdict: approve** (advisory), with the place written as the precedent that
has lasted writes it, and the four things the failures teach.

**The route I would adopt**:
1. each fix gains a **start and an end**, as 0-based half-open **byte offsets
   into its own file as stored**, never the compiler's concatenated text (rust
   #65029, Motoko #6393), beside a **line and column in the units `check
   --json` already uses** (per-file line, character column), with the file
   named per fix where it differs from the diagnostic's (rustc, gcc, clang's
   tooling, SARIF and LSP carry the file with the edit, not only with the
   diagnostic);
2. the answer **declares its column unit** once, in SARIF's words if the
   panel likes them (`unicodeCodePoints`);
3. the answer's documentation **states how its fixes combine**, because Go
   (a diagnostic's fixes are alternatives, at most one), ESLint (one `fix`,
   several `suggestions`) and SARIF (one fix's replacements in order, against
   the unmodified file) each state a different rule, and a Go-trained consumer
   would read 178's three or four certain fixes on one diagnostic as
   alternatives (inference);
4. each fix **carries the text it replaces**, as SARIF's snippet and rustc's
   `text` do;
5. `check --apply --json` **answers** instead of ignoring the flag, with the
   applier's result, ESLint's shape;
6. **`schema` stays 1** for added fields, as `cargo metadata` keeps 1; it moves
   only if an existing field changes what it means (gcc 11) or `fixes` changes
   shape (one fix of several edits). The four places that write "schema 1"
   (critic 2.7) are then untouched; the two exact pins (`suite_surface.hero:286`,
   `check.hero:370-379`) are the readers rustc's `$message_type` broke in
   cargo's tests, and whether an added field inside a fix object moves them
   is **unrun** by me.

**Its cost**: not mine to measure; I have no tree. What the precedent prices
is the cost of leaving parts out: rustfix took from 2018 (rounds) to 2024
(twins, whole-or-nothing) to learn rules Heroes already has in
`certain.hero`; LSP's unit, fixed in prose, ran from issue #376 in 2018 to
3.17's negotiation, and clangd's own extension for it is still being retired
(clangd-23).

**A falsifiable prediction**: with per-file byte offsets, a consumer that
applies one answer's `certain` fixes at once (a twin once, a fix touching one
already written refused) writes what `check --apply` writes on all 139
`tests/golden/check/` cases that carry a `.fixed` or `.applied`, and on their
CRLF conversions, and differs on exactly the three
`surface-fixtures/certain137-*` cases, which need a second round as cargo's
`::foo::<::Bar>` does. If the offsets were the concatenated text's, the same
consumer would misplace every fix outside the root file (rust #65029's shape).
The critic's one-round measurement (1.3) is the baseline this would be read
against.

**What would change my verdict**: see the condition in § 11.

## 11. The answer, in the sitting's form

- **verdict**: approve (advisory).
- **precedents**: §§ 1 to 8 above, each marked verified or unverified with its
  page and day. In brief: rustc, byte offsets and character columns, start
  and end, file per span (verified); clang, byte columns in the parseable
  fix-its, code points and no fixes in SARIF, byte offset and length in its
  tooling (verified); gcc, bytes in the parseable fix-its, `byte-column`,
  `display-column` and `column` in a JSON now removed (verified); LSP, line and
  a negotiated column, UTF-16 by default, no offsets, a version to refuse a
  stale document (verified); SARIF, line and a declared column or character
  or byte offsets, `columnKind` a SHALL with no default and no byte kind
  (verified on the standard's pages); Go, byte offsets, fixes of one
  diagnostic alternatives (verified); ESLint, character indices, ten passes,
  the fixed text in JSON (verified; the UTF-16 reading is my inference).
- **argument** (119 words): Every fix format I read gives a start and an end;
  none places a fix by its start alone. The compiler toolchains hand their
  appliers byte offsets into the file as stored (rustfix, Go's JSON, clang's
  `Replacement`, mops) and keep line and column, as rustc's JSON carries both.
  The recorded failures sit at conversions: offsets taken from a normalised
  text (rustc 2019; Motoko 2026, which garbled files) and columns read in
  another unit (clangd 2018, lsp-mode 2020, llvm-cov 2023, vim-lsp 2026). So:
  per-file byte offsets beside Heroes' character line and column, the unit
  declared as SARIF requires, and the combination rule stated, as Go, ESLint
  and SARIF each state theirs. Additive fields keep `schema: 1`, as `cargo
  metadata` does.
- **condition**: my reading changes if any of these is found: an independent
  applier of a compiler's fixes that applies by character columns alone and
  has run for years with no recorded unit or line-ending failure (it would
  weaken the byte offsets); a record of an in-band unit declaration being
  ignored to harm (it would weaken the declaration); a one-shot applier that
  reproduces a multi-round applier from one answer's places (it would weaken
  `--apply --json` answering the result); a recorded break from a field added
  under an unchanged version in a format that reserved that right, as
  `cargo metadata` does (it would move `schema`); SARIF 2.2 adopting a byte
  `columnKind` (bytes could then be a declared column rather than offsets
  only).
