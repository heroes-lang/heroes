# Panel 206's blind seat: the spec each folder read

`n1-*` and `w-*` read the spec-warden's base (the frozen spec at `e0aeb991` with panels 204 and 205's sentences, `206-spec-warden/drafts/base.md`); `n2-*` the same with F7e. The predict folders held `brief-predict.md` and `p1.hero` to `p6.hero` (this directory's), the write folders `brief-write.md` alone. F7e's line, as `diff` prints it against the base:

```
199c199
< Overflow aborts at every width. Integer division by zero aborts. `/` and `%` truncate
---
> Overflow aborts at every width. Integer division by zero aborts. An abort that literals and written constants alone compute is a compile error. `/` and `%` truncate
```
