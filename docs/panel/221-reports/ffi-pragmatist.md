# Panel 221, ffi-pragmatist: which record constants a route refuses, over the real headers

Written 2026-10-17 from 12:20, as I go. Worktree `.claude/worktrees/221-ffi-pragmatist`, detached at
`bc978b09d`, the lane's uncommitted C2 files copied in by `cp` (`git -C lane-land215 status --short`
named 16 paths); `rm -rf build`; `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed`
(real 8.89 s); `./heroes-seed build selfhost/main.hero -o heroes` (real 103.45, user 77.62, sys
17.25; the Mac held awake by powerd's assertion, `pmset -g assertions`). Every script and output is
under `<scratchpad>/221-ffi/`, `<scratchpad>` being
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad`.

## 0. Where I stand

design.md §4.19 (panel 038): *the header must hold a **value**, not an object*; `stdout` and
`errno` are refused because a zero-argument accessor over an object returns what it holds at the
time of the call, §4.2's mutable global through the back door. §1.11: any C library must stay
bindable; §1.12: the boundary complete and defended. Panel 063 condition 1 left the record twin of
that rule unasked, on a measured reason (`__builtin_constant_p` is 0 for every struct).

(sections follow as they are measured)

## 1. The routes, defined so each is a question C or clang answers

| route | what is written | who answers |
|---|---|---|
| **A** (today) | `struct pt hero_constant_value = NAME; return hero_constant_value;` | nobody asks constancy |
| **S** | `static const __typeof__(V) s = V;` at file scope | C's rule for a static initialiser (C11 6.7.9p4), with clang's GNU folding |
| **Sl** | the accessor's own local made `static`: `static struct pt hero_constant_value = NAME;` | the same rule, at block scope, in the program's own unit, at the constant's `#line` |
| **F** | `_Static_assert(__builtin_constant_p(((struct { T v; }){ NAME }).v.<field>), "heroes-ffi-const NAME")` per named field, a union's members joined by `\|\|` | the scalar rule (§4.19's builtin), asked field by field. **Correction to my own instrument**: the wrapper compound literal at file scope is itself a static object, so F inherits S's rule and adds the builtin on top |
| **K** | the brief's route: C2's dump of the accessor, refuse a leaf whose node is a call (not a `__builtin_`), a reference to a `VarDecl` that is not const or is volatile (outside `&`), a statement expression, an assignment or `++`/`--` | structure read by the compiler |

## 2. The shapes (`shapes/sh.h`, `shapes/routes.py`, `shapes/mac21*.tsv`; Apple clang 21.0.0)

`python3 -I routes.py clang sh.h shapes.tsv` and `shapes2.tsv`, 58 shapes, under the build's words
(`-std=gnu11 -Wall`, no `-pedantic`). Every shape builds today (column A). Grouped:

