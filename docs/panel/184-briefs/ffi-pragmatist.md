# Panel 184, brief for the ffi-pragmatist

Read `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/184-briefs/00-shared.md`
first: the sitting, its three questions, and every measurement they rest on,
with the commands. (Repaired on the completeness critic's first pass, 16:51
onward: `docs/panel/184-reports/completeness-critic-briefs.md`.)

**Your directory** is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/184-ffi-pragmatist/`:
`git -C /Users/joseph/Temp/heroes/heroes-lang archive a294a6ff | tar -x -C <it>`,
`rm -rf build`, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
Never build or run in the repository's working tree or another seat's
directory; in the working tree, read only this sitting's briefs directory,
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/184-briefs/`, and copy its
programs into your directory from there (`probes/after-*.hero`,
`probes/depth.py`). A program is built with `./heroes build <file> -o <bin>`
from your directory (the runtime is found under the working directory, or name
it with `HEROES_RUNTIME=<your directory>/runtime`).

## Your tasks, in order

1. **Question 1 at the C boundary.** A literal reaches C through `cstr`
   (`.lease()`, an `extern` parameter); `examples/gallery/13-lease.hero:23`
   is an `f` literal leased to C, and 00-shared.md measured its `f` removed as
   silent. **Name the C interfaces whose strings legitimately hold `{name}` or
   `{name}`-shaped text in a plain literal**, and verify each from its own
   documentation or headers rather than from memory: the coordinator thought
   of GLib's GVariant type strings (`"a{sv}"`) and D-Bus signatures, and
   checked neither; there may be others (message formats, templates, query
   languages). For the one you judge most likely in a real binding, write the
   binding and a program that passes such a literal, build it and run it on
   today's compiler, and say what each of 00-shared.md's routes (1a) to (1d)
   would do to it, by the route's own words, and whether the author of that
   binding has a spelling left under each (for (1a), an `f` literal with
   `{{`...`}}`: build it and say what bytes C receives).
2. **Question 2 at the C boundary.** Two things are measured already
   (00-shared.md § Question 2): `exit(code:)` is a BUILT-IN the checker knows
   (`spec/heroes-spec.md:319`), and the emitter drops a block no edge reaches,
   so the emitted C of `after-return.hero` holds no statement after the
   `return` and clang never sees one. What is left for this seat: a call to an
   `extern` C function that never returns (`abort`, `_exit`, `longjmp`, a C
   library's own fatal-error hook), which the checker cannot know leaves.
   Say whether the FFI can say so today (grep `spec/heroes-spec.md` § 13 and
   `selfhost/` for a way to mark it), what `missing_return` demands after one
   today (build the case), and whether 00-shared.md's one-predicate (2a) should
   reach it, which would need a mark on the binding: a new FFI word, with what
   it would cost a binding's author and what C's own `_Noreturn` /
   `[[noreturn]]` in a header gives the question.
3. **Question 3 at the C boundary.** Whether a Heroes source's nesting reaches
   the emitted C as nesting: emit C for 00-shared.md's shapes at the largest
   depth `check` accepts (`python3 <your copy of depth.py> <dir> <n>` writes
   the first ten; write the `&&` and method chains yourself), measure the
   deepest bracket and block nesting in that C, and set it against clang's
   `-fbracket-depth` (its default, from clang's own documentation or `clang
   -cc1 --help`, and the clang version you ran) and against C11's minimum
   translation limits (§5.2.4.1), quoted from the standard's text. A Heroes
   limit told as a message would be a promise about which programs build; say
   whether any C compiler this project targets (`.claude/rules/platforms.md`)
   would break that promise first, and whether an `extern` call nested in
   another (`c_f(c_f(c_f(x)))`, through a binding of your own) reaches C
   nested or through temporaries.

Your verdict per question: approve, object or veto each route, with the
condition that would change it; your veto is on ABI breakage or a binding the
route makes impossible to write, and it names the binding. Write your report
to `docs/panel/184-reports/ffi-pragmatist.md` in the repository AS YOU GO (the
one file you write there). No paid run (`claude -p`, an API call, a cloud run).
English, no em dashes.
