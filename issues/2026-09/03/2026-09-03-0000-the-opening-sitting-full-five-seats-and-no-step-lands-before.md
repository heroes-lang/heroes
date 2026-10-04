- [ ] **M-core-packages** | the opening sitting, full five seats, and no step lands before it | `design.md` §1.11, §4.15, §4.19, Part 6 · `docs/panel/028`, `032`, `036`, `039`, `049`, `097`, `099`

    **Origin:** author instruction 2026-09-03, out of the reasoning session
    recorded in `DESIGN-LOG.md:537`; the sixth question added by the author the
    same night. Put ahead of M-package-manager by the reorder late on
    2026-09-03, `DESIGN-LOG.md:539`.

    **Six questions, each measured before it is asked.** (i) The §1.11 boundary
    for a package written in Heroes alone: the ROADMAP's own test — *a binding
    is verified by clang against the header it names, and a standard library is
    verified by whoever wrote it* — puts a pure-Heroes `strings` on the second
    side, and the answer owes a falsifier (CLAUDE.md §12). (ii) Where packages
    live and how `use` reaches them: measured, `heroes test
    pkg/strings/strings.hero` runs a leaf's tests alone, but a module that
    `use`s a sibling package compiles only from the program's root (`use` cannot
    climb, panel 099 R1), so `heroes fetch` places a tree under the root and
    each tree wants a root-level driver — panel 032 R6 made concrete, with panel
    028 R3 keeping anything from being searched at run time. (iii) The byte
    buffer: a `ptr` of known length already becomes a `str` through
    `hero_str_from_bytes` (10 MB in 0.32 s), but `[u8]` is `error[ffi_type]` at
    check (`selfhost/check/ffi.hero:45`); three routes — per-byte runtime
    entries (soundness lane, ABI +1), a `[u8]` result admitted for a group over
    `heroes_runtime.h` (a diagnostic and a §4.19 sentence change, full lane), or
    a built-in (the route panel 036 refused for `read_file`). (iv) `net` as a
    runtime part: `sockaddr` differs between Darwin and glibc —
    `error[ffi_field_type]` on the other platform, measured both ways — and
    Windows is winsock; panel 097's `struct stat` shape, touching §1.11's row
    *Sockets: libc*. (v) Part 7 item 10 widened to a typedef whose width **or
    sign** differs by platform: `clockid_t` is `u32` on Darwin and `i32` on
    glibc, so `clock_gettime` has no single spelling and `timespec_get` is the
    portable clock. (vi) **Conditional compilation** — five shapes: C's textual
    `#if`; a compile-time keyword in the body (Nim `when`, D `version`, Odin
    `when`, Swift `#if`), whose inactive branch is not type-checked on this
    machine; Rust's `cfg` attributes, likewise; one file per platform (Go
    `net_linux.go`, Odin `_linux.odin`, Hare `+linux`), every file a whole
    module checked on its platform and no word in the body; and nothing in the
    language (Ada, Oberon), which is where Heroes stands.

    The record to hand the seats: panel 049 refused the platform axis with a
    veto (*"a platform question belongs where it is a measurable fact about the
    machine"*), panel 097 put the arm in `runtime/parts/`, panel 039 left
    comptime unplaced, §4.15 makes a textual difference semantic, and CI `cmp`s
    `seed/heroes.c` against what the compiler emits on every leg
    (`.github/workflows/ci.yml:515-520`), so host-dependent emission breaks an
    instrument. The limit to name: the runtime is the only C a package can add
    to (panel 036 P2 vetoed `compile "shim.c"`), so shape five serves the
    project's packages and nobody else's. **Not on the list**: the callback
    boundary — measured, no package needs it; it is M-isolated-threads' item
    below.

    **Where to look also:** `design.md` Part 7 item 10 ·
    `selfhost/check/ffi.hero:45` · `selfhost/modules.hero:125` ·
    `runtime/heroes_runtime.h`.
    **Why it matters:** a server cannot be distributed in Heroes today, and
    every reason is a compiler fact rather than a missing library.

    **Re-verified 2026-09-10: STILL OPEN, no sitting held** (`docs/panel/` ends
    at 125), **and it gained two questions.** `docs/ROADMAP.md` § M-core-packages now
    carries **(viii)** whether a group's header is the authority for its own
    declarations or the module's header set is — the compiler's own
    `selfhost/cli/process.hero:49-53` declares three `hero_os.h` functions under
    `extern "stdlib.h"` and compiles, because a TU includes every group's header
    (`selfhost/emit/unit.hero:32`) — and **(ix)** how thick a wrapper over a C group
    is, the author's question of 2026-09-10, whose already-settled half is panel 033
    R5: the wrapper is mandatory, not a matter of taste. **Two pointers moved**: the
    `[u8]` refusal is `selfhost/check/ffi.hero:89`, not `:45`, and the seed `cmp` is
    `.github/workflows/ci.yml:689-693`, not `:515-520` — the item's *"on every leg"*
    is correct, since that step sits in the single matrix job with `fail-fast:
    false`.
