# 043: a stopped frame lists only the author's bindings, shown in Heroes, and still under their C names

Measured 2026-10-13 on this Mac, as M-typed-inspection's after-table: what a
stopped program shows once step 4 (`c6f4d0157`, the emitter declaring every
temporary and every invented slot `nodebug`) and step 5 (`9f025f019`, the
formatter for lldb) have landed, measured exactly as
`docs/measurements/042-a-stopped-frame-shows-the-c-names-and-of-its-locals-the-author-wrote-fewer-than-one-in-ten.md`
measured the before: the same program, the same three frames, the same lldb
invocation, the same census, and the four failures of 2026-09-06 re-run.

## 1. Provenance

| what | value |
|---|---|
| the machine | macOS 27.0.1 (26A434), arm64 |
| the debugger | `lldb-2103.0.34.103`, run as `lldb --no-lldbinit --no-use-colors --batch`, so no `.lldbinit` moves what it prints |
| the C compiler | Apple clang 21.0.0 (clang-2100.3.34.2) |
| the Heroes compiler | `./heroes`, `heroes 0.3.0`, built by `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` in a detached worktree at `c6f4d0157`; the seed's SHA-256 `f67d4e07964cb4f5fa7b63ae13d1c2d9fb88e87444a1e0040172c616fd009add` (the one step 4's commit names), the compiler's `9087f42451102b758a598017f8feeffca2f76fe047c9cedaa5546bcf59bfa9b3` |
| the formatter | `editors/lldb/heroes_lldb.py` as committed at `9f025f019` and unchanged since in lane typed-formatter (blob `ab75150095ad966a86a64962dd7f20f168e46adb`, 1,093 lines), SHA-256 `b848194ee601cff6cf4cad1f91c081b53e8b174f73ac826fa2ef0e83ad7527a7`; copied to the worktree's `build/heroes_lldb.py` and loaded with `command script import build/heroes_lldb.py` |
| the pair | step 4's compiler and step 5's formatter stood in no single commit that day: `git merge-base --is-ancestor c6f4d0157 HEAD` in lane typed-formatter answered no, so the pair is a worktree and a copied file |
| the builds | `heroes build <file> -O0 -o <binary>`; `-O0` carries `-g` at `c6f4d0157` (`selfhost/cli/flags.hero`, `debug_words`, re-read there) |
| the DAP adapter | Xcode's `lldb-dap` (`xcrun --find lldb-dap`), LLVM version 21.0.0, driven by panel 213's ffi-pragmatist client `docs/panel/213-reports/ffi-pragmatist-work/dap.py`, copied unchanged |
| scratch | `build/census/` of the worktree, which git ignores, written 2026-10-13; kept in the archive of section 8 |

## 2. The program

042's `shapes.hero` (042 section 2, where it stands whole), restored from 042's
archive byte for byte: SHA-256
`a8b25d234176bc26e710b1f56ddcd7248c7c6a3231450de8e8a64ca8f067115c` on both
copies, and the same for its `p.txt` of twenty `p` commands
(`52f6569c86e58c541ebfa0a072090908ddcc65212673b9b6e5d6e8e7c417d137`). Built at
`c6f4d0157` it prints what it printed for 042, `7`, `ada`, `9`, `4`, `10`, `42`.
Line 30 is `print(count)`. The sessions, from the worktree's root, the first
042's word for word and the second the same with the formatter imported:

    heroes build build/census/shapes.hero -O0 -o build/census/shapes      # scratch, 2026-10-13
    lldb --no-lldbinit --no-use-colors --batch \
        -o "breakpoint set --file shapes.hero --line 30" -o run -o "frame variable" \
        -o "command source -e false build/census/p.txt" build/census/shapes   # scratch, 2026-10-13
    lldb --no-lldbinit --no-use-colors --batch -o "command script import build/heroes_lldb.py" \
        -o "breakpoint set --file shapes.hero --line 30" -o run -o "frame variable" \
        -o "command source -e false build/census/p.txt" build/census/shapes   # scratch, 2026-10-13

Both resolved the breakpoint to one location, `shapes.hero:30:11`, and stopped
there, `stop reason = breakpoint 1.1`. Neither wrote a byte to stderr.

## 3. The after-table

What `frame variable` printed with the formatter imported, word for word:

    (int64_t) h0_count = 7
    (HeroStr) h1_name = "ada"
    (h_shapes_Point) h2_p = Point(x: 3, y: 4)
    (h_shapes_Token) h3_t = .num(v: 42)
    (HeroArrayHeader *) h4_names = ["ada", "grace"]
    (HeroArrayHeader *) h5_points = [Point(x: 1, y: 2), Point(x: 5, y: 6)]
    (HeroMapHeader *) h6_ages = {"ada": 36, "grace": 85}
    (h_0opt_e201354) h7_good = ok(4)
    (h_0opt_e201354) h8_bad = fail(code: "odd", msg: "not even")
    (h_shapes_Room) h9_room = Room(name: "lab", corner: Point(x: 9, y: 10))
    (h_shapes_Token_c_num) h15_n = .num(v: 6809745884)

