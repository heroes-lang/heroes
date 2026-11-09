# Panel 239, the proposal

Convened 2026-11-08 by the coordinator of batch 26, on the author's request that defects 2363 and
2364 be heard in one sitting; under M-package-manager, whose panel gate was asked once at panel 235.
Panel 238's R3 (ratified the same day, decision 2362) lands its R1 and R2 only in the round that
repairs these two. **Full panel**: a new refusal is a diagnostic class.

1. **Defect 2363** (`blocking`, https://github.com/heroes-lang/heroes/issues/2363): one C name two
   linked libraries define binds whichever stands first on the link line, unrefused; with another
   signature the program builds at exit 0 and dies by a memory fault. Panel 238's provisional
   direction: **refuse a name a group binds that the link took from another library than its
   group's**, read from the link itself.
2. **Defect 2364** (`blocking`, https://github.com/heroes-lang/heroes/issues/2364): two other
   modules' records read in one module's unit follow the walk's order, and a correct program is
   refused with a false message whose fix leads nowhere. Panel 238's provisional direction: **the
   compiler decides the order of the other modules' headers a unit reads** (panel 204's order search),
   with a true refusal where no order works.

The questions are `00-shared.md` § The questions; the shapes measured beside each are there too.
