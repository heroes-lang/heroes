# 116 — Where the `owned` release lives

Date: 2026-09-06 · **soundness lane, two judges** · convened by author instruction
(*"panel"*) on a question panel 109 left open.

Status: **provisional — author ratification pending.** See § The resolution.

Lane: **soundness**. The question changes no surface, no diagnostic and no spec
token: `owned` is already in the language (panel 109, ratified 2026-09-04) and
already in the spec (M-declared-freer step 2, `3fb5935b`). What is open is
where its release is BUILT. The three judges who do not compile have nothing to
be differentiated about here, and the two who compile are the whole question.

## The proposal, as put

> Panel 109's condition 1 requires *"the freer called **by name** in a generated
> per-freer release, verified by the probe (arity, one pointer parameter); never
> through `void (*)(void*)`, never cast"*, and condition 3 *"the NULL guard is
> emitted, never assumed of the library"*. **The sitting did not rule where that
> release is built.**
>
> **(a)** a new `CastKind` case `cstr_to_ptr` in `selfhost/ir.hero`, and the
> whole release built in the IR: hold the returned `cstr` in a slot, call the
> library's `validated`, branch on NULL, call the freer. `--dump-ir` shows it.
>
> **(b)** a per-freer release generated in C by the emitter, wrapping the call
> as `t3 = h_owned_<freer>(<the extern call>);`. No IR change.

The coordinator's brief recommended **(a)** and was wrong about why, which the
FFI seat's first measurement showed.

## The verdicts

| seat | verdict | what it measured |
|---|---|---|
| ffi-pragmatist | **veto on (a)** as specified; **approve (b)** with one condition on the release's parameter type | `ptr` emits as `void *`; at `void *` clang accepts **10 of 10** reachable freers **silently**, at `char *` it rejects **6 of 10** naming the header's own type; `fclose((FILE *)strdup(…))` is **exit 139**; route (b)'s stated obstacle is not one — the `str?` struct is in **every** unit and `hero_str_try_from_cstr` is in the runtime header, and the release compiled, linked and ran for `free`, `sqlite3_free` and `fclose` |
| compiler-engineer | **veto on (b)**; **approve (a)** with three conditions | adding the case names **exactly two** non-exhaustive sites and no others; the release builder is **76 code lines** in a new module; `flatten.hero` **1104 → 1084** with a three-site collapse, and **no `DECIDED` row moves**; route (b)'s `h_library_validated` is **not emitted** — `call to undeclared function`, exit 2 on a legal program — and its only other arm is the one **design.md:476-480 already refused** |

**Two vetoes, in opposite directions.** Neither is a price and neither is
smoothed over. What follows is why both are right, and why the resolution is
something both seats had already written down.

## The FFI seat: the cast is the blinding agent

Ten real freers, probed in two shapes under the compiler's own fourteen flags,
Apple clang 21.0.0, against the real `sqlite3.h`, `curl/curl.h`, `stdio.h` and
`netdb.h`:

| probe parameter | outcome |
|---|---|
| `void *` | **10 of 10 accepted silently**, zero diagnostic lines even under `-Wextra` |
| `char *` | **6 of 10 rejected**, each by clang naming the header's real type |

The rejection reads `error: incompatible pointer types passing 'char *' to
parameter of type 'FILE *' (aka 'struct __sFILE *') [-Werror,-Wincompatible-pointer-types]`,
and that flag is already `-Werror` in `selfhost/cli/flags.hero`. The four that
pass at `char *` are the ones that are genuinely `void *`-shaped — and the seat
**enumerated the reason from the header rather than from memory**: `curl.h`
writes `typedef void CURL;`, so `curl_easy_cleanup(CURL *)` **is** `void *` at
the C level and no probe can catch a wrong freer there. **Six of ten, not ten of
ten**, stated rather than left to be discovered.

The stakes, run rather than argued: `fclose((FILE *)strdup("a string SQLite
gave me"))` is **exit 139, SIGSEGV**.

So `(void *)p` silences all four of `free`, `sqlite3_free`, `fclose` and
`sqlite3_close`; `(char *)p` strips the `const` — which is the only thing that
actually needs stripping — and keeps the pointee, which is what lets clang go on
checking. **Route (a) as proposed routes the freer call through `ptr`, and `ptr`
emits as `void *`.** It would buy a new IR case in order to go blind.

