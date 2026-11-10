# Panel 244, the llm-ergonomist's blind reading: whether the specification states `internal`

Run 2026-11-11 by the coordinator, on the author's yes (*Sì, fino a 3 USD*, the question form, at
about 00:40), under the rule registered before any session in `docs/panel/244-briefs/blind/README.md`.
Its briefs and tasks are beside that file.

## How it was run

- **Eight `claude -p` sessions**, one per folder of `.claude/worktrees/p244-scratch/blind244/`
  (`t1-x-1`, `t1-y-1`, `t2-x-1`, `t2-y-1`, `t1-x-2`, `t1-y-2`, `t2-x-2`, `t2-y-2`), run one after
  another by that folder's `run.sh`, each folder holding `brief.md`, `task.md` (`task-t1.md` or
  `task-t2.md`) and `heroes-spec.md`, the arm's specification: **x** the critic's
  `set-S1c2_d03_C2_d08d.md` (R1 without I2), **y** the critic's `set-S1c2_I2.md` (R1 with I2), the
  labels stripped. The second file is the specification this landing writes, `cmp` equal.
- The command, `/panel`'s with `--bare` added: `--bare --restricted --safe-mode
  --strict-mcp-config --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools
  "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" --max-budget-usd 0.36
  --output-format json`, with `GIT_CEILING_DIRECTORIES` set to the folders' parent so no session
  saw a git tree, and the key loaded from the trunk's `.env`, never printed.
- **The CLI**: Claude Code 2.1.311. **The model**: `claude-opus-5-5` answered every session; the
  CLI's report also names `claude-haiku-5-5`, at 17 to 222 output tokens a session, its own
  service calls.
- **What it cost**: 1.0231 USD in all by the CLI's `total_cost_usd`, 0.1209 to 0.1347 a session
  (`costs.txt`), under the registered bound of 2.88 and the author's 3.
- **Where the reports came from**: each session gave its report as its final message, which the CLI
  writes as `result` in `run.json`; `report.md` in each folder is that text, compared equal for all
  eight when this file was written. No permission was denied in any session.

## Graded by the registered rule

Each answer's files were copied into a fresh folder and built with batch 30's round compiler
(`.claude/worktrees/b30-fix/heroes-next`, `sources 75d1dcd260ad7c4a`) by `grade.py`; for t1 the
program of the task stands first and the answer's files over it.

| session | arm | graded | what the build said |
|---|---|---|---|
| t1-x-1 | x | wrong | predicted the program builds; built as given, it was refused, exit 1 |
| t1-x-2 | x | wrong | predicted the program builds; built as given, it was refused, exit 1 |
| t1-y-1 | y | **right** | predicted the refusal; its repair built and printed `6` |
| t1-y-2 | y | **right** | predicted the refusal; its repair built and printed `6` |
| t2-x-1 | x | wrong | `main.hero` printed `300`; `probe.hero` built, exit 0 (`money.hero` alone) |
| t2-x-2 | x | wrong | `main.hero` printed `300`; `probe.hero` built, exit 0 (`money.hero` alone) |
| t2-y-1 | y | **right** | `main.hero` printed `300`; `probe.hero` refused, exit 1 (`money/internal/helper.hero`) |
| t2-y-2 | y | **right** | `main.hero` printed `300`; `probe.hero` refused, exit 1 (`money/internal/arith.hero`) |

**Arm y 4 of 4, arm x 0 of 4.** The rule asked y to give at least two more right answers than x;
it gave four, so **I2 enters the specification**, landed with S1c2 in batch 30.

One correction to the grader, made before the count was read: its first pattern took a file's path
only from a heading or bold line written one way, and read three t2 answers as giving no files. It
was widened to the headings the answers used (`### `path``), and the eight answers graded again;
`grade.py` is the widened one.

## Context

Every session answered the brief's last question, whether any project rule, contract or memory
reached it: **no**, in all eight, each naming the three files of its folder and the CLI's own system
prompt (working directory, date, tool definitions). The reading is clean.

## The reports

Each as the session wrote it.

### t1-x-1

