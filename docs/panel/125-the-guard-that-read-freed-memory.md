# 125 — The guard that read freed memory

**Soundness lane**, two seats, 2026-09-09: the compiler-engineer and the
ffi-pragmatist, because no spec sentence and no diagnostic class was in
question — only how a mechanism panel 124 had already adopted is made sound.
The lane's cost is what it gave up: no reader of the spec judged this, and no
precedent was sourced. Both were judged at panel 124 the same day, and the
spelling that came out of this sitting is measured for reader cost in
`docs/measurements/010-spec-budget-ledger.md` row 40 rather than here.

## The proposal, verbatim

`M-held-bytes`, implementing panel 124's ratified resolution: §4.19's fourth
case, bytes Heroes holds and frees which C may read for as long as the program
says. The runtime half was built first and measured with a C driver against
the real runtime: `hero_str_held(HeroStr s) -> const char *` copied the bytes
into a block carrying a header with a magic word immediately before them,
exactly as a `str` block is laid out; `hero_held_release(const char *p)` checked
the magic and freed; a fourth counter joined the block and scratch pair and the
leak gate gained a third message accusing the **program**, which panel 124 R6
required.

Four cases ran under `-fsanitize=address,undefined`. Hold in a loop, drop the
`str`, read the held bytes after it is gone, release: correct, exit 0, gate
green. Forget the release: the new panic, exit 134. Release a lend: a named
panic, exit 134. **Release the same pointer twice: exit 134 both ways, but under
ASan it was `heap-use-after-free, READ of size 8, in hero_held_release`, and
without ASan the coordinator's own panic fired only because `malloc` had left
the zeroed magic readable in the freed block.** That is luck, and it is the
question this lane was convened on: how is a double release made impossible, or
loud without reading freed memory?

Five routes went out, with two constraints the seats were asked to confirm or
refute by measurement rather than accept from the brief: that a registry of
live pointers is blocked by design.md Part 7.13's one-allocation-site rule, and
that a new `Ty` case costs the 173-error blast radius panel 112 R1 measured.

## What the two seats measured

**The premise was right and worse than stated.** The compiler seat ran the magic
check against the four inputs its own message named, under ASan:

| `hero_held_release(p)` where `p` is | result |
|---|---|
| a C string literal | `global-buffer-overflow`, exit 134 |
| a `malloc` from C | `heap-buffer-overflow`, exit 134 |
| a stack buffer | `stack-buffer-underflow`, exit 134 |
| an interior pointer into a live held buffer | the named panic, exit 134 |

The one lend the check caught in the brief's case 3 was caught because
`hero_str_cstr`'s pointer sits sixteen bytes past a `HeroStrHeader` the runtime
owns — the one foreign pointer whose header read happens to be in bounds. **A
magic word cannot be the discriminator for "a pointer no `.held()` handed
out", for any magic word, because it must dereference before it can
validate.** And the comment the coordinator wrote above the function claimed
**three** refusals where the code had **two**: the third, *already released*,
cannot be implemented by that technique, because telling it from *never held*
is exactly the read that is undefined. Five unsanitized runs of a double
release printed the WRONG message five times out of five.

**The ffi seat found the interior pointer from the other side, and it is a
corruption class reachable by closing a use-after-free.** `strchr` and `strstr`
answer `cstr`, the same type `.held()` must answer, so `release(tail)`
type-checks. Run: with ordinary bytes, UBSan reports a misaligned access twice
at `str.c:380` and then the panic fires; **with bytes that happen to spell the
magic word, `free()` of an interior pointer — ASan `bad-free … 40 bytes inside
of 80-byte region`, and unsanitized exit 133 with an EMPTY stderr.** Fifteen
lines, and the bytes are data a binding reads from a file or a column. CL-061
applies as written: the repair is attacked at the shape next to it, and the
shape next to a double release is an interior release.

**The two constraints, verified.**

- **Part 7.13 refuses a registry, and not by argument.** The compiler seat
  wrote a second caller of `hero_grow_kept`, the one buffer the runtime keeps
  for the life of the process: *"panic: a second buffer kept for the life of a
  thread — parts/alloc.c holds one slot per thread, and it is now wrong"*, exit
  134. The precedent does not admit one more; it turned *one caller* into an
  assertion that fires. A registry needs the kept table widened to N slots,
  which `alloc.c:219-221` names as *"a shared free list, which is a second
  allocation site wearing a different hat and the one thing this file exists
  to prevent"*. The ffi seat prototyped the registry anyway, at about 45 lines,
  and it closed every bad shape with zero sanitizer reports — including the
  interior pointer and a pointer laundered through C — at zero cost for every
  shape a binding writes and **quadratic at 100,000 simultaneous holds, 2.67
  s**. What it buys that nothing else does is the laundered pointer, and it is
  still refused, because it would be the runtime's first unbounded structure.
