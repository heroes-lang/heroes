# M-rebel-library — Rebel, the non-standard standard library

**Opened 2026-11-15 by the author**, in one conversation that began from a
question, *why do the core packages hold so few functions beside Go's*, and
ended in a plan the author approved that evening. It encloses
**M-core-packages** and **M-doc-generator**, which are retired into it. One
thing of theirs stays open on a row of its own, **M-rebel-packages-manager**:
the package manager written with Rebel.

**What it delivers**: Rebel, a library a program may take or leave, complete
in the areas it covers, written from scratch in idiomatic Heroes; the removal
of the `core` packages, every trace; the compiler, its examples and its net
using no library; a second group of examples that use Rebel; the verb
`heroes doc` and the shape of a doc comment; the site, the Zen and every living
text saying the new sentence; and one register of the decisions it invalidates.

## What was measured before it was decided, 2026-11-15

Each number its command's, on this Mac: Go 1.27.3 (`go doc -all <pkg>`), Rust
1.100.0 (its documentation on disk, inherent methods only), Python 3.15.2
(`dir()`), `core` 0.6.0 (`grep -c '^function '` on its faces).

| area | Go | Rust | Python | `core` 0.6.0 |
|---|---|---|---|---|
| files, directories, environment, processes | 145 entries | 142 | 214 | `os` 4 functions |
| paths | 29 | 74 | 101 | `path/filepath` 6, no `path` |
| text | 95 | 191 | 47 | `strings` 7 |
| numbers and text | 36 | 16 | 24 | `strconv` 2 |

The three libraries do the same things under very different counts of names,
so coverage is counted in **capabilities**, what a program can do. A first
census of those four areas, a judgement over the measured lists (reduced on
Rust 1.90's lists): 206 capabilities; Go covers 160, Rust 143, Python 183, and
`core` with the built-ins 36. A sketch of signatures in idiomatic Heroes
covered 166 of 183 (files, paths, text) with 172 functions, where Go's 269
entries for the same areas cover 140. Copied to the letter, Go passes for 106
of its 274 entries in `os`, `path`, `path/filepath`, `strings` and `strconv`.

How much each function is called, counted over Go's own tree (2.3 million
lines, tests and vendored code left out, real functions of each package only):
of the 176 functions of `strings`, `os`, `path/filepath` and `strconv`, 61 make
90 % of the calls and 29 are never called; `core` 0.6.0 with the built-ins
answered 55 % of them. One corpus, and systems code.

What removing `core` means, counted the same day: 17 of the 56 programs of
`examples/` use it (35 `use` lines in 23 files); 115 modules of the net bind
its `strings` as `core_strings` and call three functions of it
(`contains` 921 times, `has_prefix` 507, `has_suffix` 159); 113 files of
`tests/golden` name it as a fixture; 18 files of `selfhost/` name it; 339
living files in all, beside 19 placed copies (741 files).

## The author's decisions of 2026-11-15

Each written as what the author meant; the Italian stands in the conversation.

1. **The name is Rebel**, *The Non-Standard Standard Library for Heroes*. The
   rule that keeps Bowie out of library names does not reach the name of the
   library as a whole: `rebel` is the catalogue entry, and the packages inside
   keep plain names (`os`, `net`, `http`). *Rebel* is a common English word, as
   *Heroes* is; that others use it is no concern.
2. **Original, from scratch**: its own tree and its own names, in idiomatic
   Heroes, inspired by Go, Rust and Python, with coverage equivalent to theirs.
3. **Everything in one wave.**
4. **A new row, at once**, enclosing M-core-packages and M-doc-generator.
5. **Massive and fast**: hours, not days; as many tokens as it takes; generate
   everything, check at the end, repair on the fly; no issue opened and closed
   at every step, no sitting for every question; the whole net at most three or
   four times.
6. **The compiler does not use Rebel to write itself.** Only the package
   manager will, on its own row.
7. **Two groups of examples.** The ones the compiler is tested on use neither
   Rebel nor `core` and stay a fairly complete set; the ones with Rebel are a
   second group. **Rebel has a CI pipeline of its own**, which compiles and
   tests Rebel and nothing else, and starts only when a push touches Rebel.
8. **`core` no longer exists**: every trace removed.
9. **The new sentence everywhere**: Heroes imposes no standard library and
   offers one that is optional. The Zen's law 18 becomes *"There is no standard
   library to obey; there is a Rebel to call, or not."*
10. **Superseded decisions are declared invalidated** in one register; living
    texts are rewritten clean; sittings, journals and closed issues stay, each
    with its note. The language is under construction: no backward
    compatibility is owed, and anything may be broken.
11. **Go is not cited in the licence.** Go, Rust and Python stand on the thanks
    page as the library's inspirations. **The runtime exception is extended to
    Rebel**: a program that uses it owes nothing to anybody.
12. **The site** explains the rebel library, *use it or do not*; the song
    *rebel rebel* goes to Rebel's page.
13. **The documentation is decided first**: the shape of the comment above a
    function, a record and a variant, and the verb `heroes doc`, both in this
    step; and a writing guide for the agents that generate, beyond format.
14. **New rules in `CLAUDE.md`**: the compiler does not use the library to
    write itself; the examples are two groups.
15. **Building Rebel is also a measure** of how well models write Heroes.
16. **Every open issue is reviewed.**
17. **The other session is closed for good** and this one takes its work; its
    two sittings, 251 and 252, go on in the background.
18. **From the Bell Labs school, the good things, without changing the plan**:
    those rules were made where writing code was the expensive part.
19. **The C doors are tried on the Windows machine before the landing.**
20. **A free hand**: pushes, the site's publication and the removal of
    leftovers are not asked for; everything else was asked before the start.
21. **Later**, Rebel and its examples may have a repository of their own.

## The author's decisions of 2026-11-16

Written as what the author meant; the hours are left out where the coordinator's
notes did not read them from a clock.

22. **The documentation's shape**: each parameter in a markdown list; one
    documented example for every public function, under `## Examples`; the
    reference pages in English in both editions of the site
    (`examples-rebel/README.md` § A7).
23. **The package list grows to what a complete library holds**, from Go, Rust
    and Python and from the ecosystem's most used libraries as yardsticks:
    `xml` and `yaml` as `json` is; `flags` as complete as clap, `csv`, `ini`,
    `toml`, `hash`, `crypto`, `random`, `uuid`, `sets`, `threads`, `stats`,
    `compress` and `archive`, `template`, `big`, one `sql` API with
    `sql/postgres` (its protocol written in Heroes) and `sql/sqlite`; time
    zones in `time`, integers to and from bytes in `bytes`, a heap in
    `lists`; `poll` for what tokio gives, `errors` for what anyhow gives, `cbor`
    and `msgpack` with conversions for what serde gives, `term`; an `http`
    client as complete as Python's requests; `tls` from the system's own TLS
    (SChannel, Apple's Network.framework, OpenSSL loaded on demand on Linux,
    nothing deprecated); `html` for what BeautifulSoup gives (WHATWG parsing,
    CSS selectors); `web` for what Echo gives (it encloses M-web-framework);
    `sql/build` and `sql/migrate`, an ORM refused; `image`, `compress/zstd`,
    `brotli`, `bzip2`, `xz`, `sql/mysql`, `redis`, `unicode/normalize`,
    `segment` and `names`. The list was then declared closed at 59.