`````markdown
I only have a file-reading tool, so I could not create `report.md`. As the brief allows, here is the whole report.

# report.md

## answer

**1. Does the compiler accept the program as written?** Yes. I expect `heroes build main.hero` to accept it, and the program prints:

```
6
```

Why, citing `heroes-spec.md`:

- **Each file is one module.** §1: "One file is one module; the file you compile holds `function main()`, which takes nothing and produces nothing." `main.hero` holds `function main()` with no parameters and no `->`.
- **`use geom` works.** §1: "`use geom` binds `geom` to `geom.hero`'s declarations, written qualified." `geom.hero` is in the folder, so `geom.area` resolves.
- **A path `use` binds its last part.** §1: "A `use` may be a path: `use syntax/decl` binds `decl`, the last part." So `use geom/internal/scan` binds `scan` to `geom/internal/scan.hero`.
- **Paths start at the folder of the compiled file.** §1: "A `use` starts at the directory of the file you compile." That folder holds `main.hero`, so `geom/internal/scan` is `./geom/internal/scan.hero`, which exists. This holds both in `main.hero` and in `geom.hero`. `geom.hero` sits in that same folder anyway, so the path resolves the same way under either reading.
- **Every module needs its own `use`.** §1: "Every module you name needs its own `use`." `main.hero` names both `geom` and `scan` and has a `use` for each. `geom.hero` names `scan` and has its own `use`, so it does not depend on `main.hero`'s.
- **Binding the same module in two files is allowed.** §1: "A file may not bind one name twice; two files may name the same module differently." No file binds a name twice. Two files binding the same module, even under the same name, is not forbidden.
- **No local name collides with a `use`.** §5: "a `use` binds its name for the whole file, so nothing else in the file may take it." No file declares anything named `geom` or `scan`. The functions are `area`, `twice` and `main`.
- **Nothing makes a module private.** The specification has no visibility rule, no export markers and no special meaning for a directory called `internal`. Top-level declarations are reachable through a `use` (§1, §4). So `main.hero` may use `geom/internal/scan` directly.
- **Every parameter is used.** §5 refuses unused bindings and parameters. `n` is read in both `area` and `twice`.
- **Literals and arithmetic are `i64`.** §2: a literal takes the type its context asks for. `2` and `1` are passed where an `i64` is expected, so they are `i64`.
  - `geom.area(2)` = `scan.twice(2)` = `4`.
  - `scan.twice(1)` = `2`.
  - `4 + 2` = `6`, an `i64` + `i64` with no overflow (§7).
- **`print` writes the number.** §11: `print` takes a number and writes it with exactly one trailing newline, so the output is `6`.
- **Layout and return paths are valid.** Each body line is indented 4 spaces (§1). Every function with a `->` ends in `return` (§8). The call result is passed to `print`, not dropped (§5).

**2. Repair.** None is needed, because the program builds. No file changes. The three files as given, whole:

`main.hero`
```
use geom
use geom/internal/scan

function main()
    print(geom.area(2) + scan.twice(1))
```

`geom.hero`
```
use geom/internal/scan

function area(n: i64) -> i64
    return scan.twice(n)
```

`geom/internal/scan.hero`
```
function twice(n: i64) -> i64
    return n * 2
```

## choice_points

1. **Where a `use` inside `geom.hero` starts.** I followed §1 literally: from the folder of the compiled file, `main.hero`'s folder. The other reading is "from the importing file's own folder." Here the two readings agree, because `geom.hero` is in the same folder, so the result is the same: it compiles and prints `6`. If `geom.hero` sat in a subfolder, the other reading would look for a nested `geom/internal/scan.hero`. The compiler would then refuse the program because the module is missing.
2. **Whether `internal` hides a module.** I took it to mean nothing, because the specification gives it no meaning and has no visibility rule. Under the other assumption, Go-style "internal is private," `main.hero`'s `use geom/internal/scan` would be refused. The "repair" would then be to delete it and route through a new `geom` function. That would change `geom`'s set of functions without any rule asking for it.
3. **Whether `geom.hero` and a folder `geom/` can coexist.** The specification never forbids a module file and a folder of the same base name. I assumed they can. If they could not, the compiler would refuse the program as ambiguous. Nothing in the text supports that.
4. **Whether two files may bind one module under the same name `scan`.** §1 allows two files to name a module "differently." I read that as permission to use any name, the same one included, since each file's bindings are its own. Reading it as "must differ" would make the compiler refuse the program. That reading has no textual basis: the rule against binding twice applies within "a file."
5. **The type of the literals.** They are `i64`, both from the parameter context and from the default (§2). No other choice fits, so the answer could not differ here.

## prediction

- **Compiler:** accepts the program and produces a binary. There are no holes, unused names, shadowing or type mismatches.
- **Run output:** `6` followed by one newline. Exit is normal.
- **Repaired program:** the same, since it is unchanged.

## context

Only the files in this directory reached my context: `brief.md`, `task.md` and `heroes-spec.md`. Beyond those, I had the general system and harness instructions for this session (working directory, date, tool-use format). No other project rule, contract, memory or document about the language reached me.
`````

### t1-y-1

