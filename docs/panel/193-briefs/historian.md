# Panel 193, the historian's brief

Read `00-shared.md` beside this file first; it holds the question and the
measured facts. You have no copy of the tree to build: your input is the web,
and every claim you make cites the page you read and the day you read it.

## Your task: how other compilers and protocols place a fix

Verify by web search, each with its source, what each of these writes for a
suggested fix's place, and in which unit:
- rustc's JSON diagnostics (`--error-format=json`): a span's byte and
  line/column fields, a suggestion's replacement and applicability;
- clang's `-fdiagnostics-parseable-fixits` and its JSON or SARIF output, if
  any;
- gcc's `-fdiagnostics-format=json` (or `json-file`, `sarif-file`): its
  fix-it hints' locations, the *byte-column* and *display-column* it reports;
- the Language Server Protocol's `TextEdit` and `Range`, its default position
  encoding and how 3.17 lets a client negotiate another;
- SARIF 2.1.0's `fix` and `replacement`, `deletedRegion`'s offsets and its
  line and column fields.

Then: which of these moved its schema's version for an added field, and which
added fields under one version; and any record of a tool that applied a
precedent's fixes wrongly because of the unit of a column. Advisory, no veto.
