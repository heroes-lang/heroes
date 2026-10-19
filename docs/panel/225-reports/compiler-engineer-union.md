# Panel 225, compiler-engineer, third step: the union, built before the synthesis

Written 2026-10-19 as it was run, in my own copy
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/225-compiler-engineer`, on top of the diff
that is `heroes-g` (saved first as `ce-work/g.diff`, 750 lines, with `ce-work/g.probe_cut.hero` and
`ce-work/g.value_early.hero`, the two new files). Read first: `225-briefs/compiler-engineer-union.md`
and `225-reports/critic.md` (the critic's second pass); FP's diff and its report §§ 7 and 12 READ in
`.claude/worktrees/225-ffi-pragmatist` (`git -C ... diff`), never built there. **s2** is my
unmodified stage 2 (`./heroes`, `sources 08c492ac05cbb532`); **CE** is `heroes-g` (`sources
2a4331af563a2a10`); **the union** is the compiler this step builds. Every number carries its
command; a claim not run says *unrun*. **In progress.**
**Complete, 2026-10-19** (the verdict is § 12).

## 1. What the union takes from whom, and the design of the mark

| part | from | what it does |
|---|---|---|
| the cut confined to a copy | FP (`Unit.cut`) | a unit that compiled is probed as today, its text never cut: `forge1` |
| the copy's extent: the opening and the asked accessors | FP | one `CUT` mark before the prelude's assertions, never closed until an asked accessor: the typedefs, the string table, every binding's check and body are cut: `tdef` |
| a refused accessor written as the program writes it, cut, left out of `Unit.defined` | CE | no inert twin: `twin`; the false message of defect 2145 stays repaired |
| E, one unit at a time | CE | `first` |
| F at `check_all`, `lock_told`, `two_ways`/`defined_twice` | CE | 2136, L1 |
| a finding the refusal already tells, dropped | FP (`whose.with_beside`) | with a compiler test, which nothing exercised before |
| G (a), the keyed mark | the critic's § 7, the coordinator's measure on the box | one identifier, `hero_probe_cut_<key>` or `hero_probe_keep_<key>`, which no macro of a header can rewrite without knowing the key |
| G (b), the marks matched | the critic's § 7 | the preprocessed text must carry the key on exactly the lines the copy wrote, in their order, or the copy is silent |

**The key, and what it is digested from**: `digest(copy + "|" + reads)`, where *copy* is the copy's
own text as the writer wrote it with its marks unkeyed (the literal `unkeyed`), and *reads* every
file the build's probes found its headers read, transitively (`probing.digests_of`, the `-M`
listing each module's header probe takes), as `path=digest` lines in the map's own order. It is
**deterministic**: the same program, headers and compiler write the same copy and the same key, so a
warm build reads the copy's kept verdict under its `value-<key>`. **Why no header can know it**: the
key covers every header's own bytes, so a header holding its copy's key holds a digest of itself;
the digest (`cli/digest.hero`, two polynomial hashes mod 2^31-1, 62 bits) is not a cryptographic
hash, so a header built to that end is a fixed-point search, not an impossibility, and that is why
**(b) carries the soundness and the key does not**: a header that wrote a keyed line, or rewrote one
through a macro on an MSVC target, changes the lines that carry the key, and the copy is silent.
The key keeps a header that was not written against this very program from silencing the probe.

**One word the probe adds, defect 2146** (found by the coordinator, filed `blocking`, to be repaired
in the lane after the union lands, so not repaired here): `value_run.preprocessed` adds `-D
main=hero_probe_hidden_main` to the unit's own words, so a header testing `main` reads otherwise
under every probe, a compiled unit's and a copy's alike. **No sentence of this report says the copy
reads every expansion as the program's unit does**: what holds, and what the union's own comments
now say (`emit/probe_cut.hero`, `cli/value_beside.hero`, `cli/value_early.hero`), is that the copy
reads every expansion as the probe of a unit the build compiled reads it. My first report's four
sentences that claimed more carry a dated correction underneath (`compiler-engineer.md`).

## 2. What changed, on top of `heroes-g`

- `emit/probe_cut.hero`, rewritten: the keyed marks (`cut_mark`, `keep_mark`, `UNKEYED`), `keyed`
  (every whole mark line made the key's, so a name of the program spelling the same words is left),
  and `cut(copy, preprocessed)`, which reads the key from the copy's first mark, requires every line
  of the copy that carries the key to be a mark, requires the lines of the preprocessed text that
  carry it to be **exactly** the copy's marks in order, and only then cuts each `CUT` to its `KEEP`.
  Three tests: the cut of a keyed copy; **one keyed line too many** (a header's forged mark), **one
  too few** (a mark rewritten as clang does for an MSVC target, `#pragma 3`) and the key inside a
  header's string, each refused; an unkeyed mark, FP's mark and CE's old mark left as text, a
  carriage return ending a mark's line, a copy with no mark refused.
