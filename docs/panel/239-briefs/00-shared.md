# Panel 239, what every seat reads first

The sitting hears defects 2363 and 2364 together, on the author's request relayed by the coordinator
of batch 26 (`proposal.md` beside this file). Panel 238 (ratified 2026-11-08 by the author's reading;
its decision issue 2362; `docs/panel/238-the-use-lines-are-one-block-in-one-order-which-the-compiler-walks-too-and-the-repositorys-static-index-stands-at-a-root-packages.md`)
ruled in its R3 that its formatter rule (R1) and the compiler walking modules by one sorted key (R2)
land in the round that repairs these two, and that each repair needs a sitting. **Full panel**: a new
refusal is a diagnostic class (CLAUDE.md § 4). Seats: compiler-engineer, ffi-pragmatist,
spec-warden, historian; a completeness critic over these briefs before any seat starts and over the
reports after. **The llm-ergonomist's blind seat is a paid `claude -p` session**: the author allowed
10 USD more for paid runs at panel 237's ratification (`git show 5d9e832b2`, 2026-11-08), and this
sitting's coordinator was told to start no paid run of any kind and to ask the author once; the
author has not been asked yet. So the seat does not sit now: its brief and its size are written at
the synthesis and marked unrun. **Repaired on the critic's first pass** (`239-reports/critic-briefs.md`,
before any seat started): question 1's criterion said against route (A) alone, five shapes added,
line 131 of the specification, the trace's other words, the trace on the author's screen, three
routes nobody listed.

## The tree, frozen

- Every seat measures the tree of lane **`p239`**: branch `p239` at **`0a62bb10c`** (*Merge branch
  'p238' into b25-round*, `git log -1 --oneline`), plus the sitting's own commits, which touch
  `docs/panel/239-*` alone. `git status --short` read clean when this brief was written.
- Each seat works in **its own detached worktree**, `.claude/worktrees/239-<seat>/`, made by the
  coordinator from `p239` after these briefs are committed, and nowhere else: never in `p239`, the
  trunk, a lane, another seat's worktree, or `p239-scratch` (which you read and never write). A
  `build/` you make is yours; put your work under `build/239/` in your worktree.