Without the formatter, step 4 alone, it printed 37 lines: the ten bindings'
36, identical to 042 section 3's block but for the addresses (compared after
writing each address `<addr>`), and `(h_shapes_Token_c_num) h15_n = (f_v =
6809745884)`, the line 042's frame also held, then among 339.

| shape | binding | what the frame shows of the value, before (042) | after, with the formatter | `p <the author's name>` | `p <the C name>`, after |
|---|---|---|---|---|---|
| `i64` | `count` | `7`, as `h0_count` | `7`, as `h0_count` | `error: use of undeclared identifier 'count'` | `(int64_t) 7` |
| `str` | `name` | `(ptr = "ada", len = 3)` | `"ada"` | `error: 'name' has unknown type; cast it to its declared type to use it` | `(HeroStr) "ada"` |
| record | `p` | `(f_x = 3, f_y = 4)` | `Point(x: 3, y: 4)` | `error: use of undeclared identifier 'p'` | `(h_shapes_Point) Point(x: 3, y: 4)` |
| variant with payload | `t` | the enumerator `h_shapes_Token_tag_num` and `as.c_num`, nine lines | `.num(v: 42)` | undeclared, as above | `(h_shapes_Token) .num(v: 42)` |
| `[str]` | `names` | an address, `HeroArrayHeader *` | `["ada", "grace"]`, the type still `HeroArrayHeader *` | undeclared | `(HeroArrayHeader *) ["ada", "grace"]` |
| `[Point]` | `points` | an address, `HeroArrayHeader *` | `[Point(x: 1, y: 2), Point(x: 5, y: 6)]` | undeclared | `(HeroArrayHeader *) [Point(x: 1, y: 2), Point(x: 5, y: 6)]` |
| `{str: i64}` | `ages` | an address, `HeroMapHeader *` | `{"ada": 36, "grace": 85}`, the type still `HeroMapHeader *` | undeclared | `(HeroMapHeader *) {"ada": 36, "grace": 85}` |
| `i64?`, ok | `good` | both arms, eleven lines, under `h_0opt_e201354` | `ok(4)`, the type still `h_0opt_e201354` | undeclared | `(h_0opt_e201354) ok(4)` |
| `i64?`, err | `bad` | both arms, the `ok` half reading `4295075736` | `fail(code: "odd", msg: "not even")` | undeclared | `(h_0opt_e201354) fail(code: "odd", msg: "not even")` |
| nested record | `room` | the fields, the inner record's in parentheses, four lines | `Room(name: "lab", corner: Point(x: 9, y: 10))` | undeclared | `(h_shapes_Room) Room(name: "lab", corner: Point(x: 9, y: 10))` |