- `emit/decls.hero`: the prelude opens the cut before the first assertion and no longer closes it,
  so the assertions, the string table and the typedefs stand inside it (FP's extent).
- `emit/unit.hero`: the cut closes around an accessor the probe asks (`value_probe.judged`) that
  clang did not refuse, and around nothing else; the closing `KEEP` at the end stays.
- `emit/writer.hero`: the marks written unkeyed (`probe_cut.UNKEYED`).
- `cli/value_run.hero`: `Unit.cut` (FP's); `preprocessed` cuts a copy alone, and a copy whose marks
  are not its own is `broken`, which beside a refusal is the copy's silence.
- `cli/value_beside.hero`: `again` takes the build's `reads` and returns the copy keyed, `cut:
  true`; `keyed` and `reads_of` (the header digests, `sort(keys(...))`, one `path=digest` line each).
- `cli/assemble.hero`: `reads_of(lent.digests)` for every copy; a compiled unit's `Unit` says `cut:
  false`. `cli/value_early.hero`: F's copies keyed with `probing.digests_of(planned.probings)`.
- `cli/whose.hero`: FP's `tells` in `with_beside`, and its first test.

`./heroes check selfhost/main.hero`: clean (exit 0), after the language's own rule refused my first
calls of `keyed` (*two of `keyed`'s parameters are `str`, so every one of them is named at the call
site*). Built: `./heroes build selfhost/main.hero -o heroes-u`, `real 35.35 user 34.81 sys 0.51`,
`sources 40ad11812c8aa370`. By `layout`'s own count, mirrored (test blocks and blank lines out):
`rounds.hero` 293, `decls.hero` 299, `writer.hero` 299, `whose.hero` 244, `value_run.hero` 247,
`unit.hero` 208, `value_beside.hero` 190, `probe_cut.hero` 141, `value_early.hero` 102.

## 3. The mark's premise, reproduced on this Mac

`ce-work/marks/marks.c` defines `cut 1`, `on 2`, `hero_probe_cut on_by_macro` and `heroes_probe 3`,
then writes CE's old mark, FP's mark, the two keyed marks, and a keyed mark by `_Pragma` from a
macro inside an expression; `clang -E -P <flags> marks.c`:

| flags | `#pragma hero_probe_cut on` | `#pragma heroes_probe cut` | keyed marks | `_Pragma` of a keyed mark in `int c = (FORGE 3);` |
|---|---|---|---|---|
| none (this Mac's default target) | as written | as written | as written | `int c = (` / the mark / `3);`: a line of its own |
| `-fms-extensions` | `#pragma on_by_macro 2` | `#pragma 3 1` | as written | the same |
| `--target=x86_64-pc-windows-msvc` | `#pragma on_by_macro 2` | `#pragma 3 1` | as written | the same |
| `--target=x86_64-pc-linux-gnu` | as written | as written | as written | the same |

The coordinator's measure on the box, reproduced: **a mark of two tokens can be rewritten by any
header that defines one of them; one identifier no macro holds prints itself.** And a forged keyed
mark written by `_Pragma` lands on a line of its own, which `probe_cut.carrying` counts, so the
match refuses it (the case "one keyed line too many" of the compiler test).

## 4. Every program, by s2, CE and the union

Sources copied (`cp -p`, no `build/`) into `ce-work/union-src/` from the critic's
`critic-work/pass2/`, FP's `ffi-work/cnt/` and my own `ce-work/`; each built in a fresh copy of its
directory by `ce-work/table.sh` (`<compiler> build [-O2] <p>.hero -o out`, a binary that built run
under `timeout 10`, 200 bytes at most), 43 programs and 129 builds, `ce-work/table-u.tsv`. *Told*
is each `error[code]` with the first name it quotes.

| program | s2 | CE (`heroes-g`) | **union (`heroes-u`)** |
|---|---|---|---|
| `tdef` | `ffi_tag_is_a_union` | the same | **+ `K_DIV`** |
| `twin` | `ffi_unknown_name` | + `K_DIV` | **+ `K_DIV`** |
| `first` (two units) | `ffi_header_refused` | + `K_DIV` | **+ `K_DIV`** |
| `mixed` | `K_SHIFT`, `narrow` | + `K_DIV`, `K_FLT` | **+ `K_DIV`, `K_FLT`** (`K_SHIFT` once) |
| `forge1` (correct, CE's old mark in its header) | exit 0, `5` | **exit 2** | **exit 0, `5`** |
| `forge2c` | `narrow` | `narrow`, **false `K_SZ`** | **`narrow`** |
| `forge2c/forge2-next` | exit 0, `-1073741821` | **exit 2** | **exit 0, `-1073741821`** |
| `forge2p`, `forge3p` (FP's mark) | `narrow` | `narrow` | `narrow` |
| `forge2p/-next`, `forge3p/-next` | exit 0, `-1073741821` | the same | the same |
| `forge3c` (CE's mark by `_Pragma`) | `narrow` | `narrow`, **false `K_SZ`** | **`narrow`** |
| `forge3c/forge2-next` | exit 0, `-1073741821` | **exit 2** | **exit 0, `-1073741821`** |
| `optoff/opt` `-O0` | `K_OFF` | the same | the same |
| `optoff/opt` `-O2` | `K_OFF`, `K_BCP` | the same | the same |
| `optoff/opt-next` `-O0` / `-O2` | exit 0, `301` / `K_BCP` | the same | the same |
| `fword/fshift` (F, lock) | `ffi_lock_copied` | + `K_SHIFT`, `K_DIV` | **+ `K_SHIFT`, `K_DIV`** |
| `fword/fshift-next` | `K_SHIFT`, `K_DIV` | the same | the same |
| `fword/fi8` | `ffi_lock_copied` | the same | the same (CE's residue 3) |
| `forgeF/ff` (F beside a forged mark) | `ffi_lock_copied` | + **false `K_SZ`** | **`ffi_lock_copied`** |
| `forgeF/ff-next` | exit 0, `-1073741824` | **exit 2** | **exit 0, `-1073741824`** |
| `line/asw` | `K_NONE`, `K_LINE` | the same | the same |
| `line/deleted` | exit 0, `1073741824` | the same | the same |
| `line/renamed` | `K_LINEX`, `K_LINE` | the same | the same |
| FP's `cnt` | `K_BAD`, `K_CNT2` | `K_BAD` | **`K_BAD`** |
| FP's `cnt/next`, `cnt/next32` | exit 0, `-1073741821` | the same | the same |
| `d2137` | `narrow` | + `K_DIV` | **+ `K_DIV`** |
| `d2137-next`, `d2136-next`, `ctr-next` | `K_DIV` | the same | the same |
| `d2136` (F, `check_all`) | `fill` | + `K_DIV` | **+ `K_DIV`** |
| S1 | `narrow` | + `K_DIV` | **+ `K_DIV`** |
| `s1-next`, S2 | `K_NONE`, `K_DIV` | the same | the same |
| U1 | union | + `K_DIV` | **+ `K_DIV`** |
| L1 (F, lock) | lock | + `K_DIV` | **+ `K_DIV`** |
| `ctr` | `narrow` | + `K_DIV` | **+ `K_DIV`** (no `K_CTR`) |
| `ctr-only`, `inert-fixed` | exit 0, `3`, `1048576` | the same | the same |
| `inert` | `K_BAD`, **false `K_CTR`** | `K_BAD` | **`K_BAD`** |
| `inert-only` | `K_CTR` | the same | the same |

**What the union owed, measured**: wherever either prototype tells a value, it tells it (CE's here
in every row; FP's `tdef`, which the critic measured on `heroes-p`, told by the union too); **no
`forge*` program tells `K_SZ`**; **every program s2 builds, the union builds**, with the same
output (`forge1`, the four `forge*-next`, `ff-next`, `opt-next` at `-O0`, `deleted`, FP's two
`next`, `ctr-only`, `inert-fixed`).

## 5. A header that learns the key: (b) carries the soundness, the key does not

The critic could not run this against the union, whose key did not exist. **Defect 2146 gives a
header a way to learn it and write it**: a file the header includes only `#ifdef main` is read by
the probe's preprocessing (`-D main=...`) and by no listing of the build's, so the key, which covers
the files the build's probes list, does not cover it. `ce-work/union-src/fk/`: `fk.h` is
`forge2c`'s header with the mark replaced by `#ifdef main` / `#include "fk-evil.h"` / `#endif`;
`fk.hero` binds `narrow(x: i64)` and `K_SZ`.

1. A first build with `fk-evil.h` empty: `../../heroes-u build fk.hero`, `narrow` alone, exit 1.
   The copy, `build/value-33c028c32fd6a4c7/fk.c`, holds four marks (lines 59, 122, 153, 209) and
   the key `11507c9347fa65e8`. `fk-evil.h` is then written `#pragma hero_probe_cut_11507c9347fa65e8`.
2. **The union, cold** (`rm -rf build` first, since the first build's kept verdict would be read
   back: the key does not change): **`narrow` alone, exit 1**. The copy carries the same key; its
   directory holds no `verdict.txt`. By hand, `clang @<the copy's words> -E -D
   main=hero_probe_hidden_main <copy>` shows five lines carrying the key, the forged one at line
   600 and the writer's four: the match refused, and the copy said nothing.
3. **The union without the match** (`heroes-nob`, `sources 3bbd8bd40b2aa3e5`: the one comparison
   in `probe_cut.cut` left out, built, the file restored at once, `cmp` silent): **`narrow` and
   `K_SZ`**, *... traps where it reads it*. **False**: `fk-next.hero` builds under s2, CE and the
   union and prints `-1073741821`, `K_SZ` defined.
4. CE on `fk`: `narrow` alone, since its fixed mark is not this one (it falls to `forge2c`).

**So the key alone does not hold** where a header can learn it, and 2146 is one way; the match
does, by construction, whatever the header knows: a line that carries the key and is not the
writer's mark, or a mark of the writer's that a macro rewrote, is a count or an order the copy did
not write.

## 6. FOUND, beside the sitting: a kept verdict does not see its header change (today's trunk)

`value_ask.asked` keys a probe's verdict by the compiler, the unit's text, its words, its level,
the runtime and the probe's own text, and **no header's content**; the seal on a kept file
(`cli/kept.hero`) guards its integrity, not its inputs. Measured on my **unmodified stage 2**
(`ce-work/stale/`): `st.h` holds `#define K_DIV 7`; `heroes build st.hero` exits 0 and prints `7`;
`st.h` is then rewritten `#define K_DIV ((-2147483647 - 1) / -1)`, and **the warm build exits 0**
(`ls build | grep -c '^value-'` still 1: no probe ran), its binary printing `6123709516`,
`6122890316`, `6164751436` over three runs; `rm -rf build` and the same header is refused,
`ffi_constant_type` for `K_DIV`, exit 1. **A wrong program accepted at exit 0** (defect 2075's own
symptom, which panel 216's probe exists to refuse), `blocking` by § Bounded discovery; **not filed
by me: the coordinator's to file**. The union's copies do not have it, their key covering every
header's digest (`reads`); the probe of a compiled unit does, in the union as in the trunk. The
repair it points at, **unbuilt**: the same `reads` in `value_ask.asked`'s key and in
`value_c2.asked`'s, so a changed header is a changed key.

## 7. What an asked accessor needs: the opening alone, records included

FP's § 12 shape, run on the union (`ce-work/r2109/`): `run/fixedbugs-2109-...` (thirteen constants
C defines, `R_OK` and `M_ROBJ_OK` records among them) with 2137's `narrow(x: i64)` added. s2 and
the union both tell `narrow` alone, exit 1, at `-O0` and `-O2`; s2 asks no copy (its 2128 path gives
up on an error elsewhere), and the union's copy holds 14 cut marks (the prelude's, and one after
each of the 13 asked accessors) and **compiled and ran**: its kept verdict reads `trap 0`, the
undefined canary alone, at both levels, and C2's (`init-5125c3e22e4ef6a3`) is empty. So nothing
beyond the opening is kept, and no case run here needed more.

## 8. Gates on the union

- `./heroes-u test selfhost/main.hero`: **`1733 tests, all passed`**, exit 0, among them, by name in
  the log, *a keyed copy loses its marked stretches after preprocessing...*, *one keyed line too
  many, or one too few, and the copy is silent*, *an unkeyed or other mark is no mark...* and *a
  probe's finding at a span the refusal tells is dropped...* (FP's de-duplication, exercised for
  the first time).
- **Nothing a build compiles moves** (`ce-work/same/`, six programs that build, by s2 and the
  union): all six exit 0 with either; `--emit-c` identical by `cmp` for all six (13,096 to 18,168
  bytes); the 13 per-module units the same contents as s2's (SHA-256 as a multiset); `hero_probe_`
  in none of them. Across the table's 43 programs the union compiled 80 units, **none holding a
  mark**; of the 44 probe texts on disk, 27 are marked copies.
- **The cost** (`ce-work/countpath/clang`, fresh copies, cold then warm; `ce-work/count/u-*`):

| program | s2 cold / warm | union cold / warm |
|---|---|---|
| `plain` | 5 / 2 | 5 / 2 |
| 2072's case | 12 / 3 | 12 / 3 |
| d2137 | 13 / 7 | 18 / 8 |
| d2136 | 10 / 5 | 15 / 6 |
| U1 | 16 / 7 | 21 / 8 |
| `inert` | 17 / 10 | 16 / 9 |

  The same as `heroes-g`'s, run for run (my first report § 9). A warm d2137 pays 8, not 18: the
  copy's verdict is read back under its key, which is the key's determinism, measured.

## 9. For the box (the marks are clang's, and Windows is where unknown pragma tokens are rewritten)

The union built there from `ce-work/union.diff` over `7b97b9438`; every program in a fresh
directory; what this Mac read, beside each, is what the box should read. **None run on the box by
me.**

1. `ce-work/marks/marks.c`, by hand, `clang -E -P marks.c` on the box's own target: the two-token
   marks rewritten (`#pragma on_by_macro 2`, `#pragma 3 1`), the keyed marks as written, the
   `_Pragma` mark on a line of its own (§ 3).
2. `heroes test selfhost/main.hero` with the union: 1733 tests passed here, the cut's three and
   `whose`'s one among them. And, from any copy's `build/value-*/probe.i`, whether the box's `-E`
   ends its lines `\r\n`: the cut takes a carriage return before a newline as the end of a mark's
   line (tested), never measured on a text clang wrote there.
3. From `ce-work/union-src/`: `repro/d2137` (`narrow` + `K_DIV`), `repro/d2136` (`fill` + `K_DIV`,
   F), `u1` (union + `K_DIV`), `tdef` (+ `K_DIV`), `twin` (+ `K_DIV`), `first` (+ `K_DIV`), `mixed`
   (+ `K_DIV`, `K_FLT`, `K_SHIFT` once), `ctr` (no `K_CTR`), `inert` (`K_BAD` alone), `forge1`
   (exit 0, `5`), `forge2c`, `forge3c`, `forge2p` (`narrow` alone, never `K_SZ`) with their `-next`
   (exit 0, `-1073741821`), and `ce-work/r2109/case.hero` at `-O0` and `-O2` (`narrow` alone, the
   copy's verdict `trap 0`). **Not on the box**: L1, `fword` and `forgeF`, whose header includes
   `pthread.h`, which the box does not have (the header's own comment).
4. **The two learnt-key programs**, each learnt there, since the key covers the box's own header
   digests: build once with `fk-evil.h` holding only its comment, read the key from the copy
   (`build/value-*/fk.c`'s first `hero_probe_cut_` line), write `fk-evil.h`, `rm -rf build`, build.
   `fk` (the forged keyed `CUT`): `narrow` alone, the copy's directory without `verdict.txt`.
   **`fk2` (the keep mark defined as a macro, `#define hero_probe_keep_<key> 3`)**: here, where the
   target does not expand a pragma's tokens, the copy answered (`trap 0`) and the build told
   `narrow` alone; **on the box the copy should be silent**, its `-E` writing every `KEEP` after the
   header as `#pragma 3`, two lines fewer than the copy wrote, and the build should still tell
   `narrow` alone. This is the one row that tests the match against the MSVC rewrite itself.
5. `ce-work/stale/` (§ 6, the trunk's, not the union's): the warm build after the header's change
   should exit 0 there too.

## 10. Every gate the sitting named, on the union (`ce-work/gate-u/`, one log each)

| gate | command (`./heroes run tests/harness/main.hero -- ./heroes-u ...` unless said) | reading |
|---|---|---|
| the critic's 17 probe cases | `unsupported fixedbugs-2072`, `-2075`, `-2101`, `-2104`, `-2109`, `-2128`; `run fixedbugs-2072`, `-2075`, `-2101`, `-2109` | 1, 5, 1, 1, 1, 4 and 1, 1, 1, 1 passed, 0 failed: **17 of 17** |
| the whole `unsupported` form | `unsupported` | **353 passed, 0 failed**, the two Windows-only cases of defect 2053 `SKIP`, exit 0, `real 134.57` |
| the whole `full` form | `full` | **40 passed, 0 failed**, exit 0 |
| `layout` | `layout` | **6 passed, 0 failed** (`rounds.hero` 293, `decls.hero` 299, `writer.hero` 299 by its rule, mirrored) |
| the compiler's own tests | `./heroes-u test selfhost/main.hero` | **1733 tests, all passed**, exit 0 |
| beyond the brief | `emit`, `order`, `canonical` | 13/0, 3/0, 2/0, each exit 0 |

**Owed before it lands, unrun here**: the whole `run` form, `emission`, `determinism`, `wholes`,
`descriptors`, `lldb`, `probe`, `records`, `seed`, `spec`, the net's own tests and the net
(`.claude/rules/verification.md`'s rows for `selfhost/**` and `selfhost/emit/**`); the new golden
cases, each annotated in its source (d2137, d2136, U1, L1, S1's second unit, `tdef`, `twin`,
`first`, `ctr`, `inert`, `forge1`, `forge2c` and the learnt-key `fk`, whose `fk-evil.h` must be
written by the case's own run since the key is the platform's); the CI's legs after the push; and
the box list of § 9.

**Lines**: `git diff --shortstat`, 11 files, **546 insertions, 113 deletions** (net +433; CE was
+348 -107 over 10, FP +158 -69 over 6 plus its 75-line new file); comments and blank lines out of
the diff, tests in: +330 -72. `emit/probe_cut.hero` 208 lines (141 by `layout`'s rule, its three
tests out), `cli/value_early.hero` 109. The two new mechanisms, the key and the match, are 30
lines of code in `probe_cut.hero` (`keyed`, `rewritten`, `carrying`, `line_from`, the match in
`cut`) and 15 in `value_beside.hero` (`keyed`, `reads_of`), by reading.
**Corrected 2026-10-19, the same hour, by count** (`awk` over each function's lines, comments,
blank lines and tests out): `probe_cut.hero` holds 91 lines of code, of which `keyed`,
`rewritten`, `carrying`, `line_from`, `key_of` and `line_at` are 56 and `cut` 24; `value_beside`'s
`keyed` and `reads_of` are 7. The figures *30* and *15* above were read, not counted, and are wrong.

## 11. Residue, said rather than hidden

1. **Defect 2146**, the probe's `-D main` word, shared by every probe; the coordinator's repair,
   after landing (§ 1).
2. **The stale kept verdict** (§ 6), the trunk's, beside the sitting; the coordinator's to file. The
   union's copies are keyed by every header's digest and do not have it; a compiled unit's probe
   does.
3. **F's copies before any unit compiles know no refused accessor** (CE's residue 3, the critic's
   `fi8`): a hard error in an asked accessor, such as a name the header lacks or a type clang
   refuses, makes the copy fail and the unit's values wait, as today.
4. **F at `bound_macro` and `pragma_ask`**: not wired; the reason (each says the unit's reading of a
   name cannot be trusted) is reasoned and **unrun**.
5. **A copy that cannot compile pays again warm** (FP's cost note, its § 12): a broken copy keeps
   no verdict, so a unit refused inside its own header pays the copy's `-E` and failing compile on
   every build; FP names the skip (where clang's own message places the error in a header file),
   **unbuilt** here.
6. **The key is not a secret against a header written for this very program** (§ 1, § 5): it
   only keeps an unrelated header from silencing the probe; the match is what holds.
7. **Measured on Apple clang 21 alone**: `-E` printing a keyed mark verbatim, the `_Pragma` case,
   `-w`; the box's list is § 9.

## 12. Verdict, condition, prediction

**Adopt the union, built** (`heroes-u`, `sources 40ad11812c8aa370`, `ce-work/union.diff`). It
answers wherever either prototype answers (§ 4), tells no false value on any forged mark the
critic or I wrote (§ 4, § 5), builds every program s2 builds, moves no byte a build compiles (§ 8),
pays what `heroes-g` paid (§ 8), and passes every gate the sitting named (§ 10).

**Condition**: I turn to object if any program shows the union's copy telling an asked constant
otherwise than the probe of the same program's compiled unit tells it (defect 2146's word aside,
which both carry); if the box's `fk2` copy answers where § 9 says it must be silent, or any
`forge*` or `fk*` program tells `K_SZ` there; or if `run`, `emission` or `determinism` moves
anything but new cases.

**Prediction**: on the Windows box, under the union, `fk2`'s copy keeps no `verdict.txt` while its
build tells `narrow` alone at exit 1, and every row of § 9's item 3 reads as this Mac's column.
Instrument: the coordinator's run of § 9 on the box. Scored at that run, before the synthesis
adopts the union or, if the box is off, at the CI's Windows leg after the landing push for the
rows that become golden cases.

## 13. State left

My worktree `.claude/worktrees/225-compiler-engineer` holds the union uncommitted (11 files; its
index carries the two new files as intent-to-add entries, from a `git add -N` of the first step);
`ce-work/union.diff` is its `git diff`, byte for byte (`cmp`), and applies cleanly to `7b97b9438`
(`git apply --cached --check` against a scratch index); `ce-work/g.diff` is `heroes-g`'s. Built
beside it: `heroes-u` and the model `heroes-nob` (the match left out, its source restored). Every
program and log of this step is under `ce-work/` (`union-src/`, `table-work/`, `table-u.tsv`,
`fkrun/`, `fk2run/`, `r2109/`, `stale/`, `marks/`, `count/u-*`, `same/heroes-u/`, `gate-u/`).
Nothing committed; nothing written outside the repository's root.

**Third step complete, 2026-10-19.**