- **Build your own compiler**: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then
  `./heroes build selfhost/main.hero -o heroes-next` in its own command; judge with `./heroes-next`.
  Measured in `p239` while this brief was written (`p239-scratch/stage2.log`, `/usr/bin/time -l`):
  stage 2 exit 0, **737,386,717,036 instructions retired**, `real 216.35`, `user 130.36`, `sys 28.64`
  at a load of about 15 (`uptime`); `./heroes-next --version`'s second line **`sources
  3d44313d62fb67a3`**. Built the same way in the `heroes-linux-arm64:latest` container from `git
  ls-files seed runtime selfhost` (`p239-scratch/linux/build.sh`, `build.out`): the same key,
  **GNU ld (GNU Binutils for Debian) 2.44**, **Debian clang 22.1.8**. This Mac: `ld -v`
  **ld-27106.1**, **Apple clang 21.0.0**.
- The machine is shared with batch 26's lanes (`b26-fmt`, `b26-index`, `b26-link`, `git worktree
  list`). **Measure a cost in instructions retired** (`/usr/bin/time -l`), never in seconds; run at
  most three processes at once.
- **Linux arm64**: tar what you send from `git ls-files` with `COPYFILE_DISABLE=1 tar
  --no-mac-metadata --no-xattrs`, never a copy that carries `.claude/worktrees`; mount your own
  worktree's `build/239/` and set `TMPDIR` under the mount, so nothing is written to the container's
  `/tmp` or the host's.

## The two defects, reproduced while this brief was written

Programs, libraries and scripts: **`.claude/worktrees/p239-scratch/repro/`** (copy what you need
into your own `build/239/`). The link programs and `two_others` are panel 238's ffi-pragmatist's,
from `docs/panel/238-reports/evidence.tar.xz` (`238-ffi-pragmatist/build/238/link/`, `units/two_others/`);
the shapes under `s2363/` and `s2364/` are the coordinator's, written for this sitting. **This Mac**:
`sh repro/mac.sh`, its output `repro/mac.out`. **Linux arm64**: `docker run --rm -v
"$PWD":/w heroes-linux-arm64:latest sh /w/linux/repro.sh` from `p239-scratch`, its output
`linux/repro.out`. Both with the compiler `sources 3d44313d62fb67a3`.

### 2363: one C name two linked libraries define

`picker.hero` binds `pick` from `a.h` with `link "a"`; `extra.hero` links `"b"` for `b_only`;
`aux.hero` links `"w"` for `w_only`. `liba.a` and `libb.a` each define `pick` in a member of its own
(`int pick(void)`, returning 1 and 2), and the same two as shared libraries; `libw.a`'s `pick` is
**`int pick(int *out)`**, which writes through its argument. `_written` and `_sorted` differ only by
the order of their two `use` lines.

| program | this Mac (ld64) | Linux arm64 (GNU ld 2.44) |
|---|---|---|
| `sig_written` (`use picker` / `use aux`) | built exit 0, prints `1 30` | built exit 0, prints `1 30` |
| `sig_sorted` (`use aux` / `use picker`) | **built exit 0, Bus error, 138** | **built exit 0, Segmentation fault, 139** |
| `dup_written`, static and shared | `1 10 20` | `1 10 20` |
| `dup_sorted`, static and shared | **`2 10 20`**, no word | **`2 10 20`**, no word |
| `dep_sorted` (an archive needing another) | `41 2` | `internal error: linking failed`, exit 2 (defect 2365's) |
| `only_top` (the needed archive on no line) | `internal error: linking failed`, exit 2 | the same (defect 2365) |

**Which library supplied a name, asked of the link** (`repro/link/c-trace.sh`, in C, `-la -lw`
against `-lw -la`): GNU ld's `-Wl,--trace-symbol=pick` prints `liba.a(pick_a.o): definition of pick`
or `libw.a(pick_w.o): definition of pick`; ld64's `-Wl,-map,<file>` lists `_pick` from `liba.a(pick_a.o)`
or `libw.a(pick_w.o)`; for the shared pair `nm -m` of the Heroes binary prints `(undefined) external
_pick (from liba)` against `(from libb)`. **The critic measured two more readings** (container,
`repro/critic/c/trace.sh`): with `liba.a` then a shared `libb.so`, GNU ld's trace prints `liba.a(pick_a.o):
definition of pick` **and `libb.so: reference to pick`**, naming the other definer under another word;
and **`ld.lld` (Debian LLD 22.1.8) is in the Linux image** and prints `libb.so: shared definition of
pick` for the same link, its `--why-extract=<file>` listing each member with the name that pulled it
(no `ld.lld` or `ld64.lld` on this Mac, `which`). No linker names an archive member it did not load.
**Where the trace goes**: GNU ld and ld64's `-why_load` write it on stderr, lld on stdout; and
`cli/link_warning.hero:37-56` (`run_link`) prints every stderr line of the link to the author but
glibc's section warnings, so a trace word added to the real link reaches the author's screen unless
the mechanism takes its lines out.

**Shapes beside, depth one, written for this sitting** (`repro/s2363/`):

- **A name the C library defines, which a linked library defines again** (`interpose.hero`):
  `printer.hero` binds `puts(s: cstr lent) -> i32` from `stdio.h` with **no** `link`; `quiet.hero`
  links `"q"` for `q_only`, and `libq.a` holds a member defining `int puts(int *out)`. **Built exit 0
  and died on both platforms**: Bus error 138 on this Mac, SIGSEGV 139 on Linux arm64. `nm -m` of the
  Mac binary: `external _puts` defined in the binary itself, from `libq.a`. So the class reaches a
  group that names no library: the definition it expects is the C library's, linked implicitly.
- **A name its group's `link` does not define where it is linked** (`remath.hero`, `extern "math.h"
  link "m"` binding `sqrt`): prints `4.0` on both; on this Mac `nm -m` says `(undefined) external
  _sqrt (from libSystem)`, and the SDK's `usr/lib/libm.tbd` is a symbolic link to `libSystem.tbd`
  (`ls -la`).
- **glibc 2.41 in the container** (`linux/repro.out`, `linux/pthread.out`): `nm -D --defined-only`
  finds `ldexp`, `frexp`, `scalbn`, `modf`, `copysign` and `finite` defined in **both**
  `libc.so.6` and `libm.so.6`, `sqrt` in `libm.so.6` alone; `pthread_create` in `libc.so.6` (two
  versioned entries) and **not** in `libpthread.so.0`, and `libpthread.a` is an empty archive of 8
  bytes (`!<arch>\n`). A C program linked `-lpthread -Wl,--trace-symbol=pthread_create` prints
  `libc.so.6: definition of pthread_create`. `libm.so` is a linker script (`GROUP (
  /lib/aarch64-linux-gnu/libm.so.6 AS_NEEDED ( .../libmvec.so.1 ) )`). **GNU ld's trace names one
  definition, the one taken**: `ldexp` linked `-lm` prints `libm.so.6: definition of ldexp` and linked
  `-lc -lm` prints `libc.so.6: definition of ldexp`, one definition line each, though both libraries
  define it.
- **The duplicate in the same member as a name the program needs from the other library** (the
  critic, `repro/critic/s2363x/`: `libb2.a` holds one member, `b2.o`, defining `pick` and `b_only`;
  `extra2.hero` links `"b2"` for `b_only`; `one_written` is `use picker` / `use extra2`, `one_sorted`
  the swap). **GNU ld**: `one_written` exit 2, `internal error: linking failed:` over `multiple
  definition of 'pick'`, the author's duplicate blamed on the compiler (2365's failure path met by a
  duplicate); `one_sorted` built exit 0, `2 20`, `libb2`'s `pick`, silent. **ld64**: both built exit
  0, `2 20`, so `liba`'s `pick` is reachable by no order; in C, renaming `b_only` to `z_only` turns
  `-la -lz2` into ld64's `duplicate symbol '_pick'`, so on ld64 the names' spelling decides as well as
  the line's order (the critic's inference from two runs that ld64 walks undefined names in name
  order; unmeasured further). A weak `pick` in a member loaded for `b_only`: GNU ld takes `liba`'s
  strong one or the weak one by order, ld64 the weak one in both orders (`repro/critic/c/trace.sh`).
- **A name the runtime calls, which a linked library defines** (the critic, `runtime_name.hero`:
  `loud.hero` links `"r"` for `r_only`, `libr.a` holds a member defining `int printf(const char *f,
  ...) { return 0; }`, no group binds `printf`): **built exit 0 on both platforms and prints two empty
  lines** where it should print `30` and `7`; the runtime's `printf` replaced. The runtime calls C
  library names itself (`grep -rohE '\b(malloc|free|calloc|realloc|write|fwrite|printf|snprintf|memcpy)\('
  runtime/ | sort | uniq -c`: `memcpy` 51, `snprintf` 30, `write` 19, `free` 11, `fwrite` 5, `printf`
  2, `malloc` 1, `calloc` 1).
- **The correct program on the other side: a library linked to replace a C library name on purpose**
  (an allocator replacing `malloc` and `free`, a profiler, a test double), panel 238's own open
  question for this sitting (`238-reports/ffi-pragmatist.md:334-335`). None is installed on either
  machine (`ls /opt/homebrew/lib`, the container's `/usr/lib/aarch64-linux-gnu`, `pkg-config
  --list-all`, grepped for `mimalloc`, `jemalloc`, `tcmalloc`: nothing), so a seat writes one.
- Tracked `link` names (`git grep -h -oE 'link "[^"]*"' -- '*.hero' | sort | uniq -c`): `sqlite3` 36,
  `m` 20, `app` 7, `z` 5, `pthread` 4, the rest one or two each, most of them fixtures of refused
  names.

### 2364: two other modules' records in one module's unit

`pw.h` defines `TWO_OTHERS_WIDE` and `Pw`; `rs.h` lays `Rs` out `{ long long a; int b; }` under
that macro and `{ int a; int b; }` without it; `rs.hero` binds `Rs` with `a: i32`, as `rs.h` read
alone says; the root (no group of its own) calls `pw.make` and `rs.make` and reads both records'
fields.

| program | both platforms, the same |
|---|---|
| `rs_first.hero` (`use rs` / `use pw`) | `9`, exit 0 |
| `pw_first.hero` (`use pw` / `use rs`) | exit 1, ``error[ffi_field_type]: `Rs.a` is `long long` in `rs.h`, a signed 64-bit integer, not `i32` ``, at `rs.hero:3:9`, `fix (guess): declare `a` with `i64`` |
| `fixguess/`, `a: i64`, both orders | exit 1, ``error[ffi_field_type]: `Rs.a` is `int` in `rs.h` ...``, `fix (guess): declare `a` with `i32``: the guess walks in a circle (this Mac by `repro/mac.sh`; Linux by the critic, `repro/critic/run2364-linux.out`, `linux/repro.sh` having no `fixguess` line) |

Where it comes from: `selfhost/emit/externs.hero:129-161` (`headers_for`): the unit's own groups'
headers first, `sort(mine)`, then `sort(others)` (line 158), the headers of other modules' group
records the unit spells, in declaration index, which is the module walk's order. The plan that asks
it is `selfhost/emit/unit_plan.hero:62-146` (`plan`, `headers_for` at line 144). Every unit asserts
each record field it spells against the binding (`selfhost/emit/extern_field.hero`); the layout check
(`selfhost/cli/layout.hero`, `check_records`, line 90) asks a unit's records *"of those headers
alone, since a record is laid out as the unit that spells it reads it"* (its comment, lines 80 to 84). **No
check compares one record's layout in two units**: `grep -rn offsetof selfhost/emit/extern_field.hero`
finds two assertions, both of a flexible or fixed array's room, none of an offset against another
unit's (a question for the compiler-engineer to settle, not a premise).

**Shapes beside, depth one, written for this sitting** (`repro/s2364/`), the same on both platforms:

- **`pack/`: the other module's header changes the record's layout and not its field types.**
  `pk.h` defines `RS_PACKED`; `rp.h` wraps `typedef struct Rp { char c; int b; } Rp;` in
  `#pragma pack(push, 1)` under that macro; `rp.hero` binds `c: i8`, `b: i32`. `rp_first.hero`
  prints `4 1 1000`. **`pk_first.hero` builds at exit 0 and prints `4 1 -402653184`**: the root's
  unit reads `Rp` packed, `rp`'s unit reads it padded, and a value crossing between them is read at
  the wrong offset. No message. This is the class's worst member, a wrong value at exit 0
  (design.md §1.12), and the same cause as 2364.
