- [ ] **M-struct-passing close — the offers** | M-selfhost-port, operators | **1 << 63 must be INT64_MIN, not a trap — how does the emitted C make a UB-free left shift reach the sign bit?**

    **Where to look:** emit_operator.hero's shift arm
    **Why it matters:** the unsigned round-trip is what lets a mask set name its own top flag while staying defined for every input
