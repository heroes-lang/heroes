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
