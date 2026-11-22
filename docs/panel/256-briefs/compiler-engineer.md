# Panel 256, compiler-engineer

Read `00-shared.md` first, whole, then `docs/panel/256-reports/critic-briefs.md` (the critic's first
reading; its findings 4 to 9, 14, 16 and 17 bear on you). Your copy is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p256-scratch/compiler-engineer/`, a detached
worktree of the frozen commit; build its compiler from the seed and stage 2 there (§ 2 of the shared
brief). Your report: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p256/docs/panel/256-reports/compiler-engineer.md`,
written as you go. **The adopted route must build**: a route nobody built is not adopted
(`.claude/skills/panel/SKILL.md` § 3c), so build what you argue for, in your copy, and run the cases.

## Read

- design.md §1.1, §1.7, Part 5 (the core/sugar table), §4.12 (`docs/design.md:1982-2040`), Part 6's traits
  row (`:3639`), each by grep;
- panel 155's sitting and its compiler-engineer's report (`docs/panel/155-*.md`,
  `docs/panel/155-reports/`), panel 160's (`docs/panel/160-*.md`), measurement 034
  (`docs/measurements/034-the-generic-body-that-reads-the-outer-level.md`), panel 254's R1 to R6;
- `selfhost/check/thread_ops.hero`, `crossing.hero`, `handed.hero`, `walk.hero` near line 1790 (the
  instantiations), `function_value.hero:211`, `waiting_names.hero:126`, `selfhost/ir/mono.hero`,
  `selfhost/emit/crossing.hero`, `selfhost/emit/threads.hero`, `selfhost/inventory.hero`
  (`operation_of`), `runtime/parts/mail.c`;
- `256-briefs/probes/q2/` (run `run.sh` with your compiler from a copy of that folder first, and say
  whether you read what `coordinator-run.txt` reads).

## Q2, first: what rule makes spec § 11 true for every instance

1. **The class.** `grep -rln '\.generic' selfhost/check/` prints 51 files (2026-11-22). Find every rule that
   walks what a type holds and answers nothing for a letter, and which of them a program reaches through a
   generic: the crossing rules of § 4.2 (the send's `@`, a handle into `spawn`, a `ptr` or `cstr` crossing,
   a unit mailbox, a queue holding itself) and the others (`e2`'s `partial` map key). Say which the
   resolution must close and which it may file.
2. **The written-type hole** (`p5c`): `check/crossing.hero` judges only the types written in the source.
   Where must a thread or mailbox type be judged so that one inferred from a generic's result, and never
   written, is judged too? Build it.
3. **Build `gc` and `gi`** (the call-site routes, `arms/spec-gc.diff`, `spec-gi.diff`): measurement 034's
   first shape, an obligation per declaration and letter that the body incurs (sent by copy, sent with
   `@`, handed to `spawn`, a thread's answer, a mailbox or thread type naming the letter), carried through a
   generic calling a generic, checked at each recorded instantiation against the types it gives the letters,
   told at that call with the body's line named. Its lines, by file; every probe; the six programs of
   `blind/task-t2.md` (`blind/t2-programs/`, with `db.hero` and the frozen compiler's run) and what each arm
   answers; panel 155's veto condition (a give-up with no guard behind it) asked of your build.
4. **What `gc`'s `@` means for a letter whose instance holds no handle**: what the emitter writes, what a read
   of `v` after the send does in each instance, and whether the lives' walk (`check/moved.hero`) can decide it
   in the body once. Measured on your build, not argued.
5. **Build `gr`** (refused in the body, panel 155's shape) and count what it refuses: the four functions of
   § 4.3, and over the 852 generic functions in the tree (`git ls-files '*.hero' | xargs grep -c -E
   '^function [a-z_0-9]+<'`, summed) any other your build finds.
6. **After `mono`** (034's second shape) and **a written bound**: priced or refused, with the reason.
7. **The cost of each built route** on `heroes check selfhost/main.hero` **and on a program that
   instantiates many generics** *(critic 17: `selfhost` declares 1 of the tree's 852, `packages` 555)*,
   such as `heroes check examples-rebel/certs/main.hero` or another of Rebel's you name: `/usr/bin/time
   -p`, the best of N stated runs, the first discarded, a noise floor measured first, and every run whose
   `real` stands above `user + sys` discarded with the reason (the machine is shared; CLAUDE.md §
   Verification).
7b. **Panel 155's obstacle** *(critic 9)*: the instantiations table names no callee and `check/` has no
   node-to-declaration map, 150 of 155's 284 lines. Say how your build answers which body's obligations
   apply at a call, and its lines.
7c. **The emitter's crossing walk after `mono`** *(critic 14)*: it already knows an instance holds a `ptr`
   (`selfhost/emit/crossing.hero:60,77`). Price a refusal there, or a run-time guard, beside the call-site
   route, and say which closes `p7` and `p8` with the least that can be wrong.
7d. **Is `gi` what § 11 already says** (`00-shared.md` § 4.6, *critic 16*)? Answer it from your build.

## Q1, the language's half

8. **`m.number()`** (`arms/spec-q1.diff`, P1 item 1): what it costs through `inventory.hero`, the operation
   dispatch of `thread_ops.hero`, `operation_names`, the emitter and `runtime/parts/mail.c` (a table of
   queues by slot and life, the number given, `hero_mail_ready(number)` and a watch the layer registers,
   whose ring is called outside every lock of the runtime's). Build the runtime half far enough that a C test
   shows a send ringing a registered watch, and say whether the boundary row of spec § 11 still holds as
   written. **Is a number the right door**, against the alternatives of § 3.3 (a mailbox handed to C under a
   mark, the pump, a language-level wait)? Say which you would build and why, by what you measured.
9. **The generic interplay**: `poll.add_mailbox<T>(set: poll.Set, m: (mailbox T)) -> i64?` must be writable
   in Rebel under the Q2 rule you recommend; check it is, in your build.

End with the verdict structure of your agent file, then one line per question: the route you would adopt,
and the one measurement that would change your mind.
