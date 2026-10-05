- [ ] **panel 027** | `to_i64`'s range check, and why reasoning about it fails. Two checks a reviewer would sign off are wrong: `v <= (double)INT64_MAX` **accepts** 2^63 and `v > -9223372036854775809.0` **rejects** `INT64_MIN`. Question: what does `(double)INT64_MAX` round to, and why does the correct check use a hex float and a half-open interval? Then: on arm64 an unchecked cast does not trap — say what `inf` and `NaN` silently become

    **Where to look:** runtime/runtime.c (hero_f64_to_int) · docs/panel/027 R7
    **Why it matters:** 8 of 13 probe values are UB under the raw cast and the hardware hides all of them
