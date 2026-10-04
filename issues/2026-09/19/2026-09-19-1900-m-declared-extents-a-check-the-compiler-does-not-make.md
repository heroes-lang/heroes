- [ ] **M-declared-extents walkthrough** | Defect 063: `sl_fill(p: s.name.ptr(), n: 64)` on an 8-byte field wrote past it at exit 0, and the repair does **not** compare 64 to 8 in Heroes. Before reading: the emitter writes `_Static_assert((int64_t)(64) <= (int64_t)sizeof(h0_s.name), …)` into the generated C. **Two shapes make a Heroes-side comparison wrong where this one is right — a record the group declared `partial`, and an extent written as a header `constant`. Which way does each fail?** | `selfhost/emit/lend_extent.hero` § THE CHECK IS C'S

    **Where to look after answering:** a `partial` record's size is **C's**, not
    the field list's, so Heroes does not know it — and `sizeof` does. A group
    `constant` lowers to a **call of its accessor**, `t5 =
    h_konst_SL_NAME_LEN()`, so a check wanting a literal would refuse the
    careful reader who named the header's macro and accept the careless one who
    typed a number. Both were run; the second is panel 166's critic's finding.

    **The question to carry away.** The check that landed compares two numbers
    neither of which the compiler holds. Ask what the compiler DOES hold here —
    and why that is enough.
