# M-vscode-extension — the extension, complete


**Scheduled, no warrant.** `editors/vscode/` already ships the TextMate grammar,
the language configuration and the icon theme; this milestone makes it an
extension somebody could install and forget about.

- **The LSP client**, speaking to M-lsp-server's `heroes lsp`: diagnostics as you
  type, hover, go-to-definition, document symbols, formatting through
  `heroes fmt`.
- **Code actions from the fixes that already exist.** §4.17's `Fix`es are tagged
  `certain | guess` and `heroes check --apply` already applies the certain ones;
  the extension surfaces exactly those as quick fixes and never the guesses. This
  costs almost nothing and is the thesis made visible in the editor — the
  likeliest mistake arrives with its repair pre-written.
- **Debugging**, and the honest shape of it first: the emitted C carries `#line`
  back to `.hero` (with `-g` repaired at M-selfhost-port), so the debug info is
  ordinary DWARF pointing at Heroes source. `lldb-dap` therefore composes with
  the generated binary without this project writing a debug adapter — which is
  CLAUDE.md §10's *"nothing if two existing invocations already compose to it"*.
  **The launch builds at `-O0`** (`heroes build`): since panel 197 that is the build
  whose debug information describes the variables, and `heroes run`'s `-O2` carries
  line tables alone, where stepping works and the variables pane is empty.
  If a launch configuration cannot be expressed that way, `heroes dap` enters
  under the stopping rule like any other verb, with the reason recorded. **The
  ceiling stated here used to be design.md §2's, and M-typed-inspection is the row
  that removes it** (amended 2026-09-06, in the commit that scheduled that row):
  `p x` showing a mangled C temporary rather than a Heroes value was true of every
  build until then, and the variables pane shows whatever lldb's formatters show,
  so this milestone inherits the answer instead of documenting the ceiling. What is
  unchanged is the ruling: `lldb-dap` composes, this project writes no debug
  adapter, and an editor **consumes** formatters rather than producing them. The
  sentence is corrected rather than deleted, because a bullet that states a limit
  the compiler has already lifted funds the wrong decision at the next sitting.
- **Packaging**: a `.vsix` that installs, with `heroes doctor` as the extension's
  own health check.
