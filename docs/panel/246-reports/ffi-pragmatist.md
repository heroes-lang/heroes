# Panel 246, ffi-pragmatist

Written 2026-11-12 from 04:54, as I went. Folder `.claude/worktrees/p246-scratch/246-ffi-pragmatist/`
(below: `F/`), the tree `F/tree/` a `git archive ef50a7478` (`git -C .claude/worktrees/b33-round
rev-parse ef50a7478 'ef50a7478^{tree}'` printed `ef50a7478c241f765c0b308cb55282f95d64053f` and
`084b36b7d5db6b342c281468fff120b2fe98349b`), `TMPDIR=F/tmp`.

## Setup

- Seed `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0, `real 19.67` against
  `user 13.33` + `sys 0.47`: a waiting run (`uptime` read load 42.75 at 04:57), not a duration.
- Stage 2 `./heroes build selfhost/main.hero -o heroes-next`, exit 0, `real 193.83` against `user
  130.04` + `sys 26.57`: waiting, not a duration. `./heroes-next --version`: `heroes 0.6.0`,
  `sources 796d1133542b1909`, **on the base**. `heroes-next doctor` names `runtime` (the tree's own).

## Where the briefs are wrong, first (each run below)

1. **Route A's layout, as 00-shared.md words it** (*"a file inside its own
   `heroes.packages/.fetching-<pid>-<n>/`"*), loses live runs or leaves dead ones: between the
   `mkdir` and the lock a recovering run either takes the folder (666 births lost in 8 s) or leaves
   a killed one for ever (4 in 8 s); and on Windows a folder holding an open file cannot be removed
   (`RemoveDirectoryW`: an empty directory only). The lock file belongs **beside** the folder, taken
   before it is made and dropped after it is removed (A-4: 0 and 0).
2. **00-shared.md B3's "numbered handle ... which R3 ... exists to refuse"**: the layer's handle IS
   a number, `(life << 32) | (index + 1)` (`hero_net.c:275-276`), and a stale number carried
   through an `i64` is refused `HERO_NET_NOT_OPEN`, never another socket (B3, measured).
3. **00-shared.md B2's route (a)**, *"no handler ... no `SA_RESTART` question"*: it still never
   reaches a server blocked in the layer's `accept` (measured), and a job started with `&` by a
   script has SIGINT ignored, so a blocked SIGINT is discarded, never pending (measured).
4. **00-shared.md's question *"Whether `shutdown` from `hero_net_close` wakes a thread blocked in
   `accept` on a listening socket"***: on Darwin, no, and the layer then hands out a connection
   made to the listener after it was closed (B2, measured).
5. **My brief's *"whether Darwin does [have `F_OFD_SETLK`] is a question"***: it does (SDK
   `sys/fcntl.h:310`, `man 2 fcntl`, and it ran).
6. **My brief, B2**: *"a header alone"* for one flag per program: one exists (a declaring header and
   a defining one bound by exactly one module), and its third mistake is told as `internal error`
   (B2).

## Two mistakes of mine, said

- My first lock driver killed the `timeout` wrapper instead of the probe, and to clear its
  leftovers I ran `pkill -9 -f '^sleep 20$'`, **a pattern that could have matched another
  session's `sleep 20`**; `pgrep` afterwards found none of mine. Every later child was my own probe,
  stopped by its path in my folder or by its exact pid.
- My first B2 driver read pids through `head -c`, which holds its input to the end: it sent signals
  to nothing, and one probe (handler armed, blocked in `accept`) outlived its `timeout 30` until
  `kill -9`, at `ELAPSED 03:10`. That run is discarded as a driver's; what it showed is kept in B2.

## Part A: defect 2453, the lock door (written as run)

### A-1. The three POSIX locks, asked the same questions (`F/a/lockprobe.c`, `F/a/a1.sh`)

`clang -std=c11 -Wall -Wextra -Werror` exit 0. Each case on APFS, then on a FAT16, a FAT32 and an
exFAT image (`hdiutil create -fs MS-DOS | 'MS-DOS FAT32' | ExFAT`, attached with `-nobrowse
-mountpoint` at `F/a/mnt-*`, `diskutil info` reading *MS-DOS FAT16*, *MS-DOS FAT32*, *ExFAT*). The
four logs are identical once the process numbers are taken out (`diff` after `sed`, *identical after
the pids* three times). The answers, `F/a/a1-apfs.log`:

| question | `flock` | `F_SETLK` | `F_OFD_SETLK` |
|---|---|---|---|
| holder `kill -9`'d (`wait` 137), a second process asks | taken | taken | taken |
| a second open of the file in the SAME process asks | **held** | **taken** | **held** |
| the holder opens and closes another descriptor of the file, another process asks | held | **taken (lost)** | held |
| a child forked and not exec'd, parent `kill -9`'d, child alive | **held** | taken | **held** |
| the same, child then killed | taken | taken | taken |
| child exec'd, descriptor `O_CLOEXEC`, parent killed | taken | taken | taken |
| child exec'd, descriptor inherited (no `O_CLOEXEC`), parent killed | **held** for the child's life | taken | **held** |

- **`F_SETLK` is wrong for route A on two counts, measured**: it is the process's, so a recovering
  run in the process that holds a folder's lock takes it again (`twice-second-same-process setlk ->
  taken`); `check` reaches `staging.begun` once per unplaced package (`module/unplaced.hero:46`), so
  one process can hold several run folders, and a recovery inside it would take its own live one.
  And closing ANY descriptor of the file drops it (Darwin's `fcntl(2)`, read today, line 269: *"all
  locks associated with a file for a given process are removed when any file descriptor for that
  file is closed by that process"*), which `closeother` measured.
- **Darwin has `F_OFD_SETLK`**: the SDK's `sys/fcntl.h:310` defines it (under `__DARWIN_C_FULL`) and
  `man 2 fcntl` documents it; `ofd` behaved as `flock` in every row. `flock` is the door's choice:
  the same answers, and one call on every POSIX the CI runs.
- **The window between `fork` and `exec`**: a child that has not exec'd shares the open file
  description and keeps an `flock` alive after its parent dies; `O_CLOEXEC` ends that at the `exec`.
  The runtime's child, forked at `runtime/parts/run.c:1807`, runs `hero_run_child` (`:1213-1357`,
  read): `setpgid`, its signals, `open` and `dup2` of its streams, then `execvp` (`:1340`) or
  `_exit(126|127)`; on Linux it asks `PR_SET_PDEATHSIG` and raises SIGKILL itself where its parent
  is already gone (`:1234-1235`). So a lock outlives a killed `heroes` by that window and no longer; Darwin's sentinel is started by `posix_spawn` with `POSIX_SPAWN_CLOEXEC_DEFAULT` (`:900`,
  read, not run), so it never holds one. A program's C that forks without exec is not on this path:
  the lock is the compiler's, held by `heroes` alone.

### A-2. File systems

- **Local, on this Mac**: FAT16, FAT32 and exFAT each answered as APFS (above). So on Darwin
  *"this file system has none"* did not arise on any local file system I could mount; the arm that
  answers it is compiled and unrun.
- **NFS and SMB: unrun** (no server inside the hard stops). Read today:
  - Darwin `mount_nfs(8)`: `nolocks` makes *"any attempt to perform file locking operations on this
    mount ... return the error ENOTSUP"*, which the door answers `HERO_OS_UNSUPPORTED`; and
    **`locallocks`** performs locking *"locally on the NFS client ... the NFS server and other NFS
    clients will have no knowledge of the locks"*, on by default for a soft read-only mount.
  - Linux `flock(2)` (man7.org): since 2.6.12 NFS emulates `flock` as whole-file `fcntl` locks;
    since 2.6.37 `local_lock=` makes them local (`nfs(5)`: *"such locks provide exclusion only
    against other applications running on the same client"*); over SMB, up to 5.4 *"flock() is not
    propagated"*, since 5.5 emulated as SMB byte-range locks, *"not advisory anymore"*.
  - Darwin's `flock(2)` and `mount_smbfs(8)` say nothing of network file systems (`grep -i lock` on
    the latter printed nothing).
  - **What route A does there**: where the door answers *none*, recovery must remove nothing (route
    D alone); where the mount's locks are local (`locallocks`, `local_lock=`, SMB before 5.5), a
    second machine's live run reads as dead and route A would remove its folder. That is the one
    shape where A is worse than D, and no call the door can make tells such a mount apart (a
    question: `statfs`'s `f_flags` was not searched for it).

### A-door. The door, written into a copy of the runtime and bound from Heroes

`F/tree-door/` is `F/tree/` cloned after the build (`cp -c -R`). In it, **`runtime/parts/lock.c`** (new, 139 lines,
`wc -l`, its comment included), two declarations, one type and one constant appended to
**`runtime/hero_compiler.h`**, and one `#include "parts/lock.c"` in `runtime/runtime.c` after
`parts/write.c`. Nothing in `hero_os.h`, `heroes_guard_open.h`, `heroes_guard_close.h` or
`selfhost/emit/guarded_names.hero` moved: **the door is inside this lane**.

