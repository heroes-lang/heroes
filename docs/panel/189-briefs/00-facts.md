# Panel 189, the facts measured before the briefs

Measured by the coordinator on 2026-10-03 between 23:20 and 23:39 by `date`,
on the trunk at `7d9f2e8f` (the frozen head), whose compiler files equal
`9bd02fca`'s (`git diff --stat 9bd02fca 7d9f2e8f -- selfhost seed runtime
tests` is empty); the trunk's compiler was built from the seed at 21:41
(seed sha256 `3bc3aa8bd2b5ba65`, compiler `958320f39dee0b9e`). **Repaired on
2026-10-04 from 00:08 after the completeness critic's first pass**
(`docs/panel/189-reports/completeness-critic-briefs.md`, 26 items; the text it
read is `00-facts-before-the-critic.md`): four facts corrected, F3, F4 and F7
widened, F8 added from the critic's own measurements. Each fact names its
command or file. What was not run says so.

## F1. What the trunk does with a source that is not UTF-8 (this Mac)

Written by `printf '%b'` into `<scratchpad>/189-facts/<case>/p.hero`, then
`heroes check p.hero` (`HEROES_RUNTIME` the trunk's `runtime/`); every row
re-run by the critic on its own compiler, holding:

| case | the bytes | `check` | what it printed |
|---|---|---|---|
| `comment-latin1` | `# caf` and the byte 0xE9 in a comment | **2** | *error: cannot read `p.hero`* |
| `string-latin1` | `print("caf` 0xE9 `")` | **2** | the same |
| `byte-ff` | 0xFF inside a string | **2** | the same |
| `lone-cont` | a lone continuation byte 0x80 in a comment | **2** | the same |
| `truncated` | the file ends with 0xC3 (a sequence cut off) | **2** | the same |
| `first-byte` | 0xE9 as the file's first byte | **2** | the same |
| `valid-ident` | `café`, valid UTF-8, as an identifier | **1** | `error[unexpected_character]`: *`é` (U+00E9) is not part of the language's syntax*, at 2:8 |
| `used` | a `use`d module `geom.hero` holding 0xE9 in a comment | **1** | `error[unknown_module]`: *there is no module `geom` ... and that file is* [not there]: **false**, the file is there |

On `comment-latin1`, `build`, `lex`, `parse` and `fmt` each exit **2** with
*error: cannot read `p.hero`*. No line, no column, no byte, no code.

## F2. Where the CLI collapses the reason