`````markdown
I can't write `report.md` because my only tool reads files. As the brief allows, here is the whole report.

# report.md

## answer

**1. Does the compiler accept the program as written?** No. It refuses `main.hero`, line 2: `use geom/internal/scan`.

The reason is in §1 (Files and layout): *"A path with `internal` as a part after its first is named only by the module the path before the last such part names and by the modules under it."*

- In `geom/internal/scan`, `internal` is the second part, so the rule applies.
- The path before it is `geom`, which names the module `geom.hero`. The modules under it are those under `geom/`.
- `geom.hero` may therefore `use geom/internal/scan`, so its line 1 is legal.
- `main.hero` is neither `geom` nor under `geom/`, so its `use geom/internal/scan` is refused. As a result `scan.twice(1)` in `main` has no binding.

Nothing else in the program is wrong:
- `use geom` binds `geom` and `use .../scan` binds `scan`, so no name is bound twice in one file.
- Both files binding `scan` is allowed: "two files may name the same module differently", and the same name in different files is not shadowing.
- No binding is unused, and the types match (`i64` throughout).

The build produces no binary and prints nothing.

**2. Smallest repair.** Leave the internal module private. Let `geom` forward `twice` publicly, and have `main` call it through `geom`. `scan.twice` and `geom.area` keep their meaning. `main` still prints `area(2) + twice(1)`, and `twice` still means `n * 2`. Two files change; `geom/internal/scan.hero` stays as it is.

`main.hero`
```
use geom

function main()
    print(geom.area(2) + geom.twice(1))
```

`geom.hero`
```
use geom/internal/scan

function area(n: i64) -> i64
    return scan.twice(n)

function twice(n: i64) -> i64
    return scan.twice(n)
```

`geom/internal/scan.hero` (unchanged)
```
function twice(n: i64) -> i64
    return n * 2
```

## choice_points

1. **How to read the `internal` rule.** I read "the module the path before the last such part names" as `geom`, meaning `geom.hero`, with "under it" meaning modules in the `geom/` directory. So `geom.hero` may use the path and `main.hero` may not.
   - Another reading: "under it" includes the importing program's root, or `internal` only restricts packages. Under that reading the original program builds and prints `6`, and my repair is unnecessary but harmless; it also prints `6`.

2. **Which repair.** I chose a forwarding `geom.twice`. The alternatives:
   - **`print(geom.area(2) + geom.area(1))`, changing one file.** The compiler accepts it and it prints `6`. But `main` no longer calls `twice`; it calls `area` for a different meaning, so I rejected it as not keeping `main`'s meaning.
   - **Move `scan` out of `internal`** (e.g. `geom/scan.hero`, used as `geom/scan` in both files). This is accepted and prints `6`. But it changes three files, and the author's choice to keep `scan` private is lost.
   - **Keep `use geom/internal/scan` in `main` and only add the forwarder.** The compiler would still refuse it on the same rule. And once `main` no longer uses `scan`, an unused `use` binding may also be refused (§5: "An unused binding … is a compile error"). So I removed the line.

3. **Naming the forwarder `twice`, in a module that also calls `scan.twice`.** §9 says "no overloading", but the two functions live in different modules and are always written qualified (`scan.twice` vs `geom.twice`), so I expect no clash. If the compiler did treat it as a clash, it would refuse the program. Renaming the forwarder (e.g. `double`) would avoid that risk at no cost to meaning.

4. **A directory `geom/` next to the file `geom.hero`.** I assume they coexist, because the rule's wording ("the module the path before … names and … the modules under it") presumes it. If they could not, the original layout would already be invalid.

## prediction

- **Original program:** the compiler refuses it, because of `use geom/internal/scan` in `main.hero` (§1 `internal` rule). No binary is produced, so nothing prints.
- **Repaired program:** the compiler accepts it. It prints `6`: `geom.area(2)` = `scan.twice(2)` = 4, plus `geom.twice(1)` = `scan.twice(1)` = 2. That is 6, with one trailing newline.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. I saw no other file, project rule, contract or memory. The only other things present were my general system instructions (I am an agent with a file-reading tool) and the working-directory path and date.
`````

### t2-x-1

`````markdown
I couldn't write `report.md` because I have no tool that writes files, so the whole report follows here as the brief allows.

# report.md

## answer

**The task can't be done as written.** The specification gives no way to make a function visible to one module and hidden from the rest. Every place I checked lacks one:

- **Declarations (§4):** there is no `private`, `pub` or export mark.
- **`use` (§1):** writing `use money` binds *all* of `money.hero`'s declarations to the qualified name. It has no list of names it imports or exports.
- **Packages (§1):** `use <pkg>/module` reaches any module of a package, so a package hides nothing either.
- **Nesting (§9):** there are no nested or anonymous functions, so `scale` can't sit inside `cents`.
- **Constants (§4):** a constant's body computes over literals and other constants, so a constant can't hold a hidden function.
- **Naming:** no naming rule, such as a leading `_`, changes visibility.

So any module that writes `use money` can call `money.scale`. The closest program the spec allows is below. `main.hero` meets its part of the task; `probe.hero` doesn't (see prediction).

### `money.hero`
```
# Euros to cents.
function cents(euros: i64) -> i64
    return scale(euros)

