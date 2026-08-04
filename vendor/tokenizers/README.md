# vendor/tokenizers — the instruments `heroes measure` counts with

design.md §1.6 sets a **measured** ceiling on `spec/heroes-spec.md`. These
two BPE tables are how it is measured, offline: no API key, no network, no
dependency. They are vendored rather than fetched so that a count taken
today is reproducible in five years.

| file | vocabulary | sha256 | origin |
|---|---|---|---|
| `claude-legacy.json` | 65k | `58dad83d85e9cd57be209172449ebfbc395df2b455c11fe8b7e5b661e6f462ad` | `@anthropic-ai/tokenizer` (`claude.json`) — Anthropic's own published tokeniser for Claude 1/2 |
| `cl100k_base.tiktoken` | 100k | `223921b76ee99bde995b7ff738513eef100fb51d18c93597a113bcffe865b2a7` | OpenAI `tiktoken`, `cl100k_base` |

## Why two

**One instrument cannot detect its own drift.** Neither table is the
tokeniser of the model that actually reads the spec — that one is not
published. Two tables disagreeing *is* the error bar, and it is small:
measured on frozen spec v0 the spread is 59 tokens (1989 vs 2048, about 3%),
against the order-of-magnitude uncertainty design.md once assumed. The
**maximum binds** and the spread is printed with every run (panel 011).

This also retires an unsourced claim design.md carried from the start —
that `tiktoken` "undercounts Claude tokens 15–20%". Measured here, cl100k
counts 3% *above* Anthropic's own legacy tokeniser on this document.

## Format notes

- `claude-legacy.json` is a JSON object whose `bpe_ranks` field is a single
  space-separated string: a two-token header, then base64 token bytes in
  rank order. The loader scans for the field rather than parsing JSON — the
  workspace has zero dependencies and this keeps it that way.
- `cl100k_base.tiktoken` is one `<base64> <rank>` pair per line.
- Both feed the same merge loop; both use the GPT-2 pre-tokenisation
  pattern, which `claude-legacy.json` states in its own `pat_str` field.

## Port debt

Rule 10 puts `measure` inside the one binary, so at M8c a 65k-rank table
and a byte-level merge loop have to exist **in Heroes** — which needs file
I/O and maps, both already mortgaged closure items (§1.0). Named here so it
is not discovered at the port (panel 011).