The seat also cites design.md:2246-2249, which names this exact mechanism one
level up: the wrong conversion *"compiles clean **with an explicit cast**"*,
where §4.19 promises it is inexpressible.

**Two more measurements this seat took, both re-run rather than trusted.**
`fclose(NULL)` **survives** on this Mac, and a release with the NULL guard
**deleted** exits 0 for both `free` and `fclose` — *including under ASan and
UBSan*. No configuration of this machine can see a missing guard, so the guard
must be unconditional and emitted from one reviewable template, and a green
local run can never justify its absence. And `-fno-strict-aliasing` is already
in the flag list, so aliasing is not the hazard; `const` is.

## The compiler seat: route (b) is neither core nor sugar

design.md §1.7 admits two kinds of thing: implemented in checker **and**
lowering **and** backend, or erased on the way into the IR. **Route (b)
implements `owned` in the checker and the backend and skips the lowering** — a
second elaborator living inside the printer, against Part 5's own *"a separate
pass is what keeps the emitter a printer"*.

Measured, that costs one of two things and both are refused elsewhere:

- **Call `h_library_validated`.** A program that does not itself call
  `validated` emits it **zero times**. Patched into a per-freer static and
  compiled: `error: call to undeclared function 'h_library_validated'` — **exit
  2 on a legal program**. `selfhost/emit/builtins.hero`'s `reachable()` computes
  reachability **by walking `.call`/`.func_ref` instructions in the IR**, and
  route (b)'s release is a string the walk cannot see. That module's own
  docstring records this defect having already shipped once, in those words.
- **Compose the `str?` in raw C.** It works — the seat compiled and ran it,
  clean under `-fsanitize=address,undefined` and the leak counter. It is also
  **exactly the arm design.md:476-480 refused**, in the same words, for the same
  type.

Route (a)'s cost, counted with `tests/harness/suite_layout.hero`'s own
`code_lines` rule:

| file | now / DECIDED | change |
|---|---|---|
| `selfhost/ir.hero` | 305 / 310 | +3 |
| `selfhost/ir/print.hero` | 465 / 470 | +1 |
| `selfhost/emit/inst.hero` | 343 / 350 | +4 |
| `selfhost/ir/flatten.hero` | 1104 / 1110 | **−20** with the three-site collapse |
| new `selfhost/ir/owned_release.hero` | — | **+76** |

**No `DECIDED` row moves and no row is added**, which is panel 109's condition 7
(*"lowered and not raised"*) honoured rather than argued.

**The exhaustiveness is measured, not assumed.** Adding `cstr_to_ptr` to
`variant CastKind` and running `check selfhost/main.hero` names **exactly two**
sites — `ir/print.hero` and `emit/inst.hero` — and no others, because every
other `.cast` site matches on `OpKind` and is already covered. That is the
language doing the work a checklist would do badly.

**And the seat enumerated a route the brief did not list** (CLAUDE.md §1's rule
that a recommendation is a claim about the option SET): **(a′)**, synthesising a
Heroes-level release *function* per freer in the IR, which satisfies condition 1
most literally. It priced it and refused it — `containers.Function` is indexed
by its AST declaration and classified by source position, so a function no
declaration backs is a new kind of thing in two passes.

**Three hook sites, not one.** `flatten.hero` reaches `emit_call` from the plain
call, the qualified method and the UFCS method. A hook at one of them means
`x.strdup()` returns an unfreed, unvalidated `cstr` — a silent leak on the two
spellings nobody would have tested.

## Where the two seats disagree, and why both are right

They are not answering the same question. The FFI seat is answering *what type
does the freer call carry at the C level*, and it is right: `void *` is blind and
`char *` is loud, measured 10-of-10 against 6-of-10. The compiler seat is
answering *which pass builds the release*, and it is right: a `T?` composed in
the printer is the thing design.md already refused once, and the reachability
walk cannot see a call that exists only as a string.

**The FFI seat wrote the exit itself.** Its veto on (a) is conditional, verbatim:
it withdraws if the IR case *"(1) is reachable **only** from an `owned`
annotation and never from user syntax, so `ffi_writable_parameter` cannot be
routed around, and (2) still emits a freer probe typed `char *` rather than
`void *`."* Those two conditions are compatible with everything the compiler seat
asked for: neither of them is about which pass builds the release.

