---
kind: decision
area: none
milestone: none
filed: 2026-08-04
commit: ecc0d707cfb8e2e46ecf94fc5d9e287a7d5b24e3
github: none
---

2026-08-04 | Two `@` arguments of one call may not share a root binding — compile error at M3c with a certain fix (pass a copy); over-rejects distinct-index element pairs by design, relaxable later; buys the emitter a non-aliasing invariant (direct-pointer lowering and `restrict` stay legal); 0 spec tokens | panel 010, adopted provisionally (author ratification queued): left legal the call's meaning depends on copy-out order, which nothing specifies (16 / 15 / 6 under the three readings) and no sourced language specifies; Ada arrived at the same rule in 2012 after 33 years of arbitrary order, Swift SE-0176 and Hylo do the same on the identical copy-in/copy-out model; E is the precondition that makes §4.10's "no aliasing exists anywhere" true | §4.8, §4.10 | 010

## Prior art

Panel 010 (`docs/panel/010-repeated-mutable-arguments.md`): each line says what the cited page itself states, the sitting's own file says what was taken from it, and every link was re-read 2026-10-11.

- **Reached first by Ada**: [Ada 2012 Reference Manual, 6.4.1 Parameter Associations (legality rule 6.15/3 to 6.16/3)](https://www.adaic.org/resources/add_content/standards/12rm/html/RM-6-4-1.html): two parameters of mode in out or out of an elementary type may not be known to denote the same object (legality rules 6.15/3 and 6.16/3)
- **Reached first by Swift**: [SE-0176 Enforce Exclusive Access to Memory](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0176-enforce-exclusive-access-to-memory.md): passing the same variable twice as inout is two overlapping write accesses to it, which conflict
