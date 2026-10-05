- [ ] **panel 028** | **Five of six off-by-one directions are silent**, and the sixth is loud only by accident — it survives because the loop bound is `len` of the thing being indexed, and the hedged form `range(0, len(s) - 1)` loses even that. Task: write the sixth case both ways and say which instrument in this project fires on each

    **Where to look:** docs/panel/028 R2
    **Why it matters:** the language's central claim, tested on the most common loop in programming
