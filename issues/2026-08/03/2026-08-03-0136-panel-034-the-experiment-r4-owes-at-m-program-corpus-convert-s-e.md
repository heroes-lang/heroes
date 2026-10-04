- [ ] **panel 034** | **The experiment R4 owes, at M-program-corpus**: convert `examples/`'s error codes to `constant`s and re-run `heroes mutate`. The ergonomist predicts codes-mismatch ≤1/20 and false positives 0/20. Question before running it: what does `typo-code` even *mean* once there are no literals left at the comparison — does its row go to zero sites, and is that a pass or a hole in the instrument?

    **Where to look:** docs/measurements/004-error-codes.md · harness/mutations/operators.md
    **Why it matters:** an instrument that reports success because it lost its sites is the failure mode measurement 004 was written to avoid
