- [ ] **panel 031** | **Two ways to break §4.19, both of them optimisations this emitter already performs.** Deduplicating extern prototypes by C name: exit 0, calling through a wrong signature, printing garbage bytes. Pruning an unused module's `#include`: exit 0 printing `6714990092` where the unpruned form is two clang errors. Question: why is one whole-program `.c` enough for §4.19 today, and which of M-separate-compilation's four acceptance rows is the one this measures early?

    **Where to look:** docs/panel/031 R9 · docs/panel/030 R2
    **Why it matters:** the guarantee is the TU's contents, and both repairs are refusals to be clever
