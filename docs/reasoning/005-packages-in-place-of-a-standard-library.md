# 005 — packages in place of a standard library

**Origin.** 2026-09-03 · reasoning session · «Vorrei ragionare sulla possibilità
di aggiungere uno step per creare una mini libreria standard, o una sorta di
package di libreria standard che uno può importare o non importare … avere
un'interfaccia di utilizzo molto più idiomatica per Heroes … dopo che avrò fatto
i thread vorrei avere tutti gli strumenti necessari a costruire un web framework
alla Rails o Django, o anche più sottile, tipo Go, Echo o FastAPI» — and, later
in the same session, «mi immagino più pacchetti che si combinano e poi quello web
che li usa tutti; il modello è Go come organizzazione», then «la compilazione
condizionale con la macro if che però non è una macro, o un'altra parola
chiave». Read: design.md §1.11, §3.5, §4.15, §4.19, §4.20, Part 6, Part 7 items
4, 10 and 13, Part 9 · spec § Files and layout, § Built-ins, § FFI ·
`docs/ROADMAP.md` § The chain and § M-package-manager · `DESIGN-LOG.md:178`,
`:282`, `:408`, `:413` · `docs/panel/013`, `028`, `032`, `036`, `039`, `049`,
`056`, `057`, `092`, `094`, `097`, `099`, `103` · `docs/reasoning/003`, `004` ·
`selfhost/check/ffi.hero`, `selfhost/emit/ctype.hero`,
`selfhost/parse/use_line.hero`, `selfhost/modules.hero`,
`selfhost/ir/place_store.hero`, `selfhost/cli/pointee.hero`, `runtime/hero_os.h`,
`runtime/heroes_runtime.h` · `examples/` · probe programs compiled in a
scratchpad on this Mac and in the Linux image
(`docs/environment/linux/LINUX-MACHINE.md`); Windows not measured. **No file of
code, spec or design modified.** The session's records — two ROADMAP rows, five
`SCHEDULED.md` items, one DESIGN-LOG line — are the commit before this note's.

## The question

"A mini standard library" is three questions wearing one name, and the session's
first job was to separate them: **what §1.11 refuses and why**; **what the record
has already promised in its place**; and **what a web server needs that the
compiler cannot express today**. The third was expected to be the long one. It
was the short one, because most of what a server needs turned out to be
expressible — measured, not read — and the two things that are not are compiler
facts with named homes rather than missing libraries.

## What the artifacts say

### §1.11 refuses it, and the ROADMAP routes it

- The refusal is a Part 1 blockquote (`design.md:443-445`): *"Heroes ships a
  minimal runtime and nothing else … Writing a broad standard library is out of
  scope, permanently."* Part 2 repeats it as a non-goal (`:585`, *"Do not
  accumulate convenience functions"*), CLAUDE.md §13 names *"A standard library"*
  as its own place not to go, panel 097 condition 5 (ratified 2026-08-30) keeps
  `selfhost/library_source.hero` closed, and that file's own header
  (`selfhost/library_source.hero:35-37`) says a declaration there must be *"a name
  from the built-in inventory and nothing else"*. The door into the language is
  shut four times over.
- The routing is in the same record. `docs/ROADMAP.md` § M-package-manager:
  *"this is where 'a standard library that wraps C' goes"* — **distributable
  bindings**, ordinary Heroes modules over real C headers, with the test sentence
  *"a binding is verified by clang against the header it names, and a standard
  library is verified by whoever wrote it"*. `DESIGN-LOG:178` (2026-08-11) says
  the same and leaves Part 7 untouched on purpose, *"so that a milestone number
  does not convert a deferral into a commitment"*. Part 9 (`design.md:2745-2750`)
  says of the web: *"JSON is an excellent fit … But it is a library you would
  write yourself, like everything else. The web will arrive through the FFI or
  not at all."* And §1.11's own table (`:504`) already says of JSON: *"write it in
  Heroes"*.
- **A package written in Heroes alone is therefore unruled.** Part 6's table
  (`design.md:2382-2404`) has no row on libraries, packages or preludes; Part 7 has
  no item. Panel 057 (`DESIGN-LOG:310-311`) refused `path_join` as a built-in on
  Principle 0 and left the Tier-2 route *"available"* — the language declines
  convenience functions, and has never said whether somebody else may distribute
  them. Panel 056 supplies the reason no Part 6 row could say no (`056:217-220`):
  *"A document cannot permanently reject the object of a verb it commits to
  shipping"*, and §3.5 commits to `heroes add`. So the line between *a package*
  and *a standard library* is a panel's to draw, and the test sentence above puts
  a pure-Heroes `strings` on the second side of it.

### What the corpus already is

