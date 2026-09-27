# Panel 179, brief for the historian

Read `00-shared.md` first. Your seat is advisory, on precedent; you have no
veto, and an unsourced precedent is inadmissible: verify every date, name and
claim by web search and cite the page.

## The question

Formatters that ship their own self-check or self-fuzz as a capability of the
formatter's command line, rather than as a test script beside it. What exists,
what shape it took (a flag, a subcommand, a separate tool), what it checks
(idempotence, tree equality, comment preservation), and what happened to it.

Candidates to verify, not to trust: Prettier's `--debug-check` (what it
verifies, when it appeared, whether it survived); `rustfmt --check` and its
idempotence tests; `gofmt -d` and Go's `TestRewrite`/idempotence tests in the
standard library; `black --check` and Black's own fuzzing (Hypothesis-based
`fuzz.py` in its repository); `clang-format --dry-run`; `zig fmt --check`;
`deno fmt --check`; any formatter whose comment placement was a recorded class
of bugs (Black's and Prettier's issue trackers are candidates). Also: any
compiler that ships a `--fuzz`-like verb in its own binary (Zig's, Swift's or
Rust's toolchains may have one; verify rather than assume).

## What to report

- For each precedent: the tool, the exact flag or verb, what it checks, the
  release or commit that introduced it with its date, and the source URL.
- What the precedents say about the shape question the sitting decides: flag
  on the formatter versus a separate verb versus a test-only script. Where a
  tool moved from one to the other, say why, with the source.
- Comment preservation specifically: which formatters check it mechanically,
  and how (a token comparison, a reparse, a diff of comments).
- A prediction the coordinator can score: for example, that no formatter in
  wide use ships a generator of deformed inputs inside the formatter's own
  binary, or that at least one does; name the instrument (a URL that would
  falsify it).

Write your report as your final message (your seat has no file write; the
coordinator writes it out to `docs/panel/179-reports/historian.md`). English,
no em dashes.
