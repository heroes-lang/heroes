# searchd

searchd: a search engine for one website, in the family of a small
Meilisearch or Elasticsearch. It crawls the site from a start URL as a
polite crawler does, asking `robots.txt` and the pages' robots meta tags,
following redirects as links and leaving other sites alone; it reads each
page's text as a reader reads it, folded so that accents, ligatures, case
and soft hyphens never keep a query from a page; it indexes the pages for
full-text search ranked by BM25, and as vectors made from their tokens for
the queries whose words the pages write otherwise; it ranks the pages by
the site's own links with PageRank, folds near-duplicate pages into one
result, corrects misspelt words, completes a query as it is typed, and
serves a web UI and a JSON API.

    heroes build examples-rebel/searchd/main.hero -o searchd
    ./searchd init searchd.toml                 # a settings file to edit
    ./searchd crawl --config searchd.toml       # the site into the crawl store
    ./searchd index --config searchd.toml       # the store into the indexes
    ./searchd serve --config searchd.toml       # the UI and the API, until Ctrl-C
    ./searchd query --server http://127.0.0.1:7700 "cardamom buns"
    ./searchd suggest --server http://127.0.0.1:7700 "pearl s"
    ./searchd demo                              # the self-contained session below

`demo` serves a small baking site on loopback, crawls it, indexes it,
serves searchd on a second port and runs scripted queries through its own
client, printing a transcript that is the same on every run; its folder
under `build/` is removed at its end.

Rebel's packages, and what for: `http` fetches the pages, serves the site
and the API, and is the client; `web` routes the UI and the API, answers
404 and 405, entity tags, health and security headers; `html` reads the
pages and the UI's answers; `html/sanitize` cleans a page down to the text
a reader reads; `url` resolves links and puts them in one form; `search`
is the full-text index and its BM25; `unicode/normalize` folds texts; the
`tokenizer` cuts them into `cl100k_base`'s tokens, from
`tokenizer/tables/cl100k_base`, which the vectors are hashed from;
`vectors` holds them in an HNSW index and fingerprints near-duplicates;
`containers/trie` holds the terms for suggestions and spelling;
`containers/disjoint` gathers near-duplicates; `graphs` gives click depths
and paths; `math/linear` multiplies PageRank's link matrix in blocks;
`i18n` and `i18n/collate` order titles as the site's language orders
them; `hash` hashes the fingerprints' word runs; `toml` reads the
settings; `flags` reads the command line; `json` is the crawl store, the
index's pages and the API; `net`, `io`, `os`, `time`, `fs`, `strings`,
`bytes`, `numbers`, `lists`, `math` and `unicode` do the rest. The demo's
threads are the language's own (`spawn`).

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run examples-rebel/searchd/main.hero -- demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 31 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`containers/disjoint`](https://heroes-lang.org/rebel/containers/disjoint/), [`containers/trie`](https://heroes-lang.org/rebel/containers/trie/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`graphs`](https://heroes-lang.org/rebel/graphs/), [`hash`](https://heroes-lang.org/rebel/hash/), [`html`](https://heroes-lang.org/rebel/html/), [`html/sanitize`](https://heroes-lang.org/rebel/html/sanitize/), [`http`](https://heroes-lang.org/rebel/http/), [`i18n`](https://heroes-lang.org/rebel/i18n/), [`i18n/collate`](https://heroes-lang.org/rebel/i18n/collate/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`lists`](https://heroes-lang.org/rebel/lists/), [`math`](https://heroes-lang.org/rebel/math/), [`math/linear`](https://heroes-lang.org/rebel/math/linear/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`search`](https://heroes-lang.org/rebel/search/), [`strings`](https://heroes-lang.org/rebel/strings/), [`time`](https://heroes-lang.org/rebel/time/), [`tokenizer`](https://heroes-lang.org/rebel/tokenizer/), [`tokenizer/tables/cl100k_base`](https://heroes-lang.org/rebel/tokenizer/tables/cl100k_base/), [`toml`](https://heroes-lang.org/rebel/toml/), [`unicode`](https://heroes-lang.org/rebel/unicode/), [`unicode/normalize`](https://heroes-lang.org/rebel/unicode/normalize/), [`url`](https://heroes-lang.org/rebel/url/), [`vectors`](https://heroes-lang.org/rebel/vectors/), [`web`](https://heroes-lang.org/rebel/web/).

## Its folders

10 folders, 37 files and 6,951 lines, this page aside; of them 34 modules, `.hero` files, of 6,795 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 303 | 1 | 147 |
| `app/` | 3 | 604 | 3 | 604 |
| `client/` | 1 | 313 | 1 | 313 |
| `crawl/` | 5 | 1,507 | 5 | 1,507 |
| `demo/` | 2 | 334 | 2 | 334 |
| `index/` | 9 | 1,648 | 9 | 1,648 |
| `query/` | 5 | 917 | 5 | 917 |
| `server/` | 4 | 607 | 4 | 607 |
| `standins/` | 2 | 513 | 2 | 513 |
| `text/` | 2 | 205 | 2 | 205 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