# Multiplies by 100. Meant for this module only; the language cannot enforce that.
function scale(n: i64) -> i64
    return n * 100
```

### `main.hero`
```
use money

function main()
    print(money.cents(3))
```

### `probe.hero`
```
use money

function main()
    print(money.scale(3))
```

## choice_points

1. **Where `scale` lives.** I put it in `money.hero`, beside `cents`, called unqualified.
   - The other choice was a separate module, such as `money/scale.hero` reached with `use money/scale`. Any root could write that same `use` line and call `scale.scale(...)`, so it would hide nothing.
   - No placement makes a program that names `scale` get refused.

2. **How `probe.hero` names the helper.** I wrote `money.scale(3)`, the call a real outside module would make.
   - The compiler accepts it, and it prints `300`. That fails the task, which wants a refusal.
   - The other choice is plain `scale(3)` with no prefix. The compiler would refuse that, but only because no `scale` is bound in `probe.hero`. It's the same error as for any made-up name, not privacy, so I didn't use it to fake a pass.

3. **Refusal as a stand-in for privacy.** I looked for any rule that refuses code naming `money.scale` from another module. Neither the unused-binding rules (§5) nor the shadowing rule applies, so I found none. Claiming one would mean `probe.hero` compiles and prints `300` when the compiler was supposed to refuse it.

4. **Small syntax choices with only one valid reading:**
   - `cents` has a single parameter, so the call needs no named argument (§9).
   - `print` takes the `i64` directly (§11).
   - Each root's `main` takes and returns nothing (§1).
   - Calling `scale` unqualified inside `money.hero` is correct, because names are qualified only through `use`.

## prediction

- **`heroes build main.hero`:** I expect the compiler to accept it. Run, it prints `300`.
- **`heroes build probe.hero`:** I expect the compiler to accept it too, because the spec has no visibility rule that could refuse it. Run, it prints `300`. This is what the task said must not happen.
- **If `probe.hero` used plain `scale(3)` instead:** the compiler would refuse it for an unbound name. That isn't the privacy the task asks for.

## context

The only inputs that reached me were `brief.md`, `task.md` and `heroes-spec.md` from this directory, plus the harness's system prompt (agent identity, working directory and date). No other file, rule, contract, memory or document reached my context.
`````

### t2-y-1

`````markdown
I only have a Read tool, so I couldn't write `report.md` or the program files. As the brief allows in that case, here is the whole report.

# report.md

## answer

The spec has no `private` or `export` keyword. Every declaration in a module can be reached by any file that `use`s that module. The only way the spec lets the compiler refuse access is the `internal` path rule (§1): "A path with `internal` as a part after its first is named only by the module the path before the last such part names and by the modules under it."

So the helper goes in its own module at `money/internal/helper.hero`:
- The part of that path before `internal` is `money`, so only `money.hero` and modules under `money/` may name it.
- `main.hero` and `probe.hero` sit at the root, outside `money/`, so `probe.hero` naming the helper is a compile error, not just a convention.

Layout of the folder:

```
main.hero
probe.hero
money.hero
money/internal/helper.hero
```

### `money/internal/helper.hero`
```
# Multiplies by 100. Reachable only from `money` and modules under `money/`.
function scale(n: i64) -> i64
    return n * 100
```

### `money.hero`
```
use money/internal/helper

# The euros, in cents.
function cents(euros: i64) -> i64
    return helper.scale(euros)
```

### `main.hero`
```
use money

function main()
    print(money.cents(3))
```

### `probe.hero`
```
use money/internal/helper

function main()
    print(helper.scale(3))