The `p <the author's name>` column is the same with the formatter and without
it: nine names answer `use of undeclared identifier`, `name` answers `has
unknown type`, each with 042's note `Expression evaluation in pure C not
supported. Ran expression as 'ISO C++'.` Without the formatter the twenty `p`
answers are 042's word for word, the addresses and the copy's path aside.

**Through `lldb-dap`**, the seat's client with the one `initCommands` entry the
extension prepends (`editors/vscode/extension.js`, `withFormatter`), the quoted
absolute import of the same file:

    python3 -I build/census/dap.py "$(xcrun --find lldb-dap)" "$PWD/build/census/shapes" \
        "$PWD/build/census/shapes.hero" 30 "command script import \"$PWD/build/heroes_lldb.py\""   # scratch, 2026-10-13

It stopped in `h_shapes_main` at line 30 and its Locals scope held the same 11
variables, each value as the table's after column (an array's and a map's
preceded by the pointer's address, `0x00000001009497a0 ["ada", "grace"]`), and
one level of children in Heroes' names: `x = 3`, `y = 4` under `h2_p`, `v = 42`
under `h3_t`, `[0] = "ada"` and `[1] = "grace"` under `h4_names`, `["ada"] = 36`
under `h6_ages`, `ok = 4` under `h7_good`, `code = "odd"` and `msg = "not even"`
under `h8_bad`, `name = "lab"` and `corner = Point(x: 9, y: 10)` under
`h9_room`. The client prints every output event holding `rror` or `Traceback`,
and printed none.

## 4. The four failures of 2026-09-06, re-run

1. **`p p` is `error: use of undeclared identifier 'p'`: still holds, word for word**, with the formatter and without it, for nine of the ten names; `name` still answers `has unknown type`.
2. **A `[T]` and a `{K: V}` are an opaque header pointer: no longer holds with the formatter**, which prints `["ada", "grace"]`, `[Point(x: 1, y: 2), Point(x: 5, y: 6)]` and `{"ada": 36, "grace": 85}`; the type column still reads `HeroArrayHeader *` and `HeroMapHeader *`, and without the formatter the value is the address alone, as in 042.
3. **A `T?` prints both arms, garbage included, under a hashed type name: no longer holds for the arms with the formatter**, `ok(4)` and `fail(code: "odd", msg: "not even")`, with no `ok = 4295075736`; the hashed name `h_0opt_e201354` still stands in the type column, and without the formatter both arms print as in 042.
4. **The frame is flooded: no longer holds.** `main` lists 11 locals where it listed 111, and `frame variable` prints 37 lines where it printed 339, 11 with the formatter; the other two frames of section 5 likewise list no `t<N>` and no invented slot.

## 5. The census

### The method, 042's, and the one line it changes

042 section 5's commands are re-run unchanged: the same three builds at `-O0`,
the same breakpoints, the same `frame variable`, the same IR dumps, and 042's
`names` and `slots` functions word for word (`build/census/census.sh` in the
archive). One step of 042's could not be kept: it joined the slots lldb printed
to the IR's slots line **in order**, which held because lldb printed every slot.
Step 4 hides the invented ones, so a printed slot `h<k>_<name>` or
`ph<k>_<name>` is joined **by its index** `k`: below the function's parameter
count (read from the IR's `function` line) it is a parameter, else it is the
slots line's entry `k` minus that count, whose name, its `$` dropped, must equal
`<name>`. The join by index was asked of 042's own transcripts and IR, restored
from 042's archive, and gave 042's numbers, 138, 121 and 111 locals split
108/20/10/0, 90/24/5/2 and 89/11/11/0, with no name failing the join; and the
three IR dumps taken at `c6f4d0157` are byte-identical to 042's (`cmp`), so the
slots each function holds did not move. The line count is 042's too: the lines
between `(lldb) frame variable` and the next `(lldb)` prompt, which reads 339,
595 and 853 on 042's transcripts.

    sh build/census/census.sh > build/census/census-out.txt     # scratch, 2026-10-13
    sh build/census/extra.sh                                     # scratch, 2026-10-13

### The numbers

| the frame | locals | `t<N>` | `$` slots | the author's slots | parameters | the author's, in all | lines `frame variable` printed | the same, with the formatter |
|---|---|---|---|---|---|---|---|---|
| (a) `adversarial-aggregate-overwrite.hero`'s `main`, stopped at :71 | 138 -> **10** | 108 -> 0 | 20 -> 0 | 10 -> 10 | 0 -> 0 | 10 (7.2%) -> **10 (100%)** | 595 -> 38 | 10 |
| (b) `syn/expr.hero`'s `compared`, stopped at :66 | 121 -> **7** | 90 -> 0 | 24 -> 0 | 5 -> 5 | 2 -> 2 | 7 (5.8%) -> **7 (100%)** | 853 -> 25 | 7 |
| (c) `shapes.hero`'s `main`, stopped at :30 | 111 -> **11** | 89 -> 0 | 11 -> 0 | 11 -> 11 | 0 -> 0 | 11 (9.9%) -> **11 (100%)** | 339 -> 37 | 11 |

The before is 042's; the after is this run's. The breakpoints resolved where
042's did, `adversarial-aggregate-overwrite.hero:71:11` and `expr.hero:66:13`.
The locals listed, by C name: in (a) `h0_c`, `h1_i`, `h2_s`, `h3_j`, `h6_f`,
`h7_tag1`, `h10_f`, `h11_tag2`, `h14_f`, `h15_n`; in (b) `ph0_c`, `ph1_t`,
`h3_left`, `h4_op`, `h7_right`, `h9_made`, `h10_second`; in (c) the ten of the
table and `h15_n`.

### What the prologue still declares

`heroes build <file> --emit-c`, each frame's prologue split on `;`:

| the frame | declared | marked `HERO_NODEBUG` | of them `t<N>` | unmarked |
|---|---|---|---|---|
| (a) | 138 | 128 | 108 | 10, the author's slots of the table above |
| (b) | 119 | 114 | 90 | 5, `h3_left` `h4_op` `h7_right` `h9_made` `h10_second` |
| (c) | 111 | 100 | 89 | 11, `h0_count` to `h9_room` and `h15_n` |

The declared counts are 042 section 5's, 138 and 119 (that section's prologue
counts for (a) and (b)) and 111; (b)'s two parameters stand in the signature,
`h_synexpr_compared(h_syncursor_Cursor *ph0_c, h_syntree_Tree *ph1_t)`. The
`--emit-c` output of each of the three programs holds one
`#define HERO_NODEBUG __attribute__((nodebug))`.

