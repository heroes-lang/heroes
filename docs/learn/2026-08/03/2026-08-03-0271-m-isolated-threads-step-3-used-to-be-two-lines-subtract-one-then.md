- [ ] **M-isolated-threads step 3** | `hero_str_decref` used to be two lines — subtract one, then ask whether the answer is zero — and is now one call whose answer is the count BEFORE the subtraction, tested against 1. Two threads, both dropping the last two references. Say what the OLD pair could print that the new one cannot, and why counting the block twice is worse than not freeing it

    **Where to look:** runtime/parts/str.c `hero_str_incref`/`hero_str_decref` · runtime/heroes_runtime.h § the reference count
    **Why it matters:** it is the whole reason an atomic read-modify-write exists, in the smallest program that shows it
