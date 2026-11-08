# Panel 237, the proposal

Convened 2026-11-08 by the coordinator of batch 24 on decision 2342
(https://github.com/heroes-lang/heroes/issues/2342), under M-package-manager, whose
panel gate was asked once at panel 235. **Full panel**: two of the three items have a
reader-facing half, a diagnostic class and a line of the specification.

1. **`shadowed_header`, landed on robustness** at `96d42c250` (defect 2333). Before any
   probe compiles, one `clang -v -M` over the runtime's own opening lists the C library
   headers the compiler's C reads; a listed file that lies in a folder the build names (the
   program's own folder, one `--include` names, one a package's `pkg-config` answers)
   stands in for the library's header, and the build is refused at exit 1, naming the
   file, the folder and the library header, the fix said in a note (rename the file or move
   it out of the folder), with no `Fix`.
2. **A package's `-D` or `-U` of a name the guard keeps** (`emit/guarded_names.hero`: every word the
   runtime's headers and the library's standard headers write, and the names C reserves that the
   compiler's C writes; decision 2342 calls them *a name C reserves*) is refused as `ffi_package`,
   landed with it; the compiler's own two `-D` switches excepted.
3. **Spec § 13's first fenced block binds `function sqlite3_close(db: Db consumes) -> i64
   when 0`**, held out of the round at `ebdc8a28c` (defect 2322, repaired at `37fb22678`),
   and design.md §4.19's copy of the example the same.

The questions are the brief's `00-shared.md` § The questions.
