# Panel 148 — compiler-engineer

- **verdict**: **A — approve. C — veto.**
- **section**: design.md **§1.7** (core plus elaboration: *"it determines the
  size of your compiler"*, and its working criterion — *does it move something
  from the core to the sugar, or remove a special case?*). The veto stands on
  panel 147 **R1**, ratified 2026-09-14, and on §4.17 for the diagnostic half.
  §1.1's ceiling is **not** breached by either option, and I say so explicitly.

## implementation_cost

Measured with `suite_layout.hero`'s own rule (`tests/harness/suite_layout.hero:484`,
`code_lines`), reimplemented and **validated against the record**: it reproduces
`check/consuming.hero` = **114**, `check/ffi_sweep.hero` = **93**,
`ast.hero` = **503**, `print/fmt.hero` = **1153** — the four numbers panels 145,
147 and `docs/measurements/032` already wrote down.

**Option A — surface: +25 code lines across 7 files.** Already measured and
already built, `docs/measurements/032` § 2. The per-call query is a field read
where the declaration is *already in hand*: `selfhost/check/walk.hero:1321`
passes `decls[t.at]`, and `selfhost/ir/owned_release.hero:147` reads
`fd.value.owned_result` off a declaration the lowering looked up for exactly
this kind of question. **No new table.**

**Option C — +90 code lines in one new module, and I built it.**
`selfhost/check/acquiring.hero`, written in the scratch copy, wired into
`selfhost/checker.hero` beside `ffi_sweep`, and **typechecked clean against the
whole tree**: `./heroes check selfhost/main.hero`, exit 0, 15.0 s. It contains
`group_of`, a `(group, ty)` string key, `consumed_in_group`, `owed`, `acquires`
and a sweep — **no diagnostic, no test block, no emitter wiring**. Those three
are unbuilt and their cost is *unrun*. Thrown away, as the brief asked.

**C's timing cost, machine still, three runs each, `/usr/bin/time -p`:**
baseline `user` 14.66 / 14.68 / 14.67; with the pass 14.69 / 14.89 / 14.84.
**+0.17 s, +1.2%** on `heroes check selfhost/main.hero` — a tree with **zero**
handle types, so the table finds nothing and is paid for anyway. Every
prototype run is above every baseline run; the separation is small but it does
not overlap.

**The counter, which both options need** (item 3). `runtime/parts/alloc.c` is
**321 lines**; `hero_live_held` is declared at `:98`, its pair at `:180` and
`:184-186`, and the three exit checks sit at `:109-137`. A fourth is the `held`
block copied: **≈25 C lines there plus 2 in `runtime/heroes_runtime.h`** —
*estimate*, the template is on the page. The IR half is also an *estimate*:
`selfhost/ir/owned_release.hero` at **277 code lines** is the nearest shipped
relative, and it does strictly more work (it composes a `str?`).

## The hinge — item 1: it is a NEW TABLE, over an entity the compiler deliberately destroyed

`consuming_positions` (`selfhost/check/consuming.hero:45-58`, **13 lines**) reads
**one** declaration the caller already holds. Asking *about a type across a
whole group* needs four things it does not have:

1. **Group identity does not exist after the parser.**
   `selfhost/parse/group.hero:6-9`: *"The group is flattened here and only here…
   Nothing after the parser knows the word `group`."* `selfhost/ast.hero:437-439`
   says the same from the other side. The only surviving trace is the head
   line's `header: token.Span?` copied onto every member, so group identity has
   to be **reconstructed** from a span offset.
2. **And reconstructing it is ambiguous in the compiler's own source.**
   `selfhost/cli/io.hero:34` and `selfhost/cli/process.hero:37` are **two
   `extern "hero_os.h"` groups in one program.** Key C on the header *text* and
   they merge; key it on the head *line* and a binding split across two modules
   — which §4.19 **forces**, `extern_across_modules` at
   `selfhost/resolve/names.hero:48-56` — loses the consumer declared in the
   other file and acquires nothing, silently. Both choices are wrong for a
   program that exists today.
3. **A program-wide map keyed on `(group, interned ty)`**, buildable only
   **after** `check_decls`, because it reads `written_types` — the ordering
   `selfhost/checker.hero:52-56` states in terms for `map_keys` and `ffi_sweep`.
4. **The only group-wide structure that ships today is not it.**
   `selfhost/emit/externs.hero:75-101`'s `headers()` builds a **de-duplicated
   list of header strings** for `#include`. It merges groups by design, which is
   right for `#include` and exactly wrong here.

So: **a new table, and one whose key the language does not define.**

## Item 2 — C's correctness, and this is the veto

Found, in this Mac's SDK, counted today:

| header:line | signature | consumer in the same group |
|---|---|---|
| `sqlite3.h:6861` | `sqlite3 *sqlite3_db_handle(sqlite3_stmt*)` | `sqlite3_close` |
| `sqlite3.h:6996` | `sqlite3_stmt *sqlite3_next_stmt(sqlite3*, sqlite3_stmt*)` | `sqlite3_finalize` |
| `sqlite3.h:6209` | `sqlite3 *sqlite3_context_db_handle(sqlite3_context*)` | `sqlite3_close` |
| `curl.h:2798` | `struct curl_slist *curl_slist_append(struct curl_slist *list, …)` | `curl_slist_free_all`, `curl.h:2808` |

