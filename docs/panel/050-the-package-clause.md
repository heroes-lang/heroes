# 050 — `package`: the machine is asked, not told

**Status** `retro-record — author decision, 2026-08-14`
**Lane** none convened; the evidence is panels 047, 048 and 049, all five judges

## The decision

The author, after reading panel 049's refusal: *"these topics are too complex for
me, you choose — but follow the principle of sparing no expense on this aspect,
because it is central, so I want the most complete and robust solution possible. I
want Windows too."*

So this file is a **retro-record**: the decision is taken, and what follows is the
argument and the objections that are real, not staged dissent.

## What landed

**One clause.** `extern "raylib.h" package "raylib"` asks the machine — through
`pkg-config` — where a library's headers and libraries are and what else it needs.
Measured cost **+123** (2983 → 3106).

It subsumes the entire platform axis panel 049 refused at +144, and it does so
**without putting a machine's name in a program**:

| what panel 049's axis would have said | what the machine answers |
|---|---|
| `link macos framework "Cocoa", "OpenGL"` | `-framework Cocoa -framework OpenGL` where that is the answer |
| `link linux "GL", "X11", "m", "dl"` | `-lGL -lX11 -lm -ldl` where that is the answer |
| — (could not be said at all) | `-I/opt/homebrew/include -L/opt/homebrew/lib` |

The last row is the one that mattered: **the measured blocker at rung 5 was the
search path**, four ways over two sittings, and design.md:2058 had said so a
milestone before anyone looked.

## The acceptance test ran

`examples/raylib/main.hero`, in a clean environment, with no platform token:

    $ env -u LIBRARY_PATH heroes run examples/raylib/main.hero
    INFO: Window closed successfully
    the window opened and closed

**§4.19's ladder rung 5 is climbed.** It had been unclimbable for a milestone, and
three panels were spent finding out why.

## The security property is the feature

A `.pc` file is **input this program did not write** — installed by a package
manager, editable by anyone who can write to a prefix. Go shipped this exact idea
without a filter and it became **CVE-2018-6574**: a repository could ship
`attack.so` beside `// #cgo CFLAGS: -fplugin=attack.so` and `go get` loaded the
plugin into the host compiler. The February 2018 fix was an allow-list, and Go's
`security.go` still opens with *"We must avoid flags like -fplugin=, which can
allow arbitrary code execution during the build."*

Heroes arrives second and gets the list on day one: **`-I`, `-L`, `-l`, `-F`,
`-framework`, and nothing else.** Measured:

    error[ffi_package]: the package `evil` answered with `-fplugin=/tmp/attack.so`,
      which this compiler does not pass on
      at evil.hero:2:5
      note: only `-I`, `-L`, `-l`, `-F` and `-framework` are accepted: everything
        else is a flag a package file could use to run code during the build

Exit **1**, on the author's line. The list is named in the spec, because the list
*is* the safety.

## Windows, and what "I want Windows" actually cost

The author asked for it. A name in a document is a promise; a job is a
measurement — so what was done is the second.

**The runtime now compiles for Windows.** Panel 049 measured 18 errors under
mingw, every one in `runtime/parts/f64.c`: `locale_t`, `newlocale`, `uselocale`.
The port is four lines of `#if` and it makes the code *better*, not merely
portable — POSIX's `uselocale` **switches** the thread's locale and must be
switched back, so every early return between the two calls is a bug waiting to be
written, while the Microsoft CRT takes the locale as an argument to the conversion
(`_snprintf_s_l`, `_strtod_l`) and removes the window entirely.

    $ zig cc --target=x86_64-windows-gnu -c runtime/runtime.c   →  0 errors
    $ … f64.c runtime.c -o f64.exe  →  PE32+ executable (console) x86-64

A whole Heroes program cross-compiles and links for Windows. **`windows-latest`
is now the third CI leg**, and what it does or does not pass is a measurement this
file does not pre-judge.

Two POSIX assumptions were removed on the way, and the second was a defect the
first was hiding: two `.hero` files wrote to `/tmp`, which Windows does not have —
and the *reason* they did was that `golden.rs` ran the `-O0` leg from the crate's
directory and the other two from the workspace root, so a relative path was a
different file in each. **The workaround had outlived its cause and the
disagreement it hid was still there.** All three legs now run from the root.

## Objections, real and recorded

- **The blind reader wanted `link` to take a list**, and `package` does not give
  it one. A program binding two libraries by name still needs two groups or one
  `link`. That gap is open.
- **`package` needs `pkg-config`.** A machine without it gets a diagnostic naming
  the alternative (`link`), not a silent fallback — but it is a dependency the
  compiler did not have yesterday, and CLAUDE.md §10's "never a script" deserves
  the reading that this is a *question asked of the system*, in the same class as
  invoking clang, rather than a build script. That reading is the author's to
  overturn.
- **A library with no `.pc` cannot use it.** `link` remains for exactly that, and
  is unchanged.
- **The corpus harness now skips a program whose library this machine lacks.** It
  is a fact about the machine, read from the compiler's own diagnostic rather than
  from a list of names, and the count is floored — a machine missing most of the
  corpus fails instead of passing green.

## What this does NOT solve, and the record must not be read as claiming it does

**383 of raylib's 600 entry points pass or return a struct by value.**
`ClearBackground(Color)` does not compile: Heroes has seven boundary types and
none of them is a struct. Rung 5's text promises *"real struct passing and
framework linking"* — the second half is now reachable through `package`, and the
first is untouched. **That is the next wall**, it is a type-vocabulary question,
and it is bigger than everything in this sitting.

## Predictions to score

| # | prediction | instrument |
|---|---|---|
| 1 | `examples/raylib/main.hero` builds and runs on the Linux CI leg with `libraylib-dev` installed, from the same `package "raylib"` line and no platform token | the CI matrix |
| 2 | The Windows leg builds the compiler and passes `cargo test`'s crate tests; the corpus legs needing sqlite3/curl/raylib skip, and the skip floor holds | the CI matrix |
| 3 | No `.pc` on any of the three runners answers with a flag outside the allow-list — if one does, the diagnostic fires and names it, which is the design working rather than failing | the CI matrix |