## 6. What the numbers say

Facts only.

- **In each of the three frames every local lldb lists is a binding the author wrote or a parameter**: 10 of 10, 7 of 7, 11 of 11. The 128, 114 and 100 it no longer lists are every `t<N>` and every `$` slot, all still declared, every one marked.
- **One line per local with the formatter**: 10, 7 and 11 lines, against 595, 853 and 339. Without it 38, 25 and 37, an aggregate still printed over several lines in C's shape.
- **The names are still the C names.** Every listed name opens `h<k>_` or `ph<k>_`, and the index keeps the gaps the hidden slots leave (`h3_left`, `h4_op`, `h7_right`, `h9_made`, `h10_second`). The type column is still the emitted C's, `HeroArrayHeader *`, `h_0opt_e201354`, `h_shapes_Token_c_num`. And `p <the author's name>` is refused for every one of the ten.
- **A binding the program has not reached still shows a value.** At (b)'s :66, `made`, bound at :72, read `6171889304`, the number 042 read, and `second`, bound at :73, reads `ok(.add)` with the formatter; at (c)'s :30, the match binding `n`, bound at :37, reads `.num(v: 6809745884)` with the formatter and `(f_v = 6809745884)` without it, as in 042.
- **The formatter bounds a long array, and reaches the stop's own frame line.** (b)'s parameter `ph0_c` printed as its address and `Cursor(tokens: [` sixteen tokens `, ... 232 more], at: 5)`, on one line. The line lldb prints on stopping read `h_synexpr_compared(ph0_c=0x000000016fdfcdc0, ph1_t=0x000000016fdfcda0)` without the formatter and `h_synexpr_compared(ph0_c=Cursor(tokens: [...], at: 5), ph1_t=Tree(exprs: [.text(value: "hello")], stmts: [], top: []))` with it, the tokens elided here.

## 7. Unrun, and named

- **Linux.** Neither of Ubuntu's lldbs, 18.1.3 and 22.1.8, ran here; step 5's commit body reports both, and this measurement repeats none of it.
- **lldb 18**, by the same token: the table's after column is lldb 2103's rendering alone.
- **The editor.** No VS Code session: the DAP rows above are Xcode's `lldb-dap` driven by the seat's client, which sends `initCommands` and not `enableAutoVariableSummaries: true`, which the extension also sets; how the Variables pane draws these values was not looked at.
- **Windows.** Not run; the milestone page records that `lldb.exe` exits `0xC0000135` before running a command.
- **An optimised build.** `-O2` carries `-gline-tables-only`, no variables, and was not measured.
- **What `name` resolves to**, 042's open item, half settled: in the stopped `shapes`, `image lookup -r -s '^_?name$'`, `image lookup -r -n '^name$'` and `image lookup -t name` printed nothing, `target variable name` answered `can't find global variable 'name'`, and `expr -- (void *)&name` answered `function 'name' with unknown type must be given a function type`. The evaluator takes `name` for a function without debug information; which image holds it, no command here found.
- **The count of 2026-09-06**, 042 section 5's question, was not re-asked.
- **Step 4's other numbers** (build time, the units' and the seed's bytes) were not re-measured.

## 8. The scratch, kept beside this file

The 67 text files of `build/census/` that sections 2 to 6 name, the two scripts
that wrote them (`census.sh`, then `extra.sh`), `shapes.hero` and `p.txt`, the
seat's client as run, the IR dumps, the emitted C, the prologue counts, the lldb
and lldb-dap transcripts, the join asked of 042's transcripts, and the
comparisons with 042's, stand in
`043-a-stopped-frame-lists-only-the-authors-bindings-shown-in-heroes-and-still-under-their-c-names.tar.xz`,
beside this file; the worktree's absolute path is written `<copy>` in them.
`tar -xJf docs/measurements/043-a-stopped-frame-lists-only-the-authors-bindings-shown-in-heroes-and-still-under-their-c-names.tar.xz -C build`
puts them back at `build/census/`, where 042's archive also restores, so one at
a time. The three binaries are not kept: `census.sh` rebuilds them with the
compiler of section 1, `extra.sh` then wants the formatter at
`build/heroes_lldb.py` and 042's archive restored at `build/before042`
(`mkdir -p build/before042` and its `tar -xJf` with `-C build/before042`).
