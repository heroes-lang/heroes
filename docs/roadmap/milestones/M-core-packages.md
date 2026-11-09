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
| `crypto/sha256`, `crypto/hmac` | `pkgconfig "libcrypto"` | writable, Mac and Linux |
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
`tag`, `partial`, `link`, `package` (`pkgconfig` since panel 240), `as` — every one a contextual word matched by
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

**Questions added 2026-10-14 for the opening sitting**, by the author's
instruction, about what the packages do for a model that writes Heroes, since
a model in a new language most often invents an API: (x) an interface card per
package, its signatures and one line each, measured with `heroes measure`
under a total budget, which is also what would admit `heroes doc` under
CLAUDE.md § 10's rule; (xi) Go's package and function names as an advantage
and a trap, with the "did you mean" of `selfhost/resolve/near_names.hero`
measured on qualified names; (xii) question (ix), thin or thick, settled by a
paid reader test rather than by opinion; (xiii) the effect measured, the
thesis harness's tasks run before and after this milestone. **One more
acceptance**: the repository's own hooks, twelve Python files under
`.claude/hooks/` doing files, processes, git, JSON and exit codes, rewritten
in Heroes over these packages, a program somebody uses every day.

**Corrected the same day**, facts above that have moved: a function value
reaches a C callback parameter since M-c-callbacks; question (vi) is answered
by panel 114's R1, *no conditional-compilation form enters the language*;
panel 036's `compile "shim.c"` was a deferral read as a veto (issue 927); the
contextual words are sixteen, not six (`selfhost/keywords.hero`); and step 0's
three repairs landed as defects 381, 010 and 143 while issue 860 stayed open.
This row moves from 67 to 75.

