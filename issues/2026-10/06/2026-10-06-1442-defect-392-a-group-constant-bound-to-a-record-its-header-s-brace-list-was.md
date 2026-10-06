---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: self
github: none
---

- [x] **392 — a group constant bound to a record its header's brace list was not written for builds at exit 0** | `docs/panel/178-reports/completeness-critic-work/w/c094/wrongtype.hero` binds Darwin's `PTHREAD_COND_INITIALIZER` as a `Mutex`: the trunk's compiler stopped it at exit 2 only by the internal error every brace-list constant hit, and since defect 094's repair (`41c5d1b4`) the round emits it at exit 0 and it runs (batch 12's census, its attribution written at 14:39:25 on 2026-10-06; lane b12-ffi13 measured the same program printing 1018212795) | `selfhost/emit/record_constant.hero:39`, which records the limit · panel 178's resolution 3, sat again as panel 194 · **class: systemic**

    **Origin:** batch 12's census, its attribution of 2026-10-06 (finding 2), and lane b12-ffi13's report on defect 094 (*a brace list has no type*); filed by the coordinator at 14:42.

    **Class: systemic**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a wrong binding accepted, which C cannot refuse because a brace list carries no type, so its repair needs a ruling no rule reaches: what the language promises about a header's initialiser bound as a record's constant. Panel 194 rules it (its brief carries the question); the item closes on that ruling and its landing.

## The ruling

Ruled by panel 194, 2026-10-06, R3 (`docs/panel/194-the-rest-is-zero-where-a-group-s-construction-says-so-and-a-lend-s-count-is-in-c-s-own-unit.md`), ratified by delegation the same day: C cannot tell which struct a brace list was written for, one brace-list shape fitting several of a libc's types (the historian's reading of Apple's, glibc's and musl's initialisers), so a group constant bound to the wrong record is the binding author's mistake, as any extern value's meaning is, and not one the compiler can refuse; the enumeration and pointer-sign errors defect 094's repair turns on catch some such values. The limit stands in `selfhost/emit/record_constant.hero:39` and here, not in the specification, the spec-warden's reading. Not a defect of the compiler; closed by the ruling.
