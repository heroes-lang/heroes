- [ ] **M-ffi-ladder** | **`_ = f(x)` emitted `-Wunused-but-set-variable` on every program that used it**, which is the ordinary way to ignore a C status code. The fix skips the temporary for a discarded **call** and for nothing else. Task: say what breaks if the same rule is applied to `_ = xs[9]` — the answer is why the restriction is not tidiness

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/inst.rs · emit/decls.rs (`discarded_call`)
    **Why it matters:** a pure op may vanish and an aborting one may not, and the difference is the whole of §4.3's abort rule
