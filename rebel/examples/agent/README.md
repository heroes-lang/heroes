# agent

agent: an AI assistant for a folder of documents, or a codebase's docs. It
cuts the documents into passages sized in a model's tokens, makes each a
vector and keeps them in an index on disk; it answers a question by giving a
model the passages nearest to it and the tools to look further (search,
read a file, query a JSON file, count tokens, run a command the user
allowed); and it serves the same tools, documents and a prompt to other
agents over the Model Context Protocol. It speaks Claude's Messages API and
OpenAI's Chat Completions, which Ollama, vLLM and llama.cpp's server speak
too, and falls back from one provider to the other.

Run for real, with a key in the environment (`ANTHROPIC_API_KEY`,
`OPENAI_API_KEY`), or typed at the prompt where none is set:

    heroes build examples-rebel/agent/main.hero -o agent
    ./agent index ./docs --store .agent
    ./agent search --store .agent "install on macOS"
    ./agent ask --store .agent --provider anthropic --fallback openai "How do I install it?"
    ./agent ask --store .agent --provider openai --base-url http://localhost:11434/v1 --model llama3.2 "..."
    ./agent ask --store .agent --allow git --allow make "Does the build pass?"
    ./agent chat --store .agent
    ./agent serve --store .agent          (an MCP server, for an editor or another agent)
    ./agent tokens --model gpt-4o README.md

`./agent demo --work build/agent-demo` runs the whole session against
loopback stand-ins of both APIs the program carries, and spends nothing.

The Rebel packages it uses, and for what:

- `mcp`: the server of `agent serve`, and the client that drives a server
  started as a child (`mcp/server`, `mcp/spawned`).
- `jsonrpc`: the messages that server writes, one a line.
- `llm/anthropic`, `llm/openai`: the two providers' APIs, their tool loops,
  Claude's token count and OpenAI's embeddings (`claude`, `gpt`).
- `tokenizer`, `tokenizer/tables`: a model's tokens counted, by its own
  encoding where OpenAI publishes it (`encodings`).
- `chunks`: documents cut at their headings into passages of so many tokens.
- `vectors`: the HNSW index of the passages, saved as bytes (`store`).
- `threads`: the documents cut on worker threads, and each stand-in served on
  a thread of its own.
- `process`: the commands `run_command` runs, and the MCP server started as
  a child.
- `os`: the program's own path, which the demo serves and allows.
- `retries`: the waits of a request sent again, and each provider's circuit
  breaker (`attempts`, `router`).
- `http`: the stand-ins' side of the wire; the clients' side is the `llm`
  packages'.
- `json`, `json/jq`: every store file and request; the `query_json` tool and
  the stand-ins' checks of a request's body.
- `term`: colour on a terminal, the chat's line editor, a key typed unseen,
  the tables of a listing.
- `flags`: this command line.
- and `bytes`, `fs`, `hash` (the local embedder's features), `io`, `net`,
  `numbers`, `path`, `strings`, `time`.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run examples-rebel/agent/main.hero -- demo --work build/agent/demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 26 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`chunks`](https://heroes-lang.org/rebel/chunks/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`hash`](https://heroes-lang.org/rebel/hash/), [`http`](https://heroes-lang.org/rebel/http/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`json/jq`](https://heroes-lang.org/rebel/json/jq/), [`jsonrpc`](https://heroes-lang.org/rebel/jsonrpc/), [`llm/anthropic`](https://heroes-lang.org/rebel/llm/anthropic/), [`llm/openai`](https://heroes-lang.org/rebel/llm/openai/), [`mcp`](https://heroes-lang.org/rebel/mcp/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`process`](https://heroes-lang.org/rebel/process/), [`retries`](https://heroes-lang.org/rebel/retries/), [`strings`](https://heroes-lang.org/rebel/strings/), [`term`](https://heroes-lang.org/rebel/term/), [`threads`](https://heroes-lang.org/rebel/threads/), [`time`](https://heroes-lang.org/rebel/time/), [`tokenizer`](https://heroes-lang.org/rebel/tokenizer/), [`tokenizer/tables`](https://heroes-lang.org/rebel/tokenizer/tables/), [`vectors`](https://heroes-lang.org/rebel/vectors/).

## Its folders

10 folders, 45 files and 6,058 lines, this page aside; of them 42 modules, `.hero` files, of 5,876 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 295 | 1 | 113 |
| `app/` | 4 | 601 | 4 | 601 |
| `demo/` | 4 | 714 | 4 | 714 |
| `index/` | 7 | 1,106 | 7 | 1,106 |
| `llm/` | 5 | 615 | 5 | 615 |
| `mcp/` | 2 | 262 | 2 | 262 |
| `retrieval/` | 2 | 106 | 2 | 106 |
| `standins/` | 8 | 1,430 | 8 | 1,430 |
| `support/` | 3 | 342 | 3 | 342 |
| `tools/` | 6 | 587 | 6 | 587 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