- **A new `Ty` case costs 175 errors across 51 files today** — check 82, emit
  76, ir 16, cli 1 — measured by adding one case to `selfhost/check/table.hero:34`.
  Panel 112 R1's 173 across 49 holds and has grown by two. **And the move
  machinery the brief hoped for does not exist**: `grep -rn "moved_out\|moved-out\|move_out" selfhost/`
  is zero hits. `hero_str_require`'s *"unassigned or moved-out slot"* names the
  emitter's `@`-parameter copy-in and copy-out, which reaches a cell of any
  type and is not affine tracking.

## The routes, each with its run

**(a) The cell is the authority — the only sound route, and the one adopted,
with two amendments.** `h: cstr @ s.held()` then `release(@h)`: an `@`
parameter, which §4.8 already compiles to a pointer, and the runtime nulls the
cell on the way out, so a second release hits a NULL before any read. The
compiler seat measured what a `cstr` cell can and cannot do today:

| shape | today |
|---|---|
| `h = x.cstr()` | `error[cstr_escapes]`, panel 122 R2 |
| `h = grab()` (extern `-> cstr`), then `g = h` | **accepted** |
| `g: cstr @ h` | **accepted** |
| `g @ h` | **accepted** |
| `drop(@h)` where `h = grab()` | `error[not_mutable]` |
| `drop(@c)` where `c: cstr` is a by-value parameter | `error[not_mutable]` |
| `relay(c: h)`, a Heroes function taking `cstr` | **accepted** |
| `function f() -> cstr` | `error[cstr_out_of_heroes]` |
| `record H { c: cstr }` outside a group | `error[cstr_in_a_record]` |
| `[cstr]`, `{i64: cstr}`, `cstr?` | `check` exit 0, `build` `unsupported[pointer_element]` |

So the release cannot follow the pointer into another frame — `@` of a by-value
parameter is refused — and the seat's amendment 1 was: **the clause is about
ORIGIN, not only position.** `release`'s operand must be a cell whose
declaration initialiser is syntactically the holding call, which is not a
dataflow analysis: `is_lend` in `check/lending.hero` is fourteen lines and the
same fourteen with a different string answer it. Plus one clause refusing any
other write to such a cell, at a measured corpus cost of **zero** — thirteen
`: cstr @` and `: cstr =` sites in the tree, twelve of them IR-dump text in
`tests/golden/ir/owned-*.expected` and the thirteenth `tail: cstr @ nullptr`,
whose right side is a literal. Amendment 2 is the runtime guard per the veto
below.

**The coordinator found a third clause by running the seat's own table.** The
seat wrote that *"an `=` copy can be read but never released"* and drew the
hole as *"exactly and only the two cell forms"*. But `g = h`, then
`release(@h)`, then `strlen(s: g)` reads freed memory through C — a
use-after-free `check` cannot see, reached by exactly the copy the table
accepts. So the pointer may live in ONE cell and nowhere else: **the cell's name
stands only as an argument of a call**, which is the lend's own position rule
applied to the lease. Three clauses, not two.

**(b) A registry — refused**, above.

**(c) A generation counter, the block never returned to the allocator —
refused, on the same curve as panel 124's candidate D.** `ru_maxrss`, 1e6
hold-and-release pairs: free on release **1,392,640** bytes; poison and keep
**49,627,136**, 35.6x; at 1e7, **483,737,600**, O(n). Second, independent kill:
the held counter read 1,000,000 at exit, so R6's panic would fire on every
CORRECT program.

**(d) The language refuses it through a new type — refused**, 175 errors, and
its premise is refuted: held and lend are already distinguishable by ORIGIN,
which is why (a)'s amendment gets (d)'s refusal for about fifty lines instead.

**(e) Refuse only under `--sanitize` — refused plainly.** §1.12: *"a goal of the
language, not a quality of its implementation"*, and *"a guarantee that ends
quietly is not one"*. A guard that exists on one CI leg is a guarantee that
ends quietly on the other two.

**Two routes the brief missed, enumerated by asking what can distinguish a
valid pointer from an invalid one.** (f) The compiler emits the release, as
`owned` does — sound by construction and **closed by ratification**: panel 124
R4 says the release is written by the author, never inferred. (g) A counted
value released by decref, reusing `HeroStrHeader` — collapses to either the
18th `Ty` case or a pin, which R3 refuses.

## The ffi seat's four answers

**Both residues of defect 024 close, from the runtime up, before the language
knew the words.** The per-column bind helper that builds its own bytes for a C
side that keeps them (`sqlite3_bind_text` with SQLITE_STATIC), and the `@tail`
out-parameter that points INTO the bytes: both **exit 0, correct, ASan and
UBSan clean, all four counters green**, written from C against the real runtime
pair.

