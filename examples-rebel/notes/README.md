# notes

notes: a multi-user publishing service, a small blog and notes server
written on Rebel's `web`. People register, sign in, write notes in
Markdown with tags, comment on each other's notes, and search them; the
site's pages and a JSON API with bearer tokens serve the same notes, a
WebSocket tells an author of a new comment as it is written, and an Atom
feed carries the newest notes to a feed reader.

    heroes build examples-rebel/notes/main.hero -o notes
    ./notes init notes.toml               # a settings file with fresh keys
    ./notes serve --config notes.toml     # serves until SIGTERM or Ctrl-C
    ./notes migrate --config notes.toml   # applies the schema's migrations
    ./notes routes                        # prints every route
    ./notes demo                          # the self-contained session below

`demo` starts the server on loopback port 0 with a fresh SQLite database
under `build/notes-demo/` and a fixed clock, drives it with its own client
on a second thread, prints a transcript that is the same on every run,
stops the server through its API and removes its folder.

Rebel's packages, and what for: `web` routes, groups, sessions in signed
cookies, CSRF tokens, rate limits, request ids, recovery, entity tags,
security headers, gzip, static files and health answers; `http` reads and
writes the messages, negotiates types and languages, and is the demo's
client with a cookie jar judged by `http/suffixes`; `template` renders the
pages in its HTML mode; `markdown` writes the notes' and comments' HTML and
`html/sanitize` cleans it; `html` reads the pages in the demo's client;
`json` is every API body; `sql`, `sql/sqlite`, `sql/build` and
`sql/migrate` hold the accounts, notes, tags and comments; `search` ranks
full-text search by BM25; `crypto` hashes passwords with Argon2id; `jose`
signs the API's tokens; `uuid` names comments and tokens; `websocket` is
the live channel and `poll` serves it beside the pages on one thread;
`openapi` describes the API, adds its routes, checks its requests and
serves its document; `xml` writes the Atom feed; `i18n` and `i18n/dates`
write the pages in English or Italian with their dates; `log` writes JSON
lines, `metrics` serves Prometheus's text; `config`, `toml` and `flags`
read the settings in layers; `time`, `net`, `os`, `fs`, `io`, `strings`,
`bytes`, `numbers` and `url` do the rest. The server's loop owns the
state, and its workers and the demo's client run on threads of the
language's own (`spawn`, mailboxes).

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run examples-rebel/notes/main.hero -- demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 36 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`config`](https://heroes-lang.org/rebel/config/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`html`](https://heroes-lang.org/rebel/html/), [`html/sanitize`](https://heroes-lang.org/rebel/html/sanitize/), [`http`](https://heroes-lang.org/rebel/http/), [`http/suffixes`](https://heroes-lang.org/rebel/http/suffixes/), [`i18n`](https://heroes-lang.org/rebel/i18n/), [`i18n/dates`](https://heroes-lang.org/rebel/i18n/dates/), [`io`](https://heroes-lang.org/rebel/io/), [`jose`](https://heroes-lang.org/rebel/jose/), [`json`](https://heroes-lang.org/rebel/json/), [`log`](https://heroes-lang.org/rebel/log/), [`markdown`](https://heroes-lang.org/rebel/markdown/), [`metrics`](https://heroes-lang.org/rebel/metrics/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`openapi`](https://heroes-lang.org/rebel/openapi/), [`os`](https://heroes-lang.org/rebel/os/), [`poll`](https://heroes-lang.org/rebel/poll/), [`search`](https://heroes-lang.org/rebel/search/), [`sql`](https://heroes-lang.org/rebel/sql/), [`sql/build`](https://heroes-lang.org/rebel/sql/build/), [`sql/migrate`](https://heroes-lang.org/rebel/sql/migrate/), [`sql/sqlite`](https://heroes-lang.org/rebel/sql/sqlite/), [`strings`](https://heroes-lang.org/rebel/strings/), [`template`](https://heroes-lang.org/rebel/template/), [`time`](https://heroes-lang.org/rebel/time/), [`toml`](https://heroes-lang.org/rebel/toml/), [`url`](https://heroes-lang.org/rebel/url/), [`uuid`](https://heroes-lang.org/rebel/uuid/), [`web`](https://heroes-lang.org/rebel/web/), [`websocket`](https://heroes-lang.org/rebel/websocket/), [`xml`](https://heroes-lang.org/rebel/xml/).

## Its folders

12 folders, 52 files and 8,886 lines, this page aside; of them 49 modules, `.hero` files, of 8,771 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 234 | 1 | 119 |
| `api/` | 4 | 785 | 4 | 785 |
| `app/` | 3 | 604 | 3 | 604 |
| `auth/` | 3 | 414 | 3 | 414 |
| `config/` | 1 | 394 | 1 | 394 |
| `demo/` | 8 | 1,036 | 8 | 1,036 |
| `domain/` | 7 | 934 | 7 | 934 |
| `http/` | 8 | 2,004 | 8 | 2,004 |
| `ops/` | 1 | 154 | 1 | 154 |
| `realtime/` | 1 | 234 | 1 | 234 |
| `store/` | 6 | 1,121 | 6 | 1,121 |
| `views/` | 6 | 972 | 6 | 972 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
