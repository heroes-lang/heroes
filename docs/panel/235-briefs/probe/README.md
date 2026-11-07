# Panel 235's probe: the facts the briefs state, each with its command

Run 2026-11-07 in the lane `235-base` at `9d894fe68` (parents `6656268fe` and `9791d4787`, tree `f01b6059aeb1f0f2d86c085fecbdb7eb0a688f43`), with the compiler built from the seed there. Each block is the command, then its whole output.

```
$ ./heroes --version
heroes 0.6.0
sources 1a1e340778db789a
```

```
$ ./heroes measure spec/heroes-spec.md
spec/heroes-spec.md
  claude-legacy      7876   (64995 ranks)
  cl100k_base        8019   (100256 ranks)
  maximum            8019   a lower bound, not the reader's tokeniser
  spread              143   between the two vendored tables (1%) — the gap between two REAL tokeniser generations was 967
  real              10514   claude-opus-5, 2026-10-30 — the binding number

An addition needs a named removal or a pre-registered falsifiable
prediction (panel 012). Headroom: 5870 against the 16384 ceiling — but the FFI floor mortgages 60 of it (panel 030 R3), so what is measured against the ceiling is 10574 and the check goes red at 16384.
```

```
$ sed -n '22,30p;40p' spec/heroes-spec.md
- One file is one module; the file you compile holds `function main()`, which
  takes nothing and produces nothing.
- `use geom` binds `geom` to `geom.hero`'s declarations, written
  qualified: `geom.dist2(a: p, b: q)`, `p: geom.Point`. Every
  module you name needs its own `use`. No wildcard.
- A `use` may be a path: `use syntax/decl` binds `decl`, the last part, and
  `use syntax/decl as sd` binds `sd` instead. A file may not bind one name twice;
  two files may name the same module differently. Every `use` starts at the
  directory of the file you compile.
    Use  = "use" ident { "/" ident } [ "as" ident ] NEWLINE .
```

```
$ sed -n '105,108p' selfhost/modules.hero
            if already
                continue
            opened = open_directory(path) + module + ".hero"
            found = reading.text_at(opened: opened, name: paths.directory_text(path) + module + ".hero", edited: edited)
```

```
$ grep -n 'assert table.len()' selfhost/cli/table.hero
238:    assert table.len() == 13
```

```
$ grep -n -i 'only place in the compiler' selfhost/cli/refresh.hero
4:# THIS IS THE ONLY PLACE IN THE COMPILER THAT REACHES THE NETWORK, and every
```

```
$ wc -l selfhost/cli/sha256.hero; grep -n '^function of' selfhost/cli/sha256.hero
     292 selfhost/cli/sha256.hero
119:function of(text: str) -> str
```

```
$ grep -n '^function run\b\|^function run(' selfhost/cli/process.hero
109:function run(program: str, words: [str], out: str, err: str) -> Ran
```

```
$ sed -n '47,57p' .claude/rules/cli-surface.md
## There is no fourth slot, because there is no fourth input class

The three shapes above all answer *what does this capability do to the inputs the
tool already has*: a `.hero` file, argv, and the machine's environment. A
**per-project file** is none of them, so this rule does not admit one (CL-016).
The refusal is **not permanent**: it is conditional on panel 056's three return
conditions, which are its only amendment path.

`heroes add` and `heroes fetch` are subcommands when they arrive
(design.md:772, and that line number is the one citation shape no instrument can
check, CL-037).
```

```
$ sed -n '135,139p' CLAUDE.md
**Before `1.0.0`, no form enters to keep an older program compiling**
(author, 2026-10-29, meant as: *I do not want to complicate the language for
backward compatibility I do not need now; an absolute rule*): no declared
version, no mode, no old spelling kept beside the new, no deprecated alias. A
change that refuses a `0.x` program is told, never undone for it; the promise
```

```
$ git grep -l -e '^function main(' v0.6.0 -- examples | wc -l
      69
```

```
$ grep -rh --include='*.hero' '^use ' selfhost | wc -l; grep -rh --include='*.hero' '^use ' tests/harness | wc -l; grep -rh --include='*.hero' '^use ' examples | wc -l; grep -rh --include='*.hero' '^use core/' examples | wc -l
    5482
     635
     130
      41
```

```
$ grep -n '"vendor"' tests/harness/suite_records.hero
84:        "vendor"
```


```
$ cat docs/panel/235-briefs/probe/angle.hero; ./heroes check docs/panel/235-briefs/probe/angle.hero; echo exit $?
use <json>/parse

function main()
    print(parse.go())
error[expected_module_name]: expected the module's name after `use`, found `<` — `use geom`, which reads `geom.hero` beside this file
  at docs/panel/235-briefs/probe/angle.hero:1:1
    |
  1 | use <json>/parse
    | ^^^
exit 1
```

```
$ cat docs/panel/235-briefs/probe/pkg.hero; ./heroes check docs/panel/235-briefs/probe/pkg.hero; echo exit $?
use pkg/json/parse

function main()
    print(parse.go())
error[unknown_module]: there is no module `pkg/json/parse` — `use pkg/json/parse` reads `docs/panel/235-briefs/probe/pkg/json/parse.hero`, from the directory of `docs/panel/235-briefs/probe/pkg.hero`, the file you compile, and that file is not there
  at docs/panel/235-briefs/probe/pkg.hero:1:5
    |
  1 | use pkg/json/parse
    |     ^^^^^^^^^^^^^^
exit 1
```

```
$ cat docs/panel/235-briefs/probe/domain.hero; ./heroes check docs/panel/235-briefs/probe/domain.hero; echo exit $?
use github.com/acme/json

function main()
    print(json.go())
error[module_path_has_no_parts]: a module path is written with `/`, not `.` — `use github/…`, and every `use` starts at the directory of the file you compile
  at docs/panel/235-briefs/probe/domain.hero:1:11
    |
  1 | use github.com/acme/json
    |           ^
exit 1
```

One block was first run with a pattern that matched nothing, a sentence that wraps across two lines in CLAUDE.md; it is re-run above as the lines that hold it, the same day.

The three programs of the last three blocks are not kept as files beside this one: two of them do not parse, which is what they show, and a committed `.hero` must parse (`.claude/hooks/staged.py`). Each block prints its program whole with `cat`; to run one again, write its text to the path the block names.

## Read again on the critic's first pass, 2026-11-07

Two readings of the blocks above were corrected by the completeness critic
(`docs/panel/235-reports/critic-briefs.md`, its item 16), each re-run that day:

- `use <json>/parse` is refused at `1:1`, the caret under `use`, as the angle
  block's own output shows; `expected_module_name` names the `<` it found in
  its text, not by its column. `00-shared.md` said *at the `<`* and now says
  this.
- `.claude/rules/cli-surface.md`, quoted in the fourth block, cites `heroes
  add` at `design.md:772`, a stale line: `grep -n 'heroes add' docs/design.md`
  prints ``898:it arrives it will be `heroes add`/`heroes fetch` — inside
  the same binary.``, in §3.5 (`grep -n '^### 3.5' docs/design.md` prints
  `887`). The quotation stands as the rule file holds it.

And one fact the briefs now state, run in `235-base`:

```
$ find examples -path '*internal*' -name '*.hero' | wc -l; ls -d examples/*/core | wc -l
      19
      14
```
