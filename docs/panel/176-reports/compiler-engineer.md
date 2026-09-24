# Panel 176: compiler-engineer

Everything below was run on 2026-09-23, Darwin 25.6.0 arm64, in
`<scratchpad>/176-compiler-engineer/`: the trunk at `a747e5a2` by `git archive`,
plus the untracked `docs/panel/176-briefs/`. The compiler was built from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 3.24 s). Prototype
compilers were built with `./heroes build selfhost/main.hero -o <name>` against
the untouched runtime (63.8 to 76.3 s real). Nothing in the trunk was built, run
or written except this file. Linux and Windows are **UNRUN** for every route.
Line counts come from `diff -u` against the untouched copy, counting added and
removed lines that are neither blank nor comments (test blocks included), and
from the `layout` suite's own unit (`code_lines`, `tests/harness/suite_layout.hero:537`,
replicated in awk and confirmed by running `layout`).

## The verdicts

| route or question | verdict | design.md section |
|---|---|---|
| the set, `acquires a \| b` (panel 175 fixed it) | **approve**, priced | §1.7 |
| **V1**, a second consuming word (prototyped as `transfers`) | **approve**, the one I would land | §1.7, §1.12 |
| **V2**, transfers inside the set | **object** | §1.12 (complete boundary) |
| **V3**, the transfer names its receiver | **object** | §1.7 working criterion |
| **R1**, a reference is its own producer mark (prototyped as `retains`) | **approve** | §1.7, §1.12 |
| **the route nobody listed**: refuse at the CALL a release no mark names | **approve**, as part of the landing | §1.12, the thesis |
| **Q2**, one C name one contract (`contract_differs`) | **approve**, with a measured false refusal named | §1.12 |
| **Q3**, a spelling per mode | **object to any new spelling**; the rule leaves all three modes reachable | §4.9, §1.11 |
| **Q4**, defect 078 | **approve** option 2 (`ffi_` at exit 1), **object** to option 1 | §1.12, `.claude/rules/c-boundary.md` |

**No veto.** Nothing here adds a core construct: design.md Part 5's seven are
untouched, no IR instruction or pass changes (`ir` 24 passed, 0 failed), and
everything the words say is read in the checker and erased into one argument of
an existing runtime call. Nothing breaches the §1.1 ceiling: the largest single
item is 152 lines. Three per-file `layout` ceilings do move (§ 7).

```
verdict: approve (set, V1, R1, call-site rule, Q2 rule, Q4 option 2) · object (V2, V3, Q3 spellings, Q4 option 1) · no veto
section: design.md §1.7 (core plus elaboration), §1.1 (the ceiling), Part 5, §1.12 (robustness, complete boundary)
implementation_cost: selfhost +377 -73 code lines over 17 files (+304 net; +354 in the layout unit),
                     runtime +70 -17 in runtime/parts/alloc.c and +3 -2 in runtime/heroes_runtime.h.
                     Of that, route A alone is selfhost +20 -7 and runtime +43 -19 (panel 175's prototype).
                     0 lines in the lexer, the IR, lowering, descriptors, ownership. ABI 22 -> 23 (route A's bump).
needed_for_self_hosting: no. selfhost/ declares 18 extern functions in 3 files; 0 carry a handle mark.
```

**argument** (116 words). None of this is core: two contextual words, one list,
one count per address and two declaration rules, all read in the checker and
erased before the IR. The prototype gives the brief's verdict on all six
programs, reaches a byte-identical fixpoint, and changes one verdict among the
510 `.hero` files of `tests/golden/` and `examples/`. The strongest reason it is
wrong is measured: the one-contract rule refuses a correct program, `realpath` in
its two modes across two modules, and spends 152 lines catching nothing in the
tree. V2 costs no compiler line and aborts a correct json-c program the moment one
creator's mark omits an adder. V3 buys wording V1 already prints. The price
outside the compiler is ABI 23, 226 re-blessed emissions and three file ceilings.

---

## 1. Baseline, reproduced

`./heroes check`, `./heroes build -O0`, three runs each, stock runtime:

| program | check | run (exit, stdout/stderr) ×3 |
|---|---|---|
| `popen_fopen` | 0 | 0, 28 B / 0 B |
| `vk_cross` | 0 | 0, 8 B / 0 B |
| `xfer_cj` | 0 | 0, 24 B / 0 B |
| `xfer_jsonc` | 0 | 0, 21 B / 0 B (`add 0 fields 1`, `put 1`) |
| `xfer_ssl` | 0 | 0, 46 B / 0 B |
| `refcount` | 0 | **134**, 0 B / 396 B |
| `getter_wrong` | 0 | 0, 17 B / 0 B |
| `getter_wrong_repaired` | 0 | **134**, 5 B / 396 B |