| shapes | S | Sl | F | K |
|---|---|---|---|---|
| 26 constants: literals, enumerators, `sizeof`, `offsetof`, compound literals (raylib's `CLITERAL`), designated, `{}`, `{0}`, nested, flat, `__builtin_inff()`, `__builtin_nanf("")`, `__builtin_bswap32`, `__builtin_expect`, a union by either member, Darwin's mutex shape `{0x32AAABA7, {0}}`, `{ "machine", 3 }`, `(void*)0`, a const object's member, `static const` int, const array element, a `static const struct` (`PT_OBJ`'s shape) | kept | kept | kept | kept |
| a call in a dead branch: `1 ? 3 : g_next()`, `sizeof(g_next())`, `__builtin_choose_expr(1, 3, g_next())`, `_Generic(0, int: 3, default: g_next())`, `__builtin_constant_p(g_next()) ? 1 : 2` (5, all C constants) | kept | kept | kept | **REFUSED** (false) |
| an address: `{ &k_obj, 2 }`, `{ &g_mut, 2 }` (address constants in C) | kept | kept | **REFUSED** | kept, **REFUSED** for `&g_mut` without the `&` rule |
| `{ ({ 3; }), 2 }` (a statement expression C folds) | REFUSED (file scope) | kept | REFUSED | REFUSED |
| two reads differ or read a writable object: `{ g_next(), 2 }` (the defect), the same in a compound literal, nested, designated, in a union, `{ g_mut, 2 }`, `pt_mut`, `next_pt()`, `{ k_vol, 2 }`, `{ (g_next(), 3), 2 }` | REFUSED | REFUSED | REFUSED | REFUSED |
| `{ (int32_t)__builtin_readcyclecounter(), 2 }` (two reads differ: the built program printed `-1327073515` then `-1327073473`) | REFUSED | REFUSED | REFUSED | **kept** (a builtin) |
| `{ *&g_mut, 2 }` | REFUSED | REFUSED | REFUSED | **kept** (the `&` rule hides it) |
| `{0x32AAABA7, { (char)g_next() }}` (a call in a partially filled array) | REFUSED | REFUSED | REFUSED | **kept by a reader of `inner` alone** (below) |
| a call whose value is fixed: `{ g_ok(), 2 }`, `make_pt()` (094's `PT_CALL`) | REFUSED | REFUSED | REFUSED | REFUSED |
| an object with no initialiser the header shows: `extern const struct pt pt_ext`, `{ k_ext, 2 }` | REFUSED | REFUSED | REFUSED | kept |
| panel 218's undefined leaves `{ INT_MIN / -1, 2 }`, `{ 1 << 31, 2 }`, `{ INT_MAX + 1, 2 }`, `{ (int32_t)1e20, 3 }` | kept | kept | kept | kept |
| `{ 1 / 0, 2 }` | REFUSED | REFUSED | REFUSED | kept |

**The scalar twins under today's rule** (`shapes/scalars.c`, `_Static_assert(__builtin_constant_p(e))`
as `emit/extern_assert.hero` writes it): refused `g_ok()`, `k_ext`, `&k_obj`, `&g_mut`, `k_vol`;
kept `k_obj`, `k_arr[1]`, the dead-branch call, `sizeof(g_next())`, `__builtin_inff()`, `({ 3; })`,
`"abc"`, `(void*)0`, `pt_origin.x`. So **S and Sl agree with the scalar rule on every shape but an
address** (they keep it; C calls it a constant, the builtin does not), **F agrees on every shape**,
and **K disagrees on nine**: five false refusals, four silent keeps of which two read different
values on two reads.

**Found beside the sitting, and it reaches C2 as built**: clang's JSON dump writes the elements of an
array initialiser that has an *array filler* under the key `array_filler`, not `inner` (Apple clang
21.0.0 and Homebrew clang 22.1.8 alike; `CallExpr reached through: /inner/inner/inner/inner/inner/array_filler/inner`).
`selfhost/cli/clang_tree.hero` reads `inner` alone (its head says so), so C2's leaf walk never sees
an element of a partially initialised array: `{0x32AAABA7, {0}}`'s `0` is Darwin's
`PTHREAD_MUTEX_INITIALIZER` shape, and an undefined leaf there, `{ 1, { 1 << 31 } }`, is the class
panel 218 closed. Unrun through the lane's compiler as a Heroes program yet (§ 4 below).

## 3. Through the compiler (my build, the lane's C2 in it)

`<scratchpad>/221-ffi/hero/t_<NAME>.hero`, each binding one shape as `record Pt tag pt` and reading it
twice, `heroes build` then the binary under `timeout 10`:

| shape | today (lane C2) | Sl prototype |
|---|---|---|
| `R_CNT` `{ g_next(), 2 }` | exit 0, prints `1` `2` | exit 1 |
| `R_CALL_OK` `{ g_ok(), 2 }` | exit 0, `7` `7` | exit 1 |
| `R_CALL_PT` `make_pt()` | exit 0, `5` `5` | exit 1 |
| `R_MUT`, `R_MUTOBJ` | exit 0 | exit 1 |
| `R_CYCLES` | exit 0, two values | exit 1 |
| `R_OBJ`, `R_KOBJ`, `R_CLIT`, `R_LIT`, `R_COND_DEAD` | exit 0 | exit 0, same output |
| `R_EXTOBJ` (no definition anywhere) | exit 1, the probe *did not link* | exit 1 |

And a real object: `record In6 tag in6_addr partial` / `s6_addr: u8[16]` binding Darwin's
`in6addr_loopback` (an `extern const struct in6_addr`) and `IN6ADDR_LOOPBACK_INIT` (its brace
list) both build today and print `1` `1`.

