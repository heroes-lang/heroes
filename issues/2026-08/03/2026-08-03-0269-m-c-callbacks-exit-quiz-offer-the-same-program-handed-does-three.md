- [ ] **M-c-callbacks** | exit-quiz offer: the same program, `atexit` handed `nullptr`, does three different things on three machines. Name them, then say what that proves about where a compile-time refusal could ever have lived

    **Where to look:** docs/records/journal/033-c-callbacks.md § What surprised · tests/golden/fixedbugs/ffi-a-null-function-pointer-says-so.hero
    **Why it matters:** the answer is that NULL-tolerance is a property of the C library, not of the type and not even of the C function
