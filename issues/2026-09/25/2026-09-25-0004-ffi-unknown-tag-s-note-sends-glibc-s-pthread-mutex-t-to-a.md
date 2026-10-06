---
kind: feature
area: records
milestone: M-buildable-structs
filed: 2026-09-25
commit: none
github: none
---

- [ ] **M-buildable-structs** | `ffi_unknown_tag`'s note sends glibc's `pthread_mutex_t` to a handle, and following it aborts; the untagged record works | `docs/panel/178-reports/ffi-pragmatist.md` § 2 · `docs/panel/178-reports/completeness-critic.md` finding 8

    **Origin:** panel 178, 2026-09-25. The ffi-pragmatist bound
    `tag pthread_mutex_t`, which spells `struct pthread_mutex_t`, got
    `ffi_unknown_tag`, followed the note's advice to a handle, and got a null
    read inside `pthread_mutex_lock`, exit 134. The critic bound it under its
    typedef name with no `tag`, `record pthread_mutex_t partial`, and it locks
    and unlocks with 0 on Linux arm64 (x86-64 **unrun**). A diagnostic whose
    fix leads to an abort owes the spelling that works (design.md §4.17).
