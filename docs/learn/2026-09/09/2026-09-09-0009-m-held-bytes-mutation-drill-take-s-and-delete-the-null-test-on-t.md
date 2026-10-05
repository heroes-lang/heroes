- [ ] **M-held-bytes, mutation drill** | Take `runtime/parts/str.c`'s `hero_held_release` and delete the NULL test on the cell; say which golden goes red, and why the magic check below it cannot take the NULL test's place for a foreign pointer | `runtime/parts/str.c` · `docs/panel/125-the-guard-that-read-freed-memory.md` § What the two seats measured | a guard that must dereference before it can validate is undefined on exactly the inputs it promises to catch

    **Origin:** M-held-bytes close, 2026-09-09.
