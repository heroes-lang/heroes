---
kind: decision
area: none
milestone: none
filed: 2026-10-03
commit: 33b72e65a0b5f02858639054a477336792fc3f10
github: none
---

# Panel 188's Windows facts measured after its ratification; defects 234 and 235 filed, one decision put to the author

2026-10-03, written at 22:48 by the clock (`date`). The author powered the
Windows box on (*"Windows on"*), and the coordinator sent three things to it
at once, each in its own folder: its own suite leg on the pushed trunk
`9818ef88` (`<scratchpad>/platforms/win-9818ef88.log`), the compiler-engineer's
155 cases with the trunk's compiler and stage E (`docs/panel/188-reports/compiler-engineer.md`
§ 22), and the ffi-pragmatist's survey, backstop scan and sweep
(`docs/panel/188-reports/ffi-pragmatist.md` § Windows, measured). What they
found is in the sitting's file, § After the ratification.

**Taken in batch 8's FFI lane, by the coordinator's message of 22:44**: defect
224 widened to lld-link's *could not open 'X.lib'*; R7 (b)'s refusal of a
leading `-` extended to a `link` string, provisional on the author's reading,
because `link "-out:pwn188"` builds at exit 0 on Windows and writes a file of
the author's naming; R3's message made true on both platforms; and defect
233, `machine_locked_path`'s library route naming `LIBRARY_PATH` that
lld-link ignores, filed in that lane's list.

**Filed here**: 234 (`blocking`), names NTFS stores as another file, which
the include opens at exit 0 on Windows where the Mac and Linux say missing;
235 (`adjacent`), names no NTFS file can hold. Both are read from the
ffi-pragmatist's tables and not yet run by the coordinator, and both say so.
Their repair widens what `check` refuses, so it is the author's:
`docs/work/DECIDE.md`, `panel 188`, recommending the extension in the FFI lane
over a new sitting.

**Corrected**: the compiler-engineer's § 22.2 read two cases' *missing
header* on Windows as clang reading them differently; its own § 22.1 shows
NTFS cannot store their names, and a dated line beneath says so.

**A slip recorded**: the ffi-pragmatist wrote
`/tmp/claude-ffi188-strings.txt` outside the scratchpad; it is left in place,
since removing it is a destructive command and the author's.
