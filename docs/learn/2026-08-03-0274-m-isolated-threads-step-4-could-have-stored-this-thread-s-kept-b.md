- [ ] **M-isolated-threads step 4** | `parts/alloc.c` could have stored this thread's kept buffer in a `_Thread_local` pointer, which is two words, and stores it in a `pthread_key_t` instead, which is twenty lines. Read panel 111's transcript of `worker: live=3` and `destructor: live=0` at the SAME address, then say in your own words what a destructor can and cannot see

    **Where to look:** runtime/parts/alloc.c § the kept buffer · docs/panel/111 § Point 3 is broken twice over
    **Why it matters:** it is a fact about the machine that no amount of reading the C standard would have given, and both seats found it by running it
