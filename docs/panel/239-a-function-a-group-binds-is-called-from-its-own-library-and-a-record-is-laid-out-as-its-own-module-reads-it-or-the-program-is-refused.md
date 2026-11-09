# Panel 239: a function a group binds is called from its own library, and a record is laid out as its own module reads it, or the program is refused

2026-11-08 to 09, convened by the coordinator of batch 26 on the author's request that defects 2363
and 2364 be heard in one sitting, under M-package-manager, whose panel gate was asked once at panel
235. Panel 238 (ratified 2026-11-08 by the author's reading; decision 2362) ruled in its R3 that its
formatter rule (R1) and the compiler walking modules by one sorted key (R2) land in the round that
repairs these two, and that each repair needs a sitting. **Full panel**: a new refusal is a
diagnostic class. **Seats**: the compiler-engineer, who built 2364's route; the ffi-pragmatist, who
built 2363's refusal; the spec-warden, on the vendored tables; the historian. A completeness critic
read the briefs before any seat started (eleven findings that changed a brief, repaired at
`1c4bc5197`) and the reports after (nine findings that changed the resolution), and the
compiler-engineer, the ffi-pragmatist and the spec-warden ran again on its second pass, the first
two building a second version each. **No paid run** of any kind: the llm-ergonomist's blind seat did
not sit; its brief and size are in `239-briefs/blind/`, unrun (process notes). **Provisional, author
ratification pending**, asked by the decision issue drafted at the sitting
(`.claude/worktrees/p239-scratch/decision-239.md`, not yet filed). Every `239-<seat>/build/239/`,
`239-critic/build/239/` and `p239-scratch/` path below is a file of `239-reports/evidence.tar.xz`
(`239-reports/evidence.md`).

## The proposal, verbatim (`239-briefs/proposal.md`)

1. **Defect 2363** (`blocking`, https://github.com/heroes-lang/heroes/issues/2363): one C name two
   linked libraries define binds whichever stands first on the link line, unrefused; with another
   signature the program builds at exit 0 and dies by a memory fault. Panel 238's provisional
   direction: **refuse a name a group binds that the link took from another library than its
   group's**, read from the link itself.
2. **Defect 2364** (`blocking`, https://github.com/heroes-lang/heroes/issues/2364): two other
   modules' records read in one module's unit follow the walk's order, and a correct program is
   refused with a false message whose fix leads nowhere. Panel 238's provisional direction: **the
   compiler decides the order of the other modules' headers a unit reads** (panel 204's order
   search), with a true refusal where no order works.

## What the sitting found first: both classes are wider than their defects, and four members are silent

Every row measured on this Mac (ld64 `ld-27106.1`, Apple clang 21.0.0) and in the Linux arm64
container (GNU ld 2.44, lld 22.1.8, Debian clang 22.1.8, glibc 2.41) with the trunk's compiler,
`sources 3d44313d62fb67a3`, by the briefs (`p239-scratch/repro/mac.out`, `linux/repro.out`), the
critic (`239-critic/build/239/`) or the seat named, and reproduced by the critic's second pass:

| shape | today | found by |
|---|---|---|
| `sig_sorted`: `pick` bound from `a.h` with `link "a"`, a library linked for another name defines `int pick(int *)` | **built exit 0, Bus error 138 / SIGSEGV 139** | defect 2363 |
| `dup_sorted`, static and shared | built, `2 10 20` where `dup_written` prints `1 10 20`, no word | 2363 |
| `interpose`: `puts` bound from `stdio.h` with no `link`, a linked archive defines `puts(int *)` | **built exit 0, Bus error / SIGSEGV** | the briefs |
| `runtime_name`: a linked archive defines `printf`, which the runtime calls and no group binds | **built exit 0, two empty lines** | the critic |
| `one_*`: the duplicate in the file loaded for the other library's needed name | GNU ld `internal error` exit 2 one way, `2 20` silent the other; ld64 `2 20` both ways | the critic |
| `ab_*`: the group's own library wins in both orders on ld64, and another library's own call to `pick` reaches it | **ld64 `1 10 10` both orders for `1 10 20`**; GNU ld silent one way, exit 2 the other | the critic, second pass (V1) |
| `self_*`: a library calls its own `pick` | `1 10` or `2 20`, never `1 20`: no link realises the program | the critic, second pass (C1) |
| `intern_*`: a name no group binds, defined by two libraries, needed by a third | ld64 `100 2 7` both orders (`41` meant); GNU ld exit 2 one way, silent the other | the ffi-pragmatist |
| `hero_name`: a library defines `hero_print_end`, a name the runtime defines | exit 2, `internal error` | the ffi-pragmatist |
| `pw_first`: two other modules' records in the root's unit | exit 1, a false `ffi_field_type` whose `guess` walks in a circle | defect 2364 |
| `own`: a module's own header configures another's record it spells | exit 1, the same false message | the briefs |
| `prefix`: the owner configures its record with an earlier header of its own | exit 1, false | the briefs |
| `both`: two others whose headers configure each other | exit 1 both orders, each on the other record | the briefs |
| `pack/pk_first`: another module's macro packs the record | **built exit 0, `4 1 -402653184`** | the briefs |
| `pragma/pk_first`: another module's header leaves `#pragma pack(1)` set | **built exit 0, `4 1 -402653184`** | the critic |
| `partial/pw_first`: another module's macro adds a hidden member to a `partial` record | **built exit 0, `4 3 0 300`; under `--sanitize` `-1094795586`, bytes nobody wrote** | the critic |
| `union2/pw_first`: a union inside a record, a field after it | **built exit 0, `4 3 0`** | the compiler-engineer |
| `packlib`: `pack` with the record's function in a library, through `--emit-c` | **the one file, compiled, `4 1 -402653184`** | the compiler-engineer |

**The specification already gives every one of these programs one meaning** (the spec-warden, § 3):
a group's function is its library's, or the C library's where it names none (line 375), a group's
record is its header's struct as its own module reads it (lines 376 and 404), and neither the order
of two `use` lines nor a module's name carries meaning (lines 28, 57, 164; panel 238's R7 and R3).
Both defects are the compiler's against the document (CLAUDE.md § 12). **And no part of C sees
either class**: two units reading one `struct` two ways compile, link and run silently under `-Wall
-Wextra -pedantic`, LTO and ASan with UBSan (the ffi-pragmatist's § 9; C11 6.2.7 compares tags,
members and types, and `#pragma pack` is outside it); every linker read binds the first definition
and refuses only two loaded members that collide (the historian's § 1).

## The resolution, provisional

**R1. 2363: a C name a group binds, or the runtime needs, that a library the program names other
than the one meant would supply, is refused at exit 1, `ffi_defined_elsewhere`, in every order of
the modules: the ffi-pragmatist's v6, built and measured on both platforms**
(`239-ffi-pragmatist/build/239/route6/`, `sources 30f949d859f52b96` on this Mac, `11a70e0569b6f6e2`
for v6b built the same in the container; `route6/diff/route6.diff`):

1. **The real link is the first reading**: its words gain `-Wl,--trace-symbol=<name>` for every
   bound name and every name the runtime needs (GNU ld, lld) or `-Wl,-map,<file>` written under
   `build/` and removed after reading (ld64, whose map names the file of every symbol it places or
   imports, a dylib's stub included); `run_link` returns what the link said and prints it without
   the trace's lines; a refusal removes the private stage before anything is published.
2. **Where the program names two or more libraries, a probe link with their `-l` words reversed in
   place** (every other word where it stood, R3's group words included), so a definition the first
   order did not reach is reached in the other; and **where a library file of objects is named, a
   probe with every such file loaded whole** (`-Wl,--whole-archive` on GNU ld and lld, `-Wl,-all_load`
   on ld64, unresolved names allowed), whose duplicates the linker itself names with both files:
   this is what sees `ab_*`, a definition no order loads (route D, the critic's). A probe the linker
   refuses on a duplicate of a bound name is that probe's answer (route F).
3. **What is refused**: a name a group binds, supplied in either reading by a library the program
   names that the binding's group does not name; for a group naming no library, a name the C
   library defines that a library the program names defines too (interposition, judged by a third
   probe with no library of the program's only where needed); a name the runtime needs (asked once
   per runtime object by linking it alone, 142 names and `main` on this Mac, cached) or defines, which
   a library the program names supplies, **unless a group of that library binds the name**: the
   author's own binding says the replacement is meant, with no new form. **What is never judged**: a
   library the program does not name, so `link "m"` (ld64's `libsystem_m.tbd`), `link "pthread"`
   (glibc's `libc.so.6` behind an empty `libpthread.a`) and glibc's pairs (`ldexp` in libc and libm)
   pass with no list of the platform's libraries.

**Measured** (the ffi-pragmatist's §§ 4, 6 and 14; v5's half reproduced by the critic, R2 and T1):

| program | today | under R1 (v6), both platforms |
|---|---|---|
| `dup_*` static and shared, `sig_*`, `one_*`, `weak_*`, `ab_*`, `self_*` | wrong value, a crash, `internal error` or silent, by order and platform | **exit 1 in both orders, the whole message byte-identical** (`cmp`) |
| `interpose`, `runtime_name` | a crash; two empty lines | **exit 1** |
| `hero_name` | exit 2 `internal error` | **exit 1** |
| `alloc_main`, `alloc_bound` (an allocator whose group binds less than it replaces) | builds | **exit 1**, *defines `calloc`, `free`* |
| `alloc_all` (its group binds `malloc`, `calloc`, `realloc`, `free`) | `100 true` | **`100 true`**, accepted |
| `remath`, `dep_*`, the glibc pair (`pthread_self` and `ldexp` under two `link`s) | as today | as today; `dep_sorted` links on GNU ld with R3's words |
| `intern_*` | silent, or exit 2 on GNU ld one way | silent `100 2 7`, or exit 1 with its words where GNU ld refuses the link (R3 below) |
| the 25 tracked roots reaching a `link` or `package` group (SQLite, libm, libcurl, libcrypto, zlib, libuv, raylib, SDL3, libjpeg) | | **identical to today on both platforms**, with and without R3's group words |

Its own five tests pass on this Mac (960,560,935,476 instructions retired, the compiler built as a
test binary); the compiler's other tests were not run with it, and are owed by the landing.

**Cost**, every link counted, this Mac (the ffi-pragmatist's § 14.3; the critic's C3 found v5's
first figure counted the compiler's process alone): no library **+0.6%**; one library **+6.3%**, no
extra link; two shared libraries **+30%**, one extra link; two libraries of which one is a file of
objects **+51%**, two extra links. Linux arm64's cost is unmeasured (the image has no counter). The
ffi-pragmatist withdrew its own condition on the cost (*"more than one extra link per two
libraries"*) on CLAUDE.md § Precedence (robustness, rank 3, over *never slow the compiler down*,
rank 5: *a guard that closes a corruption class lands, with its cost measured and reported*). **The
landing owes the cost's repair**: a cache of the probes' verdict keyed by the link's words **and the
digest of every library file the link opened** (ld64's map lists them, `-t` on GNU ld and lld), so a
warm build skips both probes; never a key of the words alone, since a rebuilt library changes the
answer under the same words (`cli/link.hero:22-24`). Unbuilt, its cost unmeasured.

**Why not the others** (the ffi-pragmatist's § 2, the critic's C1 and V1): **(A) alone, panel 238's
direction, is vetoed** by the ffi-pragmatist: its verdict follows the order (`dup_written` accepted,
`dup_sorted` refused), and read strictly it refuses every resolved `link "m"` on this Mac and `link
"pthread"` on both. **(C), choosing the order**, has no order to choose on ld64 for `one_*`, `weak_*`
and `ab_*`, and where it has one it can break the other library's own call (`self_*`: never `1 20`).
**(D) alone** sees no shared library and no C library name. **(B), a reader of every library's
symbols**, needs a tool the compiler has never started (`nm`, a third tool on Windows) and was not
built; its one remaining reach is a shared library's second definition of a name no group binds (R3).

**R2. 2363's words: the specification's**, as the ffi-pragmatist's v6 prints them (its § 14.4) and
held to the spec-warden's reference words (its § 12c): a library by its `link` name and the module
that links it, *the C library*, a file in a library by its path (`liba.a(pick_a.o)`, the one exact
name of where the definition is, never called a *member*), no *linker*, *link line* or *archive*,
and no lever of `use` order or module name; **told on the binding's line**, the one the author wrote
and the one whose meaning changed; **no `Fix`** (the repair is a library rebuilt or a `link`
removed, which no edit of the binding writes: panel 237's `shadowed_header` precedent). `sig_sorted`
and `sig_written`, one text:

```
error[ffi_defined_elsewhere]: `pick`, bound here from library `a`, is also defined by library `w`, which module `aux` links, and a program holds one `pick`: this call, or that library's own calls to it, would reach the other library's
  at picker.hero:2:5
  note: a definition is in `<path>/liba.a(pick_a.o)`
  note: a definition is in `<path>/libw.a(pick_w.o)`
  note: a program holds one definition of `pick`: link one library that defines it, or build the other library with its `pick` kept to itself (`objcopy --localize-symbol=pick` on Linux, `nmedit -R` on macOS, `-fvisibility=hidden` for a shared library)
```

The fix leads somewhere, measured in C on both platforms (the ffi-pragmatist's § 7): `libb`'s `pick`
made local (`nmedit -R`, `objcopy --localize-symbol`) or built hidden gives `pick=1` in both orders;
none of it is reachable inside the language, which is why the first remedy is *link one library*.
v5's note *"taken from the first library on the link line"* was false on ld64 (`one_written`, the
spec-warden's § 12c) and is gone from v6.

**R3. What 2363's class leaves open, ruled: `intern_*` is a known silent shape, recorded with its
reason, and filed `systemic` for the author's ruling** (the ffi-pragmatist's § 14.1, measured): a
name **no group binds and the runtime does not need**, defined by two libraries the program names
and needed by a third, is not refused, because refusing every such duplicate refuses correct static
programs whose libraries carry identical copies on purpose: **static FFmpeg, 40 names in two of
`libavcodec.a`, `libavformat.a`, `libavutil.a`; static X11, `xreallocarray` in `libX11.a` and
`libXext.a`; `libjpeg.a` with `libturbojpeg.a`, 293** (Homebrew, this Mac; Debian's static `libcurl`,
`libssl`, `libcrypto`, `libz`, `libsqlite3`, `libncurses` alone and together: none), and no symbol
tells an identical copy from `intern_*`'s two different `base_helper`s. Where the link itself refuses
it (GNU ld's written order) v6 tells it at exit 1 in the specification's words instead of `internal
error`. Also unseen by v6, said by its seat: a **shared** library's second definition of such a name
(no symbol reader; on ld64 a dylib's own calls bind to itself, on ELF they do not), and a weak
definition in a file of objects beside a strong one (unrun as a program). **Robust against
conservative here is not one axis**: refusing `intern_*` closes a silent wrong value and refuses
static FFmpeg, which design.md §1.12's other half (*"any C library must be bindable"*) forbids; the
sitting takes the ffi-pragmatist's reading and puts the other to the author (decision item 2).

**R4. 2364: every group record a unit spells is laid out there as its own module's unit lays it
out, made so by the compiler where an order of the other modules' headers can, and refused truly
where none can: the compiler-engineer's route, second version, built and measured on both
platforms** (`239-compiler-engineer/build/239/sh-agree2/`, `sources 2ed90c391b3feb46`; the first
version `sh-agree/`, `70f06afb1849e019`, reproduced by the critic):

1. **The judge is the layout (route B).** Before any unit compiles, each group record with fields
   that another module declares is read from clang's record-layout dump of the unit's header list
   (`-Xclang -fdump-record-layouts`) and compared with its owner's: every member's offset and type,
   the size, the alignment. A record read after exactly the headers its own module reads before it,
   under the same words, is the same by construction and not dumped. The dump unit opens as the real
   unit does, the header state given back after the includes, and so does the existing layout
   check's (`emit/layout_text.hero`'s `dump_unit`), which kept `Rp` at `align=1` in the root's dump
   under today's compiler (the seat's § 17d; no verdict read it, measured).
2. **What it tries before refusing (routes A and C, the composer).** The other modules' headers as
   one block per owning module, **the blocks sorted by their modules' names** (so what is tried owes
   nothing to the walk), in every order of up to four blocks, then each block preceded by its owner's
   own earlier headers in their written order; the first list under which every foreign record agrees
   becomes the unit's header list. **No block is ever put before the unit's own headers**
   (`emit/externs.hero:124-128`, panel 204's R1, defect 563).
3. **`--emit-c`'s one file is one more list**: compared and composed the same way, `ffi_one_file`
   where none agrees (`packlib`: the one file written `rp.h` before `pk.h`, prints `4 1 1000`).
4. **Refused where no list agrees**: `ffi_record_two_layouts`, a new code, before any unit compiles,
   every record that disagrees in some list tried named, by name, in the words of R5.

**Measured** (the seat's §§ 6, 7, 9 and 17; the first version reproduced row by row by the critic,
R1, C6, T2; the second version's every row read the same on Linux arm64 by `diff`, the seat's 17e):

| program | today | under R4, `build` and `--emit-c` |
|---|---|---|
| `two_others/pw_first` | refused, false `ffi_field_type` | **`9`** |
| `pack/pk_first`, `pragma/pk_first` | **`4 1 -402653184` at exit 0** | **`4 1 1000`** |
| `partial/pw_first` | **`4 3 0 300` at exit 0** | **`4 3 2 302`** |
| `union2/pw_first` | **`4 3 0` at exit 0** | **`4 3 1000`** |
| `packlib/pk_first` | **`4 1 -402653184`**, `build` and the one file | **`4 1 1000`**, both |
| `nested/pw_first` (a record reached through another's field) | refused, false | **`4 7 3 2`** |
| `prefix/main` | refused, false | **`6`** (the owner's `cfg.h` read before `rs.h`) |
| `heroes test` of `pw_first` with a test | refused, false | `1 test, all passed` |
| `own/main`, `both/*` | refused, false `ffi_field_type` | refused, `ffi_record_two_layouts`, every sentence true; `both`'s two orders one text (`cmp`) |
| the libjpeg pair as Heroes modules (the critic, C2) | refused, 16 `ffi_declared_two_ways`, 14,841 bytes | refused both orders, 624 against 656 bytes, 940 bytes of words |
| the 21 tracked roots that can reach another module's group record | | verdict, output and every unit's C byte-identical; through `--emit-c` 20 the same bytes, and `run/fixedbugs-563-a-module-s-own-headers-come-before-another-module-s-record` moves from refused (`ffi_one_file`) to written, printing `7`, its `.expected` |

**Cost**, instructions retired, warm, first pair discarded (the seat's § 17g): `rs_first` **+33M
(+1.5%)**; `rs_first --emit-c` **+68M (+1.4%)**; the failure path `both/pa_first` **-1.89G (-72%)**,
since the refusal comes before any unit compiles; `selfhost/main.hero`, which declares no group
record, **+0.03% and -0.04%** for its build and **+0.11% and +0.14%** for its `--emit-c` (the seed's text), each inside the baseline's own spread (the second version, two pairs kept of three). No cache key that exists moves but the layout check's kept answers, which
go cold once (the dump's text changes), so `cache` is not owed alone.

**Why not the others** (the seat's § 10, the critic's C2): **(A) alone, panel 238's direction, is
vetoed** by the ffi-pragmatist (on libjpeg, in C, the one order that compiles reads
`jpeg_decompress_struct` at 624 bytes against the library's 656) and objected to by the
compiler-engineer and the spec-warden: its criterion, *the unit compiles*, never fires on the silent
members. **(B) alone** refuses five correct programs and its verdict follows the order (built,
`heroes-verify`). **(C) alone** sees nothing a later foreign header does. **(D), isolation**, is
impossible for `both` without a unit per record, which panel 204's R4 refused. **(A) joined to (B),
with (C)**, is what every seat that ruled on it approves; the libjpeg pair built as Heroes modules
meets the ffi-pragmatist's own condition for lifting its veto (refused in both orders).

**Not taken, said by the seat and left to a later sitting**: panel 204's ruled refusal
`tests/golden/fixedbugs/fixedbugs-563-another-module-s-record-is-read-without-its-module-s-groups`
(`ffi_header_refused`: `main` spells `holding.Holder`, whose header compiles only after `holding`'s
earlier `need.h`) is the owner's prefix exactly, and R4 read to its end would build it; the route as
built says nothing there (its dump of `main`'s list does not compile), so the ruled refusal stands
and the case is not moved. A change to a ruled refusal is a sitting's.

**R5. 2364's words: the specification's, told where the author can change something.** The
compiler-engineer's second version (its § 17c) names the size, the alignment in plain words (*at a
multiple of 8*), **the first two fields that differ** and never every field, every record that
disagrees by name, the module that reads the header and what it reads before, a note citing `spec §
13`, and the cause read as what the reader reads before the record's header **and its own module
does not** (`cause_of`, which repaired a false *"module `cb_first`'s own `stdio.h` changes `Dinfo`"*
before the seat wrote it down). **One difference with the spec-warden's reference words is settled
for the spec-warden** (its § 12c): the message is told **where the reading module spells the
record** (`m.hero:8:19` for `own`, the root's line for `both`), with a note that the binding at
the owner's line is right, never at the owner's correct declaration, since today's `fixguess`
circle is the measured cost of pointing at a correct field (both orders of `fixguess`, the briefs
and the critic's B15). The second version's `own` words (915 bytes), the landing to move their
position as this paragraph says:

```
error[ffi_record_two_layouts]: `Rs` is 16 bytes, at a multiple of 8, where module `m` reads `rs.h` after `m.h`, and 8 bytes, at a multiple of 4, where module `rs` reads it
  at rs.hero:2:12
  note: `Rs.a` is `long long` at byte 0 where module `m` reads it, and `int` at byte 0 where module `rs` does
  note: `Rs.b` is at byte 8 where module `m` reads it, and at byte 4 where module `rs` does
  note: a group's `record` is its header's struct as its own module reads it (spec § 13), and a value of `Rs` passes between module `m` and module `rs`, so both must read it alike; no order of the headers module `m` reads, nor module `rs`'s earlier headers before `rs.h`, makes them alike
  note: module `m`'s own `m.h` changes `Rs`: spell `Rs` only in modules that read it as module `rs` does, or have module `rs` read first the header that changes it, so both read it alike
```

The last note leads somewhere, measured by the critic (C7): `rs.hero` binding `m.h` above `rs.h`
is then told truly `Rs.a is long long`, and `a: i64` builds and prints `9`.

**R6. Every diagnostic speaks the specification's words, the C boundary's first** (design.md
§4.17, the spec-warden's § 7(d) and § 12c, made exact): a refusal names a group, its header, its
`link` library, a module, a function, a record, a field, a size in bytes, and what a header
defines; the header's own C words are allowed (`long long`, `#pragma pack`, a macro's name), since
spec § 13 sends the reader to the header; the machinery's are not (*unit*, *archive*, *member*,
*link line*, *linker*, *object*: each `grep -ciw` 0 over the specification); it names every record
or library in a fixed order (by name), never the walk's; and a note cites the specification by its
own section (*spec § 13*), never design.md's (*§4.19*, which a reader holding the specification
takes for its section 4). **The 49 message lines in 25 files of `selfhost/` that cite a design.md
section today** (`git grep -n -E '"[^"]*\(§[0-9]+\.[0-9]+\)[^"]*"' -- selfhost`) are another cause
than these defects, so they are filed apart, `adjacent`, and §4.17's sentence carries a dated clause
counting them until that defect's repair.

**R7. The order of landing: R4 and R1 before panel 238's R2, never after it; R2 also waits for the
author's ruling on `intern_*`.** Measured (the compiler-engineer's § 9, reproduced by the critic's
C6): **R2 alone makes `pack`, `pragma` and `partial` print the wrong value in both orders** where
today one order of two is right, and refuses `two_others` in both orders while the same program with
`pw` renamed `zz` builds; and with R2 `intern_*` turns from GNU ld's exit 2 in one order into a silent
`100 2 7` in both (the critic's I3, R2 named by elimination). The compiler-engineer's reading (its §
17i): **R2 fixes one order for every program, so wherever today's verdict differs by the written
order it can keep the loud order or remove it, and never adds one**; so R2 lands only after every
class whose verdict depends on the order has one that does not. R1 is order-free by construction
and R4 by its sorted composer (verdict free of the written order and of the names; its bytes and
words free of the written order with R2). **The four changes compose**: the critic applied R4's
first version, v5, panel 238's `prototype.diff` and `group.diff` to one copy (disjoint files), built
it on both platforms (`sources 6377104d6b4a4f68`), and every program read as each change alone
says, `both` naming `Rb` in both orders. **That combination is of the first versions** (R4's first
and v5); the second versions (R4's `sh-agree2` and v6) were not built together with R2 and R3, which
the landing's gate owes. R1 as v6 with R3's group words was built and run on Linux arm64 by its seat:
the same verdicts, and `dep_sorted` links; R4 touches no link line (the compiler-engineer's § 17i).

**R8. The cases each owes** (`.claude/rules/verification.md` § What gates what):

- **R4**: `run` goldens in a folder of modules beside their roots (as `run/fixedbugs559/` and
  `run/fixedbugs563/` are): `two_others` in both orders, `pack`, `pragma`, `partial`, `union2` in
  both, `prefix`, `nested`, each with its value, both orders pinned; `unsupported` cases `own` and
  `both` in both orders, `#~ ffi_record_two_layouts` annotated where the message stands, and one in
  `full` to pin the notes; `packlib` as a compiler test that builds a shared library with clang
  (`-shared`, no `ar`), asks `--emit-c` and runs the file; `emission`'s new blessing of
  `run/fixedbugs-563-a-module-s-own-headers-come-before-another-module-s-record` and its
  `LEAST_REFUSED` floor where the refused count stands at it.
- **R1**: no golden case builds a C library, so a check of the net's own tests or of the compiler's
  that builds the libraries with clang alone where it can (two shared libraries defining one name, a
  third defining `puts`: `dup_*`, `interpose`, both orders, the two messages equal byte for byte),
  run on this Mac and the CI's Linux legs, and on Windows where a DLL is built (the harness builds
  one at `tests/harness/shell.hero:2332`); the archive shapes (`sig_*`, `one_*`, `weak_*`, `ab_*`,
  `self_*`, `runtime_name`, `alloc_*`, `hero_name`) need `ar`, so Mac and Linux only; and the
  tracked programs with a `link` or a `package` group, unmoved, at the round's gate.
- **Windows**: both routes are unrun there; R1 asks no probe on a Windows target, so the class stays
  open on that platform, filed apart (`blocking`); R4's dump format is unread on the MSVC target.

**R9. The text.**

- **The specification: 0 tokens** (the spec-warden, §§ 5, 12a, 12b). Every refusal of a C-world
  conflict the compiler already makes is spec-silent (`ffi_declared_two_ways`, `ffi_defined_twice`,
  the macro two headers define two ways), no sentence pays under design.md §1.6 (no removal in §
  13, no thesis task with an FFI construct, and a reader cannot see from a program which library
  defines what), and every sentence that would close the class is false of v5 alone on ld64. Priced
  and not adopted, vendored: N2 +9, W2a +21, V1s +24, V1t +30, W2b +32, the ffi-pragmatist's FP1 +48,
  R1 +6, R2 +18, M3b +23, B1 +23; vetoed: N1 and V1t, each writing a wrong-value class into the
  language.
- **design.md §4.19**: a paragraph after the one that ends at line 2721 (*A module's headers are
  included in the order its groups are written*), stating the two meanings and the two refusals in the
  spec-warden's draft (its § 7(a)), filled from R1 and R4; line 2674 to 2675's *"the header of each
  record of another module's group the unit spells"* read *"..., read as that module reads them"*;
  §4.1 line 935 to 936 gains *"each record laid out as its own module's unit lays it out (§4.19)"*;
  **§4.17** the paragraph of R6.
- **`.claude/rules/c-boundary.md`** § A clang failure that the author's own extern caused: a
  thirteenth member, `ffi_defined_elsewhere`, and a fourteenth, `ffi_record_two_layouts`; the
  narrowing paragraph (*"a symbol nobody declared stays exit 2"*) widened to the runtime's names R1
  reaches through a library the program links; and its `paths:` gaining `selfhost/cli/`, where every
  member's code since panel 202 lives and both repairs will (the spec-warden's § 8, the critic's § 7).

**R10. Found beside, drafted for filing** (`p239-scratch/defects/`, each read by `issues.hero --
draft`, not filed): `intern_*`'s ruling (`systemic`, R3); Windows asked no probe (`blocking`, R8);
the 49 citations of design.md in messages (`adjacent`, R6); a library defining a runtime name told
`internal error` (`blocking`, its own cause, 2365's failure path met by a duplicate; R1 as built
closes it at exit 1, so it closes with R1's landing).

## The verdicts

| seat | verdict | section | cost or delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object** to panel 238's 2364 direction as worded; **approve** R4, built whole; no veto | §1.12, §4.19, §4.17, §1.1, §1.7 | about 600 code lines in three new `cli/` modules and about 55 elsewhere; `rs_first` +1.5%, the failure path -72% | at the landing round's gate `emission` reports exactly one program newly blessed (`run/fixedbugs-563-a-module-s-own-...`), one fewer refusal, and no blessed file changing its bytes, because of the route | veto if the landing drops the layout comparison or lets R2 land first; against its own route on a correct program the composer does not try, an ABI difference the dump does not print, or the Windows dump |
| ffi-pragmatist | **veto** on 2363's (A) alone and 2364's order search alone; **approve** R1 as v6 | §1.12, §1.11, §4.19 (design.md:606) | 610 and 215 code lines in two new modules, +103 lines in four others; +0.6% to +51% by the libraries named | under v6 on the CI's x86-64 and `macos-26` legs `ab_*` refused with byte-identical words, `alloc_all` `100 true`, `intern_sorted` `100 2 7`, `examples/sqlite` and `examples/ledger` as today | object if a CI leg or the Windows box refuses a tracked program, if `intern_*` is ruled refused with no route sparing static FFmpeg and X11, or if the probes ship without the digest-keyed cache and a tracked warm build doubles |
| spec-warden | **object**, narrowed: to 2364's (A) alone, to any 2363 sentence beside v5 alone, to v5's words; objection to refusing both orders of `dup_*` **withdrawn**; **approve 0 tokens**; **veto** N1 and V1t | §1.6, §1.2, CLAUDE.md § 12, § 2, § 11; §4.17 | 0 / 0 (8178 / 8325, real 10900 pinned); drafts +6 to +48 vendored, real unrun | **P1**: the new refusals refuse 0 tracked programs outside their own cases, scored by the full net of the landing round | a blind run showing a sentence averts the class; P1 failing; `both` unrealisable while two C files realise it; a sentence added on the ground that refusing `dup_written` refuses a correct program; words false on a platform or told at a correct line |
| historian (advisory) | **object** to both provisional directions taken alone; **approve** the order-free criterion | §1.12, §4.17 | none on the tree; 104 fetches, 68 pages read | clang's `-Wpragma-pack` sees exactly one of the three silent members | a toolchain binding a foreign name by the link's choice with order free and no order-dependent outcome over years; a compiler that searches header orders inside one unit, shipped and kept |

## Disagreements

1. **The spec-warden's *`dup_written` is a correct program the link realises* against the
   ffi-pragmatist's refusal of both orders.** The critic built `self_*` (a library calling its own
   `pick`): no order prints `1 20`, so `dup_written` is realised only because `libb`'s file happens to
   hold no reference to `pick`, a fact the compiler cannot read and the next build of `libb` changes.
   The spec-warden re-ran it, withdrew the objection (*"a narrowing asks the value, never the world"*,
   CLAUDE.md § 11) and kept 0 tokens on the precedent of every spec-silent C-world refusal.
   **Settled.**
2. **The ffi-pragmatist's veto of the order search against the compiler-engineer's composer, which
   searches orders.** The veto's ground (libjpeg's one compiling order is the wrong ABI) is against
   an order chosen by *compiles*; the composer chooses by *every record reads as its owner reads it*,
   and the critic built the libjpeg pair as Heroes modules: refused in both orders, the veto's own
   lifting condition. **Settled.**
3. **The cost of R1.** The compiler-engineer's *"the map costs nothing measurable"* counted the clang
   driver; the ffi-pragmatist's *"+5.1%"* counted the compiler's process alone; the critic counted the
   links (C3), and v6's measured total on a two-library build is +51%. The ffi-pragmatist withdrew its
   own cost condition on robustness's rank; the resolution reports the cost and owes the
   digest-keyed cache. **Recorded, robust over fast**, the conservative alternative below.
4. **Where `ffi_record_two_layouts` is told**: the owner's declaration (the compiler-engineer) or the
   reading module's spelling (the spec-warden). The second is taken (R5), on the measured cost of
   pointing at a correct line.
5. **The allocator**: the compiler-engineer would refuse every replacement of a C library name now
   and let a later sitting rule a word; the ffi-pragmatist accepts it where the replacing library's
   group binds every name it replaces, with no new word, built (`alloc_all` accepted, `alloc_main` and
   `alloc_bound` refused). The second is taken: it adds no form (the compiler-engineer's condition was
   a new word), and it keeps a C library bindable (§1.12's completeness).
6. **`intern_*`**: refused (closes a silent wrong value, refuses static FFmpeg, X11 and libjpeg with
   libturbojpeg) or recorded (keeps them bindable, leaves the shape silent where the link does not
   refuse it). R3 records it and puts the choice to the author.

## Prior art

The precedents the resolution rests on, each `verified` by the historian (`239-reports/historian.md`,
pages read 2026-11-09), each page re-read by the coordinator the same day:
C11's 6.9p5 (*"exactly one external definition"*) and 6.2.7p2 (*"compatible type; otherwise, the
behavior is undefined"*), the two rules the classes break with no diagnostic C owes; lld's own page on
`--warn-backrefs`, which calls two links that both succeed with different archives' definitions
*"even worse"* than one failing, which (A) alone would inherit; Apple's two-level namespace and
Solaris's direct bindings, which tie a reference to the library it was linked against, as R1 asks the
link; glibc's documented replacement of `malloc` by linking and mimalloc's, the correct program R1
keeps; Microsoft's LNK2005 page (*"To use both symbols, you must create a way to distinguish them"*),
which R2's remedy follows; Clang's 2023 comparison of a C record two modules define and bindgen's
layout tests, which compare a layout as R4 does; Clang's modules, which isolate a header's
preprocessor state. The historian found no compiler that searches header orders inside one unit
(its § search); D's ImportC filed such an order dependence as a bug (issue 24280). **The historian's
prediction about `-Wpragma-pack`** is falsified on its first clause (the critic, C5): the emitted unit
opens its header region with `#pragma clang diagnostic ignored "-Wpragma-pack"`, so no clang warning
reaches anyone, and nothing in the resolution rests on it.

## Predictions to score

- **compiler-engineer**: at the landing round's gate, `emission` reports exactly one program newly
  blessed (`run/fixedbugs-563-a-module-s-own-headers-come-before-another-module-s-record`), one fewer
  refusal, and no blessed file changing its bytes, because of R4. Scored by the gate's `emission` run
  and the closing commit. (Its first prediction, *"moves no blessed emission"*, was falsified in its
  letter by its own § 17f and restated.)
- **ffi-pragmatist**: under v6 on the CI's Linux x86-64 leg and on `macos-26`, `ab_written` and
  `ab_sorted` refused at exit 1 with byte-identical words, `alloc_all` `100 true`, `intern_sorted`
  `100 2 7`, `examples/sqlite/main.hero` and `examples/ledger/main.hero` printing what today's compiler
  prints. Scored by the landing lane's run on the CI's legs at the push that carries R1.
- **spec-warden**: P1, the new refusals refuse 0 tracked programs outside their own cases, scored by
  the full net of the landing round, its counts in the closing commit.
- **historian**: clang's `-Wpragma-pack` sees exactly one of the three silent members; **falsified**
  by the critic's run (C5) before the synthesis, recorded as scored.

## What the conservative resolution would have been, so the author can choose it

- **Panel 238's directions as worded** ((A) alone for each): **vetoed** by the ffi-pragmatist, so not
  options; recorded because they were the proposal.
- **R1 as v5**, one probe per order and no whole load: half the extra links, and `ab_*` silent on
  ld64 (the critic's V1).
- **R1 without the runtime's names**: `runtime_name` silent, `hero_name` at exit 2, every allocator
  accepted without its binding.
- **The probes' cost kept down by skipping the reversed probe**: refuses less, by the order.
- **`intern_*` refused** (the other side of R3): every duplicate between two libraries the program
  names refused, bound or not, static FFmpeg and X11 with it.
- **R4 without the one-file arm**: `--emit-c` writes `packlib`'s wrong program at exit 0.
- **A sentence in the specification**: the cheapest true one beside v6 is N2 (+9 vendored), objected
  to as unpaid.

## Author's verdict

Pending.

**Ratified, 2026-11-09, about 08:48**, by the author's reading of the sitting's results as the coordinator set
them out in the conversation (R1 to R10, what each repair costs, the landing in two lanes, the two
questions asked beside the ratification), answered through the question form, meant as: *ratify*.
R1 to R10 stand as adopted. The two questions asked beside it were answered in the same form: R3's
`intern_*` (defect 2375) is kept as a known silent shape and never refused, since refusing it refuses
static FFmpeg and X11 as the sitting measured, pinned as a known cost by a case whose header gives
that reason and this ruling; and the blind seat is allowed, at most 10 USD in all (the author's
words: *yes 10 dollars*), its twelve sessions run against the landing's compiler with their grading
registered before any session starts.

## Process notes

1. **The critic's first pass changed the briefs before any seat started**: eleven findings (two new
   silent members of 2364, `pragma` and `partial`; a name the runtime calls replaced by a linked
   library; the file on which ld64 decides by the names; line 131 of the specification; GNU ld's
   trace naming another definer as a *reference*; lld in the Linux image; the trace printed to the
   author by `run_link`; three routes nobody listed; the author's 10 USD of 2026-11-08), every one
   repaired at `1c4bc5197` before the seats were launched.
2. **The critic's second pass changed the resolution and sent three seats back**: v5's hole on ld64
   (`ab_*`), R2's effect on `intern_*`, `packlib`'s one file still wrong, the cost counted on the links,
   the words of both refusals. The compiler-engineer built a second version (the one-file arm, the
   words, the sorted composer, the dump unit repaired), the ffi-pragmatist built v6 (the whole load,
   the folded forward reading, the runtime's names, the words), and the spec-warden withdrew one
   objection and wrote the reference words, each before this synthesis.
3. **The four seats were launched in one message at about 00:31** (panel 238's note 3). The
   spec-warden finished at about 00:53 and the historian at about 00:56; **at about 01:08 the account's
   five-hour limit stopped the compiler-engineer and the ffi-pragmatist mid-work**, and both were
   resumed at 01:48, when the limit reset, from their own transcripts and report files, never from
   another seat's findings. The coordinator's own message on resuming said it had been stopped before
   writing the shared brief; the record says otherwise (briefs at `a91d1abd9`, repaired at
   `1c4bc5197`, the critic's first pass 00:00 to 00:28, the seats from 00:31).
4. **The machine was loaded**: `uptime` 45 to 93 at the seats' launch and 218 during the critic's
   first stage 2, 3 to 13 after 01:48; every cost here is instructions retired.
5. **The worktrees stood under `.claude/worktrees/`** (`239-<seat>/`, `p239-scratch/`), as at panels
   235 to 238: the contract forbids a file outside the repository's root, which the `/panel` skill's
   scratchpad would be.
6. **No paid run of any kind.** The blind seat's brief, tasks and size are in `239-briefs/blind/`,
   written at the synthesis because its arms are the seats' drafted words: twelve sessions bounded at
   0.45 USD each, 5.40 USD in all, unrun, the author's to allow (the author allowed 10 USD more for
   paid runs at panel 237's ratification, `5d9e832b2`; this sitting's coordinator was told to start
   none and to ask once). What it gives up: a measurement of whether a model holding only the
   specification repairs the program from each refusal's words.
7. **The sitting's own commits touch `docs/panel/**`, a path of the site's deploy**, so the push that
   carries them publishes the site; that push is the author's to ask for.
8. **Evidence**: `239-reports/evidence.tar.xz` and `evidence.md` beside it.