```c
typedef struct hero_lock hero_lock;      /* a group's `record Lock tag hero_lock` */
#define HERO_LOCK_HELD 6                  /* the name is another's: held, or (fresh) standing */
hero_lock *hero_lock_take(const char *path, int64_t fresh, int64_t *status);
int64_t hero_lock_drop(hero_lock *lock);  /* removes the file while held, then releases */
```

- **POSIX arm**: `open(O_RDWR | O_CREAT | O_CLOEXEC | O_NOFOLLOW [| O_EXCL])`, `flock(LOCK_EX |
  LOCK_NB)`, both retried on `EINTR`; then **the identity check**, `fstat` of the descriptor against
  `stat` of the path (device and inode), `HERO_LOCK_HELD` where they differ (A-4 measures why it is
  owed). `EWOULDBLOCK` is held; `ENOTSUP`, `EOPNOTSUPP`, `ENOLCK`, `EINVAL` are
  `HERO_OS_UNSUPPORTED`; `ENOENT`/`ENOTDIR` `HERO_OS_NOT_FOUND`.
- **Windows arm**: no byte-range lock. `CreateFileW(path, GENERIC_READ | DELETE, share 0, NULL,
  fresh ? CREATE_NEW : OPEN_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE)`: the open IS the lock, the system
  deletes the file with the last handle, and NULL security attributes keep it out of a
  `CreateProcessW` child started with `bInheritHandles` TRUE, as `parts/run.c:1660` starts every
  child (read). Microsoft's pages, read today: `LockFileEx`, *"If a process terminates with a
  portion of a file locked ..., the locks are unlocked by the operating system. However, the time it
  takes ... depends upon available system resources"*; `CreateProcessW`, *"If this parameter is
  TRUE, each inheritable handle in the calling process is inherited"*; `CreateFileW`,
  `FILE_FLAG_DELETE_ON_CLOSE`, *"deleted immediately after all of its handles are closed"*;
  `RemoveDirectoryW`, *"This path must specify an empty directory"* and it *"marks a directory for
  deletion on close"*; `DeleteFileW` *"fails if an application attempts to delete a file that has
  other handles open ... (FILE_SHARE_DELETE must have been specified ...)"*. So **a folder holding
  an open lock file cannot be removed on Windows until that handle closes**: the lock file goes
  BESIDE the run's folder, never inside it, or `ended` must give the lock back before it removes
  the folder (A-4 measures what that window costs).