```

What the compiler does with each root:
- **`heroes build main.hero`:** accepts it. `main.hero` names only `money`. `money` names `money/internal/helper`, which it may, because `money` is the module the path before `internal` names. Running it prints `300`.
- **`heroes build probe.hero`:** refuses it. `probe.hero` is at the root, is not `money`, and is not under `money/`. Its `use money/internal/helper` breaks the `internal` rule, so no binary is produced.

## choice_points

1. **How to hide `scale`.**
   - **My choice:** the `internal` path rule.
   - **The other option:** put `scale` in `money.hero` next to `cents`. The spec gives no way to make one declaration private. A probe calling `money.scale(3)` after `use money` would then be accepted and print `300`. That breaks the task's main requirement, because the compiler would not refuse it.
2. **Where the helper module sits.**
   - **My choice:** `money/internal/helper.hero`. The part before `internal` is `money`, the module that needs the helper.
   - **Other option, `internal/helper.hero` at the root:** this is refused outright. The rule counts `internal` only "as a part after its first", so a leading `internal` makes no restricted path. The module would then be public and the probe would be accepted.
   - **Other option, `lib/internal/helper.hero`:** this names a module `lib` that isn't `money`. Then `money.hero` itself would be refused for naming the helper.
3. **Relative path in `money.hero`.** I wrote `use money/internal/helper` because §1 says a `use` "starts at the directory of the file you compile". That is the root, even when the `use` is inside `money.hero`. If paths instead started at each file's own directory, the path should be `internal/helper`. That doesn't fit here anyway: `money.hero` is in the root, so both readings give the same file. The way I wrote it is correct under the spec's wording.
4. **Assumption that `money.hero` and a `money/` directory can sit side by side,** with `money/...` counting as "the modules under" `money`. The spec suggests this but never shows it. If the toolchain read it otherwise, `main.hero` could be refused because `money.hero` would not be allowed to name the helper.
5. **Name of the helper module.** I chose `helper`, not `scale`. With `scale.hero`, a call would read `scale.scale(...)`, which also works and changes nothing. A function named `scale` inside a module named `scale` does not count as shadowing, because the `use` binding lives in the importing file. So this choice is about readability only.
6. **Call style.** I used `helper.scale(euros)` with a positional argument. Named arguments are required only when two parameters share a type (§9), and `scale(n: euros)` would be accepted too, with the same result.
7. **How the probe "calls scale directly".** It uses the only spelling that could reach the helper: `use money/internal/helper` and then `helper.scale(3)`. Adding `as h` would change nothing, since it's still refused. Without the `use`, `helper` would be an unknown name, which is also refused but for a different reason. My version tests the access rule itself.
8. **Return statements.** I wrote `return` explicitly in both functions. §8 makes it required on every path for a function with `->`, and a bare last expression would be refused.
9. **`probe.hero` is otherwise a valid root,** with `function main()` and the value used by `print`. That way the access rule is the only reason it is refused.

## prediction

- **`heroes build main.hero`:** I expect the compiler to accept it. Run, it prints:
  ```
  300
  ```
- **`heroes build probe.hero`:** I expect the compiler to refuse it with an error at `use money/internal/helper`: the module may be named only by `money` and modules under `money/`. It produces no binary, so nothing prints.

## context

Besides the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), only the standard agent system prompt reached my context: tool definitions, working directory and date. No other instructions, rules, contracts, memories or project documents did, and I read no other file.
`````

### t1-x-2

`````markdown
I only have a file-reading tool, so I could not write `report.md`. As the brief allows, the whole report follows here.

# report.md

## answer

**1. Does the compiler accept the program as written?** Yes. I expect it to build and print `6`.

Reasons, from the specification:

- **Each `use` points to a real file.** §1 says a `use` "starts at the directory of the file you compile". The compiled file is `main.hero`, in the folder's root.
  - `use geom` in `main.hero` finds `geom.hero`.
  - `use geom/internal/scan` finds `geom/internal/scan.hero`. This is true in `main.hero`, and also in `geom.hero` (§1 says "starts at the directory of the file you compile", so the path is read from `main.hero`'s folder).
- **The names bound are the ones the code uses.** A path `use` "binds … the last part" (§1). So `main.hero` binds `geom` and `scan`, and `geom.hero` binds `scan`. No file binds a name twice. Two files may bind the same module, and "every module you name needs its own `use`" (§1). `main.hero` writes `scan.twice` and has its own `use` for `scan`, so the rule is met.
- **No shadowing.** In `geom.hero`, nothing other than the `use` takes the name `scan`: the function is `area` and its parameter is `n`. In `scan.hero` the parameter is `n` and nothing else is bound. In `main.hero` there are no bindings at all (§5).
- **Nothing in the spec blocks access to `internal`.** The specification has no visibility rules: no `pub`, no private declarations, and no special meaning for a folder called `internal`. §1 says a `use` binds a name to "`geom.hero`'s declarations", meaning all of them. `internal` is just one part of the path.
- **Types match.** The literals `2` and `1` are passed to `i64` parameters, so they are `i64` (§2). `twice` returns `n * 2` as an `i64`, and `area` returns `scan.twice(n)` as an `i64`. `+` adds two `i64` values (§7), and `print` accepts a number (§11).
- **Function rules are met.** Each function with `->` returns on its only path (§8). `main` takes nothing and returns nothing (§1). `print(...)` gives `()`, so it may stand alone as a line (§5). Every parameter is used, so nothing is left unused.
- **The arithmetic.** `geom.area(2)` calls `scan.twice(2)`, which is `4`. `scan.twice(1)` is `2`. `4 + 2` is `6`. The parts are evaluated left to right (§7), and nothing overflows. `print` writes `6` followed by one newline.

**2. Repair:** None needed. The program builds, so I changed no files. For completeness, here are the files as built:

`main.hero`
```
use geom
use geom/internal/scan

