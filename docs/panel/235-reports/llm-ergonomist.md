# Panel 235, the llm-ergonomist's report: the blind seat

How it was run, written by the coordinator: one fresh `claude -p` session on
2026-11-07, `claude-opus-5-5`, run by `blind235.py seat` from
`build/b235-seat/` in `235-base` at `3b071e4f1`, with `--bare --restricted
--safe-mode --strict-mcp-config`, `--tools Read,Write` and the same in
`--allowedTools`, `--max-budget-usd 2.00`,
`CLAUDE_CODE_MAX_OUTPUT_TOKENS=32000`. The CLI reported 0.2419 USD, 8 tool
calls, every one a `Read` of a file in its folder (the void check passed).
The folder held `brief.md` (`docs/panel/235-briefs/blind/seat/brief.md`),
today's specification unamended, `variant-1.md` to `variant-3.md` (U-pkg,
U-angle and U-bare's sections, the order `blind/seat/variants.tsv` records)
and the reader test's T2 and T3 as `task-1.md` and `task-2.md`, with T3's
own module. **The session answered that it had no tool to write files**,
though the command named `Write` in `--tools` and `--allowedTools` (a
question for the process notes, unrun), and gave its report whole as its
final text; what follows is that text, copied byte for byte from the result
event of `build/b235-seat-out/seat.jsonl`. Its `context` section names the
harness's operating instructions and nothing of the project.

The folder's files, `shasum -a 256`:

```
3f4a41095a3e5084ca4437df7bfea0b0d04b73e069bebf04ffbf3b96d5c66fe7  brief.md
0d88388a920f89d100c144b66a1f9b354d198039f6e0cbe4873dd704f24f923a  heroes-spec.md
6f7a07e1c19f15adfce2f98d61c47624e62144736c8486b89475492b8c94a3b7  variant-1.md
128ff0ccc9b9e11982a429d7aa3c04ea96ecdf392d3c1883c38e4e3b2cb4ebc2  variant-2.md
6745b60fc7cac8cb99a4d4d4e979d27ffa7460b9574bdddd87a78fec508201c4  variant-3.md
880f6cee2accc337ced987c8c7406f902943b71fb5aa6df957b1292d85799770  task-1.md
7c39e86d2383a3c543e2ec7f01ae0e5be38ac87bb2b4cd6916e5c23afe7b200b  task-2.md
1210bb29cb7945ac89d2ae8c61107adfb5702c16359b54f5ba03111515dff38a  geom/area.hero
```

---

I couldn't create `report.md`: I only had a tool for reading files, not one for writing them. The full report, in the sections the brief asks for, is below and can be saved as `report.md` as it stands.

---

# report.md

## verdict

- **variant-1** (`use pkg/<entry>/<module>`): **approve.** `pkg/` on the line itself says the module comes from a package and names its entry, so nothing outside the line is needed to know what kind of `use` it is.
- **variant-2** (`use <entry>/<module>`): **approve.** The angle brackets mark a package as clearly as `pkg/` does. The likely slip, writing the whole path in brackets C-style (`<csv/fields>`), is caught when the program is compiled.
- **variant-3** (`use <entry>/<module>`, unmarked): **veto.** Whether `use x/y` reaches a package or the program's own folder depends on `heroes.toml`, a file the line neither names nor marks.

## experiment

The authors call both packages in task 1 `csv`. For git entries the entry name is the program's choice, so I chose `csv_split` and `csv_quote`. Each catalogue is `heroes.toml` beside the program.

### Task 1, variant 1
```toml
[dependencies.csv_split]
git = "https://git.example.org/ada/csv"
tag = "v1.2.0"
sum = "sha256-34d7dc508706d83daba5e85482dc993822ee0fc41c9c33c00e62ee5d71a1c176"

[dependencies.csv_quote]
git = "https://git.example.org/ben/csv"
tag = "1.10"
sum = "sha256-8f8ec08f37590a2ff129f489fdd22e48c2221472e13b22ee04f3892fb40bdb75"
```
```
use pkg/csv_split/fields
use pkg/csv_quote/quote

function main()
    quoted: [str] @= []
    for part in fields.split_line("red,green,blue")
        quoted @ quoted.push(quote.quote_field(part))
    print(join(quoted, ";"))
```

