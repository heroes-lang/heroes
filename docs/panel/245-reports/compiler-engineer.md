# Panel 245, the compiler-engineer's report

Written 2026-11-10 from 22:55, as the work went. Seat mandate: design.md §1.1 (simplicity is the
ceiling), §1.7 (core plus elaboration: what the checker, the lowering and the backend must all
handle), Part 5 (the core/sugar table). Every number below is from a command run in this session,
named beside it; a sentence with no command is marked *unrun* or *a question*.

## The compiler

- Worktree `.claude/worktrees/245-compiler-engineer`, `git log -1` = `7fb6868b20df...` (the base,
  00-shared § The base).
- Stage 1: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 11.04 user 10.35`.
- Stage 2: `./heroes build selfhost/main.hero -o heroes-next`, `real 183.19 user 124.77 sys 30.19`
  (the load was 3.5 to 8 at the start, `uptime`; not a duration anyone should quote).
- `heroes-next --version`: `heroes 0.6.0`, `sources 0fe313ba58ba197c`. Copied to
  `p245-scratch/compiler-engineer/heroes-base` and called the BASE compiler below.


## Q1, defect 2464: what a door's call of an advised name prints, and where the rule lives

### Reproduced

`heroes-base build main.hero` on a copy of `b30-scratch/variadic/dep584/` (`p245-scratch/compiler-engineer/q1/`):
exit 0, clang's raw *"./own.h:5:12: warning: 'sprintf' is deprecated ... [-Wdeprecated-declarations]"*
with its include stack, 20 lines; `./m` prints `2`.

### The mechanism of every route, in plain C first (`q1trial/`)

`q1trial/run.sh <variant> <shape>` writes the unit's region exactly as `header_region.opening()`
writes it (push, the ignores, `KEPT`, `FORMAT_DEFINED`, `LITERAL`, `EXTENDED`, `-Wpragma-pack`),
includes one header per shape, pops, raises `checks_raised.lines()` and deprecation's three lines as
`macro_guard.around` does, and compiles with `cli/flags.hero`'s words at `-O0 -g -c` (so a backend
word such as `-Wattribute-warning` is seen). Apple clang 21.0.0 (`clang --version`). Columns: the
base; the lane's route (deprecation's three ignored inside the region); the critic's (a)
(`ignored "-Weverything"`, then every error raised by list); (b) (six undefined-behaviour words made
errors, five advice words ignored); (c) (the include read through a wrapper whose first line is
`#pragma clang system_header`). Each cell: exit, then the word clang printed raw or *refused*.

