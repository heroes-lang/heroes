- [ ] **M-struct-passing close — the offers** | M-separate-compilation step 5 (one gate, three doors) | `bind.system(x)` is now refused — and so are `_ = bind.system` (no call!) and `"p".cstr().hero_file_read(@st)`. Why must the VALUE form be refused if nothing is called on that line? What would C need at the point where the address is taken?

    **Where to look:** selfhost/resolve/qualified.hero (the function_decl arm) · tests/golden/surface-fixtures/externroute/wrong.hero
    **Why it matters:** a rule that gates only the spelling it was named after is a sieve — "callable" undercounts how a name crosses a boundary
