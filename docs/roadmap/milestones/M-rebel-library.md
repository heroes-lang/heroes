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
    needs), `diff`, `markdown`, and `crypto/seal` and `crypto/keys` from the
    system's own cryptography (lifting `crypto`'s refusal of ciphers by the
    route `tls` took). Rebel is 71 packages.

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
