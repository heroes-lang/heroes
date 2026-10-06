---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **392 — a group constant bound to a record its header's brace list was not written for builds at exit 0** | `docs/panel/178-reports/completeness-critic-work/w/c094/wrongtype.hero` binds Darwin's `PTHREAD_COND_INITIALIZER` as a `Mutex`: the trunk's compiler stopped it at exit 2 only by the internal error every brace-list constant hit, and since defect 094's repair (`41c5d1b4`) the round emits it at exit 0 and it runs (batch 12's census, its attribution written at 14:39:25 on 2026-10-06; lane b12-ffi13 measured the same program printing 1018212795) | `selfhost/emit/record_constant.hero:39`, which records the limit · panel 178's resolution 3, sat again as panel 194 · **class: systemic**

    **Origin:** batch 12's census, its attribution of 2026-10-06 (finding 2), and lane b12-ffi13's report on defect 094 (*a brace list has no type*); filed by the coordinator at 14:42.

    **Class: systemic**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a wrong binding accepted, which C cannot refuse because a brace list carries no type, so its repair needs a ruling no rule reaches: what the language promises about a header's initialiser bound as a record's constant. Panel 194 rules it (its brief carries the question); the item closes on that ruling and its landing.
