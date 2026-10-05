- [ ] **M-package-layout step 2** | The compiler refuses two modules whose LAST parts match, anywhere in one program, and separately refuses two whose C COMPONENTS match. Say which of these three pairs trips which check, and which trips neither: (a) `syntax/decl` + `ir/decl`, (b) `print_inst` + `printinst`, (c) `syntax/decl` + `syntax/expr`

    **Where to look:** selfhost/module/paths.hero (`last_parts_collide`) · selfhost/modules.hero (`collisions`)
    **Why it matters:** one check protects the program's names, the other protects the linker, and each is blind to what the other sees
