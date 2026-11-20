# Rebel: how it is made, and how its code is written

**Rebel is the non-standard standard library of Heroes.** Heroes imposes no
standard library and ships none with the compiler; it offers one, Rebel, that a
program may take or leave. A program takes it with `heroes add rebel`, lists each
package it uses with `heroes add rebel/strings`, and names it
`use <rebel>/strings`; its catalogue's entry places the packages it lists, with
each one they use, and nothing else of Rebel. A program that takes nothing has
no catalogue at all. The compiler itself is written without it.

Rebel is written from scratch in idiomatic Heroes. Its tree, its names and its
shapes are its own, taken from no other library; Go's, Rust's and Python's are
its inspirations and its yardstick, since a package is complete when every
capability those three offer in its area is offered here or refused in writing.

This page is in two parts. **Part A** says how Rebel is made, **Part B** how its
code is written. Everyone who writes a line of Rebel reads both first.

---

# Part A. How Rebel is made

## A1. The packages

A hundred and forty-five packages, in layers. **A package may use any package of a layer below
its own**; the column "may use" names the ones it is expected to need, not a limit. There is no cycle.

| layer | package | what it holds | may use | C under it |
|---|---|---|---|---|
| 0 | `unicode` | what a character is: classes, case, the tables, UTF-8 read and written |  |  |
| 0 | `math` | the functions of mathematics, complex numbers by C11 Annex G, and the integer helpers that cannot overflow in silence |  | `math.h` |
| 0 | `lists` | what the built-ins do not do for a `[T]`: a stable sort by an order, search, slices of a list; pairs of two lists or of an index and an element refused, the language having no generic record |  |  |
| 0 | `maps` | what the built-ins do not do for a `{K: V}`: values, merging, removing; its entries as one list of pairs refused, the language having no generic record |  |  |
| 0 | `numbers` | a number read from text and written as text, in every base and precision |  |  |
| 0 | `path` | a path as text, by POSIX's rules and by Windows', touching no disk |  |  |
| 0 | `os` | the process's world: environment, working folder, who and where it runs |  | `hero_sys.h`, `hero_os.h` |
| 0 | `hash` | checksums and fast non-cryptographic hashes: CRC-32, Adler-32, FNV |  |  |
| 0 | `sets` | set operations over a `{K: bool}`, since the language has no set type |  |  |
| 0 | `errors` | context added to a failure without losing its code, several failures joined, a failure told to a person |  |  |
| 0 | `threads` | a function run on another thread and joined: the language's own doors, made safe to call |  | `hero_os.h` |
| 0 | `containers/ordered` | an ordered map and set: ranks, ranges, the nearest keys |  |  |
| 0 | `containers/deque` | a double-ended queue, bounded or not |  |  |
| 0 | `containers/bits` | a set of bits of any length, combined and searched |  |  |
| 0 | `containers/cache` | a cache that forgets the entry used least recently, with a time to live the caller gives |  |  |
| 0 | `containers/indexed` | a map that keeps the order its keys were first put in and answers by position |  |  |
| 0 | `containers/heap` | a heap over the program's own list: a priority queue |  |  |
| 0 | `graphs` | the orders, cycles, components, reachability and shortest paths of a graph |  |  |
| 0 | `qr` | QR codes made, every version and correction level, written as modules, SVG or text for a terminal |  |  |
| 0 | `containers/trie` | a prefix tree of texts to values: prefixes, the longest prefix of a text |  |  |
| 0 | `containers/disjoint` | union-find: sets merged and asked whether two values are in one |  |  |
| 1 | `unicode/normalize` | NFC, NFD, NFKC, NFKD, case folding for comparison; its tables apart, so only its users carry them | `unicode` |  |
| 1 | `unicode/segment` | the boundaries of graphemes, words and sentences (UAX #29), the width of a text on a screen (UAX #11) | `unicode` |  |
| 1 | `unicode/names` | the name of a character and the character of a name, scripts, blocks | `unicode` |  |
| 1 | `strings` | text searched, cut, trimmed, replaced, padded, cased | `unicode` |  |
| 1 | `bytes` | the same for `[u8]`, and bytes as hexadecimal and base 64 | `unicode` |  |
| 1 | `regex` | a regular expression matched in linear time | `unicode` |  |
| 1 | `random` | a seeded generator, ranges, shuffling, choosing, sampling; secure values from `os` | `math`, `os` |  |
| 1 | `stats` | mean, median, mode, variance, deviation, quantiles, correlation | `lists`, `math` |  |
| 1 | `math/linear` | vectors of two, three and four, matrices, quaternions, transforms, cameras and Euler angles in all twelve orders | `math` |  |
| 1 | `url/idna` | internationalized domain names (UTS 46): to ASCII and back, Punycode; its tables apart, so only its users carry them | `unicode` |  |
| 1 | `image` | images in memory, gray to RGBA at 8 and 16 bits: crop, resize, rotate, flip, convert, paste with alpha, simple drawing, a histogram; each file format a subpackage; a broken file is `invalid`, never a memory fault | `math` |  |
| 2 | `io` | the stream: read, write, close, seek; buffers; lines; copying | `bytes` | `hero_stream.h` |
| 2 | `json` | a JSON value parsed and rendered | `numbers`, `strings`, `unicode` |  |
| 2 | `xml` | an XML document parsed, walked, built and written; names with their namespaces; escaping | `numbers`, `strings`, `unicode` |  |
| 2 | `html` | HTML read as a browser reads it (WHATWG), never failing; searched by tag, attribute, class and CSS selectors; changed and written; every named character reference | `strings`, `unicode` |  |
| 2 | `url` | a URL parsed, built, escaped | `numbers`, `strings` |  |
| 2 | `crypto` | SHA-1, SHA-2, HMAC, HKDF, PBKDF2, comparison in constant time | `bytes` |  |
| 2 | `big` | integers and decimals of any size, exact | `numbers`, `random` |  |
| 2 | `flags` | the arguments of a program: options, values, positionals, a generated help | `numbers`, `strings` |  |
| 2 | `ini` | INI files read and written | `strings` |  |
| 2 | `semver` | versions (SemVer 2.0.0) compared, and ranges as Cargo reads them | `strings` |  |
| 2 | `diff` | the differences of two lists or texts, as values and as unified output, and their similarity | `lists`, `strings`, `unicode` |  |
| 2 | `pem` | PEM blocks read and written, their labels and headers | `bytes`, `strings` |  |
| 2 | `unicode/encodings` | the WHATWG Encoding standard's legacy encodings read and written: Windows-1252, ISO-8859, Shift_JIS, EUC-JP, GB18030, Big5, EUC-KR; its tables apart | `bytes`, `unicode` |  |
| 2 | `keyring` | secrets kept in the system's own keyring | `bytes` | `hero_keyring.h` |
| 2 | `scanner` | a configurable lexer: identifiers, numbers, strings, comments, each token with its place | `numbers`, `regex`, `strings`, `unicode` |  |
| 2 | `property` | property-based testing: generated cases, a seed that reproduces, the failing case shrunk | `math`, `random`, `unicode` |  |
| 2 | `vectors` | vectors of f32 compared (dot, cosine, distance) and the nearest found, exactly or by an HNSW index | `math`, `numbers`, `random` |  |
| 2 | `tokenizer` | byte-pair encoding as tiktoken does it: encode, decode, count; its tables in subpackages, one per encoding, so a program carries only the one it uses | `bytes`, `regex`, `unicode` |  |
| 2 | `search` | an inverted index in memory, ranked by BM25, with phrase and prefix queries | `math`, `numbers`, `strings`, `unicode` |  |
| 2 | `http/suffixes` | the Public Suffix List, so cookies for a public suffix are refused and a site's registrable domain is known; its data apart | `strings` |  |
| 2 | `image/jpeg` | JPEG read (baseline and progressive, every sampling, restart intervals, EXIF orientation) and baseline JPEG written at a quality | `bytes`, `image`, `math` |  |
| 2 | `image/gif` | GIF read and written: every frame of an animation, laid one over another as a viewer shows it | `bytes`, `image` |  |
| 2 | `image/bmp` | BMP read and written: 1 to 32 bits a pixel under every header, run-length coded files read | `bytes`, `image` |  |
| 2 | `image/netpbm` | PBM, PGM, PPM and PAM read and written, plain or raw | `bytes`, `image` |  |
| 2 | `i18n` | locales, CLDR's plural rules and message catalogs (gettext); numbers, dates, ordering and locale data in subpackages, from CLDR 49 | `bytes`, `numbers`, `os`, `strings` |  |
| 3 | `time` | an instant, a duration, the calendar, time zones (the system's files, or the IANA data Rebel carries), text of a time, sleeping | `bytes`, `io`, `numbers`, `os`, `strings` | `hero_time.h`, `hero_stream.h`, `hero_disk.h` |
| 3 | `net` | TCP, UDP, names, addresses | `io`, `numbers`, `strings` | `hero_net.h`, `hero_stream_net.h` |
| 3 | `yaml` | a YAML document parsed into values and written back | `json`, `numbers`, `strings`, `unicode` |  |
| 3 | `cbor` | CBOR (RFC 8949) read into a value of its own, which keeps bytes and tags, and crossed to and from a `json.Value` | `big`, `bytes`, `json`, `lists` |  |
| 3 | `term` | colours and styles that fall back to plain text, the cursor, the screen, a key and a secret read without echo, a progress bar | `bytes`, `io`, `os`, `unicode` | `hero_term.h` |
| 3 | `csv` | comma-separated values read and written by RFC 4180 | `io`, `strings` |  |
| 3 | `compress` | DEFLATE, gzip and zlib, written and read | `bytes`, `hash`, `io` |  |
| 3 | `template` | text and HTML made from a template and values, escaped by context | `json`, `strings`, `unicode` |  |
| 3 | `markdown` | CommonMark and GFM's tables read into a tree and written as HTML | `html`, `numbers`, `strings`, `unicode` |  |
| 3 | `html/sanitize` | HTML from outside cleaned by an allow-list policy | `html`, `regex`, `strings`, `unicode`, `url` |  |
| 3 | `json/schema` | JSON Schema 2020-12 (and draft 7) validation, every error with its place | `bytes`, `json`, `numbers`, `regex`, `strings`, `unicode`, `url` |  |
| 3 | `json/patch` | JSON Patch (RFC 6902) and JSON Merge Patch (RFC 7386) made and applied | `json`, `strings` |  |
| 3 | `json/path` | JSONPath queries (RFC 9535) | `json`, `numbers`, `regex`, `strings`, `unicode` |  |
| 3 | `json/jq` | the jq language run over a `json.Value`, so over YAML, TOML, CBOR and MessagePack read into one | `bytes`, `json`, `math`, `numbers`, `regex`, `strings`, `unicode` |  |
| 3 | `serial` | serial ports listed, opened, set and read and written as a stream | `io` | `hero_serial.h` |
| 3 | `i18n/numbers` | numbers written and read as a language writes them: separators, percentages, money with its currency, compact forms | `i18n`, `strings`, `unicode` |  |
| 3 | `i18n/collate` | texts ordered as a language orders them, by the Unicode Collation Algorithm over CLDR's root and tailorings | `i18n`, `strings`, `unicode`, `unicode/normalize` |  |
| 3 | `i18n/locales` | what CLDR knows of a language tag: its likely script and region, and the tags old names are replaced by | `i18n`, `strings` |  |
| 3 | `crypto/seal` | authenticated encryption by the system's own cryptography | `bytes`, `crypto` | `hero_crypto.h` |
| 3 | `crypto/keypairs` | key pairs, signatures and key agreement by the system's own cryptography | `bytes`, `crypto` | `hero_crypto.h` |
| 3 | `tokenizer/tables` | the encodings of OpenAI's models carried as data, by name, and which encoding a model reads; each table a module of its own, `tokenizer/tables/o200k_base`, `cl100k_base` and `o200k_harmony`, so a program carries the one it names | `tokenizer` |  |
| 4 | `fs` | files and directories by path: whole reads and writes, metadata, links, trees, temporaries | `bytes`, `io`, `os`, `path`, `time` | `hero_disk.h`, `hero_os.h` |
| 4 | `msgpack` | MessagePack read into a value of its own, which keeps bytes and extensions, and crossed to and from a `json.Value` | `bytes`, `json`, `lists`, `time` |  |
| 4 | `poll` | waiting on many streams, sockets and timers at once, and being told which are ready | `io`, `net`, `time` | `hero_poll.h` |
| 4 | `tls` | TLS client and server as an `io.Stream`, certificates checked by the system's own store: SChannel on Windows, Apple's Security framework on macOS, OpenSSL on Linux | `bytes`, `io`, `net`, `numbers`, `os`, `strings`, `threads`, `time` | `hero_tls.h` |
| 4 | `log` | lines with a time and a level, to a stream | `io`, `json`, `strings`, `time` |  |
| 4 | `uuid` | identifiers of versions 4 and 7, parsed and written | `bytes`, `random`, `time` |  |
| 4 | `toml` | TOML documents read and written, and converted to and from `json` | `json`, `numbers`, `strings`, `time`, `unicode` |  |
| 4 | `sql` | what does not depend on the database: values, parameters, rows, results, codes | `bytes`, `time` |  |
| 4 | `protobuf` | Protocol Buffers: the wire format, `.proto` files read at run time, dynamic messages bound to records, the JSON mapping, the well-known types | `bytes`, `json`, `maps`, `math`, `numbers`, `strings`, `time` |  |
| 4 | `mail` | messages (RFC 5322) and MIME read and written: addresses, encoded words, parts and attachments | `bytes`, `hash`, `io`, `numbers`, `os`, `strings`, `time` |  |
| 4 | `asn1` | ASN.1 values read and written in DER and BER, every universal type, tagged and constructed | `big`, `bytes`, `lists`, `math`, `numbers`, `time`, `unicode` |  |
| 4 | `i18n/dates` | dates and times written as a language writes them: month and day names, its patterns, relative times | `i18n`, `numbers`, `strings`, `time`, `unicode` |  |
| 4 | `dns` | a DNS client: every record kind asked and answered over UDP and TCP | `bytes`, `io`, `net`, `os`, `strings`, `threads` |  |
| 4 | `bench` | a function measured: warm-up, rounds, median and spread, two versions compared | `math`, `numbers`, `os`, `stats`, `time` |  |
| 4 | `chunks` | text cut into pieces for retrieval, by characters, sentences, markdown headings or tokens, with overlap | `markdown`, `tokenizer`, `unicode`, `unicode/segment` |  |
| 4 | `bson` | BSON read and written exactly, every type, and Extended JSON | `big`, `bytes`, `json`, `math`, `numbers`, `os`, `time` |  |
| 4 | `tui` | full-screen terminal programs in immediate mode: widgets, layout by constraints, only the changed cells written | `io`, `os`, `term`, `unicode`, `unicode/segment` |  |
| 4 | `webauthn` | a WebAuthn relying party: registration and authentication ceremonies, attestation and assertion verified | `bytes`, `cbor`, `crypto`, `crypto/keypairs`, `json`, `os` |  |
| 4 | `image/png` | PNG read and written: every colour type and bit depth, interlacing read, the filter and the compression chosen, chunks kept or dropped | `bytes`, `compress`, `hash`, `image` |  |
| 4 | `jobs` | background jobs and their workers, retried and scheduled, over a store in a subpackage: the values both stores share | `json`, `numbers`, `os`, `time` |  |
| 4 | `compress/zstd` | Zstandard (RFC 8878) read, and written at a basic level | `bytes`, `compress`, `hash` |  |
| 4 | `compress/brotli` | Brotli (RFC 7932) read, and written at a basic level | `bytes`, `compress` |  |
| 4 | `compress/bzip2` | bzip2 read and written | `bytes`, `compress` |  |
| 4 | `process` | another program run, fed, read and waited for | `io`, `os`, `path`, `strings`, `time`, `unicode` | `hero_proc.h` |
| 4 | `compress/lz4` | LZ4 frames and blocks, compressed and decompressed | `bytes`, `compress`, `hash`, `io` |  |
| 4 | `compress/xz` | xz and LZMA2 read, and written at a basic level | `bytes`, `compress`, `crypto`, `hash` |  |
| 5 | `jose` | JSON Web Tokens, signatures (JWS), keys (JWK) and encryption (JWE) where the system's cryptography allows | `asn1`, `big`, `bytes`, `crypto`, `crypto/keypairs`, `crypto/seal`, `json`, `os`, `pem`, `time` |  |
| 5 | `archive` | tar and zip, written and read | `bytes`, `compress`, `fs`, `hash`, `io`, `path`, `time` |  |
| 5 | `sql/postgres` | PostgreSQL over its own protocol, written in Heroes; the API every driver keeps | `bytes`, `crypto`, `io`, `net`, `numbers`, `os`, `sql`, `strings`, `threads`, `time`, `tls` |  |
| 5 | `sql/sqlite` | SQLite over its library, the same API name for name; only its users need the library | `sql`, `strings`, `time` | `sqlite3.h` |
| 5 | `sql/mysql` | MySQL and MariaDB over their own protocol, written in Heroes; the same API as the other two drivers | `bytes`, `crypto`, `io`, `net`, `numbers`, `os`, `sql`, `strings`, `time`, `tls`, `url` |  |
| 5 | `redis` | a Redis client over its protocol (RESP2 and RESP3): commands, pipelines, publish and subscribe | `bytes`, `io`, `net`, `numbers`, `os`, `strings`, `time`, `tls`, `url` |  |
| 5 | `sql/build` | queries as values composed by functions, placeholders for PostgreSQL and SQLite, values only as parameters, a row read into a record by its field names; the ORM itself refused, with the reason | `sql`, `strings` |  |
| 5 | `http2` | HTTP/2 and HPACK, client and server, beneath `http` and knowing nothing of it | `bytes`, `io`, `net`, `numbers`, `threads`, `time`, `tls` |  |
| 5 | `mail/smtp` | mail sent: SMTP with STARTTLS or TLS, authentication, the extensions | `bytes`, `crypto`, `io`, `mail`, `net`, `numbers`, `strings`, `threads`, `time`, `tls` |  |
| 5 | `x509` | certificates, requests and revocation lists read, their names, extensions and keys; a chain put in order | `asn1`, `big`, `bytes`, `crypto/keypairs`, `net`, `pem`, `strings`, `time` |  |
| 5 | `fs/watch` | a file or a folder watched, and told what changed | `fs`, `io`, `net`, `path`, `poll`, `time` | `hero_watch.h` |
| 5 | `sql/odbc` | any database the system's ODBC manager reaches, through the one `sql` API | `bytes`, `os`, `sql`, `time` | `hero_odbc.h` |
| 5 | `ldap` | an LDAP client: bind, search, change, over TLS | `asn1`, `bytes`, `io`, `net`, `numbers`, `os`, `strings`, `threads`, `time`, `tls`, `url` |  |
| 5 | `config` | settings read in layers (files, the environment, flags, `.env` files) with a precedence, read by path | `flags`, `ini`, `json`, `numbers`, `os`, `path`, `strings`, `time`, `toml`, `yaml` |  |
| 5 | `amqp` | an AMQP 0-9-1 client (RabbitMQ): channels, exchanges, queues, publish and consume, acknowledgements | `bytes`, `io`, `net`, `numbers`, `os`, `poll`, `strings`, `time`, `tls`, `url` |  |
| 5 | `kafka` | a Kafka client: produce, fetch, offsets and consumer groups over the wire protocol | `bytes`, `compress`, `crypto`, `hash`, `io`, `net`, `numbers`, `os`, `strings`, `threads`, `time`, `tls`, `url` |  |
| 5 | `mongodb` | a MongoDB client over its wire protocol: SCRAM, CRUD, cursors, aggregation, transactions | `bson`, `bytes`, `crypto`, `dns`, `hash`, `io`, `net`, `numbers`, `os`, `strings`, `tls`, `unicode`, `unicode/normalize`, `url`, `uuid` |  |
| 5 | `log/files` | logs written to files that rotate by size or by day, the old ones kept, counted and compressed | `compress`, `fs`, `io`, `log`, `path`, `strings`, `time` |  |
| 5 | `pdf` | PDF documents written (text, standard fonts, images, simple tables) and their text read | `bytes`, `compress`, `crypto`, `image`, `image/png`, `math`, `time`, `unicode` |  |
| 5 | `ipc` | communication between processes of one machine: a local channel by name, shared memory, a lock and a semaphore between processes | `bytes`, `io`, `os`, `process`, `threads` | `hero_ipc.h` |
| 6 | `http` | HTTP/1.1, client and server, over `net` | `bytes`, `compress`, `http2`, `io`, `json`, `net`, `numbers`, `os`, `strings`, `threads`, `time`, `tls`, `url` |  |
| 6 | `sql/migrate` | numbered migrations applied in transactions, the versions applied kept in a table, the step back | `bytes`, `crypto`, `fs`, `numbers`, `path`, `sql`, `sql/build`, `strings`, `time` |  |
| 6 | `grpc` | gRPC calls of the four kinds, client and server, over `http2` and `protobuf` | `bytes`, `compress`, `http2`, `io`, `net`, `numbers`, `protobuf`, `threads`, `time`, `tls` |  |
| 6 | `xlsx` | spreadsheets of Office Open XML read and written: sheets, cells, types, styles | `archive`, `bytes`, `fs`, `io`, `lists`, `math`, `numbers`, `strings`, `time`, `unicode`, `xml` |  |
| 6 | `jobs/postgres_store` | jobs kept in PostgreSQL: claimed with FOR UPDATE SKIP LOCKED, enqueued in the program's own transaction | `jobs`, `json`, `os`, `sql`, `sql/postgres`, `strings`, `time` |  |
| 6 | `jobs/redis_store` | jobs kept in Redis: every change a Lua script the server runs whole | `bytes`, `jobs`, `json`, `numbers`, `os`, `redis`, `strings`, `time` |  |
| 7 | `web` | a router in the manner of Echo: routes with methods and parameters, groups, middleware as functions over a request and a response, the right 404 and 405, responses for JSON, text, redirects and static files; the program calls its own handler with its own state | `bytes`, `compress`, `crypto`, `crypto/seal`, `fs`, `http`, `io`, `json`, `log`, `net`, `numbers`, `os`, `path`, `strings`, `template`, `time`, `tls`, `url`, `uuid` |  |
| 7 | `websocket` | WebSocket (RFC 6455) client and server, compressed or not, many connections on one thread | `bytes`, `compress`, `crypto`, `http`, `io`, `json`, `net`, `os`, `poll`, `strings`, `threads`, `time`, `tls`, `unicode`, `url` |  |
| 7 | `oauth2` | an OAuth 2.0 client (authorization code with PKCE, client credentials, device flow, refresh) and OpenID Connect | `bytes`, `crypto`, `http`, `io`, `jose`, `json`, `net`, `random`, `threads`, `time`, `url` |  |
| 7 | `metrics` | counters, gauges and histograms, written in Prometheus's text format and served | `bytes`, `compress`, `http`, `lists`, `math`, `net`, `numbers`, `os`, `protobuf`, `random`, `strings`, `time`, `url` |  |
| 7 | `s3` | the S3 API with Signature Version 4: objects, buckets, multipart uploads, presigned URLs | `bytes`, `crypto`, `hash`, `http`, `ini`, `io`, `json`, `lists`, `net`, `numbers`, `os`, `strings`, `time`, `tls`, `url`, `xml` |  |
| 7 | `jsonrpc` | JSON-RPC 2.0 requests, answers and notifications over a stream or HTTP | `bytes`, `containers/deque`, `http`, `io`, `json`, `net`, `numbers`, `strings`, `threads` |  |
| 7 | `tracing` | spans with their context propagated in HTTP requests, exported by OpenTelemetry | `bytes`, `compress`, `http`, `lists`, `log`, `numbers`, `os`, `protobuf`, `random`, `strings`, `time`, `url` |  |
| 7 | `acme` | an ACME client (RFC 8555): account, order, HTTP-01 and DNS-01 challenges, the certificate downloaded; the certificate request made elsewhere until `x509` writes one | `big`, `bytes`, `crypto`, `http`, `jose`, `json`, `os`, `pem`, `time`, `x509` |  |
| 7 | `llm/anthropic` | Claude's Messages API: messages, streaming, tools driven by the program, token counting, batches | `bytes`, `http`, `io`, `json`, `net`, `numbers`, `os`, `random`, `strings`, `threads`, `time`, `tls`, `url` |  |
| 7 | `llm/openai` | Chat Completions and Responses, which Ollama and vLLM also speak: streaming, tools, embeddings | `bytes`, `http`, `io`, `json`, `net`, `numbers`, `os`, `random`, `strings`, `threads`, `time`, `tls`, `url` |  |
| 7 | `graphql` | GraphQL documents and schemas parsed, validated and executed, the program resolving each field | `bytes`, `http`, `json`, `numbers`, `strings`, `unicode` |  |
| 8 | `nats` | a NATS client: publish, subscribe, request and reply, over TLS | `bytes`, `crypto/keypairs`, `io`, `json`, `maps`, `net`, `os`, `poll`, `random`, `strings`, `threads`, `time`, `tls`, `url`, `websocket` |  |
| 8 | `mqtt` | an MQTT 3.1.1 and 5 client, every quality of service | `bytes`, `io`, `maps`, `net`, `numbers`, `os`, `poll`, `random`, `strings`, `time`, `tls`, `url`, `websocket` |  |
| 8 | `mcp` | the Model Context Protocol, server and client, over standard streams and HTTP | `bytes`, `containers/deque`, `http`, `io`, `json`, `json/schema`, `jsonrpc`, `net`, `numbers`, `oauth2`, `strings`, `threads`, `time`, `url`, `web` |  |
| 8 | `openapi` | an OpenAPI 3.1 document built as data, written, served, and requests validated against it | `bytes`, `http`, `json`, `json/schema`, `numbers`, `strings`, `url`, `web`, `yaml` |  |

**The types other packages name**, each owned by one package and named
qualified everywhere else, never redefined:

| owner | types |
|---|---|
| `io` | `io.Stream` (the one handle), `io.Reader` (a buffered reader over a stream) |
| `time` | `time.Instant`, `time.Duration` |
| `fs` | `fs.Info`, `fs.Entry`, `fs.Kind` |
| `process` | `process.Command`, `process.Child`, `process.Status` |
| `net` | `net.Conn`, `net.Listener`, `net.Address` |
| `url` | `url.Url` |
| `json` | `json.Value` |
| `regex` | `regex.Regex`, `regex.Match` |

## A2. Where a package's files stand

- **The face**, `packages/rebel/0.1.0/files/<package>.hero`: every name a
  program writes is declared in it, and nothing else is. `##` headings divide
  it into topics.
- **Private modules**, `<package>/internal/<topic>.hero`: no module outside the
  package may `use` one (the compiler refuses it). A file stays short and about
  one thing, around three hundred lines, and opens with a comment saying its
  role.
- **A floor**, `<package>/hero_<x>.hero`, where the package stands on C: the
  thin binding of one header, one Heroes function per C function. No policy
  in it; the face turns C's answers into `T?`.
- **The ledger**, `<package>.ledger` (A8).
- **The driver**, `examples-rebel/reach_<package>.hero`: a program whose `main`
  names something of the package, so that its tests can be run.
- **A large table or a large capability goes in a subpackage**
  (`unicode/normalize`, `compress/zstd`, `sql/sqlite`), so only the programs
  that use it carry it: every module a program reaches is compiled into it, and
  the linker keeps what it holds (measured on 2026-11-16: the zone data in
  `time` added about 545 KB to every program that used `time` at all).

Inside Rebel a `use` starts at the library's own root: `use strings`,
`use strings/internal/scan`, `use io`. A program outside writes
`use <rebel>/strings`. A call into another module is always qualified:
`strings.trim(s)`.

## A3. Names

- **Packages, functions, parameters**: lower case, `snake_case`. **Records and
  variants**: `PascalCase`, read qualified, `fs.Info`. **Constants**:
  `SCREAMING_CASE`.
- **A function is named for what it does or answers, in the plainest verb**:
  `trim`, `split`, `replace`, `open`, `remove`. One grammar for the whole
  library says how a name is built around that verb:

| shape | means | examples |
|---|---|---|
| `is_x`, `has_x` | a question, answers `bool` | `is_abs`, `has_prefix` |
| `x_of` | where or which, of something | `index_of`, `count_of` |
| `x_where` | the same, by a function that accepts | `index_where`, `trim_where` |
| `x_by` | ordered or keyed by a function | `sort_by`, `max_by` |
| `x_all`, `x_n`, `x_last` | every one, at most n, from the end | `replace_all`, `split_n`, `index_of_last` |
| `to_x` | the same value in another form | `to_upper`, `to_slash` |
| `with_x` | a copy with one thing changed | `with_ext`, `with_name` |
| `parse_x`, `format_x` | text to a value, a value to text | `parse_int`, `format_float` |
| `read_x`, `write_x` | through a stream or a path | `read_line`, `write_all` |

- **One name, one meaning, in every package.** `index_of` answers a byte index
  as `i64?` wherever it appears; `contains` answers `bool`; `len` is the
  built-in's and no function is named so.
- **No package, module, function, parameter or binding takes a built-in's
  name** (`find`, `join`, `repeat`, `sort`, `map`, `filter`, `keys`, `len`,
  `print`, `range`, `push`, `slice`, `chars`, `fold`, `any`, `all`):
  `builtin_name_taken`, "the name is taken everywhere" (run 2026-11-16 on a
  parameter and a binding; a record's FIELD may take one, `r.keys`). Nor a
  keyword (`match`, `in`, `use`, `test`: `test = 1` does not even parse), nor
  a word another language uses as a keyword: `int`, `struct`, `enum`, `union`, `class`, `fn`, `func`, `def`, `let`, `var`, `const`, `elif`, `switch`, `case`, `null`, `try`, `import` (`selfhost/foreign_words.hero`). The compiler refuses it.
- **A parameter is named for the call that reads it**, the subject first: a
  call names every argument that shares its type with another, so the names
  are part of the surface: `strings.replace_all(s: line, old: "a", new: "b")`.
- **No abbreviation a reader must decode**, but the ones every programmer
  reads: `dir`, `env`, `ext`, `abs`, `len`, `max`, `min`.

## A4. Failure

- **A function that can fail on what it is given answers `T?`, and nothing else
  says so**: no `-1`, no empty value meaning absent, no `bool` beside a value.
- **Input from outside the program never aborts.** A package aborts only where
  its caller broke a precondition the caller could have checked, as the
  language does on an index past the end, and the doc comment says when.
- **A closed vocabulary of codes for the whole library.** A code means one
  thing in every package. The language's own keep their meanings; Rebel adds
  only the ones below, each a constant of the package that fails with it
  (`fs.EXISTS`), so a misspelt code is a compile error.

| code | means |
|---|---|
| `not_found` | a search found nothing; a name names nothing (a key, a program, a host) |
| `file_not_found` | a path names nothing, the language's own code for it |
| `exists` | it is there already and the call would not replace it |
| `permission` | the system refuses this to the program |
| `not_a_directory`, `is_a_directory`, `not_empty` | the path is of the other kind, or holds something |
| `invalid` | the input is not what the function reads; the message says where |
| `does_not_fit` | a number is outside its type, the language's own code |
| `not_text` | bytes that are not UTF-8, the language's own code |
| `closed` | the stream, or its other end, is closed |
| `timeout` | the time allowed ran out |
| `stopped` | the program was asked to stop while the call waited |
| `limit` | the system has no more of what the call needs |
| `unsupported` | this platform has no such thing, or this format has no form for the value asked (a NaN in JSON, a date in MessagePack) |
| `failed` | the system refused for a reason none of the above names; the message carries the system's own words |

- **A message says what was asked and what was found**, in words a reader can
  act on: `no "x" in "abc"`, never `failed`.

## A5. One way, nothing by halves, behaviour and not source

- **Rebel never offers a second way to do what a built-in does.** `join`,
  `repeat`, `sort`, `read_file`, `write_file`, `args` and `exit` are the
  language's; the ledger says `built-in` beside them.
- **Nothing is called complete by halves.** `http` without HTTPS says so and is
  not declared complete. `regex` matches in time linear in the text, with no
  backtracking and no back-references, because a search must not be able to
  hang a program. `time` names the IANA version of the zone data it carries. `unicode`'s tables name the
  Unicode version they hold. The environment is read and never written: in C a
  write beside another thread's read is undefined.
- **Rebel is written from behaviour, never from another library's source.**
  Read what a function does in the documentation of Go, Rust and Python, and
  ask the three on this machine what they answer; do not open their source to
  write yours, and copy no test table. An answer observed is a fact; a line
  copied is somebody's text.

## A6. The stream

Heroes has no interfaces, so what composes Rebel is one handle. A file, a
pipe, a terminal, the process's own input and outputs, a child's ends and a
socket are all an `io.Stream`, read and written by the same functions of `io`;
buffering, lines and copying are written once, there. `fs.open`, the pipes of
`process` and the connections of `net` answer an `io.Stream`, and nobody
writes a reader of their own.

## A7. Documentation

Decided with the author on 2026-11-16, on three things this project had
already ruled: **a comment directly above a declaration documents it** and a
comment is Markdown, `##` a section heading (spec § 1); **the source makes the
document, never the other way** (design.md Part 6, the literate-source row);
and **an example is a documented `test` block**, Go's and D's form, never code
inside a comment (design.md, the doctest refusal homed at M-doc-generator). So
the shape below needs no syntax of its own, every example compiles and runs,
and `heroes doc` reads all of it.

**Every public declaration has a doc comment directly above it.**

**A module** (a face) opens with a comment whose first sentence says what the
package is for, then what it does NOT do and which package does, then the codes
it fails with. `##` headings divide the face into topics.

**A function**, in this order:

1. **the first sentence says what it answers, or what it does**, and can stand
   alone (the card shows it alone);
2. a line holding `#` alone, then **one bullet per parameter, every one, in the
   signature's order**: `` - `name`: `` what it is, its unit (bytes,
   characters, nanoseconds), the values it takes and what happens at its edges
   (empty, zero, negative). Four kinds say more:
   - an `@` parameter: what the function changes in the caller's place;
   - a function given as a parameter: when it is called, with what, and what
     its answer means;
   - a handle (an `io.Stream`, a `process.Child`): who closes it;
   - a type parameter (`<A>`) gets a bullet only where it carries a condition
     the compiler cannot state (that `==` is meaningful on it, for instance).

   A function with no parameter has no list;
3. a line holding `#` alone, then **what the answer holds at its edges** where
   the first sentence does not say it: an empty result, the order of a list;
4. **`Fails` and each code with its when**, for a function that can fail,
   only codes of § A4; a function that hands on a failure it was given says
   so in those words, "Fails where `r` fails, with its code" (`errors`); **`Aborts` and when**, where it can;
5. **the cost** where it is not linear in the input or it copies, and **the
   platforms** where Windows answers otherwise (`unsupported`, another rule);
6. **`See also`** and other functions as `` `package.name` `` (or `` `name` ``
   in the same package), each of which must exist.

**A record**: what it represents, and what holds between its fields; how a
valid one is made (built directly, or only by a function, which is named); if
it holds a handle, who closes it. **A comment above every field**: what it is,
its unit, its values.

**A variant**: what it chooses between. **A comment above every case**, and
above every field of a case.

**A constant**: what it is; for a failure code, which functions fail with it.

**An example** is a `test` block in the face, under a `## Examples` heading at
the end, **with a comment directly above it** whose first sentence says what it
shows; its title opens with the function's name and a colon, which is how
`heroes doc` puts it under that function. **Every public function has at least
one example.** It shows the ordinary call first; edges stay in the ordinary
tests. A test with no comment above it is an ordinary test and is not shown.

**Nothing else**: no `since`, no `deprecated` (before 1.0 nothing is kept for
an older program), no HTML, no code inside a comment.

**What reads it**: `heroes doc <face>` prints the package's reference, the same
bytes every time: each declaration's signature, its comment, the call derived
from its signature with its labels, and its examples. The site's reference
pages are made from that output, in English in both editions. The short card a
model reads is the signature, the call and the first sentence alone. The
check of the net holds every rule above that a machine can: a comment above
every public declaration, field and case; every parameter named by its own
name, in order, once; `Fails` on every fallible function, naming codes of
§ A4 only or the words of a failure handed on; every `See also` name existing; an example for every function.

```
# Where `substr` first begins in `s`, as a byte index.
#
# - `s`: the text to search.
# - `substr`: the text to find; the empty text is found at 0.
#
# Fails `not_found` where `s` holds none. It compares bytes and cuts no slice
# on the way, so it never splits a character. See also `index_of_last`.
function index_of(s: str, substr: str) -> i64?

## Examples

# The first place a word stands in a line.
test "index_of: a word in a line"
    assert index_of(s: "to be or not to be", substr: "be").must() == 3
```

## A8. The ledger: what makes "complete" a claim that can fail

`<package>.ledger`, one block per **capability**, a thing a program can do:

```
capability text/find-first  where a text first holds another
  heroes  strings.index_of
  go      strings.Index, strings.IndexByte, strings.IndexRune
  rust    str::find
  python  str.find, str.index
  note    absent is `not_found`, where Go and Python's find answer -1
```

- `heroes` is one of: the function (`package.name`), `built-in <name>`,
  `refused: <reason>`, or `pending`.
- `go`, `rust`, `python` name the entries of each library that this capability
  covers, or `-`. **Every public entry of the three libraries in the package's
  area stands in some block or in the `dropped` section** at the end, which
  lists plumbing with its reason (capacity hints, raw descriptors, unchecked
  variants, what an interface or a closure would have needed).
- `note` says whose behaviour was taken where the three disagree, and why.
- **A package is complete when no block says `pending`.** A block that cannot
  be done well in time says `pending`; nothing is written badly to empty it.

The lists come from this machine: `go doc -all <package>`, the Rust
documentation under `~/.rustup/toolchains/stable-aarch64-apple-darwin/share/doc/rust/html/std`,
and `python3 -I -c "import m; print(dir(m))"`. Versions: Go 1.27, Rust 1.100,
Python 3.15.

## A9. Tests

- **Every function has `test` blocks in its own module, the edges first**:
  empty, one element, absent, the largest and the smallest value of every
  numeric type it takes, text with characters of more than one byte, a
  separator at either end.
- **Expected values are observed, not remembered.** Ask the reference
  libraries what they answer for the same input, with one-off commands
  (`python3 -I -c '...'`, a Go file run with `GOCACHE` set inside the scratch
  folder), and write what they answer. Where they disagree, decide, and write
  the decision in the ledger's `note`.
- A test's title says the function and the cases: `"index_of: at the start,
  inside, absent, and a text of more than one byte"`. No two tests of a
  package share a title.

---

# Part B. How the code is written

`heroes fmt` holds the format. This part is about substance: what the language
costs, what it aborts on, what it refuses, and the shapes that work. Each line
is something this project measured or the specification states.

## B1. What costs

- **`+` on `str` copies both sides**, so a concatenation in a loop is
  quadratic. Collect the pieces in a `[str]` and `join` once.
- **`xs @ xs.push(v)` grows in place when `xs` is a plain name.** Reached
  through a field or an index it copies the whole array each time: measured,
  one hundred thousand pushes took 14.78 s through a field and 0.00 s on a
  name. Work on a local name, or lend the list to a function as an `@`
  parameter.
- **Every value is a copy.** Passing a large list to a function that only
  reads it costs nothing extra; marking the parameter `lent` says so. Mark a
  parameter `@` only where the function works on the caller's own place by
  nature: a reader advancing, a builder.
- **A slice of a `str` is a new `str`.** To search, compare bytes with
  `s[i]`, which is a `u8`, and cut once at the end.

## B2. What aborts, and so must never be reached by input from outside

- **A slice that splits a character.** `s.slice(from:, to:)` aborts where
  either end falls inside a character of more than one byte. Cut only at
  indices you reached by walking characters or by matching whole bytes.
- **An index or a slice past the end.**
- **Overflow, at every width**, and division by zero. Nothing wraps. Where an
  algorithm wants wrapping arithmetic, it is written another way; where a
  result may not fit, check before the operation or use `to_<type>`, which
  answers a `T?` failing `does_not_fit`.
- **`.must()` on a value that can be an error.** It is for an invariant the
  code has just established, never for something read from outside.
- **Ordering a `nan`.**
- **Running out of stack.** Where recursion would follow the data (a nested
  JSON value, a tree of directories), hold an explicit stack in a list. A
  build under `--sanitize` takes more stack a call: `toml`'s reader held 512
  levels plain and overflowed sanitized (2026-11-16).
- **A loop that stands still never aborts: it fills the memory.** Every pass
  of a loop over input moves at least one byte or returns, on every input,
  the error paths first. `toml`'s reader stopped at a backslash in a literal
  string without moving, pushed an empty piece each pass, and grew to 7 GB
  before it was killed (2026-11-16).

**And what the compiler accepts and means otherwise**, which no check finds:

- **A byte in an `f"..."` writes its number.** `s[0]` is a `u8`, so with `s`
  holding `"use"`, `f"{s[0]}"` writes `117` (run 2026-11-16). A character is
  written as text, `f"{s.slice(from: 0, to: 1)}"`, which writes `u`.

## B3. What the compiler refuses, and a model writes anyway

- **`<`, `<=`, `>`, `>=` take numbers only.** Two characters are compared by
  their bytes: `c[0] >= '0' && c[0] <= '9'`. Two texts are ordered by a
  function of `strings`.
- **No implicit conversion, widths included.** `1 + 2.0` is an error; so is an
  `i64` where a `u8` is asked. A literal takes the type its context asks.
- **An unused binding or parameter**, a cell nothing re-binds, an `@`
  parameter nothing writes. `_` names what you do not use.
- **A name that shadows another**, a module's binding included: a parameter
  may not be called `strings` in a module that uses `strings`.
- **Two arguments of one type without their labels**: `copy(from: a, to: b)`.
- **A fallible value dropped**: `_ = f()` is refused where `f` answers `T?`.
  Answer the error: `?`, `match`, `.default(v)`.
- **`+` on two lists.** It takes `i64`, `f64` or `str` pairs alone
  (`bad_operand`, run 2026-11-16). Bytes are joined by `bytes.concat`, or by
  `push` into a list named or passed `@`.
- **Bitwise operators on anything but `i64`.**
- **`match` on a variant without every case**; `_` is not allowed there.
- **No anonymous function, no closure, no method, no default argument, no
  variadic, no tuple, no mutable global.** A function taking a function takes
  a top-level one; several results are a record; an option is a record built
  by a function.

## B4. The shapes that work

Every line below was run.

**Walk a text by characters, collect, join once.**

```
# `s` with only the characters `keeps` accepts, built in one pass.
function keep_where(s: str, keeps: (function(str) -> bool)) -> str
    kept: [str] @= []

    for c in s.chars()
        if keeps(c)
            kept @ kept.push(c)

    return kept.join("")
```

**Search by bytes, fail with a code and a message that says what was asked.**

```
constant NOT_FOUND: str
    "not_found"

# Where `substr` first begins in `s`, as a byte index. Fails `not_found`
# where `s` holds none. It compares bytes and cuts no slice on the way, so it
# never splits a character.
function index_of(s: str, substr: str) -> i64?
    last = s.len() - substr.len()
    at: i64 @= 0

    while at <= last
        matched: i64 @= 0

        while matched < substr.len() && s[at + matched] == substr[matched]
            matched @ matched + 1

        if matched == substr.len()
            return ok(at)
        at @ at + 1

    return fail(code: NOT_FOUND, msg: f"no \"{substr}\" in \"{s}\"")
```

**Several results are a record; an error goes up with `?`.**

```
# What `halves` answers: the text before the separator and the text after it.
record Halves
    before: str
    after: str

# The two parts of `s` around its first `sep`. Fails `not_found` where `s`
# holds no `sep`.
function halves(s: str, sep: str) -> Halves?
    at = index_of(s: s, substr: sep)?
    return ok(Halves(before: s.slice(from: 0, to: at), after: s.slice(from: at + sep.len(), to: s.len())))
```

**A function that works on the caller's own list takes it with `@`.**

```
# Adds `line` to `lines`, which the caller owns: an `@` parameter works on the
# caller's own list, so nothing is copied.
function add_line(@lines: [str], line: str)
    lines @ lines.push(line)
```

**A character is compared by its first byte.**

```
# Whether `c`, one character, is an ASCII digit: a character is compared by
# its first byte, since `<` takes numbers only.
function is_digit(c: str) -> bool
    return c[0] >= '0' && c[0] <= '9'
```

## B5. The loop of work

From the worktree's root, for the package `<p>`:

- **Write the face first, bodies as `???`.** `./heroes check
  examples-rebel/reach_<p>.hero` accepts a module with holes and lists each
  hole with the type it expects; a caller of a function with a hole already
  type-checks.
- **Format at the moment of writing**: `./heroes fmt <file> --in-place`.
- **While writing, `./heroes check` only.** It compiles no C and costs little.
- **When a module is whole, its tests**: `./heroes test
  examples-rebel/reach_<p>.hero`. A module holding a `???` builds no binary, so
  the tests of a package run once every module the driver reaches is whole.
  The machine is shared and its memory is short (author, 2026-11-16 11:40: two
  test runs at once at most): one test run of yours at a time, and none while
  two others are running (`ps -axo command | grep -cE '^[^ ]*heroes test '`, which counts the compiler's own command line and not the shells that launched it, as `pgrep -f` did, two or three for one run). **A test program past 1 GB of memory is a defect, never a heavy test**: watch `ps -axo rss,comm | grep reach_` and stop it yourself (on 2026-11-16 a test of `toml` grew 200 MB a second to 7 GB and the coordinator killed it).
- **Record every outcome**: append `2>&1 | tee -a` to
  `.claude/worktrees/rebel-scratch/logs/<p>.log` (the scratch folder beside the
  worktree). It is how the project measures how often a first attempt
  compiles, so it must be the compiler's words and not a summary.
- **Never** `git add`, commit, push or stash; the coordinator commits. Never
  write outside the worktree and the scratch folder. Never edit another
  package's files: where you need something of another package that is not
  there, write what you need in your report and go on.