- **Rule 3 has nothing to judge**: the descriptor lives in the block `hero_lock_take` hands out
  (`hero_malloc_raw`, given back by `hero_lock_drop`), whose life the handle marks track; no
  `static`, no `_Thread_local`, no `_Atomic`, no `SHARED_BY_DECISION` line. (The `runtime` suite's
  run on `tree-door` is below.)

**Compiled.** `clang -I runtime <the 16 words of selfhost/cli/flags.hero's flags()> -c
runtime/runtime.c`: exit 0, 0 warnings; with `-Wextra` too, exit 0, no line naming `lock.c`; the
seed's own line `clang -I runtime seed/heroes.c runtime/runtime.c`: exit 0; `nm` lists `T
_hero_lock_take`, `T _hero_lock_drop`. **Windows**: `zig cc -target x86_64-windows-gnu` over the
whole `runtime.c` with the flags clang on Windows takes: exit 0, one warning, `replace.c:89`'s
`#pragma comment`, which the base's `runtime.c` gives too (exit 0, one warning); `nm` lists `T
hero_lock_take`, `U __imp_CreateFileW`. Caches in `F/zig-cache/`, `find ~/.cache/zig -newer` 0.
`F/a/win/winlock.c`, both Windows arms side by side (the `LockFileEx(LOCKFILE_EXCLUSIVE_LOCK |
LOCKFILE_FAIL_IMMEDIATELY)` one and the share-0 one, plus a child started with `bInheritHandles`
TRUE): `zig cc -std=c11 -Wall -Wextra`, exit 0, 0 warnings, `PE32+ executable (console) x86-64`.
**Not run**: `which wine wine64` finds neither; the box is off.

**Bound from Heroes** (`F/a/door/lockdoor.hero`, built by my `heroes-next` from `F/tree-door`):

```
extern "hero_compiler.h"
    record Lock tag hero_lock
    function hero_lock_take(path: cstr lent, fresh: i64, @status: i64) -> Lock acquires hero_lock_drop
    function hero_lock_drop(lock: Lock consumes) -> i64
    constant HERO_LOCK_HELD: i64
