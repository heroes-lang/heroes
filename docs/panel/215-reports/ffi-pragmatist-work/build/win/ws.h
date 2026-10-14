/* Panel 215, the ffi-pragmatist's build of route C: Windows' locks held by
   value in a struct of the program's own, the shape a binding meets. */
#include <windows.h>
#include <threads.h>
#include <stdint.h>
struct csbox { CRITICAL_SECTION cs; int64_t n; };
struct srwbox { SRWLOCK l; int64_t n; };
struct mbox { mtx_t m; int64_t n; };
static inline int cs_init(struct csbox *b) { InitializeCriticalSection(&b->cs); return 0; }
static inline int cs_enter(struct csbox *b) { EnterCriticalSection(&b->cs); return 0; }
static inline int cs_try(struct csbox *b) { return TryEnterCriticalSection(&b->cs) ? 1 : 0; }
static inline int cs_leave(struct csbox *b) { LeaveCriticalSection(&b->cs); return 0; }
static inline long cs_count(struct csbox *b) { return b->cs.LockCount; }
static inline int srw_init(struct srwbox *b) { InitializeSRWLock(&b->l); return 0; }
static inline int srw_try(struct srwbox *b) { return TryAcquireSRWLockExclusive(&b->l) ? 1 : 0; }
static inline int srw_release(struct srwbox *b) { ReleaseSRWLockExclusive(&b->l); return 0; }
static inline int m_init(struct mbox *b) { return mtx_init(&b->m, mtx_plain); }
static inline int m_lock(struct mbox *b) { return mtx_lock(&b->m); }
static inline int m_try(struct mbox *b) { return mtx_trylock(&b->m); }
