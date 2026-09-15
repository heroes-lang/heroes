# Panel 154 — report of the compiler-engineer

## Verdicts

| route | verdict |
|---|---|
| **A** — guard every handle argument | **adopt** |
| **B** — `accepts_null` opt-out word | **object** (not core; premature, and priced below) |
| **C** — opt-in `not_null` word | **refuse, veto** |
| **D** — the existing marks decide | **refuse, veto** — premise falsified by measurement |
| **E** — refuse statically-`nullptr` | **refuse, veto** as *the* resolution (fine as an extra) |
| **F** (mine) — A now, B only when a real program is refused, on evidence | **adopt** |

`section`: design.md **§1.12** ("the compiler and the runtime **check rather than assume**, and they check at the boundary even where the value is known to be good … That is one predictable branch per FFI argument, and it is paid deliberately"), **§1.7** (seven-construct core; a parameter mark is not one), **§1.1** (ceiling), **Part 6** borrow-checker row (panel 139's measurement that no header-derived rule separates a call that invalidates a handle from one that reads it).

`needed_for_self_hosting`: **no** for every route. The compiler binds no handle (`grep -n "record .* tag" selfhost/` is empty). All five enter, if at all, through Principle 0's second door: §1.12 is a stated goal of the language, and §1.12 itself says it "does not suspend Principle 0" — it decides the *shape* of an admitted form. Defect 045 is a §1.12 breach, so the repair is admitted; the *word* in B/C is not.

## First, a correction to the brief

**`guard_cstr_arguments` does not exist in the live compiler.** The function is `guard_arguments`, `/Users/joseph/Temp/heroes-lang/selfhost/emit/ops.hero:244`, called once from `:121`. The old spelling is still carried by `.claude/rules/c-boundary.md:30`, `docs/work/DEFECTS.md:67` and `:96`, `docs/panel/096:118` and both 154 briefs. The emission site is `ops.hero:278` (`hero_cstr_nonnull(...)`); the runtime body is `runtime/parts/str.c:328`, declared `runtime/heroes_runtime.h:248`.

## What the files measure, by the instrument that judges them

`code_lines` (`tests/harness/suite_layout.hero:502-517`) read by padding each file with 1001 known lines, running `layout`, and subtracting. `CEILING` is 300 (`suite_layout.hero:44`); `DECIDED` is 19 rows (`:384-405`).

| file | code_lines | ceiling | headroom |
|---|---|---|---|
| `selfhost/emit/inst.hero` | 350 | 350 | **0** |
| `selfhost/ast.hero` | 521 | 525 | **4** |
| `selfhost/emit/extern_probe.hero` | 296 | 300 | **4** |
| `selfhost/emit/gate.hero` | 360 | 365 | 5 |
| `selfhost/check/walk.hero` | 1861 | 1870 | 9 |
| `selfhost/print/fmt.hero` | 1165 | 1175 | **10** |
| `selfhost/parse/members.hero` | 263 | 300 | 37 |
| `selfhost/emit/ops.hero` | 258 | 300 | 42 |
| `selfhost/check/acquiring.hero` | 249 | 300 | 51 |
| `selfhost/print/dump.hero` | 198 | 300 | 102 |
| `selfhost/handles.hero` | 172 | 300 | 128 |
| `selfhost/check/consuming.hero` | 114 | 300 | 186 |

## implementation_cost

### Route A — written, formatted, gated. Not compiled.

I wrote the patch in the copy rather than estimating it: `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/8d7432e4-9b44-486f-80db-fda4fdc56efe/scratchpad/p154-compiler-engineer`.

- `selfhost/emit/ops.hero`: `use handles`, one local, `.named n => is_handle @ handles.is_handle(decls[n.decl])` split out of the grouped arm (the exact spelling already live at `selfhost/check/contextual.hero:123`), one `else if` branch, ten comment lines. **258 → 272 code_lines** against 300.
- `runtime/parts/alloc.c` +10 (`hero_handle_nonnull`, beside `hero_handle_acquired:390`), `runtime/heroes_runtime.h` +1, ABI 22→23 there and at `selfhost/emit/decls.hero:79`/`:335`; `seed/heroes.c` carries the stamp **twice**, so the seed is regenerated — routine for any runtime change.
- **Total 4 files, 31 insertions, 6 deletions.** `heroes fmt --in-place` then **`canonical` 2 passed, `layout` 2 passed, `order` 3 passed**. No `DECIDED` row added or moved. **0** checker rules, **0** IR ops, **0** parser lines, **0** surface forms, **0** spec tokens, **0** files on the new-surface-form walk.
- **`tests/golden/emit/` and `tests/golden/ir/` contain zero `record X tag Y`**, so no `.expected` under the two trees where `UPDATE_GOLDEN=1` is forbidden outright changes. 13 of 263 `run/` goldens declare a handle; their `.expected` is stdout, which the guard does not move (measured below).
- **Unrun, in my own words**: I could not rebuild the compiler from `selfhost/` — the brief forbids it in a cold copy — so the patch is gated by the three text-reading suites and is *not* compiled. Everything below is measured by hand-wrapping the emitted C instead, which is the same C the patch would write.

### Routes B and C — a fourth parameter word, 13 sites

Enumerated from the three words that already exist (`consumes`, `acquires`, `borrows`), not from memory: `selfhost/ast.hero:356-379` (`Param` field) and `:650` (the empty constructor) — **and ast.hero has 4 lines of headroom against its DECIDED 525, so a field plus this repo's comment discipline forces a DECIDED bump**; `selfhost/parse/members.hero:93-123` (a marker function, ~8 lines) + `:180-184`; `selfhost/print/fmt.hero:547,563-576` (a `_suffix` function, 10 lines of headroom); `selfhost/print/dump.hero:169,190-198` (**its own second copy** of the same suffix functions); `selfhost/emit/ops.hero`; `selfhost/check/marks.hero` (a word nothing reads must be refused — **a new refusal, so `.claude/rules/verification.md` sends it to `check run emission determinism corpus`, not the `selfhost/**` row**); `spec/heroes-spec.md:389,393` grammar plus prose (`heroes measure` reads **real 7974** tokens, headroom 2266, and panel 012 wants a named removal or a registered prediction); `selfhost/cli/syntax_cmds.hero` self-check pair; `heroes mutate`; new goldens. **Measured, and it is the sharpest number here**: `editors/vscode/syntaxes/heroes.tmLanguage.json` contains **0** occurrences of `consumes`, `borrows` or `acquires`; `site/src/lib/highlight.ts` contains 2, both inside prose about string escapes. The existing three marks are **already un-coloured** — a fourth inherits an open outward-facing debt, and both files are on the push question.

### Route D — no lines, because it does not work

### Route E — a new diagnostic in a checker with no flow analysis (`check/leasing.hero:29`, `check/consuming.hero:22`), plus its golden and annotation

## Does the guard cost anything at run time

One handle call in a tight loop, emitted C, arg wrapped in a cross-TU `hero_handle_nonnull`, both compiled with `selfhost/cli/flags.hero`'s own 14 flags, `/usr/bin/time -p`, five alternating rounds, machine still:

**10^8 iterations — unguarded `real` 0.37 0.39 0.38 0.36 0.37 s; guarded 0.37 0.37 0.37 0.37 0.37 s.** Medians identical at 0.37 s; the guarded run is inside the unguarded run's own spread. At the brief's **10^6 iterations both read `real 0.00` s**, under the clock's 0.01 s resolution. `user` tracked `real` in every round, so no run was waiting.

And the guard is not foldable, which is the whole point. Same witness at four levels:

| level | HEAD | with the guard |
|---|---|---|
| `-O0` | `panic: a null pointer was read through …` exit 134 | `panic: a null handle was passed to a C function` exit 134 |
| `-O1` / `-O2` / `-O3` | **exit 133, stdout 0 bytes, stderr 0 bytes** | exit 134, named, all three |

**A correction to the shared brief's table, offered because it makes defect 045 worse rather than better**: its `-O2` row reads *prints a value for a node that does not exist, exit 0*. The same shape re-measured here is **exit 133 (SIGTRAP) with both streams empty** at -O1, -O2 and -O3 — clang emitted a trap rather than a wrong answer. Same §1.12 class, louder symptom, still unreachable by any runtime handler.

## Route D's premise against real headers, run on this Mac

The corpus first: **74 handle parameters** across `examples/`, `tests/golden/`, `spec/` — **36 UNMARKED, 27 `consumes`, 11 `acquires`** (all eleven `@` out-parameters, which route A excludes as `@cstr` already is). D reads `consumes` as *accepts NULL, do not guard*.

**Falsified in the direction that kills programs**, two release-shaped functions a binding author marks `consumes`:

- `closedir(NULL)` → **exit 139**
- `pthread_mutex_destroy(NULL)` → **exit 139**

That is defect 045's exact symptom — signal, empty streams — reintroduced by the repair. `free(NULL)`, `freeaddrinfo(NULL)`, `fclose(NULL)`, `pclose(NULL)`, `iconv_close(NULL)`, `curl_easy_cleanup(NULL)`, `sqlite3_close(NULL)`, `sqlite3_finalize(NULL)` all exit 0 — so the premise is *usually* right, which is what makes it dangerous.

**And falsified in the other direction too**, on UNMARKED parameters the corpus actually ships: `sqlite3_errmsg(NULL)` → exit 0, returns `out of memory`; `sqlite3_step(NULL)` → exit 0. D guards both and refuses two documented-legal calls.

Wrong in both directions, and it rests on a premise about the world, which `.claude/rules/module-shape.md` forbids a narrowing outright.

## What route A actually costs a program, measured rather than assumed

Route A simulated by wrapping every handle argument in the emitted C of the two shipped SQLite bindings, compiled at `-O2`:

- `examples/sqlite/main.hero` — **7 of 7** handle arguments wrapped, `sqlite3_close` and `sqlite3_finalize` among them: stdout byte-identical, exit 0 both.
- `examples/ledger/main.hero` — **17 of 17** wrapped: 29 lines of stdout byte-identical, exit 0 both.

**One refusal in the whole corpus, and it is the `freeaddrinfo(NULL)` the brief asked me to price.** `tests/golden/run/fixedbugs-getaddrinfo-is-bindable.hero` holds `res: AI @ nullptr` … `freeaddrinfo(res)`:

| | HEAD | route A |
|---|---|---|
| `127.0.0.1` (the shipped golden) | `0`, exit 0 | `0`, exit 0 — **identical, suite stays green** |
| an unresolvable host | `8`, exit 0 | `8`, then `panic: a null handle was passed to a C function`, **exit 134** |

So route A costs **zero executed call sites** and **one latent failure path**, repairable in the program by one line (`if rc == 0` before the release), which is arguably the more correct program. That single case is the whole evidentiary basis B would ever need — and there is exactly one.

## Why E cannot be the resolution

`grep -E "[a-z_]+: nullptr"` over `examples/` and `tests/golden/` finds **11** literal-`nullptr` arguments and **not one is handle-typed** — `callback`, `context`, `error`, `service`, `hints` are all declared `ptr` (`fixedbugs-getaddrinfo-is-bindable.hero:22`). The null in defect 045 comes **from C**: a producer returning NULL. Even the witness's own null arrives through a cell (`nullread/main.hero:37`), so a literal-argument rule misses the very program that provoked the defect; and the only rule the checker can express without flow analysis — the declaration's initialiser, `check/leasing.hero:25-29`'s own shape — would refuse **three shipped correct programs** (`examples/sqlite/main.hero:97`, `examples/ledger/db/sqlite.hero:224` and `:283`, all `@ nullptr` then filled by an `@` out-parameter, one of them closed on the failure path with a two-platform measurement in its comment). `tests/golden/fixedbugs/ffi-a-null-function-pointer-says-so.hero:11-18` is the ratified ruling on the identical question one argument over, under the heading *WHY IT COULD NOT BE REFUSED AT COMPILE TIME*.

## argument

§1.12 already decided this, in words: check rather than assume, at the boundary, **even where the value is known to be good**, one predictable branch per FFI argument, paid deliberately. The `cstr` half of that sentence shipped at `ops.hero:278`; the handle half is the same sentence with `.named` beside `.cstr`. Measured: 14 code lines, 4 files, 0 checker rules, 0 surface, 0 spec tokens, 0 `emit`/`ir` goldens, 28 lines of headroom left, 0.37 s against 0.37 s at 10^8 calls, 24 of 24 shipped handle arguments unchanged. D is unsound (exit 139, twice). C inverts a stated default and leaves 36 parameters open on landing day. E misses its own witness. B is right later, on the one case A refuses.

## prediction

At the close of the milestone that closes `docs/work/DEFECTS.md` 045: route A lands in **≤ 40 added non-blank lines across ≤ 5 files**, `selfhost/emit/ops.hero` measures **≤ 290 code_lines**, `DECIDED` still holds **19 rows**, and **zero** `.expected` files under `tests/golden/emit/` or `tests/golden/ir/` differ. Wrong if any one of the four is false — in particular if a new parameter word lands with it, which would put `selfhost/ast.hero` over its DECIDED 525 and make the fifth item true.

```sh
T=$(git tag --list 'm-*' --sort=creatordate | tail -1)
git diff $T^ $T --stat -- selfhost/ runtime/
git diff $T^ $T --name-only -- tests/golden/emit tests/golden/ir | wc -l
grep -c '"selfhost' tests/harness/suite_layout.hero
cp selfhost/emit/ops.hero /tmp/o && for i in $(seq 1 1000); do echo PAD; done >> selfhost/emit/ops.hero
./heroes run tests/harness/main.hero -- ./heroes layout | grep ops.hero   # subtract 1000
cp /tmp/o selfhost/emit/ops.hero
```

## condition

I withdraw the veto on **D** if somebody produces a rule over real headers whose measured false-negative set on this Mac is empty — `closedir` and `pthread_mutex_destroy` are the two it has to survive, and neither header states anything. I withdraw the **object** on **B** the day a *second* shipped program is refused by route A (the first is the getaddrinfo failure path above): two is a class, one is a witness, and `.claude/rules/module-shape.md` is explicit about the difference. I move **A** to object if the guard is implemented as a `static inline` in the emitted C rather than a cross-TU runtime entry point — that version is foldable in principle and would have to be re-measured at `-O3` before I believe it. And I withdraw the veto on **E** as an *addition* to A, never as a replacement: if it is written as the initialiser rule it needs `examples/sqlite/main.hero:97` to keep compiling, which today it would not.