```

- `check` first said `error[needs_label]` (*"two of `hero_lock_take`'s parameters are `i64`"*,
  `fix (certain)`), applied; then `check` exit 0, `build` exit 0.
- Run: `hold` printed `taken`; a second process `try` printed **`held`**; `kill -9` of the holder
  (`wait` 137), the file still there (`-rw-------`); `try` printed **`taken`**, `dropped: true`, and
  the file was gone (`ls`: *No such file or directory*); a path in no folder printed `no folder`.
- **The marks hold the compiler to its own drop**: `forget` (take, then return from `main`) printed
  `panic: 1 C handle(s) never given back ... 1 taken at lockdoor.hero:32:12, in lockdoor.main`,
  exit 134.
- **A wrong binding is a compile error, the thesis at this boundary kept**: `@status: i32` against
  `int64_t *`, `error[ffi_parameter_type]` (*"clang read the header"*), exit 1; `hero_lock_drop ...
  -> ptr`, `error[ffi_return_type]: hero_lock_drop returns int64_t in hero_compiler.h, not ptr`,
  exit 1.

### A-4. What the lock decides that the number cannot: route A's protocol under load

`F/a/proto/proto.c` (`clang -std=c11 -O1 -Wall -Wextra -Werror`, exit 0): the door's POSIX arm
copied, with the identity check switchable. Eight owners alive at once, each taking a fresh name
(`O_EXCL`, the next serial where one stands), making its folder and a file, living 1 to 20 ms,
then checking its folder still stands (a **violation** where it does not), removing it and dropping
the lock; the driver SIGKILLs about one in three at a random moment; three recovering processes
sweep the directory without pause, taking every `<name>.lock` they can and removing that folder,
which is what route A's recovery does to a run it reads as dead. 8 s a run, `timeout 60`, no
process left (`pgrep -f` printed nothing).

| layout, check | runs | owners | killed | violations | births lost | left after a last sweep |
|---|---|---|---|---|---|---|
| lock **beside** the folder, identity check **on** | 4 | 3,279 / 3,342 / 3,326 / 3,244 | 359 to 385 | **0, 0, 0, 0** | 0 | 0 |
| beside, identity check **off** | 4 | 3,395 / 3,406 / 3,287 / 3,317 | 364 to 396 | **76, 65, 74, 74** | 0 | 0 |
| lock **inside** the folder, a folder with no lock file read as **dead** | 1 | 3,996 | 324 | 0 | **666** | 0 |
| inside, a folder with no lock file read as **alive** | 1 | 3,527 | 363 | 0 | **211** | **4** |

(`F/a/proto/stress.log` and the first runs above it.) What it measures:

1. **The identity check is owed**: without it, a run that opens its fresh lock file, loses it to a
   recovering run that found no folder and removed the file, then `flock`s the orphaned inode,
   believes itself the holder; its folder is then removed under it by the next sweep, 65 to 76
   times in 8 s. With it, 0 in 13,191 owners.
2. **The lock is taken before the folder exists, and dropped after it is gone**: route A as briefed,
   a lock *"on a file inside its own `.fetching-<pid>-<n>/`"*, has a window between `mkdir` and the
   lock. Reading a lock-less folder as dead takes LIVE runs' folders (666 births lost, each a
   `heroes add` or `check` that would fail or write into a removed folder); reading it as alive
   leaves killed runs' folders for ever (4 in 8 s). Beside the folder: 0 and 0.
3. **What the number cannot decide**, beside the coordinator's measurement (the dead number handed
   to a later run removed the only copy of an edit at exit 0): a process number answers *is some
   process of that number alive here*, never *is the run that made this folder alive*; it is wrong
   under reuse, and meaningless for a run in another PID namespace sharing the folder (a container,
   a second machine), whose number names nothing or another process on this side. **Unrun**: two
   containers on one bind mount, whether their `flock`s meet (the Linux leg asks one container only).
4. **Every `.fetching-*` name needs the lock, not only the run's ledger folder** (read, not run):
   `staging.opened` names a sibling `.fetching-<pid>-<serial>` for each package's own staging
   (`fetch/git_source.hero:48`, `fetch/index_source.hero:126`, `fetch/add_wanted.hero:87`) as well
   as for the run's ledger (`staging.hero:131`), and such a folder holds a half-written tree. Each
   `opened` taking its own fresh lock is one door call; a staging folder inside the ledger folder
   is the other shape. Unwritten here: it is the compiler-engineer's Heroes.
5. **Route D from the C side**: `O_EXCL` on the lock file and `mkdir` (not `mkdir -p`) on the folder
   are the two atomic refusals of a standing name; `heroes new`'s check (`cli/stub.hero:180`,
   `files.entry(staged) != .absent`, then a write) is a look-then-act, sound only because no two
   live processes share a number on one machine, which a shared mount breaks (a question, unrun).
   `process.mkdir_all` is the only directory maker `cli/process.hero` binds (`hero_fs_mkdir_all`,
   read): D needs an exclusive `mkdir` door, or the lock's `fresh`, which the draft has.

### A-6. Rule 3, run

`heroes-next run tests/harness/main.hero -- ./heroes-next runtime` in `F/tree-door` (the door in
place): **10 passed, 0 failed**, and *"objects that survive between calls: 109 against a floor of
83"*, told `FLOOR runtime/threads`, never a failure. The same in `F/tree` (the base): 10 passed, 0
failed, **109** against 83. So the door adds **no object** Rule 3 counts, and needs no
`SHARED_BY_DECISION` line, because it keeps none: the descriptor is the handle's, and the marks
track the handle. A door that kept the descriptor in the runtime instead (a table of held locks,
or one `static int`) would owe a `SHARED_BY_DECISION` line on `hero_spawn_slots`' reason (a
read-then-write over a table, a mutex beside it), and the drop could not be checked by the marks.
(The outgrown floor, 109 against 83, is the base's own; the suite says it is raised by a commit
that says so. `real 86.86` / `85.90` against `user 65.38` / `65.22`: waiting, not durations.)

## Part B: the server step (written as run)

### B2. The stop, measured on this Mac (`F/b/stop.c` against the base's `runtime/platform/hero_net.c`)

`clang -std=gnu11 -Wall -Wextra -Werror stop.c hero_net.c`, exit 0 (`hero_net.c` also compiles
alone, `-std=gnu11 -Wall`, exit 0). Driver `F/b/b2.sh`, every probe under `timeout -k 5 30`, its
output through `dd bs=1 count=65536` (`head -c` holds its input until it ends, so a driver reading
the first line read nothing: my first run, discarded). Log `F/b/b2-darwin.log`.

| route | SIGINT while blocked | what ended it |
|---|---|---|
| handler, `SA_RESTART`, loop asks between `hero_net_accept`s | still in `accept` 2 s later | **only a connection**: served one, then asked, then returned |
| handler, **no** `SA_RESTART` | the same | the same: `hero_net.c:509-512` asks `accept` again on `EINTR`, so `SA_RESTART` changes nothing for the layer |
| (a) blocked by `pthread_sigmask`, `sigpending` between steps | the same | the same, once its dispositions were set to default (below) |
| (b) self-pipe, `poll` on listener and pipe (raw sockets) | **woke at once** | the pipe |
| (b) kqueue `EVFILT_SIGNAL` beside the listener (Darwin) | **woke at once** | `kevent woke on signal 2` |
| handler, loop blocked in `read` on a pipe, `SA_RESTART` | still in `read` | the writer's end (`read: end of input`) |
| handler, loop in `read`, no `SA_RESTART` | **woke**: `read returned -1 errno 4` | asked and stopped |

- **A stop that asks between steps never reaches a server blocked in the layer's `accept`**, with
  or without `SA_RESTART`, a handler or a mask: measured, every such row waited for a connection.
  And it **outlived every SIGTERM sent to it**: my first, broken driver left one such probe (handler
  armed, `SA_RESTART`) blocked; `ps` read it at `ELAPSED 03:10` under `timeout 30`, its handler
  having caught `timeout`'s SIGTERM and `pkill`'s, and only `kill -9` ended it. So with S1, S2 or S3
  as drafted, **`docker stop`, `systemctl stop` and `timeout` all degrade to their SIGKILL** for a
  server waiting on a quiet socket, and task 1072's leak check (`c_main.hero:83`, `:102`) never runs.
- **`SA_RESTART` decides every OTHER blocking call** (the `read` rows): without it the program's own
  C calls and a library's see `EINTR`; with it they restart and never return to ask.
- **A job a non-interactive shell starts with `&` inherits SIGINT ignored** (`F/b/disp.c`:
  foreground *default*, `sh -c 'x &'` and zsh's background *IGNORED*, SIGTERM *default* in all
  three), and a signal both ignored and blocked is discarded, never pending: route (a) saw nothing
  until the probe set both dispositions to default (`F/b/pend2.c` hung on `sigwait` until killed).
  `run.c:518-525` keeps an inherited ignore as it is; S1 to S3 must say whether a stop does too.
- **Which thread runs a process-directed SIGINT's handler**, C handler, a `pthread_create`d thread
  alive: main not blocking, **20 of 20 on the main thread**; main blocking SIGINT, **20 of 20 on the
  C thread**. With a **Heroes** handler (`F/b/wt/whichthread.hero`, `signal` bound as the
  coordinator's `h.hero` binds it, the C thread from a header the program ships): a C thread alive
  and main blocking, `panic: whichthread.on_interrupt ran on a thread this program did not start`,
  exit 134, as panel 114's R4 says. **And with a `hero_thread_spawn` worker alive instead, main
  blocking: exit 134 in 3 runs of 5**, 0 of 5 with a 1 s pause after the spawn: a signal reaching a
  thread the program DID start, before `hero_spawn_enter` claims it (`spawn.c:226`), is refused as
  foreign. A candidate defect of the base, outside this sitting (a correct program refused); its
  shape: the spawning thread could block every signal around `pthread_create`, and the worker
  unblock after its claim. Unfiled (no GitHub write).
- **`hero_net_close` does NOT wake an `accept` blocked on a listener, on Darwin**, and the layer
  then **accepts a connection on the closed listener**: `close-then-connect` printed `main:
  hero_net_close(listener) returned 0`, the thread silent for 2 s, then `hero_net_connect MADE` and
  `thread: hero_net_accept returned a socket, kind 0 os 0`. `hero_net.h:93` promises *"A call in
  flight on it on another thread ends first"*. `hero_net_shut`'s `shutdown(fd, SHUT_RDWR)` on a
  listening socket does not wake it here, and the slot, marked closing and busy, keeps its
  descriptor open until the call returns. **A candidate defect of feature 2295's layer**, outside
  this sitting, and route (c) cannot rest on it on Darwin. Linux: in `linux-a.sh`, unrun by me.
  Windows: Microsoft's `closesocket` page speaks of overlapped operations and of Winsock 1.1's
  `WSACancelBlockingCall`, nothing of a listener's `shutdown` (grep of the page: *blocking*,
  *WSAEINTR*, *cancel*): a question, unrun.
- **Why two signals and not four**: `run.c:518-525` takes SIGTERM, SIGINT, SIGHUP and SIGQUIT only to
  **pass on** whatever ends `heroes` to the children it waits on. A stop is a request to finish:
  SIGINT is the terminal's and SIGTERM every service manager's (`docker stop`, `systemctl stop`,
  `timeout`); SIGQUIT asks for a core and SIGHUP is a hung terminal, which `nohup` sets ignored. Two,
  and Windows' CTRL_C, CTRL_BREAK and CTRL_CLOSE (read below). That is a reading, not a measurement.
- **The program's own header** (feature 1069's re-check), rebuilt myself in two modules, handler
  in `handler.hero`, loop in `main.hero`, `kill(getpid(), SIGINT)` at the third request, a cap of
  1000 (`F/b/hdr-*/`):

  | the flag in `stopflag.h` | build | run |
  |---|---|---|
  | `static volatile sig_atomic_t` | exit 0 | **`drained after 1000 requests`**: `nm` lists `_stop_requested` twice, one object per unit |
  | `__attribute__((weak))` definition | **exit 1, `error[ffi_defined_twice]`** | |
  | declared `extern` in `stopflag.h`, defined in `stopflag_def.h` that ONE module binds | exit 0 | **`drained after 3 requests`**, one `S _stop_requested` |
  | both modules bind the defining header | exit 1, `ffi_defined_twice` | |
  | neither does | **exit 2, `internal error: linking failed`** (`Undefined symbols ... "_stop_requested", referenced from _stop_asked ... _stop_request`) | |

  So **one flag per program from headers alone exists** (the split), and two of its three mistakes
  are loud on the author's line. The third is told as the compiler's own fault: an object a header
  declares and a `static inline` body reads, defined nowhere, is not among
  `.claude/rules/c-boundary.md`'s fifteen members (defect 2372's widening speaks of a body that
  *calls* a name). **A second candidate defect of the base** (who is blamed), unfiled. And on
  Windows no header can make this route safe: Microsoft's HandlerRoutine page, *"the system creates
  a new thread in the process to execute the function"*, and the C runtime's `signal` page, *"SIGINT
  is not supported for any Win32 application. When a CTRL+C interrupt occurs, Win32 operating
  systems generate a new thread"*: a Heroes handler there always meets `thread.c`'s guard (unrun,
  read). Whether Heroes can hold the rule that makes the split safe: only by the C split itself;
  no form of the language says *defined once*.
- **The runtime part each route needs, written and compiled** (`F/tree-rung/`, a clone of `F/tree/`):
  `runtime/parts/stop.c` (127 lines, `wc -l`), S2's semantics, route (b) inside route (c): the first
  `hero_stop_asked()` arms it; a C handler (`SA_RESTART`, an inherited ignore kept) sets a `volatile
  sig_atomic_t` and writes one byte down a close-on-exec pipe nobody drains; Windows,
  `SetConsoleCtrlHandler` with CTRL_C, CTRL_BREAK and CTRL_CLOSE, `InterlockedExchange` and a
  manual-reset event, the CTRL_CLOSE handler waiting up to 4,500 ms of the system's 5,000 for
  `main` to return. `hero_stop_descriptor()` hands the layer the pipe's read end or the event;
  `platform/hero_net.c`'s `accept` waits on it beside its socket (`poll` on POSIX,
  `WSAEventSelect` and `WaitForMultipleObjects` on Windows, the socket put back to blocking) and
  answers a new kind, `HERO_NET_STOPPED` (12). Compiled: Darwin `clang` with `flags()` and
  `-Wextra`, `runtime.c` and `hero_net.c`, exit 0, 0 warnings; Windows `zig cc -std=gnu11 -Wall
  -Wextra`, both exit 0, no warning naming the draft. Rule 3 would owe two `SHARED_BY_DECISION`
  lines (`hero_stop_flag` on `hero_runtime_spoke`'s reason, `hero_stop_wake` on `hero_argc`'s);
  `hero_stop_state` is `_Atomic`.
- **Run on this Mac, a Heroes server over it** (`F/b/srv/server.hero`, the layer's own
  `hero_net_accept`, one thread): blocked in `accept`, **SIGTERM ended it within 0.2 s**, printing
  `accept answered kind 12` and `stopped after 0 connections; main returns`; started with `&`, its
  SIGINT (inherited ignored) left it serving; started in the foreground of `sh`, SIGINT stopped it,
  exit 0. **And task 1072's instrument runs after the stop**: the same server leaving its listener
  open printed, after `main returns`, `panic: 1 C handle(s) never given back ... 1 taken at
  server.hero:21:9`, exit 134. One stop pipe, written once, woke **four** threads each blocked in
  `poll` on its own listener (`F/b/wakeall.c`: `thread 0..3: woke on the stop`, `main: all four
  joined`), so route (c) needs no choice of the thread a signal lands on.

### B1. The rung at the C boundary

- **A `str` body is ~40 lines of C on the `i64` rung, and needs no platform arm of its own**
  (`F/tree-rung/runtime/parts/spawn-str.c`, 41 lines): `hero_thread_spawn_str(int64_t (*)(HeroStr),
  HeroStr)` copies the argument's bytes on the spawning thread into a block of the runtime's
  (`hero_malloc_raw`), crosses the existing `hero_spawn_start` as the runtime's own number for its
  own block, makes the `str` on the worker, calls the body, and releases it there: design.md Part
  7.13's *"isolated per-thread heaps, copying at the boundaries"* (`docs/design.md:4089`).
  So the runtime's arm owes the same thing on all three platforms: the copy, the trampoline already
  exists. Compiled Darwin and Windows (`nm`: `T hero_thread_spawn_str`), exit 0. Bound from Heroes
  (`F/b/rung/strbody.hero`: `function hero_thread_spawn_str(body: (function(s: str) -> i64), arg:
  str) -> i64`): `check` exit 0, build exit 0; 16 threads, each body appending to its own copy,
  `main` rewriting its text while they ran: `16 threads summed 16288 (each 1018)`, **20 of 20 runs
  right and silent, and 20 of 20 under `--sanitize`** (ASan and UBSan, the leak gate silent). It
  was spelled in `hero_os.h`: that is a surface, a Part B question, and it owes the 241-name guard
  headers and `guarded_names.hero` a line each (not done in the draft).
- **T1's *"nothing else crosses"* holds of the program's Heroes values, not of the boundary.** A
  header the program ships (`F/b/carry/carry.h`, four `static inline` lines) turns a `cstr`, a
  `ptr` or a `hero_net_socket *` into an `int64_t` and back. A lease carried that way: joined
  before `end_lease`, `the thread read 118 bytes`, plain and `--sanitize`, exit 0; **`end_lease`
  before the join: built at exit 0, ran at exit 0 printing `the thread read 0 bytes`, and under
  `--sanitize` `ERROR: AddressSanitizer: heap-use-after-free`, exit 134.** The compiler said nothing
  at either build. The sentence that is true: *"Into a thread and back the language carries one
  `i64`; what a program's own C puts in it is that C's to keep alive."* (+ tokens unpriced: a
  `measure --refresh` is paid.)
- **`hero_os.h:430-434`'s comment** (*"WHAT MAY CROSS IS DECIDED BY THE CHECKER"*): not re-run by
  me; the coordinator and the critic each built a `str` body and a handle body against the base's
  `hero_thread_spawn` (`check` exit 0, `build` `ffi_callback_type`). The carry header above adds a
  third way past it: clang refuses nothing, since every type C sees is an `int64_t`.

### B3. One connection per thread, from the C side

- **The handle MOVES across the `i64` rung with its marks, today, through the program's own C**
  (`F/b/carry/socket.hero`): `hero_net_accept(...) -> Socket acquires hero_net_close |
  socket_as_number`, `socket_as_number(s: Socket consumes) -> i64`, `number_as_socket(n: i64) ->
  Socket acquires hero_net_close`. Four connections, four threads, each receiving its own 5 bytes:
  `received 20 bytes`, **10 of 10 runs, plain and `--sanitize`**, the handle check silent. First
  bound with `acquires hero_net_close` alone, it panicked `a C handle was given back to
  socket_as_number, and the mark that began its life names hero_net_close` at exit 134: spec § 13's
  *"giving the handle to a consumes call the mark did not name aborts before C runs"*, holding.
- **A numbered handle is what the layer already issues, and R3's promise holds through the number**
  (`hero_net.c:275-276`: `(life << 32) | (index + 1)`, never dereferenced). `F/b/carry/stale.hero`:
  the first accepted socket `slot 3 life 1`, closed; `c2`, connected after, took `slot 3 life 2`;
  the stale number put back in a `Socket` and used: `send ... -1, kind 2` (`HERO_NET_NOT_OPEN`),
  close `-1`, and the newer socket untouched (`send ... 3`), plain and `--sanitize`. So *"a numbered
  handle, which R3's never another socket that took its number exists to refuse"* is a false
  opposition at this base: the life field is R3's guarantee, and it survives the `i64`.
- **What is left open, then, is not C but the language**: the move needs a header the program
  ships, which panel 114's R2 ratified and the spec never wrote (`grep -n -i ships
  spec/heroes-spec.md`, nothing), and which panel 234's R6 forbids a **package**. A rung that moves
  a handle (`hero_thread_spawn_handle`, or a body type `(function(s: Socket) -> i64)` the checker
  admits with the handle's marks transferred) would put the four header lines into the runtime;
  unwritten here.
- **`cow.c`'s `refcount == 1` from the C side**, under the `i64` rung and the copying `str` rung:
  the forms spec § 13 gives C an address by are a `cstr` lend and a `ptr` lend (each for its call),
  an `@` place (a number's, or a group record's), and a lease (a COPY, never counted); a handle is
  C's own object. None hands C a counted Heroes block to keep past a call, so a second thread
  reaches a count only through C keeping a lend past its call, panel 113's R4 exclusion (a reading
  of § 13, searched for *lend*, *lease*, *place*; not a proof). A library calling a Heroes function
  from its own thread: refused by name, measured above (exit 134). The drop list is
  `_Thread_local` (`drop.c:59-61`), so its drain has no second thread. `sort` through a function
  pointer, and the `eq` and `hash` walks: on values the calling thread holds, unrun under contention
  here. **Unrun**: a `str` shared by count (R5's reading) rather than copied, beside the copy, under
  ThreadSanitizer (no `--sanitize=thread` in `flags.hero`).

### A1, from my seat (a reading of the records, not a measurement)

R12's *"a lock"* stands in a list that opens *"a second per-project file beside the catalogue"*
(`.claude/rules/cli-surface.md:76-78`), and R2 of the same sitting says *"no lock file, the
catalogue holding each entry's fingerprint"* (`docs/panel/235-*.md:549`): the word is Cargo's lock,
a file the build READS to choose what compiles. A run's lock file is read by no build, chooses
nothing (R12's own falsifier holds: deleting it never changes which module compiles), lives in a
folder no `use` can spell (R7), and lasts one run (on Windows the system deletes it; on POSIX a kill
leaves it until the next recovery takes and removes it). R7's *"no lock between two programs is
needed"* rests on *"two programs never share a tree"* (the compiler-engineer's report, its § 7):
still true; two RUNS on one program's tree is what defect 2453 measured, and R7 does not speak of
it. R19: a recovery that prints nothing adds no text, so this lane's repair moves none of the 47
`fetch` goldens; a line saying what recovery put back is a new message, the author's.

### Part A, in the definition's structure

- **verdict**: **approve** route A **in the shape measured here** (lock BESIDE each `.fetching-*`
  name, `flock` with the identity check, `O_EXCL` births = route D inside it, Windows' share-0
  delete-on-close open); **object** to route A as briefed, the lock inside the folder.
- **section**: design.md §1.11 and §4.19 (`grep -n '### 4.19\|### 1.11' docs/design.md`), with
  §1.12 (robustness) deciding that the class closes.
- **experiment**: `F/tree-door/runtime/parts/lock.c` + 2 declarations in `hero_compiler.h`:
  clang (`flags()` + `-Wextra`) exit 0, the seed's line exit 0, `zig cc` Windows exit 0; bound from
  Heroes with `acquires`/`consumes`, held/taken across a `kill -9`, a forgotten drop a named panic,
  two wrong bindings compile errors (`ffi_parameter_type`, `ffi_return_type`); `runtime` suite 10 and
  0, Rule 3 count 109 = the base's 109; `proto.c` 13,191 owners 0 violations with the check, 65 to
  76 per 8 s without; FAT16, FAT32, exFAT identical to APFS. Linux: `linux-a.sh`, written, **unrun
  by me**. Windows: compiled, **unrun**.
- **argument**: The number in `.fetching-<pid>-<n>` cannot say whether the run that made the folder
  lives; a lock the kernel drops at death can, and costs one compiler-private door with no static
  object, no ABI stamp and no guard-header entry: the marks hold the compiler to its drop, clang
  checks the binding. But the C decides more than *flock or not*: `F_SETLK` takes the same process's
  own folder; without the identity check live runs lose their folders (65-76 per 8 s); a lock inside
  the folder loses births or leaves corpses, and cannot be removed on Windows. Where a mount's locks
  are local (`locallocks`, `local_lock=`), A can still remove a remote live run's folder; D cannot.
- **prediction**: `linux-a.sh`'s A-4 in `heroes-linux-arm64:latest` prints **0 VIOLATIONS** on both
  `identity=1` beside runs and **more than 0** on both `identity=0` runs, and its A-1 table equals
  `F/a/a1-apfs.log`'s once the pids are taken out, on overlay and on `/dev/shm` alike.
- **condition**: I move to **object** if Linux shows a violation with the identity check on, or an
  `flock` that survives `kill -9`; to **approve D alone** if the author holds the run's lock file to
  be R12's fifth class, or rules that a program folder on a mount with local-only locks must never
  lose a live run's folder (A cannot promise that; D can, at B's cost of leftovers).

### Part B, in the definition's structure

- **verdict**: **object** (no veto: no proposal breaks the C ABI). To T1/T2 as written, and to S1,
  S2, S3 as drafted without the layer's own wait on the stop.
- **section**: design.md §1.11 and §4.19; §1.12 for the stop (a server SIGTERM cannot end, whose
  leak check never runs); spec § 13 line 377 is where every candidate lands.
- **experiment**: `F/b/stop.c` against the base's `hero_net.c` (clang exit 0): every between-steps
  stop waited for a connection, with or without `SA_RESTART`, handler or mask; one probe outlived
  `timeout 30` by 2:40. `F/tree-rung/runtime/parts/stop.c` + `hero_net.c`'s `accept` waiting on the
  stop: Darwin and Windows compiles exit 0; a Heroes server ended 0.2 s after SIGTERM, its handle
  check then run. `spawn-str.c`, 41 lines: 20/20 plain, 20/20 `--sanitize`. A 4-line program header:
  a lease's use-after-free built at exit 0; a socket moved with its marks, 10/10 and 10/10.
- **argument**: T1's *"nothing else crosses"* is true of Heroes values and false at the boundary: a
  four-line header the program ships carried a lease into a thread and read it after `end_lease`,
  built at exit 0. The `str` rung R5 ratified costs 41 lines and no platform arm, and the rung
  written now is promised at 1.0.0; writing `i64`-only narrows R5 into the spec. Every S drafted
  meets `hero_net.c:509-512`'s retry: a quiet server never stops on SIGTERM. The stop must be a
  runtime part the layer waits on (measured working); a program's header is a trap (two of five
  shapes) and impossible on Windows.
- **prediction**: `linux-a.sh` in `heroes-linux-arm64:latest` prints, for `accept-handler-restart`,
  `accept-handler-norestart` and `accept-mask`, *"still running 2 s after SIGINT"*, and for
  `server` *"ended within"* at most 3 tenths of a second of SIGTERM; and its `close-then-connect`
  prints `hero_net_connect refused` (Linux's `shutdown` on a listener wakes `accept`), unlike
  Darwin's `MADE`.
- **condition**: I move to **approve** for a T sentence that says what is true (*"Into a thread and
  back the language carries one `i64`, or one `str` copied for the thread; what a program's own C
  puts in an `i64` is that C's to keep alive"*, unpriced), and for any of S1 to S3 whose runtime
  part the platform layer's blocking calls wait on, an inherited ignore kept. For B3 I approve
  **a rung that moves a handle** (the four header lines as two `hero_net.h` doors with marks, so a
  package can use it too) or **one thread**; a numbered handle needs nothing, the layer has one.

## Candidate defects of the base, found here, unfiled (no GitHub write)

1. **`hero_net_close` does not wake an `accept` blocked on a listener (Darwin), and the layer then
   accepts a connection made after the close** (`F/b/stop close-then-connect`): `hero_net.h:93`
   promises the call in flight ends first. Feature 2295's layer, b33 round. Class by the rule: a
   promise of the layer broken, `blocking` if read as a wrong value.
2. **A signal reaching a `hero_thread_spawn` thread before it claims itself is refused as foreign**
   (`F/b/wt/whichthread2`, 3 of 5 runs exit 134 with no pause, 0 of 5 with one): a correct program
   refused.
3. **An object a program's header declares and an inline body reads, defined nowhere, is
   `internal error: linking failed`, exit 2** (`F/b/hdr-neither`): the author's mistake blamed on
   the compiler.

## What is unrun, said

The Linux leg (`F/linux-a.sh`, for the coordinator: `docker run --rm -v "$PWD/<F>":/src:ro
heroes-linux-arm64:latest bash /src/linux-a.sh`, its log through `dd bs=1 count=4194304`); every
Windows run (both arms compiled); NFS and SMB; two PID namespaces on one bind mount; ThreadSanitizer
over a `str` shared by count; the tokens of any sentence I proposed (a `measure --refresh` is paid,
and I priced nothing). I did not re-run the coordinator's `fetch` kill and PID-reuse cases (the
compiler-engineer's step), nor build route A into `selfhost/`.