**The Sl prototype is one word** in `selfhost/emit/record_constant.hero`'s `accessor` (and its test's
needle): `"    static " + spelled + ...`. First tried as `static const`: the `const` shows in clang's
words (`field designator 'z' does not refer to any field in type 'const struct pt'`), so
`unsupported/fixedbugs-094-a-field-...` moved; C11 6.7.9p4 binds static storage duration, not
`const`, so `static` alone asks the same question and moves no word. Built `heroes-sl` from it
(45.8 s real). The corpus's record constants (14 in `run`, 8 in `unsupported`, all under
`tests/golden/`; no example binds one: their constants are 76 `str`, 72 `i64`, 5 `f64`, 3 `u8`,
1 `i32`, 1 `[str]`):

- `heroes-sl run tests/harness/main.hero -- ./heroes-sl run fixedbugs-094`: **7 passed, 1 failed**,
  the one being `PT_CALL` (`#define PT_CALL make_pt()`), refused;
- `... unsupported fixedbugs-094`: **6 passed, 0 failed**, every word unchanged;
- `... unsupported fixedbugs-2109`: 0 and 1 under `heroes-sl` **and under `heroes-base`** (the same
  sources with the edit reverted): the only differing line is clang's include stack,
  `In file included from build/tu-38b0a31177f9651b/...` expected, `build/tu-65ba855c11db5785/...`
  (base) and `build/tu-4eb82ac37b75f586/...` (Sl). **Found beside**: the lane's uncommitted golden
  pins a build directory's key, which moves with the compiler binary, so it fails under any binary
  but the lane's own (for the coordinator; not filed by me).

