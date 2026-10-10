# Panel 206, compiler-engineer's brief

Read `00-shared.md` first. Build in your copy a refusal of an arithmetic
expression the checker can compute from literals and constants that does not
fit its position, and run it on `p206/` and over every tracked `.hero` file
(which file moves, at which exit code, and whether any program the round
accepts and runs correctly would be refused): where the checker already
evaluates literals (`int_out_of_range`, a constant's body, defect 564's
operand typing in `selfhost/check/`), what it costs in lines (`layout`'s
unit) and in instructions on `check selfhost/main.hero`, and where the
evaluation must stop (a constant read through another constant, a call, a
generic). A route you recommend is built and run before you report.
