# Panel 213, the historian's brief

Read `00-shared.md` first. Your seat is precedent, **verified by web search**;
unsourced precedent is inadmissible (`.claude/agents/historian.md`). Write
your report to `docs/panel/213-reports/historian.md` in the sitting's tree.

**Your task**: how other compiled languages that lower to C or to native code
show their own values in a debugger, each claim sourced with a URL you read:
- **Rust**: `rust-lldb` and `rust-gdb` (wrapper scripts that load the pretty
  printers), the `.debug_gdb_scripts` section and `#[debugger_visualizer]`
  (RFC 3191), what the compiler embeds and what the debugger loads;
- **Swift**, **Go** (`runtime-gdb.py`, Delve), **Nim** (its `nim-gdb` and the
  names it emits for locals), **Zig**, **Odin**, **Crystal**, **D**, and any
  language that compiles to C and maps its names back;
- **LLDB itself**: whether it auto-loads formatters embedded in a binary
  (summary strings, the formatter bytecode of recent LLVM releases, a script
  section), since which release, and the security setting that governs scripts
  beside a binary (`target.load-script-from-symbol-file`);
- **Microsoft's `.natvis`** (embedded in a PDB, or beside it) for the Windows
  question.
For each, what it chose, what it cost and what broke, marked `verified` only
when the page you read says it. A language the sitting merely compares itself
with is not prior art (`.claude/rules/records.md` § And the record says whose
idea it was).
