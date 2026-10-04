# Panel 192, the facts: what a string or a comment may hold, and what reaches C from a `str`

Written 2026-10-04 from 17:18 by the coordinator, then **repaired from 18:08
after the completeness critic's first pass**
(`docs/panel/192-reports/completeness-critic-briefs.md`; the text it read is
`00-facts-before-the-critic.md`). **Every fact below was run in this sitting
on its base and names its command**: by the coordinator, or by the critic in
its own copy of the base, as each line says. A fact carried from an earlier
sitting says so and is a question for the seat that can run it.

**The base.** Batch 10's closing commit `4c3524fb`. Its seed has sha256
beginning `c79ffd5ad005c301` (35,206,983 bytes, its fixpoint by `cmp`), and
the compiler built from it has sha1 beginning `8084f018f5387536`. The
coordinator measured on `<scratchpad>/192-base/` (`git archive 1dad1ac9` with
that seed copied in: `git diff --stat 1dad1ac9 4c3524fb` moves only records
and the seed). The critic measured on its own `git archive 4c3524fb`.

`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.

## F1. Which raw characters `check` refuses in a string and in a comment

Run 17:13 to 17:18 over `<scratchpad>/192-facts/cp<hex>-string.hero` and
`cp<hex>-comment.hero`, one pair per code point, each `function main()` over
`print("a<X>b")` or `# a<X>b` then `print(1)`. Command, for each:
`<base>/heroes check --brief <file>`. Output:
`<scratchpad>/192-facts/table-base.txt`, 55 code points: U+0000 to U+001F
but the line end U+000A, U+007F, U+0080, U+0085, U+009F, U+00A0, U+00AD,
U+061C, U+200B to U+200F, U+2028, U+2029, U+202A to U+202E, U+2066 to
U+2069, U+FEFF.

- **In a string, one is refused**: U+000D, `raw_carriage_return`, exit 1
  (panel 066). The other 54 check at exit 0, U+0000 (NUL) among them.
- **In a comment, none is refused**: all 55 at exit 0.
- **At `build` and run** (the critic, 18:00): the 54 build at exit 0 with no
  warning, and each program prints exactly its literal's bytes, the NUL
  included. The emitter escapes them all in the C (`compile.hero:83`'s
  U+0001 is `"\001I"` at `seed/heroes.c:3196`).

## F2. The specification's sentences

At the base:
- `:35-36`: *Syntax is ASCII-only; comments may contain any UTF-8, strings
  any but a raw carriage return or line end: a string is one line.*
- `:47-48`: *Six escapes, and no others: `\n` `\t` `\r` `\\` `\"` in a
  string, `\'` instead of `\"` in a character literal. Any other escape is a
  compile error.*
- `:64`: a `str` is an *immutable UTF-8 string, indexed and measured in
  bytes*; U+0000 is UTF-8.
- `:324-325`: *`args() -> [str]` (the arguments after the program name; one
  that is not UTF-8 aborts) · `args_checked() -> [str?]` (which does not)*.
- `:382`: *`s.cstr()` lends a `str` to C*; `:384` gives `validated_bytes()`
  to *a field of bytes*, read *to its first zero or the whole field*; `:386`
  says a `cstr` *promises a zero*.
- **No sentence says what `.cstr()` does with a `str` holding a NUL.**
  `grep -n -i 'nul'` prints six lines, all `null`, `nullptr` or `null_cstr`;
  `grep -n -i zero` prints `:385` and `:386`; the critic's wider grep
  (`zero|\\0|interior|embedded|truncat|\bNUL\b`) finds none.

## F3. design.md: the escape freeze, one spelling, and the free lend

`grep -n` over `docs/design/design.md` at the base:
- `:993`: the escapes in a string are `\n` `\t` `\r` `\\` `\"`.
- `:994-995`: *each character has exactly one spelling*, §4.15's
  canonical-form rule carried into the literals; `:2029`: *there is exactly
  one correct way to write any program*.
- `:1016-1018`: *The set is frozen: `\0`, `\xNN`, `\u{...}` and octal escapes
  need a panel, because they can produce an interior NUL, which silently
  truncates every C call and voids §4.20's guarantee that `.cstr()` is
  free.*
