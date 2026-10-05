- [ ] **panel 036** | Say what `_Generic((sqlite3_open((const char*)0,(void*)0)), int:1, default:0)` costs at runtime, and why the call inside it does not happen. The answer is one clause of C11 6.5.1.1p3 — find it before reading the panel

    **Where to look:** docs/panel/036 · C11 6.5.1.1p3
    **Why it matters:** it is the whole reason the mechanism is free, and a reader who assumes the call runs will refuse the design for the wrong reason
