- [ ] **panel 027** | **A mandate the compiler never implemented, found by pricing a sentence.** design.md:809 says "Slicing that lands mid-sequence is an error"; `slice("caffè", from: 0, to: 5)` exits **0** with a corrupt byte. Task: say why the error belongs at the slice rather than at `chars`, and what law `chars` can assert over `heroes mutate`'s corpus once it is total

    **Where to look:** design.md:809 · docs/panel/027 R4 · runtime/runtime.c (hero_str_slice)
    **Why it matters:** the ergonomist found the symptom, the warden found the mandate, and neither had the other's input