| shape | base | lane | (a) | (b) | (c) |
|---|---|---|---|---|---|
| `attr_warning` | 0 raw -Wattribute-warning | 0 | 0 | 0 | 0 raw -Wattribute-warning |
| `bounds` (`a[5]` of `int[2]`) | 0 raw -Warray-bounds | 0 raw -Warray-bounds | 0 | 1 refused | 0 |
| `constconv` (`char c = 300`) | 0 raw -Wconstant-conversion | 0 raw | 0 | 0 | 0 |
| `dep_enum` | 0 raw -Wdeprecated-declarations | 0 | 0 | 0 | 0 |
| `dep_fn` | 0 raw -Wdeprecated-declarations | 0 | 0 | 0 | 0 |
| `dep_macro` | 0 raw -Wdeprecated-pragma | 0 | 0 | 0 | 0 |
| `dep_sprintf` (584's) | 0 raw -Wdeprecated-declarations | 0 | 0 | 0 | 0 |
| `divzero` (`x / 0`) | 0 raw -Wdivision-by-zero | 0 raw | 0 | 1 refused | 0 |
| `fnptr` (default error) | 1 refused | 1 refused | 1 refused | 1 refused | **0** |
| `hash_warning` (`#warning`) | 0 raw -W#warnings | 0 raw | 0 | 0 raw | 0 raw |
| `incompat_ptr` | 1 refused | 1 refused | 1 refused | 1 refused | **0** |
| `int_conv` (`int *p = 5`) | 1 refused | 1 refused | 1 refused | 1 refused | **0** |
| `kept_format` (`%d` of a string) | 1 refused | 1 refused | 1 refused | 1 refused | **0** |
| `kept_implicit` | 1 refused | 1 refused | 1 refused | 1 refused | **0** |
| `kept_return` | 1 refused | 1 refused | 1 refused | 1 refused | **0** |
| `kept_uninit` | 1 refused | 1 refused | 1 refused | 1 refused | **0** |
| `literal_args` (243's door) | 1 refused | 1 refused | 1 refused | 1 refused | **0** |
| `memcpy_chk` (6 into 2) | 0 raw -Wbuiltin-memcpy-chk-size | 0 raw | 0 | 1 refused | 0 |
| `missing_header` | 1 refused | 1 refused | 1 refused | 1 refused | 1 refused |
| `pointer_sign` | 0 raw -Wpointer-sign | 0 raw | 0 | 0 | 0 |
| `pragma_message` | 0 raw -W#pragma-messages | 0 raw | 0 | 0 raw | 0 |
| `security_param` (`printf(f)` door) | 0 | 0 | 0 | 0 | 0 |
| `security_local` (2036's `const char *m`) | 0 | 0 | 0 | 0 | 0 |
| `shift` (`1 << 40`) | 0 raw -Wshift-count-overflow | 0 raw | 0 | 1 refused | 0 |
| `shorten` | 1 refused | 1 refused | 1 refused | 1 refused | **0** |
| `stack_addr` (address of a local returned) | 0 raw -Wreturn-stack-address | 0 raw | 0 | 1 refused | 0 |

What it settles, each a measurement of this table:

- **The lane's route closes 5 of the 14 raw shapes and leaves 9 raw at exit 0**, five of them
  undefined behaviour. It is a repair of 2464's reproducer, not of its class.
- **Two undefined behaviours the critic did not list**: `x / 0` (`-Wdivision-by-zero`) and a returned
  address of a local (`-Wreturn-stack-address`), raw at exit 0 at the base. A second trial at the
  base adds `i = i++ + 1` (`-Wunsequenced`), `*(int *)0` (`-Wnull-dereference`),
  `-Wdeprecated-non-prototype`, `-Wpointer-to-int-cast` and `-Wexcess-initializers`, each raw at exit
  0; under (a) every one of them is silent at exit 0.
- **(c) is refuted on this Mac**: in clang's system-header mode a `#pragma clang diagnostic error`
  does not hold, so panel 205's R2 (`KEPT`) and clang's own default errors are gone. A missing
  return, an uninitialised read, `int *p = 5` and `%d` of a string all build at exit 0. A GNU line
  marker in the unit (`# 1 "<heroes-region>" 3`, `lm-*.c`) gives the same: `kept_return`,
  `int_conv` exit 0 with no word. (c) is unsound by §1.12. My seat holds its veto for a core construct or a breach of the ceiling, and (c) is neither, so this is an **objection** with a soundness finding, which a seat that vetoes on soundness may carry.
- **(a) speaks nothing and silences UB.** The five undefined behaviours build silent where the
  base at least printed them. It also rests on the region knowing every default error by list. At
  the base the region already loses one, below.

### Found beside: the region's `-Wall` ignore turns a default error off, and a false note follows

`printf 'static implicit_x = 3;...'`: plain `clang -std=gnu11 -c` exits 1, *"type specifier
missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]"*. With
`#pragma clang diagnostic ignored "-Wall"` above it, exit 0 (`q1trial/ii.c`, `ii2.c`). Through the
base compiler (`q1ii/`: `own.h` holding that line and a door, bound and called):
`error[ffi_header_refused]` at exit 1, which is right, but its second note is **false**:
*"`own.h` compiles after `stdio.h`, a header of C's own it uses and does not include: write a group
naming `stdio.h` above this one"*. `own.h` uses nothing of `<stdio.h>`. The note is
`cli/header_order.hero:165`. It tries each C11 header before the refused one through
`order_trial.opening`, which reads the groups inside the region, where `-Wimplicit-int` is off.
That this is the cause is an inference until the route compiler below raises the word; see § Q1
routes as compilers.

### Q1 routes as compilers

Each route is a patch of `selfhost/emit/header_region.hero` kept in `p245-scratch/compiler-engineer/routes/<route>/`,
copied over the worktree's file, built by the base compiler (`routes/build.sh`, `./heroes-next build
selfhost/main.hero -o heroes-<route>`, 49 to 108 s of `real` each), the base file restored after
(`git status --short` empty after every build). Lines are `diff base/header_region.hero <route>/header_region.hero`:

| route | what it writes in the region | lines (+/−) |
|---|---|---|
| **lane** | deprecation's three words `ignored` after `QUIETED` | +12 / 0 |
| **told** | the same three as `error`, told by `ffi_header_refused` | +12 / 0 |
| **door** | nothing: the door writes its own `push`/`ignored`/`pop` | 0 (the program's C) |
| **(a)** | `ignored "-Weverything"`, then `KEPT`, every word of `checks_raised.checked()` but `-Wsign-conversion`, and `-Wimplicit-int` raised | +8 / −1 |
| **(b)** | the base, then nine undefined-behaviour words `error`, five advice words `ignored` | +30 / 0 |
| **E** (mine: (a) and (b) together) | (a), then the eight undefined-behaviour words `error`, then `-W#warnings` and `-W#pragma-messages` `warning` (defect 248's relay) | +34 / −1 |
| **(c)** | `# 1 "<heroes-region>" 3` as the opening's last line, `# 1 "<heroes-unit>"` before the pop | +4 / −1 |

The 38 cases (`q1cases/<shape>/`, each `extern "own.h"` binding `door(x: i64) -> i64` from one shape's
header, built at `-O0` and run under `timeout 10`, `q1cases/run.sh <compiler>`, tables
`q1cases/table-<compiler>.txt`). The base's row first, then what each route moves:

- **base**: 19 shapes build at exit 0 with clang's raw words (`grep build=0 table-base.txt | grep -vc "raw=\[\]"`). Among them, `divzero` prints `0`,
  `constconv` prints `45`, `shift` and `bounds` print `2`, `stack_addr` and `unseq` print `2`, and
  `pointer_sign` prints `98`. `memcpy_chk` dies at run time of fortify's SIGTRAP and `null_deref` of
  the runtime's null panic. 12 are refused `ffi_header_refused` and 1 `ffi_missing_header`.
- **lane**: moves exactly the five advised shapes (`attr_warning`, `dep_enum`, `dep_fn`, `dep_macro`,
  `dep_sprintf`) to silent at exit 0; the other 14 raw shapes are unmoved.
- **told**: moves the same five to exit 1, `ffi_header_refused`, against panel 208's R1 outcome (*binds
  and builds silent*). For `attr_warning`, a word clang says at `-O0` and not at `-O2`, the third note
  is **false**: *"`own.h` compiles after `stdio.h`, a header of C's own it reads itself"*. The order
  trial does not compile to the backend, where the word fires, so it finds an order that "compiles".
  Panel 208 said this word *can carry no verdict*, and here it carries a false one.
- **(a)**: moves all 19 raw shapes to silent at exit 0, the seven undefined behaviours with them.
- **(b)**: refuses the seven undefined behaviours (`bounds`, `divzero`, `memcpy_chk`, `null_deref`,
  `shift`, `stack_addr`, `unseq`), quiets five advice shapes and `pointer_sign`, and leaves
  `constconv`'s value quiet. `hash_warning`, `pragma_message`, `excess_init`, `nonproto` and
  `ptr_int_cast` stay raw: the class stays open for every word it does not list.
- **E**: refuses the same seven, quiets every advice shape, and leaves `hash_warning` and
  `pragma_message` speaking exactly as the base does (defect 248's relay). No other shape is raw.
  Under (a) and E, `implicit_int`'s second note becomes the true *"repair the header, or name the one
  that declares this group's C"* where the base says the false *"compiles after `stdio.h`"*. That
  measures the cause of the found-beside defect above.
- **(c)**: accepts at exit 0 `kept_format` (prints `78514554 9`, a pointer's bits read by `%d`),
  `kept_uninit` (`2`), `kept_return` (`1`), `literal_args` (`11`), `varargs_promo` (`0.0000008`),
  `shorten` and `incompat_ptr`: every row panel 205's R2 and the build's flags refuse.

**Defect 158's carriers** (`q1trial/carriers.sh <variant>`, each carrier compiled in place in the
region, counting clang's warnings above its fatal *file not found*). `noted-outer`: base 1
(`-Wdeprecated-declarations`), lane 0, (a) 0, E 0. `warned` 1, `warned-outer` 2, `sibling-outer` 1 at
the base, and the same under the lane's route and E; (a) 0 of all four. `heroes-base run
tests/harness/main.hero -- heroes-<route> unsupported fixedbugs-158` reads **14 passed, 0 failed** for
base, lane, E and (a) alike, so each route's lost noise is invisible to the suite. Under the lane's
route and E, `fixedbugs-158-a-note-in-another-header-between` stops testing what its name says. Its
carrier `noted-outer.h` needs one line, `#pragma clang diagnostic warning "-Wdeprecated-declarations"`,
a header's own pragma deciding inside the region, to keep its note. That is unrun as a case. Under
(a), all four carriers' cases go blind.

**The census, re-run counting every word** (`census/one.sh <arm> <i> <header>`, `xargs -P 4`, each
header alone in the region at `-fsyntax-only`, the lane's 2,400 Homebrew headers and the 761 headers
`git ls-files '*.h'` tracks outside `docs/ seed/ vendor/ runtime/` at the base). Arms: the base and
route E.

| set | base exit 0 / 1 | E exit 0 / 1 | headers whose exit moved | raw words at exit 0, base | raw words, E |
|---|---|---|---|---|---|
| Homebrew 2,400 | 1,066 / 1,334 | 1,066 / 1,334 | 0 | 0 headers | 0 |
| tracked 761 | 680 / 81 | 680 / 81 | 0 | 6 headers, below | 0 |

In the census, every raw Homebrew word stood in a refused header: `-Winclude-next-absolute-path` 716,
an artefact of including by absolute path; `-Wvisibility` 7; `-W#pragma-messages` 2; `-W#warnings`
1. The tracked headers that compile and speak, artefacts aside (the census writes no `-Wpragma-pack`
or `-Wignored-pragmas` ignore, which the real unit does): `surface-fixtures/warning248/lib.h`
(`#warning`, defect 248's pinned case), `unsupported/fixedbugs-158-sibling-a.h` (`#warning`),
`unsupported/fixedbugs-337-located.h` (`#warning`, `-Winvalid-source-encoding`),
`unsupported/fixedbugs-426-no-prototype.h` (`-Wdeprecated-non-prototype`),
`unsupported/fixedbugs-2049-...-refused.h` (`-Wincompatible-library-redeclaration`) and
`run/fixedbugs-582-...h` (`-Wignored-pragmas`, the artefact). The lane's census was the
`-Wformat-nonliteral` arms' outputs, and its *0 of 2,400* is *0 of the 1,066 that compile*, plus one
refused tracked carrier. This census counts every word and agrees: no library header in the region
speaks at exit 0 when only included. What a door's call of a library's inline code prints is the
38 cases above, not this census.

**What any region route moves in the tree** (`grep -rl 'diagnostic ignored "-Wsign-conversion"'`):
465 of the 850 files of `tests/emission/` and 2 of 27 of `tests/golden/emit/`, each re-blessed, the
Linux-only ones on Linux; `cli/order_trial.hero:161`'s pinned opening; `header_region.hero`'s two
tests (the opening's indices, `never_for` for each new error word); the seed at the next refresh.
The door route moves none of these.

## Q2, defect 2465: a callee that reads past `...` what no call passed

### What already exists at the base (critic item 11, re-read)

A direct binding of a function clang marks `format`, with nothing past `...`, is refused at its
probe: `emit/ffi_call.hero:111-155`'s `call_shape` reads clang's *"format string is not a string
literal"* at the declaration's `#line`. The probe stands after the region's pop, where `-Wformat` is
raised whole (`checks_raised.lines()`), so `-Wformat-security` is an error there. Both of the brief's
first two routes are therefore the base for an attributed callee. What is open is inside the region,
where `-Wformat-security` is passed over (`header_region.hero`'s `FORMAT_DEFINED`, defect 2036), and
every callee clang does not mark.

### Plain C first (`q1trial/run.sh`, variants `sec` and `mfa`)

- `-Wformat-security` made an error in the region (`sec`): the forwarding door `say(f) { return
  printf(f); }` exit 1; 2036's `const char *m = "hi\n"; printf(m)` exit 1; `const char *const m`
  exit 0 (clang follows a `const` pointer to its literal); 243's door with data arguments stays
  `-Wformat-nonliteral`'s.
- **A narrower pragma does not exist on this clang**: `-Wmissing-format-attribute` is listed by
  `clang --print-diagnostic-options` and never fires on Apple clang 21. A parameter forwarder, a
  `va_list` forwarder and a variadic one (`q1trial/mfa.c`) give only `-Wformat-security`. Whether
  clang 22's upstream implements it is unrun here (Linux is off, item 24).

### The routes as compilers

- **sec**: `-Wformat-security` taken out of `FORMAT_DEFINED`, so `KEPT`'s `-Wformat` holds it in the
  region (`routes/sec/header_region.hero`, −1 line).
- **var**: every binding of a function the dump marks variadic that hands nothing past its `...` is
  refused at its last fixed parameter, `ffi_call_shape`, its note the door
  (`routes/var/ffi_past_prototype.hero`, +31 lines: one marker per function in `lines`, one kind in
  `refused`, one helper). My first build refused nothing: every function carries an `ARITY` row at
  `index: f.params.len()` (`cli/pointee_wants.hero:91-95`), and my `past()` read that row as a
  parameter past the `...`. Rebuilt with the row excluded.
- **C contract**: the base, said in the spec and design.md.

Cases (`q2cases/<case>/`, built at `-O0`, run under `timeout 10` with their arguments, output cut at
200 bytes, `q2cases/run.sh <compiler>`):

| case | what it binds | base | sec | var |
|---|---|---|---|---|
| `door` | `say(f) { return printf(f); }`, `n=%ld` | 0, `n=4337353440` | **1** `ffi_header_refused` | 0, `n=4344660704` |
| `sdoor` | `sscanf(s, f)` in a door, `"5 6"`, `"%d %d"` | 0, `1 after`: an `int` written through an address nobody passed | **1** `ffi_header_refused` | 0, `1 after` |
| `sqlite` | `sqlite3_mprintf(format: cstr lent)` direct, `n=%d` | 0, `n=2116552223` | 0, `n=-1883799772` | **1** `ffi_call_shape` |
| `openc` | `open(path, flags)` direct, `O_CREAT + O_WRONLY` | 0, mode `-r--r-x---` | 0, `-r--r-x---` | **1** `ffi_call_shape` |
| `open_ro` | `open(path, flags)` direct, `O_RDONLY`: **correct** | 0, `true` | 0, `true` | **1** `ffi_call_shape` |
| `open_door` | `open(p, fl)` inside a door, `O_CREAT + O_WRONLY` | 0, mode `--wx------` | 0, `--wx------` | 0, `--wx------` |
| `sqlite_door` | `sqlite3_mprintf(f)` inside a door, `n=%d` | 0, `3` | 0, `3` | 0, `3` |
| `say_s` | `printf("%s", s)` in a door, `100%d` | 0, `100%d5` | 0, `100%d5` | 0, `100%d5` |

What it settles:

- **sec closes the shape of the class clang can see**: every door that forwards a run-time format
  to a callee marked `format`, a memory fault and an unseen write among them, refused at exit 1 with
  clang's own repair in the note (*"treat the string as an argument to avoid this"*, a fix-it
  `"%s", `). It refuses one correct tracked program. `heroes-base run tests/harness/main.hero --
  heroes-sec run fixedbugs-2036` reads 3 passed, 1 failed:
  `run/fixedbugs-2036-a-header-s-own-printf-of-a-format-held-in-a-variable-builds`. Its two repairs
  build under `heroes-sec` and print the case's own `hi 3` (`q2fix/const_const`, `q2fix/percent_s`).
  `unsupported fixedbugs-2036` reads 5 passed, 0 failed under both compilers.
- **var refuses a correct program and closes nothing it refuses.** `open_ro` is correct and refused.
  The door its note names, `open_door` and `sqlite_door`, builds at exit 0 with the very fault of
  `openc` and `sqlite`. No rule the compiler can read separates `open(p, O_RDONLY)` from `open(p,
  O_CREAT)`: the difference is a run-time value. The rule would move the fault from a Heroes
  binding into C the author writes, and C converts nothing past `...` in either.
- **Its census** is grep-based, a lower bound from my vocabulary: `git ls-files '*.hero' | xargs grep
  -lE '^\s+function (printf|...|wprintw)\('` over 45 variadic names, the recovery corpus left out,
  finds 5 files. None builds at the base with nothing past `...`: `fixedbugs/ffi-call-format.hero` is
  already `ffi_call_shape`, `twoarity-lent`'s two pass a value past `...` (R1's refusals), and panel
  178's two `flock_S.hero` pass `mode` past `...`. So var refuses 0 tracked programs, and the cost it
  carries is the correct programs nobody has written yet.
- **What a program printing a run-time string writes**: a door `printf("%s", s)` (`say_s`, which
  prints the format's bytes as text, `100%d`, under all three compilers), or `fputs`, `puts` or
  Heroes' own `print`, none variadic.

## Q3, defect 2448: a header and a library of two releases, paired at exit 0

### What the base reads, and what it does not

- No module reads `LIBRARY_PATH` from the environment: `grep -rn LIBRARY_PATH selfhost` (the
  loader's and `C_INCLUDE_PATH` left out) finds one comment, `library_source.hero:26`, about a
  constant of that name. clang's driver turns it into `-L` for the linker unseen.
- **The library half exists**: the link's trace maps each bound name to the file that supplied it
  (`cli/supplied_judged.hero:52-70`, `supplied: {str: str}`; Apple's map and `-Wl,-t`,
  `cli/supplied_probe.hero:238-245`; lld-link's `/MAP`, `cli/supplied_windows.hero`).
- **The header half exists elsewhere**: which folder found each file the units read, and who named
  that folder (`cli/shadowed.hero`, `cli/shadowed_tree.hero`, `cli/stand_in.hero`'s `finder`,
  `cli/named_folders.hero`'s `of_compiler` and `of_environment`). That stage runs before any
  probe. The link runs last. No route can pair them without threading one into the other.

### P4, the note, built (`routes/P4/head_names.hero`, +1/−1 at `selfhost/head_names.hero:222`)

`heroes-<c> build l.hero` on the critic's `extern "/opt/homebrew/opt/readline/include/readline/readline.h"
link "readline"` (`q3/locked/`): both exit 1 `machine_locked_path`. The base's note reads *"set
`CPATH` to the directory holding it, or name its library with `pkgconfig`"*. P4's reads *"set
`CPATH` to the directory holding it and pass `--library <dir>` with the directory holding the
library its group links, the two halves of one release, or name its library with `pkgconfig` and
let `pkg-config` answer both"*. Following each on the critic's `rl.hero`
(`RL_READLINE_VERSION`, `rl_initialize()`):

| followed | build | prints | `otool -L` |
|---|---|---|---|
| the base's note, `CPATH=$R/include` | 0 | `header 2051`, `0` | `/usr/lib/libedit.3.dylib` |
| P4's note, `CPATH=$R/include` and `--library $R/lib` | 0 | `header 2051`, `0` | `/opt/homebrew/opt/readline/lib/libreadline.8.dylib` |

So the base's own remedy builds GNU readline 8.3's header against libedit, which is a false message
by §4.17 whatever the ruling on the pair. P4 is its repair. It moves the 3 goldens that pin the old
text (`grep -rl 'set \`CPATH\` to the directory holding it' tests`, the recovery corpus's frozen copy
left out): `check/fixedbugs-216-...`, `-225-...`, `-250-...`.

### P1, pair by provenance: not built, its cost estimated from the modules read

The rule: refuse a group with `link` whose header was found in a folder a word or a variable of the
build names (`--include`, `CPATH`, `C_INCLUDE_PATH`, `CCC_OVERRIDE_OPTIONS`, a package's `-I`) while
the file that supplied its names came from a folder none names (`--library`, a package's `-L`,
`LIBRARY_PATH` read from clang's link job as defect 2344 reads `CPATH`), or the reverse. Applied by
hand to the critic's eight rows (item 17), it refuses the six mismatched rows and passes the two
paired ones. That holds by construction and is an inference, since no compiler of it was built.
Where it would land:

- a judge of the two halves, one new module of about 120 lines with its tests;
- the header provenance threaded from `shadowed.checked`'s stage to `supplied.hero`'s
  `Judging`, about 20 lines over `cli/produce.hero`, `cli/shadowed.hero` and `cli/supplied.hero`;
- the `-L` folders clang adds from `LIBRARY_PATH`, read from the link job's `-###`
  (`cli/clang_jobs.hero` reads jobs already), about 30 lines;
- the refusal's words, about 40 lines in `cli/supplied_words.hero` (353 lines today) or a module of
  its own;
- a design.md §4.19 paragraph and a spec § 13 sentence.

Total about 200 to 250 lines over five modules. That is not core: it lands in the CLI's link stage,
and the checker, the IR and the emitter never see it. It reads the linker's answer, so lld-link
ignoring `LIBRARY_PATH` (design.md:3128-3133) is judged by the file it took and not by the variable.
That is unrun on Windows. Its false positive is a header-only folder named beside a library the
platform's linker finds: a correct program refused, whose repair is one word, `--library` naming that
library's folder.

### P2 and P3

- **P2, pair by prefix** (`<p>/include` with `<p>/lib`): a premise about where packagers put
  things. `.claude/rules/module-shape.md` § A narrowing asks the value, never the world (lines 44-62)
  owes it a falsifiable claim and a test that fires when it dies. Nix's split outputs would kill it
  (unrun). I object.
- **P3, ask `pkg-config` by the folder**: measured to give no answer in 2448's own reproducer.
  `pkg-config --list-all | wc -l` reads 510 packages on this Mac's default path, and its one
  `libcurl` is Homebrew's shim for the SDK (`pc_path` ends `.../os/mac/pkgconfig/27`), answering
  `/Library/Developer/CommandLineTools/SDKs/MacOSX27.sdk/usr/include` and `/usr/lib`. The keg's `.pc`
  is off the default path, keg-only, and with `PKG_CONFIG_PATH` naming it its `includedir` is
  `/opt/homebrew/Cellar/curl/8.23.0/include`, the resolved path and not the `opt/curl/include` that
  `CPATH` named. A by-folder question finds the keg only where the author already pointed
  `pkg-config` at it, and then `pkgconfig "libcurl"` is the binding's own route. I object.

### Refused, paired or told

This language has no warning level (design.md:4336, *"a diagnostic is exit 1 or nothing"*), so
*told* is either a refusal or a note on another diagnostic. The note the build already gives on
this path is P4's, false today. *Paired* is what P4's remedy and a `pkgconfig` group do. *Refused*
is P1.

## The module ceiling, asked of the instrument

`heroes-base run tests/harness/main.hero -- heroes-<route> layout header_region`, each route's file in
the tree for the run (`routes/harness.sh`): base, lane, (a) and sec 4 passed, 0 failed. **(b) reads
`311 lines of code, past §11's 300`, and E reads `312`.** Either one lands with a split along a seam
of `header_region.hero`, for instance its word lists into a module of their own. That moves about 45
lines and no logic. It is a module ceiling, `.claude/rules/module-shape.md`, not §1.1's.

The compiler's size, for §1.1's question: `find selfhost -name '*.hero'` gives 810 modules and
210,735 lines by `wc`. Non-blank, non-comment lines before each file's `## Tests` come to about
94,560 (my count, not the layout instrument's unit), 37,763 of them in `cli/` and `emit/`. Pascal-P4's
~4,000 lines were passed long ago. None of the routes here moves that order of magnitude. The largest
is P1's estimate of 200 to 250 lines.

## Verdicts

### Q1

- `verdict`: **object** to the lane's route as the resolution, and to (a), (b), *told* and the door.
  Objection to (c) with a soundness finding. My recommendation is **E**: every word in a header's
  code named once, in one list in `emit/header_region.hero`, quiet unless listed.
- `section`: design.md §1.12 (robustness); §4.19:2676-2677 (panel 208's R1, *binds and builds
  silent*); Part 8 at :4336 (*a diagnostic is exit 1 or nothing*); §1.7 (none of these is core).
- `implementation_cost`: E is +34/−1 lines in `selfhost/emit/header_region.hero` (402 by `wc`, 312 code
  lines by `layout` after the change, so a split is owed). Beside it: `never_for` phrases for eight
  words, about 10 lines; `cli/order_trial.hero:161`'s pinned text; the two tests of
  `header_region.hero`; 465 of 850 `tests/emission/` files and 2 of 27 `tests/golden/emit/` files
  re-blessed; one line in `unsupported/fixedbugs-158-noted-outer.h`. Emitter text only: no lexer,
  checker, IR or lowering. The lane's route is +12 lines and moves the same 467 emissions.
- `needed_for_self_hosting`: no.
- `argument`: The lane's route closes 2464's reproducer and leaves 14 of the 19 raw shapes raw,
  seven of them undefined behaviour at exit 0. (a) silences those seven and breaks defect 248's pinned
  check (`surface` 451 and 1). (c) loses every `KEPT` error (`%d` of a string prints a pointer at
  exit 0). *told* refuses what panel 208 ratified as silent, and gives a false note for a backend
  word. E refuses the seven, quiets the advice, relays `#warning`, and moves 0 of 3,161 headers'
  exits. `surface` reads 452 and 0, 158's cases 14 and 0, and the implicit-int false note is gone.
  208's outcome binds. Its mechanism, *nothing else widened*, ended when 243's R1 put program code in
  the region.
- `prediction`: at the gate that lands E, `emission` moves exactly the files that hold the region's
  text (465 at this base, plus any blessed since) and no other file, and `layout` refuses
  `header_region.hero` unless the landing splits it (312 lines unsplit).
- `condition`: a correct program E refuses, whether a tracked one, one of the 1,066 Homebrew headers
  that compile, or a door whose library code trips one of the eight words falsely. Or a default error
  clang gives outside E's list, so that a header plain clang refuses builds under E. I could not
  enumerate default errors on this Mac (`xcrun -f diagtool` fails), so that premise has no test that
  fires here. Either would move me to (b) with the relay, at the price of new words staying raw.

### Q2

- `verdict`: **approve** sec (`-Wformat-security` an error in the region), with defect 2036's
  variable-format case rewritten or pinned as a known cost. **Object** to *var*. For a callee clang
  does not mark, or one keyed by a run-time value (`open`, `fcntl`, `sqlite3_mprintf`), I'd keep the
  C contract and say so in design.md Part 7 row 10, with a spec § 13 sentence queued for the author.
- `section`: §1.12 with CLAUDE.md § Precedence ranks 3 and 4 (robustness decides *that*, Principle 0
  *how*); design.md:3944-3949 (Part 7 row 10); §1.7.
- `implementation_cost`: sec is −1 line in `header_region.hero` and its comment rewritten (about 6
  lines). It moves the same 467 emissions, shared with E if both land together. One `run` golden is
  moved or rewritten. *var* is +31 lines in `selfhost/emit/ffi_past_prototype.hero` (555 by `wc`) and
  refuses 0 tracked programs (grep lower bound).
- `needed_for_self_hosting`: no.
- `argument`: sec refuses every door that forwards a run-time format to a callee clang marks: `n=%ld`
  printing an address, `sscanf` writing through nobody's pointer. Its note carries clang's own `"%s"`
  fix-it. It refuses one correct case, whose two repairs print the case's own `hi 3`. *var* refuses a
  correct `open(p, O_RDONLY)`, and the door its note names rebuilds the same fault at exit 0
  (`--wx------`, a `%d` read from nowhere). It moves the fault into C and refuses correct code to do
  it. No rule the compiler can read tells `O_RDONLY` from `O_CREAT`.
- `prediction`: with sec landed, the whole `run` form fails on exactly one case,
  `fixedbugs-2036-a-header-s-own-printf-of-a-format-held-in-a-variable-builds`, until that case is
  rewritten or moved. I ran it narrowed to 2036, 3 passed and 1 failed; the whole form is unrun.
- `condition`: a second correct tracked program refused by sec, or a CI leg's clang where
  `-Wformat-security` in the region refuses glibc's own fortified inline code (unrun, Linux off).

### Q3

- `verdict`: **approve** P4 (built and measured). **Object** to P2 and P3. **No verdict on P1**: it is
  unbuilt, and a route I recommend is one I ran. Its cost is estimated above.
- `section`: §4.17 (a message carries what fixes the mistake, and P4's old text sends the author
  into the mismatch); §4.19 at design.md:3125-3133 (the machine's `CPATH`, `LIBRARY_PATH` and
  `--library`); §1.7 for P1 (link-stage logic, not core).
- `implementation_cost`: P4 is +1/−1 at `selfhost/head_names.hero:222` and 3 `tests/golden/check/`
  goldens. P1 is about 200 to 250 lines over five modules of `cli/` (`supplied.hero` 305,
  `supplied_words.hero` 353, `shadowed.hero` 485, `clang_jobs.hero` 269, one new module), an
  inference from reading them.
- `needed_for_self_hosting`: no.
- `argument`: Following the base's own note links GNU readline 8.3's header to `libedit.3.dylib` at
  exit 0, and following P4's links `libreadline.8.dylib` (`otool -L`). That false remedy is the
  measured part of 2448's class. The pair's harm beyond it, a wrong value or a fault, is still an
  inference: I did not measure one. P3 measured null on 2448's own reproducer, and P2 rests on where
  packagers put files. P1 reads both halves from answers the build already asks, but nobody has built
  it yet.
- `prediction`: with P4 landed, `machine_locked_path`'s note for a header path names `--library`, and
  the three goldens `fixedbugs-216`, `-225` and `-250` are the only `check` files that move.
- `condition`: P1 built and run on the critic's eight rows plus a header-only named folder beside a
  platform library, its refusals counted over the tracked programs, would let me approve or object
  to it.

## What I ran whole, and why

The brief allows a whole suite where a route changes what the checker refuses or what every unit's
opening holds, one at a time. Batch 30's gate B was running four suites wide on this Mac from about
00:00 (the coordinator's note; `uptime` read 30 to 45). So I timed nothing and ran these whole, each
alone:

- `surface` under E (452 passed, 0 failed) and under (a) (451 and 1, defect 248's check). It takes no
  case filter, and defect 248's told-once is the pinned behaviour the region routes split on.
- `unsupported` under E: **462 passed, 0 failed**. The refused cases are where a first error could
  move while the exit does not, and none moved.
- the census of 3,161 headers, base and E, which is the instrument the shared brief asked a seat
  to re-run.

Narrowed: `unsupported fixedbugs-158` (base, lane, E, (a)); `run fixedbugs-2036` and `unsupported
fixedbugs-2036` (base, sec); `layout header_region` (base and six routes). **Unrun whole**: `run`,
`emission`, `warnings` and `check` for any route. `emission` is red by construction under every region
route (the 467 files above). The census, which holds every tracked header including each `run`
golden's, predicts `run` green under E.

## Found beside, not repaired

1. **A false note from the region's `-Wall` ignore turning a default error off.** Reproducer
   `p245-scratch/compiler-engineer/q1ii/`: `own.h` is `#include <stdint.h>`, then `static implicit_x =
   3;`, then `static inline int64_t door(int64_t x) { return x + implicit_x; }`, bound `door(x: i64)
   -> i64` and called. `heroes build main.hero` at the base: exit 1 `ffi_header_refused` (right), and
   the note *"`own.h` compiles after `stdio.h`, a header of C's own it uses and does not include:
   write a group naming `stdio.h` above this one"* (false; `cli/header_order.hero:165`). Cause,
   measured: under routes (a) and E, which raise `-Wimplicit-int` in the region, the note becomes
   *"repair the header, or name the one that declares this group's C"*
   (`q1cases/out-{base,a,E}/implicit_int.err`). Proposed class **blocking**, a false message
   (`.claude/rules/verification.md` § Bounded discovery). It is defect 2052's shape: a default error
   among `-Wall`'s, lost to the region's ignore. Which other default errors `-Wall` holds is unrun
   here, with no `diagtool` on this Mac.
2. **2464's class is wider than the critic's list**, and these are members of it rather than new
   defects: `-Wdivision-by-zero` (`x / 0`, prints `0`), `-Wreturn-stack-address`, `-Wunsequenced`
   (`i = i++ + 1`) and `-Wnull-dereference` (the runtime's null panic at run time), all undefined
   behaviour, plus `-Wdeprecated-non-prototype`, `-Wpointer-to-int-cast` and
   `-Wexcess-initializers`. Each is raw at exit 0 at the base (`q1cases/table-base.txt`).
3. **A path in the shared brief**: item 18 cites `cli/head_names.hero:222`; the module is
   `selfhost/head_names.hero:222` (`ls selfhost/cli/head_names.hero` fails).