This matches the brief. (My first json-c and OpenSSL builds exited 2 because my
shell passed `--include … --library …` as one word; rerun with the flags split.)

## 2. What I built: `<scratchpad>/176-compiler-engineer/work/prototype-176-final.diff`

Route A as panel 175 fixed it (`<scratchpad>/175-compiler-engineer/prototype-A.diff`,
applied unchanged: the four files it touches are byte-identical at `a747e5a2`),
and on top of it:

- **The set.** `acquires` and `retains` read `ident { "|" ident }`
  (`parse/members.hero` `acquires_marker`, now a loop on `.pipe`, which the lexer
  already has at `token.hero:74`). `ast.Param.acquires` and
  `ast.FunctionDecl.acquires_result` become `[token.Span]`. The emitter passes the
  set as ONE C literal, `"sqlite3_close | sqlite3_close_v2"`: static storage, and a
  C identifier holds neither a space nor `|`, so the runtime splits it by content
  (`hero_handle_names`, no fixed buffer).
- **V1, `transfers`** after a parameter's type: the call hands the handle into
  another value. It arms `bindings_say_which` like `consumes` (`work/v2v3/xonly.hero`:
  `unmarked_handle_producer`), refuses a borrowed argument like `consumes`
  (`t_borrowed.hero`: `consumed_borrowed_handle`), is swept by `marks_are_read`
  (`t_unread.hero`, `transfers` on a `ptr`: `unread_mark`), and is emitted as
  `hero_handle_consumed(h, NULL, NULL)`: the address must be live (a transfer of a
  dead handle is still the stray, before C), and no releaser set is asked.
  `releaser_reads` refuses a mark that names a transfer (`t_named.hero`:
  `unread_releaser` with the note *"`cJSON_AddItemToObject` is marked `transfers`:
  it hands the handle into another value…"*). All four measured, `check` exit 1.
- **R1, `retains <releasers>`**, on a result or on a plain parameter (OpenSSL's
  `X509_up_ref(x) -> int` puts the reference on the parameter): emitted as
  `hero_handle_retained`. Each set entry carries `refs`. `acquires` on a live
  address resets the count to 1 (newest mark wins, as panel 175 fixed), `retains`
  adds one, and a release takes one and keeps the address live while any remain.
- **The route nobody listed: `unadmitted_release`, asked at the call.** Under the
  runtime's releaser set, a call of `F` with `consumes` on a live handle stops the
  program unless the acquiring mark names `F`. So where NO mark in the program names
  `F`, that call can only stop (unless it is only ever passed null), and it is
  refused at check. Asked at the call, not of the declaration: my first draft asked
  of the declaration and refused `font_whole` and `peek_same`, two correct programs
  that merely declare a releaser they never call (measured, § 3). By name, because a
  mark naming `F` for another handle type is already `unread_releaser`. A consumer
  that itself acquires (`freopen`, `realloc`) is exempt and judged at run time. The
  refusals are held in `check/state.hero`'s new `releases` (the `missing_returns`
  precedent) and judged after `bindings_say_which`, so the rule yields to
  `unmarked_handle_producer` rather than repeating it (without the yield,
  `surface-fixtures/handlearming` gained a second diagnostic and `annotations` went
  173/1; with it, 174/0).
- **Q2's rule**, `check/contracts.hero` (§ 4).

The spelling of both words is a placeholder for the seats that test spellings.
**The cost is word-independent**: each is a string compared in one reader
(`bare_marker` for the bare word, `acquires_marker(word:)` for the named one).

### The six programs, and what each prints (`heroes-final`, three runs each)

The brief's programs as written:

| program | check | run | what it says |
|---|---|---|---|
| `popen_fopen` | 0 | **134**, 0 B / 223 B ×3 | `panic: a C handle was given back to `fclose`, and the call that handed it over names `pclose` as what ends it` (before C: stdout is empty) |
| `vk_cross` | **1** | none | `error[unadmitted_release]: `buf_destroy_pooled` ends a `Buf`'s life and no call handing one over names it, so this call would stop the program before C runs`, at the call, with the note naming both repairs (`transfers`, or `acquires <releaser> \| buf_destroy_pooled`) |
| `xfer_cj`, `xfer_jsonc`, `xfer_ssl` (with `consumes`) | **1** | none | the same error on `cJSON_AddItemToObject`, `json_object_object_add`, `SSL_set0_rbio`, the note pointing at `transfers` |
| `refcount` (with `acquires` on `obj_ref`) | 0 | **134**, 0 B / 396 B ×3 | the stray message: `acquires` is not a reference |
| `getter_wrong` | 0 | 0, 17 B / 0 B ×3 | nothing |
| `getter_wrong_repaired` | 0 | **134**, 5 B / 396 B ×3 | the stray message, before the second `g_close` reaches C |