- **`own/`: a module's OWN header defines the macro that changes another module's record it spells.**
  `m.hero`'s group `m.h` defines `TWO_OTHERS_WIDE` and `m` spells `rs.Rs`: exit 1, the same false
  ``Rs.a` is `long long` in `rs.h``, at `rs.hero:3:9`, in every order (there is one root order).
- **`prefix/`: the record's own module configures it with an EARLIER header of its own.** `rs3.hero`
  binds `cfg.h` (defines `TWO_OTHERS_WIDE`) above `rs.h`, and `Rs` with `a: i64`, as `rs3`'s unit
  reads it; the root spells `Rs`: exit 1, ``Rs.a` is `int` in `rs.h` ... not `i64` ``, at
  `rs3.hero:6:9`. The root's unit reads `rs.h` without `cfg.h`.
- **`both/`: two others whose headers configure each other.** `pa.h` defines `PA_WIDE` and lays `Pa`
  out wide under `RB_WIDE`; `rb.h` the mirror. Each module's unit reads its own alone (`i32` true
  there). `pa_first` exit 1 on ``Rb.a` is `long long` in `rb.h``; `rb_first` exit 1 on ``Pa.w` is
  `long long` in `pa.h``: no order of the two works in one unit.
- **`pragma/`: a header of another module that leaves `#pragma pack(1)` set, with no macro at all**
  (the critic, `repro/critic/s2364x/pragma/`: `pk.h` ends `#pragma pack(1)`, `rp.h` is a plain
  `typedef struct Rp { char c; int b; } Rp;`). `pk_first` **exit 0, `4 1 -402653184`**, both
  platforms; `rp_first` `4 1 1000`. `emit/header_state.hero:1-23` (defect 582) gives back the state a
  header leaves set after all of a unit's group includes, never between two of them. The record's
  tokens are the same in both units and no macro is involved, so a comparison of tokens or of macros
  cannot see it.
