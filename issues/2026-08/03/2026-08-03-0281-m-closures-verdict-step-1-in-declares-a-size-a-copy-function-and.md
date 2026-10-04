- [ ] **M-closures-verdict step 1** | `hero_desc_func` in `runtime/parts/desc.c` declares a size, a copy function and a drop function. Read those three fields and answer: how many bytes does a function value occupy, what does its copy function do, and what does its drop function do. Then say what each of the three would have to become if a function value could carry two captured `i64`s

    **Where to look:** runtime/parts/desc.c § hero_desc_func · runtime/heroes_runtime.h § HeroFn
    **Why it matters:** the descriptor is the one place the runtime states what a value IS, and a panel refused a feature because that statement and the feature disagreed by 24 bytes
