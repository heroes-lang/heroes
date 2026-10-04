- [ ] **M-check-completeness golden ratification** | A test asserted `called from node_value` and was green on one platform for a day. **Before reading the repair: here is `hero_stack_blame`'s first five lines — say what `node_value` IS in that function, and what the test was therefore asserting.** | `runtime/parts/stack.c`, `hero_stack_blame` · `docs/panel/156-the-blame-line-was-never-a-platform-fact.md`

    ```c
    static const char *hero_stack_blame(uintptr_t pc, uintptr_t fp, uintptr_t lr) {
        Dl_info info;
        const char *first = NULL;
        if (dladdr((void *)pc, &info) != 0) {
            if (hero_stack_is_heroes(info.dli_sname)) return info.dli_sname;
            first = info.dli_sname;
        }
    ```

    `hero_stack_is_heroes` tests for an `h_` prefix. `node_value` is a C function
    from the program's own header.

    **Where to look after answering:** panel 156's § *the green leg was asserting
    a self-contradiction*, then `runtime/parts/stack.c`'s own comment at the
    handler, which says *"Nothing is printed when the walk finds no Heroes
    name"* — and compare that sentence with the code above it.

    **Why it matters.** `node_value` has no `h_` prefix, so it is `first`: the
    **fallback**, returned when the frame walk finds nothing. The test had been
    pinning the failure as the required answer, and it was green here not because
    this platform worked but because this platform failed in the way somebody
    transcribed. Three separate things were wrong with that one row and the
    repository could see none of them.

    **The question to carry away.** A test is supposed to be the instrument. Ask
    what distinguishes *a test that watches the world* from *a test that watches
    the program's current behaviour* — and then ask the same question of the
    golden this milestone wrote on its first morning, which replaced a unit test
    asserting `is_refusable` on a type-table entry. Both failures have the same
    shape and one of them was written by the session that criticised the other.
