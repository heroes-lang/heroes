- [ ] **M-closures-verdict step 1** | `heroes check` refuses a comparison of two `str` with a message naming two types, while `ops.hero` accepts ten. Write the four-line program that provokes it, then find the three places one string is passed to `value_errors.bad_operand`, and say which of the three fires for `-"x"`

    **Where to look:** selfhost/check/ops.hero:81-90, :131, :168 · docs/work/DEFECTS.md 018
    **Why it matters:** errors are a deliverable here, and this one tells a reader holding a `u8` to convert a value that needed no conversion
