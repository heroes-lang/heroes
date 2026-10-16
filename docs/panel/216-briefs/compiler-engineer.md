# Panel 216, the compiler-engineer's brief

Read `docs/panel/216-briefs/00-shared.md` first, whole: the question, the measurements and the
rules you keep are there.

## Your seat's question

**What it costs the compiler, and whether it is sound.** Of the routes the shared brief lists, and
any you find, which can the compiler carry: where it would sit (the accessor's value region,
`selfhost/emit/value_check.hero` and `selfhost/emit/constant_accessor.hero`; the readers,
`selfhost/emit/ffi_constant_value.hero`; or a unit of its own beside the layout check,
`selfhost/cli/layout.hero`), how many lines in which directories, and what a cold `--emit-c` of the
compiler itself and a cold build of a program binding one constant cost with it. Build the route
you would adopt in your copy and run it on the four shapes, then on the `run/`, `unsupported/` and
`emit/` golden forms (`./heroes run tests/harness/main.hero -- ./heroes <form>`), and say what
moved.

## Pointers

- `selfhost/emit/value_check.hero`: the region the accessor's `return` stands in and the warnings
  it makes errors; its head says what it does not catch (lines 35-42).
- `selfhost/emit/ffi_constant_value.hero`: how clang's refusal there is read back onto the
  constant (`told`, line 79; the code `ffi_constant_type`, line 90).
- `selfhost/emit/constant_accessor.hero`: the accessor (`accessor`, line 48).
- `selfhost/cli/header_ask.hero` (and `switch_ask`, `unswitch_ask`, `two_ways_ask`): units of
  questions of their own against a program's headers, `-ferror-limit=0`, read back error by error,
  the nearest model if the route asks clang anything; `selfhost/cli/layout.hero` is another.
- Routes F (the trap read at build time) and H (a filter, then a second question) from the
  critic's first pass are on the table with A to E: build the one you would adopt, F first if you
  have no better reason, since it alone was measured to split the 44 shapes on clang 18.
- The goldens of defects 2071 and 2072, `tests/golden/unsupported/fixedbugs-2071-*` and
  `fixedbugs-2072-*`: the messages a value told there carries today.

## What to return

`verdict` (adopt, object, veto) per route, `section` (design.md's, or CLAUDE.md's where design.md
is silent: say which you searched), `cost` in lines and in measured time, `prediction` (falsifiable,
with the instrument that scores it), `condition` (what would turn your verdict).
