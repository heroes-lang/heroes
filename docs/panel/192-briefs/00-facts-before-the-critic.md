# Panel 192, the facts: what a string or a comment may hold, and what reaches C from a `str`

Written 2026-10-04 from 17:18 by the coordinator. **Every fact below was run
for this brief, on its base, and names its command.** A fact carried from an
earlier sitting says so and is a question for the seat that can run it.

**The base.** Batch 10's round tree at `1dad1ac9`, with the seed it regenerated
installed (sha256 beginning `c79ffd5ad005c301`, 35,206,983 bytes, its fixpoint
by `cmp`). The compiler built from that seed has sha1 beginning
`8084f018f5387536`. The coordinator's copy is `<scratchpad>/192-base/`
(`git archive 1dad1ac9`, the seed copied in). The sitting's archive is batch
10's closing commit, which carries that seed; `00-shared.md` names it.

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

## F2. The specification's sentences

`grep -n` over `spec/heroes-spec.md` at the base:

- `:35-36`: *Syntax is ASCII-only; comments may contain any UTF-8, strings
  any but a raw carriage return or line end: a string is one line.*
- `:324-325`: *`args() -> [str]` (the arguments after the program name; one
  that is not UTF-8 aborts) · `args_checked() -> [str?]` (which does not)*.
- `:382`: *`s.cstr()` lends a `str` to C*. No sentence says what a `str`
  holding a NUL does there (`grep -n -i 'nul'` finds `:386`'s field rule
  and no other).

## F3. design.md's escape freeze and the free lend

`grep -n` over `docs/design/design.md` at the base:

- `:993`: the escapes in a string are `\n` `\t` `\r` `\\` `\"`.
- `:1016-1018`: *The set is frozen: `\0`, `\xNN`, `\u{...}` and octal
  escapes need a panel, because they can produce an interior NUL, which
  silently truncates every C call and voids §4.20's guarantee that
  `.cstr()` is free.*
- `:547` and `:2629`: strings are always NUL-terminated, `len+1`
  allocated, so `.cstr()` is free with zero copies.
- `runtime/parts/str.c`: `hero_str_cstr` checks the `str` and returns its
  pointer, *the NUL is already there*; it reads no byte.

## F4. A NUL reaches C, and the runtime's own doors open another file

Built and run 17:15 to 17:17 with the base's compiler, in
`<scratchpad>/192-facts/nul/`. `nul.txt` holds the three bytes `61 00 62`.

| program | prints | what it means |
|---|---|---|
| `nulc.hero`: `s = "a<NUL>b"`, a raw NUL in the literal, `strlen(s: s.cstr())` through `extern "string.h"` | `1` | builds at exit 0, C reads one byte of three |
| `rf.hero`: `s = read_file(path: "nul.txt").must()`, then `s.len()` and the same `strlen` | `3`, `1` | a file is a door with no literal |
| `qi-write.hero`: `write_file` to `"out-" + <nul.txt's text> + ".txt"`, 11 bytes | `11`, `false` | it answers ok, and the file written is `out-a`, holding `written` |
| `qi-read.hero`: `read_file` of `<nul.txt's text>`, 3 bytes, beside a file named `a` | `3`, `false`, `the file named a` | it answers ok with another file's text |

The last two are panel 191's Q-i, run here: a path holding a NUL reads or
writes a file the program never named, at exit 0.

`grep -rn memchr runtime/` finds one site, `str.c:379`, reading a C field to
its first zero; no door asks a `str` for an interior NUL.

## F5. What already carries these characters

Over the 2,014 tracked `.hero` files at `1dad1ac9` (`git ls-tree -r
--name-only 1dad1ac9`, `<scratchpad>/192-facts/tracked-hero.txt`), a Python
scan of each file's characters, anywhere in the file, string or comment
alike:

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
library.join(":")`, a cache key's separator, written raw because no escape
writes it (F3). Four tracked `.hero` files are not UTF-8, all defect 227's
cases.

## F6. The refusal that exists: a group head's string

`selfhost/head_names.hero:191`, `unshowable_character` (panel 188): in a
`extern`, `link` or `package` string, a C0 or C1 control, a code point of
Unicode's Default_Ignorable_Code_Point, U+2028 or U+2029 is refused as
`unshowable_name`, a thesis rule. `selfhost/shown_char.hero` carries the
property as `DEFAULT_IGNORABLE` (17 ranges, read from Unicode 15.0.0, its
comment says) and a wider list, `UNSEEN`, for naming a character in a
message.

## F7. The counts a route would move

- `.cstr()` call sites in tracked `.hero` files (`xargs grep -c`): 215 in
  100 files; `selfhost/` 81 in 18, `examples/` 9 in 3, `tests/` 121 in 75.
- The runtime's calls that hand a name to the operating system, by `grep -n
  -E` for `fopen`, `opendir`, `FindFirstFile`, `stat(`, `lstat(`, `unlink`,
  `rename(`, `mkdir(`, `rmdir`, `execvp`, `posix_spawn`, `CreateProcess`,
  `getenv`, `chdir`, `realpath`, `readlink`, `symlink`, `_wfopen`: 34 lines
  in `dir.c` (2), `fs.c` (10), `os.c` (4), `replace.c` (15), `run.c` (2) and
  `str.c` (1). **The list is the coordinator's vocabulary**: `spawn.c`
  matched none of it, so the enumeration is a question, not a count.

## F8. The specification's budget

`<base>/heroes measure spec/heroes-spec.md`, 17:16: `real 9392`
(claude-opus-5, measured 2026-10-03, the binding number), headroom 848
against the 10,240 ceiling, of which the FFI floor mortgages 60.

## F9. Carried from panel 191, not run for this brief

On the Windows box at `7f4c0cc5` (`docs/panel/191-reports/compiler-engineer.md`,
the row table, and `ffi-pragmatist.md`): an argument holding a lone
surrogate, `x<D800>y`, reaches `args()` as `x?y`, and as `x<U+FFFD>y` under
the UTF-8 manifest; both are valid UTF-8, so F2's `:324-325` *aborts* does
not happen. The UCRT's wide `argv` with a strict conversion refused it with
error 1113 in both seats' probes, a mechanism neither built into a route.
**To be run again on this sitting's base by the seat that takes Q5.**