Measured 2026-09-03 over `examples/`: **78** `.hero` files, **10,356** lines,
**35** programs and the gallery, **6** files with an `extern` (`ctime`, `curl`,
`gallery/08-ffi`, `raylib`, `sdl`, `sqlite`). Zero socket calls and zero thread
creation anywhere in `examples/`, `selfhost/` or `runtime/` — the only `pthread_*`
is `runtime/parts/stack.c:86-121` reading the main thread's stack bounds. Already
written and reusable by nothing: `examples/json/` (671 lines, a parser and a
canonical renderer), `examples/template/main.hero` (235 lines, `{key}` with
`{{` as its escape and a refusal on an unknown key), `examples/sqlite/` (open,
prepare, step, column, finalize, no shim), `examples/curl/` (variadic
`curl_easy_setopt`, version string only), `examples/dates/`, `examples/ctime/`.

| helper | files in `examples/` |
|---|---|
| `function split_lines(` | 8 |
| `function is_space(` | 8 |
| `function trimmed(` | 7 |
| `function index_of(` | 4 |

`tests/harness/strings.hero` carries `trimmed`, `is_space`, `lines`, `contains`,
`starts_with` and `ends_with` once more. Nothing in the language lets a program
share them, which is the next section.

### Where a module can come from

`use` starts at the directory of the file you compile (`spec:12-13`), the
resolver reads exactly `open_directory(path) + module + ".hero"`
(`selfhost/modules.hero:125`), a path cannot climb (`selfhost/parse/use_line.hero`,
`climbs`; panel 099 R4 priced `..` and refused it), and nothing is searched at run
time by ruling (panel 028 R3 adopted P-embed over P-source). A probe made the
consequence concrete: a package directory with `test` blocks is testable alone
(`heroes test pkg/strings/strings.hero`), but a module that `use`s a sibling
package compiles **only from the program's root** — so `heroes fetch` must place a
package tree under the root, and each tree wants a root-level test driver. That
is panel 032 R6 (*`use` must not inherit a global scope by silence when packages
arrive*) as an observed fact.

### What blocks a server, in the compiler

- **A function value cannot cross the FFI.** `selfhost/check/ffi.hero:45` refuses
  `.function_ty` in every extern position (also `:117`, `:136`, `:145`); panel 013
  fixed callbacks at `ptr` *"until a C-width type vocabulary exists"*. Yet
  `selfhost/emit/ctype.hero:375-380` says a Heroes function value *is* a bare C
  function pointer, *"no captured environment, because v1 has no closures"*. What
  needs the crossing: `pthread_create`, every event-driven server library,
  `sqlite3_exec`'s row callback, `CURLOPT_WRITEFUNCTION`. What avoids it,
  measured: prepare/step instead of `sqlite3_exec`; libcurl writing into
  `CURLOPT_WRITEDATA` with no write function; the built-in `sort` instead of
  `qsort`. So the packages need it nowhere, and the threads need it first.
- **No byte buffer and no streaming.** `[u8]` is `error[ffi_type]` at check; a
  `ptr` of known length becomes a `str` through `hero_str_from_bytes(p: ptr, n:
  i64)`, bound from `extern "heroes_runtime.h"` (exported by panel 035; 10 MB in
  0.32 s), but only as text; `validated` copies a NUL-terminated string only;
  `runtime/hero_os.h:61-64` reads a file whole; there is no stdin primitive, though
  `fdopen` + `fgets` and `fgetc` work.
- **The platform.** `struct sockaddr` is `sa_len: u8, sa_family: u8` on Darwin
  and `sa_family: u16` with no `sa_len` on glibc — `error[ffi_field_type]` on the
  other platform, measured both ways — and Windows is winsock. `clockid_t` is an
  unsigned enum on Darwin and `int` on glibc, so `clock_gettime` has no single
  spelling; `timespec_get` (C11) does. `htons` on Darwin is only a macro, and the
  parenthesised probe's failure is reported as `ffi_unknown_name`, which is false
  of a header that defines the name; glibc declares the function beside the macro
  and the same program prints 36895 on Linux.
- **Two compiler defects the probes found.** `xs @ xs.push(c.to_u8().must())`
  copies the array on every push: 100,000 pushes in **14.78 s** against **0.00 s**
  with the value bound first (`selfhost/ir/place_store.hero:100` chooses
  `.push_owned` for a bound value, not for an inline `.must()`). And `@value: i32`
  against a `const void *` pointee (`setsockopt`) is `internal error` at exit 2 on
  both platforms — panel 103's pointee assertion writes `sizeof(const void)`.

### What answers already today

A single-threaded HTTP server written in Heroes answered `curl` with no language
change: `socket`, `bind`, `listen`, `accept` as `extern`s; the address as `record
SockAddr tag sockaddr` with the port's two bytes computed in Heroes; the request
read through `fdopen` and `fgets`; the response through `write`; about 100 lines.
On Linux the same program with three lines of the record changed. A loopback
variant — server and client in one process — prints a deterministic transcript,
which is the shape a corpus program needs. SHA-256 and HMAC over `package
"libcrypto"` run on both platforms (libsodium on this Mac has no headers and no
`.pc`); `time`, `timespec_get`, `strftime`, `gmtime_r`, `getenv` all bind; and a
libcurl client with no callback fetched from the Heroes server. None of the three
gaps the session expected — a byte buffer, `htons`, `sockaddr_in` — stood in the
way of the first server.