24. **What reads data from outside is written in Heroes**, never bound from a C
    library; a large table or a large capability goes in a subpackage, so only
    its users carry it (§ A2).
25. **The Mac's memory is shared**: fewer agents at once (the author, on
    seeing the swap full), which the coordinator set at five agents and two
    test runs.
26. **Twelve more packages**, the list reopened by the author's own question
    (*inter-process communication, gRPC, what else?*), each chosen from a
    measured census of what the 44 written packages held: `ipc` (a local
    channel by name, shared memory, a lock and a semaphore between processes;
    the local sockets `net` had refused), `protobuf`, `http2` and `grpc`,
    `websocket`, `mail` and `mail/smtp`, `semver` (which the package manager
    needs), `diff`, `markdown`, and `crypto/seal` and `crypto/keypairs` from the
    system's own cryptography (lifting `crypto`'s refusal of ciphers by the
    route `tls` took). Rebel is 71 packages.
27. **The list closes at 85, after a census of four more standard
    libraries** (2026-11-17, the author: *before closing the set for good,
    look at what Odin, Zig, .NET and Java hold that Rebel does not*). Read
    that day from their own trees: Zig 0.15.2's `lib/std` and Odin
    dev-2026-11's `core/` and `vendor/` on this Mac, the JDK's modules in
    `openjdk/jdk` and .NET's libraries in `dotnet/runtime` on GitHub; each
    candidate then looked for in Rebel's ledgers. Added, each the author's
    choice: `containers` (ordered map and set, deque, bit set, LRU cache,
    topological order), `asn1`, `pem` and `x509`, `math/linear` with complex
    numbers in `math` (lifting `math`'s refusal), `fs/watch`,
    `unicode/encodings` (WHATWG), `i18n` (catalogs, plurals, locale formats,
    collation, from CLDR), `dns`, `keyring`, `sql/odbc`, `ldap`,
    `compress/lz4`, `serial`; and small additions: argon2 and bcrypt in
    `crypto`, atomic writes and mapped files in `fs`, memory in `os`, the line
    editor and text tables in `term`, server-sent events and a rate limit in
    `web`, ping in `net`. Left out, with the coordinator's reasons accepted:
    graphics, windows, audio and games (a family of C bindings for later);
    QUIC and HTTP/3; reflection, dependency injection and automatic
    serialization (no reflection); channels, pools and atomics (threads share
    nothing); SIMD and tensors, loading C at run time (the language's
    questions); a parser of Heroes (a second grammar); object-file formats,
    Windows-only services, scripting, feeds.
28. **And 102, after a census of the third-party libraries most used in Go,
    Python and Rust** (2026-11-17, the author: *look at the most used external
    libraries and see what makes sense to do*). Measured that day: PyPI's
    downloads of the last 30 days (15,000 projects, the ranking of
    2026-11-01), crates.io's 300 most downloaded crates of late (its API), and
    for Go, where no ranking is published, pkg.go.dev's *imported by* count of
    90 candidates the coordinator chose (the list is the coordinator's, the
    counts are pkg.go.dev's). Added, each the author's choice: `jose`,
    `oauth2` with OpenID Connect, `metrics`, `html/sanitize` (lifting `html`'s
    refusal as a subpackage of its own), `config` with `.env` files,
    `json/schema`, `json/patch`, `json/path`, `xlsx`, `pdf`, `s3`, `nats`,
    `mqtt`, `amqp`, `kafka`, `jsonrpc`, `mcp`; and small additions: IDNA in
    `url`, case styles and shell quoting in `strings`, a TOML document edited
    in place in `toml`, gitignore patterns in `path`, retries' waits and cron
    in `time`, system and process statistics in `os` and `process`, half
    floats in `numbers`, sessions and signed cookies in `web`, multipart read
    in `http`, an insertion-ordered map in `containers`, the detection of an
    undeclared encoding in `unicode/encodings`. Left out, the coordinator's
    reasons accepted: data science (n-dimensional arrays, data frames,
    plotting), terminal UI frameworks for now, validation and mapping by
    reflection, cloud SDKs beyond S3, AI API clients, syntax highlighting,
    git.
29. **The system's cryptography leaves a gap on macOS, kept in 0.1, and a
    sitting follows** (2026-11-17). Measured that day by `crypto/seal`'s and
    `crypto/keypairs`' probes: macOS offers no AES-GCM, ChaCha20-Poly1305,
    Ed25519 or X25519 through a public C API (CryptoKit is Swift's alone), and
    Windows' CNG has no Ed25519. Those answer `unsupported` there, said in the
    ledgers. The author's choice among three: the gap now and a sitting after,
    on writing those primitives in Heroes under a constant-time discipline the
    tools check, as Go writes its own; refused were loading OpenSSL where a
    Mac happens to have it (a premise about the machine) and the gap with no
    sitting.

    **Measured the same evening, on the author's question** (*have we ever
    tried calling Swift from Heroes?*: never, the repository held one line of
    design.md naming Swift's `@_cdecl` as part of the C ABI): three functions
    of Swift over CryptoKit, exported by `@_cdecl`, built by `swiftc` 6.4.2 as
    a static library in 2 s, were called from C (`-Werror`, the linker taking
    CryptoKit, Foundation and the system's Swift runtime on its own) and from
    a Heroes program through an `extern` group with `link` and `--library`,
    plain and under `--sanitize`: the Ed25519 public key of RFC 8032's first
    vector equal, a ChaCha20-Poly1305 seal opened back to the same bytes, a
    bit changed refused, by a probe that is not kept. So CryptoKit is
    reachable; what it would cost is the toolchain's: the platform layer
    compiles C alone today, and a member written in Swift needs `swiftc` on
    every Mac that builds a program using it, a change of the compiler's
    surface the sitting decides beside the constant-time route.
30. **`containers` split into subpackages** (2026-11-17, the author's question
    *is it thought out, or a bit in sets, a bit in maps, a bit in lists?*,
    answered by reading the four faces): `lists`, `maps` and `sets` work on
    the language's own shapes and stay; `containers` had become one face of
    122 functions whose prefixes (`ordered_`, `deque_`, `bits_`, `cache_`,
    `indexed_`) betrayed seven structures in one place, the graphs were
    algorithms and no container, and the heap stood in `lists`. Now
    `containers/ordered`, `/deque`, `/bits`, `/cache`, `/indexed`, `/heap`
    (moved from `lists`) and a package `graphs`, plain names, the same code.
31. **Before `main`, a review of coherence and style of the whole library**
    (2026-11-17, the author: *before going to main, a check of coherence and
    style: redundancies, things not well organised*). It is a step of its own,
    after the last package and before the final gate, and it is done in two
    halves:
    - **measured by tools first**, each an instrument of the net rather than a
      reading: the same capability `done` in two ledgers; two public functions
      of two packages with the same name and the same signature; a face whose
      functions share a prefix (the smell `containers` showed); a face past a
      size the review sets; a name the grammar of § A3 does not allow; a code
      outside § A4's list; a package whose `use` lines break the layers; a
      helper written twice in private modules (the same body in two packages,
      as `cbor`'s and `msgpack`'s floats);
    - **then read**, one reader per layer who has not written it, opening
      each package as a stranger would: is each capability where a programmer
      would look for it, does each package say what it does not do and where
      that lives, do two packages answer the same question two ways. Every
      finding is fixed or written down with its reason before the gate, and
      the list of what moved is the review's record.
32. **What a complete web framework needs, checked against the ledgers**
    (2026-11-17, the author: *think of everything a web framework needs: do
    we have every package?*). Read from Echo and Gin, Flask and FastAPI,
    Django and Rails, each capability looked for in Rebel's ledgers. Absent,
    and added by the author's choice: security headers, the client behind a
    proxy, a test client with no network, ETags and 304 for dynamic
    responses, a graceful shutdown, content negotiation and CSRF tokens in
    `web` and `http`; TOTP in `crypto` and slugs in `strings`; and three
    packages, `openapi` (a document built as data, served, requests
    validated), `graphql` (the program resolving each field, no closures) and
    `jobs` (background jobs in PostgreSQL or Redis). Rebel is 111 packages.
    **And a sitting on threads that share nothing**, by the author's choice:
    a server serving connections in parallel rebuilds its router in each
    thread and shares no connection, cache or session (`mail/smtp` refused a
    pool for it; `web` and `http` said it), the limit a web application with
    traffic meets first, which only the language can decide.
33. **The open questions sit as three sittings, after the last package and
    before the review of coherence** (2026-11-17, the author: *can we merge
    the sittings?*). Read that evening: sittings 251 and 252 had their briefs
    and the critic's reading and no seat's report, their seats stopped by the
    usage limit, and 253 had nothing written, so nothing sat is reopened.
    **A, threads and signals**: 251 (a Heroes function a C handler reaches,
    defect 2522), 252 (the end of `main` and the threads still running,
    defect 2552), and threads that share nothing (decision 32); their briefs
    are A's input. **B, the language as Rebel met it**: a private module that
    cannot use its own face (more than ten packages' witnesses), a type
    parameter not taken from a map literal's key (spec § 9), records with no
    type parameter (`containers`), a byte in `f"..."` written as its number
    with no word from the compiler, `print` writing `1e-06` where a literal
    with an exponent is refused. **C, Rebel's place and its tools**:
    design.md §1.11's sentence and the verb `heroes doc` (what 253 was to
    hold), the route of the system's cryptography on macOS (a member written
    in Swift, or primitives in Heroes under a checked constant-time
    discipline; decision 29), and a skipped outcome for `heroes test`. Each
    sitting takes the seats its questions ask for; numbers are given when
    they convene.
34. **And 118, from a list of forty kinds of library the author brought**
    (2026-11-17), each looked for in Rebel's ledgers and the coordinator
    asked to suggest only what is truly worth it. Added: `json/jq`, an
    interpreter of jq over `json.Value` (so also over YAML, TOML, CBOR,
    MessagePack and XML read into one), with a small jq among Rebel's
    examples; `scanner`, a lexer as Go's standard `text/scanner`; Aho-Corasick
    in `strings`; `containers/trie` and `containers/disjoint`; `property`
    (property-based testing with shrinking) and `bench`; `tracing` with
    OpenTelemetry's export; logs written to rotating files in `log` and a
    circuit breaker in `time`; and the whole stack of calls when a program
    aborts, which is the runtime's, joins sitting C. Not suggested, with
    their reasons: JMESPath beside jq and JSONPath, CEL for now, a Bloom
    filter, a PEG parser (`scanner` first), a data table in Polars' manner
    (data science stays out), events inside one process (no closures),
    reflection, arenas and object pools (the language's), mocks (no
    interfaces).
35. **One umbrella, one version, placed package by package** (2026-11-17, the
    author: *would it make sense to publish the packages as distinct ones,
    under one Rebel umbrella?*). Measured that evening: Rebel's 76 packages
    with a face held 313,990 lines of Heroes (841 files, 14 MB), placed whole
    in every program that uses one of them; the closure of a program's own
    packages, each counted by its face, its `internal/` and its floors, is
    15% of that for a CLI reading JSON (11 packages, 45,537 lines), 16% for a
    tool like grep, 26% for the package manager (82,716 lines) and 39% for a
    web service on PostgreSQL (120,906 lines). A first count read 34% for the
    CLI, wrong: it took a subpackage still without its face as its parent's.
    The author's choice: Rebel keeps ONE version, tested and published as one,
    and a program places only the packages it uses with their Rebel
    dependencies; independent versions per package are refused (the diamond
    conflicts of crates and npm, and the one vocabulary of codes and one
    stream the library keeps). How it is spelled (`use`, `heroes add`) and how
    it meets panel 242's R2 and panel 250's whole placement is sitting C's,
    with these measurements.
36. **And 130, from what the communities use** (2026-11-17, the author: *read
    in the communities of the various languages, and in other standard
    libraries, what is missing so that web services, microservices, command
    lines and AI agents can be written*). A census agent looked for each
    candidate in the table, in every ledger's rows, in the queued tasks and in
    the sources, and counted it missing only where all four were silent; it
    read the awesome lists of Go, Rust, Python and Node, the 2025 surveys of
    Go, Rust and Stack Overflow, JetBrains' Python survey, Deno's `@std`,
    Nim's, Erlang's, Kotlin's, Swift's, Crystal's and D's libraries, the SDKs
    of Anthropic and OpenAI, LangChain and LlamaIndex, and measured each
    candidate's downloads or stars that evening (GitHub, PyPI, npm,
    crates.io). Of its rows the coordinator recommended fifteen and the author
    took all fifteen. **Twelve new packages**: `llm/anthropic` and
    `llm/openai` (the second also reaching Ollama and vLLM), raw HTTP from the
    providers' published references since Heroes has no SDK; `tokenizer`
    (byte-pair encoding as tiktoken does it, its tables in a subpackage) and
    `chunks`; `vectors` (an HNSW index in memory) and `search` (BM25, which the
    package index will use); `bson` and `mongodb`; `tui`, immediate mode as
    ratatui; `webauthn`, `acme` and `qr`. **And additions to finished
    packages**: partial JSON in `json`, for a tool's arguments a model streams;
    markdown written, and HTML read into markdown; health endpoints and
    Standard Webhooks signatures in `web`; prompts, menus, OSC 8 links and
    OSC 52 clipboard writes in `term`; byte sizes for people in `numbers`; the
    data, state, cache and runtime folders and opening a URL in its
    application in `os`; the public suffix list and a client through a proxy
    in `http`; logs exported by OTLP with their trace ids, and StatsD, beside
    `metrics`; a distributed lock on Redis; line breaking by UAX #14 and
    wrapping by display width in `unicode/segment`. Rows of the census not
    recommended that evening, kept here so they are not lost: a Kubernetes
    client, an SSH client, CloudEvents, Web Push, RSS, Atom and JSON Feed
    (left out by decision 27), systemd's notify and socket activation,
    consistent hashing; and the census's own *not now*: readability
    extraction, vector database clients, Jaro-Winkler, Elasticsearch, feature
    flags, Vault, process supervision, Consul and etcd, idempotency keys,
    pseudo-terminals, self-update, GeoIP, WebP. The same census found the
    MCP task written against an older revision: 2026-07-28 drops the
    initialize handshake and asks RFC 9728, 8707 and 9207 of authorization;
    the task was amended that evening. The table reads 130 rows, layers 0 to
    7 and no package using one of its own layer or above.

## The author's answer of 2026-11-19: what panel 253 left owed is built here

At about 21:25, to the coordinator's question whether the landing holds the three things panel
253 left owed or leaves each a row of its own after it (the coordinator recommended the rows),
meant as: *no, I want to face them before closing this whole chat, so run them one at a time and
let us resolve them; and as always, when we close the steps no blocking may be active.* So no
row of the chain is written for them, and this milestone owes them, one at a time and in this
order, the first with the landing's umbrella lane and the two others after it:

1. **Panel 253's R6, `heroes fetch` placing a `packages` list's closure**: an entry's listed
   packages placed with each one they use, priced by the sitting's compiler-engineer at about 150
   lines and unbuilt; with the umbrella's lane, which builds the list, the refusal and `heroes add
   rebel/<path>`.
2. **Panel 253's R5, the whole stack of calls when a program aborts**: the frame walk with its
   lines resolved by the parent process, (W) with (A), and its three repairs (the walk starts at
   the aborting frame; the lines are resolved in the parent, never by `dladdr` in the child on
   Linux; the program's units built with `-fno-omit-frame-pointer` on x86-64), built and run on
   the three platforms. The sentence in spec § 6 stays vetoed.
3. **Panel 253's R3, the mixed route of the system's cryptography on macOS**: the AEADs through
   CryptoKit, Ed25519 and X25519 through a vendored C library, built and measured. It waited for
   the author's answer to decision 2594, whether decision 24 admits a vendored C cryptography
   library, and the answer of about 21:50 below admits one for primitives that read no outside
   data.

**And the milestone closes over zero open `blocking` and `systemic` defect issues**, as CLAUDE.md
§ Verification says of every tag.

## The author's answers of 2026-11-19, about 21:50: yes to the coordinator's six recommendations

Given in conversation, meant as: *yes to everything*. Each written as what it settles:

- **Decision 24 admits a vendored C cryptography library for primitives that read no outside
  data** (decision 2594, its comment of 20:51 UTC carrying the words). A cipher, a signature and a
  key agreement work on bytes the program already holds, so *"what reads data from outside is
  written in Heroes, never bound from a C library"* stays true; and the ffi-pragmatist's reserved
  veto (panel 253's R1) stands: no wording of this answer stops a program binding a C library
  itself. Its first use is panel 253's R3, the third of the owed steps above, built and measured
  before the milestone closes. Decision 2594 closes with the commit that writes this answer here
  and in design.md.
- **The panel skill's blind seat runs with the measured repairs of defect 2589**:
  `.claude/skills/panel/SKILL.md` amended (CLAUDE.md § 3: the skills are amended by the author's
  instruction), committed on `rebel-round` at `cb2ad6d7d`.
- **The folders earlier sittings' blind and paid sessions wrote under the user's own
  `~/.claude/projects/` move inside the repository's root** (CLAUDE.md § Hard stops: no file
  outside it): 694 folders and 739 transcripts, moved uncommitted to
  `.claude/worktrees/rebel-scratch/old-blind-transcripts/` with its README and MANIFEST. The
  coordinator's earlier figure, 189 folders, was a part it could not reconstruct.
- **No new paid blind reading now.**
- **Nothing is installed on the Windows box**; Rebel's own pipeline installs SQLite where its tests
  need it.

## The author's answer of 2026-11-20, in the first minutes after midnight: M-web-framework retired

Given in conversation, meant as: *OK to the web framework out of the ROADMAP*. It confirms the
retirement of **M-web-framework** into this milestone, which decision 23 decided (Rebel's `web`
gives what Echo gives, and it encloses M-web-framework) and the landing carried out: the row left
the chain, and the page stays as the record of what it held.

## The author's answers of 2026-11-20, about 14:00: the licence and four rulings on signals

Given in conversation, meant as: *OK to the licence, OK to A, B, C and D*, on the coordinator's
recommendations put in plain words that afternoon.

- **The licence (decision 11, amended in one sense).** Rebel keeps the two pieces written by others,
  the Public Suffix List in `http/suffixes` (MPL-2.0) and the tables taken from tiktoken in
  `tokenizer/tables` (MIT), each with its notice in its files and in `NOTICE`. Whoever uses Rebel owes
  nobody anything but those notices, which travel with the two files; a program whose `packages` line
  names neither does not carry them.
- **A, defect 2583**: a SIGABRT the program raises itself, under a library's handler that returns, is
  let go everywhere, as plain C lets it go; a real `abort()` still ends the process, only without the
  runtime's line (panel 175's silence over a false line).
- **B, defect 2522's rows 1 and 2**: a jump the runtime cannot tell safe (through a buffer it never saw
  filled; the inline `__builtin_setjmp` pair at `-O0` on x86-64) stays refused, a known cost told in true
  words and pinned in a case that gives its reason; robustness ranks first.
- **C, defect 2522's row 3**: the route for an installer reached through `dlsym` is built and measured,
  the runtime answering `dlsym` for the installers' names on Linux and re-trampolining lazily on Darwin;
  where either breaks a sanitizer, the row becomes a known cost as in B.
- **D, defect 2522's row 5**: no pinned census of the C library names the runtime defines; the census
  stays a dated measurement, since a check listing a machine's libraries would turn red by machine.

## The coherence review's nineteen decisions, adopted by recommendation on 2026-11-19

The review of decision 31 put nineteen questions, D01 to D19, each with its recommendation and the
count it rests on, taken that day by the review's readers over Rebel's faces; the counts below are
theirs and were not run again for this page. **The coordinator adopted every one as recommended on
2026-11-19**, by the batch's rule (`.claude/rules/verification.md` § The batch: *a batch's own
questions take the recommended answer*), **for the author to turn**:

- **D01**: § A3 gains `from_x`, `x_with`, `x_name`, `x_named` and `x_all`, rows the library already
  writes (`from_x` 77 functions in 31 faces, `x_with` 52 in 29, `x_name` 42 in 24).
- **D02**: bytes to a value and back are `decode_x` and `encode_x`, and `parse_x` and `format_x`
  mean text alone (functions answering bytes: `encode_x` 54 against `format_x` 8; reading bytes:
  `decode_x` 64 against `parse_x` 11).
- **D03**: a value is made by its type's bare noun, or `new_x` where the noun is taken or the value
  drawn fresh, and `x_of` asks a question (129 functions in 53 faces named for the record they
  return, 75 `new` or `new_x`).
- **D04**: `default_<record>()` for the defaults and `<what>_<record>()` for a named preset
  (`default_x()` and `default_options()` 40 functions in 29 faces, `defaults()` 10 faces).
- **D05**: `x_json` writes a record as a `json.Value`, `x_of(v: json.Value)` reads it back, and
  `fetch_x` is one thing asked of a server (`x_json` 61 functions in 12 faces, the readers 39,
  `fetch_x` 8 in 4).
- **D06**: § A3's abbreviations gain `opts`, `msg`, `img`, `re`, `arg`, `ttl`, `ssid` and `_ms` as
  a unit's suffix, and not `sub`, `mul` or `neg` (`opts` 35 parameters in 11 faces, `msg` 15 in 9,
  `img` 34 in 7, `_ms` 111 fields and parameters in 29 faces; no `sub_`, `mul_` or `neg_` called
  outside its package).
- **D07**: a span a caller gives is a `time.Duration` from layer 4 up and `_ms: i64` only in `net`,
  `serial` and `term`; seconds are spelled `_seconds`; `0` waits not at all and no bound is a named
  constant; a moment a server sends is a `time.Instant` (a span in a bare `i64` is the unit mistake
  the compiler cannot refuse; `time.Duration` 51 parameters in 10 faces, `_ms` 52 in 15).
- **D08**: a field that may hold nothing is `T?`, never a `bool` beside it (50 `has_x: bool` fields
  in 11 faces, 34 pairs in `x509` and 2 in `kafka` beside the value they guard).
- **D09**: a position the program holds aborts outside the structure, as the language does past an
  index's end, and only a `get` is the checked reader (no caller outside the package for the three
  functions it reaches).
- **D10**: one heading, `## Codes`, for every code constant; a face whose ledger holds a `pending`
  block names it in its header; § A2 names the private modules a folder of subpackages shares
  (`## Codes` heads 82 faces; 84 ledgers hold 417 `pending` blocks, 69 of their faces' headers
  silent on them).
- **D11**: a check in the `rebel` suite over `heroes doc`'s output, an example per function,
  `Fails` on every fallible one and every `See also` existing, written once `heroes doc` merges
  (§ A7 promised it and no check held it; the debt read then over layers 0, 4, 7 and 8: 888
  examples and 18 `Fails` missing).
- **D12**: the 21 floors owe § A7's parameter lines, and no example is owed of a binding of one C
  function (338 of the floors' 442 functions had no line, which `heroes doc` refuses).
- **D13**: a package `otlp` at layer 7, and `metrics` and `tracing` at layer 8 (five public types
  and two exporters declared twice; no package uses either).
- **D14**: retries and circuit breakers leave `time` for a package of their own at layer 4, and
  `http`'s policy is theirs (22 names a stranger does not look for in `time`, 97 public functions
  down to 81).
- **D15**: SCRAM-SHA-256 and SASLprep become one package at layer 3 (the client steps written three
  times and SASLprep once, so a password above ASCII would log in to MongoDB and not to PostgreSQL
  or Kafka: read from the modules, unrun).
- **D16**: `math/complex`, a subpackage at layer 1 (30 `_complex` suffixes in the library's largest
  face, 155 functions, about 125 after).
- **D17**: `url/idna` moves to layer 2 and takes NFC from `unicode/normalize` (no package uses it,
  and 844 of its test file's 6,389 lines answered `unsupported`).
- **D18**: the program's stop, `catch_stop`, `ask_stop` and `is_stop_asked`, moves from `net` to
  `os`, so a program that reads a serial port need not use `net` to end on Ctrl-C (22 uses in 8
  files).
- **D19**: `regex` stays at its layer (moved to 2, `scanner` and `tokenizer`, which use it, would
  climb with every package above them, for two `pending` classes).

## The author's answer of 2026-11-20, about 20:4x: the review's follow-ups before Rebel 0.1.0

Given in conversation, meant as: *before*. The follow-ups of the nineteen decisions above, the
renames and reshapes D01 to D09, the documentation's rules D10 to D12 and the packages and layer
moves D13 to D18, land in this milestone **before Rebel 0.1.0 is published**, not in a release
after it. They went in three waves, the coordinator's plan of that evening: the splits (steps 82a
to 82e), the runtime's doors (83a, 83b) and defect 2627 (84a, 84b) together; the renames and the
spans (85a to 85k, then 88a to 88j), the floors' parameter lines of D12 beside the spans (86a to
86f); the rules last and alone (86g on). The
questions the plan put were settled by the batch's rule (`.claude/rules/verification.md` § The
batch: *a batch's own questions take the recommended answer*), for the author to turn: D18 by the
route that changes no header (the stop's door bound by a floor of `os`); D07's `0` made in C, the
doors taking `-1` as no limit so that `0` waits not at all; the new packages named `retries`,
`scram`, `otlp` and `math/complex`; `mail`'s header pair named `decode_fields` and `encode_fields`;
and the 25 leftover `plain | secured` links collapsed once `tls.connect` answered a `net.Conn`.

## The author's answer of 2026-11-21, about 11:10: a host and an address are two types (A)

Given in conversation, meant as *A*, to the question D08's last field left: `net.Address` held a
port beside a flag, `has_port`, and the renames' lane counted `port` read 132 times in 33 files,
most of them a connection's or a listener's, which always has one, so making it `i64?` would have
put a `.must()` on every one. Answer *A* **splits the record in two**: `net.Host`, an IPv4 or IPv6
address with no port, and `net.Address`, a host and a port, always, as Go's `netip.Addr` and
`netip.AddrPort` and Rust's `IpAddr` and `SocketAddr` part them. Every pure function on addresses
takes and gives a `Host`; a connection's two ends, a listener's, a datagram's sender and
`send_to`'s destination are `Address`es; `has_port` and `without_port` are gone. Built at step 88i
(`836b4c9b3`), with `dns`, `x509`, `http`, `web`, `tls`, `mail/smtp` and `fs/watch` following;
`examples-rebel/README.md` § A3 names the two types and § A1's table of shared types holds both.

## The author's idea of 2026-11-21, about 15:00: a temporary pipeline that tries every install now

Given in conversation, meant as: *a temporary pipeline that tests every install with a few lines of
Rebel, so that what would fail fails now*. **The idea is the author's** (`.claude/rules/records.md`
§ And the record says whose idea it was): the coordinator had listed the first run of Rebel's own
pipeline, `.github/workflows/rebel.yml`, as the landing's first uncertainty and meant to meet it
after the push to `main`, since every workflow here runs on a push to `main`, a tag, a pull
request or a schedule, and `rebel-round`'s pushes had started none. The author allowed the pushes
of that probe's branch for the lane that holds it.

So a branch of its own, `rebel-ci-probe`, carries a workflow of its own, on that branch alone,
`.github/workflows/rebel-probe.yml` (written 2026-11-21), which runs on a push to that branch, on `rebel.yml`'s
four legs: its installs, its SQLite check and its two-stage build word for word as `rebel.yml`'s,
then one test of each package that reaches an installed library or a new member of C (`sql/sqlite`,
`tls`, `crypto/seal`, `crypto/keypairs`, `keyring`, `sql/odbc`), each driver also built and run,
and `heroes doc` over two faces. A repair it finds goes into `rebel.yml` as it is; the branch is
never merged and is deleted from GitHub once its legs are green. Its runs, read with `gh run list
--branch rebel-ci-probe` on 2026-11-21 at 17:2x: 44912132270 red on both Linux legs (their
`/usr/bin/timeout` is not GNU's) and on Windows (vcpkg's `sqlite3.h` includes a header the step had
not copied), Darwin green; 44913457527 red on Windows alone (`shadowed_header`: the folder
`--include` named held a `sqlite3.h` standing in for the package's own); 44918720387 green on the
four legs; 44920689713 green on the four, every probe's test run again under `--sanitize` on both
Linux legs.

## The author's answers of 2026-11-21, evening: Rebel's examples are applications, and what lanes leave is swept

Given while the landing's last lanes ran, each meant as:

- *the examples of `examples/` use no library, Rebel least of all*: measured that evening, no
  `use <...>` line, no catalogue and no word of Rebel or `core` stands in them, the sixteen
  programs that used `core` carrying their own helpers since step 55;
- *Rebel's examples are not a small program per package but very complex applications, as we
  said for the web framework, that use many parts of Rebel together*; then *at least about
  twenty, and between them they must touch every Rebel package*: twenty applications, the first
  `notes`, a publishing service on `web`, whose lists of faces to use name all 149
  between them (counted against the ledgers that evening), and the sixteen twins the plan of 2026-11-15 already named (`examples-rebel/README.md` § A10);
- *the apps should be little projects, with folders inside them and their own toml of
  dependencies*: each a folder made by `heroes new` and `heroes add`, § A10;
- *the generators must write comments correctly*: a comment says what a reader needs and where a
  value comes from, never the story of the work; the compiler's own comments rewritten the same
  way after this milestone (task 2640);
- *put these cleanups as a rule: every so often the system must clean the worktrees and the
  agents*: `.claude/hooks/sweep.py`, `.claude/rules/records.md` § Working in lanes.

## The author's answers of 2026-11-22, late morning: panel 254 ratified, GTK 4, and the coordinator's choices accepted

Given in conversation to the coordinator's six recommendations (`1a 2b 3a 4a 5a`), each meant as:

- **1a, panel 254 ratified by the author's reading**, R1 to R9 (issue 2674): `(thread T)` and
  `(mailbox T)` enter the language. `lane-mailbox` merges only once the blind seat's scoring shows
  no row of R2 read wrong by two of five readers (R9's condition); a row it reopens comes back to
  the author alone.
- **2b, with the author's reason**, *I want to try to stand on the latest technologies, provided
  they are stable*: the `ui` package's Linux arm is **GTK 4**, opened at run time, and its web view
  **WebKitGTK 6.0**, the GTK 4 build; the coordinator had recommended GTK 3 and WebKitGTK 4.1, the
  pair Tauri 2 stands on, and no GTK 3 arm is written.
- **3a**: the web view's page is served through the application's own scheme from the program's
  memory, **no port opened**; the page reaches only `window.heroes.send`, and the program sets the
  content security policy.
- **4a**: the application `pipeline` is renamed **`ingest`**, since the site pairs the two groups'
  examples by name and the compiler's `examples/pipeline` is another program.
- **5a**: issue 2610, the register, gains its entry for panel 254 (decision 2498 and panel 251's
  R3, a thread exchanging one `i64` or one `str` at its start and end, invalidated by R1) and closes
  at the landing with the gate.
- The sixth question, defect 2642's ruling (Ed25519 checked without the cofactor on every
  platform), had no answer of its own; it is one of the coordinator's choices the last answer below
  accepts.

And two more the same minute:

- **`args()` on an argument that is not UTF-8**, meant as *fix it*. The coordinator had put it
  as a language question owing a sitting; read before applying, spec § 11 already holds
  `args_checked() -> [str?]`, which does not abort, beside `args()`, which does. So the repair is
  the programs': every program of the tree that reads a command line a person types moves to
  `args_checked` and tells an argument that is not text as a usage error, never an abort (the
  compiler already does: `heroes check` on a file whose name is not UTF-8 says so at exit 2).
- **The coordinator's choices, accepted**, meant as *OK, I accept yours*: the coherence review's
  D01 to D19, the settlements of the author's answer of 2026-11-20 about 20:4x, and every bullet of
  § Decided on the fly by the coordinator below are turned as accepted, 2026-11-22.

## The author's answer of 2026-11-22, afternoon: everything lands together, and nothing is added

Asked whether to land once the blocking defects closed (about five to seven hours, the `ui` package's
remaining capabilities following in a second round on `main`) or once those capabilities stood on the
three systems too (about thirteen to nineteen hours, both the coordinator's inference from the lanes'
measured durations that day), the author chose the second, meant as: *there is a great deal to bring to
`main` and I want to bring it all together; the CI finds what we do not find here, but it is far slower;
finish everything, and we add nothing more*. So the landing waits for every capability of
`ui-feature-catalogue`'s ordered list on macOS, Win32 and GTK 4, and the round takes no new item beyond
the open issues and the catalogue's rows.

**Turned by the author the same night**: asked whether Rebel has a library for audio, the microphone
and printers (it has none; the `ui` catalogue names printing and the system's beep as `could`), and
given the choice between a round of its own after the landing and this round, the author answered *B*,
this round. So `audio` (playing, recording from the microphone, the devices, WAV, the beep) and printing
(the printers, the print and page dialogs answered through the event queue, a document and a web view
printed) enter the landing. And asked *is there anything else missing?*, the coordinator measures it
before answering: a catalogue of every capability area of Go's, Python's, Rust's, .NET's, Java's and
Node's libraries against Rebel's ledgers (`rebel-scratch/coord/rebel-gap-catalogue.md`), each absent area
with a verdict, put to the author.

## The author's answers of 2026-11-23, after midnight: panel 256 ratified, the dialogs stay events, the gaps enter the round

Put as three questions by `/decide` (the open `decision` queue held issue 2697, panel 256's ratification, and
the coordinator's own choice DG04), the author answered *1a 2a 3c*:

- **Panel 256 ratified by the author's reading, R1 to R12, R5 staying queued and not adopted** (issue 2697,
  closed with the sitting's commit `d812dbce7`). The two implementation lanes, `lane-p256-check` (R1 to R4) and
  `lane-p256-wait` (R6 to R10 and R12), merge only once the blind seat's scoring shows no row read wrong by two
  of five readers, and a row it reopens comes back alone, as panel 254's did.
- **The dialogs of the user interface answer through the event queue** (DG04, the coordinator's choice of
  2026-11-22, confirmed): a file or folder dialog and an alert never block the program, their answer is an event
  of the queue the clicks come through. macOS and GTK 4 have it; the Win32 arm's agent is making it.
- **The gaps enter the round** (*3c*): the four `must` areas of `rebel-gap-catalogue.md` Rebel lacked (the
  machine's processes listed, G20; a Windows service, H13; fonts read and embedded, K10; certificates, requests
  and revocation lists made and signed, J08) and every `should` area it lacks or has in part. The measured list
  is `rebel-scratch/coord/gap-3c-rows.tsv`: 44 rows, of which K14 and K16 (audio and printing) and K24 and K25
  (the ui lanes) are lanes of their own, G09 and H17 are rows of the ui catalogue (SY11, AP18), and N09 (files
  embedded in the program) is a language form, so it goes to a sitting of its own and not to this round. The
  other 37 are nine lanes (`gap-os`, `gap-service`, `gap-font`, `gap-pki`, `gap-image`, `gap-net`, `gap-mail`,
  `gap-data`, `gap-i18n`, step letters 107 to 115), the four `must` rows first (`coord/gap-lanes.md`). The
  catalogue's `could` rows and its 32 refusals stay as issues for after the landing. It turns the author's
  *we add nothing more* of the afternoon of 2026-11-22 by the author's own later word, as the audio and
  printing answer did.

## The author's answers of 2026-11-23, about 22:30: everything before the one push

Put as four questions with the picture of what stood before and after the push (`main` 867 commits past GitHub's,
five `blocking` defects open, 79 issues open), the author answered *1a 2b 3b 4a*:

- **The push waits** for the lanes then running (the Windows box's round, the three `blocking` defects 2522, 2578
  and 2583, the reach programs moved into `examples-rebel/reach/`) and for the last check on the exact tree pushed:
  the two-stage build and its fixpoint, the compiler's own tests, the net's own tests and the site's build; the whole
  net stays the CI's, as decided before.
- **The drawing area, its mouse and its keys (DR01 to DR03) on Win32 and GTK 4, and the month calendar (CT44) on the
  three systems, land before the push**, as the author's answer of 2026-11-22 afternoon already said of the ui
  catalogue. With them **AX04**, a drawing area a screen reader can read: the catalogue's own line makes it `must` the
  day DR01 lands, and DR01 landed on macOS at `015506dd5`, its area one image element.
- **The 19 `should` rows of `gap-3c-rows.tsv` never started, and the three delivered in part (K04's lossy WebP, F05's
  limit shared through the stores, H14's pending blocks), land before the push**, as the answer *3c* said. N09, files
  embedded in the program, stays a language form for a sitting of its own.
- **Decision 2610, the register of the decisions this milestone invalidates, closes at the milestone's close**, its
  dated comments standing as the register.

## The author's answers of 2026-11-23, about 23:00: where Rebel's folders stand, with three repositories in view

Asked whether `generators/` and `examples-rebel/` belong beside `packages/rebel`, the author answered, meant as:
*all of it inside one folder `rebel`; the index must not hold Rebel's files, only a pointer, a sort of link, saying
how to fetch them; and this is for the future, since the index may one day be a repository of its own, as
Homebrew's taps are, Rebel another with its generators, and Rebel's examples a third.* So, measured the same
evening before the answer (nothing of `examples-rebel`, `generators` or `packages/rebel` is on `origin/main` yet,
so no public history moves): the catalogue the specification states already has two entry forms, from an index
(`version`, `sum`) and from git (`git`, `tag`, `commit`, `sum`), so the pointer needs **no new form**, and the
compiler resolves Rebel through a `path` entry (`examples-rebel/reach/heroes.toml` since step 127a, at the top of
`examples-rebel/` that evening, 2026-11-23), the net, the site and the CI being
what reads `packages/rebel` (28 files). The git form names no subfolder (searched in the specification,
`docs/design.md` and `M-package-manager.md`, nothing found), so a git pointer needs the package at a repository's
root: while Rebel is a folder of this repository the entry is a `path`, and it becomes a git entry the day the
repository exists.

The layout, made **once, at the round's end, by a script, after every lane has merged**:

```
rebel/library/0.1.0/files/   the package, today packages/rebel/0.1.0/files
rebel/examples/              today examples-rebel, its reach programs in rebel/examples/reach/
rebel/generators/            today generators
packages/rebel/0.1.0/        the entry only: a catalogue entry, a path now, git, tag, commit and sum later
```

What the three future repositories ask of it, so that each folder can be lifted out as it stands: the folders
depend on one another **only through catalogue entries**, never by a relative path into a sibling; every place of
the net, the site and the CI that reads the library's folder reads it through one named constant, so a later split
changes one line; the one generator that reads the examples (`rebel_index`, by the Rebel CI's own comment) is the
seam, noted where it stands; and the instruments of the net that judge Rebel travel with Rebel when it leaves, the
compiler's own net using no library (CLAUDE.md § 13).

**Corrected the same night, about 23:20, by the coordinator's own reading, asked by the author to think it through**:
the block above says the pointer *needs no new form* and draws `packages/rebel/0.1.0/` holding *the entry only*. Both
are true of a program's catalogue and false of the index. An index is a protocol the compiler reads, ratified with
panel 235 (2026-11-07): `<index>/<name>/versions`, `<index>/<name>/<version>/list` and
`<index>/<name>/<version>/files/<path>`, every file at its own address and never rewritten once published
(`selfhost/fetch/index_source.hero`); and the site's build refuses any other file under `packages/` as a build error
(`checkIndex`, `site/src/lib/examples-model.ts`). So an index that holds a pointer in place of the files is a change
of that protocol, a sitting's, and belongs to M-package-index's opening one. Measured beside it: `packages/rebel/`
holds neither `versions` nor `list`, so it is a source tree in an index's shape and no index a client can read;
`index.heroes-lang.org`, the address a catalogue with no `index` line reads, resolves to nothing (`heroes add rebel`
in an empty folder: exit 2, *`curl` exited 6 ... Could not resolve host*), while the site and the rules page tell a
reader to type that command; `runtime/platform/`, the C floor under Rebel, is 59 files and 92,568 lines inside the
compiler's runtime, and the specification names no way for a package to carry C of its own (searched, nothing found:
a question, not a premise), so a repository of Rebel's own would hold its Heroes half alone.

**The author's answer to that reading, about 23:30, *1a 2a***, on two questions put with it:

- **The folders move, the index's protocol does not** (*1a*). At the round's end, once, by a script: the package's
  source to `rebel/library/0.1.0/files/`, the examples to `rebel/examples/` with their reach programs in
  `rebel/examples/reach/`, the generators to `rebel/generators/`, and **`packages/` leaves the tree** until
  M-package-index gives it an index a client can read. The reason outranks tidiness: the source every lane rewrites
  stood at the address of a published version, which the index's own rule says is never rewritten. The version folder
  stays in the source's path tonight, since taking it out changes how the net and the site find the newest version and
  not only where. The author's pointer, an index that says where a package is fetched from as Homebrew's taps do, is
  written on `M-package-index.md` as a variant its opening sitting measures. This supersedes, for where Rebel's source
  stands, the local static index at the repository's root of panel 235's R15 and panel 238's R10; the register
  (decision 2610) takes its line when the move lands.
- **The site and the rules page say what works on the day of the push** (*2a*): a program takes Rebel today by a
  `path` entry from a clone of this repository (`heroes add --path ... --as rebel`), and `heroes add rebel` by name
  waits for the index's row; no page tells a reader to type a command that exits 2.

## Decided on the fly by the coordinator, for the author to turn

- **The packages, in layers** (`examples-rebel/README.md` § A1): `io` holds
  the stream; files and directories are `fs`, apart from `os`, the process's
  world; URLs are `url`. **The layers are read from the code** (2026-11-16):
  the table had been written before the code and held twelve packages using
  one of their own layer or above; it was rewritten from the written
  packages' `use` lines, every layer one above its highest dependency, which
  also shows there is no cycle. A table written by hand ages; the final check
  recomputes it.
- **One handle composes the library.** Heroes has no interfaces, so a file, a
  pipe, a terminal, a child's ends and a socket are one `io.Stream`. In C a
  stream carries a table of operations, so a socket is a stream without a
  program that only reads files linking sockets.
- **A character is a one-character `str`**, what `chars()` yields; a code point
  is an `i64`.
- **The C stands in the platform layer** beside the runtime, new members and
  one extended: `hero_stream`, `hero_disk`, `hero_sys`, `hero_proc`,
  `hero_stream_net`, and additions to `hero_time` and `hero_net`. No door of
  `hero_os.h` changes its contract.
- **Rebel's examples are projects under `examples-rebel/`** (turned by the author on
  2026-11-21; the coordinator had decided one folder with one catalogue): the
  drivers read one catalogue at the folder's root, and each twin and each
  application is a folder with its own `heroes.toml`, whose `path` entry reads
  Rebel where it stands, so no project holds a copy of it
  (`examples-rebel/README.md` § A10).
- **Rebel is written from behaviour, never from another library's source**, and
  copies no test table; its expected values are observed from the three
  libraries on this machine. The three files of `core` that say they follow
  another library's source are not carried over.
- **One sitting, 253**, small and in the background, on the two things the
  contract asks a sitting for: `docs/design.md` §1.11's sentence and the verb
  `heroes doc`.
- **The landing is one push**, since the CI runs on pushes to `main` alone and
  a push there that touches `examples/`, `packages/`, `selfhost/` or
  `runtime/` publishes the site.

Decided on 2026-11-16:

- **A handle of the platform layer carries its table's tag** in bits 58 to
  61 (`runtime/platform/hero_handle_tag.h`). Each member numbered its slots
  alone, so a socket and a stream could hold the same bits, and the runtime's
  set of ended handles, which keeps an address with no type, refused a correct
  program (two accepted connections closed, then a read: exit 134). Found
  twice, by `http`'s and by `poll`'s tests. A value of another table's tag
  answers `NOT_OPEN`, as a handle the table never issued. The general case,
  any C library that hands out numbers as handles, is the compiler's: the set
  should key by type and address, a change of the ABI, filed apart.
- **The layer allocates through `parts/alloc.c` alone**, by four entry points
  declared in `runtime/platform/hero_layer_alloc.h` and not in
  `heroes_runtime.h`, whose every change moves the ABI and every emission;
  process-wide, since a stream is closed by the thread that leaves it last.
- **No switch turns a certificate's check off**: not in `http`, not in
  `mail/smtp`, not in `sql/postgres`. `tls`'s unchecked mode is for its own
  tests. So `sql/postgres` offers `disable`, `verify-full` (the default) and
  PostgreSQL 17's direct TLS, and refuses `allow`, `prefer`, `require` and
  `verify-ca`, each with its reason in its ledger: a peer never checked is an
  open door to whoever sits between, and `prefer` falls back to plain text in
  silence.
- **macOS's TLS after a plaintext preamble** (PostgreSQL's SSLRequest, SMTP's
  STARTTLS) by an `nw_framer` below TLS inside one Network.framework
  connection, measured to work before it was written; Secure Transport is
  deprecated, a private framework is not used, and a TLS engine written in
  Heroes is refused, since its cryptography would carry no promise of
  constant time.
- **`protobuf` has no code generator**, which would be a second tool beside
  `heroes`: `.proto` files are read at run time, messages are values, and a
  record is moved in and out by checked field names.
- **`http2` stands beneath `http`** and knows nothing of it; `http` uses it
  where TLS's negotiation answers `h2`.
- **The system's cryptography is measured before it is bound**: what a
  platform does not offer through a public, non-deprecated C API stays pending
  there, with the measurement.
- **The Mac's limits**: five agents, two test runs at once, a test program
  past 1 GB a defect (one of `toml`'s grew to 7 GB and was killed), raw
  binary never printed into an agent's output (an API filter stopped one).

## The method

The rules in one page before any package (`examples-rebel/README.md`); every
face with its signatures and its bodies as `???` before any body; then every
body and every test in parallel, one agent a package, in one worktree, the
coordinator committing by path. While they write, `heroes check` alone; what
compiles C waits its turn. The whole net twice: over batch 39's round, which
the closed session left half-gated, and once over everything at the end.

## What it measures about the thesis

Every `heroes check` and `heroes test` of the build is recorded as the
compiler said it. From them: how many modules pass their first check
(design.md Part 11's first-try rate), how many turns to a clean check and to
green tests, and the mistakes the compiler did not refuse, found by the tests,
by the comparison with the three libraries, by the gate or by the CI. It is an
observation and not an experiment: no control arm, one family of models,
writing with the specification and the guide in front of them.

**The prediction, registered before the first body was written**, 2026-11-15:
of the modules written, at least two in three pass their first check; no
mistake is left at the gate that a compile error could have told; and the
mistakes the compiler does not refuse are of three kinds above all, a slice
that splits a character, an overflow where another language wraps, and a wrong
edge case of an algorithm. The first two data came from the guide's own
snippets, the coordinator's: `c >= "0"` refused at the first check
(`bad_operand`), and a search that cut a slice at every byte, accepted by the
checker and aborting on `caffè` when run.

## The work as it stands, 2026-11-19 evening

Read from `git log` of the branch `rebel-round`, which opened from `main` at `f6c49d84c` (batch
39's round), and of the lanes running beside the landing; every hash is the commit's own.

- **Rebel, steps 3 to 53** (`89a33de42`, 2026-11-17 09:43, to `5c5e6f811`, 2026-11-19 11:31; no
  step 39 was committed): the packages, the platform layer under them in `runtime/platform/`, the
  rules page `examples-rebel/README.md` and this page; step 40 repaired what the Windows box found.
- **The three sittings** of decision 33, ratified as proposed by the author's word of about 13:10
  on 2026-11-19: panel 251 (A, decision 2590), panel 252 (B, decision 2599) and panel 253 (C,
  decision 2595).
- **Steps 54a to 54e**: panel 251's R1 and R2 and defects 2522, 2552, 2582, 2587 and 2588 (lane
  `lane-251`, merged at `06a2ecfbb`).
- **Steps 55a to 55g**: the compiler's home without a library, the net's string predicates its own,
  the examples carrying their own functions, the goldens' fixture package renamed, the hooks naming
  no `core`, and the static index's `core` removed whole at `7ddb4ed74` (lane `lane-core`, merged at `90bfb5252`).
- **Steps 56a to 56i**: the coherence review's instruments, the `rebel` suite, and its repairs
  (lane `lane-review`, merged at `b7a43bf18`); its nineteen decisions are above.
- **Steps 57a to 57e**: five Windows reds of step 53's sweep (lane `lane-win`, merged at
  `f7fa352f3`).
- **Committed on the implementation lanes and not merged at this writing**: steps 58a to 58e
  (`lane-impl-b`): a print span ends with its thread, so `exit` no longer waits on a lock nobody
  holds, `e8b13be45`, the hang filed as defect 2605 on 2026-11-19 (20:15 UTC by GitHub); modules
  may form a ring (panel 252's R1); a literal's typed part settles its letter (R2, defect 2596);
  spec § 11 names C's `strtod` (R5); containers refuse a list that is not their own (R3, defect
  2597). Steps 59a to 59c (`lane-impl-doc`): `heroes doc`, its golden forms `doc` and
  `doc_refused`, and the compiler's own tests counting it (panel 253's R2). Steps 60a and 60b
  (`lane-impl-rt`): defects 2602 and 2600. `lane-impl-skip` (panel 253's R4) had committed nothing.
- **The landing's lanes, steps 62 to 67**: the contract's and design's texts, the chain, the
  register and the final check (step 63), the site, Rebel's own pipeline, panel 253's R6, and the
  reds the branch carries at its base. Then the three owed steps above, one at a time.

**The register of the decisions this milestone invalidates** is one `decision` issue with dated
comments (decision 10, panel 253's R1), https://github.com/heroes-lang/heroes/issues/2610; this page carries no list of it. The records
that held a superseded decision each end with a dated note pointing at it.

