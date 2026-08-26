# vendor/tokenizers — the instruments `heroes measure` counts with

design.md §1.6 sets a **measured** ceiling on `spec/heroes-spec.md`. These
two BPE tables are how it is measured, offline: no API key, no network, no
dependency. They are vendored rather than fetched so that a count taken
today is reproducible in five years.

| file | vocabulary | sha256 | origin |
|---|---|---|---|
| `claude-legacy.json` | 65k | `58dad83d85e9cd57be209172449ebfbc395df2b455c11fe8b7e5b661e6f462ad` | `@anthropic-ai/tokenizer` (`claude.json`) — Anthropic's own published tokeniser for Claude 1/2 |
| `cl100k_base.tiktoken` | 100k | `223921b76ee99bde995b7ff738513eef100fb51d18c93597a113bcffe865b2a7` | OpenAI `tiktoken`, `cl100k_base` |

## Licensing — these are redistributed, so they are attributed

Both tables ship inside the `heroes` binary (`measure` is in the one command,
CLAUDE.md §10), which makes them part of what this project distributes rather
than a development convenience. Their upstream terms are **MIT** in both cases —
OpenAI's `tiktoken` for `cl100k_base.tiktoken`, and `@anthropic-ai/tokenizer`
for `claude-legacy.json` — and both are named in the repository's `NOTICE` with
the origins recorded in the table above. A formal re-check of both licence texts
against their upstream repositories is a line item before anything is published
(ROADMAP M18); until then, treat this paragraph as the project's stated
understanding rather than as a verified audit.

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

## Two is the answer, and the third one is not coming

**Author decision 2026-08-26** (`/decide`, answer `8c`), closing a question panel
011 left open and `DECIDE.md` carried for weeks: o200k is **not** vendored, and
this file is where that is written down rather than left as an absence somebody
re-discovers.

The open item's own trigger was a condition, not a date — *vendor it before any
verdict lands within 10 tokens of a ceiling* — and the condition is measured
today at **headroom 504 with a spread of 76**. Nothing is near the ceiling, and
the reason to add a third table was always that it might be the tightest: on
frozen spec v0 o200k was tighter than both of these by **2 tokens**, which is
inside the spread the pair already prints.

**What the decision costs, stated so it can be overturned**: the binding number
is the maximum of two tables, so a third and tighter one would raise it, and a
verdict taken at 10 tokens of headroom could in principle be taken against the
wrong maximum. That is the exposure, it is bounded by the printed spread, and it
is accepted. If the headroom ever falls below the spread, this paragraph is what
should be re-read — the item is closed, not forgotten.

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
