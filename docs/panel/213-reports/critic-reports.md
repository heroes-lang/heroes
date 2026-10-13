# Panel 213, the completeness critic's report, pass 2 (after the seats)

Written 2026-10-13, 00:44 to 00:57 (`date`), in my own copy
`.claude/worktrees/scratch-panel-213/critic2` at `0b8307daf`, its compiler built
from the seed (removed at the end). The generator, the records, `nd.c` and the
transcripts are kept in `213-reports/critic-reports-work/`, the copy's path
written `<copy>`; LLVM's `formatter_bytecode.py` is not copied (fetch it from
`release/22.x/lldb/examples/python/`). Not a judge, no verdict. Most consequential first.

## In short

1. **The CI's Linux legs install lldb 18** (`apt-cache policy lldb` on
   `ubuntu:24.04`: `1:18.0-59~exp2`), and **LLVM 20 is the first release that
   loads an embedded formatter at all** (`FormatterSection.cpp` 404 on
   `release/18.x` and `19.x`, 200 on `20.x`). Route C is dark on two CI legs,
   and a suite case pinning it fails there rather than skipping. (§ 1)
2. **Embedded bytecode cannot give `[T]` or `{K: V}` their elements on any lldb
   measured**: LLVM 22.x's loader takes only a summary (signature 0) from
   `__lldbformatters`, synthetic children are `main`-only on a bytecode ABI
   from 2026-10-06, `create_child_at_offset` is `main`-only, `read_memory*`
   is implemented nowhere, and no bytecode can name a type. The containers need
   Python on every measured lldb, which meets spec-warden's own condition for
   G. (§ 2)
3. **Run: an embedded bytecode summary picks a `T?`'s live arm, Python-free,
   on this Mac's lldb and lldb-dap and on the image's lldb 22.1.8 and
   lldb-dap-22**, `h7_good = 4` and `h8_bad = "odd"`, children hidden by the
   record's flags, assembled with LLVM's own `formatter_bytecode.py`. The
   "summaries cannot branch" premise holds for `__lldbsummaries` only. (§ 2b)
4. **compiler-engineer's "assembler" objection and the others' "C works" are
   both true, of different sections**; and every record integer is ULEB128,
   which the seats' one-byte sizes get right only below 128. (§ 3)
5. **F for two lines per function**: `#pragma clang attribute push
   (__attribute__((nodebug)), apply_to = variable(unless(is_parameter)))`
   hides a run of locals, and `nodebug` is honoured in DWARF on this Mac,
   in Linux clang 22.1.8's DWARF and in CodeView. (§ 4)
6. **`-ftrivial-auto-var-init=pattern` makes "not yet bound" recognisable**:
   `h15_n` reads `0xAAAAAAAAAAAAAAAA` instead of a plausible `6809745884`. (§ 5)
7. Historian's E-in-C against two measured vetoes (§ 6); the ergonomist scored
   no frame a route produces (§ 7); nobody located G's file or its tester (§ 8).

## 1. The CI's Linux legs run an lldb that predates every embedded-formatter loader (route C is dark there)

Nobody asked which lldb the milestone's own instrument runs on outside this Mac
and the local image. Measured:

- `.github/workflows/ci.yml` line 332: the Linux legs run
  `sudo apt-get install -y clang lldb ...` on `ubuntu-24.04-arm` and
  `ubuntu-latest` (`grep -n "runs-on\|ubuntu" ci.yml`).
- `docker run --rm ubuntu:24.04 sh -c 'apt-get update -qq && apt-cache policy lldb && apt-cache depends lldb'`
  printed `Candidate: 1:18.0-59~exp2` and `Depends: lldb-18`.
- The loader's first release: `curl` of
  `release/{18,19,20,21}.x/lldb/source/DataFormatters/FormatterSection.cpp`
  returned **404, 404, 200, 200**; `release/{18,19,20}.x/lldb/source/Target/Target.cpp`
  holds `LoadTypeSummariesForModule` **0, 0, 1** times (`grep -c`).

