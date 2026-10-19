# Panel 225, the coordinator's measurements

Written 2026-10-19 by the coordinator, in the lane `lane-witnessed-seed` and on the Windows box,
for the synthesis and the third step's brief. Every block below is a command and what it printed;
a program's directory is the session's scratchpad unless said, its build in a fresh directory.

## 1. `clang -E` and a pragma's tokens, this Mac and the box

`marks.c` (15 lines), written for this: `#define cut 1`, `#define keep 2`, `#define heroes_probe 3`,
`#define hero_probe_cut on_by_macro`, then `#pragma heroes_probe cut`, `#pragma hero_probe_cut on`,
`#pragma hero_probe_cut_0123abcd`, `int a = 1;`, `_Pragma("heroes_probe keep")`, `int b = 2;`,
`#define MARK _Pragma("hero_probe_keep_0123abcd")`, `MARK`, `int c = (MARK 3);`,
`#pragma hero_probe_keep_0123abcd`, `int d = 4;`. Copied to the box as `/c/w/p225g/marks.c`.

`clang -E -P marks.c` (Apple clang 21.0.0, this Mac's default target), every line it printed but
`int a = 1;`, `int b = 2;` and `int d = 4;`:

```
#pragma heroes_probe cut
#pragma hero_probe_cut on
#pragma hero_probe_cut_0123abcd
#pragma heroes_probe keep
#pragma hero_probe_keep_0123abcd
int c = (
#pragma hero_probe_keep_0123abcd
              3);
#pragma hero_probe_keep_0123abcd
```

`clang -E -P -fms-extensions marks.c` on this Mac, and `clang -E -P marks.c` on the box (clang
23.1.1, `Target: x86_64-pc-windows-msvc`), the same lines, the same on both:

```
#pragma 3 1
#pragma on_by_macro on
#pragma hero_probe_cut_0123abcd
#pragma 3 2
#pragma hero_probe_keep_0123abcd
int c = (
#pragma hero_probe_keep_0123abcd
              3);
#pragma hero_probe_keep_0123abcd
```

On the box, `-fno-ms-extensions` and `--target=x86_64-pc-linux-gnu` each printed the first block.

## 2. The probe's `-D main` word (defect 2146), the lane's stage 2 (`sources 2ee0ef0471cd67e7`)

`__BASE_FILE__` first: a header `#define K_LEN ((long long)sizeof(__BASE_FILE__))`, bound and
printed by `name.hero`: `heroes build name.hero -o name`, exit 0, `./name` prints `10`, the nine
bytes of `name.hero` and its zero. Then `m.h`, `#ifdef main` / `#define K_M ((-2147483647 - 1) /
-1)` / `#else` / `#define K_M 7` / `#endif`, bound by `m.hero`:

```
$ heroes build m.hero -o m          # exit 1
error[ffi_constant_type]: `K_M` in `m.h` cannot be declared `i64`: clang refuses the header's value at line 3: its expression holds an operation C leaves undefined ...
$ clang @build/words/words-1cd570025da05fb7.rsp -E build/tu-0b5418a770e4bfcf/m.c -o unit.i
$ grep -n __builtin_constant_p unit.i
1333:    _Static_assert(__builtin_constant_p(7), "heroes-ffi-const K_M");
```

And its mirror, `n.h` with `#ifndef main`: `heroes build n.hero -o n`, exit 0; `./n` prints
`6125380652`; the unit under its own words asserts `__builtin_constant_p(((-2147483647 - 1) / -1))`.

## 3. A kept verdict across a header's edit (defect 2147), the lane's stage 2

```
$ printf '... #define K_DIV 7\n' > st.h; heroes build st.hero -o st; ./st     # exit 0, 7
$ printf '... #define K_DIV ((-2147483647 - 1) / -1)\n' > st.h
$ heroes build st.hero -o st                                                  # exit 0
$ ./st; ./st; ./st                                      # 6104441900, 6102508604, 6097871916
$ rm -rf build st; heroes build st.hero -o st                                 # exit 1
error[ffi_constant_type]: `K_DIV` in `st.h` cannot be declared `i64`: clang refuses the header's value at line 2: ...
```

## 4. The union on the box

The box-only commit `b6ef1ec1ce287d1baa16832c23f8e499e96eb685`: the lane's HEAD `4f81c0aeb` with
`.claude/worktrees/225-compiler-engineer/ce-work/union.diff` applied in a scratch index
(`git apply --cached --check`, then `git write-tree` and `git commit-tree`), pushed to the box's
repository as `box225` and nowhere else. On the box, `git archive` of it into `/c/w/b225`, then
`clang -I runtime seed/heroes.c runtime/runtime.c -Wl,/STACK:67108864 -o heroes-s1.exe` (exit 0)
and `./heroes-s1.exe build selfhost/main.hero -o heroes.exe` (exit 0), 06:15:58 to 06:18:08 the
box's time; `heroes.exe --version`: `sources 2fe5f3ac647a2fae`, the key of the lane's compiler with
the union built on this Mac.

The compiler-engineer's § 9 programs (`ce-work/union-src/`, `r2109/`, `stale/`), each copied fresh
and built by `heroes.exe build [flags] <p>.hero -o out`, its binary run under `timeout 10`; *told*
is each `error[code]` with the first name its message quotes in backticks, so a message opening
otherwise (`ffi_union_field`) is missed, and `u1` is re-read beneath:

```
=== Mon Oct 19 06:19:18 WEDT 2026 heroes 0.4.0 sources 2fe5f3ac647a2fae
union-src/repro/d2137  | exit 1 | told: error[ffi_parameter_type] x;error[ffi_constant_type] K_DIV; | ran:  | value dirs 1, verdicts 1
union-src/repro/d2137-next  | exit 1 | told: error[ffi_constant_type] K_DIV; | ran:  | value dirs 1, verdicts 1
union-src/repro/d2136  | exit 1 | told: error[ffi_parameter_type] out;error[ffi_constant_type] K_DIV; | ran:  | value dirs 1, verdicts 1
union-src/repro/d2136-next  | exit 1 | told: error[ffi_constant_type] K_DIV; | ran:  | value dirs 1, verdicts 1
union-src/u1/u1  | exit 1 | told: error[ffi_constant_type] K_DIV; | ran:  | value dirs 1, verdicts 1
union-src/tdef/tdef  | exit 1 | told: error[ffi_tag_is_a_union] un.h;error[ffi_constant_type] K_DIV; | ran:  | value dirs 1, verdicts 1
union-src/twin/twin  | exit 1 | told: error[ffi_unknown_name] tw.h;error[ffi_constant_type] K_DIV; | ran:  | value dirs 1, verdicts 1
union-src/first/first  | exit 1 | told: error[ffi_header_refused] hbad.h;error[ffi_constant_type] K_DIV; | ran:  | value dirs 2, verdicts 1
union-src/mixed/mixed  | exit 1 | told: error[ffi_constant_type] K_SHIFT;error[ffi_parameter_type] x;error[ffi_constant_type] K_DIV;error[ffi_constant_type] K_FLT; | ran:  | value dirs 1, verdicts 1
union-src/ctr/ctr  | exit 1 | told: error[ffi_parameter_type] x;error[ffi_constant_type] K_DIV; | ran:  | value dirs 1, verdicts 1
union-src/ctr/ctr-next  | exit 1 | told: error[ffi_constant_type] K_DIV; | ran:  | value dirs 1, verdicts 1
union-src/ctr/ctr-only  | exit 0 | told:  | ran: 3  | value dirs 1, verdicts 1
union-src/inert/inert  | exit 1 | told: error[ffi_constant_type] K_BAD; | ran:  | value dirs 1, verdicts 1
union-src/inert/inert-only  | exit 1 | told: error[ffi_constant_type] K_CTR; | ran:  | value dirs 1, verdicts 1
union-src/inert/inert-fixed  | exit 0 | told:  | ran: 1048576  | value dirs 1, verdicts 1
union-src/cnt/cnt  | exit 1 | told: error[ffi_constant_type] K_BAD; | ran:  | value dirs 1, verdicts 1
union-src/forge1/forge1  | exit 0 | told:  | ran: 5  | value dirs 1, verdicts 1
union-src/forge2c/forge2  | exit 1 | told: error[ffi_parameter_type] x; | ran:  | value dirs 1, verdicts 1
union-src/forge2c/forge2-next  | exit 0 | told:  | ran: -1073741821  | value dirs 1, verdicts 1
union-src/forge3c/forge2  | exit 1 | told: error[ffi_parameter_type] x; | ran:  | value dirs 1, verdicts 1
union-src/forge3c/forge2-next  | exit 0 | told:  | ran: -1073741821  | value dirs 1, verdicts 1
union-src/forge2p/forge2  | exit 1 | told: error[ffi_parameter_type] x; | ran:  | value dirs 1, verdicts 1
union-src/forge2p/forge2-next  | exit 0 | told:  | ran: -1073741821  | value dirs 1, verdicts 1
union-src/line/asw  | exit 1 | told: error[ffi_unknown_name] ln.h;error[ffi_constant_type] K_LINE; | ran:  | value dirs 1, verdicts 1
union-src/optoff/opt  | exit 1 | told: error[ffi_constant_type] K_OFF; | ran:  | value dirs 1, verdicts 1
union-src/optoff/opt -O2 | exit 1 | told: error[ffi_constant_type] K_OFF;error[ffi_constant_type] K_BCP; | ran:  | value dirs 1, verdicts 1
r2109/case  | exit 1 | told: error[ffi_parameter_type] x; | ran:  | value dirs 1, verdicts 1
r2109/case -O2 | exit 1 | told: error[ffi_parameter_type] x; | ran:  | value dirs 1, verdicts 1
fk (cut, key 49f5ec4f10a854bd) | first exit 1 | second exit 1 | told: error[ffi_parameter_type] x; | value dirs 1, verdicts 0 | keyed lines in the copy's own text 4
  by hand: -E exit 0, lines carrying the key 5, lines reading '#pragma 3' 0
fk2 (keep, key 49f5ec4f10a854bd) | first exit 1 | second exit 1 | told: error[ffi_parameter_type] x; | value dirs 1, verdicts 0 | keyed lines in the copy's own text 4
  by hand: -E exit 0, lines carrying the key 2, lines reading '#pragma 3' 2
=== stale: a header edited between two builds (defect 2147, the trunk's)
cold, K_DIV 7: exit 0 prints 7
warm, K_DIV INT_MIN / -1: exit 0 prints 2242234981312
```

`fk` and `fk2`: a first build with `fk-evil.h` holding its comment alone, the key read from the
copy's first `hero_probe_cut_` line, then `fk-evil.h` given `#pragma hero_probe_cut_<key>` (`fk`)
or `#define hero_probe_keep_<key> 3` (`fk2`), `build` removed, built again; *by hand* is the copy
preprocessed under its own words and `-D main=hero_probe_hidden_main`, the lines carrying the key
counted.

`u1` again, and a copy's text as clang wrote it there:

```
u1 exit 1
error[ffi_union_field]: this construction of `UT` names `i` and `f`, which are the same bytes: `ffi-a-construc
error[ffi_constant_type]: `K_DIV` in `fixedbugs-2128-values.h` cannot be declared `i64`: clang refuses the hea
copy's probe text build/value-6e9a9cd9779b0392/probe.i: lines 1402, lines ending CR LF 1402
```

`cd /c/w/b225 && time ./heroes.exe test selfhost/main.hero`: exit 0, `1737 tests, all passed`,
`real 3m8.204s`.

## 5. The union in the lane

`git apply` of the same diff over `4f81c0aeb` in the lane's tree;
`/usr/bin/time -p ./heroes build selfhost/main.hero -o heroes-next`: exit 0, `real 39.28 user 37.08
sys 1.61`, `sources 2fe5f3ac647a2fae`. The landing's new cases, judged by the gate's own command,
`./heroes run tests/harness/main.hero -- ./heroes <form> <filter>`: `unsupported fixedbugs-2136` 3
passed, `-2137` 6, `-2143` 2, `-2145` 2, `run fixedbugs-2137` 1, and `annotations` over the same
four filters 3, 6, 2 and 2, each 0 failed and exit 0.
