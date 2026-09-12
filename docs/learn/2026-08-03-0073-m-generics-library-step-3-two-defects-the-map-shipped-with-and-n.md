- [ ] **M-generics-library step 3** | **Two defects the map shipped with, and neither golden could see them.** `n = m` then `m["b"] @ 2` printed `2 2 2` where the spec requires `1 2 -1` — exit 0, ASan clean, leak counter zero. And `m["a"] @ "x" + "y"` leaked one block. Task: say why `run/maps.hero` passed through both, then say what one line in that file would have to change to catch each

    **Where to look:** tests/golden/run/fixedbugs-map-store-aliased.hero · fixedbugs-map-store-leaked.hero · runtime/runtime.c (hero_map_set)
    **Why it matters:** the cheaper spelling of a container test — int values, never aliased — passes while testing neither rule