### Task 1, variant 2
The catalogue is the same as for variant 1.
```
use <csv_split>/fields
use <csv_quote>/quote

function main()
    quoted: [str] @= []
    for part in fields.split_line("red,green,blue")
        quoted @ quoted.push(quote.quote_field(part))
    print(join(quoted, ";"))
```

### Task 1, variant 3
The catalogue is the same as for variant 1. Neither entry name matches a folder or module of the program.
```
use csv_split/fields
use csv_quote/quote

function main()
    quoted: [str] @= []
    for part in fields.split_line("red,green,blue")
        quoted @ quoted.push(quote.quote_field(part))
    print(join(quoted, ";"))
```

### Task 2, variant 1
```toml
[dependencies.geom]
git = "https://git.example.org/cara/geom"
tag = "v3.0.2"
sum = "sha256-40400ef59eede9a63c4ebae1e2a9ca0633467bdd4fc652250d3869641abd8214"
```
```
use geom/area
use pkg/geom/polygon

function main()
    print("area: ", area.square(5))
    print("perimeter: ", polygon.hexagon_perimeter(5))
```

### Task 2, variant 2
The catalogue is the same as for task 2, variant 1 (entry `geom`).
```
use geom/area
use <geom>/polygon

function main()
    print("area: ", area.square(5))
    print("perimeter: ", polygon.hexagon_perimeter(5))
```

### Task 2, variant 3
The program has its own folder `geom`, so variant 3 forbids an entry named `geom`. I renamed the entry `cara_geom`.
```toml
[dependencies.cara_geom]
git = "https://git.example.org/cara/geom"
tag = "v3.0.2"
sum = "sha256-40400ef59eede9a63c4ebae1e2a9ca0633467bdd4fc652250d3869641abd8214"
```
```
use geom/area
use cara_geom/polygon

function main()
    print("area: ", area.square(5))
    print("perimeter: ", polygon.hexagon_perimeter(5))
```

## choice_points

1. **Entry names in task 1 (all variants).**
   - My choice: `csv_split` and `csv_quote`.
   - Other choice: name both `csv`. That defines the TOML table `[dependencies.csv]` twice, so the catalogue's reader rejects it.
   - Naming only one of them `csv` works, but then the line no longer says which `csv` it is.
2. **Entry name in task 2.**
   - Variants 1 and 2: I chose `geom`, the same as the program's folder. The marker keeps them apart, so this compiles and works.
   - Variant 3: `geom` is an error ("an entry named as a folder … of the program's own"), so I renamed it `cara_geom`. Keeping `geom` gives a compile-time error.
   - I don't know what the error message says. Variant 3 doesn't specify it.
3. **Mapping over the fields: loop or `map`.**
   - My choice: a `for` loop with `push`.
   - Other choice: `parts.map(quote.quote_field)`. The spec says top-level functions are values, but it never says whether a module-qualified name like `quote.quote_field` can be used as one.
   - If it can't, this is a compile-time error; if it can, the output is the same.
4. **Named or positional arguments.**
   - My choice: positional, as in `area.square(5)`. Each function has one parameter, so names aren't required.
   - Other choice: `square(side: 5)` is also allowed by the grammar. Same program.
5. **How to print.**
   - My choice: `print("area: ", x)`, which relies on `print` adding no separator.
   - Other choice: `print(f"area: {area.square(5)}")`. Same output.
   - A plain literal is safe here because it holds no braces.
6. **Renaming with `as`.**
   - I didn't need it, because the bound names (`fields`, `quote`, `area`, `polygon`) don't collide.
   - Binding one name twice would be a compile-time error.
