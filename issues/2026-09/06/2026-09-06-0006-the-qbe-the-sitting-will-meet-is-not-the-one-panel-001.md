- [ ] **M-qbe-backend** | the QBE the sitting will meet is not the one panel 001 rejected | `design.md` §3.2 · `DESIGN-LOG.md:6`, `:8` · `selfhost/emit/extern_probe.hero`

    **Origin:** measured 2026-09-06 from this Mac, after the author asked
    whether the backend should move to QBE at the end of the chain; `qbe` is not
    installed here, so what c9x.me says is READ and not run. Its home is that
    milestone because every fact below is about the tool it builds on, and the
    sitting that opens it is the reader; nothing here convenes anything.

    **Two of design.md §3.2's sentences about it have expired.**
    `https://c9x.me/compile/releases.html`, read 2026-09-06: release **1.3
    (2026-06-01)** says *"Windows ABI support"*, and release **1.2
    (2024-02-16)** added *"new experimental `dbgfile` and `dbgloc`
    directives"*, which is line-level debug information. `design.md:702-704`
    says QBE has *"no DWARF"*; the site's front page still lists amd64 (linux
    and osx), arm64 and riscv64 with no Windows target, so the two pages
    disagree about Windows; and the IL document (`doc/il.html`) does not describe
    the two directives yet. **So the sitting's first step is to install QBE 1.3
    and RUN it** — `arm64_apple` on this Mac, the Windows target on the box
    (`docs/ref/environment/windows/WINDOWS-MACHINE.md`) — before a line of emitter
    is written or a sentence of §3.2 is amended: a platform fact that has not
    been run on that platform is an inference (CLAUDE.md §1).

    **What did not expire is the reason the answer to the author's question is
    *a second backend, never a replacement*.** The IL has no way to include a C
    header or to check a declared signature against one (read the same day), and
    the verification of every `extern` in this compiler is clang reading the
    header. Counted in `selfhost/emit/` on 2026-09-07: **34** `_Static_assert`,
    **35** `_Generic`, **10** `__builtin_classify_type`, **7** `__typeof__`,
    **4** `__builtin_types_compatible_p`, **48** `#line`, **5**
    `__builtin_*_overflow`. **20 of 54** program directories under `examples/`
    (those holding a `main.hero` or `whole.hero`) declare an `extern`, and each
    would lose the header check. The runtime (**751** lines in `runtime.c` and
    the header, **4911** in `parts/`) and the seed (**747,095**) are C and still
    want a C compiler, and QBE itself emits assembly for `cc out.s` — so QBE
    adds a dependency and removes none. The `--sanitize` configuration in
    `tests/harness/suite_corpus.hero::configurations()` is clang's; QBE has no
    sanitizer. **And clang's DWARF has two consumers now, not one**:
    M-typed-inspection, scheduled 2026-09-06, is built on `-g`, on `#line` and
    on lldb reading the C types, so a second backend must say what a stopped
    program shows under it, or say plainly that it shows nothing. **Where the
    milestone sits is unchanged and the author's**: after the tools and before
    the books, for the 2026-09-03 reason that the IR should have stopped moving
    first (`docs/ROADMAP.md` § Who scheduled what). **Moved behind
    M-publication-gate on 2026-09-15, by author instruction**, and still after
    the tools and before the books; `docs/roadmap/scheduling.md` carries the
    move.

    **Where to look also:** `design.md` §3.2 (`:698-707`) · `docs/ROADMAP.md`
    § M-qbe-backend, § M-typed-inspection ·
    `tests/harness/suite_corpus.hero::configurations` ·
    `https://c9x.me/compile/releases.html`.
    **Why it matters:** a sitting that argues from a 2026-08-03 description of a
    tool that has shipped two releases since is a sitting convened on a premise
    that expired in silence.

    **Re-verified 2026-09-10: STILL OPEN, six of the seven emitter counts
    exact.** In `selfhost/emit/`: `_Static_assert` **34**, `_Generic` **35**,
    `__builtin_classify_type` **10**, `__typeof__` **7**,
    `__builtin_types_compatible_p` **4**, `#line` **48** — all as written — and
    `__builtin_*_overflow` is **6**, not 5. Both unamended design.md sentences are
    still there (`:723` *"no DWARF"*, `:721` *"no headers"*). **Four numbers moved**:
    §3.2's QBE bullet is `design.md:717-726` not `:698-707`; `runtime.c` plus its
    header is **776** lines not 751; `runtime/parts/*.c` is **5013** not 4911; and
    `seed/heroes.c` is **773,509** not 747,095. Externs are **20 of 55** program
    directories, the numerator unchanged. **And one stale citation found beside it**:
    `design.md:718` calls the QBE backend *Part 7 item 14*, where item 14 is
    declaration visibility and QBE is item **15** (`design.md:2807`) — the same slip
    the ROADMAP's own cells carried until 2026-09-03.