## The coordinator contaminated one seat's bench, and it is recorded here

**The prebuilt compiler shipped inside the frozen snapshot carried an
uncommitted change.** The coordinator had written the checker half of step 4 —
typing a marked `cstr` result as `str?` — measured that it makes clang refuse the
generated C at exit 2, and reverted it from the tree. The binary built from it
was then copied into the snapshot as `heroes-prebuilt`. Measured after the
sitting, on one marked program: that binary exits **0**, the committed compiler
exits **1** with `error[bad_operand]: `default` takes a fallible value, found
`cstr``.

**What it cost.** The compiler seat's answer to question 3 opens *"the dump is
already wrong today"*, quoting `$t4: str? = call extern strdup($t3)` — a C
function whose header says `const char *` carrying an IR result type of `str?`.
**That is the coordinator's parked change and not the snapshot's behaviour.** The
committed compiler prints `cstr` there. The seat's argument survives the
correction and is arguably strengthened: under route (b) the dump would show a
`cstr` while the program sees a `str?`, so route (b) would *introduce* the lie
rather than freeze a pre-existing one. Its cost table, its exhaustiveness
measurement and its `call to undeclared function` experiment are all independent
of the binary's typing and stand unchanged. The FFI seat's work is C compilation
and is unaffected.

**Why it is in the record rather than quietly fixed.** This skill's own procedure
says the working tree is frozen from the moment the briefs go out *"and the rule
binds the coordinator during a sitting"*, because panel 056 had a coordinator
change the flag list while judges measured. This is that failure one level worse:
not a tree changed during the sitting, but a changed tree **baked into the
brief**, where no judge could see it. The rule needs a second half, and it is
written here rather than left as a habit: **the compiler a brief ships is built
from the commit the snapshot names, and the brief says which commit that is.**

## The resolution — provisional, and the robust one

**Adopted: route (a), the release built in the IR, with the FFI seat's two
conditions and the compiler seat's three.** Neither veto is overridden; the
resolution is their intersection, and both seats had already written it down.

1. **The release is built in the lowering, not in the emitter.** The emitter
   stays a printer (design.md Part 5). This is the compiler seat's veto,
   honoured.
2. **The freer call is emitted with the pointee kept.** The cast strips `const`
   and nothing else, so clang goes on checking the argument against the header
   and refuses six of ten wrong freers with one line. **A `(void *)` anywhere on
   that path is the failure this sitting exists to prevent**, and it is the FFI
   seat's condition, honoured.
3. **The conversion is reachable only from the mark.** No user syntax reaches
   it; `ffi_writable_parameter` cannot be routed around by any program.
4. **All three `flatten.hero` hook sites**, collapsed into one helper — which is
   both the compiler seat's condition 2 and what keeps the file under its ceiling.
5. **The NULL guard is unconditional**, from one template. The FFI seat measured
   that no configuration of this machine can see it missing: `fclose(NULL)`
   survives here and a guardless release exits 0 even under ASan and UBSan.
6. **One `tests/golden/ir/` case for `owned`**, hand-written — the directory
   holds 40 files and none mention it. `UPDATE_GOLDEN=1` is forbidden there
   (CLAUDE.md §9), so the pin cannot be regenerated away.
7. **The panel states how it reads panel 109's "generated per-freer release"**,
   which is the compiler seat's condition 1. **It is read as per-freer at the C
   call site, not as a synthesised function per freer.** What condition 1 asks
   for is that the freer be named at a C call site the probe can verify, never
   reached through `void (*)(void*)` and never cast into shape; route (a) inline
   satisfies that, and (a′) — an actual function per freer — was priced by the
   compiler seat and refused as a new kind of thing in two passes. A ratified
   condition must not be quietly reinterpreted, so it is interpreted here, in
   writing, and the author may overturn this reading.

**What the conservative resolution would have been**: two vetoes in opposite
directions leave *do nothing*, and `owned` stops at the parser with a spec
sentence describing a form the compiler cannot lower. CLAUDE.md §4 takes robust
over conservative and says what conservative would have been, which is this
paragraph.

## Predictions to score

