# DEFECTS — the compiler defects that are still open

Read by whoever attacks a defect. Every item here is a **measured** failure of
the compiler on a program — a crash, a wrong answer at exit 0, a silence where
a message is owed — with its reproducer, its cause where known, and what is
owed. It exists because the author said so on 2026-09-03: *"I do not like the
defect directory … if anything is still open in defect at the end, make one
single file called DEFECTS.md inside work, so that everything is tidy"*.
Seven files under `docs/defects/` became this list and six entries in
`docs/work/DONE.md` that evening; the directory is gone.

**Only open defects live here.** The moment one is repaired its entry is ticked,
gains a *The repair* section with the measurements that prove it, and moves to
`docs/work/DONE.md` — the record — as every other list in this directory does
(CLAUDE.md §3). A defect that stays here after its fix is the shape §3 was
amended to prevent.

**One notation: `- [ ]`.** A finding written as a bare bullet is invisible to
every count in this project. The body of an entry is indented under its line;
it may be long, because a defect's reproducer, cause and measurements are the
entry, not decoration.

Format: `- [ ] **NNN — <title>** | <date, found by> | <status> | <where it came from> | <severity>`, then the body.

- [ ] **010 — a `void *` C parameter bound as `@value: i32` is refused by clang, and the compiler calls it its own internal error at exit 2** | 2026-09-04, found by panel 108's ffi-pragmatist while compiling in-out shapes (`getsockopt`'s `void *optval`) | **open**; cause located, repair not started | `selfhost/emit/` pointee-width check (design.md §4.19, CLAUDE.md §7's four exit-1 classes) | a wrong exit code and a message that blames the wrong party — no crash, no wrong answer, but §7 says this is the author's line and exit 1

  **Reproducer.** An `extern` group declaring `function getsockopt(fd: i32, level: i32, name: i32, @value: i32, @len: i32) -> i32` against `<sys/socket.h>`, whose C parameter is `void *optval`. `heroes check` accepts the declaration; `heroes build` emits the pointee check as `_Static_assert(sizeof(void) == sizeof(int32_t), …)`, clang refuses `sizeof(void)`, and the compiler answers `internal error … clang refused the generated C`, **exit 2**. Measured on macOS arm64 (Apple clang 21.0.0) in the panel's frozen copy; the FFI seat's files are under the session scratchpad at `p108/ffi2/p108/` (`getsockopt_inout.hero`, first form), and the shape is one line to reconstruct from the sentence above.

  **Why it is a defect and not a diagnostic.** CLAUDE.md §7: a clang failure is exit 2 and says the compiler is wrong, with one named exception — a failure the author's own `extern` declaration caused is exit 1 and a diagnostic on the `.hero` line, in four classes recovered by `declaration()`. This is a fifth shape of the same exception: the author declared a pointee width for a parameter whose C type has none (`void *`), the generated assertion is unsatisfiable by construction, and clang's refusal names it. The narrowing is *whose declaration*, not *whose text* (§7), so it belongs on the `.hero` line.

  **What is owed.** Either the checker refuses `@x: <sized>` against a `void *` C parameter before any C is written (the header is available to the probe, and §4.19 already reads parameter widths off it), with a message naming `ptr` as the binding for an untyped pointer; or the emitter's pointee check recognises `void` and emits no `sizeof` for it, and the exit-2 path learns to recover this class into `declaration()`. The robust one is the first: the mistake stops before clang is asked. Attack the adjacent shapes before the commit (CLAUDE.md §1): `@x` against `const void *`, against an incomplete struct pointer, against a function pointer, and a `record` field of `void *`.