- `:547` and `:2629`: strings are always NUL-terminated, `len+1` allocated,
  so `.cstr()` is free with zero copies. `hero_str_cstr`
  (`runtime/parts/str.c:434`) checks the `str` for null and reads no byte.

## F4. A NUL reaches C, and the runtime's own doors open another file

Built and run 17:15 to 17:17 with the base's compiler, in
`<scratchpad>/192-facts/nul/`, and again by the critic at 17:43 in its own
copy. `nul.txt` holds the three bytes `61 00 62`.

| program | prints | what it means |
|---|---|---|
| `nulc.hero`: `s = "a<NUL>b"`, a raw NUL in the literal, `strlen(s: s.cstr())` through `extern "string.h"` | `1` | builds at exit 0, C reads one byte of three |
| `rf.hero`: `s = read_file(path: "nul.txt").must()`, then `s.len()` and the same `strlen` | `3`, `1` | a file is a door with no literal |
| `qi-write.hero`: `write_file` to `"out-" + <nul.txt's text> + ".txt"`, 11 bytes | `11`, `false` | it answers ok, and the file written is `out-a`, holding `written` |
| `qi-read.hero`: `read_file` of `<nul.txt's text>`, 3 bytes, beside a file named `a` | `3`, `false`, `the file named a` | it answers ok with another file's text |

The last two are panel 191's Q-i: a path holding a NUL reads or writes a file
the program never named, at exit 0.

**Where a NUL is already handled**:
- **One door refuses it, by an abort.** `runtime/parts/run.c:76`,
  `hero_run_arg`, panics *an argument contains a NUL byte* where a process
  argument holds one. Its comment (`:68-70`): *a `str`'s `len` is
  authoritative and `execvp` reads to the first NUL, so the two disagree
  exactly when an argument would silently become a shorter one.* (Read by
  the coordinator at 18:09; the first version of this file said no door
  asks, which was false.)
- **Where a NUL is correct today** (the critic, 17:43): `print` of
  `nul.txt`'s text writes `61 00 62 0a`, and `write_file` of it writes the
  three bytes. Four of the runtime's own calls of `hero_str_cstr` pass
  content with its length: `os.c:864` (`hero_file_write`), `:913`,
  `replace.c:440` and `:507`.
- **Where it is cut**: a `.must()` panic whose message holds a NUL prints
  the part before it; an `assert` prints `left:  a` (the runtime's printer,
  `failure.c:116`, `:135` and `:145`, through `%s`).

**Where a NUL can come from.** Measured: a raw NUL in a literal (F1), and
`read_file` (above). By the code, the critic's reading, a question to
measure rather than a premise:
- `validated()` copies with `strlen` (`str.c:333`);
- `validated_bytes()` stops at the first zero (`memchr`, `str.c:379`), so
  `[97, 0, 98].validated_bytes()` has length 1 (run by the coordinator at
  18:09, `<scratchpad>/192-facts/repair/esc.hero`);
- `args()` builds with `strlen` (`os.c:530`).

Every other constructor the critic found goes through `strlen`, but the file
reads and the shown builders (`os.c:649`, `:703`), by its vocabulary.

## F5. What already carries these characters

Over the 2,014 tracked `.hero` files at `4c3524fb`, a Python scan of each
file's characters, anywhere in the file (`<scratchpad>/192-facts/tracked-hero.txt`),
reproduced by the critic:

| characters | occurrences | files |
|---|---|---|
| tab, U+0009 | 69 | 8, all under `tests/golden/` (seven `check/` cases, 130 to 135, and a surface fixture) |
| C0 but tab, CR and line end, and DEL | 19 | 5: **`selfhost/cli/compile.hero`**, and four cases (244, 282, 289's `full`, a surface fixture) |
| bidirectional controls | 1 | 1, a surface fixture |
| other Default_Ignorable code points | 3 | 3 cases (216, 242, a surface fixture) |
| U+2028 or U+2029 | 1 | 1 case (216) |
| NUL | 0 | 0 |

**The compiler's own source holds two**: `selfhost/cli/compile.hero:83`,
`return "<U+0001>I" + headers_from.join(":") + "<U+0001>L" +
library.join(":")`, a cache key's separator. Four tracked `.hero` files are
not UTF-8, all defect 227's cases.

**By context** (the critic's approximate lexer, not the compiler's; to be
counted with `heroes lex`): the 19 C0 and DEL bytes are 4 inside string
literals and 15 outside any literal. In strings: `compile.hero:83`, 244's and
289's ESC after a backslash (already `unknown_escape`), one raw tab
(`surface-fixtures/json102/tab.hero:2`), 216's two group heads. **In
comments: none of any class**, so a refusal in comments moves no tracked
file by this approximation.

## F6. The refusals that exist, and their three classes

Run by the critic at 17:44, in a group head's string and a string:
- **a NUL in a group head** is `unwritable_name` (`head_names.hero:155`,
  `unwritable_byte`, with the line end), and `check --permissive` keeps it:
  **not a thesis rule**;
- **ESC, a tab and U+202E in a group head** are `unshowable_name`
  (`head_names.hero:191`, `unshowable_character`, panel 188), and
  `--permissive` drops them: **a thesis rule**;
- **a raw CR in a string** is `raw_carriage_return` (panel 066), and
  `--permissive` keeps it: **not a thesis rule**.

`diag.hero`'s thesis list (`:129-153`) holds `unshowable_name` and neither
other. `unshowable_character` refuses a C0 or C1 control, a code point of
Unicode's Default_Ignorable_Code_Point (`shown_char.hero:204`,
`DEFAULT_IGNORABLE`, 17 ranges read from Unicode 15.0.0), U+2028 and U+2029.

**The compiler's wider list**, `UNSEEN` (`shown_char.hero:95`, 38 ranges),
is what it calls shown as nothing or changing what follows. It adds NBSP,
U+2000 to U+200A, U+202F, U+205F, U+3000 and U+FFF9 to U+FFFB.
`extern "a<NBSP>b.h"` and `extern "a<U+3000>b.h"` check at exit 0 (the
critic, 18:05).

