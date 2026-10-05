- [ ] **M-package-layout** | mutation drill offer: `heroes mutate` over the module-resolution path, now that a `use` line carries two spans (the path and the binding) rather than one

    **Where to look:** selfhost/parse/use_line.hero · selfhost/resolve/top.hero
    **Why it matters:** a form the mutator cannot re-print is a form it silently declines to mutate
