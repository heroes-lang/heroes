---
kind: feature
area: cli
milestone: M-package-manager
filed: 2026-09-07
commit: none
github: none
---

- [ ] **M-package-manager** | a bindings module is invisible to `heroes check` | `selfhost/cli/check.hero` · `docs/panel/091` · `docs/panel/082` R3

    **Origin:** panel 091, found by the ffi-pragmatist unasked. Load-bearing
    from M-separate-compilation onward.

    `heroes check badbind.hero` on a module that is nothing but an `extern`
    group is **exit 0 with zero output** — `check` never runs clang, so nothing
    verifies the group — while `heroes build` on a caller that reaches one of
    its two declarations reports `error[ffi_return_type]` **on the bindings
    module's own line**. This is the same shape as panel 082 R3's *check accepts
    ⇒ build succeeds* gap, one department over, and it becomes load-bearing
    exactly here: separate compilation is what makes a pure bindings module a
    normal thing to write, and M-package-manager is what makes it a thing you
    **distribute**. Whoever fixes it should read 082 R3 first — that item
    refused running `mono` inside `check` on a measured cost, and running clang
    inside `check` is the same trade at a larger price.

    **Why it matters:** a check that accepts everything is not a check, and a
    distributable binding is the one artifact whose whole value is that somebody
    verified it.

    **Re-verified 2026-09-10: STILL OPEN, and the mechanism is unchanged.**
    `selfhost/cli/check.hero` reaches no clang and no probe — the only `probe` in it
    is an unrelated local counter at `:234-241` — while clang is reached on the build
    path alone, `selfhost/cli/produce.hero:57` and `:295`. No fixture exists: the
    `badbind` name appears in `docs/panel/091`'s brief and in this list, nowhere
    else.