`selfhost/cli/input.hero`: `read_compilation` (lines 24 to 29) and
`read_root_only` (**47 to 52**, corrected) answer any failure of `read_file`
with `fail(code: "unreadable", msg: "cannot read `" + path + "`")`, which the
verbs report at exit 2. **Three more verbs read through their own calls** (the
critic): `mutate` (`cli/mutate.hero:96`), `probe` (`cli/probe.hero:115`,
`:319`), `measure` (`cli/measure.hero:44`, which reads a document, not a
compilation). The runtime does distinguish: `runtime/parts/os.c:643`,
`read_file`'s buffer checked by `hero_utf8_valid` (`runtime/parts/str.c:248`,
a `bool` and no offset), sets `HERO_OS_NOT_TEXT` (`runtime/hero_os.h:50`), and
the library's `read_file` fails with code `not_text`
(`selfhost/library_source.hero:217`, *"the bytes of " + path + " are not
UTF-8"*).

## F3. `not_text` in the language's history (read from the records, not recalled)

- **Panel 087** (convened 2026-08-19, ratified 2026-08-23,
  `docs/panel/087-the-guard-that-could-not-fire.md`): `read_file` on a file
  that is not UTF-8 had aborted the program (exit 134); the sitting chose the
  conservative `read_failed` over a named `file_not_text`, naming its cost, a
  message *"mildly dishonest"* about a file that read perfectly well. **Its
  reversal condition named this sitting's reader**: *"a named reader in the
  same commit (`cli_input.hero:24-28` widened so `heroes check <a binary>`
  says why) plus a golden that fires the arm"* (its § Author's verdict).
- **Panel 089** (2026-08-24, `56251778`, `docs/panel/089-text-from-c.md`):
  `c.validated()` copies a `cstr` into a `str?`, `not_text` for bytes that are
  not UTF-8 (`selfhost/value_errors.hero:84`); **it held the runtime's ABI at
  15 when adding a runtime function**, *"adding a function is
  self-guarding"* (its line 150).
- **2026-09-03**, `24fbf441` (defects 001 and 002): `read_file` says
  `not_text`; that commit re-blessed all 182 emission files for one library
  extern constant (the critic).
- **Panel 162** (2026-09-18) cites `not_text` as *"the code `read_file`
  already returns"*.
- The compiler binds runtime functions in **its own extern groups**
  (`selfhost/cli/files.hero:18`, `cli/process.hero:37`), a placement that
  keeps a function out of every program's C (the critic).

So `not_text` is a code **a program sees** today (`read_file`'s,
`validated`'s and `validated_bytes`'s failure); no **compiler diagnostic**
carries it.

## F4. What the specification and design.md say

- `spec/heroes-spec.md` § 1, lines 35 to 36: *"Syntax is ASCII-only; comments
  may contain any UTF-8, strings any but a raw carriage return or line end: a
  string is one line."* Line 64: `str` is an *"immutable UTF-8 string"*.
  **Line 385, § 13, already names `not_text`**: *"`f.validated_bytes()` ...
  either fails `not_text`"* (bytes from C). No sentence says what a FILE that
  is not UTF-8 gets.
- `docs/design/design.md` § 1.10 (line 458): *"Language symbols are ASCII.
  Strings and comments are full UTF-8."*
- `heroes measure spec/heroes-spec.md`: `claude-legacy` 6,990, `cl100k_base`
  7,117, **real 9,392** (pinned for `claude-opus-5`, 2026-10-03; the blind
  seat runs `claude-opus-5-5`).

## F5. `unexpected_character` today

Built at `selfhost/scan.hero:297`, message `shown_char.named(ch) + " is not
part of the language's syntax"`; also at `scan.hero:144`, `:163`, `:184` and
`selfhost/backslash.hero:69`. It judges a CHARACTER outside a string or a
comment; F1's `valid-ident` row is its reach. Columns are counted in
characters (the critic: a caret at 2:20 where bytes would give 23).

## F6. The work in a lane beside this sitting (not on the trunk)

Lane b8-source (`.claude/worktrees/lane-b8-source`, batch 8, its tip
`6ee963e7`) repaired defect 236 at `c898bd94`: a `use`d module present but
unreadable is told *cannot read* at exit 2, as the root is, and there a `use`d
module that is not UTF-8 reads the same. **That commit rewrites
`cli/input.hero` (+40 lines), `modules.hero`, and adds
`selfhost/module/reading.hero`** (the critic), so the route this sitting adopts
lands on top of it. The lane's report on 227
(`<scratchpad>/batch8/source/report.md`) names the work the same under either
code: the runtime's UTF-8 check reporting the first bad offset; a read
handing back the text with each bad byte replaced by one character;
`cli/input.hero` giving exit 1; `modules.hero` telling a bad byte in a `use`d
module. Whether lines and columns stay exact under a replacement depends on
one character per bad byte or per run of them: unrun.

## F7. The budgets the repair meets

`tests/harness/suite_layout.hero` line 464: `"selfhost/modules.hero 320"`; the
coordinator's mirror of the suite's unit (a scratchpad script, not in the
tree; **the tracked counter is `.claude/hooks/ceiling.py`, and
`suite_layout` is the judge**) reads **317** on the trunk. `selfhost/scan.hero`
reads **291** of its 300 (the critic). `selfhost/parse/` is not expected to
move.

## F8. Measured by the critic in its first pass (its report, items 14 to 20)

- **A lossy text must never reach a writer**: `check --apply --in-place`
  rewrites a file while an unfixable error stays in it (exit 0, *rewrote
  q.hero*); under a read that replaces each bad byte, a file holding a bad
  byte and any certain-fixable mistake would be rewritten with U+FFFD where
  the author's byte was.
- **Two more readers take *not text* for absence, 227's own cause**: a
  runtime directory whose `runtime.c` holds one Latin-1 byte is told *cannot
  find the Heroes runtime ... set HEROES_RUNTIME=<dir>* (exit 2;
  `cli/toolchain.hero:85`), and a C header holding one is digested `absent`
  (`cli/deps.hero:100`), so every build recompiles the unit that includes it.
- **UTF-16**, with a byte-order mark or without, is *cannot read* at exit 2
  too; **a valid U+FFFD** in a string checks at exit 0 today, and a route
  must keep it so.
- **The harness cannot read such a case**: `annotations` and `canonical` both
  report *cannot read* for it (`suite_annotations.hero:180-187`,
  `suite_canonical.hero:109-113`); the precedent for holding such bytes,
  `tests/golden/run/fixedbugs-read-file-on-bytes-that-are-not-text.hero`,
  writes them through C at run time.
- 0 of 1,910 `.hero` files in the tree are outside UTF-8, so the census
  cannot see the new diagnostic fire. A file or directory name that is not
  UTF-8 cannot be created on this Mac (the filesystem refuses it): Linux only.
