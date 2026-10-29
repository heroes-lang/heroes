# Panel 232, the ffi-pragmatist's brief

Read `00-shared.md` first, and the critic's findings it cites
(`docs/panel/232-reports/critic-briefs.md`; its programs in
`.claude/worktrees/232-critic/critic-scratch/`, read and never run there).
Your worktree is `.claude/worktrees/232-ffi-pragmatist/`; your report
`docs/panel/232-reports/ffi-pragmatist.md`, written as you go. You compile
and run the C a binding needs; a claim about the boundary not compiled is a
question in your report, not a finding.

**Item 3, `==` on a handle given back.** § 13: *Giving one back ends it:
handing it to C again aborts, `==` on it aborts, and so does a copy made
before, unless C has since handed its address out again or a million other
lives have ended.* Read design.md Part 8 wart 20 beside it, written in the
same commit (`c8b1bc878`, F13), and say whether the two read the clause the
same way. What the tree does (F9): `==` on the handle ITSELF after it is
given back aborts (the binding is emptied to the dead page and
`hero_handle_eq`, `runtime/parts/alloc.c`, aborts on it); a COPY made before
compares by its address, and the golden
`run/fixedbugs-a-freed-object-read-through-a-stale-copy-is-not-a-null-handle`
pins `null: false`. The runtime already remembers the given-back addresses
(`hero_dead_mark_locked`, `hero_dead_holds_locked`, `HERO_DEAD_WINDOW`, `1 <<
20`), and consults them at a crossing into C, not at `==`. Build and run the
golden, then the shapes beside it: a copy made before against the handle, a
copy against `nullptr`, two copies of one handle, a copy after C handed the
address out again. Then the two routes, each built and costed: `==`
consulting the remembered set (a lock and a hash probe per comparison,
measured on a program comparing handles in a loop, never estimated), or the
clause narrowed to what the runtime does. CLAUDE.md § 12: the specification
beats the compiler unless the sitting rules otherwise.

**Item 6, the promise at the C boundary.** Two headers, not one (F15):
`runtime/hero_os.h`, whose functions decision 2228 promised *with the rest at
M-compatibility-promise* (design.md Part 7 item 2, *File I/O*; it declares
57 functions, F15), and `runtime/heroes_runtime.h`, whose entry points
without a place task 2237 removes after the next release, and which the
compiler itself binds (`selfhost/cli/help.hero`, `selfhost/io.hero`, F15).
Say which declarations of each a `1.x` program may bind, what a
`HERO_RUNTIME_ABI` step does to one, and what the paragraph
(`docs/panel/232-briefs/paragraph-draft.md`) must say of a C header the
program binds that changes under it, a library the promise does not ship.

**And the author's rule** (`00-shared.md`): no route that keeps an older
binding compiling by a form of the language.