### The platform question: five shapes, and what the record already says

Other languages answer "this code is for Linux only" in five ways. **A textual
preprocessor** (C's `#if`), whose dead branch is not even read — refused here by
Part 6's Macros row. **A compile-time keyword in the body** (Nim's `when`, D's
`version` and `static if`, Odin's `when`, Swift's `#if`): not a macro, but the
inactive branch is not type-checked on this machine. **Attributes on
declarations** (Rust's `cfg`), likewise. **One file per platform, chosen by name**
(Go's `net_linux.go`, Odin's `_linux.odin`, Hare's `+linux`): no word in the body,
every file a whole module checked on its own platform. **Nothing in the language**
(Ada, Oberon, Modula), the platform living in separate bodies chosen outside —
which is where Heroes stands, with the arm in `runtime/parts/` (panel 097:
*"`#if defined(_WIN32)` is a sentence the language never has to say"*) and the
driver (`package`, `in_the_c_runtime`, `DESIGN-LOG:282`).

The record already weighs against the shapes in the body. Panel 049 refused the
platform axis with a veto (ratified 2026-08-14): *"a platform question belongs
where it is a measurable fact about the machine, not where it is a word the author
must write into a program that then means nothing on the machines it does not
name"*; its engineer stated the invariant, *"the emitted C is a function of the
source alone, never of the host"*, and CI compares `seed/heroes.c` byte for byte
against what the compiler emits on every leg (`.github/workflows/ci.yml:515-520`,
panel 087), so host-dependent emission used by the compiler breaks an instrument.
§4.15 makes every textual difference semantic, which a branch not compiled on
this host is not. Panel 039 and note 003 left comptime unplaced with three joint
return conditions. What the record has **not** said is the limit the fifth shape
carries: the runtime is the only C a package can add to (panel 036 P2 vetoed
`compile "shim.c"`), so an arm in `runtime/parts/` serves the project's packages
and nobody else's. That sentence, and the fourth shape's price — a spec sentence
under § Files, a resolver rule, a vocabulary of platform names, and the emitted C
of a *program* depending on the host — are what the sitting has to weigh.

### What Part 6 refuses at the framework level

Rails and Django rest on what Part 6 rejects permanently: Ruby-style
metaprogramming (`design.md:2395` — *"this is why LLMs err more on Rails/RSpec
than on plain Ruby"*), dynamic dispatch (`:2397`), inheritance, macros. FastAPI's
decorators and type-driven validation need reflection Heroes has not got. What
fits is Go's and Echo's shape: routes as a table of `(function(Request) ->
Response)` values (`spec:112-117`), records for request and response, middleware as
a chain of functions because there are no closures.

### Four record defects found reading forward

`docs/ROADMAP.md` § M-package-manager cited `design.md:637`, the control-flow
bullet; the `heroes add` sentence is at `:772` (corrected with the rows). Panel
056's deliverables B and D were both undischarged nineteen days after
ratification: D, the three return conditions in the ROADMAP entry, is now written
there; B, a greppable §3.5 paragraph, is design.md and waits for a sitting.
design.md's two built-in inventories (`:474-478`, `:2273-2276`) omit `validated`
and `args_checked` (landed 2026-08-24), and Part 7 item 4 (`:2447-2449`) still
reads *"no aliases, no package hierarchy"* a day after M-package-layout. And this
directory's README cited three `docs/debrief/` paths a week after the directory
was renamed (corrected with this note).

## What was settled

Author decisions, all recorded in the DESIGN-LOG line of the same evening:
"i trade" meant the threads; the milestone follows M-package-manager; the level is
Go's and Echo's, everything explicit; the shape is many small packages organised
as Go's tree, composed later by a framework that is a milestone of its own; the
names are `M-core-packages` (Odin's `core:` collection, which §1.11 cites) and
`M-web-framework`; the sitting sits at the opening, with conditional compilation
as its sixth question. The session wrote records only.

## What stayed open → where it was handed off

- The six questions of the opening sitting — the §1.11 boundary, where packages
  live, the byte buffer's three routes, `net` as a runtime part, Part 7 item 10's
  *width or sign*, conditional compilation → `SCHEDULED.md`, `M-core-packages`.
- The callback boundary, the trampoline the emitter owes, and the loopback server
  as the threads sitting's concrete program → `SCHEDULED.md`, `M-isolated-threads`.
- The two compiler defects and `htons`'s misleading diagnostic →
  `SCHEDULED.md`, `M-core-packages` step 0.
- The copied helpers as the first package's contents → `M-core-packages` step 1.
- The three design.md repairs → the item that rides the opening sitting.
- Windows: `tv_nsec: i64` against a 32-bit `long`, `fdopen` against `_fdopen`, an
  OpenSSL `.pc` on the box — measured on the box before each step's commit
  (CLAUDE.md § Commands), never predicted into a record.
- Whether a framework without closures reaches Echo's level or collapses into
  `net/http` itself → `M-web-framework`'s brief, written at `M-core-packages`'
  close.