7. **The tag `1.10`.**
   - My choice: write it as a string, which the variants require ("each value a string").
   - Other choice: an unquoted `1.10` would be a TOML float that becomes `1.1`. The reader should reject a value that isn't a string, so this is a compile-time error rather than fetching the wrong tag. The variants say values are strings but not what happens otherwise, so I'm inferring the rejection.
8. **Variant-specific slips.**
   - Variant 1: `use pkg/fields`, without the entry, doesn't match the package form. It falls through to an own module under the reserved name `pkg`, which is a compile-time error.
   - Variant 2: `use <csv_split/fields>` doesn't match the grammar, which is a compile-time error.
   - Variant 3: a misspelled entry name (`csvsplit/fields`) matches no entry. It is then looked up as an own module, so the error says a local file is missing rather than that a package is unknown.
   - Variant 3, the silent case: remove an entry from `heroes.toml` while the program has a folder of the same name. The same `use` line then quietly reaches that folder and still builds. The program does something else and nothing reports it.

## locality

**Today.** `use geom/area` needs only the file it names, `geom/area.hero`, found from the directory of the file being compiled. The line names everything it depends on.

**Variant 1.** The line alone tells you three things: this is a package, which catalogue entry it is, and which module and binding. The source and version are in `heroes.toml`, which `pkg/` marks. A `use` without `pkg/` is exactly as today. This differs from today only by one reserved word, and it is local.

**Variant 2.** Same as variant 1, with `< >` as the marker and no reserved folder name. The meaning is decided by the line itself, and `heroes.toml` is consulted only for an entry the line marks. It is local.

**Variant 3.** From `use x/y` alone you cannot tell whether it reaches a package or `./x/y.hero`. You need to read `heroes.toml`, which the line neither names nor marks, and to answer the collision rule you also need the directory listing. A `use` of an own module, which needs nothing extra today, now also depends on the catalogue. It is not local, which is grounds for the veto.

## argument

The brief's test is whether a line's meaning can be found from the line plus the files it names or marks. Variants 1 and 2 put a marker on the line, so a model, or a reader, knows from the line alone whether it is writing a package `use`. Its own-module `use` lines stay as they are today. Variant 3 makes the meaning depend on the catalogue. It also forces a renaming whenever an entry name matches a local folder, which task 2 is built to trigger: the natural name `geom` is an error. Worse, deleting a catalogue entry can silently redirect a line to a local folder. Between 1 and 2 the difference is small. `pkg/` reads as an ordinary path and costs one reserved name. `< >` borrows C's `#include`, which invites writing the whole path in brackets, an error the compiler catches.

## prediction

These are guesses for 30 first tries per variant (two models, fifteen tries each, three tasks like these):

- **variant-1: 25 of 30.** Most frequent mistake: leaving out the entry segment (`use pkg/fields`), or naming both `csv` entries `csv`.
- **variant-2: 23 of 30.** Most frequent mistake: putting the whole path in brackets, `use <csv_split/fields>`.
- **variant-3: 15 of 30.** Most frequent mistake: naming the task 2 entry `geom`, which collides with the program's own folder.

## condition

- **variant-1:** I would change to object if it scores 5 or more of 30 below variant 3, or if its most frequent mistake builds and runs wrongly instead of failing to compile.
- **variant-2:** the same thresholds as variant 1. I would also object if bracket mistakes go uncaught at compile time in the measured run.
- **variant-3:** the veto rests on locality, not on the score. Even so, I would lower it to object if variant 3 matches or beats both other variants, and no measured failure comes from the collision rule or a misresolved `use`.

## context

Besides the files in this directory, I received the agent harness's operating instructions: the working directory, tool usage, and to answer in English. Nothing about Heroes, no project rules, no contract, no memory and no other document reached me. I read only `brief.md`, `heroes-spec.md`, the three variants, the two tasks and `geom/area.hero`.
