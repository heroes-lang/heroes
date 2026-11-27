/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/rebind.c — every handler a program's C installs entered through this
 * runtime's trampoline, and a jump out of a handler told from a jump within
 * it, on Darwin (route D-rebind, panel 249's ffi-pragmatist, repaired for
 * `--sanitize` by panel 251; panel 255's R1 for the jump and R3's rules for a
 * context switched inside a handler; defects 2522, 2679 and 2702; design.md
 * §1.12, §4.19). The route is split by concern over seven files, included in
 * this order by `runtime.c`:
 *
 *   rebind.c          this doc, the includes
 *   rebind_entries.c  the handlers a thread is inside (rules 4 to 7)
 *   rebind_install.c  the C library's own, and the installs
 *   rebind_record.c   route A, the records
 *   rebind_decode.c   the key and the field, verified before `main` (rules 1
 *                     to 3), and the accessor
 *   rebind_jump.c     the jumps, the swaps and the save points
 *   rebind_image.c    the rewrite of every image's slots, and `main`'s start
 *
 * WHY NOT `parts/siginstall.c`'S WAY. Darwin's two-level namespace binds a
 * library's call of `sigaction` to libSystem's own name, so a definition of
 * the runtime's reaches the program's own objects and no dylib: panel 249
 * measured every Darwin dylib missed, 9 of 9.
 *
 * HOW. Before `main`, and for every image dyld loads later (its add-image
 * callback), the EXECUTABLE rewrites, in every loaded image outside the shared
 * cache, the pointers its stubs load for `sigaction` and `signal`, the jumps
 * of the setjmp and context families and `makecontext`, and (only where a
 * decode below did not verify) the save points, to functions of these files.
 * An install reaches `hero_rebind_sigaction`: the C library's own install,
 * then the same action handed to libsystem_kernel's `__sigaction` with this
 * route's trampoline, which keeps the entries this thread is inside. The
 * thunk asks whether any stands (`hero_rebind_inside`, `parts/sigguard.c`).
 *
 * THE TRAMPOLINE IS libSystem's `_sigtramp`'S CONTRACT (panel 249's
 * ffi-pragmatist, read in `_sigtramp`'s disassembly and run): the kernel calls
 * it with the catcher, the style, the signal, its information, the context and
 * a token; it calls the catcher with three arguments whatever the style, and
 * returns by `__sigreturn(context, 30, token)` under the install flag 0x400.
 *
 * THE RULES (panel 255):
 * 1. THE DECODE. A jump of the setjmp family reads ONE word of its target,
 *    word 12, `pacdb(sp, 0xcbed) ^ k`, and decodes the stack pointer by the
 *    process's key `k`; a context jump reads the context's own stack-pointer
 *    field, `pacda(sp, 0xcbed)` at byte 0x148. The target lives: the C
 *    library reads the same word next. Nothing a save filled is read at any
 *    other moment (defect 2679).
 * 2. THE CALIBRATION, before `main`: every callee-saved register filled with
 *    a pattern, then each save the platform has (`setjmp`, `_setjmp`,
 *    `sigsetjmp` with and without the mask, `getcontext`, the save half of
 *    `swapcontext`) made at a known stack pointer, and every word it writes
 *    checked: the unmangled registers, the frame pointer and the return
 *    address under the same key, and the stack pointer; at a frame, 1 MiB
 *    deeper, and on a second, `mmap`ed stack. The two families are verified
 *    apart, each falling to route A alone.
 * 3. WHERE A FAMILY DOES NOT VERIFY, ROUTE A for it: its save points are
 *    rewritten and keep a record (panel 251's route J), whose fingerprint is
 *    read only of a buffer on this thread's stack, the alternate stack it runs
 *    on, or a writable segment of an image loaded before `main`. Rules 4 to 7
 *    bind it too: a trusted record holds a frame just below the stack pointer
 *    its save wrote, judged as a decoded target is, and where contexts fall
 *    to it every range `makecontext` hands out is held (defect 2745).
 *    `hero_signal_jump_route()` says which path stands (bit 1 the setjmp
 *    family decodes, bit 2 contexts; the same accessor and bits as Linux's),
 *    and `HERO_JMP_FORCE_UNCALIBRATED` (1 the setjmp family, 2 contexts, 3
 *    both) forces the failure.
 * 4. STACKS. A jump's target lies on a coroutine's stack where a range
 *    `makecontext` handed out holds it, else on the thread's own stack, else
 *    on the alternate stack registered now, else on a stack of unknown extent.
 *    An entry is left by a target on the same range that lies above it; an
 *    entry records the alternate stack it was pushed on. No two addresses on
 *    two different ranges are ever compared.
 * 5. A SWAP MADE WHILE A HANDLER STANDS SUSPENDS IT: every standing entry
 *    counts the swap until it returns in its own frame, and nothing drops a
 *    suspended entry but its own handler's end or a landing that resumes it.
 *    Neither the trampoline's push nor the thunk prunes by its own frame (a
 *    stack switched where this route cannot see it would pass for the same
 *    stack); a trampoline's end drops its own entry and the entries pushed
 *    after it that nothing suspends.
 * 6. A `setcontext` WHOSE TARGET LIES ON A COROUTINE'S STACK holds every
 *    standing entry until its handler ends (the coroutine may resume the
 *    handler through `uc_link`); any other `setcontext` is a jump. A jump that
 *    leaves the alternate stack an entry was pushed on, for any other stack,
 *    leaves that entry. A jump that lands on the alternate stack from outside
 *    it re-enters a handler's frames, and stands as an entry of its own. A
 *    landing at or below a standing entry on its range is that handler
 *    running again: it releases the entry's suspension and hold.
 * 7. THE KNOWN STACKS are learned in `makecontext`'s slot, and nothing tells
 *    this route one has ended: a stale range is never asked about an entry,
 *    only about a target, where it costs a told refusal.
 * A JUMP OR AN ENTRY THIS ROUTE CANNOT JUDGE KEEPS EVERY ENTRY: a miss is a
 * told refusal, never Heroes code inside a signal.
 *
 * THE `--sanitize` REPAIR (panel 251's ffi-pragmatist): an image holding an
 * `__interpose` section is left unrewritten, and a re-install asks the kernel
 * raw (`__sigaction`), never `sigaction`.
 *
 * EVERY POINTER TO THE C LIBRARY'S OWN IS TAKEN BEFORE `main`; nothing below
 * locks, allocates or calls `dlsym` at a jump, so each rewritten slot is
 * async-signal-safe, in a handler and in a forked child alike.
 *
 * WHAT IT DOES NOT REACH, said (defect 2522's rows): an installer or a jump
 * reached inside the shared cache; one of the names held by an image's static
 * initializer, a bind of the image's data that no stub's slot holds, so it
 * keeps the C library's own (`makecontext` so held on this Mac: a coroutine's
 * stack unseen, 7 printed as 11, defect d-lane-dlsym-make-1); a raw
 * `__sigaction` naming a trampoline of
 * its own; an interposer's own installs; a C++ exception out of a handler; a
 * save made where these files do not look that a later jump then resumes
 * after its handler was left on a stack other than the alternate one; a
 * coroutine left by a switch these files never see that saves into another
 * context than the one it was switched to, leaving that one as it was, which
 * is judged ended (`rebind_entries.c`, the life of a range; defects
 * d-lane-d2745-1 and 2851); a
 * handler left by a jump and then landed in again from a coroutine on another
 * range (no range's bottom is recorded where the thread leaves it, so the
 * landing in the handler's dead frames reads as a coroutine resuming the
 * thread's own stack: panel 255's R3 residue `rr_2`, shared with the route
 * before it). */

#if defined(__APPLE__)

#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <mach-o/getsect.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <setjmp.h>

/* libsystem_kernel's own, which no SDK header declares. */
extern int __sigaction(int, struct __sigaction *, struct sigaction *);
extern int __sigreturn(void *, int, uintptr_t);

#define HERO_REBIND_TOKEN_CHECKED 0x400
#define HERO_REBIND_RETURN_FLAVOR 30

#endif
