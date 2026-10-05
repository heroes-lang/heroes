- [ ] **M-c-callbacks.3** | **`hero_thread_is_home` is never set to false, on any thread, anywhere. Why is the guard still correct?** Read `runtime/parts/thread.c` — it is short — and say which one is doing the work: that C11 zero-initialises a thread-local in each new thread, that `hero_args_set` runs before any Heroes code, or that a worker thread never calls `hero_thread_claim`. Then say what would break if the flag were an ordinary global instead

    **Where to look:** runtime/parts/thread.c · runtime/parts/os.c
    **Why it matters:** a guard whose false case is never written is the cheapest kind there is, and the whole of its correctness is in a sentence of the C standard
