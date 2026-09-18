# Panel 164 — compiler-engineer report (verbatim)

## verdict

**veto** — on **route 1** and **route 2**, as refusals on soundness, not prices. **object** on route 3 (sound, but it buys 22 of 80 call sites for ~206 selfhost lines). **approve** route 4 as the only route that costs zero compiler lines, with one correction: the shared brief's claim that no shim can route around this is **false, measured**.

## section

**design.md §4.3** (`docs/design/design.md:978`, "No implicit conversions"), with **Part 6's `Subtyping` row** (`:2644`, refused on cost alone, §1.1 making simplicity the ceiling) as the second leg, and **§1.12** (`:571-579`, "must not corrupt memory") carrying route 2. Note explicitly: **design.md says nothing about fixed arrays at all** — `grep -n "fixed" docs/design/design.md` returns 11 hits and not one is about `T[N]`. The form is panel-level (§3.1 via panel 062, §4.19 via panel 085). I am not inventing a rationale: on the fixed array itself the document is silent.

## implementation_cost

Every line count from `awk` replicating `tests/harness/suite_layout.hero:525` `code_lines`, and each zero-headroom claim **confirmed live** by appending two code lines and running `./heroes run tests/harness/main.hero -- ./heroes layout`.

**Four of the modules these routes must touch have exactly zero headroom:**

| file | code_lines | ceiling | slack | instrument said at +2 |
|---|---|---|---|---|
| `selfhost/check/table.hero` | 389 | 389 DECIDED | **0** | "391 lines of code, past the 389" |
| `selfhost/check/builtins.hero` | 378 | 378 DECIDED | **0** | "380 … past the 378" |
| `selfhost/emit/inst.hero` | 350 | 350 DECIDED | **0** | "352 … past the 350" |
| `selfhost/ir.hero` | 310 | 310 DECIDED | **0** | "312 … past the 310" |
| `selfhost/ir/print.hero` | 466 | 470 | 4 | — |
| `selfhost/check/walk.hero` | 1863 | 1870 | 7 | — |
| `selfhost/ir/flatten.hero` | 1139 | 1150 | 11 | — |
| `selfhost/check/lending.hero` | 285 | 300 (§11) | 15 | — |
| `selfhost/emit/ops.hero` | 265 | 300 | 35 | — |
| `selfhost/emit/storageless.hero` | 87 | 300 | 213 | — |

**Route 1 — automatic decay.**
- `selfhost/check/table.hero:313-357` `fits`. **0 slack.** And siting it there is the soundness failure below, because `fits` has exactly two callers: `check/walk.hero:662` (`compare`, which judges bindings and returns) and `:1817` (`bind_argument`, call arguments). Restricting to arguments means threading a callee-kind parameter into `bind_argument` (`walk.hero:1812` — its signature has no callee today), in a file with 7 lines of slack.
- `selfhost/emit/inst.hero:189` — `.value_arg v => rendered @ rendered.push(mangle.value(v.value))`. **0 slack.** Must route through `storageless.fixed_text`. Measured why: `emit/ctype.hero:380` answers `.fixed => fail("C has no assignable array")`, so `emit/body.hero:186-188` skips the declaration and the emitted C names an undeclared `t3` — exit 2, the compiler blaming itself.
- `selfhost/emit/ops.hero:253-305` `guard_arguments`, 35 slack.
- **Measured total: ~15-25 lines, three of them in files that cannot take one.**

**Route 2 — `f.cstr()` widened.**
- `selfhost/check/lending.hero:103-106`, 15 slack. The trap: `cstr` and `lease` share one `if`, so the one-line widening makes `f.lease()` legal too, and `lease` is `emit/builtins.hero:39` EMITTED with a `str` entry point — a `.fixed` receiver falls into the `str` arm of `builtins.entry` (the pattern at `:96-98`, `:113-115`).
- A new `ir.CastKind` at `selfhost/ir.hero:175-181` (**0 slack**), forcing arms at `selfhost/ir/print.hero:273` (4 slack) and `selfhost/emit/inst.hero:320` (**0 slack**), plus a producer at `selfhost/ir/flatten.hero:531,720`. Reusing `.str_to_cstr` does not escape this: its arm emits `hero_str_cstr(t2)` over a `HeroStr`, so a branch still lands in `inst.hero`.
- A hand-written `tests/golden/ir/` case (`UPDATE_GOLDEN=1` forbidden outright there).
- **~35-50 lines, two in zero-slack files.**

**Route 3 — `f.ptr()`, a new built-in.** Calibrated on the one landed eight days ago in this same family, `git show --stat c239722e`: `check/lending.hero +49`, `emit/bytes_text.hero +113` (new module), `inventory.hero +23`, `emit/ops.hero +9`, `emit/builtins.hero +6`, `measure/pinned.hero +6` = **206 selfhost lines**, plus `runtime/` +93 (`heroes_runtime.h`, `parts/str.c`, `parts/failure.c`) and `1298bc83`'s eight `.expected` snapshots that enumerate built-ins. A `ptr` lend needs no runtime, so subtract the 93 and `bytes_text`'s union-building half; realistic **~90-150 selfhost lines**, none in a zero-slack file (`lending.hero` 15, `inventory.hero` 151/300, `emit/storageless.hero` 213).