- **`partial/`: a `partial` record whose hidden member another module's macro adds** (the critic,
  `repro/critic/s2364x/partial/`: `rx.h` lays `Rx` out `{ int a; int extra; int b; }` under
  `TWO_OTHERS_WIDE` and `{ int a; int b; }` without; `rx.hero` binds `record Rx partial` with `a`, `b`
  and `rx_sum(@r: Rx)`). `rx_first` `4 3 2 302`; **`pw_first` exit 0, `4 3 0 300`**, both platforms,
  and under `heroes build --sanitize` **`4 3 -1094795586 300`** with no sanitizer word: bytes nobody
  wrote. No field assertion can fail, since both fields are true in both layouts.
- **Two shapes the critic wrote that do not reproduce, which narrow the class**: another module's
  constant read across modules reads as its own unit reads it (`const/`, both orders `4 8 8`), and a
  C function of another module called directly is refused in both orders, `extern_across_modules`
  (`fn/`). So a unit reads another module's headers for the group records it spells.
- **Where each unit's layout already meets** (the critic, read, unrun):
  `selfhost/cli/probing.hero:236-251` runs the layout check per unit and pushes each unit's padding
  membership of each record into one list keyed by declaration alone, and
  `selfhost/emit/lend_type.hero:61-70` folds it into one map, the last unit's standing for every unit;
  `emit/padding.hero:171` reads it to zero a lent record's padding (panel 215's R1). So each unit's
  layout is computed already, and the padding writer of one unit may read another unit's layout.

