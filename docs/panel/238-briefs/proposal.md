# Panel 238, the proposal

Convened 2026-11-08 by the coordinator on the author's instructions of that day, about 16:15,
given in Italian in the conversation and rendered here in English as meant (CLAUDE.md § 11). A
**`retro-record`** sitting: the decision is the author's; the sitting records the real objections,
measures the costs, settles the details the author left open, and says where each rule lands. It
lands nothing in the compiler. Milestone: M-package-manager, whose panel gate was asked once at
panel 235.

1. *"Add that the formatter should group the `use` lines by kind, perhaps putting first those in
   angle brackets and then those from the file system, similar to Go's formatter."*
2. *"The order does not matter anyway, since two equal names cannot be used; it is only something
   nice to look at; then, within a group, go in alphabetical order."*
3. *"Then use as the index the folder at the project's root, something like `packages`."*

**The coordinator's reading of 3, told to the author, who may correct it**: the repository's
static index moves from `examples/index/` (panel 235's R15, ratified; 14 catalogues
`examples/*/heroes.toml` say `index = "file://../index"`) to a folder `packages/` at the
repository's root. **The other reading**: a catalogue with no `index` line defaults to a `packages/`
folder at the program's project root. The synthesis states it as the alternative, priced, with what
would decide between them; the coordinator's reading is adopted unless a seat measures it impossible
or unsound.

As a spec diff: **none expected when the briefs were first written**. `spec/heroes-spec.md` says
nothing about the formatter (`00-shared.md` F16); the rule lands in design.md §4.15, in
`selfhost/print/`, and in the tree reformatted. The critic's first pass made it a question (a
comment's two meanings in spec section 1, and the reader-facing half), so the spec-warden sits on
it, on the vendored tables only.

The questions are `00-shared.md` § The questions, A to E.
