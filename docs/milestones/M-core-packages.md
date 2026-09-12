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

*******************************************************************************
**OPEN: 14**

- [ ] **M-core-packages** | the opening sitting, full five seats, and no step lands before it | `design.md` §1.11, §4.15, §4.19, Part 6 · `docs/panel/028`, `032`, `036`, `039`, `049`, `097`, `099`

    **Origin:** author instruction 2026-09-03, out of the reasoning session
    recorded in `DESIGN-LOG.md:537`; the sixth question added by the author the
    same night. Put ahead of M-package-manager by the reorder late on
    2026-09-03, `DESIGN-LOG.md:539`.

    **Six questions, each measured before it is asked.** (i) The §1.11 boundary
    for a package written in Heroes alone: the ROADMAP's own test — *a binding
    is verified by clang against the header it names, and a standard library is
    verified by whoever wrote it* — puts a pure-Heroes `strings` on the second
    side, and the answer owes a falsifier (CLAUDE.md §12). (ii) Where packages
    live and how `use` reaches them: measured, `heroes test
    pkg/strings/strings.hero` runs a leaf's tests alone, but a module that
    `use`s a sibling package compiles only from the program's root (`use` cannot
    climb, panel 099 R1), so `heroes fetch` places a tree under the root and
    each tree wants a root-level driver — panel 032 R6 made concrete, with panel
    028 R3 keeping anything from being searched at run time. (iii) The byte
    buffer: a `ptr` of known length already becomes a `str` through
    `hero_str_from_bytes` (10 MB in 0.32 s), but `[u8]` is `error[ffi_type]` at
    check (`selfhost/check/ffi.hero:45`); three routes — per-byte runtime
    entries (soundness lane, ABI +1), a `[u8]` result admitted for a group over
    `heroes_runtime.h` (a diagnostic and a §4.19 sentence change, full lane), or
    a built-in (the route panel 036 refused for `read_file`). (iv) `net` as a
    runtime part: `sockaddr` differs between Darwin and glibc —
    `error[ffi_field_type]` on the other platform, measured both ways — and
    Windows is winsock; panel 097's `struct stat` shape, touching §1.11's row
    *Sockets: libc*. (v) Part 7 item 10 widened to a typedef whose width **or
    sign** differs by platform: `clockid_t` is `u32` on Darwin and `i32` on
    glibc, so `clock_gettime` has no single spelling and `timespec_get` is the
    portable clock. (vi) **Conditional compilation** — five shapes: C's textual
    `#if`; a compile-time keyword in the body (Nim `when`, D `version`, Odin
    `when`, Swift `#if`), whose inactive branch is not type-checked on this
    machine; Rust's `cfg` attributes, likewise; one file per platform (Go
    `net_linux.go`, Odin `_linux.odin`, Hare `+linux`), every file a whole
    module checked on its platform and no word in the body; and nothing in the
    language (Ada, Oberon), which is where Heroes stands.

    The record to hand the seats: panel 049 refused the platform axis with a
    veto (*"a platform question belongs where it is a measurable fact about the
    machine"*), panel 097 put the arm in `runtime/parts/`, panel 039 left
    comptime unplaced, §4.15 makes a textual difference semantic, and CI `cmp`s
    `seed/heroes.c` against what the compiler emits on every leg
    (`.github/workflows/ci.yml:515-520`), so host-dependent emission breaks an
    instrument. The limit to name: the runtime is the only C a package can add
    to (panel 036 P2 vetoed `compile "shim.c"`), so shape five serves the
    project's packages and nobody else's. **Not on the list**: the callback
    boundary — measured, no package needs it; it is M-isolated-threads' item
    below.

    **Where to look also:** `design.md` Part 7 item 10 ·
    `selfhost/check/ffi.hero:45` · `selfhost/modules.hero:125` ·
    `runtime/heroes_runtime.h`.
    **Why it matters:** a server cannot be distributed in Heroes today, and
    every reason is a compiler fact rather than a missing library.

    **Re-verified 2026-09-10: STILL OPEN, no sitting held** (`docs/panel/` ends
    at 125), **and it gained two questions.** `docs/ROADMAP.md` § M-core-packages now
    carries **(viii)** whether a group's header is the authority for its own
    declarations or the module's header set is — the compiler's own
    `selfhost/cli/process.hero:49-53` declares three `hero_os.h` functions under
    `extern "stdlib.h"` and compiles, because a TU includes every group's header
    (`selfhost/emit/unit.hero:32`) — and **(ix)** how thick a wrapper over a C group
    is, the author's question of 2026-09-10, whose already-settled half is panel 033
    R5: the wrapper is mandatory, not a matter of taste. **Two pointers moved**: the
    `[u8]` refusal is `selfhost/check/ffi.hero:89`, not `:45`, and the seed `cmp` is
    `.github/workflows/ci.yml:689-693`, not `:515-520` — the item's *"on every leg"*
    is correct, since that step sits in the single matrix job with `fail-fast:
    false`.

- [ ] **M-core-packages** | four repairs to design.md that ride its opening sitting, §4.19's `#include` sentence among them | `design.md` §1.11, §3.5, §4.19, §4.20, Part 7 item 4 · `selfhost/emit/externs.hero:69-70` · `docs/panel/056`, `091`

    **Merged 2026-09-10** from two items, by author instruction: both are repairs
    to design.md, both ride the same opening sitting, and one editing pass closes
    all four. **Both bodies are kept whole below.**

    **Re-verified 2026-09-10: STILL OPEN, all four owed, and every pointer in the
    second body has moved.** `grep -n "declaration order" design.md` is **0**, so
    the `#include` invariant is still written only in code, at
    `selfhost/emit/externs.hero:69-70` (the item said `:72`, which is now the
    `hero_os.h` seed line of that same doc comment). `args_checked` appears **0**
    times in design.md and `validated` only outside §1.11 and §4.20, so both are
    still omitted; they are `spec/heroes-spec.md:204-205`, not `spec:186-190`.
    *"no aliases, no package hierarchy"* still stands at `design.md:2641`, and
    §3.5 still holds no paragraph naming what would return a project file.
    **Three pointers**: §1.11's Tier-2 list is `design.md:493`, §4.20 opens at
    `:2266`, Part 7 item 4 is `:2641`. **The second body's own evidence sentence is
    now false**: *"`grep -n unplaced design.md` hits only `:2407`"* — it hits
    `:2566`, `:2584`, `:2617` and `:2712`, and `:2407` is not among them.
    **And the `#include` sentence has to say more than it did**: the list now
    carries two unconditional seeds, `math.h` and `hero_os.h`
    (`selfhost/emit/externs.hero:76`), so *the order groups are declared in is the
    order of the includes* is true only after those two.

    **A FIFTH repair was found while verifying and it belongs here**, same class as
    the second: `design.md:806` (§4.1) still says *"there are no aliases and no
    wildcard"*, which `spec/heroes-spec.md:11` contradicts — `use syntax/decl as
    sd` has bound an alias since M-package-layout closed on 2026-09-02. The
    sentence to repair is design.md's, since §12 gives the spec precedence.

    **The first item, as it stood.** §4.19 owes one sentence: a group's `#include` order is load-bearing

    **Origin:** panel 091, the ffi-pragmatist's explicit *not covered by
    design.md*. Its home since 2026-09-07, when M-declared-freer closed without
    taking it. It named that milestone from 2026-09-04 on the ground that panel
    109 amends §4.19 and this is a §4.19 sentence — true, and the sitting ruled
    on ownership and never on the `#include` list, so the sentence stayed
    unwritten. It rides M-core-packages' opening sitting instead, which touches
    §1.11, §4.19 and §4.15 by its own six questions, and where a later item
    already parks three other design.md repairs for the same reason. The lesson
    is this file's own: *the next sitting that touches X* is a waiting condition
    and not a home, and naming a milestone did not fix that — what fixes it is
    naming a sitting whose AGENDA contains the question.

    Measured: `<jpeglib.h>` alone is 8 errors (`unknown type name 'size_t'`);
    `<stdio.h>` first, then clean. So the order in which groups are declared
    **is** the order of the `#include` list (`emit_externs.headers`, "in
    declaration order"), and it is the author's only lever on it. design.md
    §4.19 does not say so, which means nothing stops a later pass from
    reordering or thinning that list — and the sitting that would do it would be
    reasoning from a document that never mentioned the constraint. The sentence
    is owed whether or not anything is ever pruned; it is CLAUDE.md §11's
    expiring premise before it expires. Amending design.md Parts 1-11 is a panel
    path (CLAUDE.md §4), so this rides the next sitting that touches emission
    rather than convening one.

    **Where to look also:** `docs/panel/091` § What the ffi-pragmatist compiled.
    **Why it matters:** an invariant nobody wrote down is one somebody will
    optimise away.

    **The second item, as it stood.** three repairs to design.md that ride its opening sitting

    **Origin:** found 2026-09-03 reading forward (CLAUDE.md §1). They ride the
    opening sitting because that sitting touches §1.11 anyway.

    **(a)** §1.11's Tier-2 list (`design.md:474-478`) and §4.20's inventory
    (`:2273-2276`) omit `validated` and `args_checked`, landed 2026-08-24
    (`DESIGN-LOG:408`, `:413`); the spec has them (`spec:186-190`) and
    `suite_spec` polices the spec, not design.md. **(b)** Part 7 item 4
    (`design.md:2447-2449`) still reads *"no aliases, no package hierarchy"*
    after M-package-layout landed `use syntax/decl as sd` on 2026-09-02; `grep
    -n 'panel 099\|panel 100\|panel 101' design.md` is 0. **(c)** Panel 056
    deliverable B — *"a greppable paragraph in design.md §3.5"* naming what
    would return a project file — was ratified 2026-08-15 and never written:
    `grep -n unplaced design.md` hits only `:2407`, the comptime paragraph.

    All three are design.md Parts 1–11, so none is written without a sitting
    (CLAUDE.md §4); deliverable D, the same conditions in the ROADMAP's
    M-package-manager entry, was discharged 2026-09-03.

    **Why it matters:** a document that omits what shipped briefs the next
    sitting wrong.

- [ ] **M-core-packages** step 0 | four repairs before the first package, each a `fixedbugs` case, the macro-only probe among them | `selfhost/ir/place_store.hero:100` · `selfhost/cli/pointee.hero` · `selfhost/emit/extern_probe.hero:164-165` · `docs/panel/092`, `103`

    **Merged 2026-09-10** from two items, by author instruction, and the second
    named the first as its own witness: *"the macro-only item panel 092 filed above
    gains its witness"*. Same milestone, same step, same sitting, same file — four
    repairs and four `fixedbugs` cases, in one item. **Both bodies are kept whole
    below.**

    **Re-verified 2026-09-10: all four STILL OPEN, and no case exists for any of
    them.** Of **116** `fixedbugs-*` files under `tests/golden/`, none matches
    push, pointee, `htons` or `setsockopt`, and `grep -rln "htons\|htonl\|isascii"
    tests/ examples/` is empty. The `place_store` pointer is exact —
    `selfhost/ir/place_store.hero:100` still chooses `.push_owned` for a bound
    value — and `selfhost/cli/pointee.hero` (310 lines) still has no `const void`
    arm. The macro-only defect is stated in the emitter's own comment at
    `selfhost/emit/extern_probe.hero:164-165`: *"`(htonl)(a0)` is `use of
    undeclared identifier`, and so is `(&htonl)`"* — the item's `:153` lands inside
    that same block. **Two numbers were deliberately not re-run**: the 14.78 s
    against 0.00 s pair is a build timing (CL-025), and *"44 distinct C functions
    are probed by the corpus"* needs a build to reproduce. The *"none is
    macro-only"* half re-measures cheaply and still holds.

    **The first item, as it stood.** a macro-only C function has no golden case, and the parenthesised probe breaks it

    **Origin:** panel 092, the compiler-engineer's own condition 3. Its home
    since 2026-09-04: this item said *the next milestone that touches the FFI
    probe or the golden corpus* and named none, and step 0's repair (c) is
    already this item's own witness — `htons` on Darwin is defined only as a
    macro and the parenthesised probe reports it as *declares no `htons`*, which
    is false.

    `(htonl)(a0)` is `use of undeclared identifier 'htonl'` — and so is
    `(&htonl)`, so the alternative spelling does not save it. §4.19 promises
    *"Macros and `inline` functions are reachable"*, and the repair that closed
    the `_FORTIFY_SOURCE` hole narrows that promise for a name the header
    defines **only** as a macro. **Measured: 44 distinct C functions are probed
    by the corpus and none is macro-only**, so nothing fires today — which is
    exactly why it needs a case rather than a fix. The case is one `extern`
    group over a macro-only name (`htonl`, `isascii`, `major`/`minor` from
    `sys/types.h`), and the decision it forces is whether the emitter falls back
    to the unparenthesised form when the parenthesised one fails to compile —
    which cannot be asked of clang in one pass.

    **Where to look also:** `docs/panel/092` § Where the seats disagreed.
    **Why it matters:** a promise in the document with no case in the harness is
    a promise nobody will notice breaking.

    **The second item, as it stood.** three repairs before the first package, each a `fixedbugs` case

    **Origin:** found by the session's probes, 2026-09-03, on the Mac and in the
    Linux image.

    **(a)** `xs @ xs.push(c.to_u8().must())` copies the whole array on every
    push: 100,000 pushes in **14.78 s** against **0.00 s** when the value is
    bound first (`v = c.to_u8().must()` then `xs @ xs.push(v)`), `heroes
    build`'s default optimisation, this Mac; `selfhost/ir/place_store.hero:100`
    chooses `.push_owned` for a bound value and not for an inline `.must()`, so
    the runtime's copying `push` runs. Every byte-oriented package writes
    exactly this shape. **(b)** `@value: i32` against a `const void *` pointee
    (`setsockopt`) is `internal error: checking what the extern out-parameters
    point at failed` at exit 2 on both platforms — panel 103's pointee assertion
    writes `_Static_assert(sizeof(const void) == …)`, a legal author declaration
    blamed on the compiler; `SO_REUSEADDR` is what a restarted server needs.
    **(c)** `htons` on Darwin is defined only as a macro (`sys/_endian.h`, the
    prototype under `#if defined(lint)`), and the parenthesised probe's failure
    is reported as `ffi_unknown_name`, *"declares no `htons`"*, which is false —
    the macro-only item panel 092 filed above gains its witness; glibc declares
    the function beside the macro, so the same `.hero` runs on Linux and prints
    36895.

    **Where to look also:** `selfhost/emit/extern_probe.hero:153`.
    **Why it matters:** a package built on a quadratic `push` and an
    out-parameter the compiler cannot describe fails on its first real input.

- [ ] **M-core-packages** step 1 | `strings` is the first package because the corpus already wrote it | `examples/` · `tests/harness/strings.hero` · `docs/panel/057`, `097`

    **Origin:** measured 2026-09-03.

    **Four helpers copied by hand across `examples/`, sharable by nothing**:
    `function split_lines(` in 8 files, `is_space` in 8, `trimmed` in 7,
    `index_of` in 4, and `tests/harness/strings.hero` carries `trimmed`,
    `is_space`, `lines`, `contains`, `starts_with` and `ends_with` once more.
    `use` cannot reach a module outside the program's root
    (`selfhost/modules.hero:125`), no search path exists by ruling (panel 028
    R3), and the two doors into the language are shut: panel 097 condition 5
    closes `selfhost/library_source.hero`, panel 057 refused `path_join` as a
    built-in on Principle 0. A package is the only home, and the copies are the
    measurement of what it must hold before anyone invents it.

    **Where to look also:** `grep -rl '^function split_lines(' examples` ·
    `spec` § Built-ins.
    **Why it matters:** the corpus has already written the package, seven or
    eight times, without a name.

    **Re-verified 2026-09-10: STILL OPEN, and every count is UNDERSTATED.** With
    the item's own command: `split_lines` **9** files (said 8), `is_space` **9**
    (said 8), `trimmed` **7** and `index_of` **4** (both exact). And
    `tests/harness/strings.hero` carries **14** functions, not the six listed —
    `ends_with, without_suffix, split_on, lines, base_name, trimmed, is_space,
    contains, starts_with, without_prefix, to_number, index_of, split_text, words`.
    **`strconv` joined this step's package table on 2026-09-10** and `to_number`
    above is the copy that proves why.

- [ ] **M-core-packages** | a C symbol spelled with a Heroes keyword cannot be bound at all | `selfhost/keywords.hero` · `docs/panel/094` R2, R3 · `design.md` §4.19

    **Origin:** panel 013's ffi-pragmatist, pre-existing and option-independent;
    carried on the panel watch list from 2026-08-03 until it was retired
    2026-09-04. Its home since 2026-09-07, when M-declared-freer closed without
    reaching it — and the reason is worth keeping, because it is the second time
    this item has outlived its address. Panel 109 opened §4.19's reserved
    annotation vocabulary and `owned` is the first word in it, which is why the
    item was homed there; what the sitting did NOT do is rule on a second
    contextual word, and Principle 0 gave it no reason to. **M-core-packages is
    where a reason appears**: a package that binds a real library is the first
    thing in this repository that can meet a C symbol spelled with a Heroes
    keyword, and that milestone's opening sitting already has §1.11 and §4.19 on
    its agenda. Its previous home read: the panel watch list deferred it to
    §4.19's deferred annotation vocabulary at the milestone now named
    M-ffi-ladder, an address that expired when that milestone closed 2026-08-12,
    and panel 109 is what opened that vocabulary — `design.md:2120-2125`
    reserved it, *"Reserve a keyword"*, and `owned <C function>` is the first
    word in it.

    **And §4.19 has no way to say another name for it.** Measured at panel 013
    over the macOS SDK and homebrew headers, in declarator or field position:
    `function` occurs **571** times, `func` 29, `assert` 11, `test` 5, `match`
    3 — and `selfhost/keywords.hero` reserves every one of them, so `extern
    function function(…)` has no spelling. **Re-measure the five counts at the
    opening rather than trusting these**: the headers on this machine have moved
    twice since.

    **What is NOT the answer, decided and on the record**: panel 094 refused a
    rename clause (ratified 2026-08-26) — but on a premise this case never
    reached, that *"one C symbol carries one arity"*, and its file contains
    **zero** occurrences of `reserved`, `collide` or `registry`, so the
    collision was never priced. R3 of that sitting binds the spelling if it ever
    lands: **`tag`, not `= "cname"`**, which is already the word an `extern`
    record uses for the C tag (`spec:231`) and is contextual, so it costs no
    keyword. The cheap question for the sitting is whether `owned`'s arrival
    makes a second contextual word in the same position free, or whether
    Principle 0 still holds this one out — nothing on the closure list binds a
    colliding symbol, and a binding nobody can write never appears in a corpus,
    so no trigger can ever fire for it.

    **And the same sitting names the shape rule the family already obeys**
    (author question 2026-09-06, M-core-packages' question (vii) in
    `docs/ROADMAP.md`, split from M-reflection-verdict's (iii) because the
    second-contextual-word question is here): six contextual words today —
    `owned`, `tag`, `partial`, `link`, `package`, `as` — each after the thing it
    modifies, each in one position, each carrying a check, none in
    `selfhost/keywords.hero`'s table of 21 (measured 2026-09-07 from
    `selfhost/parse/`); the rule stated by panels 094 R3, 109 and 114 R7
    separately and written as a rule nowhere; whether design.md §4.19 names it,
    so that a `tag` for a colliding symbol, the buffer case M-declared-freer
    queued and panel 003's discardable mark follow it without a fourth
    re-derivation.

    **Where to look also:** `docs/panel/013-function-type-marker.md:179-184` ·
    `docs/panel/114-the-question-was-not-which-platform.md:244-252` ·
    `design.md` §4.19, `:2120-2125` (`:2139-2144` today) ·
    `selfhost/parse/members.hero:81-94` · `spec:231`.
    **Why it matters:** §1.11 says everything comes from C, and this is the one
    class of C name the language cannot reach — the hole is invisible because
    the program that would find it cannot be written.

    **Re-verified 2026-09-10: STILL OPEN, and both of its measured negatives
    hold exactly.** `selfhost/keywords.hero:35-57` is the closed table at **21**
    words and none of the six contextual words is in it; over `docs/panel/094`,
    `reserved`, `collide` and `registry` are still **0**, **0** and **0**. Two
    pointers moved: design.md's *"Reserve a keyword"* is `:2178`, and the `tag`
    sentence is `spec/heroes-spec.md:254-255` where the item says `spec:231`, which is
    now a fence. **The five SDK counts were deliberately not re-run**: they need a
    declarator scan of the macOS SDK plus the Homebrew headers, which the item itself
    asks for at the opening.

- [ ] **M-core-packages** | copy-on-write's `refcount == 1` is a test and then a mutate, and nobody could race it | `runtime/parts/cow.c:44`, `:78`, `:83` · `runtime/parts/map-write.c:127` · `docs/panel/113`

    **Origin:** `docs/panel/113`, 2026-09-06; the finding is both compiling
    seats', independently, in separate checkouts, and the ffi seat's positive
    control is what makes their silence admissible. **Re-homed a second time
    2026-09-06, at M-thread-stacks' close**: that milestone ran threads and did
    not meet the return condition either, so the item moves on rather than
    expiring with the milestone that failed to close it. Its home is now the
    next milestone whose own work runs many threads for a real reason — a web
    server is one connection per thread — because this is a WATCH item and a
    watch needs traffic, not a schedule.

    **The sitting was convened on this and re-aimed itself.**
    `runtime/parts/cow.c:44`, `:78`, `:83` and `runtime/parts/map-write.c:127`
    read `refcount == 1` and then mutate; step 3 made the read whole and did not
    make the pair single. **What no seat could do is race it.** With
    `parts/thread.c`'s guard patched down in their own copies: 8 threads and
    240,000 mutations of nested `[[str]]` and `{str: i64}` under ASan and
    ThreadSanitizer, **exit 0, zero warnings**; and 4 foreign threads with
    100,000 concurrent mutating touches of one shared header, same result, for a
    `[i64]`, a `{str: [i64]}` and a `str`. **The instrument was proved live in
    the same session**: a hand-made race on `hero_array_push_owned` is `data
    race … cow.c:80`, exit 134, 3 races, length 246821 instead of 400000.

    **The reason is the grammar, not luck**: a callback parameter arrives
    BORROWED and the emitted body increfs before it can store (`t1 = h0_a;
    hero_array_incref(t1); h1_ys = t1;`), and the two ways round that are closed
    — `@` inside a function type is `error[expected_type]`, and writing a
    callback parameter is `error[not_mutable]`. **Option A, a
    compare-and-exchange from 1 to a busy sentinel, is deferred and NOT on
    cost**: measured cheap (0.37 s against 0.36 s over 40M stores) and unsound
    as scoped, because the sentinel must be held across the CALLER's mutation —
    `hero_array_set` does `drop(place); memcpy(...)` after `unshare` returns —
    so a panic inside `drop` would strand a block busy forever. **Option B is
    vetoed by both compiling seats**: 400,000 stores at 24.50 s against
    40,000,000 at 0.36 s, O(n²) and not a percentage.

    **THE RETURN CONDITION, and it is the whole item now**: a program that
    corrupts memory through those four sites **with every reference counted** —
    that is, without C releasing a reference it still lends. Produce it and A
    lands with the critical section widened to cover the caller. The searches
    that failed are named in the sitting rather than hidden, and what was NOT
    tried is named too: `sort` through a function pointer, the `eq` and `hash`
    descriptor walks, and the drop-list drain under contention.

    **Where to look also:** `docs/panel/113` § What did NOT reproduce.
    **Why it matters:** a window nobody can reach is not a repair anybody should
    ship, and the sitting's own measurements are what say so.

    **Re-verified 2026-09-10: STILL OPEN, and every citation is exact.**
    `grep -rn 'refcount == 1' runtime/parts/*.c` returns exactly the four sites the
    item names, `cow.c:44`, `:78`, `:83` and `map-write.c:127`, and `cow.c:38-43`
    still carries the sitting's note that until this lands, `parts/thread.c`'s guard
    is what keeps any other thread out. The return condition — a corrupting program
    with every reference counted — is unmet, so the watch stands. Its 240,000-mutation
    figures need threads under TSan and were not re-run.

- [ ] **M-core-packages** | the loopback HTTP server, one connection per thread | `design.md` Part 7 item 13 · `runtime/parts/thread.c` · `docs/panel/111`

    **Origin:** carried out of the callback-boundary item that closed at
    M-c-callbacks step 0, 2026-09-05. **Re-homed 2026-09-06**: the threads it
    needs landed at M-isolated-threads and the ten example programs are the
    witness Part 7 item 13 was owed; what is still missing is SOCKETS, which is
    that milestone's question (iv) — `sockaddr` differs between Darwin and glibc
    and Windows is winsock.

    **The permission that blocked it is gone and the witness was never
    written.** A loopback HTTP server answered `curl` from Heroes on 2026-09-03,
    single-threaded; the version with one connection per thread is what
    design.md Part 7 item 13 exists for, and it could not be written at all
    while a function value could not cross the FFI. It can now —
    `pthread_create` binds, runs and joins, measured 2026-09-05 — and what it
    will meet is the four corruption classes panel 111 built, which is precisely
    why it belongs at the milestone that makes them unreachable rather than at
    the one that opened the door. **Note what the guard does to it today**:
    every worker stops by name at its entry, so the witness is not runnable
    until isolation lands, and that is the correct state rather than a
    regression.

    **And the spelling is not portable, measured 2026-09-05 on both machines**:
    `pthread_t` is an opaque pointer on Darwin and an `unsigned long` of 8 bytes
    on glibc, so the same binding is `@thread: ptr` here and `@thread: u64`
    there — both compile and run on their own machine, neither compiles on the
    other, and the compiler says which with a `guess` fix on each. That is
    M-core-packages' platform-typedef question (item (v), `clockid_t`) arriving
    in a second place, and this milestone meets it first.

    **Why it matters:** a concurrency milestone with a model and no program
    decides nothing, and this is the program.

    **Re-verified 2026-09-10: STILL OPEN in substance, one sentence now FALSE.**
    Still open: no server program exists and nothing in the tree binds `socket`,
    `bind` or `listen`. **The false sentence is the guard's**: *"every worker stops by
    name at its entry, so the witness is not runnable until isolation lands."*
    Isolation landed at M-isolated-threads on 2026-09-06, and
    `runtime/parts/spawn.c:192-193` has `hero_spawn_enter` call `hero_thread_claim()`,
    so a Heroes-spawned worker is home and the guard never fires on it — today it
    refuses only a thread **C** made itself (`runtime/parts/thread.c:10-68`), which is
    the hole it was written for. `examples/threads/main.expected` shows eight threads
    answering. So the witness IS runnable now, and that is the item becoming cheaper
    rather than staler.

- [ ] **M-core-packages** | `compile "gfx.c"` waits for a witness, not for an argument | `docs/panel/114` § R6, R7 · `selfhost/cli/libraries.hero` · `examples/sdl/main.hero`

    **Origin:** settled out of `docs/work/DECIDE.md` on 2026-09-06 by author
    instruction, after `docs/panel/114` R6 closed the thin half the same day.

    **The thin case is answered and needs no language form**: one header the
    package ships with `#ifdef` inside and `static inline` wrappers, measured on
    macOS and on the Windows box, one `.hero` source, exit 0 on both, and
    `--emit-c` carrying zero platform words. SDL is that case and
    `examples/sdl/main.hero` binds it today with no platform word anywhere in
    the file; the `link` half is `package`, which panel 050 said *subsumes the
    platform axis panel 049 refused while putting no machine's name in any
    program*.

    **The thick case is refused by Principle 0 and not by a judgement about
    `compile`**: a wrapper with hundreds of lines of implementation wants a `.c`
    file the compiler builds, nothing in the closure list calls one, and no Part
    11 metric moves — so it waits, regardless of elegance (CLAUDE.md §2).
    **What is owed here is a WITNESS, not a decision**: a package in this
    milestone's own work whose C half is too large for a header of `static
    inline` functions. If it appears, panel 036's deferral reopens with its
    terms already priced — a group clause (`extern "gfx.h" compile "gfx.c"`) was
    panel 036 P2's own veto, so the package-level shape is the one that has
    never been judged, and panel 114 R7 has already recorded what a
    declaration-level form would cost. **If it does not appear in a milestone
    about packages that compose, that is the answer** and the deferral closes
    for good.

    **Where to look also:** `docs/panel/036-the-ffi-ladder.md:275` ·
    `docs/panel/050-*.md`.
    **Why it matters:** the one question a Heroes user asks that has a good
    answer for small libraries and no answer for large ones, and it has been
    waiting for a witness since August rather than for an argument.

    **Re-verified 2026-09-10: STILL OPEN, correctly waiting, and today's commit
    sharpened it.** `grep -rn 'compile "' selfhost/ spec/ design.md` returns one hit,
    `design.md:2152`, the record of panel 036's deferral; no surface form exists and
    `examples/sdl/main.hero` still carries zero platform words. **The whole tree now
    holds exactly one C file under `examples/`** — `examples/gallery/13-lease.h`,
    seven lines of `static inline` wrappers, landed 2026-09-10 with `7965174d`. That
    is the **thin** case again, which is panel 114 R6's own answer, so the deferral
    still has no thick witness and the wait is doing its job.

- [ ] **M-core-packages** step 1 | `strconv`, because six programs write the same digit loop by hand | `selfhost/check/builtins.hero:73-95` · `examples/json/`, `calculator/`, `ini/`, `spreadsheet/`, `csv/`, `interpreter/` · `docs/ROADMAP.md` § M-core-packages

    **Origin:** author decision 2026-09-10, § What production-ready means row 6,
    the first of its four silences. The package table gained a `strconv` row the
    same day.

    **What is missing and how it is known**: `to_i64` does not take a `str`
    (`selfhost/check/builtins.hero:73-95` — an integer or a float, and a `str`
    argument is a diagnostic), so **every program whose input is text builds its
    numbers digit by digit**: `json`, `calculator`, `ini`, `spreadsheet`, `csv`,
    `interpreter`. `tests/harness/strings.hero` carries a `to_number` of its own,
    which is the seventh copy. Go's tree has `strconv` for exactly this.

    **Width and precision belong here too, and only here for now.** There is no
    route to a padded integer or two decimal places, and the compiler hand-writes
    four padders (`cli/measure.hero:305`, `cli/doctor.hero:161`,
    `print/fmt.hero:145`). A `f64` to two places is integer arithmetic and a point,
    so it is this package's work. **It becomes a question about the language only
    if this package cannot do it** — a format spec inside an `f"…"` hole is the
    shape it would take, and `f"…"` landing on 2026-09-09 is what makes it
    thinkable — and that is a return condition rather than a plan.

    **It adds no built-in and no language form**: panel 097 condition 5 keeps
    `selfhost/library_source.hero` closed, and this is ordinary Heroes.

- [ ] **M-core-packages** step 6 | a service that stops cleanly is not writable, and the server step is where that stops being theoretical | `spec/heroes-spec.md:47` · `selfhost/check/ffi.hero:262-268` · `runtime/parts/thread.c:10-68`

    **Origin:** author decision 2026-09-10, § What production-ready means row 6,
    its third silence.

    **What is true today, measured.** A signal handler is *declarable*: a callback
    may stand as a **parameter** (`selfhost/check/ffi.hero:262-268`), never as a
    result. But **there are no mutable globals** (`spec:47`), so a handler has no
    way to record that it fired — it can only call `exit`, which is not a graceful
    stop. So `SIGINT` on a server that should drain its connection and close its
    database is not writable in Heroes, in any spelling, and nothing in the record
    says so.

    **Why it is homed on the HTTP server step and not on the opening sitting**: it
    binds nothing until there is a server to stop, and the step that writes one is
    where the answer is cheap or the wall is real. **What it must not quietly
    become** is a mutable global: that is Part 6, permanently. The shapes to price
    are a runtime part that owns the flag and answers a Heroes call, and the
    server's own loop asking between connections.

- [ ] **M-core-packages** step 6 | the long run: every program in the net exits, and a service is the one that stays up | `tests/harness/suite_corpus.hero` § configurations · `runtime/parts/alloc.c` § `hero_runtime_check_leaks` · `docs/ROADMAP.md` § M-core-packages

    **Origin:** author decision 2026-09-10, § What production-ready means. The
    finding is structural rather than a defect: `main.expected` is the shape of the
    whole net, so **every instrument here watches a program that starts, prints and
    stops**.

    **What that makes invisible** is exactly what production meets first: a
    refcount that drifts by one per request, a descriptor never closed, memory that
    grows one connection at a time. None of it is reachable by a corpus of
    one-shot programs, and this milestone's own acceptance says why — a listening
    server does not fit `main.expected`, so its loopback case is server and client
    in one process with deterministic output.

    **Two cheap additions on top of that case, not a new programme**: drive the
    loopback server for N requests rather than two, and read the allocation count
    at the end; and assert `hero_runtime_check_leaks()` after the long run rather
    than after the short one. If the count is flat across N, the class is shut for
    the shape the corpus can see. **What it is not**: an endurance suite, a soak
    farm, or anything that goes red at random — CLAUDE.md's own warning about an
    instrument nobody trusts applies here first.

- [ ] **M-core-packages** step 6 | only an `i64` crosses into a thread, and the spec never says the word | `runtime/hero_os.h:251-256` · `selfhost/check/ffi.hero:76-92` · `design.md` Part 7.13 · `examples/threads/main.hero`

    **Origin:** author decision 2026-09-10, § What production-ready means row 6.

    **Two measurements, and the second is the one nobody had written down.**
    Concurrency arrives as `extern "hero_os.h"` with `hero_thread_spawn`,
    `hero_thread_join` and `hero_thread_limit`, where **only an `i64` crosses in
    each direction** and the checker enforces it — a `[T]` or `{K: V}` inside the
    callback's signature is `error[ffi_type]`
    (`selfhost/check/ffi.hero:76-92`). That is Part 7.13's data-parallelism rung
    exactly as designed, and `examples/threads/main.hero` says so in its own first
    paragraph: *"it is the model"*. **And `spec/heroes-spec.md` never says thread,
    concurrency, spawn or parallel** — zero occurrences — so a program written from
    the spec alone, which is the one reader this language exists for, cannot use a
    thread at all.

    **Why it is the server step's question.** `docs/ROADMAP.md` § M-web-framework
    describes *"a `Request` record … exactly the message the separate-heap model
    wants, small and copied once"*, and **no program can write that today**. One
    connection per thread needs only the descriptor, which is an `i64`, so the
    server step is where it is found out whether the rung is enough — and if it is,
    that is the answer and the spec is what changes, not the runtime.

    **What it may not do**: widen the model on an appetite. Panel 111 measured what
    an unguarded `str` across 32 threads costs — `heap-use-after-free` in nine ASan
    runs of ten — and `runtime/parts/cow.c`'s test-then-mutate is still this
    milestone's own open watch item.

- [ ] **M-core-packages** | `heroes test` cannot run one test, and the binary already can | `selfhost/cli/verbs.hero:104-163` · `selfhost/cli/table.hero` · `CLAUDE.md` §10

    **Origin:** author decision 2026-09-10, § What production-ready means. Homed
    here because this is the milestone that multiplies both the packages and their
    `test` blocks; it is small enough to ride any commit that touches the CLI
    table.

    **What is true today**: there are **545** `test` blocks in the corpus and no
    way to run one. `run_test` (`selfhost/cli/verbs.hero:104-163`) compiles once
    and then loops over every title, handing the binary an **index** — so the
    binary can already run test *N* and no flag exposes it. Tests are collected
    across every `use`d module, so a run cannot even be scoped to one file.

    **What it owes**: §10's stopping-rule argument, like any other flag. The
    argument available is that the harness itself will need it once a package's
    tests are a corpus of their own, which is the same shape that admitted
    `--operator` for `heroes mutate`. **A flag, never a verb.**

- [ ] **M-core-packages** | `heroes build --help` is an error at exit 2, which is a stranger's first minute | `selfhost/cli/table.hero:139-176` · `selfhost/cli/argv.hero:78-81` · `selfhost/cli/help.hero:22-46`

    **Origin:** author decision 2026-09-10, § What production-ready means.

    **Measured**: `./heroes build --help` answers ``error: `build` does not accept
    `--help` — it accepts --dump-ir, --emit-c, -o, --include, --library,
    --sanitize, -O0, -O2`` and exits **2**, the code reserved for *the tool could
    not run*. The message is good and the verdict is wrong: asking a subcommand for
    its help is not a failure, and every tool a production user has ever run
    answers it.

    **Where the answer already lives**: `--help` and `-h` are global words handled
    outside the flag table (`selfhost/cli/argv.hero:78-81`), and
    `selfhost/cli/help.hero:22-46` already prints per-command text from the one
    table that both parses argv and prints help, so the two cannot disagree. What
    is owed is that a subcommand's `--help` reaches that printer instead of the
    unknown-flag arm, and the exit code that goes with it — **0**, since the tool
    did what it was asked.

*******************************************************************************
