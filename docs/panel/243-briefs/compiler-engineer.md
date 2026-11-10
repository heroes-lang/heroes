# Panel 243, compiler-engineer

Read `00-shared.md` first, whole. Your worktree: `.claude/worktrees/243-compiler-engineer`
(detached at `89db84347`; build your compiler there from the seed in two stages).

You judge what each route costs the compiler and whether it can be built so it refuses no
correct program. For each question:

- **Q1**: read `selfhost/emit/ffi_past_prototype.hero`, `ffi_call.hero` and `ffi_build.hero`
  and say, with file and line, where a parameter past `...` is decided today and what it is held
  to. Build route (R) in your worktree: an integer argument past `...` accepted where a prototype
  states its type, or where the callee is a format function with a literal format whose
  conversion for that argument names the width; refused otherwise, with the note naming the
  wrapper route. Run it on defect 2421's reproducer, on `printf("%d", <i32>)`, `printf("%ld",
  <i32>)` and `<i64>`, `%lld`, `%zu` with `u64`, `open(path, flags, mode)` with `mode` as the
  tree binds it today (grep `extern` groups for `open`), and every `extern` group of `selfhost/`,
  `tests/` and `examples/` that binds a variadic (grep for them and count them). Say what it
  refuses that builds today and whether each refusal is a wrong program. Measure in instructions
  what it adds to `heroes check selfhost/main.hero` and to a build of `examples/curl` if one
  exists. Say how route (C) would be written and what it leaves open.
- **Q2**: read `selfhost/cli/shadowed*.hero` and say where a `pkg-config` folder is told apart
  from a platform folder. Build the narrowest rule that lets defect 2428's reproducer build and
  still refuses panel 237's cases (the `shadowed_header` cases under `tests/golden/`, counted with
  a grep you name), and run them.
- **Q3**: build the lane's reading (a quoted include found from the includer's own folder is its
  own) and run it on the shapes and on the 2420 and 2343/2344/2356/2359 cases.

Report: per question, the routes you built, the commands, the counts, and your verdict with its
reason; a route you could not build is said so.

## After the critic's first pass

Read `00-shared.md` § After the critic's first pass and the critic's report. For Q1, build (R)
with its new reader and (R0) and (E), each over every by-value argument past `...`, and name for
(R): the format grammar it reads (`*` widths, `hh h l ll j z t L`, `%c`, `%n`), how a format
function is known (a list, or clang's `format` attribute in the AST dump, as `cli/renamed.hero`
reads `AsmLabelAttr`), and what counts as a literal through `.cstr()`, `.lease()`, a `str`
constant, a `format: str` parameter. Run each on the four tracked `i64` bindings and on
`twoarity`. For Q2, build routes (a) to (d) and run them on 2428's wrapper, the critic's
`sqlitewrap/` and `curldirect/` probes and panel 237's `.pc` probes (its `ffi-pragmatist.md:227`,
`critic-reports.md:200`), and say what form holds Q2's cases (`SETTABLE` widened, a
`tests/harness/` change, or another way to reach a `.pc`). For Q3, run every route on shapes `d`,
`e`, `g` and the critic's `q3danger/` shapes, in both group orders. List and rewrite the notes of
item 22 under your recommended route.