**Route 4 — refuse.** Zero compiler lines. Cost is a diagnostic `note` (today `type_mismatch: expected cstr, found i8[16]` names no repair) plus a `tests/golden/unsupported/` case.

## needed_for_self_hosting

**no.** The compiler self-hosts without it; the shared brief says so and §1.12:589 is explicit that §1.12 "does not suspend Principle 0" — "it would be safer" is not an entry ticket. The thesis branch carries this or nothing does.

## argument

Route 1 is an implicit conversion (§4.3) and, in `table.fits`, subsumption — Part 6:2644's refused row. Worse, it has **no expression**: `x: cstr @ s.name` carries no `.cstr()` node, so `check/lending.hero`'s `cstr_escapes` sweep, which keys on the builtin call (`lending.hero:80-93`), never fires. I ran it: `x: cstr @ t.cstr()` is refused today; under route 1 the field form is admitted silently — a pointer into a dead local. Route 2 hands `strlen` a field 37 of 50 real headers do not terminate; §1.12 forbids that overread. Both also emit `-Wpointer-sign` under `selfhost/cli/flags.hero:92-109` — measured, both signs, direct and via temporary — and `guard_arguments` would wrap a guard that cannot fail, the exact thing the 2026-08-15 decision quoted at `emit/ops.hero:249-252` excludes.

## prediction

**The first commit that makes `slot_len(s.name)` compile under route 1 or route 2 will either raise a `DECIDED` row in `tests/harness/suite_layout.hero:408-428` or be red on the `layout` suite.** Checkable at that milestone's step 1 by `./heroes run tests/harness/main.hero -- ./heroes layout`. Grounded: `check/table.hero` 389/389, `emit/inst.hero` 350/350, `ir.hero` 310/310, `check/builtins.hero` 378/378, all four confirmed red at +2 today. Route 3 touches none of the four and I predict it lands green on `layout` with no ceiling rise, at 90-150 selfhost lines by `git show --stat`.

## condition

- **Route 2's veto lifts** if the termination question is answered at the binding rather than guessed — a mark the author writes, `name: i8[16] terminated`, moving the promise from the compiler to the header's reader. That is the route nobody listed. It is a **new surface form** and owes the whole walk in `.claude/rules/diagnostics-and-goldens.md` (`print/fmt.hero` round trip, every `--dump-<stage>`, `heroes mutate`, `editors/vscode/syntaxes/heroes.tmLanguage.json`, `site/src/lib/highlight.ts`, `heroes measure`), so price it before adopting it.
- **Route 1's veto lifts** on nothing I can construct: any siting that reaches `cstr` or `ptr` without an expression is invisible to a position rule.
- **Route 3's objection lifts** on a measurement that more than 22 of 80 extern parameters are reachable, or that the shim fails on a real header.

## Three findings the briefs do not have, each run today

1. **The shared brief's line 123 is false.** It says `slice` over such a field gives a `[u8]`. Measured: `error[bad_operand]: slice takes str or [T], found u8[16]`. The `[u8]` route it names as existing does not exist.
2. **"No Heroes-side shim can route around it" is true and irrelevant; a C-side `static inline` closes the gap in full, today.** Ran `probe/j_shim.hero` against a header with three `static inline`s: length out (`2`), byte out (`72`), **and C writing back through `@s: Slot`** (`id` → `9`, field reads `filled`). §4.19:2190 already blesses this — "Macros and `inline` functions are reachable because the C compiler sees the real header." Route 4 is not a hole; it is one line of C per binding, which §1.12's completeness half still counts against it.
3. **The `cstr` routes are not warning-clean and the `ptr` route is.** Under the exact flag list of `selfhost/cli/flags.hero:92-109`: `const char *t = s.name` and `slot_len(s.name)` each warn `-Wpointer-sign`, for `i8[16]` **and** `u8[16]`; `const void *t = s.name`, `slot_first(s.name)` and `slot_first((void *)s.raw)` produce **zero** warnings, exit 0. Routes 1 and 2 therefore owe an emitter cast that route 3 does not — and §7 ("the generated C compiles without a single warning") is what makes that mandatory rather than cosmetic.

## On brief question 4 — can the checker know a field is terminated?

I searched `selfhost/check/` for `terminat` and `NUL`: four hits, three about loop termination, one a **comment** at `check/lending.hero:125-126` restating panel 162's 13-of-50. So: **no pass in the checker models a field's bytes**, and after `slot_fill(@s)` — which I ran and which compiles — the bytes come from C and nothing in the frontend sees them. That is what I searched for; a seat with a different vocabulary may find a pass I missed.

## One adjacent defect, not on the four-route list

A real `utsname` binding needs **256 literal zeros per field** to construct. Ran it: `error[fixed_array_length]: i8[256] holds exactly 256, and this literal has 1`. The golden that landed yesterday says so in its own words (`tests/golden/run/ffi-a-filled-record-is-asked-for.hero:31-32`: *"The field is short here because the length is not what the case is about"*). None of the four routes touches it, and it blocks the same real headers they are argued from.
