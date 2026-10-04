# Panel 189, the compiler-engineer's report

Opened 2026-10-04 00:12 by `date`. Written as I go; a section marked
*in progress* is not a verdict yet.

## My copies

`<scratchpad>/189-compiler-engineer/trunk/` is `git archive 7d9f2e8f`, and
`<scratchpad>/189-compiler-engineer/lane/` is `git archive 6ee963e7` (lane
b8-source's tip). Each compiler built from its own seed: both seeds hash
`3bc3aa8bd2b5ba65`, both compilers `958320f39dee0b9e`, as the brief says. **The
lane's seed is the trunk's** (the batch rule regenerates the seed only at the
batch's close), so the lane's compiler of ITS `selfhost/` is built from it with
`./heroes build selfhost/main.hero`: `lane/heroes-lane0`, sha256 begins
`4ed59835c9867341` (00:14). Every lane measurement below names which binary
ran. `lane-base/` is a second, untouched `git archive 6ee963e7`, the base the
patch is taken against (`diff -ruN`); I made no git repository in a copy (the
guard refused the `git add -A` an empty scratch repository would need, and I
did not route around it).

## Resumed 2026-10-04 01:45 (`date`), after the session limit

What my last commands left, read before building on it: `diff -rq lane-base
lane` names twelve edited files and one new one (`runtime/hero_os.h`,
`runtime/parts/os.c`, `runtime/parts/str.c`, `selfhost/source.hero`,
`selfhost/source_extent.hero`, `selfhost/module/reading.hero`,
`selfhost/modules.hero`, `selfhost/lexer.hero`, `selfhost/cli/input.hero`,
`selfhost/cli/check.hero`, `selfhost/cli/probe.hero`, new
`selfhost/not_text.hero`). The last edit (00:53:52, `probe.hero` and a helper
in `not_text.hero`) was applied whole (`diff -u` shows its three
replacements); `lane/heroes-route` (00:52:05) predates it, so it is rebuilt
before anything is measured on it. `measure.hero` and `mutate.hero` were not
yet edited. Lane b8-source's tip moved to `8a989fc2` meanwhile, touching only
`tests/harness/suite_fixes.hero` (the coordinator); the copy stays at
`6ee963e7`.

## Item 1: F1 on my trunk copy, widened (done 00:21 to 00:24)

31 cases under `<scratchpad>/189-compiler-engineer/cases/`, each written by
`printf '%b'` (UTF-16 by `iconv`, a binary as the first 512 bytes of the
trunk's own compiler), run with `trunk/heroes check p.hero`:

| case | the bytes | trunk `check` |
|---|---|---|
| `comment-latin1`, `string-latin1`, `byte-ff`, `lone-cont`, `first-byte`, `truncated-eof` | F1's six | **2**, *cannot read `p.hero`* |
| `truncated-mid` | `E2 82` then `b and more` on its line | **2**, the same |
| `overlong`, `overlong3` | `C0 AF`, `E0 80 AF` | **2** |
| `surrogate` | `ED A0 80` in a string | **2** |
| `past-max`, `lead-f5` | `F4 90 80 80`, `F5 80 80 80` | **2** |
| `several-lines` | Latin-1 on lines 1, 2, 5, 6 (two on 6) | **2** |
| `char-literal`, `fstring` | `0xE9` in `'...'` and in an `f"..."` piece | **2** |
| `ident-latin1` | `caf` + `0xE9` as a name | **2** |
| `win1252` | `0x93 0x94 0x80` in a string | **2** |
| `mixed` | a valid `é` and a Latin-1 `0xE9` on one line | **2** |
| `crlf-latin1` | Latin-1 with CRLF line ends | **2** |
| `plus-type-error` | Latin-1 comment and an unrelated type error | **2** |
| `utf16le-bom`, `utf16be-bom`, `utf16le-nobom` | UTF-16 with and without a mark, holding `é` | **2** |
| `binary` | a Mach-O head | **2** |
| `used` | F1's: `geom.hero` holds `0xE9` | **1**, *unknown_module ... that file is not there* (false) |
| `used-deep` | the same, two `use`s down | **1**, the same falsehood for `b` |
| `utf16le-nobom-ascii` | UTF-16 LE, no mark, ASCII only: valid UTF-8 with NULs | **1**, 29 `unexpected_character` for U+0000, raw NULs in the gutter |
| `utf8-bom` | `EF BB BF` first | **1**, U+FEFF at 1:1 |
| `valid-ident` | `café` in UTF-8 as a name | **1**, `unexpected_character` |
| `valid-fffd`, `nul-comment` | a real U+FFFD in a string; a NUL in a comment | **0** (controls) |

Every verb, on `several-lines`, trunk: `lex`, `lex --dump-tokens`, `lex
--dump-tokens --json`, `parse`, `parse --dump-ast`, `check` and its `--json`,
`--brief`, `--permissive`, `--apply`, `--apply --in-place`, `--dump-scopes`,
`build`, `build -o q`, `--emit-c`, `--dump-ir`, `run`, `test`, `fmt`, `fmt
--in-place`, `probe`, `measure`: **22 of 22 exit 2, *cannot read `p.hero`***,
the file unchanged (`cmp`); `mutate` over a folder holding it: exit 2,
*cannot read ...* without backticks. On the lane's compiler (`heroes-lane0`)
`used` and `used-deep` are *cannot read `geom.hero`* / *`b.hero`* at **2**
(defect 236's repair), every other row as the trunk.

**Two shapes beside 227 with another cause**, measured here: a UTF-16 file
with no byte-order mark and ASCII content is valid UTF-8 and is told as 29
`unexpected_character` U+0000 (the read succeeds, so it is not 227's cause);
and the gutter prints a line's control bytes raw (that run sent 29 lines of
NULs to the terminal). Both are for Q6.

## For the ffi-pragmatist: the runtime diff (written 01:49)

`<scratchpad>/189-compiler-engineer/route-runtime.diff`, 273 lines, against
`6ee963e7`'s `runtime/` (`diff -ruN lane-base/runtime lane/runtime`):
`runtime/parts/str.c` (the validator's judge of one sequence split out as a
`static` helper, `hero_utf8_valid`'s ASCII fast path kept inline and its
answer unchanged), `runtime/parts/os.c` (the file read split out once, so
`hero_file_read` and the new read share it; the new `hero_file_read_shown`),
`runtime/hero_os.h` (its declaration). No other runtime file moves;
`HERO_RUNTIME_ABI` stays 26 (item 5 below). The whole route's patch is named
at the end of this report.

**Final, 02:40**: the runtime diff did not move after 00:35; its sha256
begins `34db6e221da8d682`. The whole patch, `<scratchpad>/189-compiler-engineer/route.diff`,
sha256 `408bd54bebf214a2`, regenerated against the final copy and equal to
it (`cmp`); the case for the platforms is
`tests/golden/run/fixedbugs-227-the-shown-read-names-every-byte.hero` in it.

## Item 3: no writer receives a text with replaced bytes (measured 01:47)

On the route's compiler (`lane/heroes-route`, rebuilt 01:46), two cases
holding the `certain`-fixable mistake `Room.name` for `Room::name`
(`cases/writer-root`, `cases/writer-module`), in copies under
`work-writers/`:

| case | `check --apply` | `check --apply --in-place` | `fmt` | `fmt --in-place` | the files after (`cmp`) |
|---|---|---|---|---|---|
| the root holds `0xE9` in a comment | exit **1**, stdout 0 bytes, the `not_text` diagnostic | exit **1**, stdout 0 | exit **1**, stdout 0 | exit **1**, stdout 0 | unchanged |
| the root is text, its `use`d `geom.hero` holds `0xE9` | exit 0, stdout 144 bytes: the root's own UTF-8 text | exit 0, *p.hero unchanged: no `certain` fix applies to its 1 diagnostic(s)* | exit 0, the root | exit 0 | both unchanged |

The alternative, measured: the same root with its `0xE9` already replaced by
U+FFFD, as a read that replaces and goes on would hand it to the checker,
given to the trunk's `check --apply --in-place`: *rewrote p.hero*, exit 0,
and the file now holds `EF BF BD` where its author's `E9` was. That is F8's
hazard, and the route closes it twice: the file is never lexed, so no stage
reaches `--apply` with its text, and `check --apply` refuses a root that holds
such a byte before any round (`not_text.in_root`). A third, a tripwire:
`source_extent.user_text`, which every writer goes through (`cli/certain`,
`cli/check`, `cli/syntax_cmds`, `print/fmt`, `print/guard`, grepped), asserts
that no fault lies in the root, so a writer added later without the question
stops by name instead of writing U+FFFD.

## Q3 measured: how many, what after, and what is written (01:48)

`<scratchpad>/189-compiler-engineer/q3.py` over every case that is not text
(28): the count of diagnostics each grouping gives, whether the caret of each
line's first such byte lands on the same column under one U+FFFD per byte,
per maximal subpart (CPython's decoder) and per run, and what lexing past the
bytes adds (the trunk's `check --brief` on the per-byte shown text, the files
around it as they are):

| case | per byte | per subpart | per run | **per line** | first caret equal in all three | lexed past: exit | what lexing past adds |
|---|---|---|---|---|---|---|---|
| `binary` | 23 | 23 | 16 | 1 | yes | 1 | `unexpected_character` x412 |
| `byte-ff`, `comment-latin1`, `crlf-latin1`, `fstring`, `lone-cont`, `string-latin1`, `truncated-eof`, `used`, `used-deep` | 1 | 1 | 1 | 1 | yes | **0** | nothing: checks clean |
| `mixed` | 1 | 1 | 1 | 1 | yes | **0** | nothing |
| `char-literal` | 1 | 1 | 1 | 1 | yes | 1 | `char_literal` x1, **false**: one Latin-1 byte fits a `u8`, U+FFFD does not |
| `first-byte`, `ident-latin1` | 1 | 1 | 1 | 1 | yes | 1 | `unexpected_character` x1, a second message for the same byte |
| `lead-f5`, `past-max` | 4 | 4 | 1 | 1 | yes | 0 | nothing |
| `overlong` | 2 | 2 | 1 | 1 | yes | 0 | nothing |
| `overlong3`, `surrogate` | 3 | 3 | 1 | 1 | yes | 0 | nothing |
| `truncated-mid` | 2 | **1** | 1 | 1 | yes | 0 | nothing |
| `several-lines` | 5 | 5 | 5 | 4 | yes | 0 | nothing |
| `win1252` | 3 | 3 | 3 | 1 | yes | 0 | nothing |
| `plus-type-error` | 1 | 1 | 1 | 1 | yes | 1 | `type_mismatch` x1, real |
| `writer-root`, `writer-module` | 1 | 1 | 1 | 1 | yes | 1 | `dot_on_a_record_name` x1, real |
| `utf16le-bom`, `utf16be-bom` | 3 | 3 | 2 | 2 | yes | 1 | `unexpected_character` x31 |
| `utf16le-nobom` | 1 | 1 | 1 | 1 | yes | 1 | `unexpected_character` x29, `expected_declaration` x1 |

What it says, each a count and not an inference:

- **The caret is exact under every convention in 28 of 28**, because a
  line's first such byte has only text before it on its line. One diagnostic
  per line, its caret there, makes the per-byte-or-per-run question moot for
  every position the compiler states; it decides only how many U+FFFD the
  gutter shows after the caret. The route takes one per byte (the C is the
  simplest, and the marks then name every byte).
- **Per line is never more than per byte, and is fewer in 9 of 28**; per file
  is one, and names one place. Where the bytes are a whole file's encoding,
  the reading note (Q2) is what makes the repair one turn under any grouping;
  where they are stray bytes in a UTF-8 file, only per line names every place
  (below, `mixed-several`).
- **Lexing past adds a real mistake in 3 of 28, and a false, duplicate or
  flooding one in 7 of 28**; in 14 the shown text checks clean at exit 0,
  which is the measure of how far the shown text is from the author's program
  (a string holding U+FFFD where a byte was). With F8's writer hazard above,
  that is why the route never lexes such a file, and lexes every other file
  of the compilation as it always did.

**Per line against per file, the one case that tells them apart**
(`cases/mixed-several`, 01:51): a UTF-8 file (`café` in UTF-8 on line 1)
with Windows-1252 quotes pasted on lines 4, 7 and 9. The route prints three
diagnostics, at 4:12 (`0x93 and 0x94`), 7:9 (`0x92`) and 9:12 (`0x93 and
0x94`), the first carrying *the rest of this file is UTF-8, so these bytes
were likely pasted in from another encoding: 0x93 is `“` (U+201C) in
Windows-1252*. Per file names one of the three places; converting the whole
file from Windows-1252, which is what an encoding reading alone suggests,
would turn the UTF-8 `é` of line 1 into `Ã©`. Only per line serves a
one-turn repair here.

## Q6 measured: the shapes beside (01:52 to 01:55)

Run on the trunk's compiler and the route's (`lane/heroes-route`), each case
in `<scratchpad>/189-compiler-engineer/q6/`; the runtime and header rows
re-run on the final build at 02:36, the same (`q6/hdr4`). *227's cause* is the shared
brief's: a read that is not text taken as unreadable or absent.

| shape | trunk | route | 227's cause? |
|---|---|---|---|
| the runtime's `runtime.c` holds one Latin-1 byte (`q6/rt-root`) | `build` exit 2, *cannot find the Heroes runtime ... set HEROES_RUNTIME=<dir>* (it is set); `doctor` *not found* | `build` exit 0 and the program prints `1`; `doctor` *ok* | **yes**, `cli/toolchain.hero:85` asked presence by a read's success; repaired |
| a part, `parts/os.c`, holds it (`q6/rt-part`) | `build` exit 2, *the bytes of .../parts/os.c are not UTF-8*, a valid C runtime refused; `doctor` *not found* | exit 0, prints `1`; `doctor` *ok* | **yes**, `cli/runtime_key.hero:41,45`; repaired: the key is over the part's bytes (`reading.key_of`) |
| a C header holds it (`q6/hdr2-*`, `hdr3-*`) | an identical second build rewrites **1** object (ASCII control: 0) | **0**; and changing only that byte (`E8` to `E9`) rewrites **1**, so the key is exact | **yes**, `cli/deps.hero:100` digested it `absent`; repaired |
| `mutate`'s `main.hero` presence check (`cli/mutate.hero:139`), run 01:56 on `q6/mutroot` (a program whose `main.hero` holds `0xE9`, its module `geom/area.hero` text) | exit 2, *cannot read prog/main.hero*: the corpus read (`:96`) refuses the file before the presence check can ask | exit 2, *`prog/main.hero` is not UTF-8: the byte 0xE9 at line 1, column 6 is the first of 1 that are not* | the same cause in the code, **unreachable as its own shape** while `:96` reads every file first; asked through `reading.source_text` anyway, so it cannot become one |
| `pkg-config` answers bytes that are not UTF-8 (the coordinator's reproducer, copied to `cases/pkgconfig`; `pkg-config` prints `-DX=caf` `E9` ` -I/nowhere` and `caf` `E9`) | `build` **exit 0**, the package's flags dropped in silence, `caf`+`E9` included, a word `filter_words` would refuse | the same: **not repaired here** | **yes**, `cli/libraries.hero:298` took it as empty. The route's `reading.source_text` and `not_text.first_said` serve its repair in about five lines, but `libraries.hero` reads **297 of 300** and lane b8-ffi holds it for defect 237 (the coordinator); priced, not built |
| `HEROES_RUNTIME` is not UTF-8 (`q6/envtest`) | with a `./runtime` beside the program, `build` **exit 0 against `./runtime`**, the setting ignored without a word; without one, exit 2 *set HEROES_RUNTIME=<dir>* (it is set) | the same: not repaired | **the same cause in an environment value** (`cli/process.hero:214` `validated(...).default("")`), not a file read; a false message and a silent wrong runtime. Filed apart unless the sitting widens 227 to it |
| the CLI's own argv not UTF-8 | exit 2, *argument 2 is not UTF-8 ...* with a note for a program's author (the critic's) | the same | **no**: told, not taken for absent |
| UTF-16 with no byte-order mark, ASCII content (`cases/utf16le-nobom-ascii`) | exit 1, 29 `unexpected_character` U+0000 | the same | **no**: valid UTF-8, the read succeeds |
| the gutter prints a line's control bytes raw (`q6/esc`: a valid file, `ESC[2J ESC[31m` in a comment on a line with a type error) | the terminal receives `ESC[2J` (clear screen) and the colour codes | the same; on the route a binary's line reaches it too, once (the NUL rule tells such a file once) | **no**: the renderer's (`diag_render.source_line`); filed apart |
| a file or directory NAME not UTF-8 | cannot be made on this Mac (the critic: APFS errno 92) | unrun | Linux only, the ffi-pragmatist's |

**Found beside, another cause** (`q6/failmsg`, 01:54): `hero_failure_not_text`
(`runtime/parts/failure.c:101-108`, the failure of `f.validated_bytes()`)
stores *these bytes are not valid UTF-8*, 31 characters, in a `char b[31]`
and gives its length as `sizeof(b) - 1`, 30. A program printing `e.msg`
gets `[these bytes are not valid UTF-] 30` (run); `.must()` prints all 31
through `%s` because the byte after the array is the struct's zero padding
(an ASan build stays silent). C accepts a literal that fills its array exactly
and drops the terminator without an error, so the file's own comment
(*lengthening it is a clang error*) does not hold at the exact fit;
`clang -Wall -Wextra` names it (`-Wunterminated-string-initialization`), the
plain build does not. A truncated message a program sees: for the coordinator
to file; the repair is `char b[32]`.

## Item 2: the route, built (00:30 to 02:33)

Built in `lane/` on top of `6ee963e7`; its compiler is `lane/heroes-route`,
rebuilt from `selfhost/` by `heroes-lane0` after the last edit (02:33). What
it does, in the order a file meets it:

1. **The read keeps what a `str` can hold** (`runtime/parts/os.c`,
   `hero_file_read_shown`): for a file that read whole and is not UTF-8, the
   text with each byte that begins no well-formed sequence replaced by one
   U+FFFD, preceded by MARKS, two characters per U+FFFD of the text in order,
   the byte in hexadecimal or `--` for a U+FFFD the file held; one read, so
   marks and text describe one version of the file. The judge of a sequence
   is one `static` function shared with `hero_utf8_valid`
   (`runtime/parts/str.c`), so the two can never disagree about a byte.
2. **The compiler holds it** (`selfhost/module/reading.hero`,
   `source_text`): a file that is not UTF-8 is no longer a failure; its text
   and its faults (`source.Fault`, offset and byte) are kept, and the
   `Source` carries every file's faults in the compilation's offsets
   (`source.Source.faults`, one constructor, `source.hero:159`).
   `cli/input.hero`'s two reads and `modules.load_text` read through it, so
   a `use`d module is held and told at its own file. The faults recorded are
   bounded and exact where told (every one of the first nine lines holding
   one, then the first of each further line), and **a file holding one is never
   lexed, not even for its `use` lines**: a root that is not text is told
   alone, a module that is not text is held and not followed (the section
   *A defect in my own route*, below, is why).
3. **The lexer tells it and reads nothing else of it** (`lexer.lex_files`):
   a file holding faults is an empty file carrying `not_text.told`'s
   diagnostics; every other file of the compilation is lexed as before.
   Every verb reaches its text through `lex_files` (`lex`, and
   `parse.parse_source` for `parse`, `check`, `build`, `run`, `test`, `fmt`,
   `probe`, `mutate`), so no verb needed its own branch.
4. **What is told** (`selfhost/not_text.hero`, new): one `error[not_text]`
   per line holding such a byte, its caret on the line's first one, at most
   eight lines and the last told saying how many more; a file holding a NUL
   byte told once; the first diagnostic carries the reading (Q2) and *nothing
   else in this file is read until it is UTF-8*; no fix.
5. **No writer receives the shown text** (item 3): `check --apply` refuses a
   root holding faults (`not_text.in_root`), `fmt` refuses a file with
   diagnostics as it always did, and `source_extent.user_text`, which every
   writer passes through, asserts there is no fault in the root.
6. **The readers that are tools, not compilations**: `measure` (a document)
   and `mutate` (a corpus) stop at exit 2 naming the first byte, its line and
   column (`not_text.first_said`), where they said *cannot read*; `probe` is
   a compilation reader and tells the diagnostics at exit 1.
7. **The readers of Q6 that share 227's cause**: the runtime's presence reads
   through `source_text`; the header digest and the runtime key through
   `reading.key_of`, the runtime's whole answer with its marks, exact and with
   no fault recorded (Q6 above).

**What every verb answers now** (`work-route-verbs`, 01:46, the
`several-lines` case): `lex`, `lex --dump-tokens`, `lex --dump-tokens --json`,
`parse`, `parse --dump-ast`, `check` and its `--json`, `--brief`,
`--permissive`, `--apply`, `--apply --in-place`, `--dump-scopes`, `build`,
`build -o q`, `--emit-c`, `--dump-ir`, `run`, `test`, `fmt`, `fmt
--in-place`: **20 of 20 exit 1 with `error[not_text]`**, stdout empty but
`lex --dump-tokens`' single `1:1 eof` row (8 bytes, 55 as JSON); `probe` exit
1, *refusing to probe a file with diagnostics*; `measure` and `mutate` exit 2,
*`p.hero` is not UTF-8: the byte 0xE9 at line 1, column 8 is the first of 5
that are not*; the file unchanged after all of them (`cmp`). The case table of
item 1, on the route (01:46): every row that was exit 2 is **exit 1,
`not_text`** (24 rows), `used` and `used-deep` are told at `geom.hero:1:4`
and `b.hero:1:4`, and the controls do not move (`valid-fffd` and
`nul-comment` 0; `valid-ident`, `utf8-bom`, `utf16le-nobom-ascii` exit 1 with
the trunk's own diagnostics).

**What it refuses that a real program needs: nothing measured.** No tracked
`.hero` file of `6ee963e7` moves (the census, item 6), and a U+FFFD a file
holds stays text (`valid-fffd`, and the golden control below).

**Its cost, by `.claude/hooks/ceiling.py`'s `code_lines` (the layout unit:
non-blank lines outside `test` blocks, comments counted)**, base against the
final route (02:33), every `selfhost/` file it touches:

| file | base | route | of | code | comment |
|---|---|---|---|---|---|
| `selfhost/not_text.hero` (new) | 0 | **207** | 300 | +152 | +55 |
| `selfhost/module/reading.hero` | 53 | **174** | 300 | +77 | +44 |
| `selfhost/lexer.hero` | 247 | 260 | 300 | +7 | +6 |
| `selfhost/source.hero` | 264 | 275 | 300 | +5 | +6 |
| `selfhost/cli/input.hero` | 54 | 63 | 300 | +3 | +6 |
| `selfhost/cli/mutate.hero` | 168 | 178 | 300 | +9 | +1 |
| `selfhost/cli/measure.hero` | 194 | 202 | 300 | +6 | +2 |
| `selfhost/source_extent.hero` | 36 | 44 | 300 | +2 | +6 |
| `selfhost/cli/deps.hero` | 281 | 285 | 300 | +1 | +3 |
| `selfhost/cli/check.hero` | 271 | 274 | 300 | +1 | +2 |
| `selfhost/cli/toolchain.hero` | 251 | 254 | 300 | +1 | +2 |
| `selfhost/cli/runtime_key.hero` | 77 | 80 | 300 | +1 | +2 |
| `selfhost/cli/probe.hero` | 295 | 296 | 300 | +1 | 0 |
| `selfhost/modules.hero` | 319 | **319** | **320** | +1 | -1 |
| **total** | | **+401** | | **+267** | **+134** |

Of the 267 code lines, **64 are the encoding reading** (`not_text.hero`'s
`reading`, `sequence`, `character`, `only_replacements`, `holds_nul` and the
two tables), the most discretionary part; about 25 serve the Q6 readers; 14
are a test helper outside `test` blocks (`not_text.shown_source`).
`scan.hero` (291 of 300) and `cli/compile.hero` (300 of 300) are not touched,
and no file passes its ceiling (`layout` whole, item 6). The runtime:
`parts/os.c`, `parts/str.c`, `hero_os.h`, `route-runtime.diff`, 273 lines
(comments included). The harness: `tests/harness/cases.hero`,
`suite_annotations.hero`, `suite_fixes.hero` (item 4). **The whole patch**,
`<scratchpad>/189-compiler-engineer/route.diff` against `6ee963e7` (`diff -ruN
lane-base/{runtime,selfhost,tests} lane/...`): 31 files, 1,787 lines added
and 72 removed, of which 961 are the run golden's blessed emission.

## Item 4: how the net holds such a case (measured 02:00 to 02:07)

Three routes, measured or priced:

- **Committed bytes in `tests/golden/check/`, read by the net as the compiler
  reads them: built.** Four cases (`fixedbugs-227-a-latin-1-byte-in-a-comment`,
  `-each-line-holding-such-a-byte-is-told` (a string, a character literal and
  an `f` piece), `-a-character-cut-short`, and the control
  `-a-replacement-character-the-file-holds-is-text`), the first three not
  UTF-8 on purpose, every diagnostic annotated in its own source (`#~`,
  `#~v`), each `.expected` the route's `--brief` output. Before the harness
  learned them: `check` 4 passed, but `annotations` 1 passed and 3 failed
  (*cannot read the case or its expectation*, F8's measurement) and `fixes` 1
  passed and 3 failed (*cannot read*): **the net had 227's own shape twice**.
  The repair is the same read: `tests/harness/cases.hero` binds
  `hero_file_read_shown` and gives `shown(path)` (the text and whether it is
  text), `suite_annotations` reads a case through it (the shown text keeps
  every line and every `#~` mark), and `suite_fixes`' promise for a case with
  no answer file becomes, for one that is not text, *`--apply` hands nothing
  back*. After: **`check` 4/0, `annotations` 4/0, `fixes` 4/0** (narrowed to
  `227`); the three not-text cases pass only because their claims were read
  (an empty read would have failed them) and `--apply` printed nothing.
  `canonical` does not walk `tests/golden/check/` (its `SOURCE_DIRS`, and its
  own test asserts it), so it never meets them. `records`' walkers over every
  tracked file `continue` past a file that will not read
  (`suite_records.hero:2337`, `:2380`, `:3249`, `:3463`): they would skip
  these cases, by reading the code; `records` cannot run in a copy without a
  git repository, so that is **unrun**. The census sees the new diagnostic
  fire on them (item 6), which F8 said it could not on the tree as it is.
- **Bytes written through C at run time: built for the runtime's own read.**
  `tests/golden/run/fixedbugs-227-the-shown-read-names-every-byte.hero`
  writes `a`, `0xE9`, `b`, a U+FFFD, a line end and a cut `0xC3` through
  `fopen`/`fputc` (the precedent F8 names) and calls `hero_file_read_shown`
  from a program, so **every platform's leg runs the C**: *marks [E9--C3]*,
  the text 12 bytes, a UTF-8 file whole with no marks, a missing one
  `HERO_OS_NOT_FOUND`. `run` narrowed: 1 passed, 0 failed; its emission is
  blessed (`tests/emission/run-fixedbugs-227-...c`, by `build --emit-c`).
  A program cannot run the compiler, so this route cannot hold a
  diagnostic; it holds the read.
- **An escape the harness decodes: not built.** The stored case would be
  UTF-8 with an escape where each byte goes, and the `check` and `fixes`
  forms would have to write the bytes into a scratch tree at the case's own
  relative path before running, or every `.expected` would name a scratch
  path; and what the harness tests would then be a file that is not in the
  repository. It costs more than the route built and holds less.

**What I judge**: the committed bytes, read by the net through the same
runtime function, plus the run golden for the platforms. Cost: 38 lines
added and 5 removed in `tests/harness/` (29 of them code: `cases.hero` 14,
`suite_fixes.hero` 13, `suite_annotations.hero` 2), four check cases, one
run golden.

## Item 5: the runtime (02:08)

**A second function, not an offset added to `hero_utf8_valid`**: the
validator's per-sequence judge is split out (`hero_utf8_sequence`, `static`),
`hero_utf8_valid` keeps its answer and its inline ASCII path, and the read
both reads share is one `static` function (`hero_file_bytes`), so
`hero_file_read` behaves as before (`run` whole, 264 passed and 0 failed,
and `emission` whole, item 6). **`HERO_RUNTIME_ABI` stays 26**, on panel
089's reading (*adding a function is self-guarding*: a runtime without it is
an undefined symbol at link): the function is bound in the compiler's own
group (`selfhost/module/reading.hero`) and the harness's
(`tests/harness/cases.hero`), and the library (`selfhost/library_source.hero`)
is not touched, so no program's emitted C names it. Its tests: the run golden
above, and the compiler's own tests over the reading (`module/reading.hero`,
five). The C compiles under `clang -std=c11 -Wall -Wextra -Wshadow
-Wconversion` with one warning, `parts/failure.c:105`, which is not mine
(Q6, found beside). **For the ffi-pragmatist**: `route-runtime.diff` (above),
and the run golden is the case to run on Linux arm64 and the Windows box.

## A defect in my own route, found and repaired (02:12 to 02:30)

Measured on the 8.4 MB compiler binary named as a source
(`q6/bigbinary/big.hero`, 3,831,070 of its bytes not UTF-8, 34,842 line
ends), by `/usr/bin/time -l`'s peak memory and instructions retired (no
wall-clock figure is given, by the shared brief's rule):

| build | answer | peak memory | instructions retired |
|---|---|---|---|
| trunk | exit 2, *cannot read `big.hero`* | 10 MB | 31 million |
| route as first built (02:12) | exit 1, the right diagnostic, after outlasting my command's 600-second limit | **1.2 GB** | **6,587 billion** |
| route, the file never lexed for discovery (02:27) | the same | 356 MB | 24.2 billion |
| route, faults bounded (02:30), **final** | the same, *31950 more lines* still exact | **62 MB** | **8.5 billion** |

The cause, read from the system sampler on the first megabyte: discovery.
`modules.load_text` lexes every file's text to find its `use` lines
(`module/paths.uses_of`), and it lexed the shown text of a binary root, where
every control byte is a refused character whose diagnostic names it through
`shown_char.named`, which rebuilds its `UNSEEN` table at each call (233 of the
sampled frames). Two repairs: **a file that is not text is never lexed, not
even for discovery** (`cli/input.read_compilation` tells a root that is not
text alone; `modules.load_text` holds a module that is not text and does not
follow its `use` lines, through `reading.followed`, so `modules.hero` stays
at 319); and **the faults recorded are bounded and exact where told**
(`reading.faults_of`: every fault of the first nine lines holding one, then
the first of each further line), with the header digest and the runtime key
given their own exact read that records no fault (`reading.key_of`). The
one-byte-encoding reading now asks the text (`not_text.only_replacements`)
instead of a fault count. What the table still shows, 62 MB for an 8.4 MB
file, is the runtime's shown text and its copies, linear in the file.

**Beside it, another cause, for Q6, measured at 02:36** (`q6/unseen`): on the
trunk, 32 KB of valid UTF-8 made of control bytes draw **31,600
`unexpected_character` diagnostics and 8.7 billion instructions retired**
(peak 47 MB), against 400 diagnostics and 179 million for 32 KB of letters:
one uncapped diagnostic per refused character, each naming it through a
table rebuilt at every call. UTF-16 without a byte-order mark is that shape
(the read succeeds). Not 227's cause; filed apart.

## Verdicts per question (written 02:10)

**Q1, the code and its class.** From the ceiling the name costs nothing: it
is a string, and the route decides on the runtime's own status,
`HERO_OS_NOT_TEXT`, so one state (*these bytes are not UTF-8*) is named once
whether a program meets it at run time or the compiler at compile time. I
support `not_text`; the precedents are the historian's and the spec's the
spec-warden's. **Its class: not a thesis rule**, and measured so:
`check --permissive` keeps it (exit 1, item 2's verb table; `diag.is_thesis_rule`
untouched). The control arm (design.md Part 11, the thesis checks switched
off) has no reading of such a file to fall back on: the only text there is is
the shown one, and Q3's table shows 14 of 28 such files checking **clean**
as shown text, a program holding U+FFFD where its author's byte was. A
refusal without which the compiler would build another program is not a
thesis rule.
**The routes listed to be refused**, priced: *accept the byte in a comment and
refuse it in a string* needs the scanner (`scan.hero`, 291 of 300) to know a
comment in a file it cannot lex, so the shown text lexed (Q3: false or
duplicate diagnostics in 7 of 28), and then a writer that hands the file back
with its bytes, which this compiler does not have: every writer takes a
`str` (`fmt`, `check --apply` through `source_extent.user_text`). What would
make that refusal wrong: a byte-preserving writer path. *An encoding
declaration or transcoding* (PEP 263's route) needs a declaration form (the
lexer and the spec), a transcoder per encoding in the runtime, and every
writer re-encoding on the way out: hundreds of lines and a spec form for a
file no model writes on purpose. What would make that refusal wrong: a
measured population of Heroes sources that must stay in another encoding.

**Q2, the message and its fix.** Built: the message names the byte or bytes
of the line and the rule (*the byte 0xE9 is not UTF-8, and a `.hero` file is
UTF-8 text*); the caret is the position; the first diagnostic's note is the
reading, which names a UTF-16 byte-order mark, a NUL byte (UTF-16 without a
mark, or a binary), a UTF-8 character cut short or one UTF-8 forbids, a file a
one-byte encoding wrote whole (*0xE9 is `é` (U+00E9) in Latin-1 and
Windows-1252; save the file as UTF-8*), or bytes pasted into a UTF-8 file
(*write in UTF-8 the character each stands for*), each worded as the guess it
is; a second note says nothing else of the file is read. **No fix, `certain`
or `guess`**, for a byte in a string and for one in a comment alike: a fix
applied is the shown text handed back, U+FFFD over every other such byte
(item 3's measurement), and which character a byte stood for is an encoding
guess, never `certain` by `.claude/rules/diagnostics-and-goldens.md`'s rule;
deleting a comment's byte would keep the program's meaning and still write
the shown text. The guess lives in a note, as prose. The reading costs **64
code lines**; whether it earns them is the blind arms' to measure.

**Q3** is measured above: **one diagnostic per line**, its caret on the
line's first such byte (exact under every convention, 28 of 28), at most
eight lines and the rest counted, a NUL-holding file once; **never lexed
past**, the other files of the compilation lexed as before; one U+FFFD per
byte; **no writer receives the shown text** (item 3).

**Q4, where it lives and what it costs**: items 2 and 5. It lands on
`6ee963e7` without moving `modules.hero` (319 of 320) and without touching
`scan.hero` or `cli/compile.hero` (300 of 300). Every reader of F2 is
covered, and the Q6 readers that share the cause and fit (the runtime's
presence and key, a header's digest). The platform runs are the
ffi-pragmatist's, on `route-runtime.diff` and the run golden.

**Q5, the specification.** Nothing in the compiler needs the sentence: the
rule is enforced in one place (`not_text.told`), and the message states it.
Whether a reader of the spec needs it, at what price, is the spec-warden's;
I have no cost objection either way.

## Item 6: the runs, on the final build (02:30 to 02:37)

Every count below is on `lane/heroes-route` as last built (02:31, after the
binary repair and the bound; one test literal corrected at 02:33, the
compiler's own tests re-run on it), the harness's own command, the output
read whole:

| run | result |
|---|---|
| my cases (`cases/`, 34: item 1's 31, `mixed-several` and the two writer cases) | the **29** that are not UTF-8 exit **1**, `not_text`; the **5** controls as the trunk (`valid-fffd`, `nul-comment` 0; `valid-ident`, `utf8-bom`, `utf16le-nobom-ascii` 1 with their own codes) |
| every verb (`work-final-verbs`) | 21 of 22 forms exit 1 with `not_text`, `measure` and `mutate` exit 2 naming the byte, the file unchanged |
| the writers (item 3) | as in item 3, re-run on this build, the files unchanged |
| `check` whole | **451 passed, 0 failed** (the lane's 447 and my 4) |
| `annotations`, narrowed to `227` | **4 passed, 0 failed** |
| `fixes`, narrowed to `227` | **4 passed, 0 failed** |
| `run`, narrowed to `227` | **1 passed, 0 failed**; `run` whole on the 02:05 build, 264 passed, 0 failed |
| the compiler's own tests | **1,133, all passed** (1,123 at `c898bd94` by its body, plus my 10) |
| `layout` whole | **5 passed, 0 failed** (its `budget` and `concat` checks included) |
| the net's own tests (I touched `tests/harness/`) | 200, 1 failed, **the same 200 lines as the pristine base copy** run the same way (02:05): the failing one is the citation check that asks git what is ignored, in a copy that has no git repository |
| `emission` whole | **744 passed, 0 failed** on the final build (02:40), as on the 02:05 build: no program's emitted C moved, the run golden's blessing included |
| the census, `check --brief` on the 1,918 tracked `.hero` files of `6ee963e7` and my 4 cases | **3 of 1,922 files move, exit 2 to exit 1, and they are the three new cases that are not UTF-8**, against the trunk's compiler and against the lane's own; every other file reads the same exit code and the same output digest. (One more file, `selfhost/module/reading.hero`, differed between my first and last census because I had edited it in between; the three compilers run on its present text agree, checked with every file the route touches.) |

**What the census sees, and what it cannot**: no tracked file of the tree is
outside UTF-8 (F8), so on the tree as it is the census proves the route moves
nothing, and only the committed cases show the diagnostic fire; it does not
see `--apply`'s refusal, a `use`d module, or a binary, which the cases above
and items 3 and 6 hold.

## The seat's verdict (written 02:37)

- `verdict`: **object**, to the proposal's letter on Q3 and not to its code,
  class or place. Its *"at the first byte that is not UTF-8"* should be **one
  diagnostic per line holding such a byte**, never lexed past, writers
  refusing; with that amendment I approve. **No veto**: no core construct and
  no ceiling breached.
- `section`: design.md §1.7 and Part 5 (no construct of the seven, nothing
  for the checker, the lowering or the backend: a read and the lexer's first
  word); §1.12 (no writer may receive the shown text; a binary must not
  exhaust the compiler); §4.17 (one turn); §1.1's ceiling.
- `implementation_cost`: `selfhost/` +401 layout lines in 14 files (267
  code, of which 64 are the encoding reading), `not_text.hero` 207 and
  `module/reading.hero` 174 of 300, `modules.hero` held at 319 of 320,
  `scan.hero` and `cli/compile.hero` untouched; `runtime/` one function and a
  shared validator judge (`route-runtime.diff`, 273 lines), ABI 26 kept;
  `tests/harness/` 29 code lines; four check cases and one run golden. The
  patch: `<scratchpad>/189-compiler-engineer/route.diff`, 31 files, against
  `6ee963e7`.
- `needed_for_self_hosting`: **no**. The compiler's own sources are UTF-8;
  this is a blocking defect's repair (§1.12, §4.17), not a form entering the
  language.
- `argument`: One diagnostic at the first byte names one place where a
  file can hold many, and the case that tells them apart is likely: stray
  bytes pasted into a UTF-8 file, where converting the whole file is wrong
  (`mixed-several`: three lines, one turn only per line). Per line costs
  about ten lines more, its caret exact under every replacement convention
  (28 of 28). Lexing past adds a real mistake in 3 of 28, a false,
  duplicate or flooding one in 7, and hands writers a lossy text (`--apply`
  wrote `EF BF BD` over `E9`). My first build missed a shape the proposal
  never names: a binary, 1.2 GB through discovery's lexing; never lexing
  and bounded faults made it 62 MB.
- `prediction`: at batch 8's gate, the round that lands 227 on this route,
  the census of `check --brief` over the tracked `.hero` files, the batch's
  compiler against the trunk's, moves **exactly the committed
  `fixedbugs-227-*` cases that are not UTF-8, exit 2 to exit 1, and no other
  file**, and `check` counts its base plus four. A fourth moved file, or a
  red `annotations`, `fixes`, `run` or `emission` traced to this route,
  falsifies it.
- `condition`: (1) the blind arms showing a single per-file diagnostic
  repaired in no more turns than per line, on a file whose bytes sit on
  several lines: then per file, ten lines fewer; (2) the ffi-pragmatist's
  Linux arm64 or Windows run of `route-runtime.diff` and the run golden
  disagreeing with this Mac: then the C is revised before any verdict holds;
  (3) a tracked-tree instrument I could not run here (`records` needs git)
  failing on a committed case that is not UTF-8: then the cases are held as
  the run golden holds bytes, written at run time, and the net loses its view
  of the rendered diagnostic; (4) a measured Heroes source that must stay in
  another encoding: then the refusal of transcoding (Q1) is wrong.

## What I did not run

The platforms (the ffi-pragmatist's, on `route-runtime.diff` and the run
golden); `records` (it needs a git repository; my copies have none, by
choice); `canonical`, `order`, `surface`, `probe`, `determinism`, `lines`,
`warnings`, `corpus` and `unsupported` whole; the seed's regeneration and its
fixpoint (the batch's close does those); any timing (the shared brief's rule:
the instruction counts above are not times); a file or directory name that is
not UTF-8 (Linux only); any paid run.

Closed 2026-10-04 02:40 by `date`.
