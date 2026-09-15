# Panel 153 — brief for the compiler-engineer

Read `docs/panel/153-briefs/00-shared.md` first. Your report goes to
`docs/panel/153-reports/compiler-engineer.md` — write it in your COPY and hand
the text back in your final message; the coordinator writes the file.

## What you judge

The ceiling (design.md §1.1, §1.7, Part 5): implementation cost and
core-versus-sugar, for **Route A** (fields on a handle, read through the
pointer, a new word after the tag) and **Route B** (two records on one tag plus
a `read` built-in), and for **any third route you can name**. You have a veto.

## Where the live compiler keeps each piece — `selfhost/`, never `archive/`

- **What a handle is**: `selfhost/handles.hero` — `is_handle_parts`,
  `is_handle_record`, `c_spelling` (the author's word, `struct` only where
  clang asked, since today), `duplicate_tag`, `handle_map_key`.
- **One tag, one type**: `selfhost/check/decls.hero:285`, `one_tag_one_type`,
  17 lines, keyed on the tag's text.
- **What a type reaches**: `selfhost/check/reaches.hero` — `reaches_handle`,
  `handle_suffixes`, the walk through group-record fields and fixed arrays.
- **The C name of every declared type**: `selfhost/emit/ctype.hero`,
  `new_names_qualified` — `handle_decls`, `aggregates[index]` is `T *` for a
  handle and `struct T` for a fielded tagged record.
- **Reading a field**: `selfhost/emit/access.hero`, `field_read` (line 124) —
  today it writes `.` on a value; Route A needs `->` on a handle.
- **The group's record as C's struct**: `selfhost/emit/extern_record.hero`,
  `selfhost/emit/ffi_record.hero`, `selfhost/emit/extern_field.hero`
  (`c_spelling` of a field's type against the header).
- **The parser's marker words**: `selfhost/parse/members.hero` —
  `consumes_marker`, `borrows_marker`, `acquires_marker`, `owned_marker`;
  `selfhost/parse/decl.hero` for `tag` and `partial` on a record line.
- **Every tool that re-prints a program** and would have to learn a new word
  (`.claude/rules/diagnostics-and-goldens.md` § A new surface form): the
  formatter `selfhost/print/fmt.hero:448`, the dump `selfhost/print/dump.hero:93`,
  `heroes mutate`, `editors/vscode/syntaxes/heroes.tmLanguage.json`,
  `site/src/lib/highlight.ts`.
- **The copy-in/copy-out of `@`**: `selfhost/emit/signature.hero:104` and
  `selfhost/emit/ops.hero`'s call emission; `selfhost/cli/pointee.hero` asks the
  header what a numeric `@` points at.
- **The null guard the runtime already has** for a `cstr`:
  `guard_cstr_arguments` in `selfhost/emit/`, and `runtime/parts/` for the
  abort shapes — a read through a null handle must abort, never segfault
  (design.md §1.12, CLAUDE.md § Precedence rank 3).

## What your report must carry

1. **Lines and modules** each route touches, counted against the files above,
   and whether any file crosses `tests/harness/suite_layout.hero`'s `DECIDED`
   ceiling (`selfhost/emit/ctype.hero` sits at a decided 395).
2. **Core or sugar**: can Route B's `read` be written in Heroes over what exists
   (it cannot today, say why in one sentence), and is Route A a new kind of type
   or a flag on a record.
3. **What breaks**: `one_tag_one_type`'s reasoning under Route B; `==` and map
   keys on a fielded handle under Route A (`handle_map_key` refuses a handle as a
   key: does it still?); `reaches_handle` when a handle carries a field of its
   own type (a CYCLE — the walk has `seen`, check that it terminates).
4. **The null read**: what the emitter must write so `ai.ai_family` on a
   `nullptr` handle aborts with a word. Measure the segfault first in your copy
   if you can produce it with hand-written C.
5. **A prediction** with the command that scores it.

Cheap route: the seed compiler, `clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes`, ~3 s. Prototype nothing in the repository; your copy only.
