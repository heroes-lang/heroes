# Panel 183, brief for the ffi-pragmatist (the sitting widened)

Read `docs/panel/183-briefs/00-shared.md` first, then the compiler-engineer's
report, `docs/panel/183-reports/compiler-engineer.md`, whose finding widened
the sitting to your seat: its § Task 1 (c) and § The verdict. The sitting sat
without you on the premise that nothing here reaches the C boundary; the
critic ran an `extern` group and the compiler-engineer measured that the
landed rule, amended, touches **what the author of a binding sees**. You are
asked whether that is so, and whether it is right.

**Your directory** is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/183-ffi-pragmatist/`.
It holds `pe-selfhost.patch` (the compiler-engineer's prototype `PE`, a diff of
`selfhost/layout.hero` and `selfhost/next_line.hero` against `171e8c45`) and
`probes/` (seven `extern` programs: `x1_extern_stray.hero`,
`x2_extern_spec_stray.hero`, `x2b_member_mistake.hero`,
`x2c_member_mistake_alone.hero`, `extern_member_eof.hero`,
`extern_member_unclosed.hero`, `e17_extern_below.hero`). Build two compilers
there: `NEW`, from `git -C /Users/joseph/Temp/heroes/heroes-lang archive
171e8c45 | tar -x -C <a folder>`, `rm -rf build`, `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`; and `PE`, a second archive with the
patch applied (`patch -p0` or by hand; the diff's paths name the engineer's
folders, so strip them) and its compiler rebuilt from `selfhost/` with the
first (`./heroes build selfhost/main.hero -o heroes-pe`, about a minute).
Never read or run in another seat's directory or the repository's working
tree.

## The rule under judgement (the compiler-engineer's `PE`)

The reach of the brackets still open ends at a line whose first word no
bracket holds in a program that compiles (a declaration's first word, a
statement's, `if`, `match`, `function` only before a name) at a margin no
deeper than the line the outermost bracket opened on, and for `else` a margin
strictly shallower; the lexer names every opener still open there and lays the
line out as what it begins. Inside an `extern` group, a member line (column 4,
`function`, `constant` or `record`) is such a line when a member above it left
a bracket open.

## Your tasks

1. **Run the seven probes under `NEW` and under `PE`** (`heroes check` on
   each, both arms, `--permissive` too) and give both outputs. For each, say
   what a binding's author reads, what they would fix, and whether the fix is
   right; x1's stray `)` after `sin(x: f64)` is told under `PE` as
   `expected_extern_signature` (*expected a `function`, a `constant` or a
   `record`, found `)`*) where a body's stray `)` gets `expected_end_of_line`:
   is that the message a binding's author needs, or does it send them to the
   wrong fix?
2. **Does anything reach the C boundary?** Headers, types, widths, marks, the
   emitted C, the ABI, a binding that compiles today: run the census of every
   tracked `.hero` file holding an `extern` group (`git ls-files '*.hero'` then
   grep `^extern`), `check --brief` and exit, `NEW` against `PE`, both arms,
   and for the ones that compile, `heroes build --emit-c` under both and `cmp`
   the C.
3. **The C a real binding needs**: write one binding of your own, to a real
   header on this machine (`math.h` or `string.h`), with one member whose `(`
   is left open and one stray closer below, compile it under both, and say
   whether the messages would get its author to a binding that compiles in
   one turn.

Verdict (approve, object, veto on ABI breakage), condition and prediction, as
your seat gives them. Write your report to
`docs/panel/183-reports/ffi-pragmatist.md` in the repository AS YOU GO (the one
file you write there). English, no em dashes.
