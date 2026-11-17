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
`examples/` use it (35 `use` lines in 23 files); 115 modules of the net write
`use <core>/strings as core_strings` and call three functions of it
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
    bit changed refused (rebel-scratch/swiftprobe/). So CryptoKit is
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
- **Rebel's examples are one folder, `examples-rebel/`, with one catalogue**:
  one copy of Rebel for the whole group, where the nineteen programs that used
  `core` held one each.
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