## What else stands, read while writing

- **Panel 238's two diffs apply to `0a62bb10c`** (`git apply --check -p1`, both printed nothing and
  passed): R2's route, `238-compiler-engineer/build/238/prototype.diff` (803 lines, the rule and the
  route, `selfhost/use_key.hero` read by `modules.load_edited`, `modules.cycles` and
  `resolve/top.read_uses`), and R3's group words, `238-ffi-pragmatist/build/238/group/group.diff`
  (114 lines). Both are files of `docs/panel/238-reports/evidence.tar.xz`, which `tar -xJf` extracts
  into your `build/239/`. **Lane `b26-link` is implementing the group words now**, in parallel with
  this sitting (238's landing list, `.claude/worktrees/p238-scratch/landing.md`, Commit 0); `b26-fmt`
  and `b26-index` hold R1/R2 and R10. None has a commit past `0a62bb10c` (`git worktree list`).
- **Defects open beside these two** (`gh issue view`): 2365, an undefined reference inside a linked
  library told as `internal error`, exit 2 (`only_top`, `dep_sorted` on GNU ld); 2366, a `link`
  library needing a `package`'s fails on GNU ld in every order (closed by the group words); 2367,
  `adjacent`, two `use` lines binding one name told by their order. They are not this sitting's
  question, but a mechanism that reads the link may meet 2365's failure path
  (`selfhost/cli/renamed.hero`, `cli/whose.hero`).
- **The compiler starts no symbol-table reader today**: `grep -rhoE 'program: "[^"]+"' selfhost`
  names `clang` (59), `pkg-config` (5), `git`, `uname`, `xcode-select`, `env`, `date`, `curl`, `cc`
  and test fixtures, and no `nm`, `otool`, `objdump`, `readelf` or `dyld_info`.
- **The FFI codes**: 34 distinct `"ffi_..."` strings in `selfhost/`
  (`grep -rhoE '"ffi_[a-z_]+"' selfhost | sort -u | wc -l`), among them `ffi_defined_twice` (one C
  name two modules' units would define, `cli/defined_twice.hero`, 220 lines), `ffi_declared_two_ways`
  (`cli/two_ways.hero`, 300), `ffi_missing_link` (a linker name a header gives that the linker
  cannot find, `cli/renamed.hero`, 216), `ffi_one_file` (`cli/one_file.hero`, 261), `ffi_field_type`.
  `.claude/rules/c-boundary.md` § *A clang failure that the author's own extern caused* lists twelve
  members.
- **The specification** (`./heroes-next measure spec/heroes-spec.md`): `claude-legacy 8178`,
  `cl100k_base 8325`, `real 10900` (`claude-opus-5`, 2026-11-08), headroom 5484. § 13 at line 375:
  *"A group names its header, and `link` a library when the symbols need one; C reads a module's
  headers in the order its groups are written: one that needs another's names comes after it, and
  one defining `_GNU_SOURCE` first. clang checks every result type, constant and record field
  against that header"*. § 13 runs from line 373 to the file's end, 490 (`wc -l`). No sentence orders
  the libraries (`grep -n -i '\border\b\|\blink\b'`), and none speaks of another module's record in a
  unit; **but line 131 rules on order**: *"Declaration order never matters but a group's (section 13);
  mutual recursion needs no forward declarations."*, and § 1 (lines 22 to 31) says what a `use`
  binds and nothing of its order. Whether a `use` line is a declaration in line 131's sense decides
  whether the document calls `pw_first`, `dup_sorted` and `sig_sorted` programs whose meaning the
  order of two lines may not change: the spec-warden's to read.
- **Panel 204** (`docs/panel/204-c-reads-a-module-s-headers-in-the-order-its-groups-are-written-and-the-spec-says-so.md`)
  ruled a module's own headers in written order (R1), refused a macro two headers define two ways
  (R2), and told a refused header the order that compiles (R3, `selfhost/cli/header_order.hero`, 342
  lines, `order_trial.hero`, 179); it refused a byte-sorted order and per-group units (R4). **Panel
  202** cut every verb's C by module (R1) and compares one external declaration two modules bind
  (C1, `ffi_declared_two_ways`) and one definition two units would hold (C2, `ffi_defined_twice`).
- **Where a case can live**: `git ls-files tests/golden | grep -E '\.c$'` prints nothing, so no
  golden case builds a C library; the harness builds one in one place, `tests/harness/shell.hero:2332`
  (a Windows DLL for a test of its own).

## The questions

1. **2363, the mechanism.** Which closes the class, a call reaching another definition than the one
   the author's group names, a memory fault at exit 0 among its outcomes, on both platforms, with no
   correct program refused, **and with its verdict free of the order** (the walk's today, R2's key
   after; a mechanism that accepts `dup_written` and refuses `dup_sorted` lets a module's name decide
   whether a program builds):
   - **(A)** the definition the link took, compared with the library the binding's group names: what
     *the group's library* is for a group with no `link` (`interpose`), a re-export (`remath` on this
     Mac), glibc's `pthread_create` and the names libc and libm both define;
   - **(B)** every library on the line that defines a bound name, whatever the linker took: how the
     definitions an archive did not load and a shared library the trace does not name are read, and
     what it refuses among glibc's and the platform's own pairs;
   - **(C)** the compiler orders the link line so that each bound name resolves to its group's
     library, refused where no order does;
   - **(D)** a whole-archive probe: a second link with every archive loaded whole
     (`-Wl,--whole-archive` on GNU ld, `-Wl,-all_load` on ld64), where the linker itself refuses every
     duplicate among archive members in both orders, naming both (the critic, in C); it sees no shared
     library and no C library name, and refuses names no group binds unless filtered;
   - **(E)** *the group's library* defined by a probe link of the group's own `link` and `package`
     words alone, the platform's defaults with them, compared with the real link (the critic, unrun):
     it answers (A)'s question for `pthread_create`, `sqrt` and `puts`, and alone inherits (A)'s order;
   - **(F)** the link's own refusal where it already happens (`one_written` on GNU ld), told on the
     binding instead of `internal error`: whatever closes the class owes it, since (A) to (C) assume
     the link succeeds;
   - **(G)** a route this list does not name. What would have to be true for it to exist.
   **The criterion rules out (A) alone, and (A) is panel 238's direction** (`proposal.md`): it accepts
   `dup_written` and refuses `dup_sorted` by construction. It is admissible only joined to a route that
   fixes the answer for both orders ((B), (C) or (D)); a seat that judges the criterion wrong says so
   with the program that shows it. (C) has no order to choose on ld64 where the duplicate shares a
   member with a needed name (`one_*`).
   For each: the words or tools it needs (the compiler starts no symbol reader today), what it costs
   a build in instructions, on which platform it is unrun, and what Windows' `lld-link` would need.
2. **2363, the words.** The code (a new one, or an existing one widened), the message, the notes and
   the fix tagged `certain` or `guess`, on which line it is told, and **what a program that needs
   both libraries can do** (after R2 no `use` order is a lever). design.md §4.17: everything needed
   to fix the mistake without opening another file.
3. **2363's reach.** A name no group binds that two linked libraries define and a third needs (a
   library's own internal reference): reached by the robust reading, or a question for later, said
   why. **A name no group binds that the runtime calls and a linked library defines** (`printf`, the
   critic's `runtime_name`, measured silent), and **a library linked on purpose to replace one**
   (an allocator): what the robust reading does with each, and how it tells the second from the
   first. A name a library defines and the program's own objects define too (a header's external
   definition: `ffi_defined_twice` covers two units).
4. **2364, the mechanism.** Which closes `pw_first`, `own`, `prefix`, `both` and the three silent
   members, `pack/pk_first`, `pragma/pk_first` and `partial/pw_first`, with no correct program
   refused:
   - **(A)** the order search over the other modules' headers a unit reads (panel 238's critic,
     second pass, finding 7), one clang probe per order tried;
   - **(B)** a check that every group record a unit spells has, in that unit, the layout its own
     module's unit gives it (size, alignment, each member's offset and type), told where it differs;
   - **(C)** the owner's earlier headers read before its record's (the owner's own unit's prefix);
   - **(D)** a unit that reads another module's record without any header beside it that the
     owner's unit does not read;
   - **(E)** another.
   Their cost on a program that builds and on the failure path, in instructions; and which of them
   the silent members need, since a field-type assertion cannot see them, and a comparison of the
   record's tokens or of the headers' macros cannot see `pragma` (the critic: only a comparison of the
   layout, or a unit that reads no foreign header before the record's, does).
5. **2364, the words where no order works** (`both`): code, message, notes, fix; whether
   `ffi_field_type`'s *"in `rb.h`"* can be made true or another code is owed.
6. **The order of landing.** Whether each repair depends on R2's walk by key, stands alone, or must
   land before it; how each meets R3's group words on GNU ld (inside one `--start-group` group: which
   definition binds, whether the group loads a member the bare line did not, a `multiple definition`
   where there was none, and whether the mechanism reads a grouped link the same way); and on ld64,
   which decides by the names as well as the line (`one_*`), what R2's key changes there.
