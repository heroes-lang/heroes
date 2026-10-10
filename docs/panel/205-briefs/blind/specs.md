# Panel 205's blind seat: the spec each folder read

Each of the eight folders held `brief.md` (this directory's) and `spec.md`: `m1-*` the trunk's spec at `46c975c4` with panels 203 and 204's ratified sentences (the spec-warden's `drafts/base1.md`), `m2-*` the same with H2f. The H2f line, as `diff` prints it against `m1-*`'s:

```
351,352c351,352
< reads a module's headers in the order its groups are written, so one that needs
< another's names comes after it. clang
---
> reads a module's headers in the order its groups are written: one that needs
> another's names comes after it, and one defining `_GNU_SOURCE` first. clang
```
