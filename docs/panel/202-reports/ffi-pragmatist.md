# Panel 202, ffi-pragmatist

Copied by the coordinator at 23:29 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

**Verdict: veto** on route (b), and on route (a) if (a) means compiling every group's header together. **Approve with conditions** route (c) together with (g), plus a narrow form of (e) for `clash`.

**Section:** design.md §1.11 (`:467`; point 3 at `:570-571`, "a wrong type in an `extern` is a *compile* error"; point 2 at `:566-569`, the macro shim is "compiled with the program's own unit"). §4.19 (`:2334`; the drafted `static inline` shim at `:2378`). §1.12 for the wrong values. `.claude/rules/c-boundary.md:47` for the six classes. design.md does not cover header coexistence across modules at all; I say so.

## Experiment

The compiler was built in three stages (stage 3 printed `heroes 0.2.0` at 23:18:43). Apple clang 21.0.0 on this Mac; Debian clang 22.1.8 and GNU ld 2.44 in Docker. Every C file is under `.claude/worktrees/scratch-b15/202-ffi-pragmatist/c/`.

**C experiments.**
- **Link, two `static inline` of one name** (`c/link/st_*`): links at `-O0` and `-O2` on both platforms and prints `6 8`. `nm` shows a local `t _twice`, so this shape never clashes at link.
- **Link, plain C99 `inline`** (`c99_*`): `nm` shows `U _twice`. At `-O0` the link fails with an undefined symbol on Mac and Linux. At `-O2` it links and prints `6 8`.
- **Link, non-static definitions** (`ext_*`): `T _twice`, duplicate symbol at both levels on both platforms.
- **Link, a tentative definition** (`tent.h`, `int counter;` in a header two units include): links on Mac (`-fcommon`) but is a multiple definition on Linux. The verdict depends on the platform.
- **`clash`** (`c/clash`): prints `6 4.000000` at `-O0`/`-O2`. With `-flto` on ld64 and on lld there is no diagnostic, and it prints `6 -0.000000`. So route (h) catches nothing with clang. gcc's `-Wlto-type-mismatch` is unrun: there is no gcc in the image.
- **Macro capture** (`c/macro`): a.h `#define twice(x) ((x)*100)`, b.h `static inline int twice`. In order b then a, clang accepts it and prints `300 400`; the function call became the macro.
- **Include stack for 550** (`c/inc`, plus the real fused units `runs/inc1/build/tu-456548f023c6493e/main.c` and rlx11's). By default, clang 21 and 22 print no include stack for the note. With `-fdiagnostics-show-note-include-stack`, the same run prints `In file included from main.c:7: / In file included from X11/Xlib.h:44: / X11/X.h:100: note: previous definition`. The unit's line 7 is the group's `#include`, so route (g) needs no second clang run. `-H` also prints the tree in the same run, but it lists every header with no unit line numbers. `-MD` gives a flat list, no graph.

**Heroes programs** (`p/`, results in `verbs1.txt` and `verbs2.txt`). `run` exited with the same code as `build` in every row except `c99`.

| case | check | build / run | test |
|---|---|---|---|
| `macro3` | 0 | 0, prints `300 8` | 1: `left: 400 right: 8`. The `--emit-c` file compiled alone prints `300 400` |
| `macro4` (binds `twice(x: i64)`, but b.h takes `int`) | 0 | 1, `ffi_parameter_type` (true) | 1, `ffi_return_type` "`twice` returns `int64_t` in `b.h`", which is **false** |
| `rlx11` (raylib + X11) | 0 | 0, prints `7 true` | 1, the **old false message** "`raylib.h` … does not compile". This is 550 on real libraries |
| `rlmath1` (raymath.h module first, raylib.h second) | 0 | 0, prints `1.0 7` | 1, "whichever comes first" |
| `rlmath2` (swapped) | 0 | 0 | 0, all passed. So that note is false |
| `tasn1a` / `tasn1b` (libtasn1 + OpenSSL) | 0 | 0 | 1 / 0, the same order flip |
| `shim2` (two modules, each with the compiler's own drafted `hero_WEXITSTATUS` shim, identical text) | 0 | 0, prints `3 2` | 1, "redefinition of 'hero_WEXITSTATUS'" |
| `c99` (header-only C99 `inline`) | 0 | build 1 `ffi_missing_link` "no group says which library has it", which is false; `run` 0, prints `6` | — |

The critic's ten cases reproduce on my build. `clash` builds and runs at exit 0 printing `6 4.0 10`.

## Census

Each header was compiled alone and then in every ordered pair, with `-std=gnu11 -fsyntax-only`, the compiler's own standard.

| platform | headers that compile alone | ordered pairs | failed | unordered pairs that fail | order-dependent |
|---|---|---|---|---|---|
| Mac | 156 of 178 | 24,180 | 32 | 24 of 12,090 | 16 |
| Linux | 75 of 77 | 5,550 | 180 | 92 of 2,775 | 4 |

- **Mac, cross-library:** 18 pairs:
  - X11 × raylib (`Font`)
  - curses and ncurses × raylib (`KEY_*` macros)
  - term.h × FLAC, GLFW, SDL2, SDL3, libavcodec, libavformat
  - libtasn1 × openssl/evp.h, openssl/ssl.h, openvpn-plugin.h
  - oniguruma and oniggnu × regex.h and pcre2posix.h, plus pcre2posix.h × regex.h
- **Mac, same library, one order only:** raymath.h then raylib.h (`Vector2`), rlgl.h then raylib.h (`Matrix`).
- **Linux:** 90 of the 92 pairs are kernel uapi headers against glibc. The other 2 are libtasn1 × OpenSSL.
- **Headers that define an external symbol:** 0 of 156 on Mac, 0 of 75 on Linux. The instrument was checked first: it reports `T` for ext_a.h, and `C` (Mac) or `B` (Linux) for tent.h.
- **Plain C99 `inline` without always_inline:** 264 hits on Mac. 146 are raymath.h, and libraylib exports them. The remaining 118 are the SDK's ctype and wctype functions, seen through the SDL, CoreFoundation and PortAudio headers; whether libSystem exports them is unrun. 0 on Linux.

## Argument (≤120 words)

Route (b) puts every group's header into one unit. Real libraries do not survive that: 18 cross-library pairs on Mac and 92 on Linux, 20 of them order-dependent. Each one builds today in two modules, which is C's own remedy. One unit also breaks importc verification: `macro4` is checked against another group's macro and told a false message, and `macro3` computes 400 where build prints 8. Heroes has no `heroes cc` (`unknown command`), and the shim design.md `:568` prescribes is compiled into that same unit. So nothing escapes: the compiler's own drafted shim (`shim2`) is refused. Route (c) keeps every pair, and `-fdiagnostics-show-note-include-stack` gives 550 its graph in the same run.

## Prediction

Under route (b), `heroes build` of `p/rlmath1` (raymath.h bound in `vec`, raylib.h in `gfx`, `use vec` first) exits 1 with "redefinition of 'Vector2'", and `p/rlmath2` exits 0 printing `1.0 7`. Under route (c), both exit 0 printing `1.0 7`. Under (b), `macro4`'s build says `ffi_return_type` naming `b.h` instead of today's true `ffi_parameter_type`.

## Condition

My verdict changes if any of these holds:
- A rerun of this census finds no cross-library pair that fails in one unit.
- Heroes gains a verified C-shim unit (`heroes cc`).
- Route (b) arrives with per-group units (route f), shown to keep by-value records and `constant` verified. Measured in C (`c/wrap`): raylib + X11 split across wrapper units does build and run, but a `constant` becomes a load, not a constant expression.

My approval of (c) is conditional on four things:
- `onedef` and `extdef` become exit 1, as a sibling of the linker member at `c-boundary.md:47`.
- `clash` is refused at `check` by comparing parameter types across groups. Results need clang's types because of §13's widening, which is the compiler-engineer's question.
- 538's note loses "whichever comes first" and says "put the two groups in two modules".
- `--emit-c` stops printing a unit that means another program.

## New, unfiled as far as my grep found

My grep searched for "C99 inline", "plain inline", "inline definition", "include-stack" and "whichever comes first"; a negative here rests on those words.
1. `macro3`: `test` and `--emit-c` compute 400 where build gives 8.
2. `macro4`: a false `ffi_return_type` in `test`.
3. 538's note "whichever comes first" is false (`rlmath`, `tasn1`).
4. 550 reproduced on raylib + X11.
5. `shim2` refused by `test`.
6. `c99`: build and run disagree, and `ffi_missing_link` is false for a header-only C99 `inline`.
7. Tentative definitions in a header: the link verdict depends on the platform.

## Unrun

- Windows: raylib × windows.h; the box was not used.
- gcc's LTO type check.
- Heroes-level runs on Linux; Linux is C-level only.
- Any duration: the load average was 56 to 89, so I took no timings.
- rlx11 in the swapped `use` order.
- Route (g) wired into the compiler; only clang's output was shown.

Files are in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/202-ffi-pragmatist/`:
- `notes.txt`
- `verbs1.txt`, `verbs2.txt`
- `census/mac/` and `census/linux/` (`pairs.tsv`, `unordered.tsv`, `errs/`, `defs.tsv`, `c99inline.tsv`)
- `c/`, `p/`, `runs/`
