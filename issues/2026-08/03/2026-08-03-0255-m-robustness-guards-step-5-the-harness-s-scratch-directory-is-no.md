- [ ] **M-robustness-guards step 5** | the harness's scratch directory is now `build/harness-<pid>`. Say what two harnesses started at the same second wrote into one `build/harness` before, and why the emission BLESSING (`UPDATE_EMISSION=1`) was the one place where that turned into a file committed as truth — then say what `is_a_capture` checks that a byte comparison could not

    **Where to look:** tests/harness/main.hero (`scratch`) · tests/harness/suite_emission.hero (`is_a_capture`, `judge`)
    **Why it matters:** the blessed traces have no other source of truth: the compiler that produced them is archived
