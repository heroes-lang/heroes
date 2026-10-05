- [ ] **M-struct-passing close — the offers** | M-selfhost-port, monomorphisation | **The queue substitutes the callee's arguments through the CALLER's bindings.** Trace outer<i64> -> inner(x): what does the template record at that call site, and what does the copy's call_instances hold? (Panel 084's defect is the wrong answer.)

    **Where to look:** ir_mono.hero::run and substituted_calls
    **Why it matters:** a span that exists once per copy cannot key a program-wide table — the answer must live on the copy
