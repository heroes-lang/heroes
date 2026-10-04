- [ ] **M-core-packages** | the loopback HTTP server, one connection per thread | `design.md` Part 7 item 13 · `runtime/parts/thread.c` · `docs/panel/111`

    **Origin:** carried out of the callback-boundary item that closed at
    M-c-callbacks step 0, 2026-09-05. **Re-homed 2026-09-06**: the threads it
    needs landed at M-isolated-threads and the ten example programs are the
    witness Part 7 item 13 was owed; what is still missing is SOCKETS, which is
    that milestone's question (iv) — `sockaddr` differs between Darwin and glibc
    and Windows is winsock.

    **The permission that blocked it is gone and the witness was never
    written.** A loopback HTTP server answered `curl` from Heroes on 2026-09-03,
    single-threaded; the version with one connection per thread is what
    design.md Part 7 item 13 exists for, and it could not be written at all
    while a function value could not cross the FFI. It can now —
    `pthread_create` binds, runs and joins, measured 2026-09-05 — and what it
    will meet is the four corruption classes panel 111 built, which is precisely
    why it belongs at the milestone that makes them unreachable rather than at
    the one that opened the door. **Note what the guard does to it today**:
    every worker stops by name at its entry, so the witness is not runnable
    until isolation lands, and that is the correct state rather than a
    regression.

    **And the spelling is not portable, measured 2026-09-05 on both machines**:
    `pthread_t` is an opaque pointer on Darwin and an `unsigned long` of 8 bytes
    on glibc, so the same binding is `@thread: ptr` here and `@thread: u64`
    there — both compile and run on their own machine, neither compiles on the
    other, and the compiler says which with a `guess` fix on each. That is
    M-core-packages' platform-typedef question (item (v), `clockid_t`) arriving
    in a second place, and this milestone meets it first.

    **Why it matters:** a concurrency milestone with a model and no program
    decides nothing, and this is the program.

    **Re-verified 2026-09-10: STILL OPEN in substance, one sentence now FALSE.**
    Still open: no server program exists and nothing in the tree binds `socket`,
    `bind` or `listen`. **The false sentence is the guard's**: *"every worker stops by
    name at its entry, so the witness is not runnable until isolation lands."*
    Isolation landed at M-isolated-threads on 2026-09-06, and
    `runtime/parts/spawn.c:192-193` has `hero_spawn_enter` call `hero_thread_claim()`,
    so a Heroes-spawned worker is home and the guard never fires on it — today it
    refuses only a thread **C** made itself (`runtime/parts/thread.c:10-68`), which is
    the hole it was written for. `examples/threads/main.expected` shows eight threads
    answering. So the witness IS runnable now, and that is the item becoming cheaper
    rather than staler.
