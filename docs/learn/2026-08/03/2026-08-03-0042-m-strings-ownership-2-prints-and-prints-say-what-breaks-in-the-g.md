- [ ] **M-strings-ownership.2** | `print(0.1)` prints `0.1` and `print(1.0)` prints `1.0`. Say what breaks in the golden harness if the second printed `1` — the answer is not about readability

    **Where to look:** tests/golden/run/f64-rendering.hero (the comment) · runtime/runtime.c (hero_f64_render)
    **Why it matters:** it is the one place a type confusion becomes observable in the artifact the harness compares
