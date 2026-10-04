- [ ] **M-module-namespace defects** | **Two namespaces wearing one name, and the emitter was reading the wrong one.** `FileEntry.module` is what the author types before the dot; `FileEntry.component` is what reaches the linker. Until panel 032 the emitter took the first. Task: say why `module_of` had *always* sanitised and the bug still existed, and why panel 031 R10's diagnostic — written for this exact pair of names — did not fire

    **Where to look:** archive/bootstrap-rs/heroes/src/source/files.rs · archive/bootstrap-rs/heroes/src/modules/tests.rs (`fixedbugs_two_modules_whose_concatenations_collide…`)
    **Why it matters:** the check was correct and was comparing the wrong two strings
