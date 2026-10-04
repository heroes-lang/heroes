- [ ] **panel 018 impl** | `function f():` costs one diagnostic with a certain fix and the block still parses. Where is the colon eaten, and on which constructs does the same helper fire?

    **Where to look:** syntax/stmt.rs eat_python_colon · tests/golden/check/trailing-colon.hero
    **Why it matters:** the blind experiment's top pre-registered slip became machine-repairable