The last one is not an exotic shape, it is **how you set an HTTP header**: N
appends, one `curl_slist_free_all`. Under C a correct program aborts at exit
saying `N-1` leaked. Under A the author writes nothing on those four lines and
the instrument stays silent.

**And C has no off switch.** The only way to disarm it for `sqlite3_db_handle`
is to delete `consumes` from `sqlite3_close` — which also deletes panel 145's
use-after-free rule for that type. The two are welded by construction: one word
would carry two meanings that a real binding needs to separate.

**§4.17.** Under C the message at a bad site must say *this call acquires
because some other declaration is marked* — the fix is in another declaration
**by construction**, which is the one thing §4.17 exists to forbid.

## Item 3 — the counter, without flow analysis: yes, with two things said out loud

**Both ends are call sites with a declaration in hand**, so neither end needs
flow analysis: `check/walk.hero:1321` and `ir/owned_release.hero:147` already do
this lookup. `check/leasing.hero:29` and `check/consuming.hero:22` stay true.

- **The increment must be NULL-guarded, and so must the decrement.** A refused
  `sqlite3_prepare_v2` leaves the out-cell null — `examples/sqlite/main.hero:69`
  initialises it to `nullptr` for that reason — while a refused `sqlite3_open`
  hands back a **non-null** connection that is still owed a close, measured on
  both platforms at `examples/ledger/db/sqlite.hero:216-226`. A null test at the
  call site asks the value, not the world, and
  `selfhost/ir/owned_release.hero:26-30` is the precedent — with the warning
  that **no run on this Mac can see that guard missing**.
- **It is a BALANCE, not a matching.** Acquire two handles, consume one of them
  twice: net zero, exit 0, one leak and one double free. That is the same hole
  `consumes` already documents at `check/consuming.hero:22-31`. It belongs in
  the spec sentence now, not in a defect later.

## Item 4 — §1.7's subtraction, and core or sugar

**Both are sugar; neither adds a core construct.** No new type, no new
expression, no new IR node, no new backend case: the increment lowers to *a
branch and a call*, Part 5's own vocabulary, which is the ground panel 116 gave
for putting `owned`'s release in the lowering. Type checker: unchanged — the
type of an acquiring call is the type it already had.

**But §1.7's criterion is a subtraction, and only A passes it.** A adds one
contextual word, read in one position, erased in the frontend into one guarded
call. C subtracts a word and **adds a special case**: a cross-declaration
inference that the checker, the lowering and every diagnostic must each
reproduce and keep agreeing about. Paying 90 lines and a new ambiguous key to
save 8 vendored spec tokens is §1.1's hierarchy read backwards — tokens win
only when comprehension is indifferent, and here it is not.

**One line on what C is right about**, since approving costs nothing: C is the
only option with zero new surface, and `docs/measurements/032` § 3 showed what
new surface costs — the formatter silently ate `acquires` within minutes. That
is real, and it is six lines, not ninety.

## needed_for_self_hosting

**no.** Counted: `grep -rnE "^ +record [A-Za-z0-9_]+ tag " selfhost/` returns
**zero**. The compiler declares four `extern` groups (`cli/io.hero:34`,
`cli/process.hero:37`, `cli/process.hero:49`, `emit/literal.hero:41`) and not one
handle type. This form enters on Principle 0's **second** clause or not at all.

## argument

Option C is panel 147's R1 wearing a group-shaped hat. R1 refused a rule keyed
on the type because *the same handle type is handed back owned by one C function
and borrowed by another* — and C infers the acquisition from `(group, type)`,
refuted by the same three `sqlite3.h` functions plus `curl_slist_append`,
which is how libcurl sets a header. Prediction 1 of that sitting is
already scored HELD on `sqlite3_db_handle`. The cost runs the same way: A's
surface is 25 measured lines and a field read; C is 90 lines I compiled, over a
group key the language does not define and which is ambiguous in the compiler's
own `hero_os.h` pair. C trades 8 spec tokens for a table.

## prediction

At **M-marked-acquisition close**: the unbuilt half — the pass that places both
ends plus the runtime's fourth counter — lands at **120 to 280 code lines** by
`suite_layout.hero`'s `code_lines`, under `ir/owned_release.hero`'s 277 and over
`ffi_sweep.hero`'s 93; and **`heroes check selfhost/main.hero` moves by less than
1%** against today's 14.67 s `user` median, because the query is per extern call
and `selfhost/` has none. If the total feature (25 + that pass + the runtime)
exceeds **400** code lines, I am wrong about the ceiling and say so.

## condition

**What flips the veto on C**: a binding rule that makes the borrowed return
impossible rather than unobserved — a measured refusal of any `extern` handing
back a handle of a type its group consumes **unless** it is marked, which is
option A with the default inverted and should be priced as such. A re-count
showing `curl_slist_append`, `sqlite3_db_handle`, `sqlite3_next_stmt` and
`sqlite3_context_db_handle` are the only four such functions across the three
headers the roadmap names would *not* flip it: four is already enough, and
`docs/measurements/031` says the corpus's silence here is *a fact about this
corpus on this date*.

**What would weaken my approval of A**: if the counter's IR half prices over
400 code lines, A stops being sugar in practice and the sitting should hear the
loud-exit-versus-compile-error question again before the word lands.
