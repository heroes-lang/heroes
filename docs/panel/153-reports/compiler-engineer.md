# Panel 153 — report of the compiler-engineer

**Verdict: Route A — object. Route B — adopt, on four conditions. Third route — none found; the search is named in § 2. No veto**: neither route adds a construct that the checker AND the lowering AND the backend must all learn (design.md §1.7, Part 5), and neither breaches the ceiling as the instrument defines it (`tests/harness/suite_layout.hero`, HEAD: `CEILING` 300 and a `DECIDED` row per file), though each lands beside a file with zero room today.

- `verdict`: Route A object; Route B approve with conditions; no third route.
- `section`: design.md §1.7 (core plus elaboration), Part 5 (core #3 *read a field*, core #7 *primitive operations*), §1.12 (robustness decides between two admissible forms), §1.1 (simplicity is the ceiling).
- `implementation_cost`: Route A about 70 to 85 code lines across 13 to 15 `selfhost/` files, 0 runtime, 0 IR, three checker refusals owed, `check/walk.hero` at 1861 of 1870 taking 10 of them; Route B about 65 to 80 code lines across 8 to 10 files, 0 runtime, 0 IR, one new primitive, `check/builtins.hero` at 377 of 377 taking a 3-line hook unless it moves. Table in § 1.
- `needed_for_self_hosting`: no. `selfhost/` declares **0** tagged group records and **4** `extern` groups (2 over `hero_os.h`, 2 over `stdlib.h`; `grep -rnE '^\s+record [A-Za-z_]+ tag ' selfhost/` and `grep -rhoE '^extern "[^"]+"' selfhost/`). The entry ticket is §1.12's *the boundary is complete*: `netdb.h` on this Mac declares **19** functions returning a struct pointer (`grep -cE '^\s*struct [a-z_]+\s*\*\s*[a-z_0-9]+\s*\(' $SDK/usr/include/netdb.h`; the shared brief counted 18 with another regex).
- `argument`: Both routes put a dereference where the compiler has never had one. Route A hides it inside core construct #3: the IR keeps `.field`, the emitter writes `->`, but three shapes the grammar already admits (a field write, construction from fields, `@ai.f` copy-out) become writes into memory C owns unless the checker refuses each, and `write_target` (`check/walk.hero:842`) plus `construct_record` (`:1849`) sit 9 code lines under a 1870 ceiling; `emit/extern_field.hero:49` would spell `((struct addrinfo * *)0)->ai_family`. Route B adds one primitive, `read`, with one guard, and nothing it admits can write through the pointer; its two premises compiled and ran today. §1.12 decides: where a missed refusal corrupts under A and copies under B, B wins at comparable cost.
- `prediction`: § 5.
- `condition`: § 6.

## 1. Lines and modules

Unit: **code lines** as `suite_layout.hero:502-517`'s `code_lines` counts them (test blocks and blank lines excluded). I did not run the layout suite (the harness is 16,621 physical lines in 30 files); I mirrored `code_lines` in awk and calibrated it: `emit/structural.hero` reads **323** today and `suite_layout.hero:361-362` recorded **323** on 2026-09-14, the file's last change (`2c7831ac`). Ceilings are HEAD's (`git show HEAD:tests/harness/suite_layout.hero`, lines 43-44 and 386-404). Physical size for scale: `selfhost/` is **61,585** lines in **212** files, `runtime/` **6,328** lines (`wc -l`); Pascal-P4's 4000 was passed long ago, so the operative ceiling is the per-file table.

**Files at zero room today** (measured = `DECIDED`): `emit/ctype.hero` **395/395** (388 when the row was set on 2026-09-14, `suite_layout.hero:361`; grown 7 by defect 037's repair `cfda0ad2` today), `check/builtins.hero` **377/377**, `emit/inst.hero` **350/350**, `value_errors.hero` **334/334**. Near: `ast.hero` 521/525, `check/walk.hero` 1861/1870, `emit/gate.hero` 360/365, `print/fmt.hero` 1165/1175, `parse/tails.hero` 288/300.

**Route A** (fields on a handle, `->`, a new word):

| file | today | change | after |
|---|---|---|---|
| `ast.hero:484-489` | 521/525 | one `bool` on `record_decl` | 522 |
| `resolve/types.hero:230` | 189 | the one non-test `.record_decl(` constructor (8 sites total, 6 inside tests) | 190 |
| `parse/tails.hero:238-264` | 288/300 | a marker mirroring `partial_marker` (5), threaded to `record_tail` and `is_handle_parts` (2) | ~295 |
| `handles.hero:51-69, 113-116` | 156 | predicate parameter and clause (3); `pointee()` as the inverse of `c_spelling` (4) | ~163 |
| `check/sized.hero:200-203` | 253 | `is_aggregate` has no handle arm, so `ai_next: AI` is a back edge and `no_size` fires (documented at `reaches.hero:38-45`); skip edges out of a handle (3). Inferred, unrun | 256 |
| `check/walk.hero:842` `write_target`, `:1849` `construct_record` | 1861/1870 | refuse a field write through a handle (6), refuse construction from fields (3), `use handles` (1; not imported today) | **1871, red**; or a new `check/through.hero` (~40) with two 1-line hooks, 1863 |
| `ffi_errors.hero` | 268 | two diagnostics (~20) | ~288 |
| `emit/aggregate.hero:280-285` | 279 | `->` when the owner is a handle (4) | 283 |
| `emit/access.hero:124-141` | 124 | the null guard (5), § 4 | 129 |
| `emit/extern_record.hero:40-60, 100` | 194 | pointee spelling for the assertions (2), skip the completeness probe for a handle (1) | 197 |
| `emit/extern_union.hero:103` | 186 | pointee for `*(c_type *)0` (2) | 188 |
| `print/fmt.hero:448-455`, `print/dump.hero:93-100` | 1165/1175, 198 | the word (3 + 3) | 1168, 201 |
| `emit/ctype.hero` | 395/395 | **must stay 0**: the pointee lives in `handles.hero` and `handle_decls` (`ctype.hero:78`) already carries the bit | 395 |
| `ir.hero`, `ir/*`, `runtime/` | 310/310, 6,328 phys. | 0: the `.field` op is unchanged; `hero_panic` exists (`runtime/heroes_runtime.h:39`) | |

Total about **70 to 85 code lines** in 13 to 15 files (about 110 with the separate module). `editors/vscode/.../heroes.tmLanguage.json`, `site/src/lib/highlight.ts` and `mutate/handles.hero` know neither `tag` nor `partial` today (`grep -c partial` is 0 in each), so 0 by precedent, CL-036 notwithstanding.

**Route B** (two records on one tag, a `read` built-in):

| file | today | change | after |
|---|---|---|---|
| `check/decls.hero:285-303` | 196 | key `seen` on tag plus kind (4) | 200 |
| `handles.hero:126-142` | 156 | reword `duplicate_tag`'s reason and note, § 3 (0 net) | 156 |
| `emit/ffi_tag.hero:180-192` | 197 | kind-aware `record_by_tag`, or a sibling (4 to 8) | ~205 |
| `inventory.hero:89-90` | 138 | `Builtin(name: "read", …)` (1) | 139 |
| `check/builtins.hero:281-285` | 377/377 | the 3-line hook `lend_call` uses | **380, red**; or the hook at `walk.hero:1240` (call dispatch), 1864/1870 |
| new `check/reading.hero` | 0 | type `h.read()`: the fielded record sharing the handle's tag is the result type (~30 to 45; `lending.hero` is 241 for two forms with a position rule) | |
| `ffi_errors.hero` | 268 | `read` on a handle whose tag has no fielded record (~10) | 278 |
| `emit/ops.hero:56-100` | 258 | a special arm beside `print` and `to_`: guard then `*t0` (8); `emit/builtins.hero` untouched, `read` is not a runtime call | 266 |
| `ir.hero:74-75`, `ir/*`, `runtime/` | | 0: `.call(callee: .builtin_fn)` exists | |
| a fielded record holding a handle-typed field | | **0, measured**: `handle_field.hero` (`record IfAddrs tag ifaddrs partial` with `ifa_addr: SockAddr`, `record SockAddr tag sockaddr`) built with the seed and ran, exit 0, printing `7` and `true`; its emitted assertion reads `_Generic(&((struct ifaddrs *)0)->ifa_addr, sockaddr * *: 1, default: 0)` and went through the `struct` round trip | |
| `@hints: AddrInfo` against `const struct addrinfo *` | | **0, measured**: `at_on_in.hero`, `function asctime(@t: Broken) -> cstr` over `const struct tm *`, built and ran, printing `Sun Jan  1 00:00:00 2000`; emitted `asctime(&h0_t)` | |

Total about **65 to 80 code lines** in 8 to 10 files. `two_tags.hero` confirms the only thing standing in the way today: `error[duplicate_tag]` at `check/decls.hero:299`.

## 2. Core or sugar

Neither is sugar: nothing is erased on the way into the IR. Neither is a three-pass construct either. Route A leaves the checker's read path (`check/access.hero:139-160` looks fields up on `.named` and would find them) and the IR (`.field`) untouched; it is a flag on `record_decl` plus a backend spelling, not a new kind of type: the type table keeps `.named` and `ctype.Names.handle_decls` already carries the bit. What it adds is in the checker's *refusals*. Route B adds one primitive operation (Part 5, core #7), registered in `inventory.hero`, typed in the checker, emitted specially: the `lease`/`end_lease` shape, which touches 6 files in 40 mention-lines today (`grep -rn end_lease selfhost/ | wc -l`).

**Route B's `read` cannot be written in Heroes over what exists** because no operation yields the struct a pointer points at, and no relation but the tag, which only the compiler reads, ties the handle type to the fielded type; `validated` is a Tier-2 library function (`library_source.hero:194`) only because a C runtime function does the copy into one fixed result type, `str?`, while `read`'s result type differs per handle.

**Third route: none found.** Searched: the IR's op list (`ir.hero:60-130`, no dereference), the Tier-2 library mechanism (above), generics (no relation between two named types), and the `@` copy-in path (`emit/signature.hero`, the `asctime(&h0_t)` measurement): every route needs the dereference born somewhere, and A's backend spelling and B's primitive are the two places. `--dump-ir` is the evidence §1.7 names: under A a handle read prints as `.field` exactly like a struct's; under B as a `.call` of a `builtin_fn`.

## 3. What breaks

- **`one_tag_one_type` under B.** Its stated reason (`handles.hero:126-132`: two Heroes types over ONE C type, so clang accepts the swap) does not reach T against `T *`: those are two C types and clang refuses the swap, which the compiler already leans on (the `_Generic` assertion above distinguishes pointer depth at compile time). Re-key on (tag, handle-or-fielded); the note's repair *give the two types two tags* becomes false for this pair and must be reworded. The live defect is `record_by_tag` first-wins (`ffi_tag.hero:180`): its callers at `:159` and `:210` would caret the first of two records.
- **`==` and map keys under A**: unchanged, by reading. `structural.hero:71` returns on `at.handle` before walking fields; `map_keys.hero:88` asks `reaches_handle`, whose walk answers on `is_handle` (`reaches.hero:111`, `:161`) before the `seen` check and before descending fields, so a self-typed field is never walked and the walk terminates; `handle_map_key` still refuses. Unrun, because the form does not exist to compile.
- **`sized.hero` under A**: row in § 1; `no_size` on `ai_next: AI` unless a handle arm is added.
- **`extern_field.hero:49` under A**: `place = "((" + c_type + " *)0)->" + member` with the handle's `c_type` `struct addrinfo *` spells a member of a pointer-to-pointer. Under B: nothing, measured.
- **The completeness probe under A** (`extern_record.hero:100`, `!rec.partial && rec.fields.len() > 0`): a through-handle naming 2 of `addrinfo`'s 8 fields would be probed and refused unless skipped, so its field list is partial by construction, while `partial` today forbids `==` and a handle compares by address (`spec/heroes-spec.md` § 13). The panel must say which it is. Under B the fielded record is `partial` or complete exactly as `examples/ctime/main.hero`'s `Broken` is.
- **Write paths under A.** A field write lowers to `.store(place:)` (`ir/flatten.hero:95`) and emits a member store, measured on `field_write.hero`: `h0_t.tm_year = t4;`; `@` copy-out is the same member store. With `->` on a handle base these write C's memory (CLAUDE.md § Precedence rank 3). `write_target`'s `.field` arm returns `read` today (`walk.hero:842-862`); `construct_record` (`:1849-1877`) has no handle arm (grep for `handle`, `fields.len`, `is_handle`: 0 hits), and where `Db()` is refused today I did not locate (panel 145 says it is; `tests/golden/check/` has no `Db()`, `AI()` or `Curl()`); `@ai.f` aliasing is judged in `resolve/writes.hero` (283 code lines, 0 references to the type table), which cannot see a handle base, so a checker-side hook is owed. Three refusals, or three corruption classes.

## 4. The null read

Measured in C: `null_read.c` (`struct addrinfo *ai = NULL; ai->ai_family`) exits **139** with no output; `guarded_read.c` prints `panic: \`AI.ai_family\` read through a null handle` and exits **134**. The emitter writes, before the read (`access.hero:124` under A, `ops.hero:56` under B):

```
    if (t0 == NULL) hero_panic("`AI.ai_family` read through a null handle");
```

`hero_panic` is `_Noreturn` at `runtime/heroes_runtime.h:39`: **0 runtime lines, 5 emitter lines**, one predictable branch per read, the price §1.12 already pays per `cstr` argument. Under A that is one branch per **field** read (a walk of n nodes reading k fields: k·n); under B one per `read()` (n). Both sites hold `decls` and `s`, so the message can name record and field (§4.17). Optional: `hero_handle_read(const void *, const char *what)` in `runtime/parts/panic.c`, about 5 C lines plus 1 header line, for one uniform message.

## 5. Prediction

If Route B lands: the commit range that closes defect 042 changes **no IR file**, and its delta over `selfhost/` in the layout unit is **at most 90 code lines**. Scored at the closing tag by:

```
git diff --stat <before>..<closing> -- selfhost/ir.hero selfhost/ir/ | tail -1        # prints nothing
for f in $(git diff --name-only <before>..<closing> -- 'selfhost/*.hero' 'selfhost/**/*.hero'); do
  a=$(git show <closing>:$f 2>/dev/null | awk "$CL"); b=$(git show <before>:$f 2>/dev/null | awk "$CL"); echo $(( ${a:-0} - ${b:-0} ))
done | paste -sd+ | bc                                                                   # <= 90
```
with `CL='BEGIN{i=0;n=0}{r=$0;if(index(r,"test \"")==1){i=1;next}if(i&&length(r)>0&&substr(r,1,1)!=" "&&substr(r,1,1)!="\t"&&substr(r,1,1)!="#")i=0;if(!i){t=r;gsub(/^[ \t]+|[ \t]+$/,"",t);if(length(t)>0)n++}}END{print n}'`, the mirror of `suite_layout.hero:502-517`. If Route A lands instead, I predict `git show <closing>:tests/harness/suite_layout.hero | grep walk.hero` reads above 1870, or a new `check/` module exists.

## 6. Conditions

Route B is adopted only with: (1) the checker hook lands where a ceiling has room (`walk.hero:1240` or a new `check/reading.hero`) and `check/builtins.hero` stays 377, or its `DECIDED` row is raised with the reason written in the table as the file's convention requires; (2) `record_by_tag` becomes kind-aware or its two callers are audited for two records on one tag; (3) `nullptr.read()` aborts through `hero_panic` naming the record, with a `run` golden exiting 134 and not 139; (4) `duplicate_tag`'s reason and note are rewritten to the C-type argument. What would move Route A to adopt: a compiled demonstration that the three refusals fit in one module of at most 60 code lines, plus a ruling that a through-handle's field list is partial by construction with `==` by address. What would move Route B to object: a measured program in which the whole-struct copy is *wrong* rather than slow.

## Process notes

- The scratch directory my brief named **pre-existed** with `tests/harness/suite_layout.hero` modified (`CEILING` 0 and every `DECIDED` row 0, mtime 2026-09-15 14:38); `diff -rq` against `/Users/joseph/Temp/heroes-lang` (excluding `.git`, `build`, `heroes`) found **no other difference**, and the original is clean at HEAD, so every count above stands and every ceiling is quoted from `git show HEAD:`. My own `cp -r` nested itself at `seat-engineer/heroes-lang` and was not used. The seed built in 4.27 s real.
- Unrun: the layout suite itself; any Route A or Route B program, since neither exists, so every `+N` is an estimate against the cited lines and not a diff. Measured: the five probe programs and three C files named above, all in the scratchpad, nothing written into the repository.