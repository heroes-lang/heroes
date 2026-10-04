- [ ] **M-robustness-guards step 4** | the handler runs on an ALTERNATE stack (`sigaltstack`) and Windows reserves one with `SetThreadStackGuarantee`. Say in one sentence why a handler for *the stack is full* cannot run on the stack, and where the name `main.down` in the message comes from when the stack it would walk is the one that overflowed

    **Where to look:** runtime/parts/stack.c (`hero_stack_guard_install`, the frame walk)
    **Why it matters:** the message names the function; that name has to come from somewhere the fault did not destroy
