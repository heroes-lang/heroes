# A dead handle is poisoned where it lay and remembered where it was

2026-09-24 | a call that ends a handle's life overwrites the place it read the
handle from with a dead value on an unmapped page, when the set's count reaches
zero, and every crossing of a handle into C is checked, callback results
included; the runtime remembers the dead addresses and clears one when C hands
it back, through a thunk at the address given to C; a read of a binding dead on
every path is an error inside its function; R and S are refused; the success
clause is on the result and governs every end, transfer and reference; a
transfer needs a receiver | R refused six of eight correct real-library programs
and missed 088, S breaks the C ABI at callbacks and structs, P alone misses
every copy and hides what ASan catches today at a callback result | design.md
§1.11, §1.12, §4.19, Part 8 wart 20 | **panel 177**, full, provisional

## What it settles, and what it leaves

It settles the repair of defects 077 and 088 for the binding and for a copy whose
address was not reused, and says what stays a limit: a copy that reaches C after
C handed its address out again, outside `--sanitize`, and a handle C keeps inside
its own struct. It corrects design.md wart 20's claim that `--sanitize` is the
remedy, which is false for a use inside an uninstrumented library. It leaves five
things to be measured before the landing lands, each named in the synthesis's
item 10.
