- [ ] **panel 031** | **The proposal's own example taught an illegal program, for the second time in this record.** `geom.dist2(a, b)` cannot be a qualified call — `dist2(a: Point, b: Point)` has two same-typed parameters, so labels are mandatory — and it *is* legal as UFCS, meaning `dist2(geom, a, b)`. Task: say why the judge that found it wrote the correct form in its own program while predicting ≥30% of readers would copy the wrong one, and what that says about where examples sit relative to rules

    **Where to look:** docs/panel/031 § The finding that decided the wording · spec line 81
    **Why it matters:** panel 023 was found the same way, and the method is writing from the document rather than reading it
