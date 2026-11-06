# Panel 234, the completeness critic's second pass: the reports

Written as it goes, 2026-11-06. No verdict is given. Read: the briefs as
repaired (`docs/panel/234-briefs/`, `00-shared.md` first), the first pass
(`critic-briefs.md`, F1 to F37), and every report of
`docs/panel/234-reports/`: `compiler-engineer.md` (CE), `ffi-pragmatist.md`
(FFI), `ffi-pragmatist-sql.md` with its deliverables (SQL), `historian.md`
(HIST), `spec-warden.md` (SW) and `blind-seat.md` (the coordinator's reading
of the reader test, BLIND). Every command below was run in this session, at
f0c4c99d2 in `.claude/worktrees/234-critic/` (whose `selfhost/`, `runtime/`
and `seed/` are the trunk's, b36cd92e5 changing neither), with its compiler,
`heroes 0.6.0` / `sources 1a1e340778db789a`; scratch runs live under
`.claude/worktrees/234-critic-scratch/r2-*/`, one folder per run. The seats'
scratch folders were read, never written. No clock, no paid run, no API call.

Findings are numbered G1, G2 ... as written; the ordered list is at the end.

## G1. The CE's dead-stripping, which answers the FFI's main measured cost of P1, drops nothing under the compiler's own link line: `-rdynamic` (panel 104) keeps every runtime function, on both machines (item F; CE § F, FFI F.4)

The CE approves P1' and "separately, as a task, dead-stripping the link
... which answers P1's growth for every program at once ... about 10 to 20
lines", measured as `print(1)` falling from 237,992 bytes and 522 runtime
functions to 78,128 and 63 (`-Wl,-dead_strip`), and 79,760 and 63 on Linux
arm64 (`--gc-sections` with `-ffunction-sections -fdata-sections`). The FFI's
refusal of P1 rests on F.4's *"every program carries the layer"*. But every
POSIX link the compiler makes carries `-rdynamic`
(`selfhost/cli/link_flags.hero:68-81`, panel 104: *"glibc's `dladdr` reads only
`.dynsym` ... without the flag the message is `panic: stack exhausted` and
nothing more"*), and the CE's hand links left it out: its `hello_stripped`
exports 11 symbols (`dyld_info -exports`), the compiler-built `hello` 247.
Relinked here, the same three objects the compiler made for `print(1)`
(scratch `r2-p2scope/`):

```
$ clang $O -o h_plain; clang -rdynamic $O -o h_rdyn; clang $O -Wl,-dead_strip -o h_ds; clang -rdynamic $O -Wl,-dead_strip -o h_rdyn_ds
hello      bytes  237976  runtime text symbols  522  exports  247  runs: 1
h_plain    bytes  237976  runtime text symbols  522  exports  247  runs: 1
h_rdyn     bytes  237976  runtime text symbols  522  exports  247  runs: 1
h_ds       bytes   78104  runtime text symbols   63  exports   11  runs: 1
h_rdyn_ds  bytes  237976  runtime text symbols  522  exports  247  runs: 1
```

and in `heroes-linux-arm64:latest` (Debian clang 22.1.8, GNU ld 2.44), the
fused C and `runtime.c` compiled with `-ffunction-sections -fdata-sections`:

```
plain    bytes  250448  runtime text symbols  515  runs: 1
gc       bytes   79752  runtime text symbols   63  runs: 1
rdyn     bytes  250448  runtime text symbols  515  runs: 1
rdyn_gc  bytes  250448  runtime text symbols  515  runs: 1
```

So the CE's measurement reproduces only without `-rdynamic`, and with it
nothing is dropped on either machine. Dead-stripping is not "10 to 20 lines"
but a change to panel 104's ruling (drop `-rdynamic` on Darwin, where the
same comment says the symbol table names the function anyway, and on Linux
replace it with an export list the stack guard still reads, or accept a
`panic: stack exhausted` without a name), and no seat priced or ran that.
Until it is, the FFI's F.4 cost stands as measured, and the CE's answer to it
is unrun under the link the compiler makes. Unrun here: Windows, and whether
a narrower export (`--export-dynamic-symbol=h_*`, or Darwin with no
`-rdynamic`) keeps the stack guard's name while letting the runtime strip.

## G2. A route nobody listed for item F: the layer as a static archive always on the link line, so the linker, not the compiler, links "only where a group names it", `-rdynamic` or not (item F; CE § F P2, FFI F.6 condition 4)

The author's words are *linked only where a group names it*; the CE prices it
as "a predicate of about 10 lines" plus a second object's plumbing (150 to
250 lines), the FFI makes it P2's condition (4), and nobody asked whether the
linker already does it. It does, at the granularity of an archive member,
and `-rdynamic` does not change that (it exports what is linked; it pulls in
nothing). The FFI's own P2 layer is an archive of two members
(`F/p2/platform/build-layer.sh`, `ar rcs ... hero_time.o hero_net.o`); copied
into scratch `r2-p2scope/ar/` and put on every link by hand:

```
$ ar t ar/libhero_platform.a
__.SYMDEF SORTED / hero_time.o / hero_net.o
$ clang -rdynamic <hello's three objects> ar/libhero_platform.a -o h_with_archive
hello + archive always linked, -rdynamic: bytes 237984, hero_net_ 0, hero_time_ 0, runs: 1
$ clang -rdynamic <clockonly's three objects> ar/libhero_platform.a -o clockonly
clock only, archive linked: hero_net_ 0, hero_time_ 1, runs: true
```

