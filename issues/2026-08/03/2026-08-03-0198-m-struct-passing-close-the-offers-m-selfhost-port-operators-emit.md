- [ ] **M-struct-passing close — the offers** | M-selfhost-port, operators (emit_operator.hero) | **% is guarded like / but reports differently — why was "integer overflow" for % a FALSE message?** And what does INT64_MIN % -1 do on arm64 versus x86 with no guard?

    **Where to look:** emit_operator.hero's div/rem arm, panel 035
    **Why it matters:** same guard, different truth: the remainder is 0 and overflows nothing — the quotient C computes on the way is what has no int64