**The FFI permits the shapes that make a consume-on-release route hard, and
they are fine under (a).** A held pointer bound to two columns of one statement,
handed to two different C functions, and `CURLOPT_POSTFIELDS` followed by
`curl_easy_perform`: exit 0 all three, **one release each**, gate green. The
lease is READ many times and released once, which is what a cell does.

**`hero_held_release` cannot be SQLite's own `xDel`**, and it should not be:
clang refuses `void (const char *)` where the header wants `void (*)(void *)`,
and the seat recommends against adding a `void *`-shaped release to buy it,
because giving the bytes away silences the fourth counter, which is the one
thing R6 bought.

**No shipped C API was found where the caller must MUTATE the buffer while the
library holds it**, so the copy is the right answer everywhere it looked:
`putenv` with a held buffer is refused by the project's own `-Werror` (a
`const char *` where `char *` is wanted), and the copy-is-a-snapshot behaviour
(`held sees: before` while the `str` is `before-after`) is sound and stated.
That is a limit named now rather than discovered later, and it is a limit and
not a change, because panel 124 R3 refused the pin.

**And the exit path.** `hero_runtime_check_leaks()` is emitted at the end of
`main` and `hero_exit` calls C's `exit`, so **R6's accusation does not fire on a
program that leaves through `exit(code)`** with a lease outstanding. The seat
offered two answers, an `atexit` registration or naming it, and this record
names it: a program that exits early has abandoned its work on purpose, the
block counter would accuse the compiler wrongly on that same path if registered,
and the instrument for the C side of that leak is the one
`.claude/rules/c-boundary.md` already requires, the Linux leg under
`--sanitize`.

## The ABI stamp, and a defect this sitting found in a README

The coordinator moved `HERO_RUNTIME_ABI` from 21 to 22 when the two runtime
functions were added, **and that made CLAUDE.md § Commands' first line red in
the working tree**: `clang -I runtime seed/heroes.c runtime/runtime.c` failed on
`22 == 21`, because the committed seed is stamped by the compiler
(`selfhost/emit/decls.hero:79`) and not by the header. Both seats caught it
independently. **224 lines in 222 files carry the stamp** — the seed, the
emitter's own line and its test, 214 `tests/emission/*.c` traces and 6
`tests/golden/emit/` cases — and all of it lands in one commit or main is red
between them. The order that works, run by the coordinator before the seats
reported and confirmed by both: the seed is regenerated by a compiler built
BEFORE the header moves — header back to 21, the emitter's line to 22, build
`heroes-next`, header to 22, `heroes-next --emit-c` into the seed, then clang and
the `cmp` fixpoint, which was silent.

**The bump is right in direction and was premature in timing**, the compiler
seat measured: the header change is purely additive and an ABI-21 seed compiles
against the ABI-22 header with only the number changed, so *"the declarations
changed shape"* was false as stated. It belongs in the commit where the emitter
first emits the call, which is this milestone's.

**And `seed/README.md:95` promised an instrument that does not exist.** It said
`the_seed_builds_from_a_clean_checkout` asserts the two numbers agree; that
test is in that file's prose and nowhere else in the tree, and CI does no such
step. The sentence is corrected in this milestone to say what is true: the
`_Static_assert` itself is the instrument, and it fired.

## Where the seats disagreed, unsmoothed

**On the registry.** The ffi seat's veto line was *"`hero_held_release` must not
read at the pointer it is given; ship the register or any structure the runtime
owns"*, and it approved the registry it had prototyped. The compiler seat
refused the registry on Part 7.13 with the assertion that fires. **The
resolution takes the compiler seat's route and the ffi seat's standard**: under
(a) with all three clauses, the runtime never receives a pointer that did not
come from a lease cell in a program the checker accepted, so in an accepted
program it reads no memory it was not handed; the magic check that remains is
defence in depth against an EMITTER bug, and its message says so. What the
registry would have bought over this — a pointer laundered through C — is
recorded as the residue below, not hidden.

**On the spelling, both seats agree and the coordinator's names lose.** The
compiler seat measured `held` and `release` at **29 errors across 11 files**:
`inventory.hero:6-9` reserves every name at the top level, as a parameter and
as a local, `error[builtin_name_taken]` fires, and `selfhost/ir/owned_release.hero:97`
is a top-level `function release`; five shipped examples carry one or the
other. Panel 121's precedent applies: refuse the spelling, keep the form. The
names that landed, **`lease` and `end_lease`**, were measured at **zero**
collisions by the same mechanism — the two names appended to the table, a
compiler built, `check` over `selfhost/main.hero` and every `examples/*/main.hero`.

