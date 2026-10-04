- [ ] **M-handle-verdict 4** | Open `selfhost/handles.hero` — it is one short file — and read `is_handle_parts`. It asks four questions and joins them with `&&`. Now take `record Db tag sqlite3` and follow it through the compiler: `selfhost/parse/tails.hero` decides it is legal, `selfhost/emit/ctype.hero` decides how it is spelled in C, `selfhost/check/contextual.hero` decides that `nullptr` is its null. Each of those three asks `handles.is_handle`, and none of them has its own copy of the question. Before reading anything else, answer: **why is that one file there at all, rather than the four lines being written where each pass needs them?** Then run `./heroes build examples/sqlite/main.hero --emit-c -o /tmp/x.c` and `grep -c 'sqlite3 \*' /tmp/x.c`.

    **Where to look:** `selfhost/handles.hero`'s module comment;
    `docs/panel/145-a-handle-is-a-pointer-with-a-name-and-the-compiler-already-reads-the-name.md`;
    `docs/records/log/2026-09-13-1400-a-handle-is-a-pointer-with-a-name-and-the-form-is-a-quarter-of-what-it-was-priced-at.md`;
    CLAUDE.md's opening paragraph.

    **Why it matters:** five scouts were sent to price this form and they came
    back with **four definitions of what a handle is, in four different homes** —
    one in the parser, one in the checker, one in the emitter, one in the
    mutation operator. Every one of them was defensible on its own. Together they
    were four places for the answer to drift, and CLAUDE.md's very first rule is
    that each rule is written in exactly one place and everything else cites it.
    The file exists because the alternative was measured and rejected, not
    because tidiness is a virtue.

    **And the second half is the harder question.** Look at what
    `is_handle_parts` does NOT ask. It does not ask whether the header actually
    leaves the type opaque — and a refusal for exactly that was proposed, written
    into a brief, and then **withdrawn on evidence**: `record File tag FILE`
    compiles and writes through `FILE *` correctly, and `FILE` is a complete type
    on this Mac. The fieldless form *means the pointer*, so completeness of the
    tag is irrelevant. A check that had looked obviously right would have broken
    a legitimate binding, and only compiling it found that out.
