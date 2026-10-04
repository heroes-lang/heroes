- [ ] **M-module-namespace defects** | **The deletion panel 030 promised and panel 031 withdrew happened anyway, for a different reason.** `writer::LIBRARY_FILE` and `Writer.source` became dead when the two `#line` entry points collapsed into one. Question: what does that say about *why* the library stopped being a special case — the file table, or the rule?

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/writer.rs · docs/panel/031 § The disagreements
    **Why it matters:** a prediction can be right about the outcome and wrong about the mechanism