**What real text spells with**: `DEFAULT_IGNORABLE` holds ZWNJ and ZWJ
(U+200C, U+200D), which Persian and Indic text use, the variation selectors
(U+FE00 to U+FE0F, as in `❤️`) and the tag characters (U+E0000 onward, as in
a subdivision flag). The tracked corpus holds none of U+200C, U+200D, U+FE0F
or U+E0001 (the critic's scan).

## F7. The counts a route would move

**`.cstr()`** (`xargs grep -c '\.cstr()'`, lines, not call sites): 215 lines
in 100 files. The critic's split:

| tree | lines | files |
|---|---|---|
| `selfhost/` | 81 | 18 |
| `examples/` | 9 | 3 |
| `tests/golden/` | 77 | 58 |
| `tests/harness/` | 24 | 4 |
| `docs/panel/` | 22 | 16 |
| `archive/` | 2 | 1 |

`grep -o` counts 234 occurrences. The critic's approximate lexer places 163
in code (28 of them in `docs/panel/` and `archive/`, which no gate builds), 45
in comments and 26 inside strings, two of which are real lends: the prelude's
`read_file` and `write_file`, written as text in
`selfhost/library_source.hero:208` and `:222`. **A fallible lend's price is
counted with `heroes lex`, by the seat.**

**The runtime's calls that hand a name to the operating system**: `grep -n
-E` for `fopen`, `opendir`, `FindFirstFile`, `stat(`, `lstat(`, `unlink`,
`rename(`, `mkdir(`, `rmdir`, `execvp`, `posix_spawn`, `CreateProcess`,
`getenv`, `chdir`, `realpath`, `readlink`, `symlink` and `_wfopen` over
`runtime/parts/*.c` prints 60 lines, comments included. With comment lines
dropped it prints 34: `dir.c` 2, `fs.c` 10, `os.c` 4, `replace.c` 15, `run.c`
2, `str.c` 1. The critic's wider vocabulary (`open(`, `CreateFileA`, `chmod`,
`link(`) finds at least 21 more. **The list is a vocabulary**, so the
enumeration is the ffi-pragmatist's, from the code. `spawn.c` starts threads
and hands no name to the system. Every door of `hero_os.h` that takes a name
takes `const char *`, so the prelude and the compiler lend `.cstr()` first;
`hero_run_arg` alone takes the `str`.

## F8. The specification's budget

`<base>/heroes measure spec/heroes-spec.md`, 17:16: `real 9392`
(claude-opus-5, measured 2026-10-03, the binding number), headroom 848
against the 10,240 ceiling, of which the FFI floor mortgages 60. The critic's
vendored-table maximum for the blind arms' drafts: A 7,117, B 7,171, C 7,165,
lower bounds.

## F9. `args()` and a lone surrogate on Windows: two conversions, one refusing

Carried from panel 191 (`docs/panel/191-reports/`), unrun here, the box held:
- an argument holding a lone surrogate, `x<D800>y`, reaches `args()` as
  `x?y` at `7f4c0cc5`, and as `x<U+FFFD>y` under the UTF-8 manifest; both are
  valid UTF-8, so `:324-325`'s *aborts* does not happen;
- **the ffi-pragmatist's** strict conversion of the UCRT's wide `argv`
  (`WC_ERR_INVALID_CHARS`) refused it with error 1113
  (`ffi-pragmatist.md:447-448`);
- **the compiler-engineer's** `__wargv` row converted by
  `hero_win_name_bytes`, a lone surrogate kept as its WTF-8 bytes
  (`compiler-engineer.md:258`, `:454`), so the runtime's own check can
  refuse it.

**On this Mac** (the critic, 17:52): `args()` over the bytes `78 ed a0 80 79`
(the WTF-8 of `x<D800>y`) and over `x<FF>y` panics at exit 134 with
*hero_str_from_bytes: not well-formed UTF-8*, and `args_checked()` answers
`not_text` for both. **To be run again on this sitting's base by the seat
that takes Q5.**

## F10. ESC is writable today, at run time, and the message steers elsewhere

Run by the coordinator at 18:09 (`<scratchpad>/192-facts/repair/`):
- **The run-time route**: `esc: [u8] = [27]`, then
  `esc.validated_bytes().must()`, prints `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b
  30 6d 0a` around `ERROR`, and it cannot make a NUL (above). The compiler
  writes its own control characters this way, 13 calls in 9 files (the
  critic: `ir/print.hero:591`, `refused_runs.hero:182`, `lexer.hero:354`,
  four in `cli/json_text.hero`), and raw in one place only,
  `compile.hero:83`. **The spec does not say a `[u8]` answers
  `validated_bytes()`** (`:384` gives it to *a field of bytes*).
- **The message a writer meets**: `print("\x1b[31mERROR")` checks at exit 1
  with *error[unknown_escape]: `\x1b` is not an escape sequence, a string
  holds its characters as themselves, and this language has no escape for
  one by its code; write `\\` for a literal backslash*, and *fix (guess):
  escape the backslash: `\\x1b`*. The critic found the same for `\u{1b}`,
  `\u001b`, `\e` and `\033`. The sentence points a writer at a raw control
  character, defect 251's trap.

## F11. The runtime prints ESC raw to the reader

Run by the critic at 18:01: a `.must()` panic message holding ESC writes `1b
5b 32 4a`, a clear screen, raw to stderr; so does an `assert` message; and
`heroes test` prints a failing test's title with its ESC raw, `FAIL "title
<1b>[2J here"`. The runtime's printer (`failure.c:116`, `:135`, `:145`) and
the test runner; defect 244 writes such a character by its code in a
diagnostic, and defect 290 in the dumps.

## F12. The tag

`docs/work/defects/` at `4c3524fb` holds 78 files: 5 `blocking` (231, 238,
292, 323, 336) and 2 `systemic` (245, 283). 231, 292 and 323 close after
batch 10's platform legs, and 238 and 336 are lane b11-windows's. **245 and
283 wait on this sitting.**
