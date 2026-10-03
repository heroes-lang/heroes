# Panel 188, the ffi-pragmatist's brief

Read `00-shared.md` and `00-facts.md` in this directory first; they bind you.
You judge the founding constraint (your charter,
`.claude/agents/ffi-pragmatist.md`): everything comes from C, and a refusal
that refuses a real header is a library the author cannot bind.

## What to compile, in `<scratchpad>/188-ffi-pragmatist/`

1. **The C each route implies.** For every character of F1, F6 and F7 (`>`,
   a line end, `\`, `"`, `'`, `//`, `/*`, a space, `<`, a trigraph, a tab, a
   NUL, the empty name), a header of that name on disk and a unit `#include
   <name>` holding the name's VALUE (route (1f): one backslash where the
   Heroes string writes `\\`), compiled with
   `-fsyntax-only` and then to an object: what clang says and whether it finds
   the file. On this Mac (Apple clang, `clang --version`), and in the Linux
   arm64 container (`docker run --rm heroes-linux-arm64 ...`, Debian clang
   22.1.8; clang 18 by `apt-get update -qq && apt-get install -y -qq clang-18`
   inside it, the image holding no apt lists until the update), one
   container at a time (`docker ps -q` empty first). The Windows box is
   offline (F1): if `ssh -o ConnectTimeout=20 win true` answers, run there in
   `/c/w/188-ffi/`, else write it unrun.
2. **What a strict rule would cost** (route (1c)): count the real header names
   holding any character outside letters, digits, `.`, `_`, `-`, `+` and `/`,
   under this Mac's SDK (`$(xcrun --show-sdk-path)/usr/include`), under
   `/opt/homebrew/include` if present, and under the Linux image's
   `/usr/include` and `/usr/local/include`; list every one found, by path.
   Nothing found is a measurement only with the command written beside it.
3. **Route (1i)**: the same headers handed to clang by `-include <path>`
   instead of an `#include` line: what it finds and what it says.
4. **Q5**: a `link` and a `package` string holding the same characters: what
   the linker and `pkg-config` do with them, on this Mac and on Linux.

Report per route: the C you compiled and what it did, per platform; the real
headers each route would refuse; your verdict, with a falsifiable prediction
and the condition that would change it.
