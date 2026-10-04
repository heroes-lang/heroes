- [ ] **M-struct-passing close — the offers** | M-selfhost-port, structural eq/hash (emit_structural.hero) | **eq and hash are one invariant, and the day they disagreed a map lost a key it held.** Which three eq rows were missing until 2026-08-12, why did hash already cover them, and what did {Box: i64} do?

    **Where to look:** emit_structural.hero's doc
    **Why it matters:** "silent on insert, loud on lookup" is the failure signature of every eq/hash drift — knowing it saves an afternoon