7. **The cases each owes**, and in which form (`.claude/rules/verification.md` § What gates what),
   given that no golden case builds a library: on which platform each runs.
8. **The text**: a spec sentence or none (the spec-warden counts with `heroes measure`, the vendored
   tables only, no `--refresh`), design.md §4.19's, and `.claude/rules/c-boundary.md`'s member list.

## Rules every seat keeps

- **Every number in your report comes from a command you ran while writing it**, the command beside
  it; a negative sentence (*no program does Y*, *the compiler has no X*) is run or written as a
  question. A figure in this brief is the coordinator's: re-run it before you rest anything on it.
- **No paid run of any kind**: no `claude -p` session, no API call, no cloud run, no `heroes measure
  --refresh`. One you find worth running goes in your report with its size, and the coordinator
  decides.
- **No credential printed**: never `gh auth token`; `gh auth status` masks it. Only `gh` reads
  (`issue view`, `issue list`, `search`), never a write.
- **No file outside `/Users/joseph/Temp/heroes/heroes-lang`**: no `/tmp`, no home directory, and in
  the container nothing outside the mount.
- **A program that may not end** is built and its binary run under `timeout`, its output to
  `/dev/null` or through `head -c 65536` into a file; never `timeout heroes run`.
- **No git command that writes** in your worktree or any other (no commit, no checkout, no stash);
  `git apply` of a diff onto a **copy** of `selfhost/` under `build/239/` is how a variant is built.
- **Write your report as you go** to `docs/panel/239-reports/<seat>.md` **in your own worktree**
  (the coordinator copies it into the lane), so a seat stopped early leaves what it had.
- Your report ends with: your verdict (approve, object, or veto with its condition), the design.md
  section it rests on, the cost or delta you measured, **one falsifiable prediction** with the run or
  milestone that scores it, and the condition that would change your verdict.
- No em dashes.
