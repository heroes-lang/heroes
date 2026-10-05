- [ ] **M-isolated-threads step 3** | `cow.c:32` is `if (a->refcount == 1) return;` and step 3 did NOT repair it, on purpose. Two threads reach that line holding the same array. Walk what each one does and say what the program ends up printing — then say why making the READ atomic does not help

    **Where to look:** runtime/parts/cow.c, the comment above the test · docs/panel/111 § The runtime corrupts memory
    **Why it matters:** it is the difference between a race an atomic closes and one that needs a protocol, and the second kind is what is left of this milestone