So **LLVM 20 is the first release that loads `__lldbsummaries` at all, and the
CI's Linux legs install lldb 18.** Historian's condition (a) ("my reading changes
if the lldb ... in CI ... predates the `FormatterSection` loader") is met, by
source and package metadata; *a C summary shown on the CI's Linux leg* is unrun
and predicted dark. ffi-pragmatist's and spec-warden's Linux evidence is the
local image's `lldb 22.1.8` (Debian 13), not the CI's. A suite case that pins
an embedded summary fails on two CI legs unless the leg installs `lldb-20` or
later (apt.llvm.org, or the Debian image), which is a CI decision the sitting
has to make in the same ruling, not after it. The `macos-26` runner's Xcode
lldb: unmeasured.

## 2. Embedded bytecode cannot give `[T]` its elements on any lldb the sitting measured; on LLVM main it can, from an ABI one week old

The seats left this as the decisive question (ffi-pragmatist's condition for
*C alone*, spec-warden's condition for *B over C*, historian's condition (b)).
It is settled by the interpreter's source, read with `curl` and `grep`, not by
`strings` (which misled: see below). Files saved in my scratchpad as
`fb22.cpp`, `fbmain.cpp`, `fs22.cpp`, `fsmain.cpp`, from
`raw.githubusercontent.com/llvm/llvm-project/{release/22.x,main}/lldb/source/DataFormatters/`.

- **The loader of LLVM 22.x accepts only a summary from `__lldbformatters`.**
  `fs22.cpp` lines 152 to 166: `if (signature == 0)` makes a
  `BytecodeSummaryFormat`; every other signature (init, num_children,
  child_at_index, update: the synthetic children) is logged *"Unsupported
  formatter signature {0} for '{1}' in {2}"* and dropped. **Synthetic children
  from an embedded section exist only on `main`** (`fsmain.cpp` 156 to 230,
  `CreateSyntheticImpl`), whose bytecode *"version 2 ABI"* landed
  **2026-10-06** (`curl api.github.com/repos/llvm/llvm-project/commits?path=lldb/source/DataFormatters/FormatterSection.cpp`,
  the newest commit, #228589). No released lldb carries it.
- **This Mac's lldb-2103 carries 22.x's string** *"Unsupported formatter
  signature {0} for '{1}' in {2}"* (`strings -n 3 .../LLDB.framework/LLDB`,
  line 18095 of the dump), which `main`'s loader no longer has
  (`grep -c "Unsupported formatter" fsmain.cpp`: 0). So Apple's lldb is the
  summary-only loader too: an inference from a string, unrun as a program.
- **The selectors `[T]` needs are `main`-only.** `grep -n "case sel_"`:
  22.x implements `summary, get_num_children, get_child_at_index,
  get_child_with_name, get_child_index, get_type, get_template_argument_type,
  get_value*, cast, strlen, fmt`; `main` adds `get_parent,
  get_synthetic_value, get_non_synthetic_value, clone, get_pointee_type,
  get_byte_size, create_child_at_offset`. **No `read_memory*` selector has a
  case in either file** (`grep -n read_memory`: nothing): they fall to
  `default:` and *"selector not implemented"*. ffi-pragmatist's selector list
  (read from `strings`, `@read_memory` and kin) names selectors the interpreter
  refuses, and misses `cast`, which 22.x does implement: **`strings` is not an
  instrument for what the bytecode can do.**
- **And the element type is reachable only through the value's own type
  graph**: there is no type literal (the opcodes are `lit_integer`,
  `lit_string`, `lit_selector`, `null` on `main`; 22.x has `lit_uint`,
  `lit_int`, `lit_string`, `lit_selector`: `curl` of
  `release/22.x/lldb/source/DataFormatters/FormatterBytecode.def`, `grep -n lit_`) and no global lookup. So even on `main`,
  `create_child_at_offset(name, 32 + i*size, T)` needs a `T` the bytecode can
  reach from `HeroArrayHeader *`'s DWARF, which today leads only to
  `const HeroDesc *`. ffi-pragmatist's typedef (`typedef HeroArrayHeader
  h_arr_int64`) carries the element type in its **name**, which bytecode cannot
  turn into a type. A shape that would carry it with no layout move, a union
  `{ HeroArrayHeader h; T *elem_type; }` or a member of the element type, is a
  route H **nobody priced**; it changes the C type every runtime call sees (unrun).

**What summary bytecode does reach on the measured lldbs** (from `fb22.cpp`; run
for a `T?` in § 2b, inferred for a variant): `if`/`ifelse` (22.x's `.def` lines 28 and
29, opcodes 0x11 and 0x12; `case op_if` and `case op_ifelse` at `fb22.cpp`
290 and 299), `get_child_with_name`, `get_value_as_unsigned`,
`summary` and `fmt`. That is enough to pick a `T?`'s live arm and a variant's
case and print its payload's summary, **with no Python**, so ffi-pragmatist's
*"summary strings cannot branch"* is true of `__lldbsummaries` and not of a
signature-0 entry in `__lldbformatters`. It is not enough for `[T]` or
`{K: V}`: no loop, and in 22.x a value at a computed address only through
`cast` to a type the value's own DWARF already leads to, which today's
`HeroArrayHeader *` does not (a `cast` route was not attempted). **Consequence
for the resolution**: spec-warden's own condition for *B over C* ("bytecode
unsupported where the milestone must deliver, so containers need Python") is
met for containers on every lldb measured, by source; ffi-pragmatist's
condition for *C alone* cannot be met this milestone. The live arm and the
variant may still go Python-free, through an emitter that writes bytecode,
which is compiler-engineer's "assembler" objection in its true place (§ 3).

## 2b. Run: an embedded bytecode summary picks a `T?`'s live arm on this Mac's lldb and on the image's lldb 22.1.8, with no Python and nothing typed

ffi-pragmatist left the bytecode unrun for want of an assembler (`find` over
the installed toolchains). **LLVM's own assembler is in its source tree**:
`lldb/examples/python/formatter_bytecode.py`, 528 lines at `release/22.x`
(`curl`, `wc -l`), not installed by either toolchain. I used its `compile()`
from a 30-line generator of mine (`critic-reports-work/mk.py`) to write one
`__lldbformatters` record for `h_0opt_e201354` (shapes.hero's `i64?`), record
fields ULEB128 per `fs22.cpp` 30 to 90, signature 0, 72 bytes of bytecode:

    dup "tag" @get_child_with_name call @get_value_as_unsigned call 0u =
    { "as" @get_child_with_name call "ok" @get_child_with_name call @get_value call }
    { "as" @get_child_with_name call "err" @get_child_with_name call
      "code" @get_child_with_name call "ptr" @get_child_with_name call @summary call }
    ifelse

appended as a `static const unsigned char[93]` with `__attribute__((used,
section(...)))` to the build's own `shapes.c`, compiled with the build's
`words-*.rsp` (zero diagnostics), relinked; the program prints `7 ada 9 4 10 42`.

- **This Mac, lldb-2103.0.34.103**, `lldb --no-lldbinit --batch`, stopped at
  `shapes.hero:30`, `frame variable h7_good h8_bad`:
  `(h_0opt_e201354) h7_good = 4 {` and `(h_0opt_e201354) h8_bad = "odd" {`.
- **`heroes-linux-arm64:latest`, lldb version 22.1.8**, the same C with
  `.lldbformatters`, built there with `clang -std=gnu11 -g -O0`
  (`readelf -S | grep -c lldbformatters` 1): `h7_good = 4 {`, `h8_bad = "odd" {`.

So **the live arm, and by the same `ifelse` over `tag` a variant's case, needs
no Python on either measured lldb**; *"a summary has no condition"* (ffi-pragmatist)
is true of `__lldbsummaries` only. What this did not show: the children beneath
print C (`tag = 0`, `as = { ok = 4 ... }`) with `flags` 0; **with `flags` 8
(the hide-children bit, my reading of lldb's `TypeSummaryImpl` flags) the same
record on this Mac prints `h7_good = 4` and `h8_bad = "odd"` and nothing beneath**
(`critic-reports-work/mk2.py`, `shapes-bc8`, run), and **the same on the image**, lldb 22.1.8
(`h7_good = 4`, `h8_bad = "odd"`, nothing beneath) and **lldb-dap-22**
(`h7_good = 4   [h_0opt_e201354]`, `h8_bad = "odd"   [h_0opt_e201354]`,
`dap.py` with no `initCommands`, `critic-reports-work/dap-linux-bc8.txt`); **Xcode's lldb-dap shows it too** (with `flags` 8): ffi-pragmatist's
`dap.py` (copied, run with no `initCommands`) over `shapes-bc8` at :30 printed
`h7_good = 4   [h_0opt_e201354]` and `h8_bad = "odd"   [h_0opt_e201354]`
(`critic-reports-work/dap-bc8.txt`); a full
`ok(4)` / `fail(code: "odd", msg: "not even")` rendering needs `fmt`, unrun.
**An emitter writing this needs a bytecode encoder in Heroes**: the program
above is a fixed template per shape with the field names substituted, and the
opcode table is 40 entries (`FormatterBytecode.def`); that is compiler-engineer's
"assembler", and its size is unpriced. And § 1 holds for it too: lldb 18 on the
CI's Linux legs loads neither section.

## 3. compiler-engineer's "C needs an assembler" and the other seats' "C works": both true, of two different sections

The contradiction is checkable and the seats' own runs settle it.
`__lldbsummaries` is **not bytecode**: a record is a version, a record size,
a type name and a summary string (`fs22.cpp` lines 34 to 103, read). ffi-pragmatist
built it as one C string literal and saw it load by itself on four debuggers
(this Mac's lldb and lldb-dap, the image's lldb 22.1.8 and lldb-dap-22), and
spec-warden on this Mac's lldb. compiler-engineer's objection is true of
`__lldbformatters`, which is bytecode, and **an emitter needs that one only
for the live arm and the variant** (§ 2). Two facts no seat wrote down:

- **Every integer in a record is ULEB128**: `fs22.cpp` line 41 (*"Integers are
  encoded using ULEB"*), and `getULEB128` at lines 60, 61, 66 and 103. Both
  seats wrote one-byte sizes (`"\x30"`, `"\x24"`), right only below 128; a
  record summary of a record with a few long field names passes 127 bytes. The
  emitter needs a ULEB128 writer (a few lines, not an assembler), and a case
  whose summary is at least 128 bytes long. ffi-pragmatist found two silent
  failures already (a wrong size ignored, a short size truncated); a wrong ULEB
  is a third of the same class (inferred from the source, unrun).
- The record's type name may be a regex (`^` prefix, `fs22.cpp` line 123), so
  one record can cover every `h_0opt_*` or every record type; it still cannot
  branch.

## 4. Route F costs two lines per function, not 347,551 marks: `#pragma clang attribute`, run on three object formats

A route nobody listed for the +7.4% compiler-engineer measured (and the
"short runtime macro" estimate, which only halves it). `build/nd.c` in my copy (kept as `critic-reports-work/nd.c`):
one local with `__attribute__((nodebug))`, one plain local, and two locals
inside `#pragma clang attribute push (__attribute__((nodebug)), apply_to =
variable(unless(is_parameter)))` ... `#pragma clang attribute pop`.

- This Mac, `clang -std=gnu11 -Wall -Wextra -g -O0 -c nd.c`, then
  `dwarfdump nd-mac.o | grep DW_AT_name`: only `visible_loc`.
- Windows CodeView, `clang --target=x86_64-pc-windows-msvc -g -gcodeview -O0 -c nd.c`
  on this Mac, then `strings nd-win.o | grep -E "hidden|visible|pragma"`: only
  `visible_loc`.
- Linux, `heroes-linux-arm64:latest`, `Debian clang version 22.1.8`, then
  `llvm-dwarfdump-22`: only `visible_loc`.
- `apply_to = variable(is_local)` is **refused** by clang (*"attribute
  'nodebug' cannot be applied to 'variable(is_local)'"*), and an empty region
  warns (`-Wpragma-clang-attribute`), so the emitter opens a region only where
  a hidden slot exists.

So `nodebug` is honoured on the three formats the project targets (the
question in the coordinator's note), and the region form makes F's emitted-C
growth one push and one pop per function **if the prologue writes its hidden
slots contiguously**, which today's slot loop interleaves with the author's
(`prologue.hero`, per compiler-engineer's diff). The byte count over the
compiler, the reordering's effect on the emitted C's determinism goldens, and
lldb-dap's Variables pane with F are unrun. On Windows the attribute is
honoured in the object; no debugger runs there (§ 7).

## 5. "A binding not yet reached shows garbage on every route": one clang flag makes it recognisable

compiler-engineer prices the fix as a live range per slot, not this milestone's.
A route nobody listed: `-ftrivial-auto-var-init=pattern` in `-O0`'s debug words.
Run in `build/ai/` of my copy over measurement 042's `shapes.hero`
(ffi-pragmatist's copy, built with `heroes build shapes.hero -O0`), the unit
recompiled with the build's own `words-*.rsp` plus the flag and relinked; the
program prints `7 ada 9 4 10 42` both ways; `lldb --batch` stopped at
`shapes.hero:30`, `frame variable`:

- today: `(h_shapes_Token_c_num) h15_n = (f_v = 6809745884)`, `h11_r0 =
  4296245248`: plausible numbers, no sign they are unwritten;
- with the flag: `h15_n = (f_v = -6148914691236517206)`, that is
  `0xAAAAAAAAAAAAAAAA`, and **76** lines of the frame carry the pattern against
  **0** today (`grep -c -i "aaaaaaaa\|-6148914691236517206"`; both frames kept as
  `critic-reports-work/frame-O0-today.txt` and `frame-O0-pattern.txt`), the
  not-yet-written `HeroStr` reading `len = -6148914691236517206`.

It does not remove the wrong value; it makes it one pattern a formatter can
render as `<not yet bound>` and that a header pointer cannot pass for a
readable address (ffi-pragmatist's sixteen tracebacks over garbage headers in
lldb-dap are that failure). Cost: `h_shapes_main`'s breakpoint offset moved
from `+1488` to `+2928` (code growth at `-O0`); the cold compiler build's time
with the flag is unrun. It reaches only `-O0`, which is the only level that
shows variables (`flags.hero` `debug_words`).

## 6. Contradiction: historian approves E in the emitted C; two seats veto it on runs

Historian's verdict is *C together with E (the author's names in the emitted
C)*, resting on Nim's unmangled locals. compiler-engineer (`docker run` in
`heroes-linux-arm64:latest`: `clang -std=gnu11` refuses `long linux = 1;`,
*expanded from macro 'linux'*) and ffi-pragmatist (`routee.c`: `errno` refused
on both platforms, `linux` on Linux only, `stdin` renamed `__stdinp` in
silence on this Mac) measured the opposite. The checkable side is the vetoes,
and both are already run; historian had no shell. What stays open is E through
the debug information alone, which nobody vetoes and compiler-engineer priced
at 2.2 times the clang stage's CPU with a textual-IR premise unrun on Linux and
Windows clang, plus his one-spelling rule (`dup.hero`: a wrong value under the
author's name at exit 0).

## 7. The ergonomist scored three frames, none of which a route on the table produces

Frames 1 to 3 were: C types with demangled names and temporaries, raw C, and
source names with Heroes values. The routes measured tonight produce a fourth:
**C names, Heroes values for records and `T?` and variants, temporaries gone**
(F plus C: `h2_p = Point(x: 3, y: 4)`, `h7_good = 4`, ffi-pragmatist's and
§ 2b's runs), with `[T]` and `{K: V}` still addresses unless Python is loaded
(§ 2). Frame 3's decisive line, `players = [Player(...), ...]`, is exactly the
part no Python-free route reaches on any measured lldb. So the ergonomist's
prediction (frame 3 at least 15 points above frame 1) does not score the
resolution; the sitting did not ask how far a frame of C names with Heroes
values sits from frame 3. Unrun, and runnable with the seat's own method.

## 8. Where a fixed Python formatter (G) lives, and whether § 10 reaches it: nobody located it

compiler-engineer accepts only G for the containers, spec-warden takes G before
per-build B, ffi-pragmatist's B carries the containers: three seats lean on a
Python file and **none names its directory or its tester.** The candidates, each
unrun: `editors/vscode/` (the extension already starts lldb-dap; ffi-pragmatist
measured `initCommands` loading a formatter on both lldb-daps), `runtime/`
beside `heroes_runtime.h` (versioned with `HERO_RUNTIME_ABI` 30), or bytes
inside `heroes` written out on demand. `.claude/rules/cli-surface.md` line 15
reads *"Never a second binary, never a script, never a Makefile"* under *One
command*, a rule about how a capability is **reached**; a file lldb imports is
reached through lldb's prompt or the editor, which is spec-warden's "the
substance reaches it" reading. Whether the compiler's own tests or the net can
exercise a Python file (compiler-engineer's "a second language the compiler's
own tests cannot check") is the question the sitting did not ask; the net's
`lldb` suite runs `--no-lldbinit`, so it would type the import itself (spec-warden).
Python-less `lldb.exe` on Windows dies before any of it (ci.yml:487).

## 9. Smaller gaps

- **The prediction instrument is this Mac alone** for every seat that predicts
  an embedded record in the `lldb` suite: on the CI's Linux legs lldb 18 runs
  (it does not SKIP) and would **fail** such a case (§ 1). The suite's
  skip-where-lldb-cannot-run rule (`ci.yml` 333 to 337) does not cover "lldb
  runs and lacks the loader".
- **ffi-pragmatist's record regex misses a match arm's payload type**
  (`h_shapes_Token_c_num`, his own note) and lldb-dap evaluates every slot,
  written or not: § 5's pattern is the cheap guard for both a Python and a
  bytecode formatter.
- **The 8-character COFF section-name question** (historian's condition (c))
  stays unrun: no `lld-link` was tried, and no Windows debugger runs.
- **compiler-engineer's "a `HERO_NODEBUG` macro halves the 8.7 MB"** is his
  estimate; § 4's region form is the run alternative to it.

## Not run by this pass

Any bytecode for `[T]` (no `cast`-based
attempt was made; whether Apple's lldb-2103 implements `cast` is unknown, its
`strings` being no instrument, § 2); the `#pragma` region inside
`selfhost/emit/prologue.hero`; the cold compiler build with
`-ftrivial-auto-var-init=pattern`; the CI's `macos-26` lldb; Windows.
