# M-lsp-server — `heroes lsp`


**Scheduled, no warrant.** ~250 lines of JSON-RPC: diagnostics on save,
formatting, hover, documentSymbol. It blocks nothing and could land any time
after M-rich-diagnostics; it is here rather than earlier by the author's choice,
and M-vscode-extension is what consumes it.

**And the incremental frontend, since 2026-09-03.** The `SCHEDULED.md` item that
M-separate-compilation step 6 left open asked *which milestone does it*, and this
is the answer: a server that re-checks a program on every save cannot wait for
`heroes check` on the compiler's own source — about 8 s on 2026-08-26, after step
9 took it down from 88 — and the per-module build's warm 45.2 s against the fused
44.1 s says the emission half is architectural (panel 093 R4 puts the emitted
text in the cache key). The frontend half is the one an editor feels, so it lands
here; the emission half keeps its numbers in that item as its trigger.

**Re-ordered 2026-10-14, by the author's instruction**: from 74 to 83,
immediately before the gate (**row 84 since 2026-10-18**, one further when
M-readonly-lend entered the chain at row 68, `b5db7e8ff`; **row 85 since
2026-10-26**, one further again when M-standalone-compiler entered at row
82). The packages are written by the assistant through
`heroes check --json`, so the server serves the people who arrive with the
launch. The incremental frontend this page gave it on 2026-09-03 leaves it and
becomes M-latency-ceiling's return condition. The 8 s above was measured on
2026-08-26; `heroes check selfhost/main.hero` read 5.8 s on 2026-09-29.

**A comparison read on 2026-10-25, offered by the session of that day for this
row.** Goose's language server (github.com/aardappel/goose,
[goose/docs/design/language_server.md](https://github.com/aardappel/goose/blob/master/docs/design/language_server.md), [goose/docs/implementation.md](https://github.com/aardappel/goose/blob/master/docs/implementation.md) § 12) suggests five
things: (1) the server is the compiler re-analysing the whole program on every
change, decided only after measuring it, 104 ms in-process on its 3,318-line
self-hosted compiler; here `heroes check selfhost/main.hero` read 5.8 s on
2026-09-29 as a process, so the number to take first is the in-process
re-check; (2) diagnostics are data and the text one rendering of them, which
`check --json` already is; (3) the extension stays usable without the verb,
and a `--probe` tells a compiler with the verb from one without; (4)
cancellation polled at each declaration and statement; (5) the tests as
expectation lines inside the fixtures, `#~ lsp:` here, and a sweep of random
one-character edits with a latency bound, which found two defects there.
