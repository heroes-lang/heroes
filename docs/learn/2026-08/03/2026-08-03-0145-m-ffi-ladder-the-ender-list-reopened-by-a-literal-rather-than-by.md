- [ ] **M-ffi-ladder** | **The ender list, reopened by a literal rather than by a rule.** `nullptr` was added to the language and missed in `is_line_ender`, so `p: ptr @ nullptr` planted no terminator and the *next* line was swallowed as a continuation — the error landed on an innocent statement, which is panel 007's own trap. Task: say why a value-producing keyword is the class that keeps being forgotten, and what `every_value_keyword_ends_a_line` can and cannot catch (it fires only for a spelling somebody remembered to add to *it*)

    **Where to look:** archive/bootstrap-rs/heroes/src/lexer/layout.rs · lexer/tests/adversarial.rs
    **Why it matters:** a list whose completeness is a premise, and the third time this project has paid for one