## The resolution, provisional — author ratification pending

**R1. Route (a) with three clauses, not two.** A lease is `x: cstr @ s.lease()`,
a cell whose initialiser is syntactically the lease call, asked through the
resolver. Its name stands only as an argument of a call (the coordinator's
clause, from the seat's own table). The only write it takes after its
declaration is `end_lease(@x)`. `end_lease` takes only such a cell. Landed in
`selfhost/check/lending.hero` as one sweep beside the lend's, with the typing of
all three built-ins moved there from `check/builtins.hero`, which is at its
decided ceiling — net **-3** in that file.

**R2. The runtime reads no memory it was not handed, in any accepted program.**
`hero_held_release` takes the CELL, `const char **`, tests it for NULL before
any other read, and nulls it on the way out. The magic check stays as defence
in depth against an emitter bug, and its message says *this is a compiler bug*
because in an accepted program that is the only way it can fire. The comment
that claimed three refusals now claims two and records why the third cannot
exist.

**R3. The spelling is `lease` and `end_lease`**, measured collision-free; the
coordinator's `held`/`release` is refused at 29 errors in 11 files, on panel
121's precedent.

**R4. The residue is named and it is defect 024's own territory, not this
sitting's.** A lease pointer laundered through a C function that returns its
argument, or retained by C past the `end_lease` the program wrote, is a
use-after-free no frontend rule and no runtime read can see: the retention
decision is the C function's, in an argument the header does not type. What the
language now has is the sound spelling for every shape a binding writes; what
it cannot have is a refusal of the wrong program, and the instrument for that is
the Linux leg under `--sanitize`, which `.claude/rules/c-boundary.md` already
requires of every program declaring an `extern`.

**R5. The exit path is named rather than closed.** A lease outstanding at
`exit(code)` is not accused, and the reasons are above.

**R6. The ABI stamp lands with the emitter's first call**, in this milestone's
commit, with the 214 traces and 6 goldens regenerated in the same commit, and
the README's phantom test named as absent.

**R7. The two seats' riders bind.** A `tests/golden/run/` case lends a computed
string inside a loop, because nothing that ran did and every candidate at panel
124 could have shipped green; and a run case for each of the two residues, and
one for the forgotten release, exist before the commit.

**What conservative would have been** (CL-040): the registry, which the ffi
seat approved and which additionally catches the laundered pointer, at the
price of the runtime's first unbounded structure and Part 7.13's second
allocation site. Recorded so the author can choose it.

## Author's verdict

**Ratified as adopted, 2026-09-09** (`/decide`, answer `1a 2a` to the two
questions the queue item carried). The cell is the authority and is nulled on
release; a lease's name stands only as an argument of a call and takes no other
write; `end_lease` takes only a lease cell; the runtime reads no memory it was
not handed; the registry stays refused on Part 7.13; the spelling is `lease` and
`end_lease`. The conservative route recorded above was put to the author as the
alternative and declined.

**And the fourth point, raised after the sitting by the site panel
(2026-09-09) and answered with it.** `spec/heroes-spec.md:246` said *a lease
nobody ends is named at exit*; what runs is `panic: 1 lease(s) never ended`, a
count, when `main` returns, and nothing on `exit()` (R5 above). Naming the
lease would take the registry this verdict refuses, so the author took the
wording: the spec says *counted when `main` returns*, and its cost is measured
with `heroes measure --refresh` in the commit that carries this verdict. The
example `examples/gallery/13-lease.hero` and the site had already been brought
to the measured sentence by the site panel's seats the same evening.

## Predictions to score

| seat | prediction | scored at |
|---|---|---|
| compiler-engineer | the feature lands with `selfhost/check/table.hero`'s `variant Ty` still at **17** cases and `suite_layout.hero`'s `DECIDED` still at **16** entries, with `check/lending.hero` at **<= 275** in the suite's unit. Falsified by an 18th `Ty` case or a 17th `DECIDED` row, either of which says the frontend route did not fit | M-held-bytes close |
| ffi-pragmatist | with the runtime pair in place, `examples/ledger/db/sqlite.hero` CAN ship a per-column held helper spelled with an `@` out-cell that keeps its `fail("cannot_bind", …)`, with no shim and no `heroes cc`, so panel 124's recorded loss — *"the held route traded a diagnostic for a silent sentinel"* — is false as a necessity. Falsified by any diagnostic on that signature | M-held-bytes close |
| ffi-pragmatist | `hero_held_release` never appears as a C destructor argument in any shipped example. Falsified by a committed `.hero` or shim passing a Heroes release where a header wants `void (*)(void *)` | M-held-bytes close |