**What Sl tells today is the wrong class**: `error[ffi_constant_type]: \`R_CNT\` in \`sh.h\` cannot
be declared \`Pt\`: clang refuses the header's value at line 55: initializer element is not a
compile-time constant`, the header line quoted (good), then the note *declare it at the record whose
value the header spells*, which is false here: the type is right and the value is no constant. The
true class exists, `ffi_not_constant`; its note's helper, `return NAME;`, does not compile for a
brace list. So Sl owes a reading (clang's *initializer element is not a compile-time constant* at a
record constant's declaration told as `ffi_not_constant`) and a record spelling of the helper
(`static inline struct pt hero_NAME(void) { struct pt v = NAME; return v; }`). No new class, no
surface, no spec token as far as I can see (spec § 13 is grepped below).

## 4. Found beside: C2 does not see an element of a partially filled array (measured)

`<scratchpad>/221-ffi/hero/af.h`, `struct arr { int32_t a[4]; int32_t n; }` bound as `record Arr tag arr`
(`a: i32[4]`, `n: i32`), built by `heroes-base` (the lane's C2, unchanged):

| macro | today |
|---|---|
| `{ { 1 << 31 }, 2 }` | **exit 0**, prints `-2147483648` `2` |
| `{ { (-2147483647 - 1) / -1 }, 2 }` | **exit 0**, prints `-2147483648` `2` |
| `{ { 1 << 31, 0, 0, 0 }, 2 }` | exit 1, `ffi_constant_type`, *an operation C leaves undefined* |

The cause is the dump's shape (§ 2): an `InitListExpr` with an array filler writes its elements under
`array_filler` (`[filler, element, ...]`), and `selfhost/cli/clang_tree.hero` keeps `inner` alone.
Panel 218's own `pop.py` `leaves()` read `inner` alone too, so its record counts carried the same
blind spot. By `.claude/rules/verification.md` § Bounded discovery this is `blocking` (a wrong
program accepted, the class panel 218 closed); for the coordinator, not filed by me. Repair, unbuilt:
the reader treats every object of an `array_filler` array as a child, the filler first (an
`ImplicitValueInitExpr` the walk already skips).

## 5. The real headers: every record constant a `constant` can name, on three clangs

`pop221.py` (usage in its docstring), over panel 216's and 218's library lists unchanged
(`spec-mac.txt`, `spec-d22.txt`, `spec-u18.txt`), in the context a unit reads a group's header in, at
`-O0` (every question here is the front end's: the battery at `-O2` gives the same verdict on all 58
shapes, `shapes/routes-lvl.py`). It lists **every object-like macro whose value is an expression of
record or union type** (`__builtin_classify_type` 12 or 13, so a compound literal, a call and an
object alike, not only a brace), **every macro that is one brace list** (asked leaf by leaf:
`static const __auto_type s = (leaf);` at file scope and as a static local, and K over each leaf),
and **every file-scope `VarDecl` of record type** (const or not; a `constant` may name an object).
Unique by (name, kind), the runtime's own `hero_*` and my control left out (`ana.py`). Containers
`ffi221-d22` (`heroes-linux-arm64:latest`, Debian clang 22.1.8, `libsdl3-dev` 3.2.10) and
`ffi221-u18` (`ubuntu:24.04`, Ubuntu clang 18.1.3), panel 218's `install.sh` libraries.

| | Apple clang 21.0.0 | Debian clang 22.1.8 | Ubuntu clang 18.1.3 |
|---|---|---|---|
| contexts that compiled | 111 of 116 (as panel 218) | 136 of 137 (raylib not packaged) | 134 of 137 (+ mpdecimal, SDL3) |
| **brace lists** (`PTHREAD_*_INITIALIZER`, `IN6ADDR_*_INIT`, glib's, lzma's, openssl's, ffmpeg's `AV_CHANNEL_LAYOUT_*`, gpg-error's) | **82: S, Sl, K keep all** | **58: all kept** | **55: all kept** |
| **compound literals** (raylib's 26 `CLITERAL(Color)`, `AV_TIME_BASE_Q`, `MACH_MSG_HEADER_EMPTY`; systemd's `SD_ID128_NULL`, `SD_ID128_ALLF`) | **28: all kept** | **3: all kept** | **3: all kept** |
| a macro naming a writable object or a call (`xmlLastError` = `(*(__xmlLastError()))`, `xmlDefaultSAXHandler`, libffi's `ffi_type_uchar` = `ffi_type_uint8`, sodium's `randombytes_salsa20_implementation`) | 15: S, Sl refuse all; K refuses all (2 lost by my batch reader, each shown alone) | 14: the same (3 lost, the same) | 14 |
| a writable extern object of record type (oniguruma's 29 `OnigEncoding*`, libffi's `ffi_type_*`, glib's `g_idle_funcs` and six more, Darwin's `averunnable`) | 53: refused by all | 68: refused by all | 56: refused by all |
| **an extern const object** (Darwin's 6 `in6addr_*`; glibc's `in6addr_any`, `in6addr_loopback`; gnutls's 27 `gnutls_ffdhe_*` and `gnutls_srp_*` group parameters) | **6: S, Sl refuse; K keeps** | **29: S, Sl refuse; K keeps** | **2: the same** |
| a brace list or compound literal holding an address leaf (where F parts from S) | 0 | 0 | 0 |
| SDL3 | 0 record constants | 0 | (no package) |

**Not one brace list or compound literal of the real headers is refused by S or Sl, on any of the
three clangs.** raylib's 26 `CLITERAL(Color)`, panel 063's worry, are kept by every route. What S and
Sl refuse is §4.19's *object*, in every row: a writable object (the defect's class, two reads may
differ), a thread-local accessor's call (`xmlLastError`: two reads of the last error), and an
extern const object whose value no header shows. Every one of them builds today. The last row is
the only place S/Sl and K part on a real header, and there the scalar rule already sides with S/Sl:
`_Static_assert(__builtin_constant_p(k_ext))` refuses `extern const int32_t k_ext` today
(`shapes/scalars.c`). Each such header carries its value spelling beside it where one exists
(`IN6ADDR_LOOPBACK_INIT` next to `in6addr_loopback`, kept by all).

The battery was run on all three clangs (`shapes/d22*.tsv`, `shapes/u18*.tsv`): **identical
verdicts to Apple clang 21 on all 58 shapes, all five columns**, and clang's words for Sl's refusal
the same 19 times on each, `initializer element is not a compile-time constant`.

## 6. The Windows box: clang 23.1.1, `x86_64-pc-windows-msvc`, SDK 10.0.26100.0

`ssh win`; everything written there under `~/ffi221`, removed at the end. No Python on the box, so a
relay: `win/send/stage1.sh` (per label: the macros, their expansions, the textual dump's file-scope
`VarDecl` lines) and `win/stage2.pl` (Perl 5.42: the record classifier, A, S and Sl per value, brace
lists leaf by leaf; K not ported, so **K on Windows is unrun**), only the 189 KB table copied back.
Panel 218's 46 real labels (22 C-runtime headers, 23 SDK labels from `windows.h` to `d3d11.h`,
`dxgi.h`, `xinput.h`, `hero_os.h`); all 46 contexts compiled.

| clang 23.1.1, the box | count (unique by name and kind) | S | Sl |
|---|---|---|---|
| brace lists (`SRWLOCK_INIT`-shaped `{0}`s, `IN6ADDR_*_INIT`, ...) | **120** | kept, all | kept, all |
| a macro naming a const GUID with an initialiser (`SID_SUrlHistory`) | 1 | kept | kept |
| const objects with an initialiser (`CLSID_CUrlHistory`, `CLSID_CUrlHistoryBoth`) | 2 | kept | kept |
| **extern const objects**: 803 `IID`, 795 `GUID`, 167 `CLSID`, 19 UCRT `_float_const` (`_Inf_C`, ...), 17 `CATID`, ... | **1,834** | REFUSED | REFUSED |
| **macros naming one** (`IID_NULL` = `GUID_NULL`, `DiskClassGuid` = `GUID_DEVINTERFACE_DISK`, `IID_ID3DBlob` = `IID_ID3D10Blob`, ...) | **86** | REFUSED | REFUSED |
| writable record objects | 0 | | |

clang's words for every Sl refusal on the box: `initializer element is not a compile-time constant`
(18,513 label-summed rows), as on the other three clangs.

**This is the real price of S and Sl, and it is Windows'.** Every GUID the SDK declares binds as a
`constant` on the trunk today (column A builds), and S/Sl refuse all 1,920. They are §4.19's
*objects*: `EXTERN_C const IID IID_ID3D11Device;` holds no value, `uuid.lib` does, exactly as
`extern const int` holds none and is refused today. With the program's own `#define INITGUID`
(`win/labels-initguid.txt`, `-DINITGUID` over `windows.h`, `d3d11.h`, `dxgi.h`, `shlobj.h`):
**91 GUIDs become definitions with an initialiser and S/Sl keep them**, 16 macros naming one too;
**748 stay objects** (706 MIDL-declared `IID`s, defined in `uuid.lib` whatever `INITGUID` says) and
stay refused. For those the way is the note's helper: one line of the program's own C per GUID.
Whether the lane's R1 probe already refuses them (it links with the runtime alone, and `uuid.lib`
holds them) is **unrun on the box**; on this Mac an extern object nothing defines is refused at the
probe's link today (`R_EXTOBJ`, § 3), so the GUIDs would meet that step, an inference.

## 7. What each route costs, and what it tells

| route | code | where it runs | the message |
|---|---|---|---|
| **Sl** | **measured: 32 lines added, 2 changed, 2 files** (`git diff --stat`): the word `static` in `record_constant.hero`'s accessor and its test's needle; in `ffi_value.hero`'s `refused_value`, clang's `initializer element is not a compile-time constant` at a record constant's declaration told `ffi_not_constant` by a new `not_constant` | the program's own compile, every build, warm or cold; no dump, no probe, no link; the same declaration whose value the program returns | `error[ffi_not_constant]: \`sh.h\` has \`R_CNT\`, but not as a constant — clang cannot compute its value while compiling`, the header's `#define` quoted, a note naming §4.4 and §4.2 and a record helper, `static inline struct pt hero_R_CNT(void) { struct pt v = R_CNT; return v; }`, **which binds and runs** (`hero/t_helper.hero`: `1` `2`, honestly a function) |
| S | as Sl plus a second declaration per record constant at file scope | the program's compile | as Sl; refuses `({ 3; })` that the scalar rule keeps |
| F | a per-field assertion writer over the record's type tree (nested records, unions joined by `\|\|`, `partial`, arrays), in `extern_assert.hero`: unbuilt | the program's compile | rides `CONSTANCY` and `ffi_declared.not_constant`, whose helper `return NAME;` is false for a brace list, so the same note is owed |
| K | kind checks over C2's walk in `value_init.hero` (unbuilt), plus the reader repaired for `array_filler` (§ 4), plus a list of which `__builtin_`s are constant | C2's dump step, cold only, per unit holding a name or a brace list | its own words per kind |

**Prototype gates, `heroes-sl2` (Sl and its reading)**: the compiler's own tests **1,706, all passed**;
`check ffi` 26 and 0; `unsupported ffi` 35 and 0; `unsupported fixedbugs-094` 6 and 0, every word
unchanged; `run fixedbugs-2109` 1 and 0; **`run fixedbugs-094` 7 and 1, the one being `PT_CALL`**,
now `error[ffi_not_constant]: \`fixedbugs-094-values.h\` has \`PT_CALL\`, but not as a constant`.
`warnings`: each of the eight `run/fixedbugs-094-*` built by `heroes-sl` shows 0 clang warnings.
raylib's 26 `CLITERAL(Color)` bound in one program (`hero/rl.hero`, `package "raylib"`): built and
run by `heroes-base` and `heroes-sl`, 0 warnings, **byte-identical output** (`cmp`). Darwin's
`PTHREAD_MUTEX_INITIALIZER` and `PTHREAD_RECURSIVE_MUTEX_INITIALIZER` (`hero/mtx.hero`, `record
pthread_mutex_t partial`): built, locked and unlocked (the recursive twice) by both, `0` six times.
Golden churn: 9 blessed emissions under `tests/emission/` (16 lines gain `static`), 0 `emit`
goldens, one `run` case's constant (`PT_CALL`).

**One residue of the reading, unbuilt**: `{ 1 / 0, 2 }` draws the same clang words under Sl, and its
note's helper would carry an undefined division into C. Today's lane tells it, through C2's probe,
*an operation C leaves undefined ... correct the value there*. The reading must keep those words for
it (clang also prints `-Wdivision-by-zero` at that line, which the scalar region makes an error).

**Three comments become false and are corrected with the route**: `record_constant.hero` 44-46 (*the
constancy question stays unasked of a struct*), `extern_assert.hero` 203-206, `assert_spelling.hero`
134-142 (*the hole that leaves is written down*). `asks_for_a_value` itself stays false for a record:
nobody asks the builtin of a struct, panel 063's measured reason still stands. No sentence of
design.md or the spec claims the hole (`grep -n -i` for *struct constant*, *record constant*,
*constancy question*, `CLITERAL`): spec § 13's *the header holds the value* already says it, so no
spec token is owed as far as this seat can tell (a spec-warden's pricing is unrun).

## 8. Verdicts, route by route

| route | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| **Sl**: the accessor's own local made `static`, its refusal told `ffi_not_constant` with a record helper | **approve** | §4.19 (*a value, not an object*), §1.12, §1.11; C11 6.7.9p4 | 32 lines added and 2 changed in 2 files (measured); 9 emissions, 16 lines; one `run` golden constant (`PT_CALL`); every build, no dump, no probe, no link. **The price is Windows'**: 1,920 GUID constants that bind today are refused (748 even with `INITGUID`), plus 6, 29 and 2 extern const objects on the Mac, Debian and Ubuntu | below | below |
| **S**: `static const T s = V;` as a separate probe line | object | §4.19 | as Sl plus a second declaration per constant | as Sl, but `({ 3; })` refused where the scalar rule keeps it | none: Sl is S in the one declaration whose value the program reads |
| **F**: `__builtin_constant_p` per named field | object | §1.12 (no gain), CLAUDE.md § Precedence (cost) | an assertion writer over the type tree, unbuilt; the same note owed | refuses address constants (0 in the real headers) and nothing else Sl keeps | a real header holding a record constant whose address leaf Sl keeps and that must not be |
| **K**: refuse a leaf by its node kind in C2's dump | **object** | §1.12; §1.1 and panel 218's veto of an evaluator of the compiler's own | unbuilt kind checks, the reader's `array_filler` repair, and a list of constant builtins | keeps `__builtin_readcyclecounter()` (two reads printed `-1327073515`, `-1327073473`), `{ *&g_mut, 2 }` and, until the reader is repaired, a call in a partially filled array; refuses five C constants (a call in a dead branch); keeps extern const objects the scalar rule refuses | K that asks clang the evaluated question instead of reading structure, which is Sl |
| `__builtin_constant_p` of the whole struct (panel 063's naive repair) | **veto** | §1.11 | refuses all 82, 58, 55 and 120 brace lists and raylib's 26 | | none |
| any route keeping extern const record objects while scalars refuse them | object | §4.19 (one rule for a value) | | | a sitting on §4.19 that rules extern const objects are values, for scalars and records together |

**Prediction (falsifiable, named bindings)**: under Sl with its reading, raylib's 26
`CLITERAL(Color)`, Darwin's six `PTHREAD_*_INITIALIZER`s, glibc's three and every brace list and
compound literal of the four populations build with **no shim and no message**, on every CI leg
(`pop221.py` and `stage2.pl` re-run through `heroes build` against the landed compiler); SDL3 needs
nothing, having no record constant; the real record constants refused are exactly §4.19's objects
and calls, **74 on this Mac, 111 on Debian clang 22, 72 on Ubuntu clang 18, 1,920 on the box**; and
`run/fixedbugs-094-...-still-builds` moves by `PT_CALL` alone. **Falsifier**: one brace list or
compound literal of those headers refused, or one refused value that C11 6.6 calls a constant
expression other than an address constant.

**Conditions** (what would change my verdict):

1. **To object**: the reading lands telling `ffi_constant_type` and *declare it at the record*
   (false of a value that is no constant); or `{ 1 / 0, 2 }` is told with a helper rather than
   panel 218's undefined words; or the three comments of § 7 stay saying the hole is open.
2. **To object**: `PT_CALL`'s move is absorbed by editing the run case in place rather than its
   comment corrected underneath and the refusal pinned as an `unsupported` case annotated
   `#~ ffi_not_constant` (`.claude/rules/records.md` § A record is never rewritten).
3. **To veto**: any version that reaches the call site (a cast, a `memcpy`, a re-declared
   prototype), changes a record's layout or storage as C sees it at the boundary, or refuses one of
   raylib's 26.
4. **To reconsider the GUIDs**: an author's ruling that a `const` object of the header, not
   `volatile`, not thread-local, is a value. Then Sl takes a second arm (an lvalue of a
   const-qualified record type kept), and the scalar rule changes with it in the same sitting.

## 9. Found beside the sitting, for the coordinator (none filed by me)

1. **C2 does not see an element of a partially filled array** (§ 4): `{ { 1 << 31 }, 2 }` builds at
   exit 0 and prints `-2147483648`; `blocking` by § Bounded discovery. Reader: `array_filler`.
2. **The lane's uncommitted `unsupported/fixedbugs-2109-...-refused.expected` pins
   `build/tu-38b0a31177f9651b`** in clang's include stack, a key that moves with the compiler
   binary: it fails under my `heroes-base`, built from the same sources (§ 3).
3. **R2.1's words for an extern object the accessor reads**: `R_EXTOBJ` is told *the header's own
   code, a constructor, a function marked used or a value's own call, reaching a library*, which
   is not what an object is; Sl supersedes it for records (`ffi_not_constant` before any probe).
4. **My own instrument**: the K batch reader of `pop221.py` lost 2 accessors on the Mac and 3 on
   Debian; each, asked alone, shows its call or writable object, so K refuses them; the table's K
   column reads them as kept and § 5 counts them as K refuses.

## 10. The prototype through the compiler on a second platform

`linux/sl.sh` in `ffi221-d22` (Debian clang 22.1.8, arm64): my worktree copied in by `tar`, its
compiler built from the seed, then `heroes-sl2` from the prototype's sources. `run fixedbugs-094`
**7 and 1** (`PT_CALL`), `unsupported fixedbugs-094` **6 and 0**, `run fixedbugs-2109` **1 and 0**;
`R_CNT`, `R_CALL_OK`, `R_MUT`, `R_CYCLES` refused `ffi_not_constant` with the Mac's words; `R_OBJ`,
`R_KOBJ`, `R_CLIT`, `R_LIT`, `R_COND_DEAD` build and print what they print on the Mac. Ubuntu clang
18 and the box through the compiler: **unrun** (their C-level verdicts are § 2's and § 6's).

## 11. Cleaned up

The box's `~/ffi221` removed (`ls` then said *No such file or directory*); containers `ffi221-u18`
and `ffi221-d22` removed; the worktree `.claude/worktrees/221-ffi-pragmatist` removed. The
prototype's diff is kept at `<scratchpad>/221-ffi/sl-prototype.diff` (70 lines).