| judge | prediction | instrument | scored at |
|---|---|---|---|
| ffi-pragmatist | annotate `sqlite3_errmsg` as `-> cstr owned sqlite3_finalize` in `examples/ledger/db/sqlite.hero` — a freer declared three lines below, so no typo and no unknown name. With the pointee kept, `heroes build examples/ledger/main.hero` **exits 1 or 2 naming `sqlite3_stmt *`**; with a `(void *)` cast anywhere on the path it **compiles clean and exits 0**. The same holds for `fclose` in `examples/ctime/` and `freeaddrinfo` in `examples/curl/` | that one-word edit plus `heroes build` | the `owned` emission step |
| compiler-engineer | on a program whose `extern` carries `-> cstr owned <freer>` and which never itself calls `validated`, `heroes build <prog>.hero --dump-ir \| grep -c '<freer>'` is **≥ 1**; and in the same commit `ls tests/golden/ir/ \| grep -c owned` is **≥ 2**. Under route (b) both are **0**, because route (b) leaves nothing in the IR to golden. `selfhost/ir/flatten.hero` measures **≤ 1090** with the three-site collapse and **> 1110** without it | `--dump-ir` and one `grep`; `suite_layout`'s `code_lines` | the `owned` emission step |
| coordinator | the emission step's `selfhost/` diff lands inside panel 109's own compiler seat's registered **180–300 code lines**, and `tests/harness/suite_layout.hero`'s `assert DECIDED.len() == 16` stays green with no row raised | `code_lines`; the layout suite | the `owned` emission step |

## What the lane gave up

The three judges who do not compile. The question has no reader-facing half —
`owned` is already spelled, already in the spec, already refused when a program
calls its own freer — so the ergonomist would have read a spec that does not
change, the warden a token count that does not move, and the historian a
precedent for an internal representation choice. What the lane cost is real and
is named in the skill: panel 037 ran full and its ergonomist produced the
session's other finding. Here there was no other finding to produce, and the two
seats that compile disagreed hard enough to be worth the whole sitting on their
own.

## Author's verdict

**RATIFIED 2026-09-06** (author instruction, *"I ratify"*), including item 7 —
the reading of panel 109's *"generated per-freer release"* as per-freer at the C
call site. That reading now stands as this sitting's, and the emission step
builds on it.

**What the yes settles**, and the shape is unusual enough to state: two seats
vetoed each other and neither veto was overridden. The resolution is their
intersection because they were answering different questions — one *which pass
builds the release*, the other *what type the call carries at the C level* — and
the FFI seat had written the exit into its own condition before the coordinator
saw it. Nothing here was traded away to reach a middle.

**What the yes does NOT settle**: the `ptr owned` half stays refused under panel
109's standing veto with its own measured return conditions.

**And the coordinator's contamination is ratified as part of the record, not
around it.** The author read the sitting knowing that a supporting sentence in
one verdict came from an uncommitted change in the coordinator's own binary.

The text below is what was put to the author, kept as written.

**PENDING (as it stood).** The open item is in `docs/work/DECIDE.md` as `panel 116`, and work
proceeds on the provisional resolution above (CLAUDE.md §4: a panel never
blocks). The author's verdict is appended here when given.

**What a yes would settle**: that the release is built in the lowering rather
than in the emitter; that the freer call keeps its pointee so clang goes on
checking it against the header; that the conversion is reachable only from an
`owned` mark and never from a program's own syntax; that the NULL guard is
unconditional; and — item 7, the one worth reading first — **that panel 109's
"generated per-freer release" is read as per-freer at the C CALL SITE rather
than as a synthesised function per freer.** That last is this sitting
interpreting a condition another sitting ratified, which is exactly the move a
panel must not make quietly, so it is made in writing and is the author's to
overturn.

**What a yes would NOT settle**: the `ptr owned` half stays refused under panel
109's standing veto with its own return conditions. Nothing here reopens it.

**And what to weigh before ratifying anything**: the coordinator contaminated a
seat's bench, which is recorded two sections up. One supporting sentence in the
compiler seat's verdict was the coordinator's own uncommitted change and not the
tree's behaviour. The verdict survives the correction and every cost measurement
is independent of it — but a sitting whose brief was contaminated is a sitting
the author should read knowing that.