The same programs spelled with the new vocabulary, one line changed each:

| program | the one line | check | run ×3 |
|---|---|---|---|
| `xfer_cj_t` | `item: Json transfers` | 0 | **0**, 24 B / 0 B |
| `xfer_jsonc_t` (real json-c 0.19) | `val: Json transfers` | 0 | **0**, 21 B / 0 B, `add 0 fields 1`, `put 1` |
| `xfer_ssl_t` (real OpenSSL 3) | `rbio: Bio transfers` | 0 | **0**, 46 B / 0 B |
| `refcount_r` | `obj_ref(o: Obj) -> Obj retains obj_unref` | 0 | **0**, 27 B / 24 B (C's own `[C] freed at refcount 0`) |
| `vk_cross_pooled` | adds `buf_make_pooled() -> Buf acquires buf_destroy_pooled` | 0 | **134**, 0 B / 240 B, `given back to `buf_destroy_pooled` … names `buf_destroy`` |

So `popen_fopen` and `vk_cross` are refused before C runs (one at run time before
the call, one at check), the three transfers run at 0, `refcount` runs at 0 once
the reference is declared as one, and `getter_wrong_repaired` is still caught
before C. **The brief's `refcount.hero`, still spelled `acquires`, stays at 134**,
which is R1's point: counting only where a declaration says a reference was added
is what keeps `getter_wrong_repaired` from becoming a raw double free.

R1 on a parameter, `work/v2v3/upref*.hero` over an `X509_up_ref`-shaped header:
two references and two frees run at 0; a third free is 134 with the stray message
after C's `freed at refcount 0` and before the third C call; one free short is 134
at exit, `1 C handle(s) never given back`. That last sentence still says *"every
call marked `acquires` owes one marked `consumes`"*, which is imprecise under
`retains` (a wording cost, one string).

### The shapes beside them (panel 175's reproducers and shapes, three runs each)

| shape | route A (175) | `heroes-final` |
|---|---|---|
| `popen_darwin`, `oneacq`, `outacq` | 134 at run time | **check 1**, `unadmitted_release` |
| `xacquires/main` | 0 | **check 1**, `contract_differs` (§ 4) |
| `font_whole`, `peek_same` (a releaser declared, never called) | 0 | 0 |
| `font_part`, `peek_other` | 134 | **check 1**, `unadmitted_release` |
| `close_v2` (one-name mark, closed with `sqlite3_close_v2`) | 134, a correct program | **check 1**, the note names `acquires sqlite3_close \| sqlite3_close_v2` |
| `close_v2_set` (that mark written) | not writable | **0**, 20 B |
| `popen_freopen` | 134 | 134 |
| `xmod`, `fixed_reissue`, `fixed_unseen`, `reissue`, `reissue_unseen`, `transfer`, `freopen` | 0 | 0 |

Every check-time refusal in this table is a program route A already aborted at run
time. That is the rule's claim, and no row contradicts it.

## 3. Price per route

Measured per file for the whole prototype (`heroes-final` against `a747e5a2`):

| file | + / − | layout unit | its ceiling |
|---|---|---|---|
| `selfhost/parse/members.hero` | +23 / −14 | 298 → **307** | §11's 300: **breached** |
| `selfhost/parse/tails.hero` | +7 / −2 | 288 → 293 | 300 |
| `selfhost/ast.hero` | +8 / −4 | 527 → **530** | DECIDED 527: **breached** |
| `selfhost/check/marks.hero` | +10 / −8 | 105 → 107 | |
| `selfhost/check/acquiring.hero` | +51 / −15 | 249 → 291 | 300 |
| `selfhost/check/consuming.hero` | +52 / −4 | 114 → 170 | |
| `selfhost/check/state.hero` | +8 / −1 | 192 → 201 | |
| `selfhost/check/walk.hero` | +1 / −1 | 1863 → 1863 | DECIDED 1870 |
| `selfhost/check/contracts.hero` (new) | +150 | 0 → 175 | |
| `selfhost/checker.hero` | +3 | 103 → 107 | |
| `selfhost/emit/handle_traffic.hero` | +34 / −8 | 130 → 167 | |
| `selfhost/emit/decls.hero` (the stamp) | +2 / −2 | 269 → 269 | |
| `selfhost/print/fmt.hero` | +10 / −5 | 1172 → **1177** | DECIDED 1175: **breached** |
| `selfhost/print/dump.hero` | +10 / −5 | 204 → 209 | |
| `selfhost/resolve/{qualified,state,types}.hero` | +8 / −4 | | |
| **selfhost total** | **+377 / −73** | **+354** | |
| `runtime/parts/alloc.c` | +70 / −17 | wc 610 → 683 | |
| `runtime/heroes_runtime.h` | +3 / −2 | wc 602 → 603 | |

Route A alone, the same counter on panel 175's files: `emit/handle_traffic.hero`
+18 −5, `emit/decls.hero` +2 −2, `alloc.c` +41 −17, the header +2 −2 (matching
that report's 18 + 2 and 41 + 3). This sitting on top of A: `alloc.c` +34 −5 and
the header +1.

Per route, **attributed by reading the hunks** (the per-file totals above are the
measurement; the split is my reading of them, and it sums to 321 against the
measured net of 304 because some edited lines serve two routes):

| route | compiler | runtime | where it lands |
|---|---|---|---|
| **the set** | ~33 | ~18 | `members.acquires_marker` (the loop), `acquiring.releaser_reads` (one mark per name), `handle_traffic.listed`, `fmt`/`dump` `acquires_suffix`; `alloc.c` `hero_handle_names`, `hero_handle_meet` |
| **V1** | ~26 | **0** | `ast` 1, `members` 1, `marks` 3, `acquiring.ends_one` and its note ~17, `handle_traffic.before_call` 4, `fmt`/`dump` inline |
| **R1** | ~31 | ~13, +1 header | `ast` 2, `members` 4, `tails` 4, the four `resolve` constructors 4, `acquiring` ~9, `handle_traffic.entry_for` 4, `fmt`/`dump` 2 each; `alloc.c` `refs`, `hero_handle_begun`, the count in `consumed`, `hero_handle_retained` |
| **call-site rule** | ~79 | 0 | `consuming` ~48, `state` 8, `acquiring.releases_are_admitted` ~22, `checker` 1, `walk` 0 (one line changed in place) |
| **V2** | **0** beyond the set | 0 | measured: `work/v2v3/xfer_cj_v2.hero`, `acquires cJSON_Delete \| cJSON_AddItemToObject`, checks and runs at 0 on `heroes-final` |
| **V3** | **UNRUN** estimate: V1 plus ~35 | **UNRUN** estimate ~30 to 40 | the receiver resolved as a sibling (the shape `check/lend_extent.hero` uses for `counted_by`), passed by `handle_traffic`, a holder field and a cascade in `alloc.c` |

**What each does to the grammar, the formatter, `heroes grammar`, `check/marks.hero`
and the ABI.**

- *Grammar.* `CParam` becomes `[ "owned" ident ] [ "consumes" | "transfers" |
  "acquires" Names | "retains" Names | "borrows" ]`, the result of `Member`
  `[ "acquires" Names | "retains" Names | "borrows" ]`, with `Names = ident { "|"
  ident } .`. V2 needs only `Names`; V3 would add a receiver after `transfers`. The
  lexer: 0 lines.
- *`heroes grammar`*: 0 compiler lines. It prints the productions from
  `spec/heroes-spec.md` at run time (`cli/grammar.hero:104-134`), so the spec's
  text is the whole change. `grammar` reads 7 passed, 0 failed on the unchanged
  spec. **Its closing sentence is already false** (§ 9).
- *The formatter*: `print/fmt.hero` and `print/dump.hero` +10 −5 each, printing
  the set, `transfers` and `retains`; `canonical` 2 passed, 0 failed after `heroes
  fmt --in-place` on every touched file.
- *`check/marks.hero`*: +10 −8, the sweep extended to both words.
- *The ABI*: 22 → 23 is route A's bump, and `hero_handle_retained` plus the count
  ride it: one bump for the whole landing, measured (§ 7).

### Why V1 and not V2, measured

V2 costs no compiler line, and that is its whole case. Against it:

- **It aborts correct programs.** A transfer is valid for any live handle of its
  type in every library in the brief (any `json_object` may be added to an object,
  any BIO handed to an SSL). V2 checks it per ACQUISITION instead. `work/v2v3/jsonc_v2_miss.hero`,
  real json-c, `json_object_new_array()` whose mark lists only `json_object_put`,
  then the array added to an object: **134, 0 B / 248 B, three of three**, *given
  back to `json_object_object_add` … names `json_object_put`*. The same program
  under V1, `jsonc_v1_same.hero`: **0**. I could not construct a program where V2's
  extra check catches something true.
- **It puts the cost in every binding.** Counted from the real header
  (`grep -ohE '\bjson_object_new_[a-z0-9_]+\s*\(' json_object.h | sort -u`): **12
  creators**, and 5 adders. Under V2 each creator's mark grows by 136 bytes, 1632 in
  all; under V1 each adder grows by ` transfers`, 10 bytes, 50 in all. Bytes, not
  tokens; tokens are other seats' instrument.
- **It takes a refusal away from the checker.** With no word for a transfer,
  `releaser_reads` cannot refuse a mark naming only transfers, which is a binding
  that never releases.

### Why not V3

§1.7's working criterion: it adds a special case (receiver resolution, a holder
per entry, a cascade on release) and removes none, for diagnostic wording V1
already has. And the cascade must admit a shape I measured as correct:
`work/v2v3/nested.hero`, a leaf transferred into a child the program only BORROWS
(`cJSON_FirstChild(...) -> Json borrows`), then the root deleted: **0, three of
three, and 0 under `--sanitize` with 0 AddressSanitizer lines**. A receiver-keyed
cascade finds that receiver not live. **UNRUN**: V3 was not built.

## 4. Question 2 as a checker rule: `check/contracts.hero`, 152 lines

**What is compared.** Two `extern` declarations of one C name (two modules
necessarily, since one module is `declared_twice`) must agree on every position
they share **at one type**: the result when the written result types are the same
interned type, and each parameter whose written type and `@` agree. Compared are
all the marks: `lent`, `owned` and its freer, `consumes`, `transfers`, the set of
`acquires` or `retains` **as a set** (sorted, so `a | b` equals `b | a`), `borrows`,
and `counted_by` by the POSITION of its sibling, since the two may label it
differently. Later declarations are compared with the first one of that name, in
one pass over the declarations with a `{str: i64}` map.

**Why `twoarity` stays legal.** Its position 1 is `i64` in one module and `cstr`
in the other, a different argument, so it is not compared; position 0 and the
result agree. Measured: `check` 0, `run` 0.

**What it says when two modules name two sets for one acquisition.** Not that one
of them is false, which would be untrue of `xmod-xacquires` (both releasers
`free`). The prototype prints:

```
error[contract_differs]: `h_open` is declared at xmod-xacquires/main.hero:4 too, and the two say different
things about its result: there `acquires h_close2`, here `acquires h_close` — one call's result is ended by
one set of calls, and each module has written part of it
  note: write `acquires h_close | h_close2` in both, which is every call either module says ends it — and
  take out of both any that does not
```

and the program with the union written in both (`work/q2/xacq-union/`) checks,
builds and runs at 0, three of three. Required as equal SETS, because under the
runtime set a handle acquired in one module and given back in the other is
checked against the first module's set.

**Measured.**

| program | `heroes-p2` (no rule) | `heroes-final` |
|---|---|---|
| `xmod-lent` | check 0 | **check 1**: `there `no mark`, here `lent`` |
| `xmod-owned` | check 0 | **check 1**: `there `owned other_free`, here `owned my_free`` |
| `xmod-xacquires` | check 0 | **check 1**, the union note above |
| `twoarity` | 0 | 0, run 0 |
| all 510 `.hero` files under `tests/golden/` and `examples/` | | **fires on 0** (same diagnostic codes as `heroes-p2` on every file) |
| `selfhost/main.hero` (18 externs, none declared twice) | | 0 errors |

**The strongest reason it is wrong, measured.** `work/q2/realpath/`: one module
binds `realpath(path, resolved: ptr) -> cstr owned free` and passes `nullptr`
(C allocates the answer and the caller frees it); another binds it unmarked and
passes a buffer (C writes into the buffer and hands it back). That is a correct
program: stock `check` 0, `run` 0, three of three, printing `/private` twice.
Under the rule it is **`contract_differs`, exit 1**. A contract that depends on an
argument's VALUE is not a mark on one position, so the rule refuses the two
modules that each state one mode. The escapes are one mode per program, or a
header-only shim that fixes the argument (§ 5). A first attempt at this program
was refused by a rule that already ships, `owned_freer_called` (`free` named as a
freer may not also be called), so it was rerun with the buffer never freed.

**Conservative alternative, recorded** (CL-040): refuse only two values of ONE
word (two freers, two sets, `consumes` against `transfers`) and admit a mark
against no mark. Its measured miss: `xmod-lent` returns to 0.

## 5. Question 3 against the rule

**All three modes stay reachable in one program**, with the declaration panel 171
already chose for `examples/ledger/db/sqlite.hero:115`: the pointer modes
unmarked (`text: cstr`, true of both, the bytes going as a lease) in one module,
and the function mode in another, where `text: ptr` and the destructor's function
type differ, so nothing is compared. `work/q3/three/`: `check` 0 under
`heroes-final`, and three of three runs print `transient=copied by sqlite`,
`static=kept by sqlite`, `function=0`.

**What the rule forbids** is one module per pointer mode where one writes `lent`
(`work/q3/permode/`: stock `check` 0, `heroes-final` `contract_differs`). That is
the right refusal: that declaration does not restrict the destructor, so `lent` is
false of some call it admits.

**And no declaration rule reaches the brief's worst case.** `lent_static.hero`
prints `stored=XXXXXXXXXX…` at exit 0, three of three, under the stock compiler and
under `heroes-final` alike: one module, one declaration, and the fault is which
constant the CALL passes.

**The spelling that makes `lent` true of every call must fix the destructor where
it is declared.** Three candidates, priced:

- **A header-only C shim**, 0 compiler lines, works today. `work/q3/shim/bind.h`
  declares `static inline int bind_text_copy(...) { return sqlite3_bind_text(...,
  SQLITE_TRANSIENT); }`, and the Heroes group declares `bind_text_copy(..., text:
  cstr lent, length: i32)`. Under `heroes-final`, with the static and function
  modes in the same program: `check` 0, three of three runs, and the lend reads back
  `hello, sqlite hello, sqlite hello, sqlite` in the same allocate-then-overwrite
  pattern where `lent_static` reads `XXXX`. design.md §1.11 (line 564) and §4.19
  (line 2309) already sanction *a thin C shim* for the hard cases; a `static inline`
  one needs no compile step, the part §4.19 leaves undecided.
- **A trailing pinned argument** in the declaration (`destructor: ptr` fixed to
  `SQLITE_TRANSIENT`): **UNRUN** estimate 40 to 60 lines (parser, the arity check,
  the emitter appending the constant, `fmt`/`dump`, the spec). It must be trailing,
  or every rule that indexes `params[position]` by the argument's position shifts
  (panel 094 priced that shape at 250 to 400 lines for `...`). **Object**: it reads
  as a default parameter value, which §4.9 (line 1511) refuses, and Principle 0
  admits neither need nor a measured effect.
- **The rename clause** (`tag`, panel 094 R3): **object as an answer to Q3**. It
  names a mode but does not pin the argument, so `lent_static`'s call is just as
  writable under the second name.

## 6. Question 4: defect 078, priced both ways

**Why the qualifier is dropped.** An `owned` `@cstr` cell is declared `const char
*` (the emitted C's line 67 for `s4_cstr_owned.hero`) and handed over as
`(char **)&cell` (`emit/ops.hero:314-324`), with the probe spelling it `char *`
(`emit/extern_probe.hero:275-278`). The comment says why: `sqlite3_exec`'s
`char **errmsg` refuses a `const char **` under `-Werror`. That is a premise about
the world, *every header spells an owned out-string `char **`*, and `r1.h`'s
`fill_out(const char **out)` breaks it: clang refuses the probe (`2:107`) and the
call (`7:20`), and because `emit/ffi_mutable.hero:90-100` steps aside for an owned
`@cstr`, nothing claims the line and the build is `internal error`, exit 2.

- **Option 2, refuse with an `ffi_` diagnostic at exit 1**: prototyped,
  `work/prototype-176-q4.diff`, **+14 −1 code lines in `emit/ffi_mutable.hero`**,
  where the reader already stands. On `s4_cstr_owned.hero`:
  `error[ffi_owned_const_cell]: `out` of `fill_out` is marked `owned`, and the header
  says `const char **` …`, on the declaration, **build exit 1**. On real `sqlite3.h`,
  `@tail: cstr owned sqlite3_free` (a false mark: `pzTail` points into the caller's
  SQL): stock **exit 2**, `internal error`; option 2 **exit 1**, the same diagnostic.
  The rare true case keeps a spelling: `@out: cstr` with `free_out(p: s)` called by
  the program checks, builds and runs at 0 three of three, and 0 under `--sanitize`.
- **Option 1, emit the header's own qualifier**: **UNRUN** estimate 40 to 60 lines.
  The qualifier has to be learned from clang, in the build's rounds the way
  `struct_tags` is (43 mentions in 9 files), or from the `__typeof__` dump
  `cli/header_types.hero` already writes; then the cast at `emit/ops.hero:324`, the
  probe's spelling at `emit/extern_probe.hero:277`, and a cast on the freer's
  argument. **Object on §1.12**: it would BUILD the false `pzTail` mark and free an
  interior pointer (inference: option 1 was not built).

## 7. What moves, the suites, the fixpoint

One at a time, `heroes-final run tests/harness/main.hero -- ./heroes-final <suite>`:

| suite | result | why |
|---|---|---|
| the compiler's own tests | **675 passed** | |
| `check` | 135 / 0 | |
| `annotations` | 174 / 0 | 173 / 1 before the yield (§ 2) |
| `fixes` · `canonical` · `unsupported` | 25 / 0 · 2 / 0 · 15 / 0 | |
| `runtime` · `ir` · `grammar` | 8 / 0 · 24 / 0 · 7 / 0 | no second table, so no new `SHARED_BY_DECISION` |
| `corpus` | **55 / 0** | no example changes verdict |
| `run` · `determinism` | 137 / 1 · 167 / 1 | **one case**, below |
| `emit` | 1 / 7 | each `.expected` differs by the stamp line only (2 diff lines each, diffed with the Q1 build; none of the 7 carries a mark) |
| `emission` | 258 / **229** | 226 re-emitted (211 stamp only, 15 stamp and handle lines), 1 program refused, **2 red before any change** (§ 9) |
| `layout` | 1 / 1 | three ceilings, below |

**The one case** is `tests/golden/run/abort-handle-borrows-that-gives-away.hero`:
a producer marked `borrows` that in fact gives the handle away. It is now
`unadmitted_release` at check, so it moves to `tests/golden/check/` as a
`fixedbugs-` case (`.claude/rules/verification.md`'s precedent). **That scores a
registered prediction**: `runtime/parts/alloc.c:289-292`, registered 2026-09-15 in
`bd03f8d9`, says *"the next rule to close one of those two will come for
`borrows`, and the case that goes red will be
`abort-handle-borrows-that-gives-away`"*. It comes for `borrows` only where no other
producer names the releaser; a lying `borrows` beside an honest `acquires` is still
the runtime's to catch.

**Over the whole tree**: `heroes check` on all 510 `.hero` files under
`tests/golden/` and `examples/`, stock against `heroes-final`, differs on **exactly
one file**, that one. Panel 175's critic counted the declaration-level form at 7
functions in 5 golden files; asked at the call, it is one.

**Ceilings**: `ast.hero` 530 against DECIDED 527; `parse/members.hero` 307 against
300, and the seam is real (the five marker readers, about 70 lines, as their own
module); `print/fmt.hero` 1177 against DECIDED 1175. `check/acquiring.hero` ends at
291 of 300.

**The fixpoint**: `heroes-final` built the prototype's source, that compiler built
it again, and the two `--emit-c` outputs are **byte-identical** (26,487,900 bytes,
stamped ABI 23); that C compiles as a seed (`clang -I runtime … runtime/runtime.c`)
with 0 errors.

## 8. The time

Item 6 asks for it where a route adds work to every call. Only one changed line
sits on the unmarked call path (`check/walk.hero:1323`, now passing `decls` and an
index), and the two program-wide rules loop once over the declarations. Timed
anyway: `/usr/bin/time -p ./<c> check <a747e5a2's selfhost>/main.hero`, the same
input for every compiler, alternated, and in both orders. `heroes-base` is
`a747e5a2` built the same way as the prototype.

| compiler | real (s) | user (s) |
|---|---|---|
| `heroes-base` | 19.83, 19.32, 19.55, 19.36, 19.65, 19.53, 19.44, 19.40, 19.41 | 19.25 to 19.48 |
| `heroes-final` | 18.27, 18.20, 17.71, 17.75, 17.66, 18.02, 17.74, 17.73, 17.90 | 17.62 to 18.05 |
| `heroes-iso` (base plus only the `walk.hero:1323` change) | 20.17, 19.34, 19.98 | 19.30 to 19.60 |

`real` stays within 0.6 s of `user + sys` in every run, so none was waiting. The
machine was NOT idle for the first six runs: `ps` showed a Chrome renderer at 121%
CPU and `duetexpertd` at 83%, which are not mine to stop. The fifteen runs after
them saw under 12% of other load, and agree. **The prototype is not slower. It measures
about 8% faster, and the call-site change is not why** (`heroes-iso` equals the
base). The cause is unmeasured. A candidate, as a question: every `Param` and
`FunctionDecl` used to build a failure value with two strings for its absent
`acquires`, and now holds an empty array.

## 9. Found in what ships, each reproduced

- **A double release before any acquisition dies at 133 with zero bytes.**
  `hero_handle_consumed`'s `if (hero_handle_cap == 0)` branch
  (`runtime/parts/alloc.c:428-433`) counts the stray and **returns**, so C runs.
  `work/cap0/cap0.hero` (a producer marked `borrows`, released twice): **133, 0 B /
  0 B, five of five**. The same double release after one acquisition has allocated
  the set (`cap1.hero`): **134, 396 B, five of five**. So defect 071's *abort before
  C* depends on the program's history. Replacing that `return;` (line 432) with
  `hero_handle_report_stray(1, h);` gives **134, 396 B, five of five** for both.
  Route A edits this function, so the line rides the same change. The `run` suite
  on that runtime is **UNRUN**. I searched `docs/work/DEFECTS.md` for `cap == 0`,
  `cap0`, `never acquired`, `no handle was ever`, `empty set` and `set was never`:
  no hit, so it looks unfiled, as far as those words reach.
- **`heroes grammar` states a false count.** It prints *"the six contextual words —
  `as`, `link`, `package`, `tag`, `partial`, `owned`"* (`cli/grammar.hero:156`, run
  and read), while `parse/members.hero` and `parse/tails.hero` also read
  `acquires`, `borrows`, `consumes`, `counted_by` and `lent`. This sitting would add
  two more. A literal list of words the parser owns is a premise that expires in
  silence; derived from one table it is about 12 lines. `DEFECTS.md` has no hit for
  "six contextual" or "contextual words".
- **`emission` is red by two at `a747e5a2`, before any route.** Stock compiler and
  runtime: **486 passed, 2 failed**, *nothing blessed* for
  `run-fixedbugs-c-writes-over-a-leases-header.c` and `…-strings-header.c`, the two
  goldens `4324daab` added. So the full net on the trunk is not green today.
- **Three gaps in my own prototype, each measured, each a few lines at landing.**
  A type consumed only by a `transfers` call, with an unmarked producer, gets
  `unmarked_handle_producer` naming an empty taker (``` `` takes one back ```,
  `work/v2v3/xonly.hero`). The borrowed-argument refusal says a transfer *"ends the
  life of what it is given"*, which is false of a transfer (`t_borrowed.hero`; the
  sentence in `check/consuming.hero` needs a branch). A mark naming only a transfer
  reports its one mistake twice, `unread_releaser` and then `unadmitted_release` on
  the real releaser (`t_named.hero`), so the call-site rule owes a second yield.

## 10. My own panel 175 prediction, scored against this

It said that at A's landing `xacquires` exits 0, `selfhost/` gains at most 30
non-test lines, and `emission` reads exactly 227 failed. **If route A lands with
this sitting's resolution in one commit, two halves are falsified**: `xacquires`
becomes `contract_differs` at check (the rule is a checker rule, which the
prediction's own last sentence expected), and `selfhost/` gains about 300 net lines,
not 30. Route A alone stays at +13 net. `emission` would read 229, or 227 once
lane 076's two are blessed.

## prediction

**Checkable at the commit that lands panel 176's resolution, inside
M-agreed-retention.** `heroes check` over the 510 `.hero` files that
`tests/golden/` and `examples/` hold at `a747e5a2` changes verdict on **exactly
one**, `tests/golden/run/abort-handle-borrows-that-gives-away.hero`, which becomes
`unadmitted_release`; `contract_differs` fires on none of them. And that commit's
`git diff --numstat -- selfhost/` adds **between 250 and 450** non-test lines, with
`check/contracts.hero` the largest new file. If a second file changes verdict, the
landing carries a rule broader than the one priced here, and it will be the
call-site rule asked of declarations instead of calls.

## condition

- **V1 over V2**: a real library where a transfer is valid for only some
  acquisitions of its type, so V2's per-acquisition check has a true positive.
- **R1**: a real binding where `retains` hides a double release that `acquires`
  catches.
- **The call-site rule**: a correct program, in the tree or over a real library,
  that the rule refuses and route A runs. None in the 510 files or the 31 programs
  of § 2.
- **The Q2 rule**: a census showing value-dependent contracts bound in two modules
  are common (the `realpath` shape). Then I would move to the conservative
  narrowing and accept `xmod-lent` at 0.
- **Q4**: a real library documenting an owned `const char **` out-parameter. Then
  option 1 is owed as well, beside option 2's refusal of the false case.

## Unrun

Linux and Windows for every route. V3, Q3's pinned argument and Q4's option 1:
estimates, not builds. The `run` suite on the `cap == 0` repair. The
`xonly` wording fix. Any reader-side effect of `transfers` or `retains`. The cause
of the 8% speed-up.

## Files

- The prototype: `<scratchpad>/176-compiler-engineer/work/prototype-176-final.diff`
  (`selfhost/` and `runtime/`), built as `heroes-final`; Q4 option 2:
  `work/prototype-176-q4.diff`, built as `heroes-q4`
- Programs: `work/q1/` (the brief's six, plus `_t`, `_r`, `vk_cross_pooled`),
  `work/shapes/`, `work/q2/` (`xmod-*`, `xacq-union`, `realpath`), `work/q3/`
  (`three`, `permode`, `shim`), `work/q4/`, `work/v2v3/`, `work/cap0/`
- Suite transcripts: `work/final_suite_*.txt`, `work/suite_emission_base.txt`
