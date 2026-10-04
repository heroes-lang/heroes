- [ ] **M-separate-compilation step 7 (the cheapest way to be correct is to be useless)** | `tests/harness/suite_cache.hero` pins three things, and the second and third exist only because of the first. If a build system only had to pass "a header edit is not invisible", one line of code would do it: never reuse anything. Say what each of the other two cases forbids, and why "only `bind.o` moved" is a statement about acceptance row 1 rather than about the cache

    **Where to look:** tests/harness/suite_cache.hero · docs/panel/033 (acceptance row 1)
    **Why it matters:** a test that only checks the loud direction lets the quiet failure through
