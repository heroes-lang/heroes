- [ ] **M-optional-map** | The leak that only existed with a computed value: `{1: "one" + "!"}` then `.default(…)` lost one block, `{1: "one"}` lost nothing. Explain the difference in one sentence, then say what it implies for how every counting test in this repo has to be written

    **Where to look:** DESIGN-LOG 2026-08-10 (Op::MapGet) · runtime/runtime.c (HERO_STR_STATIC)
    **Why it matters:** the cheaper spelling of every one of these tests would have passed
