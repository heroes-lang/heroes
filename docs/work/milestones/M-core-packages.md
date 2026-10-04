# M-core-packages — small packages that compose, and what a web server needs


**Scheduled, no warrant** (author instruction 2026-09-03; § Who scheduled what
has the words). It adds **no language form and no built-in**: panel 097
condition 5 keeps `selfhost/library_source.hero` closed, and panel 057 refused
`path_join` there on Principle 0. So Principle 0 binds nothing in this milestone;
what binds it is §1.11, and where its line falls for a package written in Heroes
alone is the first thing the opening sitting rules.

**What it delivers**: ordinary Heroes modules, distributable, organised as Go's
tree and reached by the `use` path form M-package-layout landed — `use net/http`
binds `http`, `use encoding/json` binds `json`, `as` renames (`spec:10-13`). Each
package is a directory with `test` blocks; a package over C is two modules, the
raw `extern` group and the idiomatic wrapper (panel 033 R5: an extern is never
callable across a module boundary). The list, with each package's source, was
measured on 2026-09-03 with probes on the Mac and in the Linux image; Windows was
not measured:

| package | source | today |
|---|---|---|
| `strings`, `path`, `net/url`, `log`, `sort` (generic — the built-in refuses a type parameter), `rand` (deterministic) | Heroes alone | writable |
| `strconv` — **joined 2026-09-10**: `to_i64` does not take a `str`, so six corpus programs build numbers digit by digit, and width and precision have no route either. The count and the return condition are its `docs/work/SCHEDULED.md` item's | Heroes alone | writable |
| `encoding/json` | Heroes alone — `examples/json/`, 671 lines, already parses and renders | writable |
| `html/template` | Heroes alone — `examples/template/main.hero`, 235 lines, plus escaping | writable |
| `os` | `hero_os.h` and `getenv` — `selfhost/cli/process.hero:37-56` lifted out of the compiler | writable |
| `io`, `bufio` | `stdio.h` (`FILE*`); bulk text through `hero_str_from_bytes(p: ptr, n: i64)`, exported by panel 035 | writable, POSIX |
| `bytes` | Heroes plus `fmemopen`/`fgetc`, one C call per byte, until the sitting picks a route | writable, slow, POSIX |
| `time` | `time.h`: `time`, `timespec_get`, `strftime`, `gmtime_r`, `nanosleep` | writable, POSIX; `tv_nsec: i64` unmeasured on Windows |
| `math` | `math.h` `link "m"` | writable |
| `database/sql` | `sqlite3.h`, prepare/step; blobs through `bytes` | writable |
| `crypto/sha256`, `crypto/hmac` | `package "libcrypto"` | writable, Mac and Linux |
| `net/http` client | libcurl, two modules for `curl_easy_setopt`'s two value types (panel 094 R4) | writable |
| `net` | a runtime part, or one file per platform — the sitting's questions (iv) and (vi) | per platform only |
| `net/http` server | Heroes alone over `net`, `io`, `strings` | writable, one thread |

**Measured before it was scheduled, which is why it could be**: a single-threaded
HTTP server written in Heroes answered `curl` on 2026-09-03 with no language
change — `socket`, `bind`, `listen` and `accept` as `extern`s, the address as
`record SockAddr tag sockaddr` with the port's two bytes computed in Heroes (no
`htons`), the request read through `fdopen` and `fgets`, the response through
`write`; about 100 lines, and the same program on Linux with three lines of the
record changed (`sa_len` does not exist there, `sa_family` is `u16`). None of the
three gaps the brief expected was needed: not a byte buffer, not `htons`, not
`sockaddr_in`. And the callback boundary — a function value cannot cross the FFI,
`selfhost/check/ffi.hero:45` — is needed by **nothing on the list**: `sqlite3_exec`
is avoided by prepare/step, libcurl writes into `CURLOPT_WRITEDATA` with no
`CURLOPT_WRITEFUNCTION` (measured: a Heroes client fetched from the Heroes server),
`qsort` is the built-in `sort`. It is needed by threads and by event-driven server
libraries, so it is M-isolated-threads' question and is filed there.

