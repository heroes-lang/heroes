- [ ] **M-vscode-extension** | neither thing that COLOURS a program knows any of the eight contextual marks, and no check compares their word lists | `editors/vscode/syntaxes/heroes.tmLanguage.json` · `site/src/lib/highlight.ts` · `.claude/rules/diagnostics-and-goldens.md` § A new surface form

    **Origin:** found at M-marked-acquisition step 4, 2026-09-14, while walking
    CL-036's list for `acquires`, and moved here because this is the milestone
    that will do it — the grammar is its subject and the site's highlighter has
    the same hole for the same reason.

    **Measured, not assumed**: the site DERIVES its keyword set from
    `selfhost/keywords.hero`'s own `keyword()`, so it follows the compiler for
    keywords automatically. The eight contextual marks are deliberately not
    keywords — that is what lets `owned` stay an ordinary identifier at 22 sites
    in `selfhost/` — so neither highlighter colours `owned`, `consumes`,
    `acquires`, `borrows`, `tag`, `partial`, `link` or `package`, nor `as`.

    **Why it was filed as a class and not patched for one word**: adding
    `acquires` to two files and leaving the rest uncovered would hide the gap
    rather than close it. `.claude/rules/diagnostics-and-goldens.md` already
    says what is owed and where it lives — *a check that compares each
    highlighter's word list against `selfhost/keywords.hero`'s keywords and
    `selfhost/inventory.hero`'s built-ins* — beside `suite_spec.hero`'s reading
    of `spec/reserved-words.md`. **Build the check first; the colouring follows
    from it, and a ninth mark then costs nothing to remember.**
