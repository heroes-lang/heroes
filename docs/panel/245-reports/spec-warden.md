# Panel 245, the spec-warden

Written 2026-11-10, 22:56 to about 23:40, as I went. The worktree is
`.claude/worktrees/245-spec-warden`, detached at `7fb6868b20dfaecbdc7cf501b75b79aef7b14cf3`
(`git rev-parse HEAD`). The compiler was built there in two stages:
- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 17.57`;
- `./heroes build selfhost/main.hero -o heroes-next`, `real 177.62`, `user 120.74`, with the load at
  about 8, so neither time is a duration.

`heroes-next --version` prints `heroes 0.6.0` and `sources 0fe313ba58ba197c`. Probes are in
`.claude/worktrees/p245-scratch/spec-warden/` (`q1/`, `q2/`, `q2/S/`, `q3/`, `spec/`). There was no
paid run, no `--refresh` and no GitHub write. **Every token count is vendored and a lower bound** of
the real count. The real figure quoted is the base's pin, which I did not re-measure.

## The ceiling, read today

`grep -n '^### 1.6' docs/design.md` gives line 253:
- the ceiling, *"must fit in 16384 tokens, measured by `claude-opus-5` through `POST
  /v1/messages/count_tokens`"* (`:255-256`);
- the consolidation block's 1,500 real before the thesis harness has run (`:290-293`);
- the payment rule, unconditional (`:316-322`);
- `DELTA_GATE` at 50 vendored (`tests/harness/suite_spec.hero:217-218`).

**The base, measured**: `./heroes-next measure spec/heroes-spec.md` gives **8167** legacy, **8314**
cl100k, a maximum of 8314 and a spread of 147. The real pin is **10898**, `claude-opus-5`, taken
2026-11-09. *"Headroom: 5486 against the 16384 ceiling — but the FFI floor mortgages 60 of it."* So
no route of this sitting can approach the ceiling, and what decides is the payment rule.

**What the spec says on this sitting's ground today**: `grep -n -i
'variadic\|\.\.\.\|door\|warning\|deprecat\|CPATH\|--include\|--library\|printf\|format'
spec/heroes-spec.md` matches four lines. None is in § 13:
- `:260`, *"of its own"*, about `while true`;
- `:289`, *"no user variadics"*, about Heroes functions;
- `:304`, *"`ok(...)`"*;
- `:338`, *"`print(...)`"*.

So the specification states nothing about:
- C's `...`;
- a header of the program's own;
- what clang prints of a header's code;
- the environment or the build's flags.

Panel 243 left it that way: *"no § 13 sentence: § 13's sentence that a parameter is declared at the
header's own width stays the whole rule"* (`docs/panel/243-*.md:114-115`). § 13's sentences that
bear on this sitting are three:
- *"A group names its header, and `link` a library when the symbols need one"*;
- *"clang checks every result type, constant and record field against that header"*;
- *"Instead of `link`, `extern "raylib.h" pkgconfig "raylib"` asks the system where its headers and
  libraries are"*.

**No route below makes one of the three false.**

## Q1, defect 2464: what a door's body prints

**Run** (`q1/`):

| probe | build | stderr | run |
|---|---|---|---|
| `direct.hero`, `tmpnam` (`_stdio.h:287`, `__deprecated_msg`) bound directly | exit 0 | 1 line, `wrote direct` | `true` |
| `door.hero`, the same `tmpnam` called in a door `name_it()` | exit 0 | 20 lines, `-Wdeprecated-declarations` | `true` |
| `dep584.hero`, the critic's `sprintf` door | exit 0 | 20 lines, `-Wdeprecated-declarations` | `2` |
| `wide.hero`, the critic's q1b header | exit 0 | 9 raw warnings, below | `1 1 4 5 2 2` |
| `hw.hero`, the critic's q1c header (`#warning`, `#pragma message`) | exit 0 | `-W#warnings`, `-W#pragma-messages` | `7` |

`wide.hero` printed, one line each: `-Warray-bounds`, `-Wattribute-warning`,
`-Wbuiltin-memcpy-chk-size`, `-Wconstant-conversion` and `-Wdeprecated-pragma`, then
`-Wdeprecated-declarations` twice, then `-Wpointer-sign` and `-Wshift-count-overflow`. The door with
`1 << 40` printed `2`, and the door reading `a[5]` of an `int[2]` printed `2`. Both are undefined
behaviour at exit 0.

So one name builds silent when bound and speaks when a door calls it. Since panel 243's R1 the door
is the only route to `sprintf` (design.md:3939-3941).