function main()
    print(geom.area(2) + scan.twice(1))
```

`geom.hero`
```
use geom/internal/scan

function area(n: i64) -> i64
    return scan.twice(n)
```

`geom/internal/scan.hero`
```
function twice(n: i64) -> i64
    return n * 2
```

## choice_points

1. **Which folder the `use` in `geom.hero` starts from.** I followed §1 as written: the folder of the compiled file, `main.hero`. The other reading would be the folder of the file that contains the `use`. Here both readings give the same place, because `geom.hero` sits next to `main.hero`. So either way the program builds and prints `6`. If the other reading were true and `geom.hero` sat somewhere else, the path would not resolve and the compiler would refuse the program.
2. **Whether a folder named `internal` hides its modules from code outside it.** I took it as an ordinary path part, because the specification says nothing about `internal` and nothing about visibility. If it did restrict access, the compiler would refuse `main.hero`'s direct `use geom/internal/scan`. The smallest repair would then be to drop that `use` from `main.hero` and write `print(geom.area(2) + geom.area(1))`. That still prints `6`, but it changes what `main` calls.
3. **Whether a file `geom.hero` and a folder `geom/` can sit side by side.** I assumed they can: §1 maps `use geom` to `geom.hero` and gives `geom/...` paths their own folder meaning. The other reading would be an ambiguity error, and the compiler would refuse the program.
4. **Whether one module reached through two files is loaded once.** I assumed one module with two names, as §1 implies ("two files may name the same module differently"). Even if it were loaded twice, `twice` has no state, so the output would still be `6`.

## prediction

- I expect the compiler to accept the program.
- `heroes build main.hero` produces a binary with no errors.
- Running it prints exactly one line: `6`.

## context

The only inputs that reached me were the files in this directory: `brief.md`, `task.md` and `heroes-spec.md`. Apart from the standard agent system prompt (working directory and date), no other instruction, rule, contract, memory or document reached my context.
`````

### t1-y-2

`````markdown
I couldn't write `report.md` because I have no tool that writes files, only one that reads them. As the brief allows, the whole report follows here.

# report.md

## answer

**1. Does the compiler accept the program as written?** No. It refuses `main.hero` because of its line `use geom/internal/scan`.

The rule is in §1: "A path with `internal` as a part after its first is named only by the module the path before the last such part names and by the modules under it."

- In `geom/internal/scan`, `internal` is the second part, so the rule applies.
- The path before that part is `geom`. That names the module `geom.hero`, and the modules under it are the files in the `geom/` folder.
- So only `geom.hero` and modules under `geom/` may name `geom/internal/scan`.
- `use geom/internal/scan` in `geom.hero` is allowed. §1 says a `use` starts at the folder of the file you compile, so it reaches the same `geom/internal/scan.hero`.
- `main.hero` is neither `geom` nor under `geom/`, so its `use` is refused. That makes `scan.twice(1)` in `main` refer to a module that was never bound.

The program builds nothing and prints nothing. If the rule did not exist, it would print `geom.area(2) + scan.twice(1)` = 4 + 2 = `6`.

**2. Smallest repair.** Only `main.hero` changes. It drops the `use` it may not write and gets `twice(1)` through the public module `geom`: `geom.area(1)` returns `scan.twice(1)`, which is 2. `area` and `twice` stay exactly as written, and the program still adds twice 2 to twice 1.

`main.hero`
```
use geom

function main()
    print(geom.area(2) + geom.area(1))
```

`geom.hero` (unchanged)
```
use geom/internal/scan

function area(n: i64) -> i64
    return scan.twice(n)
```

`geom/internal/scan.hero` (unchanged)
```
function twice(n: i64) -> i64
    return n * 2
