---
kind: feature
area: editors
milestone: M-vscode-extension
filed: 2026-10-12
commit: self
github: https://github.com/heroes-lang/heroes/issues/2028
---

- [x] **M-vscode-extension** | F5 on a `.hero` file builds it at `-O0` and debugs it with `lldb-dap`, and the compiler's diagnostics land in the Problems view | `editors/vscode/extension.js` · `editors/vscode/package.json` · `editors/README.md` § Debugging · `tests/harness/suite_spec.hero` § The plugin's wiring

    **Origin:** author instruction 2026-10-12, *add debugging to the VS Code
    extension*. The shape was already ruled by the Debugging bullet of
    `docs/roadmap/milestones/M-vscode-extension.md`: the emitted C carries
    `#line` back to the `.hero` file, a `heroes build` at `-O0` carries debug
    information that describes the variables (panel 197), `lldb-dap` composes
    with that binary, this project writes no debug adapter, and an editor
    consumes lldb's formatters rather than producing them.

## What was done

The extension gained its first script, `editors/vscode/extension.js`, plain
JavaScript with no dependency and no build step, in this file's own commit
(`self`). It adds a `heroes` debug type whose configuration resolver builds the
program with `heroes build <file> -O0 -o <folder>/build/debug/<path>` once
VS Code has substituted `${file}`, and returns a configuration of the
`lldb-dap` type for the binary, passed through the LLDB DAP extension's own
two resolvers where it registers them as commands (llvm-project `0e0ea714f3`).
A failed build launches nothing: the compiler's text goes to the Heroes output
channel and its diagnostics to the Problems view, read by the same three
problem matchers a `heroes` task uses (`$heroes`, `$heroes-brief`,
`$heroes-hole`). `package.json` declares `contributes.breakpoints` for `hero`,
the debugger, the task type, the matchers, the setting `heroes.path` and a
limited workspace trust; the engine stays `^1.64.0`. No verb or flag was added
to the compiler: the launch is two existing invocations, and the stopping rule
was not reached.

No `extensionDependencies` on LLDB DAP: its manifest on LLVM's main branch asks
for VS Code 1.90, and a dependency would take this extension's highlighting
from 1.64 up with it; F5 checks for it before building and offers to show it.

The formatters were looked for and none exists:
`git grep -n -i -E "formatter|type summary|command script" -- selfhost runtime docs editors`
named only the code formatter before this change, and M-typed-inspection is
still scheduled. What
the launch turns on instead is lldb-dap's own `enableAutoVariableSummaries`,
which shows `h4_p = {f_x:3, f_y:4}` and `h0_label = {ptr:"probe", len:5}` on
the collapsed rows, 74 ms against 55 ms for a frame of the compiler of 877
locals.

Measured on this Mac with no VS Code: `lldb -b` breaks on `probe.hero:9` and
`frame variable` reads `h1_sum = 12` and `h4_p = (f_x = 7, f_y = 2)` at the
second hit; a Node driver speaking the Debug Adapter Protocol to `lldb-dap`
with the configuration the extension returned verifies the two breakpoints,
stops at `probe.hero:17:11` and then `probe.hero:9:11`, and reads the locals
and a record's fields; the script loaded in Node over a stand-in for the
`vscode` module builds and returns that configuration, refuses a broken
program with the compiler's first line and its two diagnostics at 3:11 and
4:5, a hole as one Information problem, a module's error on the module's file,
and launches nothing where LLDB DAP is missing. `spec/colouring` now also
holds both manifests' categories, every file `package.json` names, and the
declared language being named by the breakpoints and by a debugger; with
`main` moved and the breakpoints emptied it read `spec: 30 passed, 1 failed`,
naming both. VS Code itself, Linux and Windows are unrun.