**Route (c), measured with plain clang** (`q1/sys/`, `clang -fsyntax-only`). A header holding:
- a `%d` handed a `long long`;
- a door forwarding its format with a data argument;
- `1 << 40`.

It was included between `push`, `error "-Wformat"`, `error "-Wformat-nonliteral"`, `error
"-Wshift-count-overflow"` and `pop`:
- read as an ordinary header: **exit 1, 3 errors**;
- read with `#pragma clang system_header`: **exit 0, 0 errors, 0 warnings**;
- read through `-isystem .`: **exit 0, 0 errors, 0 warnings**.

In a system header the region's raised errors do not fire, so route (c) as stated silences every
check the region raises.

**The spec**: no route needs a sentence. The Heroes program is the same text under every route:
- what clang says of a header's own C is not a fact of this language;
- § 13's *"clang checks every result type, constant and record field against that header"* stays
  true under each route;
- the spec holds no warning word today (grep above).

Where the rule lives is design.md §4.19, `:2676-2677` (*"A name its header marks deprecated binds and
builds silent: a deprecation is advice about a program, not a mistake in it, and this language has
no warning level"*), and `.claude/rules/c-boundary.md:154`. The general sentence is design.md:4336:
*"this language has no warning level: a diagnostic is exit 1 or nothing"*.

**§1.2 reading.** Clang's raw text at exit 0 asks a model to decide whether a built program is wrong.
A rewrite spent on advice is a rewrite rate paid for nothing. A rewrite skipped on `1 << 40` is a
wrong value shipped. Only a route where every word is exit 1 or silent gives the rewrite rate one
meaning. That is route (b), with the undefined-behaviour words errors and the advice words silent.

- `verdict`: approve (b); object to keep, to the lane's three words alone and to (a); object to (c)
  as stated
- `section`: design.md §1.6 (+0), §1.2, :4336-4337; §4.19 :2676-2677
- `spec_token_delta`: +0 under every route, 8167 / 8314 before and after (measured base; no sentence
  drafted, none owed)
- `removal`: nothing, and none is owed at +0
- `needed_for_self_hosting`: no
- `argument`: No route changes what a Heroes program says, so none owes the specification a word. The
  rule's home is design.md §4.19 and its general form is :4336, *a diagnostic is exit 1 or nothing*.
  Keeping today's state, or quieting the lane's three words alone, leaves clang's text at exit 0, so
  :4336 stays false: five words plus `#warning` and `#pragma message` still print. (a) makes :4336
  true by hiding `1 << 40` printing `2` at exit 0, which robustness (rank 3) forbids. (c) measured
  silences the raised `-Wformat` errors, so Part 7 row 10's door refusal stops holding. (b) alone
  makes every word exit 1 or silent, which is what §1.2 needs.
- `prediction`: with (b) landed, (1) `heroes measure spec/heroes-spec.md` reads 8167 / 8314 at the
  landing commit; (2) `q1/door.hero` builds at exit 0 with stderr exactly `wrote door`; (3)
  `q1/wide.hero` exits 1 naming `door_shift` or `door_bounds`. Scored at the landing's gate by those
  commands.
- `condition`: a measured advice word that names undefined behaviour, which (b) would then have to
  raise; or a route that changes a `.hero` program's meaning, which would owe a § 13 sentence.

## Q2, defect 2465: may a format function be bound directly

**Run** (`q2/`):

| probe | build | run |
|---|---|---|
| `pdirect.hero`, `printf(format: cstr lent) -> i32` | **exit 1**, `ffi_call_shape`; its note drafts the door | none |
| `pnum.hero`, `printf(format: cstr lent, n: i64)` | **exit 1**, `ffi_parameter_type`; two notes, the second drafting the door | none |
| `openc.hero`, `open(path, flags)` with `O_CREAT + O_WRONLY` and no mode | exit 0 | `3`; `made.txt` created `-r--r-x---` |
| `sqlite.hero`, `sqlite3_mprintf(format: cstr lent) -> cstr`, given `n=%d` | exit 0 | `n=-1845483673` |
| `door.hero`, `say(f) { return printf(f); }`, given `n=%ld` | exit 0 | `n=4338664160` on one run, `n=4305011424` on the next |
| `door.hero`, given eight `%s` | (same binary) | **exit 134**, *"panic: a null pointer was read through"* |
| `own.hero`, `print(a[0])`, spec § 11, given `%n%s%s%s` | exit 0 | `%n%s%s%s`, exit 0 |

**Route S** (`-Wformat-security` raised in the region), emulated by the same pragma in the header
itself (`q2/S/`). The forwarding door and defect 2036's correct case
(`run/fixedbugs-2036-a-header-s-own-printf-of-a-format-held-in-a-variable-builds.h`, copied) each
exit 1, `ffi_header_refused`. Each message has the line and clang's own note, *"treat the string as an
argument to avoid this"*, a one-edit repair.

The region's own comment carries the precedent: constructs C defines, refused *"a known cost by the
author's ruling of 2026-10-13: robustness first"* (`selfhost/emit/header_region.hero:154-155`).

**Route D** (every function its header declares with `...` refused when bound directly, which widens
`ffi_call_shape` past clang's `format` attribute). I grepped the tracked `.hero` files for 29 common variadic names. Outside `docs/panel/` records, the only bindings are the three goldens that already
annotate a refusal. A lower bound by name list; the census is the compiler-engineer's.

**What a program that prints a run-time string writes**: `print(s)`, § 11, under every route at +0.
`own.hero` printed a hostile format as text. Without the newline, it writes a door with a literal
format, `printf("%s", s)` or `fputs`, which builds under D and S alike.

**The sentence, if one were wanted.** Each draft joins § 13's width sentence after *"and what a `ptr`
points at."*, in a whole copy of the spec under `spec/`, measured from the worktree's root:

| draft | text | legacy | cl100k | delta |
|---|---|---|---|---|
| W0 | *A function its header declares with `...` is never bound: bind instead a function of a header of your own that calls it.* | 8194 | 8341 | **+27** |
| W1 | W0 with *whose prototype fixes what it hands on, a format written inside as a literal* | 8206 | 8353 | **+39** |
| W2 | *Nothing is bound to a function its header declares with `...`: a header of your own calls it from a function of fixed prototype, any format inside as a literal, and the group binds that.* | 8207 | 8354 | +40 |
| W3 | *Nothing passes past a header's `...`, and a format is never bound with nothing after it: bind instead …, a format written inside as a literal.* | 8209 | 8356 | +42 |

All four are under `DELTA_GATE`. The real cost is about ×1.25 of each, an inference from panel 243's
ratio. **W0 and W1 are true only under route D**: at the base, `open` and `sqlite3_mprintf` bind
directly. **W3 is false at the base too**: `sqlite3_mprintf`'s format is bound with nothing after it,
and it builds. The base's rule is keyed to clang's `format` attribute, which a reader cannot see. So
only D gives the class a sentence that is true and that a reader can apply from the C prototype
alone. Whether to buy it is a predictability question: the notes already draft the door, so one
refusal teaches. The answer is the blind seat's, a paid run, which this sitting does not hold. I did
not run the `spec` suite's word checks on W0 to W3. Panel 243's spec-warden ran them on V1 and V2,
both with `...` in a code span, and they passed.

- `verdict`: approve D and S together, at +0; object to keep; object to W0 landing now, unpaid
- `section`: design.md §1.6 (payment rule, +0), §1.2; CLAUDE.md § Precedence ranks 3 and 4 (panel
  215); Part 7 row 10, :3939-3950
- `spec_token_delta`: +0 for D, S and D+S (8167 / 8314); W0 would be +27 vendored, 8314 to 8341,
  a lower bound
- `removal`: none at +0. W0 has no named removal, so it is queued with a prediction, not landed.
- `needed_for_self_hosting`: no; by the name grep, no module of `selfhost/` or `packages/` binds a
  variadic
- `argument`: A refusal is no form, so Principle 0 is met. Robustness decides that the class closes,
  and D+S is how. D refuses what reads the `va_list` at exit 0 (`open` creating `-r--r-x---`,
  `sqlite3_mprintf` printing `-1845483673`). S refuses the forwarding door that dies at 134. Both
  messages carry their repair. S refuses defect 2036's variable format, a correct program; it is a
  known cost by the 2026-10-13 precedent, with clang's one-edit fix. D is also the only route under
  which a 27-token sentence describes the whole class truthfully. That sentence buys predictability
  alone, which is unmeasured, so it waits.
- `prediction`: with D+S landed, (1) the spec reads 8167 / 8314; (2) `q2/openc.hero` and
  `q2/sqlite.hero` exit 1, each with a note drafting a door; (3) `q2/door.hero` exits 1 at
  `door.h:2`; (4) `q2/own.hero` exits 0 and prints `%n%s%s%s`. Scored at the landing's gate by
  `heroes build` and `heroes measure`. For W0, registered as an observation unless the author funds
  the blind run: given a task needing `open` with `O_CREAT`, at most 3 of 10 blind readers given W0
  bind `open` directly, against at least 7 of 10 without it. The instrument is panel 217's launcher.
- `condition`: the blind run scoring W0's prediction, which would make W0 owed; or a tracked program
  that D refuses with no door able to replace it.

## Q3, defect 2448: a header and a library of two releases

**Run** (`q3/`):
- `abs_h.hero`, `extern "/opt/homebrew/opt/curl/include/curl/curl.h" link "curl"`: exit 1,
  `machine_locked_path`, *"set `CPATH` to the directory holding it, or name its library with
  `pkgconfig` and let `pkg-config` answer"*. That is `selfhost/head_names.hero:222`; the shared
  brief's `cli/head_names.hero:222` names no file.
- `abs_l.hero`, `link "/opt/homebrew/opt/curl/lib/libcurl.dylib"`: exit 1, `unwritable_name`, *"pass
  `--library <dir>` with the directory holding it"*. `machine_locked_path` still reaches a library
  name such as `:libm.a` (`head_names.hero:413`), so design.md:3134-3135 stays true, only narrower.

**A measured wrong value: none found.**
- The candidate I tried is `mouse.hero`, over ncurses: the SDK's `curses.h` is 6.0, `MOUSE_VERSION`
  1, `mmask_t` `unsigned long`; the keg's is 6.6, `MOUSE_VERSION` 2, `mmask_t` `unsigned`. Plain, it
  printed `128`, `1`, `ncurses 6.0.20150808`. Under `CPATH=/opt/homebrew/opt/ncurses/include` it is
  **refused at the base**, exit 1, `shadowed_header`, because the keg's `ncursesw/unctrl.h` includes
  `<curses.h>`. So the brief's "every build at exit 0, both directions" is curl's, not every keg's.
- curl's `curl_version_info_data` gains `rtmp_version` in the keg's header (`CURLVERSION_TWELFTH`,
  8.8.0), and the SDK's stops at `feature_names`. A group cannot read a field through the pointer
  `curl_version_info` returns, and a door including `<curl/curl.h>` under `CPATH` is refused, so I
  found no route to read it.

**The spec**: every route +0:
- The program's text is identical in a paired build and a mismatched one; the difference is the
  build's environment and flags.
- The spec names neither, and no sentence of it could let a reader predict the outcome from the
  program.
- § 13's *"`link` a library when the symbols need one"* and *"`pkgconfig` … asks the system where
  its headers and libraries are"* stay true under every route.
- P3 needs one caution: it would make `link` consult `pkg-config` in some builds. That blurs § 13's
  *"Instead of `link`"* contrast without making it false.

**The constraint the routes must meet, which no brief names**: design.md:4336-4337, *"this language
has no warning level: a diagnostic is exit 1 or nothing, so 'just warn' is not available"*. A pairing
rule (P1, P2, P3) can refuse or stay silent. It cannot *tell* at exit 0 without making :4336 false.

**A question rather than a premise.** A symmetric P1 refuses the SDK's 8.7.1 header with the keg's
8.23.0 library (the critic's last two rows). An older header against a newer library of the same
soname is what an OS update does at run time, and curl promises it. Refusing it may be refusing a
correct program. That is unrun here; it is the ffi-pragmatist's to measure.

- `verdict`: approve P4 whatever the ruling; approve a refusal route (P1 or P3) at +0 only as a
  refusal; object to any route that tells at exit 0; object to P2, which asks the world (Nix,
  unrun) rather than the value
- `section`: design.md §1.6 (+0); :4336-4337; §4.19 :3186-3196; :3124-3136
- `spec_token_delta`: +0 under every route, 8167 / 8314 (measured base; no sentence owed)
- `removal`: nothing, and none is owed at +0
- `needed_for_self_hosting`: no
- `argument`: The mismatch lives in the build's environment, which the specification does not name,
  and no route changes a `.hero` program's text or meaning. A sentence would only teach the reader
  about `CPATH`, which is outside the language. The binding constraint is design.md:4336: no
  warning level, so each pairing route either refuses or says nothing. P4 repairs a note that leads
  into the defect; panel 243's critic and this sitting's measured readline through it. The class
  line still rests on an inferred harm: I found no wrong value. The one candidate I tried, ncurses,
  is already refused at the base.
- `prediction`: under any adopted Q3 route, `heroes measure spec/heroes-spec.md` reads 8167 / 8314 at
  the landing, and `q3/mouse.hero` under that `CPATH` stays exit 1. Scored at the landing's gate.
- `condition`: a seat's measured wrong value or memory fault from a mismatch would strengthen a
  refusal, not move the spec. A route that changes what `link` means in the program's text would owe
  § 13 a sentence.

## Which design.md sentences each route makes false

The sentences, quoted with their lines:
- **A3** (:3939-3941): *"Every argument passed by value past `...` is therefore refused … and crosses
  through a header of the program's own"*.
- **A1** (:3943-3946): *"`-Wformat-nonliteral` kept an error in every group header's region, as
  `-Wformat` is, so a door forwarding its format with data arguments is refused"*.
- **A2** (:3946-3950): *"a door forwarding a format with none, `say(f) { return printf(f); }`, builds
  at exit 0 … `-Wformat-security` being ignored in the region since defect 2036"*.
- **B1**, panel 208's R1 outcome: *"a name its header marks deprecated binds and builds silent"*
  (`208-*.md:93`; design.md:2676-2677, *"… and this language has no warning level"*).
- **B2**, R1's mechanism: *"ignored over the whole unit after the groups' close, nothing else widened
  (panel 205's R2 region and R1's raised checks unchanged)"* (`208-*.md:96-98`).
- **B3** (:4336-4337): *"a diagnostic is exit 1 or nothing"*.
- **C1**, §4.19's reach paragraph, as R2 and R3 amended it (:3169-3186): *"a platform folder holds a
  file of its name and the folder is not one a `pkg-config` answer of the build names for a library
  … the header a group names is the program's own"*.
- **C2** (:3186-3196): *"The environment is a named folder … every other edit, a linker's or a
  warning's, goes through. Clearing `CPATH` for clang is refused … the compiler's own
  `machine_locked_path` note sends a GNU readline binding there"*.
- **D1** (:3124-3126): *"the search path is the machine's own `CPATH`, `LIBRARY_PATH` or
  `PKG_CONFIG_PATH` — all three measured live"*.
- **D2** (:3128-3136): `LIBRARY_PATH` is false on Windows, *"So the route `machine_locked_path`
  names for a library is `--library <dir>`"*.

| route | made false | stays true |
|---|---|---|
| Q1 keep | B1 for `sprintf`, reachable only through a door since A3; B3 | B2, A1 |
| Q1 lane's three words | B2 (*"nothing else widened"*, *"205's R2 region … unchanged"*); B3 for five words, `#warning` and `#pragma message` | B1, A1 |
| Q1 (a) `-Weverything` ignored, errors raised again | B2 | B1, B3, A1; but `1 << 40` silent at exit 0 |
| Q1 (b) undefined-behaviour words errors, advice silent | B2, both ways | B1, B3, A1 |
| Q1 (c) system-header mode | **A1**, measured: the raised errors stop firing; B2 | B1; B3 only by silence |
| Q2 keep | none; 2465 stays open | A1, A2, A3 |
| Q2 D | none: A3 refuses what passes past `...`, and D adds the shape with nothing past it | A1, A2, A3 |
| Q2 S | **A2** (the door is refused; *"ignored in the region since defect 2036"* ends), the case `run/fixedbugs-2036-…-format-held-in-a-variable-builds` | A1, A3 |
| Q3 P1 as a refusal | C2's remedy: without P4, the note's own `CPATH` route is then refused, a false remedy; D1 becomes incomplete, no longer false | B3, C1, D2 |
| Q3 P1, P2 or P3 told at exit 0 | **B3** | (not admissible) |
| Q3 P2 | none quoted; its premise is about the world | all |
| Q3 P3 | none outright. A question: C1's *"one a `pkg-config` answer of the build names"* would reach a `link` group's folder, and may make the critic's item 19 door build (unrun) | B3, C2, D1, D2 |
| Q3 P4 | none | all |
| Q3 record only | none; 2448 stays open | all |

## Found beside

1. **A note cites spec § 13 for a clause § 13 does not hold** (a question, not filed).
   `ffi_parameter_type`'s note on `pnum.hero` says *"spec § 13: a parameter is declared at the
   header's own width and sign, and clang holds it to them; past a header's `...` the header
   declares no type …"*. The clause after the semicolon is not in § 13 (the grep above). Either the
   note's citation stops at the semicolon, or W0 lands.
2. The shared brief's `cli/head_names.hero:222` is `selfhost/head_names.hero:222`.
3. ncurses' keg under `CPATH` is refused at the base, `shadowed_header`, so 2448's exit 0 is curl's
   and readline's, not every keg's.
4. A library path with `/` is refused as `unwritable_name` before `machine_locked_path` can read it;
   design.md:3134-3135 stays true through `:libm.a`.

## What I did not run

- No `--refresh`; every delta is a vendored lower bound.
- No `spec` suite on W0 to W3.
- No P1, P2 or P3 built.
- Linux and Windows: every platform sentence here is this Mac's.
- No census of tracked headers counting every warning word (Q1's re-run is the compiler-engineer's).