```

## choice_points

1. **What "under it" means.** I read "the modules under it" as the modules in the `geom/` folder, so the root-level `main.hero` is not one of them.
   - The other reading treats every module in the program as under `geom`. Then the original builds and prints `6`, the repair is unnecessary, and the repaired program also prints `6`.
   - I think that reading is wrong, because it would make the rule restrict nothing.

2. **Where `geom.hero`'s `use` starts.** §1 says a `use` starts at the folder of the file you compile, which is `main.hero`'s folder. So `geom/internal/scan` from `geom.hero` resolves to `geom/internal/scan.hero`.
   - If a `use` instead started from the folder of the file that writes it, the path would still resolve here, because `geom.hero` sits in that same folder. The result is the same.

3. **How to repair: change `main.hero` or the layout.**
   - I changed only `main.hero`, so it calls `geom.area(1)` (which is `twice(1)`) instead of `scan.twice(1)`.
   - Moving `scan.hero` out of `internal`, or adding a forwarding `twice` to `geom.hero`, would also build and print `6`. Each changes more files or adds a function, so neither is the smallest repair.
   - Keeping `use geom/internal/scan` in `main.hero` would be refused again under the same rule.

4. **Removing the `use` line instead of leaving it.** Leaving it would be refused for the `internal` rule. Even if it were allowed, §5 makes an unused binding a compile error, and nothing in the new `main` reads `scan`.

5. **Literal types.** `2` and `1` take `i64` from `area`'s parameter (§2), so the sum is an `i64`, which `print` accepts (§11). There was no real alternative here.

## prediction

- **Original program:** the compiler refuses `main.hero` at `use geom/internal/scan` (internal module named from outside `geom`). There is no binary and no output.
- **Repaired program:** I expect the compiler to accept it. It prints:
```
6
```

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this folder reached my context. Beyond that there was the standard agent system prompt, which gives the working folder and date and lists only a file-reading tool. No other instruction, rule, contract, memory or project document reached me.
`````

### t2-x-2

`````markdown
I can't create files: the only tool I have is Read. As the brief allows, the whole report is below instead of in `report.md`.

# report.md

## answer

**The task cannot be met as written.** The specification has no way to make a function private to its module. It has no `pub` or `private` keyword, no export list, and no rule that hides names starting with `_`. It also has no nested functions and no anonymous functions (§9), so `scale` can't live inside `cents`. §1 says `use money` binds `money` to all of `money.hero`'s declarations, and any module may write that `use`. Splitting `scale` into its own module or a package doesn't help either: §1 lets any module reach any module, and lets any module use a package's modules. So any module can name `money.scale`, and the compiler has no grounds to refuse it.

The files below meet the rest of the task. `probe.hero` is written honestly, and I expect the compiler to **accept** it, which breaks the task's requirement.

`money.hero`
```
# Money conversions.

# Multiplies `n` by 100. Meant for this module only, but the language
# offers no way to enforce that.
function scale(n: i64) -> i64
    return n * 100

# The euros, in cents.
function cents(euros: i64) -> i64
    return scale(euros)
```

`main.hero`
```
use money

function main()
    print(money.cents(3))
```

`probe.hero`
```
use money

function main()
    print(money.scale(3))
```

There is no `heroes.toml`: the program uses no package, so §1 says it has no catalogue.

What the compiler does with each root:
- `heroes build main.hero`: accepts it, and the program prints `300`.
- `heroes build probe.hero`: I expect it to accept it, and the program prints `300`. The task needs a refusal here, and the specification gives no way to get one.

## choice_points

1. **How to make `scale` private.** The specification offers no way to do it. I left `scale` as an ordinary top-level function and only noted the intent in a comment, which is the "merely discouraged" the task rules out.
   - Other options I considered:
     - Inline the multiplication in `cents` and drop `scale`. That breaks the requirement that `cents` work through `scale`.
     - Put `scale` in its own module, such as `money_internal.hero`. Any module can still `use` it.
     - Put it in a package. Any module can still reach it with `use <pkg>/...`.
   - Each of these gives a program the compiler accepts but that doesn't meet the task.
2. **How `probe.hero` names the helper.** I wrote `use money` and `money.scale(3)`, which is the real attempt from outside the module.
   - Writing a bare `scale(3)`, with or without `use money`, would be refused, because no `scale` is bound in `probe.hero` (§1: names from other modules are written qualified).
   - That refusal would only *look* like it meets the task. It comes from a name not being in scope, not from privacy, and the qualified call still gets through. I count it as gaming the test, so I didn't use it.
3. **Positional or named argument.** I wrote `money.cents(3)` and `money.scale(3)` with no label. Each function has a single parameter, so §9's rule that makes names mandatory doesn't apply. `money.cents(euros: 3)` should also compile, since §9's `Arg` grammar allows the label. Either way the output is the same.
4. **Unused function in a module.** In `probe.hero`, `money.cents` is never called. §5 makes unused *bindings and parameters* errors, and says nothing about unused functions. I assume a module's unused function is not an error. If it were, `probe.hero` would be refused for that reason, which would still be the wrong reason.
5. **`return n * 100` or a bare expression body.** §8 says a function with `->` must `return` on every path, so I used an explicit `return`. Leaving it out would probably be refused.

## prediction

- `heroes build main.hero`: accepted. Running it prints `300`.
- `heroes build probe.hero`: accepted, going by the specification. Running it prints `300`.
  - The task required a refusal. The specification gives no visibility mechanism, so it doesn't let a model predict or produce one.
  - If the real compiler does refuse `money.scale` from another module, that rule isn't in the specification. A model reading only the specification couldn't predict it or rely on it.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached me, plus my general system setup: the working directory path and the date. No other instruction, rule, contract, memory or document reached my context.
`````

