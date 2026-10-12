# Panel 213, the compiler-engineer's brief

Read `00-shared.md` first. Your seat judges the ceiling: implementation cost,
core against sugar, and soundness (`.claude/agents/compiler-engineer.md`); you
have a veto on soundness.

**Your inputs are the live compiler, never `archive/`**: `selfhost/emit/mangle.hero`
(`slot`), `selfhost/emit/prologue.hero` (the prologue that hoists every local),
`selfhost/ir/containers.hero` (`SlotKind`: `param_slot`, `local_slot`,
`synthetic_slot`, and `return_slot`, which the census found as a fourth),
`selfhost/emit/` broadly (what `#line` and the C names look like), `selfhost/cli/flags.hero`
(`debug_words`, `level_words`), `selfhost/cli/table.hero` (the verbs and flags),
`runtime/heroes_runtime.h` (the descriptor and the two headers).

**Your task**: price each route of the shared brief in files and lines of
`selfhost/` and `runtime/`, and rule soundness: which route can show a WRONG
value or a wrong name at exit 0 (a formatter out of step with the binary it
reads, a name map from another build, a variant whose tag the formatter reads
from the wrong offset), and what makes that impossible rather than unlikely.
Measure in your copy:
1. Route E's reach: could the emitter name an author's local `p` instead of
   `h4_p` where no clash exists, or carry the author's name in the debug
   information some other way? What breaks (C keywords, shadowing across
   blocks, two slots of one spelling, the determinism and emission suites)?
2. Route F: of a frame's locals, which can the emitter keep out of the
   debugger's view (temporaries in a nested scope or one struct, an artificial
   mark clang honours), and what does it cost the emitted C's size and the
   compile time (`/usr/bin/time -p` on `heroes build selfhost/main.hero`)?
3. Whether the `$`-slot distinction must survive the mangler, and the smallest
   way it can (a prefix the mangler keeps, a table beside the binary, nothing).
4. **Build the route you would adopt far enough to run**, in your copy: at least
   one shape shown with the author's name in `lldb -b` on a program of
   `tests/golden/lldb/` or 042's shapes program, the transcript in your report.
A route that does not build is not adoptable (`.claude/skills/panel/SKILL.md` § 3c).
