# Panel 213, the ffi-pragmatist's report

Seat: ffi-pragmatist. Copy: `.claude/worktrees/scratch-panel-213/ffi-pragmatist`
at `0b8307daf`, compiler built from the seed there (`clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`, 2026-10-13 00:09). Scratch files
under that copy's `build/s/`, `build/c/`, `build/dap/` and `build/linux/`
(removed with the copy when the sitting ends). Started 00:08, finished 00:25
(`date`).

## Running log

- `build/s/shapes.hero` is measurement 042's `shapes.hero` verbatim; `heroes
  build shapes.hero -O0 -o shapes` built it, `./shapes` printed `7 ada 9 4 10 42`.
- `nm shapes | grep -i desc` lists `_h_shapes_Point_desc` (`s`, local data)
  and the runtime's `_hero_desc_*` (`S`, external data).
- `strings` over Xcode's `LLDB.framework/LLDB` prints `__lldbsummaries`,
  `__lldbformatters`, `eSectionTypeLLDBFormatters`, `Loaded embedded type
  summary for '{0}' from {1}.` and a `FormatterBytecode.cpp` path: the
  framework carries code for both embedded sections. Whether it loads them is
  below, run.

## Task 1: a formatter over TODAY's runtime, run (00:10 to 00:13)

`build/s/heroes_lldb.py` (Python for lldb, about 280 lines, loaded with
`command script import`), against `build/s/shapes` at `-O0`, stopped at
`shapes.hero:30`, `lldb --no-lldbinit --no-use-colors --batch -s cmds5.txt`:

    (int64_t) h0_count = 7
    (HeroStr) h1_name = "ada"
    (h_shapes_Point) h2_p = Point(x: 3, y: 4) {  x = 3  y = 4 }
    (h_shapes_Token) h3_t = .num {  v = 42 }
    (HeroArrayHeader *) h4_names = 0x... ["ada", "grace"]
    (HeroArrayHeader *) h5_points = 0x... [Point(x: 1, y: 2), Point(x: 5, y: 6)]
    (HeroMapHeader *) h6_ages = 0x... {"ada": 36, "grace": 85}
    (h_0opt_e201354) h7_good = ok(4) {  ok = 4 }
    (h_0opt_e201354) h8_bad = fail(code: "odd", msg: "not even") { err = (code = "odd", msg = "not even") }
    (h_shapes_Room) h9_room = Room(name: "lab", corner: Point(x: 9, y: 10)) { name = "lab" corner = ... }

(children folded onto one line here; the transcript is the copy's
`build/s/out5.txt`). `frame variable -P 1 h5_points h6_ages` expands the
children `[0] = Point(x: 1, y: 2)`, `[1] = ...` and `["ada"] = 36`,
`["grace"] = 85`.

How each shape is read, and what it rests on:

- **`[T]`**: `len` from the header; the element type from the SYMBOL that
  `elem` points at (`SBTarget.ResolveLoadAddress(elem).GetSymbol()`:
  `hero_desc_str` maps to `HeroStr`, `h_shapes_Point_desc` to `h_shapes_Point`
  by stripping `_desc`, `hero_desc_array` to `HeroArrayHeader *`), then
  `FindFirstType`; elements at `header + sizeof(HeroArrayHeader)`, stride the
  DWARF type's size. **No descriptor field, no runtime change**.
- **`{K: V}`**: `key`/`val` descriptors resolved the same way; `cap` state
  bytes read at `header + states`, a bucket live where its byte is `1`
  (`runtime/parts/map.c:210`), keys at `header + keys + i*ksize`, values at
  `header + vals + i*vsize`.
- **`str`**: `ptr` read for `len` bytes (not to the NUL), `ptr == NULL` shown
  `<unassigned>`.
- **`T?`**: type name `^h_0opt_`; `tag` 0 shows `ok`, 1 shows `err`; one arm only.
- **variant**: a struct with `tag` and `as`; the enumerator's text after
  `_tag_` is the case, the payload `as.c_<case>` with `f_` stripped.
- **record**: any `^h_[a-z0-9_]+_[A-Z][A-Za-z0-9]*$` without `tag`; fields
  `f_x` shown `x`, and the one-line summary is Heroes' own constructor syntax.

What it did NOT fix, and could not from a formatter: the **type column** stays
C (`(HeroArrayHeader *)`, `(h_0opt_e201354)`), the **variable names** stay C
(`h4_names`), and `v h5_points[1].x` is refused (`"x" is not a member of
"(HeroArrayHeader) [1]"`: frame-variable path syntax does pointer arithmetic on
a pointer before it asks the synthetic children), as is `p` of any author name
(expression evaluator, not formatters). Bugs found while writing it, each one
mine and each a trap for whoever writes the real one: a summary registered on
`HeroArrayHeader` also applies to the dereferenced header, so the provider's
own `GetChildMemberWithName("len")` hit the synthetic and read 0; `type summary
add` without `-e` hides children; `frame variable` does not expand a pointer's
synthetic children without `-P 1`, which is why the array and map summaries
print the elements inline; `GetBasicType` returned an invalid type where
`FindFirstType("int64_t")` worked.

## Task 2: route C, measured (00:12 to 00:19)

**The smallest C** (`build/c/embed.c` in the copy): one record per type in a
section the debugger reads at load, `__TEXT,__lldbsummaries` on Mach-O and
`.lldbsummaries` on ELF, bytes `01` (version), record size, type-name size,
name and NUL, summary size, summary and NUL:

    #define HERO_LLDB_SUMMARY __attribute__((used, section("__TEXT,__lldbsummaries")))
    HERO_LLDB_SUMMARY static const unsigned char h_m_Point_lldb_summary[] =
        "\x01" "\x30" "\x0a" "h_m_Point" "\0" "\x24" "Point(x: ${var.f_x}, y: ${var.f_y})";

clang accepted it under `-Wall -Wextra` with no diagnostic. My first try had
both sizes off by one and lldb ignored it **in silence**: a wrong record is not
an error, it is no formatter (an emitter writing these needs a test that reads
the summary back, not a hope).

| where | debugger | loaded by itself? | what it printed |
|---|---|---|---|
| this Mac | `lldb-2103.0.34.103`, no `.lldbinit`, nothing typed | yes: `log enable lldb formatters` shows `Loaded embedded type summary for 'h_m_Point ' from .../embed` | `(h_m_Point) p = Point(x: 3, y: 4)` |
| this Mac | Xcode's `lldb-dap` (`xcrun -f lldb-dap`), driven by `build/dap/dap.py`, a 70-line DAP client of mine, no `initCommands` | yes | `p = Point(x: 3, y: 4)   [h_m_Point]` |
| `heroes-linux-arm64:latest` | `lldb version 22.1.8` | yes: the same log line, from `/w/embed-linux` | `(h_m_Point) p = Point(x: 3, y: 4)` |
| `heroes-linux-arm64:latest` | `lldb-dap-22` | yes | `p = Point(x: 3, y: 4)   [h_m_Point]` |

(The container's lldb needs `settings set target.disable-aslr false` and the
DAP launch `"disableASLR": false`, docker refusing `personality`; a container
fact, not a Heroes one.)

**On a Heroes program**: `heroes build shapes.hero --emit-c`, the records of
`gen_embed.py` appended (what an emitter would write: `HeroStr` as
`${var.ptr%s}`, `h_shapes_Point`, `h_shapes_Room`, `h_shapes_Token`,
`HeroArrayHeader`), built with `clang -g -O0 -I runtime`: `h1_name = "ada"`,
`h2_p = Point(x: 3, y: 4)`, `h9_room = Room(name: "lab", corner: Point(x: 9,
y: 10))` (nesting works: the inner summary is used), but `h3_t =
h_shapes_Token_tag_num` and `h4_names = len=2`. **A summary string has no
condition and no loop**: it cannot pick a `T?`'s live arm, cannot pick a
variant's payload, cannot walk `len` elements, and cannot rename the children
it prints beneath (`f_x = 3` stays). Those need the formatter-bytecode section,
`__lldbformatters`, whose strings this lldb carries; **unrun tonight**: no
bytecode assembler is on this Mac (`find` over Homebrew, `/usr/local`, Xcode's
LLDB.framework and CommandLineTools for `formatter_bytecode*`, nothing), and
whether its vocabulary can make a value of a given type at a computed address
(what `[T]` needs, the elements being in-line after the header) is a question,
not a premise.

**What survives a release build** (the ABI question's half for route C): with
`-O2 -Wl,-dead_strip` then `strip`, the Mach-O section stays (`otool -l | grep
-c lldbsummaries` 1). On Linux, `-O2 -ffunction-sections -fdata-sections
-Wl,--gc-sections` then `strip` **drops** `.lldbsummaries` (count 0) despite
`used`; adding `retain` (`__attribute__((used, retain, section(...)))`) keeps
it (count 1), and clang on this Mac takes `retain` with no warning. Heroes
links with neither flag today (`grep -n "dead_strip\|gc-sections"
selfhost/cli/*.hero`, nothing).

## Task 3: route B's loading, each tried (00:17 to 00:19)

| mechanism | tried | result |
|---|---|---|
| `command script import` typed | lldb, this Mac and Linux | works: every shape above |
| the editor's `initCommands` | lldb-dap, this Mac (Xcode) and Linux (`lldb-dap-22`), via `build/dap/dap.py` | **works**: `h4_names = ... ["ada", "grace"]` with children `[0] = "ada"`, `h6_ages` with `["ada"] = 36`, `h7_good = ok(4)`, on both |
| a script inside a dSYM, `shapes.dSYM/Contents/Resources/Python/shapes.py` (dSYM made by hand with `dsymutil`: a Heroes build makes none) | lldb, this Mac | **not loaded**: `warning: 'shapes' contains an untrusted debug script`; `target.load-script-from-symbol-file` reads `trusted` by default; with it set `true`, loaded and printed `["ada", "grace"]`. So it needs a setting somebody types |
| `.lldbinit` in the working folder | lldb, this Mac | **not read**: `There is a .lldbinit file in the current directory which is not being read`; `target.load-cwd-lldbinit` is `warn` |
| gdb's auto-load, `.debug_gdb_scripts` | not tried | unrun |

So **the one zero-typing loader for route B is the extension's
`initCommands`** (or a `heroes` verb that starts lldb with the import, route
D). `editors/README.md`'s *improves with no change in the extension* is false
for route B (the extension must add the import) and true for route C, measured
on this Mac's and Linux's lldb-dap.

A robustness finding for whoever writes B: in lldb-dap the Locals pane
evaluates **every** slot, including ones not yet written at the stop
(`h14_s0 = .0x3f8000`, `h11_r0 = 4296245248`), and my formatter threw
`TypeError: invalid null reference in method 'SBValue_CreateValueFromAddress'`
and `IndexError` on them, sixteen tracebacks into the debug console, the pane
still drawn. A formatter over a garbage header must bound `len` and `cap` (mine
caps 10,000 and 65,536) and refuse an `elem` that resolves to no descriptor;
otherwise a garbage `len` is a debugger reading gigabytes. That is design.md
§1.12's spirit applied to the tool, and an argument for F (hide what is not
yet live).

## Task 4: the ABI (00:15 to 00:19)

- **`HERO_RUNTIME_ABI` (30, `runtime/heroes_runtime.h:108`) moves for no
  route measured.** B reads today's headers; C adds a section of bytes per
  binary; neither changes a declaration.
- **A `name` field in `HeroDesc` is not needed**, and would move the ABI.
  `nm shapes | grep -c desc` is 20 at `-O0` and 20 at `-O2`, with
  `_h_shapes_Point_desc` (`s`) present in both. After `strip`, 0, **and** 0
  `OSO` entries (`nm -ap | grep -c OSO`, 2 before): a stripped Heroes binary
  has no variables at all on this Mac, so there is nothing left to format. The
  one case where the symbol matters is strip plus a dSYM (made by hand):
  there `SBAddress.GetSymbol()` returned `None` for the descriptor's address
  and my formatter printed `[]`, **but** `image lookup -v -a` still names it,
  `Variable: ... kind = static, name = "h_shapes_Point_desc"`, from the DWARF
  record of the static, and `FindGlobalVariables("h_shapes_Point_desc")`
  returns it. So a formatter that falls back to the DWARF variable (or to the
  name of the descriptor's `copy` function, `h_shapes_Point_desc_copy`, which
  DWARF also keeps) needs no field. Issue 939's *the linker already holds the
  name* is true unstripped and false stripped, as the critic said; *the debug
  information holds it* is the statement that is true in both cases a debugger
  can show variables in.
- **Route C costs a section, not a symbol**: survives `-O2` and `strip` on
  both (Linux: `clang -O2 embed.c`, `strip`, `readelf -S | grep -c
  lldbsummaries` 1), and `--gc-sections` on Linux only with `retain` (above).

## Route E from the C side: the author's name as the C name breaks the build (00:19 to 00:22)

`-std=gnu11` is what a build passes (`selfhost/cli/flags.hero:132`). Heroes
takes `linux`, `errno`, `stdin` as bindings (`build/s/names.hero`, `heroes run`
printed `8`). If route E emitted them as C locals of the same name
(`build/c/routee.c`, one function each, the real headers):

| binding | header | this Mac | Linux image |
|---|---|---|---|
| `stdin = 1` | `<stdio.h>` | **compiles, silently renamed**: the DWARF variable is `__stdinp` (`dwarfdump --name=__stdinp`), the macro's expansion | compiles (renamed the same way, unmeasured which name) |
| `errno = 3` | `<errno.h>` | error: `conflicting types for '__error'` | error: `conflicting types for '__errno_location'` |
| `true = 5` | raylib.h (its `<stdbool.h>`) | error: `expected identifier or '('` | (`<stdbool.h>`) not run |
| `linux = 4` | none: gnu11 predefines it | compiles | **error**: `expected identifier or '('` |

The lowercase object-like macros the real binding headers define, `clang
-std=gnu11 -dM -E`: raylib.h `bool false true`; Homebrew's sqlite3.h none;
`<stdio.h> <errno.h> <math.h>` `errno math_errhandling stderr stdin stdout`.
**So the `hN_` prefix is what keeps a Heroes name out of the preprocessor's
namespace, which every `--include`d header shares with the emitted C**, and a
program that builds on this Mac and not on Linux (`linux`) is exactly the class
design.md §4.19's header verification exists to stop. Route E done by renaming
the C identifiers is refused from this seat (veto, ABI-adjacent: it makes the
emitted C's validity depend on the FFI headers' macros). E done in the debug
information only (a DWARF name differing from the C name) has no C spelling:
clang has no attribute that sets a local's `DW_AT_name` (my reading of the
attribute list; a question, not a run), so it would mean rewriting DWARF after
the compile, unrun.

design.md Part 8 item 12 (`grep -n "Every name passes through a mangler"
docs/design.md`, line 3837) already says this in prose: *a valid Heroes
identifier can collide with a C keyword or a libc symbol, and without the
mangler a correct program fails to compile for reasons invisible to its
author*. The table above is that sentence run, with the three cases it does
not name: a collision that compiles and renames the variable in silence
(`stdin`), one from a binding header (`true` via raylib.h), and one on one
platform only (`linux`).

## Route H at its cheapest: a typedef, no ABI move (00:23 to 00:26)

`build/c/routeh.c`: `typedef HeroArrayHeader h_arr_int64;`, a local declared
`h_arr_int64 *xs = hero_array_new(&hero_desc_int, 2);`, passed to
`hero_array_len` and `hero_array_decref` with **no cast**: `clang -std=gnu11
-Wall -Wextra -g -O0 -I runtime routeh.c runtime/runtime.c` printed nothing.
lldb shows the typedef, `(h_arr_int64 *) xs = ... [i64] len=0`, and an
embedded summary keyed on `h_arr_int64` matched it. So the element type can
reach the debug information through a typedef that is the same C type: the
runtime's prototypes, `HERO_RUNTIME_ABI` and every `extern` that passes an
array to C are untouched. This is what route C needs in order to key a
formatter per element type without Python; route B does not need it (it reads
the descriptor's symbol at run time).

(A second silent failure, worth an emitter's test: with the summary's size
byte too small, lldb loaded the record and printed the summary **truncated**,
`[i64] len=`, with no message.)

## Formatter bytecode, read and not run

The selectors this lldb's bytecode interpreter knows, from its own strings
(`strings -n 3 .../LLDB.framework/LLDB | grep -E "^@[a-z_]+$"`, the
formatter-relevant ones): `@type_summary`, `@get_num_children`,
`@get_child_at_index`, `@get_child_with_name`, `@get_child_index`,
`@get_type`, `@get_template_argument_type`, `@get_synthetic_value`,
`@get_non_synthetic_value`, `@get_value`, `@get_value_as_unsigned`,
`@get_value_as_signed`, `@get_value_as_address`, `@read_memory`,
`@read_memory_byte`, `@read_memory_unsigned`, `@read_memory_signed`,
`@read_memory_address` (and 32-bit forms). **No selector in that list makes a
value of a type at a computed address, casts, or looks up a symbol**, which is
what `[T]`'s in-line elements and the descriptor's name need. A `T?` arm and a
variant payload are existing children (`as`, then `ok`/`err` or `c_<case>`
picked by `tag`), so they look expressible. All of this is a reading of
strings, an inference, unrun: no assembler for the bytecode was at hand.

## Verdict

- `verdict`: **approve** the mechanism set B + C + H-by-typedef, with the
  editor's `initCommands` (J) or a `heroes` verb (D) as B's loader; **veto**
  scoped to one form of route E, renaming the emitted C identifiers to the
  author's names.
- `section`: design.md §1.11 and §4.19 (the FFI is the founding constraint;
  clang verifies every `extern` against the real header), Part 8 item 12 (the
  mangler); §1.12 for the formatter's own bounds.
- `experiment`: above, every one run: `heroes_lldb.py` over `shapes` at `-O0`
  on this Mac's lldb, Linux's lldb 22.1.8, and both lldb-daps (B); `embed.c`
  and a Heroes program's emitted C with `__lldbsummaries` records, loaded by
  itself on all four (C); `routeh.c` (H), zero diagnostics; `routee.c` (E),
  `errno` refused on both platforms, `linux` refused on Linux only, `stdin`
  renamed `__stdinp` in silence on this Mac.
- `argument`: Nothing measured needs the C boundary to move: no
  `HERO_RUNTIME_ABI` bump, no descriptor field (the name is in the symbol
  table unstripped and in DWARF's static-variable record stripped with a dSYM;
  without either there are no variables to show), no cast at a runtime call.
  Route C is the only one that needs no loader and no Python and shows in
  lldb-dap with no extension change, but summary strings cannot loop or
  branch, so B carries `[T]`, `{K: V}`, the live arm and the payload until
  bytecode is shown to build typed children. Route E as C names puts every
  Heroes local into the preprocessor's namespace, which `--include`d headers
  own: refused.
- `prediction`: **no step of M-typed-inspection moves `HERO_RUNTIME_ABI` off
  30 or adds a field to `struct HeroDesc`** (scored by `git diff
  m-<previous tag>..m-typed-inspection -- runtime/heroes_runtime.h` at the
  milestone's tag); and **a record's summary the compiler writes into
  `__lldbsummaries`/`.lldbsummaries` shows in lldb-dap's Variables pane on this
  Mac and the Linux image with `editors/vscode/extension.js` unchanged**
  (scored by a DAP case of the `lldb` suite, or the milestone's own transcript,
  at its close).
- `condition`: I move to **object** if a descriptor `name` field is proposed
  without a measured binary that shows variables and lacks the descriptor's
  name in both its symbol table and its DWARF; to **veto** if route E renames
  C identifiers, or if the link gains `--gc-sections` while the section lacks
  `retain`. I move to **C alone** (dropping B) if the formatter-bytecode
  section is shown, run, to produce `[Point]`'s elements as typed children.

## What I did not run

- the formatter-bytecode section (`__lldbformatters`): no assembler at hand;
- Windows: `lldb.exe` exits `0xC0000135` per `.github/workflows/ci.yml:487`
  (the brief's reading, not re-run); whether LLVM's lldb reads
  `.lldbsummaries` from a PE/COFF binary, and whether a summary-only route
  survives a Python-less `lldb.exe`, are questions;
- gdb on Linux (its `.debug_gdb_scripts` auto-load and pretty-printers);
- lldb's handling of a DWARF name different from the C name (no C spelling
  for it found; rewriting DWARF after the compile not tried);
- the frame filter (F) and the `$`-slot table: not this seat's mechanism, and
  no C was written for them;
- my formatter's shape coverage beyond what ran: `shapes.hero` (042's ten
  bindings) and `beside.hero` (`[[i64]]`, empty `[i64]`, `[Token]`,
  `{str: [str]}`, `[bool]`, `[f64]`, all printed as Heroes literals); not run:
  a function value, `[T?]`, a map keyed by a record, a match arm's payload
  binding (`h_shapes_Token_c_num`, which my record regex misses), map order
  (shown in bucket order, `{"b": [], "a": [...]}`);
- the timing of any of it: nothing here was timed.

## The evidence, kept

The copy is removed when the seat ends, so its sources and transcripts are in
`213-reports/ffi-pragmatist-work/` (the shape of `178-reports/ffi-pragmatist-work/`):
`heroes_lldb.py` (route B), `embed.c`, `embed-retain.c` and `gen_embed.py`
(route C), `routeh.c` (H), `routee.c` and `names.hero` (E), `dap.py` (the DAP
client), `shapes.hero` and `beside.hero`, and five transcripts: lldb on this
Mac over `shapes`, lldb-dap with `initCommands` on this Mac and on Linux, and
the embedded summary through lldb and lldb-dap on Linux. The copy's absolute
path is written `<copy>` in them.
