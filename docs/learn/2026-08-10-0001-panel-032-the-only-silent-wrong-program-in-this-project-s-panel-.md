- [ ] **panel 032** | **The only silent wrong program in this project's panel record.** Under "a directory is a module", `use syntax` means one thing while `syntax.hero` is absent and another once it exists — so *adding a file* rebinds every call in every importer, and both versions type-check. Task: say why the ergonomist could build it from the wording alone, and which earlier panel's falsified form the wording repeats

    **Where to look:** docs/panel/032 § C is dead · DESIGN-LOG 2026-08-10 (panel 023)
    **Why it matters:** a rule in a subordinate clause is the highest-risk text in the document, and this is the second time
