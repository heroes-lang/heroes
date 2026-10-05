- [ ] **M-generics-library step 3** | The aliasing defect was found by **rewriting a stale comment**, not by a test: the header still said "READ-ONLY at M-optional-map … there is no `hero_map_unshare` here", which was true one milestone after it stopped being safe. Question: which instrument in this project *should* have caught it, and why did none — given that panel 022 had already measured this exact failure for arrays

    **Where to look:** docs/panel/022 R2 · runtime/heroes_runtime.h (the map paragraph)
    **Why it matters:** a comment that describes the past is a claim nobody re-checks
