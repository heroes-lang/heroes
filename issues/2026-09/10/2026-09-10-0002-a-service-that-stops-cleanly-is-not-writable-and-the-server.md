- [ ] **M-core-packages** step 6 | a service that stops cleanly is not writable, and the server step is where that stops being theoretical | `spec/heroes-spec.md:47` · `selfhost/check/ffi.hero:262-268` · `runtime/parts/thread.c:10-68`

    **Origin:** author decision 2026-09-10, § What production-ready means row 6,
    its third silence.

    **What is true today, measured.** A signal handler is *declarable*: a callback
    may stand as a **parameter** (`selfhost/check/ffi.hero:262-268`), never as a
    result. But **there are no mutable globals** (`spec:47`), so a handler has no
    way to record that it fired — it can only call `exit`, which is not a graceful
    stop. So `SIGINT` on a server that should drain its connection and close its
    database is not writable in Heroes, in any spelling, and nothing in the record
    says so.

    **Why it is homed on the HTTP server step and not on the opening sitting**: it
    binds nothing until there is a server to stop, and the step that writes one is
    where the answer is cheap or the wall is real. **What it must not quietly
    become** is a mutable global: that is Part 6, permanently. The shapes to price
    are a runtime part that owns the flag and answers a Heroes call, and the
    server's own loop asking between connections.
