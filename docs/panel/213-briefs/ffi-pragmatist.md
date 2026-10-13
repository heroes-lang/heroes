# Panel 213, the ffi-pragmatist's brief

Read `00-shared.md` first. Your seat is the founding constraint (everything comes
from C, design.md §1.11 and §4.19) and the C a mechanism really needs; you
have a veto on ABI breakage (`.claude/agents/ffi-pragmatist.md`).

**Your task, by writing and running the C and the debugger scripts, not by argument**:
1. **The formatter that reads today's runtime**: write the lldb formatter for
   `HeroArrayHeader` (elements through `elem`'s `size` and the descriptor's
   symbol name), `HeroMapHeader` (its three regions), a `HeroStr`, a `T?`
   (one live arm) and a variant (its tag and payload), against a binary of
   your copy built at `-O0` (`tests/golden/lldb/` or 042's shapes program), and
   show `lldb -b` printing Heroes values. Python for lldb, or lldb's own type
   summary and synthetic-children commands, whichever you find works; say which.
2. **Route C, measured** (a hint from the critic, not a measurement: this Mac's
   LLDB framework holds the strings `__lldbformatters` and `__lldbsummaries`): does lldb on this Mac (`lldb-2103.0.34.103`) load
   formatters embedded in the binary by itself (any section it reads at load:
   a summary-string section, a formatter-bytecode section, a script section),
   and does it on the Linux image (`docker run --rm heroes-linux-arm64:latest
   lldb --version` and a build inside it), and through `lldb-dap`? Write the
   smallest C that embeds one formatter and show the debugger using it, or show
   that it does not. A negative claim names what you tried.
3. **Route B's loading**: how a module beside the binary gets loaded with no
   one typing it (lldb's auto-load of a script beside a binary or a dSYM, its
   security setting, an `.lldbinit` in the working folder, the extension's
   `initCommands`), each tried.
4. **What it costs the ABI**: does any route need `HERO_RUNTIME_ABI` to move
   (`runtime/heroes_runtime.h`), a field added to a descriptor, or a symbol
   kept that `-O2` or a strip removes? `nm` a binary at `-O0` and `-O2`.