(`clockonly.hero` binds `hero_time_monotonic_ns` under `extern "hero_time.h"`;
built by the compiler without `link` it is `ffi_missing_link`, so the archive
was added to the link by hand, as the FFI added it.) So under P2 the compiler
needs no predicate: it passes the archive to every link and each program gets
the members it calls, which also answers the FFI's F.4 cost without touching
panel 104's `-rdynamic` (G1). What would have to be true for the same to
serve the **existing** parts (the author's split moves the process, files,
directories, spawning and threads, G3): the runtime compiled as one object
per part and archived. Run (scratch `r2-split/`, each part compiled as
`runtime.c` up to it with every other part's include removed):

```
$ ... clang -std=gnu11 -I runtime -c tu_<part>.c   (29 parts)
OK 4 (panic, thread, drop, codepage)    FAIL 25
FAIL stack: call to undeclared function 'hero_abort' ...
FAIL alloc: unknown type name 'HeroPlaces' ...
FAIL fs: use of undeclared identifier 'hero_fs_why_code' ...
```

so 25 parts read names only an earlier part declares, and splitting them
costs an internal header and the cache's key per member, unpriced. Unrun:
Windows, where whether an unreferenced member's `#pragma comment(lib,
"ws2_32.lib")` is still honoured by MSVC's linker is the question that would
decide F.4's winsock bullet.

## G3. The CE and the FFI judged two different P2s, and neither says so: the FFI's layer holds only the new sockets and clock, the author's split moves the process, files, directories, spawning and threads too (item F)

The author's instruction 4 asks for *"the language's runtime (the allocator,
`str`, the array, the map, copy-on-write, the abort) apart from a platform
layer (the process, files, directories, spawning, threads, sockets, time)
linked only where a group names it"* (`docs/roadmap/milestones/M-core-packages.md:330-334`,
read). The CE prices that split (*"the author's split moves processes, files,
directories and spawning, which the compiler itself binds"*: the compiler's
own build links the layer, and the one-line bootstrap changes in 24 living
files). The FFI's P2, the one it approves on five conditions, holds two new
headers and nothing that exists today:

```
$ ls F/p2/platform/            (the FFI's scratch, read)
build-layer.sh hero_net.c hero_net.h hero_time.c hero_time.h linux mac unit-flags.txt win
$ grep -h -o -E 'hero_[a-z_]+\(' F/p2/platform/hero_net.h F/p2/platform/hero_time.h | sort -u
hero_net_accept( hero_net_close( hero_net_last_error( hero_net_listen_loopback( hero_net_read( hero_net_write( hero_time_monotonic_ns(
```

So the FFI's P2 is a hybrid nobody named: today's platform parts stay in the
runtime behind `hero_os.h` (P1 for what exists) and only what this milestone
adds is a second object. The CE's objections (the bootstrap line, measured
here: `git grep -l -E 'seed/heroes\.c runtime/runtime\.c' b36cd92e5`, 307
files, 351 occurrences, 24 outside `docs/`; the 12 modules of `selfhost/`
with an `extern "hero_os.h"` group, `grep -rl`, 12) bind the author's split
and not the hybrid; the FFI's approval binds the hybrid and not the author's
split. And the author's split cannot keep its own clause for one of its
parts: every program's `main` calls a function of "the process",

```
$ heroes build hello.hero --emit-c | grep -n hero_args_set     (print(1))
212:    hero_args_set(argc, argv);
$ nm -u <hello's unit object> | grep hero_
_hero_args_set _hero_print_end _hero_print_int _hero_runtime_check_leaks _hero_str_from_bytes
```

so whatever object holds the arguments is linked into every program, the
CE's P2' sentence, which the FFI's report does not meet. The sitting has to
say which P2 it rules on: the author's split (whose "only where a group names
it" is false for the process by this run), or the hybrid (which the author did
not describe, and whose name in the author's terms is "one runtime, as today,
plus a layer for what is new"). Under G2's archive the difference shrinks to
which member holds `hero_args_set`.

## G4. P1', the route the CE approves, was built by no seat; built here from the FFI's own parts, it holds two of the CE's three claims and fails the third (item F)

The FFI built P1 (its clock and sockets as runtime parts, declared **in
`hero_os.h`**, a seed) and refused it on F.4; the CE approved P1' (the same
parts behind a **family** of headers that are not seeds) at *"Compiler cost:
none"*, and built nothing. Built here (scratch `r2-p1prime/`): the tree's
`runtime/` copied, the FFI's `parts/clock.c` and `parts/net.c` copied in, their
declarations written into `runtime/hero_net.h` and `runtime/hero_time.h`
(guards, `stdint.h`, the same prototypes), both included by `runtime.c` after
`hero_os.h` and neither by `hero_os.h`; the FFI's P1 package tree and programs
copied with the group lines changed to `extern "hero_time.h"` and
`extern "hero_net.h"`; every build `HEROES_RUNTIME=<the copy> heroes build`.

```
$ heroes build clock_main.hero -o clock_main && ./clock_main
wrote clock_main / true / true
$ (timeout 20 ./echo_main | head -c 4096 > server.out) & timeout 10 ./echo_client 47234 "hello from panel 234"
client got: hello from panel 234           (client exit 0; server: listening / echoed: hello from panel 234; none left running)
$ heroes build seedtest.hero      (extern "stdlib.h" / function hero_time_monotonic_ns() -> i64)
error[ffi_unknown_name]: `stdlib.h` declares no `hero_time_monotonic_ns` — clang read the header and could not find it
$ heroes build hello.hero -o hello  (print(1))
P1' hello: bytes 239480, hero_net_ 8, hero_time_ 1
$ printf '/* an edit, nothing else */\n' >> runtime/hero_time.h; heroes build clock_main.hero ...
build/runtime-6154359722accc0a-....o  ->  a second object, build/runtime-3891fe076613c8ea-....o
$ zig cc -target x86_64-windows-gnu -fsyntax-only -I runtime runtime/runtime.c
winsock2.h:15:2: warning: Please include winsock2.h before windows.h
```

So, on this Mac: P1' needs no compiler change (the refusal of a runtime
header, `selfhost/check/runtime_header.hero`, names `heroes_runtime.h` alone);
its headers are not seeds, so F.4's first cost (*"the seed widens"*) is P1's and
not P1''s; an edit of a family header named in `runtime.c` re-keys the object,
the CE's premise. But every program still carries the layer (F.4's second
cost: 8 socket functions in `print(1)`), because the CE's answer to it is
cancelled by `-rdynamic` (G1); and the one translation unit keeps F.4's third
cost, the include order mingw-w64 warns about. The synthesis has, for the
first time, P1' measured beside P2; Linux arm64 and Windows are unrun for it
here.

## G5. Item I's three checks were each run on a different part of the tree; the FFI's location check, run here over the 433 `run` goldens it was never run on, refuses a passing golden and has no answer for 25 bound names, and the SW's I3 is false as worded on a passing golden (item I; CE § I, FFI § I, SW § 6 I)

Three seats, three checks, three samples: the CE's **in-order** check (a
group's names checked against the module's headers read in written order,
after the seeds) was run over 488 groups including the `run` goldens and
refuses nothing; the FFI's **per group by declaration location, seeds not
counted** was run over `selfhost/` and `examples/` alone (139 functions) and
refuses `process.hero:66-69`, its prediction over `tests/golden/` deferred
*"when the compiler-engineer's prototype exists"*; the SW's **I3**, *"clang
checks every result type, constant and record field against that header and
the language's own"*, was priced in tokens (+5 real) and run nowhere, its
mechanism unnamed. Run here: the FFI's `census.py`, copied to scratch
`r2-census/census_golden.py` and changed in two lines only (it walks
`tests/golden/run` instead of `selfhost examples`, and puts the case's own
directory on `-I` as the compiler does), from the worktree's root:

```
$ python3 -I census_golden.py golden.mac.tsv clang runtime
Counter({'OWN': 952, 'NOT_FOUND': 25, 'OTHER': 2})
OTHER  fixedbugs-2056-a-header-s-own-macro-of-a-name-it-binds-stands.hero | fixedbugs-2056-own.h | fabs | .../MacOSX27.0.sdk/usr/include/math.h
OTHER  fixedbugs-437-a-program-whose-runner-is-killed-dies-with-it.hero | fixedbugs-437-pdeathsig.h | case437_pdeathsig | fixedbugs-437-pdeathsig.h
NOT_FOUND 25: fixedbugs-171 (5, function pointers a header holds), -559 (4), -560 (7, shims, limits and a macro), -2059 (2), -555, -563-wide, -643, -2061, -2253, -2056-...-math-header-...
```

- **One passing golden refused**: `fixedbugs-2056-...-own-macro-...`, whose
  `own.h` writes `#define fabs my_fabs`; clang's `FunctionDecl` named `fabs`
  is `<math.h>`'s (the module's other group), so the location reading calls
  the binding another header's. The golden's own header says the bound name
  *"is that header's ... and refuses nothing"*. Both of the FFI's location
  rows (seeds counted or not) refuse it, since `math.h` is no seed.
- **The 437 row is this Mac's artefact**: its header includes Linux's
  `sys/prctl.h`, `clang -M` gives no closure here, so the census calls its own
  function another's (the CE's 3 unrun groups, the same cause).
- **25 bound names get no location**: function-like macros, shims and
  function pointers held by a header, which a `FunctionDecl` question cannot
  answer; the FFI wrote that constants and records were unrun, and these are
  functions.

So the FFI's prediction (*"refuses `process.hero:66-69` and only cases
written to bind across groups"*) is false on its own mechanism as built, and
the mechanism owes macros before functions. And the SW's I3 is false as
worded on `fixedbugs-2056-a-macro-two-groups-headers-define-alike-...`:
`EXTRA` is defined in `fixedbugs-2056-common.h` only `#ifdef WANT_EXTRA`, which
the earlier group's `fixedbugs-2056-switch.h` defines, so `EXTRA` exists
"against that header and the language's own" only with another group read
first, which § 13's sentence before it (*"one that needs another's names comes
after it"*, `spec/heroes-spec.md:358-359`) makes legal. The SW's argument for
I3, that deleting an unrelated group can no longer break a binding at a
distance, is false of that golden under every reading, since deleting the
switch's group breaks `EXTRA` by design. What no seat has: one check run over
the whole tree that refuses `fb` under `a.h` (the FFI's §4.17 complaint, which
the CE's in-order check admits when `b.h` is written first) and admits both
2056 goldens and 563; the census here suggests its shape (location, a macro's
`#define` asked before a `FunctionDecl`, earlier groups' closures counted), and
it is unrun.

## G6. Item M: three of the CE's five "comparator sorts" in `selfhost/` need no comparator today, and the package's name collides with the CE's own §2.3 refusal, which no seat put beside it (item M; CE § M and § C, SW § 6 M, conventions §4.4)

**The demand.** The CE objects to *neither* on *"five hand-written comparator
sorts ... 84 lines in five places of the compiler"*; the SW says the one
insertion sort left, `modules.hero`'s, orders by *"an `i64` key `stable.order`
takes"*, and checked no other. Read here, each site's order:

| site (CE's) | lines | orders by | served today by |
|---|---|---|---|
| `selfhost/modules.hero:149-170` | 22 | `span.start`, ties in arrival order | `stable.order(by: [i64])` (`selfhost/stable.hero:17`) |
| `selfhost/emit/gate.hero:153-170` | 18 | `span.start`, `<=` keeping ties | `stable.order` |
| `selfhost/check/contracts.hero:214` `sorted` (+ `before`, 9) | 18 | bytes of a `str` (`before`: byte by byte, the shorter first) | the built-in `sort`: spec § 11, *"`sort` (ascending: ... a `str` by bytes ...)"* |
| `selfhost/mutate/mutant.hero:53` `ordered` | 14 | `(start, end > start)` | a comparator, or a composite `i64` key |
| `selfhost/mutate/hunks.hero:102` `sorted` | 12 | `(start, end)` | a comparator, or a composite `i64` key |

Run (scratch `r2-sort/strsort.hero`):

```
$ heroes run strsort.hero      # print(sort(["free","Close","fclose","close","_free","free","a_b","aB"]).join(" | "))
Close | _free | aB | a_b | close | fclose | free | free
```

which is `before`'s order. So 58 of the 84 lines are servable today with no
new function, and the compiler's own need for a comparator sort is two modules
of `selfhost/mutate/`, 26 lines, against a package copy under `selfhost/core/`
the CE prices at 20 to 30 lines plus its tests. The `examples/query` merge sort
(31 lines, records) and the harness's `sorted_versions` (12) are the rest of
the demand. Neither the CE's verdict nor the SW's rests on all five read.

**The name.** Of the page's twenty packages, only `sort` binds a built-in's
name (the last part of each path against 33 names: `inventory.hero`'s `NAMES`
and the library's `function`s, scratch `r2-sort/builtins.txt`; `COLLIDES:
sort binds sort`, no other). The CE's item C asks the compiler to refuse a
module binding a built-in's name (*"refuse `use sort` beside the built-in"*,
about 5 lines in `resolve/top.hero`), and the same report's item M says the
package must be *"named so that its binding is no built-in's"*; the SW
approves *"the `sort` package's `by`"* and conventions-draft §4.4 writes
`sort.by<A>`. If the CE's refusal lands, the SW's and the draft's spelling is
refused at every `use`. The sitting rules C's §2.3 and M's name as one
question or contradicts itself. And the author's rule has not been asked of a
package's name: Go's `sort.Slice` orders indices in place and
`slices.SortStableFunc` takes a three-way `cmp` in place (the historian's
pages), while the package's `by(xs, lt) -> [A]` answers a new array under a
two-place `bool`, so the candidate Go names (`sort`, `slices`) each carry a
Go contract the package does not keep.

## G7. Where the canonical tree stands, which the shared brief asks of every route, is answered by no seat, and the answer decides whether any suite judges the packages at all (item B; CE § B)

The shared brief closes item B with *"two questions every route answers: where
the canonical tree stands in this repository, and where the programs that use
it stand (the loopback server; the hooks of issue 2082)"*. The CE approves B2
with B3, *"held byte-identical to one canonical copy by a check of the net"*,
and its check walks *"`examples/*/core/` against the canonical path"*, which no
sentence of its report names; its prediction's `cmp` lists copies under
`examples` and `.claude/hooks` and no canonical path. No other report names
one (`grep -n -i -E 'canonical (tree|copy|path|one)|where the canonical|stands in (this|the) repository'`
over the five reports: the CE's lines 29, 150-151, 215, 245, 257, 324, 969,
none a location, and the historian's line 452, B6 *"not searched"*). What the
location decides, run at f0c4c99d2:

```
$ sed -n '69,79p' tests/harness/suite_canonical.hero      # SOURCE_DIRS
"selfhost" "tests/harness" "examples" "tests/golden/run" "tests/golden/ir" "tests/golden/emit" "tests/golden/fixedbugs" "tests/golden/lldb"
$ grep -n -E '"(core|core/[^"]*|\.claude/hooks[^"]*)"' tests/harness/*.hero
(nothing)
$ grep -n -E 'root: "' tests/harness/suite_probe.hero      # the multi family's roots
"selfhost" "tests" "examples"
```

So a canonical tree at the repository's root (`core/`), or the hooks of issue
2082 written in Heroes under `.claude/hooks/` with their copy, is held to the
formatter by no suite, probed by none, and its `test` blocks run in none:
only `records` and `unseen` read it, as text. Under `examples/` with no
`main.hero` the CE's own table says the corpus skips it, so its tests also run
nowhere until a driver `main.hero` makes it a corpus program; under
`selfhost/core/` (the CE's item M) it is the compiler's and `layout`'s. Three
copies at three roots (the canonical one, `selfhost/core/`, `.claude/hooks/core/`)
are each a different suite's or none. **A route left unpriced beside it**: the
programs' root files standing together beside one tree (B2 with the roots
co-located), which needs no copy and no byte-identity check, the CE's own
rule (*"one tree serves only programs whose root file stands beside it"*); what
it costs is the corpus's one program per directory (`program_directories`,
`tests/harness/suite_corpus.hero:500-507`), a convention of the harness and
not of the compiler, and no seat priced changing it.

## G8. Item E: the spec-warden's registered condition fired and the reading does not say so; the card's effect is two opposite effects, one per model, and the reading's explanation of Sonnet's is contradicted by Sonnet's own bare answers (BLIND, *E, the card*; SW § 4)

**The condition.** The SW registered, for its card budget: *"a card-minus-bare
lift at or under 0 in both variants says a card buys nothing at these tasks,
and the budget falls to an index line per package"* (SW § 4, condition), and
priced the card by §1.2 at a lift of at least 0.21 for the strings card. The
measured lifts, pooled as the SW registered them, are −0.20 under G and −0.10
under H (BLIND's own table): the condition is met, and BLIND reads item E as
*"it is what makes a reader use the package"* without naming it. Whatever the
sitting concludes, the record owes the sentence that the registered condition
fired, and the argument for setting it aside.

**The two effects.** From BLIND's cell table (scratch `r2-blind/`, the 36
cells parsed; intervals Newcombe's, 95 %):

```
G haiku   right@1 card 14/15 bare  9/15 lift +0.33  [+0.03, +0.58]
G sonnet  right@1 card  4/15 bare 15/15 lift -0.73  [-0.89, -0.41]
H haiku   right@1 card 15/15 bare  9/15 lift +0.40  [+0.11, +0.64]
H sonnet  right@1 card  2/15 bare 11/15 lift -0.60  [-0.78, -0.25]
G pooled  -0.20 [-0.41, +0.03]      H pooled  -0.10 [-0.32, +0.14]
```

These four are the only differences of the whole run whose intervals exclude
zero; the pooled figure BLIND reads averages a gain larger than the SW's
registered one (+0.12, +0.25) on Haiku with a larger loss on Sonnet.

**The explanation.** BLIND attributes Sonnet's loss to *"one rule of the
language, the labels a call with two parameters of one type owes, broken by
the reader's Go habit"*. Counted over the first turns' programs
(`build/b234-grade/<n>/main.hero` by `sealed/key.tsv`, read only; scratch
`r2-blind/labels.py`):

```
bare  sonnet  package calls: labelled 14 positional  4 (answers with one positional: 4) | own two-of-one-type functions 4, calls labelled 4 positional 0
card  sonnet  package calls: labelled 11 positional 42 (answers with one positional: 24)
bare  haiku   package calls: labelled 50 positional  0      card haiku  labelled 55 positional 0
```

and the two arms' briefs differ only in the card (`diff build/b234/051/brief.md
build/b234/026/brief.md`: both say *"with Go's parameter names"*). So the same
model labels 18 of its 22 two-of-one-type calls without a card, its own
helpers included, and 11 of 53 with one: what changed is the card, not a Go
habit the bare arms would show as well. That is a property of the card's form
(each function written as a declaration, `has_prefix(s: str, prefix: str) ->
bool`), which BLIND's last sentence suspects and no session tested; until it
is tested, item E's format is the variable this run points at, and the
budget's pricing by §1.2 rests on a lift whose sign depends on the model.
BLIND's counts reproduce: *use the package* 4, 9, 14, 13 and 15 everywhere
with a card; 24 of 30 Sonnet card answers with a positional call.

## G9. Item C: the bare arms never called three of the four renamed functions in either variant, so the bare names comparison measured `index` alone (BLIND, *C, names*)

Counted over S's first turns (scratch `r2-blind/`, calls `strings.<name>(` in
each program; occurrences / sessions):

```
bare-G trim:12/4 index:0/0 split_n:2/2 position:0/0 cut:0/0 split_once:0/0 lines:0/0 split_lines:0/0
bare-H trim:12/4 index:4/4 split_n:1/1 position:0/0 cut:0/0 split_once:0/0 lines:0/0 split_lines:0/0
card-G cut:20/20 lines:14/14      card-H split_once:20/20 split_lines:14/14
```

With a card, both variants call the renamed functions under the card's names
(nothing to measure: the name is on the page); without one, no reader of
either variant called Go's `cut` or `lines`, or `split_once` or
`split_lines`: they split by hand. The bare comparison's only contact with the
rule is `index`, written 4 times in bare-H and never in bare-G. So
*"C, names: the author's rule costs nothing a card does not repay"* rests on
a card arm where the rule cannot cost (12 of 20 against 12 of 20, interval
[−0.28, +0.28]) and a bare arm where three of the four names were never
reached (−0.10, [−0.36, +0.18]). The SW's design note said the tasks give the
rule little room; the sessions show how little. The reading is right that the
four `index` calls were compile errors; *costs nothing* is wider than the run.

## G10. BLIND scores the spec-warden's predictions alone; the SQL seat's and the historian's, on file before the first session, go unscored, and one of the SQL seat's is falsified by its own words (BLIND; SQL § prediction; HIST C and D)

`stat` here: `historian.md` last written 2026-11-05 22:59:29,
`ffi-pragmatist-sql.md` 23:28:33, the first session's output
(`build/b234-out/002.jsonl`) 2026-11-06 00:02:58. Scored by their own words
against BLIND's table:

- **SQL (2)**: *"Q1 is the only task where thick's `right@1` can fall below
  thin's ... Falsified ... by thick below thin on Q2 or Q3 for any other
  reason."* Q3: thick 9/10, thin 10/10, the one thick miss a Haiku
  `needs_qualifying` (BLIND's row). **Falsified.** And on Q1, where it said
  the gap would come from `.must()` on the NULL `sum`, thick and thin tie
  at 9/10, the predicted cause absent. **SQL (1)** (thick's `silent` no
  higher than thin's): **holds**, 0 and 0.
- **HIST (D)**: thick's `right@1` at least thin's on both models: Haiku 13
  and 13 of 15, Sonnet 14 and 13: **holds**; its second half (*"the two arms
  tie at a floor"* if F1 held) did not arise: F1 was repaired, and they tie
  at a ceiling.
- **HIST (C)**: in the bare arms, failures at the four functions are compile
  errors, *"in the arm with Go's names, Go's contract written on a `T?`
  (`bad_operand`)"*: no bare-G answer called any of the four (G9), so that
  half never occurred; bare-H's four `index` calls are compile errors:
  **holds where it was tested**.

The SQL seat's condition *to object to thick* (thick's `right@2` below thin's
on Q2 and Q3 on both models) is not met (30/30 each). BLIND's own D sentence,
*"The SQLite seat's 22 mistakes ... remain the measurement of the
difference"*, says more than either source: the 22 are mistakes the seat
planted, which measure what each shape does with a mistake once made; the run
measured how often a reader makes them, zero of the five thin-silent ones in
60 answers. The two together are the evidence for D, severity from the
mutants and frequency from the run, and the second is zero at these tasks.

## G11. The FFI's cache defect reproduces, is unfiled, and is the decoy panel 028 R3 argued about, arriving under the route the sitting favours; the CE's "the cache follows the header" measured only an edit (item B; FFI § B, CE § F P3)

Reproduced here (scratch `r2-cache/`): a package `core/db/sqlite3.hero`
(`extern "sqlite3.h" link "sqlite3"`, `sqlite3_libversion_number() -> i64`)
`use`d from a root `main.hero`:

```
$ heroes run main.hero                       -> true   (exit 0)
$ printf '#error "a decoy sqlite3.h at the program root"\n' > sqlite3.h
$ heroes run main.hero                       -> true   (exit 0, warm)
$ cp -R warm fresh; rm -rf fresh/build; cd fresh; heroes run main.hero
error[ffi_header_refused]: `sqlite3.h`, the header this group names, does not compile: clang refuses it at line 1: "a decoy sqlite3.h at the program root"
```

One tree, two programs, by the state of `build/`. The CE's P3 paragraph says
*"the cache follows the header ... a unit is recompiled when its header
moves"*, measured by editing a header that was already read; a file that
appears earlier on the search path was never among the files clang opened,
so the dependency record cannot name it, which is the FFI's diagnosis. Not
filed: `gh issue list --state all --limit 15 --search "sort:created-desc"`
ends at 2288 (2026-11-01); `gh issue list --state all --search "header shadow
cache in:title,body"` names only defect 646. Two things no seat said:

- **It is panel 028 R3's argument, measured, under B2.** The shared brief
  weighs B4 and B5 against R3's *"a decoy `prelude.hero` has no analog"*, and
  the historian finds no language guarding a library root against a decoy.
  Under B2 and B3, the routes the CE approves, a program's own file at its
  root already shadows the system header **every package's group** names
  (the FFI's table, B2: *"read for the package's group"*): the decoy is
  in-tree, needs no search path, and the warm cache hides it. Its repair (a
  negative dependency, or every directory on the search path keyed) is owed by
  the routes the sitting is likeliest to adopt, not only by B4.
- **Its class**: the FFI reads it as `blocking` (*a wrong one accepted*) and
  leaves the class to the coordinator; a sitting that adopts B2 adopts it.

## G12. Item K's sixth repair: the SW is right and the CE is not; `exit` has been Tier 2 since 2026-08-12, and the first pass's F28 was six days late (item K; CE § K 6, SW § 7 6; critic-briefs F28)

The CE writes *"`git log --reverse -S'"function exit('` → `7327bd9bf
2026-08-18`, F28's date confirmed"*; the SW writes that `exit` became a Heroes
function on 2026-08-12 in `0503df194`, in the bootstrap's
`crates/heroes/src/library/source.hero`. Run:

```
$ git log --reverse --format='%h %ad %s' --date=short -S'function exit(' | head -3
81402348f 2026-08-11 panel 030: the build order revised, and the half of M8a that was never a language feature
0503df194 2026-08-12 M-ffi-ladder step 8: the closure list's last three rows, as Tier 2
7327bd9bf 2026-08-18 M-selfhost-fixpoint step 2: the library stops being read from disk, and getenv was one extern away
$ git show 0503df194 | grep -n -E '^\+.*function exit\('
717:+function exit(code: int)          (crates/heroes/src/library/source.hero)
```

`81402348f` only quotes `extern function exit(code: int)` in panel 030's
record (`git show 81402348f | grep 'function exit('`: two lines of
`DESIGN-LOG.md` and the panel's table). The quoted pattern of F28 and the CE,
`"function exit(`, matches only the string list of
`selfhost/library_source.hero` and so cannot find the bootstrap's library.
**Correction of the first pass, 2026-11-06**: F28's *"`exit` has been a
Tier-2 function since 2026-08-18"* is false; the date is 2026-08-12,
`0503df194`; the sentence of design.md:534-535 is false since then, as the
SW says.

## G13. Item H: the "zero" demand both seats measured is one; the SW scanned 20 of the 29 words and its own dump holds `raise`, in a header the milestone's server step binds (item H; SW § 6 H, FFI § H)

`keyword()` holds 21 words and `declared_word` 17 arms of 23 words
(`selfhost/keywords.hero`, `selfhost/foreign_words.hero`, read: `"null" | "nil" |
"None"`, `"try" | "catch" | "throw" | "raise"`, `"import" | "include"`), so the
words a C identifier can spell are the FFI's 29. The SW's veto of `tag` rests
on *"no function declaration named with any of the 20 reserved or foreign
words a C identifier can spell"*: 20 is the arms' count less the C keywords
(10 keywords and 10 arms), which leaves out `nil None catch throw raise
include`. The SW's own header set includes `<signal.h>`
(`234-spec-warden-scratch/kw/all.c:8`), and its own dumps, searched for those
six:

```
$ grep -c -E "FunctionDecl .* (used )?raise '" ast.txt ast-linux.txt
raise    mac FunctionDecl: 1   linux: 1          (catch throw nil None include: 0 and 0)
ast.txt:5712:        |-FunctionDecl ... <line:77:1, col:14> col:5 raise 'int (int)'
ast-linux.txt:10812: |-FunctionDecl ... /usr/include/signal.h:123:12 raise 'int (int)' extern
```

The FFI's milestone set (`stdio.h time.h sys/socket.h netinet/in.h sqlite3.h
curl/curl.h openssl/...`) has no `signal.h`, so it too measured zero. And the
milestone binds `signal.h`: issue 1069, on the milestone, is *"a service that
stops cleanly is not writable"*, about `SIGINT` on the server, where
`raise(SIGINT)` is how a test signals itself. So the measured demand in
function position is one, `raise`, and its route already exists and was built
by the FFI on both machines (`c_raise`, the compiler's own prescribed
`static inline`). The conclusion the two seats draw (no form owed) is not
moved; the number on the record is, and the SW's prediction (*"no package of
this milestone needs a C function ... spelled as a reserved word"*) is
falsified the day the server's stop is tested with `raise`.

## G14. The sitting's own architecture costs every program over a C package the compile-time handle checks and the leak's location; the SQL seat measured it, the compiler-engineer never saw it, and no seat priced a remedy (items B, C, D; SQL § 2 and § 5)

Reproduced (scratch `r2-handle/`, the SQL seat's `p3/direct.hero` and
`p1/m_afterfinal.hero` with its `p1/core/` copied):

```
$ heroes build direct.hero          # sqlite3_finalize(query) then sqlite3_step(query), one function
error[handle_used_after_end]: `query` was given to `sqlite3_finalize` at line 16, which ended its life, and this reads it again
$ heroes run m_afterfinal.hero      # sql.finalize(@query) then sql.step(query), through the package
panic: a dead C handle reached the argument `statement` of `sqlite3_step` ...
  at core/database/sql.hero:24:12, in sql.step
```

Panel 033 R5 makes an `extern` uncallable across a module, so every package
over C calls C from a wrapper, and the checker decides a handle's life *"one
function at a time"* (the diagnostic's own note, quoted by the SQL seat). So
under every route of item B, every use after end in a program over a C
package moves from a compile error to a runtime abort, and every leak or dead
handle is reported at the package's line, never the program's (the FFI's
F.1 and the SQL seat's § 5 both measured the second). That is §1.12 and
§4.17 at once, a class the thesis counts as a compile error turning into an
abort, and it is a cost of the packages as the sitting shapes them, not of
thin or thick. The CE's report (finished 22:55) predates the SQL seat's
(23:28) and does not mention it; the SQL seat leaves it *"a finding for the
compiler-engineer (unrun)"*. Two routes nobody listed or priced: (a) the
checker infers, per function, that it ends an `@` parameter's life on every
path (a summary of one level), so `sql.finalize(@query)` ends `query` at the
caller and the existing per-function check refuses the next `sql.step(query)`;
(b) the runtime's handle panic names its caller's frame (the stack guard
already names a function through `dladdr`, which is what panel 104's
`-rdynamic` is for, G1). The sitting should say whether it accepts the loss
for every package over C, or which of these it owes.

## G15. Item A's question, asked as the page asks it, is answered by no seat, and its sharpest form is unread: §1.11 names JSON and HTTP among what "comes from C libraries", and the page plans both in Heroes alone (item A; frame 3)

The shared brief asks item A *"as the page asks it"*: *"verified by clang, or
by whoever wrote it, is the first question that milestone's opening sitting
has to answer for a package written in Heroes alone"*
(`docs/roadmap/milestones/M-package-manager.md:27-33`, read: *"a binding is
verified by clang against the header it names, and a standard library is
verified by whoever wrote it"*). Searched in the five reports
(`grep -n -i -E 'whoever wrote|verified by clang|third verifier|heroes alone|written in heroes alone'`):
two hits, both about bindings (SQL:248, FFI:254), which the page already puts
on clang's side. What the seats wrote instead: the CE, *"approve `strings`
and `strconv` first, on the demand measured"*; the SW, that a package placed
in the program's tree keeps *"Heroes ships a minimal runtime and nothing
else"* by its letter, and *"§1.11 with the admission test binds the
packages"*; the historian, *"No precedent tells the sitting where §1.11's line
falls; that is item A's own question"* (historian.md:294). None rules on the
verifier, nor on the third one the first pass named (F4: the Heroes checker,
every `T?` and label, and the package's own `test` blocks in the net).

And the rule's own words are sharper than the question. design.md:490-492:
*"Anything a real program needs — maths beyond arithmetic, files, sockets,
regex, **JSON**, crypto, compression, databases, **HTTP** — comes from C
libraries through the FFI. Writing a broad standard library is out of scope,
permanently."* The milestone's table
(`docs/roadmap/milestones/M-core-packages.md:20-35`, read): *"`encoding/json` |
Heroes alone — `examples/json/`, 671 lines"*, *"`html/template` | Heroes
alone"*, *"`net/http` server | Heroes alone over `net`, `io`, `strings`"*.
Frame 3 says item A rules where a package falls against §1.11, not whether
§1.11 stands, so the sitting owes a sentence that places `encoding/json` and
the HTTP server against *"comes from C libraries"*: an amendment of §1.11's
list, a reading under which a Heroes package over the runtime counts as
coming through the FFI, or the packages leaving the list. No seat wrote it,
and the decision issue cannot answer item A without it.

## G16. Item E: the budget the SW sets is in a unit the instrument the CE approves cannot read, and a ratified ruling stands between them (item E; CE § E, SW § 4)

The SW: *"Per package, at most 1,024 real, measured as above"* (each card
appended to a worktree's specification, `--refresh`, the file restored), and
*"a budget written in vendored tokens would be a third low"* (real runs 1.27
to 1.38 times vendored). The CE approves *"a program of the harness that
writes each card from its module and checks the stored card"*, run by the
net, and says a card's budget is *"stated in the vendored `maximum` and
judged by the harness program, or `measure` learns a third judged path"*.
Read here: `selfhost/measure/pinned.hero:106-111` refuses `--refresh` for
every path but the specification and the contract, *"no ceiling judges it
(panel 123 R5)"*, its reason *"a digest of another file is a digest nothing
cross-checks"*; and the net never refreshes (`tests/harness/suite_spec.hero:169`,
*"`heroes measure spec/heroes-spec.md --refresh`, never by a check: panel
123"*). So a real count of a card exists only by the SW's by-hand route (an
API call per card per edit, by a person), and the net can hold a vendored
count only. The sitting must choose: the budget in vendored tokens (1,024 real
is about 740 to 806 vendored at the SW's measured ratios, an inference), or a
recorded real count per card, which is a third class of judged document and
reopens panel 123 R5. Neither seat chose, and the thin SQL card the SW prices
206 over budget is the first card the choice decides. A further consequence
nobody priced: the SQL seat's preferred third shape publishes the floor with
its own card (*"the full thin layer ... and of a second card"*), which for
`database/sql` puts a thin-sized card (+1,230 real measured for the thin
shape) beside the face's (+923), about 2,150 real for one package against
the SW's 1,024 (arithmetic on the SW's measurements, unrun as a card).

## G17. Item N: the SW's registered expectation for the after-run points the other way from the reader test it was written beside (item N; SW § 8, BLIND)

The SW expects, for its paired run, *"Sonnet `green@1` +15 points, band +5 to
+30"* with *"the specification and the twenty cards"* in the context,
*"`green@1` and not `green@3`, because the packages' promise is the first
try"*. It was written before the reader test ran, on the failure model the
SW's own rule now calls wrong (17 of 36 cell bands hit, *"fewer than half
would say the failure model ... is wrong"*). The reader test is the sitting's
only measurement of a card in Sonnet's first try, and it reads 4/15 with a
card against 15/15 without under G, 2/15 against 11/15 under H (G8), the
first-try metric, the same model, the same card form the after-run would use.
A paired run on cards of today's form predicts a first-try loss on Sonnet, not
a gain, until G8's cause (the card's form, untested) is settled. The
registration is the author's act and comes later; what the sitting owes before
it is the card form the run will carry and an expectation re-derived from
this run.

## G18. Item G's open half rests on a form one seat proposes and no seat priced, and its measured cost is narrower than stated (item G; FFI § G)

The FFI objects to closing question (iii): *"the read-only lend of a `[u8]` is
owed, or the conventions page names the copy as the rule for every send"*,
the lend owing Principle 0's ticket, *"my measured argument for it is the
truncation"*. No seat priced it: the SW's § 5 drafts no sentence for it and the
CE's report has no compiler line for it (`grep -n -i 'read-only\|counted_by'`
over both: no draft, no price). Re-run here from a copy of the FFI's
`send_part.hero` (scratch `r2-bytes/`): `before: 6` / `written: 4` / `after:
4`, as the FFI says. And the shape beside it, the whole buffer:

```
$ heroes run send_whole.hero     # fwrite(@bytes, size: 1, n: bytes.len()...), bytes [104,101,114,111,0,255]
before: 6 / written: 6 / after: 6 first: 104 last: 255
$ od -An -tu1 out-whole.bin
104 101 114 111   0 255
```

so the fill form sends a whole array, a zero byte and a 255 included, with no
copy and nothing cut; the copy is owed only where a part is sent, which is a
socket's retry after a short `send` (the rest of the buffer). The FFI's
prediction, *"every send in them goes through a copy"*, is wider than its run.
What the sitting owes item G: whether a short write's retry, one copy of the
unsent part, is the cost the packages accept (a convention), or whether the
lend earns its ticket, priced by the SW in tokens and by the CE in the checker.

**G15, addendum.** The milestone page states the tension itself, in the
paragraph that introduces Goose's four questions: *"Goose ships a library
written in itself over `extern fn` ..., which § 1.11 refuses here; what
carries across is the method around it"*
(`docs/roadmap/milestones/M-core-packages.md:210-213`). The packages of the
table marked *Heroes alone* or *Heroes ... over* `net`, `io` are that shape,
so the page's own reading of §1.11 refuses what its table schedules, and no
seat reconciled the two.

## G19. The question the sitting did not ask: does the `1.0.0` promise bind the packages' names and contracts? (items B, C; the chain)

Panel 232 (ratified, its paragraph the one `1.0.0` rests on) promises the
language; the reports of 234 mention the promise only for B5 (the CE: *"the
compatibility promise of `1.0.0` would bind every package's API"*), for
`alias` after `1.0.0` (the SW), and for Go's `log/syslog` (the historian:
*"we continue to maintain everything in the library because of the Go 1
compatibility promise"*). In the chain (`ROADMAP.md:212-226`, read),
M-core-packages is row 76, M-web-framework (78) *"composes the core
packages"*, and M-publication-gate (87) publishes `1.0.0`: the packages will
exist when the promise binds. Under B2 with B3, a released program carries its
own copy, so the CE's *"a package change cannot break a released program"*
holds for the corpus; it does not say whether a program written after
`1.0.0` against `core/strings` may find `split_once` renamed in the next copy
it takes, which is the promise's question and frame 2's boundary (*before
`1.0.0` no form enters to keep an older program compiling*). Item C's names
are chosen now under the author's rule; whether they are then permanent is
unasked. Go's own promise excluded its `syscall` package (panel 232's R6,
*"outside the purview of the guarantees made here"*), which is item F's layer
and the packages over C asking the same question.

## G20. Item L: the second function Goose's question asks about is writable today in Heroes over `hero_os.h`, and the one seat that judged L set it aside unbuilt (item L; SW § 6 L)

The page's question (4): Goose's `os` offers *"a write that is never seen
half-written beside the plain one; the runtime here already stages, flushes
and renames ..., so whether spec § 11's `write_file` goes through it is the
question to answer **before a package offers a second function**"*
(`M-core-packages.md:225-229`). The SW answered the first half (*"`write_file`
already stages, flushes and renames where it can"*) and concluded *"no package
owes a second whole-file write"*; no other seat judged L. `hero_os.h` exports
the pieces (`hero_file_stage`, `hero_fs_replace`, `hero_file_unstage`,
`runtime/hero_os.h:268-280`), so the strict function is an ordinary package
function. Built here (scratch `r2-replace/`, 17 lines of Heroes, no C, no
compiler change, no spec token):

```
$ heroes run strict.hero        # write_file "old", replaced(path:, text: "new\n"), read back
new                              (no staged file left: ls -> build refused.hero strict.hero target.txt)
$ chmod 555 locked; heroes run lockedtest.hero     # a writable file in a directory this process cannot write
write_file failed: false, the file now: plain      (written in place, write.c's fallback)
strict replace failed: true, the file now: plain   (refused: the file left as it was)
```

So on the in-place routes the two functions part: the plain one writes, and a
reader racing it can see the file half-written; the strict one refuses with a
code. Whether the `os` package offers it (under the author's rule a different
contract, so a different name) is the decision the page asked for, with the
second function's cost now measured at 17 lines; the SW's L verdict on § 11
(silence) is untouched by it.

**G8, addendum: the helpers do not explain it.** BLIND: *"read beside the use
of the package, the bare arms' advantage is Sonnet writing its own helpers
instead"*. Split by whether the first turn uses the package (`use
core/strings` and a `strings.` call; `right@1` as BLIND defines it, from
`sealed/grades.jsonl`; scratch `r2-blind/`):

```
bare-G  sonnet  using the package: right@1  4/4    not using it: 11/11
bare-H  sonnet  using the package: right@1  5/9    not using it:  6/6
card-G  sonnet  using the package: right@1  4/15
card-H  sonnet  using the package: right@1  2/15
bare-G  haiku   using the package: right@1  8/14   card-G haiku 14/15
bare-H  haiku   using the package: right@1  7/13   card-H haiku 15/15
```

Among Sonnet's answers that call the package, bare 9 of 13 are right at the
first turn and card 6 of 30: the card's loss on Sonnet stands inside the
package's own users, so it is not the hand-written helpers; it is the
positional calls a card brought (G8's count). On Haiku the card raises the
package's users from 15 of 27 to 29 of 30.

**G3, addendum.** The SW's F3, the draft it prices as *"P2 (a platform layer,
a second object)"*, reads *"threads, processes and files from the language's
`hero_os.h`, and sockets and clocks from its platform layer's `hero_net.h` and
`hero_time.h`"*: the FFI's hybrid, not the author's split. So two seats priced
the hybrid under P2's name (the FFI in C, the SW in tokens) and one the
author's split (the CE in compiler sites and the bootstrap line), and the
author's split has no token price.

**G4, addendum: the magnitude behind F.4's second cost.** What the new layer
adds to `print(1)`: 1,536 bytes in the FFI's P1 (239,592 against 238,056,
F.4) and 1,504 in the P1' built here (239,480 against 237,976), about 0.6 %.
What the existing runtime already puts in `print(1)` and never calls: 159,872
bytes, 67 % of the binary (237,976 against the same objects linked with
`-dead_strip` and no `-rdynamic`, 78,104, G1). So the size argument the FFI
measured against P1 is a thousandth of the one the author's split addresses
(G3): "linked only where a group names it" is worth its price for the
process, files, directories, spawning and threads already in every program,
and almost nothing for the sockets and the clock the hybrid moves. No seat put
the two numbers side by side.

*Corrected 2026-11-06, as written*: *"a thousandth"* above is wrong; 1,536 /
159,872 = 0.0096, **about a hundredth**.

**G1, addendum: why the CE's precedent strips and a program's link does not.**
The CE cites *"the compiler already knows the words, for one link of its own:
the value probe"* (`selfhost/cli/value_run.hero`). That link
(`value_run.hero:186-192`, read) is `level_words`, `target_words`, `checks()`,
`-g0`, the objects and `leave_out(target)`, and never
`link_flags.link_flags()`, so it carries no `-rdynamic`; a program's link
(`selfhost/cli/link.hero`, `link_line`) carries it. The precedent is a link
the stack guard never needs to name a function from, which is the difference
G1 measured.

## G21. Routes nobody priced, each with what would have to be true for it to exist (items B, F)

- **B, per-package roots** (the historian's Goose finding, commit b59e41d,
  2026-10-11: *"An `import a.b;` in a library module now resolves under the
  library, not the program"*). What would have to be true: the compiler knows
  which directory is a package's root (a `core/` prefix it reserves, or a
  marker) and reads a module's `use` lines from there. It would make a
  package that uses another testable alone, which the CE measured
  `unknown_module` today (*"a package that `use`s another is never tested
  alone"*), and it is what the historian's B prediction says B4 or B5 would
  need. It reverses panel 099 R1's *"the same from every file of the
  program"*. No seat priced it; the CE priced B4's search and a
  module-relative header lookup, not module-relative `use`.
- **F, the C library's own way of linking on demand.** The historian's survey
  asks how languages and platform libraries split the platform's code; it
  does not survey the mechanism G2 measured, a static archive whose members
  the linker pulls by symbol, which is how C's own runtime libraries are
  commonly packaged for static links. That is a precedent class, unverified
  here (no page read), and it is the one that bears on *"linked only where a
  group names it"* without a compiler predicate.
- **D, the third shape's card** (the SQL seat's *"thick, with its floor
  published and carded ... This beats both"*): no card written, no reader
  test, no token count; G16 gives its arithmetic.

**G2, addendum: Linux arm64 agrees.** In `heroes-linux-arm64:latest`, the
fused C of `print(1)` and `runtime.c` compiled, linked with `-rdynamic` and
the FFI's Linux archive (`F/p2/platform/linux/libhero_platform.a`, copied):

```
hero_time.o hero_net.o
linux hello + archive always linked, -rdynamic: bytes 235304, hero_net_ 0, hero_time_ 0, runs: 1
```

## G22. Item E: the cards the reader test used were curated, and the card route the CE approves cannot curate (item E; CE § E, SW § 1)

The SW noted that each S card *"leaves out `matches_at` and `is_space`, public
in both modules"*; counted here, `S/core-H/core/strings.hero` declares 16
top-level functions and `S/card-H.md` lists every one but those two (`comm
-23`: `is_space`, `matches_at`), and the thick SQL face declares 11 functions
(`grep -c '^function '`) where its card gives the six a program calls (the SQL
seat: *"six functions a program calls, five private helpers"*). The CE's
approved route is *"a program of the harness that **writes** each card from
its module"*, and the language has no visibility (conventions §1.3, the CE's
table: *"not enforced"*), so a generated card lists every top-level function,
helpers included: neither card the sessions read is the card that route would
write. What would have to be true for the two to meet: a face module holds
only what a program calls (conventions §1.3's private modules, today a rule
only *"past one module's size"*, extended to every package), or a marker the
generator reads; the CE's `packages` suite prices §1.3's check as *"every `use
core/...` outside `core/` names a path that has a card"*, which says nothing of
a helper inside a face. Unpriced either way, and the measured card effect (G8)
is of curated cards.

## Claims checked that held, and what was run for each

- **CE**: the one-line bootstrap in 307 tracked files, 351 times, 24 outside
  `docs/` (`git grep` at b36cd92e5); 12 modules of `selfhost/` with an
  `extern "hero_os.h"` group; `print(1)`'s `main` calling `hero_args_set`
  (line 212 of its C); the census of §2.3's refusal, exactly five `use` lines
  in five files (`use sort` of the probe, three `use check/join`, `use
  ir/print`), over every tracked `.hero`; one file reached as `core/strings`
  and `Core/Strings` compiled twice on this Mac (`h_CoreStrings_contains`,
  `h_corestrings_contains`, `ok "contains"` twice, scratch `r2-case/`); P1'
  needing no compiler change and its header edits re-keying the runtime (G4).
- **FFI**: `hero_str_from_bytes` refused to every program, through
  `hero_os.h` as well (`error[ffi_runtime_header]`, scratch `r2-bytes/`), and
  `examples/checksum/base64.hero:31` naming it in a comment only; the
  `#define kw_constant constant` route for a group record's field (prints
  `41`, scratch `r2-field/`); 21 keywords and 23 foreign words in 17 arms;
  the partial send cutting the caller's array 6 to 4; the cache defect (G11);
  an integer descriptor refused both marks (`unread_mark` twice, scratch
  `r2-fd/`).
- **SQL**: a use after `finalize` refused at compile time in one function and
  a runtime panic at the package's line through the wrapper (G14).
- **SW**: `exit` Tier 2 since 0503df194, 2026-08-12 (G12); the held-out
  suite opening by 2026-11-24 (measurement 052 dated 2026-10-24, `v0.5.0`
  2026-10-25 22:47, the README's 30 days, no opening in its register); the base
  rates of measurements 050 (16 and 18 of 30 at turn 1, 27 and 29 by turn 2)
  and 052 (86 and 94 of 120); the variants' $3.44 for 180 generations.
- **BLIND**: 6.21 USD in all (Haiku 106 sessions, 0.4851 USD, 0.0046 each;
  Sonnet 121 sessions, 5.7249 USD, 0.0473 each, where the reading says 0.0477);
  1,010 tool calls, every one `Read` and none outside its session's folder;
  *use the package* 14, 4, 13, 9 and 15 in every card cell; 24 of 30 Sonnet
  card answers with a positional call; `trim` 12 and 12, `split_n` 2 and 1,
  `index` 4, as occurrences in the programs (as sessions' diagnostics:
  `trim` 12 and 9, `index` 3, since a session's messages stop where its
  compile does).

## Corrections to the first pass, 2026-11-06

- **F11**: *"a keyword as a field name has no route at all"* is true of a
  plain `record`, which binds no C field, and false of a group's record, the
  case that binds C: the refusal prescribes `#define kw_<word> <word>` in a
  header of the program's own, built by the FFI on both machines and here
  (`41`). And *"17 more words"* of `declared_word` are 17 arms holding 23
  words (the FFI's count, re-read).
- **F28**: `exit` has been Tier 2 since **2026-08-12** (`0503df194`, the
  bootstrap's library), not 2026-08-18 (G12).
- **F36**: *"already used by `examples/checksum/base64.hero`"* is false: that
  file names `hero_str_from_bytes` in a comment, and no program can bind it,
  through any header (the FFI's run, and here through `hero_os.h`).

## What this pass could not run, and why

- Windows x86-64 and Linux x86-64: not machines of this sitting. So G1 on
  Windows, G2's question of an unreferenced member's `#pragma comment(lib)`
  under MSVC's linker, and P1''s include order under the Windows SDK are
  unrun.
- Any `--refresh` or API call (a card's real count, G16), any web read (the
  archive precedent of G21 is unverified), any paid session.
- The location census on Linux arm64 (Mac only; its 437 row is this Mac's
  artefact), and its extension to macros, which G5 says it owes.
- No prototype in `selfhost/`: neither the in-order check, nor dead-stripping
  with a narrower export (G1), nor per-package roots (G21).
- The SW's fifth and sixth `write_file` shapes were read, not re-run.
- One slip, recorded: a redirection in one command named
  `/tmp/../Users/...`, a path that does not exist (`/tmp` resolves under
  `/private`), so the shell refused it and wrote nothing; the file was written
  in the scratch by the command's fallback. Nothing was written outside the
  repository's root.

## The findings in order of consequence

1. **G1** (F; CE, FFI): the CE's dead-stripping, its answer to the FFI's
   main cost of P1, drops nothing under the compiler's own `-rdynamic`
   (panel 104), on both machines; reproducing it means changing that ruling.
2. **G3** (F; CE, FFI, SW): the FFI and the SW priced a hybrid P2 (only the
   new sockets and clock), the CE the author's split (the process, files,
   directories, spawning and threads too), and the author's split cannot keep
   "only where a group names it" for the arguments every `main` takes.
3. **G15** (A; all seats): item A as the page asks it is answered by no seat,
   and §1.11 names JSON and HTTP among what comes from C while the page
   plans both in Heroes alone, a reading the page itself states.
4. **G8** (E; BLIND, SW): the SW's registered condition on the card fired
   unsaid; the card's effect is +0.33/+0.40 on Haiku and −0.73/−0.60 on Sonnet,
   the only intervals of the run that exclude zero; Sonnet's loss follows the
   card (42 positional calls against 4 without), not a Go habit and not its
   helpers.
5. **G2** (F; CE, FFI, HIST): a route nobody listed, a static archive always
   on the link line, links "only where a group names it" by the linker,
   `-rdynamic` or not, measured on both machines; the existing parts need an
   internal header to become members (25 of 29 do not compile alone).
6. **G5** (I; CE, FFI, SW): the three checks were run on three different
   samples; the FFI's location check, run here over the `run` goldens,
   refuses a passing golden and cannot place 25 bound names, and the SW's I3
   is false as worded on 2056's `EXTRA`.
7. **G14** (B, C, D; SQL, CE): through any C package, a use after end stops
   being a compile error and every leak or dead handle is told at the
   package's line; unpriced, and two routes unlisted.
8. **G7** (B; CE and all): where the canonical tree stands is unanswered,
   and at the repository's root or under `.claude/hooks/` no suite formats,
   probes or tests it.
9. **G4** (F; CE, FFI): P1' built here for the first time: no compiler
   change, not a seed, re-keyed on edit; but it carries the layer into every
   program (G1) and keeps the one unit's Windows include order; the layer is
   1.5 KB of `print(1)` against 160 KB of runtime it never calls.
10. **G11** (B; FFI, CE): the shadowing-header cache defect reproduces,
    unfiled, and it is panel 028 R3's decoy arriving under B2 itself.
11. **G6** (M; CE, SW, conventions): three of the CE's five "comparator
    sorts" are served today by `stable.order` or the built-in `sort`, and the
    package's name `sort` is refused by the CE's own §2.3 extension.
12. **G9** (C; BLIND): no bare answer called `cut`, `lines`, `split_once` or
    `split_lines`; the bare names comparison measured `index` alone.
13. **G16** (E; SW, CE): the card budget in real tokens cannot be judged by
    the net (panel 123 R5); the SQL seat's third shape doubles a package's
    cards.
14. **G17** (N; SW, BLIND): the SW's after-run expectation (+15 on Sonnet's
    first try with cards) points against the reader test's Sonnet result.
15. **G19** (B, C; chain): whether the `1.0.0` promise binds the packages'
    names and contracts was not asked, with the publication gate eleven rows
    after this one.
16. **G20** (L; SW): Goose's second, strict write is 17 lines of Heroes over
    `hero_os.h` today, refusing where `write_file` writes in place.
17. **G10** (D, C; BLIND, SQL, HIST): only the SW's predictions were scored;
    the SQL seat's second is falsified on Q3; the 22 planted mistakes measure
    severity, the run measured their frequency at zero.
18. **G22** (E; CE, SW): the tested cards were curated, and the approved
    route writes every top-level function, helpers included.
19. **G13** (H; SW, FFI): the measured demand is one, `raise` in
    `<signal.h>`, found in the SW's own dump; the SW scanned 20 of 29 words.
20. **G18** (G; FFI): the read-only lend is unpriced, and only a partial send
    needs a copy; a whole buffer passes untouched.
21. **G12** (K; CE, SW): `exit` is Tier 2 since 2026-08-12; the CE's
    confirmation repeats the first pass's F28 error.
22. **G21** (B, F, D): per-package roots, the C library's archive precedent
    and the third shape's card, each unpriced.

## Corrections to this pass, 2026-11-06, written beneath on a re-read

- **G1**: *"every POSIX link the compiler makes carries `-rdynamic`"* is wider
  than the code read: every **program's** link does (`link_line`); the value
  probe's own link does not (G1's addendum).
- **G5**: *"the 433 `run` goldens"* names the CE's count of **groups**
  (`grep -c '^extern "'` over `tests/golden/run`, 433); the census walked the
  377 files holding a group, 979 bound function members, of which 345 files
  have a row. And `fixedbugs-171` has **6** `NOT_FOUND` rows, not 5 (the 25 stand).
- **G10**: *"zero of the five thin-silent ones in 60 answers"* says more than
  was counted: what was counted is that no final Q program of either shape was
  `silent` (0 of 60); a mistake that moves no case's output would not show.
- **G13**: the measured fact is **one declaration**, `raise`, in the SW's own
  header set and dump; that a package of this milestone **binds** it is an
  inference from issue 1069 (the server's `SIGINT`), a question and not a
  premise, so *"the measured demand ... is one"* should read *"the measured
  declarations are one, and whether a package binds `raise` is unmeasured"*,
  and the SW's prediction is falsified only if one does.

