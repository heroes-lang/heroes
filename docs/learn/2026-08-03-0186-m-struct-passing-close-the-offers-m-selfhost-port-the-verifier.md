- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the verifier | **The copy-out check asks WHICH, not HOW MANY.** The sneaky test copies s0 out twice and s1 never — two writes for two parameters. Say why counting passes it and identity catches it, and find the other check in this codebase that made the same mistake

    **Where to look:** ir_verify.hero::check_copy_out, sweep 001 audit S8
    **Why it matters:** a count is a proxy, and a proxy that agrees with the real check on every easy case is how defects survive review
