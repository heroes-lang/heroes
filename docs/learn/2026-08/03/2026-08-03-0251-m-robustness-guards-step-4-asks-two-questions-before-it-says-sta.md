- [ ] **M-robustness-guards step 4** | `runtime/parts/stack.c` asks TWO questions before it says *stack exhausted*: where the faulting address is, and where the stack pointer is. A wild store 8 KiB under the stack answers the first one yes. Say what it answers to the second, and which of the two witnesses alone would have made the handler lie

    **Where to look:** runtime/parts/stack.c (the handler; the two-witness comment) · docs/panel/104
    **Why it matters:** a guard that fires on the wrong fault turns a memory bug into a false diagnosis, which is worse than a bare crash