### t2-y-2

`````markdown
I only have a file-reading tool, so I couldn't write `report.md`. As the brief allows, here is the whole report.

# report.md

## answer

**The task:** a module `money` offers `cents(euros: i64) -> i64`. It computes the result through a helper `scale(n: i64) -> i64` that multiplies by 100. Only `money` may use `scale`, and the compiler must refuse any other module that names it. `main.hero` prints `300`. `probe.hero` tries to call `scale` and must be refused.

**How it is done:** the specification has no `private` or export keyword. The only rule that makes the compiler refuse access is the `internal` path rule in §1: "A path with `internal` as a part after its first is named only by the module the path before the last such part names and by the modules under it."

So `scale` goes in a module at `money/internal/arith.hero`:
- The part before `internal` is `money`, which names `money.hero`.
- Only `money.hero` and the modules under `money/` may `use money/internal/arith`.
- `main.hero` and `probe.hero` sit beside `money.hero`. They are not under `money/`, so the compiler refuses that `use` in them.

Folder layout:
```
main.hero
probe.hero
money.hero
money/internal/arith.hero
```
The program uses no package, so there is no `heroes.toml`.

### `money/internal/arith.hero`
```
# Multiplies by 100; reachable only from `money` and modules under `money/`.
function scale(n: i64) -> i64
    return n * 100
```

### `money.hero`
```
use money/internal/arith

# The euros, in cents.
function cents(euros: i64) -> i64
    return arith.scale(euros)
```

### `main.hero`
```
use money

function main()
    print(money.cents(3))
```

### `probe.hero`
```
use money/internal/arith

function main()
    print(arith.scale(3))
```

**What the compiler does with each root:**
- **`heroes build main.hero`:** accepted. The binary prints `300`.
- **`heroes build probe.hero`:** refused at `use money/internal/arith`. The path contains `internal` after its first part, and `probe.hero` is neither `money` nor a module under `money/`.

## choice_points

1. **How to make `scale` private.**
   - Choice: put it in a module under `money/internal/`.
   - Other options: the specification has no keyword for this. If I had declared `scale` in `money.hero` itself, any module that does `use money` could call `money.scale(3)`. The compiler would accept `probe.hero` and it would run, printing 300. That breaks the task.
   - Another option is a top-level `internal/arith.hero`. The rule only counts `internal` "as a part after its first", so `use internal/arith` would be an ordinary path that any module may name. Again `probe.hero` would be accepted.

2. **Name of the module holding `scale`.**
   - Choice: `arith`, so the call is `arith.scale(...)`.
   - Other option: name the file `scale.hero` and call `scale.scale(...)`. As I read the shadowing rule, that should also compile, because a function name inside another module is always written qualified, so it does not clash with the module name. I avoided it in case the compiler counts it as binding one name twice.

3. **Which directory a `use` inside `money.hero` starts from.**
   - The spec says a `use` starts at the directory of the file you compile.
   - `money.hero` sits in that same root directory, so the alternative reading (start from the importing file's own directory) gives the same result here. Nothing turns on this choice.

4. **Positional or named argument for a one-parameter call.**
   - Choice: positional, `arith.scale(euros)` and `money.cents(3)`.
   - The spec makes names mandatory only when two parameters share a type. The `Arg` grammar allows an optional label, so `arith.scale(n: euros)` should also be accepted. A wrong label would be refused.

5. **How `probe.hero` tries to reach `scale`.**
   - Choice: it names the internal path directly. That is the only way to reach `scale` at all.
   - Other option: `use money` and then `money.scale(3)`. That is also refused, but for a different reason: `money` declares no `scale`. It would not test the privacy rule.

6. **The literal `3` in `money.cents(3)`.** The parameter's type makes it an `i64`, and it would default to `i64` anyway. `3 * 100` fits easily, so nothing overflows.

## prediction

- **`heroes build main.hero`:** I expect the compiler to accept it. The binary prints exactly one line: `300`.
- **`heroes build probe.hero`:** I expect the compiler to refuse it with an error at `use money/internal/arith`. The error should say that this internal path may be named only by `money` and the modules under it. No binary is produced.

## context

Nothing reached my context beyond the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), apart from the agent's system prompt (the working directory, the date, and the tool definitions). No other rule, contract, memory or document was read.
`````