**What the opening sitting rules, full five seats, before any step**: (i) the
§1.11 boundary for a package written in Heroes alone — the sentence in the entry
above, and the falsifier the answer owes (CLAUDE.md §12); (ii) where packages live
and how `use` reaches them — measured: a module that `use`s a sibling package
compiles only from the program's root, because `use` cannot climb (panel 099 R1),
so `heroes fetch` places a tree under the root and each tree wants a root-level
test driver; (iii) the byte buffer, three routes with their cost — per-byte
runtime entries (soundness lane, ABI +1), a `[u8]` result admitted for a group
over `heroes_runtime.h` (`ffi.hero:45` refuses `.array` today; a diagnostic and a
§4.19 sentence change), or a built-in (the route panel 036 refused for
`read_file`); (iv) `net` as a runtime part, because `sockaddr` differs between
Darwin and glibc — `error[ffi_field_type]` on the other platform, measured both
ways — and Windows is winsock: panel 097's `struct stat` shape, touching §1.11's
own row *Sockets: libc*; (v) Part 7 item 10 widened from `long` and `size_t` to a
typedef whose width **or sign** differs by platform, on the measurement that
`clockid_t` is `u32` on Darwin and `i32` on glibc, so `clock_gettime` has no
single spelling and `timespec_get` is the portable clock; (vi) **conditional
compilation** — the five shapes other languages use: C's textual `#if`; a
compile-time keyword in the body (Nim's `when`, D's `version`, Odin's `when`,
Swift's `#if`), whose inactive branch is not type-checked on this machine; Rust's
`cfg` attributes, likewise; one file per platform (Go's `net_linux.go`, Odin's
`_linux.odin`, Hare's `+linux`), every file a whole module checked on its platform
and no word in the body; and nothing in the language (Ada, Oberon), which is
where Heroes stands with the arm in `runtime/parts/`. The record the seats are
handed: panel 049 refused the platform axis with a veto, ratified 2026-08-14;
panel 097 put the arm in the runtime; panel 039 left comptime unplaced; §4.15
makes every textual difference semantic; and CI compares `seed/heroes.c` byte for
byte against what the compiler emits on every leg, so emission that depends on
the host breaks an instrument. The limit the sitting must name: the runtime is
the only C a package can add to, since panel 036 P2 vetoed `compile "shim.c"` —
so the shape Heroes has today serves the project's packages and nobody else's. A
refusal of any shape lands as a Part 6 row with its falsifier; (vii) **the shape
rule for compiler marks** (author question 2026-09-06, the `owned fclose`
observation that opened M-reflection-verdict's question (iii); the two halves are
homed apart because `docs/work/SCHEDULED.md (retired 2026-09-12)`'s second-contextual-word item already
sits here, moved from M-declared-freer on 2026-09-07). The language has six words
a program writes after the thing they modify and the compiler reads — `owned`,
`tag`, `partial`, `link`, `package`, `as` — every one a contextual word matched by
its text in `selfhost/parse/` and none in `selfhost/keywords.hero`'s table of 21,
every one gated to one position, every one carrying a check: `owned` types the
probe `char *` and refuses the program's own call of the freer, `partial` refuses
`==` and the map key, `tag` is matched against the header's struct. The rule that
shape obeys is on the record three times and as a rule nowhere: panel 094 R3
(*"a second notation for one idea"*, refused), panel 109 (*"a contextual word,
never a keyword"*), panel 114 R7 (*"closed and enumerated in the spec"*, and *"a
word, not `$if`"*). The question is whether design.md §4.19 names the family — a
closed set, each word in the spec, after the thing it modifies, carrying a check
— so that the buffer case M-declared-freer queued, panel 003's discardable mark
and the `tag` for a colliding symbol this milestone's own item asks about follow
it without a fourth re-derivation; and what a general annotation syntax would buy
against it, which is CLAUDE.md §9's bill paid once and no more than that.

**Two questions joined the seven on 2026-09-10**, from the session that wrote
§ What production-ready means, and both bite here because this milestone writes
eighteen groups over real headers.

**(viii) Is a group's header the authority for its own declarations, or is the
module's header set?** `spec:224-227` reads per group. The compiler answers per
**module**, and its own source is the witness: `selfhost/cli/process.hero:49-53`
declares three `hero_os.h` functions inside `extern "stdlib.h"` and compiles,
because a translation unit includes every group's header
(`selfhost/emit/unit.hero:32`) and clang sees the union. **It is not a defect** —
a wrong header alone is refused, and `selfhost/emit/ffi_declared.hero:100-137`
has translated clang's three spellings of *undeclared* since 2026-08-25 — but it
is a refusal the spec promises per group and the compiler makes per module, whose
cost is that deleting an unrelated group breaks a binding at a distance. Either
the spec narrows to what runs, or the check tightens and `process.hero` is the
first thing it refuses.

**(ix) How thick is the wrapper over a C group?** The author's question of
2026-09-10, and **half of it is already answered**: a package over C is two
modules by construction, since panel 033 R5 makes an `extern` uncallable across a
module boundary. What is open is the thickness over eighteen packages — **thin**,
one-to-one with C and the argument order showing through, against **thick**, an
idiomatic API where the `extern` never appears and the one thing 1.0 can promise,
a C library clang checks, stops being visible. **The precedent for thin is PHP,
in two authoritative facts**: php.net's manual calls `mysqli` and `PDO_MySQL`
*"lightweight wrappers on top of a C client library"*, and the language proposed
to repair the result from inside at `wiki.php.net/rfc/consistent_function_names`.
Its observable cost is one line — `strpos(haystack, needle)` against
`in_array(needle, haystack)` — which is the shape §1.1 exists to delete. **What
is commentary rather than authority**, and so goes to the historian as a
hypothesis (CL-018), is that following each C library's conventions is what
*caused* it. **And one asymmetry favours Heroes**: PHP mitigated this with named
arguments in 8.0, in 2020, and `spec:113` has required them wherever two
parameters share a type since before any package existed — so the argument-order
half of that bill is paid and the naming half is not.

**Step order** — one package or one gap per step, each step a corpus program on
the three platforms or skipped by the missing-header rule: **0** three repairs
the probes found, each a `fixedbugs` case — `xs @ xs.push(c.to_u8().must())`
copies the whole array per push (100,000 pushes in 14.78 s against 0.00 s with
the value bound first, this Mac, 2026-09-03; `selfhost/ir/place_store.hero:100`
chooses `.push_owned` for a bound value and not for an inline `.must()`),
`@value: i32` against a `const void *` pointee is `internal error` at exit 2
(`setsockopt`; panel 103's pointee assertion writes `sizeof(const void)`), and
`ffi_unknown_name` for the macro-only `htons` on Darwin says the header declares
no such name when it does · **1** `strings` and `path`, and the copies leave
`examples/` — `split_lines` in 8 files, `is_space` in 8, `trimmed` in 7,
`index_of` in 4, and `tests/harness/strings.hero` carrying a fourth `trimmed`,
**and `strconv`, which joined the table on 2026-09-10** · **2** `net`, in the
sitting's shape (the loopback program, as an `echo` program under `examples/`) ·
**3** `io` and `bufio` (M-corpus-depth's `wc` over stdin is the witness if it has
landed) · **4** `encoding/json` · **5** `net/http` client, against a `file://`
URL · **6** `net/http` server, two requests on one connection · **7**
`database/sql` (M-corpus-depth's `ledger/`) · **8** `os` · **9** `time` · **10**
`math` · **11** the byte buffer, in the sitting's route (M-corpus-depth's
`checksum/` is the witness) · **12** `crypto` · **13** `net/url`,
`html/template`, `log`, `sort`, `rand` · **14** — **moved to
M-package-manager on 2026-09-03**, when the packages were placed ahead of the
manager: `heroes fetch` under the root, the root-level driver, and the `heroes
check` item panel 091 filed there. The packages live as directories under
`examples/`, reached by `use` from a program in the same root — the rule for this
milestone, not a stopgap until `fetch` exists. **That last step is cited as
step 13 by the record and by `docs/work/SCHEDULED.md (retired 2026-09-12)`**, which is what it was
until 2026-09-10; it is 14 here because the order below moved and the record is
not rewritten.

**The order above was reversed at the top on 2026-09-10 by author decision.**
The author asked for a step before M-web-framework delivering mini packages,
JSON, an HTTP client and a small HTTP server in Go's shape; **that step is this
one and it already stood there**, so nothing was added — what the question
exposed is that the web stack sat at the **end**, `net` at 10 and the server at
12, so the first nine steps produced nothing anybody could watch answer a
request. **It costs no dependency, measured rather than argued**: the 100-line
loopback server of 2026-09-03 needed none of the byte buffer, `htons` or
`sockaddr_in`. **One dependency was corrected while the order was written**:
`io` and `bufio` sit at step 3, ahead of the server, because the table above
says the server is Heroes alone over `net`, `io`, `strings` while that program
read its request through `fdopen` and `fgets` directly.

**Acceptance**: a small HTTP server built from the packages and nothing else, in
`examples/`, in the corpus's three configurations, in the loopback shape — server
and client in one process, deterministic output — because a listening server does
not fit `main.expected`.
