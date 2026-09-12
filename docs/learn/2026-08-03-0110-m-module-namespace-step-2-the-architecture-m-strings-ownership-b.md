- [ ] **M-module-namespace step 2** | **The architecture M-strings-ownership built for one special case turned out to be the general one.** `Source` went from "one text plus a library boundary" to "N files plus a table", and the panel's number is why: 39 `Span {` construction sites and 271 `&Source` parameters, of which concatenation changes **zero**. Task: say what a file id inside `Span` would have cost at each of those two counts, and why `Span::to` is the specific function that makes it worse than arithmetic

    **Where to look:** archive/bootstrap-rs/heroes/src/source/mod.rs · docs/panel/031 R7
    **Why it matters:** the cheap change and the correct change were the same one, and it was measured rather than argued