**A comparison read on 2026-10-25, and four questions for the opening
sitting.** Goose ships a library written in itself over `extern fn`
(github.com/aardappel/goose, [goose/docs/stdlib.md](https://github.com/aardappel/goose/blob/master/docs/stdlib.md)), which § 1.11 refuses here;
what carries across is the method around it. (1) Each library function it
adopted "was measured on its own ... against the hand-written form it
replaced", and the substitutions "tested but not adopted" are written down
beside them ([goose/bench/notes.md](https://github.com/aardappel/goose/blob/master/bench/notes.md)): a package function here is measured against
the loop it replaces, and the refused substitutions recorded. (2) Its
admission test, "is the idiomatic hand-written loop more than three lines,
*or* is it easy to get subtly wrong ... If neither, it is not in the library",
published with the refused list and the loop that replaces each
([goose/docs/design/stdlib_design.md](https://github.com/aardappel/goose/blob/master/docs/design/stdlib_design.md) § 1, § 9), is one candidate for where this
row's line falls, beside panel 057's refusal of `path_join`. (3) A short page
of the packages' own conventions, their names and their failure codes, the
error form itself being spec § 6's. (4) Its `os` module offers a write that is
never seen half-written beside the plain one; the runtime here already
stages, flushes and renames (`runtime/parts/replace.c`), so whether spec
§ 11's `write_file` goes through it is the question to answer before a package
offers a second function. The same library shows what a missing convention
costs a reader: a trailing `bool`, an optional, `-1`, a sticky error read once
and a throw, five conventions in one library, where spec § 6 has one.

## Opened 2026-11-05

**Opened by the author's answer of 2026-11-05**, given in the session's
question widget on a recommendation, meant as: *yes, open the row and convene
the opening sitting*; on the trunk and in no lane yet, the opening writing
records alone, every other session of this Mac offline (the six peers
`ListAgents` named). Step 0, as rows 74 and 75 opened. It is CLAUDE.md § 4's
yes once per milestone, so a sitting this milestone convenes after the
opening one is convened without asking again.

**What it starts from**, each number its command's at this opening: the tree
at 265e11e7f, the release `v0.6.0`'s commit; the compiler `0.6.0`, `sources
1a1e340778db789a`, the tree's own by `.claude/hooks/provenance.py`'s mirror;
701 modules and 182,687 lines of Heroes (`find selfhost -name '*.hero'`); the
seed 278,527 lines and 29,043,650 bytes; `examples/` 55 programs, 120 `.hero`
files and 18,840 lines, 21 of the files holding an `extern` group (`grep -l
'^extern '`). No decision issue open (`open --kind decision`), and sixteen open
issues on the GitHub milestone (`open --milestone M-core-packages`): 1, 803,
805, 893, 925, 927, 970, 1067, 1069, 1072, 1073, 1103, 1154, 2081, 2082 and
2209.

- **The copies the first package takes out**, by issue 805's own command,
  `grep -rl '^function <name>(' examples`: `split_lines` 9 files, `is_space`
  9, `trimmed` 7, `index_of` 4, the counts of 2026-09-10 unchanged; and
  `tests/harness/strings.hero` holds 15 functions (`grep -c '^function '`),
  14 on 2026-09-10.
- **The programs the first packages are extracted from**, by `wc -l`:
  `examples/json/` 674 lines in three modules (the table above says 671 on
  2026-09-03), `examples/template/main.hero` 235, `examples/curl/main.hero`
  99, `examples/sqlite/main.hero` 119.
- **The repository's hooks, issue 2082's acceptance**: thirteen modules and
  their test file under `.claude/hooks/`, 4,414 lines and 2,562; the issue
  said twelve on 2026-10-14.
- **The platform layer as it stands**, for the author's fourth instruction
  below: `runtime/hero_os.h` declares 60 functions, every one distinct;
  `runtime/runtime.c` includes 29 parts, one per file of `runtime/parts/`;
  and 132 lines in 16 files of `runtime/` name a platform macro (`grep -c -E`
  over `defined(_WIN32)`, `defined(__APPLE__)`, `defined(__linux__)` and the
  `#ifdef`/`#ifndef` of the three), the most in `parts/replace.c` 36,
  `parts/run.c` 19, `parts/fs.c` 14, `parts/stack.c` 11 and `parts/spawn.c` 10.
- **The latency clock this row owes**: M-latency-ceiling's page stands a
  fourth clock, `heroes check` on a program with packages, empty until this
  row; it takes its first number the day the first program over a package
  exists.

**The author's three answers of 2026-11-05**, each put with a recommendation
and each answered with it:

1. **The row opens and the opening sitting is convened**, full five seats,
   before any step (issue 803).
2. **The rule for names**, meant as: *a Go name is kept only where the
   contract is the same; where Heroes' contract differs, the name differs
   too*. Go's `strings.Index` answers `-1` where nothing is found, and a
   Heroes function's natural answer is a `T?` failing `not_found`, so that
   function is not called `index`. It goes to the sitting as the author's
   position with its reason, and the sitting measures it (questions (xi) and
   (xiii)).
3. **Both paid runs are authorised**: the paid reader test that settles
   question (xii), thin or thick, and the thesis harness's tasks run before
   and after the packages for question (xiii). The sitting's brief writes
   their budget; no seat starts another run of its own initiative.

**The author's four instructions of 2026-11-05**, given after the first plan
and before the opening, each written as what the author meant:

1. **The rhythm of testing**: *do not run many tests along the way, or this
   step could never end; go ahead, develop, then test in groups, in batches;
   the net does not run for every small change*. The words ended *run the net
   every time for every single small change*, and the sentence they close says
   the opposite; the plan read it so and the author approved that plan. It is
   `.claude/rules/verification.md` § The batch, restated for this milestone: a
   lane develops several packages in sequence, each judged by its own cases
   and by `heroes test` on the package, the full net once per batch of
   packages and before each push, and a defect found beside a package is
   filed by its class and does not stop the lane (§ Bounded discovery).
2. **Look at how the libraries of Odin, Zig, Hylo and the languages like
   Heroes are made** (the words said *ILO*, read as Hylo, the value-semantics
   language design.md cites). The historian's task at the opening sitting,
   with web search and no precedent unsourced: each library's tree, its
   names, its convention for failure, how it treats the platform, and what it
   refuses to hold.
3. **Coherence first**: *the core packages must be thought through,
   coherent, clean and idiomatic, as much as can be; they are the code people
   will go and read to learn the language, and they must be organised
   extremely well*. Four consequences: (a) **a page of the packages'
   conventions**, their names, spec § 6's failure codes, the shape of a raw
   module and its wrapper, and where each thing stands in the tree, written
   **before the first package**, a deliverable of the opening sitting; (b)
   **every package re-read before its batch's gate** as a stranger opening
   `examples/` to learn Heroes would read it, the re-reading a step of the
   lane; (c) `.claude/rules/module-shape.md` binds a package as it binds the
   compiler; (d) **no copy by hand survives** a package's arrival.
4. **A platform layer beside the runtime**: *where platforms differ, the
   difference goes not in a C library outside the runtime but in something
   pulled in beside it, a `platform`, with the `#ifdef`s underneath; look at
   whether something like it already exists, as Qt and GTK do it*. The
   layer exists today and is the runtime (the counts above), the route panel
   097 chose and spec § 13 names, *threads, processes and files from the
   language's `hero_os.h`*. The question it puts to the sitting is question
   (iv) widened: **one runtime, as today, or the language's runtime (the
   allocator, `str`, the array, the map, copy-on-write, the abort) apart from
   a platform layer (the process, files, directories, spawning, threads,
   sockets, time) linked only where a group names it**, and behind one header
   or a family of them. What already binds the answer: no form of conditional
   compilation enters the language (panel 114's R1), the platform axis was
   refused with a veto (panel 049), the C a package adds lives in the runtime
   while `compile "shim.c"` waits for a witness (issue 927), and the CI
   compares the seed byte for byte on every leg. The precedents the historian
   verifies rather than recalls: GLib under GTK, Qt's platform plugins,
   libuv under Node, the Apache Portable Runtime, SDL's backends, Go's files
   per system and its `syscall`, Zig's `std.os`, Odin's `core:os`.

## The opening sitting, 2026-11-06

Panel 234 sat on 2026-11-05 and 06, the full panel with a completeness critic before and after the
seats and the reader test as the llm-ergonomist's seat (`docs/panel/234-the-core-packages-are-code-a-program-carries-in-gos-tree-under-core-a-thick-face-on-a-published-floor-a-card-that-shows-the-call-and-the-platforms-c-an-archive-beside-the-runtime.md`). Its resolution is **provisional,
the author's ratification pending**, asked by decision 2304
(https://github.com/heroes-lang/heroes/issues/2304), and the conventions page and the first batch
wait for it. What it adopted, in one breath: a core package is code a program carries, under
`core/` in Go's tree inside the program's own root, a copy per program held byte-identical to a
canonical tree at `examples/packages/`, unwritten on 2026-11-06; a function enters on a count of the programs that already
need it; a Go name only where the contract is Go's; every package over C a thick face over a thin
floor published with its own card; a card per package showing each function's call; and the
author's fourth instruction answered as an archive of per-capability C files beside the runtime,
always on the link line so the linker takes only what a program calls, `hero_os.h` staying in the
runtime. The historian's survey found the author's platform shape in libuv, APR, NSPR and QtCore,
and every language surveyed keeping that layer inside its one library.

Found beside it and filed: defects 2289, 2290 and 2291, `blocking`, so this milestone is not
tagged before they close, and 2292, `adjacent`; the work the resolution owes is tasks and
features 2293 to 2302, and task 2303 queues at M-publication-gate whether the packages enter the
`1.0.0` promise.

**Corrected 2026-11-06, of the table at the head of this page**: the `io`, `bufio` row
names *bulk text through `hero_str_from_bytes(p: ptr, n: i64)`, exported by panel 035* as the
route, and no program can bind that function, `error[ffi_runtime_header]` (the ffi-pragmatist on
this Mac and Linux arm64, the coordinator again this day). The route is spec § 13's fill form, a
`[u8] counted_by n lent` read by C and made text by `validated_bytes()` (panel 234's R7).

**And the tension with design.md §1.11 this page left unsaid** (the sitting's critic, G15): the page
plans JSON and an HTTP server in Heroes alone, while §1.11's rule lists JSON and HTTP among what
*comes from C libraries* and its own table answers JSON with *write it in Heroes ... or bind cJSON*.
Panel 234's R1 places the packages: they are modules a program carries in its own tree, so the
language still ships nothing; §1.11's sentence saying so lands with the ratification (task 2301).

**Ratified 2026-11-06**, decision 2304 (https://github.com/heroes-lang/heroes/issues/2304): the
author read the sitting and ratified R1 to R16 as adopted, the six choices as the sitting took them
and the paired run at the close in place of a run now and one at the close (the sitting's § Author's
verdict). So the conventions page and the first batch, `strings`, `path`, `strconv` and
`encoding/json`, open now, and task 2301 writes into design.md R1's §1.11 sentence, R8's family rule
in §4.19, R10's condition (b') and item 10's falsifier, and R13's correction of Part 6's traits row.

**And of the same table's `sort`**, corrected the same day: panel 234's R13 makes a stable sort by a
caller's order a function of the package `core/slices`, Go's own later home for it, and no package
is named `sort`, since a `use` binding a built-in's name is refused (R3, feature 2299); issue 1154's
library route is withdrawn with it.

**Paused after the first batch, 2026-11-07, by the author's instructions of
2026-11-06.** The dependency system is decided and built before the core
packages (M-package-manager, row 77, opened beside this row the same day):
the first batch, built and gated in the lane `core-b1` at `9791d4787`, waits
there unpushed until row 77's first version can distribute it, and the batches
after it resume once it does. Two facts of the lane go with it: the site's
build refuses it as it stands (run 2026-11-07 in `core-b1`: *packages is on no
shelf*, and its English and Italian pages missing), and its copies under
`core/` and its 41 `use core/` lines move where panel 235 decides.

**Where they moved, 2026-11-08** (M-package-manager step 14, lane pm-core;
panel 235's R15): the thirteen copies left the programs and the canonical
tree left `examples/packages/`, and the source is `core` 0.1.0 in a local static index,
then inside `examples/` and read over `file://../index`, at the repository's root
since panel 238's R10 the same day, `packages/`, read over `file://../../packages`
(every placed folder renamed with it, the key holding the `index` line as written).
Each of the fourteen programs
that uses it holds a catalogue `heroes add core` wrote and the tree it placed
under `heroes.packages/`. Of the 43 `use core/` lines (41 when the sitting
counted, `spreadsheet` having gained one with defect 2307's repair), the 23 in
the programs' own modules are `use <core>/`, and the 20 in the copies' and the
canonical tree's faces left with them; the index's two faces write theirs from
the package's own root, `use strings/internal/scan` (R4).

**A new acceptance condition**, of the same shape as the repository's hooks
rewritten in Heroes over these packages (issue 2082): **the package manager
rewritten over the core packages, every golden of its interface passing
unchanged.** It is M-package-manager's third tempo, and this row closes with
it.
